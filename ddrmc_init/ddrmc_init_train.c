#include "ddrmc_reg_addr.h"
#include "ddrphy_reg_addr.h"
#include "ddrmc_init.h"
//#include "ddr_eye.h"
#include "ddrmc_common.h"
#include "ddrmc_init_train.h"
#include "ddr4_ddrmc_timing.h"

/*******************************Training flow*************************************/
typedef struct TRAIN_CFG_INFO
{
	u32 ddr_clk;
	u32 ddr_freq_num;
	u32 odt_status;
	u32 vrefdq_rd_val;
	u32 train_cs_num;
	u32 train_phy_num;
	u32 train_byte_en;
	u32 train_addr_31_0;
	u32 train_addr_34_32;
	u32 mr6_vref_wr_val;
}TRAIN_CFG_INFO_T;

extern DRAM_INFO_T dram_info;
extern DDRMC_TMG_T *ddrmc_tmg;
extern DDRPHY_TMG_T *ddrphy_tmg;
TRAIN_CFG_INFO_T train_info;


#define BIST_RESERVE_SIZE 0x1000
#define RD_EN_MAX         0x80

//u32 dll_ac_addr[1]	    = {PARM_DDRPHY_AC_TMG1_F0_ADDR};
//u32 dll_pos_addr[4]	    = {PARM_DDRPHY_DS0_TMG3_F0_ADDR, PARM_DDRPHY_DS1_TMG3_F0_ADDR, PARM_DDRPHY_DS0_TMG3_F0_ADDR, PARM_DDRPHY_DS1_TMG3_F0_ADDR};
//u32 dll_neg_addr[4]	    = {PARM_DDRPHY_DS0_TMG4_F0_ADDR, PARM_DDRPHY_DS1_TMG4_F0_ADDR, PARM_DDRPHY_DS0_TMG4_F0_ADDR, PARM_DDRPHY_DS1_TMG4_F0_ADDR};
//u32 dll_wr_addr[4]	    = {PARM_DDRPHY_DS0_TMG1_F0_ADDR, PARM_DDRPHY_DS1_TMG1_F0_ADDR, PARM_DDRPHY_DS0_TMG1_F0_ADDR, PARM_DDRPHY_DS1_TMG1_F0_ADDR};
//u32 dll_diff_addr[4]	= {PARM_DDRPHY_DS0_TMG2_F0_ADDR, PARM_DDRPHY_DS1_TMG2_F0_ADDR, PARM_DDRPHY_DS0_TMG2_F0_ADDR, PARM_DDRPHY_DS1_TMG2_F0_ADDR};
//

/******************************************
*This variable use for distigunish first dfs
*and second dfs in ddrphy_train_fsp_set()
*******************************************/

/***********Training pattern***************/
//CA PATTERN
//#define TRAIN_CA_PATTERN_LP3			0x3ff
//#ifdef DRAM_DQ_X16_MODE
//// 16bit
////WRITE PATTERN
//#define TRAIN_WR_PATTERN_LP3_1			0xffff0000
//#define TRAIN_WR_PATTERN_LP3_2			0xffff0000
//#define TRAIN_WR_PATTERN_LP3_3			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP3_4			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP3_5			0xffff0000
//#define TRAIN_WR_PATTERN_LP3_6			0xffff0000
//#define TRAIN_WR_PATTERN_LP3_7			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP3_8			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP3_9			0xffff0000
//#define TRAIN_WR_PATTERN_LP3_10			0xffff0000
//#define TRAIN_WR_PATTERN_LP3_11			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP3_12			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP3_13			0xffff0000
//#define TRAIN_WR_PATTERN_LP3_14			0xffff0000
//#define TRAIN_WR_PATTERN_LP3_15			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP3_16			0xaaaa5555
////READ PATTERN
//#define TRAIN_RD_PATTERN_LP3_1			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_2			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_3			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_4			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_5			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_6			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_7			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_8			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_9			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_10			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_11			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_12			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_13			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_14			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_15			0x0000ffff
//#define TRAIN_RD_PATTERN_LP3_16			0x0000ffff
//#else
////WRITE PATTERN
//#define TRAIN_WR_PATTERN_LP3_1			0x0
//#define TRAIN_WR_PATTERN_LP3_2			0xffffffff
//#define TRAIN_WR_PATTERN_LP3_3			0x0
//#define TRAIN_WR_PATTERN_LP3_4			0xffffffff
//#define TRAIN_WR_PATTERN_LP3_5			0x55555555
//#define TRAIN_WR_PATTERN_LP3_6			0xaaaaaaaa
//#define TRAIN_WR_PATTERN_LP3_7			0x55555555
//#define TRAIN_WR_PATTERN_LP3_8			0xaaaaaaaa
//#define TRAIN_WR_PATTERN_LP3_9			0x0
//#define TRAIN_WR_PATTERN_LP3_10			0xffffffff
//#define TRAIN_WR_PATTERN_LP3_11			0x0
//#define TRAIN_WR_PATTERN_LP3_12			0xffffffff
//#define TRAIN_WR_PATTERN_LP3_13			0x55555555
//#define TRAIN_WR_PATTERN_LP3_14			0xaaaaaaaa
//#define TRAIN_WR_PATTERN_LP3_15			0x55555555
//#define TRAIN_WR_PATTERN_LP3_16			0xaaaaaaaa
////READ PATTERN
//#define TRAIN_RD_PATTERN_LP3_1			0xffffffff
//#define TRAIN_RD_PATTERN_LP3_2			0x00000000
//#define TRAIN_RD_PATTERN_LP3_3			0xffffffff
//#define TRAIN_RD_PATTERN_LP3_4			0x00000000
//#define TRAIN_RD_PATTERN_LP3_5			0xffffffff
//#define TRAIN_RD_PATTERN_LP3_6			0x00000000
//#define TRAIN_RD_PATTERN_LP3_7			0xffffffff
//#define TRAIN_RD_PATTERN_LP3_8			0x00000000
//#define TRAIN_RD_PATTERN_LP3_9			0xffffffff
//#define TRAIN_RD_PATTERN_LP3_10			0x00000000
//#define TRAIN_RD_PATTERN_LP3_11			0xffffffff
//#define TRAIN_RD_PATTERN_LP3_12			0x00000000
//#define TRAIN_RD_PATTERN_LP3_13			0xffffffff
//#define TRAIN_RD_PATTERN_LP3_14			0x00000000
//#define TRAIN_RD_PATTERN_LP3_15			0xffffffff
//#define TRAIN_RD_PATTERN_LP3_16			0x00000000
//#endif

//#ifdef DRAM_DQ_X16_MODE
////WRITE PATTERN
//#define TRAIN_WR_PATTERN_DDR4_1			0xa5a55aa5
//#define TRAIN_WR_PATTERN_DDR4_2			0xa5a55aa5
//#define TRAIN_WR_PATTERN_DDR4_3			0xa55a5a5a
//#define TRAIN_WR_PATTERN_DDR4_4			0xa55a5a5a
//#define TRAIN_WR_PATTERN_DDR4_5			0xa5a55a5a
//#define TRAIN_WR_PATTERN_DDR4_6			0xa5a55a5a
//#define TRAIN_WR_PATTERN_DDR4_7			0xa5a55a5a
//#define TRAIN_WR_PATTERN_DDR4_8			0xa5a55a5a
//#define TRAIN_WR_PATTERN_DDR4_9			0xaa5555aa
//#define TRAIN_WR_PATTERN_DDR4_10		0x55aa55aa
//#define TRAIN_WR_PATTERN_DDR4_11		0x55aaaa55
//#define TRAIN_WR_PATTERN_DDR4_12		0x55aaaa55
//#define TRAIN_WR_PATTERN_DDR4_13		0x5555aaaa
//#define TRAIN_WR_PATTERN_DDR4_14		0xaaaa5555
//#define TRAIN_WR_PATTERN_DDR4_15		0x5555aaaa
//#define TRAIN_WR_PATTERN_DDR4_16		0xaaaa5555
////READ PATTERN
//#define TRAIN_RD_PATTERN_DDR4_1			0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_2			0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_3			0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_4			0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_5			0xffff0000 
//#define TRAIN_RD_PATTERN_DDR4_6			0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_7			0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_8			0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_9			0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_10		0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_11		0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_12		0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_13		0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_14		0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_15		0xffff0000
//#define TRAIN_RD_PATTERN_DDR4_16		0xffff0000
//#else
//WRITE PATTERN
#define TRAIN_WR_PATTERN_DDR4_1			0x5a5a5aa5
#define TRAIN_WR_PATTERN_DDR4_2			0xa55aa5a5
#define TRAIN_WR_PATTERN_DDR4_3			0x5a5a5aa5
#define TRAIN_WR_PATTERN_DDR4_4			0xa55aa5a5

#define TRAIN_WR_PATTERN_DDR4_5			0x5aa55a5a
#define TRAIN_WR_PATTERN_DDR4_6			0xa5a5a55a
#define TRAIN_WR_PATTERN_DDR4_7			0x5aa55a5a
#define TRAIN_WR_PATTERN_DDR4_8			0xa5a5a55a

#define TRAIN_WR_PATTERN_DDR4_9			0xaa5555aa
#define TRAIN_WR_PATTERN_DDR4_10		0x55aaaa55
#define TRAIN_WR_PATTERN_DDR4_11		0xaa5555aa
#define TRAIN_WR_PATTERN_DDR4_12		0x55aaaa55

#define TRAIN_WR_PATTERN_DDR4_13		0x5555aaaa
#define TRAIN_WR_PATTERN_DDR4_14		0xaaaa5555
#define TRAIN_WR_PATTERN_DDR4_15		0x5555aaaa
#define TRAIN_WR_PATTERN_DDR4_16		0xaaaa5555
//READ PATTERN
#define TRAIN_RD_PATTERN_DDR4_1			0xffff0000
#define TRAIN_RD_PATTERN_DDR4_2			0xffff0000
#define TRAIN_RD_PATTERN_DDR4_3			0xffff0000
#define TRAIN_RD_PATTERN_DDR4_4			0xffff0000
#define TRAIN_RD_PATTERN_DDR4_5			0xffff0000
#define TRAIN_RD_PATTERN_DDR4_6			0xffff0000
#define TRAIN_RD_PATTERN_DDR4_7			0xffff0000
#define TRAIN_RD_PATTERN_DDR4_8			0xffff0000



//#endif


//#ifdef DRAM_DQ_X16_MODE
//    //WRITE PATTERN
//    #define TRAIN_WR_PATTERN_DDR3_1			0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_2			0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_3			0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_4			0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_5			0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_6			0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_7			0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_8			0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_9			0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_10		0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_11		0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_12		0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_13		0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_14		0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_15		0x00000101
//    #define TRAIN_WR_PATTERN_DDR3_16		0x00000101
//    //WRITE DSK PATTERN
//    #define TRAIN_WRDSK_PATTERN_DDR3_1		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_2		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_3		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_4		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_5		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_6		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_7		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_8		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_9		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_10		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_11		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_12		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_13		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_14		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_15		0xffff0000
//    #define TRAIN_WRDSK_PATTERN_DDR3_16		0xffff0000
//    
//    //READ PATTERN
//    #define TRAIN_RD_PATTERN_DDR3_1			0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_2			0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_3			0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_4			0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_5			0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_6			0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_7			0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_8			0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_9			0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_10		0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_11		0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_12		0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_13		0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_14		0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_15		0x01010000
//    #define TRAIN_RD_PATTERN_DDR3_16		0x01010000
//    #define D_DDRPHY_DDR3_DS0_TMG6_800      0x0000000C
//    #define D_DDRPHY_DDR3_DS1_TMG6_800      0x0000000C
//    #define D_DDRPHY_DDR3_DS2_TMG6_800      0x0000000C
//    #define D_DDRPHY_DDR3_DS3_TMG6_800      0x0000000C
//#else
//    //WRITE PATTERN
//    #define TRAIN_WR_PATTERN_DDR3_1			0x01010101
//    #define TRAIN_WR_PATTERN_DDR3_2			0x01010101
//    #define TRAIN_WR_PATTERN_DDR3_3			0x00000000
//    #define TRAIN_WR_PATTERN_DDR3_4			0x00000000
//    #define TRAIN_WR_PATTERN_DDR3_5			0x01010101
//    #define TRAIN_WR_PATTERN_DDR3_6			0x01010101
//    #define TRAIN_WR_PATTERN_DDR3_7			0x00000000
//    #define TRAIN_WR_PATTERN_DDR3_8			0x00000000
//    #define TRAIN_WR_PATTERN_DDR3_9			0x01010101
//    #define TRAIN_WR_PATTERN_DDR3_10		0x01010101
//    #define TRAIN_WR_PATTERN_DDR3_11		0x00000000
//    #define TRAIN_WR_PATTERN_DDR3_12		0x00000000
//    #define TRAIN_WR_PATTERN_DDR3_13		0x01010101
//    #define TRAIN_WR_PATTERN_DDR3_14		0x01010101
//    #define TRAIN_WR_PATTERN_DDR3_15		0x00000000
//    #define TRAIN_WR_PATTERN_DDR3_16		0x00000000
//    //WRITE DSK PATTERN
//    #define TRAIN_WRDSK_PATTERN_DDR3_1		0x00000000
//    #define TRAIN_WRDSK_PATTERN_DDR3_2		0x00000000
//    #define TRAIN_WRDSK_PATTERN_DDR3_3		0xffffffff
//    #define TRAIN_WRDSK_PATTERN_DDR3_4		0xffffffff
//    #define TRAIN_WRDSK_PATTERN_DDR3_5		0x00000000
//    #define TRAIN_WRDSK_PATTERN_DDR3_6		0x00000000
//    #define TRAIN_WRDSK_PATTERN_DDR3_7		0xffffffff
//    #define TRAIN_WRDSK_PATTERN_DDR3_8		0xffffffff
//    #define TRAIN_WRDSK_PATTERN_DDR3_9		0x00000000
//    #define TRAIN_WRDSK_PATTERN_DDR3_10		0x00000000
//    #define TRAIN_WRDSK_PATTERN_DDR3_11		0xffffffff
//    #define TRAIN_WRDSK_PATTERN_DDR3_12		0xffffffff
//    #define TRAIN_WRDSK_PATTERN_DDR3_13		0x00000000
//    #define TRAIN_WRDSK_PATTERN_DDR3_14		0x00000000
//    #define TRAIN_WRDSK_PATTERN_DDR3_15		0xffffffff
//    #define TRAIN_WRDSK_PATTERN_DDR3_16		0xffffffff
//    
//    //READ PATTERN
//    #define TRAIN_RD_PATTERN_DDR3_1			0x00000000
//    #define TRAIN_RD_PATTERN_DDR3_2			0x00000000
//    #define TRAIN_RD_PATTERN_DDR3_3			0x01010101
//    #define TRAIN_RD_PATTERN_DDR3_4			0x01010101
//    #define TRAIN_RD_PATTERN_DDR3_5			0x00000000
//    #define TRAIN_RD_PATTERN_DDR3_6			0x00000000
//    #define TRAIN_RD_PATTERN_DDR3_7			0x01010101
//    #define TRAIN_RD_PATTERN_DDR3_8			0x01010101
//    #define TRAIN_RD_PATTERN_DDR3_9			0x00000000
//    #define TRAIN_RD_PATTERN_DDR3_10		0x00000000
//    #define TRAIN_RD_PATTERN_DDR3_11		0x01010101
//    #define TRAIN_RD_PATTERN_DDR3_12		0x01010101
//    #define TRAIN_RD_PATTERN_DDR3_13		0x00000000
//    #define TRAIN_RD_PATTERN_DDR3_14		0x00000000
//    #define TRAIN_RD_PATTERN_DDR3_15		0x01010101
//    #define TRAIN_RD_PATTERN_DDR3_16		0x01010101
//    #define D_DDRPHY_DDR3_DS0_TMG6_800      0x0000000C
//    #define D_DDRPHY_DDR3_DS1_TMG6_800      0x0000000C
//    #define D_DDRPHY_DDR3_DS2_TMG6_800      0x0000000C
//    #define D_DDRPHY_DDR3_DS3_TMG6_800      0x0000000C
//
//#endif

//#ifdef DRAM_DQ_X16_MODE
////WRITE PATTERN
//#define TRAIN_WR_PATTERN_DDR5_1			0xa5a55aa5
//#define TRAIN_WR_PATTERN_DDR5_2			0xa5a55aa5
//#define TRAIN_WR_PATTERN_DDR5_3			0xa55a5a5a
//#define TRAIN_WR_PATTERN_DDR5_4			0xa55a5a5a
//#define TRAIN_WR_PATTERN_DDR5_5			0xa5a55a5a
//#define TRAIN_WR_PATTERN_DDR5_6			0xa5a55a5a
//#define TRAIN_WR_PATTERN_DDR5_7			0xa5a55a5a
//#define TRAIN_WR_PATTERN_DDR5_8			0xa5a55a5a
//#define TRAIN_WR_PATTERN_DDR5_9			0xaa5555aa
//#define TRAIN_WR_PATTERN_DDR5_10		0x55aa55aa
//#define TRAIN_WR_PATTERN_DDR5_11		0x55aaaa55
//#define TRAIN_WR_PATTERN_DDR5_12		0x55aaaa55
//#define TRAIN_WR_PATTERN_DDR5_13		0x5555aaaa
//#define TRAIN_WR_PATTERN_DDR5_14		0xaaaa5555
//#define TRAIN_WR_PATTERN_DDR5_15		0x5555aaaa
//#define TRAIN_WR_PATTERN_DDR5_16		0xaaaa5555
////READ PATTERN
//#define TRAIN_RD_PATTERN_DDR5_1			0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_2			0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_3			0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_4			0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_5			0xffff0000 
//#define TRAIN_RD_PATTERN_DDR5_6			0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_7			0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_8			0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_9			0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_10		0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_11		0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_12		0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_13		0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_14		0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_15		0xffff0000
//#define TRAIN_RD_PATTERN_DDR5_16		0xffff0000
//#else
//WRITE PATTERN
#define TRAIN_WR_PATTERN_DDR5_1			0x5a5a5aa5
#define TRAIN_WR_PATTERN_DDR5_2			0xa55aa5a5
#define TRAIN_WR_PATTERN_DDR5_3			0x5a5a5aa5
#define TRAIN_WR_PATTERN_DDR5_4			0xa55aa5a5

#define TRAIN_WR_PATTERN_DDR5_5			0x5aa55a5a
#define TRAIN_WR_PATTERN_DDR5_6			0xa5a5a55a
#define TRAIN_WR_PATTERN_DDR5_7			0x5aa55a5a
#define TRAIN_WR_PATTERN_DDR5_8			0xa5a5a55a

#define TRAIN_WR_PATTERN_DDR5_9			0xaa5555aa
#define TRAIN_WR_PATTERN_DDR5_10		0x55aaaa55
#define TRAIN_WR_PATTERN_DDR5_11		0xaa5555aa
#define TRAIN_WR_PATTERN_DDR5_12		0x55aaaa55

#define TRAIN_WR_PATTERN_DDR5_13		0x5555aaaa
#define TRAIN_WR_PATTERN_DDR5_14		0xaaaa5555
#define TRAIN_WR_PATTERN_DDR5_15		0x5555aaaa
#define TRAIN_WR_PATTERN_DDR5_16		0xaaaa5555
//READ PATTERN
#define TRAIN_RD_PATTERN_DDR5_1			0xffff0000
#define TRAIN_RD_PATTERN_DDR5_2			0xffff0000
#define TRAIN_RD_PATTERN_DDR5_3			0xffff0000
#define TRAIN_RD_PATTERN_DDR5_4			0xffff0000
#define TRAIN_RD_PATTERN_DDR5_5			0xffff0000
#define TRAIN_RD_PATTERN_DDR5_6			0xffff0000
#define TRAIN_RD_PATTERN_DDR5_7			0xffff0000
#define TRAIN_RD_PATTERN_DDR5_8			0xffff0000



//#endif


//#ifdef DRAM_DQ_X16_MODE
//// 16bit
////WRITE PATTERN
//#define TRAIN_WR_PATTERN_LP4_1			0xffff0000
//#define TRAIN_WR_PATTERN_LP4_2			0xffff0000
//#define TRAIN_WR_PATTERN_LP4_3			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP4_4			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP4_5			0xffff0000
//#define TRAIN_WR_PATTERN_LP4_6			0xffff0000
//#define TRAIN_WR_PATTERN_LP4_7			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP4_8			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP4_9			0xffff0000
//#define TRAIN_WR_PATTERN_LP4_10			0xffff0000
//#define TRAIN_WR_PATTERN_LP4_11			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP4_12			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP4_13			0xffff0000
//#define TRAIN_WR_PATTERN_LP4_14			0xffff0000
//#define TRAIN_WR_PATTERN_LP4_15			0xaaaa5555
//#define TRAIN_WR_PATTERN_LP4_16			0xaaaa5555
////READ PATTERN
//#define TRAIN_RD_PATTERN_LP4_1			0xaaaa5555
//#define TRAIN_RD_PATTERN_LP4_2			0xaaaa5555
//#define TRAIN_RD_PATTERN_LP4_3			0x5555aaaa
//#define TRAIN_RD_PATTERN_LP4_4			0x5555aaaa
//
//#define TRAIN_RD_PATTERN_LP4_5			0x55555555
//#define TRAIN_RD_PATTERN_LP4_6			0xaaaaaaaa
//#define TRAIN_RD_PATTERN_LP4_7			0xaaaaaaaa
//#define TRAIN_RD_PATTERN_LP4_8			0x55555555
//#define TRAIN_RD_PATTERN_LP4_9			0xaaaa5555
//#define TRAIN_RD_PATTERN_LP4_10			0xaaaa5555
//#define TRAIN_RD_PATTERN_LP4_11			0x5555aaaa
//#define TRAIN_RD_PATTERN_LP4_12			0x5555aaaa
//#define TRAIN_RD_PATTERN_LP4_13			0x55555555
//#define TRAIN_RD_PATTERN_LP4_14			0xaaaaaaaa
//#define TRAIN_RD_PATTERN_LP4_15			0xaaaaaaaa
//#define TRAIN_RD_PATTERN_LP4_16			0x55555555
//#else
//WRITE PATTERN
#define TRAIN_WR_PATTERN_LP4_1			0x00000000
#define TRAIN_WR_PATTERN_LP4_2			0xffffffff
#define TRAIN_WR_PATTERN_LP4_3			0x00000000
#define TRAIN_WR_PATTERN_LP4_4			0xffffffff
#define TRAIN_WR_PATTERN_LP4_5			0x55555555
#define TRAIN_WR_PATTERN_LP4_6			0xaaaaaaaa
#define TRAIN_WR_PATTERN_LP4_7			0x55555555
#define TRAIN_WR_PATTERN_LP4_8			0xaaaaaaaa
#define TRAIN_WR_PATTERN_LP4_9			0x00000000
#define TRAIN_WR_PATTERN_LP4_10			0xffffffff
#define TRAIN_WR_PATTERN_LP4_11			0x00000000
#define TRAIN_WR_PATTERN_LP4_12			0xffffffff
#define TRAIN_WR_PATTERN_LP4_13			0x55555555
#define TRAIN_WR_PATTERN_LP4_14			0xaaaaaaaa
#define TRAIN_WR_PATTERN_LP4_15			0x55555555
#define TRAIN_WR_PATTERN_LP4_16			0xaaaaaaaa
//READ PATTERN
#define TRAIN_RD_PATTERN_LP4_1			0x55555555
#define TRAIN_RD_PATTERN_LP4_2			0xaaaaaaaa
#define TRAIN_RD_PATTERN_LP4_3			0x55555555
#define TRAIN_RD_PATTERN_LP4_4			0xaaaaaaaa
#define TRAIN_RD_PATTERN_LP4_5			0xaaaaaaaa
#define TRAIN_RD_PATTERN_LP4_6			0x55555555
#define TRAIN_RD_PATTERN_LP4_7			0xaaaaaaaa
#define TRAIN_RD_PATTERN_LP4_8			0x55555555
#define TRAIN_RD_PATTERN_LP4_9			0x55555555
#define TRAIN_RD_PATTERN_LP4_10			0x55555555
#define TRAIN_RD_PATTERN_LP4_11			0xaaaaaaaa
#define TRAIN_RD_PATTERN_LP4_12			0xaaaaaaaa
#define TRAIN_RD_PATTERN_LP4_13			0xaaaaaaaa
#define TRAIN_RD_PATTERN_LP4_14			0xaaaaaaaa
#define TRAIN_RD_PATTERN_LP4_15			0x55555555
#define TRAIN_RD_PATTERN_LP4_16			0x55555555
//#endif




//u32 train_rd_pattern[] = {TRAIN_RD_PATTERN_LP3_1,
//			TRAIN_RD_PATTERN_LP3_2,
//			TRAIN_RD_PATTERN_LP3_3,
//			TRAIN_RD_PATTERN_LP3_4,
//			TRAIN_RD_PATTERN_LP3_5,
//			TRAIN_RD_PATTERN_LP3_6,
//			TRAIN_RD_PATTERN_LP3_7,
//			TRAIN_RD_PATTERN_LP3_8,
//			TRAIN_RD_PATTERN_LP3_9,
//			TRAIN_RD_PATTERN_LP3_10,
//			TRAIN_RD_PATTERN_LP3_11,
//			TRAIN_RD_PATTERN_LP3_12,
//			TRAIN_RD_PATTERN_LP3_13,
//			TRAIN_RD_PATTERN_LP3_14,
//			TRAIN_RD_PATTERN_LP3_15,
//			TRAIN_RD_PATTERN_LP3_16
//			};
//u32 train_wr_pattern[] = {TRAIN_WR_PATTERN_LP3_1,
//			TRAIN_WR_PATTERN_LP3_2,
//			TRAIN_WR_PATTERN_LP3_3,
//			TRAIN_WR_PATTERN_LP3_4,
//			TRAIN_WR_PATTERN_LP3_5,
//			TRAIN_WR_PATTERN_LP3_6,
//			TRAIN_WR_PATTERN_LP3_7,
//			TRAIN_WR_PATTERN_LP3_8,
//			TRAIN_WR_PATTERN_LP3_9,
//			TRAIN_WR_PATTERN_LP3_10,
//			TRAIN_WR_PATTERN_LP3_11,
//			TRAIN_WR_PATTERN_LP3_12,
//			TRAIN_WR_PATTERN_LP3_13,
//			TRAIN_WR_PATTERN_LP3_14,
//			TRAIN_WR_PATTERN_LP3_15,
//			TRAIN_WR_PATTERN_LP3_16
//            };

static void ddrphy_train_clear(void)
{
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x0);//rf_train_start
    wait_100ns(1);
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 1, 1, 0x1);
	wait_100ns(1);
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 1, 1, 0x0);
	wait_100ns(1);
}

void ddrphy_train_all_clear(void)
{
	u32 register_value=0;
	/*
	*bit[11]:rf_train_wreye_en
	*bit[10]:rf_train_wrdsk_en
	*bit[9]:rf_train_wrlvl_en
	*bit[8]:rf_train_rdeye_en
	*bit[7]:rf_train_rddsk_en
	*bit[6]:rf_train_gate_en
	*bit[5]:rf_train_caeye_en
	*bit[4]:rf_train_cadsk_en
	*/
    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x0);//rf_train_byte_en
	//reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 4, 8, 0xff);
	wait_us(1);
	ddrphy_train_clear();
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 4, 8, 0x0);//rf_train_type_en
	//update slave delay period 2*16 4x_clk cycles
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 2, 1, 0x0);//rf_train_enable
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR),12, 8, 0x8);   //rf_phy_resync2_cnt train_resync
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR), 0, 8, 0x8);   //rf_phy_resync1_cnt train_dll_update 
}

void ddrphy_train_pre_set(u32 fn)
{
//	u32 fn;

	//rf_data_ie_mode 1:different dfi_rddata_en 0:cs0&cs1 with same dfi_rddata_en
	//rf_data_oe_mode 1:different dfi_wrdata_en 0:cs0&cs1 with same dfi_wrdata_en
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG3_ADDR), 28, 2,
		    ((dram_info.cs_num == DRAM_CS_ALL) ? 0x3 : 0x0));
	//Single rank chip must be turned off multi rank sel.
	if (dram_info.cs_num != DRAM_CS_ALL) {
//		for (fn = 0; fn < 3; fn++) {
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR + fn*0x100), 12, 2, 0);
			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG13_F0_ADDR + fn*0x100), 12, 2, 0);
//		}
	}

	wait_100ns(1);
	//disable
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),   24, 2, 0x0);//drf_auto_mr4_en:Disable auto refresh rate update
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG21_ADDR),    0, 2, 0x0);//drf_auto_zqc_en:Disable Automatic ZQ calibration
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG16_F0_ADDR+fn*0x100), 0, 1, 0x0);//rf_period_cpst_en:It will affect phy training
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG16_ADDR),      0, 1, 0x0);//rf_gate_en
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG14_F0_ADDR+fn*0x100),   24, 1, 0x0);//drf_auto_ref_en--Enable HW auto-refresh
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),      24, 1, 0x0);//dsoft_cmd_allcs
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 0, 8,0x03);//train cmd num

	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG0_ADDR), 8, 8, 0x44);// rf_byte_pattern
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG0_ADDR), 8, 8, 0x44);// rf_byte_pattern
}

void ddrphy_train_post_set(u32 fn)
{
//	u32 fn;

	//rf_data_ie_mode 1:different dfi_rddata_en 0:cs0&cs1 with same dfi_rddata_en
	//rf_data_oe_mode 1:different dfi_wrdata_en 0:cs0&cs1 with same dfi_wrdata_en
	//Single rank chip must be turned off multi rank sel.
	if (dram_info.cs_num != DRAM_CS_ALL) {
//		for (fn = 0; fn < 3; fn++) {
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR + fn*0x100), 12, 2, 0);
			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG13_F0_ADDR + fn*0x100), 12, 2, 0);
//		}
	}

	wait_100ns(1);
	//disable
    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR),0x0);//clear train ctrl0 register
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),   24, 2, 0x0);//drf_auto_mr4_en:Disable auto refresh rate update
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG21_ADDR),    0, 2, 0x0);//drf_auto_zqc_en:Disable Automatic ZQ calibration
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG16_F0_ADDR+fn*0x100), 0, 1, 0x0);//rf_period_cpst_en:It will affect phy training

	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG14_F0_ADDR+fn*0x100),   24, 1, 0x1);//drf_auto_ref_en--Enable HW auto-refresh
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),      24, 1, 0x1);//dsoft_cmd_allcs
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),      28, 1, 0x1);//dsoft_cs_0
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 0, 8,0x00);//train cmd num
}

void ddrphy_train_en(u32 train_en)
{
	u32 tr_val;
	/*
	*rf_train_cadsk_en bit[4]  CA 去偏斜训练
	*rf_train_caeye_en bit[5]  CA 眼图训练
	*rf_train_gate_en  bit[6]  读 DQS 门控训练
	*rf_train_rddsk_en bit[7]  读去偏斜训练
	*rf_train_rdeye_en bit[8]  读眼图训练
	*rf_train_wrlvl_en bit[9]  写均衡训练
	*rf_train_wrdsk_en bit[10] 写去偏斜训练
	*rf_train_wreye_en bit[11] 写眼图训练
	*/
	tr_val = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR));//读-改-写：只动训练类型位，保留其余字段
	tr_val &= ~(0xff << 4);//rf_train_*_en：清零 8 种训练类型使能位 [11:4]
	tr_val |=  (0x1  << (train_en));//仅使能所请求的一种训练（train_en = TRAIN_*_INDEX，取值 4~11）
	__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), tr_val);//写回训练类型选择
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 2, 1, 0x1);//rf_train_enable：启动训练
}

//u32 ddrphy_train_sel(u32 train_sel)
//{
//	return (((ddrphy_tmg + train_info.ddr_freq_num)->train_flag >> ((train_sel-4) * 4))&0x1);
//}

static u32 ddrphy_train_polling_done(u32 tr_type)
{
	u32 train_done_flag = 0;
	u32 train_wait_num = 0;

	//等待训练完成：轮询该训练类型对应 4bit 段的 bit0（done 位），直到硬件置 1 才退出
	while (((__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR)) >> ((tr_type-4)*4))&0x1) == 0) {
#if TEST_DEBUG_LOG_LEVEL > 5
		//DDR_DBG_PRINT("training not done, wait count:");
		print_dec(train_wait_num ++);
		//DDR_DBG_PRINT("training type:");
		print_dec(tr_type);
		//DDR_DBG_PRINT("\r\n");
		print_phy_reg();
		print_mc_reg();
#endif
	};
	//读同 4bit 段的 bit1（fail 位）：置 1 表示该训练失败，禁止通过
	if ((__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR)) >> ((tr_type-4)*4+1))&0x1) {
#if TEST_DEBUG_LOG_LEVEL > 5
		switch(tr_type){	
			case TRAIN_CADSK_INDEX://DDR_DBG_PRINT("TRAIN_CADSK retry ");
			break;
			case TRAIN_CAEYE_INDEX://DDR_DBG_PRINT("TRAIN_CAEYE retry ");
			break;
			case TRAIN_GATE_INDEX://DDR_DBG_PRINT ("TRAIN_GATE retry "); 
			break;
			case TRAIN_RDDSK_INDEX://DDR_DBG_PRINT("TRAIN_RDDSK retry ");
			break;
			case TRAIN_RDEYE_INDEX://DDR_DBG_PRINT("TRAIN_RDEYE retry ");
			break;
			case TRAIN_WRLVL_INDEX://DDR_DBG_PRINT("TRAIN_WRLVL retry ");
			break;
			case TRAIN_WRDSK_INDEX://DDR_DBG_PRINT("TRAIN_WRDSK retry ");
			break;
			case TRAIN_WREYE_INDEX://DDR_DBG_PRINT("TRAIN_WREYE retry ");
			break;
		}
#endif
	} else {
#if TEST_DEBUG_LOG_LEVEL > 5
		switch(tr_type){			
			case TRAIN_CADSK_INDEX://DDR_DBG_PRINT("TRAIN_CADSK pass ");
			break;
			case TRAIN_CAEYE_INDEX://DDR_DBG_PRINT("TRAIN_CAEYE pass ");
			break;
			case TRAIN_GATE_INDEX://DDR_DBG_PRINT ("TRAIN_GATE pass "); 
			break;
			case TRAIN_RDDSK_INDEX://DDR_DBG_PRINT("TRAIN_RDDSK pass ");
			break;
			case TRAIN_RDEYE_INDEX://DDR_DBG_PRINT("TRAIN_RDEYE pass ");
			break;
			case TRAIN_WRLVL_INDEX://DDR_DBG_PRINT("TRAIN_WRLVL pass ");
			break;
			case TRAIN_WRDSK_INDEX://DDR_DBG_PRINT("TRAIN_WRDSK pass ");
			break;
			case TRAIN_WREYE_INDEX://DDR_DBG_PRINT("TRAIN_WREYE pass ");
			break;
		}
#endif
	}
	return 0;
}

void ddrphy_train_soft_update_ac(void)
{
	u32 fn;
	for (fn = 0; fn < 3; fn++) {
        reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG0_F0_ADDR+fn*0x100),2,1,0x1);//rf_dl_cpst_start_dqx
        reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG0_F0_ADDR+fn*0x100),2,1,0x0);//rf_dl_cpst_start_dqx
        reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG0_F0_ADDR+fn*0x100),2,1,0x1);//rf_dl_cpst_start_dqx
        reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG0_F0_ADDR+fn*0x100),2,1,0x0);//rf_dl_cpst_start_dqx
        //reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100),2,1,0x1);//rf_dl_cpst_start_dqx
        //reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100),2,1,0x0);//rf_dl_cpst_start_dqx
        //reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100),2,1,0x1);//rf_dl_cpst_start_dqx
        //reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100),2,1,0x0);//rf_dl_cpst_start_dqx
	    wait_100ns(1);
    }
}

void ddrphy_train_soft_update_dq(void)
{
	u32 fn;
	for (fn = 0; fn < 3; fn++) {
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx

	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx

	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx

	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx

	    wait_100ns(1);

    }
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
//
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
//
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x1);//rf_dl_cpst_start_dqx
//
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+train_info.ddr_freq_num*0x100), 2, 1, 0x0);//rf_dl_cpst_start_dqx
//
//	wait_100ns(1);
}

static void ddrphy_train_info_pre_set(u32 train_phy, u8 cs_num)
{
	train_info.train_cs_num = cs_num;
	train_info.train_phy_num = train_phy;

	if((dram_info.dram_type == DRAM_DDR4) || (dram_info.dram_type == DRAM_DDR3) || (dram_info.dram_type == DRAM_DDR5) || (dram_info.dram_type == DRAM_LP4))	
    {
		train_info.train_byte_en = 0x3;/*Enable byte0,byte1 Training*/
		train_info.train_addr_31_0=0x0;
		train_info.train_addr_34_32=0x0;
        
	}
	//select train cs

    if((dram_info.dram_type == DRAM_DDR4) || (dram_info.dram_type == DRAM_DDR3) || (dram_info.dram_type == DRAM_DDR5))
	{
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 20, 1, train_info.train_cs_num);//rf_train_cs_sel
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 28, 1, train_info.train_cs_num);          //rf_dsoft_cs_0
		//select training core0
		//reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 22, 1, train_info.train_phy_num);//rf_train_core
		/*Set training byte core0:byte0 and byte1, byte2 and byte3*/
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, train_info.train_byte_en);//rf_train_byte_en
		//train addr set
		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL2_ADDR), train_info.train_addr_31_0);//rf_train_addr_0_31
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 12, 3, train_info.train_addr_34_32);//rf_train_addr_34_32

		if (dram_info.dram_dq_width==DRAM_DQ_X64) 
		{
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 28, 2, 0x2);//rf_bist_datawidth
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 12, 4, 0x1);//rf_bist_burst_length
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 20, 3, 0x5);//rf_bist_data_size

		}
		else if (dram_info.dram_dq_width==DRAM_DQ_X32) 
		{
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 28, 2, 0x1);//rf_bist_datawidth
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 12, 4, 0x0);//rf_bist_burst_length
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 20, 3, 0x5);//rf_bist_data_size

		}
		else if (dram_info.dram_dq_width==DRAM_DQ_X16) 
		{
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 28, 2, 0x0);//rf_bist_datawidth
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 12, 4, 0x0);//rf_bist_burst_length
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 20, 3, 0x4);//rf_bist_data_size

		}
		else 
		{
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 28, 2, 0x3);//rf_bist_datawidth
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 12, 4, 0x0);//rf_bist_burst_length
			reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 20, 3, 0x3);//rf_bist_data_size
		}
	}
	 else if (dram_info.dram_type == DRAM_LP4)
	 {
		 reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 20, 1, train_info.train_cs_num);//rf_train_cs_sel
		 reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 28, 1, train_info.train_cs_num);          //rf_dsoft_cs_0
		 //select training core0
		 //reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 22, 1, train_info.train_phy_num);//rf_train_core
		 /*Set training byte core0:byte0 and byte1, byte2 and byte3*/
		 reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, train_info.train_byte_en);//rf_train_byte_en
		 //train addr set
		 __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL2_ADDR), train_info.train_addr_31_0);//rf_train_addr_0_31
		 reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 12, 3, train_info.train_addr_34_32);//rf_train_addr_34_32

		 reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG16_ADDR), 0, 1, 0x0);          //rf_auto_gate_en
		 //reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG3_ADDR), 28, 2, 0x3);          //rf_ie_multirank & rf_oe_multirank
		 //reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 12, 2, 0x3);//rf_multirank
		 //reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 12, 2, 0x3);//rf_multirank

		 if (dram_info.dram_dq_width == DRAM_DQ_X32)
		 {
			 reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 28, 2, 0x1);//rf_bist_datawidth
			 reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 12, 4, 0x1);//rf_bist_burst_length
		     //reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 0, 4, 0x0);//byte swap
		     //reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 4, 4, 0x1);//byte swap
		     //reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 8, 4, 0x2);//byte swap
		     //reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 12, 4, 0x3);//byte swap
		 }
		 else
		 {
			 reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 28, 2, 0x0);//rf_bist_datawidth
			 reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 12, 4, 0x0);//rf_bist_burst_length

		     //reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 0, 4, 0x0);//byte swap
		     //reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 4, 4, 0x1);//byte swap
		 }
		 reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 20, 3, 0x5);//rf_bist_data_size

     }

		     reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 0,  4, 0x0);//byte swap
		     reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 4,  4, 0x1);//byte swap
		     reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 8,  4, 0x2);//byte swap
		     reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 12, 4, 0x3);//byte swap
		     reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 16, 4, 0x0);//byte swap
		     reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 20, 4, 0x1);//byte swap
		     reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 24, 4, 0x2);//byte swap
		     reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 28, 4, 0x3);//byte swap
#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\nTraining Address:");
	//DDR_DBG_PRINT("		addr_31_0:");
	//print_hex(train_info.train_addr_31_0);
	//DDR_DBG_PRINT("		addr_32_34:");
	//print_hex(train_info.train_addr_34_32);
#endif

}

void ddrphy_train_rd_en_turning(u32 fn)

{
	u32 i = 0;
	u32 j = 0;
	u32 rd_en = 0x1;
	u32 rd_en_value = 0;
	u32 result = 0;
	u64 addr = dram_info.cs0_size + dram_info.cs1_size - 0x1000;
//	if (!ddrphy_train_sel(TRAIN_RD_EN_INDEX))
//		return 0;
#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------start rd_en training----------------\r\n");
#endif
	//train start
	//enable ctrl update
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG16_F0_ADDR+fn*0x80), 0, 1, 0x1);
	for(i = 0; i < 15; i++) {
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG7_F0_ADDR+fn*0x100 ), 0, 16, rd_en);
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG7_F0_ADDR+fn*0x100 ), 0, 16, rd_en);
		//DDR_DBG_PRINT("\r\n");
		//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG7_F0_ADDR)));
		//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG7_F0_ADDR)));
		//DDR_DBG_PRINT("\r\n");
		result = dram_bist_test(0);
		//DDR_DBG_PRINT("\r\n");
		rd_en = rd_en << 1;
//		if (dram_info.cs1_size)
			//result += dram_bist_test(addr, (addr + 0xff0));
		if (result == 0x0) {
			rd_en_value = rd_en;
			for (j = i; j < 15; j++) {
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG7_F0_ADDR+fn*0x100), 0, 16, rd_en);
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG7_F0_ADDR+fn*0x100), 0, 16, rd_en);
				//DDR_DBG_PRINT("\r\n");
				//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG7_F0_ADDR)));
				//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG7_F0_ADDR)));
				//DDR_DBG_PRINT("\r\n");
				result = dram_bist_test(0);
				//DDR_DBG_PRINT("\r\n");
				rd_en = rd_en << 1;
			}
			break;
		}

	}
	//DDR_DBG_PRINT("\r\nrd_en_training_result:");
	//print_hex(rd_en_value);
	if (rd_en > 0x800) {
		rd_en = 0x800;
	}

	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG7_F0_ADDR+fn*0x100), 0, 16, rd_en_value);
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG7_F0_ADDR+fn*0x100), 0, 16, rd_en_value);
	//DDR_DBG_PRINT("\r\nrd_en training result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG7_F0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG7_F0_ADDR)));
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG16_F0_ADDR+fn*0x80), 0, 1, 0x0);

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------rd_en training over----------------\r\n");
#endif
}

//void ddrphy_dl_sel(u32 phy_sel8)//phy delay line adjustment
////void ddrphy_dl_sel(u32 phy_sel8,u32 phy_sel7,u32 phy_sel6,u32 phy_sel5,u32 phy_sel4,u32 phy_sel3,u32 phy_sel2,u32 phy_sel1,u32 phy_sel0)//phy delay line adjustment
//{
//	u32   reg_data = 0;
//	u32   rf_wdata_latency_f0 = 0;
//	u32   rf_wdm_latency_f0 = 0;
//	u32   rf_data_oe_dl_f0 = 0;
//	u32   rf_dqs_oe_dl_f0 = 0;
//
//	//		        //DDR_DBG_PRINT("\r\nphy_dl_sel_starts\r\n");
//	// 		        print_dec(phy_sel[9]);
//
//	if (phy_sel8 == 0x0) { //works for each DQ byte
////                if(phy_sel0 == 0x1){
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) - 2);//rf_dqs_out_sel_ds0_f0
////                }
////                if(phy_sel1 == 0x1){
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) - 2);//rf_dqs_out_sel_ds0_f0
////                }
////                if(phy_sel2 == 0x1){
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) - 2);//rf_dqs_out_sel_ds0_f0
////                }
////                if(phy_sel3 == 0x1){
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) - 2);//rf_dqs_out_sel_ds0_f0
////                }
////                if(phy_sel4 == 0x1){
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) - 2);//rf_dqs_out_sel_ds0_f0
////                } 
////                if(phy_sel5 == 0x1){
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) - 2);//rf_dqs_out_sel_ds0_f0
////                }
////                if(phy_sel6 == 0x1){
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) - 2);//rf_dqs_out_sel_ds0_f0
////                }
////                if(phy_sel7 == 0x1){
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) - 2);//rf_dqs_out_sel_ds0_f0
////                }
//	}
//	else {
//		rf_wdata_latency_f0 = reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+train_info.ddr_freq_num*0x80), 0, 4);
//		rf_wdm_latency_f0 = reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+train_info.ddr_freq_num*0x80), 8, 4);
//		rf_data_oe_dl_f0 = reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+train_info.ddr_freq_num*0x80), 16, 4);
//		rf_dqs_oe_dl_f0 = reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+train_info.ddr_freq_num*0x80), 24, 4);
//
//		reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+train_info.ddr_freq_num*0x80), 0, 4, rf_wdata_latency_f0 - 1);
//		reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+train_info.ddr_freq_num*0x80), 8, 4, rf_wdm_latency_f0 - 1);
//		reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+train_info.ddr_freq_num*0x80), 16, 4, rf_data_oe_dl_f0 - 1);
//		reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+train_info.ddr_freq_num*0x80), 24, 4, rf_dqs_oe_dl_f0 - 1);
//
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) << 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) + 2);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) << 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) + 2);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) << 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) + 2);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) << 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) + 2);//rf_dqs_out_sel_ds0_f0
//
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) << 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) + 2);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) << 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) + 2);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) << 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) + 2);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 17) << 1);//rf_dqs_out_sel_ds0_f0
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 2) + 2);//rf_dqs_out_sel_ds0_f0                
//	}
//}


/*******************************************************************************
********************************************************************************/
void ddrphy_train(u32 ddr_clk, u32 fn)
{
	/****train info init*****/
	train_info.ddr_clk = ddr_clk;
	train_info.ddr_freq_num = fn;
	//switch (BOOT_FREQ_POINT)
	switch (ddr_clk)
	{
	case DDR_CLK_400M:
		train_info.odt_status = 0;
		break;
	default:
		train_info.odt_status = 1;
		break;
	}

#if TEST_DEBUG_LOG_LEVEL>0
	//DDR_DBG_PRINT("\r\nStart training:");
	//print_dec(train_info.ddr_clk);
	//DDR_DBG_PRINT("Mhz\n");
#endif

	//dfs to target frequency

    //ddr4 train 
	if(dram_info.dram_type == DRAM_DDR4)
    {
		//DDR_DBG_PRINT("\r\nddrphy_train_ddr4 starts\r\n");
		__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), 0x3);
		ddrphy_train_ddr4(fn);
		//DDR_DBG_PRINT("\r\nddrphy_train_ddr4 pass\r\n");
    }
	//else if(dram_info.dram_type == DRAM_DDR3)
    //{
	//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), 0x2);
	//	//DDR_DBG_PRINT("\r\nddrphy_train_ddr4 starts\r\n");
	//	ddrphy_train_ddr3(fn);
	//	//DDR_DBG_PRINT("\r\nddrphy_train_ddr4 pass\r\n");		
	//}
	else if(dram_info.dram_type == DRAM_DDR5)
    {
		__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), 0x4);
		//DDR_DBG_PRINT("\r\nddrphy_train_ddr4 starts\r\n");
		ddrphy_train_ddr5(fn);
		//DDR_DBG_PRINT("\r\nddrphy_train_ddr4 pass\r\n");		
	}    
	else {
		DDR_DBG_PRINT("\r\nddrphy_train_lpddr4 starts");
		ddrphy_train_lpddr4(ddr_clk,fn);
		DDR_DBG_PRINT("\r\nddrphy_train_lpddr4 pass");
	}

	//update training reslut
	ddrphy_train_soft_update_ac();
	ddrphy_train_soft_update_dq();

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),    24, 1, 0x1);//dsoft_cmd_allcs
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG14_F0_ADDR+fn*0x80), 12, 1, 0x1);//enable auto ref
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR),  0, 1, 0x0);//Disable rf_train_enable

	/*add dqs read en training*/
	//ddrphy_train_rd_en_turning(train_info.ddr_clk);
#if TEST_DEBUG_LOG_LEVEL>0
	//DDR_DBG_PRINT("\r\n");
	print_dec(train_info.ddr_clk);
	//DDR_DBG_PRINT("Mhz");
	//DDR_DBG_PRINT(" training pass!\n");
#endif
}

void ddrphy_train_ddr4(u32 fn)
{
	/*Get training bist addr and choose current training core*/
    //wrlvl training   
    //DDR_DBG_PRINT("ddr4_training_starting");  
    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR), 24, 4, 0x1);//rf_mode_reg_core
    ddrphy_train_pre_set(fn);
	//rddsk & rdeye training
	ddrphy_ddr4_train_read_training(DDRPHY_0,fn);
	//wrdsk & wreye training
    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
   reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
   reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
   reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
	ddrphy_train_wrlvl_training_pro_max(fn , 0x3);
    ddrphy_train_coarse_wrlvl_training(fn, DDRPHY_0);
	ddrphy_ddr4_train_write_training(DDRPHY_0,fn);
    #ifdef D_DDR_2CORE
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR), 24, 4, 0x2);//rf_mode_reg_core
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG7_ADDR), 16,15, RF_LINEAR_BASE_CORE0);//rf_linear_base_core1
	    ddrphy_ddr4_train_read_training(DDRPHY_1,fn);
	    //wrdsk & wreye training
        reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
        reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
        reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
        reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
	    ddrphy_ddr4_train_write_training(DDRPHY_1,fn);
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR), 24, 4, 0x3);//rf_mode_reg_core
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG7_ADDR), 16,15, RF_LINEAR_BASE_CORE1);//rf_linear_base_core1
    #endif
    ddrphy_train_post_set(fn);
//	ddrphy_train_rd_en_turning();
	//DDR_DBG_PRINT("\r\nddr4_training pass\r\n");
}

//void ddrphy_train_ddr3(u32 fn)
//{
//	/*Get training bist addr and choose current training core*/
//    //wrlvl training   
//    //DDR_DBG_PRINT("ddr3_training_starting");  
//    ddrphy_train_pre_set();
//	//rddsk & rdeye training
//	ddrphy_ddr3_train_read_training();
//	//wrdsk & wreye training
//	//ddrphy_ddr3_train_wrlvl_training_pro_max(fn , 0xff);
//	ddrphy_ddr3_train_write_training();
//    //ddrphy_train_post_set();
////	ddrphy_train_rd_en_turning();
//	//DDR_DBG_PRINT("\r\nddr4_training pass\r\n");
//}

void ddrphy_train_ddr5(u32 fn)
{
	/*Get training bist addr and choose current training core*/
    //wrlvl training   
    //DDR_DBG_PRINT("ddr4_training_starting");  
    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR), 24, 4, 0x1);//rf_mode_reg_core
    ddrphy_train_pre_set(fn);
	//rddsk & rdeye training
	ddrphy_ddr5_train_read_training(DDRPHY_0,fn);
	//wrdsk & wreye training
    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
   reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
   reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
   reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
	ddrphy_train_wrlvl_training_pro_max(fn , 0x3);
    ddrphy_train_coarse_wrlvl_training(fn, DDRPHY_0);
	ddrphy_ddr5_train_write_training(DDRPHY_0,fn);
    #ifdef D_DDR_2CORE
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR), 24, 4, 0x2);//rf_mode_reg_core
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG7_ADDR), 16,15, RF_LINEAR_BASE_CORE0);//rf_linear_base_core1
	    ddrphy_ddr5_train_read_training(DDRPHY_1,fn);
	    //wrdsk & wreye training
        reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
        reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
        reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
        reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
	    ddrphy_ddr5_train_write_training(DDRPHY_1,fn);
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR), 24, 4, 0x3);//rf_mode_reg_core
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG7_ADDR), 16,15, RF_LINEAR_BASE_CORE1);//rf_linear_base_core1
    #endif
    ddrphy_train_post_set(fn);
//	ddrphy_train_rd_en_turning();
	//DDR_DBG_PRINT("\r\nddr4_training pass\r\n");
}

void ddrphy_train_lpddr4(u32 ddr_clk,u32 fn)
{
	/*Get training bist addr and choose current training core*/
	//wrlvl training   
	DDR_DBG_PRINT("lpddr4_training_starting");
    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR), 24, 4, 0x1);//rf_mode_reg_core
	ddrphy_train_pre_set(fn);
//	DDR_DBG_PRINT("\r\n lpddr4_training_pre_set_done\r\n");
    ddrphy_lpddr4_train_ca_training(DDRPHY_0,0,ddr_clk,fn);
//		//rddsk & rdeye training
	ddrphy_lpddr4_train_read_training(DDRPHY_0,fn);
	DDR_DBG_PRINT("\r\n lpddr4_read_training_done\r\n");
    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
   reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
   reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
   reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17) >> 1);//rf_dqs_out_sel_ds0_f0
	ddrphy_train_wrlvl_training_pro_max(fn , 0x03);
    ddrphy_train_coarse_wrlvl_training(fn,DDRPHY_0);
	//wrdsk & wreye training
	ddrphy_lpddr4_train_write_training(DDRPHY_0,fn);

	ddrphy_train_post_set(fn);
	DDR_DBG_PRINT("\r\nlpddr4_training pass\r\n");
}
void ddrphy_lpddr4_train_ca_training(u32 train_phy,u32 core_num, u32 ddr_clk, u32 fn)
{
	ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
    ddrphy_ca_cfg(ddr_clk);
	ddrphy_train_lpddr4_ca_pattern(train_phy,core_num);
	ddrphy_train_lpddr4_cadsk(fn);
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
    //ddrphy_ca_cfg();
	//ddrphy_train_lpddr4_ca_pattern(train_phy,core_num);
	ddrphy_train_lpddr4_caeye(1,fn);
	DDR_DBG_PRINT("\r\nlpddr4_ca_cs0_training pass\r\n");

	//if (dram_info.cs_num == 1) {
	//	ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//	ddrphy_train_lpddr4_ca_pattern(train_phy,core_num);
	//    ddrphy_train_lpddr4_caeye(0);
	//    ddrphy_train_lpddr4_cadsk();
	//	DDR_DBG_PRINT("\r\nlpddr4_ca_cs1_training pass\r\n");
	//}
	//else {
	//	DDR_DBG_PRINT("\r\nlpddr4_ca_training pass\r\n");
	//}

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR),      28, 1, 0x0);//enable CA training
    ddrphy_train_all_clear();
    }
void ddrphy_train_lpddr4_caeye(u32 vref_scan_en, u32 fn)
{

    u32  caeye_vref_min= 0;
    u32  caeye_vref_max= 0;
	u32 pass_win = 0;
	u32 pass_val = 0;
    u32 vref_val = 0;
	u32 vref_step = 2;
    u32 pass_win_max = 0;
	u32 caeye_vref_first = 0, caeye_vref_last = 0;
	u32 mr12_vref_wr_val = 0;

#if TEST_DEBUG_LOG_LEVEL>2
	DDR_DBG_PRINT("\r\n-------------caeye training start---------------\r\n");
#endif
	ddrphy_train_en(TRAIN_CAEYE_INDEX);

	ddrphy_train_clear();
	/*start training*/

	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x1);  //rf_dqs_oe_sel
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x1);  //rf_dqs_oe_sel

	if (vref_scan_en) {
			caeye_vref_min = LPDDR4_CAEYE_VREF_MIN;
			caeye_vref_max = LPDDR4_CAEYE_VREF_MAX;
		for (vref_val = caeye_vref_min; vref_val <= caeye_vref_max; vref_val += vref_step) {

			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL6_ADDR), 12, 7, vref_val); //rf_caeye_vref
			wait_us(1000);

			//train start
			/*start training*/
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
			wait_100ns(1);
			ddrphy_train_polling_done(TRAIN_CAEYE_INDEX);
			/*get training result*/
		
		    pass_win = ((__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES1_ADDR))) >>16) & 0x7ff;   // CA EYE PASS


			if ( pass_win > pass_win_max ) {
				pass_win_max = pass_win;
				caeye_vref_first = vref_val;
			}
			else if (pass_win == pass_win_max) {
				caeye_vref_last = vref_val;
			}
			ddrphy_train_clear();
			wait_100ns(1);
            #if TEST_DEBUG_LOG_LEVEL>2
            			DDR_DBG_PRINT(" vref setting:");
            			print_hex(vref_val);
            			DDR_DBG_PRINT("  Pass Window:");
            			print_hex(pass_win);
            			DDR_DBG_PRINT("\r\n");
            #endif

			if (caeye_vref_last < caeye_vref_first) {
				caeye_vref_last = caeye_vref_first;
			}
			mr12_vref_wr_val = (caeye_vref_first + caeye_vref_last) / 2;//take average for first vref and last vref

    }
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL6_ADDR), 12, 7, mr12_vref_wr_val); //rf_caeye_vref
			wait_us(1000);

			//train start
			/*start training*/
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
			wait_100ns(1);
			ddrphy_train_polling_done(TRAIN_CAEYE_INDEX);
			/*get training result*/
		
		    pass_win = ((__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES1_ADDR))) >>16) & 0x7ff;   // CA EYE PASS
			ddrphy_train_clear();

	        reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR),      28, 1, 0x0);//enable CA training
            ddrphy_train_all_clear();
			//ddrmc_mrw(DRAM_MR_12, DRAM_CS_ALL, mr12_vref_wr_val);
            if (train_info.train_cs_num == 0) {   
		   	    ddrmc_mrw(DRAM_MR_12, DRAM_CS_0, mr12_vref_wr_val);
            }
            //else {
		   	//    ddrmc_mrw(DRAM_MR_12, DRAM_CS_1, mr12_vref_wr_val);
            //}
			wait_us(1000);
  }
  else {  
			//reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL6_ADDR), 12, 7, vref_val); //rf_caeye_vref
			//wait_us(1000);

			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
			wait_100ns(1);
			ddrphy_train_polling_done(TRAIN_CAEYE_INDEX);
			/*get training result*/
		    pass_win = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES1_ADDR));   // CA EYE PASS
			ddrphy_train_clear();
 }		
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x0);  //rf_dqs_oe_sel
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x0);  //rf_dqs_oe_sel
}
void ddrphy_train_lpddr4_cadsk(u32 fn)
{
    	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x1);  //rf_dqs_oe_sel
    	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x1);  //rf_dqs_oe_sel
    	ddrphy_train_en(TRAIN_CADSK_INDEX);
		wait_100ns(1);
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
		wait_100ns(1);
    	ddrphy_train_polling_done(TRAIN_CADSK_INDEX);
		ddrphy_train_clear();

    	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x0);  //rf_dqs_oe_sel
    	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x0);  //rf_dqs_oe_sel
}
void ddrphy_ca_cfg(u32 ddr_clk)
{

switch (ddr_clk)
{
    //case DDR_CLK_2133M:
	//    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 0, 8, 0x4);  //drf_t_cacd    lp4 = 20ns
	//    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 8, 8, 0x22);  //drf_t_caent   lp4 = 250ns

	//    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 16, 8, 0x26);  ///drf_t_vref    lp4 = 250ns,the real gap is (drf_t_vref - 1) * dram_clk * 16
    //    break;
    //case DDR_CLK_1866M:

	//    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 0, 8, 0x4);  //drf_t_cacd    lp4 = 20ns
	//    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 8, 8, 0x1e);  //drf_t_caent   lp4 = 250ns

	//    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 16, 8, 0x20);  ///drf_t_vref    lp4 = 250ns,the real gap is (drf_t_vref - 1) * dram_clk * 16
    //    break;

    case DDR_CLK_1600M:
	    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 0, 8, 0x3);  //drf_t_cacd    lp4 = 20ns
	    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 8, 8, 0x19);  //drf_t_caent   lp4 = 250ns

	    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 16, 8, 0x1b);  ///drf_t_vref    lp4 = 250ns,the real gap is (drf_t_vref - 1) * dram_clk * 16
        break;
    case DDR_CLK_1333M:
	    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 0, 8, 0x3);  //drf_t_cacd    lp4 = 20ns
	    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 8, 8, 0x15);  //drf_t_caent   lp4 = 250ns

	    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 16, 8, 0x17);  ///drf_t_vref    lp4 = 250ns,the real gap is (drf_t_vref - 1) * dram_clk * 16
        break;
    //case DDR_CLK_1200M:
	//    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 0, 8, 0x3);  //drf_t_cacd    lp4 = 20ns
	//    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 8, 8, 0x15);  //drf_t_caent   lp4 = 250ns

	//    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 16, 8, 0x17);  ///drf_t_vref    lp4 = 250ns,the real gap is (drf_t_vref - 1) * dram_clk * 16
    //    break;
    case DDR_CLK_1066M:
	    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 0, 8, 0x3);  //drf_t_cacd    lp4 = 20ns
	    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 8, 8, 0x11);  //drf_t_caent   lp4 = 250ns

	    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 16, 8, 0x11);  ///drf_t_vref    lp4 = 250ns,the real gap is (drf_t_vref - 1) * dram_clk * 16
        break;
    default:
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 0, 8, 0x2);  //drf_t_cacd    lp4 = 20ns
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 8, 8, 0xd);  //drf_t_caent   lp4 = 250ns

	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 16, 8, 0xf);  ///drf_t_vref    lp4 = 250ns,the real gap is (drf_t_vref - 1) * dram_clk * 16
    break;
}
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR),12, 8, 0x2);   //rf_phy_resync2_cnt 
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR), 0, 8, 0x4);   //rf_phy_resync1_cnt 
}
void ddrphy_train_lpddr4_ca_pattern(u32 train_phy , u32 core_num)//no dq ODT config
{
	u32 odt_timing0, odt_timing1;
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), 0x00000000);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA1_ADDR), 0xffffffff);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA2_ADDR), 0x00000000);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA3_ADDR), 0xffffffff);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA4_ADDR), 0x00000000);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA5_ADDR), 0xffffffff);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA6_ADDR), 0x00000000);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA7_ADDR), 0xffffffff);

	//    __raw_writel(DDRMC_BIST_(PARM_DDRMC_TRAIN_DATA8_ADDR), TRAIN_RD_PATTERN_DDR4_9);
	//    __raw_writel(DDRMC_BIST_(PARM_DDRMC_TRAIN_DATA9_ADDR), TRAIN_RD_PATTERN_DDR4_10);
	//    __raw_writel(DDRMC_BIST_(PARM_DDRMC_TRAIN_DATA10_ADDR), TRAIN_RD_PATTERN_DDR4_11);
	//    __raw_writel(DDRMC_BIST_(PARM_DDRMC_TRAIN_DATA11_ADDR), TRAIN_RD_PATTERN_DDR4_12);
	//    __raw_writel(DDRMC_BIST_(PARM_DDRMC_TRAIN_DATA12_ADDR), TRAIN_RD_PATTERN_DDR4_13);
	//    __raw_writel(DDRMC_BIST_(PARM_DDRMC_TRAIN_DATA13_ADDR), TRAIN_RD_PATTERN_DDR4_14);
	//    __raw_writel(DDRMC_BIST_(PARM_DDRMC_TRAIN_DATA14_ADDR), TRAIN_RD_PATTERN_DDR4_15);
	//    __raw_writel(DDRMC_BIST_(PARM_DDRMC_TRAIN_DATA15_ADDR), TRAIN_RD_PATTERN_DDR4_16);
		//byte swap
	//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 0, 16, 0x3120);
		/*set rf_train_dsk_base_value for rd training*/
		/*Make rd training do more accurate*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x2);  //rf_phy_update_cnt
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0x130); //rf_train_eye_max_value
	/*set min pass window*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 24, 8, 0x20);  //rf_dqeye_pass_min_window
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 12, 8, 0x0);  //rf_train_dsk_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 0, 8, 0x80);  //rf_train_dsk_max_value

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR),      28, 1, 0x1);//enable CA training
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),      28, 1, 0x0);//rf_dsoft_cs
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),      24, 1, 0x0);//rf_dsoft_cmd_allcs

		 if (dram_info.dram_dq_width == DRAM_DQ_X32)
        {
            if (core_num == 1) {
		         reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 12, 4, 0x5);//byte swap
            }
            else {
		         reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 4, 4, 0x1);//byte swap
                
            }
        }
        else
        {
	//	     reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 4, 4, 0x0);//byte swap
        }

}

void ddrphy_lpddr4_train_read_training(u32 train_phy,u32 fn)
{
	/**********Start RD Training *************/
	/*Set rd dq training pattern*/
	ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	ddrphy_train_lpddr4_rd_pattern();
	//ddrphy_train_gate_training(train_phy,fn);

	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	//ddrphy_train_lpddr4_rd_pattern();
	ddrphy_train_lpddr4_rddsk(train_phy, fn);
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	//ddrphy_train_lpddr4_rd_pattern();
	ddrphy_train_lpddr4_rdeye(1,train_phy,fn);
	DDR_DBG_PRINT("\r\nlpddr4_rd_cs0_training pass\r\n");

	//if (dram_info.cs_num == 1) {
	//	ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//	ddrphy_train_lpddr4_rd_pattern();
	//	//	ddrphy_train_ddr4_rddsk();
	//	ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//	ddrphy_train_lpddr4_rdeye(1,train_phy);
	//	DDR_DBG_PRINT("\r\nddr4_rd_eye_training pass\r\n");
	//}
	//else {
	//	DDR_DBG_PRINT("\r\nddr4_read_training pass\r\n");
	//}

}

void ddrphy_ddr4_train_read_training(u32 train_phy, u32 fn)
{
	/**********Start RD Training *************/
	/*Set rd dq training pattern*/
	ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	ddrphy_train_ddr4_rd_pattern();
	//ddrphy_train_gate_training(train_phy,fn);
	
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	//ddrphy_train_ddr4_rd_pattern();
	ddrphy_train_ddr4_rddsk(train_phy);
	ddrphy_train_ddr4_rdeye(1, train_phy,fn);
    ddrphy_train_all_clear();
	//DDR_DBG_PRINT("\r\nddr4_rd_cs0_training pass\r\n");

    //if(dram_info.cs_num == 1){
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//ddrphy_train_ddr4_rd_pattern();
	//ddrphy_train_gate_training(train_phy);
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//ddrphy_train_ddr4_rd_pattern();
	//ddrphy_train_ddr4_rddsk(train_phy);
	//ddrphy_train_ddr4_rdeye(1, train_phy);
    //ddrphy_train_all_clear();
	////DDR_DBG_PRINT("\r\nddr4_rd_cs1_training pass\r\n");
    //}
    //else{
    ////DDR_DBG_PRINT("\r\nddr4_read_training pass\r\n");
    //}

}

//void ddrphy_ddr3_train_read_training()
//{
//	/**********Start RD Training *************/
//	/*Set rd dq training pattern*/
//	//ddrphy_train_info_pre_set(DDRPHY_ALL, DRAM_CS_0);
//	//ddrphy_train_info_pre_set(DDRPHY_0, DRAM_CS_0);
//	//ddrphy_train_ddr3_rd_pattern();    	
//	//ddrphy_train_gate_training(DDRPHY_0);  //pass
//	//ddrphy_train_gate_training(DDRPHY_1);  //pass
//	//reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR), 0, 16, 0x10);
//	//reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG6_F0_ADDR), 0, 16, 0x10);
//	
//
//	//ddrphy_train_info_pre_set(DDRPHY_ALL, DRAM_CS_0);
//	ddrphy_train_info_pre_set(DDRPHY_0, DRAM_CS_0);
//	ddrphy_train_ddr3_rd_pattern();
//	//ddrphy_train_ddr3_rddsk();
//	ddrphy_train_ddr3_rdeye(0, DDRPHY_0);  //pass
//	//ddrphy_train_ddr3_rdeye(0, DDRPHY_1);  //pass
//    ddrphy_train_all_clear();
//	//DDR_DBG_PRINT("\r\nddr3_rd_cs0_training pass\r\n");
//
//
//
//    if(dram_info.cs_num == 1){
//	ddrphy_train_info_pre_set(DDRPHY_ALL, DRAM_CS_1);
//	ddrphy_train_ddr3_rd_pattern();  
//	//ddrphy_train_gate_training(DDRPHY_0);  //pass
//	//ddrphy_train_gate_training(DDRPHY_1);  //pass
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 16, 0x10);
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 16, 0x10);
//
//	ddrphy_train_info_pre_set(DDRPHY_ALL, DRAM_CS_1);
//	ddrphy_train_ddr3_rd_pattern();
//	//ddrphy_train_ddr3_rddsk();
//	ddrphy_train_ddr3_rdeye(0, DDRPHY_0);  //pass
//	ddrphy_train_ddr3_rdeye(0, DDRPHY_1);  //pass
//    ddrphy_train_all_clear();
//	//DDR_DBG_PRINT("\r\nddr3_rd_cs1_training pass\r\n");
//    }
//    else{
//    //DDR_DBG_PRINT("\r\nddr3_read_training pass\r\n");
//    }
//
//}

void ddrphy_ddr5_train_read_training(u32 train_phy, u32 fn)
{
	/**********Start RD Training *************/
	/*Set rd dq training pattern*/
	ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	ddrphy_train_ddr5_rd_pattern();
	//ddrphy_train_gate_training(train_phy,fn);
	
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	//ddrphy_train_ddr4_rd_pattern();
	ddrphy_train_ddr5_rddsk(train_phy);
	ddrphy_train_ddr5_rdeye(1, train_phy,fn);
    ddrphy_train_all_clear();
	//DDR_DBG_PRINT("\r\nddr4_rd_cs0_training pass\r\n");

    //if(dram_info.cs_num == 1){
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//ddrphy_train_ddr4_rd_pattern();
	//ddrphy_train_gate_training(train_phy);
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//ddrphy_train_ddr4_rd_pattern();
	//ddrphy_train_ddr4_rddsk(train_phy);
	//ddrphy_train_ddr4_rdeye(1, train_phy);
    //ddrphy_train_all_clear();
	////DDR_DBG_PRINT("\r\nddr4_rd_cs1_training pass\r\n");
    //}
    //else{
    ////DDR_DBG_PRINT("\r\nddr4_read_training pass\r\n");
    //}

}

void ddrphy_train_ddr4_rd_pattern()//no dq ODT config
{
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), TRAIN_RD_PATTERN_DDR4_1);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA1_ADDR), TRAIN_RD_PATTERN_DDR4_2);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA2_ADDR), TRAIN_RD_PATTERN_DDR4_3);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA3_ADDR), TRAIN_RD_PATTERN_DDR4_4);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA4_ADDR), TRAIN_RD_PATTERN_DDR4_5);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA5_ADDR), TRAIN_RD_PATTERN_DDR4_6);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA6_ADDR), TRAIN_RD_PATTERN_DDR4_7);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA7_ADDR), TRAIN_RD_PATTERN_DDR4_8);
    
    	__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00000000);
	//byte swap
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 0, 16, 0x3120);
	/*set rf_train_dsk_base_value for rd training*/
	/*Make rd training do more accurate*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x40); //rf_phy_update_cnt
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0x80); //rf_train_eye_max_value
	/*set min pass window*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 24, 8, 0x10);
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 12, 8, 0x0);  //rf_train_dsk_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 0,  8, 0x80); //rf_train_dsk_max_value

//	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG3_ADDR), 28, 2, 0x3);//rf_ie_multirank & rf_oe_multirank
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 12, 2, 0x3);//rf_multirank
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 12, 2, 0x3);//rf_multirank

}

void ddrphy_train_ddr5_rd_pattern()//no dq ODT config
{
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), TRAIN_RD_PATTERN_DDR5_1);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA1_ADDR), TRAIN_RD_PATTERN_DDR5_2);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA2_ADDR), TRAIN_RD_PATTERN_DDR5_3);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA3_ADDR), TRAIN_RD_PATTERN_DDR5_4);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA4_ADDR), TRAIN_RD_PATTERN_DDR5_5);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA5_ADDR), TRAIN_RD_PATTERN_DDR5_6);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA6_ADDR), TRAIN_RD_PATTERN_DDR5_7);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA7_ADDR), TRAIN_RD_PATTERN_DDR5_8);
    
    	__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00000000);
	//byte swap
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 0, 16, 0x3120);
	/*set rf_train_dsk_base_value for rd training*/
	/*Make rd training do more accurate*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x40); //rf_phy_update_cnt
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0x80); //rf_train_eye_max_value
	/*set min pass window*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 24, 8, 0x10);
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 12, 8, 0x0);  //rf_train_dsk_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 0,  8, 0x80); //rf_train_dsk_max_value

//	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG3_ADDR), 28, 2, 0x3);//rf_ie_multirank & rf_oe_multirank
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 12, 2, 0x3);//rf_multirank
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 12, 2, 0x3);//rf_multirank

}

//void ddrphy_train_ddr3_rd_pattern()//no dq ODT config
//{
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), TRAIN_RD_PATTERN_DDR3_1);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA1_ADDR), TRAIN_RD_PATTERN_DDR3_2);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA2_ADDR), TRAIN_RD_PATTERN_DDR3_3);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA3_ADDR), TRAIN_RD_PATTERN_DDR3_4);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA4_ADDR), TRAIN_RD_PATTERN_DDR3_5);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA5_ADDR), TRAIN_RD_PATTERN_DDR3_6);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA6_ADDR), TRAIN_RD_PATTERN_DDR3_7);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA7_ADDR), TRAIN_RD_PATTERN_DDR3_8);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA8_ADDR), TRAIN_RD_PATTERN_DDR3_9);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA9_ADDR), TRAIN_RD_PATTERN_DDR3_10);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA10_ADDR), TRAIN_RD_PATTERN_DDR3_11);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA11_ADDR), TRAIN_RD_PATTERN_DDR3_12);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA12_ADDR), TRAIN_RD_PATTERN_DDR3_13);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA13_ADDR), TRAIN_RD_PATTERN_DDR3_14);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA14_ADDR), TRAIN_RD_PATTERN_DDR3_15);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA15_ADDR), TRAIN_RD_PATTERN_DDR3_16);
//
//    
//    #ifdef DRAM_DQ_X16_MODE
//    	__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0xffff0000);
//    #endif
//	//byte swap
////	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 0, 16, 0x3120);
//	/*set rf_train_dsk_base_value for rd training*/
//	/*Make rd training do more accurate*/
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0xa); //rf_phy_update_cnt
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0x80); //rf_train_eye_max_value
//	/*set min pass window*/
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 24, 8, 0x2); //rf_train_dqeye_pass_window_min
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 12, 8, 0x0);  //rf_train_dsk_base_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 0,  8, 0x80); //rf_train_dsk_max_value
//
//	//eg_bit_set(DDRMC_0_(PARM_DDRMC_CFG3_ADDR), 28, 2, 0x3);//rf_ie_multirank & rf_oe_multirank
//	//reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 12, 2, 0x3);//rf_multirank
//	//reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 12, 2, 0x3);//rf_multirank
//
//}


int ddrphy_train_ddr4_rdeye(u32 vref_scan, u32 train_phy, u32 fn)
{
	u32 pass_win_pos1    = 0, pass_win_neg1    = 0;
	u32 pass_win_pos2    = 0, pass_win_neg2    = 0;
	u32 rdeye_pos_value0 = 0, rdeye_neg_value0 = 0;
	u32 rdeye_pos_value1 = 0, rdeye_neg_value1 = 0;
	u32 rdeye_pos_value2 = 0, rdeye_neg_value2 = 0;
	u32 rdeye_pos_value3 = 0, rdeye_neg_value3 = 0;
	u32 rdeye_pos_value4 = 0, rdeye_neg_value4 = 0;
	u32 rdeye_pos_value5 = 0, rdeye_neg_value5 = 0;
	u32 rdeye_pos_value6 = 0, rdeye_neg_value6 = 0;
	u32 rdeye_pos_value7 = 0, rdeye_neg_value7 = 0;

	u32 rdeye_vref_first = 0, rdeye_vref_last  = 0;
	u32 rdeye_vref_min   = 0, rdeye_vref_max   = 0;
	u32 pass_win1        = 0, pass_win2        = 0;
	u32 pass_win_sum1    = 0, pass_win_sum2    = 0;
	u32 pass_win_sum     = 0, pass_win_max     = 0;
	u32 vrefdq_val       = 0, vrefdq_step      = 2;
	u32 i = 0;
	u32 training_result;

	
	//if (!ddrphy_train_sel(TRAIN_RDEYE_INDEX))
	//	return 1;

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------rdeye training start---------------\r\n");
#endif
	// train enable
	ddrphy_train_en(TRAIN_RDEYE_INDEX);
	ddrphy_train_clear();

    if(vref_scan) 
	{
		if(train_info.odt_status) {
			rdeye_vref_min=DDR4_RDEYE_ODT_VREFDQ_MIN;
			rdeye_vref_max=DDR4_RDEYE_ODT_VREFDQ_MAX;
		}
	    else {
			rdeye_vref_min=DDR4_RDEYE_VREFDQ_MIN;
			rdeye_vref_max=DDR4_RDEYE_VREFDQ_MAX;
		}

		for(vrefdq_val=rdeye_vref_min;vrefdq_val<=rdeye_vref_max;vrefdq_val+=vrefdq_step)
	    {
		//set vrefdq at ddrphy
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 24, 1, 0x0);
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, vrefdq_val);
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x03);//rf_train_byte_en

		//else if (train_phy == DDRPHY_ALL) {
		//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 24, 1, 0x0);
		//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, vrefdq_val);
		//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 24, 1, 0x0);
		//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, vrefdq_val);
		//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xff);//rf_train_byte_en
		//}

		//train start

		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
		ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x4);
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start

		wait_100ns(10);
		ddrphy_train_polling_done(TRAIN_RDEYE_INDEX);
		//get dq eye training result
		pass_win1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
		pass_win2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));

		pass_win_sum1 = ((pass_win1 >> 0) & 0xff) + ((pass_win1 >> 8) & 0xff) + ((pass_win1 >> 16) & 0xff) + ((pass_win1 >> 24) & 0xff);
		pass_win_sum2 = ((pass_win2 >> 0) & 0xff) + ((pass_win2 >> 8) & 0xff) + ((pass_win2 >> 16) & 0xff) + ((pass_win2 >> 24) & 0xff);
		pass_win_sum  = pass_win_sum1 + pass_win_sum2;
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
		//ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0xC);
		ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x4);
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);

		if((pass_win_sum > pass_win_max) && (pass_win1+pass_win2 > 0)) {
			pass_win_max     = pass_win_sum;
			rdeye_vref_first = vrefdq_val;
		}
	    else if(pass_win_sum == pass_win_max) {
			rdeye_vref_last  = vrefdq_val;
		}
		ddrphy_train_clear();
	#if TEST_DEBUG_LOG_LEVEL>2
			//DDR_DBG_PRINT("	");
			//DDR_DBG_PRINT("rdeye training phy:");
			print_dec(train_phy);
			//DDR_DBG_PRINT("	");
			//DDR_DBG_PRINT("vref setting:");
			//print_hex(vrefdq_val);
			//DDR_DBG_PRINT("	");
	        //DDR_DBG_PRINT("Pass Window:");
			//print_hex(pass_win1 );
			//print_hex(pass_win2 );
			//print_hex(pass_win_sum );
			//DDR_DBG_PRINT("\r\n");
	#endif
   		}

		if (rdeye_vref_last < rdeye_vref_first) {
			rdeye_vref_last = rdeye_vref_first;
		}
		train_info.vrefdq_rd_val = (rdeye_vref_first + rdeye_vref_last)/2;//take average for first vref and last vref

		//update read vref
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, train_info.vrefdq_rd_val);
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x03);//rf_train_byte_en

	//	else if (train_phy == DDRPHY_ALL) {
	//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, train_info.vrefdq_rd_val);
	//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, train_info.vrefdq_rd_val);
	//		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xff);//rf_train_byte_en
	//	}
		wait_us(1000);
	}

	else {
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x03);//rf_train_byte_en
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x0);//rf_train_ecc_byte_en

		wait_us(10);
	}

//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00000000);
//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);
//   __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0xff00ff00);//read rising edge
//   __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0xff00ff00);//read rising edge
//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00ff00ff);//read falling edge
//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00ff00ff);//read falling edge
	for (i = 0; i < 1; i++) {
		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
		ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x4);
		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);

		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
		wait_100ns(10);
		ddrphy_train_polling_done(TRAIN_RDEYE_INDEX);
		wait_100ns(1);

			training_result = ((__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR)) >> 17) & 0x1);


		//get dq eye training result
		pass_win_pos1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
		pass_win_pos2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));
		pass_win_neg1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
		pass_win_neg2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));

		ddrphy_train_soft_update_dq();

		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
		ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x0);
		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);

			rdeye_pos_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_pos_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_pos_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_pos_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_neg_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_neg_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_neg_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_neg_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+fn*0x100)) & 0xfff;

		//else if (train_phy == DDRPHY_ALL) {
		//	rdeye_pos_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_neg_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//}
	    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x0); //rf_train_start
		ddrphy_train_clear();

#if TEST_DEBUG_LOG_LEVEL>2
		//DDR_DBG_PRINT("\r\nrdeye training phy:");
		//print_dec(train_phy);
		//DDR_DBG_PRINT("Max Window Final: ");
		//print_hex(train_info.vrefdq_rd_val);
		//DDR_DBG_PRINT("\r\nPos Pass Window:");
		//print_hex(pass_win_pos1);
		//print_hex(pass_win_pos2);
		//DDR_DBG_PRINT("\r\nPos Pass Result:");
		//print_hex(rdeye_pos_value0);
		//print_hex(rdeye_pos_value1);
		//print_hex(rdeye_pos_value2);
		//print_hex(rdeye_pos_value3);
		//print_hex(rdeye_pos_value4);
		//print_hex(rdeye_pos_value5);
		//print_hex(rdeye_pos_value6);
		//print_hex(rdeye_pos_value7);
		//DDR_DBG_PRINT("\r\nNeg Pass Window:");
		//print_hex(pass_win_neg1);
		//print_hex(pass_win_neg2);
		//DDR_DBG_PRINT("\r\nNeg Pass Result:");
		//print_hex(rdeye_neg_value0);
		//print_hex(rdeye_neg_value1);
		//print_hex(rdeye_neg_value2);
		//print_hex(rdeye_neg_value3);
		//print_hex(rdeye_neg_value4);
		//print_hex(rdeye_neg_value5);
		//print_hex(rdeye_neg_value6);
		//print_hex(rdeye_neg_value7);
#endif
	}
#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------rdeye training done----------------\r\n");
#endif
	if (training_result)	return 1;
	else return 0;
}


//int ddrphy_train_ddr3_rdeye(u32 vref_scan, u32 train_phy)
//{
//	u32 pass_win_pos1    = 0, pass_win_neg1    = 0;
//	u32 pass_win_pos2    = 0, pass_win_neg2    = 0;
//	u32 rdeye_pos_value0 = 0, rdeye_neg_value0 = 0;
//	u32 rdeye_pos_value1 = 0, rdeye_neg_value1 = 0;
//	u32 rdeye_pos_value2 = 0, rdeye_neg_value2 = 0;
//	u32 rdeye_pos_value3 = 0, rdeye_neg_value3 = 0;
//	u32 rdeye_pos_value4 = 0, rdeye_neg_value4 = 0;
//	u32 rdeye_pos_value5 = 0, rdeye_neg_value5 = 0;
//	u32 rdeye_pos_value6 = 0, rdeye_neg_value6 = 0;
//	u32 rdeye_pos_value7 = 0, rdeye_neg_value7 = 0;
//
//	u32 rdeye_vref_first = 0, rdeye_vref_last  = 0;
//	u32 rdeye_vref_min   = 0, rdeye_vref_max   = 0;
//	u32 pass_win1        = 0, pass_win2        = 0;
//	u32 pass_win_sum1    = 0, pass_win_sum2    = 0;
//	u32 pass_win_sum     = 0, pass_win_max     = 0;
//	u32 vrefdq_val       = 0, vrefdq_step      = 2;
//	u32 i = 0;
//	u32 training_result;
//
//	
//	//if (!ddrphy_train_sel(TRAIN_RDEYE_INDEX))
//	//	return 1;
//
//#if TEST_DEBUG_LOG_LEVEL>2
//	//DDR_DBG_PRINT("\r\n-------------rdeye training start---------------\r\n");
//#endif
//	// train enable
//	ddrphy_train_en(TRAIN_RDEYE_INDEX);
//	ddrphy_train_clear();
//
//
//    if(vref_scan) 
//	{
//		if(train_info.odt_status) {
//			rdeye_vref_min=DDR4_RDEYE_ODT_VREFDQ_MIN;
//			rdeye_vref_max=DDR4_RDEYE_ODT_VREFDQ_MAX;
//		}
//	    else {
//			rdeye_vref_min=DDR4_RDEYE_VREFDQ_MIN;
//			rdeye_vref_max=DDR4_RDEYE_VREFDQ_MAX;
//		}
//
//		for(vrefdq_val=rdeye_vref_min;vrefdq_val<=rdeye_vref_max;vrefdq_val+=vrefdq_step)
//	    {
//		//set vrefdq at ddrphy
//		if (train_phy == DDRPHY_0) {
//			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 1, 0x0);
//			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 8, vrefdq_val);
//			//reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x0f);//rf_train_byte_en
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x03);//rf_train_byte_en
//		}
//		else if (train_phy == DDRPHY_1) {
//			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 1, 0x0);
//			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 8, vrefdq_val);
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xf0);//rf_train_byte_en
//		}
//		else if (train_phy == DDRPHY_ALL) {
//			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 1, 0x0);
//			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 8, vrefdq_val);
//			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 24, 1, 0x0);
//			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 8, vrefdq_val);
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xff);//rf_train_byte_en
//		}
//		wait_us(1000);
//
//		//train start
//
//		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
//		ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x4);
//		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
//		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
//
//		wait_100ns(10);
//		ddrphy_train_polling_done(TRAIN_RDEYE_INDEX);
//		//get dq eye training result
//		pass_win1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
//		pass_win2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));
//
//		pass_win_sum1 = ((pass_win1 >> 0) & 0xff) + ((pass_win1 >> 8) & 0xff) + ((pass_win1 >> 16) & 0xff) + ((pass_win1 >> 24) & 0xff);
//		pass_win_sum2 = ((pass_win2 >> 0) & 0xff) + ((pass_win2 >> 8) & 0xff) + ((pass_win2 >> 16) & 0xff) + ((pass_win2 >> 24) & 0xff);
//		pass_win_sum  = pass_win_sum1 + pass_win_sum2;
//		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
//		ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0xC);
//		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
//
//		if((pass_win_sum > pass_win_max) && (pass_win1+pass_win2 > 0)) {
//			pass_win_max     = pass_win_sum;
//			rdeye_vref_first = vrefdq_val;
//		}
//	    else if(pass_win_sum == pass_win_max) {
//			rdeye_vref_last  = vrefdq_val;
//		}
//		ddrphy_train_clear();
//	#if TEST_DEBUG_LOG_LEVEL>2
//			//DDR_DBG_PRINT("	");
//			//DDR_DBG_PRINT("rdeye training phy:");
//			print_dec(train_phy);
//			//DDR_DBG_PRINT("	");
//			//DDR_DBG_PRINT("vref setting:");
//			//print_hex(vrefdq_val);
//			//DDR_DBG_PRINT("	");
//	        //DDR_DBG_PRINT("Pass Window:");
//			//print_hex(pass_win1 );
//			//print_hex(pass_win2 );
//			//print_hex(pass_win_sum );
//			//DDR_DBG_PRINT("\r\n");
//	#endif
//   		}
//
//		if (rdeye_vref_last < rdeye_vref_first) {
//			rdeye_vref_last = rdeye_vref_first;
//		}
//		train_info.vrefdq_rd_val = (rdeye_vref_first + rdeye_vref_last)/2;//take average for first vref and last vref
//
//		//update read vref
//		if (train_phy == DDRPHY_0) {
//			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 8, train_info.vrefdq_rd_val);
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x0f);//rf_train_byte_en
//		}
//		else if (train_phy == DDRPHY_1) {
//			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 8, train_info.vrefdq_rd_val);
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xf0);//rf_train_byte_en
//		}
//		else if (train_phy == DDRPHY_ALL) {
//			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 8, train_info.vrefdq_rd_val);
//			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+train_info.ddr_freq_num*0x100), 0, 8, train_info.vrefdq_rd_val);
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xff);//rf_train_byte_en
//		}
//		wait_us(1000);
//	}
//
//	else {
//		if (train_phy == DDRPHY_0) {
//			//reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x0f);//rf_train_byte_en
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x03);//rf_train_byte_en
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x0);//rf_train_ecc_byte_en
//		}
//		else if (train_phy == DDRPHY_1) {
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xf0);//rf_train_byte_en
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x0);//rf_train_ecc_byte_en
//		}
//		else if (train_phy == DDRPHY_ALL) {
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xff);//rf_train_byte_en
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x0);//rf_train_ecc_byte_en
//		}
//		wait_us(1);
//	}
//
////    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00000000);
////    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);
////   __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0xff00ff00);//read rising edge
////   __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0xff00ff00);//read rising edge
////    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00ff00ff);//read falling edge
////    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00ff00ff);//read falling edge
//	for (i = 0; i < 1; i++) {
//		//ddr3 enable mpr
//		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
//		//ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x4);
//		ddrmc_mrw(DRAM_MR_3, DRAM_CS_ALL, 0x4);
//		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
//
//		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
//		wait_100ns(10);
//		ddrphy_train_polling_done(TRAIN_RDEYE_INDEX);
//		wait_100ns(1);
//
//		if (train_phy == DDRPHY_ECC) {
//			training_result = ((__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES7_ADDR)) & 0xff) == 0);
//		}
//		else {
//			training_result = ((__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR)) >> 17) & 0x1);
//		}
//
//
//		//get dq eye training result
//		pass_win_pos1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
//		pass_win_pos2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));
//		pass_win_neg1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
//		pass_win_neg2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));
//
//		ddrphy_train_soft_update_dq();
//
//		//ddr3 exit mpr
//		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
//		//ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x0);
//		ddrmc_mrw(DRAM_MR_3, DRAM_CS_ALL, 0x0);
//		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
//
//		if (train_phy == DDRPHY_0) {
//			rdeye_pos_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_neg_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_neg_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_neg_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_neg_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//		}
//		else if (train_phy == DDRPHY_1) {
//			rdeye_pos_value4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_neg_value4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_neg_value5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_neg_value6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_neg_value7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//		}
//		else if (train_phy == DDRPHY_ALL) {
//			rdeye_pos_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_pos_value7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0xfff;
//			rdeye_neg_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0x3ff;
//			rdeye_neg_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0x3ff;
//			rdeye_neg_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0x3ff;
//			rdeye_neg_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0x3ff;
//			rdeye_neg_value4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0x3ff;
//			rdeye_neg_value5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0x3ff;
//			rdeye_neg_value6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0x3ff;
//			rdeye_neg_value7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+train_info.ddr_freq_num*0x100)) & 0x3ff;
//		}
//		ddrphy_train_clear();
//
//#if TEST_DEBUG_LOG_LEVEL>2
//		//DDR_DBG_PRINT("\r\nrdeye training phy:");
//		//print_dec(train_phy);
//		//DDR_DBG_PRINT("Max Window Final: ");
//		//print_hex(train_info.vrefdq_rd_val);
//		//DDR_DBG_PRINT("\r\nPos Pass Window:");
//		//print_hex(pass_win_pos1);
//		//print_hex(pass_win_pos2);
//		//DDR_DBG_PRINT("\r\nPos Pass Result:");
//		//print_hex(rdeye_pos_value0);
//		//print_hex(rdeye_pos_value1);
//		//print_hex(rdeye_pos_value2);
//		//print_hex(rdeye_pos_value3);
//		//print_hex(rdeye_pos_value4);
//		//print_hex(rdeye_pos_value5);
//		//print_hex(rdeye_pos_value6);
//		//print_hex(rdeye_pos_value7);
//		//DDR_DBG_PRINT("\r\nNeg Pass Window:");
//		//print_hex(pass_win_neg1);
//		//print_hex(pass_win_neg2);
//		//DDR_DBG_PRINT("\r\nNeg Pass Result:");
//		//print_hex(rdeye_neg_value0);
//		//print_hex(rdeye_neg_value1);
//		//print_hex(rdeye_neg_value2);
//		//print_hex(rdeye_neg_value3);
//		//print_hex(rdeye_neg_value4);
//		//print_hex(rdeye_neg_value5);
//		//print_hex(rdeye_neg_value6);
//		//print_hex(rdeye_neg_value7);
//#endif
//	}
//#if TEST_DEBUG_LOG_LEVEL>2
//	//DDR_DBG_PRINT("\r\n-------------rdeye training done----------------\r\n");
//#endif
//	if (training_result)	return 1;
//	else return 0;
//}

int ddrphy_train_ddr5_rdeye(u32 vref_scan, u32 train_phy, u32 fn)
{
	u32 pass_win_pos1    = 0, pass_win_neg1    = 0;
	u32 pass_win_pos2    = 0, pass_win_neg2    = 0;
	u32 rdeye_pos_value0 = 0, rdeye_neg_value0 = 0;
	u32 rdeye_pos_value1 = 0, rdeye_neg_value1 = 0;
	u32 rdeye_pos_value2 = 0, rdeye_neg_value2 = 0;
	u32 rdeye_pos_value3 = 0, rdeye_neg_value3 = 0;
	u32 rdeye_pos_value4 = 0, rdeye_neg_value4 = 0;
	u32 rdeye_pos_value5 = 0, rdeye_neg_value5 = 0;
	u32 rdeye_pos_value6 = 0, rdeye_neg_value6 = 0;
	u32 rdeye_pos_value7 = 0, rdeye_neg_value7 = 0;

	u32 rdeye_vref_first = 0, rdeye_vref_last  = 0;
	u32 rdeye_vref_min   = 0, rdeye_vref_max   = 0;
	u32 pass_win1        = 0, pass_win2        = 0;
	u32 pass_win_sum1    = 0, pass_win_sum2    = 0;
	u32 pass_win_sum     = 0, pass_win_max     = 0;
	u32 vrefdq_val       = 0, vrefdq_step      = 2;
	u32 i = 0;
	u32 training_result;

	
	//if (!ddrphy_train_sel(TRAIN_RDEYE_INDEX))
	//	return 1;

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------rdeye training start---------------\r\n");
#endif
	// train enable
	ddrphy_train_en(TRAIN_RDEYE_INDEX);
	ddrphy_train_clear();

    if(vref_scan) 
	{
		if(train_info.odt_status) {
			rdeye_vref_min=DDR4_RDEYE_ODT_VREFDQ_MIN;
			rdeye_vref_max=DDR4_RDEYE_ODT_VREFDQ_MAX;
		}
	    else {
			rdeye_vref_min=DDR4_RDEYE_VREFDQ_MIN;
			rdeye_vref_max=DDR4_RDEYE_VREFDQ_MAX;
		}

		for(vrefdq_val=rdeye_vref_min;vrefdq_val<=rdeye_vref_max;vrefdq_val+=vrefdq_step)
	    {
		//set vrefdq at ddrphy
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 24, 1, 0x0);
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, vrefdq_val);
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x03);//rf_train_byte_en

		//else if (train_phy == DDRPHY_ALL) {
		//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 24, 1, 0x0);
		//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, vrefdq_val);
		//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 24, 1, 0x0);
		//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, vrefdq_val);
		//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xff);//rf_train_byte_en
		//}

		//train start

		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
		ddrmc_mrw(DRAM_MR_25, DRAM_CS_0, 0x1);
		ddrmc_mrw(DRAM_MR_26, DRAM_CS_0, 0xaa);
		ddrmc_mrw(DRAM_MR_27, DRAM_CS_0, 0xcc);
		ddrmc_mrw(DRAM_MR_28, DRAM_CS_0, 0xcc);
		ddrmc_mrw(DRAM_MR_29, DRAM_CS_0, 0xcc);
		ddrmc_mrw(DRAM_MR_30, DRAM_CS_0, 0xaa);
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start

		wait_100ns(10);
		ddrphy_train_polling_done(TRAIN_RDEYE_INDEX);
		//get dq eye training result
		pass_win1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
		pass_win2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));

		pass_win_sum1 = ((pass_win1 >> 0) & 0xff) + ((pass_win1 >> 8) & 0xff) + ((pass_win1 >> 16) & 0xff) + ((pass_win1 >> 24) & 0xff);
		pass_win_sum2 = ((pass_win2 >> 0) & 0xff) + ((pass_win2 >> 8) & 0xff) + ((pass_win2 >> 16) & 0xff) + ((pass_win2 >> 24) & 0xff);
		pass_win_sum  = pass_win_sum1 + pass_win_sum2;
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);

		if((pass_win_sum > pass_win_max) && (pass_win1+pass_win2 > 0)) {
			pass_win_max     = pass_win_sum;
			rdeye_vref_first = vrefdq_val;
		}
	    else if(pass_win_sum == pass_win_max) {
			rdeye_vref_last  = vrefdq_val;
		}
		ddrphy_train_clear();
	#if TEST_DEBUG_LOG_LEVEL>2
			//DDR_DBG_PRINT("	");
			//DDR_DBG_PRINT("rdeye training phy:");
			print_dec(train_phy);
			//DDR_DBG_PRINT("	");
			//DDR_DBG_PRINT("vref setting:");
			//print_hex(vrefdq_val);
			//DDR_DBG_PRINT("	");
	        //DDR_DBG_PRINT("Pass Window:");
			//print_hex(pass_win1 );
			//print_hex(pass_win2 );
			//print_hex(pass_win_sum );
			//DDR_DBG_PRINT("\r\n");
	#endif
   		}

		if (rdeye_vref_last < rdeye_vref_first) {
			rdeye_vref_last = rdeye_vref_first;
		}
		train_info.vrefdq_rd_val = (rdeye_vref_first + rdeye_vref_last)/2;//take average for first vref and last vref

		//update read vref
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, train_info.vrefdq_rd_val);
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x03);//rf_train_byte_en

	//	else if (train_phy == DDRPHY_ALL) {
	//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, train_info.vrefdq_rd_val);
	//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, train_info.vrefdq_rd_val);
	//		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xff);//rf_train_byte_en
	//	}
		wait_us(1000);
	}

	else {
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x03);//rf_train_byte_en
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x0);//rf_train_ecc_byte_en

		wait_us(10);
	}

//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00000000);
//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);
//   __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0xff00ff00);//read rising edge
//   __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0xff00ff00);//read rising edge
//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00ff00ff);//read falling edge
//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00ff00ff);//read falling edge
	for (i = 0; i < 1; i++) {
		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
		ddrmc_mrw(DRAM_MR_25, DRAM_CS_0, 0x1);
		ddrmc_mrw(DRAM_MR_26, DRAM_CS_0, 0xaa);
		ddrmc_mrw(DRAM_MR_27, DRAM_CS_0, 0xcc);
		ddrmc_mrw(DRAM_MR_28, DRAM_CS_0, 0xcc);
		ddrmc_mrw(DRAM_MR_29, DRAM_CS_0, 0xcc);
		ddrmc_mrw(DRAM_MR_30, DRAM_CS_0, 0xaa);        
		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);

		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
		wait_100ns(10);
		ddrphy_train_polling_done(TRAIN_RDEYE_INDEX);
		wait_100ns(1);

			training_result = ((__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR)) >> 17) & 0x1);


		//get dq eye training result
		pass_win_pos1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
		pass_win_pos2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));
		pass_win_neg1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
		pass_win_neg2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));

		ddrphy_train_soft_update_dq();

		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
		//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);

			rdeye_pos_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_pos_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_pos_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_pos_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_neg_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_neg_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_neg_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
			rdeye_neg_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+fn*0x100)) & 0xfff;

		//else if (train_phy == DDRPHY_ALL) {
		//	rdeye_pos_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_pos_value7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
		//	rdeye_neg_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//	rdeye_neg_value7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+fn*0x100)) & 0x3ff;
		//}
	    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x0); //rf_train_start
		ddrphy_train_clear();

#if TEST_DEBUG_LOG_LEVEL>2
		//DDR_DBG_PRINT("\r\nrdeye training phy:");
		//print_dec(train_phy);
		//DDR_DBG_PRINT("Max Window Final: ");
		//print_hex(train_info.vrefdq_rd_val);
		//DDR_DBG_PRINT("\r\nPos Pass Window:");
		//print_hex(pass_win_pos1);
		//print_hex(pass_win_pos2);
		//DDR_DBG_PRINT("\r\nPos Pass Result:");
		//print_hex(rdeye_pos_value0);
		//print_hex(rdeye_pos_value1);
		//print_hex(rdeye_pos_value2);
		//print_hex(rdeye_pos_value3);
		//print_hex(rdeye_pos_value4);
		//print_hex(rdeye_pos_value5);
		//print_hex(rdeye_pos_value6);
		//print_hex(rdeye_pos_value7);
		//DDR_DBG_PRINT("\r\nNeg Pass Window:");
		//print_hex(pass_win_neg1);
		//print_hex(pass_win_neg2);
		//DDR_DBG_PRINT("\r\nNeg Pass Result:");
		//print_hex(rdeye_neg_value0);
		//print_hex(rdeye_neg_value1);
		//print_hex(rdeye_neg_value2);
		//print_hex(rdeye_neg_value3);
		//print_hex(rdeye_neg_value4);
		//print_hex(rdeye_neg_value5);
		//print_hex(rdeye_neg_value6);
		//print_hex(rdeye_neg_value7);
#endif
	}
#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------rdeye training done----------------\r\n");
#endif
	if (training_result)	return 1;
	else return 0;
}


void ddrphy_train_ddr4_rddsk(u32 train_phy)
{
	//if (!ddrphy_train_sel(TRAIN_RDDSK_INDEX))
	//	return;

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------rddsk training start---------------\r\n");
#endif
	ddrphy_train_en(TRAIN_RDDSK_INDEX);
	ddrphy_train_clear();

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n rddsk training starts and displays initial dq dly");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS1 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS2 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS3 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS4_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS5_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS6_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS7_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG13_ADDR)));

#endif
	//train start
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
	//ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0xC);
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
	ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x4);
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
	wait_100ns(1);
	ddrphy_train_polling_done(TRAIN_RDDSK_INDEX);
	wait_100ns(1);
	ddrphy_train_clear();
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
	//ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x8);
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
	//train start clear
    ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x0);
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));

//else if (train_phy == DDRPHY_ALL) {
//    
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG13_ADDR)));
//    
//}
#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n rddsk training starts and displays initial dq dly");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS1 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS2 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS3 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS4_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS5_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS6_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS7_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n rddsk training starts and displays initial dq dly rank1");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS1 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS2 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS3 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS4_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS5_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS6_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS7_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG16_ADDR)));
#endif
#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------rddsk training done----------------\r\n");
#endif
}


//void ddrphy_train_ddr3_rddsk(void)
//{
//	//if (!ddrphy_train_sel(TRAIN_RDDSK_INDEX))
//	//	return;
//
//#if TEST_DEBUG_LOG_LEVEL>2
//	//DDR_DBG_PRINT("\r\n-------------rddsk training start---------------\r\n");
//#endif
//	ddrphy_train_en(TRAIN_RDDSK_INDEX);
//	ddrphy_train_clear();
//
//#if TEST_DEBUG_LOG_LEVEL>2
//	//DDR_DBG_PRINT("\r\n rddsk training starts and displays initial dq dly");
//	//DDR_DBG_PRINT("\r\n DS0 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS1 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS2 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS3 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS4_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS5_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS6_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS7_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG13_ADDR)));
//
//#endif
//	//train start
//	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
//	ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0xC);
//	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
//	wait_100ns(1);
//	ddrphy_train_polling_done(TRAIN_RDDSK_INDEX);
//	wait_100ns(1);
//	ddrphy_train_clear();
//	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
//	ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x8);
//	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
//	//train start clear
//
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG13_ADDR)));
//
//#if TEST_DEBUG_LOG_LEVEL>2
//	//DDR_DBG_PRINT("\r\n rddsk training starts and displays initial dq dly");
//	//DDR_DBG_PRINT("\r\n DS0 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS1 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS2 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS3 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS4_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS5_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS6_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS7_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG12_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG13_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n rddsk training starts and displays initial dq dly rank1");
//	//DDR_DBG_PRINT("\r\n DS0 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG15_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG16_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS1 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG15_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG16_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS2 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG15_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG16_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS3 Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG15_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG16_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS4_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG15_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG16_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS5_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG15_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG16_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS6_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG15_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG16_ADDR)));
//
//	//DDR_DBG_PRINT("\r\n DS7_Perbit Rddsk Phy Result:");
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG15_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG16_ADDR)));
//#endif
//#if TEST_DEBUG_LOG_LEVEL>2
//	//DDR_DBG_PRINT("\r\n-------------rddsk training done----------------\r\n");
//#endif
//}

void ddrphy_train_ddr5_rddsk(u32 train_phy)
{
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG14_F0_ADDR), 24, 1, 0x0);//rf_auto_ref_en
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG16_F0_ADDR), 0, 1, 0x1);//rf_auto_gate_en

	//reg_bit_set(DDRMC_0_(PARM_DDRMC_BIST_CTRL0_ADDR), 12, 4, 0x7);
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_BIST_CTRL0_ADDR), 20, 3, 0x4);
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_BIST_CTRL0_ADDR), 28, 2, 0x0);

	//reg_bit_set(DDRMC_0_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 17, 1, 0x1);

	//reg_bit_set(DDRMC_0_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 12, 3, 0x0);
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x18);

	//reg_bit_set(DDRMC_0_(PARM_DDRMC_TRAIN_CTRL2_ADDR), 0, 32, 0x0);
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_TRAIN_CTRL2_ADDR), 0, 32, 0x0);



	//if (!ddrphy_train_sel(TRAIN_RDDSK_INDEX))
	//	return;

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------rddsk training start---------------\r\n");
#endif
	ddrphy_train_en(TRAIN_RDDSK_INDEX);
	ddrphy_train_clear();

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n rddsk training starts and displays initial dq dly");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS1 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS2 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS3 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS4_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS5_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS6_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS7_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG13_ADDR)));

#endif
	//train start
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
	//ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0xC);
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
	ddrmc_mrw(DRAM_MR_25, DRAM_CS_0, 0x1);
	ddrmc_mrw(DRAM_MR_26, DRAM_CS_0, 0xaa);
	ddrmc_mrw(DRAM_MR_27, DRAM_CS_0, 0xcc);
	ddrmc_mrw(DRAM_MR_28, DRAM_CS_0, 0xcc);
	ddrmc_mrw(DRAM_MR_29, DRAM_CS_0, 0xcc);
	ddrmc_mrw(DRAM_MR_30, DRAM_CS_0, 0xaa);
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
	wait_100ns(1);
	ddrphy_train_polling_done(TRAIN_RDDSK_INDEX);
	wait_100ns(1);
	ddrphy_train_clear();
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x1);
	//ddrmc_mrw(DRAM_MR_3, DRAM_CS_0, 0x8);
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 3, 2, 0x2);
	//train start clear
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));

//else if (train_phy == DDRPHY_ALL) {
//    
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG13_ADDR)));
//
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG12_ADDR)));
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG13_ADDR)));
//    
//}
#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n rddsk training starts and displays initial dq dly");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS1 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS2 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS3 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS4_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS5_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS6_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n DS7_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG12_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG13_ADDR)));

	//DDR_DBG_PRINT("\r\n rddsk training starts and displays initial dq dly rank1");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS1 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS2 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS3 Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS4_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS5_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS6_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG16_ADDR)));

	//DDR_DBG_PRINT("\r\n DS7_Perbit Rddsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG15_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG16_ADDR)));
#endif
#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------rddsk training done----------------\r\n");
#endif
}


void ddrphy_ddr4_train_write_training(u32 train_phy,u32 fn)
{
	/**********Start WR Training *************/
    /*Set wr dq training pattern*/
	ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	ddrphy_train_ddr4_wr_pattern();
	ddrphy_train_ddr4_wrdsk();
	ddrphy_train_ddr4_wreye(1,fn);
    //DDR_DBG_PRINT("\r\nddr4_wr_cs0_training pass\r\n");  

    //if(dram_info.cs_num == 1){
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//ddrphy_train_ddr4_wr_pattern();
	//ddrphy_train_ddr4_wrdsk();
	//ddrphy_train_ddr4_wreye(1);
    ////DDR_DBG_PRINT("\r\nddr4_wr_cs1_training pass\r\n");  
    //}
    //else{
    ////DDR_DBG_PRINT("\r\nddr4_write_training pass\r\n");          
    //}
}

//void ddrphy_ddr3_train_write_training()
//{
//	/**********Start WR Training *************/
//    /*Set wr dq training pattern*/
//	//ddrphy_train_info_pre_set(DDRPHY_ALL, DRAM_CS_0);
//	ddrphy_train_info_pre_set(DDRPHY_0, DRAM_CS_0);
//	ddrphy_train_ddr3_wrdsk_pattern();	
//	ddrphy_train_ddr3_wrdsk();
//	ddrphy_train_ddr3_wreye_pattern();
//	ddrphy_train_ddr3_wreye(0);
//    //DDR_DBG_PRINT("\r\nddr3_wr_cs0_training pass\r\n");  
//
//    if(dram_info.cs_num == 1){
//	ddrphy_train_info_pre_set(DDRPHY_ALL, DRAM_CS_1);
//	//ddrphy_train_ddr3_wrdsk_pattern();
//	//ddrphy_train_ddr3_wrdsk();
//	ddrphy_train_ddr3_wreye_pattern();
//	ddrphy_train_ddr3_wreye(0);
//    //DDR_DBG_PRINT("\r\nddr3_wr_cs1_training pass\r\n");  
//    }
//    else{
//    //DDR_DBG_PRINT("\r\nddr3_write_training pass\r\n");          
//    }
//}

void ddrphy_ddr5_train_write_training(u32 train_phy,u32 fn)
{
	/**********Start WR Training *************/
    /*Set wr dq training pattern*/
	ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	ddrphy_train_ddr5_wr_pattern();
	ddrphy_train_ddr5_wrdsk();
	ddrphy_train_ddr5_wreye(1,fn);
    //DDR_DBG_PRINT("\r\nddr4_wr_cs0_training pass\r\n");  

    //if(dram_info.cs_num == 1){
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//ddrphy_train_ddr4_wr_pattern();
	//ddrphy_train_ddr4_wrdsk();
	//ddrphy_train_ddr4_wreye(1);
    ////DDR_DBG_PRINT("\r\nddr4_wr_cs1_training pass\r\n");  
    //}
    //else{
    ////DDR_DBG_PRINT("\r\nddr4_write_training pass\r\n");          
    //}
}


void ddrphy_train_ddr4_wr_pattern()
{
	u32 i = 0;

	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), TRAIN_WR_PATTERN_DDR4_1);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA1_ADDR), TRAIN_WR_PATTERN_DDR4_2);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA2_ADDR), TRAIN_WR_PATTERN_DDR4_3);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA3_ADDR), TRAIN_WR_PATTERN_DDR4_4);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA4_ADDR), TRAIN_WR_PATTERN_DDR4_5);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA5_ADDR), TRAIN_WR_PATTERN_DDR4_6);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA6_ADDR), TRAIN_WR_PATTERN_DDR4_7);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA7_ADDR), TRAIN_WR_PATTERN_DDR4_8);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA8_ADDR), TRAIN_WR_PATTERN_DDR4_9);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA9_ADDR), TRAIN_WR_PATTERN_DDR4_10);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA10_ADDR), TRAIN_WR_PATTERN_DDR4_11);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA11_ADDR), TRAIN_WR_PATTERN_DDR4_12);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA12_ADDR), TRAIN_WR_PATTERN_DDR4_13);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA13_ADDR), TRAIN_WR_PATTERN_DDR4_14);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA14_ADDR), TRAIN_WR_PATTERN_DDR4_15);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA15_ADDR), TRAIN_WR_PATTERN_DDR4_16);

	//wrdata mask

        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0xffff0000);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_0_ADDR), 0xfffff000);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_1_ADDR), 0x00000000);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_0_ADDR), 0xffff0000);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_1_ADDR), 0xf00ff00f);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_0_ADDR), 0xffff0fff);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_1_ADDR), 0xf0fff0ff);
        
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_0_ADDR), 0x0000ffff);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_1_ADDR), 0x00ffffff);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_0_ADDR), 0x00000fff);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_1_ADDR), 0xffffffff);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_0_ADDR), 0x0000ffff);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_1_ADDR), 0xffff00ff);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_0_ADDR), 0x0000f0ff);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_1_ADDR), 0xff0fff0f);



	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x80); //rf_phy_update_cnt
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0x200); //rf_train_eye_max_value
	/*set min pass window*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 24, 8, 0x10);   //rf_dqeye_pass_min_window
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 12, 8, 0x0);   //rf_train_dsk_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR),  0, 8, 0xf0);  //rf_train_dsk_max_value
}

//void ddrphy_train_ddr3_wrdsk_pattern()
//{
//	u32 i = 0;
//
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), TRAIN_WRDSK_PATTERN_DDR3_1);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA1_ADDR), TRAIN_WRDSK_PATTERN_DDR3_2);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA2_ADDR), TRAIN_WRDSK_PATTERN_DDR3_3);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA3_ADDR), TRAIN_WRDSK_PATTERN_DDR3_4);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA4_ADDR), TRAIN_WRDSK_PATTERN_DDR3_5);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA5_ADDR), TRAIN_WRDSK_PATTERN_DDR3_6);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA6_ADDR), TRAIN_WRDSK_PATTERN_DDR3_7);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA7_ADDR), TRAIN_WRDSK_PATTERN_DDR3_8);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA8_ADDR), TRAIN_WRDSK_PATTERN_DDR3_9);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA9_ADDR), TRAIN_WRDSK_PATTERN_DDR3_10);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA10_ADDR), TRAIN_WRDSK_PATTERN_DDR3_11);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA11_ADDR), TRAIN_WRDSK_PATTERN_DDR3_12);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA12_ADDR), TRAIN_WRDSK_PATTERN_DDR3_13);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA13_ADDR), TRAIN_WRDSK_PATTERN_DDR3_14);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA14_ADDR), TRAIN_WRDSK_PATTERN_DDR3_15);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA15_ADDR), TRAIN_WRDSK_PATTERN_DDR3_16);
//
//	//wrdata mask
//	if (dram_info.dram_dq_width == DRAM_DQ_X64)
//	{
//		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00000000);//read rising edge
//		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);//read rising edge
//		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_0_ADDR), 0xffffffff);//read rising edge
//		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_1_ADDR), 0xffffffff);//read rising edge
//
//	}
//	else
//	{
//        #ifdef DRAM_DQ_X16_MODE
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0xffff0000);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_0_ADDR), 0xfffff000);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_1_ADDR), 0x00000000);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_0_ADDR), 0xffff0000);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_1_ADDR), 0xf00ff00f);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_0_ADDR), 0xffff0fff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_1_ADDR), 0xf0fff0ff);
//        
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_0_ADDR), 0x0000ffff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_1_ADDR), 0x00ffffff);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_0_ADDR), 0x00000fff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_1_ADDR), 0xffffffff);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_0_ADDR), 0x0000ffff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_1_ADDR), 0xffff00ff);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_0_ADDR), 0x0000f0ff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_1_ADDR), 0xff0fff0f);
//        #else
//	     	    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0xf000000f);
//	     	    //__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);
//	     	    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_0_ADDR), 0xff000000);
//	     	    //__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_1_ADDR), 0x00000000);
//	     	    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_0_ADDR), 0xff0000ff);
//	     	    //__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_1_ADDR), 0xf00ff00f);
//	     	    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_0_ADDR), 0xff00ffff);
//	     	    //__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_1_ADDR), 0xf0fff0ff);
//
//	     	    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_0_ADDR), 0xffffffff);
//	     	    //__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_1_ADDR), 0x00ffffff);
//	     	    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_0_ADDR), 0x00ffffff);
//	     	    //__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_1_ADDR), 0xffffffff);
//	     	    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_0_ADDR), 0xffffffff);
//	     	    //__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_1_ADDR), 0xffff00ff);
//	     	    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_0_ADDR), 0xffff00ff);
//	     	//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_1_ADDR), 0xff0fff0f);
//        #endif
//	}
//
//
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x4); //rf_phy_update_cnt
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0x200); //rf_train_eye_max_value
//	/*set min pass window*/
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 24, 8, 0x2);   //rf_dqeye_pass_min_window
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 12, 8, 0x0);   //rf_train_dsk_base_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR),  0, 8, 0xff);  //rf_train_dsk_max_value
//}

//void ddrphy_train_ddr3_wreye_pattern()
//{
//	u32 i = 0;
//
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), TRAIN_WR_PATTERN_DDR3_1);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA1_ADDR), TRAIN_WR_PATTERN_DDR3_2);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA2_ADDR), TRAIN_WR_PATTERN_DDR3_3);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA3_ADDR), TRAIN_WR_PATTERN_DDR3_4);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA4_ADDR), TRAIN_WR_PATTERN_DDR3_5);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA5_ADDR), TRAIN_WR_PATTERN_DDR3_6);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA6_ADDR), TRAIN_WR_PATTERN_DDR3_7);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA7_ADDR), TRAIN_WR_PATTERN_DDR3_8);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA8_ADDR), TRAIN_WR_PATTERN_DDR3_9);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA9_ADDR), TRAIN_WR_PATTERN_DDR3_10);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA10_ADDR), TRAIN_WR_PATTERN_DDR3_11);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA11_ADDR), TRAIN_WR_PATTERN_DDR3_12);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA12_ADDR), TRAIN_WR_PATTERN_DDR3_13);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA13_ADDR), TRAIN_WR_PATTERN_DDR3_14);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA14_ADDR), TRAIN_WR_PATTERN_DDR3_15);
//	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA15_ADDR), TRAIN_WR_PATTERN_DDR3_16);
//
//	//wrdata mask
//	if (dram_info.dram_dq_width == DRAM_DQ_X64)
//	{
//		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00000000);//read rising edge
//		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);//read rising edge
//		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_0_ADDR), 0xffffffff);//read rising edge
//		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_1_ADDR), 0xffffffff);//read rising edge
//
//	}
//	else
//	{
//        #ifdef DRAM_DQ_X16_MODE
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0xffff0000);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_0_ADDR), 0xfffff000);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_1_ADDR), 0x00000000);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_0_ADDR), 0xffff0000);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_1_ADDR), 0xf00ff00f);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_0_ADDR), 0xffff00f0);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_1_ADDR), 0xf0fff0ff);
//        
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_0_ADDR), 0x0000ffff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_1_ADDR), 0x00ffffff);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_0_ADDR), 0x00000fff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_1_ADDR), 0xffffffff);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_0_ADDR), 0x0000ffff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_1_ADDR), 0xffff00ff);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_0_ADDR), 0x0000ff0f);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_1_ADDR), 0xff0fff0f);
//        #else
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0xf000000f);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_0_ADDR), 0xff000000);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_1_ADDR), 0x00000000);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_0_ADDR), 0xff0000ff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_1_ADDR), 0xf00ff00f);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_0_ADDR), 0xff00ffff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_1_ADDR), 0xf0fff0ff);
//        
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_0_ADDR), 0xffffffff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_1_ADDR), 0x00ffffff);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_0_ADDR), 0x00ffffff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_1_ADDR), 0xffffffff);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_0_ADDR), 0xffffffff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_1_ADDR), 0xffff00ff);
//        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_0_ADDR), 0xffff00ff);
//        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_1_ADDR), 0xff0fff0f);
//        #endif
//	}
//
//
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x4); //rf_phy_update_cnt
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0x200); //rf_train_eye_max_value
//	/*set min pass window*/
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 24, 8, 0x2);   //rf_dqeye_pass_min_window
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 12, 8, 0x0);   //rf_train_dsk_base_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR),  0, 8, 0xff);  //rf_train_dsk_max_value
//}

void ddrphy_train_ddr5_wr_pattern()
{
	u32 i = 0;

	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), TRAIN_WR_PATTERN_DDR5_1);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA1_ADDR), TRAIN_WR_PATTERN_DDR5_2);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA2_ADDR), TRAIN_WR_PATTERN_DDR5_3);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA3_ADDR), TRAIN_WR_PATTERN_DDR5_4);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA4_ADDR), TRAIN_WR_PATTERN_DDR5_5);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA5_ADDR), TRAIN_WR_PATTERN_DDR5_6);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA6_ADDR), TRAIN_WR_PATTERN_DDR5_7);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA7_ADDR), TRAIN_WR_PATTERN_DDR5_8);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA8_ADDR), TRAIN_WR_PATTERN_DDR5_9);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA9_ADDR), TRAIN_WR_PATTERN_DDR5_10);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA10_ADDR), TRAIN_WR_PATTERN_DDR5_11);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA11_ADDR), TRAIN_WR_PATTERN_DDR5_12);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA12_ADDR), TRAIN_WR_PATTERN_DDR5_13);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA13_ADDR), TRAIN_WR_PATTERN_DDR5_14);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA14_ADDR), TRAIN_WR_PATTERN_DDR5_15);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA15_ADDR), TRAIN_WR_PATTERN_DDR5_16);

	//wrdata mask

        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0xffff0000);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_0_ADDR), 0xfffff000);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_1_ADDR), 0x00000000);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_0_ADDR), 0xffff0000);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_1_ADDR), 0xf00ff00f);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_0_ADDR), 0xffff0fff);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_1_ADDR), 0xf0fff0ff);
        
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_0_ADDR), 0x0000ffff);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_1_ADDR), 0x00ffffff);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_0_ADDR), 0x00000fff);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_1_ADDR), 0xffffffff);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_0_ADDR), 0x0000ffff);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_1_ADDR), 0xffff00ff);
        		__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_0_ADDR), 0x0000f0ff);
        		//__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_1_ADDR), 0xff0fff0f);



	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x80); //rf_phy_update_cnt
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0x200); //rf_train_eye_max_value
	/*set min pass window*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 24, 8, 0x10);   //rf_dqeye_pass_min_window
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 12, 8, 0x0);   //rf_train_dsk_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR),  0, 8, 0xf0);  //rf_train_dsk_max_value
}

void ddrphy_train_ddr4_wreye(u32 vref_scan,u32 fn)
{
	u32 shift_num  = 0;
	u32 wreye_vref_first = 0, wreye_vref_last  = 0;
	u32 wreye_vref_min   = 0, wreye_vref_max   = 0;
	u32 pass_win1        = 0, pass_win2        = 0;
	u32 pass_win_sum1    = 0, pass_win_sum2    = 0;
	u32 pass_win_sum     = 0, pass_win_max     = 0;
	u32 pass_val1        = 0, pass_val2        = 0;
	u32 vrefdq_val       = 0, vrefdq_step      = 2;
	u32 mr6_vref_wr_val  = 0;
	u32 wreye_val0 = 0;
	u32 wreye_val1 = 0;
	u32 wreye_val2 = 0;
	u32 wreye_val3 = 0;
	u32 wreye_val4 = 0;
	u32 wreye_val5 = 0;
	u32 wreye_val6 = 0;
	u32 wreye_val7 = 0;
	u32 i = 0;

//	if (!ddrphy_train_sel(TRAIN_WREYE_INDEX))
//		return;

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------wreye training start---------------\r\n");
#endif
	//shift left half ddr cycle wreye
//	shift_num = (__raw_readl(DDRPHY_DQ0_(dll_wr_addr[0] + train_info.ddr_freq_num * 0x80)) >> 24 & 0x7) - 0x1;
//	reg_bit_set(DDRPHY_DQ0_(dll_wr_addr[0] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	reg_bit_set(DDRPHY_DQ0_(dll_wr_addr[1] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	reg_bit_set(DDRPHY_DQ1_(dll_wr_addr[2] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	reg_bit_set(DDRPHY_DQ1_(dll_wr_addr[3] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	wait_100ns(1);
//	ddrphy_train_soft_update_dq();//Make the setting take effect
//	ddrphy_fifo_reset();

	//train enable
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR), 0x00000006);
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR), 0x00000006);

//	//wrlvl setting
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 0x00008290);
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 0x00008288);
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR), 0x00008380);
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR), 0x00008348);
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 0x00008120);
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 0x00008110);
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR), 0x000081e8);
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR), 0x000081e0);
	wait_100ns(1);
	ddrphy_train_en(TRAIN_WREYE_INDEX);
	ddrphy_train_clear();
	if(vref_scan) 
	{
		if(train_info.odt_status) {
			wreye_vref_min=DDR4_WREYE_ODT_VREFDQ_MIN;
			wreye_vref_max=DDR4_WREYE_ODT_VREFDQ_MAX;
		}
	    else {
			wreye_vref_min=DDR4_WREYE_VREFDQ_MIN;
			wreye_vref_max=DDR4_WREYE_VREFDQ_MAX;
		}

		for(vrefdq_val=wreye_vref_min;vrefdq_val<=wreye_vref_max;vrefdq_val+=vrefdq_step)
	    {

	//	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG14_F0_ADDR), 12, 1, 0x1);//rf_auto_ref_en
		//set vrefdq at ddrphy
		ddrmc_mrw(DRAM_MR_6, DRAM_CS_ALL, 0x0480+vrefdq_val);

		wait_us(1000);

		//train start
		/*start training*/
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start

		wait_100ns(1);
		ddrphy_train_polling_done(TRAIN_WREYE_INDEX);

		pass_win1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
		pass_win2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));

		pass_win_sum1 = ((pass_win1 >> 0) & 0xff) + ((pass_win1 >> 8) & 0xff) + ((pass_win1 >> 16) & 0xff) + ((pass_win1 >> 24) & 0xff);
		pass_win_sum2 = ((pass_win2 >> 0) & 0xff) + ((pass_win2 >> 8) & 0xff) + ((pass_win2 >> 16) & 0xff) + ((pass_win2 >> 24) & 0xff);
		pass_win_sum  = pass_win_sum1 + pass_win_sum2;

		if((pass_win_sum > pass_win_max) && (pass_win1+pass_win2 > 0)) {
			pass_win_max     = pass_win_sum;
			wreye_vref_first = vrefdq_val;
		}
	    else if(pass_win_sum == pass_win_max) {
			wreye_vref_last  = vrefdq_val;
		}
		ddrphy_train_clear();

	#if TEST_DEBUG_LOG_LEVEL>2
			//DDR_DBG_PRINT("	");
			//DDR_DBG_PRINT("vref setting:");
			//print_hex(vrefdq_val);
			//DDR_DBG_PRINT("	");
	        //DDR_DBG_PRINT("Pass Window:");
			//print_hex(pass_win1 );
			//print_hex(pass_win2 );
			//print_hex(pass_win_sum );
			//DDR_DBG_PRINT("\r\n");
	#endif
	    }
		if (wreye_vref_last < wreye_vref_first) {
			wreye_vref_last = wreye_vref_first;
		}
		mr6_vref_wr_val = (wreye_vref_first + wreye_vref_last)/2;//take average for first vref and last vref

		dram_info.wreye_vref_train_value = mr6_vref_wr_val;

		//update write vref

		ddrmc_mrw(DRAM_MR_6, DRAM_CS_ALL, 0x0480+mr6_vref_wr_val);
		ddrmc_mrw(DRAM_MR_6, DRAM_CS_ALL, 0x0400 + mr6_vref_wr_val);

		wait_us(1000);
	}

	for (i = 0; i < 1; i++) {
		//train start
		/*start training*/
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start

		wait_100ns(1);
		ddrphy_train_polling_done(TRAIN_WREYE_INDEX);

		ddrphy_train_soft_update_dq();

		/*get training result*/
		//[31:0] byte3/2/1/0_pass_window
		pass_val1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR));
		pass_win1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
		pass_val2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR));
		pass_win2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));

		wreye_val0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100));
		wreye_val1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100));
		wreye_val2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100));
		wreye_val3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100));
		wreye_val4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100));
		wreye_val5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100));
		wreye_val6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100));
		wreye_val7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100));

		/*train clear*/
		ddrphy_train_clear();

#if TEST_DEBUG_LOG_LEVEL>2
		//DDR_DBG_PRINT("\r\nWR Pass Window:");
		//print_hex(pass_win1);
		//DDR_DBG_PRINT("	");
		//print_hex(pass_win2);
		//DDR_DBG_PRINT("WR Max Window Final: ");
		//print_hex(mr6_vref_wr_val);
		//DDR_DBG_PRINT("\r\nWR Pass  Value:");
		//print_hex(pass_val1);
		//DDR_DBG_PRINT("	");
		//print_hex(pass_val2);
		//DDR_DBG_PRINT("\r\nWR Pass Result:");
		//print_hex(wreye_val0);
		//print_hex(wreye_val1);
		//print_hex(wreye_val2);
		//print_hex(wreye_val3);
		//print_hex(wreye_val4);
		//print_hex(wreye_val5);
		//print_hex(wreye_val6);
		//print_hex(wreye_val7);
		//DDR_DBG_PRINT("\r\n");
#endif
	}

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------wreye training done----------------\r\n");
#endif
}


//void ddrphy_train_ddr3_wreye(u32 vref_scan)
//{
//	u32 shift_num  = 0;
//	u32 wreye_vref_first = 0, wreye_vref_last  = 0;
//	u32 wreye_vref_min   = 0, wreye_vref_max   = 0;
//	u32 pass_win1        = 0, pass_win2        = 0;
//	u32 pass_win_sum1    = 0, pass_win_sum2    = 0;
//	u32 pass_win_sum     = 0, pass_win_max     = 0;
//	u32 pass_val1        = 0, pass_val2        = 0;
//	u32 vrefdq_val       = 0, vrefdq_step      = 2;
//	u32 mr6_vref_wr_val  = 0;
//	u32 wreye_val0 = 0;
//	u32 wreye_val1 = 0;
//	u32 wreye_val2 = 0;
//	u32 wreye_val3 = 0;
//	u32 wreye_val4 = 0;
//	u32 wreye_val5 = 0;
//	u32 wreye_val6 = 0;
//	u32 wreye_val7 = 0;
//	u32 i = 0;
//
//	//if (!ddrphy_train_sel(TRAIN_WREYE_INDEX))
//	//	return;
//
//#if TEST_DEBUG_LOG_LEVEL>2
//	//DDR_DBG_PRINT("\r\n-------------wreye training start---------------\r\n");
//#endif
//	//shift left half ddr cycle wreye
////	shift_num = (__raw_readl(DDRPHY_DQ0_(dll_wr_addr[0] + train_info.ddr_freq_num * 0x80)) >> 24 & 0x7) - 0x1;
////	reg_bit_set(DDRPHY_DQ0_(dll_wr_addr[0] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
////	reg_bit_set(DDRPHY_DQ0_(dll_wr_addr[1] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
////	reg_bit_set(DDRPHY_DQ1_(dll_wr_addr[2] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
////	reg_bit_set(DDRPHY_DQ1_(dll_wr_addr[3] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
////	wait_100ns(1);
////	ddrphy_train_soft_update_dq();//Make the setting take effect
////	ddrphy_fifo_reset();
//
//	//train enable
////	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR), 0x00000006);
////	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR), 0x00000006);
//
////	//wrlvl setting
////	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 0x00008290);
////	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 0x00008288);
////	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR), 0x00008380);
////	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR), 0x00008348);
////	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 0x00008120);
////	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 0x00008110);
////	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR), 0x000081e8);
////	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR), 0x000081e0);
//	wait_100ns(1);
//	ddrphy_train_en(TRAIN_WREYE_INDEX);
//	ddrphy_train_clear();
//	if(vref_scan) 
//	{
//		if(train_info.odt_status) {
//			wreye_vref_min=DDR4_WREYE_ODT_VREFDQ_MIN;
//			wreye_vref_max=DDR4_WREYE_ODT_VREFDQ_MAX;
//		}
//	    else {
//			wreye_vref_min=DDR4_WREYE_VREFDQ_MIN;
//			wreye_vref_max=DDR4_WREYE_VREFDQ_MAX;
//		}
//
//		for(vrefdq_val=wreye_vref_min;vrefdq_val<=wreye_vref_max;vrefdq_val+=vrefdq_step)
//	    {
//
//	//	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG14_F0_ADDR), 12, 1, 0x1);//rf_auto_ref_en
//		//set vrefdq at ddrphy
//		ddrmc_mrw(DRAM_MR_6, DRAM_CS_ALL, 0x0480+vrefdq_val);
//
//		wait_us(1000);
//
//		//train start
//		/*start training*/
//		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
//
//		wait_100ns(1);
//		ddrphy_train_polling_done(TRAIN_WREYE_INDEX);
//
//		pass_win1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
//		pass_win2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));
//
//		pass_win_sum1 = ((pass_win1 >> 0) & 0xff) + ((pass_win1 >> 8) & 0xff) + ((pass_win1 >> 16) & 0xff) + ((pass_win1 >> 24) & 0xff);
//		pass_win_sum2 = ((pass_win2 >> 0) & 0xff) + ((pass_win2 >> 8) & 0xff) + ((pass_win2 >> 16) & 0xff) + ((pass_win2 >> 24) & 0xff);
//		pass_win_sum  = pass_win_sum1 + pass_win_sum2;
//
//		if((pass_win_sum > pass_win_max) && (pass_win1+pass_win2 > 0)) {
//			pass_win_max     = pass_win_sum;
//			wreye_vref_first = vrefdq_val;
//		}
//	    else if(pass_win_sum == pass_win_max) {
//			wreye_vref_last  = vrefdq_val;
//		}
//		ddrphy_train_clear();
//
//	#if TEST_DEBUG_LOG_LEVEL>2
//			//DDR_DBG_PRINT("	");
//			//DDR_DBG_PRINT("vref setting:");
//			//print_hex(vrefdq_val);
//			//DDR_DBG_PRINT("	");
//	        //DDR_DBG_PRINT("Pass Window:");
//			//print_hex(pass_win1 );
//			//print_hex(pass_win2 );
//			//print_hex(pass_win_sum );
//			//DDR_DBG_PRINT("\r\n");
//	#endif
//	    }
//		if (wreye_vref_last < wreye_vref_first) {
//			wreye_vref_last = wreye_vref_first;
//		}
//		mr6_vref_wr_val = (wreye_vref_first + wreye_vref_last)/2;//take average for first vref and last vref
//
//		dram_info.wreye_vref_train_value = mr6_vref_wr_val;
//
//		//update write vref
//
//		ddrmc_mrw(DRAM_MR_6, DRAM_CS_ALL, 0x0480+mr6_vref_wr_val);
//		ddrmc_mrw(DRAM_MR_6, DRAM_CS_ALL, 0x0400 + mr6_vref_wr_val);
//
//		wait_us(1000);
//	}
//
//	for (i = 0; i < 1; i++) {
//		//train start
//		/*start training*/
//		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
//
//		wait_100ns(1);
//		ddrphy_train_polling_done(TRAIN_WREYE_INDEX);
//
//		ddrphy_train_soft_update_dq();
//
//		/*get training result*/
//		//[31:0] byte3/2/1/0_pass_window
//		pass_val1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR));
//		pass_win1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
//		pass_val2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR));
//		pass_win2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));
//
//		wreye_val0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100));
//		wreye_val1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100));
//		wreye_val2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100));
//		wreye_val3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100));
//		wreye_val4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100));
//		wreye_val5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100));
//		wreye_val6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100));
//		wreye_val7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+train_info.ddr_freq_num*0x100));
//
//		/*train clear*/
//		ddrphy_train_clear();
//
//#if TEST_DEBUG_LOG_LEVEL>2
//		//DDR_DBG_PRINT("\r\nWR Pass Window:");
//		//print_hex(pass_win1);
//		//DDR_DBG_PRINT("	");
//		//print_hex(pass_win2);
//		//DDR_DBG_PRINT("WR Max Window Final: ");
//		//print_hex(mr6_vref_wr_val);
//		//DDR_DBG_PRINT("\r\nWR Pass  Value:");
//		//print_hex(pass_val1);
//		//DDR_DBG_PRINT("	");
//		//print_hex(pass_val2);
//		//DDR_DBG_PRINT("\r\nWR Pass Result:");
//		//print_hex(wreye_val0);
//		//print_hex(wreye_val1);
//		//print_hex(wreye_val2);
//		//print_hex(wreye_val3);
//		//print_hex(wreye_val4);
//		//print_hex(wreye_val5);
//		//print_hex(wreye_val6);
//		//print_hex(wreye_val7);
//		//DDR_DBG_PRINT("\r\n");
//#endif
//	}
//
//#if TEST_DEBUG_LOG_LEVEL>2
//	//DDR_DBG_PRINT("\r\n-------------wreye training done----------------\r\n");
//#endif
//}

void ddrphy_train_ddr5_wreye(u32 vref_scan,u32 fn)
{
	u32 shift_num  = 0;
	u32 wreye_vref_first = 0, wreye_vref_last  = 0;
	u32 wreye_vref_min   = 0, wreye_vref_max   = 0;
	u32 pass_win1        = 0, pass_win2        = 0;
	u32 pass_win_sum1    = 0, pass_win_sum2    = 0;
	u32 pass_win_sum     = 0, pass_win_max     = 0;
	u32 pass_val1        = 0, pass_val2        = 0;
	u32 vrefdq_val       = 0, vrefdq_step      = 2;
	u32 mr6_vref_wr_val  = 0;
	u32 wreye_val0 = 0;
	u32 wreye_val1 = 0;
	u32 wreye_val2 = 0;
	u32 wreye_val3 = 0;
	u32 wreye_val4 = 0;
	u32 wreye_val5 = 0;
	u32 wreye_val6 = 0;
	u32 wreye_val7 = 0;
	u32 i = 0;

//	if (!ddrphy_train_sel(TRAIN_WREYE_INDEX))
//		return;

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------wreye training start---------------\r\n");
#endif
	//shift left half ddr cycle wreye
//	shift_num = (__raw_readl(DDRPHY_DQ0_(dll_wr_addr[0] + train_info.ddr_freq_num * 0x80)) >> 24 & 0x7) - 0x1;
//	reg_bit_set(DDRPHY_DQ0_(dll_wr_addr[0] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	reg_bit_set(DDRPHY_DQ0_(dll_wr_addr[1] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	reg_bit_set(DDRPHY_DQ1_(dll_wr_addr[2] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	reg_bit_set(DDRPHY_DQ1_(dll_wr_addr[3] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	wait_100ns(1);
//	ddrphy_train_soft_update_dq();//Make the setting take effect
//	ddrphy_fifo_reset();

	//train enable
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR), 0x00000006);
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR), 0x00000006);

//	//wrlvl setting
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 0x00008290);
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 0x00008288);
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR), 0x00008380);
//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR), 0x00008348);
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 0x00008120);
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 0x00008110);
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR), 0x000081e8);
//	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR), 0x000081e0);
	wait_100ns(1);
	ddrphy_train_en(TRAIN_WREYE_INDEX);
	ddrphy_train_clear();
	if(vref_scan) 
	{
		if(train_info.odt_status) {
			wreye_vref_min=DDR4_WREYE_ODT_VREFDQ_MIN;
			wreye_vref_max=DDR4_WREYE_ODT_VREFDQ_MAX;
		}
	    else {
			wreye_vref_min=DDR4_WREYE_VREFDQ_MIN;
			wreye_vref_max=DDR4_WREYE_VREFDQ_MAX;
		}

		for(vrefdq_val=wreye_vref_min;vrefdq_val<=wreye_vref_max;vrefdq_val+=vrefdq_step)
	    {

	//	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG14_F0_ADDR), 12, 1, 0x1);//rf_auto_ref_en
		//set vrefdq at ddrphy
		ddrmc_mrw(DRAM_MR_6, DRAM_CS_ALL, 0x0480+vrefdq_val);

		wait_us(1000);

		//train start
		/*start training*/
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start

		wait_100ns(1);
		ddrphy_train_polling_done(TRAIN_WREYE_INDEX);

		pass_win1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
		pass_win2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));

		pass_win_sum1 = ((pass_win1 >> 0) & 0xff) + ((pass_win1 >> 8) & 0xff) + ((pass_win1 >> 16) & 0xff) + ((pass_win1 >> 24) & 0xff);
		pass_win_sum2 = ((pass_win2 >> 0) & 0xff) + ((pass_win2 >> 8) & 0xff) + ((pass_win2 >> 16) & 0xff) + ((pass_win2 >> 24) & 0xff);
		pass_win_sum  = pass_win_sum1 + pass_win_sum2;

		if((pass_win_sum > pass_win_max) && (pass_win1+pass_win2 > 0)) {
			pass_win_max     = pass_win_sum;
			wreye_vref_first = vrefdq_val;
		}
	    else if(pass_win_sum == pass_win_max) {
			wreye_vref_last  = vrefdq_val;
		}
		ddrphy_train_clear();

	#if TEST_DEBUG_LOG_LEVEL>2
			//DDR_DBG_PRINT("	");
			//DDR_DBG_PRINT("vref setting:");
			//print_hex(vrefdq_val);
			//DDR_DBG_PRINT("	");
	        //DDR_DBG_PRINT("Pass Window:");
			//print_hex(pass_win1 );
			//print_hex(pass_win2 );
			//print_hex(pass_win_sum );
			//DDR_DBG_PRINT("\r\n");
	#endif
	    }
		if (wreye_vref_last < wreye_vref_first) {
			wreye_vref_last = wreye_vref_first;
		}
		mr6_vref_wr_val = (wreye_vref_first + wreye_vref_last)/2;//take average for first vref and last vref

		dram_info.wreye_vref_train_value = mr6_vref_wr_val;

		//update write vref

		ddrmc_mrw(DRAM_MR_6, DRAM_CS_ALL, 0x0480+mr6_vref_wr_val);
		ddrmc_mrw(DRAM_MR_6, DRAM_CS_ALL, 0x0400 + mr6_vref_wr_val);

		wait_us(1000);
	}

	for (i = 0; i < 1; i++) {
		//train start
		/*start training*/
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start

		wait_100ns(1);
		ddrphy_train_polling_done(TRAIN_WREYE_INDEX);

		ddrphy_train_soft_update_dq();

		/*get training result*/
		//[31:0] byte3/2/1/0_pass_window
		pass_val1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR));
		pass_win1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
		pass_val2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR));
		pass_win2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));

		wreye_val0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100));
		wreye_val1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100));
		wreye_val2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100));
		wreye_val3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100));
		wreye_val4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100));
		wreye_val5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100));
		wreye_val6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100));
		wreye_val7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100));

		/*train clear*/
		ddrphy_train_clear();

#if TEST_DEBUG_LOG_LEVEL>2
		//DDR_DBG_PRINT("\r\nWR Pass Window:");
		//print_hex(pass_win1);
		//DDR_DBG_PRINT("	");
		//print_hex(pass_win2);
		//DDR_DBG_PRINT("WR Max Window Final: ");
		//print_hex(mr6_vref_wr_val);
		//DDR_DBG_PRINT("\r\nWR Pass  Value:");
		//print_hex(pass_val1);
		//DDR_DBG_PRINT("	");
		//print_hex(pass_val2);
		//DDR_DBG_PRINT("\r\nWR Pass Result:");
		//print_hex(wreye_val0);
		//print_hex(wreye_val1);
		//print_hex(wreye_val2);
		//print_hex(wreye_val3);
		//print_hex(wreye_val4);
		//print_hex(wreye_val5);
		//print_hex(wreye_val6);
		//print_hex(wreye_val7);
		//DDR_DBG_PRINT("\r\n");
#endif
	}

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------wreye training done----------------\r\n");
#endif
}


void ddrphy_train_ddr4_wrdsk(void)
{
	//if (!ddrphy_train_sel(TRAIN_WRDSK_INDEX))
	//	return;

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------wrdsk training start---------------\r\n");
#endif

	// train enable
	ddrphy_train_en(TRAIN_WRDSK_INDEX);
	//train clear
	ddrphy_train_clear();

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n wrdsk training starts and displays initial dq dly");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS1 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS2 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS3 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS4_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS5_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS6_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS7_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG8_ADDR)));

#endif
	//train start
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
	wait_100ns(1);
	ddrphy_train_polling_done(TRAIN_WRDSK_INDEX);
	//start train clear
	//train clear
	ddrphy_train_clear();

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG8_ADDR)));

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n wrdsk training starts and displays initial dq dly");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS1 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS2 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS3 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS4_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS5_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS6_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS7_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG8_ADDR)));

	//DDR_DBG_PRINT("\r\n wrdsk training starts and displays initial dq dly rank1");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS1 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS2 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS3 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS4_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS5_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS6_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS7_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG11_ADDR)));
#endif
#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------wrdsk training done----------------\r\n");
#endif

}

void ddrphy_train_ddr3_wrdsk(void)
{
	//if (!ddrphy_train_sel(TRAIN_WRDSK_INDEX))
	//	return;

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------wrdsk training start---------------\r\n");
#endif

	// train enable
	ddrphy_train_en(TRAIN_WRDSK_INDEX);
	//train clear
	ddrphy_train_clear();

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n wrdsk training starts and displays initial dq dly");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS1 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS2 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS3 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS4_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS5_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS6_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS7_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG8_ADDR)));

#endif
	//train start
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
	wait_100ns(1);
	ddrphy_train_polling_done(TRAIN_WRDSK_INDEX);
	//start train clear
	//train clear
	ddrphy_train_clear();

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG8_ADDR)));

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------wrdsk training done----------------\r\n");
#endif

}

void ddrphy_train_ddr5_wrdsk(void)
{
	//if (!ddrphy_train_sel(TRAIN_WRDSK_INDEX))
	//	return;

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------wrdsk training start---------------\r\n");
#endif

	// train enable
	ddrphy_train_en(TRAIN_WRDSK_INDEX);
	//train clear
	ddrphy_train_clear();

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n wrdsk training starts and displays initial dq dly");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS1 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS2 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS3 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS4_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS5_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS6_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS7_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG8_ADDR)));

#endif
	//train start
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
	wait_100ns(1);
	ddrphy_train_polling_done(TRAIN_WRDSK_INDEX);
	//start train clear
	//train clear
	ddrphy_train_clear();

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG9_ADDR) , __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG10_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG11_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG8_ADDR)));

	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG9_ADDR) , __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG10_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG11_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG8_ADDR)));

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n wrdsk training starts and displays initial dq dly");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS1 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS2 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS3 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS4_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS5_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS6_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG8_ADDR)));
	//DDR_DBG_PRINT("\r\n DS7_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG6_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG7_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG8_ADDR)));

	//DDR_DBG_PRINT("\r\n wrdsk training starts and displays initial dq dly rank1");
	//DDR_DBG_PRINT("\r\n DS0 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS1 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS2 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS3 Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS4_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS5_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS6_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG11_ADDR)));
	//DDR_DBG_PRINT("\r\n DS7_Perbit Wrdsk Phy Result:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG9_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG10_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG11_ADDR)));
#endif
#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------wrdsk training done----------------\r\n");
#endif

}


void ddrphy_train_dqs_dll_cpst(u32 train_byte_sel, u32 dqs_dly, u32 fn)

{
	if (train_byte_sel & 0x1)
	{
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR+fn*0x100), 3, 8, dqs_dly);  //rf_dqs_out_dl_sel_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR+fn*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dll_cpst_start_ds0_f0
		wait_100ns(1);
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dll_cpst_start_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 20, 4, 0xf);//rf_dll_cpst_thr_ds0_f0
	}
	if (train_byte_sel & 0x2)
	{
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR+fn*0x100), 3, 8, dqs_dly);  //rf_dqs_out_dl_sel_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR+fn*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dll_cpst_start_ds0_f0
		wait_100ns(1);
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dll_cpst_start_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+fn*0x100), 20, 4, 0xf);//rf_dll_cpst_thr_ds0_f0
	}
	if (train_byte_sel & 0x4)
	{
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR+fn*0x100), 3, 8, dqs_dly);  //rf_dqs_out_dl_sel_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR+fn*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dll_cpst_start_ds0_f0
		wait_100ns(1);
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dll_cpst_start_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+fn*0x100), 20, 4, 0xf);//rf_dll_cpst_thr_ds0_f0
	}
	if (train_byte_sel & 0x8)
	{
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR+fn*0x100), 3, 8, dqs_dly);  //rf_dqs_out_dl_sel_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR+fn*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dll_cpst_start_ds0_f0
		wait_100ns(1);
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dll_cpst_start_ds0_f0
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+fn*0x100), 20, 4, 0xf);//rf_dll_cpst_thr_ds0_f0
	}
	if (train_byte_sel & 0x10)
	{
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR+fn*0x100), 3, 8, dqs_dly);  //rf_dqs_out_dl_sel_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR+fn*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dll_cpst_start_ds0_f0
		wait_100ns(1);
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dll_cpst_start_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 20, 4, 0xf);//rf_dll_cpst_thr_ds0_f0
	}
	if (train_byte_sel & 0x20)
	{
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR+fn*0x100), 3, 8, dqs_dly);  //rf_dqs_out_dl_sel_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR+fn*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dll_cpst_start_ds0_f0
		wait_100ns(1);
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dll_cpst_start_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+fn*0x100), 20, 4, 0xf);//rf_dll_cpst_thr_ds0_f0
	}
	if (train_byte_sel & 0x40)
	{
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR+fn*0x100), 3, 8, dqs_dly);  //rf_dqs_out_dl_sel_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR+fn*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dll_cpst_start_ds0_f0
		wait_100ns(1);
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dll_cpst_start_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+fn*0x100), 20, 4, 0xf);//rf_dll_cpst_thr_ds0_f0
	}
	if (train_byte_sel & 0x80)
	{
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR+fn*0x100), 3, 8, dqs_dly);  //rf_dqs_out_dl_sel_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR+fn*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x1);//rf_dll_cpst_start_ds0_f0
		wait_100ns(1);
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+fn*0x100), 2, 1, 0x0);//rf_dll_cpst_start_ds0_f0
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+fn*0x100), 20, 4, 0xf);//rf_dll_cpst_thr_ds0_f0
	}
	if (train_byte_sel & 0xf)
	{
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR+fn*0x100), 12, 2, 0x3); //rf_multirank
	}
	if (train_byte_sel & 0xf0)
	{
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG13_F0_ADDR+fn*0x100), 12, 2, 0x3); //rf_multirank
	}
}

void ddrphy_train_wrlvl_config(u32 train_byte_sel, u32 restore_config, u32 fn)
{
	u32 dq0_gen_tmg9_reg;
	u32 dq1_gen_tmg9_reg;


	dq0_gen_tmg9_reg = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG9_F0_ADDR+fn*0x100));//
	dq1_gen_tmg9_reg = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG9_F0_ADDR+fn*0x100));//

	if (restore_config == 0)
	{
		if (train_byte_sel & 0xf)
		{
			//set rpull for low-level tx_dqs  
			//reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 15, 2, 0x2);//rpull
			//set odt to zero
//			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 8, 3, 0x0);//rf_io_ds_dq_odtn_f0
//			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 12, 3, 0x0);//rf_io_ds_dq_odtp_f0
			wait_100ns(1);

			if (train_byte_sel & 0x1)
			{
				//set DDR_DQ IO ien=0 when write cmd is sent
				reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 1, 0x1); //rf_train_byte_en
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0

			}
			if (train_byte_sel & 0x2)
			{
				//set DDR_DQ IO ien=0 when write cmd is sent
				reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 25, 1, 0x1); //rf_train_byte_en
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0

			}
			if (train_byte_sel & 0x4)
			{
				//set DDR_DQ IO ien=0 when write cmd is sent
				reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 26, 1, 0x1); //rf_train_byte_en
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
			}
			if (train_byte_sel & 0x8)
			{
				//set DDR_DQ IO ien=0 when write cmd is sent
				reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 27, 1, 0x1); //rf_train_byte_en
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
				reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0

			}
		}
		if (train_byte_sel & 0xf0)
		{
			//set rpull for low-level tx_dqs  
			//reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 15, 2, 0x2);//rpull
			//set odt to zero
//			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 8, 3, 0x0);//rf_io_ds_dq_odtn_f0
//			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 12, 3, 0x0);//rf_io_ds_dq_odtp_f0
			wait_100ns(1);

			if (train_byte_sel & 0x10)
			{
				//set DDR_DQ IO ien=0 when write cmd is sent
				reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 28, 1, 0x1); //rf_train_byte_en
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
			}
			if (train_byte_sel & 0x20)
			{

				//set DDR_DQ IO ien=0 when write cmd is sent
				reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 29, 1, 0x1); //rf_train_byte_en
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0

			}
			if (train_byte_sel & 0x40)
			{
				//set DDR_DQ IO ien=0 when write cmd is sent
				reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 30, 1, 0x1); //rf_train_byte_en
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
			}
			if (train_byte_sel & 0x80)
			{
				//set DDR_DQ IO ien=0 when write cmd is sent
				reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 31, 1, 0x1); //rf_train_byte_en
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
				reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
			}
		}
	}
	else if (restore_config == 1)
	{
		if (train_byte_sel & 0xf)
		{
			//reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 15, 2, 0x0);//rpull
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR+fn*0x100), 12, 2, 0x0); //rf_multirank
			wait_100ns(1);

			if (train_byte_sel & 0x1)
			{
				__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG3_ADDR), 0x0);//restore phy registers
				__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG4_ADDR), 0x0);//restore phy registers

			}
			if (train_byte_sel & 0x2)
			{
				__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG3_ADDR), 0x0);//restore phy registers
				__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG4_ADDR), 0x0);//restore phy registers

			}
			if (train_byte_sel & 0x4)
			{
				__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG3_ADDR), 0x0);//restore phy registers
				__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG4_ADDR), 0x0);//restore phy registers
			}
			if (train_byte_sel & 0x8)
			{
				__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG3_ADDR), 0x0);//restore phy registers
				__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG4_ADDR), 0x0);//restore phy registers

			}
		}
		if (train_byte_sel & 0xf0)
		{
			//reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 15, 2, 0x0);//rpull
			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG13_F0_ADDR+fn*0x100), 12, 2, 0x0); //rf_multirank
			wait_100ns(1);

			if (train_byte_sel & 0x10)
			{
				__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG3_ADDR), 0x0);//restore phy registers
				__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG4_ADDR), 0x0);//restore phy registers
			}
			if (train_byte_sel & 0x20)
			{
				__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG3_ADDR), 0x0);//restore phy registers
				__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG4_ADDR), 0x0);//restore phy registers

			}
			if (train_byte_sel & 0x40)
			{
				__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG3_ADDR), 0x0);//restore phy registers
				__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG4_ADDR), 0x0);//restore phy registers
			}
			if (train_byte_sel & 0x80)
			{
				__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG3_ADDR), 0x0);//restore phy registers
				__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG4_ADDR), 0x0);//restore phy registers
			}
		}
	}

}


void ddrphy_train_wrlvl_training_pro_max(u32 fn , u32 train_byte_sel)
{
	u32 wrlvl_win = 0;
	u32 wrlvl_val0 = 0;
	u32 wrlvl_val1 = 0;
	u32 wrlvl_val2 = 0;
	u32 wrlvl_val3 = 0;
	u32 wrlvl_val4 = 0;
	u32 wrlvl_val5 = 0;
	u32 wrlvl_val6 = 0;
	u32 wrlvl_val7 = 0;
	u32 wrlvl_sts = 0;
	u32 wrlvl_dqvalue0 = 0;
	u32 wrlvl_dqvalue1 = 0;
	u32 wrlvl_dqvalue2 = 0;
	u32 wrlvl_dqvalue3 = 0;

	u32 wrlvl_ds0_stp0 = 0;
	u32 wrlvl_ds1_stp0 = 0;
	u32 wrlvl_ds2_stp0 = 0;
	u32 wrlvl_ds3_stp0 = 0;

	u32 wrlvl_ds0_stp1 = 0;
	u32 wrlvl_ds1_stp1 = 0;
	u32 wrlvl_ds2_stp1 = 0;
	u32 wrlvl_ds3_stp1 = 0;


	u32 wrlvl_ds0_stp2 = 0;
	u32 wrlvl_ds1_stp2 = 0;
	u32 wrlvl_ds2_stp2 = 0;
	u32 wrlvl_ds3_stp2 = 0;

	u32 wrlvl_ds4_stp0 = 0;
	u32 wrlvl_ds5_stp0 = 0;
	u32 wrlvl_ds6_stp0 = 0;
	u32 wrlvl_ds7_stp0 = 0;

	u32 wrlvl_ds4_stp1 = 0;
	u32 wrlvl_ds5_stp1 = 0;
	u32 wrlvl_ds6_stp1 = 0;
	u32 wrlvl_ds7_stp1 = 0;

	u32 wrlvl_ds4_stp2 = 0;
	u32 wrlvl_ds5_stp2 = 0;
	u32 wrlvl_ds6_stp2 = 0;
	u32 wrlvl_ds7_stp2 = 0;

	u32 wrlvl_dqvalue4 = 0;
	u32 wrlvl_dqvalue5 = 0;
	u32 wrlvl_dqvalue6 = 0;
	u32 wrlvl_dqvalue7 = 0;
	u32 wrlvl_dqvalue8 = 0;
	u32 wrlvl_maxdqvalue = 0;
	u32 wrlvl_max_value = 0xa0;
	u32 wrlvl_dly_offset = 0x10;
	u32 wrlvl_dly_thresh = 0x5;
	u32 wrlvl_dq1_byte_en = 0;
	u32 wrlvl_dq0_byte_en = 0;
	u32 wrlvl_stp0_byte_en = 0;
	u32 wrlvl_stp1_byte_en = 0;
	u32 wrlvl_stp2_byte_en = 0;
	u32 wrlvl_20max_byte_en = 0;
	u32 wrlvl_40max_byte_en = 0;
	u32 wrlvl_dqvalue_20max = 0;
	u32 wrlvl_dqvalue_40max = 0;
	u32 ddr_clk;
	u32 ds0_cfg1_reg;
	u32 ds1_cfg1_reg;
	u32 ds2_cfg1_reg;
	u32 ds3_cfg1_reg;
	u32 ds4_cfg1_reg;
    u32 mr2 = 0;

	//if (!ddrphy_train_sel(TRAIN_WRLVL_INDEX))
	//	return;
	//			ddr_clk=(freq_info+0)->ddr_clk;
#if TEST_DEBUG_LOG_LEVEL>0
	//DDR_DBG_PRINT("\r\n------------------------------------------------\r\n");
	//DDR_DBG_PRINT("\r\n-------------wrlvl training start---------------\r\n");
	//DDR_DBG_PRINT("\r\n------------------------------------------------\r\n");
#endif
	//wr odt off
	//ddrmc_mrw(DRAM_MR_2, DRAM_CS_0, 0x0);
	//ddrmc_mrw(DRAM_MR_5, DRAM_CS_0, 0x400);


	ddrphy_train_wrlvl_config(train_byte_sel, 0, fn);
	ddrphy_train_dqs_dll_cpst(train_byte_sel, 0, fn);//set byte_en and dqs delay sel
    
    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 0,  4, 0x0);//byte swap
    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 4,  4, 0x1);//byte swap
    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 8,  4, 0x2);//byte swap
    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 12, 4, 0x3);//byte swap
    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 16, 4, 0x4);//byte swap
    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 20, 4, 0x5);//byte swap
    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 24, 4, 0x6);//byte swap
    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 28, 4, 0x7);//byte swap


	ddrphy_fifo_reset();
//	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG0_ADDR), 8, 2, 0x3);// rf_byte_pattern
	//	__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x0f0f0f0f);//train data mask, read falling edge, neg dl
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 0, 11, wrlvl_max_value);// rf_train_wrlvl_max_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 16, 11, 0xff);//rf_train_wrlvl_thr_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 20, 11, 0x10);//rf_train_wrlvl_chk_offset

	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR), 0, 8, 0x10);//rf_phy_resync1_cnt
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR), 12, 8, 0x10);//rf_phy_resync2_cnt
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x40);//rf_phy_update_cnt
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 12, 3, 0x0); //rf_train_addr_34_32
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 24, 8, 0x10);//drf_t_wrlvlcd
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 20, 1, 0x0); //rf_train_cs_sel
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0x200); //rf_train_eye_max_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR), 12, 8, 0x20);//rf_phy_resync2
//set bist


/////////////////////////////////////////////////////////////////////////////////////////////////////
//modify MRW Write Leveling Enable in DRAM
//void ddrmc_mrw(DRAM_MR_TYPE_E mr_num, DRAM_CS_TYPE_E cs_num, u32 mr_data)
	if (dram_info.dram_type == DRAM_DDR4)
	{
		//ddrmc_mrw(DRAM_MR_1, DRAM_CS_1, 0x1001);
		ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x81);
	}
	else if (dram_info.dram_type == DRAM_DDR3)
	{
		ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x80);
	}
	else if (dram_info.dram_type == DRAM_DDR5)
	{
		ddrmc_mrw(DRAM_MR_5, DRAM_CS_0, 0x0); //disable DM
		ddrmc_mrw(DRAM_MR_2, DRAM_CS_0, 0x2); //enable WL
	}
	else if (dram_info.dram_type == DRAM_LP4)
    {
        mr2 = (ddrmc_tmg + fn)->dram_mr2_data;
	    ddrmc_mrw(DRAM_MR_2, DRAM_CS_0, mr2 | 0x00000080 );  
    } 
	wait_100ns(1);
    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x1);  //rf_dqs_oe_sel
    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x1);  //rf_dqs_oe_sel
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//bist clear
	////print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR)));
	ddrphy_train_en(TRAIN_WRLVL_INDEX);
	ddrphy_train_clear();
	//wait_100ns(1);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//start wrlvl mode1 training(set rf_train_wrlvl_mode=2'b1 and enable ecc train byte bit to pass wrlvl_step_r bug) to locate the edge of CK-TxDQS (When TxDQS is delayed to CK) of DQ bytes
	//if (train_byte_sel == DDRPHY_ALL)
	//{
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, train_byte_sel); //rf_train_byte_en
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x1); //rf_train_ecc_byte_en, extra byte to skip mode1 bug
//}
//else if (train_byte_sel == DDRPHY_ECC)
//{
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x0); //rf_train_byte_en
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x1); //rf_train_ecc_byte_en
//}
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 28, 2, 0x0);//rf_train_wrlvl_mode

	wait_100ns(1);
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
	wait_100ns(1);

	//wait for wrlvl result
	ddrphy_train_polling_done(TRAIN_WRLVL_INDEX);

	wrlvl_ds0_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 0, 8)) & 0xff;
	wrlvl_ds1_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 8, 8)) & 0xff;
	wrlvl_ds2_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 16, 8)) & 0xff;
	wrlvl_ds3_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 24, 8)) & 0xff;
	wrlvl_ds4_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 0, 8)) & 0xff;
	wrlvl_ds5_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 8, 8)) & 0xff;
	wrlvl_ds6_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 16, 8)) & 0xff;
	wrlvl_ds7_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 24, 8)) & 0xff;
	//wrlvl_dqvalue4 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 0, 8);
	//wrlvl_dqvalue5 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 8, 8);
	//wrlvl_dqvalue6 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 16, 8);
	//wrlvl_dqvalue7 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 24, 8);
	//wrlvl_ds8_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES6_ADDR), 0, 8)) & 0xff;


	ddrphy_train_wrlvl_config(train_byte_sel, 1,fn);
	ddrphy_fifo_reset();

	if (dram_info.dram_type == DRAM_DDR4)
	{
		ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x1);
	}
	//else if (dram_info.dram_type == DRAM_DDR3)
	//{
	//	ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x0);
	//}
	else if (dram_info.dram_type == DRAM_DDR5)
	{
		ddrmc_mrw(DRAM_MR_2, DRAM_CS_0, 0x0);//disable WL
		ddrmc_mrw(DRAM_MR_5, DRAM_CS_0, 0x20);//enable DM
	}
	else if (dram_info.dram_type == DRAM_LP4)
	{
		ddrmc_mrw(DRAM_MR_2, DRAM_CS_0, mr2);
	}
	wait_100ns(1);
 	
    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x0);  //rf_dqs_oe_sel
    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+fn*0x100), 15, 1, 0x0);  //rf_dqs_oe_sel
 	
#if TEST_DEBUG_LOG_LEVEL>0
	//DDR_DBG_PRINT("\r\n-------------wrlvl mode2 training ends(for dq0 byte training)---------------\r\n");
#endif
	wrlvl_win = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_WRLVL_STS_ADDR));

	wrlvl_val0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR+fn*0x100));
	wrlvl_val1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR+fn*0x100));
	wrlvl_val2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR+fn*0x100));
	wrlvl_val3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR+fn*0x100));
	wrlvl_val4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR+fn*0x100));
	wrlvl_val5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR+fn*0x100));
	wrlvl_val6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR+fn*0x100));
	wrlvl_val7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR+fn*0x100));

	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue0);  //rf_dqs_out_dl_sel_ds0_f0
	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue1);  //rf_dqs_out_dl_sel_ds0_f0
	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue2);  //rf_dqs_out_dl_sel_ds0_f0
	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue3);  //rf_dqs_out_dl_sel_ds0_f0
	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue4);  //rf_dqs_out_dl_sel_ds0_f0
	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue5);  //rf_dqs_out_dl_sel_ds0_f0
	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue6);  //rf_dqs_out_dl_sel_ds0_f0
	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue7);  //rf_dqs_out_dl_sel_ds0_f0
	//	ddrphy_train_soft_update_dq();

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\nwrlvl training phy:");
	print_dec(train_info.train_phy_num);
	//DDR_DBG_PRINT("\r\nWRLVL Pass Window:");
	//print_hex(wrlvl_win);
	//DDR_DBG_PRINT("\r\nWRLVL DQ DDRMC Result:\r\n");
	//print_hex(wrlvl_dqvalue0);
	//print_hex(wrlvl_dqvalue1);
	//print_hex(wrlvl_dqvalue2);
	//print_hex(wrlvl_dqvalue3);
	//print_hex(wrlvl_dqvalue4);
	//print_hex(wrlvl_dqvalue5);
	//print_hex(wrlvl_dqvalue6);
	//print_hex(wrlvl_dqvalue7);
	//DDR_DBG_PRINT("\r\nWRLVL Pass ddrphy Result:\r\n");
	//print_hex(wrlvl_val0);
	//print_hex(wrlvl_val1);
	//print_hex(wrlvl_val2);
	//print_hex(wrlvl_val3);
	//print_hex(wrlvl_val4);
	//print_hex(wrlvl_val5);
	//print_hex(wrlvl_val6);
	//print_hex(wrlvl_val7);
	//DDR_DBG_PRINT("\r\nWRLVL Pass ddrphy Result:\r\n");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR)));
#endif
	ddrphy_train_clear();

	//wr odt on
	//ddrmc_mrw(DRAM_MR_2, DRAM_CS_ALL, (ddrmc_tmg + fn)->dram_mr2_data);
	//ddrmc_mrw(DRAM_MR_5, DRAM_CS_ALL, (ddrmc_tmg + fn)->dram_mr5_data);
//	//print_hex((ddrmc_tmg + fn)->dram_mr2_data);
//	//print_hex((ddrmc_tmg + fn)->dram_mr5_data);

}


//void ddrphy_ddr3_train_wrlvl_training_pro_max(u32 fn , u32 train_byte_sel)
//{
//	u32 wrlvl_win = 0;
//	u32 wrlvl_val0 = 0;
//	u32 wrlvl_val1 = 0;
//	u32 wrlvl_val2 = 0;
//	u32 wrlvl_val3 = 0;
//	u32 wrlvl_val4 = 0;
//	u32 wrlvl_val5 = 0;
//	u32 wrlvl_val6 = 0;
//	u32 wrlvl_val7 = 0;
//	u32 wrlvl_sts = 0;
//	u32 wrlvl_dqvalue0 = 0;
//	u32 wrlvl_dqvalue1 = 0;
//	u32 wrlvl_dqvalue2 = 0;
//	u32 wrlvl_dqvalue3 = 0;
//
//	u32 wrlvl_ds0_stp0 = 0;
//	u32 wrlvl_ds1_stp0 = 0;
//	u32 wrlvl_ds2_stp0 = 0;
//	u32 wrlvl_ds3_stp0 = 0;
//
//	u32 wrlvl_ds0_stp1 = 0;
//	u32 wrlvl_ds1_stp1 = 0;
//	u32 wrlvl_ds2_stp1 = 0;
//	u32 wrlvl_ds3_stp1 = 0;
//
//
//	u32 wrlvl_ds0_stp2 = 0;
//	u32 wrlvl_ds1_stp2 = 0;
//	u32 wrlvl_ds2_stp2 = 0;
//	u32 wrlvl_ds3_stp2 = 0;
//
//	u32 wrlvl_ds4_stp0 = 0;
//	u32 wrlvl_ds5_stp0 = 0;
//	u32 wrlvl_ds6_stp0 = 0;
//	u32 wrlvl_ds7_stp0 = 0;
//
//	u32 wrlvl_ds4_stp1 = 0;
//	u32 wrlvl_ds5_stp1 = 0;
//	u32 wrlvl_ds6_stp1 = 0;
//	u32 wrlvl_ds7_stp1 = 0;
//
//	u32 wrlvl_ds4_stp2 = 0;
//	u32 wrlvl_ds5_stp2 = 0;
//	u32 wrlvl_ds6_stp2 = 0;
//	u32 wrlvl_ds7_stp2 = 0;
//
//	u32 wrlvl_dqvalue4 = 0;
//	u32 wrlvl_dqvalue5 = 0;
//	u32 wrlvl_dqvalue6 = 0;
//	u32 wrlvl_dqvalue7 = 0;
//	u32 wrlvl_dqvalue8 = 0;
//	u32 wrlvl_maxdqvalue = 0;
//	u32 wrlvl_max_value = 0xa0;
//	u32 wrlvl_dly_offset = 0x10;
//	u32 wrlvl_dly_thresh = 0x5;
//	u32 wrlvl_dq1_byte_en = 0;
//	u32 wrlvl_dq0_byte_en = 0;
//	u32 wrlvl_stp0_byte_en = 0;
//	u32 wrlvl_stp1_byte_en = 0;
//	u32 wrlvl_stp2_byte_en = 0;
//	u32 wrlvl_20max_byte_en = 0;
//	u32 wrlvl_40max_byte_en = 0;
//	u32 wrlvl_dqvalue_20max = 0;
//	u32 wrlvl_dqvalue_40max = 0;
//	u32 ddr_clk;
//	u32 ds0_cfg1_reg;
//	u32 ds1_cfg1_reg;
//	u32 ds2_cfg1_reg;
//	u32 ds3_cfg1_reg;
//	u32 ds4_cfg1_reg;
//
//	//if (!ddrphy_train_sel(TRAIN_WRLVL_INDEX))
//	//	return;
//	//			ddr_clk=(freq_info+0)->ddr_clk;
//#if TEST_DEBUG_LOG_LEVEL>0
//	//DDR_DBG_PRINT("\r\n------------------------------------------------\r\n");
//	//DDR_DBG_PRINT("\r\n-------------wrlvl training start---------------\r\n");
//	//DDR_DBG_PRINT("\r\n------------------------------------------------\r\n");
//#endif
//	//wr odt off
//	ddrmc_mrw(DRAM_MR_2, DRAM_CS_0, 0x0);
//	ddrmc_mrw(DRAM_MR_5, DRAM_CS_0, 0x400);
//
//
//	ddrphy_train_wrlvl_config(train_byte_sel, 0);
//	ddrphy_train_dqs_dll_cpst(train_byte_sel, 0);//set byte_en and dqs delay sel
//
//
//	ddrphy_fifo_reset();
//	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG0_ADDR), 8, 2, 0x3);// rf_byte_pattern
//	//	__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x0f0f0f0f);//train data mask, read falling edge, neg dl
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 0, 11, wrlvl_max_value);// rf_train_wrlvl_max_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 16, 11, 0xff);//rf_train_wrlvl_thr_value
//
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR), 0, 8, 0x10);//rf_phy_resync1_cnt
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR), 12, 8, 0x10);//rf_phy_resync2_cnt
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x40);//rf_phy_update_cnt
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 12, 3, 0x0); //rf_train_addr_34_32
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 24, 8, 0x10);//drf_t_wrlvlcd
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 20, 1, 0x0); //rf_train_cs_sel
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0x200); //rf_train_eye_max_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR), 12, 8, 0x20);//rf_phy_resync2
////set bist
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
////modify MRW Write Leveling Enable in DRAM
////void ddrmc_mrw(DRAM_MR_TYPE_E mr_num, DRAM_CS_TYPE_E cs_num, u32 mr_data)
//	if (dram_info.dram_type == DRAM_DDR4)
//	{
//		//ddrmc_mrw(DRAM_MR_1, DRAM_CS_1, 0x1001);
//		ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x81);
//	}
//	else if (dram_info.dram_type == DRAM_DDR3)
//	{
//		ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x80);
//	}
//	wait_100ns(1);
//	/////////////////////////////////////////////////////////////////////////////////////////////////////
//	//bist clear
//	////print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR)));
//	ddrphy_train_en(TRAIN_WRLVL_INDEX);
//	ddrphy_train_clear();
//	//wait_100ns(1);
//
//	/////////////////////////////////////////////////////////////////////////////////////////////////////
//	//start wrlvl mode1 training(set rf_train_wrlvl_mode=2'b1 and enable ecc train byte bit to pass wrlvl_step_r bug) to locate the edge of CK-TxDQS (When TxDQS is delayed to CK) of DQ bytes
//	//if (train_byte_sel == DDRPHY_ALL)
//	//{
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, train_byte_sel); //rf_train_byte_en
//	//reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x1); //rf_train_ecc_byte_en, extra byte to skip mode1 bug
//
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 28, 2, 0x1);//rf_train_wrlvl_mode
//
//	wait_100ns(1);
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
//	wait_100ns(1);
//
//	//wait for wrlvl result
//	ddrphy_train_polling_done(TRAIN_WRLVL_INDEX);
//
//	wrlvl_ds0_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 0, 8)) & 0xff;
//	wrlvl_ds1_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 8, 8)) & 0xff;
//	wrlvl_ds2_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 16, 8)) & 0xff;
//	wrlvl_ds3_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 24, 8)) & 0xff;
//	wrlvl_ds4_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 0, 8)) & 0xff;
//	wrlvl_ds5_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 8, 8)) & 0xff;
//	wrlvl_ds6_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 16, 8)) & 0xff;
//	wrlvl_ds7_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 24, 8)) & 0xff;
//	//wrlvl_dqvalue4 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 0, 8);
//	//wrlvl_dqvalue5 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 8, 8);
//	//wrlvl_dqvalue6 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 16, 8);
//	//wrlvl_dqvalue7 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 24, 8);
//	//wrlvl_ds8_stp0 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES6_ADDR), 0, 8)) & 0xff;
//
//	ddrphy_train_clear();
//
//
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, wrlvl_dly_offset);  //rf_train_eye_base_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, train_byte_sel); //rf_train_byte_en
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x1); //rf_train_ecc_byte_en, extra byte to skip mode1 bug
//	ddrphy_train_dqs_dll_cpst(train_byte_sel, wrlvl_dly_offset);
//
//	wait_100ns(1);
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
//	wait_100ns(1);
//	//wait for wrlvl result
//	ddrphy_train_polling_done(TRAIN_WRLVL_INDEX);
//
//	wrlvl_ds0_stp1 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 0, 8)) & 0xff;
//	wrlvl_ds1_stp1 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 8, 8)) & 0xff;
//	wrlvl_ds2_stp1 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 16, 8)) & 0xff;
//	wrlvl_ds3_stp1 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 24, 8)) & 0xff;
//
//	wrlvl_ds4_stp1 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 0, 8)) & 0xff;
//	wrlvl_ds5_stp1 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 8, 8)) & 0xff;
//	wrlvl_ds6_stp1 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 16, 8)) & 0xff;
//	wrlvl_ds7_stp1 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 24, 8)) & 0xff;
//	//wrlvl_dqvalue4 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 0, 8);
//	//wrlvl_dqvalue5 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 8, 8);
//	//wrlvl_dqvalue6 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 16, 8);
//	//wrlvl_dqvalue7 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 24, 8);
//	//wrlvl_ds8_stp1 = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES6_ADDR), 0, 8)) & 0xff;
//
//	//low-level detected byte in the 1st wrlvl training, and enable wrlvl training for these bytes with  0 base value
//	//if ((wrlvl_ds0_stp0 == wrlvl_max_value) & ((wrlvl_ds0_stp1 == wrlvl_max_value) | (wrlvl_ds0_stp1 < wrlvl_dly_thresh)))
//	//{
//	//	wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x1;
//	//}
//	//if ((wrlvl_ds1_stp0 == wrlvl_max_value) & ((wrlvl_ds1_stp1 == wrlvl_max_value) | (wrlvl_ds1_stp1 < wrlvl_dly_thresh)))
//	//{
//	//	wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x2;
//	//}
//	//if ((wrlvl_ds2_stp0 == wrlvl_max_value) & ((wrlvl_ds2_stp1 == wrlvl_max_value) | (wrlvl_ds2_stp1 < wrlvl_dly_thresh)))
//	//{
//	//	wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x4;
//	//}
//	//if ((wrlvl_ds3_stp0 == wrlvl_max_value) & ((wrlvl_ds3_stp1 == wrlvl_max_value) | (wrlvl_ds3_stp1 < wrlvl_dly_thresh)))
//	//{
//	//	wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x8;
//	//}
//	//if ((wrlvl_ds4_stp0 == wrlvl_max_value) & ((wrlvl_ds4_stp1 == wrlvl_max_value) | (wrlvl_ds4_stp1 < wrlvl_dly_thresh)))
//	//{
//	//	wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x10;
//	//}
//	//if ((wrlvl_ds5_stp0 == wrlvl_max_value) & ((wrlvl_ds5_stp1 == wrlvl_max_value) | (wrlvl_ds5_stp1 < wrlvl_dly_thresh)))
//	//{
//	//	wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x20;
//	//}
//	//if ((wrlvl_ds6_stp0 == wrlvl_max_value) & ((wrlvl_ds6_stp1 == wrlvl_max_value) | (wrlvl_ds6_stp1 < wrlvl_dly_thresh)))
//	//{
//	//	wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x40;
//	//}
//	//if ((wrlvl_ds7_stp0 == wrlvl_max_value) & ((wrlvl_ds7_stp1 == wrlvl_max_value) | (wrlvl_ds7_stp1 < wrlvl_dly_thresh)))
//	//{
//	//	wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x80;
//	//}
//	//if ((wrlvl_ds8_stp0 == wrlvl_max_value) & ((wrlvl_ds8_stp1 == wrlvl_max_value) | (wrlvl_ds8_stp1 < wrlvl_dly_offset)))
//	//{
//		//wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x100;
//	//}
//
//	//high-level detected byte in the 1st wrlvl training, and enable wrlvl training for these bytes with max-dqvalue base value
//	if (((wrlvl_ds0_stp0 > wrlvl_dly_thresh) & (wrlvl_ds0_stp0 < wrlvl_max_value)) | ((wrlvl_ds0_stp1 == wrlvl_max_value) & (wrlvl_ds0_stp0 <= wrlvl_dly_thresh)))
//	{
//		wrlvl_stp2_byte_en = wrlvl_stp2_byte_en | 0x1;
//	}
//	else
//	{
//		wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x1;
//	}
//
//	if (((wrlvl_ds1_stp0 > wrlvl_dly_thresh) & (wrlvl_ds1_stp0 < wrlvl_max_value)) | ((wrlvl_ds1_stp1 == wrlvl_max_value) & (wrlvl_ds1_stp0 <= wrlvl_dly_thresh)))
//	{
//		wrlvl_stp2_byte_en = wrlvl_stp2_byte_en | 0x2;
//	}
//	else
//	{
//		wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x2;
//	}
//
//	if (((wrlvl_ds2_stp0 > wrlvl_dly_thresh) & (wrlvl_ds2_stp0 < wrlvl_max_value)) | ((wrlvl_ds2_stp1 == wrlvl_max_value) & (wrlvl_ds2_stp0 <= wrlvl_dly_thresh)))
//	{
//		wrlvl_stp2_byte_en = wrlvl_stp2_byte_en | 0x4;
//	}
//	else
//	{
//		wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x4;
//	}
//	if (((wrlvl_ds3_stp0 > wrlvl_dly_thresh) & (wrlvl_ds3_stp0 < wrlvl_max_value)) | ((wrlvl_ds3_stp1 == wrlvl_max_value) & (wrlvl_ds3_stp0 <= wrlvl_dly_thresh)))
//	{
//		wrlvl_stp2_byte_en = wrlvl_stp2_byte_en | 0x8;
//	}
//	else
//	{
//		wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x8;
//	}
//
//	if (((wrlvl_ds4_stp0 > wrlvl_dly_thresh) & (wrlvl_ds4_stp0 < wrlvl_max_value)) | ((wrlvl_ds4_stp1 == wrlvl_max_value) & (wrlvl_ds4_stp0 <= wrlvl_dly_thresh)))
//	{
//		wrlvl_stp2_byte_en = wrlvl_stp2_byte_en | 0x10;
//	}
//	else
//	{
//		wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x10;
//	}
//
//	if (((wrlvl_ds5_stp0 > wrlvl_dly_thresh) & (wrlvl_ds5_stp0 < wrlvl_max_value)) | ((wrlvl_ds5_stp1 == wrlvl_max_value) & (wrlvl_ds5_stp0 <= wrlvl_dly_thresh)))
//	{
//		wrlvl_stp2_byte_en = wrlvl_stp2_byte_en | 0x20;
//	}
//	else
//	{
//		wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x20;
//	}
//
//	if (((wrlvl_ds6_stp0 > wrlvl_dly_thresh) & (wrlvl_ds6_stp0 < wrlvl_max_value)) | ((wrlvl_ds6_stp1 == wrlvl_max_value) & (wrlvl_ds6_stp0 <= wrlvl_dly_thresh)))
//	{
//		wrlvl_stp2_byte_en = wrlvl_stp2_byte_en | 0x40;
//	}
//	else
//	{
//		wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x40;
//	}
//
//	if (((wrlvl_ds7_stp0 > wrlvl_dly_thresh) & (wrlvl_ds7_stp0 < wrlvl_max_value)) | ((wrlvl_ds7_stp1 == wrlvl_max_value) & (wrlvl_ds7_stp0 <= wrlvl_dly_thresh)))
//	{
//		wrlvl_stp2_byte_en = wrlvl_stp2_byte_en | 0x80;
//	}
//	else
//	{
//		wrlvl_stp1_byte_en = wrlvl_stp1_byte_en | 0x80;
//	}
//	//if (((wrlvl_ds8_stp0 > wrlvl_dly_offset) & (wrlvl_ds8_stp0 < wrlvl_max_value)) | ((wrlvl_ds8_stp1 == wrlvl_max_value) & (wrlvl_ds8_stp0 < wrlvl_dly_offset)))
//	//{
//		//wrlvl_stp2_byte_en = wrlvl_stp2_byte_en | 0x100;
//	//}
//
//	ddrphy_train_clear();
//
//#if TEST_DEBUG_LOG_LEVEL>0
//	//DDR_DBG_PRINT("\r\n-------------wrlvl mode2 training step0 starts---------------\r\n");
//#endif
//
//	wrlvl_dqvalue0 = 0xff;
//	wrlvl_dqvalue1 = 0xff;
//	wrlvl_dqvalue2 = 0xff;
//	wrlvl_dqvalue3 = 0xff;
//	wrlvl_dqvalue8 = 0xff;
//
//
//	wrlvl_stp2_byte_en = wrlvl_stp2_byte_en & train_byte_sel; //rf_train_byte_en
//
//
//	//wrlvl_dqvalue5 = 0xff;
//	//wrlvl_dqvalue6 = 0xff;
//	//wrlvl_dqvalue7 = 0xff;
//	u32 wrlvl_delay_thr0 = 0x20;
//	if (wrlvl_stp2_byte_en & 0x1ff)
//	{
//		if (wrlvl_stp2_byte_en & 0x1) {
//			if (wrlvl_ds0_stp0 < wrlvl_delay_thr0) {
//				if (wrlvl_dqvalue_20max < wrlvl_ds0_stp0) {
//					wrlvl_dqvalue_20max = wrlvl_ds0_stp0;
//				}
//				wrlvl_20max_byte_en = wrlvl_20max_byte_en | 0x1;
//			}
//			else {
//				if (wrlvl_dqvalue_40max < wrlvl_ds0_stp0) {
//					wrlvl_dqvalue_40max = wrlvl_ds0_stp0;
//				}
//				wrlvl_40max_byte_en = wrlvl_40max_byte_en | 0x1;
//			}
//		}
//
//		if (wrlvl_stp2_byte_en & 0x2) {
//			if (wrlvl_ds1_stp0 < wrlvl_delay_thr0) {
//				if (wrlvl_dqvalue_20max < wrlvl_ds1_stp0) {
//					wrlvl_dqvalue_20max = wrlvl_ds1_stp0;
//				}
//				wrlvl_20max_byte_en = wrlvl_20max_byte_en | 0x2;
//			}
//			else {
//				if (wrlvl_dqvalue_40max < wrlvl_ds1_stp0) {
//					wrlvl_dqvalue_40max = wrlvl_ds1_stp0;
//				}
//				wrlvl_40max_byte_en = wrlvl_40max_byte_en | 0x2;
//			}
//		}
//
//		if (wrlvl_stp2_byte_en & 0x4) {
//			if (wrlvl_ds2_stp0 < wrlvl_delay_thr0) {
//				if (wrlvl_dqvalue_20max < wrlvl_ds2_stp0) {
//					wrlvl_dqvalue_20max = wrlvl_ds2_stp0;
//				}
//				wrlvl_20max_byte_en = wrlvl_20max_byte_en | 0x4;
//			}
//			else {
//				if (wrlvl_dqvalue_40max < wrlvl_ds2_stp0) {
//					wrlvl_dqvalue_40max = wrlvl_ds2_stp0;
//				}
//				wrlvl_40max_byte_en = wrlvl_40max_byte_en | 0x4;
//			}
//		}
//
//		if (wrlvl_stp2_byte_en & 0x8) {
//			if (wrlvl_ds3_stp0 < wrlvl_delay_thr0) {
//				if (wrlvl_dqvalue_20max < wrlvl_ds3_stp0) {
//					wrlvl_dqvalue_20max = wrlvl_ds3_stp0;
//				}
//				wrlvl_20max_byte_en = wrlvl_20max_byte_en | 0x8;
//			}
//			else {
//				if (wrlvl_dqvalue_40max < wrlvl_ds3_stp0) {
//					wrlvl_dqvalue_40max = wrlvl_ds3_stp0;
//				}
//				wrlvl_40max_byte_en = wrlvl_40max_byte_en | 0x8;
//			}
//		}
//
//		if (wrlvl_stp2_byte_en & 0x10) {
//			if (wrlvl_ds4_stp0 < wrlvl_delay_thr0) {
//				if (wrlvl_dqvalue_20max < wrlvl_ds4_stp0) {
//					wrlvl_dqvalue_20max = wrlvl_ds4_stp0;
//				}
//				wrlvl_20max_byte_en = wrlvl_20max_byte_en | 0x10;
//			}
//			else {
//				if (wrlvl_dqvalue_40max < wrlvl_ds4_stp0) {
//					wrlvl_dqvalue_40max = wrlvl_ds4_stp0;
//				}
//				wrlvl_40max_byte_en = wrlvl_40max_byte_en | 0x10;
//			}
//		}
//
//		if (wrlvl_stp2_byte_en & 0x20) {
//			if (wrlvl_ds5_stp0 < wrlvl_delay_thr0) {
//				if (wrlvl_dqvalue_20max < wrlvl_ds5_stp0) {
//					wrlvl_dqvalue_20max = wrlvl_ds5_stp0;
//				}
//				wrlvl_20max_byte_en = wrlvl_20max_byte_en | 0x20;
//			}
//			else {
//				if (wrlvl_dqvalue_40max < wrlvl_ds5_stp0) {
//					wrlvl_dqvalue_40max = wrlvl_ds5_stp0;
//				}
//				wrlvl_40max_byte_en = wrlvl_40max_byte_en | 0x20;
//			}
//		}
//
//		if (wrlvl_stp2_byte_en & 0x40) {
//			if (wrlvl_ds6_stp0 < wrlvl_delay_thr0) {
//				if (wrlvl_dqvalue_20max < wrlvl_ds6_stp0) {
//					wrlvl_dqvalue_20max = wrlvl_ds6_stp0;
//				}
//				wrlvl_20max_byte_en = wrlvl_20max_byte_en | 0x40;
//			}
//			else {
//				if (wrlvl_dqvalue_40max < wrlvl_ds6_stp0) {
//					wrlvl_dqvalue_40max = wrlvl_ds6_stp0;
//				}
//				wrlvl_40max_byte_en = wrlvl_40max_byte_en | 0x40;
//			}
//		}
//
//		if (wrlvl_stp2_byte_en & 0x80) {
//			if (wrlvl_ds7_stp0 < wrlvl_delay_thr0) {
//				if (wrlvl_dqvalue_20max < wrlvl_ds7_stp0) {
//					wrlvl_dqvalue_20max = wrlvl_ds7_stp0;
//				}
//				wrlvl_20max_byte_en = wrlvl_20max_byte_en | 0x80;
//			}
//			else {
//				if (wrlvl_dqvalue_40max < wrlvl_ds7_stp0) {
//					wrlvl_dqvalue_40max = wrlvl_ds7_stp0;
//				}
//				wrlvl_40max_byte_en = wrlvl_40max_byte_en | 0x80;
//			}
//		}
//		//start wrlvl mode2 training(set rf_train_wrlvl_mode=2'h2 and disable ecc byte) to start write leveling with a base eye value 
//		if (wrlvl_20max_byte_en & 0x1ff)
//		{
//			//if ((wrlvl_maxdqvalue < wrlvl_ds8_stp0) & (wrlvl_ds8_stp0 < wrlvl_max_value))
//			//{
//				//wrlvl_maxdqvalue = wrlvl_dqvalue8;
//			//}
//
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 28, 2, 0x2);//rf_train_wrlvl_mode
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, (wrlvl_20max_byte_en & 0xff)); //rf_train_byte_en
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, (wrlvl_20max_byte_en & 0x100)); //rf_train_ecc_byte_en
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, wrlvl_dqvalue_20max + wrlvl_dly_offset);  //rf_train_eye_base_value
//
//			ddrphy_train_dqs_dll_cpst(wrlvl_20max_byte_en, wrlvl_dqvalue_20max + wrlvl_dly_offset);
//			wait_100ns(1);
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
//			wait_100ns(1);
//			//wait for wrlvl result
//			ddrphy_train_polling_done(TRAIN_WRLVL_INDEX);
//
//			if (wrlvl_dqvalue_20max & 0x1)
//			{
//				wrlvl_dqvalue0 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 0, 8);
//			}
//			if (wrlvl_dqvalue_20max & 0x2)
//			{
//				wrlvl_dqvalue1 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 8, 8);
//			}
//			if (wrlvl_dqvalue_20max & 0x4)
//			{
//				wrlvl_dqvalue2 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 16, 8);
//			}
//			if (wrlvl_dqvalue_20max & 0x8)
//			{
//				wrlvl_dqvalue3 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 24, 8);
//			}
//			if (wrlvl_dqvalue_20max & 0x10)
//			{
//				wrlvl_dqvalue4 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 0, 8);
//			}
//			if (wrlvl_dqvalue_20max & 0x20)
//			{
//				wrlvl_dqvalue5 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 8, 8);
//			}
//			if (wrlvl_dqvalue_20max & 0x40)
//			{
//				wrlvl_dqvalue6 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 16, 8);
//			}
//			if (wrlvl_dqvalue_20max & 0x80)
//			{
//				wrlvl_dqvalue7 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 24, 8);
//			}
//
//			wrlvl_win = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_WRLVL_STS_ADDR));
//			wrlvl_val0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR));
//			wrlvl_val1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR));
//			wrlvl_val2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR));
//			wrlvl_val3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR));
//			wrlvl_val4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR));
//			wrlvl_val5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR));
//			wrlvl_val6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR));
//			wrlvl_val7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR));
//
//
//			ddrphy_train_clear();
//		}
//
//		//start wrlvl mode2 training(set rf_train_wrlvl_mode=2'h2 and disable ecc byte) to start write leveling with a base eye value 
//		if (wrlvl_40max_byte_en & 0x1ff)
//		{
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 28, 2, 0x2);//rf_train_wrlvl_mode
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, (wrlvl_40max_byte_en & 0xff)); //rf_train_byte_en
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, (wrlvl_40max_byte_en & 0x100)); //rf_train_ecc_byte_en
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, wrlvl_dqvalue_40max + wrlvl_dly_offset);  //rf_train_eye_base_value
//
//			ddrphy_train_dqs_dll_cpst(wrlvl_40max_byte_en, wrlvl_dqvalue_40max + wrlvl_dly_offset);
//			wait_100ns(1);
//			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
//			wait_100ns(1);
//			//wait for wrlvl result
//			ddrphy_train_polling_done(TRAIN_WRLVL_INDEX);
//
//			if (wrlvl_dqvalue_40max & 0x1)
//			{
//				wrlvl_dqvalue0 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 0, 8);
//			}
//			if (wrlvl_dqvalue_40max & 0x2)
//			{
//				wrlvl_dqvalue1 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 8, 8);
//			}
//			if (wrlvl_dqvalue_40max & 0x4)
//			{
//				wrlvl_dqvalue2 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 16, 8);
//			}
//			if (wrlvl_dqvalue_40max & 0x8)
//			{
//				wrlvl_dqvalue3 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 24, 8);
//			}
//			if (wrlvl_dqvalue_40max & 0x10)
//			{
//				wrlvl_dqvalue4 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 0, 8);
//			}
//			if (wrlvl_dqvalue_40max & 0x20)
//			{
//				wrlvl_dqvalue5 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 8, 8);
//			}
//			if (wrlvl_dqvalue_40max & 0x40)
//			{
//				wrlvl_dqvalue6 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 16, 8);
//			}
//			if (wrlvl_dqvalue_40max & 0x80)
//			{
//				wrlvl_dqvalue7 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 24, 8);
//			}
//
//			wrlvl_win = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_WRLVL_STS_ADDR));
//			wrlvl_val0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR));
//			wrlvl_val1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR));
//			wrlvl_val2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR));
//			wrlvl_val3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR));
//			wrlvl_val4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR));
//			wrlvl_val5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR));
//			wrlvl_val6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR));
//			wrlvl_val7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR));
//
//
//			ddrphy_train_clear();
//		}
//	}
//
//	else
//	{
//#if TEST_DEBUG_LOG_LEVEL>2
//		//DDR_DBG_PRINT("\r\n all byte detected at low-level during 1st mode1 wrlvl training:");
//#endif
//	}
//
//
//	wrlvl_stp1_byte_en = wrlvl_stp1_byte_en & train_byte_sel; //rf_train_byte_en
//
//	if (wrlvl_stp1_byte_en & 0x1ff)
//	{
//#if TEST_DEBUG_LOG_LEVEL>0
//		//DDR_DBG_PRINT("\r\n-------------wrlvl mode2 training step1 starts---------------\r\n");
//#endif
//
//		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 28, 2, 0x2);//rf_train_wrlvl_mode
//		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, (wrlvl_stp1_byte_en & 0xff)); //rf_train_byte_en
//		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, (wrlvl_stp1_byte_en & 0x100)); //rf_train_ecc_byte_en
//		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
//
//		ddrphy_train_dqs_dll_cpst(wrlvl_stp1_byte_en, 0x0);
//		wait_100ns(1);
//		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
//		wait_100ns(1);
//		//wait for wrlvl result
//		ddrphy_train_polling_done(TRAIN_WRLVL_INDEX);
//		if (wrlvl_stp1_byte_en & 0x1)
//		{
//			wrlvl_dqvalue0 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 0, 8);
//		}
//		if (wrlvl_stp1_byte_en & 0x2)
//		{
//			wrlvl_dqvalue1 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 8, 8);
//		}
//		if (wrlvl_stp1_byte_en & 0x4)
//		{
//			wrlvl_dqvalue2 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 16, 8);
//		}
//		if (wrlvl_stp1_byte_en & 0x8)
//		{
//			wrlvl_dqvalue3 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR), 24, 8);
//		}
//		if (wrlvl_stp1_byte_en & 0x10)
//		{
//			wrlvl_dqvalue4 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 0, 8);
//		}
//		if (wrlvl_stp1_byte_en & 0x20)
//		{
//			wrlvl_dqvalue5 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 8, 8);
//		}
//		if (wrlvl_stp1_byte_en & 0x40)
//		{
//			wrlvl_dqvalue6 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 16, 8);
//		}
//		if (wrlvl_stp1_byte_en & 0x80)
//		{
//			wrlvl_dqvalue7 = reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR), 24, 8);
//		}
//#if TEST_DEBUG_LOG_LEVEL>2
//		//DDR_DBG_PRINT("\r\n-------------wrlvl mode2 training ends(for dq0 byte training) result :---------------\r\n");
//		//print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR)));
//		//print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR)));
//		//print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR)));
//		//print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES6_ADDR)));
//#endif
//
//		ddrphy_train_clear();
//	}
//
//	ddrphy_train_wrlvl_config(train_byte_sel, 1);
//	ddrphy_fifo_reset();
//
//	if (dram_info.dram_type == DRAM_DDR4)
//	{
//		ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x1);
//	}
//	else if (dram_info.dram_type == DRAM_DDR3)
//	{
//		ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x0);
//	}
//	wait_100ns(1);
//
//
//#if TEST_DEBUG_LOG_LEVEL>0
//	//DDR_DBG_PRINT("\r\n-------------wrlvl mode2 training ends(for dq0 byte training)---------------\r\n");
//#endif
//	wrlvl_win = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_WRLVL_STS_ADDR));
//
//	wrlvl_val0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR));
//	wrlvl_val1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR));
//	wrlvl_val2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR));
//	wrlvl_val3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR));
//	wrlvl_val4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR));
//	wrlvl_val5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR));
//	wrlvl_val6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR));
//	wrlvl_val7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR));
//
//	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue0);  //rf_dqs_out_dl_sel_ds0_f0
//	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue1);  //rf_dqs_out_dl_sel_ds0_f0
//	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue2);  //rf_dqs_out_dl_sel_ds0_f0
//	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue3);  //rf_dqs_out_dl_sel_ds0_f0
//	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue4);  //rf_dqs_out_dl_sel_ds0_f0
//	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue5);  //rf_dqs_out_dl_sel_ds0_f0
//	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue6);  //rf_dqs_out_dl_sel_ds0_f0
//	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR), 3, 8, wrlvl_dqvalue7);  //rf_dqs_out_dl_sel_ds0_f0
//	//	ddrphy_train_soft_update_dq();
//
//
//	ddrphy_train_clear();
//
//	//wr odt on
//	ddrmc_mrw(DRAM_MR_2, DRAM_CS_ALL, (ddrmc_tmg + fn)->dram_mr2_data);
//	ddrmc_mrw(DRAM_MR_5, DRAM_CS_ALL, (ddrmc_tmg + fn)->dram_mr5_data);
////	//print_hex((ddrmc_tmg + fn)->dram_mr2_data);
////	//print_hex((ddrmc_tmg + fn)->dram_mr5_data);
//
//}


//void	ddrphy_train_wrlvl_training(u32 fn)
//{
//    
//	u32 wrlvl_max_value = 0xa0;
//    u32 mr2;
//    u32 wrlvl_fail;
//    u32 wrlvl_sts;
//
//	//if (!ddrphy_train_sel(TRAIN_WRLVL_INDEX))
//	//	return;
//	//			ddr_clk=(freq_info+0)->ddr_clk;
//#if TEST_DEBUG_LOG_LEVEL>0
//	//DDR_DBG_PRINT("\r\n------------------------------------------------\r\n");
//	//DDR_DBG_PRINT("\r\n-------------wrlvl training start---------------\r\n");
//	//DDR_DBG_PRINT("\r\n------------------------------------------------\r\n");
//#endif
//    
//	ddrphy_train_info_pre_set(DDRPHY_ALL, DRAM_CS_0);
//
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x1);//rf_dqs_out_dl_cpst_en_ds0_f0
//
//
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 0, 11, wrlvl_max_value);// rf_train_wrlvl_max_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 16, 11, 0xff);//rf_train_wrlvl_thr_value
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL9_ADDR), 28, 2, 0x2);//rf_train_wrlvl_mode
//
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR), 0, 8, 0x10);//rf_phy_resync1_cnt
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL5_ADDR), 12, 8, 0x10);//rf_phy_resync2_cnt
//
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x40);//rf_phy_update_cnt
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 12, 3, 0x0); //rf_train_addr_34_32
//
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_TMG0_ADDR), 24, 8, 0x10);//drf_t_wrlvlcd
//
//
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG3_ADDR), 16, 9, 0x1ff);//rf_iomux_data_ie & rf_iomux_dqm_ie
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG3_ADDR), 0, 9, 0x1ff);//rf_iomux_data_sel & rf_iomux_dqm_sel
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
//
//
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR+train_info.ddr_freq_num*0x100), 12, 2, 0x3); //rf_multirank
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG13_F0_ADDR+train_info.ddr_freq_num*0x100), 12, 2, 0x3); //rf_multirank
//
//    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x1);  //rf_dqs_oe_sel
//    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x1);  //rf_dqs_oe_sel
//
//	ddrphy_fifo_reset();
//
////modify MRW Write Leveling Enable in DRAM
////void ddrmc_mrw(DRAM_MR_TYPE_E mr_num, DRAM_CS_TYPE_E cs_num, u32 mr_data)
//	if (dram_info.dram_type == DRAM_DDR4)
//	{
//		//ddrmc_mrw(DRAM_MR_1, DRAM_CS_1, 0x1001);
//		ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x81);
//	}
//	else if (dram_info.dram_type == DRAM_DDR3)
//	{
//		ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x80);
//	}
//	else if (dram_info.dram_type == DRAM_LP4)
//    {
//        mr2 = (ddrmc_tmg + fn)->dram_mr2_data;
//	    ddrmc_mrw(DRAM_MR_2, DRAM_CS_ALL, mr2 | 0x00000080 );  
//    } 
//
//
//	//bist clear
//	////print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR)));
//	ddrphy_train_en(TRAIN_WRLVL_INDEX);
//	ddrphy_train_clear();
//
//	wait_100ns(1);
//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
//	wait_100ns(1);
//
//	//wait for wrlvl result
//	ddrphy_train_polling_done(TRAIN_WRLVL_INDEX);
//
//	wrlvl_fail = (reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR), 21, 1)) & 0x1;
//
//	wrlvl_sts = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_WRLVL_STS_ADDR));
//    if (wrlvl_sts & 0x3) {
//		    __raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), ((ddrphy_tmg + fn)->ds0_tmg6)>>1);
//    }
//
//    if ((wrlvl_sts>>2) & 0x3) {
//		    __raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), ((ddrphy_tmg + fn)->ds1_tmg6)>>1);
//    }
//    if ((wrlvl_sts>>4) & 0x3) {
//		    __raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), ((ddrphy_tmg + fn)->ds2_tmg6)>>1);
//    }
//    if ((wrlvl_sts>>6) & 0x3) {
//		    __raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+train_info.ddr_freq_num*0x100), ((ddrphy_tmg + fn)->ds3_tmg6)>>1);
//    }
//
//	ddrphy_train_clear();
//
//    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR+train_info.ddr_freq_num*0x100), 12, 2, 0x0); //rf_multirank
//    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG13_F0_ADDR+train_info.ddr_freq_num*0x100), 12, 2, 0x0); //rf_multirank
//
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG3_ADDR), 16, 9, 0x0);//rf_iomux_data_ie & rf_iomux_dqm_ie
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG3_ADDR), 0, 9, 0x0);//rf_iomux_data_sel & rf_iomux_dqm_sel
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG3_ADDR), 16, 9, 0x0);//rf_iomux_data_ie & rf_iomux_dqm_ie
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG3_ADDR), 0, 9, 0x0);//rf_iomux_data_sel & rf_iomux_dqm_sel
//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG3_ADDR), 16, 9, 0x0);//rf_iomux_data_ie & rf_iomux_dqm_ie
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG3_ADDR), 0, 9, 0x0);//rf_iomux_data_sel & rf_iomux_dqm_sel
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG3_ADDR), 16, 9, 0x0);//rf_iomux_data_ie & rf_iomux_dqm_ie
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG3_ADDR), 0, 9, 0x0);//rf_iomux_data_sel & rf_iomux_dqm_sel
//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG4_ADDR), 16, 9, 0x0);//rf_iomux_data_oe_ds0
//
//
//	ddrphy_fifo_reset();
//
//	if (dram_info.dram_type == DRAM_DDR4)
//	{
//		ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x1);
//	}
//	else if (dram_info.dram_type == DRAM_DDR3)
//	{
//		ddrmc_mrw(DRAM_MR_1, DRAM_CS_0, 0x0);
//	}
//	else if (dram_info.dram_type == DRAM_LP4)
//	{
//		ddrmc_mrw(DRAM_MR_2, DRAM_CS_ALL, mr2);
//	}
//	wait_100ns(1);
// 	
//
//    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x0);  //rf_dqs_oe_sel
//    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG3_F0_ADDR+train_info.ddr_freq_num*0x100), 15, 1, 0x0);  //rf_dqs_oe_sel
//
//    ddrphy_train_all_clear();
//}



void ddrphy_train_coarse_wrlvl_training(u32 fn,u32 train_phy)
{
	u32 wrlvl_coarse_neg_startpoint = 1;  //(wrlvl_coarse_neg_startpoint * -2) cycle
	u32 wrlvl_coarse_range = 3;  		  //wrlvl coarse point search range
	u32 wrlvl_coarse_dqs_out_range = 3;   //wrlvl dqs out dly range
	u32 post_set_meth = 0;
	u32 byte_pass_pos[8] = {0,0,0,0,0,0,0,0};
	u32 byte_pass_flag =    0;
	u32 byte_pass_pos_max = 0;
	u32 byte_pass_pos_min = 0xffff;
	u32 byte_sel = 0;
	u32 dqs_out_sel;
	u32 dqs_out_sel_tmp;
	u32 dq_out_sel;
	u32 ddrmc_dqs_oe_dly;
	u32 ddrmc_data_oe_dly;
	u32 ddrmc_wdm_latency;
	u32 ddrmc_wdata_latency;

#if TEST_DEBUG_LOG_LEVEL > 1
	DDR_DBG_PRINT("\r\n-------------------------------------------------------\r\n");
	DDR_DBG_PRINT("\r\n-------------coarse wrlvl training start---------------\r\n");
	DDR_DBG_PRINT("\r\n-------------------------------------------------------\r\n");
#endif
#if TEST_DEBUG_LOG_LEVEL > 2
	DDR_DBG_PRINT("\r\nfine wrlvl DSX_TMG5 Result:\r\n");
    if (train_phy == DDRPHY_0) {
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR+fn*0x100), 0, 11));  //rf_dqs_out_dl_ds0_f0
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR+fn*0x100), 0, 11));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG5_F0_ADDR+fn*0x100), 0, 11));  //rf_dqs_out_dl_ds0_f0
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG5_F0_ADDR+fn*0x100), 0, 11));
    }
    else {
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR+fn*0x100), 0, 11));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR+fn*0x100), 0, 11));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG5_F0_ADDR+fn*0x100), 0, 11));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG5_F0_ADDR+fn*0x100), 0, 11));
    }
#endif
	// reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR), 0, 11, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 0, 11));  //rf_clkwr_dl_sel_ds0_f0
	// reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR), 0, 11, reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 0, 11));
	// reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR), 0, 11, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG5_F0_ADDR), 0, 11));
	// reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR), 0, 11, reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG5_F0_ADDR), 0, 11));
	// ddrphy_train_soft_update_ac();
	// ddrphy_train_soft_update_dq();
#if TEST_DEBUG_LOG_LEVEL > 2
	DDR_DBG_PRINT("\r\nfine wrlvl DSX_TMG1 init Result:\r\n");
    if (train_phy == DDRPHY_0) {
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100), 0, 31));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100), 0, 31));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100), 0, 31));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100), 0, 31));
    }
    else {
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100), 0, 31));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100), 0, 31));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100), 0, 31));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100), 0, 31));
    }
#endif
	//ddrphy_fifo_reset();

	//dqs_out_sel_ds0 set presim value
    if (train_phy == DDRPHY_0) {
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x3);//rf_train_byte_en
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17, 0x1e);
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17, 0x1e);
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17, 0x1e);
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17, 0x1e);
    }
    else{
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xf0);//rf_train_byte_en
	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17, 0x1e);
	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17, 0x1e);
	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17, 0x1e);
	    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17, 0x1e);
	//#ifndef DRAM_DQ_X16_MODE
	//reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17, 0x1e);
	//reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17, 0x1e);
	//#endif
    }

	//get dqs & dq wrlvl coarse init value
    if (train_phy == DDRPHY_0) {
	    dqs_out_sel 		= reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17);  //rf_dqs_out_sel_ds0_f0
	    dq_out_sel 			= reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100), 24, 2);  //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
    }
    else {
	    dqs_out_sel 		= reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17);  //rf_dqs_out_sel_ds0_f0
	    dq_out_sel 			= reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100), 24, 2);  //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
        
    }
	ddrmc_dqs_oe_dly 	= reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 24, 4);  		  //rf_dqs_oe_dly_f0
	ddrmc_data_oe_dly 	= reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 16, 4);         //rf_data_oe_dly_f0
	ddrmc_wdm_latency 	= reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 8, 4);          //rf_wdm_latenc_f0
	ddrmc_wdata_latency = reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 0, 4);          //rf_wdata_latency_f0
#if TEST_DEBUG_LOG_LEVEL > 2
	DDR_DBG_PRINT("\r\ncoarse wrlvl init dly parameter:\r\n");
	print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 24, 4));			//rf_dqs_oe_dly_f0
	print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 16, 4));			//rf_data_oe_dly_f0
	print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 8, 4));			//rf_wdm_latenc_f0
	print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 0, 4));			//rf_wdata_latency_f0

    if (train_phy == DDRPHY_0) {
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17));	//rf_dqs_out_sel_ds0_f0
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100), 24, 2));	//rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100), 24, 2));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17));	//rf_dqs_out_sel_ds0_f0
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100), 24, 2));	//rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100), 24, 2));
    }
    else {
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100), 24, 2));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100), 24, 2));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100), 24, 2));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100), 24, 2));
    }
#endif

	//set search coarse wrlvl start point
	ddrmc_dqs_oe_dly    = ddrmc_dqs_oe_dly - 0x1 * wrlvl_coarse_neg_startpoint;
	ddrmc_data_oe_dly   = ddrmc_data_oe_dly - 0x1 * wrlvl_coarse_neg_startpoint;
	ddrmc_wdm_latency   = ddrmc_wdm_latency - 0x1 * wrlvl_coarse_neg_startpoint;
	ddrmc_wdata_latency = ddrmc_wdata_latency - 0x1 * wrlvl_coarse_neg_startpoint;
	//wreye pre set
	ddrphy_train_info_pre_set(DDRPHY_0, DRAM_CS_0);
	ddrphy_train_lpddr4_wr_pattern();

	//coarse wrlvl dqs out search
	for(u32 wrlvl_dqs_out_index = 0; wrlvl_dqs_out_index < wrlvl_coarse_dqs_out_range; wrlvl_dqs_out_index++)
	{
		if(wrlvl_dqs_out_index != 0)
		{
			dqs_out_sel = dqs_out_sel << 0x1;  //add all byte dqs out 1cycle dly
            if (train_phy == DDRPHY_0) {
			    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17, dqs_out_sel);  //rf_dqs_out_sel_ds0_f0
			    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17, dqs_out_sel);  //rf_dqs_out_sel_ds0_f0		
			    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17, dqs_out_sel);  //rf_dqs_out_sel_ds0_f0
			    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17, dqs_out_sel);  //rf_dqs_out_sel_ds0_f0		
            }
            else {
			    //#ifndef DRAM_DQ_X16_MODE
			    //reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17, dqs_out_sel);  //rf_dqs_out_sel_ds1_f0
			    //reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17, dqs_out_sel);  //rf_dqs_out_sel_ds1_f0
			    //#endif
			    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17, dqs_out_sel);  //rf_dqs_out_sel_ds0_f0
			    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17, dqs_out_sel);  //rf_dqs_out_sel_ds0_f0		
			    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17, dqs_out_sel);  //rf_dqs_out_sel_ds0_f0
			    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17, dqs_out_sel);  //rf_dqs_out_sel_ds0_f0		
            }
			ddrphy_train_soft_update_ac();
			ddrphy_train_soft_update_dq();
		}
		#if TEST_DEBUG_LOG_LEVEL > 2
			DDR_DBG_PRINT("\r\n------------cur coarse wrlvl dqs out sel is---------\r\n");
			print_hex(wrlvl_dqs_out_index);
			print_hex(dqs_out_sel);
		#endif


		dqs_out_sel_tmp = dqs_out_sel;
		//dq_out and dqs_out preset
		if (dq_out_sel > 0x2) {
			 //nop
		}
		else {
			dq_out_sel = dq_out_sel + 0x2;
			dqs_out_sel_tmp = dqs_out_sel_tmp << 0x1;
		}
	    byte_pass_pos_max = 0;
	    byte_pass_pos_min = 0xffff;
 		byte_pass_flag = 0;

		//all byte coarse wrlvl start
		for(byte_sel = 0; byte_sel < 4; byte_sel++)
		//#ifndef DRAM_DQ_X16_MODE
		//for(byte_sel = 0; byte_sel < 4; byte_sel++)
		//#else
		////for(byte_sel = 0; byte_sel < 4; byte_sel+=2)
		//for(byte_sel = 0; byte_sel < 2; byte_sel+=1)
		//#endif
		{
			byte_pass_pos[byte_sel] = 0;
			#if TEST_DEBUG_LOG_LEVEL > 2
					DDR_DBG_PRINT("\r\n------------coarse wrlvl cur byte is---------\r\n");
					print_hex(byte_sel);
			#endif

			//coarse wrlvl search start
			for (u32 wrlvl_coarse_index = 0; wrlvl_coarse_index < wrlvl_coarse_range; wrlvl_coarse_index++)
			{
				if (wrlvl_coarse_index % 2 == 0x1) {
                    if (train_phy == DDRPHY_0) {
						reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17, dqs_out_sel_tmp); //rf_dqs_out_sel_ds0_f0
						reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, dq_out_sel); 	//rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0

                    }
                    else {
						reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel ) * 0x1000), 0, 17, dqs_out_sel_tmp); //rf_dqs_out_sel_ds0_f0
						reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel )* 0x1000), 24, 2, dq_out_sel);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
                        
                    }

					//if(byte_sel < 2)
					//{
					//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17, dqs_out_sel_tmp); //rf_dqs_out_sel_ds0_f0
					//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, dq_out_sel); 	//rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
					//}
					//else
					//{
					//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel - 2) * 0x1000), 0, 17, dqs_out_sel_tmp); //rf_dqs_out_sel_ds0_f0
					//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel - 2)* 0x1000), 24, 2, dq_out_sel);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
					//}
				}
				else {
					reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 24, 4, ddrmc_dqs_oe_dly + wrlvl_coarse_index / 2);
					reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 16, 4, ddrmc_data_oe_dly + wrlvl_coarse_index / 2);
					reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 8, 4, ddrmc_wdm_latency + wrlvl_coarse_index / 2);
					reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 0, 4, ddrmc_wdata_latency + wrlvl_coarse_index / 2);
                    if (train_phy == DDRPHY_0) {
						reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17, dqs_out_sel_tmp >> 0x1); //rf_dqs_out_sel_ds0_f0
						reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, dq_out_sel - 0x2);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0

                    }
                    else {
						reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel ) * 0x1000), 0, 17, dqs_out_sel_tmp >> 0x1);  //rf_dqs_out_sel_ds0_f0
						reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel )* 0x1000), 24, 2, dq_out_sel - 0x2);     //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
                        
                    }
					//if(byte_sel < 2)
					//{
					//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17, dqs_out_sel_tmp >> 0x1); //rf_dqs_out_sel_ds0_f0
					//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, dq_out_sel - 0x2);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
					//}
					//else
					//{
					//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel - 2) * 0x1000), 0, 17, dqs_out_sel_tmp >> 0x1);  //rf_dqs_out_sel_ds0_f0
					//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel - 2)* 0x1000), 24, 2, dq_out_sel - 0x2);     //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
					//}
				}
				ddrphy_train_soft_update_ac();
				ddrphy_train_soft_update_dq();
				#if TEST_DEBUG_LOG_LEVEL > 2
						DDR_DBG_PRINT("\r\nwrlvl cur dly parameter:\r\n");
						print_hex(wrlvl_coarse_index);
                        if (train_phy == DDRPHY_0) {
							print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17));
							print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2));
                        }
                        else {
							print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel) * 0x1000), 0, 17));
							print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel) * 0x1000), 24, 2));
                        }
						//if(byte_sel < 2)
						//{
						//	print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17));
						//	print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2));
						//}
						//else
						//{
						//	print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel-2) * 0x1000), 0, 17));
						//	print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel-2) * 0x1000), 24, 2));
						//}
						print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 24, 4));
						print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 16, 4));
						print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80),  8, 4));
						print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80),  0, 4));
				#endif

				//wreye polling
				//ddrphy_train_info_pre_set(DDRPHY_ALL, DRAM_CS_0);
				ddrphy_train_en(TRAIN_WREYE_INDEX);
				ddrphy_train_clear();
                if (train_phy == DDRPHY_0) {
				    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x1 << byte_sel);
                }
                else {
				    reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x1 << (byte_sel+4));
                }
				reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
				wait_100ns(1);
				ddrphy_train_polling_done(TRAIN_WREYE_INDEX);

				//check wreye result
				if(reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR), 28, 2) == 0x1)  //wreye pass
				{
                    if (train_phy == DDRPHY_0) {
					    byte_pass_pos[byte_sel] = wrlvl_coarse_index;
					    byte_pass_flag = byte_pass_flag | (0x1 << byte_sel);
					    if(byte_pass_pos[byte_sel] > byte_pass_pos_max)
					    {
					    	byte_pass_pos_max = byte_pass_pos[byte_sel];
					    }

					    if(byte_pass_pos[byte_sel] < byte_pass_pos_min)
					    {
					    	byte_pass_pos_min = byte_pass_pos[byte_sel];
					    }
                    }
                    else {
					    byte_pass_pos[byte_sel+4] = wrlvl_coarse_index;
					    byte_pass_flag = byte_pass_flag | (0x1 << (byte_sel+4));
					    if(byte_pass_pos[byte_sel+4] > byte_pass_pos_max)
					    {
					    	byte_pass_pos_max = byte_pass_pos[byte_sel+4];
					    }

					    if(byte_pass_pos[byte_sel+4] < byte_pass_pos_min)
					    {
					    	byte_pass_pos_min = byte_pass_pos[byte_sel+4];
					    }
                        
                        
                    }
					//DDR_DBG_PRINT("\r\n------------cur byte coarse wrlvl passed!---------\r\n");
					//DDR_DBG_PRINT("\r\ncur byte coarse wrlvl pass window and dly parameter:\r\n");
					//print_hex(wrlvl_coarse_index);
					// print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR), 24, 4));
					// print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR), 16, 4));
					// print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR),  8, 4));
					// print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR),  0, 4));
					// if(byte_sel < 2)
					// {
					// 	print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR)));  //train_dqeye_value_3_0
					// 	print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR)));  //train_dqeye_pass_value_3_0
					// 	print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR + byte_sel * 0x1000), 0, 17));  //rf_dqs_out_sel_ds0_f0
					// 	print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR + byte_sel * 0x1000), 24, 2));  //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
					// 	print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR + byte_sel * 0x1000)));         //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0 rf_clkwr_cnt_ds0_f0  rf_clk_wr_dl_cpst_en_ds0
					// }
					// else
					// {
					// 	print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR)));  //train_dqeye_value_7_4
					// 	print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR)));  //train_dqeye_pass_value_7_4
					// 	print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR + (byte_sel-4) * 0x1000), 0, 17));  //rf_dqs_out_sel_ds0_f0
					// 	print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR + (byte_sel-4) * 0x1000), 24, 2));  //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
					// 	print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR + (byte_sel-4) * 0x1000)));         //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0 rf_clkwr_cnt_ds0_f0  rf_clk_wr_dl_cpst_en_ds0
					// }
					ddrphy_train_soft_update_dq();
					ddrphy_train_clear();
					break;
				}
				else if(reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR),28,2) == 0x3) //wreye failed
				{
					if(wrlvl_coarse_index < wrlvl_coarse_range - 1)
					{
						ddrphy_train_soft_update_dq();
					}
					else
					{
						ddrphy_train_soft_update_dq();
						DDR_DBG_PRINT("\r\n------------cur byte coarse wrlvl failed!---------\r\n");
					}
				}
				ddrphy_train_clear();
			}  //coarse wrlvl search end
		} 	//all byte coarse wrlvl end

		//check all byte coarse wrlvl result

		//#ifndef DRAM_DQ_X16_MODE
		//if(byte_pass_flag == 0xf)  //byte0/1/2/3 wreye pass
		//#else
		//if(byte_pass_flag == 0x3)  //byte0/1 wreye pass
		//#endif
        if (train_phy == DDRPHY_0) {
		     if(byte_pass_flag == 0xf)  //byte0/1/2/3 wreye pass
		     {
		     	// DDR_DBG_PRINT("\r\n------------cur all byte coarse wrlvl pass!---------\r\n");
		     	// print_hex(wrlvl_dqs_out_index);
		     	// print_hex(dqs_out_sel);
		     	// for(u32 byte_sel = 0; byte_sel<8; byte_sel++)
		     	// {
		     	// 	print_hex(byte_pass_pos[byte_sel]);
		     	// }
		     	// print_hex(byte_pass_pos_max);
		     	// print_hex(byte_pass_pos_min);
		     	break;
		     }
		     else
		     {
		     	// DDR_DBG_PRINT("\r\n-----------cur all byte coarse wrlvl failed!--------\r\n");
		     	// print_hex(wrlvl_dqs_out_index);
		     	// print_hex(dqs_out_sel);
		     	// for(u32 byte_sel; byte_sel<4; byte_sel++)
		     	// {
		     	// 	print_hex(byte_pass_pos[byte_sel]);
		     	// }
		     	// print_hex(byte_pass_flag);
		     }
        }
        else{
		     if(byte_pass_flag == 0xf0)  //byte0/1/2/3 wreye pass
		     {
		     	// DDR_DBG_PRINT("\r\n------------cur all byte coarse wrlvl pass!---------\r\n");
		     	// print_hex(wrlvl_dqs_out_index);
		     	// print_hex(dqs_out_sel);
		     	// for(u32 byte_sel = 0; byte_sel<8; byte_sel++)
		     	// {
		     	// 	print_hex(byte_pass_pos[byte_sel]);
		     	// }
		     	// print_hex(byte_pass_pos_max);
		     	// print_hex(byte_pass_pos_min);
		     	break;
		     }
		     else
		     {
		     	// DDR_DBG_PRINT("\r\n-----------cur all byte coarse wrlvl failed!--------\r\n");
		     	// print_hex(wrlvl_dqs_out_index);
		     	// print_hex(dqs_out_sel);
		     	// for(u32 byte_sel; byte_sel<4; byte_sel++)
		     	// {
		     	// 	print_hex(byte_pass_pos[byte_sel]);
		     	// }
		     	// print_hex(byte_pass_flag);
		     }
        }


	}  //coarse wrlvl dqs out search end


	if(byte_pass_pos_max - byte_pass_pos_min > 1)
	{
		DDR_DBG_PRINT("\r\n------------coarse wrlvl training failed!---------\r\n");
		DDR_DBG_PRINT("\r\n---------byte to byte skew more than 1 cycle------\r\n");
	}
	else if((byte_pass_pos_max - byte_pass_pos_min == 1) && (byte_pass_pos_max % 2 == 0))
	{
		if(post_set_meth == 1)  //coarse wrlvl post set method1
		{
			//update pass wreye dly parameter
			reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 24, 4, ddrmc_dqs_oe_dly + byte_pass_pos_min / 2);
			reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 16, 4, ddrmc_data_oe_dly + byte_pass_pos_min / 2);
			reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 8, 4, ddrmc_wdm_latency + byte_pass_pos_min / 2);
			reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 0, 4, ddrmc_wdata_latency + byte_pass_pos_min / 2);
			for(u32 byte_sel = 0; byte_sel < 4; byte_sel++)
			//#ifndef DRAM_DQ_X16_MODE
			//for(u32 byte_sel = 0; byte_sel < 4; byte_sel++)
			//#else
			////for(u32 byte_sel = 0; byte_sel < 4; byte_sel+=2)
			//for(u32 byte_sel = 0; byte_sel < 2; byte_sel+=1)
			//#endif
			{
			  	u32 reg_tmp;
                if (train_phy == DDRPHY_0) {
				    if(byte_pass_pos[byte_sel] > byte_pass_pos_min){
				    		reg_tmp = reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17);
				    		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17, reg_tmp << 0x2); //rf_dqs_out_sel_ds0_f0
				    		reg_tmp = reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2);
				    		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, reg_tmp + 0x3);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0

                    }
				    	//if(byte_sel < 2)
				    	//{
				    	//	reg_tmp = reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17);
				    	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17, reg_tmp << 0x2); //rf_dqs_out_sel_ds0_f0
				    	//	reg_tmp = reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2);
				    	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, reg_tmp + 0x3);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
				    	//}
				    	//else
				    	//{
				    	//	reg_tmp = reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel-2) * 0x1000), 0, 17);
				    	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel - 2) * 0x1000), 0, 17,  reg_tmp << 0x2);  //rf_dqs_out_sel_ds0_f0
				    	//	reg_tmp = reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel-2) * 0x1000), 24, 2);
				    	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel - 2)* 0x1000), 24, 2, reg_tmp + 0x3);     //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
				    	//}
				    }
                else {
				    if(byte_pass_pos[byte_sel+4] > byte_pass_pos_min) {
					    reg_tmp = reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel) * 0x1000), 0, 17);
					    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel) * 0x1000), 0, 17,  reg_tmp << 0x2);  //rf_dqs_out_sel_ds0_f0
					    reg_tmp = reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel) * 0x1000), 24, 2);
					    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel)* 0x1000), 24, 2, reg_tmp + 0x3);     //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
                   } 
                }
			}
			ddrphy_train_soft_update_ac();
			ddrphy_train_soft_update_dq();
		}
		else  //coarse wrlvl post set method2
		{
			//update wreye pass dly parameter
			reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 24, 4, ddrmc_dqs_oe_dly + byte_pass_pos_max / 2);
			reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 16, 4, ddrmc_data_oe_dly + byte_pass_pos_max / 2);
			reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 8, 4, ddrmc_wdm_latency + byte_pass_pos_max / 2);
			reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 0, 4, ddrmc_wdata_latency + byte_pass_pos_max / 2);
			for(u32 byte_sel = 0; byte_sel < 4; byte_sel++)
			//#ifndef DRAM_DQ_X16_MODE
			//for(u32 byte_sel = 0; byte_sel < 4; byte_sel++)
			//#else
			////for(u32 byte_sel = 0; byte_sel < 4; byte_sel+=2)
			//for(u32 byte_sel = 0; byte_sel < 2; byte_sel+=1)
			//#endif
			{
                if (train_phy == DDRPHY_0) {
				    if(byte_pass_pos[byte_sel] < byte_pass_pos_max)
				    {
				    		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17, dqs_out_sel >> 0x1); //rf_dqs_out_sel_ds0_f0
				    		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, dq_out_sel - 0x2);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
				    	//}
                            
				    	//if(byte_sel < 2)
				    	//{
				    	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 0, 17, dqs_out_sel >> 0x1); //rf_dqs_out_sel_ds0_f0
				    	//	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, dq_out_sel - 0x2);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
				    	//}
				    	//else
				    	//{
				    	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel - 2) * 0x1000), 0, 17, dqs_out_sel >> 0x1);  //rf_dqs_out_sel_ds0_f0
				    	//	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel - 2)* 0x1000), 24, 2, dq_out_sel - 0x2);     //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
				    	//}
				    }
                }
                else {
				    if(byte_pass_pos[byte_sel+4] < byte_pass_pos_max){
				    		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100 + (byte_sel) * 0x1000), 0, 17, dqs_out_sel >> 0x1);  //rf_dqs_out_sel_ds0_f0
				    		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel)* 0x1000), 24, 2, dq_out_sel - 0x2);     //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
                    }
                }
			}
			ddrphy_train_soft_update_ac();
			ddrphy_train_soft_update_dq();
		}
	}
	else if((byte_pass_pos_max - byte_pass_pos_min == 1) && (byte_pass_pos_max % 2 == 1))
	{
		for(u32 byte_sel = 0; byte_sel < 4; byte_sel++)
		//#ifndef DRAM_DQ_X16_MODE
		//for(u32 byte_sel = 0; byte_sel < 4; byte_sel++)
		//#else
		////for(u32 byte_sel = 0; byte_sel < 4; byte_sel+=2)
		//for(u32 byte_sel = 0; byte_sel < 2; byte_sel+=1)
		//#endif
		{

                if (train_phy == DDRPHY_0) {
			        if(byte_pass_pos[byte_sel] == byte_pass_pos_min) {
			       
					    dq_out_sel = reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
					    if(dq_out_sel == 0x2)
					    {
					    	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, 0x0);
					    }
					    else if(dq_out_sel == 0x1)
					    {
					    	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, 0x0);
					    }
			       }

                }
                else {
			        if(byte_pass_pos[byte_sel+4] == byte_pass_pos_min) {
					    dq_out_sel = reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel) * 0x1000), 24, 2);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
					    if(dq_out_sel == 0x2)
					    {
					    	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel) * 0x1000), 24, 2, 0x0);
					    }
					    else if(dq_out_sel == 0x1)
					    {
					    	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel) * 0x1000), 24, 2, 0x0);
					    }
                    }
                }
				//if(byte_sel < 2)
				//{
				//	dq_out_sel = reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
				//	if(dq_out_sel == 0x2)
				//	{
				//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, 0x0);
				//	}
				//	else if(dq_out_sel == 0x1)
				//	{
				//		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + byte_sel * 0x1000), 24, 2, 0x0);
				//	}
				//}
				//else
				//{
				//	dq_out_sel = reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel - 2) * 0x1000), 24, 2);   //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
				//	if(dq_out_sel == 0x2)
				//	{
				//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel - 2) * 0x1000), 24, 2, 0x0);
				//	}
				//	else if(dq_out_sel == 0x1)
				//	{
				//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100 + (byte_sel - 2) * 0x1000), 24, 2, 0x0);
				//	}
				//}
		}
		ddrphy_train_soft_update_ac();
		ddrphy_train_soft_update_dq();
	}


//coarse wrlvl rewreye test
#if 0
	//wrlvl rewreye test start
	for(byte_sel = 0; byte_sel < 8; byte_sel++)
	{
		//rewreye polling start
		ddrphy_train_en(TRAIN_WREYE_INDEX);
		ddrphy_train_clear();
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x1 << byte_sel);
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
		wait_us(1);
		ddrphy_train_polling_done(TRAIN_WREYE_INDEX);

		//check result
		if(reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR), 28, 2) == 0x1)  //wreye pass
		{

			DDR_DBG_PRINT("\r\n----------wrlvl rewreye passed!---------\r\n");
		}
		else if(reg_bit_get(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR),28,2) == 0x3) //wreye failed
		{
			DDR_DBG_PRINT("\r\n----------wrlvl rewreye failed!---------\r\n");
		}

		DDR_DBG_PRINT("\r\nwrlvl rewreye window value:\r\n");
		print_hex(byte_sel);
		print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR), 24, 4));
		print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR), 16, 4));
		print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR),  8, 4));
		print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR),  0, 4));
		if(byte_sel < 2)
		{
			print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR)));  //train_dqeye_value_3_0
			print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR)));  //train_dqeye_pass_value_3_0
			print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR + byte_sel * 0x1000), 0, 17));  //rf_dqs_out_sel_ds0_f0
			print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR + byte_sel * 0x1000), 24, 2));  //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
			print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR + byte_sel * 0x1000)));  //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0 rf_clkwr_cnt_ds0_f0  rf_clk_wr_dl_cpst_en_ds0
		}
		else
		{
			print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES4_ADDR)));  //train_dqeye_value_7_4
			print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR)));  //train_dqeye_pass_value_7_4
			print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR + (byte_sel-2) * 0x1000), 0, 17));  //rf_dqs_out_sel_ds0_f0
			print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR + (byte_sel-2) * 0x1000), 24, 2));  //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0
			print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR + (byte_sel-2) * 0x1000)));  //rf_clkwr_2cycle_dly_ds0_f0  rf_clkwr_1cycle_dly_ds0_f0 rf_clkwr_cnt_ds0_f0  rf_clk_wr_dl_cpst_en_ds0
		}
		ddrphy_train_clear();
	}
	//wrlvl rewreye test end
#endif


#if TEST_DEBUG_LOG_LEVEL > 2
	DDR_DBG_PRINT("\r\ncoarse wrlvl final dly parameter:\r\n");
	print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 24, 4));  //rf_dqs_oe_dly_f0
	print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 16, 4));  //rf_data_oe_dly_f0
	print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 8, 4));   //rf_wdm_oe_latency_f0
	print_hex(reg_bit_get(DDRMC_0_(PARM_DDRMC_TMG11_F0_ADDR+fn*0x80), 0, 4));   //rf_wdata_latency_f0
    if (train_phy == DDRPHY_0) {
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100), 24, 2));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100), 24, 2));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100), 24, 2));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100), 24, 2));
    }
    else {
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100), 24, 2));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100), 24, 2));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100), 24, 2));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG6_F0_ADDR+fn*0x100), 0, 17));
	    print_hex(reg_bit_get(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100), 24, 2));
    }
#endif


#if TEST_DEBUG_LOG_LEVEL > 1
	DDR_DBG_PRINT("\r\n-------------------------------------------------------\r\n");
	DDR_DBG_PRINT("\r\n-------------coarse wrlvl training end-----------------\r\n");
	DDR_DBG_PRINT("\r\n-------------------------------------------------------\r\n");
#endif
}


int ddrphy_train_gate_training(u32 train_phy,u32 fn) {
	u32 i = 0;
	u32 j = 0;
	u32 k = 0;
	u32 fail = 0;
	u32 gate_timing = 0x1;
	u32 GATE_BIST_CNT = 1;
	u32 left_pass = 0;
	u32 right_pass = 0;
	u32 gate_value = 0;

	//if (!ddrphy_train_sel(TRAIN_GATE_INDEX))
	//	return 0;

#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n-------------start gate training----------------\r\n");
#endif
	//train enable
	ddrphy_train_en(TRAIN_GATE_INDEX);
	//train start

	//DDR_DBG_PRINT("gate training phy: ");
	//print_dec(train_phy);
	//DDR_DBG_PRINT("\r\n");


	//if(dram_info.dram_type != DRAM_LP4)	
    //{
    //	//set train_byte_en
    //	if (train_phy == DDRPHY_0) {
    //		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x0f);//rf_train_byte_en
    //		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x0);//rf_train_ecc_byte_en
    //	}
    //	else if (train_phy == DDRPHY_1) {
    //		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xf0);//rf_train_byte_en
    //		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x0);//rf_train_ecc_byte_en
    //	}
    //	else if (train_phy == DDRPHY_ALL) {
    //		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0xff);//rf_train_byte_en
    //		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x0);//rf_train_ecc_byte_en
    //	}
    //	else if (train_phy == DDRPHY_ECC) {
    //		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x0);//rf_train_byte_en
    //		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 1, 0x1);//rf_train_ecc_byte_en
    //		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG19_ADDR), 2, 1, 1);  //rf_ecc_rd_en
    //		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG19_ADDR), 1, 1, 1);  //rf_ecc_wr_en
    //	}
    //}

	for (i = 1; i < 15; i++) {
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR+fn*0x100), 0, 16, gate_timing);

		ddrphy_fifo_reset();
		for (k = 0; k < GATE_BIST_CNT; k++) {
			//DDR_DBG_PRINT("\r\n");
			//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR)));
			//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG6_F0_ADDR)));
			//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR)));
			//			fail = dram_bist_test(2);
			if(dram_info.dram_type == DRAM_DDR4)
			{
				fail = ddrphy_train_ddr4_rdeye(0, train_phy,fn);
			}
			//else if(dram_info.dram_type == DRAM_DDR3)
			//{
			//	fail = ddrphy_train_ddr3_rdeye(0, train_phy);					
			//}
            else if (dram_info.dram_type == DRAM_LP4)
            {
	           fail = ddrphy_train_lpddr4_rdeye(0,train_phy,fn);

            }
		}
		gate_timing = gate_timing << 1;
		if (fail == 0x0) {
			right_pass = i;
			left_pass = i;
			for (j = i; j < 15; j++) {
					reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR+fn*0x100), 0, 16, gate_timing);
				ddrphy_fifo_reset();
				for (k = 0; k < GATE_BIST_CNT; k++) {
					//DDR_DBG_PRINT("\r\n");
					//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR)));
					//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG6_F0_ADDR)));
					//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR)));
					//					fail = dram_bist_test(2);
					if(dram_info.dram_type == DRAM_DDR4)
					{
						fail = ddrphy_train_ddr4_rdeye(0, train_phy,fn);
					}
					//else if(dram_info.dram_type == DRAM_DDR3)
					//{
					//	fail = ddrphy_train_ddr3_rdeye(0, train_phy);					
					//}
                    else if (dram_info.dram_type == DRAM_LP4)
                    {
	                   fail = ddrphy_train_lpddr4_rdeye(0,train_phy,fn);

                    }
					ddrphy_fifo_reset();
				}
				gate_timing = gate_timing << 1;
				if (fail == 0x0) {
					left_pass = j + 1;
				}
				else if (j - left_pass > 1) {
					break;
				}
			}
			break;
		}
	}

	if (left_pass - right_pass > 4) {
		gate_value = left_pass - 2;
	}
	else {
		gate_value = (left_pass + right_pass) / 2;
	}
	//DDR_DBG_PRINT("\r\ngate left pass:");
	//print_hex(left_pass);
	//DDR_DBG_PRINT("\r\ngate right pass:");
	//print_hex(right_pass);
	//DDR_DBG_PRINT("\r\ngate value:");
	//print_hex(gate_value);



		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR+fn*0x100), 0, 16, 1 << gate_value);

	ddrphy_fifo_reset();


	//DDR_DBG_PRINT("\r\ngate training result:phy0/phy1/phyecc\r\n");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG6_F0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR)));

	//close byte_en
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 23, 0, 0x0);//rf_train_byte_en

	return 0;
#if TEST_DEBUG_LOG_LEVEL>2
	//DDR_DBG_PRINT("\r\n--------------gate training done----------------\r\n");
#endif

}

void ddrphy_train_lpddr4_rddsk(u32 train_phy, u32 fn)
{

	u32 pass_win_pos = 0, pass_win_neg = 0;
	u32 pass_val_pos = 0, pass_val_neg = 0;
	u32 rdeye_pos_value0 = 0, rdeye_neg_value0 = 0;
	u32 rdeye_pos_value1 = 0, rdeye_neg_value1 = 0;
	u32 rdeye_pos_value2 = 0, rdeye_neg_value2 = 0;
	u32 rdeye_pos_value3 = 0, rdeye_neg_value3 = 0;
	u32 rdeye_pos_value4 = 0, rdeye_neg_value4 = 0;
	u32 rdeye_pos_value5 = 0, rdeye_neg_value5 = 0;
	u32 rdeye_pos_value6 = 0, rdeye_neg_value6 = 0;
	u32 rdeye_pos_value7 = 0, rdeye_neg_value7 = 0;
//	if (!ddrphy_train_sel(TRAIN_RDDSK_INDEX))
//		return;

#if TEST_DEBUG_LOG_LEVEL>2
	DDR_DBG_PRINT("\r\n-------------rddsk training start---------------\r\n");
#endif

	//	ddrmc_mrw(DRAM_MR_15, DRAM_CS_ALL, 0x0);
	//	ddrmc_mrw(DRAM_MR_20, DRAM_CS_ALL, 0x0);

	for (u32 i = 0; i < 8; i++) {
//		if (i == 0) {
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG1_ADDR), 0x00000000);
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG1_ADDR), 0x00000000);
//		}
//		else if (i == 1) {
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG1_ADDR), 0x11111111);
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG1_ADDR), 0x11111111);
//		}
//		else if (i == 2) {
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG1_ADDR), 0x22222222);
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG1_ADDR), 0x22222222);
//		}
//		else if (i == 3) {
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG1_ADDR), 0x33333333);
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG1_ADDR), 0x33333333);
//		}
//		else if (i == 4) {
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG1_ADDR), 0x44444444);
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG1_ADDR), 0x44444444);
//		}
//		else if (i == 5) {
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG1_ADDR), 0x55555555);
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG1_ADDR), 0x55555555);
//		}
//		else if (i == 6) {
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG1_ADDR), 0x66666666);
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG1_ADDR), 0x66666666);
//		}
//		else if (i == 7) {
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG1_ADDR), 0x77777777);
//			__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG1_ADDR), 0x77777777);
//		}

		ddrphy_train_en(TRAIN_RDDSK_INDEX);
		wait_100ns(1);
		reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
		wait_100ns(1);
		ddrphy_train_polling_done(TRAIN_RDDSK_INDEX);
		wait_100ns(1);
		pass_win_pos = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
		rdeye_pos_value0 = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+fn*0x100)));
		rdeye_pos_value1 = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+fn*0x100)));

		rdeye_neg_value0 = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+fn*0x100)));
		rdeye_neg_value1 = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+fn*0x100)));

		ddrphy_train_clear();


	if (dram_info.cs_num == 1) {
    }
    else{
        if (train_phy == DDRPHY_0) {
    	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
    	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));
    
    	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
    	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));
    
    	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
    	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));
    
    	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
    	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));
   }
   else if (train_phy == DDRPHY_1) {
    	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG12_ADDR)));
    	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG13_ADDR)));
    
    	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG12_ADDR)));
    	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG13_ADDR)));
    
    	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG12_ADDR)));
    	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG13_ADDR)));
    
    	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG12_ADDR)));
    	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG13_ADDR)));
    }
    //else {
    //	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG12_ADDR)));
    //	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG13_ADDR)));
    //
    //	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG12_ADDR)));
    //	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG13_ADDR)));
    //
    //	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG12_ADDR)));
    //	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_CFG13_ADDR)));
    //
    //	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG12_ADDR)));
    //	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_CFG13_ADDR)));

    //	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG12_ADDR)));
    //	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_CFG13_ADDR)));
    //
    //	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG12_ADDR)));
    //	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_CFG13_ADDR)));
    //
    //	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG12_ADDR)));
    //	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_CFG13_ADDR)));
    //
    //	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG15_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG12_ADDR)));
    //	__raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG16_ADDR), __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_CFG13_ADDR)));
    //    
    //    
    //    }
}


#if TEST_DEBUG_LOG_LEVEL>1
		DDR_DBG_PRINT("\r\nrdeye training phy:");
		print_dec(train_info.train_phy_num);
		DDR_DBG_PRINT("Max Window Final: ");
		print_hex(train_info.vrefdq_rd_val);
		DDR_DBG_PRINT("\r\nPos Pass Window:");
		print_hex(pass_win_pos);
		DDR_DBG_PRINT("\r\nPos Pass Result:");
		print_hex(rdeye_pos_value1);
		print_hex(rdeye_pos_value0);
		DDR_DBG_PRINT("\r\nNeg Pass Window:");
		print_hex(pass_win_neg);
		DDR_DBG_PRINT("\r\nNeg Pass Result:");
		print_hex(rdeye_neg_value1);
		print_hex(rdeye_neg_value0);
#endif
		ddrphy_train_clear();
	}

	//	ddrmc_mrw(DRAM_MR_15, DRAM_CS_ALL, 0x55);
	//	ddrmc_mrw(DRAM_MR_20, DRAM_CS_ALL, 0x55);

		//PINMUX
	//__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_CFG3_ADDR), 0x76514032);
	//__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_CFG6_ADDR), 0x20173465);
	//__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_CFG7_ADDR), 0x41023756);
	//__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_CFG8_ADDR), 0x00080008);
	//__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_CFG9_ADDR), 0x42536701);
	//__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_CFG10_ADDR), 0x23574601);
	//__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_CFG11_ADDR), 0x00080008);
	//train start clear

#if TEST_DEBUG_LOG_LEVEL>2
	DDR_DBG_PRINT("\r\n-------------rddsk training done----------------\r\n");
#endif
}

int ddrphy_train_lpddr4_rdeye(u32 vref_scan_en, u32 train_phy, u32 fn)
{
	u32 pass_win_pos1 = 0, pass_win_neg1 = 0;
	u32 pass_win_pos2 = 0, pass_win_neg2 = 0;
	u32 rdeye_pos_value0 = 0, rdeye_neg_value0 = 0;
	u32 rdeye_pos_value1 = 0, rdeye_neg_value1 = 0;
	u32 rdeye_pos_value2 = 0, rdeye_neg_value2 = 0;
	u32 rdeye_pos_value3 = 0, rdeye_neg_value3 = 0;
	u32 rdeye_pos_value4 = 0, rdeye_neg_value4 = 0;
	u32 rdeye_pos_value5 = 0, rdeye_neg_value5 = 0;
	u32 rdeye_pos_value6 = 0, rdeye_neg_value6 = 0;
	u32 rdeye_pos_value7 = 0, rdeye_neg_value7 = 0;

	u32 rdeye_vref_first = 0, rdeye_vref_last = 0;
	u32 rdeye_vref_min = 0, rdeye_vref_max = 0;
	u32 pass_win1 = 0, pass_win2 = 0;
	u32 pass_win_sum1 = 0, pass_win_sum2 = 0;
	u32 pass_win_sum = 0, pass_win_max = 0;
	u32 vrefdq_val = 0, vrefdq_step = 2;
	u32 i = 0;
	u32 training_result;

#if TEST_DEBUG_LOG_LEVEL>2
	DDR_DBG_PRINT("\r\n-------------rdeye training start---------------\r\n");
#endif
	// train enable
	if (vref_scan_en)
	{
		if (train_info.odt_status) {
			rdeye_vref_min = LPDDR4_RDEYE_VREFDQ_MIN;
			rdeye_vref_max = LPDDR4_RDEYE_VREFDQ_MAX;
		}
		else {
			rdeye_vref_min = 0x50;
			rdeye_vref_max = 0x50;
		}
		for (vrefdq_val = rdeye_vref_min; vrefdq_val <= rdeye_vref_max; vrefdq_val += vrefdq_step)
		{
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 24, 1, 0x0);    //Power down of Internal VREF
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 25, 1, 0x0);    //Power down of Internal VREF se
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, vrefdq_val);  //vrefi_adj
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 8, 8, vrefdq_val);  //vrefi_se_adj
			//reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x03);//rf_train_byte_en
			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 24, 1, 0x0);    //Power down of Internal VREF
			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 25, 1, 0x0);    //Power down of Internal VREF se
			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, vrefdq_val);  //vrefi_adj
			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 8, 8, vrefdq_val);  //vrefi_se_adj
			wait_us(1000);

			//train start for cs0

			ddrphy_train_en(TRAIN_RDEYE_INDEX);
			//train start
			/*start training*/
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
			wait_100ns(1);
			ddrphy_train_polling_done(TRAIN_RDEYE_INDEX);
			wait_100ns(1);
			//get dq eye taining result
			pass_win1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));   //DQ eye training results : the width of byte0 ~ 3
			pass_win2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));   //DQ eye training results : the width of byte4 ~ 7

			pass_win_sum1 = ((pass_win1 >> 0) & 0xff) + ((pass_win1 >> 8) & 0xff) + ((pass_win1 >> 16) & 0xff) + ((pass_win1 >> 24) & 0xff);
			pass_win_sum2 = ((pass_win2 >> 0) & 0xff) + ((pass_win2 >> 8) & 0xff) + ((pass_win2 >> 16) & 0xff) + ((pass_win2 >> 24) & 0xff);
			pass_win_sum = pass_win_sum1 + pass_win_sum2;

			if ((pass_win_sum > pass_win_max) && (pass_win1 + pass_win2 > 0)) {
				pass_win_max = pass_win_sum;
				rdeye_vref_first = vrefdq_val;
			}
			else if (pass_win_sum == pass_win_max) {
				rdeye_vref_last = vrefdq_val;
			}
			ddrphy_train_clear();

#if TEST_DEBUG_LOG_LEVEL>0
			DDR_DBG_PRINT("	");
			DDR_DBG_PRINT("rdeye training phy:");
			DDR_DBG_PRINT("	");
			DDR_DBG_PRINT("vref setting:");
			print_hex(vrefdq_val);
			DDR_DBG_PRINT("	");
			DDR_DBG_PRINT("Pass Window:");
			print_hex(pass_win1);
			print_hex(pass_win2);
			print_hex(pass_win_sum);
			DDR_DBG_PRINT("\r\n");

#endif
		}
		if (rdeye_vref_last < rdeye_vref_first) {
			rdeye_vref_last = rdeye_vref_first;
		}
		train_info.vrefdq_rd_val = (rdeye_vref_first + rdeye_vref_last) / 2;//take average for first vref and last vref

		//update read vref
        if (train_phy == DDRPHY_0) {
		    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, train_info.vrefdq_rd_val);
		    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 8, 8, train_info.vrefdq_rd_val);
		    //reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x03);//rf_train_byte_en
       } 

       else if (train_phy == DDRPHY_1){
		    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 0, 8, train_info.vrefdq_rd_val);
		    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR+fn*0x100), 8, 8, train_info.vrefdq_rd_val);
		    //reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 24, 8, 0x03);//rf_train_byte_en

       }
		wait_us(1000);
	}


	//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0xff00ff00);//read rising edge
	//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0xff00ff00);//read rising edge
	//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00ff00ff);//read falling edge
	//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00ff00ff);//read falling edge


	ddrphy_train_en(TRAIN_RDEYE_INDEX);
	wait_100ns(10);
	//train start
	/*start training*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
	ddrphy_train_polling_done(TRAIN_RDEYE_INDEX);
	wait_100ns(1);

	//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_TMG9_F0_ADDR), (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_TMG9_F0_ADDR))   + 0x20));
	//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_TMG10_F0_ADDR), (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_TMG10_F0_ADDR)) + 0x20));
	//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_TMG14_F0_ADDR), (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_TMG14_F0_ADDR)) + 0x20));
	//	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_TMG15_F0_ADDR), (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_TMG15_F0_ADDR)) + 0x20));

	training_result = ((__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR)) >> 17) & 0x1);

		//get dq eye training result
	pass_win_pos1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
	pass_win_pos2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));
	pass_win_neg1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));
	pass_win_neg2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));

	rdeye_pos_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_pos_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_pos_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_pos_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+fn*0x100)) & 0xfff;

	rdeye_pos_value4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_pos_value5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_pos_value6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG3_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_pos_value7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG3_F0_ADDR+fn*0x100)) & 0xfff;

	rdeye_neg_value0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_neg_value1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_neg_value2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_neg_value3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+fn*0x100)) & 0xfff;

	rdeye_neg_value4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_neg_value5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_neg_value6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
	rdeye_neg_value7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG4_F0_ADDR+fn*0x100)) & 0xfff;
	ddrphy_train_clear();




#if TEST_DEBUG_LOG_LEVEL>1
	DDR_DBG_PRINT("\r\nrdeye training phy:");
	DDR_DBG_PRINT("Max Window Final: ");
	print_hex(train_info.vrefdq_rd_val);
	DDR_DBG_PRINT("\r\nPos Pass Window:");
	print_hex(pass_win_pos1);
	print_hex(pass_win_pos2);
	DDR_DBG_PRINT("\r\nPos Pass Result:");
	print_hex(rdeye_pos_value0);
	print_hex(rdeye_pos_value1);
	print_hex(rdeye_pos_value2);
	print_hex(rdeye_pos_value3);
	print_hex(rdeye_pos_value4);
	print_hex(rdeye_pos_value5);
	print_hex(rdeye_pos_value6);
	print_hex(rdeye_pos_value7);
	DDR_DBG_PRINT("\r\nNeg Pass Window:");
	print_hex(pass_win_neg1);
	print_hex(pass_win_neg2);
	DDR_DBG_PRINT("\r\nNeg Pass Result:");
	print_hex(rdeye_neg_value0);
	print_hex(rdeye_neg_value1);
	print_hex(rdeye_neg_value2);
	print_hex(rdeye_neg_value3);
	print_hex(rdeye_neg_value4);
	print_hex(rdeye_neg_value5);
	print_hex(rdeye_neg_value6);
	print_hex(rdeye_neg_value7);
#endif


#if TEST_DEBUG_LOG_LEVEL>0
	DDR_DBG_PRINT("\r\n-------------rdeye training done----------------\r\n");
#endif

	if (training_result)	return 1;
	else return 0;
}
void ddrphy_train_lpddr4_rd_pattern()//no dq ODT config
{
	u32 odt_timing0, odt_timing1;

	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), TRAIN_RD_PATTERN_LP4_1 );
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA1_ADDR), TRAIN_RD_PATTERN_LP4_2);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA2_ADDR), TRAIN_RD_PATTERN_LP4_3);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA3_ADDR), TRAIN_RD_PATTERN_LP4_4);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA4_ADDR), TRAIN_RD_PATTERN_LP4_5);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA5_ADDR), TRAIN_RD_PATTERN_LP4_6);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA6_ADDR), TRAIN_RD_PATTERN_LP4_7);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA7_ADDR), TRAIN_RD_PATTERN_LP4_8);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA8_ADDR),  TRAIN_RD_PATTERN_LP4_9);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA9_ADDR),  TRAIN_RD_PATTERN_LP4_10);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA10_ADDR), TRAIN_RD_PATTERN_LP4_11);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA11_ADDR), TRAIN_RD_PATTERN_LP4_12);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA12_ADDR), TRAIN_RD_PATTERN_LP4_13);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA13_ADDR), TRAIN_RD_PATTERN_LP4_14);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA14_ADDR), TRAIN_RD_PATTERN_LP4_15);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA15_ADDR), TRAIN_RD_PATTERN_LP4_16);
		//byte swap
	//	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 0, 16, 0x3120);
		/*set rf_train_dsk_base_value for rd training*/
		/*Make rd training do more accurate*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x80);  //rf_phy_update_cnt
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0x80); //rf_train_eye_max_value
	/*set min pass window*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 24, 8, 0x2);  //rf_dqeye_pass_min_window
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 12, 8, 0x0);  //rf_train_dsk_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 0, 8, 0x80);  //rf_train_dsk_max_value

	__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00000000);
	__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);

}

void ddrphy_lpddr4_train_write_training(u32 train_phy,u32 fn)
{
	/**********Start WR Training *************/
	/*Set wr dq training pattern*/
	ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	ddrphy_train_lpddr4_wr_pattern();
	ddrphy_train_ddr4_wrdsk();
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	ddrphy_train_lpddr4_wreye(1,fn);
	//ddrphy_train_info_pre_set(train_phy, DRAM_CS_0);
	//	ddrphy_train_lpddr4_wreye(1);
	DDR_DBG_PRINT("\r\nddr4_wr_cs0_training pass\r\n");

	//if (dram_info.cs_num == 1) {
	//	ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//	ddrphy_train_lpddr4_wr_pattern();
	//	//	ddrphy_train_ddr4_wrdsk();
	//	ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//	ddrphy_train_lpddr4_wreye(0);
	//	ddrphy_train_info_pre_set(train_phy, DRAM_CS_1);
	//	//	ddrphy_train_lpddr4_wreye(1);
	//	DDR_DBG_PRINT("\r\nddr4_wr_cs1_training pass\r\n");
	//}
	//else {
	//	DDR_DBG_PRINT("\r\nddr4_write_training pass\r\n");
	//}
}

void ddrphy_train_lpddr4_wr_pattern()
{
	u32 i = 0;

	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), TRAIN_WR_PATTERN_DDR4_1);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA1_ADDR), TRAIN_WR_PATTERN_DDR4_2);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA2_ADDR), TRAIN_WR_PATTERN_DDR4_3);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA3_ADDR), TRAIN_WR_PATTERN_DDR4_4);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA4_ADDR), TRAIN_WR_PATTERN_DDR4_5);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA5_ADDR), TRAIN_WR_PATTERN_DDR4_6);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA6_ADDR), TRAIN_WR_PATTERN_DDR4_7);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA7_ADDR), TRAIN_WR_PATTERN_DDR4_8);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA8_ADDR),  TRAIN_WR_PATTERN_DDR4_9 );
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA9_ADDR),  TRAIN_WR_PATTERN_DDR4_10);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA10_ADDR), TRAIN_WR_PATTERN_DDR4_11);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA11_ADDR), TRAIN_WR_PATTERN_DDR4_12);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA12_ADDR), TRAIN_WR_PATTERN_DDR4_13);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA13_ADDR), TRAIN_WR_PATTERN_DDR4_14);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA14_ADDR), TRAIN_WR_PATTERN_DDR4_15);
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA15_ADDR),  TRAIN_WR_PATTERN_DDR4_16);

		//wrdata mask

		//    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL7_ADDR), 0xffffaaaa);
				__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_0_ADDR), 0x00000000);
				__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_1_ADDR), 0x00000000);
				__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_0_ADDR), 0x00000000);
			    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP1_1_ADDR), 0x00000000);
		    	__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_0_ADDR), 0xf000000f);
		    	__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP2_1_ADDR), 0xf000000f);
			    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_0_ADDR), 0xfffff3ff);
			    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_MASK_STP3_1_ADDR), 0xfffff3ff);

				__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_0_ADDR), 0xffffffff);                  
				__raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_1_ADDR), 0xffffffff);                  
			    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_0_ADDR), 0xffffffff);
			    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP1_1_ADDR), 0xffffffff);
			    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_0_ADDR), 0xffffffff);
			    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP2_1_ADDR), 0xffffffff);
			    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_0_ADDR), 0xfffffcff);
			    __raw_writel(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_STRB_STP3_1_ADDR), 0xfffffcff);



	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL1_ADDR), 20, 8, 0x80); //rf_phy_update_cnt
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 12, 8, 0x0);  //rf_train_eye_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 0, 11, 0xA0); //rf_train_eye_max_value
	/*set min pass window*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL3_ADDR), 24, 8, 0x10);   //rf_dqeye_pass_min_window
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 12, 8, 0x0);   //rf_train_dsk_base_value
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL4_ADDR), 0, 8, 0x80);  //rf_train_dsk_max_value
}

void ddrphy_train_lpddr4_wreye(u32 vref_scan_en, u32 fn)
{
	u32 shift_num = 0;
	u32 pass_win = 0;
	u32 pass_val = 0;
	u32 wreye_val0 = 0;
	u32 wreye_val1 = 0;
	u32 wreye_val2 = 0;
	u32 wreye_val3 = 0;
	u32 wreye_val4 = 0;
	u32 wreye_val5 = 0;
	u32 wreye_val6 = 0;
	u32 wreye_val7 = 0;
	u32 mr6_data = 0;
	u32 vref_data = 0;
    u32 vref_sel = 0;
	u32 clkwr_dl_sel_ds0 = 0;
	u32 clkwr_dl_sel_ds1 = 0;
	u32 vrefdq_step = 2;
	u32 vrefdq_val = 0;
	u32 mr6_vref_wr_val = 0;
	u32 pass_win1 = 0, pass_win2 = 0;
	u32 pass_win_sum1 = 0, pass_win_sum2 = 0;
	u32 pass_win_sum = 0, pass_win_max = 0;
	u32 wreye_vref_min = 0, wreye_vref_max = 0;
	u32 wreye_vref_first = 0, wreye_vref_last = 0;
	//if (!ddrphy_train_sel(TRAIN_WREYE_INDEX))
	//	return;

#if TEST_DEBUG_LOG_LEVEL>2
	DDR_DBG_PRINT("\r\n-------------wreye training start---------------\r\n");
#endif
	//shift left half ddr cycle wreye
//	shift_num = (__raw_readl(DDRPHY_DQ0_(dll_wr_addr[0] + train_info.ddr_freq_num * 0x80)) >> 24 & 0x7) - 0x1;
//	reg_bit_set(DDRPHY_DQ0_(dll_wr_addr[0] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	reg_bit_set(DDRPHY_DQ0_(dll_wr_addr[1] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	reg_bit_set(DDRPHY_DQ1_(dll_wr_addr[2] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	reg_bit_set(DDRPHY_DQ1_(dll_wr_addr[3] + train_info.ddr_freq_num * 0x80), 24, 3, shift_num);
//	wait_100ns(1);
//	ddrphy_train_soft_update_dq();//Make the setting take effect
//	ddrphy_fifo_reset();

	//train enable
	/*enable train clear*/
	ddrphy_train_clear();
	/*start training*/

	//    ddrmc_mrw(DRAM_MR_6, DRAM_CS_0, 0x48a);
	ddrphy_train_en(TRAIN_WREYE_INDEX);
	ddrphy_train_clear();
	if (vref_scan_en) {
		if (train_info.odt_status) {
			wreye_vref_min = LPDDR4_WREYE_ODT_VREFDQ_MIN;
			wreye_vref_max = LPDDR4_WREYE_ODT_VREFDQ_MAX;
		}
		else {
			wreye_vref_min = LPDDR4_WREYE_VREFDQ_MIN;
			wreye_vref_max = LPDDR4_WREYE_VREFDQ_MAX;
		}
		for (vrefdq_val = wreye_vref_min; vrefdq_val <= wreye_vref_max; vrefdq_val += vrefdq_step) {

			ddrmc_mrw(DRAM_MR_14, DRAM_CS_ALL, vrefdq_val);
			wait_us(1000);

			//train start
			/*start training*/
			reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
			wait_100ns(1);
			ddrphy_train_polling_done(TRAIN_WREYE_INDEX);
			/*get training result*/
		
			pass_win1 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));   //DQ eye training results : the width of byte0 ~ 3
			pass_win2 = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES5_ADDR));  //rf_train_dqeye_pass_window_7_4

			pass_win_sum1 = ((pass_win1 >> 0) & 0xff) + ((pass_win1 >> 8) & 0xff) + ((pass_win1 >> 16) & 0xff) + ((pass_win1 >> 24) & 0xff);   //四个byte的眼图窗口相加
			pass_win_sum2 = ((pass_win2 >> 0) & 0xff) + ((pass_win2 >> 8) & 0xff) + ((pass_win2 >> 16) & 0xff) + ((pass_win2 >> 24) & 0xff);
			pass_win_sum = pass_win_sum1 + pass_win_sum2;


			//	if (__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES0_ADDR)) >> 0x1c) {
			//		if (pass_win > pass_win_max) {
			//			pass_win_max = pass_win_sum;
			//			vref_sel = vrefdq_val;
			//			clkwr_dl_sel_ds0 = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_TMG7_F0_ADDR)));
			//			clkwr_dl_sel_ds1 = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_TMG12_F0_ADDR)));
			//			DDR_DBG_PRINT("  Pass Win Value:");
			//			print_hex(pass_win_max);
			//			ddrphy_train_clear();
			//		}
			//		else {
			//			ddrphy_train_clear();
			//		}
			//	}
			pass_win = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));

			if ((pass_win_sum > pass_win_max) && (pass_win > 0)) {
				pass_win_max = pass_win_sum;
				wreye_vref_first = vrefdq_val;
			}
			else if (pass_win_sum == pass_win_max) {
				wreye_vref_last = vrefdq_val;
			}
			ddrphy_train_clear();

#if TEST_DEBUG_LOG_LEVEL>2
			DDR_DBG_PRINT(" vref setting:");
			print_hex(vrefdq_val);
			DDR_DBG_PRINT("  Pass Window:");
			print_hex(pass_win);
			print_hex(pass_win_sum);
			DDR_DBG_PRINT("\r\n");
#endif

			if (wreye_vref_last < wreye_vref_first) {
				wreye_vref_last = wreye_vref_first;
			}
			mr6_vref_wr_val = (wreye_vref_first + wreye_vref_last) / 2;//take average for first vref and last vref

			//update write vref
//			ddrmc_mrw(DRAM_MR_6, DRAM_CS_ALL, 0x0480 + mr6_vref_wr_val);
//			ddrmc_mrw(DRAM_MR_6, DRAM_CS_ALL, 0x0400 + mr6_vref_wr_val);
			ddrmc_mrw(DRAM_MR_14, DRAM_CS_ALL, mr6_vref_wr_val);
			wait_us(1000);

		}
		//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_TMG7_F0_ADDR), clkwr_dl_sel_ds0);
		//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_TMG12_F0_ADDR), clkwr_dl_sel_ds1);
	}

	//	ddrphy_resync();

		/*clear train start*/


		//train start
		/*start training*/
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 0, 1, 0x1); //rf_train_start
	wait_100ns(1);
	ddrphy_train_polling_done(TRAIN_WREYE_INDEX);

	ddrphy_train_soft_update_dq();
	//get dq eye taining result
	pass_win = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES3_ADDR));   
	pass_val = __raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_RES2_ADDR));

	wreye_val0 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100));
	wreye_val1 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100));
	wreye_val2 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100));
	wreye_val3 = __raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100));

	wreye_val4 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG1_F0_ADDR+fn*0x100));
	wreye_val5 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG1_F0_ADDR+fn*0x100));
	wreye_val6 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG1_F0_ADDR+fn*0x100));
	wreye_val7 = __raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG1_F0_ADDR+fn*0x100));

	ddrphy_train_clear();

	// modify wreye value

#if TEST_DEBUG_LOG_LEVEL>2
	DDR_DBG_PRINT("\r\nWR Pass Window:");
	print_hex(pass_win);
	DDR_DBG_PRINT("WR Max Window Final: ");
	print_hex(mr6_vref_wr_val);
	DDR_DBG_PRINT("\r\nWR Pass  Value:");
	print_hex(pass_val);
	DDR_DBG_PRINT("\r\nWR Pass Result:");
	print_hex(wreye_val0);
	print_hex(wreye_val1);
	print_hex(wreye_val2);
	print_hex(wreye_val3);
	print_hex(wreye_val4);
	print_hex(wreye_val5);
	print_hex(wreye_val6);
	print_hex(wreye_val7);

	DDR_DBG_PRINT("\r\n");
#endif

	/*train clear*/

//	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG14_F0_ADDR), 12, 1, 0x1);//rf_auto_ref_en

#if TEST_DEBUG_LOG_LEVEL>2
	DDR_DBG_PRINT("\r\n-------------wreye training done----------------\r\n");
#endif
}


