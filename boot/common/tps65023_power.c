/*
 * PMU driver for tps65023 PMU
 *
 */
#include <config.h>
#include <tps65023_power.h>
#include <simulate_i2c.h>
#include <serial.h>
#include <common.h>
#define EIO 2
/*************************************************************************/
static int tps65023_i2c_read_device(unsigned char reg, int bytes, void *dest)
{
    int ret;
	
	ret = simulate_i2c_read((unsigned char)TPS65023_I2C_ADDR, (unsigned char *)dest, reg, bytes);
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
static int tps65023_i2c_write_device(unsigned char reg, int bytes, void *src)
{
    int ret;

	ret = simulate_i2c_write((unsigned char)TPS65023_I2C_ADDR, (unsigned char *)src, reg, bytes);
	if (ret < 0)
		return ret;
	if (ret < bytes)
		return -EIO;
    
	return 0;    
}

static int tps65023_read(unsigned char reg, int bytes, void *dest)
{
    int ret, i;
	unsigned char *buf = dest;

	ret = tps65023_i2c_read_device(reg, bytes, dest);
	if (ret < 0)
		return ret;

	serial_puts_info("tps65023: Read register --");
	serial_put_hex((unsigned int)reg);
	serial_puts_info("data: ");
	for (i = 0; i < bytes; i++) {
		serial_put_hex(buf[i]);
	}

	return 0;
}

static int tps65023_write(unsigned char reg, int bytes, void *src)
{
    unsigned char *buf = src;
	int i;
	serial_puts_info("tps65023: Write register --");
	serial_put_hex((unsigned int)reg);
	serial_puts_info("data: ");
	for (i = 0; i < bytes; i++) {
		serial_put_hex(buf[i]);
	}

	return tps65023_i2c_write_device(reg, bytes, src);

}
 
static unsigned char tps65023_reg_read(unsigned char reg)
{
    unsigned char val;
	unsigned char ret;

	ret = tps65023_read(reg, 1, &val);

	if (ret < 0)
		return ret;
	else
		return val;

}

static unsigned char tps65023_reg_write(unsigned char reg, unsigned char val)
{
    unsigned char ret;

	ret = tps65023_write(reg, 1, &val);

	return ret;
}

int tps65023_ldo_enable(int voltage_set_reg, int voltage)
{   

   int timeout = 100;
   int reg_ctrl = 0;
   int ldo_shift = 0;
   int ldo_en = 0;
   int ldo_voltage = 0;
   
   if(voltage_set_reg == TPS65023_LDO1){
       ldo_shift = TPS65023_LDO1_SHIFT;
       ldo_en = TPS65023_LDO1_EN;
   }else if(voltage_set_reg == TPS65023_LDO2){
       ldo_shift = TPS65023_LDO2_SHIFT;
       ldo_en = TPS65023_LDO2_EN;     
   }
   
   do{
       ldo_voltage = tps65023_reg_read(TPS65023_LDO_CTRL_REG);
       if( ( ( voltage << ldo_shift | ldo_voltage ) & 0x0f ) > TPS65023_HDMI_1_8) {
           serial_puts_info("hdmi voltage too big!!!!!!!");
           return 0;
       }
       tps65023_reg_write(TPS65023_LDO_CTRL_REG, ( voltage << ldo_shift | ldo_voltage ) );
       __gpio_as_output(GPIO_PMU_LDO_EN);
       __gpio_set_pin(GPIO_PMU_LDO_EN);
       reg_ctrl = tps65023_reg_read(TPS65023_REG_CTRL_REG);
       tps65023_reg_write(TPS65023_REG_CTRL_REG, reg_ctrl | ldo_en);
   }while((timeout --) && !(tps65023_reg_read(TPS65023_REG_CTRL_REG)&ldo_en) );   
   if(timeout <= 0){	 
       serial_puts_info("tps65023: Write timeout !!!!!!!");
       return 1;
   }
   
   return 0;
}

int tps65023_ldo_disable(int voltage_set_reg)
{  	
    __gpio_as_output(GPIO_PMU_LDO_EN);
    __gpio_clear_pin(GPIO_PMU_LDO_EN);
}
int tps65023_core_output_set(int regvalue){
    int val;
    tps65023_reg_write(TPS65023_DEFCORE_REG,regvalue);
    val = tps65023_reg_read(TPS65023_CON_CTRL2_REG);
    tps65023_reg_write(TPS65023_CON_CTRL2_REG,(val & (~0x40)) | 0x80);
}

/*************************************************************************/
#ifdef TPS65023_HDMI_3_3
void tps65023_ldo_hdmi_poweron(void)
{
	int error;
	error = tps65023_ldo_enable(TPS65023_LDO_HDMI_1_8,TPS65023_HDMI_1_8);
	if(error)
		serial_puts_info("tps65023: enable hdmi1.8v erro !!!");
	error = tps65023_ldo_enable(TPS65023_LDO_HDMI_3_3,TPS65023_HDMI_3_3);
	if(error)
		serial_puts_info("tps65023: enable hdmi3.3v erro !!!");

}
void tps65023_ldo_hdmi_poweroff(void)
{
 	int error;
	error = tps65023_ldo_disable(TPS65023_LDO_HDMI_1_8);
	if(error)
		serial_puts_info("tps65023: disable hdmi1.8v erro !!!");
	error = tps65023_ldo_disable(TPS65023_LDO_HDMI_3_3);
	if(error)
		serial_puts_info("tps65023: disable hdmi3.3v erro !!!");
}
#endif
/*************************************************************************/
