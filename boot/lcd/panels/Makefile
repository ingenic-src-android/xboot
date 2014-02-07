######################################################

include $(TOPDIR)/config.mk
include $(TOPDIR)/include/config.mk

######################################################
######################################################

obj-$(CONFIG_LCD_BM800480_8766FTGU) += bm800480-8766ftgu.o
obj-$(CONFIG_LCD_KFM701A21_1A) += kfm701a21_1a.o
obj-$(CONFIG_LCD_HS_DS07012D) += hs-ds07012d.o
obj-$(CONFIG_LCD_TM080SDH01_00) += tm080sdh01_00.o
obj-$(CONFIG_LCD_KD301_M03545_0317A) += KD301_M03545_0317A.o
obj-$(CONFIG_LCD_TM035PDH03) += TM035PDH03.o

######################################################
######################################################

COBJS	= $(obj-y)

SRCS	:= $(COBJS:.o=.c)

OBJS	:= $(addprefix $(obj),$(COBJS))

LIB	= libpanel.a

######################################################

.PHONY: $(LIB)
.PHONY: all

######################################################

all: $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

######################################################

include $(TOPDIR)/rules.mk
sinclude $(obj).depend

######################################################
