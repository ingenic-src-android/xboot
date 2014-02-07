
#ifndef _XBOOT_IMAGE_H_
#define _XBOOT_IMAGE_H_

#include <config.h>
#define XBOOT_MAGIC "X-BOOT!!"
#define XBOOT_MAGIC_SIZE 8
#define XBOOT_NAME_SIZE 16

#ifndef  CONFIG_NAND_X_BOOT
#define XBOOT_PAGE_SIZE 512
#else
#define XBOOT_PAGE_SIZE CFG_NAND_PAGE_SIZE
#endif

struct image_desc
{
	unsigned image_offset; /* in page size from file start*/
	unsigned image_size;
	unsigned image_runaddress;
#define XBOOT_SECOND_BOOT 1
#define XBOOT_SECOND_BOOT_DIGISIGN 2
#define XBOOT_THIRD_BOOT 3
#define XBOOT_THIRD_BOOT_DIGISIGN 4
#define XBOOT_PUBLIC_KEY 5
#define XBOOT_LOGO 6
#define XBOOT_USB_ONLINE_LOGO 7
#define XBOOT_USB_OFFLINE_LOGO 8
#define XBOOT_RECOVERY_LOGO 9
	unsigned image_flag;
	char name[XBOOT_NAME_SIZE];
};

struct xboot_img_hdr
{
    unsigned char magic[XBOOT_MAGIC_SIZE];
    unsigned hdr_version;
    unsigned page_size;    /* every image offset is aligned to page_size */
    unsigned image_count;
};

#ifdef  CONFIG_NAND_X_BOOT
#define SECOND_IMAGE_SECTOR (CFG_NAND_BLOCK_SIZE/CFG_NAND_PAGE_SIZE*2)	/*x-boot is load at 2 block (1 block :128 page) */
#else
#define SECOND_IMAGE_SECTOR (33)
#endif

#define THIRD_IMAGE_SECTOR (1*1024*1024/512)

#endif
