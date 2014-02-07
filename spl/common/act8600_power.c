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
//#define I2C_READ_OPS
/*************************************************************************/
#ifdef I2C_READ_OPS
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
#endif
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
#ifdef I2C_READ_OPS
static int act8600_read(unsigned char reg, int bytes, void *dest)
{
	int ret, i;
	unsigned char *buf = dest;

	ret = act8600_i2c_read_device(reg, bytes, dest);
	if (ret < 0)
		return ret;

	serial_puts_info("act8600: Read register --");
	serial_put_hex((unsigned int)reg);
	serial_puts_info("data: ");
	for (i = 0; i < bytes; i++) {
		serial_put_hex(buf[i]);
	}

	return 0;
}
#endif
static int act8600_write(unsigned char reg, int bytes, void *src)
{
	unsigned char *buf = src;
	int i;
//	serial_puts_info("act8600: Write register --");
//	serial_put_hex((unsigned int)reg);
//	serial_puts_info("data: ");
//	for (i = 0; i < bytes; i++) {
//		serial_put_hex(buf[i]);
//	}

	return act8600_i2c_write_device(reg, bytes, src);
}
#ifdef I2C_READ_OPS
unsigned char act8600_reg_read(unsigned char reg)
{
	unsigned char val;
	unsigned char ret;

	ret = act8600_read(reg, 1, &val);

	if (ret < 0)
		return ret;
	else
		return val;
}
#endif
static unsigned char act8600_reg_write(unsigned char reg, unsigned char val)
{
	unsigned char ret;

	ret = act8600_write(reg, 1, &val);

	return ret;
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


void core_voltage_regulate_pmu(int vol)
{
	if (vol > 0) {
		unsigned char value = 0;
		unsigned int i = 200000 * (CFG_EXTAL / 2000000);
		serial_puts("core voltage regulate:");serial_put_dec(vol);
		__gpio_as_output(GPIO_SDA);
		__gpio_clear_pin(GPIO_SDA);

		__gpio_as_output(GPIO_SCL);
		__gpio_clear_pin(GPIO_SCL);

		if ((vol >= 1000) && (vol <= 1200)) {
			value = (vol - 600) / 25;
		} else if ((vol > 1200) && (vol <= 1500)) {
			value = vol / 50;
		} else {
			serial_puts("unsupported voltage\n");
			return;
		}
		act8600_output_set(1, value);    //24 --> 1.2V

		__asm__ __volatile__ (
			"\t.set noreorder\n"
			"1:\n\t"
			"bne\t%0, $0, 1b\n\t"
			"addi\t%0, %0, -1\n\t"
			".set reorder\n"
			: "=r" (i)
			: "0" (i)
			);
	} else {
		serial_puts("core voltage regulate:default\n");
	}
}

#endif // CONFIG_ACT8600
