#########################################################################
#
# Makefile for XBoot.
#
# Copyright (C) 2005 - 2009  Ingenic Semiconductor Corp.
#
#########################################################################

SUBDIRS = init common usb nand msc libc lcd board tomsfastmath libtomcrypt third second

#########################################################################
TEXT_BASE := $(X_BOOT_LOAD_ADDR)
export TEXT_BASE

# load other configuration
include $(TOPDIR)/config.mk
-include $(TOPDIR)/include/config.mk

#########################################################################
# X-Boot objects....order is important (i.e. start must be first)

OBJS  = init/init.o
OBJS := $(addprefix $(obj),$(OBJS))

LIBS  = common/libcommon.a
LIBS += ../tools/security/libdata_verify.a
LIBS += usb/libnm.a
LIBS += nand/libnand.a
LIBS += usb/libusb.a
LIBS += libc/libc.a
LIBS += lcd/panels/libpanel.a
LIBS += lcd/liblcd.a
LIBS += board/lib_board.a
ifeq ($(CONFIG_RSA_VERIFY),y)
LIBS += tomsfastmath/libtomsfastmath.a
LIBS += libtomcrypt/libtomcrypt.a
endif
SECOND_LIBS = second/libsecond.a
THIRD_LIBS = third/libthird.a


ifeq ($(TARGET),second)
LIBS += $(SECOND_LIBS)
TYPE = 1
else
LIBS += $(THIRD_LIBS)
TYPE = 3
endif


ifeq ($(CONFIG_MSC_X_BOOT),y)
LIBS += msc/libmsc.a
endif

LIBS := $(addprefix $(obj),$(LIBS))
.PHONY : $(LIBS) mkxbootimg ext_img $(SECOND_LIBS) $(THIRD_LIBS) pack x-boot.bin

__OBJS := $(subst $(obj),,$(OBJS))
__LIBS := $(subst $(obj),,$(LIBS)) ../lib/libgcc.a

#########################################################################
#########################################################################


all:	x-boot.bin

mkxbootimg:
	gcc -O2 -I $(TOPDIR)/include tools/mkxbootimg.c -o mkxbootimg
ext_img:
	gcc -O2 -I $(TOPDIR)/include tools/ext_img.c -o ext_img
pack:
	gcc -g -O2 -I $(TOPDIR)/include tools/pack.c -o pack

$(obj)x-boot.bin:	$(obj)x-boot mkxbootimg
		$(OBJDUMP) -d $< > $(obj)x-boot.S
		$(OBJCOPY) ${OBJCFLAGS} -O binary $< x-boot.bin
		./mkxbootimg --image x-boot.bin  -o x-boot.bin --runaddress $(TEXT_BASE) --type $(TYPE)

$(obj)x-boot:		depend $(SUBDIRS)  $(OBJS) $(LIBS) $(LDSCRIPT)
		$(CC) -mno-abicalls -nostdlib -msoft-float $(LDFLAGS) $(__OBJS) \
			-Wl,--start-group $(__LIBS) -lgcc -Wl,--end-group \
			-o $@

$(OBJS):
		$(MAKE) -C init $(notdir $@)

#$(LIBS) $(SECOND_LIBS) $(THIRD_LIBS):
$(LIBS):
		$(MAKE) -C $(dir $(subst $(obj),,$@))

$(SUBDIRS):
		$(MAKE) -C $@ all

depend:
		for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir _depend ; done

#########################################################################
#########################################################################
#########################################################################

clean:
	find $(OBJTREE) -type f \
		\( -name 'core' -o -name '*.bak' -o -name '*~' \
		-o -name '*.o'  -o -name '*.a' -o -name '.depend' \) -print \
		| xargs rm -f
	rm -f x-boot* *.bin *.S

#########################################################################
