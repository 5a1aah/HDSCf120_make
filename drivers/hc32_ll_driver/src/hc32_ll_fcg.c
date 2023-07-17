/**
 *******************************************************************************
 * @file  hc32_ll_fcg.c
 * @brief This file provides firmware functions to manage the Function Clock
 *        Gate (FCG).
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_fcg.h"
#include "hc32_ll_utility.h"

/**
 * @addtogroup LL_Driver
 * @{
 */

/**
 * @defgroup LL_FCG FCG
 * @brief FCG Driver Library
 * @{
 */

#if (LL_FCG_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup FCG_Local_Macros FCG Local Macros
 * @{
 */

/**
 * @defgroup FCG_Check_Parameters_Validity FCG Check Parameters Validity
 * @{
 */
/* Parameter validity check for peripheral in fcg0. */
#define IS_FCG0_PERIPH(per)                                 \
(   ((per) != 0x00UL)                           &&          \
    (((per) | FCG_FCG0_PERIPH_MASK) == FCG_FCG0_PERIPH_MASK))

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
 * @defgroup FCG_Global_Functions FCG Global Functions
 * @{
 */

/**
 * @brief Enable or disable the FCG0 peripheral clock.
 * @param [in] u32Fcg0Periph The peripheral in FCG0 @ref FCG_FCG0_Peripheral.
 * @param [in] enNewState An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void FCG_Fcg0PeriphClockCmd(uint32_t u32Fcg0Periph, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_FCG0_PERIPH(u32Fcg0Periph));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (ENABLE == enNewState) {
        CLR_REG32_BIT(CM_CMU->FCG, u32Fcg0Periph);
    } else {
        SET_REG32_BIT(CM_CMU->FCG, u32Fcg0Periph);
    }
}

#endif /* LL_FCG_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
