#ifndef WS2812_INC
#define WS2812_INC

#include "mx_hal_def.h"
#include "mx_system.h"
#include "FreeRTOS.h"
#include "queue.h"

#include <stdbool.h>

#define IS_RGBW false

void ws2812_send_frame(const uint8_t *rgb_data);
void ws2812_init(void);

#define DMX_BLOCK_SIZE  512
#define NR_LEDS			170
#define POOL_SIZE		2

extern QueueHandle_t xFullQueue;
extern QueueHandle_t xEmptyQueue;

#endif
