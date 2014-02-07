#########################################################################

include $(TOPDIR)/config.mk
include $(TOPDIR)/include/config.mk

#########################################################################

LIB	= $(obj)liblcd.a

SOBJS   =
COBJS   =

ifeq ($(CONFIG_JZ4780),y)

COBJS   += logo.o lcd.o jz4780_lcd.o lcd_panel_data.o

else


COBJS   = lcd.o logo.o

ifeq ($(CONFIG_JZ4750),y)
COBJS   += jz4750_lcd.o
endif

ifeq ($(CONFIG_JZ4760),y)
COBJS   += jz4760_lcd.o
endif

ifeq ($(CONFIG_JZ4760B),y)
COBJS   += jz4760_lcd.o
endif

ifeq ($(CONFIG_JZ4770),y)
COBJS   += jz4770_lcd.o
endif

ifeq ($(CONFIG_JZ4775),y)
COBJS   += jz4775_lcd.o lcd_panel_data.o
endif

endif	#CONFIG_JZ4780

ifeq ($(CONFIG_HDMI_IT6610),y)
COBJS   += hdmi.o it6610/it6610_drv.o it6610/chip_drv.o it6610/chip_sys.o it6610/gpio_i2c.o
endif

ifeq ($(CONFIG_HDMI_JZ4780),y)
COBJS	+= jz4780-hdmi/jz4780_hdmi.o \
jz4780-hdmi/api/api.o \
jz4780-hdmi/core/halAudioClock.o \
jz4780-hdmi/core/halFrameComposerPackets.o \
jz4780-hdmi/core/audioParams.o \
jz4780-hdmi/core/halFrameComposerGcp.o \
jz4780-hdmi/core/halAudioHbr.o \
jz4780-hdmi/core/packets.o \
jz4780-hdmi/core/control.o \
jz4780-hdmi/core/halFrameComposerVsd.o \
jz4780-hdmi/core/halVideoSampler.o \
jz4780-hdmi/core/halFrameComposerAvi.o \
jz4780-hdmi/core/halAudioI2s.o \
jz4780-hdmi/core/audio.o \
jz4780-hdmi/core/halVideoGenerator.o \
jz4780-hdmi/core/halAudioGenerator.o \
jz4780-hdmi/core/productParams.o \
jz4780-hdmi/core/video.o \
jz4780-hdmi/core/halColorSpaceConverter.o \
jz4780-hdmi/core/halVideoPacketizer.o \
jz4780-hdmi/core/halMainController.o \
jz4780-hdmi/core/halInterrupt.o \
jz4780-hdmi/core/halAudioSpdif.o \
jz4780-hdmi/core/halFrameComposerGamut.o \
jz4780-hdmi/core/halFrameComposerVideo.o \
jz4780-hdmi/core/halFrameComposerSpd.o \
jz4780-hdmi/core/halFrameComposerAcp.o \
jz4780-hdmi/core/halAudioDma.o \
jz4780-hdmi/core/halFrameComposerDebug.o \
jz4780-hdmi/core/halIdentification.o \
jz4780-hdmi/core/halFrameComposerAudio.o \
jz4780-hdmi/core/halFrameComposerIsrc.o \
jz4780-hdmi/core/videoParams.o \
jz4780-hdmi/core/halAudioGpa.o \
jz4780-hdmi/core/halFrameComposerAudioInfo.o \
jz4780-hdmi/hdcp/hdcpVerify.o \
jz4780-hdmi/hdcp/halHdcp.o \
jz4780-hdmi/hdcp/hdcp.o \
jz4780-hdmi/hdcp/hdcpParams.o \
jz4780-hdmi/phy/phy.o \
jz4780-hdmi/phy/halSourcePhy.o \
jz4780-hdmi/phy/halI2cMasterPhy.o \
jz4780-hdmi/edid/speakerAllocationDataBlock.o \
jz4780-hdmi/edid/shortAudioDesc.o \
jz4780-hdmi/edid/halEdid.o \
jz4780-hdmi/edid/videoCapabilityDataBlock.o \
jz4780-hdmi/edid/monitorRangeLimits.o \
jz4780-hdmi/edid/colorimetryDataBlock.o \
jz4780-hdmi/edid/hdmivsdb.o \
jz4780-hdmi/edid/edid.o \
jz4780-hdmi/edid/dtd.o \
jz4780-hdmi/edid/shortVideoDesc.o \
jz4780-hdmi/bsp/system.o \
jz4780-hdmi/bsp/access.o \
jz4780-hdmi/bsp/mutex.o \
jz4780-hdmi/bsp/board.o \
jz4780-hdmi/util/error.o \
jz4780-hdmi/util/log.o \
jz4780-hdmi/util/bitOperation.o
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

