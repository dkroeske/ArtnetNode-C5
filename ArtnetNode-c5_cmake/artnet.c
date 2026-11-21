#include "artnet.h"
#include "ws2812.h"
#include "app_config.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "app_lwip.h"
#include "app_logger.h"
#include "mx_lwip.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/sys.h"
#include "lwip/netif.h"
#include "mx_system.h"

// Defines
#define ARTNET_UNIVERSE	(uint16_t) 8
//#define NR_LEDS	(uint16_t) 10

// Local prototypes
static void ArtNetClientThread(void *args);
void artnet_handle_poll_reply(int sock, struct sockaddr_in *remote_addr);
void artnet_handle_dmx(uint8_t *payload, uint16_t len);

// Globals
static volatile BaseType_t RunClient = pdTRUE;
static TaskHandle_t ArtNetClientTaskHandle;
extern EventGroupHandle_t AppLwipEvents;

// Up to ARTNET_MAX_UNIVERSES * 512 bytes hold a single channel
ArtNetDMX_t artdmx[ARTNET_MAX_UNIVERSES];
uint8_t artdmx_index = 0;
uint8_t p_sequence = 0;
static uint8_t nrArtPollResponses = 0;


/*
 * ArtPoll received, reply with ArtPollReply
 */
void artnet_handle_poll_reply(int sock, struct sockaddr_in *remote_addr) {

	// ... wait random 100ms < t < 500ms according protocol
	uint32_t random_delay = 100 + (rand() % 401);
	vTaskDelay(pdMS_TO_TICKS(random_delay));

	ArtPollReply_t reply;
	memset(&reply, 0, sizeof(ArtPollReply_t));

	// Construct ArtPollReply
	strcpy(reply.ID,"Art-Net");
	reply.OpCode = ARTNET_OPCODE_POLLREPLY; // 0x2100 Little Endian
	reply.Port = ARTNET_DISCOVERY_PORT; 	// 0x1936 = 6454d Little Endian

	// Copy own IP
	uint32_t ip = ip4_addr_get_u32(netif_ip4_addr(netif_list));
	memcpy(reply.IPAddr, &ip, 4);
	memcpy(reply.BindIp, &ip, 4);
	reply.BindIndex = 1;

	// MAC address
	memcpy(reply.MAC, netif_list->hwaddr, 6);

	// Firmware & OEM info
	reply.VersInfoH = 0x01;
	reply.VersInfoL = 0x00;
	reply.OemHi = 0x00;
	reply.OemLo = 0xFF;
	reply.EstaManLo = 0xF0;
	reply.EstaManHi = 0x7F;
	reply.Status1 = 0xD0;
	reply.Status2 = 0x08;

	// Name & status
	snprintf(reply.ShortName, sizeof(reply.ShortName), ARTNET_NODE_NAME);
	snprintf(reply.LongName, sizeof(reply.LongName), ARTNET_SHORT_NAME);
	snprintf(reply.NodeReport, sizeof(reply.NodeReport), "#0001 %.4d 0001 by dkroeske@gmail.com", nrArtPollResponses++);

	reply.NumPortsHi = 0x00; 		// Always zero
	reply.NumPortsLo = 0x01; 		// 1 active port
	reply.PortTypes[0] = 0x80; 		// 1 input, DMX512
	reply.GoodOutputA[0] = 0x80; 	// 1 input, DMX512

	reply.SwOut[0] = 8; 			// Xlights presents universe
	reply.AcnPriority = 100;

	// Reply
    struct sockaddr_in dest_addr = *remote_addr;
    dest_addr.sin_port = htons(ARTNET_DISCOVERY_PORT);
    sendto(sock, &reply, sizeof(ArtPollReply_t), 0,
           (struct sockaddr *)&dest_addr, sizeof(dest_addr));
}

void artnet_handle_dmx(uint8_t *payload, uint16_t len)
{
	if(len<18) return;

	// Get and convert payload data to ArtDmx
	ArtNetDMX_t dmx = {};
	memset( (uint8_t *)&dmx, 0, sizeof(ArtNetDMX_t));

	uint16_t clipped_len = (len > sizeof(ArtNetDMX_t)) ? sizeof(ArtNetDMX_t) : len;
	memcpy( (uint8_t *)&dmx, payload, clipped_len);

	uint16_t dmx_channels = (dmx.LengthHi << 8) | dmx.Length;

	// Get and check if ArtDmx in within universe
	int universe = ARTNET_UNIVERSE;
	if( (dmx.SubUni >= universe) && (dmx.SubUni < (universe + ARTNET_MAX_UNIVERSES)) ) {

		uint16_t dmx_len = (payload[16] << 8) | payload[17];
	    if (dmx_len > DMX_BLOCK_SIZE) dmx_len = DMX_BLOCK_SIZE;
	    if (len < (18 + dmx_len)) return;

	    uint8_t *pBuf = NULL;

	    // probeer een lege buffer te pakken
	    if (xQueueReceive(xEmptyQueue, &pBuf, 0) != pdPASS) {
	        // Geen lege buffer? Pak dan het nog niet verzonden frame uit xFullQueue (altijd nieuwste frame)
	        xQueueReceive(xFullQueue, &pBuf, 0);
	    }

	    if (pBuf != NULL) {
	        // Kopieer de 512 DMX bytes direct uit de payload
	        memset(pBuf, 0, DMX_BLOCK_SIZE);
	        memcpy(pBuf, &payload[18], dmx_len);

	        // Stuur pointer naar ws2812Thread (dit wekt de task direct in het ritme van de zender)
	        xQueueSend(xFullQueue, &pBuf, 0);
	    }
	}

//	// If so play leds every time sequence changes. Save multiple 512 DMX-
//	// frames
//	if( p_sequence != dmx.Sequence ) {
//		// For all leds map led-number to 512 dmx buffers
//		unsigned int nr_leds = NR_LEDS;
//		for(uint16_t idx = 0; idx < nr_leds*3; idx+=3) {
//			uint8_t r,g,b;
//			r = artdmx[idx/512].Data[idx%512];
//			g = artdmx[(idx+1)/512].Data[(idx+1)%512];
//			b = artdmx[(idx+2)/512].Data[(idx+2)%512];
//			//pixel(r,g,b);
//		}
////					for(uint8_t idx = 0; idx < artdmx_index+1; idx++) {
////						printf("SubUni %.2d: Sequence: %.2d\n", artdmx[idx].SubUni, artdmx[idx].Sequence);
////					}
//			artdmx_index = 0;
//			artdmx[artdmx_index] = dmx;
//		} else {
//			if(artdmx_index < ARTNET_MAX_UNIVERSES) {
//				artdmx_index++ ;
//				artdmx[artdmx_index] = dmx;
//			}
//		}
//		p_sequence = dmx.Sequence;
//	}


	// Update time
//	art_status.dirty = true;
//	art_status.last_update = HAL_GetTick();
}


static void ArtNetClientThread(void *args)
{
	APP_LOG_INFO(APP_NAME, "Initializing ArtNet client ...\n");
	xEventGroupWaitBits(AppLwipEvents,
					  EVT_IP_ADDR_SET,
					  pdTRUE,
					  pdFALSE,
					  portMAX_DELAY);

    // Create UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        APP_LOG_ERROR(APP_NAME, "Socket creation failed\r\n");
        vTaskDelete(NULL);
        return;
    }

    int broadcast = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    // Timeout 500ms
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000; // 500 ms timeout
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    // Bind socket to ArtNet discovery port (6454)
    struct sockaddr_in local_addr = {0};
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(ARTNET_DISCOVERY_PORT);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        APP_LOG_ERROR(APP_NAME, "Socket bind failed\r\n");
        close(sock);
        vTaskDelete(NULL);
        return;
    }

    // All well, inform
    APP_LOG_INFO(APP_NAME, "ArtNet client initialized\n");

    uint8_t rx_buffer[530];
    struct sockaddr_in remote_addr;
    socklen_t addr_len = sizeof(remote_addr);

//    while (RunClient == pdTRUE)
//    {
//        addr_len = sizeof(remote_addr); // Belangrijk: reset elke iteratie!
//        int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer), 0,
//                           (struct sockaddr *)&remote_addr, &addr_len);
//
//        // Controleer minimale lengte en "Art-Net\0" header
//        if (len >= 10 && memcmp(rx_buffer, "Art-Net\0", 8) == 0) {
//            uint16_t opcode = rx_buffer | (rx_buffer << 8); // Little-Endian
//
//            if (opcode == ARTNET_OPCODE_POLL) {
//                APP_LOG_INFO(APP_NAME, "ArtPoll ontvangen van xLights, stuur reply...");
//                artnet_send_poll_reply(sock, &remote_addr);
//            }
//            else if (opcode == ARTNET_OPCODE_DMX) {
//                // Hier komt je lichtdata binnen (kanaaldata start op rx_buffer)
//                artnet_handle_recv(rx_buffer);
//            }
//        }
//    }



	while (RunClient == pdTRUE)
	{
        int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer), 0,
                           (struct sockaddr *)&remote_addr, &addr_len);

        // Check for valid header (Art-Net) and switch on OpCode
        if (len >= 10 && memcmp(rx_buffer, "Art-Net\0", 8) == 0) {
        	uint16_t OpCode = ((rx_buffer[9]<<8)+rx_buffer[8]);

        	switch(OpCode) {

        		case ARTNET_OPCODE_POLL:
                	APP_LOG_INFO(APP_NAME, "ArtNet POLL received");
                	artnet_handle_poll_reply(sock, &remote_addr);

					break;

        		case ARTNET_OPCODE_DMX:
		        	//APP_LOG_INFO(APP_NAME, "ArtNet DMX received");
		        	artnet_handle_dmx(rx_buffer, len);
		        	break;
        	}
        } else {
        	APP_LOG_ERROR(APP_NAME, "Invalid ArtNet header received");
        }
	}

	close(sock);

	/* Inspect task stack high water mark on exit. */
	//APP_LOG_INFO(APP_NAME, "Task High Water Mark: (%ld)\n", uxTaskGetStackHighWaterMark(NULL));

	/* Delete the task */
	APP_LOG_INFO(APP_NAME, "ArtNet client stopped, delete ArtNet Task\n");
	vTaskDelete(NULL);
}

/*
 * Init UDP interface
 */
int artnet_init( void(*cb)(void *) )
{

	/**
	* Creates and starts the ArtNet client thread
	*/
	BaseType_t xReturn;
	xReturn = xTaskCreate(ArtNetClientThread, APP_NAME, configMINIMAL_STACK_SIZE,
						(void *) NULL, ARTNET_CLIENT_THREAD_PRIO, &ArtNetClientTaskHandle);
	if (xReturn != pdPASS)
	{
		APP_LOG_ERROR(APP_NAME, "ArtNet Task Creation failed (%ld)\n", xReturn);
	}
//	// Save callback
//	onDataAvailable = cb;
//
//	// Setup new UDP connection
//	pcb = udp_new();
//
//	// Bind to port Artnet discovery port
//  	err_t err = udp_bind(pcb, IP_ADDR_ANY, ARTNET_DISCOVERY_PORT);
//
//	// Start receiver and set callback if all is ok
//	if(ERR_OK == err) {
//		udp_recv(pcb, artnet_recv, NULL);
//	}
//
//	if(ERR_OK == err) {
//		return OK;
//	} else {
//		return NOK;
//	}
	return 0;
}

int artnet_deinit(void) {
	return 0;
}
