/**
  ******************************************************************************
  * @file    lan8742_io.c
  * @brief   This file provides a set of functions needed to interface
  *          the lan8742 ethernet PHY (template, custom interface).
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

/* Includes ------------------------------------------------------------------*/
#include "lan8742_io.h"

int32_t lan8742_io_init(lan8742_io_t *pio)
{
  switch (pio->id)
  {
    /* assign resources to the pio according to its ID */
    /* case LAN8742_ID_0: ... */
    default:
      return -1; /* unrecognized object: return an error */
  }
  return 0;
}

int32_t lan8742_io_write_c22_reg(lan8742_io_t *pio, uint8_t device_register, uint16_t value)
{
  /* Implement the write operation for your custom interface here */
  return -1;
}

int32_t lan8742_io_read_c22_reg(lan8742_io_t *pio, uint8_t device_register, uint16_t *pvalue)
{
  /* Implement the read operation for your custom interface here */
  return -1;
}

/**
  * @brief  Enables the IRQ line and starts the EXTI.
  * @param  pio : Pointer to io interface object
  * @retval error status (0=success, -1=error)
  */
int32_t lan8742_io_enable_irq(lan8742_io_t *pio)
{
  return -1;
}

/**
  * @brief  Disables the IRQ line and clears the interrupt pending bit.
  * @param  pio   Pointer to io interface object
  * @retval error status (0=success, -1=error)
  */
int32_t lan8742_io_disable_irq(lan8742_io_t *pio)
{
  return -1;
}

#if defined (LAN8742_CALLBACKS) && (LAN8742_CALLBACKS == 1)

/**
  * @brief Registers a callback on the IT pin of the LAN8742.
  * @param  pio   : Pointer to io interface object
  * @param  io_cb : Callback function to be registered
  * @param  arg   : Optional argument given to the callback
  * @retval error status (0=success, -1=error)
  */
int32_t lan8742_io_register_callback(lan8742_io_t *pio, lan8742_io_callback_t io_cb, void *arg)
{
  pio->exti_cb_ctx.io_callback = io_cb;
  pio->exti_cb_ctx.arg = arg;

  /* Implement your own callback mechanism here. */

  return -1;
}

#endif /* LAN8742_CALLBACKS */
