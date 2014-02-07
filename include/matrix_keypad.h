#ifndef __MATRIX_KEYPAD_H__
#define __MATRIX_KEYPAD_H__

extern const int key_rows[];
extern const int key_cols[];
extern const int key_maps[][2];
extern const int key_pad_r_num;
extern const int key_pad_c_num;

int key_status(const int key[2],int row_num,int col_num);

enum {
	KEY_UP = 0,
	KEY_DOWN,
};

#endif
