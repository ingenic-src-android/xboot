#ifndef MBR_____________INCLUDE
#define MBR_____________INCLUDE

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
//#define MBR_P4_SIZE (2421*MBYTE)// 4G movinand
#define MBR_P4_SIZE (6090*MBYTE)// 8G movinand
#define MBR_P4_TYPE FAT_FS

#endif /* MBR_____________INCLUDE */
