#########################################################################
#
# Makefile for XBoot.
#
# Copyright (C) 2005 - 2009  Ingenic Semiconductor Corp.
#
#########################################################################

SUBDIRS = init common msc nand

#########################################################################
TEXT_BASE := $(SPL_LOAD_ADDR)
export TEXT_BASE

# load other configuration
include $(TOPDIR)/config.mk
-include $(TOPDIR)/include/config.mk

#########################################################################
# X-Boot objects....order is important (i.e. start must be first)

OBJS  = init/init.o
OBJS := $(addprefix $(obj),$(OBJS))

LIBS  = common/libcommon.a

ifeq ($(CONFIG_MSC_X_BOOT),y)
LIBS += msc/libmsc_boot.a
endif

ifeq ($(CONFIG_NAND_X_BOOT),y)
LIBS += ../tools/security/libdata_verify.a
LIBS += nand/libnand_boot.a
endif

LIBS := $(addprefix $(obj),$(LIBS))
.PHONY : $(LIBS)

__OBJS := $(subst $(obj),,$(OBJS))
__LIBS := $(subst $(obj),,$(LIBS))

#########################################################################
#########################################################################

ifeq ($(CONFIG_MSC_X_BOOT),y)
all:	msc-spl.bin
endif

ifeq ($(CONFIG_NAND_X_BOOT),y)
all:	nand-spl.bin
endif

$(obj)nand-spl.bin:	$(obj)nand-spl
		$(OBJDUMP) -Dz $< > $(obj)nand-spl.dump
		$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@
		cp nand-spl.bin $(TOPDIR)

$(obj)nand-spl:		depend $(SUBDIRS) $(OBJS) $(LIBS) $(LDSCRIPT)
		$(CC) -nostdlib -mno-abicalls $(LDFLAGS) $(__OBJS) \
			-Wl,--start-group $(__LIBS) -Wl,--end-group \
			-o $@

$(obj)msc-spl.bin:	$(obj)msc-spl
		$(OBJDUMP) -Dz $< > $(obj)msc-spl.dump
		$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@
		cp msc-spl.bin $(TOPDIR)

$(obj)msc-spl:		depend $(SUBDIRS) $(OBJS) $(LIBS) $(LDSCRIPT)
		$(CC) -nostdlib -mno-abicalls $(LDFLAGS) $(__OBJS) \
			-Wl,--start-group $(__LIBS) -Wl,--end-group \
			-o $@

$(OBJS):
		$(MAKE) -C init $(notdir $@)

$(LIBS):
		$(MAKE) -C $(dir $(subst $(obj),,$@))

$(SUBDIRS):
		$(MAKE) -C $@ all

pad:
		tools/$(BOOTTYPE)-spl-pad.sh $(CPU_TYPE)

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
	rm -f nand-spl nand-spl.bin nand-spl.dump nand-spl-pad.bin $(TOPDIR)/nand-spl.bin
	rm -f msc-spl msc-spl.bin msc-spl.dump msc-spl-pad.bin $(TOPDIR)/msc-spl.bin

#########################################################################
