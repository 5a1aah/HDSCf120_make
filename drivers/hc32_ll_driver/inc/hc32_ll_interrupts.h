/**
 *******************************************************************************
 * @file  hc32_ll_interrupts.h
 * @brief This file contains all the functions prototypes of the interrupt driver
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
#ifndef __HC32_LL_INTERRUPTS_H__
#define __HC32_LL_INTERRUPTS_H__

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
 * @addtogroup LL_INTERRUPTS
 * @{
 */

#if (LL_INTERRUPTS_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup INTC_Global_Types INTC Global Types
 * @{
 */

/**
 * @brief  Interrupt registration structure definition
 */
typedef struct {
    en_int_src_t enIntSrc;  /*!< Peripheral interrupt number, can be any value @ref en_int_src_t    */
    IRQn_Type enIRQn;       /*!< Peripheral IRQ type, can be INT000_IRQn~INT127_IRQn @ref IRQn_Type */
    func_ptr_t pfnCallback; /*!< Callback function for corresponding peripheral IRQ                 */
} stc_irq_signin_config_t;

/**
 * @brief  NMI initialize configuration structure definition
 */
typedef struct {
    uint32_t u32Src;            /*!< NMI trigger source, @ref NMI_TriggerSrc_Sel for details */
    uint32_t u32Edge;           /*!< NMI pin trigger edge, @ref NMI_Trigger_level_Sel for details */
    uint32_t u32Filter;         /*!< NMI filter function setting, @ref NMI_FilterClock_Sel for details */
    uint32_t u32FilterClock;    /*!< NMI filter clock division, @ref NMI_FilterClock_Div for details */
} stc_nmi_init_t;

/**
 * @brief  EXTINT initialize configuration structure definition
 */
typedef struct {
    uint32_t u32Filter;         /*!< ExtInt filter (A) function setting, @ref EXTINT_FilterClock_Sel for details */
    uint32_t u32FilterClock;    /*!< ExtInt filter (A) clock division, @ref EXTINT_FilterClock_Div for details */
    uint32_t u32Edge;           /*!< ExtInt trigger edge, @ref EXTINT_Trigger_Sel for details */
} stc_extint_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup INTC_Global_Macros INTC Global Macros
 * @{
 */
/**
 * @defgroup INTC_Priority_Sel Interrupt Priority Level 00 ~ 15
 * @{
 */
#define DDL_IRQ_PRIO_00                 (0U)
#define DDL_IRQ_PRIO_01                 (1U)
#define DDL_IRQ_PRIO_02                 (2U)
#define DDL_IRQ_PRIO_03                 (3U)
#define DDL_IRQ_PRIO_04                 (4U)
#define DDL_IRQ_PRIO_05                 (5U)
#define DDL_IRQ_PRIO_06                 (6U)
#define DDL_IRQ_PRIO_07                 (7U)
#define DDL_IRQ_PRIO_08                 (8U)
#define DDL_IRQ_PRIO_09                 (9U)
#define DDL_IRQ_PRIO_10                 (10U)
#define DDL_IRQ_PRIO_11                 (11U)
#define DDL_IRQ_PRIO_12                 (12U)
#define DDL_IRQ_PRIO_13                 (13U)
#define DDL_IRQ_PRIO_14                 (14U)
#define DDL_IRQ_PRIO_15                 (15U)

#define DDL_IRQ_PRIO_DEFAULT            (DDL_IRQ_PRIO_03)
/**
 * @}
 */

/**
 * @defgroup NMI_TriggerSrc_Sel NMI Trigger Source Selection
 * @{
 */
#define NMI_SRC_PIN                     (INTC_NMIFR_NMIF)
#define NMI_SRC_XTAL                    (INTC_NMIFR_XTALSTPF)
#define NMI_SRC_SWDT                    (INTC_NMIFR_SWDTF)
#define NMI_SRC_LVD                     (INTC_NMIFR_PVDF)
#define NMI_SRC_ALL                     (INTC_NMIFR_NMIF | NMI_SRC_XTAL | NMI_SRC_SWDT | NMI_SRC_LVD)

/**
 * @}
 */

/**
 * @defgroup NMI_Trigger_level_Sel NMI Pin Trigger Edge Selection
 * @{
 */
#define NMI_TRIG_FALLING                (0UL)
#define NMI_TRIG_RISING                 (INTC_NMICR_NMITRG)
/**
 * @}
 */

/**
 * @defgroup NMI_FilterClock_Sel NMI Pin Filter Selection
 * @{
 */
#define NMI_FILTER_OFF                  (0UL)
#define NMI_FILTER_ON                   (INTC_NMICR_NMIFEN)
/**
 * @}
 */

/**
 * @defgroup NMI_FilterClock_Div NMI Pin Filter Sampling Clock Division Selection
 * @{
 */
#define NMI_FCLK_DIV1                   (0UL << INTC_NMICR_NMIFCLK_POS)
#define NMI_FCLK_DIV8                   (1UL << INTC_NMICR_NMIFCLK_POS)
#define NMI_FCLK_DIV32                  (2UL << INTC_NMICR_NMIFCLK_POS)
#define NMI_FCLK_DIV64                  (3UL << INTC_NMICR_NMIFCLK_POS)
/**
 * @}
 */

/**
 * @defgroup EXTINT_Channel_Sel External Interrupt Channel Selection
 * @{
 */
#define EXTINT_CH00                     (1UL << 0U)
#define EXTINT_CH01                     (1UL << 1U)
#define EXTINT_CH02                     (1UL << 2U)
#define EXTINT_CH03                     (1UL << 3U)
#define EXTINT_CH04                     (1UL << 4U)
#define EXTINT_CH05                     (1UL << 5U)
#define EXTINT_CH06                     (1UL << 6U)
#define EXTINT_CH07                     (1UL << 7U)
#define EXTINT_CH08                     (1UL << 8U)
#define EXTINT_CH09                     (1UL << 9U)
#define EXTINT_CH_ALL                   (EXTINT_CH00 | EXTINT_CH01 | EXTINT_CH02 | EXTINT_CH03 |    \
                                         EXTINT_CH04 | EXTINT_CH05 | EXTINT_CH06 | EXTINT_CH07 |    \
                                         EXTINT_CH08 | EXTINT_CH09)
/**
 * @}
 */

/**
 * @defgroup INTC_Event_Channel_Sel Event Channel Selection
 * @{
 */
#define INTC_EVT0                       INTC_EVTER_EVTE0
#define INTC_EVT1                       INTC_EVTER_EVTE1
#define INTC_EVT2                       INTC_EVTER_EVTE2
#define INTC_EVT3                       INTC_EVTER_EVTE3
#define INTC_EVT4                       INTC_EVTER_EVTE4
#define INTC_EVT5                       INTC_EVTER_EVTE5
#define INTC_EVT6                       INTC_EVTER_EVTE6
#define INTC_EVT7                       INTC_EVTER_EVTE7
#define INTC_EVT_ALL                    (0xFFUL)
/**
 * @}
 */

/**
 * @defgroup INTC_EKEY_Channel INTC EKEY Channel Selection
 * @{
 */
#define EKEY_CH00                       INTC_EKEYCR_EKEY0EN
#define EKEY_CH01                       INTC_EKEYCR_EKEY1EN
#define EKEY_CH02                       INTC_EKEYCR_EKEY2EN
#define EKEY_CH03                       INTC_EKEYCR_EKEY3EN
#define EKEY_CH04                       INTC_EKEYCR_EKEY4EN
#define EKEY_CH05                       INTC_EKEYCR_EKEY5EN
#define EKEY_CH_ALL                     (EKEY_CH00 | EKEY_CH01 | EKEY_CH02 |        \
                                         EKEY_CH03 | EKEY_CH04 | EKEY_CH05)
/**
 * @}
 */

/**
 * @defgroup EXTINT_FilterClock_Sel External Interrupt Filter A Function Selection
 * @{
 */
#define EXTINT_FILTER_OFF               (0UL)
#define EXTINT_FILTER_ON                INTC_EIRQCR_EIRQFEN
/**
 * @}
 */

/**
 * @defgroup EXTINT_FilterClock_Div External Interrupt Filter A Sampling Clock Division Selection
 * @{
 */
#define EXTINT_FCLK_DIV1                (0UL)
#define EXTINT_FCLK_DIV8                (INTC_EIRQCR_EIRQFCLK_0)
#define EXTINT_FCLK_DIV32               (INTC_EIRQCR_EIRQFCLK_1)
#define EXTINT_FCLK_DIV64               (INTC_EIRQCR_EIRQFCLK)

/**
 * @}
 */

/**
 * @defgroup EXTINT_Trigger_Sel External Interrupt Trigger Edge Selection
 * @{
 */
#define EXTINT_TRIG_LOW             (0UL)
#define EXTINT_TRIG_RISING              INTC_EIRQCR_EIRQTRG_0
#define EXTINT_TRIG_FALLING             INTC_EIRQCR_EIRQTRG_1
#define EXTINT_TRIG_BOTH                INTC_EIRQCR_EIRQTRG

/**
 * @}
 */

/**
 * @defgroup INTC_Stop_Wakeup_Source_Sel Stop Mode Wakeup Source Selection
 * @{
 */
#define INTC_STOP_WKUP_EXTINT_CH0       INTC_WUPEN_EIRQWUEN_0
#define INTC_STOP_WKUP_EXTINT_CH1       INTC_WUPEN_EIRQWUEN_1
#define INTC_STOP_WKUP_EXTINT_CH2       INTC_WUPEN_EIRQWUEN_2
#define INTC_STOP_WKUP_EXTINT_CH3       INTC_WUPEN_EIRQWUEN_3
#define INTC_STOP_WKUP_EXTINT_CH4       INTC_WUPEN_EIRQWUEN_4
#define INTC_STOP_WKUP_EXTINT_CH5       INTC_WUPEN_EIRQWUEN_5
#define INTC_STOP_WKUP_EXTINT_CH6       INTC_WUPEN_EIRQWUEN_6
#define INTC_STOP_WKUP_EXTINT_CH7       INTC_WUPEN_EIRQWUEN_7
#define INTC_STOP_WKUP_EXTINT_CH8       INTC_WUPEN_EIRQWUEN_8
#define INTC_STOP_WKUP_EXTINT_CH9       INTC_WUPEN_EIRQWUEN_9
#define INTC_STOP_WKUP_SWDT             INTC_WUPEN_SWDTWUEN
#define INTC_STOP_WKUP_EKEY             INTC_WUPEN_EKEYWUEN
#define INTC_STOP_WKUP_TMR0_CMP         INTC_WUPEN_TMR0CMPWUEN
#define INTC_STOP_WKUP_LVD              INTC_WUPEN_PVDWUEN
#define INTC_WUPEN_ALL                  (INTC_WUPEN_EIRQWUEN    | INTC_WUPEN_SWDTWUEN           |           \
                                         INTC_WUPEN_EKEYWUEN    | INTC_WUPEN_TMR0CMPWUEN        |           \
                                         INTC_WUPEN_PVDWUEN)
/**
 * @}
 */

/**
 * @defgroup INTC_Register_Protect_Key INTC Registers Protect Key
 * @{
 */
#ifndef INTC_REG_UNLOCK_KEY
#define INTC_REG_UNLOCK_KEY          (0xA5U)
#endif
#ifndef INTC_REG_LOCK_KEY
#define INTC_REG_LOCK_KEY            (0x00U)
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

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @addtogroup INTC_Global_Functions
 * @{
 */
/**
 * @brief  INTC lock, register write disable
 * @param  None
 * @retval None
 */
__STATIC_INLINE void INTC_REG_Lock(void)
{
    WRITE_REG32(CM_INTC->FPRCR, INTC_REG_LOCK_KEY);
}

/**
 * @brief  INTC unlock, register write enable
 * @param  None
 * @retval None
 */
__STATIC_INLINE void INTC_REG_Unlock(void)
{
    WRITE_REG32(CM_INTC->FPRCR, INTC_REG_UNLOCK_KEY);
}

int32_t INTC_IrqSignIn(const stc_irq_signin_config_t *pstcIrqSignConfig);
int32_t INTC_IrqSignOut(IRQn_Type enIRQn);
void INTC_WakeupSrcCmd(uint32_t u32WakeupSrc, en_functional_state_t enNewState);
void INTC_EventCmd(uint32_t u32Event, en_functional_state_t enNewState);

void INTC_EKeyCmd(uint8_t u8EKey, en_functional_state_t enNewState);

int32_t NMI_Init(const stc_nmi_init_t *pstcNmiInit);
int32_t NMI_StructInit(stc_nmi_init_t *pstcNmiInit);
en_flag_status_t NMI_GetNmiStatus(uint32_t u32Src);
void NMI_NmiSrcCmd(uint32_t u32Src, en_functional_state_t enNewState);
void NMI_ClearNmiStatus(uint32_t u32Src);

int32_t EXTINT_Init(uint32_t u32Ch, const stc_extint_init_t *pstcExtIntInit);
int32_t EXTINT_StructInit(stc_extint_init_t *pstcExtIntInit);
en_flag_status_t EXTINT_GetExtIntStatus(uint32_t u32ExtIntCh);
void EXTINT_ClearExtIntStatus(uint32_t u32ExtIntCh);

void IRQ008_Handler(void);
void IRQ009_Handler(void);
void IRQ010_Handler(void);
void IRQ011_Handler(void);
void IRQ012_Handler(void);
void IRQ013_Handler(void);
void IRQ014_Handler(void);
void IRQ015_Handler(void);

void IRQ016_Handler(void);
void IRQ017_Handler(void);
void IRQ018_Handler(void);
void IRQ019_Handler(void);
void IRQ020_Handler(void);
void IRQ021_Handler(void);
void IRQ022_Handler(void);
void IRQ023_Handler(void);

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

#ifdef __cplusplus
}
#endif

#endif /* __HC32_LL_INTERRUPTS_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
