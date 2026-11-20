/**
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  *                  Calls target system initialization then loop in main.
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_logger.h"
#include "stdio.h"
#include "app.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

//int __io_putchar(int ch)
//{
//	hal_uart_handle_t *p_usart2 = mx_usart2_uart_gethandle();
//    if (p_usart2 != NULL)
//    {
//        HAL_UART_Transmit(p_usart2, (uint8_t *)&ch, 1, 10);
//    }
//    return ch;
//}

/**
  * brief:  The application entry point.
  * retval: none but we specify int to comply with C99 standard
  */
int main(void)
{
  /** System Init: this code placed in targets folder initializes your system.
    * It calls the initialization (and sets the initial configuration) of the peripherals.
    * You can use STM32CubeMX to generate and call this code or not in this project.
    * It also contains the HAL initialization and the initial clock configuration.
    */
  if (mx_system_init() != SYSTEM_OK)
  {
    return (-1);
  }
  else
  {
    /*
      * You can start your application code here
      *
      */

	  /* Uart test */
//	  hal_uart_handle_t *p_usart2 = mx_usart2_uart_gethandle();
//	  uint8_t test_msg[] = "UART test geslaagd!\r\n";
//	  HAL_UART_Transmit(p_usart2, test_msg, sizeof(test_msg) - 1, 100);

	  app_init();

	  //printf("[INFO] Starting FreeRTOS scheduler\n");

	  vTaskStartScheduler();

//    while (1) {}
  }
} /* end main */

