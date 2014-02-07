#########################################################################

include $(TOPDIR)/config.mk
include $(TOPDIR)/include/config.mk

#########################################################################

LIB	= $(obj)libcommon.a

SOBJS   =
COBJS   = cpu.o common.o
ifneq ($(CONFIG_SERIAL_DISABLE),y)
   COBJS  += jz_serial.o debug.o
endif


# SOC select ############################################################

ifeq ($(CONFIG_ACT8600),y)
     COBJS   += simulate_i2c.o act8600_power.o
endif

ifeq ($(CONFIG_RICOH618),y)
	COBJS   += simulate_i2c.o ricoh618_power.o
endif

ifeq ($(CONFIG_JZ4760),y)
COBJS += jz4760.o jz4760_board.o jz4760_ddr.o
endif

ifeq ($(CONFIG_JZ4760B),y)
COBJS += jz4760b.o jz4760b_board.o jz4760b_ddr.o
endif

ifeq ($(CONFIG_JZ4770),y)
COBJS += jz4770.o jz4770_board.o jz4770_ddr.o
endif

ifeq ($(CONFIG_JZ4780),y)
COBJS += jz4780.o jz4780_board.o jz4780_ddr.o
endif

ifeq ($(CONFIG_JZ4775),y)
COBJS += jz4775.o jz4775_board.o jz4775_ddr.o
endif
#########################################################################

ifeq ($(CONFIG_USE_MDDR),y)
#COBJS += $(board-obj-y)jz_ddr_dqs_train.o
COBJS += $(board-obj-y)jz_mddr_init.o
endif

ifeq ($(CONFIG_USE_DDR2),y)
COBJS += $(board-obj-y)jz_ddr2_init.o
endif

ifeq ($(CONFIG_USE_DDR3),y)
COBJS += $(board-obj-y)jz_ddr_dqs_train.o
COBJS += $(board-obj-y)jz_ddr3_init.o
ifeq ($(CONFIG_JZ4780),y)
CFLAGS    += -DJZ4780
endif
ifeq ($(CONFIG_JZ4775),y)
CFLAGS    += -DJZ4775
endif
endif

ifeq ($(CONFIG_USE_LPDDR2),y)
ifeq ($(CONFIG_JZ4780),y)
CFLAGS    += -DJZ4780
endif
ifeq ($(CONFIG_JZ4775),y)
CFLAGS    += -DJZ4775
endif
COBJS += $(board-obj-y)jz_ddr_dqs_train.o
COBJS += $(board-obj-y)jz_lpddr2_init.o
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
