/**
 *******************************************************************************
 * @file  hc32_ll_adc.h
 * @brief This file contains all the functions prototypes of the ADC driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-10-31       CDT             Modify macro group definition: ADC_Scan_Mode
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
#ifndef __HC32_LL_ADC_H__
#define __HC32_LL_ADC_H__

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
 * @addtogroup LL_ADC
 * @{
 */

#if (LL_ADC_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup ADC_Global_Types ADC Global Types
 * @{
 */
/**
 * @brief Structure definition of analog watchdog(AWD) configuration.
 */
typedef struct {
    uint16_t u16WatchdogMode;           /*!< Specifies the ADC analog watchdog mode.
                                             This parameter can be a value of @ref ADC_AWD_Mode */
    uint16_t u16LowThreshold;           /*!< Specifies the ADC analog watchdog Low threshold value. */
    uint16_t u16HighThreshold;          /*!< Specifies the ADC analog watchdog High threshold value. */
} stc_adc_awd_config_t;

/**
 * @brief Structure definition of ADC initialization.
 */
typedef struct {
    uint16_t u16ScanMode;               /*!< Specifies the ADC scan convert mode.
                                             This parameter can be a value of @ref ADC_Scan_Mode */
    uint16_t u16Resolution;             /*!< Specifies the ADC resolution.
                                             This parameter can be a value of @ref ADC_Resolution */
    uint16_t u16DataAlign;              /*!< Specifies ADC data alignment.
                                             This parameter can be a value of @ref ADC_Data_Align */
} stc_adc_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup ADC_Global_Macros ADC Global Macros
 * @{
 */

/**
 * @defgroup ADC_Sequence ADC Sequence
 * @{
 */
#define ADC_SEQ_A                       (0U)                /*!< ADC sequence A. */
#define ADC_SEQ_B                       (1U)                /*!< ADC sequence B. */
/**
 * @}
 */

/**
 * @defgroup ADC_Channel ADC Channel
 * @{
 */
#define ADC_CH0                         (0U)        /*!< Input pin: P20 */
#define ADC_CH1                         (1U)        /*!< Input pin: P21 */
#define ADC_CH2                         (2U)        /*!< Input pin: P22 */
#define ADC_CH3                         (3U)        /*!< Input pin: P23 */
#define ADC_CH4                         (4U)        /*!< Input pin: P24 */
#define ADC_CH5                         (5U)        /*!< Input pin: P25 */
#define ADC_CH6                         (6U)        /*!< Input pin: P26 */
#define ADC_CH7                         (7U)        /*!< Input pin: P27 */
#define ADC_CH8                         (8U)        /*!< Input pin: P147 */
#define ADC_CH9                         (9U)        /*!< Input pin: P00 */
#define ADC_CH10                        (10U)       /*!< Input pin: P01 */
#define ADC_CH11                        (11U)       /*!< Input pin: P120 */

#define ADC_EXT_CH                      (ADC_CH11)  /*!< Extended channel, input source can be P120,
                                                         internal reference voltage, internal temperature sensor. */
/**
 * @}
 */

/**
 * @defgroup ADC_Scan_Mode ADC Scan Convert Mode
 * @{
 */
#define ADC_MD_SEQA_SINGLESHOT              (0x0U)                      /*!< Sequence A single shot. Sequence B is disabled. */
#define ADC_MD_SEQA_CONT                    (0x1U << ADC_CR0_MS_POS)    /*!< Sequence A continuous. Sequence B is disabled. */
#define ADC_MD_SEQA_SEQB_SINGLESHOT         (0x2U << ADC_CR0_MS_POS)    /*!< Sequence A and B both single shot. */
#define ADC_MD_SEQA_CONT_SEQB_SINGLESHOT    (0x3U << ADC_CR0_MS_POS)    /*!< Sequence A continuous and sequence B single shot. */
/**
 * @}
 */

/**
 * @defgroup ADC_Resolution ADC Resolution
 * @{
 */
#define ADC_RESOLUTION_12BIT            (0x0U)              /*!< Resolution is 12 bit. */
#define ADC_RESOLUTION_10BIT            (ADC_CR0_ACCSEL_0)  /*!< Resolution is 10 bit. */
#define ADC_RESOLUTION_8BIT             (ADC_CR0_ACCSEL_1)  /*!< Resolution is 8 bit. */
/**
 * @}
 */

/**
 * @defgroup ADC_Data_Align ADC Data Align
 * @{
 */
#define ADC_DATAALIGN_RIGHT             (0x0U)              /*!< Right alignment of converted data. */
#define ADC_DATAALIGN_LEFT              (ADC_CR0_DFMT)      /*!< Left alignment of converted data. */
/**
 * @}
 */

/**
 * @defgroup ADC_SeqA_Resume_Mode ADC Sequence A Resume Mode
 * @brief After interrupted by sequence B, sequence A continues to scan from the interrupt channel or the first channel.
 * @{
 */
#define ADC_SEQA_RESUME_SCAN_CONT       (0U)                /*!< Scanning will continue from the interrupted channel. */
#define ADC_SEQA_RESUME_SCAN_RESTART    (ADC_CR1_RSCHSEL)   /*!< Scanning will start from the first channel. */
/**
 * @}
 */

/**
 * @defgroup ADC_Hard_Trigger_Sel ADC Hard Trigger Selection
 * @{
 */
#define ADC_HARDTRIG_ADTRG_PIN          (0x0U)                      /*!< Selects the following edge of pin ADTRG as the trigger of ADC sequence. */
#define ADC_HARDTRIG_EVT0               (ADC_TRGSR_TRGSELA_0)       /*!< Selects an internal event as the trigger of ADC sequence.
                                                                         This event is specified by register ADCx_ITRGSELR0(x=(null), 1, 2, 3). */
#define ADC_HARDTRIG_EVT1               (ADC_TRGSR_TRGSELA_1)       /*!< Selects an internal event as the trigger of ADC sequence.
                                                                         This event is specified by register ADCx_ITRGSELR1(x=(null), 1, 2, 3). */
#define ADC_HARDTRIG_EVT0_EVT1          (ADC_TRGSR_TRGSELA)         /*!< Selects two internal events as the trigger of ADC sequence.
                                                                         The two events are specified by register ADCx_ITRGSELR0 and register ADCx_ITRGSELR1. */
/**
 * @}
 */

/**
 * @defgroup ADC_Int_Type ADC Interrupt Type
 * @{
 */
#define ADC_INT_EOCA                    (ADC_ICR_EOCAIEN)           /*!< Interrupt of the end of conversion of sequence A. */
#define ADC_INT_EOCB                    (ADC_ICR_EOCBIEN)           /*!< Interrupt of the end of conversion of sequence B. */
#define ADC_INT_ALL                     (ADC_INT_EOCA | ADC_INT_EOCB)
/**
 * @}
 */

/**
 * @defgroup ADC_Ext_Ch_Analog_Src ADC Extended Channel Analog Source
 * @{
 */
#define ADC_EXTCH_EXTERN_ANALOG_PIN     (0x0U)                      /*!< The analog source of extended channel is external analog input pin. */
#define ADC_EXTCH_INTERN_ANALOG_SRC     (ADC_EXCHSELR_EXCHSEL)      /*!< The analog source of extended channel is internal analog signal. */
/**
 * @}
 */

/**
 * @defgroup ADC_Status_Flag ADC Status Flag
 * @{
 */
#define ADC_FLAG_EOCA                   (ADC_ISR_EOCAF)             /*!< Status flag of the end of conversion of sequence A. */
#define ADC_FLAG_EOCB                   (ADC_ISR_EOCBF)             /*!< Status flag of the end of conversion of sequence B. */
#define ADC_FLAG_ALL                    (ADC_FLAG_EOCA | ADC_FLAG_EOCB)
/**
 * @}
 */

/**
 * @defgroup ADC_AWD_Unit ADC Analog Watchdog Unit
 * @{
 */
#define ADC_AWD0                        (0U)    /*!< ADC analog watchdog 0. */
#define ADC_AWD1                        (1U)    /*!< ADC analog watchdog 1. */
/**
 * @}
 */

/**
 * @defgroup ADC_AWD_Int_Type ADC AWD Interrupt Type
 * @{
 */
#define ADC_AWD_INT_AWD0                (ADC_AWDCR_AWD0IEN)     /*!< Interrupt of AWD0. */
#define ADC_AWD_INT_AWD1                (ADC_AWDCR_AWD1IEN)     /*!< Interrupt of AWD1. */
#define ADC_AWD_INT_ALL                 (ADC_AWD_INT_AWD0 | ADC_AWD_INT_AWD1)
/**
 * @}
 */

/**
 * @defgroup ADC_AWD_Mode ADC Analog Watchdog Mode
 * @{
 */
#define ADC_AWD_MD_CMP_OUT              (0x0U)  /*!< ADCValue > HighThreshold or ADCValue < LowThreshold */
#define ADC_AWD_MD_CMP_IN               (0x1U)  /*!< LowThreshold < ADCValue < HighThreshold */
/**
 * @}
 */

/**
 * @defgroup ADC_AWD_Comb_Mode ADC AWD(Analog Watchdog) Combination Mode
 * @note If combination mode is valid(ADC_AWD_COMB_OR/ADC_AWD_COMB_AND/ADC_AWD_COMB_XOR) and
 *       the Channels selected by the AWD0 and AWD1 are deferent, make sure that the channel
 *       of AWD1 is converted after the channel conversion of AWD0 ends.
 * @{
 */
#define ADC_AWD_COMB_INVD               (0U)                /*!< Combination mode is invalid. */
#define ADC_AWD_COMB_OR                 (ADC_AWDCR_AWDCM_0) /*!< The status of AWD0 is set or the status of AWD1 is set, the status of combination mode is set. */
#define ADC_AWD_COMB_AND                (ADC_AWDCR_AWDCM_1) /*!< The status of AWD0 is set and the status of AWD1 is set, the status of combination mode is set. */
#define ADC_AWD_COMB_XOR                (ADC_AWDCR_AWDCM)   /*!< Only one of the status of AWD0 and AWD1 is set, the status of combination mode is set. */
/**
 * @}
 */

/**
 * @defgroup ADC_AWD_Status_Flag ADC AWD Status Flag
 * @{
 */
#define ADC_AWD_FLAG_AWD0               (ADC_AWDSR_AWD0F)   /*!< Flag of AWD0. */
#define ADC_AWD_FLAG_AWD1               (ADC_AWDSR_AWD1F)   /*!< Flag of AWD1. */
#define ADC_AWD_FLAG_COMB               (ADC_AWDSR_AWDCMF)  /*!< Flag of combination of mode. */
#define ADC_AWD_FLAG_ALL                (ADC_AWD_FLAG_AWD0 | ADC_AWD_FLAG_AWD1 | ADC_AWD_FLAG_COMB)
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
 * @addtogroup ADC_Global_Functions
 * @{
 */
/*******************************************************************************
  Basic features
 ******************************************************************************/
int32_t ADC_Init(CM_ADC_TypeDef *ADCx, const stc_adc_init_t *pstcAdcInit);
void ADC_DeInit(CM_ADC_TypeDef *ADCx);
int32_t ADC_StructInit(stc_adc_init_t *pstcAdcInit);
void ADC_ChCmd(CM_ADC_TypeDef *ADCx, uint8_t u8Seq, uint8_t u8Ch, en_functional_state_t enNewState);
void ADC_SetSampleTime(CM_ADC_TypeDef *ADCx, uint8_t u8Ch, uint8_t u8SampleTime);

/* Extended channel. */
void ADC_SetExtChSrc(CM_ADC_TypeDef *ADCx, uint8_t u8ExtChSrc);

void ADC_TriggerConfig(CM_ADC_TypeDef *ADCx, uint8_t u8Seq, uint16_t u16TriggerSel);
void ADC_TriggerCmd(CM_ADC_TypeDef *ADCx, uint8_t u8Seq, en_functional_state_t enNewState);
void ADC_IntCmd(CM_ADC_TypeDef *ADCx, uint8_t u8IntType, en_functional_state_t enNewState);
void ADC_Start(CM_ADC_TypeDef *ADCx);
void ADC_Stop(CM_ADC_TypeDef *ADCx);
uint16_t ADC_GetValue(const CM_ADC_TypeDef *ADCx, uint8_t u8Ch);
en_flag_status_t ADC_GetStatus(const CM_ADC_TypeDef *ADCx, uint8_t u8Flag);
void ADC_ClearStatus(CM_ADC_TypeDef *ADCx, uint8_t u8Flag);
/*******************************************************************************
  Advanced features
 ******************************************************************************/

/* Analog watchdog */
int32_t ADC_AWD_Config(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit, uint8_t u8Ch, const stc_adc_awd_config_t *pstcAwd);
/* Combination mode. */
void ADC_AWD_SetCombMode(CM_ADC_TypeDef *ADCx, uint16_t u16CombMode);
void ADC_AWD_SetMode(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit, uint16_t u16WatchdogMode);
uint16_t ADC_AWD_GetMode(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit);
void ADC_AWD_SetThreshold(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit, uint16_t u16LowThreshold, uint16_t u16HighThreshold);
void ADC_AWD_SelectCh(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit, uint8_t u8Ch);
void ADC_AWD_Cmd(CM_ADC_TypeDef *ADCx, uint8_t u8AwdUnit, en_functional_state_t enNewState);
void ADC_AWD_IntCmd(CM_ADC_TypeDef *ADCx, uint16_t u16IntType, en_functional_state_t enNewState);
en_flag_status_t ADC_AWD_GetStatus(const CM_ADC_TypeDef *ADCx, uint32_t u32Flag);
void ADC_AWD_ClearStatus(CM_ADC_TypeDef *ADCx, uint32_t u32Flag);

void ADC_DataRegAutoClearCmd(CM_ADC_TypeDef *ADCx, en_functional_state_t enNewState);
void ADC_SetSeqAResumeMode(CM_ADC_TypeDef *ADCx, uint16_t u16SeqAResumeMode);
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

#ifdef __cplusplus
}
#endif

#endif /* __HC32_LL_ADC_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
