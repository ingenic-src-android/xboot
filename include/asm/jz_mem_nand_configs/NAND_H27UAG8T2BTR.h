#ifndef __NAND_CONFIG_H
#define __NAND_CONFIG_H

#define NEW_HYNIX_NAND      1
/*
 * This file contains the nand configuration parameters for the cygnus board.
 */
/*-----------------------------------------------------------------------
 * NAND FLASH configuration
 */
#define CFG_NAND_BCH_BIT	24
#define CFG_NAND_BW8		1               /* Data bus width: 0-16bit, 1-8bit */
#define CFG_NAND_PAGE_SIZE      (1024 * 8)
#define CFG_NAND_ROW_CYCLE	3
#define CFG_NAND_BLOCK_SIZE	(2048 << 10)	/* NAND chip block size		*/
#define CFG_NAND_BADBLOCK_PAGE	0		/* NAND bad block was marked at this page in a block, starting from 0 */
#define CFG_NAND_OOB_SIZE	448
#define CFG_NAND_TOTAL_BLOCKS   (1024*8)      /*8G nand*/

#define X_DELAY_TRR		25
#define X_DELAY_TADL		100
#define X_DELAY_TWHR		100
#endif /* __NAND_CONFIG_H */
