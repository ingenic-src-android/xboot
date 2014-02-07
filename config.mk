
#
# Include the make variables (CC, etc...)
#
CROSS_COMPILE = mipsel-linux-android-

AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CPP	= $(CC) -E
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)RANLIB

ARFLAGS = crv
OPTFLAGS= -Os

ifndef LDSCRIPT
LDSCRIPT := $(TOPDIR)/x-boot.lds
endif

OBJCFLAGS += --gap-fill=0xff

gccincdir := $(shell $(CC) -print-file-name=include)

CPPFLAGS = $(OPTFLAGS) -DTEXT_BASE=$(TEXT_BASE) -DLTC_NO_FILE -DLTC_NO_WCHAR -DARGTYPE=3 -DLTC_SOURCE -DTFM_DESC -DUSE_TFM -g


CPPFLAGS += -I$(TOPDIR)/include
CPPFLAGS += -I$(TOPDIR)/include/libc
CPPFLAGS += -I$(TOPDIR)/include/lcd_configs
CPPFLAGS += -I$(TOPDIR)/boot/tomsfastmath/src/headers -I$(TOPDIR)/boot/libtomcrypt/src/headers/
CPPFLAGS += -isystem $(gccincdir) -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs \
	 -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration \
	-O2 -ffunction-sections -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe \
	-msoft-float -ffreestanding -march=mips32 -EL -Wall

CFLAGS = $(CPPFLAGS)

AFLAGS := -D__ASSEMBLY__ $(CPPFLAGS)

#LDFLAGS += -Bstatic -T $(LDSCRIPT) -Ttext $(TEXT_BASE) -EL
LDFLAGS += -Wl,-Bstatic -Wl,-T,$(LDSCRIPT) -Wl,-Ttext,$(TEXT_BASE) -EL

#########################################################################

export	CROSS_COMPILE AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP

export	TEXT_BASE CPPFLAGS CFLAGS AFLAGS LDFLAGS

#########################################################################

%.s:	%.S
	@echo $(CPP) $(AFLAGS) -o $@ $<
	$(CPP) $(AFLAGS) -o $@ $<
%.o:	%.S
	@echo $(CC) $(AFLAGS) -o $@ $<
	$(CC) $(AFLAGS) -c -o $@ $<
%.o:	%.c
	@echo $(CC) $(CFLAGS) -c -o $@ $<
	$(CC) $(CFLAGS) -c -o $@ $<

#########################################################################
