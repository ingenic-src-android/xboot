#include "io.h"
#include <config.h>
#include <serial.h>
#include <debug.h>
#include <libc.h>
#include "common.h"

extern void it6610_ctrl_init(void);

void hdmi_ctrl_init(void)
{
	serial_puts("hdmi_ctrl_init\n");
#ifdef CONFIG_HDMI_IT6610
	it6610_ctrl_init();
#endif
}
