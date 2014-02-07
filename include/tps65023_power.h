/*
 * tps65023_power.h -- Core interface for TPS65023
 *
 */

#ifndef __TPS65023_POWER_H__
#define __TPS65023_POWER_H__


#define TPS65023_I2C_ADDR      0x48

#define TPS65023_VERSION_REG   0X00
#define TPS65023_PGOODZ_REG    0X01
#define TPS65023_MASK_REG      0X02
#define TPS65023_REG_CTRL_REG  0X03
#define TPS65023_CON_CTRL_REG  0X04
#define TPS65023_CON_CTRL2_REG 0X05
#define TPS65023_DEFCORE_REG   0X06
#define TPS65023_DEFSLEW_REG   0X07
#define TPS65023_LDO_CTRL_REG  0X08

#define TPS65023_LDO1_SHIFT    0
#define TPS65023_LDO2_SHIFT    4

#define TPS65023_LDO1_EN       (1 << 1)
#define TPS65023_LDO2_EN       (1 << 2)
#define TPS65023_DCDC3_EN      (1 << 3)
#define TPS65023_DCDC2_EN      (1 << 4)
#define TPS65023_DCDC1_EN      (1 << 5)


#define GPIO_PMU_LDO_EN        ( 32 * 2 + 1 )

#define TPS65023_LDO1          1
#define TPS65023_LDO2          2

#define TPS65023_DCDC1         1
#define TPS65023_DCDC2         2
#define TPS65033_DCDC3         3

#define TPS65023_LDO1_1_8      0X03
#define TPS65023_LDO2_3_3      0X07
void tps65023_ldo_hdmi_poweron(void);
void tps65023_ldo_hdmi_poweroff(void);
int tps65023_core_output_set(int regvalue);

#endif
