/*
 *  LCD control code for KFM701A21_1A
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#ifndef __KFM701A21_1A_H__
#define __KFM701A21_1A_H__

void kfm701a21_1a_panel_display_on(void);
void kfm701a21_1a_panel_display_off(void);
void kfm701a21_1a_panel_display_pin_init(void);

#define __lcd_display_pin_init()                             \
	do {                                                 \
		kfm701a21_1a_panel_display_pin_init();  \
	} while (0)

#define __lcd_display_on()                                   \
	do {                                                 \
		kfm701a21_1a_panel_display_on();        \
	} while (0)

#define __lcd_display_off()                                  \
	do {                                                 \
		kfm701a21_1a_panel_display_off();       \
	} while (0)


#define __lcd_special_on()         __lcd_display_on()
#define __lcd_special_off()        __lcd_display_off()
#define __lcd_special_pin_init()   __lcd_display_pin_init()
#endif
