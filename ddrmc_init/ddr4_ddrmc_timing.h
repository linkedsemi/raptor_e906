#include "ddrmc_init.h"
/**************************************DDR4 DDRMC TMG**************************************/
#define DDRMC_DDR4_1600_MR0		0x0b44//CL=20
#define DDRMC_DDR4_1600_MR1		0x0001
#define DDRMC_DDR4_1600_MR2		0x0020 //0x0820//CWL=14
#define DDRMC_DDR4_1600_MR3	    0x0000 //0x0008 gear down
#define DDRMC_DDR4_1600_MR4	    0x0000
#define DDRMC_DDR4_1600_MR5	    0x0580
#define DDRMC_DDR4_1600_MR6	    0x0c00
#define DDRMC_DDR4_1600_MR22	0x0000
#define D_DDRMC_DDR4_TMG0_1600        0x000A000A  //t_rp             t_rpab                        
#define D_DDRMC_DDR4_TMG1_1600        0x030B120A  //t_rtp    t_mrd   t_wr       t_rcd              
#define D_DDRMC_DDR4_TMG2_1600        0x00060507  //t_rtw            t_cke      t_xp               
#define D_DDRMC_DDR4_TMG3_1600        0x00010000  //t_wtw    t_rtr   t_wtw_cs   t_rtr_cs           
#define D_DDRMC_DDR4_TMG4_1600        0x000A0402  //t_wtr_cs t_wtr   t_rrd      t_ccd              
#define D_DDRMC_DDR4_TMG5_1600        0x000F0504  //t_wtr_l          t_rrd_l    t_ccd_l            
#define D_DDRMC_DDR4_TMG6_1600        0x000000EA  //t_rfcpb          t_rfc                         
#define D_DDRMC_DDR4_TMG7_1600        0x000000F0  //t_ccdmw          t_xsr                         
#define D_DDRMC_DDR4_TMG8_1600        0x0A0A020A  //t_cksrx  t_cksre t_ppd      t_escke            
#define D_DDRMC_DDR4_TMG9_1600        0x06B01416  //t_fc     t_faw              t_ras              
#define D_DDRMC_DDR4_TMG10_1600       0x01030202  //t_dllk                                         
#define D_DDRMC_DDR4_TMG11_1600       0x06060505  //dqs_oe   data_oe wdm_lat    wdata_lat          
#define D_DDRMC_DDR4_TMG12_1600       0x00000008  //                 data_ie_x8 data_ie_x16        
#define D_DDRMC_DDR4_TMG13_1600       0x00000000
#define D_DDRMC_DDR4_TMG14_1600       0x00000000
#define D_DDRMC_DDR4_TMG15_1600       0x0001f000
#define D_DDRMC_DDR4_TMG16_1600       0x07010000
#define D_DDRMC_DDR4_TMG17_1600       0x000fff00
#define D_DDRMC_DDR4_TMG18_1600       0x0003ff00
#define D_DDRMC_DDR4_TMG19_1600       0x00000000
#define D_DDRMC_DDR4_TMG20_1600       0x00000022
#define D_DDRMC_DDR4_TMG24_1600       0x01260000
#define D_DDRMC_DDR4_TMG25_1600       0x45632061    
#define D_DDRMC_DDR4_TMG26_1600       0x00200C00            
#define D_DDRMC_DDR4_TMG27_1600       0x0a440001
#define D_DDRMC_DDR4_TMG28_1600       0x08000001
#define D_DDRMC_DDR4_TMG29_1600       0x00100634
#define D_DDRMC_DDR4_TMG30_1600       0x08000000
#define D_DDRMC_DDR4_TMG31_1600       0x00000400  

#define DDRMC_DDR4_1333_MR0		0x0b44//CL=20
#define DDRMC_DDR4_1333_MR1		0x0001
#define DDRMC_DDR4_1333_MR2		0x0020 //0x0820//CWL=14
#define DDRMC_DDR4_1333_MR3	    0x0000 //0x0008 gear down
#define DDRMC_DDR4_1333_MR4	    0x0000
#define DDRMC_DDR4_1333_MR5	    0x0580
#define DDRMC_DDR4_1333_MR6	    0x0c00
#define DDRMC_DDR4_1333_MR22	0x0000
#define D_DDRMC_DDR4_TMG0_1333        0x000A000A  //t_rp             t_rpab                        
#define D_DDRMC_DDR4_TMG1_1333        0x030B120A  //t_rtp    t_mrd   t_wr       t_rcd              
#define D_DDRMC_DDR4_TMG2_1333        0x00060507  //t_rtw            t_cke      t_xp               
#define D_DDRMC_DDR4_TMG3_1333        0x00010000  //t_wtw    t_rtr   t_wtw_cs   t_rtr_cs           
#define D_DDRMC_DDR4_TMG4_1333        0x000A0402  //t_wtr_cs t_wtr   t_rrd      t_ccd              
#define D_DDRMC_DDR4_TMG5_1333        0x000F0504  //t_wtr_l          t_rrd_l    t_ccd_l            
#define D_DDRMC_DDR4_TMG6_1333        0x000000EA  //t_rfcpb          t_rfc                         
#define D_DDRMC_DDR4_TMG7_1333        0x000000F0  //t_ccdmw          t_xsr                         
#define D_DDRMC_DDR4_TMG8_1333        0x0A0A020A  //t_cksrx  t_cksre t_ppd      t_escke            
#define D_DDRMC_DDR4_TMG9_1333        0x06B01416  //t_fc     t_faw              t_ras              
#define D_DDRMC_DDR4_TMG10_1333       0x01030202  //t_dllk                                         
#define D_DDRMC_DDR4_TMG11_1333       0x06060505  //dqs_oe   data_oe wdm_lat    wdata_lat          
#define D_DDRMC_DDR4_TMG12_1333       0x00000008  //                 data_ie_x8 data_ie_x16        
#define D_DDRMC_DDR4_TMG13_1333       0x00000000
#define D_DDRMC_DDR4_TMG14_1333       0x00000000
#define D_DDRMC_DDR4_TMG15_1333       0x0001f000
#define D_DDRMC_DDR4_TMG16_1333       0x07010000
#define D_DDRMC_DDR4_TMG17_1333       0x000fff00
#define D_DDRMC_DDR4_TMG18_1333       0x0003ff00
#define D_DDRMC_DDR4_TMG19_1333       0x00000000
#define D_DDRMC_DDR4_TMG20_1333       0x00000022
#define D_DDRMC_DDR4_TMG24_1333       0x01260000
#define D_DDRMC_DDR4_TMG25_1333       0x45632061    
#define D_DDRMC_DDR4_TMG26_1333       0x00200C00            
#define D_DDRMC_DDR4_TMG27_1333       0x0a440001
#define D_DDRMC_DDR4_TMG28_1333       0x08000001
#define D_DDRMC_DDR4_TMG29_1333       0x00100634
#define D_DDRMC_DDR4_TMG30_1333       0x08000000
#define D_DDRMC_DDR4_TMG31_1333       0x00000400  


#define DDRMC_DDR4_1200_MR0		0x0834//CL=18	
#define DDRMC_DDR4_1200_MR1		0x0001
#define DDRMC_DDR4_1200_MR2		0x0018//CWL=12
#define DDRMC_DDR4_1200_MR3	    0x0000
#define DDRMC_DDR4_1200_MR4	    0x0000
#define DDRMC_DDR4_1200_MR5	    0x0400
#define DDRMC_DDR4_1200_MR6	    0x0800
#define DDRMC_DDR4_1200_MR22	0x0000
#define  D_DDRMC_DDR4_TMG0_1200  0x00090009  //t_rp             t_rpab                
#define  D_DDRMC_DDR4_TMG1_1200  0x030B1009  //t_rtp    t_mrd   t_wr       t_rcd      
#define  D_DDRMC_DDR4_TMG2_1200  0x00050507  //t_rtw            t_cke      t_xp       
#define  D_DDRMC_DDR4_TMG3_1200  0x00010000  //t_wtw    t_rtr   t_wtw_cs   t_rtr_cs   
#define  D_DDRMC_DDR4_TMG4_1200  0x00090302  //t_wtr_cs t_wtr   t_rrd      t_ccd      
#define  D_DDRMC_DDR4_TMG5_1200  0x000d0403  //t_wtr_l          t_rrd_l    t_ccd_l    
#define  D_DDRMC_DDR4_TMG6_1200  0x00000172  //t_rfcpb          t_rfc                 
#define  D_DDRMC_DDR4_TMG7_1200  0x000001f0  //t_ccdmw          t_xsr                 
#define  D_DDRMC_DDR4_TMG8_1200  0x09090209  //t_cksrx  t_cksre t_ppd      t_escke    
#define  D_DDRMC_DDR4_TMG9_1200  0x06001214  //t_fc     t_faw              t_ras      
#define D_DDRMC_DDR4_TMG10_1200  0x000001c0  //t_dllk                                 
#define D_DDRMC_DDR4_TMG11_1200  0x05050404  //dqs_oe   data_oe wdm_lat    wdata_lat  
#define D_DDRMC_DDR4_TMG12_1200  0x00000007  //               data_ie_x8 data_ie_x16
#define D_DDRMC_DDR4_TMG13_1200  0x00000000
#define D_DDRMC_DDR4_TMG14_1200  0x00000000
#define D_DDRMC_DDR4_TMG15_1200  0x0001F000
#define D_DDRMC_DDR4_TMG16_1200  0x070100F0
#define D_DDRMC_DDR4_TMG17_1200  0x000fff00
#define D_DDRMC_DDR4_TMG18_1200  0x0003ff00
#define D_DDRMC_DDR4_TMG19_1200  0x00000000
#define D_DDRMC_DDR4_TMG20_1200  0x00000022

#define DDRMC_DDR4_1066_MR0		0x0334//CL=16
#define DDRMC_DDR4_1066_MR1		0x0001
#define DDRMC_DDR4_1066_MR2		0x0820//CWL=14
#define DDRMC_DDR4_1066_MR3	    0x0008
#define DDRMC_DDR4_1066_MR4	    0x0000
#define DDRMC_DDR4_1066_MR5	    0x0580
#define DDRMC_DDR4_1066_MR6	    0x040A
#define DDRMC_DDR4_1066_MR22	0x0000
#define  D_DDRMC_DDR4_TMG0_1066  0x00080008  //t_rp             t_rpab                
#define  D_DDRMC_DDR4_TMG1_1066  0x020B0E08  //t_rtp    t_mrd   t_wr       t_rcd      
#define  D_DDRMC_DDR4_TMG2_1066  0x00050406  //t_rtw            t_cke      t_xp       
#define  D_DDRMC_DDR4_TMG3_1066  0x00010000  //t_wtw    t_rtr   t_wtw_cs   t_rtr_cs   
#define  D_DDRMC_DDR4_TMG4_1066  0x00080302  //t_wtr_cs t_wtr   t_rrd      t_ccd      
#define  D_DDRMC_DDR4_TMG5_1066  0x000C0403  //t_wtr_l          t_rrd_l    t_ccd_l    
#define  D_DDRMC_DDR4_TMG6_1066  0x000000BB  //t_rfcpb          t_rfc                 
#define  D_DDRMC_DDR4_TMG7_1066  0x000000C0  //t_ccdmw          t_xsr                 
#define  D_DDRMC_DDR4_TMG8_1066  0x08080208  //t_cksrx  t_cksre t_ppd      t_escke    
#define  D_DDRMC_DDR4_TMG9_1066  0x05601012  //t_fc     t_faw              t_ras      
#define D_DDRMC_DDR4_TMG10_1066  0x00000126  //t_dllk                                 
#define D_DDRMC_DDR4_TMG11_1066  0x06060505  //dqs_oe   data_oe wdm_lat    wdata_lat  
#define D_DDRMC_DDR4_TMG12_1066  0x00000007  //               data_ie_x8 data_ie_x16
#define D_DDRMC_DDR4_TMG13_1066  0x00000000
#define D_DDRMC_DDR4_TMG14_1066  0x00000000
#define D_DDRMC_DDR4_TMG15_1066  0x00000000
#define D_DDRMC_DDR4_TMG16_1066  0x270100F0
#define D_DDRMC_DDR4_TMG17_1066  0x000fff00
#define D_DDRMC_DDR4_TMG18_1066  0x0003ff00
#define D_DDRMC_DDR4_TMG19_1066  0x00000000
#define D_DDRMC_DDR4_TMG20_1066  0x00000022

#define DDRMC_DDR4_933_MR0		0x0320//CL=14
#define DDRMC_DDR4_933_MR1		0x0001
#define DDRMC_DDR4_933_MR2		0x0808//CWL=10
#define DDRMC_DDR4_933_MR3		0x0008
#define DDRMC_DDR4_933_MR4		0x0000
#define DDRMC_DDR4_933_MR5		0x0580
#define DDRMC_DDR4_933_MR6		0x040A
#define DDRMC_DDR4_933_MR22		0x0000
#define  D_DDRMC_DDR4_TMG0_933  0x00070007  //t_rp             t_rpab                
#define  D_DDRMC_DDR4_TMG1_933  0x020B0D07  //t_rtp    t_mrd   t_wr       t_rcd      
#define  D_DDRMC_DDR4_TMG2_933  0x00040406  //t_rtw            t_cke      t_xp       
#define  D_DDRMC_DDR4_TMG3_933  0x00010000  //t_wtw    t_rtr   t_wtw_cs   t_rtr_cs   
#define  D_DDRMC_DDR4_TMG4_933  0x00080202  //t_wtr_cs t_wtr   t_rrd      t_ccd      
#define  D_DDRMC_DDR4_TMG5_933  0x000B0303  //t_wtr_l          t_rrd_l    t_ccd_l    
#define  D_DDRMC_DDR4_TMG6_933  0x000000A4  //t_rfcpb          t_rfc                 
#define  D_DDRMC_DDR4_TMG7_933  0x000000A8  //t_ccdmw          t_xsr                 
#define  D_DDRMC_DDR4_TMG8_933  0x07070207  //t_cksrx  t_cksre t_ppd      t_escke    
#define  D_DDRMC_DDR4_TMG9_933  0x04B00E0F  //t_fc     t_faw              t_ras      
#define D_DDRMC_DDR4_TMG10_933  0x00000126  //t_dllk                                 
#define D_DDRMC_DDR4_TMG11_933  0x04040303  //dqs_oe   data_oe wdm_lat    wdata_lat  
#define D_DDRMC_DDR4_TMG12_933  0x00000006  //                 data_ie_x8 data_ie_x16
#define D_DDRMC_DDR4_TMG13_933  0x00000000
#define D_DDRMC_DDR4_TMG14_933  0x00000000
#define D_DDRMC_DDR4_TMG15_933  0x00000000
#define D_DDRMC_DDR4_TMG16_933  0x270100F0
#define D_DDRMC_DDR4_TMG17_933  0x000fff00
#define D_DDRMC_DDR4_TMG18_933  0x0003ff00
#define D_DDRMC_DDR4_TMG19_933  0x00000000
#define D_DDRMC_DDR4_TMG20_933  0x00000022

#define DDRMC_DDR4_800_MR0		0x0b44//CL=20
#define DDRMC_DDR4_800_MR1		0x0001
#define DDRMC_DDR4_800_MR2		0x0020 //0x0820//CWL=14
#define DDRMC_DDR4_800_MR3	    0x0000 //0x0008 gear down
#define DDRMC_DDR4_800_MR4	    0x0000
#define DDRMC_DDR4_800_MR5	    0x0580
#define DDRMC_DDR4_800_MR6	    0x0c00
#define DDRMC_DDR4_800_MR22	0x0000
#define D_DDRMC_DDR4_TMG0_800        0x000A000A  //t_rp             t_rpab                        
#define D_DDRMC_DDR4_TMG1_800        0x030B120A  //t_rtp    t_mrd   t_wr       t_rcd              
#define D_DDRMC_DDR4_TMG2_800        0x00060507  //t_rtw            t_cke      t_xp               
#define D_DDRMC_DDR4_TMG3_800        0x00010000  //t_wtw    t_rtr   t_wtw_cs   t_rtr_cs           
#define D_DDRMC_DDR4_TMG4_800        0x000A0402  //t_wtr_cs t_wtr   t_rrd      t_ccd              
#define D_DDRMC_DDR4_TMG5_800        0x000F0504  //t_wtr_l          t_rrd_l    t_ccd_l            
#define D_DDRMC_DDR4_TMG6_800        0x000000EA  //t_rfcpb          t_rfc                         
#define D_DDRMC_DDR4_TMG7_800        0x000000F0  //t_ccdmw          t_xsr                         
#define D_DDRMC_DDR4_TMG8_800        0x0A0A020A  //t_cksrx  t_cksre t_ppd      t_escke            
#define D_DDRMC_DDR4_TMG9_800        0x06B01416  //t_fc     t_faw              t_ras              
#define D_DDRMC_DDR4_TMG10_800       0x01030202  //t_dllk                                         
#define D_DDRMC_DDR4_TMG11_800       0x06060505  //dqs_oe   data_oe wdm_lat    wdata_lat          
#define D_DDRMC_DDR4_TMG12_800       0x00000008  //                 data_ie_x8 data_ie_x16        
#define D_DDRMC_DDR4_TMG13_800       0x00000000
#define D_DDRMC_DDR4_TMG14_800       0x00000000
#define D_DDRMC_DDR4_TMG15_800       0x0001f000
#define D_DDRMC_DDR4_TMG16_800       0x07010000
#define D_DDRMC_DDR4_TMG17_800       0x000fff00
#define D_DDRMC_DDR4_TMG18_800       0x0003ff00
#define D_DDRMC_DDR4_TMG19_800       0x00000000
#define D_DDRMC_DDR4_TMG20_800       0x00000022
#define D_DDRMC_DDR4_TMG24_800       0x01260000
#define D_DDRMC_DDR4_TMG25_800       0x45632061    
#define D_DDRMC_DDR4_TMG26_800       0x00200C00            
#define D_DDRMC_DDR4_TMG27_800       0x0a440001
#define D_DDRMC_DDR4_TMG28_800       0x08000001
#define D_DDRMC_DDR4_TMG29_800       0x00100634
#define D_DDRMC_DDR4_TMG30_800       0x08000000
#define D_DDRMC_DDR4_TMG31_800       0x00000400  


#define DDRMC_DDR4_400_MR0		0x0310 //CL=10
#define DDRMC_DDR4_400_MR1		0x0001
#define DDRMC_DDR4_400_MR2		0x0000 //CWL=9
#define DDRMC_DDR4_400_MR3		0x0000
#define DDRMC_DDR4_400_MR4		0x0000
#define DDRMC_DDR4_400_MR5		0x0400
#define DDRMC_DDR4_400_MR6		0x0400
#define DDRMC_DDR4_400_MR22		0x0000
#define  D_DDRMC_DDR4_TMG0_400   0x00080009  //t_rp             t_rpab                
#define  D_DDRMC_DDR4_TMG1_400   0x01050a08  //t_rtp    t_mrd   t_wr       t_rcd      
#define  D_DDRMC_DDR4_TMG2_400   0x00060804  //t_rtw            t_cke      t_xp       
#define  D_DDRMC_DDR4_TMG3_400   0x00020000  //t_wtw    t_rtr   t_wtw_cs   t_rtr_cs   
#define  D_DDRMC_DDR4_TMG4_400   0x00070402  //t_wtr_cs t_wtr   t_rrd      t_ccd      
#define  D_DDRMC_DDR4_TMG5_400   0x00000000  //t_wtr_l          t_rrd_l    t_ccd_l    
#define  D_DDRMC_DDR4_TMG6_400   0x00240054  //t_rfcpb          t_rfc                 
#define  D_DDRMC_DDR4_TMG7_400   0x00100087  //t_ccdmw          t_xsr                 
#define  D_DDRMC_DDR4_TMG8_400   0x03030203  //t_cksrx  t_cksre t_ppd      t_escke    
#define  D_DDRMC_DDR4_TMG9_400   0x00001411  //t_fc     t_faw              t_ras      
#define D_DDRMC_DDR4_TMG10_400   0x00000000  //t_dllk                                 
#define D_DDRMC_DDR4_TMG11_400   0x03030202  //dqs_oe   data_oe wdm_lat    wdata_lat  
#define D_DDRMC_DDR4_TMG12_400   0x00000005  //                 data_ie_x8 data_ie_x16
#define D_DDRMC_DDR4_TMG13_400   0x00000000
#define D_DDRMC_DDR4_TMG14_400   0x00000000
#define D_DDRMC_DDR4_TMG15_400   0x00000000
#define D_DDRMC_DDR4_TMG16_400   0x00010080
#define D_DDRMC_DDR4_TMG17_400   0x00000100
#define D_DDRMC_DDR4_TMG18_400   0x00000100
#define D_DDRMC_DDR4_TMG19_400   0x00000000
#define D_DDRMC_DDR4_TMG20_400   0x00000022

#define DDRMC_DDR4_200_MR0		0x00
#define DDRMC_DDR4_200_MR1		0x00
#define DDRMC_DDR4_200_MR2		0x00
#define DDRMC_DDR4_200_MR3		0x00
#define DDRMC_DDR4_200_MR4		0x00
#define DDRMC_DDR4_200_MR5		0x00
#define DDRMC_DDR4_200_MR6		0x00
#define DDRMC_DDR4_200_MR22		0x00
#define  D_DDRMC_DDR4_TMG0_200   0x00050006  //t_rp             t_rpab                
#define  D_DDRMC_DDR4_TMG1_200   0x00040705  //t_rtp    t_mrd   t_wr       t_rcd      
#define  D_DDRMC_DDR4_TMG2_200   0x00040203  //t_rtw            t_cke      t_xp       
#define  D_DDRMC_DDR4_TMG3_200   0x00020000  //t_wtw    t_rtr   t_wtw_cs   t_rtr_cs   
#define  D_DDRMC_DDR4_TMG4_200   0x00050302  //t_wtr_cs t_wtr   t_rrd      t_ccd      
#define  D_DDRMC_DDR4_TMG5_200   0x00040000  //t_wtr_l          t_rrd_l    t_ccd_l    
#define  D_DDRMC_DDR4_TMG6_200   0x00180038  //t_rfcpb          t_rfc                 
#define  D_DDRMC_DDR4_TMG7_200   0x0000003B  //t_ccdmw          t_xsr                 
#define  D_DDRMC_DDR4_TMG8_200   0x04040204  //t_cksrx  t_cksre t_ppd      t_escke    
#define  D_DDRMC_DDR4_TMG9_200   0x03600E0C  //t_fc     t_faw              t_ras      
#define D_DDRMC_DDR4_TMG10_200   0x00000000  //t_dllk                                 
#define D_DDRMC_DDR4_TMG11_200   0x01010000  //dqs_oe   data_oe wdm_lat    wdata_lat  
#define D_DDRMC_DDR4_TMG12_200   0x00000004  //                 data_ie_x8 data_ie_x16
#define D_DDRMC_DDR4_TMG13_200   0x00000000
#define D_DDRMC_DDR4_TMG14_200   0x00000000
#define D_DDRMC_DDR4_TMG15_200   0x00000000
#define D_DDRMC_DDR4_TMG16_200   0x00010080
#define D_DDRMC_DDR4_TMG17_200   0x00000400
#define D_DDRMC_DDR4_TMG18_200   0x00001000
#define D_DDRMC_DDR4_TMG19_200   0x00000000
#define D_DDRMC_DDR4_TMG20_200   0x00000022


