#include <config.h>

#if 0
#include <common.h>

#include <serial.h>

static int Tcu_Init_Flag = 1;
static int Tcu_Init_Flag_1 = 1;

static void Tcu_Init(int tcu_number){
	serial_puts_info("Tcu has been inited ...\n");
	switch(tcu_number){
		case 0:
			/*Init TCSR to zero , Default value is 0*/
			REG_TCU_TCSR0 = 0x0;
			/*Init the counter value to zero */
			REG_TCU_TCNT0 = 0x0;
			/*Fill the TDFR0 to the max counter value*/
			REG_TCU_TDFR0 = 0xFFFF;
			/*Fill the TDHR0 to 12000,about 1ms*/
			REG_TCU_TDHR0 = 0x2EE0;
			/*Select the EXT clock as the input clock*/
			REG_TCU_TCSR0 = 0x4;
			/*Tcu_Init_Flag add 1*/
			Tcu_Init_Flag ++;
			return;
		case 1:
			/*Init TCSR to zero , Default value is 0*/
			REG_TCU_TCSR1 = 0x0;
			/*Init the counter value to zero */
			REG_TCU_TCNT1 = 0x0;
			/*Fill the TDFR1 to the max counter value*/
			REG_TCU_TDFR1 = 0xFFFF;
			/*Fill the TDHR1 to 12000,about 1ms*/
			REG_TCU_TDHR1 = 0x2EE0;
			/*Select the EXT clock as the input clock*/
			REG_TCU_TCSR1 = 0x4;
			/*Tcu_Init_Flag_1 add 1*/
			Tcu_Init_Flag_1 ++;
			return;
	}
}
static void Counter_Reset(int tcu_number){

	switch(tcu_number){
		case 0:
			/*Stop the counter*/
			REG_TCU_TECR = REG_TCU_TECR | 0x1;
			/*Clear the counter value*/
			REG_TCU_TCNT0 = 0x0;
			/*Clear the match half flag*/
			REG_TCU_TFCR = REG_TCU_TFCR | TCU_TFR_HFLAG0;
			return;
		case 1:
			/*Stop the counter*/
			REG_TCU_TECR = REG_TCU_TECR | 0x2;
			/*Clear the counter value*/
			REG_TCU_TCNT1 = 0x0;
			/*Clear the match half flag*/
			REG_TCU_TFCR = REG_TCU_TFCR | TCU_TFR_HFLAG1;
			return;
	}
}
static void Counter_Begin(int tcu_number){

	switch(tcu_number){
		case 0 :
			/*Enalbe the TCNT0*/
			REG_TCU_TESR = REG_TCU_TESR | 0x1;
			return;
		case 1 :
			/*Enalbe the TCNT1*/
			REG_TCU_TESR = REG_TCU_TESR | 0x2;
			return;
	}
}

//TCU0 delay
void msdelay(unsigned int ms){

	int i = 0;
	/*Tcu Init once*/
	if(1 == Tcu_Init_Flag)
		Tcu_Init(0);
	for(i=0;i<ms;i++){
		Counter_Begin(0);
		/*Waiting for the count up*/
		while(!(REG_TCU_TFR & TCU_TFR_HFLAG0));
		Counter_Reset(0);
	}
	return;	
}

//TCU1 delay

void ms1delay(unsigned int ms){

	int i = 0;
	/*Tcu Init once*/
	if(1 == Tcu_Init_Flag_1)
		Tcu_Init(1);
	for(i=0;i<ms;i++){
		Counter_Begin(1);
		/*Waiting for the count up*/
		while(!(REG_TCU_TFR & TCU_TFR_HFLAG1));
		Counter_Reset(1);
	}
	return;	
}
#endif
#if 0
void mdelay(unsigned  msecs)
{
	int i, j;
	int wait;
	wait = 0;
	for (i = 0; i < msecs; i++)
		for (j = 0; j < msecs; j++) {
			wait = 1;
		}
}

void udelay(unsigned  usecs)
{
	mdelay(usecs);
}

void my_mdelay(int msec)
{
    int i;
    unsigned long j;
    volatile int wait;

    for (i = 0; i < msec; i++)
    {   
        wait = 0;
        j = CFG_CPU_SPEED;
        j /= 10000;
        while(j--) wait++;
    }   
}
#endif

void udelay(unsigned int usec)
{
	unsigned int i = usec * (CFG_CPU_SPEED / 2000000);

	__asm__ __volatile__ (
		"\t.set noreorder\n"
		"1:\n\t"
		"bne\t%0, $0, 1b\n\t"
		"addi\t%0, %0, -1\n\t"
		".set reorder\n"
		: "=r" (i)
		: "0" (i)
	);
}

void mdelay(unsigned int msec)
{
	unsigned int i;

	for (i = 0; i < msec; i++)
		udelay(1000);
}


