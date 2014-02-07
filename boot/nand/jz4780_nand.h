#include <config.h>

#if defined(NEW_HYNIX_NAND)
#define RETRY_COUNT     8
#elif defined(HYNIX_NAND)
#define RETRY_COUNT     7
#else
#define RETRY_COUNT     0
#endif

extern void get_read_retry_data(void);
extern void set_read_retry_data(void);
void get_nand_id(char *nand_id);
/*
 * NAND flash definitions
 */
#define REG_NEMC_PNCR REG32(NEMC_PNCR)
#define REG_NEMC_PNDR REG32(NEMC_PNDR)

#define NAND_DATAPORT	CFG_NAND_BASE
#define NAND_ADDRPORT   (CFG_NAND_BASE | NAND_ADDR_OFFSET)
#define NAND_COMMPORT   (CFG_NAND_BASE | NAND_CMD_OFFSET)

#define ECC_BLOCK	1024
#define ECC_BYTES	 (CFG_NAND_BCH_BIT * 14 / 8)
#define ECC_STEPS	(CFG_NAND_PAGE_SIZE / ECC_BLOCK)
#define FS_BYTES	CFG_NAND_ECC_POS

#define __nand_cmd(n)		(REG8(NAND_COMMPORT) = (n))
#define __nand_addr(n)		(REG8(NAND_ADDRPORT) = (n))
#define __nand_data8()		REG8(NAND_DATAPORT)
#define __nand_data16()		REG16(NAND_DATAPORT)
#define __read_data(n)		((n) = REG8(NAND_DATAPORT))
#define __write_data(n)		(REG8(NAND_DATAPORT) = (n))

#define __nand_enable()		(REG_NEMC_NFCSR |= NEMC_NFCSR_NFE1 | NEMC_NFCSR_NFCE1)
//#define __nand_disable()        (REG_NEMC_NFCSR &= ~(NEMC_NFCSR_NFE1 | NEMC_NFCSR_NFCE1))
#define __nand_disable()        do {} while (0)//(REG_NEMC_NFCSR &= ~(NEMC_NFCSR_NFE1 | NEMC_NFCSR_NFCE1))

#define __tnand_enable() \
	do { \
		        REG_NEMC_NFCSR |= NEMC_NFCSR_TNFE1 | NEMC_NFCSR_NFE1; \
		        __tnand_dphtd_sync(1); \
		        REG_NEMC_NFCSR |= NEMC_NFCSR_NFCE1 | NEMC_NFCSR_DAEC; \
	} while (0)

#define __tnand_disable() \
	do { \
		        REG_NEMC_NFCSR &= ~NEMC_NFCSR_NFCE1; \
		        __tnand_dphtd_sync(1); \
		        REG_NEMC_NFCSR &= ~(NEMC_NFCSR_TNFE1 | NEMC_NFCSR_NFE1); \
	} while (0)



#define __tnand_read_perform() \
	do { \
		        REG_NEMC_TGWE |= NEMC_TGWE_DAE; \
		        __tnand_dae_sync(); \
	} while (0)

#define __nemc_pn_reset_and_enable() \
	do { \
		        REG_NEMC_PNCR = NEMC_PNCR_PNRST | NEMC_PNCR_PNEN; \
	} while (0)

#define __nemc_pn_disable() \
	do { \
		        REG_NEMC_PNCR = 0x0; \
	} while (0)
