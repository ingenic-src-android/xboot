#include <config.h>
#include <serial.h>
#include <boot_msg.h>
#include <libc.h>
#include <debug.h>
#include <common.h>

#if defined(CONFIG_XBOOT_LOGO_FILE)
#include <lcd.h>

#define XBOOT_LOGO_ADDR 0x801c8000

static void memset16(void *_ptr, unsigned short val, unsigned count)
{
    unsigned short *ptr = _ptr;
    while (count--)
        *ptr++ = val;
}
static void memset32(void *_ptr,unsigned short val ,unsigned count)
{
    int val_32;
    int rdata,gdata,bdata;
    rdata=val>>11;
    gdata=val>>5 &0x003F;
    bdata=val&0x001F;
    val_32=rdata<<19 | 0x7<<16 | gdata<<10 | 0x3<<8 | bdata<<3 | 0x7;
    unsigned int *ptr=_ptr;
    while(count--)
        *ptr++ = val_32;
}
/* 565RLE image format: [count(2 bytes), rle(2 bytes)] */

int display_fb_rle565(unsigned short *buf, unsigned char * dst_buf,int count) {
    int vm_width, vm_height;
    unsigned int *tmp;
    unsigned short  *ptr;
    unsigned int *bits_32;
    unsigned short *bits_16;
    int bpp;

    bpp = jzfb_get_controller_bpp(lcd_config_info.bpp);
    vm_width = lcd_config_info.modes->xres;
    vm_height = lcd_config_info.modes->yres;
#if 0
    serial_put_hex(bpp);
    serial_put_hex(vm_width);
    serial_put_hex(vm_height);
#endif
    tmp = ptr = (unsigned short *)buf;
    bits_32=(unsigned int *)dst_buf;
    bits_16=(unsigned short *)dst_buf;

    int photo_width,photo_height;
    photo_width=ptr[0];
    photo_height=ptr[1];
    ptr +=2;
#if 0
    serial_put_hex(photo_width);
    serial_put_hex(photo_height);

    serial_puts_msg("-----------------\n");

    serial_put_hex(tmp[0]);
    serial_put_hex(tmp[1]);
    serial_put_hex(tmp[2]);
    serial_put_hex(tmp[3]);
    serial_put_hex(tmp[4]);
    serial_put_hex(tmp[5]);
    serial_put_hex(tmp[6]);
    serial_put_hex(tmp[7]);
#endif
    int width,height,ewidth,eheight,flag;

    height=photo_height<vm_height ? photo_height : vm_height;
    ewidth=vm_width-photo_width;
    eheight=vm_height-photo_height;

    flag=photo_width*photo_height-vm_width*vm_height;

    if (flag<=0) {
        while(count > 0) {
            if(eheight>0)
            {
                if(bpp==16)
                    bits_16 += (eheight/2)*vm_width;
                else
                    bits_32 += (eheight/2)*vm_width;
            }
            while(height > 0) {

                width=photo_width<vm_width ? photo_width : vm_width;
                if(ewidth > 0)
                {
                    if(bpp==16)
                        bits_16 +=ewidth/2;
                    else
                        bits_32 +=ewidth/2;
                }
                while(width > 0)
                {
                    if( count < 0)
                        break;
                    unsigned on = ptr[0];
		    unsigned n = on;
		    if (on > width) n = width;

                    if(bpp==16)
                    {
                        memset16(bits_16, ptr[1], n);
                        bits_16 +=n;
                    }
                    else
                    {
                        memset32(bits_32, ptr[1], n);
                        bits_32 +=n;
                    }

		    if (on > n) {
			    ptr[0] = (unsigned short)(on - n);
		    } else {
			    ptr +=2;
			    count -= 2;
		    }

                    width -=n;
                }

                if (ewidth > 0)
                {
                    if(bpp==16)
                        bits_16 +=ewidth/2;
                    else
                        bits_32 +=ewidth/2;
                } else {
                    int xwidth = -ewidth;

                    while(xwidth>0)
                    {
                        unsigned on = ptr[0];
			unsigned n = on;

			if (on > xwidth) n = xwidth;

			if (on > n) {
				ptr[0] = (unsigned short)(on - n);
			} else {
				ptr +=2;
				count -= 2;
			}

                        xwidth -= n;
                    }

                }

                height -=1;
            }

            if (eheight > 0)
            {
                if(bpp==16)
                    bits_16 += (eheight/2)*vm_width;
                else
                    bits_32 += (eheight/2)*vm_width;
            }

            if( height <= 0 )
                return 0;
        }
    }
    else
    {
        while(count > 0)
        {
            while(height > 0)
            {
                width=photo_width<vm_width ? photo_width : vm_width;
                while(width > 0)
                {
                    if(count < 0)
                        break;
                    unsigned n = ptr[0];
                    if(bpp==16)
                    {
                        memset16(bits_16, ptr[1], n);
                        bits_16 +=n;
                    }
                    else
                    {
                        memset32(bits_32, ptr[1], n);
                        bits_32 +=n;
                    }
                    ptr +=2;
                    count -= 2;
                    width -=n;
                }
                if(width <0 )
                {
                    ptr -=2;
                    count +=2;
                    if(bpp==16)
                        bits_16 +=width;
                    else
                        bits_32 +=width;
                    ptr[0]=-width;
                }
                int extra_width=photo_width-vm_width;
                while(extra_width > 0)
                {
                    unsigned n = ptr[0];
                    ptr +=2;
                    count -= 2;
                    extra_width -=n;
                }
                if(extra_width < 0)
                {
                    ptr -=2;
                    count +=2;
                    ptr[0] = -extra_width;
                }
                height -=1;
            }
            if( height <= 0 )
                break;
        }
    }
    return 0;
}

int load_565_image(unsigned char *src_buf, unsigned char * dst_buf, unsigned len)
{
    display_fb_rle565(src_buf,dst_buf, len);
    return 0;
}

int print_logo(unsigned char *logo, unsigned logo_len)
{

    lcd_fb_base_init();
    drv_lcd_init();
    //	__lcd_special_on();
    __lcd_display_on();
    mdelay(40);
    lcd_clear(0, 1, 1, 0);
    load_565_image(logo, fb_base, logo_len);
    flush_cache_all();
#ifdef DEFAULT_BACKLIGHT_LEVEL
    __lcd_set_backlight_level(DEFAULT_BACKLIGHT_LEVEL);
#else
    __lcd_set_backlight_level(80);
#endif
}

#endif
