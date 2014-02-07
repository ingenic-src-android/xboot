#include <config.h>
#include <common.h>
#include <matrix_keypad.h>

#if defined(CONFIG_MATRIX_KEYPAD)
static inline void pin_as_input_nopull(int pin)
{
	__gpio_as_input(pin);
	__gpio_disable_pull(pin);
	return;
}

static inline void row_as_input(int pin)
{
	__gpio_as_input(pin);
	__gpio_enable_pull(pin);
	return;
}

static inline void all_cols_as_input(int col_num)
{
	int i = 0;
	for (i = 0;i < col_num ; i ++)
		__gpio_as_input(key_cols[i]);
	return;
}

static inline void col_as_output(int pin)
{
	__gpio_as_output(pin);
	__gpio_disable_pull(pin);
	__gpio_clear_pin(pin);
	return;
}

static inline int get_status(int pin)
{

#define GPIO_DEBOUNCE  20

	int cnt = GPIO_DEBOUNCE,v = 0, t = 0;

	while (cnt--) {
		t = !!__gpio_get_pin(pin);
		if (v != t) {
			cnt = GPIO_DEBOUNCE;
			mdelay(1);;
		}
		v = t;
	}

	return v;
}

int key_status(const int key[2],int row_num,int col_num)
{
	int v = 0,i = 0,j = 0;

	if (0 == key[1] || 1 == key[1]) {
		pin_as_input_nopull(key[0]);
		if (key[1] == get_status(key[0]))
			return KEY_DOWN;
		else
			return KEY_UP;
	} else {
		for (i = 0;i < row_num;i ++) {
			row_as_input(key_rows[i]);
			for (j = 0;j < col_num;j ++) {
				all_cols_as_input(col_num);
				col_as_output(key_cols[j]);
				v = get_status(key_rows[i]);
				if (key[0] == key_rows[i] && key[1] == key_cols[j] && 0 == v) {
					return KEY_DOWN;
				}
			}
		}
		return KEY_UP;
	}
}
#endif
