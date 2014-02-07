/*
 * Copyright (C) 2007 Ingenic Semiconductor Inc.
 * Author: Sun Jiwei <jwsun@ingenic.cn>
 *
 * Simulation KEY use GPIO, can simulate a cycle key and a random key.
 * We can use this function to test other tablet sleep and wake auto.
 *
 */

#include <config.h>
#include <simulation_key.h>
#include <serial.h>
#include <common.h>

#if defined CYCLE_STOP_PIN && defined CYCLE_STOP_PIN

static unsigned int status = 0;

static unsigned int random_fun(void)
{
	static unsigned long r;

	r = (25173 * r + 13849) % 65536;

	return r;
}

static unsigned long random(unsigned long max, unsigned long min)
{
	unsigned long tmp;

	do {
		tmp = random_fun() % (max + 1);
	} while (tmp < min);

	serial_puts_info("The random number is ");
	serial_put_hex(tmp);

	return tmp;
}

static void simulation_begin(void)
{
	__gpio_as_input(CYCLE_BEGIN_PIN);

	if (__gpio_get_pin(CYCLE_BEGIN_PIN) == 0) {
		mdelay(20);
		if (__gpio_get_pin(CYCLE_BEGIN_PIN) == 0) {
			serial_puts_info("Simulation begin!\n");
			status = 1;
		}
	}
}

static void simulation_stop(void)
{
	__gpio_as_input(CYCLE_STOP_PIN);

	if (__gpio_get_pin(CYCLE_STOP_PIN) == 0) {
		mdelay(20);
		if (__gpio_get_pin(CYCLE_STOP_PIN) == 0) {
			status = 0;
			serial_puts_info("Simulation stop\n");
		}
	}
}

static int get_s_delay(unsigned int time)
{
	unsigned int i;

	for (i = 0; i < time; i++) {
		simulation_stop();
		if (status == 0)
			return 1;
		mdelay(1000);
	}

	return 0;
}


/*
 * Function:Use GPIO simulate cycle key.
 * pin:The GPIO which we can use simulation, end of the END_PIN.
 * low_level_time:Low level delay time.
 * high_level_time: High level delay time.
 *
 */
void simulation_cycle_key(unsigned int *pin, unsigned long low_level_time, unsigned long high_level_time)
{
	unsigned int i;
	unsigned int tmp_a, tmp_b;
	unsigned int tmp_c, tmp_d;

	tmp_a = low_level_time / 1000;	/*Get low level delay second time*/
	tmp_b = high_level_time / 1000; /*Get high level delay second time*/
	tmp_c = low_level_time % 1000;	/*Get low level delay millisecond time*/
	tmp_d = high_level_time % 1000;	/*Get high level delay millisecond time*/

	serial_puts_info("Simulate cycle key\n");

	/*Set the simulation GPIO*/
	for (i = 0; (pin[i] != END_PIN) || (pin[i] < END_PIN); i++) {
		serial_put_hex(pin[i]);
		__gpio_as_output(pin[i]);
	}

	if (pin[i] > END_PIN) {
		serial_puts_info("The GPIO set error!\n");
		while(1);
	}

	serial_puts_info("Ok begin\n");

	while (1) {
		simulation_begin();

		while (status) {
			for (i = 0; pin[i] != END_PIN; i++) {
				__gpio_clear_pin(pin[i]);
			}

			if (get_s_delay(tmp_a))
				break;
			mdelay(tmp_c);

			for (i = 0; pin[i] != END_PIN; i++) {
				__gpio_set_pin(pin[i]);
			}
			if (get_s_delay(tmp_b))
				break;
			mdelay(tmp_d);
		}
	}
}

/* Function:Use GPIO simulate random key.
 * pin:The GPIO which we can use simulation, end of the END_PIN.
 * level:The level of delay long time
 * min_time:delay min time, ms
 * max_time:delay max time, ms
 */
void simulation_random_key(unsigned int *pin, unsigned long min_time, unsigned long max_time, unsigned int level)
{
	unsigned int i;
	unsigned long tmp;
	unsigned int tmp_s, tmp_ms;

	serial_puts_info("Simulate random key\n");

	for (i = 0; (pin[i] != END_PIN) || (pin[i] < END_PIN); i++) {
		serial_puts_info("The GPIO is ");
		serial_put_hex(pin[i]);
		__gpio_as_output(pin[i]);
	}

	if (pin[i] > END_PIN) {
		serial_puts_info("The GPIO set error!\n");
		while(1);
	}

	serial_puts_info("Ok begin\n");

	if (level == 1) {
		while (1) {
			simulation_begin();

			while (status) {
				for (i = 0; pin[i] != END_PIN; i++) {
					__gpio_clear_pin(pin[i]);
				}
				mdelay(180);

				for (i = 0; pin[i] != END_PIN; i++) {
					__gpio_set_pin(pin[i]);
				}

				tmp = random(max_time, min_time);
				tmp_s = tmp / 1000;
				tmp_ms = tmp % 1000;
				if (tmp_s != 0) {
					if (get_s_delay(tmp_s))
						break;
				}
				mdelay(tmp_ms);
			}
		}
	} else if (level == 0) {
		while (1) {
			simulation_begin();

			while (status) {
				for (i = 0; pin[i] != END_PIN; i++) {
					__gpio_set_pin(pin[i]);
				}
				mdelay(180);

				for (i = 0; pin[i] != END_PIN; i++) {
					__gpio_clear_pin(pin[i]);
				}

				tmp = random(max_time, min_time);
				tmp_s = tmp / 1000;
				tmp_ms = tmp % 1000;
				if (tmp_s != 0) {
					if (get_s_delay(tmp_s))
						break;
				}
				mdelay(tmp_ms);
			}
		}
	}
}
#endif
