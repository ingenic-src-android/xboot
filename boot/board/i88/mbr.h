#ifndef __MBR_H__
#define __MBR_H__
#include <config.h>

#define KBYTE  (1024LL)
#define MBYTE  ((KBYTE)*(KBYTE))

#define LINUX_FS 0x83
#define FAT_FS 0x0b

#if defined(CONFIG_MKBURNER)
/* burner */
#define MBR_P1_OFFSET (64*MBYTE)
#define MBR_P1_SIZE (CONFIG_CAPACITY*MBYTE)
#define MBR_P1_TYPE FAT_FS

#else
/* cache */
#define MBR_P3_OFFSET (28*MBYTE)
#define MBR_P3_SIZE (30*MBYTE)
#define MBR_P3_TYPE LINUX_FS

/* system */
#define MBR_P1_OFFSET (64*MBYTE)
#define MBR_P1_SIZE (512*MBYTE)
#define MBR_P1_TYPE LINUX_FS

/* data */
#define MBR_P2_OFFSET (580*MBYTE)
#define MBR_P2_SIZE (1024*MBYTE)
#define MBR_P2_TYPE LINUX_FS

/* vfat */
#define MBR_P4_OFFSET (1606*MBYTE)
#define MBR_P4_SIZE (6200*MBYTE)
#define MBR_P4_TYPE FAT_FS

#endif

#endif /* __MBR_H__ */
