/**
 *******************************************************************************
 * @file  system_hc32f120.h
 * @brief This file contains all the functions prototypes of the HC32 System.
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
#ifndef __SYSTEM_HC32F120_H__
#define __SYSTEM_HC32F120_H__

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
 * @addtogroup HC32F120_System_Global_Macros
 * @{
 */

/**
 * @addtogroup HC32F120_System_Clock_Source
 * @{
 */
#if !defined (LRC_VALUE)
#define LRC_VALUE                       (32768UL)       /*!< Internal low speed RC freq. */
#endif

#if !defined (XTAL_VALUE)
#define XTAL_VALUE                      (20000000UL)    /*!< External high speed OSC freq. */
#endif

#if !defined (HCLK_VALUE)
#define HCLK_VALUE                      (SystemCoreClock >> (CM_CMU->SCKDIVR & CMU_SCKDIVR_SCKDIV))
#endif

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
/**
 * @addtogroup HC32F120_System_Exported_Variable
 * @{
 */

extern uint32_t SystemCoreClock;        /*!< System clock frequency (Core clock) */
extern uint32_t HRC_VALUE;              /*!< HRC frequency */

/**
 * @}
 */

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup HC32F120_System_Global_Functions
 * @{
 */

extern void SystemInit(void);             /*!< Initialize the system */
extern void SystemCoreClockUpdate(void);  /*!< Update SystemCoreClock variable */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_HC32F120_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
