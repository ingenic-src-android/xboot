######################################################################

include $(TOPDIR)/config.mk
-include $(TOPDIR)/include/config.mk

#########################################################################

LIB	=  $(obj)libdata_verify.a

SOBJS   =

#########################################################################

COBJS   = data_verify.o

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

clean:
	find $(OBJTREE) -type f \
		\( -name 'core' -o -name '*.bak' -o -name '*~' \
		-o -name '*.o'  -o -name '*.a' -o -name '.depend' \) -print \
		| xargs rm -f
