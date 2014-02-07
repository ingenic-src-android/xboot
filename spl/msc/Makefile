#########################################################################

include $(TOPDIR)/config.mk
include $(TOPDIR)/include/config.mk
#include config.mk
#########################################################################

LIB	= $(obj)libmsc_boot.a

SOBJS   =
ifeq ($(CONFIG_JZ4760),y)
COBJS   = msc_boot_jz4760.o 
endif

ifeq ($(CONFIG_JZ4760B),y)
COBJS   = msc_boot_jz4760b.o 
endif

ifeq ($(CONFIG_JZ4770),y)
COBJS   = msc_boot_jz4770.o 
endif

ifeq ($(CONFIG_JZ4780),y)
ifneq ($(CONFIG_NOR_SPL),y)
COBJS   = msc_boot_jz4780.o
else
COBJS   = msc_boot_jz4780_norspl.o
endif
endif

ifeq ($(CONFIG_JZ4775),y)
COBJS   = msc_boot_jz4775.o 
endif

SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)

OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))

all:	$(obj).depend $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(TOPDIR)/rules.mk

sinclude $(obj).depend

#########################################################################
