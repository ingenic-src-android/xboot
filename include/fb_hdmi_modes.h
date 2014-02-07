/*
 * fb_fb_hdmi_modes.h
 *
 * Copyright (c) 2012 Engenic Semiconductor Co., Ltd.
 *              http://www.engenic.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


#define ADD_HDMI_VIDEO_MODE(mode) mode

/*
 * struct fb_videomode - Defined in kernel/include/linux/fb.h
 * name
 * refresh, xres, yres, pixclock, left_margin, right_margin
 * upper_margin, lower_margin, hsync_len, vsync_len
 * sync
 * vmode, flag
 */

/*
 * Note:
  "flag" is used as hdmi mode search index.
 */

/* 1 */
#define HDMI_640X480_P_60HZ_4X3						\
	{"640x480-p-60hz-4:3",						\
			60, 640, 480, KHZ2PICOS(25200), 48, 16,		\
			33, 10, 96, 2,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 1}

/* 2 */
#define HDMI_720X480_P_60HZ_4X3						\
	{"720x480-p-60hz-4:3",						\
			60, 720, 480, KHZ2PICOS(27020), 60, 16,		\
			30, 9, 62, 6,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 2}

/* 3 */
#define HDMI_720X480_P_60HZ_16X9					\
	{"720x480-p-60hz-16:9",						\
			60, 720, 480, KHZ2PICOS(27020), 60, 16,		\
			30, 9, 62, 6,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 3}

/* 4 */
#define HDMI_1280X720_P_60HZ_16X9					\
	{"1280x720-p-60hz-16:9",					\
			60, 1280, 720, KHZ2PICOS(74250), 220, 110,	\
			20, 5, 40, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 4}

/* 5 */
#define HDMI_1920X1080_I_60HZ_16X9					\
	{"1920x1080-i-60hz-16:9",					\
			60, 1920, 540, KHZ2PICOS(74250), 148, 88,	\
			15, 2, 44, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_INTERLACED, 5}

/* 6 */
#define HDMI_720_1440X480_I_60HZ_4X3					\
	{"720-1440x480-i-60hz-4:3",					\
			60, 1440, 240, KHZ2PICOS(27027), 114, 38,	\
			15, 4, 124, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_INTERLACED, 6}

/* 7 */
#define HDMI_720_1440X480_I_60HZ_16X9					\
	{"720-1440x480-i-60hz-16:9",					\
			60, 1440, 240, KHZ2PICOS(27027), 114, 38,	\
			15, 4, 124, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_INTERLACED, 7}

/* 8 */
#define HDMI_720_1440X240_P_60HZ_4X3					\
	{"720-1440x240-p-60hz-4:3",					\
			60, 1440, 240, KHZ2PICOS(27027), 114, 38,	\
			15, 5, 124, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 8}

/* 9 */
#define HDMI_720_1440X240_P_60HZ_16X9					\
	{"720-1440x240-p-60hz-16:9",					\
			60, 1440, 240, KHZ2PICOS(27027), 114, 38,	\
			15, 5, 124, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 9}

/* 10 */
#define HDMI_2880X480_I_60HZ_4X3					\
	{"2880x480-i-60hz-4:3",						\
			60, 2880, 240, KHZ2PICOS(54054), 228, 76,	\
			15, 4, 248, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_INTERLACED, 10}

/* 11 */
#define HDMI_2880X480_I_60HZ_16X9					\
	{"2880x480-i-60hz-16:9",					\
			60, 2880, 240, KHZ2PICOS(54054), 228, 76,	\
			15, 4, 248, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_INTERLACED, 11}

/* 12 */
#define HDMI_2880X240_P_60HZ_4X3					\
	{"2880x240-p-60hz-4:3",						\
			60, 2880, 240, KHZ2PICOS(54054), 228, 76,	\
			15, 4, 248, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 12}

/* 13 */
#define HDMI_2880X240_P_60HZ_16X9					\
	{"2880x240-p-60hz-16:9",					\
			60, 2880, 240, KHZ2PICOS(54054), 228, 76,	\
			15, 4, 248, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 13}

/* 14 */
#define HDMI_1440X480_P_60HZ_4X3					\
	{"1440x480-p-60hz-4:3",						\
			60, 1440, 480, KHZ2PICOS(54054), 120, 32,	\
			30, 9, 124, 6,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 14}

/* 15 */
#define HDMI_1440X480_P_60HZ_16X9					\
	{"1440x480-p-60hz-16:9",					\
			60, 1440, 480, KHZ2PICOS(54054), 120, 32,	\
			30, 9, 124, 6,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 15}

/* 16 */
#define HDMI_1920X1080_P_60HZ_16X9					\
	{"1920x1080-p-60hz-16:9",					\
			60, 1920, 1080, KHZ2PICOS(148500), 148, 88,	\
			36, 4, 44, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 16}

/* 17 */
#define HDMI_720X576_P_50HZ_4X3						\
	{"720x576-p-50hz-4:3",						\
			50, 720, 576, KHZ2PICOS(27000), 68, 12,		\
			39, 5, 64, 5,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 17}

/* 18 */
#define HDMI_720X576_P_50HZ_16X9					\
	{"720x576-p-50hz-16:9",						\
			50, 720, 576, KHZ2PICOS(27000), 68, 12,		\
			39, 5, 64, 5,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 18}

/* 19 */
#define HDMI_1280X720_P_50HZ_16X9					\
	{"1280x720-p-50hz-16:9",					\
			50, 1280, 720, KHZ2PICOS(74250), 220, 440,	\
			20, 5, 40, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 19}

/* 20 */
#define HDMI_1920X1080_I_50HZ_16X9					\
	{"1920x1080-i-50hz-16:9",					\
			50, 1920, 540, KHZ2PICOS(74250), 148, 528,	\
			15, 2, 44, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_INTERLACED, 20}

/* 21 */
#define HDMI_720_1440X576_I_50HZ_4X3					\
	{"720-1440x576-i-50hz-4:3",					\
			50, 1440, 288, KHZ2PICOS(27000), 138, 24,	\
			19, 2, 126, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_INTERLACED, 21}

/* 22 */
#define HDMI_720_1440X576_I_50HZ_16X9					\
	{"720-1440x576-i-50hz-16:9",					\
			50, 1440, 288, KHZ2PICOS(27000), 138, 24,	\
			19, 2, 126, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_INTERLACED, 22}

/* 23 */
#define HDMI_720_1440X288_P_50HZ_4X3					\
	{"720-1440x288-p-50hz-4:3",					\
			50, 1440, 288, KHZ2PICOS(27000), 138, 24,	\
			19, 2, 126, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 23}
/* 24 */
#define HDMI_720_1440X288_P_50HZ_16X9					\
	{"720-1440x288-p-50hz-16:9",					\
			50, 1440, 288, KHZ2PICOS(27000), 138, 24,	\
			19, 2, 126, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 24}

/* 25 */
#define HDMI_2880X576_I_50HZ_4X3					\
	{"2880x576-i-50hz-4:3",						\
			50, 2880, 288, KHZ2PICOS(54000), 276, 48,	\
			19, 2, 252, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_INTERLACED, 25}

/* 26 */
#define HDMI_2880X576_I_50HZ_16X9					\
	{"2880x576-i-50hz-16:9",					\
			50, 2880, 288, KHZ2PICOS(54000), 276, 48,	\
			19, 2, 252, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_INTERLACED, 26}


/* 27 */
#define HDMI_2880X288_P_50HZ_4X3					\
	{"2880x288-p-50hz-4:3",					\
			50, 2880, 288, KHZ2PICOS(54000), 276, 48,	\
			19, 2, 252, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 27}
/* 28 */
#define HDMI_2880X288_P_50HZ_16X9					\
	{"2880x288-p-50hz-16:9",					\
			50, 2880, 288, KHZ2PICOS(54000), 276, 48,	\
			19, 2, 252, 3,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 28}

/* 29 */
#define HDMI_1440X576_P_50HZ_4X3					\
	{"1440x576-p-50hz-4:3",						\
			50, 1440, 576, KHZ2PICOS(54000), 136, 24,	\
			39, 5, 128, 5,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 29}

/* 30 */
#define HDMI_1440X576_P_50HZ_16X9					\
	{"1440x576-p-50hz-16:9",					\
			50, 1440, 576, KHZ2PICOS(54000), 136, 24,	\
			39, 5, 128, 5,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 30}

/* 31 */
#define HDMI_1920X1080_P_50HZ_16X9					\
	{"1920x1080-p-50hz-16:9",					\
			50, 1920, 1080, KHZ2PICOS(148500), 148, 528,	\
			36, 4, 44, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 31}

/* 32 */
#define HDMI_1920X1080_P_24HZ_16X9					\
	{"1920x1080-p-24hz-16:9",					\
			24, 1920, 1080, KHZ2PICOS(74250), 148, 638,	\
			36, 4, 44, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 32}

/* 33 */
#define HDMI_1920X1080_P_25HZ_16X9					\
	{"1920x1080-p-25hz-16:9",					\
			25, 1920, 1080, KHZ2PICOS(74250), 148, 528,	\
			36, 4, 44, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 33}

/* 34 */
#define HDMI_1920X1080_P_30HZ_16X9					\
	{"1920x1080-p-30hz-16:9",					\
			30, 1920, 1080, KHZ2PICOS(74250), 148, 88,	\
			36, 4, 44, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 34}

/* 41 */
#define HDMI_1280X720_P_100HZ_16X9					\
	{"1280x720-p-100hz-16:9",					\
			100, 1280, 720, KHZ2PICOS(148500), 220, 440,	\
			20, 5, 40, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 41}

/* 47 */
#define HDMI_1280X720_P_120HZ_16X9					\
	{"1280x720-p-120hz-16:9",					\
			120, 1280, 720, KHZ2PICOS(148500), 220, 110,	\
			20, 5, 40, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 47}

/* 48 */
#define HDMI_720X480_P_120HZ_4X3					\
	{"720x480-p-120hz-4:3",						\
			120, 720, 480, KHZ2PICOS(54000), 60, 16,	\
			30, 9, 62, 6,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 48}

/* 49 */
#define HDMI_720X480_P_120HZ_16X9					\
	{"720x480-p-120hz-16:9",					\
			120, 720, 480, KHZ2PICOS(54000), 60, 16,	\
			30, 9, 62, 6,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 49}

/* 56 */
#define HDMI_720X480_P_240HZ_4X3					\
	{"720x480-p-240hz-4:3",						\
			240, 720, 480, KHZ2PICOS(108000), 60, 16,	\
			30, 9, 62, 6,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 56}

/* 57 */
#define HDMI_720X480_P_240HZ_16X9					\
	{"720x480-p-240hz-16:9",					\
			240, 720, 480, KHZ2PICOS(108000), 60, 16,	\
			30, 9, 62, 6,					\
			0, /* Negative Sync */				\
			FB_VMODE_NONINTERLACED, 57}

/* 60 */
#define HDMI_1280X720_P_24HZ_16X9					\
	{"1280x720-p-24hz-16:9",					\
			24, 1280, 720, KHZ2PICOS(59400), 220, 1760,	\
			20, 5, 40, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 60}

/* 61 */
#define HDMI_1280X720_P_25HZ_16X9					\
	{"1280x720-p-25hz-16:9",					\
			25, 1280, 720, KHZ2PICOS(74250), 220, 2420,	\
			20, 5, 40, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 61}

/* 62 */
#define HDMI_1280X720_P_30HZ_16X9					\
	{"1280x720-p-30hz-16:9",					\
			30, 1280, 720, KHZ2PICOS(74250), 220, 1760,	\
			20, 5, 40, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 62}

/* 63 */
#define HDMI_1920X1080_P_120HZ_16X9					\
	{"1920x1080-p-120hz-16:9",					\
			120, 1920, 1080, KHZ2PICOS(297000), 148, 88,	\
			36, 4, 44, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 63}

/* 64 */
#define HDMI_1920X1080_P_100HZ_16X9					\
	{"1920x1080-p-100hz-16:9",					\
			100, 1920, 1080, KHZ2PICOS(297000), 148, 528,	\
			36, 4, 44, 5,					\
			FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,	\
			FB_VMODE_NONINTERLACED, 64}
#if 0
#define DEFAULT_HDMI_VIDEO_MODE_LIST					\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_60HZ_16X9), /* 4 */		\
	ADD_HDMI_VIDEO_MODE(HDMI_1920X1080_P_60HZ_16X9), /* 16 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_50HZ_16X9), /* 19 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1920X1080_P_50HZ_16X9), /* 31 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1920X1080_P_24HZ_16X9), /* 32 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1920X1080_P_25HZ_16X9), /* 33 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1920X1080_P_30HZ_16X9), /* 34 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_100HZ_16X9), /* 41 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_120HZ_16X9), /* 47 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_720X480_P_120HZ_16X9), /* 49 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_720X480_P_240HZ_16X9), /* 57 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_25HZ_16X9), /* 61 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_30HZ_16X9), /* 62 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1920X1080_P_120HZ_16X9), /* 63 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1920X1080_P_100HZ_16X9) /* 64 */

//	ADD_HDMI_VIDEO_MODE(HDMI_720X480_P_60HZ_16X9), /* 3 */		
//	ADD_HDMI_VIDEO_MODE(HDMI_640X480_P_60HZ_4X3), /* 1 */		
//	ADD_HDMI_VIDEO_MODE(HDMI_720X576_P_50HZ_16X9), /* 18 */		
//	ADD_HDMI_VIDEO_MODE(HDMI_1440X480_P_60HZ_16X9), /* 15 */
#else
#define DEFAULT_HDMI_VIDEO_MODE_LIST					\
	ADD_HDMI_VIDEO_MODE(HDMI_1920X1080_P_60HZ_16X9), /* 16 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1920X1080_P_50HZ_16X9), /* 31 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_60HZ_16X9), /* 4 */		\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_50HZ_16X9), /* 19 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_100HZ_16X9), /* 41 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_120HZ_16X9), /* 47 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_25HZ_16X9), /* 61 */	\
	ADD_HDMI_VIDEO_MODE(HDMI_1280X720_P_30HZ_16X9) /* 62 */
#endif
