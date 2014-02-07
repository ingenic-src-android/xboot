#include <config.h>
#include <libc.h>
#include <boot_msg.h>
#include <boot.h>
#include <nand.h>
#include <serial.h>
#include <debug.h>


int get_bootloader_message(struct bootloader_message *out)
{
	unsigned char data[CFG_NAND_PAGE_SIZE];

	memset(data, '\0', CFG_NAND_PAGE_SIZE);
	do_nand (PTN_MISC_OFFSET, CFG_NAND_PAGE_SIZE, data);
   	memcpy(out, data, sizeof(struct bootloader_message));

	return 0;
}

int set_bootloader_message(const struct bootloader_message *in)
{
	unsigned char data[CFG_NAND_PAGE_SIZE];

	memset(data, '\0', CFG_NAND_PAGE_SIZE);
	memcpy(data, in, sizeof(struct bootloader_message));

	/* Clear MISC partition, and write bootloader_message. */
	if (nand_erase_block(64) || nand_erase_block(64 + 1)) {
		serial_puts_info("NAND erase failed!!!\n");
		return 1;
	}

	if (nand_program_page(data, PTN_MISC_OFFSET / CFG_NAND_PAGE_SIZE)) {
		serial_puts_info("NAND program failed !!!\n");
		return 1;
	}

	serial_puts_info("set_bootloader_message finish ...\n");

	return 0;
}

void msg_test(void)//struct bootloader_message *msg)
{
	unsigned char data[CFG_NAND_PAGE_SIZE];


	// clear data array ...
	if (nand_erase_block(64) || nand_erase_block(64 + 1))
		serial_puts_info("NAND erase failed !!!\n");


	if (nand_program_page(data, 8192))
		serial_puts_info("NAND program failed !!!\n");

	serial_puts_info("msg_test finish.\n");
}
