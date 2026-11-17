#include "ddrmc_init.h"
//1333
//{{{
#define     D_DDRMC_DDR5_TMG0_1333        0x26EA0009  //t_rp             t_rpab                        
#define     D_DDRMC_DDR5_TMG1_1333        0x040B2409  //t_rtp    t_mrd   t_wr       t_rcd              
#define     D_DDRMC_DDR5_TMG2_1333        0x03450608  //t_rtw            t_cke      t_xp               
#define     D_DDRMC_DDR5_TMG3_1333        0x03010000  //t_wtw    t_rtr   t_wtw_cs   t_rtr_cs           
#define     D_DDRMC_DDR5_TMG4_1333        0x00160304  //t_wtr_cs t_wtr   t_rrd      t_ccd              
#define     D_DDRMC_DDR5_TMG5_1333        0x39D70204  //t_wtr_l          t_rrd_l    t_ccd_l            
//    `ifdef D_DDR_X8
#define     D_DDRMC_DDR5_TMG6_1333        0x4400010F  //t_rfcpb          t_rfc                         
//    `else
//#define     D_DDRMC_DDR5_TMG6_1333        0x440000C2  //t_rfcpb          t_rfc                         
//    `endif
#define     D_DDRMC_DDR5_TMG7_1333        0x10000200  //t_ccdmw          t_xsr                         
#define     D_DDRMC_DDR5_TMG8_1333        0x0C0C000C  //t_cksrx  t_cksre t_ppd      t_escke            
#define     D_DDRMC_DDR5_TMG9_1333        0x00001318  //t_fc     t_faw              t_ras              
#define    D_DDRMC_DDR5_TMG10_1333        0x00000202  //t_dllk                                         
#define    D_DDRMC_DDR5_TMG11_1333        0x090A0909  //dqs_oe   data_oe wdm_lat    wdata_lat          
#define    D_DDRMC_DDR5_TMG12_1333        0x0000000B  //                 data_ie_x8 data_ie_x16        
#define    D_DDRMC_DDR5_TMG13_1333        0x00000000
#define    D_DDRMC_DDR5_TMG14_1333        0x00000000
#define    D_DDRMC_DDR5_TMG15_1333        0x00000000
#define    D_DDRMC_DDR5_TMG16_1333        0x07010000
#define    D_DDRMC_DDR5_TMG17_1333        0x000fff00
#define    D_DDRMC_DDR5_TMG18_1333        0x0003ff00
#define    D_DDRMC_DDR5_TMG19_1333        0x00000000
#define    D_DDRMC_DDR5_TMG20_1333        0x00000022
#define    D_DDRMC_DDR5_TMG24_1333        0x01260000
#define    D_DDRMC_DDR5_TMG25_1333        0x45632061    
#define    D_DDRMC_DDR5_TMG26_1333        0x05200004            
#define    D_DDRMC_DDR5_TMG27_1333        0x08080600
#define    D_DDRMC_DDR5_TMG28_1333        0x08000080
#define    D_DDRMC_DDR5_TMG29_1333        0x00100634
#define    D_DDRMC_DDR5_TMG30_1333        0x08000000
#define    D_DDRMC_DDR5_TMG31_1333        0x00000400  
//}}}

