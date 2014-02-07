#ifndef MBR_____________INCLUDE
#define MBR_____________INCLUDE
#include <config.h>

#define KBYTE  (1024LL)
#define MBYTE  ((KBYTE)*(KBYTE))

#define LINUX_FS 0x83
#define FAT_FS 0x0b


/* system */
#define MBR_P1_OFFSET (38*MBYTE)
#define MBR_P1_SIZE (305*MBYTE)
#define MBR_P1_TYPE LINUX_FS

/* data */
#define MBR_P2_OFFSET (363*MBYTE)
#define MBR_P2_SIZE (138*MBYTE)
#define MBR_P2_TYPE LINUX_FS

/* cache */
#define MBR_P3_OFFSET (343*MBYTE)
#define MBR_P3_SIZE (20*MBYTE)
#define MBR_P3_TYPE LINUX_FS

/* vfat */
#define MBR_P4_OFFSET (501*MBYTE)
#define MBR_P4_SIZE (8*MBYTE)
#define MBR_P4_TYPE FAT_FS

#endif /* MBR_____________INCLUDE */
