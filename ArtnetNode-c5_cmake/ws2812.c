#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "app_logger.h"
#include "app_config.h"

#include "ws2812.h"

#define WS2812_BIT_0	0x2D
#define WS2812_BIT_1	0x78

// globals
static volatile BaseType_t RunClient = pdTRUE;
extern EventGroupHandle_t AppLwipEvents;

hal_tim_handle_t *ws2812_tim = NULL;

static uint8_t dmx_pool[POOL_SIZE][DMX_BLOCK_SIZE];

QueueHandle_t xFullQueue = NULL;
QueueHandle_t xEmptyQueue = NULL;
TaskHandle_t xWs2812TaskHandle = NULL;

#define WS2812_DMA_SIZE ((60*24)+1)
uint8_t ws2812_dma_buf[WS2812_DMA_SIZE];

//
void HAL_TIM_CompareMatchCallback(hal_tim_handle_t *htim, hal_tim_channel_t channel)
{
	if ((htim->instance == HAL_TIM8) && (channel == HAL_TIM_CHANNEL_1))
	{

		HAL_TIM_OC_StopChannel_DMA(ws2812_tim, HAL_TIM_CHANNEL_1);

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (xWs2812TaskHandle != NULL) {
            vTaskNotifyGiveFromISR(xWs2812TaskHandle, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
	}
}

//

static void ws2812Thread(void *args) {

	ws2812_dma_buf[WS2812_DMA_SIZE-1] = 0;

	ws2812_tim = mx_tim8_gethandle();
	HAL_TIM_Start(ws2812_tim);

	uint32_t frame_count = 0;
	uint8_t *pDmxData = NULL;

	uint32_t buf_idx = 0;

	while (RunClient == pdTRUE)
	{
        // Wacht max 100ms op een nieuw 512-byte blok van Art-Net
        if (xQueueReceive(xFullQueue, &pDmxData, pdMS_TO_TICKS(100)) == pdPASS)
        {

            APP_LOG_INFO(APP_NAME, "Frame %lu | Buf: %p | RGB: (%d:%d:%d)",
                          frame_count, pDmxData, pDmxData[0], pDmxData[1], pDmxData[2]);

        	buf_idx = 0;
        	for(uint16_t idx = 0; idx < 60; idx++) {
        		uint8_t r = pDmxData[idx * 3 + 0];
				uint8_t g = pDmxData[idx * 3 + 1];
				uint8_t b = pDmxData[idx * 3 + 2];
				uint32_t grb = ((uint32_t)g << 16) | ((uint32_t)r << 8) | b;

				for(int8_t bit = 23; bit >=0; bit--) {
					ws2812_dma_buf[buf_idx++] = ((grb>>bit) & 1U) ? WS2812_BIT_1 : WS2812_BIT_0;
				}
			}

            // Buffer is omgezet, geef direct terug aan lwIP/Art-Net
            xQueueSend(xEmptyQueue, &pDmxData, 0);

            // Start de PWM DMA transfer
            HAL_TIM_OC_StartChannel_DMA(ws2812_tim, HAL_TIM_CHANNEL_1, ws2812_dma_buf, WS2812_DMA_SIZE);

            // Wacht tot de DMA helemaal klaar is
            ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(50));
        }
    }

	/* Delete the task */
	APP_LOG_INFO(APP_NAME, "WS2811 stopped, delete WS2812 Task\n");
	vTaskDelete(NULL);
}




void ws2812_queue_init(void)
{
    xFullQueue  = xQueueCreate(POOL_SIZE, sizeof(uint8_t*));
    xEmptyQueue = xQueueCreate(POOL_SIZE, sizeof(uint8_t*));

    for (int i = 0; i < POOL_SIZE; i++) {
        uint8_t *pBuf = dmx_pool[i];
        xQueueSend(xEmptyQueue, &pBuf, 0);
    }
}

//
void ws2812_init() {

	BaseType_t xReturn;

	ws2812_queue_init();

	xReturn = xTaskCreate(ws2812Thread, APP_NAME, configMINIMAL_STACK_SIZE,
						(void *) NULL, WS2812_THREAD_PRIO, &xWs2812TaskHandle);

	if (xReturn != pdPASS)
	{
		APP_LOG_ERROR(APP_NAME, "WS2811 Task Creation failed (%ld)\n", xReturn);
	}
}

//void ws2812_send_frame(const uint8_t *rgb_data) {
//
//	uint32_t buf_idx = 0;
//
//	while(ws2812_complete_flag == 0) {
//	  __NOP();
//	}
//	ws2812_complete_flag = 0;
//
//	for(uint16_t idx = 0; idx < 8; idx++) {
//		uint8_t r = rgb_data[idx * 3 + 0];
//		uint8_t g = rgb_data[idx * 3 + 1];
//		uint8_t b = rgb_data[idx * 3 + 2];
//		uint32_t grb = ((uint32_t)g << 16) | ((uint32_t)r << 8) | b;
//
//		for(int8_t bit = 23; bit >=0; bit--) {
//			ws2812_dma_buf[buf_idx++] = ((grb>>bit) & 1U) ? WS2812_BIT_1 : WS2812_BIT_0;
//		}
//	}
//
//	HAL_TIM_OC_StartChannel_DMA(ws2812_tim, HAL_TIM_CHANNEL_1, ws2812_dma_buf, WS2812_DMA_SIZE);
//}
