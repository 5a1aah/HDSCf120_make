/**
 *******************************************************************************
 * @file  hc32_ll_spi.h
 * @brief This file contains all the functions prototypes of the SPI driver
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
#ifndef __HC32_LL_SPI_H__
#define __HC32_LL_SPI_H__

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
 * @addtogroup LL_SPI
 * @{
 */

#if (LL_SPI_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup SPI_Global_Types SPI Global Types
 * @{
 */

/**
 * @brief Structure definition of SPI initialization.
 */
typedef struct {
    uint32_t u32WireMode;           /*!< SPI wire mode, 3 wire mode or 4 wire mode.
                                         This parameter can be a value of @ref SPI_Wire_Mode_Define */
    uint32_t u32TransMode;          /*!< SPI transfer mode, send only or full duplex.
                                         This parameter can be a value of @ref SPI_Trans_Mode_Define */
    uint32_t u32MasterSlave;        /*!< SPI master/slave mode.
                                         This parameter can be a value of @ref SPI_Master_Slave_Mode_Define */
    uint32_t u32ModeFaultDetect;    /*!< SPI mode fault detect command.
                                         This parameter can be a value of @ref SPI_Mode_Fault_Detect_Command_Define */
    uint32_t u32Parity;             /*!< SPI parity check selection.
                                         This parameter can be a value of @ref SPI_Parity_Check_Define */
    uint32_t u32SpiMode;            /*!< SPI mode.
                                         This parameter can be a value of @ref SPI_Mode_Define */
    uint32_t u32BaudRatePrescaler;  /*!< SPI baud rate prescaler.
                                         This parameter can be a value of @ref SPI_Baud_Rate_Prescaler_Define */
    uint32_t u32DataBits;           /*!< SPI data bits, 4 bits ~ 32 bits.
                                         This parameter can be a value of @ref SPI_Data_Size_Define */
    uint32_t u32FirstBit;           /*!< MSB first or LSB first.
                                         This parameter can be a value of @ref SPI_First_Bit_Define */
} stc_spi_init_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup SPI_Global_Macros SPI Global Macros
 * @{
 */

/**
 * @defgroup SPI_Wire_Mode_Define SPI Wire Mode Define
 * @{
 */
#define SPI_4_WIRE                  (0UL)
#define SPI_3_WIRE                  (SPI_CR1_SPIMDS)
/**
 * @}
 */

/**
 * @defgroup SPI_Trans_Mode_Define SPI Transfer Mode Define
 * @{
 */
#define SPI_FULL_DUPLEX             (0UL)               /*!< Full duplex. */
#define SPI_SEND_ONLY               (SPI_CR1_TXMDS)     /*!< Send only. */
/**
 * @}
 */

/**
 * @defgroup SPI_Master_Slave_Mode_Define SPI Master Slave Mode Define
 * @{
 */
#define SPI_SLAVE                   (0UL)
#define SPI_MASTER                  (SPI_CR1_MSTR)
/**
 * @}
 */

/**
 * @defgroup SPI_Loopback_Selection_Define SPI Loopback Selection Define
 * @note Loopback mode is mainly used for parity self-diagnosis in 4-wire full-duplex mode.
 * @{
 */
#define SPI_LOOPBACK_INVD           (0UL)
#define SPI_LOOPBACK_MOSI_INVT      (SPI_CR1_SPLPBK)    /*!< MISO data is the inverse of the data output by MOSI. */
#define SPI_LOOPBACK_MOSI           (SPI_CR1_SPLPBK2)   /*!< MISO data is the data output by MOSI. */
/**
 * @}
 */

/**
 * @defgroup SPI_Int_Type_Define SPI Interrupt Type Define
 * @{
 */
#define SPI_INT_ERR                 (SPI_CR1_EIE)       /*!< Including overload, underload and parity error. */
#define SPI_INT_TX_BUF_EMPTY        (SPI_CR1_TXIE)
#define SPI_INT_RX_BUF_FULL         (SPI_CR1_RXIE)
#define SPI_INT_IDLE                (SPI_CR1_IDIE)
#define SPI_IRQ_ALL                 (SPI_INT_ERR | SPI_INT_TX_BUF_EMPTY | SPI_INT_RX_BUF_FULL | SPI_INT_IDLE )
/**
 * @}
 */

/**
 * @defgroup SPI_Mode_Fault_Detect_Command_Define SPI Mode Fault Detect Command Define
 * @{
 */
#define SPI_MD_FAULT_DETECT_DISABLE (0UL)               /*!< Disable mode fault detection. */
#define SPI_MD_FAULT_DETECT_ENABLE  (SPI_CR1_MODFE)     /*!< Enable mode fault detection. */
/**
 * @}
 */

/**
 * @defgroup SPI_Parity_Check_Define SPI Parity Check Mode Define
 * @{
 */
#define SPI_PARITY_INVD             (0UL)                           /*!< Parity check invalid. */
#define SPI_PARITY_EVEN             (SPI_CR1_PAE)                   /*!< Parity check selection even parity. */
#define SPI_PARITY_ODD              (SPI_CR1_PAE | SPI_CR1_PAOE)    /*!< Parity check selection odd parity. */
/**
 * @}
 */

/**
 * @defgroup SPI_SS_Pin_Define SPI SSx Define
 * @{
 */
#define SPI_PIN_SS0                 (SPI_CFG1_SS0PV)
/**
 * @}
 */

/**
 * @defgroup SPI_Mode_Define SPI Mode Define
 * @{
 */
#define SPI_MD_0                    (0UL)                           /*!< SCK pin output low in idle state; \
                                                                         MOSI/MISO pin data valid in odd edge, \
                                                                         MOSI/MISO pin data change in even edge */
#define SPI_MD_1                    (SPI_CFG2_CPHA)                 /*!< SCK pin output low in idle state; \
                                                                         MOSI/MISO pin data valid in even edge, \
                                                                         MOSI/MISO pin data change in odd edge */
#define SPI_MD_2                    (SPI_CFG2_CPOL)                 /*!< SCK pin output high in idle state; \
                                                                         MOSI/MISO pin data valid in odd edge, \
                                                                         MOSI/MISO pin data change in even edge */
#define SPI_MD_3                    (SPI_CFG2_CPOL | SPI_CFG2_CPHA) /*!< SCK pin output high in idle state; \
                                                                         MOSI/MISO pin data valid in even edge, \
                                                                         MOSI/MISO pin data change in odd edge */

/**
 * @}
 */

/**
 * @defgroup SPI_Baud_Rate_Prescaler_Define SPI Baudrate Prescaler Define
 * @{
 */
#define SPI_BR_CLK_DIV2             (0UL << SPI_CFG2_MBR_POS)   /*!< PCLK1 / 2. */
#define SPI_BR_CLK_DIV4             (1UL << SPI_CFG2_MBR_POS)   /*!< PCLK1 / 4. */
#define SPI_BR_CLK_DIV8             (2UL << SPI_CFG2_MBR_POS)   /*!< PCLK1 / 8. */
#define SPI_BR_CLK_DIV16            (3UL << SPI_CFG2_MBR_POS)   /*!< PCLK1 / 16. */
#define SPI_BR_CLK_DIV32            (4UL << SPI_CFG2_MBR_POS)   /*!< PCLK1 / 32. */
#define SPI_BR_CLK_DIV64            (5UL << SPI_CFG2_MBR_POS)   /*!< PCLK1 / 64. */
#define SPI_BR_CLK_DIV128           (6UL << SPI_CFG2_MBR_POS)   /*!< PCLK1 / 128. */
#define SPI_BR_CLK_DIV256           (7UL << SPI_CFG2_MBR_POS)   /*!< PCLK1 / 256. */
/**
 * @}
 */

/**
 * @defgroup SPI_Data_Size_Define SPI Data Size Define
 * @{
 */
#define SPI_DATA_SIZE_8BIT          (0UL << SPI_CFG2_DSIZE_POS)
#define SPI_DATA_SIZE_16BIT         (1UL << SPI_CFG2_DSIZE_POS)
/**
 * @}
 */

/**
 * @defgroup SPI_First_Bit_Define SPI First Bit Define
 * @{
 */
#define SPI_FIRST_MSB               (0UL)
#define SPI_FIRST_LSB               (SPI_CFG2_LSBF)
/**
 * @}
 */

/**
 * @defgroup SPI_State_Flag_Define SPI State Flag Define
 * @{
 */
#define SPI_FLAG_OVERLOAD           (SPI_SR_OVRERF)
#define SPI_FLAG_IDLE               (SPI_SR_IDLNF)
#define SPI_FLAG_MD_FAULT           (SPI_SR_MODFERF)
#define SPI_FLAG_PARITY_ERR         (SPI_SR_PERF)
#define SPI_FLAG_UNDERLOAD          (SPI_SR_UDRERF)
#define SPI_FLAG_TX_BUF_EMPTY       (SPI_SR_TDEF)       /*!< This flag is set when the data in the data register     \
                                                             is copied into the shift register, but the transmission \
                                                             of the data bit may not have been completed. */
#define SPI_FLAG_RX_BUF_FULL        (SPI_SR_RDFF)       /*!< Indicates that a data was received. */
#define SPI_FLAG_CLR_ALL            (SPI_FLAG_OVERLOAD | SPI_FLAG_MD_FAULT | SPI_FLAG_PARITY_ERR | SPI_FLAG_UNDERLOAD)
#define SPI_FLAG_ALL                (SPI_FLAG_OVERLOAD | SPI_FLAG_IDLE | SPI_FLAG_MD_FAULT | SPI_FLAG_PARITY_ERR | \
                                     SPI_FLAG_UNDERLOAD | SPI_FLAG_TX_BUF_EMPTY | SPI_FLAG_RX_BUF_FULL)
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
 * @addtogroup SPI_Global_Functions
 * @{
 */
int32_t SPI_StructInit(stc_spi_init_t *pstcSpiInit);
int32_t SPI_Init(CM_SPI_TypeDef *SPIx, const stc_spi_init_t *pstcSpiInit);
void SPI_DeInit(CM_SPI_TypeDef *SPIx);

void SPI_IntCmd(CM_SPI_TypeDef *SPIx, uint32_t u32IntType, en_functional_state_t enNewState);
void SPI_Cmd(CM_SPI_TypeDef *SPIx, en_functional_state_t enNewState);
void SPI_WriteData(CM_SPI_TypeDef *SPIx, uint32_t u32Data);
uint32_t SPI_ReadData(const CM_SPI_TypeDef *SPIx);

en_flag_status_t SPI_GetStatus(const CM_SPI_TypeDef *SPIx, uint32_t u32Flag);
void SPI_ClearStatus(CM_SPI_TypeDef *SPIx, uint32_t u32Flag);
void SPI_LoopbackModeConfig(CM_SPI_TypeDef *SPIx, uint32_t u32Mode);
void SPI_ParityCheckCmd(CM_SPI_TypeDef *SPIx, en_functional_state_t enNewState);
void SPI_SSValidLevelConfig(CM_SPI_TypeDef *SPIx, uint32_t u32SSPin, en_functional_state_t enNewState);

int32_t SPI_Trans(CM_SPI_TypeDef *SPIx, const void *pvTxBuf, uint32_t u32TxLen, uint32_t u32Timeout);
int32_t SPI_Receive(CM_SPI_TypeDef *SPIx, void *pvRxBuf, uint32_t u32RxLen, uint32_t u32Timeout);
int32_t SPI_TransReceive(CM_SPI_TypeDef *SPIx, const void *pvTxBuf, void *pvRxBuf, uint32_t u32Len, uint32_t u32Timeout);

/**
 * @}
 */

#endif /* LL_SPI_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32_LL_SPI_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
