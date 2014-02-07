#ifndef PM_H
#define PM_H

extern int jz_pm_do_hibernate(void);
extern int low_battery_detect(void);
extern int charge_detect(void);
extern void low_battery_warning(void);
extern void charge_logo_display(void);
extern void Jz4760_Do_Charge(void);
extern int poweron_key_long_pressed(void);
extern int set_poweron_key_long_pressed_flag(int flag);
extern int get_poweron_key_long_pressed_flag(void);
extern void low_battery_judgement(void);
#endif
