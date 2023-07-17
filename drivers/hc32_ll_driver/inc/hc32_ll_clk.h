/**
 *******************************************************************************
 * @file  hc32_ll_clk.h
 * @brief This file contains all the functions prototypes of the CLK driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-06-30       CDT             Refine stc_clock_freq_t
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
#ifndef __HC32_LL_CLK_H__
#define __HC32_LL_CLK_H__

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
 * @addtogroup LL_CLK
 * @{
 */

#if (LL_CLK_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup CLK_Global_Types CLK Global Types
 * @{
 */
/**
 * @brief  CLK XTAL configuration structure definition
 */
typedef struct {
    uint8_t u8State;            /*!< The new state of the XTAL.
                                     This parameter can be a value of @ref CLK_XTAL_Config   */
    uint8_t u8Drv;              /*!< The XTAL drive ability.
                                     This parameter can be a value of @ref CLK_XTAL_Config   */
    uint8_t u8Mode;             /*!< The XTAL mode selection osc or exclk.
                                     This parameter can be a value of @ref CLK_XTAL_Config   */
    uint8_t u8SuperDrv;         /*!< The XTAL super drive on or off.
                                     This parameter can be a value of @ref CLK_XTAL_Config   */
    uint8_t u8StableTime;       /*!< The XTAL stable time selection.
                                     This parameter can be a value of @ref CLK_XTAL_Config   */
} stc_clock_xtal_init_t;

/**
 * @brief  CLK XTAL fault detect configuration structure definition
 */
typedef struct {
    uint8_t u8State;            /*!< Specifies the new state of XTALSTD.
                                     This parameter can be a value of @ref CLK_XTALSTD_Config    */
    uint8_t u8Mode;             /*!< Specifies the XTALSTD mode.
                                     This parameter can be a value of @ref CLK_XTALSTD_Config    */
    uint8_t u8Int;              /*!< Specifies the XTALSTD interrupt on or off.
                                     This parameter can be a value of @ref CLK_XTALSTD_Config    */
    uint8_t u8Reset;            /*!< Specifies the XTALSTD reset on or off.
                                     This parameter can be a value of @ref CLK_XTALSTD_Config    */
} stc_clock_xtalstd_init_t;

/**
 * @brief  CLK bus frequency structure definition
 */
typedef struct {
    uint32_t u32SysclkFreq;        /*!< System clock frequency. */
    uint32_t u32HclkFreq;          /*!< Hclk frequency.         */
} stc_clock_freq_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup CLK_Global_Macros CLK Global Macros
 * @{
 */

/**
 * @defgroup CLK_XTAL_Config XTAL Config
 * @{
 */
/**
 * @brief XTAL function config.
 */
#define CLK_XTAL_OFF                    (CMU_XTALCR_XTALSTP)
#define CLK_XTAL_ON                     (0x00U)

/**
 * @brief XTAL driver ability
 * @note
 @verbatim
 *            High      |       Mid       |       Low       |      ULow     |
 *          [20]        |     [10~20)     |     (4~10)      |     [4]       |
 @endverbatim
 */
#define CLK_XTAL_DRV_HIGH               (0x00U << CMU_XTALCFGR_XTALDRV_POS)
#define CLK_XTAL_DRV_MID                (0x01U << CMU_XTALCFGR_XTALDRV_POS)
#define CLK_XTAL_DRV_LOW                (0x02U << CMU_XTALCFGR_XTALDRV_POS)
#define CLK_XTAL_DRV_ULOW               (0x03U << CMU_XTALCFGR_XTALDRV_POS)

/**
 * @brief XTAL super drive on or off
 */
#define CLK_XTAL_SUPDRV_ON              (CMU_XTALCFGR_SUPDRV)
#define CLK_XTAL_SUPDRV_OFF             (0x00U)

/**
 * @brief XTAL mode selection osc or exclk
 */
#define CLK_XTAL_MD_OSC                 (0x00U)
#define CLK_XTAL_MD_EXCLK               (CMU_XTALCFGR_XTALMS)

/**
 * @brief Xtal stable time selection.
 * @note  a cycle of stable counter = a cycle of Xtal
 */
#define CLK_XTAL_STB_2E8                (0x00U)       /*!< 2^8 * Xtal cycle.  */
#define CLK_XTAL_STB_2E9                (0x01U)       /*!< 2^9 * Xtal cycle.  */
#define CLK_XTAL_STB_2E10               (0x02U)       /*!< 2^10 * Xtal cycle. */
#define CLK_XTAL_STB_2E11               (0x03U)       /*!< 2^11 * Xtal cycle. */
#define CLK_XTAL_STB_2E13               (0x04U)       /*!< 2^13 * Xtal cycle. */
#define CLK_XTAL_STB_2E15               (0x05U)       /*!< 2^15 * Xtal cycle. */
#define CLK_XTAL_STB_2E16               (0x06U)       /*!< 2^16 * Xtal cycle. */
#define CLK_XTAL_STB_2E17               (0x07U)       /*!< 2^17 * Xtal cycle. */
/**
 * @}
 */

/**
 * @defgroup CLK_XTALSTD_Config XTALSTD Config
 * @{
 */

/**
 * @brief XTAL error detection on or off
 */
#define CLK_XTALSTD_OFF                 (0x00U)
#define CLK_XTALSTD_ON                  (CMU_XTALSTDCR_XTALSTDE)

/**
 * @brief XTALSTD mode selection
 */
#define CLK_XTALSTD_MD_RST              (CMU_XTALSTDCR_XTALSTDRIS)
#define CLK_XTALSTD_MD_INT              (0x00U)

/**
 * @brief XTALSTD reset on or off
 */
#define CLK_XTALSTD_RST_OFF             (0x00U)
#define CLK_XTALSTD_RST_ON              (CMU_XTALSTDCR_XTALSTDRE)

/**
 * @brief XTALSTD interrupt on or off
 */
#define CLK_XTALSTD_INT_OFF             (0x00U)
#define CLK_XTALSTD_INT_ON              (CMU_XTALSTDCR_XTALSTDIE)
/**
 * @}
 */

/**
 * @defgroup CLK_HRC_Config HRC Config
 * @{
 */
#define CLK_HRC_OFF                     (CMU_HRCCR_HRCSTP)
#define CLK_HRC_ON                      (0x00U)
/**
 * @}
 */

/**
 * @defgroup CLK_HrcFreq_Sel Hrc Freqency Selection
 * @{
 */
#define CLK_HRC_FREQ_32M                (0x00U)   /*!< ICG.HRCFREQS[3] = 0. */
#define CLK_HRC_FREQ_16M                (0x01U)   /*!< ICG.HRCFREQS[3] = 0. */
#define CLK_HRC_FREQ_8M                 (0x02U)   /*!< ICG.HRCFREQS[3] = 0. */
#define CLK_HRC_FREQ_4M                 (0x03U)   /*!< ICG.HRCFREQS[3] = 0. */
#define CLK_HRC_FREQ_2M                 (0x04U)   /*!< ICG.HRCFREQS[3] = 0. */
#define CLK_HRC_FREQ_1M                 (0x05U)   /*!< ICG.HRCFREQS[3] = 0. */
#define CLK_HRC_FREQ_48M                (0x00U)   /*!< ICG.HRCFREQS[3] = 1. */
#define CLK_HRC_FREQ_24M                (0x01U)   /*!< ICG.HRCFREQS[3] = 1. */
#define CLK_HRC_FREQ_12M                (0x02U)   /*!< ICG.HRCFREQS[3] = 1. */
#define CLK_HRC_FREQ_6M                 (0x03U)   /*!< ICG.HRCFREQS[3] = 1. */
#define CLK_HRC_FREQ_3M                 (0x04U)   /*!< ICG.HRCFREQS[3] = 1. */
#define CLK_HRC_FREQ_1P5M               (0x05U)   /*!< ICG.HRCFREQS[3] = 1. */
/**
 * @}
 */

/**
 * @defgroup CLK_STB_Flag CLK Stable Flags
 * @{
 */
#define CLK_STB_FLAG_HRC                (CMU_OSCSTBSR_HRCSTBF)
#define CLK_STB_FLAG_XTAL               (CMU_OSCSTBSR_XTALSTBF)
#define CLK_STB_FLAG_MASK               (CMU_OSCSTBSR_HRCSTBF | CMU_OSCSTBSR_XTALSTBF)
/**
 * @}
 */

/**
 * @defgroup CLK_System_Clock_Source System Clock Source
 * @{
 */
#define CLK_SYSCLK_SRC_HRC              (0x00U)
#define CLK_SYSCLK_SRC_XTAL             (0x01U)
#define CLK_SYSCLK_SRC_LRC              (0x02U)
/**
 * @}
 */

/**
 * @defgroup CLK_Bus_Clock_Sel Clock Bus Clock Category Selection
 * @{
 */
#define CLK_BUS_HCLK                    (CMU_SCKDIVR_SCKDIV)
#define CLK_BUS_CLK_ALL                 (CLK_BUS_HCLK)
/**
 * @}
 */

/**
 * @defgroup CLK_Clock_Divider Clock Divider
 * @{
 */

/**
 * @defgroup CLK_System_Clock_Divider System Clock Divider
 * @{
 */
#define CLK_SYSCLK_DIV1                 (0x00U)
#define CLK_SYSCLK_DIV2                 (0x01U)
#define CLK_SYSCLK_DIV4                 (0x02U)
#define CLK_SYSCLK_DIV8                 (0x03U)
#define CLK_SYSCLK_DIV16                (0x04U)
#define CLK_SYSCLK_DIV32                (0x05U)
#define CLK_SYSCLK_DIV64                (0x06U)
/**
 * @}
 */

/**
 * @defgroup CLK_HCLK_Divider CLK HCLK Divider
 * @{
 */
#define CLK_HCLK_DIV1                   (CLK_SYSCLK_DIV1)
#define CLK_HCLK_DIV2                   (CLK_SYSCLK_DIV2)
#define CLK_HCLK_DIV4                   (CLK_SYSCLK_DIV4)
#define CLK_HCLK_DIV8                   (CLK_SYSCLK_DIV8)
#define CLK_HCLK_DIV16                  (CLK_SYSCLK_DIV16)
#define CLK_HCLK_DIV32                  (CLK_SYSCLK_DIV32)
#define CLK_HCLK_DIV64                  (CLK_SYSCLK_DIV64)
/**
 * @}
 */

/**
 * @}
 */

/**
 * @defgroup CLK_PERIPH_Sel CLK Peripheral Clock Selection
 * @note    ADC,I2S,DAC,TRNG
 * @{
 */
#define CLK_PERIPHCLK_HCLK_DIV1         (0x00U)
#define CLK_PERIPHCLK_HCLK_DIV2         (0x01U)
#define CLK_PERIPHCLK_HCLK_DIV4         (0x02U)
#define CLK_PERIPHCLK_HCLK_DIV8         (0x03U)
#define CLK_PERIPHCLK_HCLK_DIV16        (0x04U)
#define CLK_PERIPHCLK_HCLK_DIV32        (0x05U)
/**
 * @}
 */

/**
 * @defgroup CLK_MCO_Channel_Sel CLK MCO Channel Select
 * @{
 */
#define CLK_MCO1                        (0x00U)
/**
 * @}
 */

/**
 * @defgroup CLK_MCO_Clock_Source CLK MCO Clock Source
 * @{
 */
#define CLK_MCO_SRC_HRC                 (0x00U)
#define CLK_MCO_SRC_LRC                 (0x01U)
#define CLK_MCO_SRC_XTAL                (0x02U)
#define CLK_MCO_SRC_HCLK                (0x04U)
/**
 * @}
 */

/**
 * @defgroup CLK_MCO_Clock_Prescaler CLK MCO Clock Prescaler
 * @{
 */
#define CLK_MCO_DIV1                    (0x00U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV2                    (0x01U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV4                    (0x02U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV8                    (0x03U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV16                   (0x04U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV32                   (0x05U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV64                   (0x06U << CMU_MCOCFGR_MCODIV_POS)
#define CLK_MCO_DIV128                  (0x07U << CMU_MCOCFGR_MCODIV_POS)
/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup CLK_Global_Functions
 * @{
 */
int32_t CLK_HrcFreqConfig(uint8_t u8HrcFreq);
int32_t CLK_HrcCmd(en_functional_state_t enNewState);
int32_t CLK_LrcCmd(en_functional_state_t enNewState);

int32_t CLK_XtalStructInit(stc_clock_xtal_init_t *pstcXtalInit);
int32_t CLK_XtalInit(const stc_clock_xtal_init_t *pstcXtalInit);
int32_t CLK_XtalCmd(en_functional_state_t enNewState);

int32_t CLK_XtalStdStructInit(stc_clock_xtalstd_init_t *pstcXtalStdInit);
int32_t CLK_XtalStdInit(const stc_clock_xtalstd_init_t *pstcXtalStdInit);
void CLK_ClearXtalStdStatus(void);
en_flag_status_t CLK_GetXtalStdStatus(void);

void CLK_MCOConfig(uint8_t u8Ch, uint8_t u8Src, uint8_t u8Div);
void CLK_MCOCmd(uint8_t u8Ch, en_functional_state_t enNewState);

en_flag_status_t CLK_GetStableStatus(uint8_t u8Flag);
void CLK_SetSysClockSrc(uint8_t u8Src);
void CLK_SetClockDiv(uint32_t u32Clock, uint32_t u32Div);
int32_t CLK_GetClockFreq(stc_clock_freq_t *pstcClockFreq);
uint32_t CLK_GetBusClockFreq(uint32_t u32Clock);

void CLK_SetPeriClockSrc(uint16_t u16Src);

/**
 * @}
 */

#endif /* LL_CLK_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32_LL_CLK_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
