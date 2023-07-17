/**
 *******************************************************************************
 * @file  sk_hc32f120_lqfp44.c
 * @brief This file provides firmware functions for SK_HC32F120_LQFP44 BSP
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
#include "sk_hc32f120_lqfp44.h"

/**
 * @defgroup BSP BSP
 * @{
 */

/**
 * @defgroup SK_HC32F120_LQFP44 SK_HC32F120_LQFP44
 * @{
 */

/**
 * @defgroup SK_HC32F120_LQFP44_COMMON SK_HC32F120_LQFP44 COMMON
 * @{
 */

#if (BSP_SK_HC32F120_LQFP44 == BSP_SK_HC32F1XX)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static const uint8_t BSP_LED_PORT[BSP_LED_NUM] =                               \
{BSP_LED_RED_PORT, BSP_LED_YELLOW_PORT, BSP_LED_BLUE_PORT};

static const uint8_t BSP_LED_PIN[BSP_LED_NUM] =                                \
{BSP_LED_RED_PIN, BSP_LED_YELLOW_PIN, BSP_LED_BLUE_PIN};

static const uint8_t BSP_KEY_PORT[BSP_KEY_NUM] =                               \
{BSP_KEY1_PORT, BSP_KEY2_PORT};

static const uint8_t BSP_KEY_PIN[BSP_KEY_NUM] =                                \
{BSP_KEY1_PIN, BSP_KEY2_PIN};

static uint32_t m_u32GlobalKey = 0x00UL;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup SK_HC32F120_LQFP44_Global_Functions SK_HC32F120_LQFP44 Global Functions
 * @{
 */

/**
 * @brief  LED initialize.
 * @param  None
 * @retval None
 */
void BSP_LED_Init(void)
{
    uint8_t i;
    stc_gpio_init_t stcGpioInit;

    /* Initialize GPIO structure */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDir = PIN_DIR_OUT;
    stcGpioInit.u16PinState = PIN_STAT_SET;

    /* Initialize RGB LED pin */
    for (i = 0U; i < BSP_LED_NUM; i++) {
        (void)GPIO_Init(BSP_LED_PORT[i], BSP_LED_PIN[i], &stcGpioInit);
    }
}

/**
 * @brief  Turn on LEDs.
 * @param  [in] u8Led LED
 *   @arg  LED_RED
 *   @arg  LED_YELLOW
 *   @arg  LED_BLUE
 * @retval None
 */
void BSP_LED_On(uint8_t u8Led)
{
    uint8_t i;

    for (i = 0U; i < BSP_LED_NUM; i++) {
        if (0U != ((u8Led >> i) & 1U)) {
            GPIO_ResetPins(BSP_LED_PORT[i], BSP_LED_PIN[i]);
        }
    }
}

/**
 * @brief  Turn off LEDs.
 * @param  [in] u8Led LED
 *   @arg  LED_RED
 *   @arg  LED_YELLOW
 *   @arg  LED_BLUE
 * @retval None
 */
void BSP_LED_Off(uint8_t u8Led)
{
    uint8_t i;

    for (i = 0U; i < BSP_LED_NUM; i++) {
        if (0U != ((u8Led >> i) & 1U)) {
            GPIO_SetPins(BSP_LED_PORT[i], BSP_LED_PIN[i]);
        }
    }
}

/**
 * @brief  Toggle LEDs.
 * @param  [in] u8Led LED
 *   @arg  LED_RED
 *   @arg  LED_YELLOW
 *   @arg  LED_BLUE
 * @retval None
 */
void BSP_LED_Toggle(uint8_t u8Led)
{
    uint8_t i;

    for (i = 0U; i < BSP_LED_NUM; i++) {
        if (0U != ((u8Led >> i) & 1U)) {
            GPIO_TogglePins(BSP_LED_PORT[i], BSP_LED_PIN[i]);
        }
    }
}

#if (LL_PRINT_ENABLE == DDL_ON)
/**
 * @brief  BSP printf device, clock and port pre-initialize.
 * @param  [in] vpDevice                Pointer to print device
 * @param  [in] u32Baudrate             Print device communication baudrate
 * @retval int32_t:
 *           - LL_OK:                   Initialize successfully.
 *           - LL_ERR:                  Initialize unsuccessfully.
 *           - LL_ERR_INVD_PARAM:       The u32Baudrate value is 0.
 */
int32_t BSP_PRINTF_Preinit(void *vpDevice, uint32_t u32Baudrate)
{
    uint32_t u32Div;
    float32_t f32Error;
    stc_usart_uart_init_t stcUartInit;
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    (void)vpDevice;

    if (0UL != u32Baudrate) {
        /* Set TX port function */
        GPIO_SetFunc(BSP_PRINTF_PORT, BSP_PRINTF_PIN, BSP_PRINTF_PORT_FUNC);

        /* Enable clock  */
        FCG_Fcg0PeriphClockCmd(BSP_PRINTF_DEVICE_FCG, ENABLE);

        /* Configure UART */
        (void)USART_UART_StructInit(&stcUartInit);
        stcUartInit.u32OverSampleBit = USART_OVER_SAMPLE_8BIT;
        (void)USART_UART_Init(BSP_PRINTF_DEVICE, &stcUartInit, NULL);

        for (u32Div = 0UL; u32Div <= USART_CLK_DIV64; u32Div++) {
            USART_SetClockDiv(BSP_PRINTF_DEVICE, u32Div);
            i32Ret = USART_SetBaudrate(BSP_PRINTF_DEVICE, u32Baudrate, &f32Error);
            if ((LL_OK == i32Ret) && \
                ((-BSP_PRINTF_BAUDRATE_ERR_MAX <= f32Error) && (f32Error <= BSP_PRINTF_BAUDRATE_ERR_MAX))) {
                USART_FuncCmd(BSP_PRINTF_DEVICE, USART_TX, ENABLE);
                break;
            } else {
                i32Ret = LL_ERR;
            }
        }
    }

    return i32Ret;
}
#endif

/**
 * @brief  XTAL initialize.
 * @param  None
 * @retval None
 */
static int32_t BSP_HrcInit(void)
{
    /* HRC config */
    (void)CLK_HrcFreqConfig(CLK_HRC_FREQ_48M);
    return CLK_HrcCmd(ENABLE);
}

/**
 * @brief  BSP clock initialize.
 *         Set board system clock HRC@48MHz
 *         Flash: 1 wait
 * @param  None
 * @retval None
 */
__WEAKDEF void BSP_CLK_Init(void)
{
    /* HCLK Max 48MHz */
    CLK_SetClockDiv(CLK_BUS_HCLK, CLK_SYSCLK_DIV1);
    /* 1-wait @ 48MHz */
    (void)EFM_SetWaitCycle(EFM_WAIT_CYCLE1);
    /* Enable High driver voltage */
    (void)PWC_LowSpeedToHighSpeed();
    (void)BSP_HrcInit();
    CLK_SetSysClockSrc(CLK_SYSCLK_SRC_HRC);
}

#if (DDL_ON == BSP_INT_KEY_ENABLE)
/**
 * @brief  NVIC config
 * @param  [in] IRQn IRQ number
 * @param  [in] priority IRQ priority
 *   @arg  DDL_IRQ_PRIO_00
 *   @arg  DDL_IRQ_PRIO_01
 *   @arg  DDL_IRQ_PRIO_02
 *   @arg  DDL_IRQ_PRIO_03
 * @retval None
 */
static void NVIC_Config(IRQn_Type IRQn, uint32_t priority)
{
    NVIC_ClearPendingIRQ(IRQn);
    NVIC_SetPriority(IRQn, priority);
    NVIC_EnableIRQ(IRQn);
}

/**
 * @brief  KEY1 (External interrupt Ch.6) ISR
 * @param  None
 * @retval None
 */
void EXTINT06_Handler(void)
{
    for (;;) {
        DDL_DelayMS(BSP_KEY_DELAY_MS);
        if (PIN_RESET == GPIO_ReadInputPins(BSP_KEY1_PORT, BSP_KEY1_PIN)) {
            m_u32GlobalKey |= 0x01UL;
        } else {
            EXTINT_ClearExtIntStatus(BSP_KEY1_INT_CH);
            break;
        }
    }
    BSP_KEY_KEY1_IrqCallback();
}

/**
 * @brief  KEY2 (External interrupt Ch.0) ISR
 * @param  None
 * @retval None
 */
void EXTINT00_Handler(void)
{
    for (;;) {
        DDL_DelayMS(BSP_KEY_DELAY_MS);
        if (PIN_RESET == GPIO_ReadInputPins(BSP_KEY2_PORT, BSP_KEY2_PIN)) {
            m_u32GlobalKey |= 0x02UL;
        } else {
            EXTINT_ClearExtIntStatus(BSP_KEY2_INT_CH);
            break;
        }
    }
    BSP_KEY_KEY2_IrqCallback();
}

/**
 * @brief  User callback function for BSP KEY1.
 * @param  None
 * @retval None
 */
__WEAKDEF void BSP_KEY_KEY1_IrqCallback(void)
{
    /* This function should be implemented by the user application. */
}

/**
 * @brief  User callback function for BSP KEY2.
 * @param  None
 * @retval None
 */
__WEAKDEF void BSP_KEY_KEY2_IrqCallback(void)
{
    /* This function should be implemented by the user application. */
}
#endif

/**
 * @brief  BSP key initialize
 * @param  [in] u8Mode Key work mode
 *   @arg  BSP_KEY_MD_GPIO
 *   @arg  BSP_KEY_MD_EXTINT
 * @retval None
 */
void BSP_KEY_Init(uint8_t u8Mode)
{
    uint8_t i;
    stc_gpio_init_t stcGpioInit;
#if (DDL_ON == BSP_INT_KEY_ENABLE)
    stc_extint_init_t stcExtIntInit;
#endif
    (void)GPIO_StructInit(&stcGpioInit);
    if (BSP_KEY_MD_GPIO == u8Mode) {
        for (i = 0U; i < BSP_KEY_NUM; i++) {
            (void)GPIO_Init(BSP_KEY_PORT[i], BSP_KEY_PIN[i], &stcGpioInit);
        }
    }
#if (DDL_ON == BSP_INT_KEY_ENABLE)
    else if (BSP_KEY_MD_EXTINT == u8Mode) {
        stcGpioInit.u16ExtInt = PIN_EXTINT_ON;
        for (i = 0U; i < BSP_KEY_NUM; i++) {
            (void)GPIO_Init(BSP_KEY_PORT[i], BSP_KEY_PIN[i], &stcGpioInit);
        }

        (void)EXTINT_StructInit(&stcExtIntInit);
        stcExtIntInit.u32Edge = EXTINT_TRIG_FALLING;

        (void)EXTINT_Init((BSP_KEY1_INT_CH | BSP_KEY2_INT_CH), &stcExtIntInit);

        EXTINT_ClearExtIntStatus(BSP_KEY1_INT_CH | BSP_KEY2_INT_CH);

        NVIC_Config(BSP_KEY1_INT_IRQn, DDL_IRQ_PRIO_DEFAULT);
        NVIC_Config(BSP_KEY2_INT_IRQn, DDL_IRQ_PRIO_DEFAULT);
    }
#endif
    else {
        /* avoid MISRAC 2012-15.7 */
        ;
    }
}

/**
 * @brief  Get BSP key status
 * @param  [in] u32Key chose one macro from below
 *   @arg  BSP_KEY_1
 *   @arg  BSP_KEY_2
 * @param  [in] u8Mode Key work mode
 *   @arg  BSP_KEY_MD_GPIO
 *   @arg  BSP_KEY_MD_EXTINT
 * @retval An @ref en_flag_status_t enumeration type value.
 */
en_flag_status_t BSP_KEY_GetStatus(uint32_t u32Key, uint8_t u8Mode)
{
    en_flag_status_t enStatus = RESET;

    if (BSP_KEY_MD_GPIO == u8Mode) {
        DDL_DelayMS(BSP_KEY_DELAY_MS);
        enStatus = (PIN_SET == GPIO_ReadInputPins(BSP_KEY_PORT[u32Key], BSP_KEY_PIN[u32Key])) ? RESET : SET;
    } else if (BSP_KEY_MD_EXTINT == u8Mode) {
        if (0UL != (m_u32GlobalKey & (1UL << u32Key))) {
            enStatus = SET;
            m_u32GlobalKey &= ~(1UL << u32Key);
        }
    } else {
        /* avoid MISRAC 2012-15.7 */
        ;
    }
    return enStatus;
}

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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
