/**
 *******************************************************************************
 * @file  hc32_ll_fcg.h
 * @brief This file contains all the functions prototypes of the FCG driver
 *        library.
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
#ifndef __HC32_LL_FCG_H__
#define __HC32_LL_FCG_H__

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
 * @addtogroup LL_FCG
 * @{
 */

#if (LL_FCG_ENABLE == DDL_ON)
/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup FCG_Global_Macros FCG Global Macros
 * @{
 */
/**
 * @defgroup FCG_FCG0_Peripheral FCG FCG0 peripheral
 * @{
 */
#define FCG0_PERIPH_ADC                 (CMU_FCG_ADC)
#define FCG0_PERIPH_CTC                 (CMU_FCG_CTC)
#define FCG0_PERIPH_AOS                 (CMU_FCG_AOS)
#define FCG0_PERIPH_DMA                 (CMU_FCG_DMA)
#define FCG0_PERIPH_CRC                 (CMU_FCG_CRC)
#define FCG0_PERIPH_TMRB_1              (CMU_FCG_TMRB1)
#define FCG0_PERIPH_TMRB_2              (CMU_FCG_TMRB2)
#define FCG0_PERIPH_TMRB_3              (CMU_FCG_TMRB3)
#define FCG0_PERIPH_TMRB_4              (CMU_FCG_TMRB4)
#define FCG0_PERIPH_TMRB_5              (CMU_FCG_TMRB5)
#define FCG0_PERIPH_TMRB_6              (CMU_FCG_TMRB6)
#define FCG0_PERIPH_TMRB_7              (CMU_FCG_TMRB7)
#define FCG0_PERIPH_TMRB_8              (CMU_FCG_TMRB8)
#define FCG0_PERIPH_TMR0                (CMU_FCG_TMR0)
#define FCG0_PERIPH_USART1              (CMU_FCG_UART1)
#define FCG0_PERIPH_USART2              (CMU_FCG_UART2)
#define FCG0_PERIPH_USART3              (CMU_FCG_UART3)
#define FCG0_PERIPH_USART4              (CMU_FCG_UART4)
#define FCG0_PERIPH_I2C                 (CMU_FCG_I2C)
#define FCG0_PERIPH_SPI                 (CMU_FCG_SPI)

/**
 * @}
 */

/**
 * @defgroup FCG_FCGx_Peripheral_Mask FCG FCGx Peripheral Mask
 * @{
 */
#define FCG_FCG0_PERIPH_MASK            (0x3F81FF93UL)
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
 * @addtogroup FCG_Global_Functions
 * @{
 */

void FCG_Fcg0PeriphClockCmd(uint32_t u32Fcg0Periph, en_functional_state_t enNewState);

/**
 * @}
 */

#endif /* LL_FCG_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32_LL_FCG_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
