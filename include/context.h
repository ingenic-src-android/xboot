/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : CONTEXT.h                                               *
*                                                                         *
*   AUTHOR      : Michael Anburaj                                         *
*                 URL  : http://geocities.com/michaelanburaj/             *
*                 EMAIL: michaelanburaj@hotmail.com                       *
*                                                                         *
*   PROCESSOR   : MIPS 4Kc (32 bit RISC) - ATLAS board                    *
*                                                                         *
*   TOOL-CHAIN  : SDE & Cygnus                                            *
*                                                                         *
*   DESCRIPTION :                                                         *
*   Context switch macros.                                                *
*                                                                         *
**************************************************************************/


#ifndef __CONTEXT_H__
#define __CONTEXT_H__


// push context: at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define STORE_REG_RET \
		.set noat; \
    .set noreorder; \
	lui k0,0x8800;\
        subu k0,192; \
	sw sp,128(k0); \
	move sp, k0; \
        sw ra,0(sp); \
        sw fp,4(sp); \
        sw gp,8(sp); \
        sw t9,12(sp); \
        sw t8,16(sp); \
        sw s7,20(sp); \
        sw s6,24(sp); \
        sw s5,28(sp); \
        sw s4,32(sp); \
        sw s3,36(sp); \
        sw s2,40(sp); \
        sw s1,44(sp); \
        sw s0,48(sp); \
        sw t7,52(sp); \
        sw t6,56(sp); \
        sw t5,60(sp); \
        sw t4,64(sp); \
        sw t3,68(sp); \
        sw t2,72(sp); \
        sw t1,76(sp); \
        sw t0,80(sp); \
        sw a3,84(sp); \
        sw a2,88(sp); \
        sw a1,92(sp); \
        sw a0,96(sp); \
        sw v1,100(sp); \
        sw v0,104(sp); \
        sw AT,108(sp); \
        mfc0 k1,CP0_STATUS; \
        sw k1,112(sp); \
        mfc0 k1, CP0_EPC; \
        sw k1,116(sp); \
        mflo k1;       \
        sw k1,120(sp); \
        mfhi k1;       \
        sw k1,124(sp); \
	.set at; \
	la k0, ori_context; \
	sw sp, 0(k0)

    
// pop context: at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, & pc
#define RESTORE_REG_ERET \
		.set noat; \
		.set noreorder; \
        lw ra,0(sp); \
        lw fp,4(sp); \
        lw gp,8(sp); \
        lw t9,12(sp); \
        lw t8,16(sp); \
        lw s7,20(sp); \
        lw s6,24(sp); \
        lw s5,28(sp); \
        lw s4,32(sp); \
        lw s3,36(sp); \
        lw s2,40(sp); \
        lw s1,44(sp); \
        lw s0,48(sp); \
        lw t7,52(sp); \
        lw t6,56(sp); \
        lw t5,60(sp); \
        lw t4,64(sp); \
        lw t3,68(sp); \
        lw t2,72(sp); \
        lw t1,76(sp); \
        lw t0,80(sp); \
        lw a3,84(sp); \
        lw a2,88(sp); \
        lw a1,92(sp); \
        lw a0,96(sp); \
        lw v1,100(sp); \
        lw v0,104(sp); \
        lw AT,108(sp); \
        lw k0,112(sp); \
        mfc0 k1,CP0_STATUS; \
        srl k1,  k1, 1; \
        sll k1,  k1, 1; \
        andi k0, k0, 1;  \
        or k0, k1, k0;  \
        mtc0 k0,CP0_STATUS; \
        lw k0,116(sp); \
        mtc0 k0,CP0_EPC; \
        lw k0,120(sp); \
        mtlo k0;       \
        lw k0,124(sp); \
        mthI  k0;      \
	lw sp, 128(sp); \
        eret; \
        nop; \
.set at


#endif /*__CONTEXT_H__*/
