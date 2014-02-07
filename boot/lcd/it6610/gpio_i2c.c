///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   >mcuio.c<
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2009/08/24
//   @fileversion: CAT6611_SAMPLEINTERFACE_1.12
//******************************************/

#include <config.h>
#include <serial.h>
#include "hdmitx.h"

static unsigned int SIO_C = GPIO_HDMI_I2C_SCK;
static unsigned int SIO_D = GPIO_HDMI_I2C_SDA;

#define IT6610SCCB_ID 0x98 //or 0x9A
#define IT6610SCCB_DELAY  10 

#define TIMEOUT         0xffff

#define HIGH                    1
#define LOW                     0

#define GPIO_MODE_IN                    (0<<31)
#define GPIO_MODE_OUT                   (1UL<<31)
#define read_gpio_bit(x)            __gpio_get_pin(x)
#define write_gpio_bit_set(x)       __gpio_set_pin(x)
#define write_gpio_bit_clear(x)     __gpio_clear_pin(x)
static __inline void set_gpio_ctrl(unsigned int gpio)
{
	if(gpio&GPIO_MODE_OUT)
		__gpio_as_output(gpio);
	else
		__gpio_as_input(gpio);
}
#define MAKE_HIGH(_x)  write_gpio_bit_set(_x)
#define MAKE_LOW(_x)   write_gpio_bit_clear(_x)
#define BIT_READ(_x)   read_gpio_bit(_x)
#define CFG_READ(_x)   set_gpio_ctrl(_x | GPIO_MODE_IN)
#define CFG_WRITE(_x)  set_gpio_ctrl(_x | GPIO_MODE_OUT)

#define WAIT_STAB udelay(IT6610SCCB_DELAY)

#if 0
static int i2c_disable(int I2C_ID)
{
	int timeout = TIMEOUT;

	__i2c_disable(I2C_ID);
	while(__i2c_is_enable(I2C_ID) && (timeout > 0)) {
		udelay(1);
		timeout--;
	}
	if(timeout)
		return 0;
	else
		return 1;
}
static int i2c_enable(int I2C_ID)
{
	int timeout = TIMEOUT;

	__i2c_enable(I2C_ID);
	while(__i2c_is_disable(I2C_ID) && (timeout > 0)) {
		mdelay(1);
		timeout--;
	}
	if(timeout)
		return 0;
	else
		return 1;
}
static void disable_i2c_normal(void)
{
	i2c_disable(0);
}
static void enable_i2c_normal(void)
{
	i2c_enable(0);
	__gpio_as_i2c(0);
}
#endif

static void set_scl( BOOL bit_value )
{
//      SCL_PORT = bit_value;
     	if(bit_value)
		MAKE_HIGH(SIO_C);
	else
		MAKE_LOW(SIO_C);
	WAIT_STAB;
}

static void set_sda( BOOL bit_value, BYTE device )
{
//	SDA_PORT = bit_value;
	if(bit_value)
		MAKE_HIGH(SIO_D);
	else
		MAKE_LOW(SIO_D);
	WAIT_STAB;
}

static BOOL get_sda( BYTE device )
{

	//TX0_SDA_PORT = 1;
	//return TX0_SDA_PORT;

	MAKE_HIGH(SIO_D);
	WAIT_STAB;
	return BIT_READ(SIO_D);

}

static void i2c_start( BYTE device )
{
    set_sda( HIGH, device );
    set_scl( HIGH );
    set_sda( LOW, device );
    set_scl( LOW );
}

static void i2c_write( BYTE byte_data, BYTE device )
{
 BYTE bit_cnt, tmp;
 BOOL bit_value;
     for(bit_cnt=0; bit_cnt<8; bit_cnt++) {
         tmp = (byte_data << bit_cnt) & 0x80;
		 bit_value = tmp && 0x80;
         set_sda( bit_value, device );
         set_scl( HIGH );
         set_scl( LOW );
     }
}

static BOOL i2c_wait_ack( BYTE device )
{
    BOOL ack_bit_value;
    set_sda( HIGH, device );
    set_scl( HIGH );
    ack_bit_value = get_sda( device );
    set_scl( LOW );
    return ack_bit_value;
}

static BYTE i2c_read( BYTE device )
{
	BYTE  bit_cnt, byte_data;
	BOOL  bit_value;

	byte_data = 0;
	for(bit_cnt=0; bit_cnt<8; bit_cnt++) {
		set_scl( HIGH );

		bit_value = get_sda( device );

		byte_data = (byte_data << 1) | bit_value;

		set_scl( LOW );
	}

	return byte_data;
}

static void i2c_send_ack( BOOL bit_value, BYTE device )
{
     set_sda( bit_value, device );
     set_scl( HIGH );
     set_scl( LOW );
     set_sda( HIGH, device );
}

static void i2c_end( BYTE device )
{
     set_sda( LOW, device );
     set_scl( HIGH );
     set_sda( HIGH, device );
}

static BOOL i2c_write_byte( BYTE address, BYTE offset, BYTE byteno, BYTE *p_data, BYTE device )
{
	BYTE i;

     i2c_start(device);				// S

     i2c_write(address&0xFE, device);		// slave address (W)
     if( i2c_wait_ack(device)==1 )	{		// As
         i2c_end(device);
	 return 0;
      }

     i2c_write(offset, device);			// offset
     if( i2c_wait_ack(device)==1 )	{		// As
         i2c_end(device);
	 return 0;
     }

     for(i=0; i<byteno-1; i++) {
     	 i2c_write(*p_data, device);		// write d
     	 if( i2c_wait_ack(device)==1 ) {		// As
     	     i2c_end(device);
	     return 0;
         }
         p_data++;
     }
		
     i2c_write(*p_data, device);			// write last d
     if( i2c_wait_ack(device)==1 )	{		// As
     	 i2c_end(device);
	 return 0;
     }
     else {
     	 i2c_end(device);
	 return 1;
     }
}

static BOOL i2c_read_byte( BYTE address, BYTE offset, BYTE byteno, BYTE *p_data, BYTE device )
{
     BYTE i;

 //    printk("i2c_read_byte:offset=0x%x\n",offset);


     i2c_start(device);				// S

     i2c_write(address&0xFE, device);		// slave address (W)
     if( i2c_wait_ack(device)==1 ) {		// As
         i2c_end(device);
         return 0;
     }
     
     i2c_write(offset, device);			// offset
     if( i2c_wait_ack(device)==1 ) {		// As
         i2c_end(device);
         return 0;
     }
     
     i2c_start(device);
     
     i2c_write(address|0x01, device);		// slave address (R)
     if( i2c_wait_ack(device)==1 ) {		// As
         i2c_end(device);
         return 0;
     }
     
     for(i=0; i<byteno-1; i++) {
         *p_data = i2c_read(device);		// read d
         i2c_send_ack(LOW, device);		// Am
//	 printk("p_data[%d]=0x%x\n",i,p_data[i]);  
         p_data++;	    
     }
     
     *p_data = i2c_read(device);			// read last d
    //  printk("p_data[%d]=0x%x\n",i,p_data[i]);
     i2c_send_ack(HIGH, device);			// NAm
     i2c_end(device);

    return 1;
}


///////////////////////////////////////////////////////////////////////////////
// I2C for original function call
///////////////////////////////////////////////////////////////////////////////
BYTE HDMITX_ReadI2C_Byte(BYTE RegAddr)
{
 BYTE p_data;
 
 BYTE ret = i2c_read_byte(IT6610SCCB_ID, RegAddr, 1, &p_data, 0);
 if(0==ret)
   {
	   p_data=0;
//	   printk("read err: reg:0x%x\n",RegAddr);
   }
 return p_data;
}

int HDMITX_WriteI2C_Byte(BYTE RegAddr, BYTE d)
{
 BOOL flag;

 flag = i2c_write_byte(IT6610SCCB_ID, RegAddr, 1, &d, 0);
// printk("HDMITX_WriteI2C_Byte IT6610SCCB_ID=0x%x flag=%d\n",IT6610SCCB_ID,flag);
 return !flag;
}

int HDMITX_ReadI2C_ByteN(BYTE RegAddr, BYTE *pData, int N)
{
 BOOL flag;

 flag = i2c_read_byte(IT6610SCCB_ID, RegAddr, N, pData, 0);
 //printk("HDMITX_ReadI2C_ByteN IT6610SCCB_ID=0x%x flag=%d\n",IT6610SCCB_ID,flag);
 return !flag;
}

int HDMITX_WriteI2C_ByteN(BYTE RegAddr, BYTE *pData, int N)
{
 BOOL flag;

 flag = i2c_write_byte(IT6610SCCB_ID, RegAddr, N, pData, 0);
 //printk("HDMITX_WriteI2C_ByteN IT6610SCCB_ID=0x%x flag=%d\n",IT6610SCCB_ID,flag);
 return !flag;
}

void IT6610I2cPinInit(struct hdmi_it6610_pin_info *pin_info)
{
//	printk("HDMI-GPIO_I2C: SCL(32*%d + %d), SDA(32*%d+%d)\n",
//		SIO_C/32,SIO_C%32,SIO_D/32,SIO_D%32);
}

void IT6610I2cInit(void)
{
//	disable_i2c_normal();
	CFG_WRITE(SIO_C);
	CFG_WRITE(SIO_D);
	mdelay(10);
}

void IT6610I2cDeinit(void)
{
//	enable_i2c_normal();
//	CFG_READ(SIO_C);
//	CFG_READ(SIO_D);
}

///////////////////////////////////////////////////////////
// Function Body
///////////////////////////////////////////////////////////
void
DelayMS(USHORT ms)
{
	USHORT i;
    BYTE j;

    for(i=0; i<ms; i++)
       for( j=0; j<122; j++ );

}

///////////////////////////////////////////////////////////////////////////////
// Start: I2C for jz4760
///////////////////////////////////////////////////////////////////////////////

