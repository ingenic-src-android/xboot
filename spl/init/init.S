/*
 *  Startup Code for the SPL.
 *
 *  Copyright (c) 2005 - 2009	Ingenic Semiconductor Corp.
 *
 */
#include <config.h>
#include <regdef.h>
#include <mipsregs.h>

#define JZ_MSCBOOT_CFG       0x4d53504c
#define JZ_NORBOOT_CFG       0x00000000

#if defined(CONFIG_JZ4760) || defined(CONFIG_JZ4760B) || \
    defined(CONFIG_JZ4810) || defined(CONFIG_JZ4770) || \
    defined(CONFIG_JZ4780) || defined(CONFIG_JZ4775)
#define JZ4760_NANDBOOT_CFG0 (0x55555500 | (CFG_NAND_BW8*0xff))
#define JZ4760_NANDBOOT_CFG1 0x55555555
#define JZ4760_NANDBOOT_CFG2 ((CFG_NAND_PAGE_SIZE==2048)&0xff0000) | ((CFG_NAND_PAGE_SIZE!=512)&0xff00) | ((CFG_NAND_ROW_CYCLE==3)&0xff)
#endif

	.set noreorder

	.globl _start
	.text
_start:
#if defined(CONFIG_NAND_SPL) || defined(CONFIG_NAND_X_BOOT)
#if defined(CONFIG_JZ4750) || defined(CONFIG_JZ4750D) || defined(CONFIG_JZ4750L)
        /* First three words fetched by CPU during NAND Boot */
        .word JZ4750_NANDBOOT_CFG0
        .word JZ4750_NANDBOOT_CFG1
        .word JZ4750_NANDBOOT_CFG2
#endif

#if defined(CONFIG_JZ4760)  || defined(CONFIG_JZ4810) || \
    defined(CONFIG_JZ4760B) || defined(CONFIG_JZ4770) || \
    defined(CONFIG_JZ4780)  || defined(CONFIG_JZ4775)
	/* First 40 words fetched by CPU during NAND Boot */

#if !defined(CONFIG_JZ4780)

	/* buswidth_flag */
#if (CFG_NAND_BW8 == 1)
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555

#else // CFG_NAND_BW8 

	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
#endif // CFG_NAND_BW8
#endif /* !defined(CONFIG_JZ4780) */

#if defined(CONFIG_JZ4780) || defined(CONFIG_JZ4775)

#ifndef CFG_NAND_TOGGLE

	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
#else
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa	
#endif
#endif /* CONFIG_JZ4780 */
	
	/* row cycle flag */
#if (CFG_NAND_ROW_CYCLE == 3)
	.word   0xaaaaaaaa	/*CFG_NAND_ROW_CYCLE == 3*/
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
#else
	.word   0x55555555	/*CFG_NAND_ROW_CYCLE == 2*/
	.word   0x55555555
	.word   0x55555555
	.word   0x55555555
	.word   0x55555555
	.word   0x55555555
	.word   0x55555555
	.word   0x55555555
#endif /* row cycle flag */
	/* pagesize_flag1 */
#if (CFG_NAND_PAGE_SIZE < 4096)
        .word   0x55555555
        .word   0x55555555
        .word   0x55555555
        .word   0x55555555
        .word   0x55555555
        .word   0x55555555
        .word   0x55555555
	.word	0x55555555
#else
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
#endif
	/* pagesize_flag0 */
#if ((!defined CFG_NAND_TOGGLE && CFG_NAND_PAGE_SIZE == 512) || (CFG_NAND_PAGE_SIZE == 4096))
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
	.word	0x55555555
#else
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
	.word	0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
#endif

#if defined(CONFIG_JZ4780) || defined(CONFIG_JZ4775)
#if (CFG_NAND_PAGE_SIZE == (16 * 1024))
	.word	0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
        .word   0xaaaaaaaa
#else
	.word	0x55555555
        .word   0x55555555
        .word   0x55555555
        .word   0x55555555
        .word   0x55555555
        .word   0x55555555
        .word   0x55555555
        .word   0x55555555

#endif	
#endif
#endif /* CONFIG_JZ4760 || defined(CONFIG_JZ4770) || CONFIG_JZ4810 || CONFIG_JZ4780 || CONFIG_JZ4775 */

#elif defined(CONFIG_NOR_SPL)
	.word	JZ_NORBOOT_CFG
#elif defined(CONFIG_MSC_SPL) || defined(CONFIG_MSC_X_BOOT)
	.word	JZ_MSCBOOT_CFG
#endif

/* ==========================================================================*/
/*;; SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSsss */
/* ==========================================================================*/

        /* flush BTB */
	mfc0	$2, $16, 7
	nop
	ori	$2, $2, 2    /* bit 1: write 1 to BTB invalid */
	mtc0	$2, $16, 7
	nop
	

#ifdef CONFIG_JzRISC
	/*
	 * CU0=UM=EXL=IE=0, BEV=ERL=1, IP2~7=1
	 */
	li	t0, 0x0040FC04
	mtc0	t0, CP0_STATUS
#endif
	/* CAUSE register */
#ifdef CONFIG_JzRISC
	/* IV=1, use the specical interrupt vector (0x200) */
	li	t1, 0x00800000
	mtc0	t1, CP0_CAUSE

#if defined(CONFIG_JZ4780) || defined(CONFIG_JZ4775)
	/*	enable bridge radical mode
	*/
	la	t0, CPM_BASE
	lw	t1, 0x24(t0)
	ori	t1, t1, 0x22
	sw	t1, 0x24(t0)
#endif
#endif

	/* Initialize GOT pointer.
	*/
	bal     1f
	nop
/*	.word   _GLOBAL_OFFSET_TABLE_ */
	1:
	move    gp, ra
	lw      t1, 0(ra)
	move	gp, t1

#if 1
init_caches:
	la	$25, 0x80000000
        li      $2, 3                   // cacheable for kseg0 access
        mtc0    $2, $16                 // CP0_CONFIG
        nop  

        li      $2, 0x20000000          // enable idx-store-data cache insn 
        mtc0    $2, $26                 // CP0_ERRCTL

        ori     $2, $25, 0              // start address
        ori     $3, $2, 0x7fe0          // end address, total 32KB 
        mtc0    $0, $28, 0              // CP0_TAGLO
        mtc0    $0, $28, 1              // CP0_DATALO
#endif
cache_clear_a_line:
        cache   0x8, 0($2)              // Index_Store_Tag_I
        cache   0x9, 0($2)              // Index_Store_Tag_D
        bne     $2, $3, cache_clear_a_line
        addiu   $2, $2, 32              // increment CACHE_LINE_SIZE

#if 1
        ori     $2, $25, 0              // start address
        ori     $3, $2, 0x3fe0          // end address, total 16KB spl stack space 
        la      $4, 0x1ffff000          // physical address and 4KB page mask
cache_alloc_a_line:
        and     $5, $2, $4
        ori     $5, $5, 1               // V bit of the physical tag
        mtc0    $5, $28, 0              // CP0_TAGLO
        cache   0x8, 0($2)              // Index_Store_Tag_I
        cache   0x9, 0($2)              // Index_Store_Tag_D
        bne     $2, $3, cache_alloc_a_line
        addiu   $2, $2, 32              // increment CACHE_LINE_SIZE
#endif

/* ==========================================================================*/
/* ;; EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEe */
/* ==========================================================================*/
	/* Setup stack pointer and jump to C routine. kernel param address */
	la	sp, 0x80004000//(TEXT_BASE+0x4000)
	j	spl_main
	nop


#if defined(CONFIG_JZ4760) || defined(CONFIG_JZ4760B) || \
    defined(CONFIG_JZ4770) || defined(CONFIG_JZ4780)  || \
    defined(CONFIG_JZ4775)

        /*
         * cpu was reset from sleep  mode(when power down p0)
         */
resume_from_sleep:

#if defined(CONFIG_JZ4760B)
	la	t0, 0xb3020050
	li	t1, 0x0000ff00
	sw	t1, 0(t0)

	la	t0, CPM_CPPCR
	lw	t1, 0(t0)
        li      t0,  ~(0x1<<9)
	and	t1, t1, t0
	la	t0, CPM_CPPCR
	sw	t1, 0(t0)

	la	t0, 0xb3420000
	lw	t1, 0(t0)
	la	t0, 0xb3020018
	sw	t1, 0(t0)
#elif defined(CONFIG_JZ4770)
	la	t0, 0xb302005c
	lw	t1, 0(t0)
	li	t2, ~(0xff000000)
	and	t1, t1, t2
	sw	t1, 0(t0)
#elif defined(CONFIG_JZ4780)
	/*do some thing*/
#elif defined(CONFIG_JZ4775)
	/*do some thing*/
#endif
        /*
         * jump to resume entry point
         */
        la      t0, CPM_CPSPR
        lw      t1, 0(t0)
        li      t0, 0x80000000
        or      t0, t1

        j       t0
        nop

#endif /* CONFIG_JZ4760 */

	.globl	validate_cache
	.type	validate_cache, @function
validate_cache:
#if 1
	la	$25, 0x80000000
        ori     $2, $25, 0              // start address
        ori     $3, $2, 0x3fe0          // end address, total 16KB spl stack space
	la      $4, 0x1ffff000          // physical address and 4KB page mask
cache_alloc_a_line_2:
        and     $5, $2, $4		// V bit of the physical tag
        mtc0    $5, $28, 0              // CP0_TAGLO
        cache   0x8, 0($2)              // Index_Store_Tag_I
        cache   0x9, 0($2)              // Index_Store_Tag_D
        bne     $2, $3, cache_alloc_a_line_2
        addiu   $2, $2, 32              // increment CACHE_LINE_SIZE
	jr	$31
	nop
#endif
