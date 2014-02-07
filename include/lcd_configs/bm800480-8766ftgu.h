/*
 *  MODULE NAME : BM800480-8766FTGU
 *  800*480 RGB interface
 */

#ifndef __BM800480_8766FTGU_H__
#define __BM800480_8766FTGU_H__

void bm800480_8766ftgu_panel_display_on(void);
void bm800480_8766ftgu_panel_display_off(void);
void bm800480_8766ftgu_panel_display_pin_init(void);

#define __lcd_display_pin_init()                             \
	do {                                                 \
		bm800480_8766ftgu_panel_display_pin_init();  \
	} while (0)

#define __lcd_display_on()                                   \
	do {                                                 \
		bm800480_8766ftgu_panel_display_on();        \
	} while (0)

#define __lcd_display_off()                                  \
	do {                                                 \
		bm800480_8766ftgu_panel_display_off();       \
	} while (0)


#define __lcd_special_on()         __lcd_display_on()
#define __lcd_special_off()        __lcd_display_off()
#define __lcd_special_pin_init()   __lcd_display_pin_init()
#endif

