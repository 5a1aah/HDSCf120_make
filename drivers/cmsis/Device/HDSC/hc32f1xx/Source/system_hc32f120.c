/**
 *******************************************************************************
 * @file  system_hc32f120.c
 * @brief This file provides two functions and two global variables to be called
 *        from user application.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-10-31       CDT             Delete the __low_level_init function of IAR and $Sub$$main function of MDK.
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
#include "system_hc32f120.h"

/**
 * @addtogroup CMSIS
 * @{
 */

/**
 * @addtogroup HC32F120_System
 * @{
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('define')
 ******************************************************************************/
/**
 * @defgroup HC32F120_System_Local_Macros HC32F120 System Local Macros
 * @{
 */
#define HRC_32MHz_VALUE                 (32000000UL)
#define HRC_48MHz_VALUE                 (48000000UL)

#define HRC_FREQ_CONFIG_MASK            (EFM_HRCCFGR_HRCFREQS_0 | EFM_HRCCFGR_HRCFREQS_1 | EFM_HRCCFGR_HRCFREQS_2)

/* Vector Table base offset field */
#ifndef VECT_TAB_OFFSET
#define VECT_TAB_OFFSET                 (0x0UL)     /*!< This value must be a multiple of 0x100. */
#endif
/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/**
 * @addtogroup HC32F120_System_Global_Variable
 * @{
 */

/*!< System clock frequency (Core clock) */
__NO_INIT uint32_t SystemCoreClock;
/*!< High speed RC frequency (HCR clock) */
__NO_INIT uint32_t HRC_VALUE;

/**
 * @}
 */

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @addtogroup HC32F120_System_Global_Functions
 * @{
 */

/**
 * @brief  Setup the microcontroller system. Initialize the System and update
 *         the SystemCoreClock variable.
 * @param  None
 * @retval None
 */
void SystemInit(void)
{
    SystemCoreClockUpdate();
    /* Configure the Vector Table relocation */
    SCB->VTOR = VECT_TAB_OFFSET;    /* Vector Table Relocation */
}

/**
 * @brief  Update HRC frequency according to Clock Register Values.
 * @param  None
 * @retval HRC frequency
 */
static uint32_t HrcUpdate(void)
{
    uint8_t u8HrcDiv;
    uint32_t u32HrcVal;

    u8HrcDiv = CM_EFM->HRCCFGR & HRC_FREQ_CONFIG_MASK;
    u32HrcVal = (0U == (CM_EFM->HRCCFGR & EFM_HRCCFGR_HRCFREQS_3)) ? HRC_32MHz_VALUE : HRC_48MHz_VALUE;
    u32HrcVal = (u8HrcDiv < 5U) ? (u32HrcVal >> u8HrcDiv) : (u32HrcVal >> 5U);

    return u32HrcVal;
}

/**
 * @brief  Update SystemCoreClock variable according to Clock Register Values.
 * @param  None
 * @retval None
 */
void SystemCoreClockUpdate(void)
{
    uint8_t u8SysClkSrc;

    HRC_VALUE = HrcUpdate();
    u8SysClkSrc = CM_CMU->CKSWR & CMU_CKSWR_CKSW;
    switch (u8SysClkSrc) {
        case 0x00U:     /* use internal high speed RC */
            SystemCoreClock = HRC_VALUE;
            break;
        case 0x01U:     /* use external high speed OSC */
            SystemCoreClock = XTAL_VALUE;
            break;
        case 0x02U:     /* use internal low speed RC */
            SystemCoreClock = LRC_VALUE;
            break;
        default:
            break;
    }
    SystemCoreClock >>= (uint8_t)(CM_CMU->SCKDIVR & CMU_SCKDIVR_SCKDIV);
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
