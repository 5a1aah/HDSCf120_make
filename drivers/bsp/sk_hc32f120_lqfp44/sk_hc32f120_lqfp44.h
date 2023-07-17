/**
 *******************************************************************************
 * @file  sk_hc32f120_lqfp44.h
 * @brief This file contains all the functions prototypes of the
 *        SK_HC32F120_LQFP44 BSP driver library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2023-01-15       CDT             Add XTAL IO define.
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
#ifndef __SK_HC32F120_LQFP44_H__
#define __SK_HC32F120_LQFP44_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_clk.h"
#include "hc32_ll_efm.h"
#include "hc32_ll_fcg.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll_icg.h"
#include "hc32_ll_interrupts.h"
#include "hc32_ll_pwc.h"
#include "hc32_ll_usart.h"
#include "hc32_ll_utility.h"

/**
 * @addtogroup BSP
 * @{
 */

/**
 * @addtogroup SK_HC32F120_LQFP44
 * @{
 */

/**
 * @addtogroup SK_HC32F120_LQFP44_COMMON
 * @{
 */

#if (BSP_SK_HC32F120_LQFP44 == BSP_SK_HC32F1XX)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup SK_HC32F120_LQFP44_Global_Macros SK_HC32F120_LQFP44 Global Macros
 * @{
 */
/**
 * @defgroup SK_HC32F120_LQFP44_LED_Sel SK_HC32F120_LQFP44 LED definition
 * @{
 */
#define LED_RED                 (0x01U)
#define LED_YELLOW              (0x02U)
#define LED_BLUE                (0x04U)
#define LED_ALL                 (LED_RED | LED_YELLOW | LED_BLUE)
/**
 * @}
 */

/**
 * @defgroup SK_HC32F120_LQFP44_KEY_Sel SK_HC32F120_LQFP44 KEY definition
 * @{
 */
#define BSP_KEY_1               (0x00U)
#define BSP_KEY_2               (0x01U)
/**
 * @}
 */

/**
 * @defgroup SK_HC32F120_LQFP44_KEY_MD_Sel SK_HC32F120_LQFP44 KEY work mode definition
 * @{
 */
#define BSP_KEY_MD_GPIO         (0x00U)
#define BSP_KEY_MD_EXTINT       (0x01U)
/**
 * @}
 */

/**
 * @defgroup SK_HC32F120_LQFP44_LED_Number SK_HC32F120_LQFP44 LED Number
 * @{
 */
#define BSP_LED_NUM             (3U)
/**
 * @}
 */

/**
 * @defgroup SK_HC32F120_LQFP44_LED_PortPin_Sel SK_HC32F120_LQFP44 LED port/pin definition
 * @{
 */
#define BSP_LED_RED_PORT        (GPIO_PORT_2)
#define BSP_LED_RED_PIN         (GPIO_PIN_05)
#define BSP_LED_YELLOW_PORT     (GPIO_PORT_2)
#define BSP_LED_YELLOW_PIN      (GPIO_PIN_06)
#define BSP_LED_BLUE_PORT       (GPIO_PORT_2)
#define BSP_LED_BLUE_PIN        (GPIO_PIN_07)
/**
 * @}
 */

/**
 * @defgroup SK_HC32F120_LQFP44_KEY_Number SK_HC32F120_LQFP44 KEY Number
 * @{
 */
#define BSP_KEY_NUM             (2U)
/**
 * @}
 */

/**
 * @defgroup SK_HC32F120_LQFP44_KEY_PortPin_Sel SK_HC32F120_LQFP44 KEY port/pin definition
 * @{
 */
#define BSP_KEY1_PORT           (GPIO_PORT_6)
#define BSP_KEY1_PIN            (GPIO_PIN_02)
#define BSP_KEY2_PORT           (GPIO_PORT_7)
#define BSP_KEY2_PIN            (GPIO_PIN_00)
/**
 * @}
 */

/**
 * @defgroup SK_HC32F120_LQFP44_KEY_INT_PortPin_Sel SK_HC32F120_LQFP44 KEY interrupt definition
 * @{
 */
/* KEY1 EXTINT6 */
#define BSP_KEY1_INT_CH         (EXTINT_CH06)
#define BSP_KEY1_INT_IRQn       (EXTINT06_IRQn)

/* KEY2 EXTINT0 */
#define BSP_KEY2_INT_CH         (EXTINT_CH00)
#define BSP_KEY2_INT_IRQn       (EXTINT00_IRQn)
#define BSP_KEY2_INT_WAKEUP     (INTC_STOP_WKUP_EXTINT_CH0)
#define BSP_KEY2_INT_EVT        (EVT_SRC_PORT_EIRQ0)

/* KEY Dithering Elimination */
#define BSP_KEY_DELAY_MS        (10UL)
/**
 * @}
 */

/**
 * @defgroup SK_HC32F120_LQFP44_PRINT_CONFIG SK_HC32F120_LQFP44 PRINT Configure definition
 * @{
 */
#define BSP_PRINTF_DEVICE               (CM_USART1)
#define BSP_PRINTF_DEVICE_FCG           (FCG0_PERIPH_USART1)

#define BSP_PRINTF_BAUDRATE             (115200UL)
#define BSP_PRINTF_BAUDRATE_ERR_MAX     (0.025F)

#define BSP_PRINTF_PORT                 (GPIO_PORT_1)
#define BSP_PRINTF_PIN                  (GPIO_PIN_02)
#define BSP_PRINTF_PORT_FUNC            (GPIO_FUNC_3)
/**
 * @}
 */

/**
 * @defgroup BSP_XTAL_CONFIG BSP XTAL Configure definition
 * @{
 */
#define BSP_XTAL_PORT                   (GPIO_PORT_12)
#define BSP_XTAL_IN_PIN                 (GPIO_PIN_01)
#define BSP_XTAL_OUT_PIN                (GPIO_PIN_02)
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
 * @addtogroup SK_HC32F120_LQFP44_Global_Functions
 * @{
 */
void BSP_LED_Init(void);
void BSP_LED_On(uint8_t u8Led);
void BSP_LED_Off(uint8_t u8Led);
void BSP_LED_Toggle(uint8_t u8Led);
void BSP_CLK_Init(void);
void BSP_KEY_Init(uint8_t u8Mode);
en_flag_status_t BSP_KEY_GetStatus(uint32_t u32Key, uint8_t u8Mode);

#if (LL_PRINT_ENABLE == DDL_ON)
int32_t BSP_PRINTF_Preinit(void *vpDevice, uint32_t u32Baudrate);
#endif

void EXTINT00_Handler(void);
void EXTINT06_Handler(void);

/* User Callbacks: User has to implement these functions in his code if they're needed.
   These functions are called when BSP_CLK_Init is initialized to BSP_KEY_MD_EXTINT mode. */
#if (DDL_ON == BSP_INT_KEY_ENABLE)
void BSP_KEY_KEY1_IrqCallback(void);
void BSP_KEY_KEY2_IrqCallback(void);
#endif

/**
 * @}
 */

#endif /* BSP_SK_HC32F120_LQFP44 */
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

#endif /* __SK_HC32F120_LQFP44_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
