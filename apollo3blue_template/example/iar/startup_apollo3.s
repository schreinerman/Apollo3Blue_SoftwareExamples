;/*******************************************************************************
;*                                                                              *
;*   Abstract    : This file contains interrupt vector and startup code.        *
;*                                                                              *
;*   Functions   : Reset_Handler                                                *
;*                                                                              *
;*   Target      : Cypress FM microcontrollers                                  *
;*                                                                              *
;*   Environment : IAR Embedded Workbench                                       *
;*                                                                              *
;*   Distribution: The file is distributed "as is," without any warranty        *
;*                 of any kind.                                                 *
;*                                                                              *
;*******************************************************************************/

;
;Copyright (C) 2013-2017, Fujitsu Electronics Europe GmbH or a               
;subsidiary of Fujitsu Electronics Europe GmbH.  All rights reserved.        
;                                                                            
;This software, including source code, documentation and related             
;materials ("Software"), is owned by Fujitsu Electronics Europe GmbH or      
;one of its subsidiaries ("Fujitsu").
;                                                                            
;If no EULA applies, Fujitsu hereby grants you a personal, non-exclusive,    
;non-transferable license to copy, modify, and compile the                   
;Software source code solely for use in connection with Fujitsu's            
;integrated circuit products.  Any reproduction, modification, translation,  
;compilation, or representation of this Software except as specified         
;above is prohibited without the express written permission of Fujitsu.      
;                                                                            
;Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO                        
;WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,                        
;BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED                                
;WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                             
;PARTICULAR PURPOSE. Fujitsu reserves the right to make                      
;changes to the Software without notice. Fujitsu does not assume any         
;liability arising out of the application or use of the Software or any      
;product or circuit described in the Software. Fujitsu does not              
;authorize its products for use in any products where a malfunction or       
;failure of the Fujitsu product may reasonably be expected to result in      
;significant property damage, injury or death ("High Risk Product"). By      
;including Fujitsu's product in a High Risk Product, the manufacturer        
;of such system or application assumes all risk of such use and in doing     
;so agrees to indemnify Fujitsu against all liability.                       
;

;/*****************************************************************************/
;/*  Startup for IAR                                                          */
;/*  Version     V1.0                                                         */
;/*  Date        2013-03-22                                                   */
;/*****************************************************************************/


                MODULE  ?cstartup

                ;; Forward declaration of sections.
                SECTION CSTACK:DATA:NOROOT(3)

                SECTION .intvec:CODE:NOROOT(2)

                EXTERN  __iar_program_start
                EXTERN  SystemInit
                PUBLIC  __vector_table

                DATA
__vector_table  DCD     sfe(CSTACK)               ; Top of Stack
		        DCD     Reset_Handler             ; Reset
                DCD     NMI_Handler               ; NMI
                DCD     HardFault_Handler         ; Hard Fault
                DCD     MemManage_Handler         ; MPU Fault
                DCD     BusFault_Handler          ; Bus Fault
                DCD     UsageFault_Handler        ; Usage Fault
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall
                DCD     DebugMon_Handler          ; Debug Monitor
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV
                DCD     SysTick_Handler           ; SysTick

; Numbered IRQ handler vectors

; Note: renaming to device dependent ISR function names are done in
;       pdl.h (section "IRQ name definition for all type MCUs"

                DCD     BROWNOUT_IRQHandler
                DCD     WDT_IRQHandler
                DCD     RTC_IRQHandler
                DCD     VCOMP_IRQHandler
                DCD     IOSLAVE_IRQHandler
                DCD     IOSLAVEACC_IRQHandler
                DCD     IOMSTR0_IRQHandler
                DCD     IOMSTR1_IRQHandler
                DCD     IOMSTR2_IRQHandler
                DCD     IOMSTR3_IRQHandler
                DCD     IOMSTR4_IRQHandler
                DCD     IOMSTR5_IRQHandler
                DCD     BLE_IRQHandler
                DCD     GPIO_IRQHandler
                DCD     CTIMER_IRQHandler
                DCD     UART0_IRQHandler
                DCD     UART1_IRQHandler
                DCD     SCARD_IRQHandler
                DCD     ADC_IRQHandler
                DCD     PDM_IRQHandler
                DCD     MSPI_IRQHandler
                DCD     Dummy
                DCD     STIMER_IRQHandler
                DCD     STIMER_CMPR0_IRQHandler
                DCD     STIMER_CMPR1_IRQHandler
                DCD     STIMER_CMPR2_IRQHandler
                DCD     STIMER_CMPR3_IRQHandler
                DCD     STIMER_CMPR4_IRQHandler
                DCD     STIMER_CMPR5_IRQHandler
                DCD     STIMER_CMPR6_IRQHandler
                DCD     STIMER_CMPR7_IRQHandler
                DCD     CLKGEN_IRQHandler


                THUMB
; Dummy Exception Handlers (infinite loops which can be modified)

                PUBWEAK Reset_Handler
                SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler

                LDR.W R0, =0xE000ED88            ; CPACR is located at address 0xE000ED88
                LDR R1, [R0]                     ; Read CPACR
                ORR R1, R1, #(0xF << 20)         ; Set bits 20-23 to enable CP10 and CP11 coprocessors
                STR R1, [R0]                     ; Write back the modified value to the CPACR

                LDR.W    R0, =__vector_table     
                LDR.W    R1,=0x1FFFFF80          ; AND __vector_table with 0x1FFFFF80         
                ANDS     R0,R0,R1                ; AND __vector_table with 0x1FFFFF80
                LDR.W    R1, =0xE000ED08         ; Load SCB->VTOR Address
                STR      R0,[R1]                 ; Store new vector table offset

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__iar_program_start
                BX      R0
                
                PUBWEAK NMI_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
                B       NMI_Handler

                PUBWEAK HardFault_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
                B       HardFault_Handler

                PUBWEAK MemManage_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
                B       MemManage_Handler

                PUBWEAK BusFault_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
                B       BusFault_Handler

                PUBWEAK UsageFault_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
                B       UsageFault_Handler

                PUBWEAK SVC_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
                B       SVC_Handler

                PUBWEAK DebugMon_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
                B       DebugMon_Handler

                PUBWEAK PendSV_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
                B       PendSV_Handler

                PUBWEAK SysTick_Handler
                SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
                B       SysTick_Handler

                
                PUBWEAK BROWNOUT_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
BROWNOUT_IRQHandler
                B       BROWNOUT_IRQHandler

                PUBWEAK WDT_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
WDT_IRQHandler
                B       WDT_IRQHandler

                PUBWEAK RTC_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
RTC_IRQHandler
                B       RTC_IRQHandler

                PUBWEAK VCOMP_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
VCOMP_IRQHandler
                B       VCOMP_IRQHandler

                PUBWEAK IOSLAVE_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
IOSLAVE_IRQHandler
                B       IOSLAVE_IRQHandler

                PUBWEAK IOSLAVEACC_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
IOSLAVEACC_IRQHandler
                B       IOSLAVEACC_IRQHandler

                PUBWEAK IOMSTR0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
IOMSTR0_IRQHandler
                B       IOMSTR0_IRQHandler

                PUBWEAK IOMSTR1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
IOMSTR1_IRQHandler
                B       IOMSTR1_IRQHandler

                PUBWEAK IOMSTR2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
IOMSTR2_IRQHandler
                B       IOMSTR2_IRQHandler

                PUBWEAK IOMSTR3_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
IOMSTR3_IRQHandler
                B       IOMSTR3_IRQHandler

                PUBWEAK IOMSTR4_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
IOMSTR4_IRQHandler
                B       IOMSTR4_IRQHandler

                PUBWEAK IOMSTR5_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
IOMSTR5_IRQHandler
                B       IOMSTR5_IRQHandler

                PUBWEAK BLE_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
BLE_IRQHandler
                B       BLE_IRQHandler

                PUBWEAK GPIO_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
GPIO_IRQHandler
                B       GPIO_IRQHandler

                PUBWEAK CTIMER_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
CTIMER_IRQHandler
                B       CTIMER_IRQHandler

                PUBWEAK UART0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
UART0_IRQHandler
                B       UART0_IRQHandler

                PUBWEAK UART1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
UART1_IRQHandler
                B       UART1_IRQHandler

                PUBWEAK SCARD_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
SCARD_IRQHandler
                B       SCARD_IRQHandler

                PUBWEAK ADC_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
ADC_IRQHandler
                B       ADC_IRQHandler

                PUBWEAK PDM_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
PDM_IRQHandler
                B       PDM_IRQHandler

                PUBWEAK MSPI_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
MSPI_IRQHandler
                B       MSPI_IRQHandler

                PUBWEAK STIMER_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
STIMER_IRQHandler
                B       STIMER_IRQHandler

                PUBWEAK STIMER_CMPR0_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
STIMER_CMPR0_IRQHandler
                B       STIMER_CMPR0_IRQHandler

                PUBWEAK STIMER_CMPR1_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
STIMER_CMPR1_IRQHandler
                B       STIMER_CMPR1_IRQHandler

                PUBWEAK STIMER_CMPR2_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
STIMER_CMPR2_IRQHandler
                B       STIMER_CMPR2_IRQHandler

                PUBWEAK STIMER_CMPR3_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
STIMER_CMPR3_IRQHandler
                B       STIMER_CMPR3_IRQHandler

                PUBWEAK STIMER_CMPR4_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
STIMER_CMPR4_IRQHandler
                B       STIMER_CMPR4_IRQHandler

                PUBWEAK STIMER_CMPR5_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
STIMER_CMPR5_IRQHandler
                B       STIMER_CMPR5_IRQHandler

                PUBWEAK STIMER_CMPR6_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
STIMER_CMPR6_IRQHandler
                B       STIMER_CMPR6_IRQHandler

                PUBWEAK STIMER_CMPR7_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
STIMER_CMPR7_IRQHandler
                B       STIMER_CMPR7_IRQHandler

                PUBWEAK CLKGEN_IRQHandler
                SECTION .text:CODE:REORDER:NOROOT(1)
CLKGEN_IRQHandler
                B       CLKGEN_IRQHandler




                PUBWEAK Dummy
                SECTION .text:CODE:REORDER:NOROOT(1)
Dummy
                B       Dummy

                END
