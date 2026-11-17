#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/version.h>
#include <zephyr/irq.h>
#include "reg_base_addr.h"
#include "reg_top_dig.h"
#include "ls_hal_ssiv2.h"
#include "reg_cpu0_sys.h"
#include "reg_cpu1_sys.h"
#include "field_manipulate.h"
#include "raptor.h"
#include "ls_hal_iwdgv2.h"

/* 烧录 / 引导 模式切换：1 = 先写(内嵌固件→flash)；0 = 后读(flash→DDR) */
#define FLASH_PROGRAM_MODE  0
#define DDR_KNL             0

#define C908_BOOT_FLASH_OFFSET  0x0
#define C908_BOOT_SIZE          0x5d00   //23552

#define FWDYN_LOAD_ADDR     0xc0000000UL   /* DDR 基址 */
#define FWDYN_FLASH_OFFSET  0x5e00
#define FWDYN_SIZE_BYTES    0x42000   // 270336

#define DTB_LOAD_ADDR       0xc0300000UL   /* DDR 里的目标地址 */
#define DTB_FLASH_OFFSET    0x48000
#define DTB_SIZE_BYTES      0x1c29   //7209

#define ZSB_LOAD_ADDR       0xc03f8000UL
#define ZSB_FLASH_OFFSET    0x4a000
#define ZSB_SIZE_BYTES      0x1218   //4632


#define IMAGE_LOAD_ADDR     0xc0400000UL
#define IMAGE_FLASH_OFFSET  0x4b300
#define IMAGE_SIZE_BYTES    0x1523c00   //22166528

#define BUSYBOX_LOAD_ADDR      0xd0000000
#define BUSYBOX_FLASH_OFFSET   0x156f000
#define BUSYBOX_SIZE_BYTES     0x1c9a00   // 1874432

extern const uint8_t _binary_c908_boot_bin_start[];
extern const uint8_t _binary_fw_dynamic_bin_start[];
extern const uint8_t _binary_u_boot_dtb_start[];
extern const uint8_t _binary_zero_stage_boot_bin_start[];

static struct SSIV2_HandleTypeDef g_flash;

K_MSGQ_DEFINE(rst_notify_msgq, sizeof(uint32_t), 8, 4);

void pll_clk_cfg()
{
    TOP_DIG_CFG -> APLL_CTL0 = 0x81aa0412;
    TOP_DIG_CFG -> BPLL_CTL0 = 0x82820414;
    TOP_DIG_CFG -> CPLL_CTL0 = 0x81820412;
    TOP_DIG_CFG -> DPLL_CTL0 = 0x81aa0412;
    TOP_DIG_CFG -> VPLL_CTL0 = 0x894a1452;
}

#if DDR_KNL
    extern void sdram_init(void);
    void knl_ddr_init()
    {
        sdram_init();
    }
#else
    // #define FPGA_DDR32(off) (*(volatile uint32_t *)(0xa4010000u + (off)))
    // void fpga_ddr_init(void)
    // {
    //     /* 8 组 DDRPHY 时序初值（每组 3 个寄存器） */
    //     FPGA_DDR32(0x000) = 0x01000202;
    //     FPGA_DDR32(0x004) = 0x00000000;
    //     FPGA_DDR32(0x008) = 0x00000000;
    //     FPGA_DDR32(0x010) = 0x01000202;
    //     FPGA_DDR32(0x014) = 0x00000000;
    //     FPGA_DDR32(0x018) = 0x00000000;
    //     FPGA_DDR32(0x020) = 0x01000202;
    //     FPGA_DDR32(0x024) = 0x00000000;
    //     FPGA_DDR32(0x028) = 0x00000000;
    //     FPGA_DDR32(0x030) = 0x01000202;
    //     FPGA_DDR32(0x034) = 0x00000000;
    //     FPGA_DDR32(0x038) = 0x00000000;
    //     FPGA_DDR32(0x040) = 0x01000202;
    //     FPGA_DDR32(0x044) = 0x00000000;
    //     FPGA_DDR32(0x048) = 0x00000000;
    //     FPGA_DDR32(0x050) = 0x01000202;
    //     FPGA_DDR32(0x054) = 0x00000000;
    //     FPGA_DDR32(0x058) = 0x00000000;
    //     FPGA_DDR32(0x060) = 0x01000202;
    //     FPGA_DDR32(0x064) = 0x00000000;
    //     FPGA_DDR32(0x068) = 0x00000000;
    //     FPGA_DDR32(0x070) = 0x01000202;
    //     FPGA_DDR32(0x074) = 0x00000000;
    //     FPGA_DDR32(0x078) = 0x00000000;

    //     /* 控制器 / PHY 基础配置 */
    //     FPGA_DDR32(0x080) = 0x00030200;
    //     FPGA_DDR32(0x084) = 0x00201010;
    //     FPGA_DDR32(0x088) = 0x11301522;
    //     FPGA_DDR32(0x08c) = 0x00000000;
    //     FPGA_DDR32(0x090) = 0x00000000;
    //     FPGA_DDR32(0x094) = 0x00000000;
    //     FPGA_DDR32(0x098) = 0x00000000;
    //     FPGA_DDR32(0x09c) = 0x04000000;
    //     FPGA_DDR32(0x0a0) = 0x00000000;
    //     FPGA_DDR32(0x0a4) = 0x00000800;
    //     FPGA_DDR32(0x0a8) = 0x00000000;
    //     FPGA_DDR32(0x0ac) = 0x00000000;
    //     FPGA_DDR32(0x0b0) = 0x00000000;
    //     FPGA_DDR32(0x0b4) = 0x00000000;
    //     FPGA_DDR32(0x0b8) = 0x00000000;
    //     FPGA_DDR32(0x0bc) = 0x0F00F040;
    //     FPGA_DDR32(0x0c0) = 0x00FF0000;
    //     FPGA_DDR32(0x0c4) = 0xFFFFFFFF;
    //     FPGA_DDR32(0x0c8) = 0xFFFFFFFF;
    //     FPGA_DDR32(0x0cc) = 0x00000000;
    //     FPGA_DDR32(0x0d4) = 0x00C90000;
    //     FPGA_DDR32(0x0d8) = 0x0000F800;
    //     FPGA_DDR32(0x0dc) = 0x3F3F3F3F;
    //     FPGA_DDR32(0x0e0) = 0x3F3F3F3F;
    //     FPGA_DDR32(0x0e4) = 0x00000000;
    //     FPGA_DDR32(0x0e8) = 0x00000000;
    //     FPGA_DDR32(0x0ec) = 0x00000000;
    //     FPGA_DDR32(0x0f0) = 0x00000000;
    //     FPGA_DDR32(0x0f4) = 0x00000000;

    //     /* 时序 / 训练参数 */
    //     FPGA_DDR32(0x180) = 0x000A000A;
    //     FPGA_DDR32(0x184) = 0x030B120A;
    //     FPGA_DDR32(0x188) = 0x00060507;
    //     FPGA_DDR32(0x18c) = 0x00010000;
    //     FPGA_DDR32(0x190) = 0x000A0402;
    //     FPGA_DDR32(0x194) = 0x000F0504;
    //     FPGA_DDR32(0x198) = 0x000000EA;
    //     FPGA_DDR32(0x19c) = 0x000000F0;
    //     FPGA_DDR32(0x1a0) = 0x0A0A020A;
    //     FPGA_DDR32(0x1a4) = 0x06B01416;
    //     FPGA_DDR32(0x1a8) = 0x01030202;
    //     FPGA_DDR32(0x1ac) = 0x06060505;
    //     FPGA_DDR32(0x1b0) = 0x00000008;
    //     FPGA_DDR32(0x1b4) = 0x00000000;
    //     FPGA_DDR32(0x1b8) = 0x00000000;
    //     FPGA_DDR32(0x1bc) = 0x0001F000;
    //     FPGA_DDR32(0x1c0) = 0x07010000;
    //     FPGA_DDR32(0x1c4) = 0x000FFF00;
    //     FPGA_DDR32(0x1c8) = 0x0003FF00;
    //     FPGA_DDR32(0x1cc) = 0x00000000;
    //     FPGA_DDR32(0x1d0) = 0x00000022;

    //     /* 训练：触发 DFI 训练，再按状态机逐级推进 */
    //     FPGA_DDR32(0x088) = 0x01301522;
    //     FPGA_DDR32(0x080) = 0x00330001;
    //     FPGA_DDR32(0x080) = 0x00330100;
    //     FPGA_DDR32(0x0d8) = 0x0000F800;
    //     FPGA_DDR32(0x0d8) = 0x0020f800;
    //     FPGA_DDR32(0x0d8) = 0x0020f802;
    //     FPGA_DDR32(0x0d8) = 0x0030f802;

    //     FPGA_DDR32(0x0fc) = 0x03000000;
    //     FPGA_DDR32(0x0f8) = 0x01000000;
    //     FPGA_DDR32(0x100) = 0x01000002;

    //     FPGA_DDR32(0x0fc) = 0x06000000;
    //     FPGA_DDR32(0x0f8) = 0x01000C00;
    //     FPGA_DDR32(0x100) = 0x01000002;

    //     FPGA_DDR32(0x0fc) = 0x05000000;
    //     FPGA_DDR32(0x0f8) = 0x01000400;
    //     FPGA_DDR32(0x100) = 0x01000002;

    //     FPGA_DDR32(0x0fc) = 0x04000000;
    //     FPGA_DDR32(0x0f8) = 0x01000000;
    //     FPGA_DDR32(0x100) = 0x01000002;

    //     FPGA_DDR32(0x0fc) = 0x02000000;
    //     FPGA_DDR32(0x0f8) = 0x01000020;
    //     FPGA_DDR32(0x100) = 0x01000002;

    //     FPGA_DDR32(0x0fc) = 0x01000000;
    //     FPGA_DDR32(0x0f8) = 0x01000001;
    //     FPGA_DDR32(0x100) = 0x01000002;

    //     FPGA_DDR32(0x0fc) = 0x00000000;
    //     FPGA_DDR32(0x0f8) = 0x01000A44;
    //     FPGA_DDR32(0x100) = 0x01000002;

    //     FPGA_DDR32(0x124) = 0x0000007F;
    //     FPGA_DDR32(0x0f8) = 0x01000E44;
    //     FPGA_DDR32(0x100) = 0x01000010;

    //     FPGA_DDR32(0x1cc) = 0x00001000;
    //     FPGA_DDR32(0x1b4) = 0x00000000;
    //     FPGA_DDR32(0x1c0) = 0x07010050;
    //     FPGA_DDR32(0x1bc) = 0x8001F000;
    //     FPGA_DDR32(0x0f4) = 0x00000080;
    //     FPGA_DDR32(0x084) = 0x00221010;
    //     FPGA_DDR32(0x084) = 0x00021010;
    //     FPGA_DDR32(0x088) = 0x00311642;
    //     FPGA_DDR32(0x098) = 0x00000000;
    //     FPGA_DDR32(0x09c) = 0x00200000;
    //     FPGA_DDR32(0x0a0) = 0x00000000;
    //     FPGA_DDR32(0x0a4) = 0x00000080;
    //     FPGA_DDR32(0x104) = 0x00000000;
    //     FPGA_DDR32(0x108) = 0x00000000;
    //     FPGA_DDR32(0x0c0) = 0x00FF0001;

    //     FPGA_DDR32(0x000) = 0x01000002;
    //     FPGA_DDR32(0x010) = 0x01000002;
    //     FPGA_DDR32(0x020) = 0x01000002;
    //     FPGA_DDR32(0x030) = 0x01000002;

    //     /* 关 PCIe2（与 DDR 共用 AXI，清掉避免抢带宽） */
    //     *(volatile uint32_t *)0x605005ccu = 0;
    //     *(volatile uint32_t *)0x605005d0u = 0;
    // }
#endif

static void flash_init(void)
{
    g_flash.reg = LSSSIV2;
    g_flash.slave_select = SLAVE1;
    HAL_SSIV2_Init(g_flash.reg, 4, 0);
    hal_flashx_software_reset_v2(g_flash.reg, g_flash.slave_select);
}

static void flash_read(uint32_t flash_offset, void *dst, uint32_t size_bytes)
{
    hal_flashx_fast_read_v2(g_flash.reg, g_flash.slave_select,
                            flash_offset, (uint16_t *)dst, size_bytes / 2);
}

#if FLASH_PROGRAM_MODE

/* NOR flash 页/块大小 */
#define FLASH_PAGE_SIZE   256
#define FLASH_BLOCK_SIZE  65536   /* 64KB，块擦除的最小粒度 */

/* 只擦除、不编程：按 64KB 块清空覆盖 [flash_offset, flash_offset+size_bytes) 的区域。
   擦完为 0xFF，供后续 flash_program() 写入。 */
static void flash_erase(uint32_t flash_offset, uint32_t size_bytes)
{
    uint32_t start = flash_offset & ~(FLASH_BLOCK_SIZE - 1u);
    uint32_t end = (flash_offset + size_bytes + FLASH_BLOCK_SIZE - 1u)
                   & ~(FLASH_BLOCK_SIZE - 1u);

    for (uint32_t a = start; a < end; a += FLASH_BLOCK_SIZE) {
        printk("1\n");
        hal_flashx_block_64K_erase_v2(g_flash.reg, g_flash.slave_select, a);
    }
}

/* 只编程、不擦除：把 src 处 size_bytes 字节按 256B 页写到 flash_offset。
   调用前须先 flash_erase() 清空目标区域。 */
static void flash_program(uint32_t flash_offset, const void *src, uint32_t size_bytes)
{
    const uint8_t *p = (const uint8_t *)src;
    uint32_t addr = flash_offset;
    uint32_t remaining = size_bytes;

    while (remaining) {
        uint32_t chunk = FLASH_PAGE_SIZE - (addr & (FLASH_PAGE_SIZE - 1u));
        if (chunk > remaining) {
            chunk = remaining;
        }
        hal_flashx_page_program_v2(g_flash.reg, g_flash.slave_select,
                                   addr, (uint8_t *)p, (uint16_t)chunk);
        addr += chunk;
        p += chunk;
        remaining -= chunk;
    }
}
#endif


void boot_c908(void)
{
    printk("step3 flash_read fwdyn/u-boot/zsb/image/busybox\n");
    flash_read(FWDYN_FLASH_OFFSET, (void *)FWDYN_LOAD_ADDR, FWDYN_SIZE_BYTES);
    printk("fw_dynamic.bin copy done\n");
    flash_read(DTB_FLASH_OFFSET, (void *)DTB_LOAD_ADDR, DTB_SIZE_BYTES);
    printk("u-boot.dtb copy done\n");
    flash_read(ZSB_FLASH_OFFSET, (void *)ZSB_LOAD_ADDR, ZSB_SIZE_BYTES);
    printk("zero_stage_boot.bin copy done\n");
    flash_read(IMAGE_FLASH_OFFSET, (void *)IMAGE_LOAD_ADDR, IMAGE_SIZE_BYTES);
    printk("image.bin copy done\n");
    flash_read(BUSYBOX_FLASH_OFFSET, (void *)BUSYBOX_LOAD_ADDR, BUSYBOX_SIZE_BYTES);
    printk("busybox.cpio copy done\n");

    // 4. 校验
    printk("step4 verify TODO\n");


    // 5. 释放C908复位；复位释放之前要设置c908启动地址；4个核的复位要一起释放，启动地址一样
    CPU0_SYS_CFG -> CPU_SYS_SOFT_CORE0_RVBA_L = 0xc03f8000;
    CPU0_SYS_CFG -> CPU_SYS_SOFT_CORE1_RVBA_L = 0xc03f8000;
    CPU0_SYS_CFG -> CPU_SYS_SOFT_CORE2_RVBA_L = 0xc03f8000;
    CPU0_SYS_CFG -> CPU_SYS_SOFT_CORE3_RVBA_L = 0xc03f8000;

    CPU0_SYS_CFG -> CPU_SYS_SOFT_APB_BASE_L = 0x0;
    CPU0_SYS_CFG -> CPU_SYS_SOFT_APB_BASE_H = 0x40;

    CPU0_SYS_CFG -> CPU1_SYS_CRG_CPU1_SYS_PLL_CPU_CLK_TOP_RST_N_REG0 =  CPU0_SYS_CFG -> CPU1_SYS_CRG_CPU1_SYS_PLL_CPU_CLK_TOP_RST_N_REG0 | (0x1 << 0);
    printk("C908 BOOT: axi_rst/culster rst release\n");  

    //PIC clk  400Mhz
    REG_FIELD_WR(CPU0_SYS_CFG -> CPU1_SYS_PIC_CLK_REG0, CPU0_SYS_CPU1_SYS_PIC_CLK_REG0, 0x1);
    //PIC rst_n
    CPU0_SYS_CFG -> CPU1_SYS_CRG_PIC_CLK_TOP_RST_N_REG0 = CPU0_SYS_CFG -> CPU1_SYS_CRG_PIC_CLK_TOP_RST_N_REG0 | (0x1 << 0);
    printk("C908 BOOT: pic rst_n release\n");  

    /** SETP5: <devif_rst_b> release */
    //devif clk: this clk is cpu1_sys apb_clk
    REG_FIELD_WR(CPU0_SYS_CFG -> CPU1_SYS_CFG_REG3 ,CPU1_CFG_C908_CACHE_APB_CLK_CLK_CG0_18, 0x1);

    CPU0_SYS_CFG -> CPU1_SYS_CRG_CPU1_SYS_APB_CLK_TOP_RST_N_REG0 = CPU0_SYS_CFG -> CPU1_SYS_CRG_CPU1_SYS_APB_CLK_TOP_RST_N_REG0 | (0x1 << 18);
    printk("C908 BOOT: devif rst_n release\n");

    CPU0_SYS_CFG -> IP_SOFT_RST_N_REG0  = CPU0_SYS_CFG -> IP_SOFT_RST_N_REG0 | (0x1 << 0);
    printk("C908 BOOT: pad rst_n release\n");

    CPU0_SYS_CFG -> IP_SOFT_RST_N_REG0  = CPU0_SYS_CFG -> IP_SOFT_RST_N_REG0 | (0x1 << 1) | (0x1 << 2) | (0x1 << 3) | (0x1 << 4);
    printk("C908 BOOT: core0~3 rst_n release\n");
}

static void c908_notify_rst_isr()
{
    uint32_t token = 1;
    printk("[irq] C908 关 notif 中断\n");
    CPU1_SYS_CFG -> CPU_SOFT1_C908_INTR = CPU1_SYS_CPU_SOFT1_C908_INTR_POS;
    printk("e906复位c908\n");
    CPU0_SYS_CFG -> CPU1_SYS_CRG_CPU1_SYS_PLL_CPU_CLK_TOP_RST_N_REG0_CLR = 0x1;
    //PIC rst_n	
    CPU0_SYS_CFG -> CPU1_SYS_CRG_PIC_CLK_TOP_RST_N_REG0_CLR = 0x1;
    //devif_rst_n
    CPU0_SYS_CFG -> CPU1_SYS_CRG_CPU1_SYS_APB_CLK_TOP_RST_N_REG0_CLR = (0x1 << 18);
    //PAD RST + CORE0~3 RST_N（与 boot_c908 释放时的 |= 0x1f 对称，4 个核都要拉低复位）
    CPU0_SYS_CFG -> IP_SOFT_RST_N_REG0_CLR  = 0x1f;
    (void)k_msgq_put(&rst_notify_msgq, &token, K_NO_WAIT);
}

static void c908_rst_irq_config(void)
{
    IRQ_CONNECT(CPU_SOFT1_C908_IRQn, 3, c908_notify_rst_isr, NULL, 0);
    irq_enable(CPU_SOFT1_C908_IRQn);
}

int main(void)
{
    uint32_t token;
    c908_rst_irq_config();
    HAL_IWDG_DeInit(IWDT0);
    /* 1. PLL 1G时钟 SRAM 400M*/
    printk("step1 pll clk cfg\n");
    // pll_clk_cfg();  //fpga 不生效
    flash_init();

#if FLASH_PROGRAM_MODE
    flash_erase(0x0, ZSB_FLASH_OFFSET + ZSB_SIZE_BYTES);
    flash_program(C908_BOOT_FLASH_OFFSET, _binary_c908_boot_bin_start, C908_BOOT_SIZE);
    flash_program(DTB_FLASH_OFFSET, _binary_u_boot_dtb_start, DTB_SIZE_BYTES);
    flash_program(ZSB_FLASH_OFFSET, _binary_zero_stage_boot_bin_start, ZSB_SIZE_BYTES);
    while(1);
#else
    // 开 DDR 时钟
    REG_FIELD_WR(CPU1_SYS_CFG -> DDR_SYS_CFG_REG1, DDR_SYS_APB_CLK_CLK_CG0_0, 0x1);
    REG_FIELD_WR(CPU1_SYS_CFG -> DDR_SYS_CFG_REG1, DDR_SYS_DDRPHY_PCLK_CLK_CG0_1, 0x1);
    REG_FIELD_WR(CPU1_SYS_CFG -> DDR_SYS_CFG_REG1, DDR_SYS_DDR_CFG_CLK_CLK_CG0_2, 0x1);
    REG_FIELD_WR(CPU1_SYS_CFG -> DDR_SYS_CFG_REG2, DDR_SYS_AXI_CLK_CLK_CG0_0, 0x1);
    REG_FIELD_WR(CPU1_SYS_CFG -> DDR_SYS_CFG_REG2, DDR_SYS_AXI_DMC_CLK_CLK_CG0_1, 0x1);
    //2. 初始化和训练
    printk("step2 ddr init\n");
#if DDR_KNL
    knl_ddr_init();
#else
    // fpga_ddr_init();
#endif

    token = 1;
    k_msgq_put(&rst_notify_msgq, &token, K_NO_WAIT);
    while (1) {
        printk("=== wait rst token ===\n");
        k_msgq_get(&rst_notify_msgq, &token, K_FOREVER);
        boot_c908();
        printk("boot done, back to wait\n");
    }
#endif
}