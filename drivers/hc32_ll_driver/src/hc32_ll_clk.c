/**
 *******************************************************************************
 * @file  hc32_ll_clk.c
 * @brief This file provides firmware functions to manage the Clock(CLK).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2023-01-15       CDT             Add ADC clock assert.
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
#include "hc32_ll_clk.h"
#include "hc32_ll_utility.h"

/**
 * @addtogroup LL_Driver
 * @{
 */

/**
 * @defgroup LL_CLK CLK
 * @brief Clock Driver Library
 * @{
 */

#if (LL_CLK_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup CLK_Local_Macros CLK Local Macros
 * @{
 */

/**
 * @brief CLK_FREQ Clock frequency definition
 */
#define CLK_FREQ_48M                    (48UL*1000UL*1000UL)
#define CLK_FREQ_64M                    (64UL*1000UL*1000UL)
#define CLK_FREQ_32M                    (32UL*1000UL*1000UL)

/**
 * @brief Be able to modify TIMEOUT according to board condition.
 */
#define CLK_TIMEOUT                     ((uint32_t)0x1000UL)
#define CLK_LRC_TIMEOUT                 ((uint32_t)0x200U)

/**
 * @brief LRC State ON or OFF
 */
#define CLK_LRC_OFF                     (CMU_LRCCR_LRCSTP)
#define CLK_LRC_ON                      (0x00U)

#define EFM_HRC_FREQ_CFG                (EFM_HRCCFGR_HRCFREQS_0 | EFM_HRCCFGR_HRCFREQS_1 | EFM_HRCCFGR_HRCFREQS_2)

/**
 * @defgroup CLK_Check_Parameters_Validity CLK Check Parameters Validity
 * @{
 */
/* Check CLK register lock status. */
#define IS_CLK_UNLOCKED()               ((CM_PWC->FPRC & PWC_FPRC_CKRWE) == PWC_FPRC_CKRWE)
#define IS_CLK_EFM_UNLOCK()             (CM_EFM->FAPRT == 0x00000001UL)

/* Parameter valid check for XTAL state */
#define IS_CLK_XTAL_STATE(x)                                                   \
(   ((x) == CLK_XTAL_OFF)                         ||                           \
    ((x) == CLK_XTAL_ON))

/* Parameter valid check for XTAL mode */
#define IS_CLK_XTAL_MD(x)                                                      \
(   ((x) == CLK_XTAL_MD_OSC)                      ||                           \
    ((x) == CLK_XTAL_MD_EXCLK))

/* Parameter valid check for XTAL super drive state */
#define IS_CLK_XTAL_SUPDRV_STATE(x)                                            \
(   ((x) == CLK_XTAL_SUPDRV_ON)                   ||                           \
    ((x) == CLK_XTAL_SUPDRV_OFF))

/* Parameter valid check for XTAL driver ability mode */
#define IS_CLK_XTAL_DRV_MD(x)                                                  \
(   ((x) == CLK_XTAL_DRV_HIGH)                    ||                           \
    ((x) == CLK_XTAL_DRV_MID)                     ||                           \
    ((x) == CLK_XTAL_DRV_LOW)                     ||                           \
    ((x) == CLK_XTAL_DRV_ULOW))

#define IS_CLK_XTAL_STB_SEL(x)                                                 \
(   ((x) == CLK_XTAL_STB_2E8)                     ||                           \
    ((x) == CLK_XTAL_STB_2E9)                     ||                           \
    ((x) == CLK_XTAL_STB_2E10)                    ||                           \
    ((x) == CLK_XTAL_STB_2E11)                    ||                           \
    ((x) == CLK_XTAL_STB_2E13)                    ||                           \
    ((x) == CLK_XTAL_STB_2E15)                    ||                           \
    ((x) == CLK_XTAL_STB_2E17))

/* Parameter valid check for XTALSTD state */
#define IS_CLK_XTALSTD_STATE(x)                                                \
(   ((x) == CLK_XTALSTD_OFF)                      ||                           \
    ((x) == CLK_XTALSTD_ON))

/* Parameter valid check for XTALSTD mode */
#define IS_CLK_XTALSTD_MD(x)                                                   \
(   ((x) == CLK_XTALSTD_MD_RST)                   ||                           \
    ((x) == CLK_XTALSTD_MD_INT))

/* Parameter valid check for XTALSTD interrupt state */
#define IS_CLK_XTALSTD_INT_STATE(x)                                            \
(   ((x) == CLK_XTALSTD_INT_OFF)                  ||                           \
    ((x) == CLK_XTALSTD_INT_ON))

/* Parameter valid check for XTALSTD reset state */
#define IS_CLK_XTALSTD_RST_STATE(x)                                            \
(   ((x) == CLK_XTALSTD_RST_OFF)                  ||                           \
    ((x) == CLK_XTALSTD_RST_ON))

/* Parameter valid check for system clock source */
#define IS_CLK_SYSCLK_SRC(x)                                                   \
(   ((x) == CLK_SYSCLK_SRC_HRC)                   ||                           \
    ((x) == CLK_SYSCLK_SRC_LRC)                   ||                           \
    ((x) == CLK_SYSCLK_SRC_XTAL))

/* Parameter valid check for CLK stable flag. */
#define IS_CLK_STB_FLAG(x)                                                     \
(   ((x) != 0x00U)                                &&                           \
    (((x) | CLK_STB_FLAG_MASK) == CLK_STB_FLAG_MASK))

/* Parameter valid check for bus clock category */
#define IS_CLK_BUS_CLK_CATE(x)                   (((x) & CLK_BUS_CLK_ALL) != (0x00U))

/* Parameter valid check for HCLK divider */
#define IS_CLK_HCLK_DIV(x)                                                     \
(   ((x) == CLK_HCLK_DIV1)                        ||                           \
    ((x) == CLK_HCLK_DIV2)                        ||                           \
    ((x) == CLK_HCLK_DIV4)                        ||                           \
    ((x) == CLK_HCLK_DIV8)                        ||                           \
    ((x) == CLK_HCLK_DIV16)                       ||                           \
    ((x) == CLK_HCLK_DIV32)                       ||                           \
    ((x) == CLK_HCLK_DIV64))

/* Parameter valid check for bus clock */
#define IS_CLK_BUS_CLK(x)                         ((x) == CLK_BUS_HCLK)

/* parameter valid check for HRC state */
#define IS_CLK_HRC_FREQ_SEL(x)                                                 \
(   ((x) == CLK_HRC_FREQ_32M)                     ||                           \
    ((x) == CLK_HRC_FREQ_16M)                     ||                           \
    ((x) == CLK_HRC_FREQ_8M)                      ||                           \
    ((x) == CLK_HRC_FREQ_4M)                      ||                           \
    ((x) == CLK_HRC_FREQ_2M)                      ||                           \
    ((x) == CLK_HRC_FREQ_1M)                      ||                           \
    ((x) == CLK_HRC_FREQ_48M)                     ||                           \
    ((x) == CLK_HRC_FREQ_24M)                     ||                           \
    ((x) == CLK_HRC_FREQ_12M)                     ||                           \
    ((x) == CLK_HRC_FREQ_6M)                      ||                           \
    ((x) == CLK_HRC_FREQ_3M)                      ||                           \
    ((x) == CLK_HRC_FREQ_1P5M))

/* Parameter valid check for peripheral source */
#define IS_CLK_PERIPHCLK_SRC(x)                                                \
(   ((x) == CLK_PERIPHCLK_HCLK_DIV1)              ||                           \
    ((x) == CLK_PERIPHCLK_HCLK_DIV2)              ||                           \
    ((x) == CLK_PERIPHCLK_HCLK_DIV4)              ||                           \
    ((x) == CLK_PERIPHCLK_HCLK_DIV8)              ||                           \
    ((x) == CLK_PERIPHCLK_HCLK_DIV16)             ||                           \
    ((x) == CLK_PERIPHCLK_HCLK_DIV32))

#define IS_CLK_ADC_FREQ(x)                        ((x) <= CLK_FREQ_32M)

/* Parameter valid check for CLK MCO clock source  . */
#define IS_CLK_MCO_SRC(x)                                                      \
(   ((x) == CLK_MCO_SRC_HRC)                      ||                           \
    ((x) == CLK_MCO_SRC_LRC)                      ||                           \
    ((x) == CLK_MCO_SRC_XTAL)                     ||                           \
    ((x) == CLK_MCO_SRC_HCLK))

/* Parameter valid check for CLK MCO clock divide. */
#define IS_CLK_MCO_DIV(x)                                                      \
(   ((x) == CLK_MCO_DIV1)                         ||                           \
    ((x) == CLK_MCO_DIV2)                         ||                           \
    ((x) == CLK_MCO_DIV4)                         ||                           \
    ((x) == CLK_MCO_DIV8)                         ||                           \
    ((x) == CLK_MCO_DIV16)                        ||                           \
    ((x) == CLK_MCO_DIV32)                        ||                           \
    ((x) == CLK_MCO_DIV64)                        ||                           \
    ((x) == CLK_MCO_DIV128))

/* Parameter valid check for CLK MCO channel. */
#define IS_CLK_MCO_CH(x)                          ((x) == CLK_MCO1)
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
 * @defgroup CLK_Local_Functions CLK Local Functions
 * @{
 */
/**
 * @brief Clk delay function
 * @param [in] u32Delay         count
 * @retval when switch clock srouce,shoud be delay some time to wait stable.
 */
static void CLK_Delay(uint32_t u32Delay)
{
    __IO uint32_t u32Timeout = 0UL;

    while (u32Timeout < u32Delay) {
        u32Timeout++;
    }
}

/**
 * @brief  Wait clock stable flag.
 * @param  [in] u8Flag      Specifies the stable flag to be wait. @ref CLK_STB_Flag
 * @param  [in] u32Time     Specifies the time to wait while the flag not be set.
 * @retval int32_t
 */
static int32_t CLK_WaitStable(uint8_t u8Flag, uint32_t u32Time)
{
    __IO uint32_t u32Timeout = 0UL;
    int32_t i32Ret = LL_ERR_TIMEOUT;

    while (u32Timeout <= u32Time) {
        if (SET == CLK_GetStableStatus(u8Flag)) {
            i32Ret = LL_OK;
            break;
        }
        u32Timeout++;
    }
    return i32Ret;
}

static void SetSysClockSrc(uint8_t u8Src)
{
    DDL_ASSERT(IS_CLK_SYSCLK_SRC(u8Src));
    DDL_ASSERT(IS_CLK_UNLOCKED());

    /* Set system clock source */
    WRITE_REG8(CM_CMU->CKSWR, u8Src);
    /* Update system clock */
    SystemCoreClockUpdate();
}

static void GetClockFreq(stc_clock_freq_t *pstcClockFreq)
{
    uint32_t u32HrcValueMax;
    /* Get system clock. */
    switch (READ_REG8_BIT(CM_CMU->CKSWR, CMU_CKSWR_CKSW)) {
        case CLK_SYSCLK_SRC_HRC: {
            if (EFM_HRCCFGR_HRCFREQS_3 == (CM_EFM->HRCCFGR & EFM_HRCCFGR_HRCFREQS_3)) {
                u32HrcValueMax = CLK_FREQ_48M;
            } else {
                u32HrcValueMax = CLK_FREQ_32M;
            }
            pstcClockFreq->u32SysclkFreq = u32HrcValueMax >> (READ_REG8_BIT(CM_EFM->HRCCFGR, EFM_HRC_FREQ_CFG));
            break;
        }
        case CLK_SYSCLK_SRC_XTAL:
            pstcClockFreq->u32SysclkFreq = XTAL_VALUE;
            break;
        case CLK_SYSCLK_SRC_LRC:
            pstcClockFreq->u32SysclkFreq = LRC_VALUE;
            break;
        default:
            break;
    }
    /* Get hclk. */
    pstcClockFreq->u32HclkFreq = pstcClockFreq->u32SysclkFreq >> (READ_REG8_BIT(CM_CMU->SCKDIVR, CMU_SCKDIVR_SCKDIV));
}

static void SetSysClockDiv(uint32_t u32Clock, uint32_t u32Div)
{
    DDL_ASSERT(IS_CLK_BUS_CLK_CATE(u32Clock));
    DDL_ASSERT(IS_CLK_HCLK_DIV(u32Div));
    DDL_ASSERT(IS_CLK_UNLOCKED());

    /* Set system clock divider */
    WRITE_REG8(CM_CMU->SCKDIVR, (uint32_t)u32Div);
}
/**
* @}
*/

/**
 * @defgroup CLK_Global_Functions CLK Global Functions
 * @{
 */
/**
 * @brief  LRC function enable/disable.
 * @param  [in] enNewState An @ref en_functional_state_t enumeration value.
 * @retval int32_t:
 *         - LL_OK: LRC operate successfully
 *         - LL_ERR_BUSY: LRC is the system clock, CANNOT stop it.
 * @note   DO NOT STOP LRC while using it as system clock.
 */
int32_t CLK_LrcCmd(en_functional_state_t enNewState)
{
    int32_t i32Ret = LL_OK;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_CLK_UNLOCKED());

    if (DISABLE == enNewState) {
        if (CLK_SYSCLK_SRC_LRC == READ_REG8_BIT(CM_CMU->CKSWR, CMU_CKSWR_CKSW)) {
            i32Ret = LL_ERR_BUSY;
        } else {
            WRITE_REG8(CM_CMU->LRCCR, CLK_LRC_OFF);
        }
    } else {
        WRITE_REG8(CM_CMU->LRCCR, CLK_LRC_ON);

        CLK_Delay(CLK_LRC_TIMEOUT);
    }

    return i32Ret;
}

/**
 * @brief  Configure the Hrc frequency.
 * @param  [in] u8HrcFreq           Specifies the frequency of HRC.  @ref CLK_HrcFreq_Sel
 * @retval int32_t:
 *         - LL_OK: HRC configuration successful.
 *         - LL_ERR_TIMEOUT: Timeout.
 */
int32_t CLK_HrcFreqConfig(uint8_t u8HrcFreq)
{
    int32_t i32Ret;

    DDL_ASSERT(IS_CLK_HRC_FREQ_SEL(u8HrcFreq));
    DDL_ASSERT(IS_CLK_UNLOCKED());
    DDL_ASSERT(IS_CLK_EFM_UNLOCK());

    /* Configure Hrc frequency */
    WRITE_REG8(CM_EFM->HRCCFGR, u8HrcFreq);
    /* Wait Hrc stable */
    i32Ret = CLK_WaitStable(CLK_STB_FLAG_HRC, CLK_TIMEOUT);
    /* Update system clock */
    SystemCoreClockUpdate();

    return i32Ret;
}

/**
 * @brief  HRC function enable/disable.
 * @param  [in] enNewState An @ref en_functional_state_t enumeration value.
 * @retval int32_t:
 *         - LL_OK: HRC operate successfully
 *         - LL_ERR_BUSY: HRC is the system clock or as the PLL source clock, CANNOT stop it.
 *         - LL_ERR_TIMEOUT: HRC operate Timeout
 * @note   DO NOT STOP HRC while using it as system clock or as the PLL source clock.
 */
int32_t CLK_HrcCmd(en_functional_state_t enNewState)
{
    int32_t i32Ret = LL_OK;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_CLK_UNLOCKED());

    if (DISABLE == enNewState) {
        if (CLK_SYSCLK_SRC_HRC == READ_REG8_BIT(CM_CMU->CKSWR, CMU_CKSWR_CKSW)) {
            i32Ret = LL_ERR_BUSY;
        } else {
            WRITE_REG8(CM_CMU->HRCCR, CLK_HRC_OFF);
        }
    } else {
        WRITE_REG8(CM_CMU->HRCCR, CLK_HRC_ON);
        i32Ret = CLK_WaitStable(CLK_STB_FLAG_HRC, CLK_TIMEOUT);
    }

    return i32Ret;
}

/**
 * @brief  Init Xtal initial structure with default value.
 * @param  [in] pstcXtalInit specifies the Parameter of XTAL.
 * @retval int32_t:
 *         - LL_OK: Initialize success
 *         - LL_ERR_INVD_PARAM: NULL pointer
 */
int32_t CLK_XtalStructInit(stc_clock_xtal_init_t *pstcXtalInit)
{
    int32_t i32Ret = LL_OK;

    /* Check if pointer is NULL */
    if (NULL == pstcXtalInit) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        /* Configure to default value */
        pstcXtalInit->u8State = CLK_XTAL_OFF;
        pstcXtalInit->u8Mode  = CLK_XTAL_MD_OSC;
        pstcXtalInit->u8Drv   = CLK_XTAL_DRV_HIGH;
        pstcXtalInit->u8SuperDrv = CLK_XTAL_SUPDRV_ON;
        pstcXtalInit->u8StableTime = CLK_XTAL_STB_2E8;
    }
    return i32Ret;
}

/**
 * @brief  XTAL initialize.
 * @param  [in] pstcXtalInit specifies the XTAL initial config.
 * @retval int32_t:
 *         - LL_OK: XTAL initial successfully.
 *         - LL_ERR_TIMEOUT: XTAL operate timeout.
 *         - LL_ERR_BUSY: XTAL is the system clock, CANNOT stop it.
 *         - LL_ERR_INVD_PARAM: NULL pointer.
 * @note   DO NOT STOP XTAL while using it as system clock.
 */
int32_t CLK_XtalInit(const stc_clock_xtal_init_t *pstcXtalInit)
{
    int32_t i32Ret;

    if (NULL == pstcXtalInit) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        DDL_ASSERT(IS_CLK_XTAL_STATE(pstcXtalInit->u8State));
        DDL_ASSERT(IS_CLK_XTAL_DRV_MD(pstcXtalInit->u8Drv));
        DDL_ASSERT(IS_CLK_XTAL_MD(pstcXtalInit->u8Mode));
        DDL_ASSERT(IS_CLK_XTAL_SUPDRV_STATE(pstcXtalInit->u8SuperDrv));
        DDL_ASSERT(IS_CLK_XTAL_STB_SEL(pstcXtalInit->u8StableTime));
        DDL_ASSERT(IS_CLK_UNLOCKED());

        WRITE_REG8(CM_CMU->XTALSTBCR, pstcXtalInit->u8StableTime);
        WRITE_REG8(CM_CMU->XTALCFGR, (pstcXtalInit->u8SuperDrv | pstcXtalInit->u8Drv | pstcXtalInit->u8Mode));
        if (CLK_XTAL_ON == pstcXtalInit->u8State) {
            i32Ret = CLK_XtalCmd(ENABLE);
        } else {
            i32Ret = CLK_XtalCmd(DISABLE);
        }
    }

    return i32Ret;
}

/**
 * @brief  XTAL function enable/disable.
 * @param  [in] enNewState An @ref en_functional_state_t enumeration value.
 * @retval int32_t:
 *         - LL_OK: XTAL operate successfully
 *         - LL_ERR_BUSY: XTAL is the system clock or as the PLL source clock, CANNOT stop it.
 *         - LL_ERR_TIMEOUT: XTAL operate timeout.
 * @note   DO NOT STOP XTAL while using it as system clock or as the PLL source clock.
 */
int32_t CLK_XtalCmd(en_functional_state_t enNewState)
{
    int32_t i32Ret = LL_OK;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_CLK_UNLOCKED());

    if (DISABLE == enNewState) {
        if (CLK_SYSCLK_SRC_XTAL == READ_REG8_BIT(CM_CMU->CKSWR, CMU_CKSWR_CKSW)) {
            i32Ret = LL_ERR_BUSY;
        } else {
            WRITE_REG8(CM_CMU->XTALCR, CLK_XTAL_OFF);
        }
    } else {
        WRITE_REG8(CM_CMU->XTALCR, CLK_XTAL_ON);
        CLK_Delay(CLK_TIMEOUT);
    }

    return i32Ret;
}

/**
 * @brief  Init XtalStd initial structure with default value.
 * @param  [in] pstcXtalStdInit specifies the Parameter of XTALSTD.
 * @retval int32_t:
 *         - LL_OK: Initialize success
 *         - LL_ERR_INVD_PARAM: Invalid parameter
 */
int32_t CLK_XtalStdStructInit(stc_clock_xtalstd_init_t *pstcXtalStdInit)
{
    int32_t i32Ret = LL_OK;

    /* Check if pointer is NULL */
    if (NULL == pstcXtalStdInit) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        /* Configure to default value */
        pstcXtalStdInit->u8State = CLK_XTALSTD_OFF;
        pstcXtalStdInit->u8Mode  = CLK_XTALSTD_MD_INT;
        pstcXtalStdInit->u8Int   = CLK_XTALSTD_INT_OFF;
        pstcXtalStdInit->u8Reset = CLK_XTALSTD_RST_OFF;
    }

    return i32Ret;
}

/**
 * @brief  Initialise the XTAL status detection.
 * @param  [in] pstcXtalStdInit specifies the Parameter of XTALSTD.
 *   @arg  u8State: The new state of the XTALSTD.
 *   @arg  u8Mode:  The XTAL status detection occur interrupt or reset.
 *   @arg  u8Int:   The XTAL status detection interrupt on or off.
 *   @arg  u8Reset:   The XTAL status detection reset on or off.
 * @retval int32_t:
 *         - LL_OK: Initialize success
 *         - LL_ERR_INVD_PARAM: Invalid parameter
 */
int32_t CLK_XtalStdInit(const stc_clock_xtalstd_init_t *pstcXtalStdInit)
{
    int32_t i32Ret = LL_OK;

    /* Check if pointer is NULL */
    if (NULL == pstcXtalStdInit) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        /* Parameter valid check */
        DDL_ASSERT(IS_CLK_XTALSTD_STATE(pstcXtalStdInit->u8State));
        DDL_ASSERT(IS_CLK_UNLOCKED());
        /* Parameter valid check */
        DDL_ASSERT(IS_CLK_XTALSTD_MD(pstcXtalStdInit->u8Mode));
        DDL_ASSERT(IS_CLK_XTALSTD_INT_STATE(pstcXtalStdInit->u8Int));
        DDL_ASSERT(IS_CLK_XTALSTD_RST_STATE(pstcXtalStdInit->u8Reset));

        /* Configure and enable XTALSTD */
        WRITE_REG8(CM_CMU->XTALSTDCR, (pstcXtalStdInit->u8State |   \
                                       pstcXtalStdInit->u8Mode  |   \
                                       pstcXtalStdInit->u8Int   |   \
                                       pstcXtalStdInit->u8Reset));
    }

    return i32Ret;
}

/**
 * @brief  Clear the XTAL error flag.
 * @param  None
 * @retval None
 * @note   The system clock should not be XTAL before call this function.
 */
void CLK_ClearXtalStdStatus(void)
{
    DDL_ASSERT(IS_CLK_UNLOCKED());

    if (0x01U == READ_REG8(CM_CMU->XTALSTDSR)) {
        /* Clear the XTAL STD flag */
        WRITE_REG8(CM_CMU->XTALSTDSR, 0x00U);
    }
}

/**
 * @brief  Get the XTAL error flag.
 * @param  None
 * @retval An @ref en_flag_status_t enumeration type value.
 */
en_flag_status_t CLK_GetXtalStdStatus(void)
{
    return ((0x00U != READ_REG32(CM_CMU->XTALSTDSR)) ? SET : RESET);
}

/**
 * @brief  Selects the clock source to output on MCO pin.
 * @param  [in] u8Ch        Specifies the MCO channel. @ref CLK_MCO_Channel_Sel
 * @param  [in] u8Src       Specifies the clock source to output. @ref CLK_MCO_Clock_Source
 * @param  [in] u8Div       Specifies the MCOx prescaler. @ref CLK_MCO_Clock_Prescaler
 * @retval None
 * @note   MCO pin should be configured in alternate function 1 mode.
 */
void CLK_MCOConfig(uint8_t u8Ch, uint8_t u8Src, uint8_t u8Div)
{
    __IO uint8_t *MCOCFGRx;

    /* Check the parameters. */
    DDL_ASSERT(IS_CLK_MCO_SRC(u8Src));
    DDL_ASSERT(IS_CLK_MCO_DIV(u8Div));
    DDL_ASSERT(IS_CLK_MCO_CH(u8Ch));
    /* enable register write. */
    DDL_ASSERT(IS_CLK_UNLOCKED());

    MCOCFGRx = &(*(__IO uint8_t *)((uint32_t)&CM_CMU->MCO1CFGR + u8Ch));
    /* Config the MCO */
    MODIFY_REG8(*MCOCFGRx, (CMU_MCOCFGR_MCOSEL | CMU_MCOCFGR_MCODIV), (u8Src | u8Div));
}

/**
 * @brief  Enable or disable the MCO1 output.
 * @param  [in] u8Ch        Specifies the MCO channel. @ref CLK_MCO_Channel_Sel
 * @param  [in] enNewState  An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void CLK_MCOCmd(uint8_t u8Ch, en_functional_state_t enNewState)
{
    __IO uint8_t *MCOCFGRx;

    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_CLK_UNLOCKED());
    DDL_ASSERT(IS_CLK_MCO_CH(u8Ch));

    MCOCFGRx = &(*(__IO uint8_t *)((uint32_t)&CM_CMU->MCO1CFGR + u8Ch));
    /* Enable or disable clock output. */
    MODIFY_REG8(*MCOCFGRx, CMU_MCOCFGR_MCOEN, (uint8_t)enNewState << CMU_MCOCFGR_MCOEN_POS);
}

/**
 * @brief  PLL/XTAL/HRC stable flag read.
 * @param  [in] u8Flag      specifies the stable flag to be read. @ref CLK_STB_Flag
 * @retval An @ref en_flag_status_t enumeration type value.
 */
en_flag_status_t CLK_GetStableStatus(uint8_t u8Flag)
{
    DDL_ASSERT(IS_CLK_STB_FLAG(u8Flag));

    return ((0x00U != READ_REG8_BIT(CM_CMU->OSCSTBSR, u8Flag)) ? SET : RESET);
}

/**
 * @brief  Set the system clock source.
 * @param  [in] u8Src specifies the source of system clock. @ref CLK_System_Clock_Source
 * @retval None
 */
void CLK_SetSysClockSrc(uint8_t u8Src)
{
    /* Set system clock source */
    SetSysClockSrc(u8Src);
    /* Update system clock */
    SystemCoreClockUpdate();
}

/**
 * @brief  Get bus clock frequency.
 * @param  [out] pstcClockFreq specifies the pointer to get bus frequency.
 * @retval int32_t:
 *         - LL_OK: Initialize success
 *         - LL_ERR_INVD_PARAM: NULL pointer
 */
int32_t CLK_GetClockFreq(stc_clock_freq_t *pstcClockFreq)
{
    int32_t i32Ret = LL_OK;

    if (NULL == pstcClockFreq) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        GetClockFreq(pstcClockFreq);
    }
    return i32Ret;
}

/**
 * @brief  Get bus clock frequency.
 * @param  [in] u32Clock specifies the bus clock to get frequency. @ref CLK_Bus_Clock_Sel
 * @retval int32_t:
 *         - LL_OK: Initialize success
 *         - LL_ERR_INVD_PARAM: NULL pointer
 */
uint32_t CLK_GetBusClockFreq(uint32_t u32Clock)
{
    uint32_t u32ClockFreq;
    DDL_ASSERT(IS_CLK_BUS_CLK(u32Clock));

    switch (u32Clock) {
        case CLK_BUS_HCLK:
            u32ClockFreq = SystemCoreClock >> (READ_REG8_BIT(CM_CMU->SCKDIVR, CMU_SCKDIVR_SCKDIV));
            break;
        default:
            u32ClockFreq = SystemCoreClock;
            break;
    }
    return u32ClockFreq;
}

/**
 * @brief  HCLK/PCLK divide setting.
 * @param  [in] u32Clock specifies the clock to be divided.  @ref CLK_Bus_Clock_Sel
 * @param  [in] u32Div specifies the clock divide factor. @ref CLK_Clock_Divider
 * @retval None
 * @note   u32Div could choose CLK_HCLK_Divider, CLK_PCLK0_Divider, CLK_PCLK1_Divider,
 * CLK_PCLK2_Divider, CLK_PCLK3_Divider, CLK_PCLK4_Divider, CLK_EXCLK_Divider, according to the MCU
 */
void CLK_SetClockDiv(uint32_t u32Clock, uint32_t u32Div)
{
    /* Set clock divider */
    SetSysClockDiv(u32Clock, u32Div);
    /* Update system clock */
    SystemCoreClockUpdate();
}

/**
 * @brief  Set peripheral clock source.
 * @param  [in] u16Src specifies the peripheral clock source. @ref CLK_PERIPH_Sel
 * @retval None
 * @note   peripheral only for ADC
 */
void CLK_SetPeriClockSrc(uint16_t u16Src)
{
    DDL_ASSERT(IS_CLK_PERIPHCLK_SRC(u16Src));
    DDL_ASSERT(IS_CLK_UNLOCKED());
    DDL_ASSERT(IS_CLK_ADC_FREQ(CLK_GetBusClockFreq(CLK_BUS_HCLK) / (u16Src + 1UL)));

    WRITE_REG8(CM_CMU->PERICKSEL, u16Src);
}

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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
