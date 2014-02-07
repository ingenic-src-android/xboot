/*
 * PMU driver for ricoh618 PMU
 *
 */
#include <config.h>

#ifdef CONFIG_RICOH618

#include <ricoh618_power.h>
#include <simulate_i2c.h>
#include <serial.h>
#include <common.h>
#define EIO 2
#define I2C_READ_OPS
/*************************************************************************/
#ifdef I2C_READ_OPS
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
#endif
/* 	Currently we allocate the write buffer on the stack; this is OK for
  * 	small writes - if we need to do large writes this will need to be
  * 	revised.
  */
static int ricoh618_i2c_write_device(unsigned char reg, int bytes, void *src)
{
	int ret;

	ret = simulate_i2c_write((unsigned char)RICOH618_I2C_ADDR, (unsigned char *)src, reg, bytes);
	if (ret < 0)
		return ret;
	if (ret < bytes)
		return -EIO;

	return 0;
}
#ifdef I2C_READ_OPS
static int ricoh618_read(unsigned char reg, int bytes, void *dest)
{
	int ret, i;
	unsigned char *buf = dest;

	ret = ricoh618_i2c_read_device(reg, bytes, dest);
	if (ret < 0)
		return ret;

	serial_puts_info("ricoh618: Read register --");
	serial_put_hex((unsigned int)reg);
	serial_puts_info("data: ");
	for (i = 0; i < bytes; i++) {
		serial_put_hex(buf[i]);
	}

	return 0;
}
#endif
static int ricoh618_write(unsigned char reg, int bytes, void *src)
{
	unsigned char *buf = src;
	int i;
//	serial_puts_info("ricoh618: Write register --");
//	serial_put_hex((unsigned int)reg);
//	serial_puts_info("data: ");
//	for (i = 0; i < bytes; i++) {
//		serial_put_hex(buf[i]);
//	}

	return ricoh618_i2c_write_device(reg, bytes, src);
}
#ifdef I2C_READ_OPS
unsigned char ricoh618_reg_read(unsigned char reg)
{
	unsigned char val;
	unsigned char ret;

	ret = ricoh618_read(reg, 1, &val);

	if (ret < 0)
		return ret;
	else
		return val;
}
#endif
unsigned char ricoh618_reg_write(unsigned char reg, unsigned char val)
{
	unsigned char ret;

	ret = ricoh618_write(reg, 1, &val);

	return ret;
}

int ricoh618_output_set(int outnum,int regvalue)
{
	char reg, reg_banksel;
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

int core_voltage_read_ricoh618(int reg)
{

	return ricoh618_reg_read(reg);
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

		if ((vol >= 1000) && (vol <= 1300)) {
			value = (((vol - 600 )*10 + 124) / 125);
		} else {
			serial_puts("unsupported voltage\n");
			return;
		}
		serial_puts("core voltage value :");	serial_put_hex(value);
		ricoh618_output_set(1, value);    //24 --> 1.2V
//		ricoh618_output_set(RICOH618_OUT2, 0x48);

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

#endif // CONFIG_RICOH618
