#ifndef __DDR_COMMON_H
#define __DDR_COMMON_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long ulong;
typedef unsigned long long u64;
typedef char boolean;


#ifndef REG32
#define REG32(_X_)	(*(volatile u32*)((u64)(_X_)))
#endif

#ifndef TRUE
#define TRUE		1
#endif
#ifndef FALSE
#define FALSE		0
#endif
#ifndef NULL
#define NULL		0
#endif

#define	DDR_CLK_26M	    26
#define	DDR_CLK_133M	133
#define	DDR_CLK_200M	200
#define	DDR_CLK_400M	400
#define	DDR_CLK_533M	533
#define	DDR_CLK_667M	667
#define	DDR_CLK_800M	800
#define	DDR_CLK_933M	933
#define	DDR_CLK_1066M	1066
#define	DDR_CLK_1200M	1200
#define	DDR_CLK_1333M	1333
#define	DDR_CLK_1600M	1600
#define	DDR_CLK_1866M	1866
#define	DDR_CLK_2133M	2133

#define SOC_SOFT_RST_CTRL			0x00000000
#define DDRMC_REG_BASE_ADDR		    0xC4000000
#define DDRPHY_AC0_REG_BASE_ADDR	0xC4008000        
#define DDRPHY_AC1_REG_BASE_ADDR	0xC4008000        
#define DDRPHY_DQ0_REG_BASE_ADDR	0xC4008000
#define DDRPHY_DQ1_REG_BASE_ADDR	0xC4008000

#define DDRMC_TRAIN_(x)	(DDRMC_REG_BASE_ADDR + 0x00000C00 + (x))
#define DDRMC_BIST_(x)	(DDRMC_REG_BASE_ADDR + 0x00000E00 + (x))
#define DDRMC_0_(x)	    (DDRMC_REG_BASE_ADDR + (x))
#define DDRPHY_AC0_(x)	(DDRPHY_AC0_REG_BASE_ADDR + (x))
#define DDRPHY_AC1_(x)	(DDRPHY_AC1_REG_BASE_ADDR + (x))
#define DDRPHY_DQ0_(x)	(DDRPHY_DQ0_REG_BASE_ADDR + (x))
#define DDRPHY_DQ1_(x)	(DDRPHY_DQ1_REG_BASE_ADDR + (x))
#define DDR_STATUS  	(DDRMC_REG_BASE_ADDR + 0x0000)


u32 __raw_readl(u32 addr);
void __raw_writel(u32 addr, u32 val);
void reg_bit_set(u32 addr, u32 start_bit, u32 bits_num, u32 val);
u32 reg_bit_get(u32 addr, u32 start_bit, u32 bits_num);
void wait_us(u32 us);
void wait_100ns(u32 ns);
void DDR_DBG_PRINT(const char *string);
void print_dec(int val);
void print_hex(u32 val);
void print_string(const char *string);
#endif
