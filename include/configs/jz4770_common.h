/*
 * The file define all the common macro for the board based on the JZ4760
 */


#ifndef __JZ4770_COMMON_H__
#define __JZ4770_COMMON_H__

#define DIV_1 0
#define DIV_2 1
#define DIV_3 2
#define DIV_4 3
#define DIV_6 4
#define DIV_8 5
#define DIV_12 6
#define DIV_16 7
#define DIV_24 8
#define DIV_32 9

#define DIV(a,b,c,d,e,f)					\
({								\
	unsigned int retval;					\
	retval = (DIV_##a << CPM_CPCCR_CDIV_BIT)   |		\
		 (DIV_##b << CPM_CPCCR_H0DIV_BIT)  |		\
		 (DIV_##c << CPM_CPCCR_PDIV_BIT)   |		\
		 (DIV_##d << CPM_CPCCR_C1DIV_BIT)  |		\
		 (DIV_##e << CPM_CPCCR_H2DIV_BIT)  |		\
		 (DIV_##f << CPM_CPCCR_H1DIV_BIT);		\
	retval;							\
})

#define PLL_OUT_MAX 1400		/* 1200MHz. */
//#define CFG_CPU_SPEED_11 (1088 * 1000000)
//#define CFG_CPU_SPEED_22 (1200 * 1000000)

//The first time boot

#define __CFG_EXTAL     (CFG_EXTAL / 1000000)
#define __CFG_PLL_OUT   (CFG_CPU_SPEED_1 / 1000000)

#ifdef CFG_PLL1_FRQ
    #define __CFG_PLL1_OUT  ((CFG_PLL1_FRQ)/1000000)    /* Set PLL1 default: 240MHz */
#else
    #define __CFG_PLL1_OUT  (432)                       /* Set PLL1 default: 432MHZ  UHC:48MHZ TVE:27MHZ */
#endif

/*pll_0*/ 
#if (__CFG_PLL_OUT > PLL_OUT_MAX)
	#error "PLL output can NOT more than 1000MHZ"
#elif (__CFG_PLL_OUT > 600)
	#define __PLL_BS1          1
	#define __PLL_OD1          0
#elif (__CFG_PLL_OUT > 300)
	#define __PLL_BS1          0
	#define __PLL_OD1          0
#elif (__CFG_PLL_OUT > 155)
	#define __PLL_BS1          0
	#define __PLL_OD1          1
#elif (__CFG_PLL_OUT > 76)
	#define __PLL_BS1          0
	#define __PLL_OD1          2
#elif (__CFG_PLL_OUT > 47)
	#define __PLL_BS1          0
	#define __PLL_OD1          3
#else
	#error "PLL ouptput can NOT less than 48"
#endif

#define __PLL_NO1		0
#define NR 			(__PLL_NO1 + 1)
#define NO 			(0x1 << __PLL_OD1)
#define __PLL_MO1		(((__CFG_PLL_OUT / __CFG_EXTAL) * NR * NO) - 1)
#define NF 			(__PLL_MO1 + 1)
#define FOUT			(__CFG_EXTAL * NF / NR / NO)

#if ((__CFG_EXTAL / NR > 50) || (__CFG_EXTAL / NR < 10))
	#error "Can NOT set the value to PLL_N"
#endif

#if ((__PLL_MO1 > 127) || (__PLL_MO1 < 1))
	#error "Can NOT set the value to PLL_M"
#endif

#if (__PLL_BS1 == 1)
	#if (((FOUT * NO) > PLL_OUT_MAX) || ((FOUT * NO) < 500))
		#error "FVCO check failed : BS = 1"
	#endif
#elif (__PLL_BS1 == 0)
	#if (((FOUT * NO) > 600) || ((FOUT * NO) < 300))
		#error "FVCO check failed : BS = 0"
	#endif
#endif

#define CPCCR_M_N_OD_1	((__PLL_MO1 << 24) | (__PLL_NO1 << 18) | (__PLL_OD1 << 16) | (__PLL_BS1 << 31))


/**************************************************************************************************************/

#if (__CFG_PLL1_OUT > PLL_OUT_MAX)
	#error "PLL1 output can NO1T more than 1000MHZ"
#elif (__CFG_PLL1_OUT > 600)
	#define __PLL1_BS          1
	#define __PLL1_OD          0
#elif (__CFG_PLL1_OUT > 300)
	#define __PLL1_BS          0
	#define __PLL1_OD          0
#elif (__CFG_PLL1_OUT > 155)
	#define __PLL1_BS          0
	#define __PLL1_OD          1
#elif (__CFG_PLL1_OUT > 76)
	#define __PLL1_BS          0
	#define __PLL1_OD          2
#elif (__CFG_PLL1_OUT > 47)
	#define __PLL1_BS          0
	#define __PLL1_OD          3
#else
	#error "PLL1 ouptput can NOT less than 48"
#endif

#define __PLL1_NO1		0
#define NR1 			(__PLL1_NO1 + 1)
#define NO1 			(0x1 << __PLL1_OD)
#define __PLL1_MO		(((__CFG_PLL1_OUT / __CFG_EXTAL) * NR1 * NO1) - 1)
#define NF1 			(__PLL1_MO + 1)
#define FOUT1			(__CFG_EXTAL * NF1 / NR1 / NO1)

#if ((__CFG_EXTAL / NR1 > 50) || (__CFG_EXTAL / NR1 < 10))
	#error "Can NOT set the value to PLL1_N"
#endif

#if ((__PLL1_MO > 127) || (__PLL1_MO < 1))
	#error "Can NOT set the value to PLL1_M"
#endif

#if (__PLL1_BS == 1)
	#if (((FOUT1 * NO1) > 1000) || ((FOUT1 * NO1) < 500))
		#error "FVCO1 check failed : BS1 = 1"
	#endif
#elif (__PLL1_BS == 0)
	#if (((FOUT1 * NO1) > 600) || ((FOUT1 * NO1) < 300))
		#error "FVCO1 check failed : BS1 = 0"
	#endif
#endif

#define CPCCR1_M_N_OD	((__PLL1_MO << 24) | (__PLL1_NO1 << 18) | (__PLL1_OD << 16) | (__PLL1_BS << 31))



//The second time boot
//#undef __PLL_BS
//#undef __PLL_OD
//#undef __CFG_PLL_OUT   
//#undef __CFG_PLL_OUT   
//#undef __CFG_PLL_OUT   
//#undef __CFG_PLL_OUT   

/*
#undef __PLL_NO		
#undef NR 		
#undef NO 		
#undef __PLL_MO	
#undef NF 	////        retval = __gpio_get_pin(VERSION_GPIO);                        \        __gpio_as_input(VERSION_GPIO);                                  \
#undef FOUT
*/
#ifdef VERSION_GPIO
#define __CFG_PLL_OUT1 (get_cpu_speed()/ 1000000)
#else
#define __CFG_PLL_OUT1 (CFG_CPU_SPEED_2 / 1000000)
#endif
#define get_cpccr()                                                     \
    ({                                                                  \
        int __PLL_BS = 0,__PLL_OD = 0,__PLL_NO = 0,NR2 = 0,NO2 = 0,__PLL_MO = 0,NF2 = 0,FOUT2 = 0,cpccr; \
        if(__CFG_PLL_OUT1 > PLL_OUT_MAX)                                \
            while(1);                                                   \
        else if(__CFG_PLL_OUT1 > 600){                                  \
            __PLL_BS = 1;                                               \
            __PLL_OD = 0;                                               \
        }else if (__CFG_PLL_OUT1 > 300){                                \
            __PLL_BS = 0;                                               \
            __PLL_OD = 0;                                               \
        }else if(__CFG_PLL_OUT1 > 155){                                 \
            __PLL_BS = 0;                                               \
            __PLL_OD = 1;                                               \
        }else if(__CFG_PLL_OUT1 > 76){                                  \
            __PLL_BS = 0;                                               \
            __PLL_OD = 2;                                               \
        }else if(__CFG_PLL_OUT1 > 47){                                  \
            __PLL_BS = 0;                                               \
            __PLL_OD = 3;                                               \
        }else                                                           \
            while(1);                                                   \
                                                                        \
        __PLL_NO = 0;                                                   \
        NR2 = (__PLL_NO + 1);                                           \
        NO2 = (0x1 << __PLL_OD);                                        \
        __PLL_MO = (((__CFG_PLL_OUT1 / __CFG_EXTAL) * NR2 * NO2) - 1);  \
        NF2 = (__PLL_MO + 1);                                           \
        FOUT2 = (__CFG_EXTAL * NF2 / NR2 / NO2);                         \
                                                                        \
        if((__CFG_EXTAL / NR2 > 50) || (__CFG_EXTAL / NR2 < 10))        \
            while(1);                                                   \
                                                                        \
        if((__PLL_MO > 127) || (__PLL_MO < 1))                          \
            while(1);                                                   \
                                                                        \
        if(__PLL_BS == 1){                                              \
            if(((FOUT2 * NO2) > PLL_OUT_MAX) || ((FOUT2 * NO2) < 500))    \
                while(1);                                               \
        }else if(__PLL_BS == 0){                                        \
            if(((FOUT2 * NO2) > 600) || ((FOUT2 * NO2) < 300))            \
                while(1);                                               \
        }                                                               \
        cpccr = 	((__PLL_MO << 24) | (__PLL_NO << 18) | (__PLL_OD << 16) | (__PLL_BS << 31)); \
        cpccr;                                                          \
    })


#define CPCCR_M_N_OD_2	get_cpccr()
#endif /* __JZ4770_COMMON_H__ */
