/*
 *  MODULE NAME : KD301_M03545_0317A
 *  320*480 RGB interface
 */

#include <config.h>
#include <serial.h>
#include <common.h>
#include <act8600_power.h>
#include <lcd_configs/KD301_M03545_0317A.h>

#define GPIO_LCD_SDO		GPC(0)
#define GPIO_LCD_SDI		GPC(1)
#define GPIO_LCD_SCL		GPC(10)
#define GPIO_LCD_CS		GPC(11)
#define GPIO_LCD_RESET		GPB(28)
#define GPIO_LCD_RS		GPC(21)

static void inline set_lcd_power_on(void)
{
	/* 3.3V */
	act8600_ldo_enable(ACT8600_LDO7_VOLTAGE_SET, 0x39);
}

static void inline set_lcd_power_off(void) {
	act8600_ldo_disable(ACT8600_LDO7_VOLTAGE_SET);
}

static int lcd_write_cmd( unsigned char cmd )
{
	int i;
	__gpio_clear_pin(GPIO_LCD_CS);
	udelay(2);
	__gpio_clear_pin(GPIO_LCD_SCL);
	udelay(5);
	__gpio_clear_pin(GPIO_LCD_SDI);
	udelay(5);
	__gpio_set_pin(GPIO_LCD_SCL);
	udelay(5);
	for( i = 0; i < 8 ;i++ )
		{
			udelay(5);
			__gpio_clear_pin(GPIO_LCD_SCL);
			udelay(5);
			if( cmd & 0x80 )
				__gpio_set_pin(GPIO_LCD_SDI);
			else
				__gpio_clear_pin(GPIO_LCD_SDI);

			udelay( 5 );
			__gpio_set_pin(GPIO_LCD_SCL);
			udelay( 5 );
			cmd <<= 1;
		}
	__gpio_set_pin(GPIO_LCD_CS);
	udelay(5);
	return 0;
}

static int lcd_write_data( unsigned char data )
{
	int i;
	__gpio_clear_pin(GPIO_LCD_CS);
	udelay(2);
	__gpio_clear_pin(GPIO_LCD_SCL);
	udelay(5);
	__gpio_set_pin(GPIO_LCD_SDI);
	udelay(5);
	__gpio_set_pin(GPIO_LCD_SCL);
	udelay(5);
	for( i = 0; i < 8 ;i++ )
		{
			udelay(5);
			__gpio_clear_pin(GPIO_LCD_SCL);
			udelay(5);
			if( data & 0x80 )
				__gpio_set_pin(GPIO_LCD_SDI);
			else
				__gpio_clear_pin(GPIO_LCD_SDI);

			udelay(5);
			__gpio_set_pin(GPIO_LCD_SCL);
			udelay( 5 );
			data <<= 1;
		}
	__gpio_set_pin(GPIO_LCD_CS);
	udelay(2);
	return 0;
}

static void special_pin_init(void)
{
	lcd_write_cmd(0x01);

	lcd_write_cmd(0xC0);
	lcd_write_data(0x15);  //lcd_write_data(0x15);
	lcd_write_data(0x15); //lcd_write_data(0x15);

	lcd_write_cmd(0xC1);
	lcd_write_data(0x45);
	lcd_write_data(0x07);

	lcd_write_cmd(0xC5);
	lcd_write_data(0x00);
	lcd_write_data(0x42);
	lcd_write_data(0x80);

	lcd_write_cmd(0xC2);
	lcd_write_data(0x33);

	lcd_write_cmd(0xB1);
	lcd_write_data(0xD0);
	lcd_write_data(0x11);

	lcd_write_cmd(0xB4);
	lcd_write_data(0x02);

	lcd_write_cmd(0xB6);
	lcd_write_data(0x00);
	lcd_write_data(0x22); //lcd_write_data(0x02);
	lcd_write_data(0x3B);

	lcd_write_cmd(0xB7);
	lcd_write_data(0x07);//07

	lcd_write_cmd(0xF0);
	lcd_write_data(0x36);
	lcd_write_data(0xA5);
	lcd_write_data(0xD3);

	lcd_write_cmd(0xE5);
	lcd_write_data(0x80);

	lcd_write_cmd(0xE5);
	lcd_write_data(0x01);

	lcd_write_cmd(0xB3);
	lcd_write_data(0x00);

	lcd_write_cmd(0xE5);
	lcd_write_data(0x00);

	lcd_write_cmd(0xF0);
	lcd_write_data(0x36);
	lcd_write_data(0xA5);
	lcd_write_data(0x53);

	lcd_write_cmd(0xE0);
	lcd_write_data(0x13);
	lcd_write_data(0x36);
	lcd_write_data(0x21);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x13);
	lcd_write_data(0x36);
	lcd_write_data(0x21);
	lcd_write_data(0x00);
	lcd_write_data(0x04); //lcd_write_data(0x04);
	lcd_write_data(0x04); //lcd_write_data(0x04);

	lcd_write_cmd(0x36);
	lcd_write_data(0x08);

	lcd_write_cmd(0xEE);
	lcd_write_data(0x00);

	lcd_write_cmd(0x3A);
	lcd_write_data(0x66);

	lcd_write_cmd(0xB0);
	lcd_write_data(0x86);
	lcd_write_cmd(0xB6);
	lcd_write_data(0x32);

	lcd_write_cmd(0x20);

	lcd_write_cmd(0x11);
	udelay(110);

	lcd_write_cmd(0x29);
	lcd_write_cmd(0x2C);
}

static jz_lcd_hard_reset(void)
{
	__gpio_set_pin(GPIO_LCD_RESET);
	mdelay(15);
	__gpio_clear_pin(GPIO_LCD_RESET);
	mdelay(15);
	__gpio_set_pin(GPIO_LCD_RESET);
}

static void display_panel_init()
{
	serial_puts_info("====>display_panel_init.\n");
	jz_lcd_hard_reset();
	special_pin_init();
}

void KD301_M03545_0317A_panel_display_pin_init(void)
{
	/* turn on the lcd power supply */
	set_lcd_power_on();
	udelay(50);

	__gpio_as_output(GPIO_LCD_SDO);
	__gpio_as_output(GPIO_LCD_SDI);
	__gpio_as_output(GPIO_LCD_SCL);
	__gpio_as_output(GPIO_LCD_CS);
	__gpio_set_pin(GPIO_LCD_CS);
	__gpio_as_input(GPIO_LCD_RS);

	display_panel_init();

        serial_puts_info("KD301_M03545_0317A panel display pin init\n");
}

void KD301_M03545_0317A_panel_display_on(void)
{
#if 0
	set_lcd_power_on();
	udelay(50);

	display_panel_init();
#endif

        serial_puts_info("KD301_M03545_0317A panel display on\n");
}

void KD301_M03545_0317A_panel_display_off(void)
{
	set_lcd_power_off();
	__gpio_clear_pin(GPIO_LCD_RESET);
	mdelay(15);
        serial_puts_info("KD301_M03545_0317A panel display off\n");
}
