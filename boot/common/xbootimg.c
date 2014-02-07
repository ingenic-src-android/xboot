#include <config.h>
#include <xbootimg.h>
#include <serial.h>
#include <stdlib.h>

#if defined(CONFIG_NAND_X_BOOT)
int *do_nand(int offs, int xboot_size, unsigned char *dst);
#define do_msc(addr, off, size) do_nand(off, size, addr);
#else
int do_msc(unsigned long addr, unsigned long off, unsigned long size);
#endif

int get_image(unsigned char *hdr, int image_flag, unsigned image_base, unsigned char **image, unsigned *image_size)
{
	struct xboot_img_hdr *header = (struct xboot_img_hdr *)hdr;
	struct image_desc *desc;
	int max_des = header->image_count;
	int i;
	for (i=0; i<max_des; i++) {
		desc = (struct image_desc*)(hdr + sizeof(struct xboot_img_hdr) + i*(sizeof(struct image_desc)));
		if (desc->image_flag == image_flag) {
			unsigned image_offset = image_base*XBOOT_PAGE_SIZE + desc->image_offset*XBOOT_PAGE_SIZE;
			unsigned size = (desc->image_size+XBOOT_PAGE_SIZE) & ~(XBOOT_PAGE_SIZE -1);
  			*image = malloc(size);
			*image_size = desc->image_size;
			do_msc((unsigned long )*image, image_offset, size);
			return 0;
		}
	}
	serial_puts_info("Cannot find image...\n");
	return -1;
}


