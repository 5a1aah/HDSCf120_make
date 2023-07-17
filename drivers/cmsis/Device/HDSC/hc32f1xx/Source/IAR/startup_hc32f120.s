;/*****************************************************************************
; * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd. All rights reserved.
; *
; * This software component is licensed by XHSC under BSD 3-Clause license
; * (the "License"); You may not use this file except in compliance with the
; * License. You may obtain a copy of the License at:
; *                    opensource.org/licenses/BSD-3-Clause
; *
; */
;/****************************************************************************/
;/*  Startup for IAR                                                         */
;/*  Version     V1.0                                                        */
;/*  Date        2022-03-31                                                  */
;/*  Target-mcu  HC32F120                                                    */
;/****************************************************************************/


                MODULE  ?cstartup

                ;; Forward declaration of sections.
                SECTION CSTACK:DATA:NOROOT(3)

                SECTION .intvec:CODE:NOROOT(2)

                EXTERN  __iar_program_start
                EXTERN  SystemInit
                PUBLIC  __vector_table

                DATA
__vector_table
                DCD     sfe(CSTACK)               ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; Peripheral Interrupts
                DCD     EXTINT00_Handler
                DCD     EXTINT01_Handler
                DCD     EXTINT02_Handler
                DCD     EXTINT03_Handler
                DCD     EXTINT04_Handler
                DCD     EXTINT05_Handler
                DCD     EXTINT06_Handler
                DCD     EXTINT07_Handler
                DCD     IRQ008_Handler
                DCD     IRQ009_Handler
                DCD     IRQ010_Handler
                DCD     IRQ011_Handler
                DCD     IRQ012_Handler
                DCD     IRQ013_Handler
                DCD     IRQ014_Handler
                DCD     IRQ015_Handler
                DCD     IRQ016_Handler
                DCD     IRQ017_Handler
                DCD     IRQ018_Handler
                DCD     IRQ019_Handler
                DCD     IRQ020_Handler
                DCD     IRQ021_Handler
                DCD     IRQ022_Handler
                DCD     IRQ023_Handler
                DCD     IRQ024_Handler
                DCD     IRQ025_Handler
                DCD     IRQ026_Handler
                DCD     IRQ027_Handler
                DCD     IRQ028_Handler
                DCD     IRQ029_Handler
                DCD     IRQ030_Handler
                DCD     IRQ031_Handler

                THUMB
; Dummy Exception Handlers (infinite loops which can be modified)

                PUBWEAK Reset_Handler
                SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
                LDR     R2, =0x40014020 ; PWC_FPRC
                LDR     R0, =0xA502
                STR     R0, [R2]        ; PWC_FPRC = 0xA502

                MOVS    R0, #1
                LDR     R1, =0x42280190 ; PWC_RAMCR_RPERDIS
                STRB    R0, [R1]        ; PWC_RAMCR_RPERDIS = 1

                LDR     R1, =0x42280008
                STRB    R0, [R1]        ; PWC_STPMCR_bit2 = 1

                LDR     R0, =0xA500
                STR     R0, [R2]        ; PWC_FPRC = 0xA500

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__iar_program_start
                BX      R0

                PUBWEAK NMI_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
                B       NMI_Handler

                PUBWEAK HardFault_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
                B       HardFault_Handler

                PUBWEAK SVC_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
                B       SVC_Handler

                PUBWEAK PendSV_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
                B       PendSV_Handler

                PUBWEAK SysTick_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
                B       SysTick_Handler

                PUBWEAK EXTINT00_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXTINT00_Handler
                B       EXTINT00_Handler


                PUBWEAK EXTINT01_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXTINT01_Handler
                B       EXTINT01_Handler


                PUBWEAK EXTINT02_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXTINT02_Handler
                B       EXTINT02_Handler


                PUBWEAK EXTINT03_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXTINT03_Handler
                B       EXTINT03_Handler


                PUBWEAK EXTINT04_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXTINT04_Handler
                B       EXTINT04_Handler


                PUBWEAK EXTINT05_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXTINT05_Handler
                B       EXTINT05_Handler


                PUBWEAK EXTINT06_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXTINT06_Handler
                B       EXTINT06_Handler


                PUBWEAK EXTINT07_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
EXTINT07_Handler
                B       EXTINT07_Handler

                PUBWEAK IRQ008_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ008_Handler
                B       IRQ008_Handler

                PUBWEAK IRQ009_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ009_Handler
                B       IRQ009_Handler

                PUBWEAK IRQ010_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ010_Handler
                B       IRQ010_Handler

                PUBWEAK IRQ011_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ011_Handler
                B       IRQ011_Handler

                PUBWEAK IRQ012_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ012_Handler
                B       IRQ012_Handler

                PUBWEAK IRQ013_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ013_Handler
                B       IRQ013_Handler

                PUBWEAK IRQ014_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ014_Handler
                B       IRQ014_Handler

                PUBWEAK IRQ015_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ015_Handler
                B       IRQ015_Handler

                PUBWEAK IRQ016_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ016_Handler
                B       IRQ016_Handler

                PUBWEAK IRQ017_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ017_Handler
                B       IRQ017_Handler

                PUBWEAK IRQ018_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ018_Handler
                B       IRQ018_Handler

                PUBWEAK IRQ019_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ019_Handler
                B       IRQ019_Handler

                PUBWEAK IRQ020_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ020_Handler
                B       IRQ020_Handler

                PUBWEAK IRQ021_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ021_Handler
                B       IRQ021_Handler

                PUBWEAK IRQ022_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ022_Handler
                B       IRQ022_Handler

                PUBWEAK IRQ023_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ023_Handler
                B       IRQ023_Handler

                PUBWEAK IRQ024_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ024_Handler
                B       IRQ024_Handler

                PUBWEAK IRQ025_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ025_Handler
                B       IRQ025_Handler

                PUBWEAK IRQ026_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ026_Handler
                B       IRQ026_Handler

                PUBWEAK IRQ027_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ027_Handler
                B       IRQ027_Handler

                PUBWEAK IRQ028_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ028_Handler
                B       IRQ028_Handler

                PUBWEAK IRQ029_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ029_Handler
                B       IRQ029_Handler

                PUBWEAK IRQ030_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ030_Handler
                B       IRQ030_Handler

                PUBWEAK IRQ031_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
IRQ031_Handler
                B       IRQ031_Handler

                END
