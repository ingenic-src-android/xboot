#ifndef _IMAGE_H_
#define _IMAGE_H_
int calculate_size(void *buff,int minsize);
unsigned short CRC_16( unsigned char * aData, unsigned long aSize );
unsigned long CRC_32( unsigned char * aData, unsigned long aSize );
#endif /* _IMAGE_H_ */
