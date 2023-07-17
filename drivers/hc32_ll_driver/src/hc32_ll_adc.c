/**
 *******************************************************************************
 * @file  hc32_ll_adc.c
 * @brief This file provides firmware functions to manage the Analog-to-Digital
 *        Converter(ADC).
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
#include "hc32_ll_adc.h"
#include "hc32_ll_utility.h"

/**
 * @addtogroup LL_Driver
 * @{
 */

/**
 * @defgroup LL_ADC ADC
 * @brief Analog-to-Digital Converter Driver Library
 * @{
 */

#if (LL_ADC_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup ADC_Local_Macros ADC Local Macros
 * @{
 */

/**
 * @defgroup ADC_AWD_DR_CHSR ADC AWD DR CHSR
 * @{
 */
#define ADC_AWDx_DR(awd, reg_base)      (*(__IO uint16_t *)((uint32_t)(reg_base) + ((uint32_t)(awd) * 8U)))
#define ADC_AWDx_CHSR(awd, reg_base)    (*(__IO uint8_t *)((uint32_t)(reg_base) + ((uint32_t)(awd) * 8U)))
/**
 * @}
 */

/**
 * @defgroup ADC_Channel_Max ADC Channel Max
 * @{
 */
#define ADC_CH_MAX                      (ADC_CH11)
/**
 * @}
 */

/**
 * @defgroup ADC_Check_Parameters_Validity ADC check parameters validity
 * @{
 */
#define IS_ADC_1BIT_MASK(x)             (((x) != 0U) && (((x) & ((x) - 1U)) == 0U))
#define IS_ADC_BIT_MASK(x, mask)        (((x) != 0U) && (((x) | (mask)) == (mask)))

/* ADC unit check */
#define IS_ADC_UNIT(x)                  ((x) == CM_ADC)

/* ADC sequence check */
#define IS_ADC_SEQ(x)                   (((x) == ADC_SEQ_A) || ((x) == ADC_SEQ_B))

/* ADC channel check */
#define IS_ADC_CH(adc, ch)                                                     \
(   (((adc) == CM_ADC) && ((ch) <= ADC_CH_MAX)))

#define IS_ADC_SCAN_MD(x)                                                      \
(   ((x) == ADC_MD_SEQA_SINGLESHOT)         ||                                 \
    ((x) == ADC_MD_SEQA_CONT)               ||                                 \
    ((x) == ADC_MD_SEQA_SEQB_SINGLESHOT)    ||                                 \
    ((x) == ADC_MD_SEQA_CONT_SEQB_SINGLESHOT))

#define IS_ADC_RESOLUTION(x)                                                   \
(   ((x) == ADC_RESOLUTION_8BIT)            ||                                 \
    ((x) == ADC_RESOLUTION_10BIT)           ||                                 \
    ((x) == ADC_RESOLUTION_12BIT))

#define IS_ADC_HARDTRIG(x)                                                     \
(   ((x) == ADC_HARDTRIG_ADTRG_PIN)         ||                                 \
    ((x) == ADC_HARDTRIG_EVT0)              ||                                 \
    ((x) == ADC_HARDTRIG_EVT1)              ||                                 \
    ((x) == ADC_HARDTRIG_EVT0_EVT1))

#define IS_ADC_DATAALIGN(x)                                                    \
(   ((x) == ADC_DATAALIGN_RIGHT)            ||                                 \
    ((x) == ADC_DATAALIGN_LEFT))

#define IS_ADC_SEQA_RESUME_MD(x)                                               \
(   ((x) == ADC_SEQA_RESUME_SCAN_CONT)      ||                                 \
    ((x) == ADC_SEQA_RESUME_SCAN_RESTART))

#define IS_ADC_SAMPLE_TIME(x)           ((x) >= 5U)

#define IS_ADC_INT(x)                   IS_ADC_BIT_MASK(x, ADC_INT_ALL)
#define IS_ADC_FLAG(x)                  IS_ADC_BIT_MASK(x, ADC_FLAG_ALL)

/* Extended channel. */
#define IS_ADC_EXTCH_SRC(x)                                                    \
(   ((x) == ADC_EXTCH_EXTERN_ANALOG_PIN)    ||                                 \
    ((x) == ADC_EXTCH_INTERN_ANALOG_SRC))

/* Analog watchdog. */
#define IS_ADC_AWD_MD(x)                                                       \
(   ((x) == ADC_AWD_MD_CMP_OUT)             ||                                 \
    ((x) == ADC_AWD_MD_CMP_IN))

#define IS_ADC_AWD(x)                   ((x) <= ADC_AWD1)

/* AWD flag check */
#define IS_ADC_AWD_FLAG(x)              IS_ADC_BIT_MASK(x, ADC_AWD_FLAG_ALL)

/* Two AWD units */
#define IS_ADC_AWD_COMB_MD(x)                                                  \
(   ((x) == ADC_AWD_COMB_INVD)              ||                                 \
    ((x) == ADC_AWD_COMB_OR)                ||                                 \
    ((x) == ADC_AWD_COMB_AND)               ||                                 \
    ((x) == ADC_AWD_COMB_XOR))

#define IS_ADC_AWD_INT(x)               IS_ADC_BIT_MASK(x, ADC_AWD_INT_ALL)

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
 * @defgroup ADC_Global_Functions ADC Global Functions
 * @{
 */

/**
 * @brief  Initializes the specified ADC peripheral according to the specified parameters
 *         in the structure pstcAdcInit.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  pstcAdcInit            Pointer to a @ref stc_adc_init_t structure that contains the
 *                                      configuration information for the specified ADC.
 * @retval int32_t:
 *           - LL_OK:                   No errors occurred.
 *           - LL_ERR_INVD_PARAM:       pstcAdcInit == NULL.
 */
int32_t ADC_Init(CM_ADC_TypeDef *ADCx, const stc_adc_init_t *pstcAdcInit)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    DDL_ASSERT(IS_ADC_UNIT(ADCx));

    if (pstcAdcInit != NULL) {
        DDL_ASSERT(IS_ADC_SCAN_MD(pstcAdcInit->u16ScanMode));
        DDL_ASSERT(IS_ADC_RESOLUTION(pstcAdcInit->u16Resolution));
        DDL_ASSERT(IS_ADC_DATAALIGN(pstcAdcInit->u16DataAlign));
        /* Configures scan mode, resolution, data align. */
        WRITE_REG16(ADCx->CR0, pstcAdcInit->u16ScanMode | pstcAdcInit->u16Resolution | pstcAdcInit->u16DataAlign);
        i32Ret = LL_OK;
    }

    return i32Ret;
}

/**
 * @brief  Deinitializes the specified ADC peripheral registers to their default reset values.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @retval None
 */
void ADC_DeInit(CM_ADC_TypeDef *ADCx)
{

    DDL_ASSERT(IS_ADC_UNIT(ADCx));

    /* Stop the ADC. */
    WRITE_REG8(ADCx->STR, 0U);
    /* Set the registers to reset value. */
    WRITE_REG16(ADCx->CR0, 0x0U);
    WRITE_REG16(ADCx->CR1, 0x0U);
    WRITE_REG16(ADCx->TRGSR, 0x0U);
    WRITE_REG32(ADCx->CHSELRA, 0x0U);
    WRITE_REG32(ADCx->CHSELRB, 0x0U);
    WRITE_REG8(ADCx->ICR, 0x03U);

    /* SSTRx */
    WRITE_REG8(ADCx->SSTR, 0x0BU);

    /* ISR clearing */
    WRITE_REG8(ADCx->ISCLRR, 0x03U);

    /* Analog watchdog */
    WRITE_REG16(ADCx->AWDCR, 0x0U);
    WRITE_REG8(ADCx->AWDSCLRR, 0x13U);
    WRITE_REG16(ADCx->AWD0DR0, 0x0U);
    WRITE_REG16(ADCx->AWD0DR1, 0xFFFFU);
    WRITE_REG16(ADCx->AWD1DR0, 0x0U);
    WRITE_REG16(ADCx->AWD1DR1, 0xFFFFU);
    WRITE_REG8(ADCx->AWD0CHSR, 0x0U);
    WRITE_REG8(ADCx->AWD1CHSR, 0x0U);

}

/**
 * @brief  Set each @ref stc_adc_init_t field to default value.
 * @param  [in]  pstcAdcInit            Pointer to a @ref stc_adc_init_t structure
 *                                      whose fields will be set to default values.
 * @retval int32_t:
 *           - LL_OK:                   No errors occurred.
 *           - LL_ERR_INVD_PARAM:       pstcAdcInit == NULL.
 */
int32_t ADC_StructInit(stc_adc_init_t *pstcAdcInit)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    if (pstcAdcInit != NULL) {
        pstcAdcInit->u16ScanMode   = ADC_MD_SEQA_SINGLESHOT;
        pstcAdcInit->u16Resolution = ADC_RESOLUTION_12BIT;
        pstcAdcInit->u16DataAlign  = ADC_DATAALIGN_RIGHT;
        i32Ret = LL_OK;
    }

    return i32Ret;
}

/**
 * @brief  Enable or disable the specified ADC channel.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8Seq                  The sequence whose channel specified by 'u8Ch' will be enabled or disabled.
 *                                      This parameter can be a value of @ref ADC_Sequence
 *   @arg  ADC_SEQ_A:                   ADC sequence A.
 *   @arg  ADC_SEQ_B:                   ADC sequence B.
 * @param  [in]  u8Ch                   The ADC channel.
 *                                      This parameter can be values of @ref ADC_Channel
 * @param  [in]  enNewState             An @ref en_functional_state_t enumeration value.
 * @note   Sequence A and Sequence B CAN NOT include the same channel!
 * @note   Sequence A can always started by software(by calling @ref ADC_Start()),
 *         regardless of whether the hardware trigger source is valid or not.
 * @note   Sequence B must be specified a valid hard trigger by calling functions @ref ADC_TriggerConfig()
 *         and @ref ADC_TriggerCmd().
 */
void ADC_ChCmd(CM_ADC_TypeDef *ADCx, uint8_t u8Seq, uint8_t u8Ch, en_functional_state_t enNewState)
{
    uint32_t u32CHSELAddr;

    DDL_ASSERT(IS_ADC_CH(ADCx, u8Ch));
    DDL_ASSERT(IS_ADC_SEQ(u8Seq));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    u32CHSELAddr = (uint32_t)&ADCx->CHSELRA + (u8Seq * 4UL);
    if (enNewState == ENABLE) {
        /* Enable the specified channel. */
        SET_REG32_BIT(RW_MEM32(u32CHSELAddr), 1UL << u8Ch);
    } else {
        /* Disable the specified channel. */
        CLR_REG32_BIT(RW_MEM32(u32CHSELAddr), 1UL << u8Ch);
    }
}

/**
 * @brief  Set sampling time for the specified channel.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8Ch                   The channel to be set sampling time.
 *                                      This parameter can be values of @ref ADC_Channel
 * @param  [in]  u8SampleTime           Sampling time for the channel that specified by 'u8Ch'.
 * @retval None
 */
void ADC_SetSampleTime(CM_ADC_TypeDef *ADCx, uint8_t u8Ch, uint8_t u8SampleTime)
{

    DDL_ASSERT(IS_ADC_SAMPLE_TIME(u8SampleTime));

    (void)(u8Ch);
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    WRITE_REG8(ADCx->SSTR, u8SampleTime);
}

/**
 * @brief  Specifies the analog input source of extended channel.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8ExtChSrc             The analog input source of extended channel.
 *                                      This paramter can be a value of @ref ADC_Ext_Ch_Analog_Src
 * @retval None
 */
void ADC_SetExtChSrc(CM_ADC_TypeDef *ADCx, uint8_t u8ExtChSrc)
{
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_EXTCH_SRC(u8ExtChSrc));
    WRITE_REG8(ADCx->EXCHSELR, u8ExtChSrc);
}

/**
 * @brief  Specifies the hard trigger for the specified ADC sequence.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADCx or CM_ADC
 * @param  [in]  u8Seq                  The sequence to be configured.
 *                                      This parameter can be a value of @ref ADC_Sequence
 *   @arg  ADC_SEQ_A:                   Sequence A.
 *   @arg  ADC_SEQ_B:                   Sequence B.
 * @param  [in]  u16TriggerSel          Hard trigger selection. This parameter can be a value of @ref ADC_Hard_Trigger_Sel
 *   @arg  ADC_HARDTRIG_ADTRG_PIN:      Selects the following edge of pin ADTRG as the trigger of ADC sequence.
 *   @arg  ADC_HARDTRIG_EVT0:           Selects an internal event as the trigger of ADC sequence.
                                        This event is specified by register ADCx_ITRGSELR0(x=(null), 1, 2, 3).
 *   @arg  ADC_HARDTRIG_EVT1:           Selects an internal event as the trigger of ADC sequence.
                                        This event is specified by register ADCx_ITRGSELR1(x=(null), 1, 2, 3).
 *   @arg  ADC_HARDTRIG_EVT0_EVT1:      Selects two internal events as the trigger of ADC sequence.
                                        The two events are specified by register ADCx_ITRGSELR0 and register ADCx_ITRGSELR1.
 * @retval None
 * @note   ADC must be stopped while calling this function.
 * @note   The trigger source CANNOT be an event that generated by the sequence itself.
 */
void ADC_TriggerConfig(CM_ADC_TypeDef *ADCx, uint8_t u8Seq, uint16_t u16TriggerSel)
{
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_SEQ(u8Seq));
    DDL_ASSERT(IS_ADC_HARDTRIG(u16TriggerSel));

    u8Seq *= ADC_TRGSR_TRGSELB_POS;
    MODIFY_REG16(ADCx->TRGSR, (uint32_t)ADC_TRGSR_TRGSELA << u8Seq, (uint32_t)u16TriggerSel << u8Seq);
}

/**
 * @brief  Enable or disable the hard trigger of the specified ADC sequence.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADCx or CM_ADC
 * @param  [in]  u8Seq                  The sequence to be configured.
 *                                      This parameter can be a value of @ref ADC_Sequence
 *   @arg  ADC_SEQ_A:                   Sequence A.
 *   @arg  ADC_SEQ_B:                   Sequence B.
 * @param  [in]  enNewState             An @ref en_functional_state_t enumeration value.
 * @retval None
 * @note   ADC must be stopped while calling this function.
 */
void ADC_TriggerCmd(CM_ADC_TypeDef *ADCx, uint8_t u8Seq, en_functional_state_t enNewState)
{
    uint32_t u32Addr;

    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_SEQ(u8Seq));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    u32Addr = (uint32_t)&ADCx->TRGSR;
    /* Enable bit position: u8Seq * sequence_offset + enable_bit_base. */
    WRITE_REG32(PERIPH_BIT_BAND(u32Addr, (uint32_t)u8Seq * ADC_TRGSR_TRGSELB_POS + ADC_TRGSR_TRGENA_POS), enNewState);
}

/**
 * @brief  Enable or disable ADC interrupts.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8IntType              ADC interrupt.
 *                                      This parameter can be values of @ref ADC_Int_Type
 *   @arg  ADC_INT_EOCA:                Interrupt of the end of conversion of sequence A.
 *   @arg  ADC_INT_EOCB:                Interrupt of the end of conversion of sequence B.
 * @param  [in]  enNewState             An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void ADC_IntCmd(CM_ADC_TypeDef *ADCx, uint8_t u8IntType, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_INT(u8IntType));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (enNewState == ENABLE) {
        SET_REG8_BIT(ADCx->ICR, u8IntType);
    } else {
        CLR_REG8_BIT(ADCx->ICR, u8IntType);
    }
}

/**
 * @brief  Start sequence A conversion.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @retval None
 */
void ADC_Start(CM_ADC_TypeDef *ADCx)
{
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    WRITE_REG8(ADCx->STR, ADC_STR_STRT);
}

/**
 * @brief  Stop ADC conversion, both sequence A and sequence B.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @retval None
 */
void ADC_Stop(CM_ADC_TypeDef *ADCx)
{
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    WRITE_REG8(ADCx->STR, 0U);
}

/**
 * @brief  Get the ADC value of the specified channel.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8Ch                   The ADC channel.
 *                                      This parameter can be values of @ref ADC_Channel
 * @retval An uint16_t type value of ADC value.
 */
uint16_t ADC_GetValue(const CM_ADC_TypeDef *ADCx, uint8_t u8Ch)
{
    DDL_ASSERT(IS_ADC_CH(ADCx, u8Ch));

    return RW_MEM16((uint32_t)&ADCx->DR0 + u8Ch * 2UL);
}

/**
 * @brief  Get the status of the specified ADC flag.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8Flag                 ADC status flag.
 *                                      This parameter can be a value of @ref ADC_Status_Flag
 * @retval An @ref en_flag_status_t enumeration type value.
 */
en_flag_status_t ADC_GetStatus(const CM_ADC_TypeDef *ADCx, uint8_t u8Flag)
{
    en_flag_status_t enStatus = RESET;

    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_FLAG(u8Flag));

    if (READ_REG8_BIT(ADCx->ISR, u8Flag) != 0U) {
        enStatus = SET;
    }

    return enStatus;
}

/**
 * @brief  Clear the status of the specified ADC flag.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8Flag                 ADC status flag.
 *                                      This parameter can be valueS of @ref ADC_Status_Flag
 * @retval None
 */
void ADC_ClearStatus(CM_ADC_TypeDef *ADCx, uint8_t u8Flag)
{
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_FLAG(u8Flag));

    WRITE_REG8(ADCx->ISCLRR, u8Flag);
}

/**
 * @brief  Configures analog watchdog.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8AwdUnit              AWD unit that is going to be configured.
 *                                      This parameter can be a value of @ref ADC_AWD_Unit
 * @param  [in]  u8Ch                   The channel that to be used as an analog watchdog channel.
 *                                      This parameter can be a value of @ref ADC_Channel
 * @param  [in]  pstcAwd                Pointer to a @ref stc_adc_awd_config_t structure value that
 *                                      contains the configuration information of the AWD.
 * @retval int32_t:
 *           - LL_OK:                   No errors occurred.
 *           - LL_ERR_INVD_PARAM:       pstcAwd == NULL.
 */
int32_t ADC_AWD_Config(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit, uint8_t u8Ch, const stc_adc_awd_config_t *pstcAwd)
{
    uint32_t u32AwdDr0;
    uint32_t u32AwdDr1;
    uint32_t u32AwdChsr;
    uint32_t u32Addr;
    uint8_t u8Pos;
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    DDL_ASSERT(IS_ADC_CH(ADCx, u8Ch));
    DDL_ASSERT(IS_ADC_AWD(u8AwdUnit));

    if (pstcAwd != NULL) {
        DDL_ASSERT(IS_ADC_AWD_MD(pstcAwd->u16WatchdogMode));

        u8Pos      = (u8AwdUnit * 4U) + ADC_AWDCR_AWD0MD_POS;
        u32Addr    = (uint32_t)&ADCx->AWDCR;
        u32AwdDr0  = (uint32_t)&ADCx->AWD0DR0;
        u32AwdDr1  = (uint32_t)&ADCx->AWD0DR1;
        u32AwdChsr = (uint32_t)&ADCx->AWD0CHSR;

        WRITE_REG32(PERIPH_BIT_BAND(u32Addr, u8Pos), pstcAwd->u16WatchdogMode);
        WRITE_REG16(ADC_AWDx_DR(u8AwdUnit, u32AwdDr0), pstcAwd->u16LowThreshold);
        WRITE_REG16(ADC_AWDx_DR(u8AwdUnit, u32AwdDr1), pstcAwd->u16HighThreshold);
        WRITE_REG8(ADC_AWDx_CHSR(u8AwdUnit, u32AwdChsr), u8Ch);
        i32Ret = LL_OK;
    }

    return i32Ret;
}

/**
 * @brief  Specifies combination mode of analog watchdog.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u16CombMode            Combination mode of analog watchdog.
 *                                      This parameter can be a value of @ref ADC_AWD_Comb_Mode
 *   @arg  ADC_AWD_COMB_INVD            Combination mode is invalid.
 *   @arg  ADC_AWD_COMB_OR:             The status of AWD0 is set or the status of AWD1 is set, the status of combination mode is set.
 *   @arg  ADC_AWD_COMB_AND:            The status of AWD0 is set and the status of AWD1 is set, the status of combination mode is set.
 *   @arg  ADC_AWD_COMB_XOR:            Only one of the status of AWD0 and AWD1 is set, the status of combination mode is set.
 * @retval None
 */
void ADC_AWD_SetCombMode(CM_ADC_TypeDef *ADCx, uint16_t u16CombMode)
{
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_AWD_COMB_MD(u16CombMode));
    MODIFY_REG16(ADCx->AWDCR, ADC_AWDCR_AWDCM, u16CombMode);
}

/**
 * @brief  Specifies the comapre mode of analog watchdog.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8AwdUnit              AWD unit that is going to be configured.
 *                                      This parameter can be a value of @ref ADC_AWD_Unit
 * @param  [in]  u16WatchdogMode        Analog watchdog comapre mode.
 *                                      This parameter can be a value of @ref ADC_AWD_Mode
 *   @arg  ADC_AWD_MD_CMP_OUT:          ADCValue > HighThreshold or ADCValue < LowThreshold
 *   @arg  ADC_AWD_MD_CMP_IN:           LowThreshold < ADCValue < HighThreshold
 * @retval None
 */
void ADC_AWD_SetMode(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit, uint16_t u16WatchdogMode)
{
    uint8_t u8Pos;
    uint32_t u32Addr;

    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_AWD(u8AwdUnit));
    DDL_ASSERT(IS_ADC_AWD_MD(u16WatchdogMode));

    u8Pos   = (u8AwdUnit * 4U) + ADC_AWDCR_AWD0MD_POS;
    u32Addr = (uint32_t)&ADCx->AWDCR;
    WRITE_REG32(PERIPH_BIT_BAND(u32Addr, u8Pos), u16WatchdogMode);
}

/**
 * @brief  Get the comapre mode of analog watchdog.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8AwdUnit              AWD unit that is going to be configured.
 *                                      This parameter can be a value of @ref ADC_AWD_Unit
 * @retval Analog watchdog compare mode. A value of @ref ADC_AWD_Mode
 *         - ADC_AWD_MD_CMP_OUT:        ADCValue > HighThreshold or ADCValue < LowThreshold
 *         - ADC_AWD_MD_CMP_IN:         LowThreshold < ADCValue < HighThreshold
 */
uint16_t ADC_AWD_GetMode(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit)
{
    uint16_t u16RetMode;

    uint8_t u8Pos;
    uint32_t u32Addr;

    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_AWD(u8AwdUnit));

    u8Pos   = (u8AwdUnit * 4U) + ADC_AWDCR_AWD0MD_POS;
    u32Addr = (uint32_t)&ADCx->AWDCR;
    u16RetMode = (uint16_t)PERIPH_BIT_BAND(u32Addr, u8Pos);

    return u16RetMode;
}

/**
 * @brief  Specifies the low threshold and high threshold of analog watchdog.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8AwdUnit              AWD unit that is going to be configured.
 *                                      This parameter can be a value of @ref ADC_AWD_Unit
 * @param  [in]  u16LowThreshold        Low threshold of analog watchdog.
 * @param  [in]  u16HighThreshold       High threshold of analog watchdog.
 * @retval None
 */
void ADC_AWD_SetThreshold(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit, uint16_t u16LowThreshold, uint16_t u16HighThreshold)
{
    uint32_t u32AwdDr0;
    uint32_t u32AwdDr1;

    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_AWD(u8AwdUnit));

    u32AwdDr0 = (uint32_t)&ADCx->AWD0DR0;
    u32AwdDr1 = (uint32_t)&ADCx->AWD0DR1;
    WRITE_REG16(ADC_AWDx_DR(u8AwdUnit, u32AwdDr0), u16LowThreshold);
    WRITE_REG16(ADC_AWDx_DR(u8AwdUnit, u32AwdDr1), u16HighThreshold);
}

/**
 * @brief  Select the specified ADC channel as an analog watchdog channel.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8AwdUnit              AWD unit that is going to be configured.
 *                                      This parameter can be a value of @ref ADC_AWD_Unit
 * @param  [in]  u8Ch                   The channel that to be used as an analog watchdog channel.
 *                                      This parameter can be a value of @ref ADC_Channel
 * @retval None
 */
void ADC_AWD_SelectCh(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit, uint8_t u8Ch)
{
    uint32_t u32AwdChsr;
    DDL_ASSERT(IS_ADC_CH(ADCx, u8Ch));
    DDL_ASSERT(IS_ADC_AWD(u8AwdUnit));

    u32AwdChsr = (uint32_t)&ADCx->AWD0CHSR;
    WRITE_REG8(ADC_AWDx_CHSR(u8AwdUnit, u32AwdChsr), u8Ch);
}

/**
 * @brief  Enable or disable the specified analog watchdog.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u8AwdUnit              AWD unit that is going to be enabled or disabled.
 *                                      This parameter can be a value of @ref ADC_AWD_Unit
 * @param  [in]  enNewState             An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void ADC_AWD_Cmd(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit, en_functional_state_t enNewState)
{
    uint32_t u32Addr;

    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_AWD(u8AwdUnit));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    u32Addr = (uint32_t)&ADCx->AWDCR;
    /* Enable bit position: u8AwdUnit * 4 */
    WRITE_REG32(PERIPH_BIT_BAND(u32Addr, (u8AwdUnit * 4UL)), enNewState);
}

/**
 * @brief  Enable or disable the specified analog watchdog interrupts.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u16IntType             Interrupt of AWD.
 *                                      This parameter can be a value of @ref ADC_AWD_Int_Type
 * @param  [in]  enNewState             An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void ADC_AWD_IntCmd(CM_ADC_TypeDef *ADCx, uint16_t u16IntType, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_AWD_INT(u16IntType));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (enNewState == ENABLE) {
        SET_REG16_BIT(ADCx->AWDCR, u16IntType);
    } else {
        CLR_REG16_BIT(ADCx->AWDCR, u16IntType);
    }
}

/**
 * @brief  Get the status of the specified analog watchdog flag.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u32Flag                AWD status flag.
 *                                      This parameter can be values of @ref ADC_AWD_Status_Flag
 * @retval An @ref en_flag_status_t enumeration type value.
 */
en_flag_status_t ADC_AWD_GetStatus(const CM_ADC_TypeDef *ADCx, uint32_t u32Flag)
{
    en_flag_status_t enStatus = RESET;

    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_AWD_FLAG(u32Flag));
    if (READ_REG8_BIT(ADCx->AWDSR, u32Flag) != 0U) {
        enStatus = SET;
    }

    return enStatus;
}

/**
 * @brief  Clear the status of the specified analog watchdog flag.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u32Flag                AWD status flag.
 *                                      This parameter can be values of @ref ADC_AWD_Status_Flag
 * @retval None
 */
void ADC_AWD_ClearStatus(CM_ADC_TypeDef *ADCx, uint32_t u32Flag)
{
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_AWD_FLAG(u32Flag));
    WRITE_REG8(ADCx->AWDSCLRR, u32Flag);
}

/**
 * @brief  Enable or disable automatically clear data register.
 *         The automatic clearing function is mainly used to detect whether the data register is updated.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  enNewState             An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void ADC_DataRegAutoClearCmd(CM_ADC_TypeDef *ADCx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (enNewState == ENABLE) {
        SET_REG16_BIT(ADCx->CR0, ADC_CR0_CLREN);
    } else {
        CLR_REG16_BIT(ADCx->CR0, ADC_CR0_CLREN);
    }
}

/**
 * @brief  Sequence A restart channel selection.
 * @param  [in]  ADCx                   Pointer to ADC instance register base.
 *                                      This parameter can be a value of the following:
 *   @arg  CM_ADC or CM_ADCx:           ADC instance register base.
 * @param  [in]  u16SeqAResumeMode      Sequence A resume mode.
 *                                      This parameter can be a value of @ref ADC_SeqA_Resume_Mode
 *   @arg  ADC_SEQA_RESUME_SCAN_CONT:   Scanning will continue from the interrupted channel.
 *   @arg  ADC_SEQA_RESUME_SCAN_RESTART: Scanning will start from the first channel.
 * @retval None
 */
void ADC_SetSeqAResumeMode(CM_ADC_TypeDef *ADCx, uint16_t u16SeqAResumeMode)
{
    DDL_ASSERT(IS_ADC_UNIT(ADCx));
    DDL_ASSERT(IS_ADC_SEQA_RESUME_MD(u16SeqAResumeMode));
    WRITE_REG16(ADCx->CR1, u16SeqAResumeMode);
}

/**
 * @}
 */

#endif /* LL_ADC_ENABLE */

/**
 * @}
 */

/**
* @}
*/

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
