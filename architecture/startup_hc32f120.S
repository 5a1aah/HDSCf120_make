;/*****************************************************************************
; * Copyright (C) 2022-2023, Xiaohua Semiconductor Co., Ltd. All rights reserved.
; *
; * This software component is licensed by XHSC under BSD 3-Clause license
; * (the "License"); You may not use this file except in compliance with the
; * License. You may obtain a copy of the License at:
; *                    opensource.org/licenses/BSD-3-Clause
; *
; */
/*****************************************************************************/
/*  Startup for GCC                                                          */
/*  Version     V1.0                                                         */
/*  Date        2022-03-31                                                   */
/*  Target-mcu  HC32F120                                                     */
/*****************************************************************************/

/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

                .syntax     unified
                .arch       armv6-m
                .cpu        cortex-m0plus
                .thumb

/*
;<h> Stack Configuration
;  <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>
*/
                .equ        Stack_Size, 0x00000300

                .section    .stack
                .align      3
                .globl      __StackTop
                .globl      __StackLimit
__StackLimit:
                .space      Stack_Size
                .size       __StackLimit, . - __StackLimit
__StackTop:
                .size       __StackTop, . - __StackTop


/*
;<h> Heap Configuration
;  <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>
*/
                .equ        Heap_Size, 0x00000300

                .if         Heap_Size != 0                     /* Heap is provided */
                .section    .heap
                .align      3
                .globl      __HeapBase
                .globl      __HeapLimit
__HeapBase:
                .space      Heap_Size
                .size       __HeapBase, . - __HeapBase
__HeapLimit:
                .size       __HeapLimit, . - __HeapLimit
                .endif

/*
;<h> Interrupt vector table start.
*/
                .section    .vectors, "a", %progbits
                .align      2
                .type       __Vectors, %object
                .globl      __Vectors
                .globl      __Vectors_End
                .globl      __Vectors_Size
__Vectors:
                .long       __StackTop                         /*     Top of Stack */
                .long       Reset_Handler                      /*     Reset Handler */
                .long       NMI_Handler                        /* -14 NMI Handler */
                .long       HardFault_Handler                  /* -13 Hard Fault Handler */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       SVC_Handler                        /*  -5 SVCall Handler */
                .long       0                                  /*     Reserved */
                .long       0                                  /*     Reserved */
                .long       PendSV_Handler                     /*  -2 PendSV Handler */
                .long       SysTick_Handler                    /*  -1 SysTick Handler */

                /* Interrupts */
                .long       EXTINT00_Handler
                .long       EXTINT01_Handler
                .long       EXTINT02_Handler
                .long       EXTINT03_Handler
                .long       EXTINT04_Handler
                .long       EXTINT05_Handler
                .long       EXTINT06_Handler
                .long       EXTINT07_Handler
                .long       IRQ008_Handler
                .long       IRQ009_Handler
                .long       IRQ010_Handler
                .long       IRQ011_Handler
                .long       IRQ012_Handler
                .long       IRQ013_Handler
                .long       IRQ014_Handler
                .long       IRQ015_Handler
                .long       IRQ016_Handler
                .long       IRQ017_Handler
                .long       IRQ018_Handler
                .long       IRQ019_Handler
                .long       IRQ020_Handler
                .long       IRQ021_Handler
                .long       IRQ022_Handler
                .long       IRQ023_Handler
                .long       IRQ024_Handler
                .long       IRQ025_Handler
                .long       IRQ026_Handler
                .long       IRQ027_Handler
                .long       IRQ028_Handler
                .long       IRQ029_Handler
                .long       IRQ030_Handler
                .long       IRQ031_Handler
__Vectors_End:
                .equ        __Vectors_Size, __Vectors_End - __Vectors
                .size       __Vectors, . - __Vectors
/*
;<h> Interrupt vector table end.
*/

/*
;<h> Reset handler start.
*/
                .section    .text.Reset_Handler
                .align      2
                .weak       Reset_Handler
                .type       Reset_Handler, %function
                .globl      Reset_Handler
Reset_Handler:
                ldr         r2, =0x40014020     /* PWC_FPRC */
                ldr         r0, =0xA502
                str         r0, [r2]            /* PWC_FPRC = 0xA502 */

                movs        r0, #1
                ldr         r1, =0x42280190     /* PWC_RAMCR_RPERDIS */
                strb        r0, [r1]            /* PWC_RAMCR_RPERDIS = 1 */

                ldr         r1, =0x42280004     /* PWC_STPMCR_CKSHRC */
                strb        r0, [r1]            /* PWC_STPMCR_CKSHRC = 1 */

                ldr         r0, =0xA500
                str         r0, [r2]            /* PWC_FPRC = 0xA500 */

/* Single section scheme.
 *
 * The ranges of copy from/to are specified by following symbols
 *   __etext: LMA of start of the section to copy from. Usually end of text
 *   __data_start__: VMA of start of the section to copy to
 *   __data_end__: VMA of end of the section to copy to
 *
 * All addresses must be aligned to 4 bytes boundary.
 */
                /* Copy data from read only memory to RAM. */
CopyData:
                ldr         r1, =__etext
                ldr         r2, =__data_start__
                ldr         r3, =__data_end__

                subs        r3, r2
                ble         CopyLoopExit
CopyLoop:
                subs        r3, #4
                ldr         r0, [r1,r3]
                str         r0, [r2,r3]
                bgt         CopyLoop
CopyLoopExit:

/* This part of work usually is done in C library startup code.
 * Otherwise, define this macro to enable it in this startup.
 *
 * There are two schemes too.
 * One can clear multiple BSS sections. Another can only clear one section.
 * The former is more size expensive than the latter.
 *
 * Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
 * Otherwise define macro __STARTUP_CLEAR_BSS to choose the later.
 */
/* Single BSS section scheme.
 *
 * The BSS section is specified by following symbols
 *   __bss_start__: start of the BSS section.
 *   __bss_end__: end of the BSS section.
 *
 * Both addresses must be aligned to 4 bytes boundary.
 */
                /* Clear BSS section. */
ClearBss:
                ldr         r1, =__bss_start__
                ldr         r2, =__bss_end__

                movs        r0, 0
                subs        r2, r1
                ble         ClearLoopExit
ClearLoop:
                subs        r2, #4
                str         r0, [r1, r2]
                bgt         ClearLoop
ClearLoopExit:
                /* Call the clock system initialization function. */
                bl          SystemInit
                /* Call the application's entry point. */
                bl          main
                bx          lr
                .size       Reset_Handler, . - Reset_Handler
/*
;<h> Reset handler end.
*/

/*
;<h> Default handler start.
*/
                .section    .text.Default_Handler, "ax", %progbits
                .align      2
Default_Handler:
                b           .
                .size       Default_Handler, . - Default_Handler
/*
;<h> Default handler end.
*/

/* Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
                .macro      Set_Default_Handler  Handler_Name
                .weak       \Handler_Name
                .set        \Handler_Name, Default_Handler
                .endm

/* Default exception/interrupt handler */

                Set_Default_Handler    NMI_Handler
                Set_Default_Handler    HardFault_Handler
                Set_Default_Handler    SVC_Handler
                Set_Default_Handler    PendSV_Handler
                Set_Default_Handler    SysTick_Handler

                Set_Default_Handler    EXTINT00_Handler
                Set_Default_Handler    EXTINT01_Handler
                Set_Default_Handler    EXTINT02_Handler
                Set_Default_Handler    EXTINT03_Handler
                Set_Default_Handler    EXTINT04_Handler
                Set_Default_Handler    EXTINT05_Handler
                Set_Default_Handler    EXTINT06_Handler
                Set_Default_Handler    EXTINT07_Handler
                Set_Default_Handler    IRQ008_Handler
                Set_Default_Handler    IRQ009_Handler
                Set_Default_Handler    IRQ010_Handler
                Set_Default_Handler    IRQ011_Handler
                Set_Default_Handler    IRQ012_Handler
                Set_Default_Handler    IRQ013_Handler
                Set_Default_Handler    IRQ014_Handler
                Set_Default_Handler    IRQ015_Handler
                Set_Default_Handler    IRQ016_Handler
                Set_Default_Handler    IRQ017_Handler
                Set_Default_Handler    IRQ018_Handler
                Set_Default_Handler    IRQ019_Handler
                Set_Default_Handler    IRQ020_Handler
                Set_Default_Handler    IRQ021_Handler
                Set_Default_Handler    IRQ022_Handler
                Set_Default_Handler    IRQ023_Handler
                Set_Default_Handler    IRQ024_Handler
                Set_Default_Handler    IRQ025_Handler
                Set_Default_Handler    IRQ026_Handler
                Set_Default_Handler    IRQ027_Handler
                Set_Default_Handler    IRQ028_Handler
                Set_Default_Handler    IRQ029_Handler
                Set_Default_Handler    IRQ030_Handler
                Set_Default_Handler    IRQ031_Handler

                .end
