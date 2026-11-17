#include <zephyr/sys/printk.h>
#include "ddrmc_common.h"

u32 __raw_readl(u32 addr)
{
	volatile u32 val;
	val = REG32(addr);
	return val;
}

void __raw_writel(u32 addr, u32 val)
{
	REG32(addr) = val;
}

void inline reg_bit_set(u32 addr, u32 start_bit, u32 bits_num, u32 val)
{
	u32 tmp_val, bit_mask = ((1 << bits_num) - 1);
	tmp_val = REG32(addr);
	tmp_val &= ~(bit_mask << start_bit);
	tmp_val |= ((val & bit_mask) << start_bit);
	REG32(addr) = tmp_val;
}

u32 reg_bit_get(u32 addr, u32 start_bit, u32 bits_num)
{
	u32 tmp_val, bit_msk = ((1 << bits_num) - 1);
	tmp_val = REG32(addr);
	tmp_val &= (bit_msk << start_bit);
	tmp_val = tmp_val >> start_bit;
	return tmp_val;
}

void wait_us(u32 us)
{
	volatile u32 i = 0;
	volatile u32 j = 0;
	volatile u32 reg = 0;
	for (i = 0; i < us; i++) {
		for (j = 0; j < 14; j++) //calculate according to freq
			reg = REG32(DDR_STATUS);
	}
}
void wait_100ns(u32 ns)
{
	volatile u32 i = 0;
	volatile u32 reg = 0;
	for (i = 0; i < ns; i++) //calculate according to freq
		reg = REG32(DDR_STATUS);
}

#define UART_TX_BUF_ADDR 0x0000
#define UART_TX_BUF_CNT ((REG32(UART_TX_BUF_ADDR + 0xc)>>8)&0xff)
void DDR_DBG_PRINT(const char *string)
{
	//char *s1 = NULL;

	//s1 = (char *)string;

	//while (*s1 != '\0') {
	//    //wait until uart1 tx fifo empty
	//    while(UART_TX_BUF_CNT != 0);
	//    //put out char by uart1 tx fifo
	//    REG32(UART_TX_BUF_ADDR) = *s1;
	//    s1++;
	//}
     printk("%s", string);
}

static char  log_string[33];
static char *  toStrDec(u32 val)
{

	int i, j, num;
	u32 temp = val;	num = 0;
	do {
		num++;
		temp /= 10;
	} while (temp > 0);
	for (i = num - 1; i >= 0; i--) {
		temp = (val % 10) + 0x30;
		val /= 10;
		log_string[i] = temp & 0xff;
	}
	log_string[num] = ' ';
	log_string[num + 1] = 0;
	return log_string;
}

static char *  toStrHex(u32 val)
{
	int i, j, num;
	u32 temp = val;
	log_string[0] = '0';
	log_string[1] = 'x';
	for (i = 0; i < 8; i++) {
		temp = (val >> ((7 - i) * 4)) & 0xf;
		if (temp < 0xa)
			log_string[2 + i] = temp + 0x30;
		else
			log_string[2 + i] = 'A' + temp - 0xa;
	}
	log_string[10] = ' ';
	log_string[11] = 0;
	return log_string;
}

void print_dec(int val)
{
	if (val < 0) {
		DDR_DBG_PRINT("-");
		val = 0 - val;
	}
	DDR_DBG_PRINT(toStrDec(val));
}

void print_hex(u32 val)
{
	DDR_DBG_PRINT(toStrHex(val));
}

void ddr_reg_dump(void)
{
	u32 regval, i, j;
	u32 reg_len[4] = {
		0x7EC, 0x7EC, 0x7EC, 0x624
	};
	u32 base_add[4] = {
		DDRPHY_AC0_REG_BASE_ADDR,
		DDRPHY_DQ0_REG_BASE_ADDR,
		DDRMC_REG_BASE_ADDR
	};

	for (j = 0; j < 2; j++) {
		if (j == 0)
			DDR_DBG_PRINT("ddrphy ac0 reg:\r\n");
		else if (j == 1)
			DDR_DBG_PRINT("ddrphy dq0 reg:\r\n");
		for (i = 0; i <= reg_len[j];) {
			print_hex(i);
			DDR_DBG_PRINT(":");
			regval = REG32(base_add[j] + i);
			print_hex(regval);
			regval = REG32(base_add[j] + 4 + i);
			print_hex(regval);
			regval = REG32(base_add[j] + 8 + i);
			print_hex(regval);
			regval = REG32(base_add[j] + 12 + i);
			print_hex(regval);
			DDR_DBG_PRINT("\r\n");
			i += 0x10;
		}
	}
}

