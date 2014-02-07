/*
 * act8600_power.h -- Core interface for ACT8600
 *
 */

#ifndef __ACT8600_POWER_H__
#define __ACT8600_POWER_H__


#define ATC8930_I2C_ADDR   					0x5A
/*
 * Register values.
 */
#if 1
#define ACT8600_LDO1_VOLTAGE_SET    0X10
#define ACT8600_LDO2_VOLTAGE_SET    0X20
#define ACT8600_LDO3_VOLTAGE_SET    0X30
#define ACT8600_LDO4_VOLTAGE_SET    0X40
#define ACT8600_LDO5_VOLTAGE_SET    0X50
#define ACT8600_LDO6_VOLTAGE_SET    0X60
#define ACT8600_LDO7_VOLTAGE_SET    0X70
#define ACT8600_LDO8_VOLTAGE_SET    0X80

#define ACT8600_LDO1_VOLTAGE_CONTROL    0X12
#define ACT8600_LDO2_VOLTAGE_CONTROL    0X22
#define ACT8600_LDO3_VOLTAGE_CONTROL    0X32
#define ACT8600_LDO4_VOLTAGE_CONTROL    0X41
#define ACT8600_LDO5_VOLTAGE_CONTROL    0X51
#define ACT8600_LDO6_VOLTAGE_CONTROL    0X61
#define ACT8600_LDO7_VOLTAGE_CONTROL    0X71
#define ACT8600_LDO8_VOLTAGE_CONTROL    0X81
//#else
/*
 * Register values.
 */
#define ACT8600_OUT1        1
#define ACT8600_OUT2        2
#define ACT8600_OUT3        3
#define ACT8600_OUT4        4
#define ACT8600_OUT5        5
#define ACT8600_OUT6        6
#define ACT8600_OUT7        7
#define ACT8600_OUT8        8

#define ACT8600_OUT_ON      1
#define ACT8600_OUT_OFF     0

#define ACT8600_REG1_VSET       0x10
#define ACT8600_REG2_VSET       0x20
#define ACT8600_REG3_VSET       0x30
#define ACT8600_REG4_VSET       0x40
#define ACT8600_REG5_VSET       0x50
#define ACT8600_REG6_VSET       0x60
#define ACT8600_REG7_VSET       0x70
#define ACT8600_REG8_VSET       0x80

#define ACT8600_REG1_VCON   0x12
#define ACT8600_REG2_VCON   0x22
#define ACT8600_REG3_VCON   0x32
#define ACT8600_REG4_VCON   0x41

#define ACT8600_REG5_VCON   0x51
#define ACT8600_REG6_VCON   0x61
#define ACT8600_REG7_VCON   0x71
#define ACT8600_REG8_VCON   0x81
    #define ACT8600_REG_VCON_OK     (1 << 0)
    #define ACT8600_REG_VCON_DIS        (1 << 2)
    #define ACT8600_REG_VCON_ON     (1 << 7)

#define ACT8600_APCH_INTR0  0xa1
    #define ACT8600_APCH_INTR0_SUS      (1 << 7)
#define ACT8600_APCH_INTR1  0xa8
    #define ACT8600_APCH_INTR1_INSTAT   (1 << 5)
    #define ACT8600_APCH_INTR1_CHGSTAT  (1 << 4)
    #define ACT8600_APCH_INTR1_INDAT    (1 << 1)
    #define ACT8600_APCH_INTR1_CHGDAT   (1 << 0)
#define ACT8600_APCH_INTR2  0xa9
    #define ACT8600_APCH_INTR2_INCON    (1 << 5)
    #define ACT8600_APCH_INTR2_CHGEOCIN     (1 << 4)
    #define ACT8600_APCH_INTR2_INDIS    (1 << 1)
    #define ACT8600_APCH_INTR2_CHGEOCOUT    (1 << 0)
#define ACT8600_APCH_STAT   0xaa
    #define ACT8600_APCH_STAT_STATE_MASK    (0x30)
    #define ACT8600_APCH_STAT_STATE_PRE (0x30)
    #define ACT8600_APCH_STAT_STATE_CHAGE   (0x20)
    #define ACT8600_APCH_STAT_STATE_EOC (0x10)
    #define ACT8600_APCH_STAT_STATE_SUSPEND (0x00)

#define ACT8600_OTG_CON     0xb0
    #define ACT8600_OTG_CON_Q1      (1 << 7)
    #define ACT8600_OTG_CON_Q2      (1 << 6)
    #define ACT8600_OTG_CON_Q3      (1 << 5)
    #define ACT8600_OTG_CON_VBUSDAT     (1 << 0)
#define ACT8600_OTG_INTR    0xb2
    #define ACT8600_OTG_INTR_INVBUSR    ((1 << 7) | 0x3)
    #define ACT8600_OTG_INTR_INVBUSF    ((1 << 6) | 0x3)

#define ACT8600_SYS0        0x00
#define ACT8600_SYS1        0x01

#endif

/*
  * voltage configure
  */
#define ACT8600_1_8     0x24
#define ACT8600_3_3     0x39

/*
  * voltage control
*/
#define ACT8600_POWER_ON        (0x1 << 7)
#define ACT8600_POWER_OFF          0x0


/*
  * Function list:
  */
void act8600_ldo_hdmi_poweron(void);
void act8600_ldo_hdmi_poweroff(void);
void act8600_vbus_disable(void);
int act8600_output_set(int outnum,int regvalue);
int act8600_ldo_enable(int voltage_set_reg,int voltage);
int act8600_ldo_disable(int voltage_set_reg);
#endif  /* __ACT8600_POWER_H__ */
