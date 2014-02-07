#########################################################################

include $(TOPDIR)/config.mk
include $(TOPDIR)/include/config.mk

#########################################################################

LIB	= $(obj)libnand.a

SOBJS   =

#########################################################################

COBJS   = boot_msg.o  nand_boot.o fw_update.o nand_retry.o

ifeq ($(CONFIG_JZ4750),y)
COBJS   += jz4750_nand.o
endif

ifeq ($(CONFIG_JZ4760),y)
COBJS   += jz4760_nand.o
endif

ifeq ($(CONFIG_JZ4760B),y)
COBJS   += jz4760b_nand.o
endif

ifeq ($(CONFIG_JZ4770),y)
COBJS   += jz4770_nand.o
endif

ifeq ($(CONFIG_JZ4780),y)
COBJS   += jz4780_nand.o nand_ids.o
endif

ifeq ($(CONFIG_JZ4775),y)
COBJS   += jz4775_nand.o nand_ids.o
endif
#########################################################################

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
