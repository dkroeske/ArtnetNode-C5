/**
  ******************************************************************************
  * @file    lan8742_io.h
  * @brief   LAN8742 header IO driver file for ETH
  ******************************************************************************
  * @attention
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LAN8742_IO_H
#define LAN8742_IO_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/**
  * @brief Macros definitions
  */
#if defined (USE_HAL_EXTI_REGISTER_CALLBACKS) && (USE_HAL_EXTI_REGISTER_CALLBACKS == 1)
#if defined (USE_HAL_EXTI_USER_DATA) && (USE_HAL_EXTI_USER_DATA == 1)
#define LAN8742_CALLBACKS 1
#endif /* USE_HAL_EXTI_USER_DATA */
#endif /* USE_HAL_EXTI_REGISTER_CALLBACKS */

#if defined (USE_HAL_MUTEX) && (USE_HAL_MUTEX == 1)
#define LAN8742_USE_HAL_MUTEX 1
#define LAN8742_ACQUIRE_BUS_TIMEOUT_MS  500U
#endif /* USE_HAL_MUTEX */

#define LAN8742_INVALID_PIN     0xFFuL
#define LAN8742_INVALID_IRQ     ((IRQn_Type) INT32_MAX)

/* Exported types ------------------------------------------------------------*/

typedef struct lan8742_io_s lan8742_io_t;

#if defined (LAN8742_CALLBACKS) && (LAN8742_CALLBACKS == 1)
/*!< IO callback function pointer definition */
typedef void (*lan8742_io_callback_t)(lan8742_io_t *pio, void *arg);

typedef struct
{
  lan8742_io_callback_t io_callback;
  void *arg;
} lan8742_io_cb_ctx_t;

#endif /* LAN8742_CALLBACKS */

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)

#endif /* LAN8742_USE_HAL_MUTEX */

/**
  * @brief ETH IO interface structure
  */

struct lan8742_io_s
{
  hal_eth_handle_t         *heth;             /*!< ETH HAL handle for communication          */
  uint8_t                  addr;              /*!< PHY address on the MDIO bus               */
  hal_exti_handle_t        *hexti;            /*!< EXTI HAL handle for asynchronous features */
  IRQn_Type                irq_src;           /*!< LAN8742 interrupt request line            */
  hal_gpio_t               it_port;           /*!< IT GPIO port                              */
  uint32_t                 it_pin;            /*!< IT GPIO pin                               */
#if defined (LAN8742_CALLBACKS) && (LAN8742_CALLBACKS == 1)
  lan8742_io_cb_ctx_t      exti_cb_ctx; /*!< Callback context storing the function pointer and argument */
#endif /* LAN8742_CALLBACKS */
  uint32_t                 nrst_pin;          /*!< RST PIN                                   */
  hal_gpio_t               nrst_port;         /*!< RST GPIO port                             */
  uint8_t                  nrst_active_state; /*!< RST PIN active state                      */
  uint32_t                 id;                /*!< LAN8742 part ID                           */
};

/* Exported functions */
int32_t lan8742_io_init(lan8742_io_t *pio);
int32_t lan8742_io_read_c22_reg(lan8742_io_t *pio, uint8_t reg, uint16_t *pvalue);
int32_t lan8742_io_write_c22_reg(lan8742_io_t *pio, uint8_t reg, uint16_t value);
int32_t lan8742_io_get_phy_address(lan8742_io_t *pio, uint8_t *paddr);
int32_t lan8742_io_set_phy_address(lan8742_io_t *pio, uint8_t addr);
void lan8742_io_hw_reset(lan8742_io_t *pio);

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
int32_t lan8742_io_acquire_bus(lan8742_io_t *pio);
int32_t lan8742_io_release_bus(lan8742_io_t *pio);
#endif /* LAN8742_USE_HAL_MUTEX */

int32_t lan8742_io_enable_irq(lan8742_io_t *pio);
int32_t lan8742_io_disable_irq(lan8742_io_t *pio);

#if defined (LAN8742_CALLBACKS) && (LAN8742_CALLBACKS == 1)
int32_t lan8742_io_register_callback(lan8742_io_t *pio, lan8742_io_callback_t io_cb, void *arg);
#endif /* LAN8742_CALLBACKS */

#ifdef __cplusplus
}
#endif

#endif /* LAN8742_IO_H */
