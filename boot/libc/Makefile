#########################################################################

include $(TOPDIR)/config.mk

#########################################################################

LIB	= $(obj)libc.a

SOBJS   =
COBJS   = memset.o memcmp.o memcpy.o strstr.o strcpy.o strlen.o strcmp.o strcat.o\
	cprintf.o malloc.o uint2str.o

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
