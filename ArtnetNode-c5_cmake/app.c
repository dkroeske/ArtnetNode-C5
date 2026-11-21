/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app.h"

#include "app_logger.h"
#include "app_lwip.h"
#include "artnet.h"
#include "ws2812.h"
//#include "app_sntp_client.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  /**
    * Initializes the logger (FreeRTOS queue)
    */
  if( app_logger_init() != EXEC_STATUS_INIT_OK ) {
	  APP_LOG_ERROR(APP_NAME, "Logger NOT initialized\n");
  }
  APP_LOG_INFO(APP_NAME, "Logger initialized.");

  /**
    * Initializes the LwIP stack
    */
  app_lwip_init();

  /**
    * Starts the ArtNet client thread
    */
  artnet_init( NULL );
  APP_LOG_INFO(APP_NAME, "ArtNet initialized.");

  /**
    * Starts the ArtNet client thread
    */
  ws2812_init();
  APP_LOG_INFO(APP_NAME, "ws2812 initialized.");

  return return_status;
}


app_status_t app_deinit(void)
{
  /** This API is not used in this example (infinite loop).
    * It is optimized out by the toolchain.
    */
  app_lwip_deinit();
  return EXEC_STATUS_ERROR;
}
