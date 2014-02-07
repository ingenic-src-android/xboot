/*
 * PMU driver for act8600 PMU
 *
 */
#include <config.h>
#ifdef CONFIG_RICOH618

#include <ricoh618_power.h>
#include <simulate_i2c.h>
#include <serial.h>
#include <common.h>
#define EIO 2
/*************************************************************************/
static int ricoh618_i2c_read_device(unsigned char reg, int bytes, void *dest)
{
	int ret;

	ret = simulate_i2c_read((unsigned char)RICOH618_I2C_ADDR, (unsigned char *)dest, reg, bytes);
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
static int ricoh618_i2c_write_device(unsigned char reg, int bytes, void *src)
{
	int ret;

//	ret = simulate_i2c_write((unsigned char)RICOH618_I2C_ADDR, (unsigned char *)src, reg, bytes);
	if (ret < 0)
		return ret;
	if (ret < bytes)
		return -EIO;

	return 0;
}

static int ricoh618_read(unsigned char reg, int bytes, void *dest)
{
	int ret, i;
	unsigned char *buf = dest;

	ret = ricoh618_i2c_read_device(reg, bytes, dest);
	if (ret < 0)
		return ret;
#if 0
	serial_puts_info("act8600: Read register --");
	serial_put_hex((unsigned int)reg);
	serial_puts_info("data: ");
#endif
	for (i = 0; i < bytes; i++) {
		serial_put_hex(buf[i]);
	}

	return 0;
}

static int ricoh618_write(unsigned char reg, int bytes, void *src)
{
	unsigned char *buf = src;
	int i;
#if 0
	serial_puts_info("act8600: Write register --");
	serial_put_hex((unsigned int)reg);
	serial_puts_info("data: ");
#endif
	for (i = 0; i < bytes; i++) {
		serial_put_hex(buf[i]);
	}

	return ricoh618_i2c_write_device(reg, bytes, src);
}

static unsigned char ricoh618_reg_read(unsigned char reg)
{
	unsigned char val;
	unsigned char ret;

	ret = ricoh618_read(reg, 1, &val);

	if (ret < 0)
		return ret;
	else
		return val;
}

unsigned char ricoh618_reg_write(unsigned char reg, unsigned char val)
{
	unsigned char ret;

	ret = ricoh618_write(reg, 1, &val);

	return ret;
}

int act8600_ldo_enable(int voltage_set_reg, int voltage)
{
	return 0;
}

int act8600_ldo_disable(int voltage_set_reg)
{
	return 0;
}
int act8600_output_set(int outnum,int regvalue)
{
	char reg;
	switch(outnum) {
		case RICOH618_OUT1:reg = RICOH618_REG1_VSET;break;
		case RICOH618_OUT2:reg = RICOH618_REG2_VSET;break;
		case RICOH618_OUT3:reg = RICOH618_REG3_VSET;break;
		case RICOH618_LDO2:reg = RICOH618_LDO2_VOLTAGE_SET;break;
		case RICOH618_LDO5:reg = RICOH618_LDO5_VOLTAGE_SET;break;
		default:return -1;
	}

	return ricoh618_reg_write(reg,regvalue);
}

void act8600_vbus_disable(void)
{
/*
 * enable Q3, disable Q1
 */

}

/*************************************************************************/
#ifdef ACT8600_HDMI_3_3
void act8600_ldo_hdmi_poweron(void)
{

}
void act8600_ldo_hdmi_poweroff(void)
{

}
#endif
/*************************************************************************/

#endif // CONFIG_RICOH618
