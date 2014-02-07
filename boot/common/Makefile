#########################################################################

include $(TOPDIR)/config.mk
include $(TOPDIR)/include/config.mk

#########################################################################

LIB	= $(obj)libcommon.a

SOBJS   = 

COBJS   = cpu.o libimg.o common.o xbootimg.o aes.o rsa_verify.o simulation_key.o tlb.o matrix_keypad.o
ifneq ($(CONFIG_SERIAL_DISABLE),y)
   COBJS  += jz_serial.o debug.o
endif

ifeq ($(CONFIG_POWER_MANAGEMENT),y)
   COBJS	+= pm.o
endif   
ifeq ($(CONFIG_ACT8930),y)
     COBJS   += simulate_i2c.o act8930_power.o
endif
ifeq ($(CONFIG_ACT8600),y)
     COBJS   += simulate_i2c.o act8600_power.o
endif
ifeq ($(CONFIG_RICOH618),y)
     COBJS   += simulate_i2c.o ricoh618_power.o
endif
ifeq ($(CONFIG_TPS65023),y)
     COBJS   += simulate_i2c.o tps65023_power.o
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
