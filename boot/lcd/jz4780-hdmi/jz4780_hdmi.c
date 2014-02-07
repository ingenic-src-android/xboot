/* boot/lcd/jz4780-hdmi/jz4780_hdmi.c
 *
 * Copyright (c) 2012 Ingenic Semiconductor Co., Ltd.
 *              http://www.ingenic.com/
 *
 * Core file for Ingenic HDMI driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <io.h>
#include <config.h>
#include <serial.h>
#include <debug.h>
#include <libc.h>
#include <jz4780.h>

#include "jz4780_hdmi.h"

#define HDMI_DEVICE_DEBUG 0

/*In normal mode this define should not open*/
//#define DVIMODE 1

static struct jzhdmi jzhdmi_info;
static videoParams_t pVideo;
static audioParams_t pAudio;
//static hdcpParams_t *pHdcp;
static productParams_t pProduct;

#if 0
static void edid_callback(void *param)
{
	serial_puts_info("EDID reading done");
	jzhdmi_info.edid_done = HDMI_HOTPLUG_EDID_DONE;
}

static void hpd_callback(void *param)
{
	u8 hpd = *((u8*)(param));
	if (hpd != TRUE) {
		serial_puts_info("HPD DISCONNECT\n");
		jzhdmi_info.hdmi_info.hdmi_status = HDMI_HOTPLUG_DISCONNECTED;
	} else {
		serial_puts_info("HPD CONNECT\n");
		jzhdmi_info.hdmi_info.hdmi_status = HDMI_HOTPLUG_CONNECTED;
	}
}

#ifdef DVIMODE
static u8 cliEdid_GetDtdCode(const dtd_t *dtd)
{
	u8 cea_mode = dtd_GetCode(dtd);
	dtd_t tmp_dtd;
	if (cea_mode == (u8)(-1))
	{
		for (cea_mode = 1; cea_mode < 60; cea_mode++)
		{
			dtd_Fill(&tmp_dtd, cea_mode, 0);
			if (dtd_IsEqual(dtd, &tmp_dtd))
			{
				break;
			}
		}
		if (cea_mode >= 60)
		{
			return -1;
		}
	}
	return cea_mode;
}
#endif

int compliance_Standby(struct jzhdmi *jzhdmi)
{
#if 0
	videoParams_t *pVideo = jzhdmi->pVideo;
	audioParams_t *pAudio = jzhdmi->pAudio;
	hdcpParams_t *pHdcp = jzhdmi->pHdcp;
	productParams_t *pProduct = jzhdmi->pProduct;
#endif
	return TRUE;
}

static int hdmi_read_edid(struct jzhdmi *jzhdmi)
{
	int timeout = 0;
	jzhdmi->edid_done = 0;
	if (!api_EdidRead(edid_callback)){
		serial_puts_info("---edid failed\n");
		return FALSE;
	}

	while ((jzhdmi->edid_done != HDMI_HOTPLUG_EDID_DONE ||
	       jzhdmi->hdmi_info.hdmi_status != HDMI_HOTPLUG_CONNECTED) &&
	       ++timeout <= 400) {
		mdelay(5);
	}
	if(timeout > 400){
		serial_puts_info("---hdmi read edid timeout\n");
		return FALSE;
	}
	return TRUE;
}
#endif

static int hdmi_config(struct jzhdmi *jzhdmi)
{
	dtd_t tmp_dtd;
	u8 ceaCode = 0;
	videoParams_t *pVideo = jzhdmi->pVideo;
	audioParams_t *pAudio = jzhdmi->pAudio;
	hdcpParams_t *pHdcp = jzhdmi->pHdcp;
	productParams_t *pProduct = jzhdmi->pProduct;

	videoParams_SetHdmi(pVideo, TRUE);
	videoParams_SetColorResolution(pVideo, 0);

	ceaCode = jzhdmi->hdmi_info.out_type;
	if (board_SupportedRefreshRate(ceaCode) != -1) {
		dtd_Fill(&tmp_dtd, ceaCode, board_SupportedRefreshRate(ceaCode));
		videoParams_SetDtd(pVideo, &tmp_dtd);
	} else {
		serial_puts_info("CEA mode not supported\n");
	}

	if (api_Configure(pVideo, pAudio, pProduct, pHdcp)) {
		serial_puts_info("api_Configure ok\n");
	} else {
		serial_puts_info("api_Configure fail\n");
	}

	return TRUE;
}

static int hdmi_init(struct jzhdmi *jzhdmi)
{
	videoParams_t *pVideo = jzhdmi->pVideo;
	audioParams_t *pAudio = jzhdmi->pAudio;
//	hdcpParams_t *pHdcp = jzhdmi->pHdcp;
	productParams_t *pProduct = jzhdmi->pProduct;

	const u8 vName[] = "Synopsys";
	const u8 pName[] = "Complnce";

	if (!pProduct) {
		serial_puts_info("pVideo is NULL\n");
		return FALSE;
	}
	productParams_Reset(pProduct);
	productParams_SetVendorName(pProduct, vName,sizeof(vName) - 1);
	productParams_SetProductName(pProduct, pName,sizeof(pName) - 1);
	productParams_SetSourceType(pProduct, 0x0A);

	if (!pAudio) {
		serial_puts_info("pAudio is NULL\n");
		return FALSE;
	}
	audioParams_Reset(pAudio);
#if 0
	/*GPA*/

	serial_puts_info("GPA interface\n");
	audioParams_SetInterfaceType(pAudio, GPA);
	audioParams_SetCodingType(pAudio, PCM);
	audioParams_SetChannelAllocation(pAudio, 0);
	audioParams_SetPacketType(pAudio, AUDIO_SAMPLE);
	audioParams_SetSampleSize(pAudio, 16);
	audioParams_SetSamplingFrequency(pAudio, 44100);
	audioParams_SetLevelShiftValue(pAudio, 0);
	audioParams_SetDownMixInhibitFlag(pAudio, 0);
	audioParams_SetClockFsFactor(pAudio, 128);
#else
	audioParams_SetInterfaceType(pAudio, I2S);
	audioParams_SetCodingType(pAudio, PCM);
	audioParams_SetChannelAllocation(pAudio, 0);
	audioParams_SetPacketType(pAudio, AUDIO_SAMPLE);
	audioParams_SetSampleSize(pAudio, 16);
	audioParams_SetSamplingFrequency(pAudio, 44100);
	//audioParams_SetSamplingFrequency(pAudio, 48000);
	audioParams_SetLevelShiftValue(pAudio, 0);
	audioParams_SetDownMixInhibitFlag(pAudio, 0);
	audioParams_SetClockFsFactor(pAudio, 64);
#endif

	if (!pVideo) {
		serial_puts_info("pVideo is NULL\n");
		return FALSE;
	}
	videoParams_Reset(pVideo);
	videoParams_SetEncodingIn(pVideo, RGB);
	videoParams_SetEncodingOut(pVideo, RGB);
	return TRUE;
}

#if HDMI_DEVICE_DEBUG
static void print_hdmi_info(void)
{
	int i = 0;
	serial_puts_msg("\n Audio Configure:\n");
	serial_puts_msg("\n ===>REG[3100]= ");
	serial_put_hex(api_CoreRead(0x3100));
	serial_puts_msg("\n ===>REG[3101]= ");
	serial_put_hex(api_CoreRead(0x3101));
	serial_puts_msg("\n ===>REG[3102]= ");
	serial_put_hex(api_CoreRead(0x3102));
	serial_puts_msg("\n ===>REG[3103]= ");
	serial_put_hex(api_CoreRead(0x3103));
	serial_puts_msg("\n ===>REG[3200]= ");
	serial_put_hex(api_CoreRead(0x3200));
	serial_puts_msg("\n ===>REG[3201]= ");
	serial_put_hex(api_CoreRead(0x3201));
	serial_puts_msg("\n ===>REG[3202]= ");
	serial_put_hex(api_CoreRead(0x3202));
	serial_puts_msg("\n ===>REG[3203]= ");
	serial_put_hex(api_CoreRead(0x3203));
	serial_puts_msg("\n ===>REG[3204]= ");
	serial_put_hex(api_CoreRead(0x3204));
	serial_puts_msg("\n ===>REG[3205]= ");
	serial_put_hex(api_CoreRead(0x3205));
	serial_puts_msg("\n ===>REG[3206]= ");
	serial_put_hex(api_CoreRead(0x3206));

	serial_puts_msg("\n Main Clock Ctrl:\n");
	serial_puts_msg("\n ===>REG[4001]= ");
	serial_put_hex(api_CoreRead(0x4001));
	serial_puts_msg("\n ===>REG[4002]= ");
	serial_put_hex(api_CoreRead(0x4002));
	serial_puts_msg("\n ===>REG[4006]= ");
	serial_put_hex(api_CoreRead(0x4006));

	serial_puts_msg("\n FC Audio:\n");
	for (i = 0x1025; i <= 0x1028; i++) {
		serial_puts_msg("\n ===>REG[");
		dump_uint(i);
		serial_puts_msg("]= ");
		serial_put_hex(api_CoreRead(i));
	}

	for (i = 0x1063; i <= 0x106f; i++) {
		serial_puts_msg("\n ===>REG[");
		dump_uint(i);
		serial_puts_msg("]= ");
		serial_put_hex(api_CoreRead(i));
	}

	for (i = 0x3000; i <= 0x3007; i++) {
		serial_puts_msg("\n ===>REG[");
		dump_uint(i);
		serial_puts_msg("]= ");
		serial_put_hex(access_CoreReadByte(i));
	}
}
#endif

static void hdmi_power_on(void)
{
	__gpio_as_output(17);
	__gpio_set_pin(17);
}

void hdmi_ctrl_init(void)
{
	int i;
	unsigned long rate;
	unsigned long vpll_tmp;
	unsigned nf, nr, no;
	unsigned rate_div = 0;
	struct jzhdmi *jzhdmi;

	jzhdmi_info.pVideo = &pVideo;
	jzhdmi_info.pAudio = &pAudio;
	/*this need null,wo do not need hdcp now */
	jzhdmi_info.pHdcp = NULL;
	jzhdmi_info.pProduct = &pProduct;

	jzhdmi = (struct jzhdmi *)&jzhdmi_info;
	jzhdmi->hdmi_info.hdmi_status = HDMI_HOTPLUG_DISCONNECTED;
	jzhdmi->edid_faild = 0;

	vpll_tmp = INREG32(CPM_CPVPCR);
	//serial_puts_msg("CPM_CPVPCR = ");
	//dump_uint(vpll_tmp);
	nf = ((vpll_tmp >> 19) & 0x1fff) + 1;
	nr = ((vpll_tmp >> 13) & 0x3f) + 1;
	no = ((vpll_tmp >> 9) & 0xf ) + 1;
	vpll_tmp = (CFG_EXTAL / 1000) * nf / nr / no; /* KHz */

	rate = 27000000; /* 18M~27M */
	rate /= 1000; /* KHz */

	for (i = 1; i <= 0x100; i++) {
		if (vpll_tmp / i <= rate) {
			rate_div = i - 1;
			break;
		}
	}
	__cpm_hdmiclk_disable();
	__cpm_set_hdmidiv(rate_div);
	__cpm_hdmiclk_enable();

	__cpm_start_hdmi();

#if HDMI_DEVICE_DEBUG
	serial_puts_info("HDMI cgu div= ");
	serial_put_dec(__cpm_get_hdmidiv());
	serial_puts_msg("REG_CPM_HDMICDR= ");
	dump_uint(REG_CPM_HDMICDR);
	serial_puts_msg("\nREG_CPM_CLKGR1 = ");
	serial_put_hex(REG_CPM_CLKGR1);
#endif

	hdmi_power_on();

	/*request_irq in bsp/system.c*/
	if (!access_Initialize((u8 *)HDMI_BASE)) {
		return;
	}

	//api_EventEnable(HPD_EVENT, hpd_callback, FALSE);
	if (!api_Initialize(0, 1, 2500,0)) {
		serial_puts_info("api_Initialize fail\n");
		return;
	}

#ifdef CONFIG_FORCE_RESOLUTION
	if (CONFIG_FORCE_RESOLUTION <= 0 || CONFIG_FORCE_RESOLUTION > 64) {
		serial_puts_info("Force HDMI init VIC is illegal");
		return;
	}
	jzhdmi->hdmi_info.out_type = CONFIG_FORCE_RESOLUTION;
#else
	jzhdmi->hdmi_info.out_type = 4;
#endif
	//api_phy_enable(PHY_ENABLE);
	hdmi_init(jzhdmi);
	hdmi_config(jzhdmi);

	serial_puts_info("HDMI init VIC = ");
	serial_put_dec(jzhdmi->hdmi_info.out_type);

#if HDMI_DEVICE_DEBUG
	print_hdmi_info();
#endif

	return;
}
