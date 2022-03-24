
/********************************** (C) COPYRIGHT *******************************
* File Name		  : Main.C
* Author			 : WCH
* Version			: V1.1
* Date			   : 2017/11/18
* Description		: GPIO 设置与使用和GPIO中断使用示例   
*******************************************************************************/
#include "CH552.H" 
#include "Debug.H"

#include "GPIO.H"
#include "intrins.h"

#include "stdio.h"
#include <string.h>
#include "stdint.h"

#include "CDC.H"
#include "I2C.H"
#include "5351.H"

void configClk(){
	SAFE_MOD = 0x55;
	SAFE_MOD = 0xAA;
	CLOCK_CFG = CLOCK_CFG & ~ MASK_SYS_CK_SEL | 0x06;  // 24MHz
	SAFE_MOD = 0x00;
}

sbit LED = P3^4;

void main( ) 
{
	P3_MOD_OC = P3_MOD_OC | (1<<2);
  P3_DIR_PU = P3_DIR_PU &	~(1<<2);
	
	P3_MOD_OC = P3_MOD_OC | (1<<4);
  P3_DIR_PU = P3_DIR_PU &	~(1<<4);
	
	P1_MOD_OC = P3_MOD_OC | (1<<4);
  P1_DIR_PU = P3_DIR_PU &	~(1<<4);
	
	//Port3Cfg(2,2);
	//Port1Cfg(2,4);
	LED=0;
	SCL=1;
	SDA=1;
	
	mDelaymS(10);
	configClk();
	mDelaymS(10);
	
	mDelaymS(10);
	init5351();
	
	
	USBDeviceCfg();															   //设备模式配置
  USBDeviceEndPointCfg();																					//端点配置
  USBDeviceIntCfg();																						  //中断初始化
	UEP0_T_LEN = 0;
  UEP1_T_LEN = 0;																  //预使用发送长度一定要清空	
  UEP2_T_LEN = 0;
	
	LED=1;
	
	while(1){
		if(RXLEN>15){
			memcpy(processBuf,RecvBuf,16);
			LED=0;
			RXLEN=0;
			TXLEN=0;
			if(checkData()==1){
				
				calculatecfg();
				cfg5351();
				genData();
				
				TXLEN=16;
				
			}
			SendData();
			LED=1;
		}
	}
}