.globl _start

.section ".vectors", "ax"

/*
 *************************************************************************
 *
 * Exception vectors as described in ARM reference manuals
 *
 * Uses indirect branch to allow reaching handlers anywhere in memory.
 *
 *************************************************************************
 */
_start:
	b	reset
	ldr	pc, _undefined_instruction
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq

/*
 *************************************************************************
 *
 * Indirect vectors table
 *
 * Symbols referenced here must be defined somewhere else
 *
 *************************************************************************
 */
.globl	_undefined_instruction
.globl	_software_interrupt
.globl	_prefetch_abort
.globl	_data_abort
.globl	_not_used
.globl	_irq
.globl	_fiq

_undefined_instruction:	.word undefined_instruction
_software_interrupt:	.word software_interrupt
_prefetch_abort:	.word prefetch_abort
_data_abort:		.word data_abort
_not_used:		.word not_used
_irq:			.word irq
_fiq:			.word fiq

.balignl 16,0xdeadbeef

undefined_instruction:
    bl undefined_instruction

software_interrupt:
    bl software_interrupt
	
prefetch_abort:
    bl prefetch_abort 

data_abort:
    bl data_abort
    
not_used:
    bl not_used

irq:
    bl	irq

fiq:
    bl fiq