#ifndef	__DATA_VERIFY_H__
#define	__DATA_VERIFY_H__

#define ENV_SPL          1
#define ENV_BOOTLOADER   2

int data_verify(const void *src, unsigned int special_data_offset, const char env);

#endif
