#ifndef MBR_____________INCLUDE
#define MBR_____________INCLUDE
#include <config.h>

#define KBYTE  (1024LL)
#define MBYTE  ((KBYTE)*(KBYTE))

#define LINUX_FS 0x83
#define FAT_FS 0x0b

#define MBR_P1_OFFSET (64*MBYTE)
#define MBR_P1_SIZE (256*MBYTE)
#define MBR_P1_TYPE LINUX_FS

#define MBR_P2_OFFSET (321*MBYTE)
#define MBR_P2_SIZE (1024*MBYTE)
#define MBR_P2_TYPE LINUX_FS

#define MBR_P3_OFFSET (28*MBYTE)
#define MBR_P3_SIZE (30*MBYTE)
#define MBR_P3_TYPE LINUX_FS

#define MBR_P4_OFFSET (1352*MBYTE)
#if defined(CONFIG_MOVINAND_4G)
#define MBR_P4_SIZE (2360*MBYTE)// 4G movinand
#elif defined(CONFIG_MOVINAND_8G)
#define MBR_P4_SIZE (6080*MBYTE)// 8G movinand
#else
#define MBR_P4_SIZE (2360*MBYTE)// 4G movinand
#endif

#define MBR_P4_TYPE FAT_FS

#endif /* MBR_____________INCLUDE */
