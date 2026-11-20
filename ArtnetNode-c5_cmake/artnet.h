#ifndef ARTNET_INC
#define ARTNET_INC

#include <stdlib.h>
#include <stdbool.h>
#include "app.h"

#define ARTNET_DISCOVERY_PORT 6454
#define ARTNET_MAX_UNIVERSES	8

#define ARTNET_OPCODE_POLL		0x2000
#define ARTNET_OPCODE_POLLREPLY	0x2100
#define ARTNET_OPCODE_DMX		0x5000

#define ARTNET_NODE_NAME		"ArtnetNode-C5"
#define ARTNET_SHORT_NAME		"ArtnetNode-C5 by dkroeske@gmail.com"

int artnet_init( void(*cb)(void *) );
int artnet_deinit();

struct ArtNetStatus {
	bool dirty;
	uint64_t	last_update;
};

struct ArtNetStatus artnet_status(void);


//struct ArtPoll {
//	char ID[8];
//	uint16_t OpCode;
//	uint8_t ProtVerHi;
//	uint8_t ProtVerLo;
//	uint8_t Flags;
//};

typedef struct __attribute__((packed)) {
	char ID[8]; 
	uint16_t OpCode;
	uint8_t IPAddr[4];
	uint16_t Port;
	uint8_t VersInfoH;
	uint8_t VersInfoL;
	uint8_t NetSwitch;
	uint8_t SubSwitch; 
	uint8_t OemHi;
	uint8_t OemLo;
	uint8_t UbeaVersion;
	uint8_t Status1;
	uint8_t EstaManLo;
	uint8_t EstaManHi;
	char ShortName[18];
	char LongName[64];
	char NodeReport[64];
	uint8_t NumPortsHi;
	uint8_t NumPortsLo;
	uint8_t PortTypes[4];
	uint8_t GoodInput[4];
	uint8_t GoodOutputA[4];
	uint8_t SwIn[4];
	uint8_t SwOut[4];
	uint8_t AcnPriority;
	uint8_t SwMacro;
	uint8_t SwRemote;
	uint8_t Spare[3];
	uint8_t Style;
	uint8_t MAC[6];
	uint8_t BindIp[4];
	uint8_t BindIndex;
	uint8_t Status2;
	uint8_t GoodOutputB[4];
	uint8_t Status3;
	uint8_t DefaultRespUID[6];
	uint8_t Filler[15];
} ArtPollReply_t;


//typedef struct __attribute__((packed)) {
//    uint8_t  id;           // "Art-Net\0"
//    uint16_t opCode;          // 0x2100 (Little-Endian op STM32)
//    uint8_t  ipAddr;       // Eigen IP-adres van de STM32 (bijv. 192, 168, 1, 50)
//    uint16_t port;            // 0x1936 (6454 in Little-Endian)
//    uint16_t versInfo;        // Firmware versie (Big-Endian, bijv. 0x0100)
//    uint8_t  netSwitch;       // Bits 14-8 van 15-bit Port-Address (0x00)
//    uint8_t  subSwitch;       // Bits 7-4 van 15-bit Port-Address (0x00)
//    uint16_t oem;             // OEM code (Big-Endian, bijv. 0x00FF voor onbekend/proto)
//    uint8_t  ubeaVersion;     // 0
//    uint8_t  status1;         // Status flags (bijv. 0xD0: normal indicator, Port-Address via frontpanel/netwerk)
//    uint16_t estaMan;         // ESTA fabrikantcode (Little-Endian, 0x0000)
//    char     shortName;   // Korte naam voor in xLights (max 17 tekens + '\0')
//    char     longName;    // Lange omschrijving (max 63 tekens + '\0')
//    char     nodeReport;  // Status tekst, bijv. "#0001 [0000] OK"
//    uint16_t numPorts;        // Aantal poorten (Big-Endian: 1 poort = htons(1))
//    uint8_t  portTypes;    // Type per poort: 0x80 = Output (van Art-Net naar DMX/LED)
//    uint8_t  goodInput;    // Input status (0x00)
//    uint8_t  goodOutput;   // Output status (0x80 = data wordt verzonden)
//    uint8_t  swIn;         // Input universe (0-15)
//    uint8_t  swOut;        // Output universe (0-15, bijv. {0, 0, 0, 0} voor Universe 0)
//    uint8_t  swVideo;         // 0
//    uint8_t  swMacro;         // 0
//    uint8_t  swRemote;        // 0
//    uint8_t  spare;        // 0
//    uint8_t  style;           // 0x00 = StNode (DMX/LED node)
//    uint8_t  mac;          // MAC-adres van de STM32
//    uint8_t  bindIp;       // Zelfde als ipAddr
//    uint8_t  bindIndex;       // 1
//    uint8_t  status2;         // 0x08 = Node ondersteunt Art-Net 3/4 (15-bit port address)
//    uint8_t  filler;      // Opvulling tot exact 239 bytes
//} ArtPollReply_t;

typedef struct __attribute__((packed)) {
	char ID[8];
	uint16_t OpCode;
	uint8_t ProtVerHi;
	uint8_t ProtVerLo;
	uint8_t Sequence;
	uint8_t Physical;
	uint8_t SubUni;
	uint8_t Net;
	uint8_t LengthHi;
	uint8_t Length;
	uint8_t Data[512];
} ArtNetDMX_t;

#endif
