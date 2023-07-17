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
;/*  Startup for MDK                                                         */
;/*  Version     V1.0                                                        */
;/*  Date        2022-03-31                                                  */
;/*  Target-mcu  HC32F120                                                    */
;/****************************************************************************/

; Stack Configuration
; Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; Heap Configuration
;  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
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

__Vectors_End

__Vectors_Size                     EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main

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
                LDR     R0, =__main
                BX      R0
                ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  EXTINT00_Handler          [WEAK]
                EXPORT  EXTINT01_Handler          [WEAK]
                EXPORT  EXTINT02_Handler          [WEAK]
                EXPORT  EXTINT03_Handler          [WEAK]
                EXPORT  EXTINT04_Handler          [WEAK]
                EXPORT  EXTINT05_Handler          [WEAK]
                EXPORT  EXTINT06_Handler          [WEAK]
                EXPORT  EXTINT07_Handler          [WEAK]
                EXPORT  IRQ008_Handler            [WEAK]
                EXPORT  IRQ009_Handler            [WEAK]
                EXPORT  IRQ010_Handler            [WEAK]
                EXPORT  IRQ011_Handler            [WEAK]
                EXPORT  IRQ012_Handler            [WEAK]
                EXPORT  IRQ013_Handler            [WEAK]
                EXPORT  IRQ014_Handler            [WEAK]
                EXPORT  IRQ015_Handler            [WEAK]
                EXPORT  IRQ016_Handler            [WEAK]
                EXPORT  IRQ017_Handler            [WEAK]
                EXPORT  IRQ018_Handler            [WEAK]
                EXPORT  IRQ019_Handler            [WEAK]
                EXPORT  IRQ020_Handler            [WEAK]
                EXPORT  IRQ021_Handler            [WEAK]
                EXPORT  IRQ022_Handler            [WEAK]
                EXPORT  IRQ023_Handler            [WEAK]
                EXPORT  IRQ024_Handler            [WEAK]
                EXPORT  IRQ025_Handler            [WEAK]
                EXPORT  IRQ026_Handler            [WEAK]
                EXPORT  IRQ027_Handler            [WEAK]
                EXPORT  IRQ028_Handler            [WEAK]
                EXPORT  IRQ029_Handler            [WEAK]
                EXPORT  IRQ030_Handler            [WEAK]
                EXPORT  IRQ031_Handler            [WEAK]

EXTINT00_Handler
EXTINT01_Handler
EXTINT02_Handler
EXTINT03_Handler
EXTINT04_Handler
EXTINT05_Handler
EXTINT06_Handler
EXTINT07_Handler
IRQ008_Handler
IRQ009_Handler
IRQ010_Handler
IRQ011_Handler
IRQ012_Handler
IRQ013_Handler
IRQ014_Handler
IRQ015_Handler
IRQ016_Handler
IRQ017_Handler
IRQ018_Handler
IRQ019_Handler
IRQ020_Handler
IRQ021_Handler
IRQ022_Handler
IRQ023_Handler
IRQ024_Handler
IRQ025_Handler
IRQ026_Handler
IRQ027_Handler
IRQ028_Handler
IRQ029_Handler
IRQ030_Handler
IRQ031_Handler
                B .

                ENDP

                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

                END
