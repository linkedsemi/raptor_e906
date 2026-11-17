#pragma once
void ddrphy_train_soft_update_ac(void);
void ddrphy_train_soft_update_dq(void);
void ddrphy_train_ddr4(u32 fn);
void ddrphy_ddr4_train_read_training(u32 train_phy, u32 fn);
void ddrphy_train_ddr4_rd_pattern();
int ddrphy_train_ddr4_rdeye(u32 vref_scan, u32 train_phy,u32 fn);
void ddrphy_train_ddr4_rddsk(u32 train_phy);
void ddrphy_ddr4_train_write_training(u32 train_phy,u32 fn);
void ddrphy_train_ddr4_wr_pattern();
void ddrphy_train_ddr4_wreye(u32 vref_scan,u32 fn);
void ddrphy_train_ddr4_wrdsk();

void ddrphy_train_ddr3(u32 fn);
void ddrphy_ddr3_train_read_training();
void ddrphy_train_ddr3_rd_pattern();
int ddrphy_train_ddr3_rdeye(u32 vref_scan, u32 train_phy);

void ddrphy_ddr3_train_write_training();
void ddrphy_train_ddr3_wrdsk_pattern();
void ddrphy_train_ddr3_wreye_pattern();
void ddrphy_train_ddr3_wrdsk();
void ddrphy_train_ddr3_wreye(u32 vref_scan);
void ddrphy_train_wrlvl_training(u32 fn);
void ddrphy_train_coarse_wrlvl_training(u32 fn, u32 train_phy);
void ddrmc_auto_refresh_open();
int ddrphy_train_gate_training(u32 train_phy,u32 fn);
void ddrphy_train_rd_en_turning(u32 fn);
void ddrphy_train_wrlvl_config(u32 train_byte_sel, u32 restore_config , u32 fn);
void ddrphy_train_wrlvl_training_pro_max(u32 fn, u32 train_byte_sel);
void ddrphy_train_lpddr4_rd_pattern();
int  ddrphy_train_lpddr4_rdeye(u32 vref_scan_en,u32 train_phy,u32 fn);
void ddrphy_train_lpddr4_rddsk(u32 train_phy, u32 fn);
void ddrphy_lpddr4_train_write_training(u32 train_phy, u32 fn);
void ddrphy_train_lpddr4_wr_pattern();
void ddrphy_train_lpddr4_wreye(u32 vref_scan_en,u32 fn);
void ddrphy_train_lpddr4(u32 ddr_clk, u32 fn);
void ddrphy_lpddr4_train_read_training(u32 train_ddrphy,u32 fn);
void ddrphy_lpddr4_train_ca_training(u32 train_phy,u32 core_num, u32 ddr_clk, u32 fn);
void ddrphy_train_lpddr4_caeye(u32 vref_scan_en, u32 fn);
void ddrphy_train_lpddr4_cadsk(u32 fn);
void ddrphy_train_lpddr4_ca_pattern(u32 train_phy,u32 core_num);
void ddrphy_ca_cfg(u32 ddr_clk);

// 牛芯未提供
void ddrphy_train_ddr5_wr_pattern();
void ddrphy_train_ddr5_wrdsk();
void ddrphy_train_ddr5_wreye(u32 vref_scan,u32 fn);
void ddrphy_train_ddr5(u32 fn);
void ddrphy_ddr5_train_read_training(u32 train_phy, u32 fn);
void ddrphy_ddr5_train_write_training(u32 train_phy,u32 fn);
void ddrphy_train_ddr5_rd_pattern();
void ddrphy_train_ddr5_rddsk(u32 train_phy);
int ddrphy_train_ddr5_rdeye(u32 vref_scan, u32 train_phy, u32 fn);
