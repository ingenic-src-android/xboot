/*
 *  MODULE NAME : TM035PDH03
 *  320*480 RGB interface
 */

#ifndef __TM035PDH03_H__
#define __TM035PDH03_H__

void TM035PDH03_panel_display_on(void);
void TM035PDH03_panel_display_off(void);
void TM035PDH03_panel_display_pin_init(void);

#define __lcd_display_pin_init()                             \
	do {                                                 \
		TM035PDH03_panel_display_pin_init();  \
	} while (0)

#define __lcd_display_on()                                   \
	do {                                                 \
		TM035PDH03_panel_display_on();        \
	} while (0)

#define __lcd_display_off()                                  \
	do {                                                 \
		TM035PDH03_panel_display_off();       \
	} while (0)


#define __lcd_special_on()         __lcd_display_on()
#define __lcd_special_off()        __lcd_display_off()
#define __lcd_special_pin_init()   __lcd_display_pin_init()
#endif
