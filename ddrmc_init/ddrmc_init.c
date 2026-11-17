#include "ddrmc_reg_addr.h"
#include "ddrphy_reg_addr.h"
#include "ddrmc_init.h"
#include "ddrmc_common.h"
#include "ddrmc_init_train.h"
//#include "ns_config.h"
//#include "ddr_eye.h"
//#include "ddr_wreye.h"
// #include "cluster_system_common.h" // 牛芯未提供


#ifdef  D_DRAM_DDR4
    extern DDRMC_TMG_T ddrmc_tmg_ddr4[];
    extern DDRPHY_TMG_T ddrphy_tmg_ddr4[];
#elif   D_DRAM_DDR3
    extern DDRMC_TMG_T ddrmc_tmg_ddr3[];
    extern DDRPHY_TMG_T ddrphy_tmg_ddr3[];
#elif   D_DRAM_DDR5
    extern DDRMC_TMG_T ddrmc_tmg_ddr5[];
    extern DDRPHY_TMG_T ddrphy_tmg_ddr5[];
#elif   D_DRAM_LPDDR4
    extern DDRMC_TMG_T ddrmc_tmg_lpddr4[];
    extern DDRPHY_TMG_T ddrphy_tmg_lpddr4[];
#endif
extern DDRMC_FREQ_INFO_T ddrmc_freq_info[];


DRAM_INFO_T dram_info;
DDRMC_TMG_T *ddrmc_tmg;
DDRPHY_TMG_T *ddrphy_tmg;
DDRMC_FREQ_INFO_T *freq_info=(DDRMC_FREQ_INFO_T *)(&ddrmc_freq_info[0]);


u32 user_mode_pattern[]=
{
	BIST_DATA_00_PATTERN,
	BIST_DATA_01_PATTERN,
	BIST_DATA_02_PATTERN,
	BIST_DATA_03_PATTERN,
	BIST_DATA_04_PATTERN,
	BIST_DATA_05_PATTERN,
	BIST_DATA_06_PATTERN,
	BIST_DATA_07_PATTERN,
	BIST_DATA_08_PATTERN,
	BIST_DATA_09_PATTERN,
	BIST_DATA_10_PATTERN,
	BIST_DATA_11_PATTERN,
	BIST_DATA_12_PATTERN,
	BIST_DATA_13_PATTERN,
	BIST_DATA_14_PATTERN,
	BIST_DATA_15_PATTERN,
	BIST_DATA_16_PATTERN,
	BIST_DATA_17_PATTERN,
	BIST_DATA_18_PATTERN,
	BIST_DATA_19_PATTERN,
	BIST_DATA_20_PATTERN,
	BIST_DATA_21_PATTERN,
	BIST_DATA_22_PATTERN,
	BIST_DATA_23_PATTERN,
	BIST_DATA_24_PATTERN,
	BIST_DATA_25_PATTERN,
	BIST_DATA_26_PATTERN,
	BIST_DATA_27_PATTERN,
	BIST_DATA_28_PATTERN,
	BIST_DATA_29_PATTERN,
	BIST_DATA_30_PATTERN,
	BIST_DATA_31_PATTERN
};

u32 sipi_mode_pattern[]=
{
	BIST_SIPI_BIT_PATTERN_0,
	BIST_SIPI_BIT_PATTERN_1,
	BIST_SIPI_DATA_00,
	BIST_SIPI_DATA_01,
	BIST_SIPI_DATA_02,
	BIST_SIPI_DATA_03,
	BIST_SIPI_DATA_04,
	BIST_SIPI_DATA_05,
	BIST_SIPI_DATA_06,
	BIST_SIPI_DATA_07,
	BIST_SIPI_DATA_08,
	BIST_SIPI_DATA_09,
	BIST_SIPI_DATA_10,
	BIST_SIPI_DATA_11,
	BIST_SIPI_DATA_12,
	BIST_SIPI_DATA_13,
	BIST_SIPI_DATA_14,
	BIST_SIPI_DATA_15
};

u32 lfsr_mode_pattern[] =
{
	BIST_DATA_LFSR_SEED_L0,
	BIST_DATA_LFSR_SEED_L1,
	BIST_DATA_LFSR_SEED_H0,
	BIST_DATA_LFSR_SEED_H1
};

void ddrphy_dll_cnt_print()
{
	//DDR_DBG_PRINT("\r\nac0 dll cnt:");
	//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG0_F0_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_AC_STS0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_AC_STS1_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_AC_STS2_ADDR)));
	//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_AC_STS3_ADDR)));

	//DDR_DBG_PRINT("\r\ndq0_ds0 dll cnt:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_STS0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_STS1_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_STS2_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_STS3_ADDR)));

	//DDR_DBG_PRINT("\r\ndq0_ds1 dll cnt:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG0_F0_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_STS0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_STS1_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_STS2_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_STS3_ADDR)));

	//DDR_DBG_PRINT("\r\ndq0_ds2 dll cnt:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG0_F0_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_STS0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_STS1_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_STS2_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_STS3_ADDR)));

	//DDR_DBG_PRINT("\r\ndq0_ds3 dll cnt:");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG0_F0_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_STS0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_STS1_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_STS2_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_STS3_ADDR)));

	//DDR_DBG_PRINT("\r\ndq1_ds0 dll cnt:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG0_F0_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_STS0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_STS1_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_STS2_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS0_STS3_ADDR)));

	//DDR_DBG_PRINT("\r\ndq1_ds1 dll cnt:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG0_F0_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_STS0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_STS1_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_STS2_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS1_STS3_ADDR)));

	//DDR_DBG_PRINT("\r\ndq1_ds2 dll cnt:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG0_F0_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_STS0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_STS1_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_STS2_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS2_STS3_ADDR)));

	//DDR_DBG_PRINT("\r\ndq1_ds3 dll cnt:");
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG0_F0_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_STS0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_STS1_ADDR)));
//	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_STS2_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_DS3_STS3_ADDR)));

	//DDR_DBG_PRINT("\r\ndll 0: ");
	//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG0_F0_ADDR)));
	//DDR_DBG_PRINT("\r\nio 1: ");
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG9_F0_ADDR)));

}


void ddrmc_freq_sel_search(u32 ddr_clk,u32* fn)
{
	*fn=0xffffffff;
	switch(ddr_clk)
	{
	case DDR_CLK_800M:*fn=0;break;
	case DDR_CLK_1333M:*fn=0;break;
	case DDR_CLK_1600M:*fn=0;break;
	default:*fn = 0; break;
	}
	return;
}

const QOS_TIMEOUT_PORT_T qos_timeout_per_port[]=
{
	//rf_pri_rd_portx	rf_pri_wr_portx		rf_timeout_thr_rd_portx		rf_timeout_thr_wr_portx
	{0x0,					0x0,						0x0,				0x0},
	{0x0,					0x0,						0x0,				0x0},
	{0x0,					0x0,						0x0,				0x0},
	{0x0,					0x0,						0x0,				0x0},
	{0x0,					0x0,						0x0,				0x0},
	{0x0,					0x0,						0x0,				0x0},
	{0x0,					0x0,						0x0,				0x0},
};

void ddrmc_qos_timeout_set()
{
	u32 port_num=0;
	for(port_num=0;port_num<DDRMC_PORT_NUM;port_num++)
	{
		reg_bit_set(DDRMC_0_(port_num*0x10+PARM_DDRMC_PORT0_CFG1_ADDR), 0, 4,qos_timeout_per_port[port_num].pri_rd_ch);//rf_pri_rd_ch
		reg_bit_set(DDRMC_0_(port_num*0x10+PARM_DDRMC_PORT0_CFG1_ADDR), 8, 4,qos_timeout_per_port[port_num].pri_wr_ch);//rf_pri_wr_ch
		reg_bit_set(DDRMC_0_(port_num*0x10+PARM_DDRMC_PORT0_CFG2_ADDR), 0, 8,qos_timeout_per_port[port_num].timeout_thr_rd_ch);//rf_timeout_thr_rd_ch
		reg_bit_set(DDRMC_0_(port_num*0x10+PARM_DDRMC_PORT0_CFG2_ADDR),16, 8,qos_timeout_per_port[port_num].timeout_thr_wr_ch);//rf_timeout_thr_wr_ch
	}

}

void ddrmc_urgent_set()
{
	//enable urgent machine
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR),0,1,0x1);//rf_urgent_qos_en
	//high urgent
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG11_ADDR),0,16,0x8080);//rf_hi_urgent_map
	//low urgent
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG12_ADDR),0,16,0x4040);//rf_lo_urgent_map
}

void ddrmc_perf_set()
{
	//axi sync mode 0:async mode; 1:sync mode; 
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT0_CFG0_ADDR), 17, 1,0x0);//rf_sync_sel_port0
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT1_CFG0_ADDR), 17, 1,0x0);//rf_sync_sel_port1
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT2_CFG0_ADDR), 17, 1,0x0);//rf_sync_sel_port2
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT3_CFG0_ADDR), 17, 1,0x0);//rf_sync_sel_port3
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT4_CFG0_ADDR), 17, 1,0x0);//rf_sync_sel_port4
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT5_CFG0_ADDR), 17, 1,0x0);//rf_sync_sel_port5
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT6_CFG0_ADDR), 17, 1,0x0);//rf_sync_sel_port6
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT7_CFG0_ADDR), 17, 1,0x0);//rf_sync_sel_port7

	//performace
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT0_CFG0_ADDR),  1, 1,0x1);//rf_port_wr_out_of_order_port0
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT1_CFG0_ADDR),  1, 1,0x1);//rf_port_wr_out_of_order_port1
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT2_CFG0_ADDR),  1, 1,0x1);//rf_port_wr_out_of_order_port2
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT3_CFG0_ADDR),  1, 1,0x1);//rf_port_wr_out_of_order_port3
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT4_CFG0_ADDR),  1, 1,0x1);//rf_port_wr_out_of_order_port4
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT5_CFG0_ADDR),  1, 1,0x1);//rf_port_wr_out_of_order_port5
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT6_CFG0_ADDR),  1, 1,0x1);//rf_port_wr_out_of_order_port6
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT7_CFG0_ADDR),  1, 1,0x1);//rf_port_wr_out_of_order_port7

	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT0_CFG0_ADDR),  9, 1,0x1);//rf_port_rd_out_of_order_port0
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT1_CFG0_ADDR),  9, 1,0x1);//rf_port_rd_out_of_order_port1
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT2_CFG0_ADDR),  9, 1,0x1);//rf_port_rd_out_of_order_port2
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT3_CFG0_ADDR),  9, 1,0x1);//rf_port_rd_out_of_order_port3
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT4_CFG0_ADDR),  9, 1,0x1);//rf_port_rd_out_of_order_port4
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT5_CFG0_ADDR),  9, 1,0x1);//rf_port_rd_out_of_order_port5
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT6_CFG0_ADDR),  9, 1,0x1);//rf_port_rd_out_of_order_port6
	reg_bit_set(DDRMC_0_(PARM_DDRMC_PORT7_CFG0_ADDR),  9, 1,0x1);//rf_port_rd_out_of_order_port7

	//cmd queue cfg
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG15_ADDR),  0, 8,0x40);//rf_max_rw_cnt
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG15_ADDR), 12, 8,0x20);//rf_max_act_cnt
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG15_ADDR), 24, 4,0x0 );//rf_max_samecs_cnt
}

void ddrmc_qos_set()
{
	ddrmc_qos_timeout_set();
	ddrmc_urgent_set();
	ddrmc_perf_set();
}

void ddrmc_interleave_init()
{

}

void ddrmc_axi_perf_mon(u32 mon_interval)
{
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 8, 2, 0x0);//rf_perfmon_clr

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 9, 1, 0x1);//rf_perfmon_clr
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 9, 1, 0x0);//rf_perfmon_clr

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 8, 1, 0x1);//rf_perfmon_en
	wait_us(mon_interval);
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 8, 1, 0x0);//rf_perfmon_en

	//DDR_DBG_PRINT("\r\n ddrmc axi perf mon result \r\n");
	//DDR_DBG_PRINT("\r\n AXI PORT0 WR CMD NUM : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF0_ADDR)));
	//DDR_DBG_PRINT("\r\n AXI PORT0 WR CMD BW  : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF1_ADDR)));
	//DDR_DBG_PRINT("\r\n AXI PORT0 WR CMD LAT : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF2_ADDR)));
	//DDR_DBG_PRINT("\r\n AXI PORT0 RD CMD NUM : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF4_ADDR)));
	//DDR_DBG_PRINT("\r\n AXI PORT0 RD CMD BW  : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF5_ADDR)));
	//DDR_DBG_PRINT("\r\n AXI PORT0 RD CMD LAT : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF6_ADDR)));

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 9, 1, 0x1);//rf_perfmon_clr
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 9, 1, 0x0);//rf_perfmon_clr
}

void ddrmc_axi_perf_mon_enable(u32 mon_interval)
{
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 8, 2, 0x0);//rf_perfmon_clr

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 9, 1, 0x1);//rf_perfmon_clr
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 9, 1, 0x0);//rf_perfmon_clr

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 8, 1, 0x1);//rf_perfmon_en
}

void ddrmc_axi_perf_mon_disable(u32 mon_interval)
{
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 8, 1, 0x0);//rf_perfmon_en

	//DDR_DBG_PRINT("\r\n ddrmc axi perf mon result \r\n");
	//DDR_DBG_PRINT("\r\n AXI PORT0 WR CMD NUM : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF0_ADDR)));
	//DDR_DBG_PRINT("\r\n AXI PORT0 WR CMD BW  : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF1_ADDR)));
	//DDR_DBG_PRINT("\r\n AXI PORT0 WR CMD LAT : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF2_ADDR)));
	//DDR_DBG_PRINT("\r\n AXI PORT0 RD CMD NUM : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF4_ADDR)));
	//DDR_DBG_PRINT("\r\n AXI PORT0 RD CMD BW  : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF5_ADDR)));
	//DDR_DBG_PRINT("\r\n AXI PORT0 RD CMD LAT : ");
	//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_PERF6_ADDR)));

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 9, 1, 0x1);//rf_perfmon_clr
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG10_ADDR), 9, 1, 0x0);//rf_perfmon_clr
}

void print_phy_reg()
{
	//DDR_DBG_PRINT("\r\n------print ddrphy reg begin-----\r\n");
	u32 reg_addr_index;
	u32 reg_addr_value = 0;

	for (reg_addr_index = 0; reg_addr_value <= 0x2118; reg_addr_index++)
	{
		reg_addr_value = reg_addr_index * 4;
		while (
			(reg_addr_value >= 0x0000 && reg_addr_value <= 0x0004) ||
			(reg_addr_value >= 0x0100 && reg_addr_value <= 0x0134) || 
			(reg_addr_value >= 0x0E00 && reg_addr_value <= 0x0E3C) ||
			(reg_addr_value >= 0x1000 && reg_addr_value <= 0x1108) || 
			(reg_addr_value >= 0x2000 && reg_addr_value <= 0x2118)
			)
		{
			//print_hex(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG0_ADDR) + reg_addr_index * 4);
			//DDR_DBG_PRINT(": ");
			//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG0_ADDR) + reg_addr_index * 4));
			//DDR_DBG_PRINT("\r\n");
			break;
		}
	}
	reg_addr_value = 0;
	for (reg_addr_index = 0; reg_addr_value <= 0x5118; reg_addr_index++)
	{
		reg_addr_value = reg_addr_index * 4;
		while (
			(reg_addr_value >= 0x0000 && reg_addr_value <= 0x0004) ||
			(reg_addr_value >= 0x0100 && reg_addr_value <= 0x0134) ||
			(reg_addr_value >= 0x0E00 && reg_addr_value <= 0x0E3C) ||
			(reg_addr_value >= 0x2000 && reg_addr_value <= 0x2118) ||
			(reg_addr_value >= 0x3000 && reg_addr_value <= 0x3118) ||
			(reg_addr_value >= 0x4000 && reg_addr_value <= 0x4118) ||
			(reg_addr_value >= 0x5000 && reg_addr_value <= 0x5118)
			)
		{
			//print_hex(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG0_ADDR) + reg_addr_index * 4);
			//DDR_DBG_PRINT(": ");
			//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG0_ADDR) + reg_addr_index * 4));
			//DDR_DBG_PRINT("\r\n");
			break;
		}
	}
	reg_addr_value = 0;
	for (reg_addr_index = 0; reg_addr_value <= 0x5118; reg_addr_index++)
	{
		reg_addr_value = reg_addr_index * 4;
		while ( 
			(reg_addr_value >= 0x0000 && reg_addr_value <= 0x0004) ||
			(reg_addr_value >= 0x0100 && reg_addr_value <= 0x0134) ||
			(reg_addr_value >= 0x0E00 && reg_addr_value <= 0x0E3C) ||
			(reg_addr_value >= 0x2000 && reg_addr_value <= 0x2118) ||
			(reg_addr_value >= 0x3000 && reg_addr_value <= 0x3118) ||
			(reg_addr_value >= 0x4000 && reg_addr_value <= 0x4118) ||
			(reg_addr_value >= 0x5000 && reg_addr_value <= 0x5118)
			)
		{
			//print_hex(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG0_ADDR) + reg_addr_index * 4);
			//DDR_DBG_PRINT(": ");
			//print_hex(__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG0_ADDR) + reg_addr_index * 4));
			//DDR_DBG_PRINT("\r\n");
			break;
		}
	}
	//DDR_DBG_PRINT("\r\n------print ddrphy reg done-----\r\n ");
}

void print_mc_reg()
{
	//DDR_DBG_PRINT("\r\n------print ddrmc reg begin-----\r\n");
	u32 reg_addr_index;
	u32 reg_addr_value = 0;

	for (reg_addr_index = 0; reg_addr_value <= 0xB40; reg_addr_index++)
	{
		reg_addr_value = reg_addr_index * 4;
		while (
			(reg_addr_value >= 0x0000 && reg_addr_value <= 0x01D0) ||
			(reg_addr_value >= 0x0800 && reg_addr_value <= 0x0B40)
			)
		{
			//print_hex(DDRMC_0_(PARM_DDRMC_PORT0_CFG0_ADDR) + reg_addr_index * 4);
			//DDR_DBG_PRINT(": ");
			//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_PORT0_CFG0_ADDR) + reg_addr_index * 4));
			//DDR_DBG_PRINT("\r\n");
			break;
		}
	}
	reg_addr_value = 0;
	for (reg_addr_index = 0; reg_addr_value < 0x120; reg_addr_index++)
	{
		reg_addr_value = reg_addr_index * 4;
		//print_hex(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR) + reg_addr_index * 4);
		//DDR_DBG_PRINT(": ");
		//print_hex(__raw_readl(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR) + reg_addr_index * 4));
		//DDR_DBG_PRINT("\r\n");
	}
	reg_addr_value = 0;
	for (reg_addr_index = 0; reg_addr_value < 0x11C; reg_addr_index++)
	{
		reg_addr_value = reg_addr_index * 4;
		//print_hex(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR) + reg_addr_index * 4);
		//DDR_DBG_PRINT(": ");
		//print_hex(__raw_readl(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR) + reg_addr_index * 4));
		//DDR_DBG_PRINT("\r\n");
	}
	//DDR_DBG_PRINT("\r\n------print ddrmc reg done-----\r\n ");
}

void ddrphy_pinmux_set()
{
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG1_ADDR),CA_BIT_PATTERN_AC0_1);
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG2_ADDR),CA_BIT_PATTERN_AC0_2);
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG3_ADDR),CA_BIT_PATTERN_AC0_3);
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG4_ADDR),CA_BIT_PATTERN_AC0_4);
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG5_ADDR),CA_BIT_PATTERN_AC0_5);

		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG0_ADDR),DQ_BIT_OUT_PATTERN_DS0);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG1_ADDR),DQ_BIT_IN_PATTERN_DS0);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG2_ADDR),DQ_BIT_OUT_IN_PATTERN_DS0);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG0_ADDR),DQ_BIT_OUT_PATTERN_DS1);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG1_ADDR),DQ_BIT_IN_PATTERN_DS1);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG2_ADDR),DQ_BIT_OUT_IN_PATTERN_DS1);



	//	if(dram_info.dram_type == DRAM_DDR3)
	//	{
	//	   // __raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG4_ADDR) , 0xfedcbf98);  //bg-a15
	//		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG21_ADDR), 0x00000001);  //rf_clk_out_aclb_sel
	//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_AC_CFG21_ADDR), 0x00000001);  //rf_clk_out_aclb_sel
	//	}
}

void ddrmc_soft_reset()
{
	//ddrmc soft reset
	//reg_bit_set(SOC_SOFT_RST_CTRL, 2, 1,0x1);//ddrmc_soft_rst
}

void ddrmc_soft_release()
{
	//reg_bit_set(SOC_SOFT_RST_CTRL, 2, 1,0x0);//ddrmc_soft_rst
}

void ddrphy_soft_reset()
{
	//ddrphy soft reset
	//reg_bit_set(SOC_SOFT_RST_CTRL, 0, 1,0x1);//ddrphy_soft_rst
}

void ddrphy_soft_release()
{
	//reg_bit_set(SOC_SOFT_RST_CTRL, 0, 1,0x0);//ddrphy_soft_rst
}

void ddrphy_fifo_reset()
{
	wait_us(1);
	//reset fifo
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG0_ADDR), 17, 1,0x1);//rf_phy_sample_rst
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG0_ADDR), 17, 1,0x0);
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG0_ADDR), 17, 1,0x1);//rf_phy_sample_rst
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG0_ADDR), 17, 1,0x0);
	wait_us(1);
}

void soc_dfs_go(u32 fn)
{
	//(freq_info+fn)->ratio_d2;
	//(freq_info+fn)->ratio;
	//(freq_info+fn)->clk_mode;
	//fn;
}

void ddrmc_clk_cfg(u32 fn)
{
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG26_ADDR),16, 3, fn);//drf_dfs_reg_sel
	//wait_us(1);
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG26_ADDR),20, 1, 1);//drf_dfs_reg_sel
	//wait_us(1);
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG26_ADDR),20, 1, 0);//drf_dfs_reg_sel
	//soc_dfs_go(fn);
}

void ddrmc_timing_fn()
{
	u32 fn,i;
	//if(dram_info.dram_type == DRAM_LP3)
	//{
	//	//ddrmc_tmg=(DDRMC_TMG_T *)(&ddrmc_tmg_lp3[dram_info.ddr_freq]);
	//}else if(dram_info.dram_type == DRAM_DDR4){
	//	ddrmc_tmg=(DDRMC_TMG_T *)(&ddrmc_tmg_ddr4[dram_info.ddr_freq]);
    //}else if(dram_info.dram_type == DRAM_DDR3){
	//	ddrmc_tmg=(DDRMC_TMG_T *)(&ddrmc_tmg_ddr3[dram_info.ddr_freq]);
	//}
    #ifdef D_DRAM_DDR4
		//ddrmc_tmg=(DDRMC_TMG_T *)(&ddrmc_tmg_ddr4[dram_info.ddr_freq]);
		ddrmc_tmg=(DDRMC_TMG_T *)(&ddrmc_tmg_ddr4[0]);

    #elif D_DRAM_DDR3
		ddrmc_tmg=(DDRMC_TMG_T *)(&ddrmc_tmg_ddr3[0]);

    #elif D_DRAM_DDR5
		ddrmc_tmg=(DDRMC_TMG_T *)(&ddrmc_tmg_ddr5[0]);

    #elif D_DRAM_LPDDR4
		//ddrmc_tmg=(DDRMC_TMG_T *)(&ddrmc_tmg_lpddr4[dram_info.ddr_freq]);
		ddrmc_tmg=(DDRMC_TMG_T *)(&ddrmc_tmg_lpddr4[0]);

    #endif

	//for(fn=0;fn<dram_info.fn_max;fn++)
	//{
    //    for(i=0; i<20 ;i++)
	//	    __raw_writel(DDRMC_0_(0x0180+i*4+fn*0x80), ((u32*)(ddrmc_tmg+fn))[i+10]);
	//}
        for(i=0; i<20 ;i++) {
		    __raw_writel(DDRMC_0_(0x0180+i*4), ((u32*)(ddrmc_tmg))[i+10]);
        }
}

void ddrmc_lowpower_set()
{
	//ddrmc config
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR), 0, 1, 0x0);    //rf_auto_sleep_en
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG16_ADDR), 8, 1, 0x0);	//rf_ext_clk_ag_en
}

void ddrmc_basic_set()
{
	//support different dram type
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG2_ADDR),24, 2,0x0);//drf_data_width  0:x16/1:x32

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 8, 1,0x0);//rf_dual_dburst_mode 16bit disable

	//pinmux mode set
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG2_ADDR), 28, 2,0x0);//drf_pinmux_mode

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG2_ADDR),20, 3,(dram_info.dram_type == DRAM_DDR5)?0x4:0x3);//rf_dburst_length

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 0, 6,(dram_info.dram_type == DRAM_DDR4)?0x10:
                                                     (dram_info.dram_type == DRAM_DDR3)?0x08:
                                                                                        0x20);//rf_ddr5_mode
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG2_ADDR), 8, 3,((dram_info.dram_type == DRAM_LP3) || (dram_info.dram_type == DRAM_DDR3))?0x7:0x6);//rf_cs_position

	//512Mb x16 
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 20, 3,(dram_info.dram_type == DRAM_DDR4)?0x2:
                                                      (dram_info.dram_type == DRAM_DDR5)?0x2:                
                                                                                         0x0);//rf_bg_position
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 12, 3,(dram_info.dram_type == DRAM_DDR4)?0x2:
                                                      (dram_info.dram_type == DRAM_DDR5)?0x3:
                                                                                         0x0);//drf_bg_mode 
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG2_ADDR), 12, 3,(dram_info.dram_type == DRAM_DDR4)?0x2:
                                                      (dram_info.dram_type == DRAM_DDR5)?0x3:
                                                                                         0x1);//drf_bank_mode 
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG2_ADDR),  4, 3,(dram_info.dram_type == DRAM_DDR3)?0x1:
                                                      (dram_info.dram_type == DRAM_DDR4)?0x2:
                                                                                         0x2);//drf_row_mode
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG2_ADDR),  0, 3,(dram_info.dram_type == DRAM_DDR3)?0x2:0x2);//drf_column_mode
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG2_ADDR),  8, 3,(dram_info.dram_type == DRAM_DDR3)?0x4:0x4);//rf_cs_position
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG19_ADDR),10, 1,0x0);//rf_force_dual_dburst_mode
    
    
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR), 17, 1, 0x1);//rf_no_bubble

	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR), 24, 4, 0x1);//rf_mode_reg_core
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG0_ADDR),16, 4,0x1);//rf_ddrmc_core_en

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG3_ADDR), 0, 1, 0);//mpu 1:enable 0:disable
	//bit[13]:first4_dis bit[14]:last4_dis bit[15]:last8_dis
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG26_ADDR), 13, 3,(dram_info.dram_type==DRAM_LP3)?0x5:0x0);

	//interleave size config
	ddrmc_interleave_init();
	//ddrmc qos set
	ddrmc_qos_set();
}

void ddrphy_timing_fn()
{
	u32 fn=0,i;
	//if(dram_info.dram_type == DRAM_LP3)
	//{
	//	//ddrphy_tmg=(DDRPHY_TMG_T *)(&ddrphy_tmg_lp3[dram_info.ddr_freq]);
	//}else if(dram_info.dram_type == DRAM_DDR4)
	//{
	//   DDR_DBG_PRINT("\r\nddr4 ddrphy timing start\r\n");
	//   print_hex(dram_info.ddr_freq);
	//   DDR_DBG_PRINT("\r\nddr4 ddrphy_timing end\r\n");
	//	ddrphy_tmg=(DDRPHY_TMG_T *)(&ddrphy_tmg_ddr4[dram_info.ddr_freq]);

    //}else if(dram_info.dram_type == DRAM_DDR3)
	//{
	//   DDR_DBG_PRINT("\r\nddr3 ddrphy timing start\r\n");
	//   print_hex(dram_info.ddr_freq);
	//   DDR_DBG_PRINT("\r\nddr3 ddrphy_timing end\r\n");
	//	ddrphy_tmg=(DDRPHY_TMG_T *)(&ddrphy_tmg_ddr3[dram_info.ddr_freq]);
	//}
    //
    #ifdef D_DRAM_DDR4
	//	ddrphy_tmg=(DDRPHY_TMG_T *)(&ddrphy_tmg_ddr4[dram_info.ddr_freq]);
		ddrphy_tmg=(DDRPHY_TMG_T *)(&ddrphy_tmg_ddr4[0]);
    #elif D_DRAM_DDR3
		ddrphy_tmg=(DDRPHY_TMG_T *)(&ddrphy_tmg_ddr3[dram_info.ddr_freq]);

    #elif D_DRAM_DDR5
		ddrphy_tmg=(DDRPHY_TMG_T *)(&ddrphy_tmg_ddr5[0]);

    #elif D_DRAM_LPDDR4
		//ddrphy_tmg=(DDRPHY_TMG_T *)(&ddrphy_tmg_lpddr4[dram_info.ddr_freq]);
		ddrphy_tmg=(DDRPHY_TMG_T *)(&ddrphy_tmg_lpddr4[0]);

    #endif

        //for(i=0; i<14; i++) //GEN
		//    __raw_writel(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+2]);
        for(i=0; i<3; i++)  //AC
		    __raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG0_F0_ADDR+i*4),  ((u32*)(ddrphy_tmg+dram_info.ddr_freq))[i+16]);
        //for(i=0; i<7; i++)  //ECC byte
		//    __raw_writel(DDRPHY_AC0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+19]);
        for(i=0; i<14; i++) //GEN
		    __raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG0_F0_ADDR+i*4), ((u32*)(ddrphy_tmg+dram_info.ddr_freq))[i+2]);
        for(i=0; i<7; i++)  //DS0
		    __raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+i*4), ((u32*)(ddrphy_tmg+dram_info.ddr_freq))[i+19]);
        for(i=0; i<7; i++)  //DS1
		    __raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+i*4), ((u32*)(ddrphy_tmg+dram_info.ddr_freq))[i+26]);
    
	//for(fn=0;fn<dram_info.fn_max;fn++)
	//{
    //    //for(i=0; i<14; i++) //GEN
	//	//    __raw_writel(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+2]);
    //    for(i=0; i<3; i++)  //AC
	//	    __raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG0_F0_ADDR+i*4+fn*0x100),  ((u32*)(ddrphy_tmg+fn))[i+16]);
    //    //for(i=0; i<7; i++)  //ECC byte
	//	//    __raw_writel(DDRPHY_AC0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+19]);
    //    for(i=0; i<14; i++) //GEN
	//	    __raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+2]);
    //    for(i=0; i<7; i++)  //DS0
	//	    __raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+19]);
    //    for(i=0; i<7; i++)  //DS1
	//	    __raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+26]);
    //    for(i=0; i<7; i++)  //DS2
	//	    __raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+33]);
    //    for(i=0; i<7; i++)  //DS3
	//	    __raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+40]);
    //    for(i=0; i<14; i++) //GEN
	//	    __raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+2]);
    //    for(i=0; i<3; i++)  //AC
	//	    __raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG0_F0_ADDR+i*4+fn*0x100),  ((u32*)(ddrphy_tmg+fn))[i+16]);
    //    for(i=0; i<7; i++)  //DS4
	//	    __raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+19]);
    //    for(i=0; i<7; i++)  //DS5
	//	    __raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+26]);
    //    for(i=0; i<7; i++)  //DS6
	//	    __raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+33]);
    //    for(i=0; i<7; i++)  //DS7
	//	    __raw_writel(DDRPHY_DQ1_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+i*4+fn*0x100), ((u32*)(ddrphy_tmg+fn))[i+40]);
	//}
}

void ddrphy_basic_set()
{
	    //reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG0_ADDR), 0, 4, (dram_info.dram_type == DRAM_LP3 )?0x1:
        //                                                          (dram_info.dram_type == DRAM_DDR4)?0x8:
        //                                                          (dram_info.dram_type == DRAM_DDR3)?0x4:
        //                                                                                             0x2);//rf_phy_lpddr3_mode, 2: lpddr4; 4:ddr3; 8: ddr4
        //wait_100ns(1);
	    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG0_ADDR), 0, 4, (dram_info.dram_type == DRAM_LP3 )?0x1:
                                                                  (dram_info.dram_type == DRAM_DDR4)?0x8:
                                                                  (dram_info.dram_type == DRAM_DDR3)?0x4:
                                                                                                     0x2);//rf_phy_lpddr3_mode, 2: lpddr4; 4:ddr3; 8: ddr4

        wait_100ns(1);
	    //reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_GEN_CFG0_ADDR), 0, 4, (dram_info.dram_type == DRAM_LP3 )?0x1:
        //                                                          (dram_info.dram_type == DRAM_DDR4)?0x8:
        //                                                          (dram_info.dram_type == DRAM_DDR3)?0x4:
        //                                                                                             0x2);//rf_phy_lpddr3_mode, 2: lpddr4; 4:ddr3; 8: ddr4
	                                                                                 
        wait_100ns(1);                                                                                                    
    //#ifdef D_ARAGIO_SSTL_IO
	//    reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG0_ADDR), 19, 1, 1);   //OE_Polarity 
    //    wait_100ns(1);
	//    reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG0_ADDR), 19, 1, 1);   //OE_Polarity 
    //    wait_100ns(1);
	//    reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG0_ADDR), 19, 1, 1);   //OE_Polarity 
    //    wait_100ns(1);
	//    reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG19_ADDR), 0, 16, 0xffff);   //clk dly sel 
    //#endif
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG21_ADDR), 0, 16, 0xffff);   //clk dly sel 


}

void ddrphy_pll_io_set(u32 fn)
{
	//reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG11_F0_ADDR+fn*0x100), 30, 2, 0x3); //rf_dskpll_pd_force_f0 & rf_dskpll_rst_force_f0
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG11_F0_ADDR+fn*0x100), 30, 2, 0x3); //rf_dskpll_pd_force_f0 & rf_dskpll_rst_force_f0
    // PLL enable
	//reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG11_F0_ADDR+fn*0x100), 28, 2, 0x0); //enable pll en & assert rstb
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG11_F0_ADDR+fn*0x100), 28, 2, 0x0); //enable pll en & assert rstb

	wait_us(1);
	//reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG11_F0_ADDR), 28, 1, 0x1); //deassert rstb
	//reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG11_F0_ADDR), 28, 1, 0x1); //deassert rstb

}

void ddrmc_mrw(DRAM_MR_TYPE_E mr_num, DRAM_CS_TYPE_E cs_num, u32 mr_data)
{
        if (dram_info.dram_type == DRAM_LP3) {
    	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG31_ADDR),  0,  8, mr_data);//drf_mode_reg_op
    	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR),  0, 16, mr_num);//drf_mode_reg_a
        } 
        else if (dram_info.dram_type == DRAM_DDR5) {
    	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR),  0, 15, mr_data);//drf_mode_reg_op
    	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG31_ADDR),  0,  8, mr_num);//drf_mode_reg_a
	}
        else { //ddr3 & ddr4 
    	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR),  0, 15, mr_data);//drf_mode_reg_op
    	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG31_ADDR), 24,  3, mr_num);//drf_mode_reg_a
        }  
	switch(cs_num)
	{
	case DRAM_CS_ALL:
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 24, 1, 0x1);
		break;
	case DRAM_CS_0:
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 24, 1, 0x0);
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 28, 1, 0x0);//dsoft_cs
		break;
	case DRAM_CS_1:
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 24, 1, 0x0);
		reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 28, 1, 0x1);//dsoft_cs
		break;
	default:while(1);
	}
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),  1, 1, 0x1);//dsoft_mrw
	while (((__raw_readl(DDRMC_0_(PARM_DDRMC_CFG32_ADDR))) & 0x1ff) != 0);
}

int ddrmc_mrr(DRAM_MR_TYPE_E mr_num,DRAM_CS_TYPE_E cs_num,u32* mr_data)
{
	u32 mrr_valid[4] =  {0};
	//set mr num
    if (dram_info.dram_type == DRAM_LP3 || dram_info.dram_type == DRAM_LP4) {
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR),  0, 16, mr_num);//drf_mode_reg_a
    } else { //ddr3 & ddr4
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG31_ADDR), 24,  3, mr_num);//drf_mode_reg_a
    }
	//clear allcs
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 24, 1, 0x0);//dsoft_cmd_allcs
	//set cs
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 28, 1, cs_num);//dsoft_cs
	//mr read cmd
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 2, 1, 0x1);//dsoft_mrr
	while (((__raw_readl(DDRMC_0_(PARM_DDRMC_CFG32_ADDR))) & 0x1ff) !=0);

	mrr_valid[0] = (__raw_readl(DDRMC_0_(PARM_DDRMC_CFG32_ADDR))>>20)&0x1;
	mrr_valid[1] = (__raw_readl(DDRMC_0_(PARM_DDRMC_CFG33_ADDR))>>20)&0x1;
	mrr_valid[2] = (__raw_readl(DDRMC_0_(PARM_DDRMC_CFG34_ADDR))>>20)&0x1;
	//mrr_valid[3] = (__raw_readl(DDRMC_0_(PARM_DDRMC_CFG35_ADDR))>>20)&0x1;   tbd
	if (   (mrr_valid[0] == 0) && (mrr_valid[1] == 0)
		&& (mrr_valid[2] == 0) && (mrr_valid[3] == 0)) {
		return -1;
	}

	if (mrr_valid[0]) {
		*mr_data  = ((__raw_readl(DDRMC_0_(PARM_DDRMC_CFG32_ADDR))>>12) & 0xff);
	}
	if (mrr_valid[1]) {
		*mr_data |= ((__raw_readl(DDRMC_0_(PARM_DDRMC_CFG33_ADDR))>>12) & 0xff)<<8;
	}
	if (mrr_valid[2]) {
		*mr_data |= ((__raw_readl(DDRMC_0_(PARM_DDRMC_CFG34_ADDR))>>12) & 0xff)<<16;
	}
	if (mrr_valid[3]) {
		//*mr_data |= ((__raw_readl(DDRMC_0_(PARM_DDRMC_CFG35_ADDR))>>12) & 0xff)<<24;  tbd
	}
	return 0;
}

void ddrmc_mpc(u32 data,DRAM_CS_TYPE_E cs_num,u32 core_num)
{
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG31_ADDR), 0, 8, data);//drf_mode_reg_op
	switch(cs_num)
	{
	case DRAM_CS_ALL:
		if(core_num==0) {
            reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 24, 1, 0x1);
        }
        else {
            reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG33_ADDR), 24, 1, 0x1);
        }
		break;
	case DRAM_CS_0:
		if(core_num==0) {
		    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),24, 1,  0x0);
		    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),28, 1 , 0x0);//dsoft_cs
        }
        else {
		    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG33_ADDR),24, 1,  0x0);
		    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG33_ADDR),28, 1 , 0x0);//dsoft_cs
        }
		break;
	case DRAM_CS_1:
		if(core_num==0) {
		    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),24, 1,  0x0);
		    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),28, 1 , 0x1);//dsoft_cs
        }
        else {
		    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG33_ADDR),24, 1,  0x0);
		    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG33_ADDR),28, 1 , 0x1);//dsoft_cs
        }
		break;
	default:while(1);
	}
	if(core_num==0) {
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 6, 1, 0x1);//dsoft_mpc
	    while (((__raw_readl(DDRMC_0_(PARM_DDRMC_CFG32_ADDR))) & 0x1ff) !=0);
    }
    else {
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG33_ADDR), 6, 1, 0x1);//dsoft_mpc
	    while (((__raw_readl(DDRMC_0_(PARM_DDRMC_CFG33_ADDR))) & 0x1ff) !=0);
    }
}

void ddrmc_auto_refresh_open(u32 fn)
{
	//step3:enable auto refresh
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG21_ADDR), 16, 12, 0x64);//rf_t_ref 27M 3.9us
//	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG21_ADDR), 16, 12, 0x12C);//rf_t_ref 50M 7.8us
    if (dram_info.dram_type == DRAM_DDR5) {
     	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),20,1,0x1);//drf_cke_output_high
     }
    else {
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG14_F0_ADDR+fn*0x80), 28, 3, 0x0);//auto pre bank refresh
     }
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG14_F0_ADDR+fn*0x80), 25, 1, 0x1);//rf_auto_ref_allcs_en
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG14_F0_ADDR+fn*0x80), 24, 1, 0x1);//rf_auto_ref_en
}

void ddrphy_master_dll_lock(u32 fn)
{
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG0_F0_ADDR+fn*0x100),  0x00371113);//cfg_dll_ac
	//__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 0x00371113);//cfg_dll_ecc
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG0_F0_ADDR+fn*0x100), 0x00371113);//cfg_dll_ds0
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG0_F0_ADDR+fn*0x100), 0x00371113);//cfg_dll_ds1
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG0_F0_ADDR+fn*0x100), 0x00371113);//cfg_dll_ds2
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG0_F0_ADDR+fn*0x100), 0x00371113);//cfg_dll_ds3

	//wait_us(200);  tbd
	wait_us(2);

	//set rf_phy_init_start_ahb to high abd soc release dcc_rstn
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG0_ADDR), 0, 4, 0xf);
    //DDR_DBG_PRINT("\r\nddrphy_master_dll_lock\r\n");
	    while(((__raw_readl(DDRMC_0_(PARM_DDRMC_CFG0_ADDR)) >> 8) & 0x1) != 0x1);
//	while(((__raw_readl(DDRMC_0_(PARM_DDRMC_CFG0_ADDR)) >> 8) & 0xf) != 0xf);
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG0_ADDR), 0, 4, 0x0);
}

void dram_zqc_seq(u32 phy_num)
{
	if (dram_info.dram_type == DRAM_LP3) {
		ddrmc_mrw(DRAM_MR_10,DRAM_CS_0,0xff);
		wait_us(2);
		ddrmc_mrw(DRAM_MR_10,DRAM_CS_1,0xff);
	} else if ((dram_info.dram_type == DRAM_DDR4) || (dram_info.dram_type == DRAM_DDR3)) {
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG30_ADDR), 10, 1, 0x1);
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR), 24, 1, 0x1);//allcs
	    reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),  4, 1, 0x1);
		wait_us(10);
	} else {
		//rank0
		ddrmc_mpc(0x4f, DRAM_CS_0, phy_num);
		wait_us(1);
		ddrmc_mpc(0x51, DRAM_CS_0, phy_num);
		//rank1
		ddrmc_mpc(0x4f, DRAM_CS_1, phy_num);
		wait_us(1);
		ddrmc_mpc(0x51, DRAM_CS_1, phy_num);
	}
}

void lpddr4_powerup_seq(u32 fn)
{
	u32 mrr_data = 0;

	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR), 21, 1, 0x0);//ddr3_rst
	//tinit1 = 200us
	wait_us(20);  
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR), 21, 1, 0x1);//ddr3_rst
	//tinit4 =5tclk
	wait_us(1);
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR), 1, 1, 0x1);//rf_clkdmem_out_en
	//tinit3 = 2000us
	wait_us(200); 
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR), 20, 1, 0x1);//drf_cke_output_high
	//tinit5 = 2us
	wait_us(2);



	DDR_DBG_PRINT("\r\nmrw start\r\n");

	//issue mr2 data
	ddrmc_mrw(DRAM_MR_2, DRAM_CS_ALL, (ddrmc_tmg + fn)->dram_mr2_data);
	wait_us(1);
	//issue mr1 data
	ddrmc_mrw(DRAM_MR_1, DRAM_CS_ALL, (ddrmc_tmg + fn)->dram_mr1_data);
	wait_us(1);
	//issue mr3 data
	ddrmc_mrw(DRAM_MR_3, DRAM_CS_ALL, (ddrmc_tmg + fn)->dram_mr3_data);
	wait_us(1);
	//issue mr11 data
	ddrmc_mrw(DRAM_MR_11, DRAM_CS_ALL, (ddrmc_tmg + fn)->dram_mr11_data);
	wait_us(1);
	//issue mr12 data
	ddrmc_mrw(DRAM_MR_12, DRAM_CS_ALL, (ddrmc_tmg + fn)->dram_mr12_data);
	wait_us(1);
	//issue mr22 data /zq 
	ddrmc_mrw(DRAM_MR_22, DRAM_CS_ALL, (ddrmc_tmg + fn)->dram_mr22_data);
	wait_us(1);
	//DRAM ZQ calibration
	dram_zqc_seq(0);
	wait_us(1);

		//DDR_DBG_PRINT("\r\nmrr start\r\n");
		////issue mr0 data
		//ddrmc_mrr(DRAM_MR_5, DRAM_CS_0, &mrr_data);
		//DDR_DBG_PRINT("\r\nmr5 value:\r\n");
		//print_hex(mrr_data);
		//
		////issue mr8 data
		//ddrmc_mrr(DRAM_MR_8, DRAM_CS_0, &mrr_data);
		//DDR_DBG_PRINT("\r\nmr8 value:\r\n");
		//print_hex(mrr_data);
		//DDR_DBG_PRINT("\r\nmrr done\r\n");

	//reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG0_ADDR), 0, 2, 0x1); //[1:0]:LDO bias current 01+10%
	//reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG0_ADDR), 2, 1, 0x1); //[2]:"1" LDO bypass
	//reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG0_ADDR), 4, 3, 0x7); //[2]:rpull ctrl [0]:RX enable
	//lp4
	//reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR), 28, 4, 0x8);////DQS  GATE
	//reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR),  0,15, 0xff);////DQS  GATE
	//DDR_DBG_PRINT("\r\nddr4 mrr0:\r\n");
	print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_STS4_ADDR)));    //DQS gate check status
	ddrmc_mrw(DRAM_MR_14, DRAM_CS_0, 0x06);
	ddrmc_mrr(DRAM_MR_14, DRAM_CS_0, &mrr_data);
	DDR_DBG_PRINT("\r\nLPddr4 mrr14:\r\n");
	print_hex(mrr_data);
	print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_STS4_ADDR)));    //DQS gate check status



}

void ddr4_powerup_seq(u32 fn)
{
	u32 mrr_data=0;
	//step0:reset
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),21,1,0x0);//ddr3_rst
	wait_us(21);
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),21,1,0x1);//ddr3_rst
	wait_us(1); // 

	//step1:ck enable
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),1,1,0x1);//rf_clkdmem_out_en
	wait_us(1); // tinit2
	//step2:cke high
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),20,1,0x1);//drf_cke_output_high
	//tinit3
	wait_us(200);
	//tinit5
	wait_us(10);

	//DDR_DBG_PRINT("\r\nmrw start\r\n");
	//issue mr3 data
	ddrmc_mrw(DRAM_MR_3,DRAM_CS_ALL,(ddrmc_tmg+fn)->dram_mr3_data);
	//issue mr6 data
	ddrmc_mrw(DRAM_MR_6,DRAM_CS_ALL,(ddrmc_tmg+fn)->dram_mr6_data);
	//issue mr5 data
	ddrmc_mrw(DRAM_MR_5,DRAM_CS_ALL,(ddrmc_tmg+fn)->dram_mr5_data);
	//issue mr4 data
	ddrmc_mrw(DRAM_MR_4,DRAM_CS_ALL,(ddrmc_tmg+fn)->dram_mr4_data);
	//issue mr2 data
	ddrmc_mrw(DRAM_MR_2,DRAM_CS_ALL,(ddrmc_tmg+fn)->dram_mr2_data);
	//issue mr1 data
	ddrmc_mrw(DRAM_MR_1,DRAM_CS_ALL,(ddrmc_tmg+fn)->dram_mr1_data);
	//issue mr0 data
	ddrmc_mrw(DRAM_MR_0,DRAM_CS_ALL,(ddrmc_tmg+fn)->dram_mr0_data);
	//DRAM ZQ calibration
	dram_zqc_seq(0);
	wait_us(1);

	//DDR_DBG_PRINT("\r\nmrr start\r\n");
	//issue mr0 data
	//ddrmc_mrr(DRAM_MR_0,DRAM_CS_0,&mrr_data);
    ////DDR_DBG_PRINT("\r\nmr0 value:\r\n");
	////print_hex(mrr_data);

	//issue mr8 data
	//ddrmc_mrr(DRAM_MR_8,DRAM_CS_0,&mrr_data);
    ////DDR_DBG_PRINT("\r\nmr8 value:\r\n");
	////print_hex(mrr_data);
	////DDR_DBG_PRINT("\r\nmrr done\r\n");
}

void ddr3_powerup_seq(u32 fn)
{
	u32 mrr_data=0;
	//step0:reset
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),21,1,0x0);//ddr3_rst
	wait_us(21);
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),21,1,0x1);//ddr3_rst
	wait_us(1); // 

	//step1:ck enable
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),1,1,0x1);//rf_clkdmem_out_en
	wait_us(1); // tinit2
	//step2:cke high
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),20,1,0x1);//drf_cke_output_high
	//tinit3
	//wait_us(200);
	wait_us(20);
	//tinit5
	wait_us(10);

	//DDR_DBG_PRINT("\r\nmrw start\r\n");
	//issue mr2 data
	ddrmc_mrw(DRAM_MR_2,DRAM_CS_ALL,(ddrmc_tmg+fn)->dram_mr2_data);
	//issue mr3 data
	ddrmc_mrw(DRAM_MR_3,DRAM_CS_ALL,(ddrmc_tmg+fn)->dram_mr3_data);
	//issue mr1 data
	ddrmc_mrw(DRAM_MR_1,DRAM_CS_ALL,(ddrmc_tmg+fn)->dram_mr1_data);
	//issue mr0 data
	ddrmc_mrw(DRAM_MR_0,DRAM_CS_ALL,(ddrmc_tmg+fn)->dram_mr0_data);
	//DRAM ZQ calibration
	dram_zqc_seq(0);
	wait_us(1);
}
void ddr5_powerup_seq(u32 fn)
{
	u32 mrr_data=0;
	//step0:reset
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR),6,1,0x1);//ddr5 cs blk
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),21,1,0x0);//ddr3_rst
	wait_us(21);
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),21,1,0x1);//ddr3_rst
	wait_us(1); // 
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),1,1,0x1);//rf_clkdmem_out_en
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG1_ADDR),6,1,0x0);//ddr5 cs blk rls
	wait_us(10); // tinit2


	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG32_ADDR),9,1,0x1);//nop
	wait_us(1); // 

    ddrmc_mpc(0x80,DRAM_CS_ALL,0x0);
    ddrmc_mpc(0x2,DRAM_CS_ALL,0x0);
    ddrmc_mpc(0x5,DRAM_CS_ALL,0x0);
	wait_us(1); // 
    ddrmc_mpc(0x4,DRAM_CS_ALL,0x0);
    ddrmc_mpc(0x9,DRAM_CS_ALL,0x0);


	//DDR_DBG_PRINT("\r\nmrw start\r\n");
	ddrmc_mrw(0x4,DRAM_CS_ALL,0x0);
	ddrmc_mrw(0x20,DRAM_CS_ALL,0x5);
	ddrmc_mrw(0x0,DRAM_CS_ALL,0x6);
	ddrmc_mrw(0x8,DRAM_CS_ALL,0x8);
	ddrmc_mrw(0x0,DRAM_CS_ALL,0x28);
	ddrmc_mrw(0x0,DRAM_CS_ALL,0x32);
	wait_us(1);
}


void dram_powerup_seq(u32 fn)
{
	//enable auto resync
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR+fn*0x100), 16, 1,0x1);//rf_phy_sample_auto_rst_en
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR+fn*0x100), 16, 1,0x1);//rf_phy_sample_auto_rst_en
	if(dram_info.dram_type == DRAM_DDR4) {
		ddr4_powerup_seq(fn);
	}
        else if(dram_info.dram_type == DRAM_DDR3) {
		ddr3_powerup_seq(fn);
	}
        else if(dram_info.dram_type == DRAM_LP4){
		lpddr4_powerup_seq(fn);
	}
	else {
		ddr5_powerup_seq(fn);
	}
}

void loopback_ac_test(u32 external_lb, u32 ac_value)   
{
	u32 ddr_clk;
	ddr_clk = (freq_info + dram_info.ddr_freq)->ddr_clk;

	u32 ac_value_temp;
	ac_value_temp = 0x00008000 + ac_value;

	if (ddr_clk == DDR_CLK_1066M) {
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG1_F0_ADDR), 0x000080C0);
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG2_F0_ADDR), 0x01008100);
	}
	//else if (ddr_clk == DDR_CLK_933M) {
	//	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG1_F0_ADDR), ac_value_temp);//ca dl 0x00008200
	//	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG2_F0_ADDR), ac_value_temp);//cmd dl 0x00008200
	//}
	else if (ddr_clk == DDR_CLK_800M) {
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG1_F0_ADDR), 0x00008180);
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG2_F0_ADDR), 0x01008100);
	}
	//else if (ddr_clk == DDR_CLK_400M) {
	//	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG1_F0_ADDR), 0x01008100);
	//	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG2_F0_ADDR), 0x01008100);
	//}
	else {
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG1_F0_ADDR), 0x01008080);
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_TMG2_F0_ADDR), 0x01008100);
	}

	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 0, 1, 0x1);//clkdmem

	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 16, 1, 0x0);//close arf cpst
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG21_ADDR), 0, 17, 0x00002);//clk_out ac loopback
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG20_ADDR), 0, 17, 0x0000f);//clk_gate ac loopback
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR), 28, 1, 0x0);//trf_dqs_gate_mode_sel_f
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR), 0, 17, 0x0003c);//dqs_gate
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG2_F0_ADDR), 0, 16, 0x7f);//data_oe for ca

	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG1_ADDR), 0x76543210);
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG2_ADDR), 0xfedcba98);
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG3_ADDR), 0x76543210);
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG4_ADDR), 0xfedcba98);
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG5_ADDR), 0xffff0000);

	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 28, 1, 0x1);//rf_lb_s_edge
	if (external_lb == 0x1)
	{
		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 26, 2, 0x3);//rf_lb_ext_io
		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 24, 2, 0x0);//rf_lb_int_io
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 0x00000066);//rpull on
		__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR), 0x00404040);//vref
	}
	else
	{
		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 26, 2, 0x0);//rf_lb_ext_io
		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 24, 2, 0x3);//rf_lb_int_io
	}

	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 20, 3, 0x1);//rf_lb_data_size
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 16, 2, 0x1);//rf_lb_data_pat_mode
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 8, 1, 0x1);//rf_lb_enable_ac
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 0, 4, 0x0);//rf_lb_enable
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL3_ADDR), 0, 16, 0x1);//rf_lb_dm_lfsr_seed

	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL1_ADDR), 0x00000010);//rf_lb_trans_num
	//__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL2_ADDR), 0x001f0000);//rf_lb_data_mask
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL2_ADDR), 0xfffc3600);//rf_lb_data_mask
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL4_ADDR), 0x12345678);//rf_lb_usr_data_pattern_0
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL5_ADDR), 0x9abcdef1);//rf_lb_usr_data_pattern_1
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL6_ADDR), 0x00010001);//rf_lb_data_lfsr_seed_L0 
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL7_ADDR), 0x00010001);//rf_lb_data_lfsr_seed_L1 
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL8_ADDR), 0x00210011);//rf_lb_data_lfsr_seed_H0 
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL9_ADDR), 0x00810041);//rf_lb_data_lfsr_seed_H1

	ddrphy_train_soft_update_ac();

	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 1, 0x1);//rf_lb_start
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 1, 0x0);//rf_lb_start

	while (((__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_LB_RES0_ADDR)) >> 16) & 0x1) != 0x1);

	if (((__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_LB_RES0_ADDR)) >> 24) & 0x1) == 0x1)
	{
		////DDR_DBG_PRINT("\r\nac  loopback fail");
		if (external_lb == 0x1)
		{
			//DDR_DBG_PRINT("  ext loopback fail");
		}
		else
		{
			//DDR_DBG_PRINT("  int loopback fail");
		}
		//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_LB_RES1_ADDR)));
		//		while (1);
	}
	else
	{
		////DDR_DBG_PRINT("\r\nac  loopback pass");
		if (external_lb == 0x1)
		{
			//DDR_DBG_PRINT(" ext loopback pass");
		}
		else
		{
			//DDR_DBG_PRINT(" int loopback pass");
		}
	}
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x0);//disable
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x2);//clear
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x0);//disable
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 8, 1, 0x0);//rf_lb_enable_ac
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 28, 1, 0x0);//rf_lb_s_edge

	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 0, 1, 0x0);//clkdmem

}

void loopback_ac1_test(u32 external_lb, u32 ac_value)
{
	u32 ddr_clk;
	ddr_clk = (freq_info + dram_info.ddr_freq)->ddr_clk;

	u32 ac_value_temp;
	ac_value_temp = 0x00008000 + ac_value;

	if (ddr_clk == DDR_CLK_1066M) {
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG1_F0_ADDR), 0x000080C0);
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG2_F0_ADDR), 0x01008100);
	}
	else if (ddr_clk == DDR_CLK_933M) {
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG1_F0_ADDR), ac_value_temp);//ca dl 0x00008200
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG2_F0_ADDR), ac_value_temp);//cmd dl 0x00008200
	}
	else if (ddr_clk == DDR_CLK_800M) {
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG1_F0_ADDR), 0x00008180);
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG2_F0_ADDR), 0x01008100);
	}
	else if (ddr_clk == DDR_CLK_400M) {
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG1_F0_ADDR), 0x01008100);
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG2_F0_ADDR), 0x01008100);
	}
	else {
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG1_F0_ADDR), 0x01008080);
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_TMG2_F0_ADDR), 0x01008100);
	}

	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 0, 1, 0x1);//clkdmem

	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 16, 1, 0x0);//close arf cpst
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG21_ADDR), 0, 17, 0x00002);//clk_out ac loopback
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG20_ADDR), 0, 17, 0x0000f);//clk_gate ac loopback
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR), 28, 1, 0x0);//trf_dqs_gate_mode_sel_f
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR), 0, 17, 0x0003c);//dqs_gate
	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_GEN_TMG2_F0_ADDR), 0, 16, 0x7f);//data_oe for ca

	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_CFG1_ADDR), 0x76543210);
	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_CFG2_ADDR), 0xfedcba98);
	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_CFG3_ADDR), 0x76543210);
	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_CFG4_ADDR), 0xfedcba98);
	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_AC_CFG5_ADDR), 0xffff0000);

	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 28, 1, 0x1);//rf_lb_s_edge
	if (external_lb == 0x1)
	{
		reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 26, 2, 0x3);//rf_lb_ext_io
		reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 24, 2, 0x0);//rf_lb_int_io
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 0x00000066);//rpull on
		__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR), 0x00404040);//vref
	}
	else
	{
		reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 26, 2, 0x0);//rf_lb_ext_io
		reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 24, 2, 0x3);//rf_lb_int_io
	}

	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 20, 3, 0x1);//rf_lb_data_size
	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 16, 2, 0x1);//rf_lb_data_pat_mode
	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 8, 1, 0x1);//rf_lb_enable_ac
	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 0, 4, 0x0);//rf_lb_enable
	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL3_ADDR), 0, 16, 0x1);//rf_lb_dm_lfsr_seed

	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL1_ADDR), 0x00000010);//rf_lb_trans_num
	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL2_ADDR), 0xfff0ff80);//rf_lb_data_mask
	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL4_ADDR), 0x12345678);//rf_lb_usr_data_pattern_0
	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL5_ADDR), 0x9abcdef1);//rf_lb_usr_data_pattern_1
	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL6_ADDR), 0x00010001);//rf_lb_data_lfsr_seed_L0 
	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL7_ADDR), 0x00010001);//rf_lb_data_lfsr_seed_L1 
	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL8_ADDR), 0x00210011);//rf_lb_data_lfsr_seed_H0 
	__raw_writel(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL9_ADDR), 0x00810041);//rf_lb_data_lfsr_seed_H1

	ddrphy_train_soft_update_ac();

	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 1, 0x1);//rf_lb_start
	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 1, 0x0);//rf_lb_start

	while (((__raw_readl(DDRPHY_AC1_(PARM_DDRPHY_LB_RES0_ADDR)) >> 16) & 0x1) != 0x1);

	if (((__raw_readl(DDRPHY_AC1_(PARM_DDRPHY_LB_RES0_ADDR)) >> 24) & 0x1) == 0x1)
	{
		////DDR_DBG_PRINT("\r\nac  loopback fail");
		if (external_lb == 0x1)
		{
			//DDR_DBG_PRINT("  ext loopback fail");
		}
		else
		{
			//DDR_DBG_PRINT("  int loopback fail");
		}
		//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_LB_RES1_ADDR)));
		//		while (1);
	}
	else
	{
		////DDR_DBG_PRINT("\r\nac  loopback pass");
		if (external_lb == 0x1)
		{
			//DDR_DBG_PRINT(" ext loopback pass");
		}
		else
		{
			//DDR_DBG_PRINT(" int loopback pass");
		}
	}
	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x0);//disable
	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x2);//clear
	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x0);//disable
	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 8, 1, 0x0);//rf_lb_enable_ac
	reg_bit_set(DDRPHY_AC1_(PARM_DDRPHY_LB_CTRL0_ADDR), 28, 1, 0x0);//rf_lb_s_edge

	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 0, 1, 0x0);//clkdmem

}

void loopback_dq_0_test(u32 external_lb, u32 byte_en, u32 dl_value)
{
	u32 ddr_clk;
	//	u32 byte_en;
	ddr_clk = (freq_info + dram_info.ddr_freq)->ddr_clk;

	u32 wr_value_temp,rd_value_temp;
	wr_value_temp = 0x01008000 + dl_value;
	rd_value_temp = 0x00008000 + dl_value;

	if (ddr_clk == DDR_CLK_1066M) {
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR), 0x02008000);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR), 0x000081C0);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR), 0x000081C0);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR), 0x02008000);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR), 0x000081C0);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR), 0x000081C0);
	}
//	else if (ddr_clk == DDR_CLK_933M) {
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR), 0x02008000);
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR), 0x02008000);
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG1_F0_ADDR), 0x02008000);
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG1_F0_ADDR), 0x02008000);
//
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR), rd_value_temp);
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR), rd_value_temp);
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG3_F0_ADDR), rd_value_temp);
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG3_F0_ADDR), rd_value_temp);
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR), rd_value_temp);
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR), rd_value_temp);
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG4_F0_ADDR), rd_value_temp);
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG4_F0_ADDR), rd_value_temp);
//	}
	else if (ddr_clk == DDR_CLK_800M) {
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR), 0x02008000);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR), 0x00008100);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR), 0x00008100);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR), 0x02008000);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR), 0x00008100);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR), 0x00008100);
	}
//	else if (ddr_clk == DDR_CLK_400M) {
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR), 0x02008000);//WRITE
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR), 0x000081C0);//READ POS
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR), 0x000081C0);//READ NEG
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR), 0x02008000);//WRITE
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR), 0x00008100);//READ POS
//		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR), 0x00008100);//READ NEG
//	}
	else {
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG1_F0_ADDR), 0x02008000);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR), 0x00008180);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG4_F0_ADDR), 0x00008180);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG1_F0_ADDR), 0x02008000);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR), 0x00008180);
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG4_F0_ADDR), 0x00008180);
	}

	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG13_F0_ADDR), 16, 1, 0x0);//close arf cpst


	if (external_lb == 0x1)
	{
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR), 0, 17, 0x00078);//dqs_gate
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG2_F0_ADDR), 0, 17, 0x0003f);//data_oe
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG4_F0_ADDR), 0, 17, 0x000ff);//data_ie
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG8_F0_ADDR), 0x00000000);//odt off
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 0x00018077);//rpull on
		__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR), 0x00404040);//vref
	}
	else
	{
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR), 0, 17, 0x003C);//dqs_gate
	}

	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG6_F0_ADDR), 28, 1, 0x0);//dqs_gate_mode_sel_f
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG6_F0_ADDR), 0, 17, 0x0008);//dqs_out
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG6_F0_ADDR), 0, 17, 0x0008);//dqs_out
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG6_F0_ADDR), 0, 17, 0x0008);//dqs_out
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG6_F0_ADDR), 0, 17, 0x0008);//dqs_out

	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 28, 1, 0x0);//rf_lb_s_edge
	if (external_lb == 0x1)
	{
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 26, 2, 0x3);//rf_lb_ext_io
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 24, 2, 0x0);//rf_lb_int_io
	}
	else
	{
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 26, 2, 0x0);//rf_lb_ext_io
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 24, 2, 0x3);//rf_lb_int_io
	}

	//byte_en = 0x1;
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 20, 3, 0x2);//rf_lb_data_size
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 16, 2, 0x1);//rf_lb_data_pat_mode
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 8, 1, 0x0);//rf_lb_enable_ac
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 0, 4, byte_en);//rf_lb_enable

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL1_ADDR), 0x00000110);//rf_lb_trans_num
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL2_ADDR), 0x00000000);//rf_lb_data_mask

	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL3_ADDR), 0, 16, 0x12345678);//rf_lb_dm_lfsr_seed


	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL4_ADDR), 0x12345678);//rf_lb_usr_data_pattern_0
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL5_ADDR), 0x9abcdef1);//rf_lb_usr_data_pattern_1
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL6_ADDR), 0x00010001);//rf_lb_data_lfsr_seed_L0
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL7_ADDR), 0x00010001);//rf_lb_data_lfsr_seed_L1
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL8_ADDR), 0x00010001);//rf_lb_data_lfsr_seed_H0
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL9_ADDR), 0x00010001);//rf_lb_data_lfsr_seed_H1

	ddrphy_train_soft_update_dq();

	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 1, 0x1);//rf_lb_start
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 1, 0x0);//rf_lb_start

	while (((__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_LB_RES0_ADDR)) >> 0) & byte_en) != byte_en);

	if (((__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_LB_RES0_ADDR)) >> 8) & byte_en) != 0x0)
	{
		////DDR_DBG_PRINT("\r\ndq0 loopback fail");
		if (external_lb == 0x1)
		{
			//			//DDR_DBG_PRINT("\r\nExternal DQ0 loopback fail");
			//DDR_DBG_PRINT("  ext loopback fail");
			//DDR_DBG_PRINT("  dl_sel:");
			//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR)));
			//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR)));
			//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG3_F0_ADDR)));
			//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG3_F0_ADDR)));
		}
		else
		{
			//			//DDR_DBG_PRINT("\r\nInternal DQ0 loopback fail");
			//DDR_DBG_PRINT("  int loopback fail");
		}
//		//DDR_DBG_PRINT("\r\nfail data:\r\n");
//		//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_LB_RES2_ADDR)));
//		//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_LB_RES3_ADDR)));
//		//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_LB_RES4_ADDR)));
//		//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_LB_RES5_ADDR)));

		//		while (1);
	}
	else
	{
		////DDR_DBG_PRINT("\r\ndq0 loopback pass");
		if (external_lb == 0x1)
		{
			//			//DDR_DBG_PRINT("\r\nExternal DQ0 loopback pass");
			//DDR_DBG_PRINT("  ext loopback		pass");
			//DDR_DBG_PRINT("  dl_sel:");
			//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS0_TMG3_F0_ADDR)));
			//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS1_TMG3_F0_ADDR)));
			//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS2_TMG3_F0_ADDR)));
			//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_DS3_TMG3_F0_ADDR)));
		}
		else
		{
			//			//DDR_DBG_PRINT("\r\nInternal DQ0 loopback pass");
			//DDR_DBG_PRINT("  int loopback		pass");
		}
	}
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x0);//disable
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x2);//clear
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x0);//disable
}

void loopback_dq_1_test(u32 external_lb, u32 byte_en, u32 dl_value)
{
}
void loopback_acdq_all_test()
{
	ddrphy_train_soft_update_ac();
	ddrphy_train_soft_update_dq();
	//DDR_DBG_PRINT("\r\nAC  loopback start");
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 1, 0x1);//rf_lb_start
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 1, 0x0);//rf_lb_start
	//DDR_DBG_PRINT("\r\nDQ0 loopback start");
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 1, 0x1);//rf_lb_start
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 1, 0x0);//rf_lb_start

	while (((__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_LB_RES0_ADDR)) >> 16) & 0x1) != 0x1);
	if (((__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_LB_RES0_ADDR)) >> 24) & 0x1) == 0x1)
	{
		//DDR_DBG_PRINT("\r\nAC  loopback fail");
		while (1);
	}
	else
	{
		//DDR_DBG_PRINT("\r\nAC  loopback pass");
	}
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x0);//disable
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x2);//clear
	reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x0);//disable

	while (((__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_LB_RES0_ADDR)) >> 0) & 0x3) != 0x3);
	if (((__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_LB_RES0_ADDR)) >> 8) & 0x3) != 0x0)
	{
		//DDR_DBG_PRINT("\r\nDQ0 loopback fail");
		while (1);
	}
	else
	{
		//DDR_DBG_PRINT("\r\nDQ0 loopback pass");
	}
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x0);//disable
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x2);//clear
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_LB_CTRL0_ADDR), 4, 2, 0x0);//disable

}

void ddrmc_post_set(u32 fn)
{
	//step1:enable auto gate&auto sleep
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR), 0, 1,0x0);//rf_auto_sleep_en
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG16_ADDR), 4, 2,0x3);//rf_auto_gate_en_rf
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG16_ADDR), 0, 2,0x3);//rf_auto_gate_en
	//step2:enable auto mr4
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG22_ADDR),24, 2,0x0);//drf_auto_mr4_en
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG28_ADDR), 0,23,0xffff);//drf_t_mr4
	//step4:enable dfs
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG26_ADDR), 0, 4,0x0);//drf_dfs_en
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG26_ADDR), 1, 1,0x0);//drf_dfs_dll_rst_en
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG26_ADDR), 2, 1,0x1);//drf_dfs_cmd_mrw_en
	//reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG26_ADDR), 3, 1,0x1);//drf_dfs_pll_lock_en
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG26_ADDR),17, 1,0x0);//drf_dfs_clk_stop_en
	//step5:enable auto zqc
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG21_ADDR), 0,2,0x0);//drf_auto_zqc_en/drf_auto_zqc_allcs
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG21_ADDR), 8,2,0x0);//drf_auto_zq_sel
	//step6:enable auto cpst
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG16_F0_ADDR+fn*0x80),0,1,0x0);//rf_period_cpst_en
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG16_F0_ADDR+fn*0x80),4,4,0x8);//rf_cpst_wait
	reg_bit_set(DDRMC_0_(PARM_DDRMC_CFG29_ADDR), 0,12,0x19);//drf_t_cpst--3.9us for cpst
	//step7:enable auto self-refresh
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG18_F0_ADDR+fn*0x80),27,1,0x1);//rf_auto_clk_stop_en
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG18_F0_ADDR+fn*0x80),24,3,0x7);//rf_ca_shutdown_en
	reg_bit_set(DDRMC_0_(PARM_DDRMC_TMG18_F0_ADDR+fn*0x80), 0,2,0x0);//rf_auto_self_ref_en
}

void ddrmc_train_seq(u32 fn)
{
	u32 start_train_fn,end_train_fn,ddr_clk;

    if((dram_info.dram_type==DRAM_DDR4) || (dram_info.dram_type==DRAM_DDR3) || (dram_info.dram_type==DRAM_DDR5))
    {
		//ddrphy_train(BOOT_FREQ_POINT, DDR4_TRAIN_START_FN);
		ddrphy_train(BOOT_FREQ_POINT, fn);
	    //DDR_DBG_PRINT("\r\n ddr4_train_seq pass! \r\n");
    }
    else if (dram_info.dram_type == DRAM_LP4) {

		print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_TRAIN_CTRL0_ADDR)));
		//ddrphy_train(BOOT_FREQ_POINT, LP4_TRAIN_START_FN);
		ddrphy_train(BOOT_FREQ_POINT, fn);
		DDR_DBG_PRINT("\r\n lpddr4_train_seq pass! \r\n");

	}
}

void dram_init(u32 ddr_clk,u32 fn)
{
	//u32 fn;
	u32 dll_cnt;
	//ddrmc_freq_sel_search(ddr_clk, &fn);
	//step1:close ddrmc auto sleep and gate clk enable
	// 关控制器自动低功耗，避免初始化期间干扰
	ddrmc_lowpower_set();
	//step2: ddrphy regs config
	// 复位控制器 + 下发 PHY 时序 + 设位宽/行列
	ddrphy_soft_reset();
	// 下发 PHY 时序（GEN/AC/DS 各 TMG 寄存器）
	ddrphy_timing_fn();//ddrphy timing fn
	// 设 PHY 工作模式（DDR4=0x8 等）+ clk dly
	ddrphy_basic_set();
	ddrphy_soft_release();

	//DDR_DBG_PRINT("\r\ndll 0: ");
	//print_hex(__raw_readl(DDRPHY_AC0_(PARM_DDRPHY_GEN_TMG0_F0_ADDR)));
	//print_hex(__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG0_F0_ADDR)));


	//step3:ddrmc regs config
	ddrmc_soft_reset();
	// 下发控制器时序（TMG0~TMG19 → 0x180 起）
	ddrmc_timing_fn();//ddrmc timing fn
	// 设数据位宽/突发长度/bank/row/column 等
	ddrmc_basic_set();
	ddrmc_soft_release();
//	print_mc_reg();
	//step4:ddr clk cfg
	// 输入时钟配置（当前为空实现）
	ddrmc_clk_cfg(fn);
	//step5:pll io cfg
	//DDR_DBG_PRINT("\r\nstep5\r\n");
	// PHY PLL & IO 上电（force pd/rst → 释放）
	ddrphy_pll_io_set(fn);
	//reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_DS0_CFG3_ADDR), 25, 1, 0x1); //close ecc data slice
	//step6:master dll lock
	//DDR_DBG_PRINT("\r\nstep6\r\n");
	//  初始化 DDRPHY，等 DLL & PLL 锁定
	ddrphy_master_dll_lock(fn);
	//print_phy_reg();
	//update default timing setting
	// 更新 delay line 生效
	ddrphy_train_soft_update_ac();
	ddrphy_train_soft_update_dq();

	//loopback_test();  //for ate test

	//step7:dram power sequence
	//DDR_DBG_PRINT("\r\nstep7\r\n");
	// 颗粒上电时序 + 写 MR + ZQ
	dram_powerup_seq(fn);
	//DDR_DBG_PRINT("\r\ndram_powerup_seq pass\r\n");
}

void ddrmc_lock()
{
	//step8:enable ddrmc_rf wr lock
	__raw_writel(DDRMC_0_(0x0ffc),0x0);
	__raw_writel(DDRPHY_AC0_(0x7ffc),0x0);
	__raw_writel(DDRPHY_DQ0_(0x7ffc),0x0);
}

int loopback_test()
{
	u32 i, j;
#if DDR_test_type==2
#if loopback_test_type==0
	//DDR_DBG_PRINT("\r\nddr_loopback_scan_test start\r\n");
	u32 loopback_scan_stop = 64;
	u32 loopback_scan_start = 0;

#if loopback_ac_en
	for (i = 0; i < loopback_scan_stop; i++) {
		//DDR_DBG_PRINT("\r\nAC:");
		loopback_ac_test(loopback_type, i * 0x10);
		print_dec(i);
	}
#endif


#if loopback_dq0_ds0_en
	for (i = loopback_scan_start; i < loopback_scan_stop; i++) {
		//DDR_DBG_PRINT("\r\nDQ0_DS0:");
		loopback_dq_0_test(loopback_type, 1, i * 0x10);
		print_dec(i);
	}
#endif

#if loopback_dq0_ds1_en
	for (i = loopback_scan_start; i < loopback_scan_stop; i++) {
		//DDR_DBG_PRINT("\r\nDQ0_DS1:");
		loopback_dq_0_test(loopback_type, 2, i * 0x10);
		print_dec(i);
	}
#endif

	ddrmc_lock();
	exit(3);
#endif

#if loopback_test_type==1
	//DDR_DBG_PRINT("\r\nddr_loopback_test over\r\n");

	for (j = 0; j < 1000; j++)
	{
#if loopback_ac_en
		//DDR_DBG_PRINT("\r\nAC:");
		loopback_ac_test(loopback_type, 0x100);
#endif

#if loopback_dq0_ds0_en
		//DDR_DBG_PRINT("\r\nDQ0_DS0:");
		loopback_dq_0_test(loopback_type, 0x1, 0x220);
#endif
#if loopback_dq0_ds1_en
		//DDR_DBG_PRINT("\r\nDQ0_DS1:");
		loopback_dq_0_test(loopback_type, 0x2, 0x230);
#endif
	ddrmc_lock();
	exit(3);
#endif

#endif
	return 0;
}

void ddrphy_zqc_seq()
{

	u32 zqcal_flag, zqcal_val_a = 0, zqcal_val_b = 0;
	u32 zq_ncal = 0, zq_pcal = 0;
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR), 0, 8, 0x40);  //Adjust value of Internal VREF
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG10_F0_ADDR), 24, 2, 0x0);  //Power down of Internal VREF
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 26, 1, 0);	  //Enables lpddr4y IO of DQS		---disable
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 2, 1, 0x1);		  //Shutdown ZQ calibration circuit  ---close
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 0, 1, 0x0);		  //Enables NMOS ZQ calibration		---disable
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 16, 1, 0x0);	  //Enables PMOS ZQ calibration		---disable
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, 0x0);	  //PMOS IO setting for calibration	---reset
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, 0x0);		  //NMOS IO setting for calibration																				---reset
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 2, 1, 0x0);		  //Shutdown ZQ calibration circuit	---enable

	/***start NMOS zq calibration****/
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 0, 1, 0x1);//Enables NMOS ZQ calibration		---enable
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, 0x0);//reset NMOS IO setting for calibration to 0
	wait_us(20);
	zqcal_flag = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 1) & 0x1;	//Status of NMOS compare result
	if (zqcal_flag == 0x0) {
		while (1) {
			zqcal_val_a = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 4) & 0x3f;//read out NMOS IO setting for calibration
			zqcal_val_a += 1;
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, zqcal_val_a);		//write in NMOS IO setting for calibration
			wait_us(20);
			zqcal_flag = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 1) & 0x1;	//Status of NMOS compare result
			if (zqcal_flag == 1) {
				break;
			}
			if (zqcal_val_a == 0x3f) {
				//zq cal fail
				DDR_DBG_PRINT("\r\nI N =0x3f");//Inc NCAL fail
				break;
			}
		}
	}
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 0, 1, 0x0);	//Enables NMOS ZQ calibration		---disable
#if TEST_DEBUG_LOG_LEVEL>1

	DDR_DBG_PRINT("\r\npdio inc directions:");
	print_dec(zqcal_val_a);
#endif

	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 0, 1, 0x1);//Enables NMOS ZQ calibration		---enable
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, 0x3f);//NMOS IO setting for calibration		---reset to f
	wait_us(20);
	zqcal_flag = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 1) & 0x1;//ncomp
	if (zqcal_flag == 0x1) {
		while (1) {
			zqcal_val_b = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 4) & 0x3f;
			zqcal_val_b -= 1;
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, zqcal_val_b);//rf_zq_cal_pd_sel
			wait_us(20);
			zqcal_flag = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 1) & 0x1;//ncomp
			if (zqcal_flag == 0) {
				break;
			}
			if (zqcal_val_b == 0x0) {
				//zq cal fail
				DDR_DBG_PRINT("\r\nD N =0");//Dec NCAL fail
				break;
			}
		}
	}
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 0, 1, 0x0);//rf_zq_cal_ncal_en
#if TEST_DEBUG_LOG_LEVEL>1
	DDR_DBG_PRINT("\r\npdio dec directions:");
	print_dec(zqcal_val_b);
#endif

	if (zqcal_val_a > zqcal_val_b) {
		if ((zqcal_val_a - zqcal_val_b) <= 3) {
			zq_ncal = (zqcal_val_a + zqcal_val_b) / 2;
		}
		else {
			DDR_DBG_PRINT("\r\nD N > 4");//delta N > 4
//			while (1);
			zq_ncal = 0x5;
		}
	}
	else {
		if ((zqcal_val_b - zqcal_val_a) <= 3) {
			zq_ncal = (zqcal_val_a + zqcal_val_b) / 2;
		}
		else {
			DDR_DBG_PRINT("\r\nD P > 4");//delta P > 4
//			while (1);
			zq_ncal = 0x5;
		}
	}



	//	/***start pmos zq calibration****/
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 16, 1, 0x1);	//Enables PMOS ZQ calibration  ---enable
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, 0x3f);	//reset PMOS IO setting for calibration to f
	wait_us(20);
	zqcal_flag = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 17) & 0x1;	//Status of PMOS compare result
	if (zqcal_flag == 0x1) {
		while (1) {
			zqcal_val_a = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 20) & 0x3f;
			zqcal_val_a -= 1;
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, zqcal_val_a);//rf_zq_cal_pu_sel
			wait_us(200);
			zqcal_flag = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 17) & 0x1;
			if (zqcal_flag == 0) {
				break;
			}
			if (zqcal_val_a == 0x0) {
				//zq cal fail
				DDR_DBG_PRINT("\r\nD P =0");//Dec PCAL fail
				break;
			}
		}
	}
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 16, 1, 0x0); //Enables PMOS ZQ calibration  ---disable
#if TEST_DEBUG_LOG_LEVEL>1
	DDR_DBG_PRINT("\r\npuio dec directions:");
	print_dec(zqcal_val_a);
#endif

	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 16, 1, 0x1);//rf_zq_cal_pcal_en
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, 0x0);//rf_zq_cal_pu_sel
	wait_us(20);
	zqcal_flag = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 17) & 0x1;//pcomp
	if (zqcal_flag == 0x0) {
		while (1) {
			zqcal_val_b = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 20) & 0x3f;
			zqcal_val_b += 1;
			reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, zqcal_val_b);//rf_zq_cal_pu_sel
			wait_us(20);
			zqcal_flag = (__raw_readl(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 17) & 0x1;
			if (zqcal_flag == 1) {
				break;
			}
			if (zqcal_val_b == 0x3f) {
				//zq cal fail
				DDR_DBG_PRINT("\r\nI P =0xf");//Inc PCAL fail
				break;
			}
		}
	}
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 16, 1, 0x0);//rf_zq_cal_pcal_en
#if TEST_DEBUG_LOG_LEVEL>1
	DDR_DBG_PRINT("\r\npuio inc directions:");
	print_dec(zqcal_val_b);
#endif

	if (zqcal_val_a > zqcal_val_b) {
		if ((zqcal_val_a - zqcal_val_b) <= 3) {
			zq_pcal = (zqcal_val_a + zqcal_val_b) / 2;
		}
		else {
			DDR_DBG_PRINT("\r\nD P > 4");//delat P > 4
//			while (1);
			zq_pcal = 0x5;
		}
	}
	else {
		if ((zqcal_val_b - zqcal_val_a) <= 3) {
			zq_pcal = (zqcal_val_a + zqcal_val_b) / 2;
		}
		else {
			DDR_DBG_PRINT("\r\nD N > 4");//delat N > 4
//			while (1);
			zq_pcal = 0x5;
		}
	}

	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, zq_pcal);//set zq_pcal

	/***finish n/pmos zq calibration****/
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, 0x0);	//rf_zq_cal_pd_sel
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, 0x0);//rf_zq_cal_pu_sel
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 2, 1, 0x1);	//Shutdown ZQ calibration circuit
	if ((zq_ncal <= 0) || (zq_ncal >= 0x3f)) {
		//		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, 0x5);//rf_zq_cal_pdio_sel
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, 0x5);//rf_zq_cal_pdio_sel
	}
	else {
		//		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, zq_ncal);//rf_zq_cal_pdio_sel
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, zq_ncal);//rf_zq_cal_pdio_sel
	}
	if ((zq_ncal <= 0) || (zq_ncal >= 0x3f)) {
		////	if (zq_pcal >= 0xe) {
//		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, 0x6);//rf_zq_cal_puio_sel
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, 0x6);//rf_zq_cal_puio_sel
	}
	else {
		//		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, zq_pcal);//rf_zq_cal_puio_sel
		reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, zq_pcal);//rf_zq_cal_puio_sel
	}
	reg_bit_set(DDRPHY_DQ0_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, zq_ncal);//set zq_ncal

	//DDR_DBG_PRINT("\r\nzq_cal_puio_sel:");
	print_dec(zq_pcal);
	DDR_DBG_PRINT("\r\nzq_cal_pdio_sel:");
	print_dec(zq_ncal);
#if TEST_DEBUG_LOG_LEVEL>1
	//DDR_DBG_PRINT("\r\nzq_cal_puio_sel:");
	print_dec(zq_pcal);
	//DDR_DBG_PRINT("\r\nzq_cal_pdio_sel:");
	print_dec(zq_ncal);
#endif

}


void ddrphy_zqc_seq_ch1()
{

	u32 zqcal_flag, zqcal_val_a = 0, zqcal_val_b = 0;
	u32 zq_ncal = 0, zq_pcal = 0;
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR), 0, 8, 0x40);  //Adjust value of Internal VREF
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG10_F0_ADDR), 24, 2, 0x0);  //Power down of Internal VREF
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_TMG9_F0_ADDR), 26, 1, 0);	  //Enables lpddr4y IO of DQS		---disable
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 2, 1, 0x1);		  //Shutdown ZQ calibration circuit  ---close
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 0, 1, 0x0);		  //Enables NMOS ZQ calibration		---disable
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 16, 1, 0x0);	  //Enables PMOS ZQ calibration		---disable
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, 0x0);	  //PMOS IO setting for calibration	---reset
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, 0x0);		  //NMOS IO setting for calibration																				---reset
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 2, 1, 0x0);		  //Shutdown ZQ calibration circuit	---enable

	/***start NMOS zq calibration****/
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 0, 1, 0x1);//Enables NMOS ZQ calibration		---enable
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, 0x0);//reset NMOS IO setting for calibration to 0
	wait_us(20);
	zqcal_flag = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 1) & 0x1;	//Status of NMOS compare result
	if (zqcal_flag == 0x0) {
		while (1) {
			zqcal_val_a = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 4) & 0x3f;//read out NMOS IO setting for calibration
			zqcal_val_a += 1;
			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, zqcal_val_a);		//write in NMOS IO setting for calibration
			wait_us(20);
			zqcal_flag = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 1) & 0x1;	//Status of NMOS compare result
			if (zqcal_flag == 1) {
				break;
			}
			if (zqcal_val_a == 0x3f) {
				//zq cal fail
				DDR_DBG_PRINT("\r\nI N =0x3f");//Inc NCAL fail
				break;
			}
		}
	}
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 0, 1, 0x0);	//Enables NMOS ZQ calibration		---disable
#if TEST_DEBUG_LOG_LEVEL>1

	DDR_DBG_PRINT("\r\npdio inc directions:");
	print_dec(zqcal_val_a);
#endif

	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 0, 1, 0x1);//Enables NMOS ZQ calibration		---enable
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, 0x3f);//NMOS IO setting for calibration		---reset to f
	wait_us(20);
	zqcal_flag = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 1) & 0x1;//ncomp
	if (zqcal_flag == 0x1) {
		while (1) {
			zqcal_val_b = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 4) & 0x3f;
			zqcal_val_b -= 1;
			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, zqcal_val_b);//rf_zq_cal_pd_sel
			wait_us(20);
			zqcal_flag = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 1) & 0x1;//ncomp
			if (zqcal_flag == 0) {
				break;
			}
			if (zqcal_val_b == 0x0) {
				//zq cal fail
				DDR_DBG_PRINT("\r\nD N =0");//Dec NCAL fail
				break;
			}
		}
	}
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 0, 1, 0x0);//rf_zq_cal_ncal_en
#if TEST_DEBUG_LOG_LEVEL>1
	DDR_DBG_PRINT("\r\npdio dec directions:");
	print_dec(zqcal_val_b);
#endif

	if (zqcal_val_a > zqcal_val_b) {
		if ((zqcal_val_a - zqcal_val_b) <= 3) {
			zq_ncal = (zqcal_val_a + zqcal_val_b) / 2;
		}
		else {
			DDR_DBG_PRINT("\r\nD N > 4");//delta N > 4
//			while (1);
			zq_ncal = 0x5;
		}
	}
	else {
		if ((zqcal_val_b - zqcal_val_a) <= 3) {
			zq_ncal = (zqcal_val_a + zqcal_val_b) / 2;
		}
		else {
			DDR_DBG_PRINT("\r\nD P > 4");//delta P > 4
//			while (1);
			zq_ncal = 0x5;
		}
	}



	//	/***start pmos zq calibration****/
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 16, 1, 0x1);	//Enables PMOS ZQ calibration  ---enable
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, 0x3f);	//reset PMOS IO setting for calibration to f
	wait_us(20);
	zqcal_flag = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 17) & 0x1;	//Status of PMOS compare result
	if (zqcal_flag == 0x1) {
		while (1) {
			zqcal_val_a = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 20) & 0x3f;
			zqcal_val_a -= 1;
			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, zqcal_val_a);//rf_zq_cal_pu_sel
			wait_us(200);
			zqcal_flag = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 17) & 0x1;
			if (zqcal_flag == 0) {
				break;
			}
			if (zqcal_val_a == 0x0) {
				//zq cal fail
				DDR_DBG_PRINT("\r\nD P =0");//Dec PCAL fail
				break;
			}
		}
	}
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 16, 1, 0x0); //Enables PMOS ZQ calibration  ---disable
#if TEST_DEBUG_LOG_LEVEL>1
	DDR_DBG_PRINT("\r\npuio dec directions:");
	print_dec(zqcal_val_a);
#endif

	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 16, 1, 0x1);//rf_zq_cal_pcal_en
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, 0x0);//rf_zq_cal_pu_sel
	wait_us(20);
	zqcal_flag = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 17) & 0x1;//pcomp
	if (zqcal_flag == 0x0) {
		while (1) {
			zqcal_val_b = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 20) & 0x3f;
			zqcal_val_b += 1;
			reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, zqcal_val_b);//rf_zq_cal_pu_sel
			wait_us(20);
			zqcal_flag = (__raw_readl(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR)) >> 17) & 0x1;
			if (zqcal_flag == 1) {
				break;
			}
			if (zqcal_val_b == 0x3f) {
				//zq cal fail
				DDR_DBG_PRINT("\r\nI P =0xf");//Inc PCAL fail
				break;
			}
		}
	}
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 16, 1, 0x0);//rf_zq_cal_pcal_en
#if TEST_DEBUG_LOG_LEVEL>1
	DDR_DBG_PRINT("\r\npuio inc directions:");
	print_dec(zqcal_val_b);
#endif

	if (zqcal_val_a > zqcal_val_b) {
		if ((zqcal_val_a - zqcal_val_b) <= 3) {
			zq_pcal = (zqcal_val_a + zqcal_val_b) / 2;
		}
		else {
			DDR_DBG_PRINT("\r\nD P > 4");//delat P > 4
//			while (1);
			zq_pcal = 0x5;
		}
	}
	else {
		if ((zqcal_val_b - zqcal_val_a) <= 3) {
			zq_pcal = (zqcal_val_a + zqcal_val_b) / 2;
		}
		else {
			DDR_DBG_PRINT("\r\nD N > 4");//delat N > 4
//			while (1);
			zq_pcal = 0x5;
		}
	}

	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, zq_pcal);//set zq_pcal

	/***finish n/pmos zq calibration****/
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 4, 6, 0x0);	//rf_zq_cal_pd_sel
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 20, 6, 0x0);//rf_zq_cal_pu_sel
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 2, 1, 0x1);	//Shutdown ZQ calibration circuit
	if ((zq_ncal <= 0) || (zq_ncal >= 0x3f)) {
		//		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, 0x5);//rf_zq_cal_pdio_sel
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, 0x5);//rf_zq_cal_pdio_sel
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, 0x5);//rf_zq_cal_pdio_sel
	}
	else {
		//		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, zq_ncal);//rf_zq_cal_pdio_sel
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, zq_ncal);//rf_zq_cal_pdio_sel
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, zq_ncal);//rf_zq_cal_pdio_sel
	}
	if ((zq_ncal <= 0) || (zq_ncal >= 0x3f)) {
		////	if (zq_pcal >= 0xe) {
//		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, 0x6);//rf_zq_cal_puio_sel
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, 0x6);//rf_zq_cal_puio_sel
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, 0x6);//rf_zq_cal_puio_sel
	}
	else {
		//		reg_bit_set(DDRPHY_AC0_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, zq_pcal);//rf_zq_cal_puio_sel
		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, zq_pcal);//rf_zq_cal_puio_sel
//		reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 26, 6, zq_pcal);//rf_zq_cal_puio_sel
	}
	reg_bit_set(DDRPHY_DQ1_(PARM_DDRPHY_GEN_CFG1_ADDR), 10, 6, zq_ncal);//set zq_ncal

	//DDR_DBG_PRINT("\r\nzq_cal_puio_sel:");
	print_dec(zq_pcal);
	DDR_DBG_PRINT("\r\nzq_cal_pdio_sel:");
	print_dec(zq_ncal);
#if TEST_DEBUG_LOG_LEVEL>1
	//DDR_DBG_PRINT("\r\nzq_cal_puio_sel:");
	print_dec(zq_pcal);
	//DDR_DBG_PRINT("\r\nzq_cal_pdio_sel:");
	print_dec(zq_ncal);
#endif

}

void dram_info_detect()
{
#ifdef D_DRAM_DDR3
	DDR_DBG_PRINT("\r\ndram_type is DDR3\r\n");
	dram_info.dram_type=DRAM_DDR3;
	dram_info.dram_detect_type=DRAM_DDR3;
#elif D_DRAM_DDR4
	DDR_DBG_PRINT("\r\ndram_type is DDR4\r\n");
	dram_info.dram_type=DRAM_DDR4;
	dram_info.dram_detect_type=DRAM_DDR4;
#elif D_DRAM_DDR5
	DDR_DBG_PRINT("\r\ndram_type is DDR5\r\n");
	dram_info.dram_type=DRAM_DDR5;
	dram_info.dram_detect_type=DRAM_DDR5;
#elif D_DRAM_LPDDR3
	DDR_DBG_PRINT("\r\ndram_type is LP3\r\n");
	dram_info.dram_type=DRAM_LP3;
	dram_info.dram_detect_type=DRAM_LP3;
#elif D_DRAM_LPDDR4
	DDR_DBG_PRINT("\r\ndram_type is LP4\r\n");
	dram_info.dram_type=DRAM_LP4;
	dram_info.dram_detect_type=DRAM_LP4;
#endif

#ifdef DRAM_DQ_X16_MODE
	dram_info.dram_dq_width = DRAM_DQ_X16;
#else
	dram_info.dram_dq_width = DRAM_DQ_X32;
#endif

	dram_info.intlv_size=0x40;
	dram_info.fn_max=3;
	dram_info.cs_num=0;
	dram_info.cs0_size=0x10000000; //256MB
	dram_info.cs1_size=0x0;
	dram_info.mr_reg_cs0=0x0;
	dram_info.mr_reg_cs1=0x0;

//0:1600, 1:1333,2:800
     if (BOOT_FREQ_POINT == DDR_CLK_800M) {
           dram_info.ddr_freq = 0;
     }
     else if (BOOT_FREQ_POINT == DDR_CLK_1333M) {
           dram_info.ddr_freq = 0;
     }
     else {
           dram_info.ddr_freq = 0;
     }
//	switch (BOOT_FREQ_POINT)
//	{
//	case DDR_CLK_1600M:
//		dram_info.ddr_freq = 0;
//		break;
//	case DDR_CLK_1333M:
//		dram_info.ddr_freq = 1;
//		break;
//	case DDR_CLK_1200M:
//		dram_info.ddr_freq = 0;
//		break;
//	case DDR_CLK_1066M:{
//		if(dram_info.dram_type == DRAM_DDR3)
//		{
//			dram_info.ddr_freq = 0;
//		}
//		else
//		{
//			dram_info.ddr_freq = 3;
//		}
//        break;
//		}
//	case DDR_CLK_933M:
//		dram_info.ddr_freq = 4;
//		break;
//	case DDR_CLK_800M:{
//		if(dram_info.dram_type == DRAM_DDR3)
//		{
//			dram_info.ddr_freq = 1;
//		}
//		else
//		{
//			dram_info.ddr_freq = 1;
//		}
//        break;
//		}
//	case DDR_CLK_400M:
//		dram_info.ddr_freq = 6;
//		break;
//	default:
//		dram_info.ddr_freq = 0;
//		break;
//	}
//	DDR_DBG_PRINT("\r\nddr_freq start\r\n");
//	print_hex(dram_info.ddr_freq);
//	DDR_DBG_PRINT("\r\nddr_freq end\r\n");
}

void bist_init(u32 write_or_read, u32 data_pattern_mode, u32 bist_data_mode, u32 bist_size, u32 bist_src)
{
	u32 i = 0;

	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR),0x0);
	//bist enable
	reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 0, 1, 0x1);
	//bist_mode, 2'b00:write 2'01 read, 2'b10 all read after all write 2'b11 one read after one write
	reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 4, 2, write_or_read);
	//bist_data_pattern_mode, 2'b00:user data pattern, 2'b01:sipi data pattern, 2'b10:lfsr data pattern
	reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 8, 2, data_pattern_mode);
	//bist_burst_length
	reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 12, 4,0x7);
	//bist_data_size, 3'b000:byte, 3'b001:half word, 3'b010:word, 3'b011:two words, 3'b100:four words, 3'b101:eight words
	reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 20, 3, bist_data_mode);
	//bist_write_outstanding_en/bist_read_outstanding_en
	if(write_or_read == 3) {
		reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 24, 2, 0x0);
	}
	else {
		reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 24, 2, 0x3);
	}
	//datawidth_mode, 1'b0 16bit, 1'b1 32bit
	if (dram_info.dram_dq_width==DRAM_DQ_X32) {
        reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 28, 2, 1);
    } else if (dram_info.dram_dq_width==DRAM_DQ_X64) {
        reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 28, 2, 2);
    } else if (dram_info.dram_dq_width==DRAM_DQ_X08) {
        reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 28, 2, 3);
    } else {
	    reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 28, 2, 0);
    }
	//bist_trans_num
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL2_ADDR), bist_size);
	//bist_start_addr
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL3_ADDR), bist_src);
	switch(data_pattern_mode)
	{
		case USER_DATA_PATTERN:
			for(i = 0; i < 32; i++)
			{
				__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR+i*4),user_mode_pattern[i]);
			}
			break;
		case SIPI_DATA_PATTERN:
			for(i = 0; i < 16; i++)
			{
				__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR+i*4) , sipi_mode_pattern[2+i]);
				__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA16_ADDR+i*4),~sipi_mode_pattern[2+i]);
			}
	        reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 26, 1, 1);      //rf_sipi_auto_mode
	        reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL1_ADDR), 12, 10, 0x150); //rf_sipi_data_pattern_bit_num 336
	        reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL1_ADDR), 24,  4, 0x0);   //rf_sipi_data_auto_mode
            __raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_SIPI_BIT_PATTERN0_ADDR),sipi_mode_pattern[0]); //rf_bist_sipi_bit_pattern_0
            __raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_SIPI_BIT_PATTERN1_ADDR),sipi_mode_pattern[1]); //rf_bist_sipi_bit_pattern_1
			break; 
		case LFSR_DATA_PATTERN:
			for(i = 0; i < 4; i++)
			{
				__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_LFSR_SEED0_ADDR+i*4),lfsr_mode_pattern[i]);
			}
			break;
	}
}

void print_bist_fail(void)
{
	u32 i = 0;
	u32 expt_data = 0;
	u32 real_data = 0;

	//bist fail cnt
	//DDR_DBG_PRINT(" fail cnt: ");
	print_dec(__raw_readl(DDRMC_BIST_(PARM_DDRMC_BIST_FAIL_CNT_ADDR)));
	//DDR_DBG_PRINT(" fail addr: ");
	//print_hex(__raw_readl(DDRMC_BIST_(PARM_DDRMC_BIST_FAIL_CNT_ADDR)));
	for (i = 0; i < 4; i++) {
		expt_data = __raw_readl(DDRMC_BIST_(PARM_DDRMC_BIST_EXPT_DATA0_ADDR + i * 4));
		real_data = __raw_readl(DDRMC_BIST_(PARM_DDRMC_BIST_FAIL_DATA0_ADDR + i * 4));
//		if (real_data != expt_data) {
		if (1) {
			//DDR_DBG_PRINT("\r\nexpt: ");
			//print_hex(expt_data);
			//DDR_DBG_PRINT("real: ");
			//print_hex(real_data);
		}
	}
	for (i = 0; i < 4; i++) {
		expt_data = __raw_readl(DDRMC_BIST_(PARM_DDRMC_BIST_EXPT_DATA4_ADDR + i * 4));
		real_data = __raw_readl(DDRMC_BIST_(PARM_DDRMC_BIST_FAIL_DATA4_ADDR + i * 4));
//		if (real_data != expt_data) {
		if (1) {
			//DDR_DBG_PRINT("\r\nexpt: ");
			//print_hex(expt_data);
			//DDR_DBG_PRINT("real: ");
			//print_hex(real_data);
		}

	}
	for (i = 0; i < 4; i++) {
		expt_data = __raw_readl(DDRMC_BIST_(PARM_DDRMC_BIST_FAIL_ALL_DATA0_ADDR + i * 4));
		if (1) {
			//DDR_DBG_PRINT("\r\nfail_sum: ");
			//print_hex(expt_data);
		}
	}
}

int bist_test()
{
    //bist_clear
	reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR),1,2,0x2);
	reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR),1,2,0x0);
	//bist_trigger
   	reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR),1,1,0x1);
    //bist_wait_done
	while((__raw_readl(DDRMC_BIST_(PARM_DDRMC_BIST_RESULT_ADDR))&0x1) != 0x1);
    wait_100ns(1);
	while((__raw_readl(DDRMC_BIST_(PARM_DDRMC_BIST_RESULT_ADDR))&0x1) != 0x1);

	if((__raw_readl(DDRMC_BIST_(PARM_DDRMC_BIST_RESULT_ADDR))&0x2) == 0x0)
	{
		//DDR_DBG_PRINT(" bist pass");
		return 0;
	} else
	{
		//DDR_DBG_PRINT(" bist fail");
		print_bist_fail();
//		while (1);
		return 1;

	}
}

void ddrphy_ca_deskew_pre_set()
{
	//ca output delay mannual setting
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG11_ADDR), 0x00000000);//ca3  ca2  ca1 ca0
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG12_ADDR), 0x00000000);//ca7  ca6  ca5 ca4
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG13_ADDR), 0x00000000);//ca11 ca10 ca9 ca8
	__raw_writel(DDRPHY_AC0_(PARM_DDRPHY_AC_CFG14_ADDR), 0x00000000);//cs1  cs0  ca13 ca12
}

int dram_bist_test(u32 start)
{
	//bist clear
	//dram_info.cs0_size = 0x10;
	//dram_info.cs1_size = 0x10;
	//u32 bist_addr = dram_info.cs0_size; //dram_info.cs1_size};
	u32 bist_addr = 0x0; //dram_info.cs1_size};
	u32 size;
	u32 bist_result = 0;
	int ret = 0;
//	size = 0xfffffe;//512x16 all memory space
//	size = 0x7ffffe;//256x16 all memory space
	size = 0x0fe;
   // if (start == 1) {
//	     bist_addr = 0x20000000; //dram_info.cs1_size};
    //    }
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL4_ADDR), 0x0); //data_0 mask
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL5_ADDR), 0x0); //data_1 mask
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 2, 1, 0x1);//rf_train_enable
	//size = (u32)(dram_info.cs0_size + dram_info.cs1_size);

#if bist_test_type==0

	/*if(start % 6==0) {
		//DDR_DBG_PRINT("BIST_ALLWRC USER_DATA_PATTERN");
	    bist_init(BIST_ALLWRC, USER_DATA_PATTERN, 5, size, bist_addr);
	}
	else if(start % 6==1) {
		//DDR_DBG_PRINT("BIST_ALLWRC SIPI_DATA_PATTERN");
	    bist_init(BIST_ALLWRC, SIPI_DATA_PATTERN, 5, size, bist_addr);
	}
	else if(start % 6==2) {
		//DDR_DBG_PRINT("BIST_ALLWRC LFSR_DATA_PATTERN");
	    bist_init(BIST_ALLWRC, LFSR_DATA_PATTERN, 5, size, bist_addr);
	}
	else if(start % 6==3) {
		//DDR_DBG_PRINT("BIST_ONEWRC USER_DATA_PATTERN");
	    bist_init(BIST_ONEWRC, USER_DATA_PATTERN, 5, size, bist_addr);
	}
	else if(start % 6==4) {
		//DDR_DBG_PRINT("BIST_ONEWRC SIPI_DATA_PATTERN");
	    bist_init(BIST_ONEWRC, SIPI_DATA_PATTERN, 5, size, bist_addr);
	}
	else if(start % 6==5) {
		//DDR_DBG_PRINT("BIST_ONEWRC LFSR_DATA_PATTERN");
	    bist_init(BIST_ONEWRC, LFSR_DATA_PATTERN, 5, size, bist_addr);
	}*/
	    bist_init(BIST_ONEWRC, USER_DATA_PATTERN, 5, size, bist_addr);
#elif bist_test_type==1
//bist read or write only
	// write only
	//DDR_DBG_PRINT("BIST_WRITE SIPI_DATA_PATTERN");
	bist_init(BIST_WRITE, SIPI_DATA_PATTERN, 5, size, bist_addr);
	reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 3, 1, 0x1);


#elif bist_test_type==2
	// read only
	//DDR_DBG_PRINT("BIST_READ SIPI_DATA_PATTERN");
	bist_init(BIST_WRITE, SIPI_DATA_PATTERN, 5, size, bist_addr);
	reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 3, 1, 0x0);
	bist_test();
	bist_init(BIST_READ, SIPI_DATA_PATTERN, 5, size, bist_addr);
	reg_bit_set(DDRMC_BIST_(PARM_DDRMC_BIST_CTRL0_ADDR), 3, 1, 0x1);

#endif
	ret = bist_test();
	reg_bit_set(DDRMC_TRAIN_(PARM_DDRMC_TRAIN_CTRL0_ADDR), 2, 1, 0x0);//rf_train_enable
	return ret;
}

void ddrphy_dq_deskew_pre_set()
{
	//dq0 output delay mannual setting
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG9_ADDR),  0x00000000);//dq3  dq2  dq1 dq0
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG10_ADDR), 0x00000000);//dq7  dq6  dq5 dq4
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG11_ADDR), 0x00000000);//dqm

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG9_ADDR),  0x00000000);//dq3  dq2  dq1 dq0
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG10_ADDR), 0x00000000);//dq7  dq6  dq5 dq4
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG11_ADDR), 0x00000000);//dqm

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG15_ADDR), 0x00000000);//dq3  dq2  dq1 dq0
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG16_ADDR), 0x00000000);//dq7  dq6  dq5 dq4
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS0_CFG17_ADDR), 0x00000000);//dqm

	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG15_ADDR), 0x00000000);//dq3  dq2  dq1 dq0
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG16_ADDR), 0x00000000);//dq7  dq6  dq5 dq4
	__raw_writel(DDRPHY_DQ0_(PARM_DDRPHY_DS1_CFG17_ADDR), 0x00000000);//dqm


}

void sdram_print_lockvalue()
{
    DDR_DBG_PRINT("\r\nddr lock value:\r\n");

	print_hex(__raw_readl(DDRMC_0_(0x0ffc)));
	print_hex(__raw_readl(DDRPHY_DQ0_(0x7ffc)));
}

void sdram_init()
{
	u32 i, j;
	u32 fn;
	// ddr init start
	DDR_DBG_PRINT("\r\nddr init start\r\n");
	sdram_print_lockvalue();

	//dram size auto-detect
	// 确定 DRAM 类型/位宽/容量/频率点
	dram_info_detect();

	//zq calibration
//	DDR_DBG_PRINT("\r\nzq calibration\r\n");
    
//    ddrphy_zqc_seq();  
	//pinmux setting
	//DDR_DBG_PRINT("\r\npinmux setting\r\n");
	// CA/DQ 引脚映射（pin swap）
	ddrphy_pinmux_set();
//	ddrphy_ca_deskew_pre_set(); //use for debug 
//
//	ddrphy_ca_deskew_pre_set(); //use for debug
//
//	ddrphy_dq_deskew_pre_set(); //use for debug
	DDR_DBG_PRINT("\r\ndram init\r\n");
	// 频率点 → fn 索引（当前恒返回 0）
	ddrmc_freq_sel_search(BOOT_FREQ_POINT, &fn);
	// 主体初始化
	dram_init(BOOT_FREQ_POINT,fn);
	//training flow
	// 复位DDRPHY内部fifo
	ddrphy_fifo_reset();
	__raw_writel(DDRMC_BIST_(PARM_DDRMC_BIST_DATA0_ADDR), 0x1);
	// 训练
	ddrmc_train_seq(fn);
	ddrphy_fifo_reset();

	// 开自动刷新
	ddrmc_auto_refresh_open(fn);
    DDR_DBG_PRINT("\r\nddrmc_auto_refresh_open over\r\n");
	ddrphy_fifo_reset();
	DDR_DBG_PRINT("\r\nddr init end\r\n");

	ddrphy_dll_cnt_print();

//	print_phy_reg();
//	print_mc_reg();
//	ddrmc_axi_perf_mon_enable(0x1);

#if DDR_test_type==1
	//DDR_DBG_PRINT("\r\ndram_eyeScan_test start\r\n");
	rdeye_test();
	wreye_test();
	//DDR_DBG_PRINT("\r\ndram_eyeScan_test over\r\n");
#elif DDR_test_type==0
	//DDR_DBG_PRINT("\r\ndram_bist_test start\r\n");
	//for (i = 0; i < 60; i++) {
		//DDR_DBG_PRINT("\r\nloop:");
//		print_dec(i);
		//dram_bist_test(i);
//	}
    //DDR_DBG_PRINT("\r\ndram_bist_test over\r\n");

//	ddrmc_axi_perf_mon_disable(0x1);
	/*
	*ddr init end
	*1.ddrmc lowpower
	*3.mr4 function
	*4.dfs function
	*5.auto cpst
	*6.auto zqc
	*/

	// 使能低功耗/自动 zqc/cpst 等
	ddrmc_post_set(fn);

	/***memtest***/
	//dram_mem_test(0x80000000,0xc0000000);
	DDR_DBG_PRINT("\r\ndram_bist_test start\r\n");
	for (i = 0; i < 2; i++) {
		//DDR_DBG_PRINT("\r\nloop:");
		print_dec(i);
		dram_bist_test(i);
//		//print_hex(__raw_readl(DDRMC_0_(PARM_DDRMC_CFG32_ADDR)));//mrr, temp sensor
	}
	DDR_DBG_PRINT("\r\ndram_bist_test over\r\n");
#endif

	// 锁定寄存器访问
	ddrmc_lock();
}

/* 供应商裸机 main() 已移除：Zephyr 有自己的 main()，
   由 src/main.c 的 ddr_init() 调用 sdram_init() 进入 DDR 初始化。 */
