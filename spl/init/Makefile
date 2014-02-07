#########################################################################

include $(TOPDIR)/config.mk

#########################################################################

INIT	= init.o
SOBJS   = 
COBJS   = 

SRCS	:= $(INIT:.o=.S) $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))
INIT	:= $(addprefix $(obj),$(INIT))

all:	$(obj).depend $(INIT)

#########################################################################

# defines $(obj).depend target
include $(TOPDIR)/rules.mk

sinclude $(obj).depend

#########################################################################
