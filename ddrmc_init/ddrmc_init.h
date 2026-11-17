#include	"ddrmc_common.h"
#ifndef __DDRMC_INIT_H
#define __DDRMC_INIT_H

//DDR FREQ define by customer
//#define BOOT_FREQ_POINT DDR_CLK_1600M
#define BOOT_FREQ_POINT DDR_CLK_1333M
//#define BOOT_FREQ_POINT DDR_CLK_1200M
//#define BOOT_FREQ_POINT DDR_CLK_1066M
//#define BOOT_FREQ_POINT DDR_CLK_933M
//#define BOOT_FREQ_POINT DDR_CLK_800M
//#define BOOT_FREQ_POINT DDR_CLK_400M
//#define BOOT_FREQ_POINT DDR_CLK_200M

//DDR test type
#define DDR_test_type 0		//0:bist_test	1:eyeScan_test	2:loopback_test

//bist test configuration by customer
#define bist_test_type 0    //0:WRC  1:WO  2:RO

//eyeScan test configuragion by customer

//loopback test configuration by customer
#define loopback_test_type 0   // 0:loopback_Scan_test  1:loopback_ac_dq_test
#define loopback_ac_en		1  // ac enable
#define loopback_ac1_en		1  // ac1 enable
#define loopback_dq0_ds0_en 1  // dq0_ds0 enable
#define loopback_dq0_ds1_en 1  // dq0_ds1 enable
#define loopback_dq0_ds2_en 1  // dq0_ds2 enable
#define loopback_dq0_ds3_en 1  // dq0_ds3 enable
#define loopback_dq1_ds0_en 1  // dq1_ds0 enable
#define loopback_dq1_ds1_en 1  // dq1_ds1 enable
#define loopback_dq1_ds2_en 1  // dq1_ds2 enable
#define loopback_dq1_ds3_en 1  // dq1_ds3 enable
#define loopback_type 1		   //1:external loopback  0:internal loopback


#define DDR3_FN_MAX		1
#define DDR4_FN_MAX		1
#define DDR5_FN_MAX		1
#define LP3_FN_MAX		1
#define LP4_FN_MAX		1


#define TEST_DEBUG_LOG_LEVEL	0 //0:necessary log, 1:init info log 2:training log and more init flow log

//training turn on/off configuration by customer
/*
	*rf_train_cadsk_en bit[0]
	*rf_train_caeye_en bit[4]
	*rf_train_gate_en  bit[8]
	*rf_train_rddsk_en bit[12]
	*rf_train_rdeye_en bit[16]
	*rf_train_wrlvl_en bit[20]
	*rf_train_wrdsk_en bit[24]
	*rf_train_wreye_en bit[28]

#define DDRPHY_LP3_200_TRAIN_EN	    0x10010000
#define DDRPHY_LP3_400_TRAIN_EN	    0x10010000
#define DDRPHY_LP3_800_TRAIN_EN	    0x11011011
#define DDRPHY_LP3_933_TRAIN_EN	    0x11011011
#define DDRPHY_LP3_1066_TRAIN_EN    0x11011011
*/
#define DDRPHY_DDR4_200_TRAIN_EN	    0x00000000
#define DDRPHY_DDR4_400_TRAIN_EN	    0x10010000
#define DDRPHY_DDR4_800_TRAIN_EN	    0x10110000
#define DDRPHY_DDR4_933_TRAIN_EN	    0x11111100
#define DDRPHY_DDR4_1066_TRAIN_EN       0x11111100
#define DDRPHY_DDR4_1200_TRAIN_EN       0x11111100
#define DDRPHY_DDR4_1333_TRAIN_EN       0x11111100
#define DDRPHY_DDR4_1600_TRAIN_EN       0x11111100

#define DDRPHY_DDR5_200_TRAIN_EN	    0x00000000
#define DDRPHY_DDR5_400_TRAIN_EN	    0x10010000
#define DDRPHY_DDR5_800_TRAIN_EN	    0x10110000
#define DDRPHY_DDR5_933_TRAIN_EN	    0x11111100
#define DDRPHY_DDR5_1066_TRAIN_EN       0x11111100
#define DDRPHY_DDR5_1200_TRAIN_EN       0x11111100
#define DDRPHY_DDR5_1333_TRAIN_EN       0x11111100
#define DDRPHY_DDR5_1600_TRAIN_EN       0x11111100

// for GF55 IO
//#define  D_ARAGIO_SSTL_IO   //not used
//for psram
//#define D_PSRAM_AP   //not used

//#define   D_DRAM_DDR3 1
//#define   D_PINMUX_0 1     //default ddr3 pinmux1  define D_PINMUX_0 for ddr3 pinmux0  
//
//#define   D_DRAM_DDR4 1
//#define   D_DRAM_LPDDR4 1

#define   D_DRAM_DDR5 1

//#define DRAM_DQ_X16_MODE


//#define D_DDR_2CORE

//Control SET DQ ODT VALUE on/off
#define LP3_ODT_EN_FN           0x1
#define LP3_ODT_EN(fn)          ((LP3_ODT_EN_FN>>fn)&0x1)

#define CAEYE_PASS_WIN_MIN  			0x10
#define WREYE_ODT_PASS_WIN_MIN			4
#define RDEYE_ODT_PASS_WIN_MIN			4
#define WREYE_PASS_WIN_MIN		    	0x10
#define RDEYE_PASS_WIN_MIN			    0x10

//DDR4 RD(50%-93%) ODT
#define DDR4_RDEYE_ODT_VREFDQ_MIN		0x50
#define DDR4_RDEYE_ODT_VREFDQ_MAX		0x60
//DDR4 RD(50%) NO ODT
#define DDR4_RDEYE_VREFDQ_MIN			0x40
#define DDR4_RDEYE_VREFDQ_MAX			0x40

//DDR4 WR(60%-80%) ODT
#define DDR4_WREYE_ODT_VREFDQ_MIN		0x00//MR6 60%
#define DDR4_WREYE_ODT_VREFDQ_MAX		0x1F//MR6 80%
//DDR4 WR(50%) NO ODT
#define DDR4_WREYE_VREFDQ_MIN			0x00//MR6
#define DDR4_WREYE_VREFDQ_MAX			0x00//MR6


//PINMUX defined by customer
//#define CA_BIT_PATTERN_AC0_1			0x76543210
//#define CA_BIT_PATTERN_AC0_2			0xfedcba98
//#define CA_BIT_PATTERN_AC0_3			0x76543210
//#define CA_BIT_PATTERN_AC0_4			0xfefcba98
//#define CA_BIT_PATTERN_AC0_5			0xdfff0000
//#define CA_BIT_PATTERN_AC1_1			0xfedcba98
//#define CA_BIT_PATTERN_AC1_2			0xfedcba98
//#define CA_BIT_PATTERN_AC1_3			0x76543211
//#define CA_BIT_PATTERN_AC1_4			0xfedcba9e
//#define CA_BIT_PATTERN_AC1_5			0xfeff0000
//if not swap
//#define DQ_BIT_OUT_PATTERN_DS0		    0x76543210
//#define DQ_BIT_IN_PATTERN_DS0		    0x76543210
//#define DQ_BIT_OUT_IN_PATTERN_DS0	    0x00080008
//#define DQ_BIT_OUT_PATTERN_DS1		    0x76543210
//#define DQ_BIT_IN_PATTERN_DS1		    0x76543210
//#define DQ_BIT_OUT_IN_PATTERN_DS1	    0xFF18FF18
////#define DQ_BIT_OUT_IN_PATTERN_DS1	    0x00080008

#if D_DRAM_DDR4
       #define CA_BIT_PATTERN_AC0_1			0x76543210
       #define CA_BIT_PATTERN_AC0_2			0xfedcba98
       #define CA_BIT_PATTERN_AC0_3			0x765432d0
       #define CA_BIT_PATTERN_AC0_4			0x6edcba98
       #define CA_BIT_PATTERN_AC0_5			0xffff0000

       #define CA_BIT_PATTERN_AC1_1			0x76543210
       #define CA_BIT_PATTERN_AC1_2			0xfedcba98
       #define CA_BIT_PATTERN_AC1_3			0x765432d0
       #define CA_BIT_PATTERN_AC1_4			0xfedcba98
       #define CA_BIT_PATTERN_AC1_5			0xffff0000

       #define DQ_BIT_OUT_PATTERN_DS0		0x76543210
       #define DQ_BIT_IN_PATTERN_DS0		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS0	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS1		0x76543210
       #define DQ_BIT_IN_PATTERN_DS1		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS1	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS2		0x76543210
       #define DQ_BIT_IN_PATTERN_DS2		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS2	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS3		0x76543210
       #define DQ_BIT_IN_PATTERN_DS3		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS3	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS4		0x76543210
       #define DQ_BIT_IN_PATTERN_DS4		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS4	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS5		0x76543210
       #define DQ_BIT_IN_PATTERN_DS5		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS5	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS6		0x76543210
       #define DQ_BIT_IN_PATTERN_DS6		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS6	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS7		0x76543210
       #define DQ_BIT_IN_PATTERN_DS7		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS7	0x00080008
#elif D_DRAM_LPDDR4
       #define CA_BIT_PATTERN_AC0_1			0x76543210
       #define CA_BIT_PATTERN_AC0_2			0xfedcba98
       #define CA_BIT_PATTERN_AC0_3			0x76543200
       #define CA_BIT_PATTERN_AC0_4			0xfedcba98
       #define CA_BIT_PATTERN_AC0_5			0xffff0000

       #define CA_BIT_PATTERN_AC1_1			0x76543210
       #define CA_BIT_PATTERN_AC1_2			0xfedcba98
       #define CA_BIT_PATTERN_AC1_3			0x76543200
       #define CA_BIT_PATTERN_AC1_4			0xfedcba98
       #define CA_BIT_PATTERN_AC1_5			0xffff0000

       #define DQ_BIT_OUT_PATTERN_DS0		0x76543210
       #define DQ_BIT_IN_PATTERN_DS0		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS0	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS1		0x76543210
       #define DQ_BIT_IN_PATTERN_DS1		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS1	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS2		0x76543210
       #define DQ_BIT_IN_PATTERN_DS2		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS2	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS3		0x76543210
       #define DQ_BIT_IN_PATTERN_DS3		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS3	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS4		0x76543210
       #define DQ_BIT_IN_PATTERN_DS4		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS4	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS5		0x76543210
       #define DQ_BIT_IN_PATTERN_DS5		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS5	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS6		0x76543210
       #define DQ_BIT_IN_PATTERN_DS6		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS6	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS7		0x76543210
       #define DQ_BIT_IN_PATTERN_DS7		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS7	0x00080008
#elif D_DRAM_DDR3 
       #define CA_BIT_PATTERN_AC0_1			0x76543210
       #define CA_BIT_PATTERN_AC0_2			0xfedcba98
       #define CA_BIT_PATTERN_AC0_3			0x765432d0
       #define CA_BIT_PATTERN_AC0_4			0x6edcba98
       #define CA_BIT_PATTERN_AC0_5			0xffff0000

       #define CA_BIT_PATTERN_AC1_1			0x76543210
       #define CA_BIT_PATTERN_AC1_2			0xfedcba98
       #define CA_BIT_PATTERN_AC1_3			0x765432d0
       #define CA_BIT_PATTERN_AC1_4			0xfedcba98
       #define CA_BIT_PATTERN_AC1_5			0xffff0000

       #define DQ_BIT_OUT_PATTERN_DS0		0x76543210
       #define DQ_BIT_IN_PATTERN_DS0		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS0	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS1		0x76543210
       #define DQ_BIT_IN_PATTERN_DS1		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS1	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS2		0x76543210
       #define DQ_BIT_IN_PATTERN_DS2		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS2	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS3		0x76543210
       #define DQ_BIT_IN_PATTERN_DS3		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS3	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS4		0x76543210
       #define DQ_BIT_IN_PATTERN_DS4		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS4	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS5		0x76543210
       #define DQ_BIT_IN_PATTERN_DS5		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS5	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS6		0x76543210
       #define DQ_BIT_IN_PATTERN_DS6		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS6	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS7		0x76543210
       #define DQ_BIT_IN_PATTERN_DS7		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS7	0x00080008
#elif D_DRAM_DDR5
       #define CA_BIT_PATTERN_AC0_1			0x76543210
       #define CA_BIT_PATTERN_AC0_2			0xfedcba98
       #define CA_BIT_PATTERN_AC0_3			0x765432d0
       #define CA_BIT_PATTERN_AC0_4			0x6edcba98
       #define CA_BIT_PATTERN_AC0_5			0xffff0000

       #define CA_BIT_PATTERN_AC1_1			0x76543210
       #define CA_BIT_PATTERN_AC1_2			0xfedcba98
       #define CA_BIT_PATTERN_AC1_3			0x765432d0
       #define CA_BIT_PATTERN_AC1_4			0xfedcba98
       #define CA_BIT_PATTERN_AC1_5			0xffff0000

       #define DQ_BIT_OUT_PATTERN_DS0		0x76543210
       #define DQ_BIT_IN_PATTERN_DS0		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS0	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS1		0x76543210
       #define DQ_BIT_IN_PATTERN_DS1		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS1	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS2		0x76543210
       #define DQ_BIT_IN_PATTERN_DS2		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS2	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS3		0x76543210
       #define DQ_BIT_IN_PATTERN_DS3		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS3	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS4		0x76543210
       #define DQ_BIT_IN_PATTERN_DS4		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS4	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS5		0x76543210
       #define DQ_BIT_IN_PATTERN_DS5		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS5	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS6		0x76543210
       #define DQ_BIT_IN_PATTERN_DS6		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS6	0x00080008
       #define DQ_BIT_OUT_PATTERN_DS7		0x76543210
       #define DQ_BIT_IN_PATTERN_DS7		0x76543210
       #define DQ_BIT_OUT_IN_PATTERN_DS7	0x00080008
#endif


#define LPDDR4_RDEYE_VREFDQ_MIN			0x08
#define LPDDR4_RDEYE_VREFDQ_MAX			0x18

#define LPDDR4_WREYE_VREFDQ_MIN			0x00
#define LPDDR4_WREYE_VREFDQ_MAX			0x01

#define LPDDR4_WREYE_ODT_VREFDQ_MIN	    0x00 //MR14
#define LPDDR4_WREYE_ODT_VREFDQ_MAX	    0x32 //MR14

#define LPDDR4_CAEYE_VREF_MIN			0x00//MR12
#define LPDDR4_CAEYE_VREF_MAX			0x32//MR12


typedef enum CFG_TYPE
{
	CFG_DIS=0,
	CFG_EN=1
}CFG_TYPE_E;

typedef enum DRAM_MR_TYPE
{
	DRAM_MR_0=0,
	DRAM_MR_1=1,
	DRAM_MR_2=2,
	DRAM_MR_3=3,
	DRAM_MR_4=4,
	DRAM_MR_5=5,
	DRAM_MR_6=6,
	DRAM_MR_7=7,
	DRAM_MR_8=8,
	DRAM_MR_9=9,
	DRAM_MR_10=10,
	DRAM_MR_11=11,
	DRAM_MR_12=12,
	DRAM_MR_13=13,
	DRAM_MR_14=14,
	DRAM_MR_15=15,
	DRAM_MR_16=16,
	DRAM_MR_22=22,
	DRAM_MR_25=25,
	DRAM_MR_26=26,
	DRAM_MR_27=27,
	DRAM_MR_28=28,
	DRAM_MR_29=29,
	DRAM_MR_30=30,
	DRAM_MR_63=63
}DRAM_MR_TYPE_E;

typedef enum DRAM_TYPE
{
	DRAM_LP3=0x30,
	DRAM_LP4=0x40,
	DRAM_DDR3=0x50,
	DRAM_DDR4=0x60,
	DRAM_DDR5=0x70,
    DRAM_LP4X=0x41
}DRAM_TYPE_E;

typedef enum DRAM_DQ_WIDTH
{
	DRAM_DQ_X16=0x0,
	DRAM_DQ_X32=0x1,
	DRAM_DQ_X64=0x2,
	DRAM_DQ_X08=0x3
}DRAM_DQ_WIDTH_E;

typedef enum DRAM_CS_TYPE
{
	DRAM_CS_0=0,
	DRAM_CS_1=1,
	DRAM_CS_ALL=2
}DRAM_CS_TYPE_E;

typedef enum DRAM_LP3_DS_TYPE
{
	DRAM_LP3_DS_34=0x1,
	DRAM_LP3_DS_40=0x2,
	DRAM_LP3_DS_48=0x3,
	DRAM_LP3_DS_60=0x4,
	DRAM_LP3_DS_80=0x5,
	DRAM_LP3_DS_120=0x6,
	DRAM_LP3_DS_240=0x7
}DRAM_LP3_DS_TYPE_E;

typedef enum DDRPHY_TYPE
{
	DDRPHY_0=0,
	DDRPHY_1,
	DDRPHY_ALL,
	DDRPHY_ECC
}DDRPHY_TYPE_E;

typedef enum DDR_IO_DS_TYPE
{
	DS_RFU=0,
	DS_240=1,
	DS_120=2,
	DS_80=3,
	DS_60=4,
	DS_48=5,
	DS_40=6,
	DS_34=7
}DDR_IO_DS_TYPE;

typedef enum DDR_IO_ODT_TYPE
{
	ODT_RFU=0,
	ODT_240=1,
	ODT_120=2,
	ODT_80=3,
	ODT_60=4,
	ODT_48=5,
	ODT_40=6,
	ODT_34=7
}DDR_IO_ODT_TYPE;

typedef struct DDRMC_TMG
{
	u32 dram_mr0_data;
	u32 dram_mr1_data;
	u32 dram_mr2_data;
	u32 dram_mr3_data;
	u32 dram_mr4_data;
	u32 dram_mr5_data;
	u32 dram_mr6_data;
	u32 dram_mr11_data;
	u32 dram_mr12_data;
	u32 dram_mr22_data;
	u32 ddrmc_tmg0;
	u32 ddrmc_tmg1;
	u32 ddrmc_tmg2;
	u32 ddrmc_tmg3;
	u32 ddrmc_tmg4;
	u32 ddrmc_tmg5;
	u32 ddrmc_tmg6;
	u32 ddrmc_tmg7;
	u32 ddrmc_tmg8;
	u32 ddrmc_tmg9;
	u32 ddrmc_tmg10;
	u32 ddrmc_tmg11;
	u32 ddrmc_tmg12;
	u32 ddrmc_tmg13;
	u32 ddrmc_tmg14;
	u32 ddrmc_tmg15;
	u32 ddrmc_tmg16;
	u32 ddrmc_tmg17;
	u32 ddrmc_tmg18;
	u32 ddrmc_tmg19;
	u32 ddrmc_tmg20;
}DDRMC_TMG_T;

typedef struct DDRPHY_TMG
{
	u32 ddr_clk;
	u32 train_flag;
	u32 gen_tmg0;		//offset:0x0100
	u32 gen_tmg1;		//offset:0x0104
	u32 gen_tmg2;		//offset:0x0108
	u32 gen_tmg3;		//offset:0x010C
	u32 gen_tmg4;		//offset:0x0110
	u32 gen_tmg5;		//offset:0x0114
	u32 gen_tmg6;		//offset:0x0118
	u32 gen_tmg7;		//offset:0x011C
	u32 gen_tmg8;		//offset:0x0120
	u32 gen_tmg9;		//offset:0x0124
	u32 gen_tmg10;		//offset:0x0128
	u32 gen_tmg11;		//offset:0x012C
	u32 gen_tmg12;		//offset:0x0130
	u32 gen_tmg13;		//offset:0x0134
	u32 ac_tmg0;        //offset:0x0900
	u32 ac_tmg1;        //offset:0x0904
	u32 ac_tmg2; 	    //offset:0x0908
	u32 ds0_tmg0;		//offset:0x0C00
	u32 ds0_tmg1;		//offset:0x0C04
	u32 ds0_tmg2;		//offset:0x0C08
	u32 ds0_tmg3;		//offset:0x0C0C
	u32 ds0_tmg4;		//offset:0x0C10
	u32 ds0_tmg5;		//offset:0x0C14
	u32 ds0_tmg6;		//offset:0x0C18
	u32 ds1_tmg0;		//offset:0x0F00
	u32 ds1_tmg1;		//offset:0x0F04
	u32 ds1_tmg2;		//offset:0x0F08
	u32 ds1_tmg3;		//offset:0x0F0C
	u32 ds1_tmg4;		//offset:0x0F10
	u32 ds1_tmg5;		//offset:0x0F14
	u32 ds1_tmg6;		//offset:0x0F18
	u32 ds2_tmg0;		//offset:0x0C00
	u32 ds2_tmg1;		//offset:0x0C04
	u32 ds2_tmg2;		//offset:0x0C08
	u32 ds2_tmg3;		//offset:0x0C0C
	u32 ds2_tmg4;		//offset:0x0C10
	u32 ds2_tmg5;		//offset:0x0C14
	u32 ds2_tmg6;		//offset:0x0C18
	u32 ds3_tmg0;		//offset:0x0F00
	u32 ds3_tmg1;		//offset:0x0F04
	u32 ds3_tmg2;		//offset:0x0F08
	u32 ds3_tmg3;		//offset:0x0F0C
	u32 ds3_tmg4;		//offset:0x0F10
	u32 ds3_tmg5;		//offset:0x0F14
	u32 ds3_tmg6;		//offset:0x0F18
}DDRPHY_TMG_T;

typedef struct DDRMC_FREQ_INFO
{
	u32 ddr_clk;
	u32 freq_sel;
//	u32 ratio;
//	u32 clk_mode;
}DDRMC_FREQ_INFO_T;


/********************************************Train****************************************/
#define TRAIN_EN 1
#define TRAIN_DIS 0

#define LP3_TRAIN_START_FN	0
#define LP4_TRAIN_START_FN	0
#define DDR4_TRAIN_START_FN	0
#define DDR5_TRAIN_START_FN	0


typedef enum TRAIN_CTRL_TYPE
{
	TRAIN_START_DIS=0,
	TRAIN_START_EN,
	TRAIN_CLEAR_DIS,
	TRAIN_CLEAR_EN
}TRAIN_CTRL_TYPE_E;

typedef enum TRAIN_TYPE
{
	TRAIN_CADSK_INDEX=4,
	TRAIN_CAEYE_INDEX=5,
	TRAIN_GATE_INDEX=6,
	TRAIN_RDDSK_INDEX=7,
	TRAIN_RDEYE_INDEX=8,
	TRAIN_WRLVL_INDEX=9,
	TRAIN_WRDSK_INDEX=10,
	TRAIN_WREYE_INDEX=11,
	TRAIN_ALL_INDEX
}TRAIN_TYPE_E;


void ddrphy_train(u32 ddr_clk,u32 debug_flag);

typedef struct QOS_TIMEOUT_PORT
{
	u32 pri_rd_ch;
	u32	pri_wr_ch;
	u32 timeout_thr_rd_ch;
	u32 timeout_thr_wr_ch;
}QOS_TIMEOUT_PORT_T;

typedef struct DRAM_INFO
{
	DRAM_TYPE_E dram_type;
	DRAM_DQ_WIDTH_E dram_dq_width;
	u32 dram_detect_type;
	u32 intlv_size;
	u32 fn_max;
	u32 cs_num;
	u64 cs0_size;
	u64 cs1_size;
	u32 mr_reg_cs0;
	u32 mr_reg_cs1;
	u8  ddr_freq;
	u8 wreye_vref_train_value;
}DRAM_INFO_T;

/*
*bit[0]:DDRPHY_DS_CA enable
*bit[1]:DDRPHY_DS_DQ enable
*bit[2]:DDRPHY_ODT_DQ enable
*bit[3]:DDRPHY_VREF_DQ enable
*bit[8]:DRAM_DS_DQ enable
*/
//IO Config defined by customer
#define DDRPHY_DQ_ODTP_LP3		ODT_60
#define DDRPHY_DQ_ODTN_LP3		ODT_RFU
#define DRAM_DQ_ODT_LP3			ODT_RFU
#define WR_DBI_EN_LP3			0x0
#define RD_DBI_EN_LP3			0x0

#define DDRMC_PORT_NUM	7

#define INTERLEAVE_0_1
#define RF_LINEAR_BASE_CORE0	0x0
#define RF_LINEAR_BASE_CORE1	0x200
#define RF_INTERLEAVE_BASE		0x400
#define RF_INTERLEAVE_OFFSET	0x0

enum BIST_DATA_PATTERN_MODE
{
	USER_DATA_PATTERN,
	SIPI_DATA_PATTERN,
	LFSR_DATA_PATTERN
};

enum BIST_MODE
{
	BIST_WRITE=0,
	BIST_READ,
	BIST_ALLWRC,
	BIST_ONEWRC
};

//USER PATTERN
#define	BIST_DATA_00_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_01_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_02_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_03_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_04_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_05_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_06_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_07_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_08_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_09_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_10_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_11_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_12_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_13_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_14_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_15_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_16_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_17_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_18_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_19_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_20_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_21_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_22_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_23_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_24_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_25_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_26_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_27_PATTERN		0x00000000  //0x00000000 
#define	BIST_DATA_28_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_29_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_30_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 
#define	BIST_DATA_31_PATTERN		0xFFFFFFFF  //0xFFFFFFFF 

//SIPI PATTERN
#define	BIST_SIPI_DATA_00			0x38ccccaa
#define	BIST_SIPI_DATA_01			0xe1e1e38e
#define	BIST_SIPI_DATA_02			0x111249e1
#define	BIST_SIPI_DATA_03			0xdb084211
#define	BIST_SIPI_DATA_04			0xdef77776
#define	BIST_SIPI_DATA_05			0x3333557b
#define	BIST_SIPI_DATA_06			0x1e1c71c7
#define	BIST_SIPI_DATA_07			0xedb61e1e
#define	BIST_SIPI_DATA_08			0xf7bdeeee
#define	BIST_SIPI_DATA_09			0x08888924
#define	BIST_SIPI_DATA_10			0x00008421
#define	BIST_SIPI_DATA_11			0x00000000
#define	BIST_SIPI_DATA_12			0x00000000
#define	BIST_SIPI_DATA_13			0x00000000
#define	BIST_SIPI_DATA_14			0x00000000
#define	BIST_SIPI_DATA_15			0x00000000
#define	BIST_SIPI_BIT_PATTERN_0		0x55555554
#define	BIST_SIPI_BIT_PATTERN_1		0x55555554

//LFSR PATTERN
#define	BIST_DATA_LFSR_SEED_L0		0x12345678
#define	BIST_DATA_LFSR_SEED_L1		0x89abcdef
#define	BIST_DATA_LFSR_SEED_H0		0x8abce529
#define	BIST_DATA_LFSR_SEED_H1		0xfbed20ec

void ddrphy_fifo_reset();
void soc_dfs_go(u32 fn);
int dram_bist_test(u32 start);
void ddrmc_mrw(DRAM_MR_TYPE_E mr_num, DRAM_CS_TYPE_E cs_num, u32 mr_data);
int loopback_test();

#endif

