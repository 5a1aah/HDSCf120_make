/**
 *******************************************************************************
 * @file  hc32_ll_spi.c
 * @brief This file provides firmware functions to manage the Serial Peripheral
 *        Interface(SPI).
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-06-30       CDT             Modify API SPI_TxRx(),SPI_Tx()
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
#include "hc32_ll_spi.h"
#include "hc32_ll_utility.h"

/**
 * @addtogroup LL_Driver
 * @{
 */

/**
 * @defgroup LL_SPI SPI
 * @brief Serial Peripheral Interface Driver Library
 * @{
 */

#if (LL_SPI_ENABLE == DDL_ON)

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup SPI_Local_Macros SPI Local Macros
 * @{
 */
#define SPI_CFG1_DEFAULT        (0x00000000UL)
#define SPI_CFG2_DEFAULT        (0x0000011DUL)

#define SPI_SR_DEFAULT          (0x00000020UL)

/**
 * @defgroup SPI_Check_Parameters_Validity SPI Check Parameters Validity
 * @{
 */

/*! Parameter valid check for SPI peripheral */
#define IS_VALID_SPI_UNIT(x)                                                   \
(   CM_SPI == (x))

/*! Parameter valid check for SPI wire mode */
#define IS_SPI_WIRE_MD(x)                                                      \
(   ((x) == SPI_4_WIRE)                     ||                                 \
    ((x) == SPI_3_WIRE))

/*! Parameter valid check for SPI transfer mode */
#define IS_SPI_TRANS_MD(x)                                                     \
(   ((x) == SPI_FULL_DUPLEX)                ||                                 \
    ((x) == SPI_SEND_ONLY))

/*! Parameter valid check for SPI master slave mode */
#define IS_SPI_MASTER_SLAVE(x)                                                 \
(   ((x) == SPI_SLAVE)                      ||                                 \
    ((x) == SPI_MASTER))

/*! Parameter valid check for SPI loopback mode */
#define IS_SPI_SPLPBK(x)                                                       \
(   ((x) == SPI_LOOPBACK_INVD)              ||                                 \
    ((x) == SPI_LOOPBACK_MOSI_INVT)         ||                                 \
    ((x) == SPI_LOOPBACK_MOSI))

/*! Parameter valid check for SPI fault dectet function status */
#define IS_SPI_MD_FAULT_DETECT_CMD(x)                                          \
(   ((x) == SPI_MD_FAULT_DETECT_DISABLE)    ||                                 \
    ((x) == SPI_MD_FAULT_DETECT_ENABLE))

/*! Parameter valid check for SPI parity check mode */
#define IS_SPI_PARITY_CHECK(x)                                                 \
(   ((x) == SPI_PARITY_INVD)                ||                                 \
    ((x) == SPI_PARITY_EVEN)                ||                                 \
    ((x) == SPI_PARITY_ODD))

/*! Parameter valid check for SPI mode */
#define IS_SPI_SPI_MD(x)                                                       \
(   ((x) == SPI_MD_0)                       ||                                 \
    ((x) == SPI_MD_1)                       ||                                 \
    ((x) == SPI_MD_2)                       ||                                 \
    ((x) == SPI_MD_3))

/*! Parameter valid check for SPI SS signal */
#define IS_SPI_SS_PIN(x)                                                       \
(   (x) == SPI_PIN_SS0)

/*! Parameter valid check for SPI baudrate prescaler */
#define IS_SPI_BIT_RATE_DIV(x)                                                 \
(   ((x) == SPI_BR_CLK_DIV2)                ||                                 \
    ((x) == SPI_BR_CLK_DIV4)                ||                                 \
    ((x) == SPI_BR_CLK_DIV8)                ||                                 \
    ((x) == SPI_BR_CLK_DIV16)               ||                                 \
    ((x) == SPI_BR_CLK_DIV32)               ||                                 \
    ((x) == SPI_BR_CLK_DIV64)               ||                                 \
    ((x) == SPI_BR_CLK_DIV128)              ||                                 \
    ((x) == SPI_BR_CLK_DIV256))

/*! Parameter valid check for SPI data bits */
#define IS_SPI_DATA_SIZE(x)                                                    \
(   ((x) == SPI_DATA_SIZE_8BIT)             ||                                 \
    ((x) == SPI_DATA_SIZE_16BIT))

/*! Parameter valid check for SPI LSB MSB mode */
#define IS_SPI_FIRST_BIT(x)                                                    \
(   ((x) == SPI_FIRST_MSB)                  ||                                 \
    ((x) == SPI_FIRST_LSB))

/*! Parameter valid check for SPI Communication mode */
#define IS_SPI_COMM_MD(x)                                                      \
(   ((x) == SPI_COMM_MD_NORMAL)             ||                                 \
    ((x) == SPI_COMM_MD_CONTINUE))

/*! Parameter valid check for interrupt flag */
#define IS_SPI_INT(x)                                                          \
(   ((x) != 0UL)                            &&                                 \
    (((x) | SPI_IRQ_ALL) == SPI_IRQ_ALL))

/*! Parameter valid check for SPI status flag */
#define IS_SPI_STD_FLAG(x)                                                     \
(   ((x) != 0UL)                            &&                                 \
    (((x) | SPI_FLAG_ALL) == SPI_FLAG_ALL))

/*! Parameter valid check for SPI status flag for clear */
#define IS_SPI_CLR_STD_FLAG(x)                                                 \
(   ((x) != 0UL)                            &&                                 \
    (((x) | SPI_FLAG_CLR_ALL) == SPI_FLAG_CLR_ALL))

/*! Parameter valid check for SPI command*/
#define IS_SPI_CMD_ALLOWED(x)                                                  \
(   (READ_REG32_BIT(SPIx->SR, SPI_FLAG_MD_FAULT) == 0UL)    ||                 \
    ((x) == DISABLE))

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
 * @defgroup SPI_Local_Func SPI Local Functions
 * @{
 */

/**
 * @brief  SPI check status.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  u32FlagMask        Bit mask of status flag.
 * @param  [in]  u32Value           Valid value of the status.
 * @param  [in]  u32Timeout         Timeout value.
 * @retval int32_t:
 *         - LL_OK:                 No errors occurred.
 *         - LL_ERR_TIMEOUT:        SPI transmit timeout.
 */
static int32_t SPI_WaitStatus(const CM_SPI_TypeDef *SPIx, uint32_t u32FlagMask, uint32_t u32Value, uint32_t u32Timeout)
{
    int32_t i32Ret = LL_OK;

    while (READ_REG32_BIT(SPIx->SR, u32FlagMask) != u32Value) {
        if (u32Timeout == 0UL) {
            i32Ret = LL_ERR_TIMEOUT;
            break;
        }
        u32Timeout--;
    }
    return i32Ret;
}

/**
 * @brief  SPI transmit and receive data in full duplex mode.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  pvTxBuf            The pointer to the buffer which contains the data to be sent.
 * @param  [out] pvRxBuf            The pointer to the buffer which the received data will be stored.
 * @param  [in]  u32Len             The length of the data in byte or half word.
 * @param  [in]  u32Timeout         Timeout value.
 * @retval int32_t:
 *         - LL_OK:                 No errors occurred
 *         - LL_ERR_TIMEOUT:        SPI transmit and receive timeout.
 */
static int32_t SPI_TxRx(CM_SPI_TypeDef *SPIx, const void *pvTxBuf, void *pvRxBuf, uint32_t u32Len, uint32_t u32Timeout)
{
    uint32_t u32BitSize;
    __IO uint32_t u32Count = 0U;
    int32_t i32Ret = LL_OK;
    uint32_t u32Tmp;
    __UNUSED __IO uint32_t u32Read;

    /* Get data bit size, SPI_DATA_SIZE_4BIT ~ SPI_DATA_SIZE_32BIT */
    u32BitSize = READ_REG32_BIT(SPIx->CFG2, SPI_CFG2_DSIZE);

    while (u32Count < u32Len) {
        if (pvTxBuf != NULL) {
            if (u32BitSize <= SPI_DATA_SIZE_8BIT) {
                /* SPI_DATA_SIZE_4BIT ~ SPI_DATA_SIZE_8BIT */
                WRITE_REG32(SPIx->DR, ((const uint8_t *)pvTxBuf)[u32Count]);
            } else {
                /* SPI_DATA_SIZE_16BIT */
                WRITE_REG32(SPIx->DR, ((const uint16_t *)pvTxBuf)[u32Count]);
            }
        } else {
            WRITE_REG32(SPIx->DR, 0xFFFFFFFFUL);
        }

        /* Check RX buffer. */
        i32Ret = SPI_WaitStatus(SPIx, SPI_FLAG_RX_BUF_FULL, SPI_FLAG_RX_BUF_FULL, u32Timeout);
        if (i32Ret == LL_OK) {
            u32Tmp = READ_REG32(SPIx->DR);
            if (pvRxBuf != NULL) {
                if (u32BitSize <= SPI_DATA_SIZE_8BIT) {
                    /* SPI_DATA_SIZE_4BIT ~ SPI_DATA_SIZE_8BIT */
                    ((uint8_t *)pvRxBuf)[u32Count] = (uint8_t)u32Tmp;
                } else {
                    /* SPI_DATA_SIZE_16BIT */
                    ((uint16_t *)pvRxBuf)[u32Count] = (uint16_t)u32Tmp;
                }
            } else {
                /* Dummy read */
                u32Read = READ_REG32(SPIx->DR);
            }
            u32Count++;
        } else {
            break;
        }
    }
    if (i32Ret == LL_OK) {
        i32Ret = SPI_WaitStatus(SPIx, SPI_FLAG_IDLE, 0UL, u32Timeout);
    }

    return i32Ret;
}

/**
 * @brief  SPI send data only.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  pvTxBuf            The pointer to the buffer which contains the data to be sent.
 * @param  [in]  u32Len             The length of the data in byte or half word or word.
 * @param  [in]  u32Timeout         Timeout value.
 * @retval int32_t:
 *         - LL_OK:                 No errors occurred.
 *         - LL_ERR_TIMEOUT:        SPI transmit timeout.
 */
static int32_t SPI_Tx(CM_SPI_TypeDef *SPIx, const void *pvTxBuf, uint32_t u32Len, uint32_t u32Timeout)
{
    __IO uint32_t u32Count = 0U;
    uint32_t u32BitSize;
    int32_t i32Ret = LL_OK;

    /* Get data bit size, SPI_DATA_SIZE_4BIT ~ SPI_DATA_SIZE_32BIT */
    u32BitSize = READ_REG32_BIT(SPIx->CFG2, SPI_CFG2_DSIZE);

    while (u32Count < u32Len) {
        if (u32BitSize <= SPI_DATA_SIZE_8BIT) {
            /* SPI_DATA_SIZE_4BIT ~ SPI_DATA_SIZE_8BIT */
            WRITE_REG32(SPIx->DR, ((const uint8_t *)pvTxBuf)[u32Count]);
        } else {
            /* SPI_DATA_SIZE_16BIT */
            WRITE_REG32(SPIx->DR, ((const uint16_t *)pvTxBuf)[u32Count]);
        }
        /* Wait TX buffer empty. */
        i32Ret = SPI_WaitStatus(SPIx, SPI_FLAG_TX_BUF_EMPTY, SPI_FLAG_TX_BUF_EMPTY, u32Timeout);
        if (i32Ret != LL_OK) {
            break;
        }
        u32Count++;
    }
    if (i32Ret == LL_OK) {
        i32Ret = SPI_WaitStatus(SPIx, SPI_FLAG_IDLE, 0UL, u32Timeout);
    }

    return i32Ret;
}

/**
 * @}
 */

/**
 * @defgroup SPI_Global_Functions SPI Global Functions
 * @{
 */

/**
 * @brief  Initializes the SPI peripheral according to the specified parameters
 *         in the structure stc_spi_init.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  pstcSpiInit        Pointer to a stc_spi_init_t structure that contains
 *                                  the configuration information for the SPI.
 * @retval int32_t:
 *         - LL_OK:                 No errors occurred
 *         - LL_ERR_INVD_PARAM:     pstcSpiInit == NULL or configuration parameter error.
 */

int32_t SPI_Init(CM_SPI_TypeDef *SPIx, const stc_spi_init_t *pstcSpiInit)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));

    if (NULL != pstcSpiInit) {
        DDL_ASSERT(IS_SPI_WIRE_MD(pstcSpiInit->u32WireMode));
        DDL_ASSERT(IS_SPI_TRANS_MD(pstcSpiInit->u32TransMode));
        DDL_ASSERT(IS_SPI_MASTER_SLAVE(pstcSpiInit->u32MasterSlave));
        DDL_ASSERT(IS_SPI_MD_FAULT_DETECT_CMD(pstcSpiInit->u32ModeFaultDetect));
        DDL_ASSERT(IS_SPI_PARITY_CHECK(pstcSpiInit->u32Parity));
        DDL_ASSERT(IS_SPI_SPI_MD(pstcSpiInit->u32SpiMode));
        DDL_ASSERT(IS_SPI_BIT_RATE_DIV(pstcSpiInit->u32BaudRatePrescaler));
        DDL_ASSERT(IS_SPI_DATA_SIZE(pstcSpiInit->u32DataBits));
        DDL_ASSERT(IS_SPI_FIRST_BIT(pstcSpiInit->u32FirstBit));

        /* Configuration parameter check */
        if ((SPI_MASTER == pstcSpiInit->u32MasterSlave) && (SPI_MD_FAULT_DETECT_ENABLE == pstcSpiInit->u32ModeFaultDetect)) {
            /* pstcSpiInit->u32ModeFaultDetect can not be SPI_MD_FAULT_DETECT_ENABLE in master mode */
        } else if ((SPI_3_WIRE == pstcSpiInit->u32WireMode) && (SPI_SLAVE == pstcSpiInit->u32MasterSlave)
                   && ((SPI_MD_0 == pstcSpiInit->u32SpiMode) || (SPI_MD_2 == pstcSpiInit->u32SpiMode))) {
            /* SPI_3_WIRE can not support SPI_MD_0 and SPI_MD_2 */
        } else {
            WRITE_REG32(SPIx->CR1, pstcSpiInit->u32WireMode | pstcSpiInit->u32TransMode | pstcSpiInit->u32MasterSlave
                        | pstcSpiInit->u32ModeFaultDetect | pstcSpiInit->u32Parity);
            WRITE_REG32(SPIx->CFG2, pstcSpiInit->u32SpiMode | pstcSpiInit->u32BaudRatePrescaler | pstcSpiInit->u32DataBits
                        | pstcSpiInit->u32FirstBit);
            i32Ret = LL_OK;
        }
    }
    return i32Ret;
}

/**
 * @brief  De-initializes the SPI peripheral.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @retval None
 */
void SPI_DeInit(CM_SPI_TypeDef *SPIx)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));

    WRITE_REG32(SPIx->CR1, 0UL);
    WRITE_REG32(SPIx->CFG1, SPI_CFG1_DEFAULT);
    WRITE_REG32(SPIx->CFG2, SPI_CFG2_DEFAULT);
    CLR_REG32_BIT(SPIx->SR, SPI_FLAG_CLR_ALL);
}

/**
 * @brief  Set a default value for the SPI initialization structure.
 * @param  [in]  pstcSpiInit        Pointer to a stc_spi_init_t structure that
 *                                  contains configuration information.
 * @retval int32_t:
 *         - LL_OK:                 No errors occurred.
 *         - LL_ERR_INVD_PARAM:     pstcSpiInit == NULL.
 */
int32_t SPI_StructInit(stc_spi_init_t *pstcSpiInit)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    if (NULL != pstcSpiInit) {
        pstcSpiInit->u32WireMode          = SPI_4_WIRE;
        pstcSpiInit->u32TransMode         = SPI_FULL_DUPLEX;
        pstcSpiInit->u32MasterSlave       = SPI_MASTER;
        pstcSpiInit->u32ModeFaultDetect   = SPI_MD_FAULT_DETECT_DISABLE;
        pstcSpiInit->u32Parity            = SPI_PARITY_INVD;
        pstcSpiInit->u32SpiMode           = SPI_MD_0;
        pstcSpiInit->u32BaudRatePrescaler = SPI_BR_CLK_DIV8;
        pstcSpiInit->u32DataBits          = SPI_DATA_SIZE_8BIT;
        pstcSpiInit->u32FirstBit          = SPI_FIRST_MSB;
        i32Ret = LL_OK;
    }
    return i32Ret;
}

/**
 * @brief  Enable or disable SPI interrupt.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  u32IntType         SPI interrupt type. Can be one or any
 *                                  combination of the parameter @ref SPI_Int_Type_Define
 * @param  [in]  enNewState         An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void SPI_IntCmd(CM_SPI_TypeDef *SPIx, uint32_t u32IntType, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_SPI_INT(u32IntType));

    if (enNewState == ENABLE) {
        SET_REG32_BIT(SPIx->CR1, u32IntType);
    } else {
        CLR_REG32_BIT(SPIx->CR1, u32IntType);
    }
}

/**
 * @brief  SPI function enable or disable.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  enNewState         An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void SPI_Cmd(CM_SPI_TypeDef *SPIx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));
    DDL_ASSERT(IS_SPI_CMD_ALLOWED(enNewState));

    if (ENABLE == enNewState) {
        SET_REG32_BIT(SPIx->CR1, SPI_CR1_SPE);
    } else {
        CLR_REG32_BIT(SPIx->CR1, SPI_CR1_SPE);
    }
}

/**
 * @brief  Write SPI data register.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  u32Data            The data will be written to the data register.
 * @retval None.
 */
void SPI_WriteData(CM_SPI_TypeDef *SPIx, uint32_t u32Data)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    WRITE_REG32(SPIx->DR, u32Data);
}

/**
 * @brief  Read SPI data register.
 * @param  [in]  SPIx               SPI unit
 *   @arg  CM_SPIx or CM_SPI
 * @retval uint32_t                 A 32-bit data of SPI data register.
 */
uint32_t SPI_ReadData(const CM_SPI_TypeDef *SPIx)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));

    return READ_REG32(SPIx->DR);
}

/**
 * @brief  SPI get status flag.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  u32Flag            SPI state flag. Can be one or any
 *                                  combination of the parameter of @ref SPI_State_Flag_Define
 * @retval An @ref en_flag_status_t enumeration type value.
 */
en_flag_status_t SPI_GetStatus(const CM_SPI_TypeDef *SPIx, uint32_t u32Flag)
{
    en_flag_status_t enFlag = RESET;
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_SPI_STD_FLAG(u32Flag));

    if (0U != READ_REG32_BIT(SPIx->SR, u32Flag)) {
        enFlag = SET;
    }

    return enFlag;
}

/**
 * @brief  SPI clear state flag.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  u32Flag            SPI state flag. Can be one or any combination of the parameter below
 *   @arg  SPI_FLAG_OVERLOAD
 *   @arg  SPI_FLAG_MD_FAULT
 *   @arg  SPI_FLAG_PARITY_ERR
 *   @arg  SPI_FLAG_UNDERLOAD
 * @retval None
 */
void SPI_ClearStatus(CM_SPI_TypeDef *SPIx, uint32_t u32Flag)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_SPI_CLR_STD_FLAG(u32Flag));

    CLR_REG32_BIT(SPIx->SR, u32Flag);
}

/**
 * @brief  SPI loopback function configuration.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  u32Mode            Loopback mode. Can be one parameter @ref SPI_Loopback_Selection_Define
 * @retval None
 */
void SPI_LoopbackModeConfig(CM_SPI_TypeDef *SPIx, uint32_t u32Mode)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_SPI_SPLPBK(u32Mode));

    MODIFY_REG32(SPIx->CR1, SPI_CR1_SPLPBK | SPI_CR1_SPLPBK2, u32Mode);
}

/**
 * @brief  SPI parity check error self diagnosis function enable or disable.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  enNewState         An @ref en_functional_state_t enumeration value.
 * @retval None
 */

void SPI_ParityCheckCmd(CM_SPI_TypeDef *SPIx, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (ENABLE == enNewState) {
        SET_REG32_BIT(SPIx->CR1, SPI_CR1_PATE);
    } else {
        CLR_REG32_BIT(SPIx->CR1, SPI_CR1_PATE);
    }
}

/**
 * @brief  SPI SS signal valid level configuration
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  u32SSPin           Specify the SS pin @ref SPI_SS_Pin_Define
 * @param  [in]  enNewState         An @ref en_functional_state_t enumeration value.
 * @retval None
 */
void SPI_SSValidLevelConfig(CM_SPI_TypeDef *SPIx, uint32_t u32SSPin, en_functional_state_t enNewState)
{
    DDL_ASSERT(IS_VALID_SPI_UNIT(SPIx));
    DDL_ASSERT(IS_SPI_SS_PIN(u32SSPin));
    DDL_ASSERT(IS_FUNCTIONAL_STATE(enNewState));

    if (ENABLE == enNewState) {
        SET_REG32_BIT(SPIx->CFG1, u32SSPin);
    } else {
        CLR_REG32_BIT(SPIx->CFG1, u32SSPin);
    }
}

/**
 * @brief  SPI transmit data.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  pvTxBuf            The pointer to the buffer which contains the data to be sent.
 * @param  [in]  u32TxLen           The length of the data to be sent.
 * @param  [in]  u32Timeout         Timeout value.
 * @retval int32_t:
 *         - LL_OK:                 No errors occurred
 *         - LL_ERR_TIMEOUT:        SPI transmit timeout.
 *         - LL_ERR_INVD_PARAM:     pvTxBuf == NULL or u32TxLen == 0U
 * @note   -No SS pin active and inactive operation in 3-wire mode. Add operations of SS pin depending on your application.
 *         -This function supports full duplex mode and send only mode.
 */
int32_t SPI_Trans(CM_SPI_TypeDef *SPIx, const void *pvTxBuf, uint32_t u32TxLen, uint32_t u32Timeout)
{
    uint32_t u32Flags;
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    if ((pvTxBuf != NULL) && (u32TxLen != 0U)) {
        u32Flags = READ_REG32_BIT(SPIx->CR1, SPI_CR1_TXMDS);
        if (u32Flags == SPI_SEND_ONLY) {
            /* Transmit data in send only mode. */
            i32Ret = SPI_Tx(SPIx, pvTxBuf, u32TxLen, u32Timeout);
        } else {
            /* Transmit data in full duplex mode. */
            i32Ret = SPI_TxRx(SPIx, pvTxBuf, NULL, u32TxLen, u32Timeout);
        }
    }
    return i32Ret;
}

/**
 * @brief  SPI receive data.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  pvRxBuf            The pointer to the buffer which the received data to be stored.
 * @param  [in]  u32RxLen           The length of the data to be received.
 * @param  [in]  u32Timeout         Timeout value.
 * @retval int32_t:
 *         - LL_OK:                 No errors occurred
 *         - LL_ERR_TIMEOUT:        SPI receive timeout.
 *         - LL_ERR_INVD_PARAM:     pvRxBuf == NULL or u32RxLen == 0U
 * @note   -No SS pin active and inactive operation in 3-wire mode. Add operations of SS pin depending on your application.
 *         -This function only works in full duplex master mode.
 */
int32_t SPI_Receive(CM_SPI_TypeDef *SPIx, void *pvRxBuf, uint32_t u32RxLen, uint32_t u32Timeout)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    if ((pvRxBuf != NULL) && (u32RxLen != 0U)) {
        /* Receives data in full duplex master mode. */
        i32Ret = SPI_TxRx(SPIx, NULL, pvRxBuf, u32RxLen, u32Timeout);
    }
    return i32Ret;
}

/**
 * @brief  SPI transmit and receive data.
 * @param  [in]  SPIx               SPI unit
 *   @arg CM_SPIx or CM_SPI
 * @param  [in]  pvTxBuf            The pointer to the buffer which contains the data to be sent.
 *                                  If this pointer is NULL and the pvRxBuf is NOT NULL, the MOSI output high
 *                                  and the the received data will be stored in the buffer pointed by pvRxBuf.
 * @param  [out] pvRxBuf            The pointer to the buffer which the received data will be stored.
 *                                  This for full duplex transfer.
 * @param  [in]  u32Len             The length of the data(in byte or half word) to be sent and received.
 * @param  [in]  u32Timeout         Timeout value.
 * @retval int32_t:
 *         - LL_OK:                 No errors occurred
 *         - LL_ERR_TIMEOUT:        SPI transmit and receive timeout.
 *         - LL_ERR_INVD_PARAM:     pvRxBuf == NULL or pvRxBuf == NULL or u32Len == 0U
 * @note   SPI receives data while sending data. Only works in full duplex master mode.
 */
int32_t SPI_TransReceive(CM_SPI_TypeDef *SPIx, const void *pvTxBuf, void *pvRxBuf, uint32_t u32Len, uint32_t u32Timeout)
{
    int32_t i32Ret = LL_ERR_INVD_PARAM;

    if ((pvTxBuf != NULL) && (pvRxBuf != NULL) && (u32Len != 0U)) {
        /* Transmit and receive data in full duplex master mode. */
        i32Ret = SPI_TxRx(SPIx, pvTxBuf, pvRxBuf, u32Len, u32Timeout);
    }
    return i32Ret;
}
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

/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/
