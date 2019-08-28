	.cpu cortex-m4
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"system_apollo3.c"
	.text
.Ltext0:
	.global	SystemCoreClock
	.section	.data.SystemCoreClock,"aw",%progbits
	.align	2
	.type	SystemCoreClock, %object
	.size	SystemCoreClock, 4
SystemCoreClock:
	.word	48000000
	.section	.text.SystemCoreClockUpdate,"ax",%progbits
	.align	1
	.global	SystemCoreClockUpdate
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	SystemCoreClockUpdate, %function
SystemCoreClockUpdate:
.LFB123:
	.file 1 "C:\\Users\\mschre\\Documents\\Projects\\Customer\\112Motion\\I2S\\apollo3-iom_i2c_direct_cmd-v10\\common\\system_apollo3.c"
	.loc 1 51 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 57 0
	mov	r3, #1073758208
	ldr	r3, [r3, #24]
	ubfx	r3, r3, #0, #1
	uxtb	r3, r3
	adds	r3, r3, #1
	ldr	r2, .L2
	sdiv	r3, r2, r3
	mov	r2, r3
	ldr	r3, .L2+4
	str	r2, [r3]
	.loc 1 58 0
	nop
	bx	lr
.L3:
	.align	2
.L2:
	.word	48000000
	.word	SystemCoreClock
.LFE123:
	.size	SystemCoreClockUpdate, .-SystemCoreClockUpdate
	.section	.text.SystemInit,"ax",%progbits
	.align	1
	.global	SystemInit
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	SystemInit, %function
SystemInit:
.LFB124:
	.loc 1 71 0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 77 0
	ldr	r3, .L5
	ldr	r2, .L5+4
	str	r2, [r3]
	.loc 1 79 0
	mov	r3, #1073758208
	movs	r2, #71
	str	r2, [r3, #20]
	.loc 1 80 0
	mov	r2, #1073758208
	ldrb	r3, [r2, #24]
	bfc	r3, #0, #1
	strb	r3, [r2, #24]
	.loc 1 81 0
	mov	r3, #1073758208
	movs	r2, #0
	str	r2, [r3, #20]
	.loc 1 82 0
	nop
	bx	lr
.L6:
	.align	2
.L5:
	.word	SystemCoreClock
	.word	48000000
.LFE124:
	.size	SystemInit, .-SystemInit
	.section	.debug_frame,"",%progbits
.Lframe0:
	.4byte	.LECIE0-.LSCIE0
.LSCIE0:
	.4byte	0xffffffff
	.byte	0x3
	.ascii	"\000"
	.uleb128 0x1
	.sleb128 -4
	.uleb128 0xe
	.byte	0xc
	.uleb128 0xd
	.uleb128 0
	.align	2
.LECIE0:
.LSFDE0:
	.4byte	.LEFDE0-.LASFDE0
.LASFDE0:
	.4byte	.Lframe0
	.4byte	.LFB123
	.4byte	.LFE123-.LFB123
	.align	2
.LEFDE0:
.LSFDE2:
	.4byte	.LEFDE2-.LASFDE2
.LASFDE2:
	.4byte	.Lframe0
	.4byte	.LFB124
	.4byte	.LFE124-.LFB124
	.align	2
.LEFDE2:
	.text
.Letext0:
	.file 2 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.10a/include/stdint.h"
	.file 3 "C:\\Users\\mschre\\Documents\\Projects\\Customer\\112Motion\\I2S\\apollo3-iom_i2c_direct_cmd-v10\\common\\system_apollo3.h"
	.file 4 "C:/Users/mschre/AppData/Local/SEGGER/SEGGER Embedded Studio/v3/packages/CMSIS_4/CMSIS/Include/core_cm4.h"
	.file 5 "C:\\Users\\mschre\\Documents\\Projects\\Customer\\112Motion\\I2S\\apollo3-iom_i2c_direct_cmd-v10\\common\\apollo3.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x751
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF76
	.byte	0xc
	.4byte	.LASF77
	.4byte	.LASF78
	.4byte	.Ldebug_ranges0+0
	.4byte	0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF2
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF3
	.uleb128 0x3
	.4byte	.LASF4
	.byte	0x2
	.byte	0x37
	.4byte	0x51
	.uleb128 0x4
	.4byte	0x41
	.uleb128 0x5
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x3
	.4byte	.LASF5
	.byte	0x2
	.byte	0x38
	.4byte	0x6d
	.uleb128 0x4
	.4byte	0x58
	.uleb128 0x6
	.4byte	0x63
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF6
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF7
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF8
	.uleb128 0x7
	.4byte	.LASF9
	.byte	0x3
	.byte	0x18
	.4byte	0x58
	.uleb128 0x8
	.4byte	.LASF10
	.byte	0x4
	.2byte	0x744
	.4byte	0x4c
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x660
	.4byte	0xb3
	.uleb128 0xa
	.4byte	.LASF11
	.byte	0x5
	.2byte	0x661
	.4byte	0x63
	.byte	0x4
	.byte	0xb
	.byte	0x15
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x65d
	.4byte	0xd5
	.uleb128 0xc
	.4byte	.LASF11
	.byte	0x5
	.2byte	0x65e
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF12
	.byte	0x5
	.2byte	0x669
	.4byte	0x99
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x66f
	.4byte	0xef
	.uleb128 0xa
	.4byte	.LASF13
	.byte	0x5
	.2byte	0x670
	.4byte	0x63
	.byte	0x4
	.byte	0x12
	.byte	0xe
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x66c
	.4byte	0x111
	.uleb128 0xc
	.4byte	.LASF13
	.byte	0x5
	.2byte	0x66d
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF14
	.byte	0x5
	.2byte	0x679
	.4byte	0xd5
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x67f
	.4byte	0x12b
	.uleb128 0xa
	.4byte	.LASF15
	.byte	0x5
	.2byte	0x680
	.4byte	0x63
	.byte	0x4
	.byte	0x18
	.byte	0x8
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x67c
	.4byte	0x14d
	.uleb128 0xc
	.4byte	.LASF15
	.byte	0x5
	.2byte	0x67d
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF16
	.byte	0x5
	.2byte	0x683
	.4byte	0x111
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x689
	.4byte	0x1a7
	.uleb128 0xa
	.4byte	.LASF17
	.byte	0x5
	.2byte	0x68a
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1f
	.byte	0
	.uleb128 0xa
	.4byte	.LASF18
	.byte	0x5
	.2byte	0x68b
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1e
	.byte	0
	.uleb128 0xd
	.ascii	"FOS\000"
	.byte	0x5
	.2byte	0x68d
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x19
	.byte	0
	.uleb128 0xa
	.4byte	.LASF19
	.byte	0x5
	.2byte	0x68f
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x18
	.byte	0
	.uleb128 0xa
	.4byte	.LASF20
	.byte	0x5
	.2byte	0x690
	.4byte	0x63
	.byte	0x4
	.byte	0x3
	.byte	0x15
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x686
	.4byte	0x1c9
	.uleb128 0xc
	.4byte	.LASF21
	.byte	0x5
	.2byte	0x687
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF22
	.byte	0x5
	.2byte	0x695
	.4byte	0x14d
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x69b
	.4byte	0x1f3
	.uleb128 0xa
	.4byte	.LASF23
	.byte	0x5
	.2byte	0x69c
	.4byte	0x63
	.byte	0x4
	.byte	0x6
	.byte	0x1a
	.byte	0
	.uleb128 0xa
	.4byte	.LASF24
	.byte	0x5
	.2byte	0x69e
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x18
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x698
	.4byte	0x215
	.uleb128 0xc
	.4byte	.LASF25
	.byte	0x5
	.2byte	0x699
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF26
	.byte	0x5
	.2byte	0x69f
	.4byte	0x1c9
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x6a5
	.4byte	0x22f
	.uleb128 0xa
	.4byte	.LASF27
	.byte	0x5
	.2byte	0x6a6
	.4byte	0x63
	.byte	0x4
	.byte	0x20
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x6a2
	.4byte	0x251
	.uleb128 0xc
	.4byte	.LASF27
	.byte	0x5
	.2byte	0x6a3
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF28
	.byte	0x5
	.2byte	0x6a7
	.4byte	0x215
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x6ad
	.4byte	0x26b
	.uleb128 0xa
	.4byte	.LASF29
	.byte	0x5
	.2byte	0x6ae
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1f
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x6aa
	.4byte	0x28d
	.uleb128 0xc
	.4byte	.LASF30
	.byte	0x5
	.2byte	0x6ab
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF31
	.byte	0x5
	.2byte	0x6af
	.4byte	0x251
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x6b5
	.4byte	0x2b7
	.uleb128 0xa
	.4byte	.LASF32
	.byte	0x5
	.2byte	0x6b6
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1f
	.byte	0
	.uleb128 0xa
	.4byte	.LASF33
	.byte	0x5
	.2byte	0x6b9
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1e
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x6b2
	.4byte	0x2d9
	.uleb128 0xc
	.4byte	.LASF34
	.byte	0x5
	.2byte	0x6b3
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF35
	.byte	0x5
	.2byte	0x6ba
	.4byte	0x28d
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x6c0
	.4byte	0x333
	.uleb128 0xa
	.4byte	.LASF36
	.byte	0x5
	.2byte	0x6c1
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1f
	.byte	0
	.uleb128 0xa
	.4byte	.LASF37
	.byte	0x5
	.2byte	0x6c2
	.4byte	0x63
	.byte	0x4
	.byte	0x3
	.byte	0x1c
	.byte	0
	.uleb128 0xa
	.4byte	.LASF38
	.byte	0x5
	.2byte	0x6c4
	.4byte	0x63
	.byte	0x4
	.byte	0xc
	.byte	0xc
	.byte	0
	.uleb128 0xa
	.4byte	.LASF39
	.byte	0x5
	.2byte	0x6c5
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0xb
	.byte	0
	.uleb128 0xa
	.4byte	.LASF40
	.byte	0x5
	.2byte	0x6c6
	.4byte	0x63
	.byte	0x4
	.byte	0x3
	.byte	0x8
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x6bd
	.4byte	0x355
	.uleb128 0xc
	.4byte	.LASF41
	.byte	0x5
	.2byte	0x6be
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF42
	.byte	0x5
	.2byte	0x6c7
	.4byte	0x2d9
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x6ce
	.4byte	0x36f
	.uleb128 0xa
	.4byte	.LASF43
	.byte	0x5
	.2byte	0x6cf
	.4byte	0x63
	.byte	0x4
	.byte	0x20
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x6cb
	.4byte	0x391
	.uleb128 0xc
	.4byte	.LASF43
	.byte	0x5
	.2byte	0x6cc
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF44
	.byte	0x5
	.2byte	0x6d0
	.4byte	0x355
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x6d6
	.4byte	0x3ab
	.uleb128 0xa
	.4byte	.LASF45
	.byte	0x5
	.2byte	0x6d7
	.4byte	0x63
	.byte	0x4
	.byte	0x20
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x6d3
	.4byte	0x3cd
	.uleb128 0xc
	.4byte	.LASF45
	.byte	0x5
	.2byte	0x6d4
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF46
	.byte	0x5
	.2byte	0x6d8
	.4byte	0x391
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x6de
	.4byte	0x3e7
	.uleb128 0xa
	.4byte	.LASF47
	.byte	0x5
	.2byte	0x6df
	.4byte	0x63
	.byte	0x4
	.byte	0x20
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x6db
	.4byte	0x409
	.uleb128 0xc
	.4byte	.LASF47
	.byte	0x5
	.2byte	0x6dc
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF48
	.byte	0x5
	.2byte	0x6e0
	.4byte	0x3cd
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x6e6
	.4byte	0x443
	.uleb128 0xa
	.4byte	.LASF49
	.byte	0x5
	.2byte	0x6e7
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1f
	.byte	0
	.uleb128 0xa
	.4byte	.LASF50
	.byte	0x5
	.2byte	0x6e8
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1e
	.byte	0
	.uleb128 0xa
	.4byte	.LASF51
	.byte	0x5
	.2byte	0x6eb
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1d
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x6e3
	.4byte	0x465
	.uleb128 0xc
	.4byte	.LASF52
	.byte	0x5
	.2byte	0x6e4
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF53
	.byte	0x5
	.2byte	0x6ec
	.4byte	0x409
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x6f3
	.4byte	0x4cf
	.uleb128 0xa
	.4byte	.LASF54
	.byte	0x5
	.2byte	0x6f4
	.4byte	0x63
	.byte	0x4
	.byte	0xa
	.byte	0x16
	.byte	0
	.uleb128 0xa
	.4byte	.LASF55
	.byte	0x5
	.2byte	0x6f6
	.4byte	0x63
	.byte	0x4
	.byte	0xa
	.byte	0xc
	.byte	0
	.uleb128 0xa
	.4byte	.LASF56
	.byte	0x5
	.2byte	0x6f8
	.4byte	0x63
	.byte	0x4
	.byte	0x2
	.byte	0xa
	.byte	0
	.uleb128 0xa
	.4byte	.LASF57
	.byte	0x5
	.2byte	0x6f9
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x9
	.byte	0
	.uleb128 0xa
	.4byte	.LASF58
	.byte	0x5
	.2byte	0x6fa
	.4byte	0x63
	.byte	0x4
	.byte	0x4
	.byte	0x5
	.byte	0
	.uleb128 0xa
	.4byte	.LASF59
	.byte	0x5
	.2byte	0x6fb
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x4
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x6f0
	.4byte	0x4f1
	.uleb128 0xc
	.4byte	.LASF60
	.byte	0x5
	.2byte	0x6f1
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF61
	.byte	0x5
	.2byte	0x6fc
	.4byte	0x465
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x703
	.4byte	0x52a
	.uleb128 0xd
	.ascii	"ACF\000"
	.byte	0x5
	.2byte	0x704
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1f
	.byte	0
	.uleb128 0xd
	.ascii	"ACC\000"
	.byte	0x5
	.2byte	0x705
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1e
	.byte	0
	.uleb128 0xd
	.ascii	"OF\000"
	.byte	0x5
	.2byte	0x706
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1d
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x700
	.4byte	0x54c
	.uleb128 0xc
	.4byte	.LASF62
	.byte	0x5
	.2byte	0x701
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF63
	.byte	0x5
	.2byte	0x707
	.4byte	0x4f1
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x70d
	.4byte	0x585
	.uleb128 0xd
	.ascii	"ACF\000"
	.byte	0x5
	.2byte	0x70e
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1f
	.byte	0
	.uleb128 0xd
	.ascii	"ACC\000"
	.byte	0x5
	.2byte	0x70f
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1e
	.byte	0
	.uleb128 0xd
	.ascii	"OF\000"
	.byte	0x5
	.2byte	0x710
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1d
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x70a
	.4byte	0x5a7
	.uleb128 0xc
	.4byte	.LASF64
	.byte	0x5
	.2byte	0x70b
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF65
	.byte	0x5
	.2byte	0x711
	.4byte	0x54c
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x717
	.4byte	0x5e0
	.uleb128 0xd
	.ascii	"ACF\000"
	.byte	0x5
	.2byte	0x718
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1f
	.byte	0
	.uleb128 0xd
	.ascii	"ACC\000"
	.byte	0x5
	.2byte	0x719
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1e
	.byte	0
	.uleb128 0xd
	.ascii	"OF\000"
	.byte	0x5
	.2byte	0x71a
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1d
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x714
	.4byte	0x602
	.uleb128 0xc
	.4byte	.LASF66
	.byte	0x5
	.2byte	0x715
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF67
	.byte	0x5
	.2byte	0x71b
	.4byte	0x5a7
	.byte	0
	.uleb128 0x9
	.byte	0x4
	.byte	0x5
	.2byte	0x721
	.4byte	0x63b
	.uleb128 0xd
	.ascii	"ACF\000"
	.byte	0x5
	.2byte	0x722
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1f
	.byte	0
	.uleb128 0xd
	.ascii	"ACC\000"
	.byte	0x5
	.2byte	0x723
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1e
	.byte	0
	.uleb128 0xd
	.ascii	"OF\000"
	.byte	0x5
	.2byte	0x724
	.4byte	0x63
	.byte	0x4
	.byte	0x1
	.byte	0x1d
	.byte	0
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.2byte	0x71e
	.4byte	0x65d
	.uleb128 0xc
	.4byte	.LASF68
	.byte	0x5
	.2byte	0x71f
	.4byte	0x63
	.uleb128 0xc
	.4byte	.LASF69
	.byte	0x5
	.2byte	0x725
	.4byte	0x602
	.byte	0
	.uleb128 0xe
	.2byte	0x110
	.byte	0x5
	.2byte	0x65b
	.4byte	0x6ff
	.uleb128 0xf
	.4byte	0xb3
	.byte	0
	.uleb128 0xf
	.4byte	0xef
	.byte	0x4
	.uleb128 0xf
	.4byte	0x12b
	.byte	0x8
	.uleb128 0xf
	.4byte	0x1a7
	.byte	0xc
	.uleb128 0xf
	.4byte	0x1f3
	.byte	0x10
	.uleb128 0xf
	.4byte	0x22f
	.byte	0x14
	.uleb128 0xf
	.4byte	0x26b
	.byte	0x18
	.uleb128 0xf
	.4byte	0x2b7
	.byte	0x1c
	.uleb128 0xf
	.4byte	0x333
	.byte	0x20
	.uleb128 0x10
	.4byte	.LASF70
	.byte	0x5
	.2byte	0x6c9
	.4byte	0x68
	.byte	0x24
	.uleb128 0xf
	.4byte	0x36f
	.byte	0x28
	.uleb128 0xf
	.4byte	0x3ab
	.byte	0x2c
	.uleb128 0xf
	.4byte	0x3e7
	.byte	0x30
	.uleb128 0xf
	.4byte	0x443
	.byte	0x34
	.uleb128 0x10
	.4byte	.LASF71
	.byte	0x5
	.2byte	0x6ee
	.4byte	0x68
	.byte	0x38
	.uleb128 0xf
	.4byte	0x4cf
	.byte	0x3c
	.uleb128 0x10
	.4byte	.LASF72
	.byte	0x5
	.2byte	0x6fe
	.4byte	0x714
	.byte	0x40
	.uleb128 0x11
	.4byte	0x52a
	.2byte	0x100
	.uleb128 0x11
	.4byte	0x585
	.2byte	0x104
	.uleb128 0x11
	.4byte	0x5e0
	.2byte	0x108
	.uleb128 0x11
	.4byte	0x63b
	.2byte	0x10c
	.byte	0
	.uleb128 0x12
	.4byte	0x68
	.4byte	0x70f
	.uleb128 0x13
	.4byte	0x6d
	.byte	0x2f
	.byte	0
	.uleb128 0x6
	.4byte	0x6ff
	.uleb128 0x4
	.4byte	0x70f
	.uleb128 0x14
	.4byte	.LASF73
	.byte	0x5
	.2byte	0x727
	.4byte	0x65d
	.uleb128 0x15
	.4byte	0x82
	.byte	0x1
	.byte	0x26
	.uleb128 0x5
	.byte	0x3
	.4byte	SystemCoreClock
	.uleb128 0x16
	.4byte	.LASF74
	.byte	0x1
	.byte	0x46
	.4byte	.LFB124
	.4byte	.LFE124-.LFB124
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x16
	.4byte	.LASF75
	.byte	0x1
	.byte	0x32
	.4byte	.LFB123
	.4byte	.LFE123-.LFB123
	.uleb128 0x1
	.byte	0x9c
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.uleb128 0x2134
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xd
	.uleb128 0xb
	.uleb128 0xc
	.uleb128 0xb
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x17
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xd
	.uleb128 0xb
	.uleb128 0xc
	.uleb128 0xb
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0xd
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0xd
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x34
	.byte	0
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_pubnames,"",%progbits
	.4byte	0x4b
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x755
	.4byte	0x725
	.ascii	"SystemCoreClock\000"
	.4byte	0x732
	.ascii	"SystemInit\000"
	.4byte	0x743
	.ascii	"SystemCoreClockUpdate\000"
	.4byte	0
	.section	.debug_pubtypes,"",%progbits
	.4byte	0xc4
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x755
	.4byte	0x25
	.ascii	"signed char\000"
	.4byte	0x2c
	.ascii	"unsigned char\000"
	.4byte	0x33
	.ascii	"short int\000"
	.4byte	0x3a
	.ascii	"short unsigned int\000"
	.4byte	0x51
	.ascii	"int\000"
	.4byte	0x41
	.ascii	"int32_t\000"
	.4byte	0x6d
	.ascii	"unsigned int\000"
	.4byte	0x58
	.ascii	"uint32_t\000"
	.4byte	0x74
	.ascii	"long long int\000"
	.4byte	0x7b
	.ascii	"long long unsigned int\000"
	.4byte	0x719
	.ascii	"CLKGEN_Type\000"
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x24
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.LFB123
	.4byte	.LFE123-.LFB123
	.4byte	.LFB124
	.4byte	.LFE124-.LFB124
	.4byte	0
	.4byte	0
	.section	.debug_ranges,"",%progbits
.Ldebug_ranges0:
	.4byte	.LFB123
	.4byte	.LFE123
	.4byte	.LFB124
	.4byte	.LFE124
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF68:
	.ascii	"INTRPTSET\000"
.LASF78:
	.ascii	"C:\\Users\\mschre\\Documents\\Projects\\Customer\\1"
	.ascii	"12Motion\\I2S\\apollo3-iom_i2c_direct_cmd-v10\\example\\"
	.ascii	"segger\000"
.LASF45:
	.ascii	"CLOCKEN2STAT\000"
.LASF24:
	.ascii	"CKEN\000"
.LASF2:
	.ascii	"short int\000"
.LASF43:
	.ascii	"CLOCKENSTAT\000"
.LASF57:
	.ascii	"TONADJUSTEN\000"
.LASF38:
	.ascii	"HFXTADJ\000"
.LASF53:
	.ascii	"FREQCTRL_b\000"
.LASF46:
	.ascii	"CLOCKEN2STAT_b\000"
.LASF36:
	.ascii	"HFADJEN\000"
.LASF40:
	.ascii	"HFADJGAIN\000"
.LASF55:
	.ascii	"TONHIGHTHRESHOLD\000"
.LASF42:
	.ascii	"HFADJ_b\000"
.LASF71:
	.ascii	"RESERVED1\000"
.LASF77:
	.ascii	"C:\\Users\\mschre\\Documents\\Projects\\Customer\\1"
	.ascii	"12Motion\\I2S\\apollo3-iom_i2c_direct_cmd-v10\\common\\"
	.ascii	"system_apollo3.c\000"
.LASF17:
	.ascii	"STOPXT\000"
.LASF60:
	.ascii	"BLEBUCKTONADJ\000"
.LASF19:
	.ascii	"OSEL\000"
.LASF22:
	.ascii	"OCTRL_b\000"
.LASF65:
	.ascii	"INTRPTSTAT_b\000"
.LASF23:
	.ascii	"CKSEL\000"
.LASF15:
	.ascii	"ACALCTR\000"
.LASF14:
	.ascii	"CALRC_b\000"
.LASF41:
	.ascii	"HFADJ\000"
.LASF49:
	.ascii	"BURSTREQ\000"
.LASF7:
	.ascii	"long long int\000"
.LASF25:
	.ascii	"CLKOUT\000"
.LASF56:
	.ascii	"TONADJUSTPERIOD\000"
.LASF30:
	.ascii	"CCTRL\000"
.LASF54:
	.ascii	"TONLOWTHRESHOLD\000"
.LASF67:
	.ascii	"INTRPTCLR_b\000"
.LASF44:
	.ascii	"CLOCKENSTAT_b\000"
.LASF20:
	.ascii	"ACAL\000"
.LASF11:
	.ascii	"CALXT\000"
.LASF61:
	.ascii	"BLEBUCKTONADJ_b\000"
.LASF47:
	.ascii	"CLOCKEN3STAT\000"
.LASF50:
	.ascii	"BURSTACK\000"
.LASF39:
	.ascii	"HFWARMUP\000"
.LASF1:
	.ascii	"unsigned char\000"
.LASF52:
	.ascii	"FREQCTRL\000"
.LASF51:
	.ascii	"BURSTSTATUS\000"
.LASF62:
	.ascii	"INTRPTEN\000"
.LASF0:
	.ascii	"signed char\000"
.LASF4:
	.ascii	"int32_t\000"
.LASF8:
	.ascii	"long long unsigned int\000"
.LASF5:
	.ascii	"uint32_t\000"
.LASF27:
	.ascii	"CLKKEY\000"
.LASF6:
	.ascii	"unsigned int\000"
.LASF59:
	.ascii	"ZEROLENDETECTEN\000"
.LASF28:
	.ascii	"CLKKEY_b\000"
.LASF31:
	.ascii	"CCTRL_b\000"
.LASF18:
	.ascii	"STOPRC\000"
.LASF21:
	.ascii	"OCTRL\000"
.LASF73:
	.ascii	"CLKGEN_Type\000"
.LASF74:
	.ascii	"SystemInit\000"
.LASF64:
	.ascii	"INTRPTSTAT\000"
.LASF3:
	.ascii	"short unsigned int\000"
.LASF16:
	.ascii	"ACALCTR_b\000"
.LASF37:
	.ascii	"HFADJCK\000"
.LASF75:
	.ascii	"SystemCoreClockUpdate\000"
.LASF26:
	.ascii	"CLKOUT_b\000"
.LASF9:
	.ascii	"SystemCoreClock\000"
.LASF33:
	.ascii	"OSCF\000"
.LASF48:
	.ascii	"CLOCKEN3STAT_b\000"
.LASF63:
	.ascii	"INTRPTEN_b\000"
.LASF72:
	.ascii	"RESERVED2\000"
.LASF66:
	.ascii	"INTRPTCLR\000"
.LASF13:
	.ascii	"CALRC\000"
.LASF29:
	.ascii	"CORESEL\000"
.LASF32:
	.ascii	"OMODE\000"
.LASF12:
	.ascii	"CALXT_b\000"
.LASF70:
	.ascii	"RESERVED\000"
.LASF10:
	.ascii	"ITM_RxBuffer\000"
.LASF76:
	.ascii	"GNU C99 7.3.1 20180622 (release) [ARM/embedded-7-br"
	.ascii	"anch revision 261907] -fmessage-length=0 -mcpu=cort"
	.ascii	"ex-m4 -mlittle-endian -mfloat-abi=hard -mfpu=fpv4-s"
	.ascii	"p-d16 -mthumb -mtp=soft -munaligned-access -std=gnu"
	.ascii	"99 -g2 -gpubnames -fomit-frame-pointer -fno-dwarf2-"
	.ascii	"cfi-asm -fno-builtin -ffunction-sections -fdata-sec"
	.ascii	"tions -fshort-enums -fno-common\000"
.LASF34:
	.ascii	"STATUS\000"
.LASF35:
	.ascii	"STATUS_b\000"
.LASF69:
	.ascii	"INTRPTSET_b\000"
.LASF58:
	.ascii	"ZEROLENDETECTTRIM\000"
	.ident	"GCC: (GNU) 7.3.1 20180622 (release) [ARM/embedded-7-branch revision 261907]"
