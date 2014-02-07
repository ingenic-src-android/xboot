#########################################################################

include $(TOPDIR)/config.mk
include $(TOPDIR)/include/config.mk

#########################################################################

LIB	= $(obj)libusb.a
NMLIB   = $(obj)libnm.a
SOBJS   =

#COBJS   = usbloader.o board.o hsusb.o poll.o
COBJS   = board.o

ifeq ($(CONFIG_NAND_X_BOOT),y)
COBJS	+= flash.o nm/convert_img.o nm/APP_vNand.o
endif

ifeq ($(CONFIG_MSC_X_BOOT),y)
COBJS	+= card.o
endif

ifeq ($(CONFIG_JZ4750),y)
COBJS   += jz4750_usb.o
endif

ifeq ($(CONFIG_JZ4760),y)
COBJS   += jz4760_usb.o
endif

ifeq ($(CONFIG_JZ4760B),y)
COBJS   += jz4760_usb.o
endif

ifeq ($(CONFIG_JZ4770),y)
COBJS   += jz4770_usb.o
endif

ifeq ($(CONFIG_JZ4780),y)
COBJS   += jz4780_usb.o jz4780_init.o jz4780_poll.o
endif

ifeq ($(CONFIG_JZ4775),y)
COBJS   += jz4775_usb.o jz4775_init.o jz4775_poll.o
endif
#########################################################################
SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)

OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))

all:	$(obj).depend $(LIB) $(NMLIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(NMLIB): libnm.hex
	cp libnm.hex libnm.a

#########################################################################

# defines $(obj).depend target
include $(TOPDIR)/rules.mk

sinclude $(obj).depend

#########################################################################
