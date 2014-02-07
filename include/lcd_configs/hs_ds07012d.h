/*
 *  MODULE NAME : HS-DS07012D
 *  800*480 RGB interface
 */

#ifndef __HS_DS07012D_H__
#define __HS_DS07012D_H__

void hs_ds07012d_panel_display_on(void);
void hs_ds07012d_panel_display_off(void);
void hs_ds07012d_panel_display_pin_init(void);

#define __lcd_display_pin_init()                             \
	do {                                                 \
		hs_ds07012d_panel_display_pin_init();  \
	} while (0)

#define __lcd_display_on()                                   \
	do {                                                 \
		hs_ds07012d_panel_display_on();        \
	} while (0)

#define __lcd_display_off()                                  \
	do {                                                 \
		hs_ds07012d_panel_display_off();       \
	} while (0)


#define __lcd_special_on()         __lcd_display_on()
#define __lcd_special_off()        __lcd_display_off()
#define __lcd_special_pin_init()   __lcd_display_pin_init()
#endif

