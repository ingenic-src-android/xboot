/*
 *  MODULE NAME : KD301_M03545_0317A
 *  320*480 RGB interface
 */

#ifndef __KD301_M03545_0317A_H__
#define __KD301_M03545_0317A_H__

void KD301_M03545_0317A_panel_display_on(void);
void KD301_M03545_0317A_panel_display_off(void);
void KD301_M03545_0317A_panel_display_pin_init(void);

#define __lcd_display_pin_init()                             \
	do {                                                 \
		KD301_M03545_0317A_panel_display_pin_init();  \
	} while (0)

#define __lcd_display_on()                                   \
	do {                                                 \
		KD301_M03545_0317A_panel_display_on();        \
	} while (0)

#define __lcd_display_off()                                  \
	do {                                                 \
		KD301_M03545_0317A_panel_display_off();       \
	} while (0)


#define __lcd_special_on()         __lcd_display_on()
#define __lcd_special_off()        __lcd_display_off()
#define __lcd_special_pin_init()   __lcd_display_pin_init()
#endif

