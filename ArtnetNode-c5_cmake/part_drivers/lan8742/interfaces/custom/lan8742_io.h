/**
  ******************************************************************************
  * @file    lan8742_io.h
  * @brief   LAN8742 header IO driver file (template, custom interface).
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
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

/*
 * Callback features are optional; the feature flag LAN8742_CALLBACKS signals their availability.
 * You can conditionally define this flag based on any preprocessor condition.
 * Examples using feature flags in mx_hal_def.h: USE_HAL_EXTI_REGISTER_CALLBACKS, USE_HAL_EXTI_USER_DATA...
 */
#define LAN8742_CALLBACKS 1

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

/** @brief CUSTOM IO object structure.
  * @note Add fields as needed to store HAL handles and other resources.
  */
struct lan8742_io_s
{
  uint32_t id;        /*!< Device ID                                 */
  uint32_t addr;      /*!< PHY address on the MDIO bus               */

  /* Add fields as needed to store HAL handles and other resources */

#if defined (LAN8742_CALLBACKS) && (LAN8742_CALLBACKS == 1)
  lan8742_io_cb_ctx_t      exti_cb_ctx; /*!< Callback context storing the function pointer and argument */
#endif /* LAN8742_CALLBACKS */
};

int32_t lan8742_io_init(lan8742_io_t *pio);
int32_t lan8742_io_write_c22_reg(lan8742_io_t *pio, uint8_t reg, uint16_t value);
int32_t lan8742_io_read_c22_reg(lan8742_io_t *pio, uint8_t reg, uint16_t *pvalue);

int32_t lan8742_io_enable_irq(lan8742_io_t *pio);
int32_t lan8742_io_disable_irq(lan8742_io_t *pio);

#if defined (LAN8742_CALLBACKS) && (LAN8742_CALLBACKS == 1)
int32_t lan8742_io_register_callback(lan8742_io_t *pio, lan8742_io_callback_t io_cb, void *arg);
#endif /* LAN8742_CALLBACKS */

#ifdef __cplusplus
}
#endif
#endif /* LAN8742_IO_H */
