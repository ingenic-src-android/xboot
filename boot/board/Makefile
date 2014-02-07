#########################################################################

include $(TOPDIR)/config.mk
include $(TOPDIR)/include/config.mk

#########################################################################

board-$(CONFIG_JZ4760_LYNX)		= lynx/
board-$(CONFIG_JZ4760_LEPUS)		= lepus/
board-$(CONFIG_JZ4760_CRATER)		= crater/
board-$(CONFIG_JZ4760_Z800)		= z800/
board-$(CONFIG_JZ4760_ALTAIR)		= altair/
board-$(CONFIG_JZ4760_TABLET_P2)	= tablet_p2/
board-$(CONFIG_JZ4760_PT701_8)		= pt701_8/
board-$(CONFIG_JZ4760_TABLET_8II)	= tablet_8II/
board-$(CONFIG_JZ4760_PX7_2R)   	= px7_2r/
board-$(CONFIG_JZ4770_PYXIS)   		= pyxis/
board-$(CONFIG_JZ4770_PISCES)  		= pisces/
board-$(CONFIG_JZ4770_MAPLE)  		= maple/
board-$(CONFIG_JZ4770_NPM701)  		= npm701/
board-$(CONFIG_JZ4770_NPM702)  		= npm702/
board-$(CONFIG_JZ4770_NPM703)  		= npm703/
board-$(CONFIG_JZ4770_NPM704)  		= npm704/
board-$(CONFIG_JZ4770_PS47)  		= ps47/
board-$(CONFIG_JZ4770_ZORROO)  		= zorroo/
board-$(CONFIG_JZ4770_UMIDO706)		= umido706/
board-$(CONFIG_JZ4770_O1)  		= o1/
board-$(CONFIG_JZ4770_LINDEN)  		= linden/
board-$(CONFIG_JZ4770_LINDEN_V_1_1_1) 	= linden_v_1_1_1/
board-$(CONFIG_JZ4770_JB01)  		= jb01/
board-$(CONFIG_JZ4770_AURORA)  		= aurora/
board-$(CONFIG_JZ4770_TVB)  		= tvb/
board-$(CONFIG_JZ4770_N55)  		= n55/
board-$(CONFIG_JZ4770_PI3800)  		= pi3800/
board-$(CONFIG_JZ4770_GPS1)  		= gps1/
board-$(CONFIG_JZ4770_VENUS)  		= venus/
board-$(CONFIG_JZ4770_HOLLY)  		= holly/
board-$(CONFIG_JZ4770_GZ21)  		= gz21/
board-$(CONFIG_JZ4770_OAK)  		= oak/
board-$(CONFIG_JZ4780_TEST)  		= test/
board-$(CONFIG_JZ4780_WARRIOR) 		= warrior/
board-$(CONFIG_JZ4780_NPM3701) 		= npm3701/
board-$(CONFIG_JZ4780_EBOOK) 		= ebook/
board-$(CONFIG_JZ4780_VEHICLE) 		= vehicle/
board-$(CONFIG_JZ4780_URBOARD) 		= urboard/
board-$(CONFIG_JZ4780_PRINTER) 		= printer/
board-$(CONFIG_JZ4780_NPM801) 		= npm801/
board-$(CONFIG_JZ4780_NPM709J) 		= npm709j/
board-$(CONFIG_JZ4780_M80) 		= m80/
board-$(CONFIG_JZ4780_JI8070A) 		= ji8070a/
board-$(CONFIG_JZ4780_JI8070B) 		= ji8070b/
board-$(CONFIG_JZ4780_Q8) 		= q8/
board-$(CONFIG_JZ4780_HDMI_80) 		= hdmi_80/
board-$(CONFIG_JZ4780_M80701) 		= m80701/
board-$(CONFIG_JZ4780_I88) 		= i88/
board-$(CONFIG_JZ4780_GRUS) 		= grus/
board-$(CONFIG_JZ4775_MENSA) 		= mensa/
board-$(CONFIG_JZ4775_K101) 		= k101/
board-$(CONFIG_JZ4775_COMET) 		= comet/
board-$(CONFIG_JZ4780_ZPAD80) 		= zpad80/
board-$(CONFIG_JZ4780_DWIN) 		= dwin/
board-$(CONFIG_JZ4780_RY1000) 		= ry1000/
board-$(CONFIG_JZ4775_CONE) 		= cone/
board-$(CONFIG_JZ4775_CONE_V1_2)	= cone_v1_2/
#########################################################################

include $(board-y)Makefile

LIB	= $(obj)lib_board.a

SSRCS   = $((obj-y:.o=.c)
COBJS   = $(addprefix $(board-y),$(obj-y))

#########################################################################

SRCS	:= $(addprefix $(board-y),$(SSRCS))

OBJS	:= $(addprefix $(obj),$(COBJS))

all:	$(obj).depend $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(TOPDIR)/rules.mk

sinclude $(obj).depend

#########################################################################


