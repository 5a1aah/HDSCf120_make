/**
 *******************************************************************************
 * @file    hc32_ll_icg.h
 * @brief   This file contains all the Macro Definitions of the ICG driver
 *          library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */
#ifndef __HC32_LL_ICG_H__
#define __HC32_LL_ICG_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_def.h"

#include "hc32f1xx.h"
#include "hc32f1xx_conf.h"

/**
 * @addtogroup LL_Driver
 * @{
 */

/**
 * @addtogroup LL_ICG
 * @{
 */

#if (LL_ICG_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup ICG_Global_Macros ICG Global Macros
 * @{
 */

/**
 * @defgroup ICG_SWDT_Reset_State ICG SWDT Reset State
 * @{
 */
#define ICG_SWDT_RST_START                      (0UL)                   /*!< SWDT auto start after reset */
#define ICG_SWDT_RST_STOP                       (ICG_ICG0_SWDTAUTS)     /*!< SWDT stop after reset       */
/**
 * @}
 */

/**
 * @defgroup ICG_SWDT_Exception_Type ICG SWDT Exception Type
 * @{
 */
#define ICG_SWDT_EXP_TYPE_INT                   (0UL)               /*!< SWDT trigger interrupt */
#define ICG_SWDT_EXP_TYPE_RST                   (ICG_ICG0_SWDTITS)  /*!< SWDT trigger reset     */
/**
 * @}
 */

/**
 * @defgroup ICG_SWDT_Count_Period ICG SWDT Count Period
 * @{
 */
#define ICG_SWDT_CNT_PERIOD256                  (0UL)                   /*!< 256 clock cycle   */
#define ICG_SWDT_CNT_PERIOD4096                 (ICG_ICG0_SWDTPERI_0)   /*!< 4096 clock cycle  */
#define ICG_SWDT_CNT_PERIOD16384                (ICG_ICG0_SWDTPERI_1)   /*!< 16384 clock cycle */
#define ICG_SWDT_CNT_PERIOD65536                (ICG_ICG0_SWDTPERI)     /*!< 65536 clock cycle */
/**
 * @}
 */

/**
 * @defgroup ICG_SWDT_Clock_Division ICG SWDT Clock Division
 * @{
 */
#define ICG_SWDT_CLK_DIV1                       (0UL)                               /*!< CLK      */
#define ICG_SWDT_CLK_DIV16                      (0x04UL << ICG_ICG0_SWDTCKS_POS)    /*!< CLK/16   */
#define ICG_SWDT_CLK_DIV32                      (0x05UL << ICG_ICG0_SWDTCKS_POS)    /*!< CLK/32   */
#define ICG_SWDT_CLK_DIV64                      (0x06UL << ICG_ICG0_SWDTCKS_POS)    /*!< CLK/64   */
#define ICG_SWDT_CLK_DIV128                     (0x07UL << ICG_ICG0_SWDTCKS_POS)    /*!< CLK/128  */
#define ICG_SWDT_CLK_DIV256                     (0x08UL << ICG_ICG0_SWDTCKS_POS)    /*!< CLK/256  */
#define ICG_SWDT_CLK_DIV2048                    (0x0BUL << ICG_ICG0_SWDTCKS_POS)    /*!< CLK/2048 */
/**
 * @}
 */

/**
 * @defgroup ICG_SWDT_Refresh_Range ICG SWDT Refresh Range
 * @{
 */
#define ICG_SWDT_RANGE_0TO25PCT                 (0x01UL << ICG_ICG0_SWDTWDPT_POS)   /*!< 0%~25%             */
#define ICG_SWDT_RANGE_25TO50PCT                (0x02UL << ICG_ICG0_SWDTWDPT_POS)   /*!< 25%~50%            */
#define ICG_SWDT_RANGE_0TO50PCT                 (0x03UL << ICG_ICG0_SWDTWDPT_POS)   /*!< 0%~50%             */
#define ICG_SWDT_RANGE_50TO75PCT                (0x04UL << ICG_ICG0_SWDTWDPT_POS)   /*!< 50%~75%            */
#define ICG_SWDT_RANGE_0TO25PCT_50TO75PCT       (0x05UL << ICG_ICG0_SWDTWDPT_POS)   /*!< 0%~25% & 50%~75%   */
#define ICG_SWDT_RANGE_25TO75PCT                (0x06UL << ICG_ICG0_SWDTWDPT_POS)   /*!< 25%~75%            */
#define ICG_SWDT_RANGE_0TO75PCT                 (0x07UL << ICG_ICG0_SWDTWDPT_POS)   /*!< 0%~75%             */
#define ICG_SWDT_RANGE_75TO100PCT               (0x08UL << ICG_ICG0_SWDTWDPT_POS)   /*!< 75%~100%           */
#define ICG_SWDT_RANGE_0TO25PCT_75TO100PCT      (0x09UL << ICG_ICG0_SWDTWDPT_POS)   /*!< 0%~25% & 75%~100%  */
#define ICG_SWDT_RANGE_25TO50PCT_75TO100PCT     (0x0AUL << ICG_ICG0_SWDTWDPT_POS)   /*!< 25%~50% & 75%~100% */
#define ICG_SWDT_RANGE_0TO50PCT_75TO100PCT      (0x0BUL << ICG_ICG0_SWDTWDPT_POS)   /*!< 0%~50% & 75%~100%  */
#define ICG_SWDT_RANGE_50TO100PCT               (0x0CUL << ICG_ICG0_SWDTWDPT_POS)   /*!< 50%~100%           */
#define ICG_SWDT_RANGE_0TO25PCT_50TO100PCT      (0x0DUL << ICG_ICG0_SWDTWDPT_POS)   /*!< 0%~25% & 50%~100%  */
#define ICG_SWDT_RANGE_25TO100PCT               (0x0EUL << ICG_ICG0_SWDTWDPT_POS)   /*!< 25%~100%           */
#define ICG_SWDT_RANGE_0TO100PCT                (0x0FUL << ICG_ICG0_SWDTWDPT_POS)   /*!< 0%~100%            */
/**
 * @}
 */

/**
 * @defgroup ICG_SWDT_LPM_Count ICG SWDT Low Power Mode Count
 * @brief    Counting control of SWDT in sleep/stop mode
 * @{
 */
#define ICG_SWDT_LPM_CNT_CONTINUE               (0UL)                   /*!< Continue counting in sleep/stop mode */
#define ICG_SWDT_LPM_CNT_STOP                   (ICG_ICG0_SWDTSLPOFF)   /*!< Stop counting in sleep/stop mode     */
/**
 * @}
 */

/**
 * @defgroup ICG_NMI_Pin_Filter_Clock_Division ICG NMI Pin Filter Clock Division
 * @{
 */
#define REDEF_ICG_NMIFCLK_POS                   ICG_ICG0_NMIFCLK_POS

#define ICG_NMI_PIN_FILTER_CLK_DIV1             (0UL)                               /*!< CLK    */
#define ICG_NMI_PIN_FILTER_CLK_DIV8             (0x01UL << REDEF_ICG_NMIFCLK_POS)   /*!< CLK/8  */
#define ICG_NMI_PIN_FILTER_CLK_DIV32            (0x02UL << REDEF_ICG_NMIFCLK_POS)   /*!< CLK/32 */
#define ICG_NMI_PIN_FILTER_CLK_DIV64            (0x03UL << REDEF_ICG_NMIFCLK_POS)   /*!< CLK/64 */
/**
 * @}
 */

/**
 * @defgroup ICG_NMI_Pin_Filter ICG NMI Pin Filter
 * @{
 */
#define ICG_NMI_PIN_FILTER_DISABLE              (0UL)               /*!< Disable NMI Pin filter */
#define ICG_NMI_PIN_FILTER_ENABLE               (ICG_ICG0_NMIFEN)   /*!< Enable NMI Pin filter  */
/**
 * @}
 */

/**
 * @defgroup ICG_NMI_Pin_Trigger_Edge ICG NMI Pin Trigger Edge
 * @{
 */
#define ICG_NMI_PIN_TRIG_EDGE_FALLING           (0UL)               /*!< Falling edge trigger */
#define ICG_NMI_PIN_TRIG_EDGE_RISING            (ICG_ICG0_NMITRG)   /*!< Rising edge trigger  */
/**
 * @}
 */

/**
 * @defgroup ICG_NMI_Pin_Interrupt ICG NMI Pin Interrupt
 * @{
 */
#define ICG_NMI_PIN_INT_DISABLE                 (0UL)               /*!< Disable NMI pin interrupt */
#define ICG_NMI_PIN_INT_ENABLE                  (ICG_ICG0_NMIEN)    /*!< Enable NMI pin interrupt  */
/**
 * @}
 */

/**
 * @defgroup ICG_NMI_Pin_Reset_State ICG NMI Pin Reset State
 * @{
 */
#define ICG_NMI_PIN_RST_ENABLE                  (0UL)                   /*!< Enable NMI pin after reset  */
#define ICG_NMI_PIN_RST_DISABLE                 (ICG_ICG0_NMIICGEN)     /*!< Disable NMI pin after reset */
/**
 * @}
 */

/**
 * @defgroup ICG_HRC_Frequency_Select ICG HRC Frequency Select
 * @{
 */
#define REDEF_ICG_HRCFRQSEL_POS                 ICG_ICG0_HRCREQS_POS

#define ICG_HRC_1M                              (0x07UL << REDEF_ICG_HRCFRQSEL_POS) /*!< HRC = 1MHZ   */
#define ICG_HRC_1P5M                            (0x0FUL << REDEF_ICG_HRCFRQSEL_POS) /*!< HRC = 1.5MHZ */
#define ICG_HRC_2M                              (0x04UL << REDEF_ICG_HRCFRQSEL_POS) /*!< HRC = 2MHZ   */
#define ICG_HRC_3M                              (0x0CUL << REDEF_ICG_HRCFRQSEL_POS) /*!< HRC = 3MHZ   */
#define ICG_HRC_4M                              (0x03UL << REDEF_ICG_HRCFRQSEL_POS) /*!< HRC = 4MHZ   */
#define ICG_HRC_6M                              (0x0BUL << REDEF_ICG_HRCFRQSEL_POS) /*!< HRC = 6MHZ   */
#define ICG_HRC_8M                              (0x02UL << REDEF_ICG_HRCFRQSEL_POS) /*!< HRC = 8MHZ   */
#define ICG_HRC_12M                             (0x0AUL << REDEF_ICG_HRCFRQSEL_POS) /*!< HRC = 12MHZ  */
#define ICG_HRC_16M                             (0x01UL << REDEF_ICG_HRCFRQSEL_POS) /*!< HRC = 16MHZ  */
#define ICG_HRC_24M                             (0x09UL << REDEF_ICG_HRCFRQSEL_POS) /*!< HRC = 24MHZ  */
#define ICG_HRC_32M                             (0UL)                               /*!< HRC = 32MHZ  */
#define ICG_HRC_48M                             (0x08UL << REDEF_ICG_HRCFRQSEL_POS) /*!< HRC = 48MHZ  */
/**
 * @}
 */

/**
 * @defgroup ICG_LVD_Voltage_Threshold ICG LVD Voltage Threshold
 * @{
 */
#define ICG_LVD_THRESHOLD_LVL0                  (0UL)                               /*!< LVD voltage threshold less than 3.92 or higher than 4.07 */
#define ICG_LVD_THRESHOLD_LVL1                  (0x01UL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold less than 3.67 or higher than 3.77 */
#define ICG_LVD_THRESHOLD_LVL2                  (0x02UL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold less than 3.06 or higher than 3.15 */
#define ICG_LVD_THRESHOLD_LVL3                  (0x03UL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold less than 2.96 or higher than 3.04 */
#define ICG_LVD_THRESHOLD_LVL4                  (0x04UL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold less than 2.86 or higher than 2.94 */
#define ICG_LVD_THRESHOLD_LVL5                  (0x05UL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold less than 2.75 or higher than 2.83 */
#define ICG_LVD_THRESHOLD_LVL6                  (0x06UL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold less than 2.65 or higher than 2.73 */
#define ICG_LVD_THRESHOLD_LVL7                  (0x07UL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold less than 2.55 or higher than 2.63 */
#define ICG_LVD_THRESHOLD_LVL8                  (0x08UL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold less than 2.45 or higher than 2.52 */
#define ICG_LVD_THRESHOLD_LVL9                  (0x09UL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold less than 2.04 or higher than 2.11 */
#define ICG_LVD_THRESHOLD_LVL10                 (0x0AUL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold less than 1.94 or higher than 2.00 */
#define ICG_LVD_THRESHOLD_LVL11                 (0x0BUL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold less than 1.84 or higher than 1.90 */
#define ICG_LVD_EXT_INPUT                       (0x0EUL << ICG_ICG1_LVDLVL_POS)     /*!< LVD voltage threshold reference external input           */
/**
 * @}
 */

/**
 * @defgroup ICG_LVD_Reset_State ICG LVD Reset State
 * @{
 */
#define ICG_LVD_RST_ENABLE                      (0UL)               /*!< Enable LVD after reset  */
#define ICG_LVD_RST_DISABLE                     (ICG_ICG1_LVDDIS)   /*!< Disable LVD after reset */
/**
 * @}
 */

/**
 * @defgroup ICG_LVD_Filter_Period ICG LVD Filter Period
 * @{
 */
#define ICG_LVD_FILTER_PERIOD_2LRC              (0UL)               /*!< 2 LRC clock cycle  */
#define ICG_LVD_FILTER_PERIOD_4LRC              (ICG_ICG1_DFS_0)    /*!< 4 LRC clock cycle  */
#define ICG_LVD_FILTER_PERIOD_8LRC              (ICG_ICG1_DFS_1)    /*!< 8 LRC clock cycle  */
#define ICG_LVD_FILTER_PERIOD_16LRC             (ICG_ICG1_DFS)      /*!< 16 LRC clock cycle */
/**
 * @}
 */

/**
 * @defgroup ICG_LVD_Filter_Reset_State ICG LVD Filter Reset State
 * @{
 */
#define ICG_LVD_FILTER_RST_ENABLE               (0UL)               /*!< Enable LVD filter after reset  */
#define ICG_LVD_FILTER_RST_DISABLE              (ICG_ICG1_DFDIS)    /*!< Disable LVD filter after reset */
/**
 * @}
 */

/**
 * @defgroup ICG_LVD_Interrupt_Type ICG LVD Interrupt Type
 * @{
 */
#define ICG_LVD_INT_TYPE_MASK_INT               (0UL)               /*!< LVD choose maskable interrupt     */
#define ICG_LVD_INT_TYPE_NMI                    (ICG_ICG1_NMIS)     /*!< LVD choose non-maskable interrupt */
/**
 * @}
 */

/**
 * @defgroup ICG_LVD_Exception_Type ICG LVD Exception Type
 * @{
 */
#define ICG_LVD_EXP_TYPE_INT                    (0UL)               /*!< LVD trigger interrupt */
#define ICG_LVD_EXP_TYPE_RST                    (ICG_ICG1_IRS)      /*!< LVD trigger reset     */
/**
 * @}
 */

/**
 * @defgroup ICG_LVD_Exception_State ICG LVD Exception State
 * @{
 */
#define ICG_LVD_EXP_ENABLE                      (0UL)               /*!< Enable LVD exception after reset (interrupt or reset)  */
#define ICG_LVD_EXP_DISABLE                     (ICG_ICG1_IRDIS)    /*!< Disable LVD exception after reset (interrupt or reset) */
/**
 * @}
 */

/**
 * @}
 */

/**
 * @defgroup ICG_Register_Configuration ICG Register Configuration
 * @{
 */

/**
 * @defgroup ICG_SWDT_Preload_Configuration ICG SWDT Preload Configuration
 * @{
 */
/* SWDT register config */
#define ICG_RB_SWDT_AUTS                        (ICG_SWDT_RST_STOP)
#define ICG_RB_SWDT_ITS                         (ICG_SWDT_EXP_TYPE_RST)
#define ICG_RB_SWDT_PERI                        (ICG_SWDT_CNT_PERIOD65536)
#define ICG_RB_SWDT_CKS                         (ICG_SWDT_CLK_DIV2048)
#define ICG_RB_SWDT_WDPT                        (ICG_SWDT_RANGE_0TO100PCT)
#define ICG_RB_SWDT_SLTPOFF                     (ICG_SWDT_LPM_CNT_STOP)

/* SWDT register value */
#define ICG_REG_SWDT_CONFIG                     (ICG_RB_SWDT_AUTS | ICG_RB_SWDT_ITS  | ICG_RB_SWDT_PERI | \
                                                 ICG_RB_SWDT_CKS  | ICG_RB_SWDT_WDPT | ICG_RB_SWDT_SLTPOFF)
/**
 * @}
 */

/**
 * @defgroup ICG_NMI_Pin_Preload_Configuration ICG NMI Pin Preload Configuration
 * @{
 */
/* NMI register config */
#define ICG_RB_NMI_FCLK                         (ICG_NMI_PIN_FILTER_CLK_DIV64)
#define ICG_RB_NMI_FEN                          (ICG_NMI_PIN_FILTER_ENABLE)
#define ICG_RB_NMI_TRG                          (ICG_NMI_PIN_TRIG_EDGE_RISING)
#define ICG_RB_NMI_EN                           (ICG_NMI_PIN_INT_ENABLE)
#define ICG_RB_NMI_ICGEN                        (ICG_NMI_PIN_RST_DISABLE)

/* NMI register value */
#define ICG_REG_NMI_CONFIG                      (ICG_RB_NMI_FCLK | ICG_RB_NMI_FEN | ICG_RB_NMI_TRG | \
                                                 ICG_RB_NMI_EN   | ICG_RB_NMI_ICGEN)
/**
 * @}
 */

/**
 * @defgroup ICG_HRC_Preload_Configuration ICG HRC Preload Configuration
 * @{
 */
/* HRC register config */
#define ICG_RB_HRC_HRCREQS                      (ICG_HRC_1P5M)

/* HRC register value */
#define ICG_REG_HRC_CONFIG                      (ICG_RB_HRC_HRCREQS)
/**
 * @}
 */

/**
 * @defgroup ICG_LVD_Preload_Configuration ICG LVD Preload Configuration
 * @{
 */
/* LVD register config */
#define ICG_RB_LVD_DFS                          (ICG_LVD_FILTER_PERIOD_16LRC)
#define ICG_RB_LVD_DFDIS                        (ICG_LVD_FILTER_RST_DISABLE)
#define ICG_RB_LVD_LVL                          (ICG_LVD_THRESHOLD_LVL11)
#define ICG_RB_LVD_NMIS                         (ICG_LVD_INT_TYPE_NMI)
#define ICG_RB_LVD_IRS                          (ICG_LVD_EXP_TYPE_RST)
#define ICG_RB_LVD_IRDIS                        (ICG_LVD_EXP_DISABLE)
#define ICG_RB_LVD_DIS                          (ICG_LVD_RST_DISABLE)

/* LVD register value */
#define ICG_REG_LVD_CONFIG                      (ICG_RB_LVD_DFS  | ICG_RB_LVD_DFDIS | ICG_RB_LVD_LVL   | \
                                                 ICG_RB_LVD_NMIS | ICG_RB_LVD_IRS   | ICG_RB_LVD_IRDIS | \
                                                 ICG_RB_LVD_DIS)
/**
 * @}
 */

/**
 * @}
 */

/**
 * @defgroup ICG_Register_Value ICG Register Value
 * @{
 */
/* ICG register value */
#ifndef ICG_REG_CFG0_CONST
#define ICG_REG_CFG0_CONST                      (ICG_REG_NMI_CONFIG | ICG_REG_HRC_CONFIG | ICG_REG_SWDT_CONFIG | 0x03F0E000UL)
#endif
#ifndef ICG_REG_CFG1_CONST
#define ICG_REG_CFG1_CONST                      (ICG_REG_LVD_CONFIG | 0xFFFF00F8UL)
#endif
/* ICG reserved value */
#define ICG_REG_RESV_CONST                      (0xFFFFFFFFUL)

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/

#endif /* LL_ICG_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32_LL_ICG_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
