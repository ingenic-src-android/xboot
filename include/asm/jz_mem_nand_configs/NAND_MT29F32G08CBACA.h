#ifndef __NAND_CONFIG_H
#define __NAND_CONFIG_H

/*
 *  * This file contains the nand configuration parameters for the cygnus board.
 *   */
/*-----------------------------------------------------------------------
 *  * NAND FLASH configuration
 *   */

#define CONFIG_NAND_K9GAG08U0D

#define CFG_NAND_BCH_BIT        24               /* Speif () {
						   y the hardware BCH algorithm for nand (4|8) */
#define CFG_NAND_BW8            1               /* Data bus width: 0-16bit, 1-8bit */
#define CFG_NAND_PAGE_SIZE      4096
#define CFG_NAND_OOB_SIZE       224             /* Size of OOB space per page (e.g. 64 128 etc.) */
#define CFG_NAND_ROW_CYCLE      3
#define CFG_NAND_BLOCK_SIZE     (1024 << 10)     /* NAND chip block size         */
#define CFG_NAND_BADBLOCK_PAGE  0             /* NAND bad block was marked at this page in a block, starting from 0 */
#define CFG_NAND_TOTAL_BLOCKS   (1024*4)      /*4G nand*/

#define X_DELAY_TRR		20
#define X_DELAY_TADL		70
#define X_DELAY_TWHR		60
#endif /* __NAND_CONFIG_H */
