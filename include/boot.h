#ifndef _BOOT_H
#define _BOOT_H


/*-------------------------------------------------------------------
 * For X-Boot, boot select.
 */
#define FAST_BOOT			0
#define NORMAL_BOOT			1
#define RECOVERY_BOOT			2
#define PRETEST_BOOT                    3
#define FASTBOOT_RECOVERY_BOOT          4

#define UPDATE_XBOOT			0
#define UPDATE_RADIO			1

/*-------------------------------------------------------------------
 * For Nand SPL, to load X-Boot
 */
#define CFG_X_BOOT_DST			0x80100000		/* Load NUB to this addr	*/
#define CFG_X_BOOT_START		CFG_X_BOOT_DST	/* Start NUB from this addr	*/
#define CFG_X_BOOT_OFFS			(32 << 10)		/* Offset of X-Boot		*/
#define CFG_X_BOOT_SIZE			(512<< 10)		/* Size of RAM X-Boot image	*/

/*-------------------------------------------------------------------
 * For X-Boot, to load usbloader or boot.img or recovery.img
 */
#define CFG_KERNEL_DST			0x80f00000		    /* Load Linux kernel to this addr	*/
#define CFG_KERNEL_START		CFG_KERNEL_DST		/* Start Linux kernel from this addr	*/
#define CFG_RAMDISK_DST			0x81a00000          /* initrd address */
#define CFG_BOOT_OFFS			PTN_BOOT_OFFSET		/* Offset of boot.img			*/
#define CFG_RECOVERY_OFFS		PTN_RECOVERY_OFFSET	/* Offset of recovery.img		*/
#define CFG_PRETEST_OFFS		PTN_PRETEST_OFFSET	/* Offset of pretest.img		*/
#define CFG_BOOT_SIZE			PTN_BOOT_SIZE		/* Size of Linux kernel			*/
#define CFG_RECOVERY_SIZE		PTN_RECOVERY_SIZE	/* Size of Linux kernel			*/
#define CFG_PRETEST_SIZE		PTN_PRETEST_SIZE	/* Size of Linux kernel			*/

#endif
