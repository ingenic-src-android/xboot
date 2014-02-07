/*
 * act8930_power.h -- Core interface for ACT8930
 *
 */

#ifndef __ACT8930_POWER_H__
#define __ACT8930_POWER_H__


#define ATC8930_I2C_ADDR   					0x5B
/*
 * Register values.
 */
#define ACT8930_LDO1_VOLTAGE_SET    0X20
#define ACT8930_LDO2_VOLTAGE_SET    0X30
#define ACT8930_LDO3_VOLTAGE_SET    0X40
#define ACT8930_LDO4_VOLTAGE_SET    0X50
#define ACT8930_LDO5_VOLTAGE_SET    0X54
#define ACT8930_LDO6_VOLTAGE_SET    0X60
#define ACT8930_LDO7_VOLTAGE_SET    0X64

#define ACT8930_LDO1_VOLTAGE_CONTROL    0X22
#define ACT8930_LDO2_VOLTAGE_CONTROL    0X32
#define ACT8930_LDO3_VOLTAGE_CONTROL    0X42
#define ACT8930_LDO4_VOLTAGE_CONTROL    0X51
#define ACT8930_LDO5_VOLTAGE_CONTROL    0X55
#define ACT8930_LDO6_VOLTAGE_CONTROL    0X61
#define ACT8930_LDO7_VOLTAGE_CONTROL    0X65

/*
  * voltage configure
  */
#define ACT8930_0_6     0x00    /*0.6v*/
#define ACT8930_0_6_5   0x02    /*0.65v*/
#define ACT8930_0_7     0x04
#define ACT8930_0_7_5   0x06
#define ACT8930_0_8     0x08
#define ACT8930_0_8_5   0x0a
#define ACT8930_0_9     0x0c
#define ACT8930_0_9_5   0x0e
#define ACT8930_1_0     0x10    /*1.0v*/
#define ACT8930_1_0_5   0x12    /*1.05v*/
#define ACT8930_1_1     0x14
#define ACT8930_1_1_5   0x16
#define ACT8930_1_2     0x18
#define ACT8930_1_2_5   0x19
#define ACT8930_1_3     0x1a
#define ACT8930_1_3_5   0x1b
#define ACT8930_1_4     0x1c
#define ACT8930_1_4_5   0x1d
#define ACT8930_1_5     0x1e
#define ACT8930_1_5_5   0x1f
#define ACT8930_1_6     0x20    
#define ACT8930_1_6_5   0x21
#define ACT8930_1_7     0x22
#define ACT8930_1_7_5   0x23
#define ACT8930_1_8     0x24
#define ACT8930_1_8_5   0x25
#define ACT8930_1_9     0x26
#define ACT8930_1_9_5   0x27
#define ACT8930_2_0     0x28    /*2.0v*/
#define ACT8930_2_0_5   0x29    /*2.05v*/
#define ACT8930_2_1     0x2a
#define ACT8930_2_1_5   0x2b
#define ACT8930_2_2     0x2c
#define ACT8930_2_2_5   0x2d
#define ACT8930_2_3     0x2e
#define ACT8930_2_3_5   0x2f
#define ACT8930_2_4     0x30    
#define ACT8930_2_5     0x31
#define ACT8930_2_6     0x32
#define ACT8930_2_7     0x33
#define ACT8930_2_8     0x34
#define ACT8930_2_9     0x35
#define ACT8930_3_0     0x36    /*3.0v*/
#define ACT8930_3_1     0x37    /*3.1v*/
#define ACT8930_3_2     0x38
#define ACT8930_3_3     0x39
#define ACT8930_3_4     0x3a
#define ACT8930_3_5     0x3b
#define ACT8930_3_6     0x3c
#define ACT8930_3_7     0x3d
#define ACT8930_3_8     0x3e
#define ACT8930_3_9     0x3f
/*
  * voltage control
  */
#define ACT8930_POWER_ON        (0x1 << 7)
#define ACT8930_POWER_OFF          0x0

#define ACT8930_APCH1   0x71
#define ACT8930_APCH2   0x78
#define ACT8930_APCH3   0x7A

/*ACIN Status. Indicates the state of the ACIN input, 
typically inorder to identify the type of input supply 
connected. Value is1 when ACIN is above the 1.2V 
precision threshold, value is0 when ACIN is below this 
threshold.*/
#define ACT8930_APCH3_ACINSTAT_MASK                             0x02 /*BIT[1]*/

/*
  * Function list:
  */
void act8930_ldo_msc0_poweron(void);
void act8930_ldo_lcd_poweron(void);
void act8930_ldo_wifi_poweron(void);
void act8930_ldo_compass_poweron(void);
void act8930_ldo_msc0_poweroff(void);
void act8930_ldo_lcd_poweroff(void);
void act8930_ldo_wifi_poweroff(void);
void act8930_ldo_compass_poweroff(void);

#endif  /* __ACT8930_POWER_H__ */

