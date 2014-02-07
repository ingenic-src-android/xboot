/*
 * PMU driver for act8600 PMU
 *
 */
#include <config.h>
#ifdef CONFIG_ACT8600

#include <act8600_power.h>
#include <simulate_i2c.h>
#include <serial.h>
#include <common.h>
#define EIO 2
/*************************************************************************/
static int act8600_i2c_read_device(unsigned char reg, int bytes, void *dest)
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
static int act8600_i2c_write_device(unsigned char reg, int bytes, void *src)
{
	int ret;

	ret = simulate_i2c_write((unsigned char)ATC8930_I2C_ADDR, (unsigned char *)src, reg, bytes);
	if (ret < 0)
		return ret;
	if (ret < bytes)
		return -EIO;

	return 0;
}

static int act8600_read(unsigned char reg, int bytes, void *dest)
{
	int ret, i;
	unsigned char *buf = dest;

	ret = act8600_i2c_read_device(reg, bytes, dest);
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

static int act8600_write(unsigned char reg, int bytes, void *src)
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

	return act8600_i2c_write_device(reg, bytes, src);
}

static unsigned char act8600_reg_read(unsigned char reg)
{
	unsigned char val;
	unsigned char ret;

	ret = act8600_read(reg, 1, &val);

	if (ret < 0)
		return ret;
	else
		return val;
}

static unsigned char act8600_reg_write(unsigned char reg, unsigned char val)
{
	unsigned char ret;

	ret = act8600_write(reg, 1, &val);

	return ret;
}

int act8600_ldo_enable(int voltage_set_reg, int voltage)
{
	int timeout = 100;
	if(voltage_set_reg > ACT8600_LDO3_VOLTAGE_SET){
		do{
			act8600_reg_write(voltage_set_reg, voltage);
			act8600_reg_write((voltage_set_reg + 0x01), ACT8600_POWER_ON);
		}while((timeout --) && !(act8600_reg_read((voltage_set_reg + 0x01)) == 0x81));
  		if(timeout <= 0){
			serial_puts_info("act8600: Write timeout !!!!!!!");
			return 1;
		}
	}
	else{
		do{
			act8600_reg_write( voltage_set_reg, voltage);
			act8600_reg_write((voltage_set_reg + 0x02), ACT8600_POWER_ON);
		}while((timeout --) && !(act8600_reg_read((voltage_set_reg + 0x02)) == 0x81));
		if(timeout <= 0){
			serial_puts_info("act8600: Write timeout !!!!!!!");
			return 1;
		}
	}
	return 0;
}

int act8600_ldo_disable(int voltage_set_reg)
{
	if(voltage_set_reg > ACT8600_LDO3_VOLTAGE_SET){
		act8600_reg_write((voltage_set_reg + 0x01), ACT8600_POWER_OFF);
		if(act8600_reg_read((voltage_set_reg +0x01)) !=0x1)
			return 1;
	}
	else{
		act8600_reg_write((voltage_set_reg + 0x02), ACT8600_POWER_OFF);
		if(act8600_reg_read((voltage_set_reg +0x02)) !=0x1)
			return 1;
	}
	return 0;
}
int act8600_output_set(int outnum,int regvalue)
{
	char reg;
	switch(outnum) {
	case ACT8600_OUT1:reg = ACT8600_REG1_VSET;break;
	case ACT8600_OUT2:reg = ACT8600_REG2_VSET;break;
	case ACT8600_OUT3:reg = ACT8600_REG3_VSET;break;
	case ACT8600_OUT4:reg = ACT8600_REG4_VSET;break;
	case ACT8600_OUT5:reg = ACT8600_REG5_VSET;break;
	case ACT8600_OUT6:reg = ACT8600_REG6_VSET;break;
	case ACT8600_OUT7:reg = ACT8600_REG7_VSET;break;
	case ACT8600_OUT8:reg = ACT8600_REG8_VSET;break;
	default:return -1;
	}

	return act8600_reg_write(reg,regvalue);
}

void act8600_vbus_disable(void)
{
/*
 * enable Q3, disable Q1
 */
	act8600_reg_write(ACT8600_OTG_CON, ACT8600_OTG_CON_Q3);
}

/*************************************************************************/
#ifdef ACT8600_HDMI_3_3
void act8600_ldo_hdmi_poweron(void)
{

	int error;
	error = act8600_ldo_enable(ACT8600_LDO_HDMI_1_8,ACT8600_HDMI_1_8);
	if(error)
		serial_puts_info("act8600: enable hdmi1.8v erro !!!");
	error = act8600_ldo_enable(ACT8600_LDO_HDMI_3_3,ACT8600_HDMI_3_3);
	if(error)
		serial_puts_info("act8600: enable hdmi3.3v erro !!!");
}
void act8600_ldo_hdmi_poweroff(void)
{
	int error;
	error = act8600_ldo_disable(ACT8600_LDO_HDMI_1_8);
	if(error)
		serial_puts_info("act8600: disable hdmi1.8v erro !!!");
	error = act8600_ldo_disable(ACT8600_LDO_HDMI_3_3);
	if(error)
		serial_puts_info("act8600: disable hdmi3.3v erro !!!");
}
#endif
/*************************************************************************/

#endif // CONFIG_ACT8600
