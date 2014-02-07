/*
 *  Startup Code for the X-Boot.
 *
 *  Copyright (c) 2005 - 2009	Ingenic Semiconductor Corp.
 *
 */
#include <config.h>
#include <regdef.h>
#include <context.h>
#include <mipsregs.h>

	.set noreorder

	.globl _start
	.text
_start:	
	la	t0, _fbss
	la	t1, _end
1:	sw	zero, 0(t0)
	bne	t0, t1, 1b
	addiu	t0, 4

	/* need to be store after bss init */
	la	t0, sector_num
	sw	a0, 0(t0)
	la	sp, TEXT_BASE		// set sp
	j	boot_main		// jump to c routine
	nop


#if 1
	.global real_exception_entry
	.type	real_exception_entry,@function
	.set noreorder
real_exception_entry:
	mfc0	k0, CP0_CAUSE
	andi	k0, k0, 0x7c
	beq	zero, k0, intr_handler
	nop
excpt_handler:
	STORE_REG_RET
        jal c_except_handler
	nop
        RESTORE_REG_ERET
	nop
intr_handler:
        STORE_REG_RET
        jal C_vINTHandler
	nop
        RESTORE_REG_ERET
	nop

	.global except_common_entry
	.type	except_common_entry,@function
except_common_entry:
	la	k0, real_exception_entry
	jr	k0
	nop
	nop
	nop


#endif
