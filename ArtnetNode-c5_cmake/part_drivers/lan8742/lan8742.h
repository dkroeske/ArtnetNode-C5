/**
  ******************************************************************************
  * @file    lan8742.h
  * @brief   This file contains the functions prototypes for the
  *          lan8742.c ETH PHY driver.
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
#ifndef LAN8742_H
#define LAN8742_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lan8742_io.h"
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

/**
  * @brief LAN8742 part driver state definition.
  */
typedef enum
{
  LAN8742_RESET = 0,          /*!<  Instance not initialized  */
  LAN8742_IDLE,               /*!<  Instance initialized      */
  LAN8742_ACTIVE
} lan8742_state_t;

/**
  * @brief LAN8742 status structure.
  */
typedef enum
{
  LAN8742_STATUS_FATAL = -7,   /*!< Fatal error: driver state is no longer reliable; deinit/reinit required */
  LAN8742_STATUS_AUTONEGO_NOTDONE = -6,
  LAN8742_STATUS_READ_ERROR = -5,
  LAN8742_STATUS_WRITE_ERROR = -4,
  LAN8742_STATUS_SW_RESET_TIMEOUT = -3,
  LAN8742_STATUS_ERR_INVALID_STATE = -2,
  LAN8742_STATUS_ERROR = -1,              /*!< Error Status     */
  LAN8742_STATUS_OK = 0                   /*!< Success Status   */
} lan8742_status_t;

typedef enum
{
  LAN8742_LINK_DOWN = 0U,
  LAN8742_LINK_UP = 1U
} lan8742_link_status_t;

typedef enum
{
  LAN8742_LINK_SPEED_NONE = 0U,
  LAN8742_LINK_SPEED_10 = 10U,
  LAN8742_LINK_SPEED_100 = 100U
} lan8742_link_speed_t;

typedef enum
{
  LAN8742_LINK_DUPLEX_NONE = 0U,
  LAN8742_LINK_HALF_DUPLEX = 1U,
  LAN8742_LINK_FULL_DUPLEX = 2U
} lan8742_link_duplex_t;

/**
  * @brief LAN8742 ethernet link structure.
  */
typedef struct
{
  lan8742_link_status_t status;   /*!< Link status: LAN8742_LINK_DOWN or LAN8742_LINK_UP */
  lan8742_link_speed_t speed;     /*!< Link speed: LAN8742_LINK_SPEED_10 or LAN8742_LINK_SPEED_100 */
  lan8742_link_duplex_t duplex;   /*!< Duplex mode: LAN8742_LINK_HALF_DUPLEX or LAN8742_LINK_FULL_DUPLEX */
  /* uint8_t pause;          / Symmetric pause           */
  /* uint8_t asym_pause;     / Asymmetric pause          */
} lan8742_link_t;

/**
  * @brief LAN8742 link configuration structure.
  */
typedef struct
{
  lan8742_link_speed_t speed;     /*!< Link speed: LAN8742_LINK_SPEED_10 or LAN8742_LINK_SPEED_100 */
  lan8742_link_duplex_t duplex;   /*!< Duplex mode: LAN8742_LINK_HALF_DUPLEX or LAN8742_LINK_FULL_DUPLEX */
  /*  uint8_t pause;          / Symmetric pause           */
  /*  uint8_t asym_pause;     / Asymmetric pause          */
} lan8742_link_config_t;

typedef struct lan8742_obj_s lan8742_obj_t;

#if defined (LAN8742_CALLBACKS) && (LAN8742_CALLBACKS == 1)
/*!< callback function pointer definition */
typedef void (*lan8742_callback_t)(lan8742_obj_t *pobj, void *arg);

/**
  * @brief Callback context structure
  */
typedef struct
{
  lan8742_callback_t callback;   /*!< Callback function to register           */
  void *arg;                          /*!< Optional argument given to the callback */
} lan8742_cb_ctx_t;

#endif /* LAN8742_CALLBACKS */

/**
  * @brief LAN8742 part object structure.
  */
struct lan8742_obj_s
{
  lan8742_state_t          state;           /*!< Init state              */
  lan8742_io_t             io;              /*!< IO interface structure  */
#if defined (LAN8742_CALLBACKS) && (LAN8742_CALLBACKS == 1)
  lan8742_cb_ctx_t cb_ctx_event;    /*!< callback context for PHY events */
#endif /* LAN8742_CALLBACKS */
};

/* Exported constants --------------------------------------------------------*/

#define LAN8742_MAX_DEV_ADDR   ((uint32_t)31U)

/******************************************************************************
  * @brief  LAN8742 Serial Management Interface (SMI) Register Mapping
  ****************************************************************************/

#define LAN8742_BCR                           ((uint8_t)0x00U)
#define LAN8742_BSR                           ((uint8_t)0x01U)
#define LAN8742_PHYI1R                        ((uint8_t)0x02U)
#define LAN8742_PHYI2R                        ((uint8_t)0x03U)
#define LAN8742_ANAR                          ((uint8_t)0x04U)
#define LAN8742_ANLPAR                        ((uint8_t)0x05U)
#define LAN8742_ANER                          ((uint8_t)0x06U)
#define LAN8742_ANNPTR                        ((uint8_t)0x07U)
#define LAN8742_ANNPRR                        ((uint8_t)0x08U)
#define LAN8742_MMDACR                        ((uint8_t)0x0DU)
#define LAN8742_MMDAADR                       ((uint8_t)0x0EU)
#define LAN8742_ENCTR                         ((uint8_t)0x10U)
#define LAN8742_MCSR                          ((uint8_t)0x11U)
#define LAN8742_SMR                           ((uint8_t)0x12U)
#define LAN8742_TPDCR                         ((uint8_t)0x18U)
#define LAN8742_TCSR                          ((uint8_t)0x19U)
#define LAN8742_SECR                          ((uint8_t)0x1AU)
#define LAN8742_SCSIR                         ((uint8_t)0x1BU)
#define LAN8742_CLR                           ((uint8_t)0x1CU)
#define LAN8742_ISFR                          ((uint8_t)0x1DU)
#define LAN8742_IMR                           ((uint8_t)0x1EU)
#define LAN8742_PHYSCSR                       ((uint8_t)0x1FU)


/** LAN8742 Basic Control Register (BCR) Bit Definition
  */
#define LAN8742_BCR_SOFT_RESET                ((uint16_t)0x8000U)
#define LAN8742_BCR_LOOPBACK                  ((uint16_t)0x4000U)
#define LAN8742_BCR_SPEED_SELECT              ((uint16_t)0x2000U)
#define LAN8742_BCR_AUTONEGO_EN               ((uint16_t)0x1000U)
#define LAN8742_BCR_POWER_DOWN                ((uint16_t)0x0800U)
#define LAN8742_BCR_ISOLATE                   ((uint16_t)0x0400U)
#define LAN8742_BCR_RESTART_AUTONEGO          ((uint16_t)0x0200U)
#define LAN8742_BCR_DUPLEX_MODE               ((uint16_t)0x0100U)


/** LAN8742 Basic Status Register (BSR) Bit Definition
  */
#define LAN8742_BSR_100BASE_T4                ((uint16_t)0x8000U)
#define LAN8742_BSR_100BASE_TX_FD             ((uint16_t)0x4000U)
#define LAN8742_BSR_100BASE_TX_HD             ((uint16_t)0x2000U)
#define LAN8742_BSR_10BASE_T_FD               ((uint16_t)0x1000U)
#define LAN8742_BSR_10BASE_T_HD               ((uint16_t)0x0800U)
#define LAN8742_BSR_100BASE_T2_FD             ((uint16_t)0x0400U)
#define LAN8742_BSR_100BASE_T2_HD             ((uint16_t)0x0200U)
#define LAN8742_BSR_EXTENDED_STATUS           ((uint16_t)0x0100U)
#define LAN8742_BSR_AUTONEGO_CPLT             ((uint16_t)0x0020U)
#define LAN8742_BSR_REMOTE_FAULT              ((uint16_t)0x0010U)
#define LAN8742_BSR_AUTONEGO_ABILITY          ((uint16_t)0x0008U)
#define LAN8742_BSR_LINK_STATUS               ((uint16_t)0x0004U)
#define LAN8742_BSR_JABBER_DETECT             ((uint16_t)0x0002U)
#define LAN8742_BSR_EXTENDED_CAP              ((uint16_t)0x0001U)


/** LAN8742 PHY Identifier 1 Register (PHYI1R) Bit Definition
  */
#define LAN8742_PHYI1R_OUI_3_18               ((uint16_t)0xFFFFU)
#define LAN8742_PHYI1R_OUI_3_18_DEFAULT       ((uint16_t)0x0007U)


/** LAN8742 PHY Identifier 2 Register (PHYI2R) Bit Definition
  */
#define LAN8742_PHYI2R_OUI_19_24              ((uint16_t)0xFC00U)
#define LAN8742_PHYI2R_OUI_19_24_DEFAULT      ((uint16_t)0xC000U)
#define LAN8742_PHYI2R_MODEL_NBR              ((uint16_t)0x03F0U)
#define LAN8742_PHYI2R_MODEL_NBR_DEFAULT      ((uint16_t)0x0130U)
#define LAN8742_PHYI2R_REVISION_NBR           ((uint16_t)0x000FU)
#define LAN8742_PHYI2R_REVISION_NBR_DEFAULT   ((uint16_t)0x0000U)


/** LAN8742 Auto-Negotiation Advertisement Register (ANAR) Bit Definition
  */
#define LAN8742_ANAR_NEXT_PAGE                ((uint16_t)0x8000U)
#define LAN8742_ANAR_REMOTE_FAULT             ((uint16_t)0x2000U)
#define LAN8742_ANAR_PAUSE_OPERATION          ((uint16_t)0x0C00U)
#define LAN8742_ANAR_PO_NOPAUSE               ((uint16_t)0x0000U)
#define LAN8742_ANAR_PO_SYMMETRIC_PAUSE       ((uint16_t)0x0400U)
#define LAN8742_ANAR_PO_ASYMMETRIC_PAUSE      ((uint16_t)0x0800U)
#define LAN8742_ANAR_PO_ADVERTISE_SUPPORT     ((uint16_t)0x0C00U)
#define LAN8742_ANAR_100BASE_TX_FD            ((uint16_t)0x0100U)
#define LAN8742_ANAR_100BASE_TX               ((uint16_t)0x0080U)
#define LAN8742_ANAR_10BASE_T_FD              ((uint16_t)0x0040U)
#define LAN8742_ANAR_10BASE_T                 ((uint16_t)0x0020U)
#define LAN8742_ANAR_SELECTOR_FIELD           ((uint16_t)0x000FU)


/** LAN8742 Auto-Negotiation Link Partner Ability Register (ANLPAR) Bit Definition
  */
#define LAN8742_ANLPAR_NEXT_PAGE              ((uint16_t)0x8000U)
#define LAN8742_ANLPAR_REMOTE_FAULT           ((uint16_t)0x2000U)
#define LAN8742_ANLPAR_PAUSE_OPERATION        ((uint16_t)0x0C00U)
#define LAN8742_ANLPAR_PO_NOPAUSE             ((uint16_t)0x0000U)
#define LAN8742_ANLPAR_PO_SYMMETRIC_PAUSE     ((uint16_t)0x0400U)
#define LAN8742_ANLPAR_PO_ASYMMETRIC_PAUSE    ((uint16_t)0x0800U)
#define LAN8742_ANLPAR_PO_ADVERTISE_SUPPORT   ((uint16_t)0x0C00U)
#define LAN8742_ANLPAR_100BASE_TX_FD          ((uint16_t)0x0100U)
#define LAN8742_ANLPAR_100BASE_TX             ((uint16_t)0x0080U)
#define LAN8742_ANLPAR_10BASE_T_FD            ((uint16_t)0x0040U)
#define LAN8742_ANLPAR_10BASE_T               ((uint16_t)0x0020U)
#define LAN8742_ANLPAR_SELECTOR_FIELD         ((uint16_t)0x000FU)


/** LAN8742 Auto-Negotiation Expansion Register (ANER) Bit Definition
  */
#define LAN8742_ANER_RX_NP_LOCATION_ABLE      ((uint16_t)0x0040U)
#define LAN8742_ANER_RX_NP_STORAGE_LOCATION   ((uint16_t)0x0020U)
#define LAN8742_ANER_PARALLEL_DETECT_FAULT    ((uint16_t)0x0010U)
#define LAN8742_ANER_LP_NP_ABLE               ((uint16_t)0x0008U)
#define LAN8742_ANER_NP_ABLE                  ((uint16_t)0x0004U)
#define LAN8742_ANER_PAGE_RECEIVED            ((uint16_t)0x0002U)
#define LAN8742_ANER_LP_AUTONEG_ABLE          ((uint16_t)0x0001U)


/** LAN8742 Auto-Negotiation Next Page TX Register (ANNPTR) Bit Definition
  */
#define LAN8742_ANNPTR_NEXT_PAGE              ((uint16_t)0x8000U)
#define LAN8742_ANNPTR_MESSAGE_PAGE           ((uint16_t)0x2000U)
#define LAN8742_ANNPTR_ACK2                   ((uint16_t)0x1000U)
#define LAN8742_ANNPTR_TOGGLE                 ((uint16_t)0x0800U)
#define LAN8742_ANNPTR_MESSAGGE_CODE          ((uint16_t)0x07FFU)


/** LAN8742 Auto-Negotiation Next Page RX Register (ANNPRR) Bit Definition
  */
#define LAN8742_ANNPRR_NEXT_PAGE              ((uint16_t)0x8000U)
#define LAN8742_ANNPRR_ACK                    ((uint16_t)0x4000U)
#define LAN8742_ANNPRR_MESSAGE_PAGE           ((uint16_t)0x2000U)
#define LAN8742_ANNPRR_ACK2                   ((uint16_t)0x1000U)
#define LAN8742_ANNPRR_TOGGLE                 ((uint16_t)0x0800U)
#define LAN8742_ANNPRR_MESSAGGE_CODE          ((uint16_t)0x07FFU)


/** LAN8742 MMD Access Control Register (MMDACR) Bit Definition
  */
#define LAN8742_MMDACR_MMD_FUNCTION           ((uint16_t)0xC000U)
#define LAN8742_MMDACR_MMD_FUNCTION_ADDR      ((uint16_t)0x0000U)
#define LAN8742_MMDACR_MMD_FUNCTION_DATA      ((uint16_t)0x4000U)
#define LAN8742_MMDACR_MMD_DEV_ADDR           ((uint16_t)0x001FU)

/** LAN8742 MMD Access Address/Data Register (MMDADR) Bit Definition
  */
#define LAN8742_MMDAADR_MMD_REG_ADDR_DATA_MASK    ((uint16_t)0xFFFFU)
#define LAN8742_MMDAADR_MMD_REG_ADDR_DATA_DEFAULT ((uint16_t)0x0000U)

/** LAN8742 EDPD NLP/Crossover Time Register (ENCTR) Bit Definition
  */
#define LAN8742_ENCTR_TX_ENABLE               ((uint16_t)0x8000U)
#define LAN8742_ENCTR_TX_TIMER                ((uint16_t)0x6000U)
#define LAN8742_ENCTR_TX_TIMER_1S             ((uint16_t)0x0000U)
#define LAN8742_ENCTR_TX_TIMER_768MS          ((uint16_t)0x2000U)
#define LAN8742_ENCTR_TX_TIMER_512MS          ((uint16_t)0x4000U)
#define LAN8742_ENCTR_TX_TIMER_265MS          ((uint16_t)0x6000U)
#define LAN8742_ENCTR_RX_ENABLE               ((uint16_t)0x1000U)
#define LAN8742_ENCTR_RX_MAX_INTERVAL         ((uint16_t)0x0C00U)
#define LAN8742_ENCTR_RX_MAX_INTERVAL_64MS    ((uint16_t)0x0000U)
#define LAN8742_ENCTR_RX_MAX_INTERVAL_256MS   ((uint16_t)0x0400U)
#define LAN8742_ENCTR_RX_MAX_INTERVAL_512MS   ((uint16_t)0x0800U)
#define LAN8742_ENCTR_RX_MAX_INTERVAL_1S      ((uint16_t)0x0C00U)
#define LAN8742_ENCTR_EX_CROSS_OVER           ((uint16_t)0x0002U)
#define LAN8742_ENCTR_EX_MANUAL_CROSS_OVER    ((uint16_t)0x0001U)


/** LAN8742 Mode Control/Status Register (MCSR) Bit Definition
  */
#define LAN8742_MCSR_EDPWRDOWN                ((uint16_t)0x2000U)
#define LAN8742_MCSR_FARLOOPBACK              ((uint16_t)0x0200U)
#define LAN8742_MCSR_ALTINT                   ((uint16_t)0x0040U)
#define LAN8742_MCSR_ENERGYON                 ((uint16_t)0x0002U)


/** LAN8742 Special Mode Register (SMR) Bit Definition
  */
#define LAN8742_SMR_MODE                      ((uint16_t)0x00E0U)
#define LAN8742_SMR_PHY_ADDR                  ((uint16_t)0x001FU)


/** LAN8742 TDR Patterns/Delay Control Register (TPDCR) Bit Definition
  */
#define LAN8742_TPDCR_DELAY_IN                ((uint16_t)0x8000U)
#define LAN8742_TPDCR_LINE_BREAK_COUNTER      ((uint16_t)0x7000U)
#define LAN8742_TPDCR_PATTERN_HIGH            ((uint16_t)0x0FC0U)
#define LAN8742_TPDCR_PATTERN_LOW             ((uint16_t)0x003FU)


/** LAN8742 TDR Control/Status Register (TCSR) Bit Definition
  */
#define LAN8742_TCSR_TDR_ENABLE               ((uint16_t)0x8000U)
#define LAN8742_TCSR_TDR_AD_FILTER_ENABLE     ((uint16_t)0x4000U)
#define LAN8742_TCSR_TDR_CH_CABLE_TYPE        ((uint16_t)0x0600U)
#define LAN8742_TCSR_TDR_CH_CABLE_DEFAULT     ((uint16_t)0x0000U)
#define LAN8742_TCSR_TDR_CH_CABLE_SHORTED     ((uint16_t)0x0200U)
#define LAN8742_TCSR_TDR_CH_CABLE_OPEN        ((uint16_t)0x0400U)
#define LAN8742_TCSR_TDR_CH_CABLE_MATCH       ((uint16_t)0x0600U)
#define LAN8742_TCSR_TDR_CH_STATUS            ((uint16_t)0x0100U)
#define LAN8742_TCSR_TDR_CH_LENGTH            ((uint16_t)0x00FFU)


/** LAN8742 Symbol Error Counter Register (SECR) Bit Definition
  */

/** LAN8742 Special Control/Status Indications Register (SCSIR) Bit Definition
  */
#define LAN8742_SCSIR_AUTO_MDIX_ENABLE        ((uint16_t)0x8000U)
#define LAN8742_SCSIR_CHANNEL_SELECT          ((uint16_t)0x2000U)
#define LAN8742_SCSIR_SQE_DISABLE             ((uint16_t)0x0800U)
#define LAN8742_SCSIR_XPOLALITY               ((uint16_t)0x0010U)


/** LAN8742 Cable Length Register (CLR) Bit Definition
  */
#define LAN8742_CLR_CABLE_LENGTH              ((uint16_t)0xF000U)

/** LAN8742 Interrupt Source Flag Register (IMR)
  * & LAN8742 Interrupt Mask Register (IMR) Bit Definition
  */
#define LAN8742_INT_8                         ((uint16_t)0x0100U)
#define LAN8742_INT_7                         ((uint16_t)0x0080U)
#define LAN8742_INT_6                         ((uint16_t)0x0040U)
#define LAN8742_INT_5                         ((uint16_t)0x0020U)
#define LAN8742_INT_4                         ((uint16_t)0x0010U)
#define LAN8742_INT_3                         ((uint16_t)0x0008U)
#define LAN8742_INT_2                         ((uint16_t)0x0004U)
#define LAN8742_INT_1                         ((uint16_t)0x0002U)

/** LAN8742 PHY Special Control/Status Register (PHYSCSR) Bit Definition
  */
#define LAN8742_PHYSCSR_AUTONEGO_DONE         ((uint16_t)0x1000U)
#define LAN8742_PHYSCSR_HCDSPEEDMASK          ((uint16_t)0x001CU)
#define LAN8742_PHYSCSR_10BT_HD               ((uint16_t)0x0004U)
#define LAN8742_PHYSCSR_10BT_FD               ((uint16_t)0x0014U)
#define LAN8742_PHYSCSR_100BTX_HD             ((uint16_t)0x0008U)
#define LAN8742_PHYSCSR_100BTX_FD             ((uint16_t)0x0018U)

/** LAN8742 IT Flags
  */
#define  LAN8742_WOL_IT                        LAN8742_INT_8
#define  LAN8742_ENERGYON_IT                   LAN8742_INT_7
#define  LAN8742_AUTONEGO_COMPLETE_IT          LAN8742_INT_6
#define  LAN8742_REMOTE_FAULT_IT               LAN8742_INT_5
#define  LAN8742_LINK_DOWN_IT                  LAN8742_INT_4
#define  LAN8742_AUTONEGO_LP_ACK_IT            LAN8742_INT_3
#define  LAN8742_PARALLEL_DETECTION_FAULT_IT   LAN8742_INT_2
#define  LAN8742_AUTONEGO_PAGE_RECEIVED_IT     LAN8742_INT_1

/**
  * @brief LAN8742 interrupt source bitmask type.
  *
  * This enum is intended to be used with lan8742_enable_it_source(),
  * lan8742_disable_it_source() and lan8742_get_it_status().
  */
typedef enum
{
  LAN8742_IT_SRC_WOL                       = LAN8742_WOL_IT,
  LAN8742_IT_SRC_ENERGYON                  = LAN8742_ENERGYON_IT,
  LAN8742_IT_SRC_AUTONEGO_COMPLETE         = LAN8742_AUTONEGO_COMPLETE_IT,
  LAN8742_IT_SRC_REMOTE_FAULT             = LAN8742_REMOTE_FAULT_IT,
  LAN8742_IT_SRC_LINK_DOWN                = LAN8742_LINK_DOWN_IT,
  LAN8742_IT_SRC_AUTONEGO_LP_ACK          = LAN8742_AUTONEGO_LP_ACK_IT,
  LAN8742_IT_SRC_PARALLEL_DETECTION_FAULT = LAN8742_PARALLEL_DETECTION_FAULT_IT,
  LAN8742_IT_SRC_AUTONEGO_PAGE_RECEIVED   = LAN8742_AUTONEGO_PAGE_RECEIVED_IT
} lan8742_it_source_t;

/******************************************************************************
  * @brief  LAN8742 MDIO Manageable Device (MMD) Registers
  ****************************************************************************/

/* MMD Device Address */
#define LAN8742_MMD_PCS                       ((uint8_t)0x03U)
#define LAN8742_MMD_VENDOR_SPECIFIC           ((uint8_t)0x1EU)

/* MMD Physical Coding Sublayer (PCS) (Device Address 0x03) Registers */
#define LAN8742_PCS_MMD_DEVICES_PRESENT_1        ((uint16_t)0x0005U)
#define LAN8742_PCS_MMD_DEVICES_PRESENT_2        ((uint16_t)0x0006U)
#define LAN8742_PCS_WUCSR                        ((uint16_t)0x8010U)
#define LAN8742_PCS_WUF_CFGA                     ((uint16_t)0x8011U)
#define LAN8742_PCS_WUF_CFGB                     ((uint16_t)0x8012U)
#define LAN8742_PCS_WUF_MASK_127_112             ((uint16_t)0x8021U)
#define LAN8742_PCS_WUF_MASK_111_96              ((uint16_t)0x8022U)
#define LAN8742_PCS_WUF_MASK_95_80               ((uint16_t)0x8023U)
#define LAN8742_PCS_WUF_MASK_79_64               ((uint16_t)0x8024U)
#define LAN8742_PCS_WUF_MASK_63_48               ((uint16_t)0x8025U)
#define LAN8742_PCS_WUF_MASK_47_32               ((uint16_t)0x8026U)
#define LAN8742_PCS_WUF_MASK_31_16               ((uint16_t)0x8027U)
#define LAN8742_PCS_WUF_MASK_15_0                ((uint16_t)0x8028U)
#define LAN8742_PCS_RX_ADDRA                     ((uint16_t)0x8061U)
#define LAN8742_PCS_RX_ADDRB                     ((uint16_t)0x8062U)
#define LAN8742_PCS_RX_ADDRC                     ((uint16_t)0x8063U)
#define LAN8742_PCS_MCFGR                        ((uint16_t)0x8064U)

/* MMD Vendor Specific (Device Address 0x1E) Registers */
#define LAN8742_VSPEC_MMD1_DEV_ID1               ((uint16_t)0x0002U)
#define LAN8742_VSPEC_MMD1_DEV_ID2               ((uint16_t)0x0003U)
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_1         ((uint16_t)0x0005U)
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_2         ((uint16_t)0x0006U)
#define LAN8742_VSPEC_MMD1_STATUS                ((uint16_t)0x0008U)
#define LAN8742_VSPEC_TDR_MATCH_THRESHOLD        ((uint16_t)0x000BU)
#define LAN8742_VSPEC_TDR_SHORT_OPEN_THRESHOLD   ((uint16_t)0x000CU)
#define LAN8742_VSPEC_MMD1_PACKAGE_ID1           ((uint16_t)0x000EU)
#define LAN8742_VSPEC_MMD1_PACKAGE_ID2           ((uint16_t)0x000FU)

/******************************************************************************
  * @brief  LAN8742 MMD PCS Register Mapping
  ****************************************************************************/

/** LAN8742 PCS MMD Devices Present 1 Register Bit Definition
  */

#define LAN8742_PCS_MMD_DEVICES_PRESENT_1_AUTO_NEG     ((uint16_t)0x0080U)
#define LAN8742_PCS_MMD_DEVICES_PRESENT_1_TC           ((uint16_t)0x0040U)
#define LAN8742_PCS_MMD_DEVICES_PRESENT_1_DTE_XS       ((uint16_t)0x0020U)
#define LAN8742_PCS_MMD_DEVICES_PRESENT_1_PHY_XS       ((uint16_t)0x0010U)
#define LAN8742_PCS_MMD_DEVICES_PRESENT_1_PCS          ((uint16_t)0x0008U)
#define LAN8742_PCS_MMD_DEVICES_PRESENT_1_WIS          ((uint16_t)0x0004U)
#define LAN8742_PCS_MMD_DEVICES_PRESENT_1_PMD_PMA      ((uint16_t)0x0002U)
#define LAN8742_PCS_MMD_DEVICES_PRESENT_1_CLAUSE22     ((uint16_t)0x0001U)

/** LAN8742 PCS MMD Devices Present 2 Register Bit Definition
  */
#define LAN8742_PCS_MMD_DEVICES_PRESENT_2_VSPEC_DEV2   ((uint16_t)0x8000U)
#define LAN8742_PCS_MMD_DEVICES_PRESENT_2_VSPEC_DEV1   ((uint16_t)0x4000U)
#define LAN8742_PCS_MMD_DEVICES_PRESENT_2_CLAUSE22_EXT ((uint16_t)0x2000U)


/** LAN8742 Wakeup Control And Status Register (WUCSR) Bit Definition
  */
#define LAN8742_PCS_WUCSR_INTERFACE_DISABLE            ((uint16_t)0x8000U)
#define LAN8742_PCS_WUCSR_LED1_FUNC_SELECT_MASK        ((uint16_t)0x6000U)
#define LAN8742_PCS_WUCSR_LED1_FUNC_LINK_ACTIVITY      ((uint16_t)0x0000U)
#define LAN8742_PCS_WUCSR_LED1_FUNC_NINT               ((uint16_t)0x2000U)
#define LAN8742_PCS_WUCSR_LED1_FUNC_NPME               ((uint16_t)0x4000U)
#define LAN8742_PCS_WUCSR_LED1_FUNC_LINK_SPEED         ((uint16_t)0x6000U)
#define LAN8742_PCS_WUCSR_LED2_FUNC_SELECT_MASK        ((uint16_t)0x1800U)
#define LAN8742_PCS_WUCSR_LED2_FUNC_LINK_SPEED         ((uint16_t)0x0000U)
#define LAN8742_PCS_WUCSR_LED2_FUNC_NINT               ((uint16_t)0x0800U)
#define LAN8742_PCS_WUCSR_LED2_FUNC_NPME               ((uint16_t)0x1000U)
#define LAN8742_PCS_WUCSR_LED2_FUNC_LINK_ACTIVITY      ((uint16_t)0x1800U)
#define LAN8742_PCS_WUCSR_RESERVED                     ((uint16_t)0x0400U)
#define LAN8742_PCS_WUCSR_NPME_SELF_CLEAR              ((uint16_t)0x0200U)
#define LAN8742_PCS_WUCSR_WOL_CONFIGURED               ((uint16_t)0x0100U)
#define LAN8742_PCS_WUCSR_PFDA_FR                      ((uint16_t)0x0080U)
#define LAN8742_PCS_WUCSR_WUFR                         ((uint16_t)0x0040U)
#define LAN8742_PCS_WUCSR_MPR                          ((uint16_t)0x0020U)
#define LAN8742_PCS_WUCSR_BCAST_FR                     ((uint16_t)0x0010U)
#define LAN8742_PCS_WUCSR_PFDA_EN                      ((uint16_t)0x0008U)
#define LAN8742_PCS_WUCSR_WUEN                         ((uint16_t)0x0004U)

/** LAN8742 Wakeup Filter Configuration Register A (WUF_CFGA) Bit Definition
  */
#define LAN8742_PCS_WUF_CFGA_FILTER_ENABLE             ((uint16_t)0x8000U)
#define LAN8742_PCS_WUF_CFGA_FILTER_TRIGGERED          ((uint16_t)0x4000U)
#define LAN8742_PCS_WUF_CFGA_ADDR_MATCH_ENABLE         ((uint16_t)0x0400U)
#define LAN8742_PCS_WUF_CFGA_FILTER_ANY_MCAST_ENABLE   ((uint16_t)0x0200U)
#define LAN8742_PCS_WUF_CFGA_FILTER_BCAST_ENABLE       ((uint16_t)0x0100U)
#define LAN8742_PCS_WUF_CFGA_PATTERN_OFFSET_MASK       ((uint16_t)0x00FFU)
#define LAN8742_PCS_WUF_CFGA_PATTERN_OFFSET_DEFAULT    ((uint16_t)0x0000U)

/** LAN8742 Wakeup Filter Configuration Register B (WUF_CFGB) Bit Definition
  */
#define LAN8742_PCS_WUF_CFGB_FILTER_CRC                ((uint16_t)0xFFFFU)
#define LAN8742_PCS_WUF_CFGB_FILTER_CRC_DEFAULT        ((uint16_t)0x0000U)

/** LAN8742 Wakeup Filter Byte Mask Registers (WUF_MASK) Bit Definition
  */
#define LAN8742_PCS_WUF_MASK_127_112_MASK              ((uint16_t)0xFFFFU)
#define LAN8742_PCS_WUF_MASK_127_112_DEFAULT           ((uint16_t)0x0000U)
#define LAN8742_PCS_WUF_MASK_111_96_MASK               ((uint16_t)0xFFFFU)
#define LAN8742_PCS_WUF_MASK_111_96_DEFAULT            ((uint16_t)0x0000U)
#define LAN8742_PCS_WUF_MASK_95_80_MASK                ((uint16_t)0xFFFFU)
#define LAN8742_PCS_WUF_MASK_95_80_DEFAULT             ((uint16_t)0x0000U)
#define LAN8742_PCS_WUF_MASK_79_64_MASK                ((uint16_t)0xFFFFU)
#define LAN8742_PCS_WUF_MASK_79_64_DEFAULT             ((uint16_t)0x0000U)
#define LAN8742_PCS_WUF_MASK_63_48_MASK                ((uint16_t)0xFFFFU)
#define LAN8742_PCS_WUF_MASK_63_48_DEFAULT             ((uint16_t)0x0000U)
#define LAN8742_PCS_WUF_MASK_47_32_MASK                ((uint16_t)0xFFFFU)
#define LAN8742_PCS_WUF_MASK_47_32_DEFAULT             ((uint16_t)0x0000U)
#define LAN8742_PCS_WUF_MASK_31_16_MASK                ((uint16_t)0xFFFFU)
#define LAN8742_PCS_WUF_MASK_31_16_DEFAULT             ((uint16_t)0x0000U)
#define LAN8742_PCS_WUF_MASK_15_0_MASK                 ((uint16_t)0xFFFFU)
#define LAN8742_PCS_WUF_MASK_15_0_DEFAULT              ((uint16_t)0x0000U)

/** LAN8742 MAC Receive Address A Register (RX_ADDRA) Bit Definition
  */
#define LAN8742_PCS_RX_ADDRA_PHYS_ADDR_MASK            ((uint16_t)0xFFFFU)
#define LAN8742_PCS_RX_ADDRA_PHYS_ADDR_DEFAULT         ((uint16_t)0xFFFFU)


/** LAN8742 MAC Receive Address B Register (RX_ADDRB) Bit Definition
  */
#define LAN8742_PCS_RX_ADDRB_PHYS_ADDR_MASK            ((uint16_t)0xFFFFU)
#define LAN8742_PCS_RX_ADDRB_PHYS_ADDR_DEFAULT         ((uint16_t)0xFFFFU)

/** LAN8742 MAC Receive Address C Register (RX_ADDRC) Bit Definition
  */
#define LAN8742_PCS_RX_ADDRC_PHYS_ADDR_MASK            ((uint16_t)0xFFFFU)
#define LAN8742_PCS_RX_ADDRC_PHYS_ADDR_DEFAULT         ((uint16_t)0xFFFFU)

/** LAN8742 Miscellaneous Configuration Register (MCFGR) Bit Definition
  */
#define LAN8742_PCS_MCFGR_NPME_ASSERT_DELAY_MASK       ((uint16_t)0xFFFFU)
#define LAN8742_PCS_MCFGR_NPME_ASSERT_DELAY_DEFAULT    ((uint16_t)0x1000U)

/******************************************************************************
  * @brief  LAN8742 MMD Vendor Specific Register Mapping
  ****************************************************************************/

/** LAN8742 Vendor Specific MMD 1 Device ID 1 Register Bit Definition
  *
  * Bits 15:0 : RESERVED (RO, default 0x0000)
  */

/** LAN8742 Vendor Specific MMD 1 Device ID 2 Register Bit Definition
  *
  * Bits 15:0 : RESERVED (RO, default 0x0000)
  */

/** LAN8742 Vendor Specific 1 MMD Devices Present 1 Register Bit Definition
  */
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_1_AUTO_NEG      ((uint16_t)0x0080U)
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_1_TC            ((uint16_t)0x0040U)
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_1_DTE_XS        ((uint16_t)0x0020U)
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_1_PHY_XS        ((uint16_t)0x0010U)
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_1_PCS           ((uint16_t)0x0008U)
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_1_WIS           ((uint16_t)0x0004U)
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_1_PMD_PMA       ((uint16_t)0x0002U)
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_1_CLAUSE22      ((uint16_t)0x0001U)

/** LAN8742 Vendor Specific 1 MMD Devices Present 2 Register Bit Definition
  */
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_2_VSPEC_DEV2    ((uint16_t)0x8000U)
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_2_VSPEC_DEV1    ((uint16_t)0x4000U)
#define LAN8742_VSPEC_MMD1_DEV_PRESENT_2_CLAUSE22_EXT  ((uint16_t)0x2000U)

/** LAN8742 Vendor Specific MMD 1 Status Register Bit Definition
  */
#define LAN8742_VSPEC_MMD1_STATUS_DEVICE_PRESENT_MASK     ((uint16_t)0xC000U)
#define LAN8742_VSPEC_MMD1_STATUS_DEVICE_PRESENT_RESP     ((uint16_t)0x8000U)

/** LAN8742 TDR Match Threshold Register Bit Definition
  */
#define LAN8742_VSPEC_TDR_MATCH_THRESH_MATCH_HIGH_MASK    ((uint16_t)0x03E0U)
#define LAN8742_VSPEC_TDR_MATCH_THRESH_MATCH_HIGH_DEFAULT ((uint16_t)0x0240U)
#define LAN8742_VSPEC_TDR_MATCH_THRESH_MATCH_LOW_MASK     ((uint16_t)0x001FU)
#define LAN8742_VSPEC_TDR_MATCH_THRESH_MATCH_LOW_DEFAULT  ((uint16_t)0x0009U)

/** LAN8742 TDR Short/Open Threshold Register Bit Definition
  */
#define LAN8742_VSPEC_TDR_SHORT_OPEN_SHORT_LOW_MASK       ((uint16_t)0x03E0U)
#define LAN8742_VSPEC_TDR_SHORT_OPEN_SHORT_LOW_DEFAULT    ((uint16_t)0x0120U)
#define LAN8742_VSPEC_TDR_SHORT_OPEN_OPEN_HIGH_MASK       ((uint16_t)0x001FU)
#define LAN8742_VSPEC_TDR_SHORT_OPEN_OPEN_HIGH_DEFAULT    ((uint16_t)0x0012U)

/** LAN8742 Vendor Specific MMD 1 package ID 1 Register Bit Definition
  *
  * Bits 15:0 : RESERVED (RO, default 0x0000)
  */

/** LAN8742 Vendor Specific MMD 1 package ID 2 Register Bit Definition
  *
  * Bits 15:0 : RESERVED (RO, default 0x0000)
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
lan8742_status_t lan8742_init(lan8742_obj_t *pobj, uint32_t dev_id);
lan8742_status_t lan8742_deinit(lan8742_obj_t *pobj);
lan8742_status_t lan8742_get_id(lan8742_obj_t *pobj, uint8_t *id);
lan8742_status_t lan8742_software_reset(lan8742_obj_t *pobj);
lan8742_status_t lan8742_hardware_reset(lan8742_obj_t *pobj);
lan8742_status_t lan8742_get_phy_address(lan8742_obj_t *pobj, uint8_t *paddr);
lan8742_status_t lan8742_set_phy_address(lan8742_obj_t *pobj, uint8_t addr);
lan8742_status_t lan8742_get_link_mode(lan8742_obj_t *pobj, lan8742_link_t *link_mode);
lan8742_status_t lan8742_get_link_config(lan8742_obj_t *pobj, lan8742_link_config_t *pconfig);
lan8742_status_t lan8742_set_link_config(lan8742_obj_t *pobj, const lan8742_link_config_t *pconfig);
lan8742_status_t lan8742_start_link_autonego(lan8742_obj_t *pobj);
lan8742_status_t lan8742_write_c22_reg(lan8742_obj_t *pobj, uint8_t reg, uint16_t value);
lan8742_status_t lan8742_read_c22_reg(lan8742_obj_t *pobj, uint8_t reg, uint16_t *pvalue);
lan8742_status_t lan8742_enable_it_source(lan8742_obj_t *pobj, lan8742_it_source_t it_src);
lan8742_status_t lan8742_disable_it_source(lan8742_obj_t *pobj, lan8742_it_source_t it_src);
int32_t lan8742_get_it_status(lan8742_obj_t *pobj, lan8742_it_source_t it_src);

#if defined (LAN8742_USE_HAL_MUTEX) && (LAN8742_USE_HAL_MUTEX == 1)
lan8742_status_t lan8742_acquire_bus(lan8742_obj_t *pobj);
lan8742_status_t lan8742_release_bus(lan8742_obj_t *pobj);
#endif /* LAN8742_USE_HAL_MUTEX */

#if defined (LAN8742_CALLBACKS) && (LAN8742_CALLBACKS == 1)
lan8742_status_t lan8742_register_event_callback(lan8742_obj_t *pobj, lan8742_callback_t cb,
                                                 void *arg);
#endif /* LAN8742_CALLBACKS */

#ifdef __cplusplus
}
#endif

#endif /* LAN8742_H */
