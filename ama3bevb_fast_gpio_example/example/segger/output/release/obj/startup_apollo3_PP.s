# 1 "C:\\Users\\mschre\\Documents\\Projects\\Apollo3\\sw_examples\\ama3bevb_fast_gpio_example-v10\\example\\segger\\startup_apollo3.s"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "C:\\Users\\mschre\\Documents\\Projects\\Apollo3\\sw_examples\\ama3bevb_fast_gpio_example-v10\\example\\segger\\startup_apollo3.s"
# 12 "C:\\Users\\mschre\\Documents\\Projects\\Apollo3\\sw_examples\\ama3bevb_fast_gpio_example-v10\\example\\segger\\startup_apollo3.s"
.macro ISR_HANDLER name=
  .section .vectors, "ax"
  .word \name
  .section .init, "ax"
  .thumb_func
  .weak \name
\name:
1: b 1b
.endm

.macro ISR_RESERVED
  .section .vectors, "ax"
  .word 0
.endm

  .syntax unified
  .global reset_handler

  .section .vectors, "ax"
  .code 16
  .global _vectors

.macro DEFAULT_ISR_HANDLER name=
  .thumb_func
  .weak \name
\name:
1: b 1b
.endm

_vectors:
  .word __stack_end__
  .word reset_handler
ISR_HANDLER NMI_Handler
ISR_HANDLER HardFault_Handler
ISR_HANDLER MemManage_Handler
ISR_HANDLER BusFault_Handler
ISR_HANDLER UsageFault_Handler
ISR_RESERVED
ISR_RESERVED
ISR_RESERVED
ISR_RESERVED
ISR_HANDLER SVC_Handler
ISR_HANDLER DebugMon_Handler
ISR_RESERVED
ISR_HANDLER PendSV_Handler
ISR_HANDLER SysTick_Handler



ISR_HANDLER BROWNOUT_IRQHandler
ISR_HANDLER WDT_IRQHandler
ISR_HANDLER RTC_IRQHandler
ISR_HANDLER VCOMP_IRQHandler
ISR_HANDLER IOSLAVE_IRQHandler
ISR_HANDLER IOSLAVEACC_IRQHandler
ISR_HANDLER IOMSTR0_IRQHandler
ISR_HANDLER IOMSTR1_IRQHandler
ISR_HANDLER IOMSTR2_IRQHandler
ISR_HANDLER IOMSTR3_IRQHandler
ISR_HANDLER IOMSTR4_IRQHandler
ISR_HANDLER IOMSTR5_IRQHandler
ISR_HANDLER BLE_IRQHandler
ISR_HANDLER GPIO_IRQHandler
ISR_HANDLER CTIMER_IRQHandler
ISR_HANDLER UART0_IRQHandler
ISR_HANDLER UART1_IRQHandler
ISR_HANDLER SCARD_IRQHandler
ISR_HANDLER ADC_IRQHandler
ISR_HANDLER PDM_IRQHandler
ISR_HANDLER MSPI_IRQHandler
ISR_RESERVED
ISR_HANDLER STIMER_IRQHandler
ISR_HANDLER STIMER_CMPR0_IRQHandler
ISR_HANDLER STIMER_CMPR1_IRQHandler
ISR_HANDLER STIMER_CMPR2_IRQHandler
ISR_HANDLER STIMER_CMPR3_IRQHandler
ISR_HANDLER STIMER_CMPR4_IRQHandler
ISR_HANDLER STIMER_CMPR5_IRQHandler
ISR_HANDLER STIMER_CMPR6_IRQHandler
ISR_HANDLER STIMER_CMPR7_IRQHandler
ISR_HANDLER CLKGEN_IRQHandler


  .section .vectors, "ax"
_vectors_end:

  .section .init, "ax"
  .thumb_func

  reset_handler:

  LDR.W R0, =_vectors
  CMP R0,#0
  BEQ.N _end_vector_relocation

  LDR.W R1,=0x1FFFFF80
  ANDS R0,R0,R1
  LDR.W R1, =0xE000ED08
  STR R0,[R1]

_end_vector_relocation:

  ldr r0, =__SRAM_segment_end__
  mov sp, r0
  bl SystemInit




  movw r0, 0xED88
  movt r0, 0xE000
  ldr r1, [r0]
  orrs r1, r1, #(0xf << 20)
  str r1, [r0]


  b _start


  .thumb_func
  .weak SystemInit
SystemInit:
  bx lr
