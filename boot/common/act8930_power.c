/*
 * PMU driver for act8930 PMU
 *
 */
#include <config.h>
#include <act8930_power.h>
#include <simulate_i2c.h>
#include <serial.h>

#define EIO 2
/*************************************************************************/
static int act8930_i2c_read_device(unsigned char reg, int bytes, void *dest)
{
	int ret;
	
	ret = simulate_i2c_read((unsigned char)ATC8930_I2C_ADDR, (unsigned char *)dest, reg, bytes);
	if (ret < 0)
		return ret;
	if (ret < bytes)
		return -EIO;
	
	return 0;
}

/* 	Currently we allocate the write buffer on the stack; this is OK for
  * 	small writes - if we need to do large writes this will need to be
  * 	revised.
  */
static int act8930_i2c_write_device(unsigned char reg, int bytes, void *src)
{
	int ret;

	ret = simulate_i2c_write((unsigned char)ATC8930_I2C_ADDR, (unsigned char *)src, reg, bytes);
	if (ret < 0)
		return ret;
	if (ret < bytes)
		return -EIO;

	return 0;
}

static int act8930_read(unsigned char reg, int bytes, void *dest)
{
	int ret, i;
	unsigned char *buf = dest;

	ret = act8930_i2c_read_device(reg, bytes, dest);
	if (ret < 0)
		return ret;

	serial_puts_info("act8930: Read register --");
	serial_put_hex((unsigned int)reg);
	serial_puts_info("data: ");
	for (i = 0; i < bytes; i++) {
		serial_put_hex(buf[i]);
	}

	return 0;
}

static int act8930_write(unsigned char reg, int bytes, void *src)
{
	unsigned char *buf = src;
	int i;
	serial_puts_info("act8930: Write register --");
	serial_put_hex((unsigned int)reg);
	serial_puts_info("data: ");
	for (i = 0; i < bytes; i++) {
		serial_put_hex(buf[i]);
	}

	return act8930_i2c_write_device(reg, bytes, src);
}
 
static unsigned char act8930_reg_read(unsigned char reg)
{
	unsigned char val;
	unsigned char ret;

	ret = act8930_read(reg, 1, &val);

	if (ret < 0)
		return ret;
	else
		return val;
}

static unsigned char act8930_reg_write(unsigned char reg, unsigned char val)
{
	unsigned char ret;

	ret = act8930_write(reg, 1, &val);

	return ret;
}

int act8930_ldo_enable(int voltage_set_reg, int voltage)
{   
    if(voltage_set_reg > ACT8930_LDO3_VOLTAGE_SET){
        act8930_reg_write(voltage_set_reg, voltage);
        act8930_reg_write((voltage_set_reg + 0x01), ACT8930_POWER_ON);
        if(act8930_reg_read((voltage_set_reg + 0x01)) != 0x81)
            return 1;
        }   
    else{
        act8930_reg_write( voltage_set_reg, voltage);
        act8930_reg_write((voltage_set_reg + 0x02), ACT8930_POWER_ON);
        if(act8930_reg_read((voltage_set_reg + 0x02)) != 0x81 )
            return 1;
        }   
    return 0;
}

int act8930_ldo_disable(int voltage_set_reg)
{   
    if(voltage_set_reg > ACT8930_LDO3_VOLTAGE_SET){
        act8930_reg_write((voltage_set_reg + 0x01), ACT8930_POWER_OFF);
        if(act8930_reg_read((voltage_set_reg +0x01)) !=0x1) 
            return 1;   
    }   
    else{
        act8930_reg_write((voltage_set_reg + 0x02), ACT8930_POWER_OFF);
        if(act8930_reg_read((voltage_set_reg +0x02)) !=0x1)
            return 1;
    }
    return 0;
}

/*************************************************************************/
#ifdef ACT8930_LDO_MSC0
void act8930_ldo_msc0_poweron(void)
{
	act8930_ldo_enable(ACT8930_LDO_MSC0,ACT8930_MSC0_VOLTAGE);
}

void act8930_ldo_msc0_poweroff(void)
{
 	act8930_ldo_disable(ACT8930_LDO_MSC0);
}
#endif

#ifdef ACT8930_LDO_LCD
void act8930_ldo_lcd_poweron(void)
{
	act8930_ldo_enable(ACT8930_LDO_LCD,ACT8930_LCD_VOLTAGE);
}

void act8930_ldo_lcd_poweroff(void)
{
 	act8930_ldo_disable(ACT8930_LDO_LCD);
}
#endif

#ifdef ACT8930_LDO_WIFI
void act8930_ldo_wifi_poweron(void)
{	
	act8930_ldo_enable(ACT8930_LDO_WIFI,ACT8930_WIFI_VOLTAGE);
}

void act8930_ldo_wifi_poweroff(void)
{
 	act8930_ldo_disable(ACT8930_LDO_WIFI);
}
#endif

#ifdef ACT8930_LDO_COMPASS
void act8930_ldo_compass_poweron(void)
{
	act8930_ldo_enable(ACT8930_LDO_COMPASS,ACT8930_COMPASS_VOLTAGE);
}

void act8930_ldo_compass_poweroff(void)
{
 	act8930_ldo_disable(ACT8930_LDO_COMPASS);
}
#endif
/*************************************************************************/

