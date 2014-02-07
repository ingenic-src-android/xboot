#ifndef __MCP_CONFIG_H
#define __MCP_CONFIG_H

#include "ddr.h"

/*
 * This file contains the memory configuration parameters
 * 
 * mobile ddr SAMSUNG KMS5U000KM-B308 
 * 4Gb (64M x32 + 64M x32) Mobile DDR SDRAM
 */

/* Chip Select */
#define DDR_CS1EN 1 /* CSENx: whether a ddr chip exists
							0: un-used, 1: used */
#define DDR_CS0EN 1
#define DDR_DW32 1 /* 0: 16-bit data width, 1: 32-bit data width */

/* DDR paramters */
#define DDR_ROW 14 /* row address */
#define DDR_COL 10 /* column address */
#define DDR_BANK8 0 /* Banks each chip: 0: 4banks, 1: 8banks */
#define DDR_CL 3 /* CAS latency: 3, 2 */

#define DDR_tAL 0 /* tCK, Additive Latency */
#define DDR_BL 4 /* Burst length: 4: 16, 3: 8, 2: 4, 1:2 */

/* DDR controller timing1 register */
#define DDR_tWL 1 /* tCK, Write Latency */
#define DDR_tWR 12 /* ns, WRITE Recovery Time defined by register MR */
#define DDR_tWTR MAX(2, 0) /* tCK, WRITE to READ command delay */
#define DDR_tRTP MAX(2, 0) /* tCK, READ to PRECHARGE command period */

/* DDR controller timing2 register */
#define DDR_tRL (DDR_tAL + DDR_CL) /* tCK, Read Latency */
#define DDR_tRCD 15 /* ns, ACTIVE to READ or WRITE command period
						  to the same bank */
#define DDR_tRAS 40 /* ns, ACTIVE to PRECHARGE command period
						  to the same bank */
#define DDR_tCCD 4 /* tCK, CAS# to CAS# command delay (2 * tWTR) */

/* DDR controller timing3 register */
#define DDR_tRP 15 /* ns, PRECHARGE command period to the same bank */
#define DDR_tRC (DDR_tRAS + DDR_tRP) /* ns, ACTIVE to ACTIVE
											command period to the same bank */
#define DDR_tRRD 10 /* ns, ACTIVE bank A to ACTIVE bank B command period */
#define DDR_tCKSRE MAX(1, 0) /** ns, Valid Clock Requirement after
								  Self Refresh Entry or Power-Down Entry */

/* DDR controller timing4 register */
#define DDR_tCKE MAX(2, 0) /* tCK, CKE minimum pulse width */
#define DDR_tRFC 120 /* ns, AUTO-REFRESH command period */
#define DDR_tMINSR DDR_GET_VALUE(DDR_tRFC, ps) /* Minimum Self-Refresh /
												  Deep-Power-Down */
#define DDR_tXP 2 /* tCK, EXIT-POWER-DOWN to next valid command period */
#define DDR_tMRD 2 /* tCK, unit: tCK Load-Mode-Register
						  to next valid command period */

/* DDR controller timing5 register */
#define DDR_tRDLAT (DDR_tRL - 2)
#define DDR_tWDLAT (DDR_tWL - 1)
#define DDR_tRTW (DDR_tRL + DDR_tCCD + 2 - DDR_tWL) /* Read to Write delay */

/* DDR controller timing6 register */
#define DDR_tFAW 50 /* ns, Four bank activate period */
#define DDR_tXS 120 /* ns, Exit self-refresh to next valid command delay */
#define DDR_tXSRD DDR_GET_VALUE(DDR_tXS, ps) /* ns, Exit self refresh to
												   a read command */

/*
 * MDDR controller refcnt register
 */
#define DDR_tREFI 7800 /* Refresh period: 4096 refresh cycles/64ms */
#define DDR_CLK_DIV 1 /* Clock Divider. auto refresh
			   * cnt_clk = memclk/(16*(2^DDR_CLK_DIV))
			   */
#define CONFIG_SDRAM_SIZE_512M 1
#endif  /* __MCP_CONFIG_H */


