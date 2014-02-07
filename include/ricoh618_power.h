/*
 * ricoh618_power.h -- Core interface for RICOH618
 *
 */

#ifndef __RICOH618_POWER_H__
#define __RICOH618_POWER_H__


#define RICOH618_I2C_ADDR   					0x32
#define ATC8930_I2C_ADDR   					0x5A

/*
 * Register values.
 */
#if 1
#define RICOH618_LDO2_VOLTAGE_SET    0X4D
#define RICOH618_LDO5_VOLTAGE_SET    0X50

//#else
/*
 * Register values.
 */
#define RICOH618_OUT1        1
#define RICOH618_OUT2        2
#define RICOH618_OUT3        3
#define RICOH618_LDO2		10
#define	RICOH618_LDO5		11

#define RICOH618_OUT_ON      1
#define RICOH618_OUT_OFF     0

#define RICOH618_REG1_VSET       0x36
#define RICOH618_REG2_VSET       0x37
#define RICOH618_REG3_VSET       0x38


#endif

/*
  * voltage configure
  */
#define RICOH618_1_8     0x24
#define RICOH618_3_3     0x39

/*
  * voltage control
*/

/*
  * Function list:
  */
void ricoh618_ldo_hdmi_poweron(void);
void ricoh618_ldo_hdmi_poweroff(void);
void ricoh618_vbus_disable(void);
int ricoh618_output_set(int outnum,int regvalue);
int ricoh618_ldo_enable(int voltage_set_reg,int voltage);
int ricoh618_ldo_disable(int voltage_set_reg);
#endif  /* __RICOH618_POWER_H__ */
