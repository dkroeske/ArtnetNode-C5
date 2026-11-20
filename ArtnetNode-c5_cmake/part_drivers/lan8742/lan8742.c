/**
  ******************************************************************************
  * @file    lan8742.c
  * @brief   This file provides a set of functions needed to manage the LAN8742
  *          Ethernet PHY devices.
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
#include "lan8742.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LAN8742_SW_RESET_TO    ((uint32_t)500U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions definitions -----------------------------------------------------*/

/**
  * @brief  Initialize the LAN8742 PHY and configure the needed hardware resources.
  * @param  pobj   [in]     : Pointer to the part object structure
  * @param  dev_id [in]     : Instance ID
  * @retval error status
  */
lan8742_status_t lan8742_init(lan8742_obj_t *pobj, uint32_t dev_id)
{
  lan8742_status_t status = LAN8742_STATUS_OK;

  /* Instance must be in RESET to initialize */
  if (pobj->state != LAN8742_RESET)
  {
    status = LAN8742_STATUS_ERR_INVALID_STATE;
  }

  pobj->io.id = dev_id;

  /* Initialize the IO BUS layer: 0 == success */
  if (lan8742_io_init(&pobj->io) != 0)
  {
    status = LAN8742_STATUS_ERROR;
  }
  else
  {
    pobj->state = LAN8742_IDLE;
  }

  return status;
}

/**
  * @brief  De-Initialize the LAN8742 driver state.
  * @param  pobj [in,out] : Pointer to the part object structure
  * @retval LAN8742 status.
  */
lan8742_status_t lan8742_deinit(lan8742_obj_t *pobj)
{
  /* Only allowed when in IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

  pobj->state = LAN8742_RESET;

  return LAN8742_STATUS_OK;
}


/**
  * @brief  Perform a software reset of the LAN8742 PHY and wait for its completion.
  * @param  pobj [in] : Pointer to the LAN8742 device object
  * @retval LAN8742_STATUS_OK           if the reset completed successfully
  *         LAN8742_STATUS_WRITE_ERROR  if writing the reset command failed
  *         LAN8742_STATUS_READ_ERROR   if reading the register failed during reset
  *         LAN8742_STATUS_SW_RESET_TIMEOUT if the reset did not complete within the timeout period
  */
lan8742_status_t lan8742_software_reset(lan8742_obj_t *pobj)
{
  uint32_t tickstart = 0U;
  uint16_t regvalue = 0U;

  /* Only allowed from IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_acquire_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  /* Issue software reset */
  if (lan8742_io_write_c22_reg(&pobj->io, LAN8742_BCR, LAN8742_BCR_SOFT_RESET) != 0)
  {
    return LAN8742_STATUS_WRITE_ERROR;
  }

  /* Read BCR to observe reset bit */
  if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_BCR, &regvalue) != 0)
  {
    return LAN8742_STATUS_READ_ERROR;
  }

  tickstart = HAL_GetTick();

  /* Wait until software reset bit is cleared or timeout */
  while ((regvalue & LAN8742_BCR_SOFT_RESET) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > LAN8742_SW_RESET_TO)
    {
      return LAN8742_STATUS_SW_RESET_TIMEOUT;
    }

    if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_BCR, &regvalue) != 0)
    {
      return LAN8742_STATUS_READ_ERROR;
    }
  }

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_release_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  return LAN8742_STATUS_OK;
}

/**
  * @brief  Perform a hardware reset of the LAN8742 PHY using the IO layer.
  * @param  pobj [in] : Pointer to the part object structure
  * @retval LAN8742_STATUS_OK if OK, LAN8742_STATUS_ERROR otherwise
  */
lan8742_status_t lan8742_hardware_reset(lan8742_obj_t *pobj)
{
  /* Only allowed from IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

  /* Perform hardware reset sequence through IO layer */
  lan8742_io_hw_reset(&pobj->io);

  return LAN8742_STATUS_OK;
}

/**
  * @brief  Get the PHY ID of the LAN8742 device.
  * @param  pobj [in] : Pointer to component object
  * @param  id   [out] : Pointer to variable to store the 8-bit ID (model number)
  * @retval LAN8742_STATUS_OK if OK, LAN8742_STATUS_READ_ERROR otherwise
  */
lan8742_status_t lan8742_get_id(lan8742_obj_t *pobj, uint8_t *id)
{
  uint16_t phy_id1 = 0;
  uint16_t phy_id2 = 0;

  /* Only allowed in IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_acquire_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_PHYI1R, &phy_id1) != 0)
  {
    return LAN8742_STATUS_READ_ERROR;
  }

  if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_PHYI2R, &phy_id2) != 0)
  {
    return LAN8742_STATUS_READ_ERROR;
  }

  /* Extract the model number (bits 4-9 of the PHYI2R register) */
  uint8_t model = (uint8_t)((phy_id2 & LAN8742_PHYI2R_MODEL_NBR) >> 4);

  *id = model;

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_release_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  return LAN8742_STATUS_OK;
}

/**
  * @brief  Get the current MDIO PHY address used to access the LAN8742 device.
  * @param  pobj  [in]  Pointer to the LAN8742 component object.
  * @param  paddr [out] Pointer to store the retrieved MDIO address (0..31).
  * @retval LAN8742_STATUS_OK on success,
  *         LAN8742_STATUS_ERR_INVALID_STATE if called in invalid state,
  *         LAN8742_STATUS_ERROR otherwise.
  */
lan8742_status_t lan8742_get_phy_address(lan8742_obj_t *pobj, uint8_t *paddr)
{
  lan8742_status_t status = LAN8742_STATUS_ERROR;

  if (pobj->state != LAN8742_IDLE)
  {
    status = LAN8742_STATUS_ERR_INVALID_STATE;
  }
  else
  {
    status = (lan8742_io_get_phy_address(&pobj->io, paddr) == 0) ?
             LAN8742_STATUS_OK : LAN8742_STATUS_ERROR;
  }

  return status;
}

/**
  * @brief  Set the MDIO PHY address used to access the LAN8742 device.
  * @param  pobj [in] Pointer to the LAN8742 component object.
  * @param  addr [in] new PHY address on the MDIO bus to set (0..31).
  * @retval LAN8742_STATUS_OK on success,
  *         LAN8742_STATUS_ERR_INVALID_STATE if called in invalid state.
  *
  * Error handling contract:
  * - Any error different from LAN8742_STATUS_FATAL guarantees "no side effect": the PHY address
  *   is unchanged both in the IO context and in the PHY SMR register.
  * - If LAN8742_STATUS_FATAL is returned, the driver instance is no longer reliable.
  *   The caller must deinit and reinit the driver.
  */
lan8742_status_t lan8742_set_phy_address(lan8742_obj_t *pobj, uint8_t addr)
{
  lan8742_status_t status = LAN8742_STATUS_OK;
  uint8_t commit_done = 0U;
  uint8_t original_addr = 0U;
  uint16_t original_smr = 0U;

  if (addr > 31U)
  {
    status = LAN8742_STATUS_ERROR;
  }
  else if (pobj->state != LAN8742_IDLE)
  {
    status = LAN8742_STATUS_ERR_INVALID_STATE;
  }
  else if (lan8742_io_get_phy_address(&pobj->io, &original_addr) != 0)
  {
    /* IO context is not consistent: force reinitialization. */
    pobj->state = LAN8742_RESET;
    status = LAN8742_STATUS_FATAL;
  }
  else if (addr == original_addr)
  {
    /* Nothing to do. */
    status = LAN8742_STATUS_OK;
  }
  else
  {
#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
    lan8742_io_acquire_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

    /* Read SMR at the current address to preserve other bits and enable rollback. */
    if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_SMR, &original_smr) != 0)
    {
      status = LAN8742_STATUS_READ_ERROR;
    }

    /* Commit point: write the new PHY address into SMR at the current MDIO address. */
    if (status == LAN8742_STATUS_OK)
    {
      uint16_t new_smr = ((original_smr & ~LAN8742_SMR_PHY_ADDR) | ((uint16_t)addr & LAN8742_SMR_PHY_ADDR));
      if (lan8742_io_write_c22_reg(&pobj->io, LAN8742_SMR, new_smr) != 0)
      {
        /* No side effect guaranteed: commit did not succeed. */
        status = LAN8742_STATUS_WRITE_ERROR;
      }
      else
      {
        commit_done = 1U;
      }
    }

    /* Update IO layer address so subsequent accesses target the PHY at its new address. */
    if (status == LAN8742_STATUS_OK)
    {
      if (lan8742_io_set_phy_address(&pobj->io, addr) != 0)
      {
        /* After commit, we must not continue with an inconsistent software state. */
        pobj->state = LAN8742_RESET;
        status = LAN8742_STATUS_FATAL;
      }
    }

    /* Verify by reading SMR at the new address. */
    if (status == LAN8742_STATUS_OK)
    {
      uint16_t verify = 0U;
      if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_SMR, &verify) != 0)
      {
        status = LAN8742_STATUS_READ_ERROR;
      }
      else if ((verify & LAN8742_SMR_PHY_ADDR) != ((uint16_t)addr & LAN8742_SMR_PHY_ADDR))
      {
        status = LAN8742_STATUS_ERROR;
      }
      else
      {
        status = LAN8742_STATUS_OK;
      }
    }

    /* If an error occurred after the commit point, rollback to guarantee "no side effect".
     * If rollback cannot be completed, return LAN8742_STATUS_FATAL.
     */
    if ((status != LAN8742_STATUS_OK) && (commit_done != 0U) && (status != LAN8742_STATUS_FATAL))
    {
      uint8_t rollback_ok = 0U;

      /* Ensure we are addressing the PHY at the new address before reverting SMR. */
      if (lan8742_io_set_phy_address(&pobj->io, addr) != 0)
      {
        pobj->state = LAN8742_RESET;
        status = LAN8742_STATUS_FATAL;
      }

      if (status != LAN8742_STATUS_FATAL)
      {
        if (lan8742_io_write_c22_reg(&pobj->io, LAN8742_SMR, original_smr) == 0)
        {
          if (lan8742_io_set_phy_address(&pobj->io, original_addr) == 0)
          {
            rollback_ok = 1U;
          }
        }

        if (rollback_ok == 0U)
        {
          pobj->state = LAN8742_RESET;
          status = LAN8742_STATUS_FATAL;
        }
      }
    }

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
    lan8742_io_release_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */
  }

  return status;
}

/**
  * @brief  Get the link mode of LAN8742 device.
  * @param  pobj [in] : Pointer to component object
  * @param  link_mode [out] : link mode can be one of the following
  *         LAN8742_STATUS_LINK_DOWN  if link is down
  *         LAN8742_STATUS_100MBITS_FULLDUPLEX if 100Mb/s FD
  *         LAN8742_STATUS_100MBITS_HALFDUPLEX if 100Mb/s HD
  *         LAN8742_STATUS_10MBITS_FULLDUPLEX  if 10Mb/s FD
  *         LAN8742_STATUS_10MBITS_HALFDUPLEX  if 10Mb/s HD
  * @retval
  *         LAN8742_STATUS_OK if OK
  *         LAN8742_STATUS_AUTONEGO_NOTDONE if Auto nego not completed
  *         LAN8742_STATUS_READ_ERROR if cannot read register
  */
lan8742_status_t lan8742_get_link_mode(lan8742_obj_t *pobj, lan8742_link_t *link_mode)
{
  uint16_t readval = 0;
  lan8742_status_t status = LAN8742_STATUS_OK;

  /* Only allowed in IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_acquire_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  /* Read Status register (twice per spec) */
  if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_BSR, &readval) != 0)
  {
    status = LAN8742_STATUS_READ_ERROR;
  }
  if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_BSR, &readval) != 0)
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  if ((readval & LAN8742_BSR_LINK_STATUS) != (uint16_t)0U)
  {
    if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_PHYSCSR, &readval) != 0)
    {
      status = LAN8742_STATUS_READ_ERROR;
    }

    switch (readval & LAN8742_PHYSCSR_HCDSPEEDMASK)
    {
      /* 100BASE-T full-duplex */
      case LAN8742_PHYSCSR_100BTX_FD:
        link_mode->speed = LAN8742_LINK_SPEED_100;
        link_mode->duplex = LAN8742_LINK_FULL_DUPLEX;
        break;
      /* 100BASE-T half-duplex */
      case LAN8742_PHYSCSR_100BTX_HD:
        link_mode->speed = LAN8742_LINK_SPEED_100;
        link_mode->duplex = LAN8742_LINK_HALF_DUPLEX;
        break;
      /* 10BASE-T full-duplex */
      case LAN8742_PHYSCSR_10BT_FD:
        link_mode->speed = LAN8742_LINK_SPEED_10;
        link_mode->duplex = LAN8742_LINK_FULL_DUPLEX;
        break;
      /* 10BASE-T half-duplex */
      case LAN8742_PHYSCSR_10BT_HD:
        link_mode->speed = LAN8742_LINK_SPEED_10;
        link_mode->duplex = LAN8742_LINK_HALF_DUPLEX;
        break;
      /* Unknown operation mode */
      default:
        link_mode->speed = LAN8742_LINK_SPEED_NONE;
        link_mode->duplex = LAN8742_LINK_DUPLEX_NONE;
        break;
    }

    /* Check if auto nego not done */
    if ((readval & LAN8742_PHYSCSR_AUTONEGO_DONE) == (uint16_t)0U)
    {
      status = LAN8742_STATUS_AUTONEGO_NOTDONE;
    }
  }
  else
  {
    /* Return Link Down status */
    link_mode->status = LAN8742_LINK_DOWN;
    link_mode->speed = LAN8742_LINK_SPEED_NONE;
    link_mode->duplex = LAN8742_LINK_DUPLEX_NONE;
    status = LAN8742_STATUS_OK;
  }

  link_mode->status = LAN8742_LINK_UP;

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_release_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  return status;
}

/**
  * @brief  Get the current link configuration from the LAN8742 device registers.
  * @param  pobj [in] : Pointer to component object
  * @param  pconfig [out] : Pointer to a lan8742_link_config_t structure to fill with the current configuration
  * @retval LAN8742_STATUS_OK if successful
  *         LAN8742_STATUS_ERROR if pconfig is NULL
  *         LAN8742_STATUS_READ_ERROR if register read fails
  */
lan8742_status_t lan8742_get_link_config(lan8742_obj_t *pobj, lan8742_link_config_t *pconfig)
{
  uint16_t bcr = 0;
  lan8742_status_t status = LAN8742_STATUS_OK;

  /* Only allowed in IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_acquire_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  if (pconfig == NULL)
  {
    status = LAN8742_STATUS_ERROR;
  }

  if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_BCR, &bcr) != 0)
  {
    status = LAN8742_STATUS_READ_ERROR;
  }

  /* Check Auto negotiation */
  if ((bcr & LAN8742_BCR_AUTONEGO_EN) != (uint16_t)0U)
  {
    /* Auto-negotiation enabled, automatic configuration */
    pconfig->speed = LAN8742_LINK_SPEED_NONE;
    pconfig->duplex = LAN8742_LINK_DUPLEX_NONE;
  }
  else
  {
    /* Auto-negotiation disabled, read speed and duplex */
    pconfig->speed = (((bcr & LAN8742_BCR_SPEED_SELECT) != (uint16_t)0U) ? LAN8742_LINK_SPEED_100 :
                      LAN8742_LINK_SPEED_10);
    pconfig->duplex = (((bcr & LAN8742_BCR_DUPLEX_MODE) != (uint16_t)0U) ? LAN8742_LINK_FULL_DUPLEX :
                       LAN8742_LINK_HALF_DUPLEX);
  }

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_release_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  return status;
}

/**
  * @brief  Set the forced link configuration of the LAN8742 device via registers.
  * @param  pobj [in,out] : Pointer to component object
  * @param  pconfig [in] : Pointer to a lan8742_link_config_t structure to apply. Must not be NULL.
  *                        Auto-negotiation must be requested by calling `lan8742_start_link_autonego()` separately.
  * @retval LAN8742_STATUS_OK if operation successful
  *         LAN8742_STATUS_ERR_INVALID_STATE if not in IDLE state
  *         LAN8742_STATUS_ERROR if parameters are invalid (NULL, or speed/duplex set to NONE)
  *         LAN8742_STATUS_READ_ERROR if register read fails
  *         LAN8742_STATUS_WRITE_ERROR if register write fails
  */
lan8742_status_t lan8742_set_link_config(lan8742_obj_t *pobj, const lan8742_link_config_t *pconfig)
{
  uint16_t bcr = 0;
  lan8742_status_t ret = LAN8742_STATUS_OK;

  /* Only allowed in IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    ret = LAN8742_STATUS_ERR_INVALID_STATE;
  }

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_acquire_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  /* Read current Basic Control Register (BCR) */
  if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_BCR, &bcr) != 0)
  {
    ret = LAN8742_STATUS_READ_ERROR;
  }

  if (pconfig == NULL)
  {
    /* Invalid parameter */
    ret = LAN8742_STATUS_ERROR;
  }
  else
  {
    /* Validate speed (must be 10 or 100; NONE is invalid here) */
    if ((pconfig->speed != LAN8742_LINK_SPEED_10) && (pconfig->speed != LAN8742_LINK_SPEED_100))
    {
      ret = LAN8742_STATUS_ERROR;
    }

    /* Validate duplex (must be HALF or FULL; NONE is invalid here) */
    if ((pconfig->duplex != LAN8742_LINK_HALF_DUPLEX) && (pconfig->duplex != LAN8742_LINK_FULL_DUPLEX))
    {
      ret = LAN8742_STATUS_ERROR;
    }

    if (ret == LAN8742_STATUS_OK)
    {
      /* Disable auto-negotiation and apply forced settings */
      bcr &= ~LAN8742_BCR_AUTONEGO_EN;

      /* Set speed bit */
      if (pconfig->speed == LAN8742_LINK_SPEED_100)
      {
        bcr |= LAN8742_BCR_SPEED_SELECT;
      }
      else
      {
        bcr &= ~LAN8742_BCR_SPEED_SELECT;
      }

      /* Set duplex bit */
      if (pconfig->duplex == LAN8742_LINK_FULL_DUPLEX)
      {
        bcr |= LAN8742_BCR_DUPLEX_MODE;
      }
      else
      {
        bcr &= ~LAN8742_BCR_DUPLEX_MODE;
      }

      /* Write updated BCR register */
      if (lan8742_io_write_c22_reg(&pobj->io, LAN8742_BCR, bcr) != 0)
      {
        ret = LAN8742_STATUS_WRITE_ERROR;
      }
    }
  }

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_release_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  return ret;
}

/**
  * @brief  Start the auto negotiation process.
  * @param  pobj [in,out] : Pointer to component object
  * @retval LAN8742_STATUS_OK  if OK
  *         LAN8742_STATUS_READ_ERROR if cannot read register
  *         LAN8742_STATUS_WRITE_ERROR if cannot write to register
  */
lan8742_status_t lan8742_start_link_autonego(lan8742_obj_t *pobj)
{
  uint16_t readval = 0;

  /* Only allowed in IDLE state (IDLE -> ACTIVE on autonego start) */
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_acquire_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_BCR, &readval) != 0)
  {
    return LAN8742_STATUS_READ_ERROR;
  }

  readval |= LAN8742_BCR_AUTONEGO_EN | LAN8742_BCR_RESTART_AUTONEGO;

  /* Apply configuration */
  if (lan8742_io_write_c22_reg(&pobj->io, LAN8742_BCR, readval) != 0)
  {
    return LAN8742_STATUS_WRITE_ERROR;
  }

  /* update state to ACTIVE */
  pobj->state = LAN8742_ACTIVE;

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
  lan8742_io_release_bus(&pobj->io);
#endif /* LAN8742_USE_HAL_MUTEX */

  return LAN8742_STATUS_OK;
}

/**
  * @brief  Write a value to a LAN8742 register.
  * @param  pobj [in] : Pointer to component object
  * @param  reg [in] : Register address within the LAN8742 device
  * @param  value [in] : Data value to write to the specified register
  * @retval LAN8742_STATUS_OK if OK
  *         LAN8742_STATUS_WRITE_ERROR if cannot write to register
  *         LAN8742_STATUS_ERR_INVALID_STATE if not in IDLE state
  */
lan8742_status_t lan8742_write_c22_reg(lan8742_obj_t *pobj, uint8_t reg, uint16_t value)
{
  /* Only allowed in IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

  if (lan8742_io_write_c22_reg(&pobj->io, reg, value) != 0)
  {
    return LAN8742_STATUS_WRITE_ERROR;
  }

  return LAN8742_STATUS_OK;
}

/**
  * @brief  Read a value from a LAN8742 register.
  * @param  pobj [in] : Pointer to component object
  * @param  reg [in] : the device register to read
  * @param  pvalue [out] : pointer to the read value
  * @retval LAN8742_STATUS_OK if OK
  *         LAN8742_STATUS_READ_ERROR if cannot read register
  *         LAN8742_STATUS_ERR_INVALID_STATE if not in IDLE state
  */
lan8742_status_t lan8742_read_c22_reg(lan8742_obj_t *pobj, uint8_t reg, uint16_t *pvalue)
{
  /* Only allowed in IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

  if (lan8742_io_read_c22_reg(&pobj->io, reg, pvalue) != 0)
  {
    return LAN8742_STATUS_READ_ERROR;
  }

  return LAN8742_STATUS_OK;
}

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)

/**
  * @brief Acquire access to the ETH bus.
  * @param pobj [in] : Pointer to component object
  * @retval LAN8742_STATUS_OK The bus was successfully acquired.
  *         LAN8742_STATUS_ERROR The bus could not be acquired.
  */
lan8742_status_t lan8742_acquire_bus(lan8742_obj_t *pobj)
{
  if (lan8742_io_acquire_bus(&pobj->io) != 0)
  {
    return LAN8742_STATUS_ERROR;
  }

  return LAN8742_STATUS_OK;
}

/**
  * @brief Release access to the ETH bus.
  * @param pobj [in] : Pointer to component object
  * @retval LAN8742_STATUS_OK The bus was successfully released.
  *         LAN8742_STATUS_ERROR The bus could not be released.
  */
lan8742_status_t lan8742_release_bus(lan8742_obj_t *pobj)
{
  if (lan8742_io_release_bus(&pobj->io) != 0)
  {
    return LAN8742_STATUS_ERROR;
  }

  return LAN8742_STATUS_OK;
}

#endif /* LAN8742_USE_HAL_MUTEX */

/******************************************************************************/
/*                     Interruption functions                                 */
/******************************************************************************/

/**
  * @brief  Enables the interrupt mode for the selected IT source.
  * @param  pobj [in] : Pointer to component object.
  * @param  it_src [in] : The interrupt source to be configured.
  * @retval error status
  */
lan8742_status_t lan8742_enable_it_source(lan8742_obj_t *pobj, lan8742_it_source_t it_src)
{
  uint16_t imrvalue = 0;
  uint16_t prev_imr = 0;

  /* Only allowed in IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

  /* Get the current value of the Interrupt Mask Register (IMR) */
  if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_IMR, &imrvalue) != 0)
  {
    return LAN8742_STATUS_READ_ERROR;
  }

  /* Save the current state of the interrupts */
  prev_imr = imrvalue;
  /* Set the interrupts to be Enabled (truncate it_src to 16 bit) */
  imrvalue |= (uint16_t)it_src;

  /* Set the register */
  if (lan8742_io_write_c22_reg(&pobj->io, LAN8742_IMR, imrvalue) != 0)
  {
    return LAN8742_STATUS_WRITE_ERROR;
  }

  /* Enable the IRQ if this is the first interrupt */
  if (prev_imr == 0U)
  {
    if (lan8742_io_enable_irq(&pobj->io) != 0)
    {
      return LAN8742_STATUS_ERROR;
    }
  }

  return LAN8742_STATUS_OK;
}

/**
  * @brief  Disables the interrupt mode for the selected IT source.
  * @param  pobj [in] : Pointer to component object.
  * @param  it_src [in] : The interrupt source to be disabled.
  * @retval error status.
  */
lan8742_status_t lan8742_disable_it_source(lan8742_obj_t *pobj, lan8742_it_source_t it_src)
{
  uint16_t imrvalue = 0;

  /* Only allowed in IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

  /* Get the current value of the Interrupt Mask Register (IMR) */
  if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_IMR, &imrvalue) < 0)
  {
    return LAN8742_STATUS_READ_ERROR;
  }

  /* Set the interrupts to be Disabled (truncate it_src to 16 bit) */
  imrvalue &= (uint16_t)~((uint16_t)it_src);

  /* Set the register */
  if (lan8742_io_write_c22_reg(&pobj->io, LAN8742_IMR, imrvalue) < 0)
  {
    return LAN8742_STATUS_WRITE_ERROR;
  }

  /* if all interrupts are disabled, disable the irq */

  if (imrvalue == 0U)
  {
    if (lan8742_io_disable_irq(&pobj->io) != 0)
    {
      return LAN8742_STATUS_ERROR;
    }
  }

  return LAN8742_STATUS_OK;
}

/**
  * @brief  Get IT Flag status.
  * @param  pobj [in] : Pointer to component object
  * @param  it_src [in] : The interrupt source to be disabled.
  * @retval 1 IT flag is SET
  *         0 IT flag is RESET
  *         LAN8742_STATUS_READ_ERROR if cannot read register
  */
int32_t lan8742_get_it_status(lan8742_obj_t *pobj, lan8742_it_source_t it_src)
{
  uint16_t isfrvalue = 0;
  uint16_t bsrvalue = 0;
  int32_t status = 0;

  /* Only allowed in IDLE state */
  if (pobj->state != LAN8742_IDLE)
  {
    return (int32_t)LAN8742_STATUS_ERR_INVALID_STATE;
  }

  if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_ISFR, &isfrvalue) >= 0)
  {
    uint16_t it_mask = (uint16_t)it_src;
    status = (int32_t)(((isfrvalue & it_mask) == it_mask) ? 1 : 0);
  }
  else
  {
    status = (int32_t)LAN8742_STATUS_READ_ERROR;
  }

  /* Read BSR register to clear link-down interrupt(s). */
  if (((uint16_t)it_src & (uint16_t)LAN8742_IT_SRC_LINK_DOWN) != 0U)
  {
    if (lan8742_io_read_c22_reg(&pobj->io, LAN8742_BSR, &bsrvalue) < 0)
    {
      status = (int32_t)LAN8742_STATUS_READ_ERROR;
    }
  }

  return status;
}

#if defined (LAN8742_CALLBACKS) && (LAN8742_CALLBACKS == 1)

/**
  * @brief  Interruption callback handler for LAN8742.
  * @param  pio [in] : IO interface pointer
  * @param  arg [in] : Pointer to lan8742_obj_t
  */
static void lan8742_io_callback(lan8742_io_t *pio, void *arg)
{
  (void)pio;
  lan8742_obj_t *pobj = arg;

  if (pobj->cb_ctx_event.callback != NULL)
  {
    pobj->cb_ctx_event.callback(pobj, pobj->cb_ctx_event.arg);
  }
}

/**
  * @brief  Registers an event callback for LAN8742.
  * @param  pobj [in,out] : Pointer to the LAN8742 component object.
  * @param  cb [in] : Event callback function to be registered.
  * @param  arg [in] : Optional argument to be passed to the callback function.
  * @retval LAN8742_STATUS_OK if the callback was registered successfully.
  *         LAN8742_STATUS_ERR_INVALID_STATE if the component is not in the idle state.
  *         LAN8742_STATUS_ERROR if the callback registration failed.
  *
  * This function registers a user-defined event callback for LAN8742.
  * The callback will be invoked when an event is detected. The function can only
  * be called when the component is in the idle state.
  */
lan8742_status_t lan8742_register_event_callback(lan8742_obj_t *pobj, lan8742_callback_t cb, void *arg)
{
  if (pobj->state != LAN8742_IDLE)
  {
    return LAN8742_STATUS_ERR_INVALID_STATE;
  }

  pobj->cb_ctx_event.callback = cb;
  pobj->cb_ctx_event.arg = arg;

  if (lan8742_io_register_callback(&pobj->io, lan8742_io_callback, pobj) == 0)
  {
    return LAN8742_STATUS_OK;
  }
  else
  {
    return LAN8742_STATUS_ERROR;
  }
}

#endif /* LAN8742_CALLBACKS */
