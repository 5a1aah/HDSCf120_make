/**
 *******************************************************************************
 * @file  hc32_ll_interrupts.c
 * @brief This file provides firmware functions to manage the Interrupt Controller
 *        (INTC).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2023-01-15       CDT             Add macro-definition: EIRQFR_REG/NMIENR_REG/INTWKEN_REG.
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
#include "hc32_ll_interrupts.h"
#include "hc32_ll_utility.h"

/**
 * @addtogroup LL_Driver
 * @{
 */

/**
 * @defgroup LL_INTERRUPTS INTERRUPTS
 * @brief INTC Driver Library
 * @{
 */

#if (LL_INTERRUPTS_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup INTC_Local_Macros INTC Local Macros
 * @{
 */
/**
 * @brief   Maximum IRQ handler number
 */
#define IRQ_NUM_MAX             (16U)
#define IRQn_MIN                (INT008_IRQn)
#define IRQn_MAX                (INT023_IRQn)
#define IRQn_OFFSET             (8U)
#define EIRQCFR_REG             (CM_INTC->EIRQCLR)
#define EIRQFR_REG              (CM_INTC->EIRQFR)
#define NMIENR_REG              (CM_INTC->NMIENR)
#define NMICFR_REG              (CM_INTC->NMICLR)
#define INTWKEN_REG             (CM_INTC->WUPEN)
#define INTSEL_REG              (uint32_t)(&CM_INTC->ISELAR8)
#define INTSEL_RST_VALUE        (0x00U)
#define IRQ_GRP_MOD             (16UL)
#define IRQ_GRP_NUM             (2UL)
#define IRQ_GRP_LOW             (8UL)
#define IRQ_GRP_HIGH            (9UL)
#define EXTINT_CH_NUM_MAX       (10U)

/**
 * @defgroup INTC_Check_Parameters_Validity INTC Check Parameters Validity
 * @{
 */
/*! Parameter validity check for wakeup source from stop mode. */
#define IS_INTC_WKUP_SRC(src)                                                   \
(   ((src) != 0x00UL)                           &&                              \
    (((src) | INTC_WUPEN_ALL) == INTC_WUPEN_ALL))

/*! Parameter validity check for event index. */
#define IS_INTC_EVT(event)                                                      \
(   ((event) != 0x00UL)                         &&                              \
    (((event) | INTC_EVT_ALL) == INTC_EVT_ALL))

/*! Parameter validity check for NMI trigger source. */
#define IS_NMI_SRC(src)                                                         \
(   ((src) != 0x00UL)                           &&                              \
    (((src) | NMI_SRC_ALL) == NMI_SRC_ALL))

/*! Parameter validity check for NMI trigger edge. */
#define IS_NMI_TRIG(trigger)                                                    \
(   ((trigger) == NMI_TRIG_FALLING)             ||                              \
    ((trigger) == NMI_TRIG_RISING))

/*! Parameter validity check for NMI filter A function. */
#define IS_NMI_FAE(fae)                                                         \
(   ((fae) == NMI_FILTER_OFF)                   ||                              \
    ((fae) == NMI_FILTER_ON))

/*! Parameter validity check for NMI filter A clock division. */
#define IS_NMI_FACLK(faclk)                                                     \
(   ((faclk) == NMI_FCLK_DIV1)                  ||                              \
    ((faclk) == NMI_FCLK_DIV8)                  ||                              \
    ((faclk) == NMI_FCLK_DIV32)                 ||                              \
    ((faclk) == NMI_FCLK_DIV64))

/*! Parameter validity check for EXTINT filter A function. */
#define IS_EXTINT_FAE(fae)                                                      \
(   ((fae) == EXTINT_FILTER_OFF)                ||                              \
    ((fae) == EXTINT_FILTER_ON))

/*! Parameter validity check for EXTINT filter A clock division. */
#define IS_EXTINT_FACLK(faclk)                                                  \
(   ((faclk) == EXTINT_FCLK_DIV1)               ||                              \
    ((faclk) == EXTINT_FCLK_DIV8)               ||                              \
    ((faclk) == EXTINT_FCLK_DIV32)              ||                              \
    ((faclk) == EXTINT_FCLK_DIV64))

/*! Parameter validity check for EXTINT trigger edge. */
#define IS_EXTINT_TRIG(trigger)                                                 \
(   ((trigger) == EXTINT_TRIG_LOW)              ||                              \
    ((trigger) == EXTINT_TRIG_RISING)           ||                              \
    ((trigger) == EXTINT_TRIG_FALLING)          ||                              \
    ((trigger) == EXTINT_TRIG_BOTH))

/*! Parameter validity check for EXTINT channel. */
#define IS_EXTINT_CH(ch)                                                        \
(   ((ch) != 0x00UL)                            &&                              \
    (((ch) | EXTINT_CH_ALL) == EXTINT_CH_ALL))

/* Parameter validity check for INTC register lock status. */
#define IS_INTC_UNLOCK()            (INTC_REG_UNLOCK_KEY == (CM_INTC->FPRCR))

/*  Parameter validity check for EKEY. */
#define IS_INTC_EKEY(ekey)          (((ekey) & EKEY_CH_ALL) != 0x00U)
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
/**
 * @defgroup INTC_Local_Variable INTC Local Variable
 * @{
 */
static func_ptr_t m_apfnIrqHandler[IRQ_NUM_MAX] = {NULL};
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
 * @defgroup INTC_Global_Functions INTC Global Functions
 * @{
 */
/**
 * @brief  IRQ sign in function
 * @param  [in] pstcIrqSignConfig: pointer of IRQ registration structure
 *   @arg  enIntSrc: can be any value @ref en_int_src_t
 *   @arg  enIRQn: can be any value from IRQn_MIN ~ IRQn_MAX for different product
 *   @arg  pfnCallback: Callback function
 * @retval int32_t:
 *           - LL_OK: IRQ register successfully
 *           - LL_ERR_INVD_PARAM: IRQ No. and Peripheral Int source are not match; NULL pointer.
 *           - LL_ERR_UNINIT: Specified IRQ entry was signed before.
 */
int32_t INTC_IrqSignIn(const stc_irq_signin_config_t *pstcIrqSignConfig)
{
    __IO uint32_t *ISELRx;
    int32_t i32Ret = LL_OK;

    /* Check if pointer is NULL */
    if (NULL == pstcIrqSignConfig) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        DDL_ASSERT(pstcIrqSignConfig->enIntSrc <= INT_SRC_MAX);
        /* Checking validity for Interrupt source of Group 0~7 and IRQ No. */
        if ((((uint32_t)pstcIrqSignConfig->enIntSrc / IRQ_GRP_MOD) * IRQ_GRP_NUM + IRQ_GRP_LOW !=               \
                (uint32_t)pstcIrqSignConfig->enIRQn)                                                    &&      \
                (((uint32_t)pstcIrqSignConfig->enIntSrc / IRQ_GRP_MOD) * IRQ_GRP_NUM + IRQ_GRP_HIGH !=          \
                 (uint32_t)pstcIrqSignConfig->enIRQn)) {
            i32Ret = LL_ERR_INVD_PARAM;
        } else {
            /* EIRQ0~7 are fixed allocation of IRQ handler 0~7 */
            if ((0U == (uint32_t)pstcIrqSignConfig->enIntSrc % IRQ_GRP_MOD)                             ||      \
                    ((uint32_t)pstcIrqSignConfig->enIRQn < (uint32_t)IRQn_MIN)) {
                i32Ret = LL_ERR_INVD_PARAM;
            } else {
                DDL_ASSERT(IS_INTC_UNLOCK());
                ISELRx = (__IO uint32_t *)(INTSEL_REG + (4U * ((uint32_t)pstcIrqSignConfig->enIRQn - IRQn_OFFSET)));
                WRITE_REG32(*ISELRx, ((uint32_t)pstcIrqSignConfig->enIntSrc) % IRQ_GRP_MOD);
                /* Callback function */
                m_apfnIrqHandler[(uint32_t)pstcIrqSignConfig->enIRQn - IRQn_OFFSET] = pstcIrqSignConfig->pfnCallback;
            }
        }
    }
    return i32Ret;
}

/**
 * @brief  IRQ sign out function
 * @param  [in] enIRQn: can be any value from IRQn_MIN ~ IRQn_MAX for different product
 * @retval int32_t:
 *           - LL_OK: IRQ sign out successfully
 *           - LL_ERR_INVD_PARAM: IRQ No. is out of range
 */
int32_t INTC_IrqSignOut(IRQn_Type enIRQn)
{
    __IO uint32_t *INTC_SELx;
    int32_t i32Ret = LL_OK;

    if ((enIRQn < IRQn_MIN) || (enIRQn > IRQn_MAX)) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        DDL_ASSERT(IS_INTC_UNLOCK());
        INTC_SELx = (__IO uint32_t *)(INTSEL_REG + (4UL * (uint32_t)enIRQn));
        WRITE_REG32(*INTC_SELx, INTSEL_RST_VALUE);
        m_apfnIrqHandler[(uint8_t)enIRQn - IRQn_OFFSET] = NULL;
    }
    return i32Ret;
}

/**
 * @brief  Stop mode wake-up source configure
 * @param  [in] u32WakeupSrc: Wake-up source, @ref INTC_Stop_Wakeup_Source_Sel for details
 * @param  [in] enNewState: An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void INTC_WakeupSrcCmd(uint32_t u32WakeupSrc, en_functional_state_t enNewState)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_INTC_WKUP_SRC(u32WakeupSrc));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_INTC_UNLOCK());

    if (ENABLE == enNewState) {
        SET_REG32_BIT(INTWKEN_REG, u32WakeupSrc);
    } else {
        CLR_REG32_BIT(INTWKEN_REG, u32WakeupSrc);
    }
}

/**
 * @brief  Event or Interrupt output configure
 * @param  [in] u32Event: Event index, @ref INTC_Event_Channel_Sel for details
 * @param  [in] enNewState: An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void INTC_EventCmd(uint32_t u32Event, en_functional_state_t enNewState)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_INTC_EVT(u32Event));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_INTC_UNLOCK());

    if (ENABLE == enNewState) {
        SET_REG32_BIT(CM_INTC->EVTER, u32Event);
    } else {
        CLR_REG32_BIT(CM_INTC->EVTER, u32Event);
    }
}

/**
 * @brief  Configure the specified EKEY
 * @param  [in] u8EKey: EKEY channel, @ref INTC_EKEY_Channel for details
 * @param  [in] enNewState: An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void INTC_EKeyCmd(uint8_t u8EKey, en_functional_state_t enNewState)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_INTC_EKEY(u8EKey));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_INTC_UNLOCK());

    if (ENABLE == enNewState) {
        SET_REG8_BIT(CM_INTC->EKEYCR, u8EKey);
    } else {
        CLR_REG8_BIT(CM_INTC->EKEYCR, u8EKey);
    }
}

/**
 * @brief  Initialize NMI. Fill each pstcNmiInit with default value
 * @param  [in] pstcNmiInit: Pointer to a stc_nmi_init_t structure that
 *                             contains configuration information.
 * @retval int32_t:
 *           - LL_OK: NMI structure initialize successful
 *           - LL_ERR_INVD_PARAM: NULL pointer
 */
int32_t NMI_StructInit(stc_nmi_init_t *pstcNmiInit)
{
    int32_t i32Ret = LL_OK;

    /* Check if pointer is NULL */
    if (NULL == pstcNmiInit) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        /* Configure to default value */
        pstcNmiInit->u32Src        = 0UL;
        pstcNmiInit->u32Edge = NMI_TRIG_FALLING;
        pstcNmiInit->u32Filter = NMI_FILTER_OFF;
        pstcNmiInit->u32FilterClock = NMI_FCLK_DIV1;
    }
    return i32Ret;
}

/**
 * @brief  Initialize NMI.
 * @param  [in] pstcNmiInit: Pointer to a pstcNmiInit structure that
 *                             contains configuration information.
 * @retval int32_t:
 *           - LL_OK: NMI initialize successful
 *           - LL_ERR_INVD_PARAM: NULL pointer
 */
int32_t NMI_Init(const stc_nmi_init_t *pstcNmiInit)
{
    int32_t i32Ret = LL_OK;
    uint32_t u32NMICR = 0UL;

    /* Check if pointer is NULL */
    if (NULL == pstcNmiInit) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        /* Parameter validity checking */
        DDL_ASSERT(IS_NMI_SRC(pstcNmiInit->u32Src));
        DDL_ASSERT(IS_INTC_UNLOCK());
        /* Clear all NMI trigger source before set */
        WRITE_REG32(NMICFR_REG, NMI_SRC_ALL);

        /* NMI trigger source configure */
        WRITE_REG32(NMIENR_REG, pstcNmiInit->u32Src);

        DDL_ASSERT(IS_NMI_TRIG(pstcNmiInit->u32Edge));
        DDL_ASSERT(IS_NMI_FAE(pstcNmiInit->u32Filter));
        DDL_ASSERT(IS_NMI_FACLK(pstcNmiInit->u32FilterClock));
        u32NMICR |= pstcNmiInit->u32Edge | pstcNmiInit->u32Filter | pstcNmiInit->u32FilterClock;
        WRITE_REG32(CM_INTC->NMICR, u32NMICR);
    }
    return i32Ret;
}

/**
 * @brief  Get NMI trigger source
 * @param  [in] u32Src: NMI trigger source, @ref NMI_TriggerSrc_Sel for details
 * @retval An @ref en_flag_status_t enumeration type value.
 */
en_flag_status_t NMI_GetNmiStatus(uint32_t u32Src)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_NMI_SRC(u32Src));

    return (((READ_REG32(CM_INTC->NMIFR) & u32Src)) != 0UL) ? SET : RESET;
}

/**
 * @brief  Set NMI trigger source
 * @param  [in] u32Src: NMI trigger source, @ref NMI_TriggerSrc_Sel for details
 * @param  [in] enNewState: An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void NMI_NmiSrcCmd(uint32_t u32Src, en_functional_state_t enNewState)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_NMI_SRC(u32Src));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_INTC_UNLOCK());

    if (ENABLE == enNewState) {
        SET_REG32_BIT(NMIENR_REG, u32Src);
    } else {
        CLR_REG32_BIT(NMIENR_REG, u32Src);
    }
}

/**
 * @brief  Clear specified NMI trigger source
 * @param  [in] u32Src: NMI trigger source, @ref NMI_TriggerSrc_Sel for diff. MCU in details
 * @retval None
 */
void NMI_ClearNmiStatus(uint32_t u32Src)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_NMI_SRC(u32Src));
    DDL_ASSERT(IS_INTC_UNLOCK());

    SET_REG32_BIT(NMICFR_REG, u32Src);
}

/**
 * @brief  Initialize External interrupt.
 * @param  [in] u32Ch: ExtInt channel.
 * @param  [in] pstcExtIntInit: Pointer to a stc_extint_init_t structure that
 *                             contains configuration information.
 * @retval int32_t:
 *           - LL_OK:  EXTINT initialize successful
 *           - LL_ERR_INVD_PARAM: NULL pointer
 */
int32_t EXTINT_Init(uint32_t u32Ch, const stc_extint_init_t *pstcExtIntInit)
{
    uint8_t u8ExtIntPos;
    int32_t i32Ret = LL_OK;
    uint32_t EIRQCRVal;
    __IO uint32_t *EIRQCRx;

    /* Check if pointer is NULL */
    if (NULL == pstcExtIntInit) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        /* Parameter validity checking */
        DDL_ASSERT(IS_INTC_UNLOCK());
        DDL_ASSERT(IS_EXTINT_CH(u32Ch));
        DDL_ASSERT(IS_EXTINT_FAE(pstcExtIntInit->u32Filter));
        DDL_ASSERT(IS_EXTINT_FACLK(pstcExtIntInit->u32FilterClock));
        DDL_ASSERT(IS_EXTINT_TRIG(pstcExtIntInit->u32Edge));
        for (u8ExtIntPos = 0U; u8ExtIntPos < EXTINT_CH_NUM_MAX; u8ExtIntPos++) {
            if (0UL != (u32Ch & (1UL << u8ExtIntPos))) {
                EIRQCRVal = pstcExtIntInit->u32Filter | pstcExtIntInit->u32FilterClock  |   \
                            pstcExtIntInit->u32Edge;
                EIRQCRx = (__IO uint32_t *)((uint32_t)&CM_INTC->EIRQCR0 + 4UL * u8ExtIntPos);
                WRITE_REG32(*EIRQCRx, EIRQCRVal);
            }
        }
    }
    return i32Ret;
}

/**
 * @brief  Initialize ExtInt. Fill each pstcExtIntInit with default value
 * @param  [in] pstcExtIntInit: Pointer to a stc_extint_init_t structure
 *                              that contains configuration information.
 * @retval int32_t:
 *           - LL_OK: EXTINT structure initialize successful
 *           - LL_ERR_INVD_PARAM: NULL pointer
 */
int32_t EXTINT_StructInit(stc_extint_init_t *pstcExtIntInit)
{
    int32_t i32Ret = LL_OK;

    /* Check if pointer is NULL */
    if (NULL == pstcExtIntInit) {
        i32Ret = LL_ERR_INVD_PARAM;
    } else {
        /* Configure to default value */
        pstcExtIntInit->u32Filter      = EXTINT_FILTER_OFF;
        pstcExtIntInit->u32FilterClock = EXTINT_FCLK_DIV1;
        pstcExtIntInit->u32Edge        = EXTINT_TRIG_FALLING;
    }
    return i32Ret;
}

/**
 * @brief  Clear specified External interrupt trigger source
 * @param  [in] u32ExtIntCh: External interrupt channel, @ref EXTINT_Channel_Sel for details
 * @retval None
 */
void EXTINT_ClearExtIntStatus(uint32_t u32ExtIntCh)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_EXTINT_CH(u32ExtIntCh));
    DDL_ASSERT(IS_INTC_UNLOCK());

    SET_REG32_BIT(EIRQCFR_REG, u32ExtIntCh);
}

/**
 * @brief  Get specified External interrupt trigger source
 * @param  [in] u32ExtIntCh: External interrupt channel, @ref EXTINT_Channel_Sel for details
 * @retval An @ref en_flag_status_t enumeration type value.
 */
en_flag_status_t EXTINT_GetExtIntStatus(uint32_t u32ExtIntCh)
{
    /* Parameter validity checking */
    DDL_ASSERT(IS_EXTINT_CH(u32ExtIntCh));

    return ((READ_REG16(EIRQFR_REG) & u32ExtIntCh) != 0U) ? SET : RESET;
}

/**
 * @brief  Interrupt No.008 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ008_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT008_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.009 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ009_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT009_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.010 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ010_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT010_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.011 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ011_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT011_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.012 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ012_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT012_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.013 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ013_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT013_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.014 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ014_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT014_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.015 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ015_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT015_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.016 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ016_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT016_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.017 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ017_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT017_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.018 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ018_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT018_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.019 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ019_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT019_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.020 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ020_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT020_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.021 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ021_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT021_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.022 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ022_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT022_IRQn - IRQn_OFFSET]();
}

/**
 * @brief  Interrupt No.023 IRQ handler
 * @param  None
 * @retval None
 */
void IRQ023_Handler(void)
{
    m_apfnIrqHandler[(uint32_t)INT023_IRQn - IRQn_OFFSET]();
}

/**
 * @}
 */

#endif /* LL_INTERRUPTS_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
