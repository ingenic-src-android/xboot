//#include <jz4780.h>
#include <config.h>
#include "jz4780_nand.h"
#include "nand_ids.h"

#define LP_OPTIONS (NAND_SAMSUNG_LP_OPTIONS | NAND_NO_READRDY | NAND_NO_AUTOINCR)
#define LP_OPTIONS16 (LP_OPTIONS | NAND_BUSWIDTH_16)

/*
*	Chip ID list
*
*	Name. ID code, pagesize, chipsize in MegaByte, eraseblock size,
*	options
*
*	Pagesize; 0, 256, 512
*	0	get this information from the extended chip ID
+	256	256 Byte page size
*	512	512 Byte page size
*/
NAND_FLASH_DEV nand_flash_chips[] =
{
	/*
        {name              		id        	extid           planes  dienum
        tals	talh	trp	twp     trhw	twhr	twhr2	trr	twb     tadl    tcwaw   pagesize
        blocksize oobsize cycle maxbadblocks maxvalidblocks eccblock  eccbit  buswidth  badblockpos options}
	lowdriver	normaldriver	highdriver      high2driver	timemode
	*/
	{"MICRON_MT29F32G08CBADAWP",    0x2C44,         0x00A94B44,     2, 1,
	 10,     5,    15, 15,   100,    60,     200,     20,    100,    70,     0,      8192,
	 2048*1024,      744, 3, 74, 2128,  1024, 40, 8, 0, MICRON_NAND,
	 0x03,  0x02,  0x01,  0x00,           0x04},

	{"SAMSUNG_K9GBG08U0A",          0xECD7,		0x43547A94, 	2,	1,
        12,     5,  	25,	25,	100,    120,    300,    20,     100,    300,    300,    8192,
        1024*1024,      640,    3,	116,	4152,		1024,   24,	8,	0,     	SAMSUNG_NAND,
	0x02,		0x04,		0x06,		0,              0},

	{"SAMSUNG_K9GAG08U0E",          0xECD5,         0x42507284,     2,      1,
        15,     5,      15,     15,     100,    60,     300,     20,    100,    300,	300,    8192,
        1024*1024,      436,    3,      58,     2076,           1024,   24,     8,      0,      SAMSUNG_NAND,
	0x02,		0x04,		0x06,		0,              0,},

	{"MICRON_MT29F32G08CBACAWP",    0x2C68,         0x00A94A04,     2,	1,
        10,     5,   	15,	15,  	100,    60,     200,     20,    100,    70,     0,      4096,
        1024*1024,      224,	3,	100,	4096,		1024,	24,	8,	0,	MICRON_NAND,
	0x03,		0x02,		0x01,		0x00,           0x04},

	{"SAMSUNG_K9GAG08U0D", 		0xECD5,		0x41342994,	2,	1,
        15,	5,	15,	15,	100,	60,	60,     20,     0,     100,     0,      4096,
        512*1024,       218, 	3,	100,	4096,		1024,	24,	8,	0,	SAMSUNG_NAND,
	0x02,		0x04,		0x06,		0,              0},

	{"SAMSUNG_K9K8G08U0D", 0xECD3, 0x00589551,  4,		1,
        12,	5,	12,	12,	100,	60,	60,     20,     0,     70,     0,      2048,
        128*1024,       64, 	3,	160,	8192,		1024,	4,	8,	0,	SAMSUNG_NAND,
	0,		0,		0,		0,              0},

	{"MICRON_MT29F32G08CBABAWP",	0x2C68,		0x00894604,	2,	1,
	10,	5,	15,	15,	100,	60,	200,	20,	100,	70,	0,	4096,
	1024*1024,	224,	3,	100,	4096,		1024,	24,	8,	0,	MICRON_NAND,
	0x03,		0x02,		0x01,		0x00,           0x04},

	{"HYNIX_HY27UBG8T2BTR",		0xADD7,		0xC374DA94,	2,	1,
	10,	5,	15,	15,	100,	80,	200,	20,	100,	200,	0,	8192,
	2048*1024,	640,	3,	48,	2048,		1024,	40,	8,	0,	HYNIX_NAND,
	0,		0,		0,		0,              0},

	{"MICRON_MT29F64G08CBAAAWP",	0x2C88,		0x00A94B04,	2,	1,
	10,	5,	15,	15,	100,	60,	200,	20,	100,	70,	0,	8192,
	2048*1024,	448,	3,	100,	4096,		1024,	24,	8,	0,	MICRON_NAND,
	0x03,		0x02,		0x01,		0x00,           0x04},

	{"HYNIX_H27UCG8T2ATR",		0xADDE,		0xC474DA94,	2,	1,
	10,	5,	12,	12,	100,	80,	200,	20,	100,	200,	0,	8192,
	2048*1024,	640,	3,	120,	4180,		1024,	40,	8,	0,	NEW_HYNIX_NAND,
	0,		0,		0,		0,              0},

	{"HYNIX_H27UAG8T2BTR",		0xADD5,		0x42749A94,	2,	1,
	12,	5,	12,	12,	100,	100,	200,	25,	100,	200,	0,	8192,
	2048*1024,	448,	3,	120,	1024,		1024,	24,	8,	0,	NEW_HYNIX_NAND,
	0,		0,		0,		0,              0},

	{"MICRON_MT29F128G08CFABA",	0x2C64,		0x00A94B44,	2,	1,
	10,	5,	15,	15,	100,	60,	200,	20,	100,	70,	0,	8192,
	2048*1024,	744,	3,	100,	4096,		1024,	40,	8,	0,	MICRON_NAND,
	0x03,		0x02,		0x01,		0x00,           0x04},

	{"SAMSUNG_K9HBG08U1M",		0xECD5, 	0x00682555,  	4,	1,
	 15,	5,	15,	15,     100,	60,	300,     20,     100,     70,   70,  2048,
	256*1024,       64, 	3,	200,	8192,	        1024,	4,	8,	0,	SAMSUNG_NAND,
	0x0,		0x0,		0x0,		0,              0},

        /* -------------------------------------no test---------------------------------- */
	{"TOSHIBA_TC58TEG6DCJTA00",	0x98DE,		0x57729384,	2,	1,
	10,	5,	10,	10,	30,	180,	300,	20,	100,	300,	0,	16384,
	4096*1024,	1280,	3,	74,	2092,		1024,	40,	8,	0,	TOSHIBA_NAND,
	0x02,		0x04,		0x06,   	0x00,           0},

#if 0
        /* -------------------------------------no test---------------------------------- */
        {"TOSHIBA_TC58NVG2S3ETA00",     0x98DC,         0x00761590,     2,      1,
        12,	5,	12,	12,	100,	60,	-1,     -1,     -1,     -1,     2048,
        128*1024,       64, 	3,	100,	4096,	512,	4,	8,	0,	0},

	{"SAMSUNG_K9F1208U0C",	  	0xEC76,		0x00743F5A,	1,	1,
        12,	5,	12,	12,	100,	60,	-1,     -1,     -1,     -1,     512,
        16*1024,        16, 	3,	100,	4096,	512,	8,	8,	0,	0},

	{"HYNIX_HY27UF084G2B",	        0xADDC,	        0x00549510,	1,	1,
        15,	5,	15,	15,	60,	60,	-1,     -1,     -1,     -1,     2048,
        128*1024,       64, 	3,	80,	4096,	512,	1,	0,	0,	LP_OPTIONS},

	{"MICRON_MT29F64G08CFAAA",	0x2CD7,		0x00843E94,	2,	1,
        10,	5,	10,	10,	100,	60,	-1,     -1,     -1,     -1,     4096,
        512*1024,       218,	3,	400,	4096*4,	539,	12,	8,	0,	LP_OPTIONS},

	{"SAMSUNG_K9GAG08U0M", 		0xECD5,		0x0074b614,	2,	1,
        12,	5,	12,	12,	100,	60,	-1,     -1,     -1,     -1,     4096,
        0512*1024,       128, 	3,	100,	4096,	512,	4,	8,	0,	LP_OPTIONS},

	{"SAMSUNG_K9GAG08U0E", 		0xECD5,		0x42507284,	2,	1,
        12,	5,	12,	12,	100,	60,	-1,     -1,     -1,     -1,     8192,
        1024*1024,      436, 	3,	100,	2048,	512,	8,	8,	0,	LP_OPTIONS},

	{"SAMSUNG_K9GBG08U0M",          0xECD7,		0x42547294, 	2,	1,
        12,     5,  	12,	12,	100,    60,     -1,     -1,     -1,     -1,     8192,
        1024*1024,      436,    3,	100,	2048,	512,   	8,	8,	0,     	LP_OPTIONS},

	{"HYNIX_H8BCS0UN0MCR",          0xADBC,	 	0x00545510,	1,	1,
        25,	10,	25,	25,	100,	60,	-1,     -1,     -1,     -1,     2048,
        128*1024,       64,	3,	80,	4096,	512,	4,	16,	0,	LP_OPTIONS16},

	{"SAMSUNG_K9G8G08U0M",          0xECD3, 	0x00642514,	2,	1,
        15,	5,	15,     15,	60,   	60,  	-1,     -1,     -1,     -1,     2048,
        256*1024,       64,     3,	100,	4096,	512,	4,	8,	0,	LP_OPTIONS},

	{"SAMSUNG_K9F1G08U0B",          0xECF1,		0x00409500,    	1,	1,
        15,	5,	15,	15,	100,	60, 	-1,     -1,     -1,     -1,     2048,
        128*1024,       64, 	3,	20,	1024,	512,	1,	8,	0,	LP_OPTIONS},

	{"SAMSUNG_K9F1G08U0M",		0xECF1,         0,	        1,	1,
        15,	5,	15,	15,     100, 	60,	-1,     -1,     -1,     -1,     2048,
        128*1024,       64, 	3,	20,	1024,	512,	4,	8,	0,	LP_OPTIONS},

	{"SAMSUNG_K9HBG08U1M",		0xECD5, 	0x00682555,  	2,	2,
        15,	5,	15,	15,     100,	60,	-1,     -1,     -1,     -1,     2048,
        256*1024,       64, 	3,	200,	8192,	512,	4,	8,	0,	LP_OPTIONS},

	{"SAMSUNG_K9LBG08U0M",		0xECD7, 	0x0078B655,     1,	1,
        12,	5,	15,	15,	100,	60,	-1,     -1,     -1,     -1,     4096,
        512*1024,       64, 	3,     	200,	8192,	512,	4,	8,	0,	LP_OPTIONS},

        /* -------------------------------------no test---------------------------------- */

	{"SAMSUNG_K9G8G08U0A", 0xECD3, 0x0064a514,      2,		12,	5,  	12,	12,  	100,  60,  2048,     256*1024,	64, 	3,	100,		4096,		,	0,	,	,	},
	{"SAMSUNG_K9K8G08U0A", 0xECD3, 0x00589551,  0x400002,		12, 	5,   	12,	12,  	100,  60,  2048,     128*1024,	64, 	3,	160,		8192,		,	0,	,	,	},
	{"SAMSUNG_K9G8G08U0B", 0xECD3, 0x0064a514,      2,		12,	5,   	12,	12,  	100,  60,  2048,     256*1024,	64, 	3,	100,		4096,		,	0,	,	,	},
	{"SAMSUNG_K9LAG08U0M", 0xECD5, 0x00682555,  0x800002,		12, 	5,   	12,	12,  	 60,  60,  2048,     256*1024,	64, 	3,	200,		8192,		,	0,	,	,	},
	{"SAMSUNG_K9WAG08U1M", 0xECD3, 0x01589551,  0x400002,		12,	5,   	12,	12,  	100,  60,  2048,     128*1024,	64, 	3,	160*2,		8192*2,		512,	1,	,	,	},
	{"SAMSUNG_K9LAG08U1M", 0xECD3, 0x01642514,	2,		15,	5,   	15,	15,  	100,  60,  2048,     256*1024,	64, 	3,	200,		8192,		,	0,	,	,	},
	{"SAMSUNG_K9LAG08U0A", 0xECD5, 0x00682555,  0x800002,		15,	5,   	15,	15,  	100,  60,  2048,     256*1024,	64, 	3,	200,		8192,		,	0,	,	,	},
	{"SAMSUNG_K9HBG08U1A", 0xECD5, 0x01682555,  0x800002,		15,	5,   	15,	15,  	100,  60,  2048,     256*1024,	64, 	3,	200*2,		8192*2,		,	0,	,	,	},
	{"SAMSUNG_K9F4G08U0A", 0xECD3, 0x00549510,	2,		12,	5,   	12,	12,  	100,  60,  2048,     512*1024,	64, 	3,	80,		4096,		512,	1,	,	,	},
	{"SAMSUNG_K9K8G08U0M", 0xECD3, 0x00589551,	2,		12,	5,   	12,	12,  	100,  60,  2048,     1024*1024,	64, 	3,	160,		8192,		512,	1,	,	,	},

	{"HYNIX_HY27UU08AG5M", 0xADD3, 0x0064A514,  	2,		12,	5,	12,	12,	100,  80,  2048,     256*1024,	64, 	3,	100,		4096,		,	0,	,	,	},
	{"HYNIX_HY27UU08AG5M", 0xADD3, 0x0164A514,  	2,		12, 	5,	12,	12,	100,  80,  2048,     256*1024,	64, 	3,	200,		8192,		,	0,	,	,	},
	{"HYNIX_HY27UT088G2M", 0xADD3, 0x0064A514,  0x400002,		12,	5,	12,	12,	100,  80,  2048,     256*1024,	64, 	3,	100,		4096,		528,	4,	,	,	},
	{"HYNIX_HY27UT088G2A", 0xADD3, 0x0034A514,  	2,		12,  	5,   	12,	12,  	100,  80,  2048,     256*1024,	64, 	3,	100,		4096,		528,	4,	,	,	},
	{"HYNIX_HY27UF081G2M", 0xADF1, 0x00001500,  	1,		5, 	15,  	25,	40,	60,   60,  2048,     128*1024,	64, 	2,	20,		1024,		,	0,	,	,	},
	{"HYNIX_HY27UU08AG5A", 0xADD3, 0x0134A514,  	2,		12,  	5,   	12,	12,  	100,  80,  2048,     256*1024,	64, 	3,	200,		8192,		528,	4,	,	,	},
	{"HYNIX_HY27UV08BG5A", 0xADD5, 0x0138A555,  0x800002,		12, 	5,   	12,	12,	100,  80,  2048,     256*1024,	64, 	3,	200*2,		8192*2,		528,	4,	,	,	},
	{"HYNIX_HY27UU088G5M", 0xADDC, 0x01002584,      1,		5,  	15,  	40,	40,	80,   80,  2048,     256*1024,	64, 	3,	118,		4096,		,	0,	,	,	},
	{"HYNIX_HY27UF082G2M", 0xADDA, 0x00001500,  	1,		0,  	10,  	25,	25,  	60,   60,  2048,     128*1024,	64, 	3,	40,		2048,		,	0,	,	,	},
	{"HYNIX_HY27UF084G2M", 0xADDC, 0x00009580,  	1,		15,  	5,   	15,	15,  	60,   60,  2048,     128*1024,	64, 	3,	80,		4096,		512,	1,	,	,	},
	{"HYNIX_HY27UT084G2M", 0xADDC, 0x00002584,  	1,		5, 	15,	40,	40,  	80,   80,  2048,     256*1024,	64, 	3,	59,		2048,		512,	4,	,	,	},

	{"MICRON_MT29F16G08MAA",  0x2CD3, 0x00743E94,   1,	 	10, 	5,   	10,	10,  	100,  60,  4096,     512*1024,	128,	3,	160,		4096,		539,	8,	,	,	},
	{"MICRON_MT29F32G08QAA",  0x2CD5, 0x00743E94,   1,	 	10, 	5,   	10,	10,  	100,  60,  4096,     512*1024,	128,	3,	160*2,		4096*2,		,	0,	,	,	},
	{"MICRON_MT29F64G08TAA",  0x2CD5, 0x00783ED5,   1,	 	10, 	5,   	10,	10,  	100,  60,  4096,     512*1024,	128,	3,	160*4,		4096*4,		,	0,	,	,	},
	{"MICRON_MT29F32G08CBAAA",0x2CD7, 0x00843E94,   1,	 	10, 	5,   	10,	10,  	100,  60,  4096,     512*1024,	218,	3,	200,		8192,		,	0,	,	,	},

	{"INTEL_29F16G08AAMC1",	  0x89D5, 0x00743E94,   1,		10, 	5,   	10,	10,  	100,  60,  4096,     512*1024,	128, 	3,	160,		4096,		539,	8,	,	,	},
	{"INTEL_29F32G08CAMC1",	  0x89D5, 0x01743E94,   1,		10, 	5,   	10,	10,  	100,  60,  4096,     512*1024,	128, 	3,	160*2,		4096*2,		539,	8,	,	,	},
	{"INTEL_29F64G08FAMC1",	  0x89D7, 0x01783ED5,   1,		10,	5,   	10,	10,  	100,  60,  4096,     512*1024,	128, 	3,	160*4,		4096*4,		539,	8,	,	,	},
	{"INTEL_29F32G08AAMD1",	  0x89D7, 0x00843E94,   1,		10,	5,   	10,	10,  	100,  60,  4096,     512*1024,	218, 	3,	160,		4096,		,	0,	,	,	},
	{"INTEL_29F32G08AAMDB",	  0x8968, 0x00a94604,   1,		10,  	5,   	10,	10,  	100,  60,  4096,     1024*1024,	218, 	3,	160,		4096,		,	0,	,	,	},



		///////////////////////////////////////////////////
		//The following list no tested......
		///////////////////////////////////////////////////

	{"HYNIX_HY27UV08AG5A",	 0xADD3, 0x01002585,	1,		5,  	15,	40,	40,	80,   80,  2048,      256*1024, 64, 	3,             236,          8192,         528,  4        	,	},
	{"HYNIX_HY27UV08BG5M",	 0xADD5, 0x0138A555,	2,		12,	5,	12,	12,	100,  80,  2048,      256*1024, 64, 	3,             400,         16384,         ,        0       ,	},
	{"HYNIX_HY27UF081G2A",	 0xADF1, 0x00001D80,	1,		15,	5,	15,	15,	60,   60,  2048,      128*1024, 64, 	2,             20,          1024,          528,  1        	,	},
	{"HYNIX_HY27UH08AGDM",	 0xADD3, 0x010095C1,	1,		15,	5,	15,	15,	60,   60,  2048,      128*1024, 64, 	3,             160*2,        8192*2,       512,  1       ,	},
	{"HYNIX_HY27UH08AG5M",	 0xADD3, 0x010095C1,	1,		15,	5,	15,	15,	60,   60,  2048,      128*1024, 64, 	3,             160*2,        8192*2,       512,  1       ,	},
	{"HYNIX_HY27UH088GDM",	 0xADDC, 0x00001500,	1,		5,	10,	25,	25,   60,   60,  2048,      128*1024, 64, 	3,             160,          8192,         ,     0        	,	},
	{"HYNIX_HY27UH088G2M",	 0xADD3, 0x00001500,	1,		5,	10,	25,	25,   60,   60,  2048,      128*1024, 64, 	3,             160,          8192,         ,        0     	,	},
	{"HYNIX_HY27UG088G5M",	 0xADDC, 0x01009580,	1,		15,	5,	15,	15,   60,   60,  2048,      128*1024, 64, 	3,             160,          8192,         512,  1        	,	},
	{"HYNIX_HY27UG088GDM",	 0xADDC, 0x01009580,	1,		15,	5,	15,	15,   60,   60,  2048,      128*1024, 64, 	3,             160,          8192,         512,  1        	,	},
	{"HYNIX_HY27UG084G2M",	 0xADDC, 0x00001500,	1,		5,	10,	25,	25,   60,   60,  2048,      128*1024, 64, 	3,             80,          4096,          ,     0        	,	},
	{"HYNIX_HY27UG084GDM",	 0xADDA, 0x00001500,	1,		5,	10,	25,	25,   60,   60,  2048,      128*1024, 64, 	3,             80,          4096,           ,    0        	,	},
	{"HYNIX_HY27UV08AG5M",	 0xADD3, 0x01002585,	1,		5,	15,	40,	40,  	80,   80,  2048,      256*1024, 64, 	3,             236,          8192,         528,  4        	,	},

	{"SAMSUNG_K9L8G08U0M",	 0xECD3, 0x00582555,	2,		15,	5,	15,	15,  	100,  60,	 2048,      256*1024, 64, 	3,             100,          4096,          ,     0        	},
	{"SAMSUNG_K9G4G08U0A",	 0xECDC, 0x00542514,	2,		15,	5,	15,	15,  	100,  60,  2048,      256*1024, 64, 	3,             50,          2048,           512,  4        	},
	{"SAMSUNG_K9MCG08U5M", 	 0xECD5, 0x03682555,0x800002,		25,	10,	25,	25,  	100,  60,  2048,      256*1024, 64, 	3,             200*4,        8192*4,        ,    0        	},
	{"SAMSUNG_K9HCG08U1M",	 0xECD7, 0x0178B655,	1,		12,	5,	12,	12,  	100,  60,  4096,      512*1024, 128, 	3,             200*2,        8192*2,         ,     0        	},
	{"SAMSUNG_K9K8G08U1M",	 0xECD3, 0x01009510,	1,		12,	5,	12,	12,  	100,  60,  2048,      128*1024, 64, 	3,             80*2,        4096*2,         ,     0        	},
	{"SAMSUNG_K9W8G08U1M",	 0xECDC, 0x01001500,	1,		0,	10,	25,	15,  	100,  60,  2048,      128*1024, 64, 	3,             80*2,        4096*2,          ,     0        	},
	{"SAMSUNG_K9F8G08U0M",	 0xECD3, 0x0064A610,	1,		12,	5,	12,	12,  	100,  60,  4096,      256*1024, 128, 	3,             80,          4096,           ,     0        	},
	{"SAMSUNG_K9NBG08U5M",	 0xECD3, 0x03589551,0x400002,		25,	10,	25,	25,  	100,  60,  2048,      128*1024, 64, 	3,             160*4,        8192*4,         512,     1        	},
	{"SAMSUNG_K9HAG08U1M",	 0xECD3, 0x01642514,	2,		15,	5,	15,	15,  	100,  60,  2048,      256*1024, 64, 	3,             200,          8192,           ,    0        	},

	{"MICRON_MT29F8G08FACWP",0x2CDC, 0x01001500,	1,		10,	5,	15,	15,   60,   60,  2048,      128*1024,  64, 		3,           80*2,        4096*2,          ,     0        	},
	{"MICRON_MT29F8G08FABWP",0x2CDC, 0x01001500,	1,		10,	5,	15,	15,   60,   60,  2048,      128*1024,  64, 		3,           80*2,        4096*2,          ,     0        	},
	{"MICRON_MT29F8G08BAA",	 0x2CD3, 0x005895D1,0x400002,		25,	10,	25,	25,  	100,  60,  2048,      128*1024,  64, 		3,           160,          8192,           ,     0       	},
	{"MICRON_MT29F8G08DAA",	 0x2CDC, 0x01549590,	2,		25,	10,	25,	25,  	100,  60,  2048,      128*1024,  64, 		3,           80*2,        4096*2,          ,     0       	},
	{"MICRON_MT29F16G08FAA", 0x2CD3, 0x015895D1,0x400002,		25,	10,	25,	25,  	100,  60,  2048,      128*1024,  64, 		3,           160*2,        8192*2,         ,     0        	},
	{"MICRON_MT29F8G08AAA",	 0x2CD3, 0x00642E90,	1,		10,	5,	10,	10,  	100,  60,  4096,      256*1024,  128, 	3,          80,          4096,             ,     0        	},
	{"MICRON_MT29F16G08DAA", 0x2CD3, 0x01642E90,	1,		10,	5,	10,	10,  	100,  60,  4096,      256*1024,  128, 		3,         80*2,        4096*2,           ,     0       	},
	{"MICRON_MT29F32G08FAA", 0x2CD5, 0x01682ED1,	1,		10,	5,	10,	10,  	100,  60,  4096,      256*1024,  128, 		3,         160*2,        8192*2,          ,     0        	},
	{"MICRON_MT29F8G08MAAWC",0x2CD3, 0x0064A594,	2,		10,	5,	15,	15,   60,   60,  2048,      256*1024,   64, 		3,         80,          4096,            ,     0        	},
	{"MICRON_MT29F16G08QAAWC",0x2CD3,0x0164A594,	2,		10,	5,	25,	15,  	100,  60,  2048,      256*1024,   64, 		3,         80*2,        4096*2,          ,     0        	},
	{"MICRON_MT29F32G08TAAWC",0x2CD5,0x0168A5D5,0x800002,		10,	5,	25,	15,  	100,  60,  2048,      256*1024,  64, 		3,           160*2,        8192*2,         ,     0        	},
	{"MICRON_MT29F1G08ABB",	 0x2CA1, 0x00009580,	1,		25,	10,	25,	25,  	100,  80,  2048,      128*1024,  64, 		2,           80,          4096,             ,     0        	},

	{"INTEL_29F16G08FANB1",  0x89D3, 0x015895D1,0x400002,		25,	10,	25,	25,  	100,  60,  2048,      128*1024,   64, 		3,         160*2,        8192*2,         ,     0        	},
	{"INTEL_29F08G08AAMB1",	 0x89D3, 0x0064A594,	2,		10,	5,	10,	15,  	100,  60,  2048,      256*1024,   64, 		3,         160,          4096,           ,     0        	},
	{"INTEL_29F16G08CAMB1",  0x89D3, 0x0164A594,	2,		10,	5,	10,	15,  	100,  60,  2048,      256*1024,   64, 		3,         320,          8192,           ,     0        	},
	{"INTEL_29F32G08FAMB1",  0x89D5, 0x0168A5D5,0x800002,		10,	5,	10,	15,  	100,  60,  2048,      256*1024,   64, 		3,         320*2,        8192*2,         ,     0        	},
	{"INTEL_29F08G08AAMB2",  0x89D3, 0x0064A594,	2,		10,	5,	8,	15,  	100,  60,  2048,      256*1024,   64, 		3,         160,          4096,           ,     0        	},
	{"INTEL_29F16G08CAMB2",  0x89D3, 0x0164A594,	2,		10,	5,	8,	15,  	100,  60,  2048,      256*1024,   64, 		3,         160*2,        4096*2,         ,     0        	},
	{"INTEL_29F32G08FAMB2",  0x89D5, 0x0168A5D5,0x800002,		10,	5,	8,	15,  	100,  60,  2048,      256*1024,   64, 		3,         320*2,        8192*2,         ,     0        	},

	{"STMICRON_NAND08GW3B2AN6",0x20D3, 0x00009581,	1,		15,	5,	15,	15,   60,   60,  2048,      128*1024,   64, 		3,         160,          8192,           ,     0        	},
	{"STMICRON_NAND08GW3C2AN1",0x20D3, 0x006CA514,	2,		12,	5,	12,	12,  	100,  80,  2048,      256*1024,   64, 		3,         80,          4096,            528,    4        	},
#endif
};

static int get_flash_count(void)
{
	return (sizeof(nand_flash_chips) / sizeof(NAND_FLASH_DEV));
}

static NAND_FLASH_DEV *nand_scan_table(unsigned char *nand_id)
{
	unsigned short dev_id;
	unsigned int ext_id;
	int i, flash_num;
	int index = -1;

	flash_num = get_flash_count();

	dev_id = ((nand_id[0] << 8) | nand_id[1]);
	ext_id = ((nand_id[4] << 16) | (nand_id[3] << 8) | nand_id[2]);
	printf("INFO: Nand Flash dev_id:0x%x ext_id:0x%x\n", dev_id, ext_id);
	/* Lookup the flash id */
	for (i = 0; i < flash_num; i++)
	{
		if ((dev_id == nand_flash_chips[i].id) && (ext_id == (nand_flash_chips[i].extid & 0x00FFFFFF)))
		{
			index = i;
			break;
		}
	}

	if (index == -1)
	{
		printf("DEBUG nand: No NAND Found!\n");
		return 0;
	}

	return &nand_flash_chips[index];
}
static int get_nemc_clk()
{
	unsigned int cpccr = REG_CPM_CPCCR;
	int hb2_div = __cpm_get_h2div();
	int hb2_src = (REG_CPM_CPCCR & CPM_CPCCR_SEL_H2PLL_MASK) >> CPM_CPCCR_SEL_H2PLL_BIT;

	switch (hb2_src){
	case 0:
		printf("the clk stopped !!!\n");
		goto error;
	case 1:
		hb2_src = SCLK_APLL;
		break;
	case 2:
		hb2_src = SCLK_MPLL;
		break;
	case 3:
		printf("not support this mode!!!\n");
		goto error;
	}
	//printf("nemc_clk: hb2_div=%d hb2_src=%d xpll=%d clk=%d \n",
	//       hb2_div,hb2_src,__cpm_get_xpllout(hb2_src),__cpm_get_xpllout(hb2_src) / (hb2_div + 1));
	return  __cpm_get_xpllout(hb2_src) / (hb2_div + 1);
error:
	return -1;
}

static int nand_calc_smcr(NAND_FLASH_DEV *id_info)
{
	unsigned int clk = get_nemc_clk();
	int cycle = 1000000000 / (clk / 1000);  //unit: ps
	int smcr = 0;
	int valume;

	/* NEMC.TAS */
	valume = (id_info->tals * 1000 + cycle - 1) / cycle;
	smcr |= (valume & NEMC_SMCR_TAS_MASK) << NEMC_SMCR_TAS_BIT;
	/* NEMC.TAH */
	valume = (id_info->talh * 1000 + cycle -1) / cycle;
	smcr |= (valume & NEMC_SMCR_TAH_MASK) << NEMC_SMCR_TAH_BIT;
	/* NEMC.TBP */
	valume = (id_info->twp * 1000 + cycle - 1) / cycle;
	smcr |= (valume & NEMC_SMCR_TBP_MASK) << NEMC_SMCR_TBP_BIT;
	/* NEMC.TAW */
	valume = (id_info->trp * 1000 + cycle -1) / cycle;
	smcr |= (valume & NEMC_SMCR_TAW_MASK) << NEMC_SMCR_TAW_BIT;
	/* NEMC.STRV */
	valume = (id_info->trhw * 1000 + cycle - 1) / cycle;
	smcr |= (valume & NEMC_SMCR_STRV_MASK) << NEMC_SMCR_STRV_BIT;

	printf("INFO: hb2clk=%d tals=%d talh=%d twp=%d trp=%d smcr=0x%x\n"
	       , clk,id_info->tals, id_info->talh, id_info->twp, id_info->trp, smcr);
	return smcr;
}

void setup_nandtiming_optimize()
{
	char nandid[NAND_ID_SIZE];
	NAND_FLASH_DEV *idinfo;
	int smcr;

	get_nand_id(nandid);
	idinfo = nand_scan_table(nandid);
	if(!idinfo)
		printf("EEROR: No NAND Found !\n");

	smcr = nand_calc_smcr(idinfo);

#if CFG_NAND_BW8 == 1
	REG_NEMC_SMCR1 = smcr;
#else
	REG_NEMC_SMCR1 = smcr | 0x40;
#endif
}
