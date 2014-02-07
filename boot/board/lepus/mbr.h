#ifndef MBR_____________INCLUDE
#define MBR_____________INCLUDE

#define LINUX_FS 0x83
#define FAT_FS 0x0b

#define MBR_P1_OFFSET 0x8000000
#define MBR_P1_SIZE 0x10000000
#define MBR_P1_TYPE LINUX_FS

#define MBR_P2_OFFSET 0x19000000
#define MBR_P2_SIZE 0x20000000
#define MBR_P2_TYPE LINUX_FS

#define MBR_P3_OFFSET 0x3a400000
#define MBR_P3_SIZE 0x2000000
#define MBR_P3_TYPE LINUX_FS

#define MBR_P4_OFFSET 0x40000000
#define MBR_P4_SIZE 0xa8c00000
#define MBR_P4_TYPE FAT_FS



#endif /* MBR_____________INCLUDE */
