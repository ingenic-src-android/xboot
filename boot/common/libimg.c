#include <config.h>
#include <serial.h>
#include <boot_img.h>
#include <boot.h>
#include <libc.h>
#include <debug.h>
int calculate_size(void *buff,int minsize)
{
        struct boot_img_hdr bootimginfo;
        int kernel_actual;
        int ramdisk_actual;
        unsigned int page_mask;
        if(minsize < sizeof(struct boot_img_hdr)){
                serial_puts_info("size too small");
        }
        memcpy(&bootimginfo,buff,sizeof(struct boot_img_hdr));
        page_mask = bootimginfo.page_size - 1;

        kernel_actual = (bootimginfo.kernel_size + page_mask) & (~page_mask);
        ramdisk_actual = (bootimginfo.ramdisk_size + page_mask) & (~page_mask);

        serial_put_hex(kernel_actual);

        serial_put_hex(ramdisk_actual);

        return kernel_actual + ramdisk_actual;
}


// 注意：因最高位一定为“1”，故略去
const unsigned short cnCRC_16 = 0x8005;
// CRC-16 = X16 + X15 + X2 + X0
const unsigned short cnCRC_CCITT = 0x1021;
// CRC-CCITT = X16 + X12 + X5 + X0，据说这个 16 位 CRC 多项式比上一个要好
const unsigned long cnCRC_32 = 0x04C10DB7;
// CRC-32 = X32 + X26 + X23 + X22 + X16 + X11 + X10 + X8 + X7 + X5 + X4 + X2 + X1 + X0
unsigned long Table_CRC[256]; // CRC 表
// 构造 16 位 CRC 表



void BuildTable16( unsigned short aPoly )
{
        unsigned short i, j;
        unsigned short nData;
        unsigned short nAccum;
        for ( i = 0; i < 256; i++ )
        {
                nData = ( unsigned short )( i << 8 );
                nAccum = 0;
                for ( j = 0; j < 8; j++ )
                {
                        if ( ( nData ^ nAccum ) & 0x8000 )
                                nAccum = ( nAccum << 1 ) ^ aPoly;
                        else
                                nAccum <<= 1;
                        nData <<= 1;
                }
                Table_CRC[i] = ( unsigned long )nAccum;
        }
}


// 计算 16 位 CRC 值，CRC-16 或 CRC-CCITT
unsigned short CRC_16( unsigned char * aData, unsigned long aSize )
{
        unsigned long i;
        unsigned short nAccum = 0;
        BuildTable16( cnCRC_16 ); // or cnCRC_CCITT
        for ( i = 0; i < aSize; i++ )
                nAccum = ( nAccum << 8 ) ^ ( unsigned short )Table_CRC[( nAccum >> 8 ) ^ *aData++];
        return nAccum;
}

// 构造 32 位 CRC 表
void BuildTable32( unsigned long aPoly )
{
        unsigned long i, j;
        unsigned long nData;
        unsigned long nAccum;
        for ( i = 0; i < 256; i++ )
        {
                nData = ( unsigned long )( i << 24 );
                nAccum = 0;
                for ( j = 0; j < 8; j++ )
                {
                        if ( ( nData ^ nAccum ) & 0x80000000 )
                                nAccum = ( nAccum << 1 ) ^ aPoly;
                        else
                                nAccum <<= 1;
                        nData <<= 1;
                }
                Table_CRC[i] = nAccum;
        }
}
// 计算 32 位 CRC-32 值
unsigned long CRC_32( unsigned char * aData, unsigned long aSize )
{
        unsigned long i;
        unsigned long nAccum = 0;
        BuildTable32( cnCRC_32 );
        for ( i = 0; i < aSize; i++ )
                nAccum = ( nAccum << 8 ) ^ Table_CRC[( nAccum >> 24 ) ^ *aData++];
        return nAccum;
}

