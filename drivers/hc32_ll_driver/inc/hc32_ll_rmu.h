/**
 *******************************************************************************
 * @file  hc32_ll_rmu.h
 * @brief This file contains all the functions prototypes of the RMU driver
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
#ifndef __HC32_LL_RMU_H__
#define __HC32_LL_RMU_H__

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
 * @addtogroup LL_RMU
 * @{
 */
#if (LL_RMU_ENABLE == DDL_ON)

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup RMU_Global_Macros RMU Global Macros
 * @{
 */

/**
 * @defgroup RMU_ResetCause Rmu reset cause
 * @{
 */
#define RMU_FLAG_PWR_ON                 (RMU_RSTF0_PORF)        /*!< Power on reset */
#define RMU_FLAG_PIN                    (RMU_RSTF0_PINRF)       /*!< Reset pin reset */
#define RMU_FLAG_LVD                    (RMU_RSTF0_LVRF)        /*!< Low power detect reset */
#define RMU_FLAG_SWDT                   (RMU_RSTF0_WDRF)        /*!< Watchdog timer reset */
#define RMU_FLAG_SW                     (RMU_RSTF0_SWRF)        /*!< Software reset */
#define RMU_FLAG_RAM_PARITY_ERR         (RMU_RSTF0_RAMPERF)     /*!< Ram parity error reset */
#define RMU_FLAG_CPU_LOCKUP             (RMU_RSTF0_CPULKUPRF)   /*!< M0+ Lockup reset  */
#define RMU_FLAG_XTAL_ERR               (RMU_RSTF0_XTALERF)     /*!< Xtal error reset */
#define RMU_FLAG_MX                     (RMU_RSTF0_MULTIRF)     /*!< Multiply reset cause */
#define RMU_FLAG_ALL                    (RMU_FLAG_PWR_ON | RMU_FLAG_PIN | RMU_FLAG_LVD | RMU_FLAG_SWDT | RMU_FLAG_SW | \
                                        RMU_FLAG_RAM_PARITY_ERR | RMU_FLAG_XTAL_ERR | RMU_FLAG_CPU_LOCKUP | RMU_FLAG_MX)

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
 * @addtogroup RMU_Global_Functions
 * @{
 */

en_flag_status_t RMU_GetStatus(uint32_t u32RmuResetCause);
void RMU_ClearStatus(void);

/**
 * @}
 */

#endif /* LL_RMU_ENABLE */

/**
 * @}
 */

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif /* __HC32_LL_RMU_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

