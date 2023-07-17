/**
 *******************************************************************************
 * @file  hc32_ll_efm.h
 * @brief This file contains all the functions prototypes of the EFM driver
 *        library.
 @verbatim
   Change Logs:
   Date             Author          Notes
   2022-03-31       CDT             First version
   2022-10-31       CDT             Add Flash protect level define.
   2023-01-15       CDT             Code refine.
   2023-01-15       CDT             Remove redundancy code.
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
#ifndef __HC32_LL_EFM_H__
#define __HC32_LL_EFM_H__

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
 * @addtogroup LL_EFM
 * @{
 */

#if (LL_EFM_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup EFM_Global_Types EFM Global Types
 * @{
 */
/**
 * @brief EFM unique ID definition
 */
typedef struct {
    uint32_t            u32UniqueID0;      /*!< unique ID 0.       */
    uint32_t            u32UniqueID1;      /*!< unique ID 1.       */
    uint32_t            u32UniqueID2;      /*!< unique ID 2.       */
} stc_efm_unique_id_t;

/**
 * @}
 */

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup EFM_Global_Macros EFM Global Macros
 * @{
 */
/**
 * @defgroup EFM_Address EFM Address Area
 * @{
 */
#define EFM_START_ADDR                  (0x00000000UL)    /*!< Flash start address */

#define EFM_END_ADDR                    (0x0000FFFFUL)    /*!< Flash end address */

/**
 * @}
 */

/**
 * @defgroup EFM_Chip_Sel EFM Chip Selection
 * @{
 */
#define EFM_CHIP_ALL                    (EFM_FSTP_FSTP)
/**
 * @}
 */

/**
 * @defgroup EFM_Bus_Status EFM Bus Status
 * @{
 */
#define EFM_BUS_HOLD                    (0x0UL)     /*!< Bus busy while flash program or erase */
#define EFM_BUS_RELEASE                 (0x1UL)     /*!< Bus release while flash program or erase */
/**
 * @}
 */

/**
 * @defgroup EFM_Wait_Cycle EFM Wait Cycle
 * @{
 */

#define EFM_WAIT_CYCLE0                 (0U << EFM_FRMC_FLWT_POS)      /*!< Don't insert read wait cycle */
#define EFM_WAIT_CYCLE1                 (1U << EFM_FRMC_FLWT_POS)      /*!< Insert 1 read wait cycle     */

/**
 * @}
 */

/**
 * @defgroup EFM_OperateMode_Sel EFM Operate Mode Selection
 * @{
 */
#define EFM_MD_READONLY                 (0x0UL << EFM_FWMC_PEMOD_POS)   /*!< Read only mode               */
#define EFM_MD_PGM_SINGLE               (0x1UL << EFM_FWMC_PEMOD_POS)   /*!< Program single mode          */
#define EFM_MD_PGM_READBACK             (0x2UL << EFM_FWMC_PEMOD_POS)   /*!< Program and read back mode   */
#define EFM_MD_PGM_SEQ                  (0x3UL << EFM_FWMC_PEMOD_POS)   /*!< Program sequence mode        */
#define EFM_MD_ERASE_SECTOR             (0x4UL << EFM_FWMC_PEMOD_POS)   /*!< Sector erase mode            */

#define EFM_MD_ERASE_ALL_CHIP           (0x5UL << EFM_FWMC_PEMOD_POS)   /*!< Chip erase mode      */
/**
 * @}
 */

/**
 * @defgroup EFM_Flag_Sel  EFM Flag Selection
 * @{
 */
#define EFM_FLAG_PEWERR                 (EFM_FSR_PEWERR)        /*!< EFM Programming/erase error flag.              */
#define EFM_FLAG_PGMISMTCH              (EFM_FSR_PGMISMTCH)     /*!< EFM Programming missing match error flag       */
#define EFM_FLAG_OPTEND                 (EFM_FSR_OPTEND)        /*!< EFM End of operation flag.                     */
#define EFM_FLAG_COLERR                 (EFM_FSR_COLERR)        /*!< EFM Read collide error flag.                   */
#define EFM_FLAG_PEPRTERR               (EFM_FSR_PEPRTERR)      /*!< EFM write protect address error flag           */
#define EFM_FLAG_RDY                    (EFM_FSR_RDY)           /*!< EFM ready flag.                                */

#define EFM_FLAG_ALL                (EFM_FLAG_PEWERR | EFM_FLAG_PGMISMTCH | EFM_FLAG_OPTEND | \
                                     EFM_FLAG_COLERR | EFM_FLAG_PEPRTERR  | EFM_FLAG_RDY)

/**
 * @}
 */

/**
 * @defgroup EFM_Interrupt_Sel EFM Interrupt Selection
 * @{
 */
#define EFM_INT_PEERR                   (EFM_FITE_PEERRITE)     /*!< Program/erase error Interrupt source    */
#define EFM_INT_OPTEND                  (EFM_FITE_OPTENDITE)    /*!< End of EFM operation Interrupt source   */
#define EFM_INT_COLERR                  (EFM_FITE_COLERRITE)    /*!< Read collide error Interrupt source     */

#define EFM_INT_ALL                     (EFM_FITE_PEERRITE | EFM_FITE_OPTENDITE | EFM_FITE_COLERRITE)
/**
 * @}
 */

/**
 * @defgroup EFM_Keys EFM Keys
 * @{
 */
#define EFM_REG_UNLOCK_KEY1             (0x0123UL)
#define EFM_REG_UNLOCK_KEY2             (0x3210UL)
#define EFM_REG_LOCK_KEY                (0x0000UL)
/**
 * @}
 */

/**
 * @defgroup EFM_Sector_Size EFM Sector Size
 * @{
 */
#define SECTOR_SIZE                     (0x200UL)
/**
 * @}
 */

/**
 * @defgroup EFM_Sector_Address EFM Sector Address
 * @{
 */
#define EFM_SECTOR_ADDR(x)          (uint32_t)(SECTOR_SIZE * (x))
/**
 * @}
 */

/**
 * @defgroup EFM_Protect_Level EFM protect level
 * @{
 */
#define EFM_PROTECT_LEVEL1          (1U)
#define EFM_PROTECT_LEVEL2          (2U)
/**
 * @}
 */

/**
 * @defgroup EFM_MCU_Status EFM protect level
 * @{
 */
#define EFM_MCU_PROTECT1_FREE       (0U)
#define EFM_MCU_PROTECT1_LOCK       (1U)
#define EFM_MCU_PROTECT1_UNLOCK     (2U)
#define EFM_MCU_PROTECT2_LOCK       (4U)
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
 * @addtogroup EFM_Global_Functions
 * @{
 */

/**
 * @brief  EFM Protect Unlock.
 * @param  None
 * @retval None
 */

__STATIC_INLINE void EFM_REG_Unlock(void)
{
    WRITE_REG32(CM_EFM->FAPRT, EFM_REG_UNLOCK_KEY1);
    WRITE_REG32(CM_EFM->FAPRT, EFM_REG_UNLOCK_KEY2);
}

/**
 * @brief  EFM Protect Lock.
 * @param  None
 * @retval None
 */
__STATIC_INLINE void EFM_REG_Lock(void)
{
    WRITE_REG32(CM_EFM->FAPRT, EFM_REG_LOCK_KEY);
}

void EFM_Cmd(uint32_t u32Flash, en_functional_state_t enNewState);
void EFM_FWMC_Cmd(en_functional_state_t enNewState);
void EFM_SetBusStatus(uint32_t u32Status);
void EFM_IntCmd(uint32_t u32EfmInt, en_functional_state_t enNewState);
void EFM_ClearStatus(uint32_t u32Flag);
int32_t EFM_SetWaitCycle(uint32_t u32WaitCycle);
int32_t EFM_SetOperateMode(uint32_t u32Mode);
int32_t EFM_ReadByte(uint32_t u32Addr, uint8_t *pu8ReadBuf, uint32_t u32ByteLen);
int32_t EFM_Program(uint32_t u32Addr, uint8_t *pu8Buf, uint32_t u32Len);
int32_t EFM_ProgramWord(uint32_t u32Addr, uint32_t u32Data);
int32_t EFM_ProgramWordReadBack(uint32_t u32Addr, uint32_t u32Data);
int32_t EFM_SequenceProgram(uint32_t u32Addr, uint8_t *pu8Buf, uint32_t u32Len);
int32_t EFM_SectorErase(uint32_t u32Addr);
int32_t EFM_ChipErase(uint8_t u8Chip);

int32_t EFM_ProgramHalfWord(uint32_t u32Addr, uint16_t u16Data);
int32_t EFM_ProgramHalfWordReadBack(uint32_t u32Addr, uint16_t u16Data);

int32_t EFM_ProgramByte(uint32_t u32Addr, uint8_t u8Data);
int32_t EFM_ProgramByteReadBack(uint32_t u32Addr, uint8_t u8Data);

en_flag_status_t EFM_GetAnyStatus(uint32_t u32Flag);
en_flag_status_t EFM_GetStatus(uint32_t u32Flag);
void EFM_GetUID(stc_efm_unique_id_t *pstcUID);

void EFM_PrefetchCmd(en_functional_state_t enNewState);

void EFM_SetWindowProtectAddr(uint32_t u32StartAddr, uint32_t u32EndAddr);

void EFM_Protect_Enable(uint8_t u8Level);
int32_t EFM_WriteSecurityCode(uint8_t *pu8Buf, uint32_t u32Len);

/**
 * @}
 */

#endif /* LL_EFM_ENABLE */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32_LL_EFM_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
