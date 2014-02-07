#ifndef __JZ4780_HDMI_H__
#define __JZ4780_HDMI_H__

#include "api/api.h"
#include "edid/edid.h"
#include "util/log.h"
#include "bsp/mutex.h"
#include "bsp/board.h"

#include "core/control.h"
#include "core/video.h"
#include "core/audio.h"
#include "core/packets.h"
#include "hdcp/hdcp.h"
#include "edid/edid.h"
#include "phy/halSourcePhy.h"
#include "util/error.h"

#define GPA_ENABLE 0

#define HDMI_VIDEO_MODE_NUM 64 /* for test */

struct hdmi_device_params{
	videoParams_t *pVideo;
	audioParams_t *pAudio;
	hdcpParams_t *pHdcp;
	productParams_t *pProduct;
	//dtd_t           *dtd /* video mode */
};

enum HMDI_STATUS {
	HDMI_HOTPLUG_DISCONNECTED,
	HDMI_HOTPLUG_CONNECTED,
	HDMI_HOTPLUG_EDID_DONE,
};

struct hdmi_info{
	enum HMDI_STATUS  hdmi_status;
	unsigned int out_type;
	unsigned int support_modenum;
	unsigned int *support_mode;
};

struct jzhdmi {
	videoParams_t *pVideo;
	audioParams_t *pAudio;
	hdcpParams_t *pHdcp;
	productParams_t *pProduct;

	struct hdmi_device_params hdmi_params;
	struct hdmi_info hdmi_info;

	unsigned int hpd_connected;
	unsigned int edid_done;
	unsigned int edid_faild;
};

#endif
