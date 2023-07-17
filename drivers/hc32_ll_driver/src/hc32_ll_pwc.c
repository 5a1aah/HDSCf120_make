/**
 *******************************************************************************
 * @file  hc32_ll_pwc.c
 * @brief This file provides firmware functions to manage the Power Control(PWC).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-06-30       CDT             Refine API PWC_STOP_Enter().
   2023-01-15       CDT             Optimize API PWC_STOP_ClockSelect() & comment.
   2023-01-15       CDT             Remove redundancy code.
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_pwc.h"
#include "hc32_ll_utility.h"

/**
 * @addtogroup LL_Driver
 * @{
 */

/**
 * @defgroup LL_PWC PWC
 * @brief Power Control Driver Library
 * @{
 */

#if (LL_PWC_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup PWC_Local_Macros PWC Local Macros
 * @{
 */

/* Get the backup register address of PWC */

#define PWC_MD_SWITCH_TIMEOUT           (30UL)
#define PWC_MD_SWITCH_TIMEOUT2          (0x1000UL)

#define PWC_LVD_EN_REG                  (CM_EFM->LVDICGCR)
#define PWC_LVD_EN_BIT                  (EFM_LVDICGCR_LVDDIS)
#define PWC_LVD_EXT_INPUT_EN_REG        (bCM_PWC->LVDCSR_b.EXVCCINEN)
#define PWC_LVD_CMP_OUTPUT_EN_REG       (CM_PWC->LVDCSR)
#define PWC_LVD_CMP_OUTPUT_EN_BIT       (PWC_LVDCSR_CMPOE)
#define PWC_LVD_FILTER_EN_REG           (CM_EFM->LVDICGCR)
#define PWC_LVD_FILTER_EN_BIT           (EFM_LVDICGCR_DFDIS)
#define PWC_LVD_STATUS_REG              (CM_PWC->LVDCSR)

#define PWC_LVD_BIT_OFFSET(x)           (0U)
#define PWC_LVD_EN_BIT_OFFSET(x)        (0U)

#define PWC_LVD_FLAG_MASK               (PWC_LVD1_FLAG_MON | PWC_LVD1_FLAG_DETECT)

/**
 * @defgroup PWC_Check_Parameters_Validity PWC Check Parameters Validity
 * @{
 */

/* Check PWC register lock status. */
#define IS_PWC_UNLOCKED()               ((CM_PWC->FPRC & PWC_FPRC_PWRWE) == PWC_FPRC_PWRWE)
/* Check PWC LVD register lock status. */
#define IS_PWC_LVD_UNLOCKED()           ((CM_PWC->FPRC & PWC_FPRC_LVRWE) == PWC_FPRC_LVRWE)

/* Parameter validity check for LVD channel. */
#define IS_PWC_LVD_CH(x)                ((x) == PWC_LVD_CH1)

/* Parameter validity check for LVD function setting. */
#define IS_PWC_LVD_EN(x)                                                        \
(   ((x) == PWC_LVD_ON)                             ||                          \
    ((x) == PWC_LVD_OFF))

/* Parameter validity check for LVD compare output setting. */
#define IS_PWC_LVD_CMP_EN(x)                                                    \
(   ((x) == PWC_LVD_CMP_ON)                         ||                          \
    ((x) == PWC_LVD_CMP_OFF))

/*  Parameter validity check for PWC LVD exception type. */
#define IS_PWC_LVD_EXP_TYPE(x)                                                  \
(   ((x) == PWC_LVD_EXP_TYPE_NONE)                  ||                          \
    ((x) == PWC_LVD_EXP_TYPE_INT)                   ||                          \
    ((x) == PWC_LVD_EXP_TYPE_NMI)                   ||                          \
    ((x) == PWC_LVD_EXP_TYPE_RST))

/* Parameter validity check for LVD digital noise filter function setting. */
#define IS_PWC_LVD_FILTER_EN(x)                                                 \
(   ((x) == PWC_LVD_FILTER_ON)                      ||                          \
    ((x) == PWC_LVD_FILTER_OFF))

/* Parameter validity check for LVD digital noise filter clock setting. */
#define IS_PWC_LVD_FILTER_CLK(x)                                                \
(   ((x) == PWC_LVD_FILTER_LRC_MUL2)                ||                          \
    ((x) == PWC_LVD_FILTER_LRC_MUL4)                ||                          \
    ((x) == PWC_LVD_FILTER_LRC_MUL8)                ||                          \
    ((x) == PWC_LVD_FILTER_LRC_MUL16))

#define IS_PWC_LVD_THRESHOLD_VOLTAGE(x)                                         \
(   ((x) == PWC_LVD_THRESHOLD_LVL0)                 ||                          \
    ((x) == PWC_LVD_THRESHOLD_LVL1)                 ||                          \
    ((x) == PWC_LVD_THRESHOLD_LVL2)                 ||                          \
    ((x) == PWC_LVD_THRESHOLD_LVL3)                 ||                          \
    ((x) == PWC_LVD_THRESHOLD_LVL4)                 ||                          \
    ((x) == PWC_LVD_THRESHOLD_LVL5)                 ||                          \
    ((x) == PWC_LVD_THRESHOLD_LVL6)                 ||                          \
    ((x) == PWC_LVD_THRESHOLD_LVL7)                 ||                          \
    ((x) == PWC_LVD_THRESHOLD_LVL8)                 ||                          \
    ((x) == PWC_LVD_THRESHOLD_LVL9)                 ||                          \
    ((x) == PWC_LVD_THRESHOLD_LVL10)                ||                          \
    ((x) == PWC_LVD_THRESHOLD_LVL11)                ||                          \
    ((x) == PWC_LVD_EXTVCC))

/* Parameter validity check for LVD NMI function setting. */
#define IS_PWC_LVD_NMI(x)                                                       \
(   ((x) == PWC_LVD_INT_MASK)                       ||                          \
    ((x) == PWC_LVD_INT_NONMASK))

/* Parameter validity check for LVD trigger setting. */
#define IS_PWC_LVD_CLR_FLAG(x)              ((x) == PWC_LVD1_FLAG_DETECT)

/* Parameter validity check for LVD flag. */
#define IS_PWC_LVD_GET_FLAG(x)                                                  \
(   ((x) != 0x00U)                                  &&                          \
    (((x) | PWC_LVD_FLAG_MASK) == PWC_LVD_FLAG_MASK))

/* Parameter validity check for clock setting after wake-up from stop mode. */
#define IS_PWC_STOP_CLK(x)                                                      \
(   ((x) == PWC_STOP_CLK_KEEP)                      ||                          \
    ((x) == PWC_STOP_CLK_HRC))

/*  Parameter validity check for stop mode high POR sel. */
#define IS_PWC_STOP_HIGH_PRECISION_POR(x)                                       \
(   ((x) == PWC_HIGH_PRECISION_POR_ON)              ||                          \
    ((x) == PWC_HIGH_PRECISION_POR_OFF))

/*  Parameter validity check for power monitor sel. */
#define IS_PWC_PWR_MON_SEL(x)                                                   \
(   ((x) == PWC_PWR_MON_IREF)                       ||                          \
    ((x) == PWC_PWR_MON_TSENSOR))

/* Parameter validity check for WKT Comparision Value. */
#define IS_PWC_WKT_COMPARISION_VALUE(x)             ((x) <= 0x0FFFU)

/* Parameter validity check for Ram protect area selection. */
#define IS_PWC_RAM_PROTECT_AREA(x)                                              \
(   ((x) == PWC_RAM_PROTECT_AREA_NONE)              ||                          \
    ((x) == PWC_RAM_PROTECT_AREA_128BYTE)           ||                          \
    ((x) == PWC_RAM_PROTECT_AREA_256BYTE)           ||                          \
    ((x) == PWC_RAM_PROTECT_AREA_512BYTE))

/* Parameter validity check for Ram Parity error flag. */
#define IS_PWC_RAM_PARITY_ERR_FLAG(x)                                           \
(   ((x) != 0x00U)                                  &&                          \
    (((x) | PWC_FLAG_RAM_PARITY_MASK) == PWC_FLAG_RAM_PARITY_MASK))

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @defgroup PWC_Local_Functions PWC Local Functions
 * @{
 */
/**
 * @brief  Backup HRC state and enable HRC before enter stop mode.
 * @param  None
 * @retval None
 */
static uint32_t PWC_ClockBackup(void)
{
    uint32_t u32HrcState;

    u32HrcState = READ_REG32(bCM_CMU->HRCCR_b.HRCSTP);

    /* Ensure HRC is Enable before enter stop mode. */
    if (1UL == u32HrcState) {
        /* Enable HRC. */
        WRITE_REG32(bCM_CMU->HRCCR_b.HRCSTP, 0UL);
    }

    return u32HrcState;
}

/**
 * @brief  HRC state recover after wakeup stop mode.
 * @param  u32HrcState     Specifies the recover HRC state
 * @retval None
 */
static void PWC_ClockRecover(uint32_t u32HrcState)
{
    WRITE_REG32(bCM_CMU->HRCCR_b.HRCSTP, u32HrcState);
}
/**
 * @}
 */

/**
 * @defgroup PWC_Global_Functions PWC Global Functions
 * @{
 */

/**
 * @brief  Enter stop mode.
 * @param  [in] u8StopType specifies the XTAL initial config.
 *   @arg  PWC_STOP_WFI
 *   @arg  PWC_STOP_WFE
 * @retval None
 */
void PWC_STOP_Enter(uint8_t u8StopType)
{
    uint32_t u32HrcState;

    DDL_ASSERT(IS_PWC_UNLOCKED());

    /* HRC backup and enable HRC before entry from stop mode. */
    u32HrcState = PWC_ClockBackup();

    SET_REG8_BIT(CM_PWC->STPMCR, PWC_STPMCR_STOP);
    if (PWC_STOP_WFI == u8StopType) {
        __WFI();
    } else {
        SET_REG32_BIT(SCB->SCR, SCB_SCR_SLEEPDEEP_Msk);
        __SEV();
        __WFE();
        __WFE();
    }

    /* HRC recover after wakeup from stop mode. */
    PWC_ClockRecover(u32HrcState);

}

/**
 * @brief  Enter sleep mode.
 * @param  None
 * @retval None
 */
void PWC_SLEEP_Enter(void)
{
    DDL_ASSERT(IS_PWC_UNLOCKED());

    CLR_REG8_BIT(CM_PWC->STPMCR, PWC_STPMCR_STOP);

    __WFI();
}

/**
 * @brief  Initialize LVD config structure. Fill each pstcLvdInit with default value
 * @param  [in] pstcLvdInit Pointer to a stc_pwc_lvd_init_t structure that contains configuration information.
 * @retval int32_t:
 *          - LL_OK: LVD structure initialize successful
 *          - LL_ERR_INVD_PARAM: NULL pointer
 */
int32_t PWC_LVD_StructInit(stc_pwc_lvd_init_t *pstcLvdInit)
{
    int32_t i32Ret = LL_OK;
    /* Check if pointer is NULL */
    if (NULL == pstcLvdInit) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        /* RESET LVD init structure parameters values */
        pstcLvdInit->u32State              = PWC_LVD_OFF;
        pstcLvdInit->u32CompareOutputState = PWC_LVD_CMP_OFF;
        pstcLvdInit->u32ExceptionType      = PWC_LVD_EXP_TYPE_NONE;
        pstcLvdInit->u32Filter             = PWC_LVD_FILTER_OFF;
        pstcLvdInit->u32FilterClock        = PWC_LVD_FILTER_LRC_MUL2;
        pstcLvdInit->u32ThresholdVoltage   = PWC_LVD_THRESHOLD_LVL0;
    }
    return i32Ret;
}

/**
 * @brief LVD configuration.
 * @param [in] u8Ch LVD channel @ref PWC_LVD_Channel.
 * @param [in] pstcLvdInit Pointer to a stc_pwc_lvd_init_t structure that contains configuration information.
 * @retval int32_t:
 *          - LL_OK: LVD initialize successful
 *          - LL_ERR_INVD_PARAM: NULL pointer
 */
int32_t PWC_LVD_Init(uint8_t u8Ch, const stc_pwc_lvd_init_t *pstcLvdInit)
{
    int32_t i32Ret = LL_OK;

    /* Check if pointer is NULL */
    if (NULL == pstcLvdInit) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        DDL_ASSERT(IS_PWC_LVD_UNLOCKED());
        DDL_ASSERT(IS_PWC_LVD_CH(u8Ch));
        DDL_ASSERT(IS_PWC_LVD_EN(pstcLvdInit->u32State));
        DDL_ASSERT(IS_PWC_LVD_EXP_TYPE(pstcLvdInit->u32ExceptionType));
        DDL_ASSERT(IS_PWC_LVD_CMP_EN(pstcLvdInit->u32CompareOutputState));
        DDL_ASSERT(IS_PWC_LVD_FILTER_EN(pstcLvdInit->u32Filter));
        DDL_ASSERT(IS_PWC_LVD_FILTER_CLK(pstcLvdInit->u32FilterClock));
        DDL_ASSERT(IS_PWC_LVD_THRESHOLD_VOLTAGE(pstcLvdInit->u32ThresholdVoltage));

        /* disable filter function in advance */
        SET_REG16_BIT(CM_EFM->LVDICGCR, EFM_LVDICGCR_DFDIS);
        /* Set the LVD digital filter sample clock. */
        MODIFY_REG16(CM_EFM->LVDICGCR, EFM_LVDICGCR_DFS, pstcLvdInit->u32FilterClock);

        MODIFY_REG16(CM_EFM->LVDICGCR, \
                     (EFM_LVDICGCR_DFDIS | EFM_LVDICGCR_LVDLVL | EFM_LVDICGCR_LVDDIS | \
                      EFM_LVDICGCR_NMIS  | EFM_LVDICGCR_IRS    | EFM_LVDICGCR_IRDIS),  \
                     (pstcLvdInit->u32Filter | pstcLvdInit->u32ThresholdVoltage | pstcLvdInit->u32State | \
                      pstcLvdInit->u32ExceptionType));

        MODIFY_REG8(CM_PWC->LVDCSR, PWC_LVDCSR_CMPOE, pstcLvdInit->u32CompareOutputState);

    }
    return i32Ret;
}

/**
 * @brief  Enable or disable LVD.
 * @param  [in] u8Ch Specifies whitch channel to operate. @ref PWC_LVD_Channel.
 * @param  [in] enNewState An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void PWC_LVD_Cmd(uint8_t u8Ch, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_PWC_LVD_CH(u8Ch));
    DDL_ASSERT(IS_PWC_LVD_UNLOCKED());

    if (ENABLE == enNewState) {
        SET_REG_BIT(PWC_LVD_EN_REG, PWC_LVD_EN_BIT << PWC_LVD_EN_BIT_OFFSET(u8Ch));
    } else {
        CLR_REG_BIT(PWC_LVD_EN_REG, PWC_LVD_EN_BIT << PWC_LVD_EN_BIT_OFFSET(u8Ch));
    }
}

/**
 * @brief  Enable or disable LVD external input.
 * @param  [in] enNewState An @ref en_functional_state_t enumeration value.
 * @retval None
 * @note   Threshold voltage must set PWC_LVD_EXTVCC.
 */
void PWC_LVD_ExtInputCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_PWC_LVD_UNLOCKED());

    WRITE_REG32(PWC_LVD_EXT_INPUT_EN_REG, enNewState);

}

/**
 * @brief  Enable or disable LVD compare output.
 * @param  [in] u8Ch Specifies whitch channel to operate. @ref PWC_LVD_Channel.
 * @param  [in] enNewState An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void PWC_LVD_CompareOutputCmd(uint8_t u8Ch, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_PWC_LVD_CH(u8Ch));
    DDL_ASSERT(IS_PWC_LVD_UNLOCKED());

    if (ENABLE == enNewState) {
        SET_REG_BIT(PWC_LVD_CMP_OUTPUT_EN_REG, PWC_LVD_CMP_OUTPUT_EN_BIT << PWC_LVD_BIT_OFFSET(u8Ch));
    } else {
        CLR_REG_BIT(PWC_LVD_CMP_OUTPUT_EN_REG, PWC_LVD_CMP_OUTPUT_EN_BIT << PWC_LVD_BIT_OFFSET(u8Ch));
    }
}

/**
 * @brief  Enable or disable LVD digital filter.
 * @param  [in] u8Ch Specifies whitch channel to operate. @ref PWC_LVD_Channel.
 * @param  [in] enNewState An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void PWC_LVD_DigitalFilterCmd(uint8_t u8Ch, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_PWC_LVD_CH(u8Ch));
    DDL_ASSERT(IS_PWC_LVD_UNLOCKED());

    if (ENABLE == enNewState) {
        CLR_REG_BIT(PWC_LVD_FILTER_EN_REG, PWC_LVD_FILTER_EN_BIT << PWC_LVD_BIT_OFFSET(u8Ch));
    } else {
        SET_REG_BIT(PWC_LVD_FILTER_EN_REG, PWC_LVD_FILTER_EN_BIT << PWC_LVD_BIT_OFFSET(u8Ch));
    }
}

/**
 * @brief  Enable or disable LVD compare output.
 * @param  [in] u8Ch Specifies whitch channel to operate. @ref PWC_LVD_Channel.
 * @param  [in] u32Clock Specifies filter clock. @ref PWC_LVD_DFS_Clk_Sel
 * @retval None
 */
void PWC_LVD_SetFilterClock(uint8_t u8Ch, uint32_t u32Clock)
{
    DDL_ASSERT(IS_PWC_LVD_CH(u8Ch));
    DDL_ASSERT(IS_PWC_LVD_FILTER_CLK(u32Clock));
    DDL_ASSERT(IS_PWC_LVD_UNLOCKED());
    MODIFY_REG16(CM_EFM->LVDICGCR, EFM_LVDICGCR_DFS, (uint16_t)u32Clock);
}

/**
 * @brief Set LVD threshold voltage.
 * @param  [in] u8Ch        Specifies whitch channel to operate. @ref PWC_LVD_Channel.
 * @param  [in] u32Voltage  Specifies threshold voltage. @ref PWC_LVD_Detection_Voltage_Sel
 * @retval None
 * @note    PWC_LVD_EXTVCC only valid while EXTINPUT enable.
 */
void PWC_LVD_SetThresholdVoltage(uint8_t u8Ch, uint32_t u32Voltage)
{
    DDL_ASSERT(IS_PWC_LVD_CH(u8Ch));
    DDL_ASSERT(IS_PWC_LVD_THRESHOLD_VOLTAGE(u32Voltage));
    DDL_ASSERT(IS_PWC_LVD_UNLOCKED());

    MODIFY_REG16(CM_EFM->LVDICGCR, EFM_LVDICGCR_LVDLVL, (uint16_t)u32Voltage);

}

/**
 * @brief  Get LVD flag.
 * @param  [in] u8Flag LVD flag to be get @ref PWC_LVD_Flag
 * @retval An @ref en_flag_status_t enumeration value
 * @note   PVDxDETFLG is avaliable when PVDCR0.PVDxEN and PVDCR1.PVDxCMPOE are set to '1'
 */
en_flag_status_t PWC_LVD_GetStatus(uint8_t u8Flag)
{
    DDL_ASSERT(IS_PWC_LVD_GET_FLAG(u8Flag));
    return ((0x00U != READ_REG8_BIT(PWC_LVD_STATUS_REG, u8Flag)) ? SET : RESET);
}

/**
 * @brief  Clear LVD flag.
 * @param  [in] u8Flag LVD flag to be get @ref PWC_LVD_Flag
 *  @arg      PWC_LVD1_FLAG_DETECT
 *  @arg      PWC_LVD2_FLAG_DETECT
 * @retval None
 */
void PWC_LVD_ClearStatus(uint8_t u8Flag)
{
    DDL_ASSERT(IS_PWC_LVD_UNLOCKED());
    DDL_ASSERT(IS_PWC_LVD_CLR_FLAG(u8Flag));

    CLR_REG8_BIT(PWC_LVD_STATUS_REG, u8Flag);
}

/**
 * @brief  Switch high speed to ultra low speed, set the drive ability.
 * @param  None
 * @retval int32_t:
 *          - LL_OK: Mode switch successful.
 *          - LL_ERR: Mode switch failure, check whether EFM was unlocked please.
 * @note   Before calling this API, please switch system clock to the required
 *         low speed frequency in advance, and make sure NO any flash program
 *         or erase operation background.
 */
int32_t PWC_HighSpeedToLowSpeed(void)
{
    DDL_ASSERT(IS_PWC_UNLOCKED());

    CLR_REG8_BIT(CM_PWC->PWRC, PWC_PWRC_PWDRV);

    /* Delay 30uS*/
    DDL_DelayUS(PWC_MD_SWITCH_TIMEOUT);

    return LL_OK;
}

/**
 * @brief  Switch ultra low speed to high speed, set the drive ability.
 * @param  None
 * @retval int32_t:
 *          - LL_OK: Mode switch successful.
 *          - LL_ERR: Mode switch failure, check whether EFM was unlocked please.
 * @note   After calling this API, the system clock is able to switch high frequency.
 */
/**
 * @brief  Switch ultra low speed to high speed, set the drive ability.
 * @param  None
 * @retval int32_t:
 *          - LL_OK: Mode switch successful.
 *          - LL_ERR: Mode switch failure, check whether EFM was unlocked please.
 * @note   After calling this API, the system clock is able to switch high frequency.
 */
int32_t PWC_LowSpeedToHighSpeed(void)
{
    DDL_ASSERT(IS_PWC_UNLOCKED());

    SET_REG8_BIT(CM_PWC->PWRC, PWC_PWRC_PWDRV);

    /* Delay 30uS*/
    DDL_DelayUS(PWC_MD_SWITCH_TIMEOUT);

    return LL_OK;
}

/**
 * @brief Stop mode config.
 * @param [in] pstcStopConfig Chip config before entry stop mode.
 *  @arg    u8StopDrv, MCU from which speed mode entry stop mode.
 *  @arg    u16Clock, System clock setting after wake-up from stop mode.
 *  @arg    u16FlashWait, Whether wait flash stable after wake-up from stop mode.
 *  @arg    u16ExBusHold, ExBus status in stop mode.
 * @retval int32_t:
 *          - LL_OK: Stop mode config successful
 *          - LL_ERR_INVD_PARAM: NULL pointer
 */
int32_t PWC_STOP_Config(const stc_pwc_stop_mode_config_t *pstcStopConfig)
{
    int32_t i32Ret = LL_OK;

    /* Check if pointer is NULL */
    if (NULL == pstcStopConfig) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {

        DDL_ASSERT(IS_PWC_UNLOCKED());

        DDL_ASSERT(IS_PWC_STOP_CLK(pstcStopConfig->u8Clock));
        DDL_ASSERT(IS_PWC_STOP_HIGH_PRECISION_POR(pstcStopConfig->u8HighPrecisionPOR));
        MODIFY_REG8(CM_PWC->STPMCR, (PWC_STPMCR_CKSHRC | PWC_STPMCR_HAPORDIS), \
                    (pstcStopConfig->u8Clock | pstcStopConfig->u8HighPrecisionPOR));

    }
    return i32Ret;
}

/**
 * @brief  Initialize stop mode config structure. Fill each pstcStopConfig with default value
 * @param  [in] pstcStopConfig Pointer to a stc_pwc_stop_mode_config_t structure that
 *                            contains configuration information.
 * @retval int32_t:
 *          - LL_OK: Stop down mode structure initialize successful
 *          - LL_ERR_INVD_PARAM: NULL pointer
 */
int32_t PWC_STOP_StructInit(stc_pwc_stop_mode_config_t *pstcStopConfig)
{
    int32_t i32Ret = LL_OK;

    /* Check if pointer is NULL */
    if (NULL == pstcStopConfig) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        pstcStopConfig->u8Clock = PWC_STOP_CLK_KEEP;
        pstcStopConfig->u8HighPrecisionPOR = PWC_HIGH_PRECISION_POR_ON;
    }
    return i32Ret;
}

/**
 * @brief Stop mode wake up clock config.
 * @param [in] u8Clock System clock setting after wake-up from stop mode. @ref PWC_STOP_CLK_Sel
 * @retval None
 */
void PWC_STOP_ClockSelect(uint8_t u8Clock)
{
    DDL_ASSERT(IS_PWC_STOP_CLK(u8Clock));
    DDL_ASSERT(IS_PWC_UNLOCKED());

    MODIFY_REG8(CM_PWC->STPMCR, PWC_STPMCR_CKSHRC, u8Clock);
}

/**
 * @brief  PWC high precision POR command.
 * @param  [in] enNewState      An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void PWC_STOP_HighPrecisionPORCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_PWC_UNLOCKED());

    if (ENABLE == enNewState) {
        CLR_REG8_BIT(CM_PWC->STPMCR, PWC_STPMCR_HAPORDIS);
    } else {
        SET_REG8_BIT(CM_PWC->STPMCR, PWC_STPMCR_HAPORDIS);
    }
}

/**
 * @brief  PWC power monitor command.
 * @param  [in] enNewState      An @ref en_functional_state_t enumeration value.
 * @retval None
 * @note   This monitor power is used for ADC and output to REGC pin.
 */
void PWC_PowerMonitorCmd(en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_PWC_UNLOCKED());
    if (ENABLE == enNewState) {
        SET_REG8_BIT(CM_PWC->PWRC, PWC_PWRC_PWMONE);
    } else {
        CLR_REG8_BIT(CM_PWC->PWRC, PWC_PWRC_PWMONE);
    }

}

/**
 * @brief  PWC power monitor voltage config.
 * @param  [in] u8VoltageSrc   PWC power monitor voltage config @ref PWC_Monitor_Power.
 *         This parameter can be one of the following values
 *  @arg    PWC_PWR_MON_IREF
 *  @arg    PWC_PWR_MON_TSENSOR or PWC_PWR_MON_VBAT_DIV2
 * @retval None
 * @note   This monitor power is used for ADC and output to REGC pin.
 *         PWC_PWR_MON_VBAT_DIV2 is only valid while HC32F4A0.
 */
void PWC_SetPowerMonitorVoltageSrc(uint8_t u8VoltageSrc)
{
    DDL_ASSERT(IS_PWC_PWR_MON_SEL(u8VoltageSrc));
    DDL_ASSERT(IS_PWC_UNLOCKED());

    MODIFY_REG8(CM_PWC->PWRC, PWC_PWRC_PWMONSEL, u8VoltageSrc);
}

/**
 * @brief  Set ram protect area
 * @param  [in] u8Area   Specifies.whitch ram area to protect @ref PWC_Ram_Protect_Area_Sel
 *  @arg   PWC_RAM_PROTECT_AREA_NONE
 *  @arg   PWC_RAM_PROTECT_AREA_128BYTE
 *  @arg   PWC_RAM_PROTECT_AREA_256BYTE
 *  @arg   PWC_RAM_PROTECT_AREA_512BYTE
 * @retval None
 */
void PWC_RAM_SetProtectArea(uint8_t u8Area)
{
    DDL_ASSERT(IS_PWC_RAM_PROTECT_AREA(u8Area));
    DDL_ASSERT(IS_PWC_LVD_UNLOCKED());

    MODIFY_REG8(CM_PWC->RAMCR, PWC_RAMCR_RPRTA, u8Area);
}

/**
 * @brief  The Ram Parity error reset function command.
 * @param  [in] enNewState An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void PWC_RAM_ParityResetCmd(en_functional_state_t enNewState)
{
    /* Parameter valid check */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_PWC_UNLOCKED());

    if (ENABLE == enNewState) {
        CLR_REG8_BIT(CM_PWC->RAMCR, PWC_RAMCR_RPERDIS);
    } else {
        SET_REG8_BIT(CM_PWC->RAMCR, PWC_RAMCR_RPERDIS);
    }
}

/**
 * @brief  Get the ram parity error check flag.
 * @param  [in] u8Flag                  PWC Ram Parity flag @ref PWC_Ram_Parity_Error_Flag
 * @retval An @ref en_flag_status_t enumeration type value.
 */
en_flag_status_t PWC_RAM_GetParityStatus(uint8_t u8Flag)
{
    /* Parameter valid check */
    DDL_ASSERT(IS_PWC_RAM_PARITY_ERR_FLAG(u8Flag));

    return ((0U != READ_REG8_BIT(CM_PWC->RAMCR, u8Flag)) ? SET : RESET);
}

/**
 * @brief  Enable or disable the Debug.
 * @param  [in] enNewState      An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void PWC_DebugCmd(en_functional_state_t enNewState)
{
    /* Parameter valid check */
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_PWC_UNLOCKED());

    WRITE_REG32(bCM_PWC->DBGC_b.DBGEN, enNewState);
}

/**
 * @}
 */

#endif  /* LL_PWC_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
