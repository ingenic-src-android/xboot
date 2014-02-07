#ifndef __NAND_IDS_H_
#define __NAND_IDS_H_


/* Static Memory Control Register(SMCR) */
#define NEMC_SMCR_SMT_BIT   0
#define NEMC_SMCR_BL_BIT    1
#define NEMC_SMCR_BW_BIT    6   /* bus width, 0: 8-bit 1: 16-bit */
#define NEMC_SMCR_TAS_BIT   8
#define NEMC_SMCR_TAH_BIT   12
#define NEMC_SMCR_TBP_BIT   16
#define NEMC_SMCR_TAW_BIT   20
#define NEMC_SMCR_STRV_BIT  24

#define NEMC_SMCR_TAS_MASK  0xf
#define NEMC_SMCR_TAH_MASK  0xf
#define NEMC_SMCR_TBP_MASK  0xf
#define NEMC_SMCR_TAW_MASK  0xf
#define NEMC_SMCR_STRV_MASK 0x3f

/* for read-retry */
#define SAMSUNG_NAND            0x01
#define HYNIX_NAND              0x02
#define NEW_HYNIX_NAND          0x03
#define MICRON_NAND             0x04
#define TOSHIBA_NAND            0x05


/* Option constants for bizarre disfunctionality and real
 * features
 */
/* Chip can not auto increment pages */
#define NAND_NO_AUTOINCR        0x00000001
/* Buswitdh is 16 bit */
#define NAND_BUSWIDTH_16        0x00000002
/* Device supports partial programming without padding */
#define NAND_NO_PADDING         0x00000004
/* Chip has cache program function */
#define NAND_CACHEPRG           0x00000008
/* Chip has copy back function */
#define NAND_COPYBACK           0x00000010
/* AND Chip which has 4 banks and a confusing page / block
 * assignment. See Renesas datasheet for further information */
#define NAND_IS_AND             0x00000020
/* Chip has a array of 4 pages which can be read without
 * additional ready /busy waits */
#define NAND_4PAGE_ARRAY        0x00000040
/* Chip requires that BBT is periodically rewritten to prevent
 * bits from adjacent blocks from 'leaking' in altering data.
 * This happens with the Renesas AG-AND chips, possibly others.  */
#define BBT_AUTO_REFRESH        0x00000080
/* Chip does not require ready check on read. True
 * for all large page devices, as they do not support
 * autoincrement.*/
#define NAND_NO_READRDY         0x00000100
/* Chip does not allow subpage writes */
#define NAND_NO_SUBPAGE_WRITE   0x00000200
/* Options valid for Samsung large page devices */
#define NAND_SAMSUNG_LP_OPTIONS \
        (NAND_NO_PADDING | NAND_CACHEPRG | NAND_COPYBACK)

/*
 * Only 5 bytes are required to identify the NAND device,
 * but this must be a multiple of the maximum data bus width
 */
#define NAND_ID_SIZE            6

/**
 * struct nand_flash_dev - NAND Flash Device ID Structure
 * @name:	Identify the device type
 * @id:		device ID code
 * @pagesize:	Pagesize in bytes. Either 256 or 512 or 0
 * 		If the pagesize is 0, then the real pagesize
 * 		and the eraseize are determined from the
 * 		extended id bytes in the chip
 * @erasesize:	Size of an erase block in the flash device.
 * @chipsize:	Total chipsize in Mega Bytes
 * @options:	Bitfield to store chip relevant options
 */
struct nand_flash_dev {
        char *name;
        int id;
        unsigned int extid;
        int realplanenum;
        int dienum;
        int tals;
        int talh;
        int trp;
        int twp;
        int trhw;
        int twhr;
	int twhr2;
	int trr;
	int twb;
	int tadl;
	int tcwaw;
        unsigned long pagesize;
        unsigned long erasesize;
        unsigned int oobsize;
        int rowcycle;
        int maxbadblocks;
        int maxvalidblocks;
        int eccblock;
        int eccbit;
        int buswidth;
        int badblockpos;
        unsigned long options;
	unsigned char lowdriver;
	unsigned char normaldriver;
	unsigned char highdriver;
	unsigned char high2driver;
	unsigned char timemode;
};
typedef struct nand_flash_dev NAND_FLASH_DEV;

void setup_nandtiming_optimize();
void printf(const char *fmt, ...);
#endif
