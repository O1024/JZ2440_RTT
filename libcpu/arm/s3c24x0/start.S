/* CPU mode, CPSR */
.equ    USERMODE,           0x10
.equ    FIQMODE,            0x11
.equ    IRQMODE,            0x12
.equ    SVCMODE,            0x13
.equ    ABORTMODE,          0x17
.equ    UNDEFMODE,          0x1b

.equ    MODEMASK,           0x1f
.equ    NOINT,              0xc0

.equ    MEM_CTL_BASE,       0x48000000
.equ    SDRAM_BASE,         0x30000000

.equ    NFCONF,             0x4E000000
.equ    NFCONT,             0x4E000004
.equ    NFCMD,              0x4E000008
.equ    NFSTAT,             0x4E000020

/* interrupt */
.equ    INTMSK,             0x4a000008
.equ    INTSUBMSK,          0x4a00001c

/* watch dog */
.equ    WTCON,              0x53000000


.globl	reset
.section ".init", "ax"

reset:
    /* now we in 4k Steppingstone */
	ldr     sp, =4096

	/* set the cpu to SVC32 mode 	*/
	mrs	r0, cpsr
	bic	r0, r0, #MODEMASK
	orr	r0, r0, #SVCMODE
	orr	r0, r0, #0xc0 				@ disable fiq and irq
	msr	cpsr, r0
    
	/* disable watch dog 			*/
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

    mov     r1, #MEM_CTL_BASE       @ 存储控制器的13个寄存器的开始地址
    adrl    r2, mem_cfg_val         @ 这13个值的起始存储地址
    add     r3, r1, #52             @ 13*4 = 54
1:  
    ldr r4,     [r2], #4            @ 读取设置值，并让r2加4
    str r4,     [r1], #4            @ 将此值写入寄存器，并让r1加4
    cmp r1,     r3                  @ 判断是否设置完所有13个寄存器
    bne 1b                          @ 若没有写成，继续

    /* 设置 NAND Flash 时序 */
    ldr     r0, =NFCONF
    ldr     r1, =0x300
    str     r1, [r0]
    
    /* 使能 NAND Flash 控制器, 初始化 ECC, 禁止片选 */
    ldr     r0, =NFCONT
    ldr     r1, =0x13
    str     r1, [r0]

    /* 使能 NAND Flash 片选 */
    ldr r0, =NFCONF
    ldr r1, [r0]                @ 读取 NFCONF 寄存器内容
    
    mov r2, #0xfffffffd     
    and r1, r1, r2              @ 清除 Reg_nCE bit[1] 位
    str r1, [r0]                @ 结果写回 NFCONF 寄存器

    /* 延时一段时间，为了什么？ */
    mov r0, #10
delay_loop:
    subs r0, r0, #1
    bne delay_loop

    /* 发送复位命令 */
    ldr     r0, =NFCMD
    ldr     r1, =0xff
    str     r1, [r0]

    /* 等待 NAND Flash ready */
wait_nand_idle:
    ldr r0, =NFSTAT
    ldr r1, [r0]
    and r2, r1, #1
    cmp r2, #1
    beq nand_idle

    mov r0, #10

delay_loop2:
    subs r0, r0, #1       /* 将计数器 r0 减 1 */
    bne delay_loop2        /* 如果 r0 不等于 0，则跳回 delay_loop 标签继续循环 */
    b wait_nand_idle

nand_idle:

    @ @ s3c2440nand->NFCONT |= (1<<1);
    ldr r0, =NFCONT   /* nfcont 寄存器的基地址（请根据实际地址调整） */

    /* 读取 nfcont 寄存器的值 */
    ldr r1, [r0]          /* 将寄存器值加载到 r1 中 */

    /* 执行按位或操作 */
    orr r1, r1, #0x2        /* 将掩码与寄存器值进行按位或操作 */

    /* 将更新后的值写回 nfcont 寄存器 */
    str r1, [r0]          /* 将更新后的值写回寄存器 */

    ldr     r0,     =0x30000000     @1. 目标地址=0x30000000，这是SDRAM的起始地址
    mov     r1,     #0x2000           @2.  源地址   = 4096，连接的时候，main.c中的代码都存在NAND Flash地址4096开始处
    mov     r2,     #0x20000           @3.  复制长度= 2048(bytes)，对于本实验的main.c，这是足够了
    bl      nand_read               @调用C函数nand_read

    ldr     sp, =0x34000000         @设置栈
    ldr     lr, =halt_loop          @设置返回地址
    ldr     pc, =mymain               @b指令和bl指令只能前后跳转32M的范围，所以这里使用向pc赋值的方法进行跳转

halt_loop:
    b       halt_loop

.align 4
mem_cfg_val:
    @ 存储控制器13个寄存器的设置值
    .long   0x22011110      @ BWSCON
    .long   0x00000700      @ BANKCON0
    .long   0x00000700      @ BANKCON1
    .long   0x00000700      @ BANKCON2
    .long   0x00000700      @ BANKCON3  
    .long   0x00000700      @ BANKCON4
    .long   0x00000700      @ BANKCON5
    .long   0x00018005      @ BANKCON6
    .long   0x00018005      @ BANKCON7
    .long   0x008C07A3      @ REFRESH
    .long   0x000000B1      @ BANKSIZE
    .long   0x00000030      @ MRSRB6
    .long   0x00000030      @ MRSRB7