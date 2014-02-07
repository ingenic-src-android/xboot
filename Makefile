#########################################################################
#
# Makefile for XBoot.
#
# Copyright (C) 2005 - 2010  Ingenic Semiconductor Corp.
#
#########################################################################

SUBDIRS = spl boot

#########################################################################

TOPDIR	:= $(shell /bin/pwd)
export TOPDIR

obj :=
src :=
export obj src

CROSS_COMPILE = mipsel-linux-android-
export CROSS_COMPILE

#########################################################################

# load other configuration
ifeq ($(TOPDIR)/include/config.mk,$(wildcard $(TOPDIR)/include/config.mk))
-include $(TOPDIR)/include/config.mk
endif

#########################################################################

COLOR_YELLOW = \033[1;33m
COLOR_LIGHT_BLUE = \033[1;36m
COLOR_TWINKLE = \033[5m
COLOR_END = \033[0m

ifeq ($(CONFIG_JZ4750),y)
CPU_TYPE = JZ4750
endif

ifeq ($(CONFIG_JZ4750L),y)
CPU_TYPE = JZ4750L
endif

ifeq ($(CONFIG_JZ4760),y)
CPU_TYPE = JZ4760
endif

ifeq ($(CONFIG_JZ4760B),y)
CPU_TYPE = JZ4760B
endif

ifeq ($(CONFIG_JZ4760B),y)
CPU_TYPE = JZ4770
endif

ifeq ($(CONFIG_JZ4780),y)
CPU_TYPE = JZ4780
endif

ifeq ($(CONFIG_JZ4775),y)
CPU_TYPE = JZ4775
endif

export CPU_TYPE

#########################################################################
#########################################################################

ifeq ($(CONFIG_NAND_X_BOOT),y)
NAND_SPL = nand-spl.bin
SPL_LOAD_ADDR = 0xf4000800
BOOTTYPE = nand
ALL = x-boot-nand.bin
endif

ifeq ($(CONFIG_MSC_X_BOOT),y)
MSC_SPL = msc-spl.bin

ifneq ($(CONFIG_NOR_SPL),y)
SPL_LOAD_ADDR = 0xf4000a00
else
SPL_LOAD_ADDR = 0xBA000000
endif
BOOTTYPE = msc
ifeq ($(CONFIG_USE_GPT_PARTITIONS),y)
ALL = mbr-xboot-gpt.bin
else
ALL = mbr-xboot.bin
endif
endif

export BOOTTYPE SPL_LOAD_ADDR X_BOOT_LOAD_ADDR

#########################################################################
#########################################################################

ifeq ($(CONFIG_JZ4760_PT701_8),y)
BOARDDIR = pt701_8
endif

export BOARDDIR

#########################################################################
#########################################################################

#X_BOOT = x-boot.bin

MAKE := make

ifeq ($(CONFIG_THREE_STAGE),y)
X_BOOT2 = x-boot2.bin
X_BOOT3 = x-boot3.bin
MBR_XBOOT_DEPEND =
ALL += pack_logo
else
X_BOOT2 = x-boot3.bin
X_BOOT3 =
MBR_XBOOT_DEPEND = pack_logo
endif

ifeq ($(CONFIG_RSA_VERIFY),y)
MBR_XBOOT_DEPEND += pack_public_key
endif

.PHONY: $(ALL) $(MSC_SPL) $(NAND_SPL) pack_public_key pack_logo x-boot2.bin x-boot3.bin
.PHONY:	cyrpto_kit digi_sign mkxbootimg MBR GPT zlib mbr_creater gpt_creater

all:		$(ALL)

$(X_BOOT2) : $(X_BOOT3)

crypto_kit:
		make -C boot mkxbootimg
		make -C boot pack
		make -C boot/tools/crypt/
		cp boot/tools/crypt/sign_boot crypto
		cp boot/tools/crypt/sign_kernel crypto
		cp boot/mkxbootimg crypto
		cp boot/pack crypto

digi_sign:
		cp mbr-xboot.bin crypto
		cp x-boot3.bin crypto
		cd crypto ; ./sign_boot_and_pack.sh

x-boot2.bin:
		X_BOOT_LOAD_ADDR=0x80100000 TARGET=second $(MAKE) -C boot x-boot.bin
		cp boot/x-boot.bin x-boot2.bin

x-boot3.bin:
		X_BOOT_LOAD_ADDR=0x80300000 TARGET=third $(MAKE) -C boot x-boot.bin
		cp boot/x-boot.bin x-boot3.bin

mkxbootimg:
		make -C boot mkxbootimg

pack_logo: x-boot3.bin mkxbootimg
		@./boot/logohandler.sh $< $<

pack_public_key: $(X_BOOT2) mkxbootimg
		./boot/mkxbootimg --image boot/_mklogo.sh --addto $< -o $< --type 5

$(NAND_SPL):
		@echo "\n##########################################################################################$(COLOR_TWINKLE)"
		@echo "$(COLOR_YELLOW)NOTICE:$(COLOR_END)"
		@echo "$(COLOR_LIGHT_BLUE)Make sure you've added the path of Android compiler into PATH$(COLOR_END)"
		@echo "$(COLOR_YELLOW)\nExample:$(COLOR_END)"
		@echo "$(COLOR_LIGHT_BLUE)export PATH=~/ANDROID_ROOT/prebuilts/gcc/linux-x86/mips/mipsel-linux-android-4.6/bin:\$$PATH$(COLOR_END)"
		@echo "##########################################################################################\n"
		$(MAKE) -C spl $@

x-boot-nand.bin: $(NAND_SPL) $(MBR_XBOOT_DEPEND)
		$(MAKE) -C spl pad
ifeq ($(CONFIG_SECURITY_ENABLE),y)
		./tools/security/signatures_bootloader $(X_BOOT2) $(CONFIG_NAND_PAGESIZE) 6
		cat spl/nand-spl-pad.bin $(X_BOOT2).sig > x-boot-nand.bin
		./tools/security/signatures_spl x-boot-nand.bin 0 6
else
		cat spl/nand-spl-pad.bin $(X_BOOT2) > x-boot-nand.bin
endif

$(MSC_SPL):
		@echo "\n##########################################################################################$(COLOR_TWINKLE)"
		@echo "$(COLOR_YELLOW)NOTICE:$(COLOR_END)"
		@echo "$(COLOR_LIGHT_BLUE)Make sure you've added the path of Android compiler into PATH$(COLOR_END)"
		@echo "$(COLOR_YELLOW)\nExample:$(COLOR_END)"
		@echo "$(COLOR_LIGHT_BLUE)export PATH=~/ANDROID_ROOT/prebuilts/gcc/linux-x86/mips/mipsel-linux-android-4.6/bin:\$$PATH$(COLOR_END)"
		@echo "##########################################################################################\n"
		$(MAKE) -C spl $@

mbr-xboot.bin:	$(MSC_SPL) MBR $(MBR_XBOOT_DEPEND)
		$(MAKE) -C spl pad
		cat spl/msc-spl-pad.bin $(X_BOOT2) > x-boot-msc.bin
		cat mbr.bin x-boot-msc.bin > mbr-xboot.bin


mbr-xboot-gpt.bin: $(MSC_SPL) $(MBR_XBOOT_DEPEND) partitions.tab GPT
		$(MAKE) -C spl pad
		@cat spl/msc-spl-pad.bin $(X_BOOT2) > x-boot-msc.bin
		@chmod +x ${PWD}/spl/tools/mk-gpt-xboot.sh
		@${PWD}/spl/tools/mk-gpt-xboot.sh mbr-of-gpt.bin x-boot-msc.bin gpt.bin partitions.tab $@
		@echo ${CL_CYN}"Made xboot image: $@"${CL_RST}

zlib:
		make -C zlib

mbr_creater:
		gcc  spl/tools/mbr_creater/mbr_creater.c -o spl/tools/mbr_creater/mbr_creater -I$(TOPDIR) -I$(TOPDIR)/include

gpt_creater: zlib
		gcc  spl/tools/gpt_creater/gpt_creater.c -o spl/tools/gpt_creater/gpt_creater -I$(TOPDIR) -I$(TOPDIR)/include -I$(TOPDIR)/zlib -L$(TOPDIR)/zlib -lz

MBR: mbr_creater
		spl/tools/mbr_creater/mbr_creater mbr.bin
		@echo ${CL_CYN}"Made mbr: mbr.bin "${CL_RST}

GPT: gpt_creater
		spl/tools/gpt_creater/gpt_creater partitions.tab mbr-of-gpt.bin gpt.bin
		@echo ${CL_CYN}"Made mbr & gpt: mbr-of-gpt.bin & gpt.bin "${CL_RST}

$(LIBS):
		$(MAKE) -C $(dir $(subst $(obj),,$@))

PLL_CAL:
		gcc spl/tools/pll_calc/pll.c -o spl/tools/pll_calc/pllcalc -lm

#########################################################################

unconfig:
	@rm -f $(obj)include/config.h
	@rm -f $(obj)boot/mklogo.sh
	@echo "#!/bin/sh" > boot/mklogo.sh
	@chmod 777 boot/mklogo.sh
	@chmod 777 boot/_mklogo.sh
	@rm -rf ${PWD}/spl/common/jz_serial.c
	@rm -rf ${PWD}/spl/common/cpu.c
	@rm -rf ${PWD}/spl/common/debug.c
	@rm -rf ${PWD}/spl/common/common.c
	@rm -rf spl/tools/mbr_creater/mbr_creater
	@rm -rf spl/tools/mbr_creater/mbr.h
	@rm -rf spl/tools/pll_calc/pllcalc
	@sed -i "s/#define CONFIG_SERIAL_DISABLE 1/#define CONFIG_SERIAL_DISABLE 0/" include/serial.h
	@rm -rf ${PWD}/spl/tools/gpt_creater/gpt_creater

mass_storage_disable_config:
	@sed -i "s/#define CONFIG_NO_MASS_STORAGE 1/#define CONFIG_NO_MASS_STORAGE 0/" include/configs/warrior.h

serial_disable_config:
	@sed -i "s/#define CONFIG_SERIAL_DISABLE 0/#define CONFIG_SERIAL_DISABLE 1/" include/serial.h
	@echo "CONFIG_SERIAL_DISABLE = y" >> include/config.mk

security_enable_config:
	@echo "#define CONFIG_SECURITY_ENABLE" >> include/config.h
	@echo "CONFIG_SECURITY_ENABLE = y" >> include/config.mk
#########################################################################
#########################################################################
dwin_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_TM080SDH01_00 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_DWIN 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
#	@echo "#define CONFIG_ACT8600" >> include/config.h
#	@echo "#define FAST_BOOT_SUPPORT" >> include/config.h
#	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 dwin BOARD"
	@./mkconfig jz4780 dwin msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
#	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_DWIN = y" >> include/config.mk
#	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_online_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_offline_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh recovery_mode_800_480.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

comet_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_HS_DS07012D 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4775_LCDC 1" >> include/config.h
#	@echo "#define CONFIG_LCD_FORMAT_X8B8G8R8 1" >> include/config.h
	@echo "#define CONFIG_JZ4775 1" >> include/config.h
	@echo "#define CONFIG_JZ4775_COMET 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C2_BASE" >> include/config.h
	@echo "#define FAST_BOOT_SUPPORT" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 36468" >> include/config.h
	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4775 comet BOARD"
	@./mkconfig jz4775 comet
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4775 = y" >> include/config.mk
	@echo "CONFIG_JZ4775_COMET = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_LCD_HS_DS07012D = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh jz_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_online_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_offline_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh recovery_mode_800_480.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

test_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_AT070TN93 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_TEST 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 test BOARD"
	@./mkconfig jz4780 test
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_TEST = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "CONFIG_NOR_SPL = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c


printer_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_HSD070IDW1 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_PRINTER 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define NO_CHARGE_DETE" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 printer BOARD"
	@./mkconfig jz4780 printer
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_PRINTER = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh printer_800_480.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c
printer_tf_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_HSD070IDW1 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_PRINTER 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define NO_CHARGE_DETE" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile msc boot image for soc jz4780 printer BOARD"
	@./mkconfig jz4780 printer msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_PRINTER = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c
urboard_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define HDMI_DEVICES_FASTBOOT_SUPPORT" >> include/config.h
#	@echo "#define CONFIG_LCD_KR070LA0S_270 1" >> include/config.h
#	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_URBOARD 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
#	@echo "#define NO_CHARGE_DETE" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
#	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "#define CONFIG_RELEASE_VERSION 0" >>include/config.h
	@echo "#define CONFIG_PRODUCT_NAME \"urboard\"" >>include/config.h
	@echo "Compile nand boot image for soc jz4780 urboard BOARD"
	@./mkconfig jz4780 urboard msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_URBOARD = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

ebook_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_TM080TDH01 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_EBOOK 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
#	@echo "#define NO_CHARGE_DETE" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 17468" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "#define CONFIG_RELEASE_VERSION 0" >>include/config.h
	@echo "#define CONFIG_PRODUCT_NAME \"ebook\"" >>include/config.h
	@echo "Compile nand boot image for soc jz4780 ebook BOARD"
	@./mkconfig jz4780 ebook msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_EBOOK = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

vehicle_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_AT065TN14 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_VEHICLE 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
#	@echo "#define NO_CHARGE_DETE" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 17468" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "#define CONFIG_RELEASE_VERSION 0" >>include/config.h
	@echo "#define CONFIG_PRODUCT_NAME \"vehicle\"" >>include/config.h
	@echo "Compile nand boot image for soc jz4780 vehicle BOARD"
	@./mkconfig jz4780 vehicle msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_VEHICLE = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

warrior_nand_config: unconfig mass_storage_disable_config
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KR070LA0S_270 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_WARRIOR 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define FAST_BOOT_SUPPORT" >> include/config.h
	@echo "#define NO_CHARGE_DETE" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "#define CONFIG_RELEASE_VERSION 0" >>include/config.h
	@echo "#define CONFIG_PRODUCT_NAME \"CRUZ_I800\"" >>include/config.h
	@echo "Compile nand boot image for soc jz4780 warrior BOARD"
	@./mkconfig jz4780 warrior msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_WARRIOR = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_online_1024_600.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_offline_1024_600.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh recovery_mode_1024_600.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

warrior_release_config: unconfig warrior_nand_config
	@sed -i -r "s/CONFIG_RELEASE_VERSION 0/CONFIG_RELEASE_VERSION 1/" include/config.h

warrior_nand_se_config: warrior_nand_config security_enable_config serial_disable_config
	@echo "CONFIG_NAND_PAGESIZE = 4096" >> include/config.mk

trooper_config: warrior_nand_config
	@sed -i -r "s/CONFIG_NO_MASS_STORAGE 0/CONFIG_NO_MASS_STORAGE 1/" include/configs/warrior.h

cuckoo_config: npm801_nand_config
	@sed -i -r "s/CONFIG_NO_MASS_STORAGE 0/CONFIG_NO_MASS_STORAGE 1/" include/configs/npm801.h

grus_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_BYD_BM8766U 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_GRUS 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define FAST_BOOT_SUPPORT" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 grus BOARD"
	@./mkconfig jz4780 grus msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_GRUS = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_online_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_offline_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh recovery_mode_800_480.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

grus_nand_v101_config: unconfig grus_nand_config
	@sed -i -r "s/CONFIG_ACT8600/CONFIG_RICOH618/" include/config.h
	@sed -i -r "s/CONFIG_ACT8600/CONFIG_RICOH618/" include/config.mk


grus_msc_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KD50G2_40NM_A2 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_GRUS 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC0_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 grus BOARD"
	@./mkconfig jz4780 grus msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_GRUS = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

grus_msc_lpddr2_config: unconfig grus_msc_config
	@sed -i -r "s/CONFIG_SDRAM_DDR3/COFIG_SDRAM_LPDDR2/" include/config.h
	@sed -i -r "s/CONFIG_USE_DDR3/CONFIG_USE_LPDDR2/" include/config.mk

hdmi_80_tf_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
#	@echo "#define CONFIG_LCD_HSD101PWW1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_HDMI_80 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC0 1" >> include/config.h
	@echo "#define CONFIG_ANDROID_LCD_HDMI_DEFAULT 1" >> include/config.h
	@echo "#define CONFIG_FORCE_RESOLUTION 4" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 415744" >> include/config.h
	@echo "Compile TF (MSC1) boot image for soc jz4780 hdmi_80 BOARD"
	@./mkconfig jz4780 hdmi_80 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_HDMI_80 = y" >> include/config.mk
	@echo "CONFIG_HDMI_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh Dongle_1280_720.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

vga_tf_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_720P 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_HDMI_80 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
#	@echo "#define CONFIG_ANDROID_LCD_HDMI_DEFAULT 1" >> include/config.h
	@echo "#define CONFIG_FORCE_RESOLUTION 4" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 415744" >> include/config.h
	@echo "Compile TF (MSC1) boot image for soc jz4780 hdmi_80 BOARD"
	@./mkconfig jz4780 hdmi_80 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_HDMI_80 = y" >> include/config.mk
	@echo "CONFIG_HDMI_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh Dongle_1280_720.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

grus_msc1_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KD50G2_40NM_A2 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_AT070TN93 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_GRUS 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile MSC boot image for soc jz4780 grus BOARD"
	@./mkconfig jz4780 grus msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_GRUS = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

test_msc_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_AT070TN93 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_TEST 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_SD" >> include/config.h
	@echo "Compile MSC boot image for soc jz4780 test BOARD"
	@./mkconfig jz4780 test msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_TEST = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "CONFIG_NOR_SPL = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

hdmi_80_nand_for_fighter_config: unconfig
	   @echo "#define CONFIG_BOOT_ZIMAGE_KERNEL" > include/config.h
	   @echo "#define CONFIG_NAND_X_BOOT" >> include/config.h
	#      @echo "#define CONFIG_ANDROID_LCD_TFT_AT070TN93 1" >> include/config.h
	   @echo "#define CONFIG_JZ4780 1" >> include/config.h
	   @echo "#define CONFIG_JZ4780_HDMI_80 1" >> include/config.h
	   @echo "#define CONFIG_FB_JZ4780_LCDC0 1" >> include/config.h
	   @echo "#define CONFIG_ANDROID_LCD_HDMI_DEFAULT 1" >> include/config.h
	   @echo "#define CONFIG_FORCE_RESOLUTION 4" >> include/config.h
	   @echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	#   @echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	#   @echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	   @echo "Compile nand boot image for soc jz4780 hdmi_80 BOARD"
	   @./mkconfig jz4780 hdmi_80 msc
	   @echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	   @echo "CONFIG_JZ4780 = y" >> include/config.mk
	   @echo "CONFIG_JZ4780_HDMI_80 = y" >> include/config.mk
	   @echo "CONFIG_HDMI_JZ4780 = y" >> include/config.mk
	#      @echo "CONFIG_ACT8600 = y" >> include/config.mk
	   @echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	   @echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	   @ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	   @ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	   @ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	   @ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

hdmi_80_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
#	@echo "#define CONFIG_ANDROID_LCD_TFT_AT070TN93 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_HDMI_80 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC0 1" >> include/config.h
	@echo "#define CONFIG_ANDROID_LCD_HDMI_DEFAULT 1" >> include/config.h
	@echo "#define CONFIG_FORCE_RESOLUTION 4" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 415744" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
#	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 hdmi_80 BOARD"
	@./mkconfig jz4780 hdmi_80 msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_HDMI_80 = y" >> include/config.mk
	@echo "CONFIG_HDMI_JZ4780 = y" >> include/config.mk
#	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh Dongle_1280_720.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

vga_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_720P 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_HDMI_80 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
#	@echo "#define CONFIG_ANDROID_LCD_HDMI_DEFAULT 1" >> include/config.h
	@echo "#define CONFIG_FORCE_RESOLUTION 4" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 415744" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
#	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 hdmi_80 BOARD"
	@./mkconfig jz4780 hdmi_80 msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_HDMI_80 = y" >> include/config.mk
#	@echo "CONFIG_HDMI_JZ4780 = y" >> include/config.mk
#	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh Dongle_1280_720.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

m80_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	# @echo "#define CONFIG_LCD_HSD101PWW1 1" >> include/config.h
	@echo "#define CONFIG_LCD_LP101WX1_SLN2 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_M80 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 m80 BOARD"
	@./mkconfig jz4780 m80 msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_M80 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh hdmi_1280_720.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

m80_tsd_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_AT070TN93 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_M80 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile TSD (MSC0) boot image for soc jz4780 m80 BOARD"
	@./mkconfig jz4780 m80 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_M80 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

m80_tf_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_LP101WX1_SLN2 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_M80 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile TF (MSC1) boot image for soc jz4780 m80 BOARD"
	@./mkconfig jz4780 m80 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_M80 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh hdmi_1280_720.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

ji8070a_tf_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_HSD070IDW1 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_JI8070A 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile TF (MSC1) boot image for soc jz4780 ji8070a BOARD"
	@./mkconfig jz4780 ji8070a msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_JI8070A = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

zpad80_tf_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_HSD070IDW1 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_ZPAD80 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile TF (MSC1) boot image for soc jz4780 ji8070a BOARD"
	@./mkconfig jz4780 zpad80 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_ZPAD80 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

ji8070a_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_HSD070IDW1 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_JI8070A 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 ji8070a BOARD"
	@./mkconfig jz4780 ji8070a msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_JI8070A = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

zpad80_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_HSD070IDW1 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_ZPAD80 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 ji8070a BOARD"
	@./mkconfig jz4780 zpad80 msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_ZPAD80 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c


ji8070b_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_HHX070ML208CP21 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_JI8070B 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 ji8070b BOARD"
	@./mkconfig jz4780 ji8070b msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_JI8070B = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c
ji8070b_tf_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_HHX070ML208CP21 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_JI8070B 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile TF (MSC1) boot image for soc jz4780 ji8070b BOARD"
	@./mkconfig jz4780 ji8070b msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_JI8070B = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

q8_nand_for_fighter_config: unconfig
	@echo "#define CONFIG_BOOT_ZIMAGE_KERNEL" > include/config.h
	@echo "#define CONFIG_NAND_X_BOOT" >> include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_HSD070IDW1 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_Q8 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 q8 BOARD"
	@./mkconfig jz4780 q8 msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_Q8 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

q8a_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_HSD070IDW1 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_Q8 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 q8 BOARD"
	@./mkconfig jz4780 q8 msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_Q8 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c
q8b_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_HHX070ML208CP21 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_Q8 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 q8 BOARD"
	@./mkconfig jz4780 q8 msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_Q8 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c
q8a_tf_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_HSD070IDW1 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_Q8 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile TF (MSC1) boot image for soc jz4780 ji8070a BOARD"
	@./mkconfig jz4780 q8 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_Q8 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c
q8b_tf_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_HHX070ML208CP21 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_Q8 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile TF (MSC1) boot image for soc jz4780 ji8070a BOARD"
	@./mkconfig jz4780 q8 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_Q8 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c
m80701_tsd_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_AT070TN93 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_M80701 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile TSD (MSC0) boot image for soc jz4780 m80701 BOARD"
	@./mkconfig jz4780 m80701 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_M80701 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

m80701_tf_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_AT070TN93 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_M80701 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile TF (MSC1) boot image for soc jz4780 m80701 BOARD"
	@./mkconfig jz4780 m80701 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_M80701 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

warrior_msc_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KR070LA0S_270 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_WARRIOR 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define NO_CHARGE_DETE" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile MSC boot image for soc jz4780 warrior BOARD"
	@./mkconfig jz4780 warrior msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_WARRIOR = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

urboard_msc1_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_URBOARD 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "Compile MSC boot image for soc jz4780 urboard BOARD"
	@./mkconfig jz4780 urboard msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_URBOARD = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

warrior_msc1_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KR070LA0S_270 1" >> include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_AT070TN93 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_WARRIOR 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile MSC boot image for soc jz4780 warrior BOARD"
	@./mkconfig jz4780 warrior msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_JZ4780_WARRIOR = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

npm3701_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
#	@echo "#define CONFIG_LCD_KR070LA0S_270 1" >> include/config.h
	@echo "#define CONFIG_LCD_S369FG06 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_NPM3701 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define FAST_BOOT_SUPPORT" >> include/config.h
	@echo "#define NO_CHARGE_DETE" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "#define CONFIG_RELEASE_VERSION 0" >>include/config.h
	@echo "#define CONFIG_PRODUCT_NAME \"npm3701\"" >>include/config.h
	@echo "Compile nand boot image for soc jz4780 npm3701 BOARD"
	@./mkconfig jz4780 npm3701 msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_NPM3701 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
#	@echo "./_mklogo.sh novo7_1.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh nopa_480.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c


npm709j_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KD50G2_40NM_A2 1" >> include/config.h
#	@echo "#define CONFIG_LCD_KR080LA4S_250 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_NPM709J 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define FAST_BOOT_SUPPORT" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 npm709j BOARD"
	@./mkconfig jz4780 npm709j msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_NPM709J = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh jz_800_480.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

npm801_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KR080LA4S_250 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_NPM801 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define FAST_BOOT_SUPPORT" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "#define CONFIG_PRODUCT_NAME \"npm801\"" >>include/config.h
	@echo "Compile nand boot image for soc jz4780 npm801 BOARD"
	@./mkconfig jz4780 npm801 msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_NPM801 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh jz_1024_768.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_online_1024_768.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_offline_1024_768.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh recovery_mode_1024_768.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

t700d_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_SL007DC18B05 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_NPM801 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_NPM801_T700D 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 npm801 BOARD"
	@./mkconfig jz4780 npm801 msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_NPM801 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_NPM801_T700D = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh jz_1024_768.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

leaf_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_JCMT070T115A18 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_NPM801 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_NPM801_LEAF 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 npm801 BOARD"
	@./mkconfig jz4780 npm801 msc
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_NPM801 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_NPM801_LEAF = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh jz_800_480.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c


npm801_msc1_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KR080LA4S_250 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_NPM801 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 124568" >> include/config.h
	@echo "#define CONFIG_MSC_BURN" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile MSC boot image for soc jz4780 npm801 BOARD"
	@./mkconfig jz4780 npm801 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_MSC_BURN = y" >> include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_JZ4780_NPM801 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh jz_1024_768.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

i88_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KR080LA4S_250 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_I88 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 36468" >> include/config.h
	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4780 i88 BOARD"
	@./mkconfig jz4780 i88
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_I88 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh jz_1024_768.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

i88_burner_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KR080LA4S_250 1" >> include/config.h
	@echo "#define CONFIG_ANDROID_LCD_TFT_AT070TN93 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC1 1" >> include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_I88 1" >> include/config.h
	@echo "#define CONFIG_MKBURNER 1" >> include/config.h
	@echo "#define CONFIG_CAPACITY 1700" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C1_BASE" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 36468" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile MSC boot image for soc jz4780 i88 BOARD"
	@./mkconfig jz4780 i88 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_JZ4780_I88 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh jz_1024_768.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

mensa_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_BM800480_8766FTGU 1" >> include/config.h
#	@echo "#define CONFIG_LCD_KFM701A21_1A	1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4775_LCDC 1" >> include/config.h
#	@echo "#define CONFIG_LCD_FORMAT_X8B8G8R8 1" >> include/config.h
	@echo "#define CONFIG_JZ4775 1" >> include/config.h
	@echo "#define CONFIG_JZ4775_MENSA 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C2_BASE" >> include/config.h
	@echo "#define FAST_BOOT_SUPPORT" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 36468" >> include/config.h
	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4775 mensa BOARD"
	@./mkconfig jz4775 mensa
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4775 = y" >> include/config.mk
	@echo "CONFIG_JZ4775_MENSA = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_LCD_BM800480_8766FTGU = y" >> include/config.mk
#	@echo "CONFIG_LCD_KFM701A21_1A = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh jz_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_online_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_offline_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh recovery_mode_800_480.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

mensa_msc_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_BM800480_8766FTGU 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4775_LCDC 1" >> include/config.h
#	@echo "#define CONFIG_LCD_FORMAT_X8B8G8R8 1" >> include/config.h
	@echo "#define CONFIG_JZ4775 1" >> include/config.h
	@echo "#define CONFIG_JZ4775_MENSA 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C2_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 36468" >> include/config.h
	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile msc boot image for soc jz4775 mensa BOARD"
	@./mkconfig jz4775 mensa msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4775 = y" >> include/config.mk
	@echo "CONFIG_JZ4775_MENSA = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_LCD_BM800480_8766FTGU = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh jz_800_480.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

mensa_msc1_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_BM800480_8766FTGU 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4775_LCDC 1" >> include/config.h
#	@echo "#define CONFIG_LCD_FORMAT_X8B8G8R8 1" >> include/config.h
	@echo "#define CONFIG_JZ4775 1" >> include/config.h
	@echo "#define CONFIG_JZ4775_MENSA 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C2_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 36468" >> include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC1_BOOT" >> include/config.h
	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile msc boot image for soc jz4775 mensa BOARD"
	@./mkconfig jz4775 mensa msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4775 = y" >> include/config.mk
	@echo "CONFIG_JZ4775_MENSA = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_LCD_BM800480_8766FTGU = y" >> include/config.mk
	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh jz_800_480.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

k101_nand_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_BM800480_8766FTGU 1" >> include/config.h
#	@echo "#define CONFIG_LCD_KFM701A21_1A	1" >> include/config.h
#	@echo "#define CONFIG_FB_JZ4775_LCDC 1" >> include/config.h
#	@echo "#define CONFIG_LCD_FORMAT_X8B8G8R8 1" >> include/config.h
	@echo "#define CONFIG_JZ4775 1" >> include/config.h
	@echo "#define CONFIG_JZ4775_K101 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_MDDR" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C2_BASE" >> include/config.h
	@echo "#define FAST_BOOT_SUPPORT" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 36468" >> include/config.h
#	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
#	@echo "#define CONFIG_XBOOT_LOW_BATTERY_DETECT" >> include/config.h
	@echo "Compile nand boot image for soc jz4775 k101 BOARD"
	@./mkconfig jz4775 k101
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4775 = y" >> include/config.mk
	@echo "CONFIG_JZ4775_K101 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_LCD_BM800480_8766FTGU = y" >> include/config.mk
#	@echo "CONFIG_LCD_KFM701A21_1A = y" >> include/config.mk
	@echo "CONFIG_USE_MDDR = y" >> include/config.mk
	@echo "./_mklogo.sh jz_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_online_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh usb_offline_800_480.rle" >> boot/mklogo.sh
	@echo "./_mklogo.sh recovery_mode_800_480.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

ry1000_tf_config: unconfig ry1000_msc_config
	@sed -i -r "s/CONFIG_MSC0_BOOT/CONFIG_MSC1_BOOT" include/config.h

ry1000_msc_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_JZ4780 1" >> include/config.h
	@echo "#define CONFIG_JZ4780_RY1000 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4780_LCDC0 1" >> include/config.h
	@echo "#define CONFIG_ANDROID_LCD_HDMI_DEFAULT 1" >> include/config.h
	@echo "#define CONFIG_FORCE_RESOLUTION 4" >> include/config.h
	@echo "#define COFIG_SDRAM_LPDDR2" >> $(obj)include/config.h
#	@echo "#define CONFIG_SDRAM_DDR3" >> $(obj)include/config.h
	@echo "#define CONFIG_MSC_TYPE_MMC" >> include/config.h
	@echo "#define CONFIG_MSC0_BOOT" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE_LEN 415744" >> include/config.h
	@echo "Compile MSC0 boot image for soc jz4780 ry1000 BOARD"
	@./mkconfig jz4780 ry1000 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_JZ4780_RY1000 = y" >> include/config.mk
	@echo "CONFIG_HDMI_JZ4780 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_USE_LPDDR2 = y" >> include/config.mk
#	@echo "CONFIG_USE_DDR3 = y" >> include/config.mk
	@echo "./_mklogo.sh Dongle_1280_720.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

cone_emcp_config: unconfig
	@echo "#define CONFIG_NAND_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KD301_M03545_0317A 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4775_LCDC 1" >> include/config.h
	@echo "#define CONFIG_JZ4775 1" >> include/config.h
	@echo "#define CONFIG_JZ4775_CONE 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_MDDR" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C0_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
	@echo "Compile nand boot image for soc jz4775 CONE BOARD"
	@./mkconfig jz4775 cone
	@echo "CONFIG_NAND_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4775 = y" >> include/config.mk
	@echo "CONFIG_JZ4775_CONE = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_LCD_KD301_M03545_0317A = y" >> include/config.mk
	@echo "CONFIG_USE_MDDR = y" >> include/config.mk
	@echo "./_mklogo.sh jz_watchdata.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c

cone_v1_2_emcp_config: unconfig
	@echo "#define CONFIG_MSC_X_BOOT" > include/config.h
	@echo "#define CONFIG_LCD_KD301_M03545_0317A 1" >> include/config.h
	@echo "#define CONFIG_FB_JZ4775_LCDC 1" >> include/config.h
	@echo "#define CONFIG_JZ4775 1" >> include/config.h
	@echo "#define CONFIG_JZ4775_CONE_V1_2 1" >> include/config.h
	@echo "#define CONFIG_SDRAM_MDDR" >> $(obj)include/config.h
	@echo "#define CONFIG_ACT8600" >> include/config.h
	@echo "#define I2C_BASE I2C0_BASE" >> include/config.h
	@echo "#define CONFIG_XBOOT_POWERON_LONG_PRESSED 1" >> include/config.h
	@echo "#define CONFIG_XBOOT_LOGO_FILE 1" >> include/config.h
	@echo "#define CONFIG_HAVE_CHARGE_LOGO" >> include/config.h
	@echo "Compile msc boot image for soc jz4775 CONE_V1_2 BOARD"
	@./mkconfig jz4775 cone_v1_2 msc
	@echo "CONFIG_MSC_X_BOOT = y" > include/config.mk
	@echo "CONFIG_JZ4775 = y" >> include/config.mk
	@echo "CONFIG_JZ4775_CONE_V1_2 = y" >> include/config.mk
	@echo "CONFIG_ACT8600 = y" >> include/config.mk
	@echo "CONFIG_POWER_MANAGEMENT = y" >> include/config.mk
	@echo "CONFIG_LCD_KD301_M03545_0317A = y" >> include/config.mk
	@echo "CONFIG_USE_MDDR = y" >> include/config.mk
	@echo "./_mklogo.sh jz_watchdata.rle" >> boot/mklogo.sh
	@ln -s ${PWD}/boot/common/jz_serial.c ${PWD}/spl/common/jz_serial.c
	@ln -s ${PWD}/boot/common/cpu.c ${PWD}/spl/common/cpu.c
	@ln -s ${PWD}/boot/common/debug.c ${PWD}/spl/common/debug.c
	@ln -s ${PWD}/boot/common/common.c ${PWD}/spl/common/common.c	

release_config:
	echo "CONFIG_RELEASE = y" >> include/config.mk


#########################################################################
#########################################################################
#########################################################################

clean:
	$(MAKE) -C spl $@
	$(MAKE) -C boot $@
	$(MAKE) -C tools/security $@
	rm -f $(ALL)
	rm -rf ${PWD}/spl/common/jz_serial.c
	rm -rf ${PWD}/spl/common/cpu.c
	rm -rf ${PWD}/spl/common/debug.c
	rm -rf ${PWD}/spl/common/common.c
	rm -rf $(PWD)/include/config.mk
	rm -f *.bin
	rm -f *.sig
	rm -f crypto/*.bin
	@sed -i "s/#define CONFIG_SERIAL_DISABLE 1/#define CONFIG_SERIAL_DISABLE 0/" include/serial.h

distclean: clean
	rm -f crypto/*.key boot/public.binary boot/pack
	rm -f crypto/mkxbootimg crypto/pack crypto/sign_boot crypto/sign_kernel

########################################################################
