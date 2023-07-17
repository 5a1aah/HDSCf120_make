/**
 *******************************************************************************
 * @file  hc32_ll_pwc.h
 * @brief This file contains all the functions prototypes of the PWC driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-06-30       CDT             Refine API PWC_STOP_Enter().
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
#ifndef __HC32_LL_PWC_H__
#define __HC32_LL_PWC_H__

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
 * @addtogroup LL_PWC
 * @{
 */

#if (LL_PWC_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup PWC_Global_Types PWC Global Types
 * @{
 */
/**
 * @brief PWC LVD Init
 */
typedef struct {
    uint32_t u32State;              /*!< LVD function setting, @ref PWC_LVD_Config for details */
    uint32_t u32CompareOutputState; /*!< LVD compare output function setting, @ref PWC_LVD_CMP_Config for details */
    uint32_t u32ExceptionType;      /*!< LVD interrupt or reset selection, @ref PWC_LVD_Exception_Type_Sel for details */
    uint32_t u32Filter;             /*!< LVD digital filter function setting, @ref PWC_LVD_DF_Config for details */
    uint32_t u32FilterClock;        /*!< LVD digital filter clock setting, @ref PWC_LVD_DFS_Clk_Sel for details */
    uint32_t u32ThresholdVoltage;   /*!< LVD detect voltage setting, @ref PWC_LVD_Detection_Voltage_Sel for details */
} stc_pwc_lvd_init_t;

/**
 * @brief PWC Stop mode Init
 */
typedef struct {
    uint8_t u8Clock;            /*!< System clock setting after wake-up from stop mode,
                                    @ref PWC_STOP_CLK_Sel for details.        */
    uint8_t u8HighPrecisionPOR; /*!< Specifies the high precision POR(power on reset) on or off while stop mode.
                                    @ref PWC_HPOR_Config for details.         */
} stc_pwc_stop_mode_config_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup PWC_Global_Macros PWC Global Macros
 * @{
 */

/**
 * @defgroup PWC_HPOR_Config PWC High Precision POR(power on reset) Config
 * @{
 */
#define PWC_HIGH_PRECISION_POR_ON       (0x00U)                 /*!< High precision POR is valid while in stop mode   */
#define PWC_HIGH_PRECISION_POR_OFF      (PWC_STPMCR_HAPORDIS)   /*!< High precision POR is invalid while in stop mode */
/**
 * @}
 */

/**
 * @defgroup PWC_STOP_CLK_Sel System clock setting after wake-up from stop mode
 * @{
 */
#define PWC_STOP_CLK_KEEP               (0x00U)                 /*!< Keep System clock setting after wake-up from stop mode */
#define PWC_STOP_CLK_HRC                (PWC_STPMCR_CKSHRC)     /*!< System clock switch to HRC after wake-up from stop mode */
/**
 * @}
 */

/**
 * @defgroup PWC_Stop_Type PWC stop mode type.
 * @{
 */
#define PWC_STOP_WFI                    (0x00U)
#define PWC_STOP_WFE                    (0x01U)
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_Channel PWC LVD channel
 * @{
 */
#define PWC_LVD_CH1                     (0x00U)

/**
 * @}
 */

/**
 * @defgroup PWC_LVD_Config PWC LVD Config
 * @{
 */
#define PWC_LVD_ON                      (0x00U)
#define PWC_LVD_OFF                     (EFM_LVDICGCR_LVDDIS)
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_Exception_Type_Sel PWC LVD Exception Type Select
 * @{
 */
#define PWC_LVD_EXP_TYPE_NONE           (EFM_LVDICGCR_IRDIS)
#define PWC_LVD_EXP_TYPE_INT            (0x00U)
#define PWC_LVD_EXP_TYPE_NMI            (EFM_LVDICGCR_NMIS)
#define PWC_LVD_EXP_TYPE_RST            (EFM_LVDICGCR_IRS)
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_CMP_Config PWC LVD Compare Config
 * @{
 */
#define PWC_LVD_CMP_OFF                 (0x00U)
#define PWC_LVD_CMP_ON                  (PWC_LVDCSR_CMPOE)
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_DF_Config LVD digital filter ON or OFF
 * @{
 */
#define PWC_LVD_FILTER_ON               (0x00UL << EFM_LVDICGCR_DFDIS_POS)
#define PWC_LVD_FILTER_OFF              (0x01UL << EFM_LVDICGCR_DFDIS_POS)
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_DFS_Clk_Sel LVD digital filter sample ability
 * @note     modified this value must when PWC_LVD_FILTER_OFF
 * @{
 */
#define PWC_LVD_FILTER_LRC_MUL2        (0x00U)                                 /*!< 2 LRC cycle    */
#define PWC_LVD_FILTER_LRC_MUL4        (0x01U)                                 /*!< 4 LRC cycle    */
#define PWC_LVD_FILTER_LRC_MUL8        (0x02U)                                 /*!< 8 LRC cycle    */
#define PWC_LVD_FILTER_LRC_MUL16       (0x03U)                                 /*!< 16 LRC cycles  */
/**
 * @}
 */

/**
 * @defgroup PWC_LVD_Detection_Voltage_Sel PWC LVD Detection voltage
 * @{
 * @note
 * @verbatim
 * LVL0  |    3.92V ~ 4.07V     ||
 * LVL1  |    3.67V ~ 3.77V     ||
 * LVL2  |    3.06V ~ 3.15V     ||
 * LVL3  |    2.96V ~ 3.04V     ||
 * LVL4  |    2.86V ~ 2.94V     ||
 * LVL5  |    2.75V ~ 2.83V     ||
 * LVL6  |    2.65V ~ 2.73V     ||
 * LVL7  |    2.55V ~ 2.63V     ||
 * LVL8  |    2.45V ~ 2.52V     ||
 * LVL9  |    2.04V ~ 2.11V     ||
 * LVL10 |    1.94V ~ 2.00V     ||
 * LVL11 |    1.84V ~ 1.90V     ||
 * EXVCC |        EXVCC         ||
 * @endverbatim
 */
#define PWC_LVD_THRESHOLD_LVL0          (0x00U << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_THRESHOLD_LVL1          (0x01U << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_THRESHOLD_LVL2          (0x02U << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_THRESHOLD_LVL3          (0x03U << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_THRESHOLD_LVL4          (0x04U << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_THRESHOLD_LVL5          (0x05U << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_THRESHOLD_LVL6          (0x06U << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_THRESHOLD_LVL7          (0x07U << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_THRESHOLD_LVL8          (0x08U << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_THRESHOLD_LVL9          (0x09U << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_THRESHOLD_LVL10         (0x0AU << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_THRESHOLD_LVL11         (0x0BU << EFM_LVDICGCR_LVDLVL_POS)
#define PWC_LVD_EXTVCC                  (0x0EU << EFM_LVDICGCR_LVDLVL_POS)

/**
 * @}
 */

/**
 * @defgroup PWC_LVD_Flag LVD flag
 * @{
 */
#define PWC_LVD1_FLAG_DETECT            (PWC_LVDCSR_DETF)                /*!< VCC across VLVD    */
#define PWC_LVD1_FLAG_MON               (PWC_LVDCSR_LVIF)                /*!< VCC < VLVD         */

/**
 * @}
 */

/**
 * @defgroup PWC_Monitor_Power PWC Power Monitor voltage definition
 * @{
 */
#define PWC_PWR_MON_IREF                (0x00U)                 /*!< Internal reference voltage */
#define PWC_PWR_MON_TSENSOR             (PWC_PWRC_PWMONSEL)     /*!< temperature sensor voltage */
/**
 * @}
 */

/**
 * @defgroup PWC_Ram_Protect_Area_Sel PWC ram protect area selection
 * @{
 */
#define PWC_RAM_PROTECT_AREA_NONE       (0x00U)
#define PWC_RAM_PROTECT_AREA_128BYTE    (0x01U)
#define PWC_RAM_PROTECT_AREA_256BYTE    (0x02U)
#define PWC_RAM_PROTECT_AREA_512BYTE    (0x03U)
/**
 * @}
 */

/**
 * @defgroup PWC_Ram_Parity_Reset_State PWC Ram Parity Reset State
 * @{
 */
#define PWC_RAM_PARITY_RST_ON           (0x00)              /*!< PWC Ram parity reset on */
#define PWC_RAM_PARITY_RST_OFF          (PWC_RAMCR_RPERDIS) /*!< PWC Ram parity reset off */
/**
 * @}
 */

/**
 * @defgroup PWC_Ram_Parity_Error_Flag PWC Ram Parity Error Flag
 * @{
 */
#define PWC_FLAG_RAM_PARITY_ERR         (PWC_RAMCR_RPEF)        /*!< RAM parity error flag       */
#define PWC_FLAG_RAM_PARITY_MASK        (PWC_FLAG_RAM_PARITY_ERR)
/**
 * @}
 */

/**
 * @defgroup PWC_REG_Write_Unlock_Code PWC register unlock code.
 * @brief Lock/unlock Code for each module
 *        PWC_UNLOCK_CODE0:
 *          Below registers are locked in CLK module.
 *              XTALCFGR, XTALSTBCR, XTALCR, XTALSTDCR, XTALSTDSR, HRCTRM, HRCCR,
 *              MRCTRM, MRCCR, PLLCFGR, PLLCR, UPLLCFGR, UPLLCR, OSCSTBSR, CKSWR,
 *              SCFGR, USBCKCFGR, TPIUCKCFGR, MCO1CFGR, MCO2CFGR, XTAL32CR,
 *              XTALC32CFGR, XTAL32NFR, LRCCR, LRCTRM.
 *        PWC_UNLOCK_CODE1:
 *          Below registers are locked in PWC module.
 *              PWRC0, PWRC1, PWRC2, PWRC3, PDWKE0, PDWKE1, PDWKE2, PDWKES, PDWKF0,
 *              PDWKF1, PWCMR, PWR_STPMCR, RAMPC0, RAMOPM.
 *          Below registers are locked in CLK module.
 *              PERICKSEL, I2SCKSEL,
 *          Below register is locked in RMU module.
 *              RSTF0
 *        PWC_UNLOCK_CODE2:
 *          Below registers are locked in PWC module.
 *              PVDCR0, PVDCR1, PVDFCR, PVDLCR, PVDICR, PVDDSR
 * @{
 */
#define PWC_WRITE_ENABLE                (0xA500U)
#define PWC_UNLOCK_CODE0                (0xA501U)
#define PWC_UNLOCK_CODE1                (0xA502U)
#define PWC_UNLOCK_CODE2                (0xA508U)

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
 * @addtogroup PWC_Global_Functions
 * @{
 */
/**
 * @brief  Lock PWC, CLK, RMU register.
 * @param  [in] u16Module Lock code for each module.
 *   @arg  PWC_UNLOCK_CODE0
 *   @arg  PWC_UNLOCK_CODE1
 *   @arg  PWC_UNLOCK_CODE2
 * @retval None
 */
__STATIC_INLINE void PWC_REG_Lock(uint16_t u16Module)
{
    CM_PWC->FPRC = (PWC_WRITE_ENABLE | (uint16_t)((uint16_t)(~u16Module) & (CM_PWC->FPRC)));
}

/**
 * @brief  Unlock PWC, CLK, RMU register.
 * @param  [in] u16Module Unlock code for each module.
 *   @arg  PWC_UNLOCK_CODE0
 *   @arg  PWC_UNLOCK_CODE1
 *   @arg  PWC_UNLOCK_CODE2
 * @retval None
 */
__STATIC_INLINE void PWC_REG_Unlock(uint16_t u16Module)
{
    SET_REG16_BIT(CM_PWC->FPRC, u16Module);
}

/* PWC Sleep Function */
void PWC_SLEEP_Enter(void);

/* PWC Stop Function */
void PWC_STOP_Enter(uint8_t u8StopType);
int32_t PWC_STOP_StructInit(stc_pwc_stop_mode_config_t *pstcStopConfig);
int32_t PWC_STOP_Config(const stc_pwc_stop_mode_config_t *pstcStopConfig);
void PWC_STOP_ClockSelect(uint8_t u8Clock);

void PWC_STOP_HighPrecisionPORCmd(en_functional_state_t enNewState);

/* PWC Speed Switch Function */
int32_t PWC_HighSpeedToLowSpeed(void);
int32_t PWC_LowSpeedToHighSpeed(void);

/* PWC LDO Function */

/* PWC LVD/PVD Function */
int32_t PWC_LVD_Init(uint8_t u8Ch, const stc_pwc_lvd_init_t *pstcLvdInit);
int32_t PWC_LVD_StructInit(stc_pwc_lvd_init_t *pstcLvdInit);
void PWC_LVD_Cmd(uint8_t u8Ch, en_functional_state_t enNewState);
void PWC_LVD_ExtInputCmd(en_functional_state_t enNewState);
void PWC_LVD_CompareOutputCmd(uint8_t u8Ch, en_functional_state_t enNewState);
void PWC_LVD_DigitalFilterCmd(uint8_t u8Ch, en_functional_state_t enNewState);
void PWC_LVD_SetFilterClock(uint8_t u8Ch, uint32_t u32Clock);
void PWC_LVD_SetThresholdVoltage(uint8_t u8Ch, uint32_t u32Voltage);
void PWC_LVD_ClearStatus(uint8_t u8Flag);
en_flag_status_t PWC_LVD_GetStatus(uint8_t u8Flag);

/* PWC Power Monitor Function */
void PWC_PowerMonitorCmd(en_functional_state_t enNewState);
void PWC_SetPowerMonitorVoltageSrc(uint8_t u8VoltageSrc);

/* PWC RAM Function */
void PWC_RAM_SetProtectArea(uint8_t u8Area);
void PWC_RAM_ParityResetCmd(en_functional_state_t enNewState);
en_flag_status_t PWC_RAM_GetParityStatus(uint8_t u8Flag);

void PWC_DebugCmd(en_functional_state_t enNewState);

/**
 * @}
 */

#endif /* LL_PWC_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32_LL_PWC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
