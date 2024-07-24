/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-13     Bernard      first version
 * 2006-10-05     Alsor.Z      for s3c2440 initialize
 * 2008-01-29     Yi.Qiu       for QEMU emulator
 */

#define CONFIG_STACKSIZE    512
#define S_FRAME_SIZE        72

#define S_OLD_R0            68
#define S_PSR               64
#define S_PC                60
#define S_LR                56
#define S_SP                52

#define S_IP                48
#define S_FP                44
#define S_R10               40
#define S_R9                36
#define S_R8                32
#define S_R7                28
#define S_R6                24
#define S_R5                20
#define S_R4                16
#define S_R3                12
#define S_R2                8
#define S_R1                4
#define S_R0                0

.equ    USERMODE,           0x10
.equ    FIQMODE,            0x11
.equ    IRQMODE,            0x12
.equ    SVCMODE,            0x13
.equ    ABORTMODE,          0x17
.equ    UNDEFMODE,          0x1b
.equ    MODEMASK,           0x1f
.equ    NOINT,              0xc0

.equ    RAM_BASE,           0x30000000  /*Start address of RAM      */

.equ    INTMSK,             0x4a000008
.equ    INTSUBMSK,          0x4a00001c
.equ    WTCON,              0x53000000

.equ    GPHCON,             0x56000070  /*Port H control            */
.equ    GPHUP,              0x56000078  /*Pull-up control H         */

.equ    BWSCON,             0x48000000  /*Bus width & wait status   */
.equ    BANKCON0,           0x48000004  /*Boot ROM control          */
.equ    BANKCON1,           0x48000008  /*BANK1 control             */
.equ    BANKCON2,           0x4800000c  /*BANK2 cControl            */
.equ    BANKCON3,           0x48000010  /*BANK3 control             */
.equ    BANKCON4,           0x48000014  /*BANK4 control             */
.equ    BANKCON5,           0x48000018  /*BANK5 control             */
.equ    BANKCON6,           0x4800001c  /*BANK6 control             */
.equ    BANKCON7,           0x48000020  /*BANK7 control             */
.equ    REFRESH,            0x48000024  /*DRAM/SDRAM efresh         */
.equ    BANKSIZE,           0x48000028  /*Flexible Bank Size        */
.equ    MRSRB6,             0x4800002c  /*Mode egister set for SDRAM*/
.equ    MRSRB7,             0x48000030  /*Mode egister set for SDRAM*/

.equ    NFCONF,             0x4e000000
.equ    NFCONT,             0x4e000004
.equ    NFCMMD,             0x4e000008
.equ    NFADDR,             0x4e00000C
.equ    NFDATA,             0x4e000010
.equ    NFSTAT,             0x4e000020

/*
 *************************************************************************
 *
 * Jump vector table
 *
 *************************************************************************
 */
.globl	reset
.section .init, "ax"
.code 32

/*
 *************************************************************************
 *
 * Startup Code (reset vector)
 * relocate armboot to ram
 * setup stack
 * jump to second stage
 *
 *************************************************************************
 */

_TEXT_BASE:
    .word   TEXT_BASE

/*
 * rtthread kernel start and end
 * which are defined in linker script
 */
.globl _rtthread_start
_rtthread_start:
    .word _start

.globl _rtthread_end
_rtthread_end:
    .word  _end

/*
 * rtthread bss start and end which are defined in linker script
 */
.globl _bss_start
_bss_start:
    .word __bss_start

.globl _bss_end
_bss_end:
    .word __bss_end

/* IRQ stack memory (calculated at run-time)                        */
.globl IRQ_STACK_START
IRQ_STACK_START:
    .word _irq_stack_start + 2 * CONFIG_STACKSIZE

.globl FIQ_STACK_START
FIQ_STACK_START:
    .word _fiq_stack_start + 2 * CONFIG_STACKSIZE

.globl UNDEFINED_STACK_START
UNDEFINED_STACK_START:
    .word _undefined_stack_start + CONFIG_STACKSIZE

.globl ABORT_STACK_START
ABORT_STACK_START:
    .word _abort_stack_start + CONFIG_STACKSIZE

.globl _STACK_START
_STACK_START:
    .word _svc_stack_start + 8 * CONFIG_STACKSIZE

/* ----------------------------------entry------------------------------*/
reset:

    /* set the cpu to SVC32 mode    */
    mrs     r0, cpsr
    bic     r0, r0, #MODEMASK
    orr     r0, r0, #SVCMODE
    msr     cpsr, r0

    /* watch dog disable            */
    ldr     r0, =WTCON
    ldr     r1, =0x0
    str     r1, [r0]

    /* mask all IRQs by clearing all bits in the INTMRs                 */
    ldr     r1, =INTMSK
    ldr     r0, =0xffffffff
    str     r0, [r1]
    ldr     r1, =INTSUBMSK
    ldr     r0, =0x7fff             /*all sub interrupt disable         */
    str     r0, [r1]

    /* 设置栈顶指针寄存器 SP，Nand 启动时此时位于前 4k Steppingstone */
    ldr     sp, =0x1000




    /* clear .bss */
    mov     r0,#0                   /* get a zero                       */
    ldr     r1,=__bss_start         /* bss start                        */
    ldr     r2,=__bss_end           /* bss end                          */

bss_loop:
    cmp     r1,r2                   /* check if data to clear           */
    strlo   r0,[r1],#4              /* clear 4 bytes                    */
    blo     bss_loop                /* loop until done                  */

    /* call C++ constructors of global objects                          */
    ldr     r0, =__ctors_start__
    ldr     r1, =__ctors_end__

ctor_loop:
    cmp     r0, r1
    beq     ctor_end
    ldr     r2, [r0], #4
    stmfd   sp!, {r0-r1}
    mov     lr, pc
    bx      r2
    ldmfd   sp!, {r0-r1}
    b       ctor_loop

ctor_end:

    @ ldr     r0, =__boot_text_end    // source address
	@ ldr     r1, _TEXT_BASE          // target address
	@ ldr     r2, =__data_end
    @ @ ldr     r3, =__text_start
	@ sub     r2, r2, r3              // text, rodata and data section size

    /* 
     * 配置代码重定向后所使用栈空间
     * 注意：
     *   1. 提前配置存储控制，确保 SDRAM 可正常读写；
     *   2. 不同 CPU 工作模式下的栈空间需单独配置；
     */
    bl      stack_setup

    /* start RT-Thread Kernel       */
    ldr     pc, _rtthread_startup

_rtthread_startup:
    .word rtthread_startup

/*
 *************************************************************************
 *
 * Interrupt handling
 *
 *************************************************************************
 */

/* exception handlers               */
    .align  5
vector_undef:
    sub     sp, sp, #S_FRAME_SIZE
    stmia   sp, {r0 - r12}          /* Calling r0-r12                   */
    add     r8, sp, #S_PC
    stmdb   r8, {sp, lr}^           /* Calling SP, LR                   */
    str     lr, [r8, #0]            /* Save calling PC                  */
    mrs     r6, spsr
    str     r6, [r8, #4]            /* Save CPSR                        */
    str     r0, [r8, #8]            /* Save OLD_R0                      */
    mov     r0, sp

    bl      rt_hw_trap_udef

    .align  5
vector_swi:
    bl      rt_hw_trap_swi

    .align  5
vector_pabt:
    bl      rt_hw_trap_pabt

    .align  5
vector_dabt:
    sub     sp, sp, #S_FRAME_SIZE
    stmia   sp, {r0 - r12}          /* Calling r0-r12                   */
    add     r8, sp, #S_PC
    stmdb   r8, {sp, lr}^           /* Calling SP, LR                   */
    str     lr, [r8, #0]            /* Save calling PC                  */
    mrs     r6, spsr
    str     r6, [r8, #4]            /* Save CPSR                        */
    str     r0, [r8, #8]            /* Save OLD_R0                      */
    mov     r0, sp

    bl      rt_hw_trap_dabt

    .align  5
vector_resv:
    bl      rt_hw_trap_resv

.globl      rt_interrupt_enter
.globl      rt_interrupt_leave
.globl      rt_thread_switch_interrupt_flag
.globl      rt_interrupt_from_thread
.globl      rt_interrupt_to_thread
vector_irq:
    stmfd   sp!, {r0-r12,lr}
    bl      rt_interrupt_enter
    bl      rt_hw_trap_irq
    bl      rt_interrupt_leave

    /* if rt_thread_switch_interrupt_flag set, jump to _interrupt_thread_switch and don't return */
    ldr     r0, =rt_thread_switch_interrupt_flag
    ldr     r1, [r0]
    cmp     r1, #1
    beq     _interrupt_thread_switch

    ldmfd   sp!, {r0-r12,lr}
    subs    pc, lr, #4

    .align  5
vector_fiq:
    stmfd   sp!,{r0-r7,lr}
    bl      rt_hw_trap_fiq
    ldmfd   sp!,{r0-r7,lr}
    subs    pc,lr,#4

_interrupt_thread_switch:
    mov     r1,  #0                 /* clear rt_thread_switch_interrupt_flag*/
    str     r1,  [r0]

    ldmfd   sp!, {r0-r12,lr}        /* reload saved registers           */
    stmfd   sp!, {r0-r3}            /* save r0-r3                       */
    mov     r1,  sp
    add     sp,  sp, #16            /* restore sp                       */
    sub     r2,  lr, #4             /* save old task's pc to r2         */

    mrs     r3,  spsr               /* disable interrupt                */
    orr     r0,  r3, #NOINT
    msr     spsr_c, r0

    ldr     r0,  =.+8               /* switch to interrupted task's stack*/
    movs    pc,  r0

    stmfd   sp!, {r2}               /* push old task's pc               */
    stmfd   sp!, {r4-r12,lr}        /* push old task's lr,r12-r4        */
    mov     r4,  r1                 /* Special optimised code below     */
    mov     r5,  r3
    ldmfd   r4!, {r0-r3}
    stmfd   sp!, {r0-r3}            /* push old task's r3-r0            */
    stmfd   sp!, {r5}               /* push old task's psr              */
    mrs     r4,  spsr
    stmfd   sp!, {r4}               /* push old task's spsr             */

    ldr     r4,  =rt_interrupt_from_thread
    ldr     r5,  [r4]
    str     sp,  [r5]               /* store sp in preempted tasks's TCB*/

    ldr r6,  =rt_interrupt_to_thread
    ldr r6,  [r6]
    ldr sp,  [r6]                   /* get new task's stack pointer     */

    ldmfd   sp!, {r4}               /* pop new task's spsr              */
    msr     SPSR_cxsf, r4
    ldmfd   sp!, {r4}               /* pop new task's psr               */
    msr     CPSR_cxsf, r4

    ldmfd   sp!, {r0-r12,lr,pc}     /* pop new task's r0-r12,lr & pc    */

stack_setup:
    mrs     r0, cpsr
    bic     r0, r0, #MODEMASK
    orr     r1, r0, #UNDEFMODE|NOINT
    msr     cpsr_cxsf, r1           /* undef mode                       */
    ldr     sp, UNDEFINED_STACK_START

    orr     r1, r0, #ABORTMODE|NOINT
    msr     cpsr_cxsf, r1            /* abort mode                       */
    ldr     sp, ABORT_STACK_START

    orr     r1, r0, #IRQMODE|NOINT
    msr     cpsr_cxsf, r1            /* IRQ mode                         */
    ldr     sp, IRQ_STACK_START

    orr     r1, r0, #FIQMODE|NOINT
    msr     cpsr_cxsf, r1            /* FIQ mode                         */
    ldr     sp, FIQ_STACK_START

    bic     r0, r0, #MODEMASK
    orr     r1, r0, #SVCMODE|NOINT
    msr     cpsr_cxsf, r1            /* SVC mode                         */

    ldr     sp, _STACK_START

    /* USER mode is not initialized. */

/*/*}*/

