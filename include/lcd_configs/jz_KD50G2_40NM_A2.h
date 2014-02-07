/*
 *  LCD panel setting file.
 */

#ifndef __JZ_KD50G2_40NM_A2_H__
#define __JZ_KD50G2_40NM_A2_H__

#if defined(CONFIG_JZ4760_CRATER) || defined(CONFIG_JZ4760_LYNX) || defined(CONFIG_JZ4770_MAPLE)
#define __lcd_special_pin_init()		\
	do {						\
		__gpio_as_output(GPIO_LCD_DISP_N);           \
		__gpio_set_pin(GPIO_LCD_DISP_N);             \
	    } while (0)
#define __lcd_special_on()                          \
	do {                                            \
	} while (0)
#define __lcd_special_off()     \
	do {                        \
	} while (0)

#elif defined(CONFIG_JZ4760_LEPUS)
#define __lcd_special_pin_init()		\
	do {						\
		__gpio_as_output(GPIO_LCD_DISP_N);           \
		__gpio_set_pin(GPIO_LCD_DISP_N);             \
	    } while (0)
#define __lcd_special_on()                          \
	do {                                            \
		__gpio_as_output(GPIO_LCD_POWER_N);\
		__gpio_set_pin(GPIO_LCD_POWER_N);\
	} while (0)
#define __lcd_special_off()     \
	do {                        \
		__gpio_as_output(GPIO_LCD_POWER_N);\
		__gpio_clear_pin(GPIO_LCD_POWER_N);\
	} while (0)
#endif
#endif	/* __JZ_KD50G2_40NM_A2_H__ */

