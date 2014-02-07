#########################################################################

include $(TOPDIR)/config.mk
include $(TOPDIR)/include/config.mk

#########################################################################

LIB	= $(obj)libmsc.a

SOBJS   =

COBJS   = cmd_msc.o ctype.o msc_boot.o

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


