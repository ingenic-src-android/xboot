#########################################################################

include $(TOPDIR)/config.mk
include $(TOPDIR)/include/config.mk

#########################################################################

LIB	= $(obj)libnand_boot.a

SOBJS   =
ifeq ($(CONFIG_JZ4750),y)
COBJS   = jz4750_nand.o nand_boot_jz4750.o
endif
ifeq ($(CONFIG_JZ4760),y)
COBJS   = jz4760_nand.o nand_boot_jz4760.o 
endif
ifeq ($(CONFIG_JZ4760B),y)
COBJS   = jz4760b_nand.o nand_boot_jz4760b.o 
endif
ifeq ($(CONFIG_JZ4770),y)
COBJS   = jz4770_nand.o nand_boot_jz4770.o 
endif
ifeq ($(CONFIG_JZ4780),y)
COBJS   = nand_retry.o jz4780_nand.o nand_boot_jz4780.o
endif
ifeq ($(CONFIG_JZ4775),y)
COBJS   = nand_retry.o jz4775_nand.o nand_boot_jz4775.o 
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
