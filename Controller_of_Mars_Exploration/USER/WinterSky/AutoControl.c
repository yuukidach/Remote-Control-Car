#include "AutoControl.h"

#include "ADCConfig.h"
#include "TrackControl.h"
#include "delay.h"
#include "movement.h"
#include "ultrasonic.h"
#include "BuzzerControl.h"
#include "GrayscaleControl.h"
#include <stdio.h>

#include "mpu6050.h" 
#include "mpuiic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


float yaw = 0, dYaw = 0, target;	//欧拉角

u8 bStrat = 0, bSeek = 0, bFind;
u8 nToward = 2, nTurn = 2;

u8 nGrayF = 0, nGrayB = 0;

void AutoControlConfig(void) {
	TrackConfig();
	BuzzerConfig();
}


void LightCheck(void) {
	int flag = 0;
	for(int i = 0; i < 8; i++) {
		if(AD_Value[i] > ONE) flag++;
	}
	if(!flag) {
		nToward = 2;
		nTurn = 2;
		return;
	}
	
	u16 front = AD_Value[0] + AD_Value[1] + AD_Value[2] + AD_Value[3];
	u16 back = AD_Value[4] + AD_Value[5] + AD_Value[6] + AD_Value[7];
	
	if(AD_Value[0] + AD_Value[3] + AD_Value[4] + AD_Value[7] > 1000) {
		if(front > back + 500) {
			nToward = 0;
		} else if(back > front + 1000) {
			nToward = 1;
		}
	} else {
		if(front > back) {
			nToward = 0;
		} else if(back > front) {
			nToward = 1;
		}
	}
}


u8 bTaskOn = 0;

void TaskStart(u16 nms) {
	if(nms > 1800) nms = 1800;
	
	SysTick->LOAD=(u32)nms*9000;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01 ;          //开始倒数
	
	bTaskOn = 1;
}


u8 Tasking(void) {
	u32 temp=SysTick->CTRL;
	u8 re = temp&0x01&&!(temp&(1<<16));
	if(!re) {
		SysTick->CTRL=0x00;       //关闭计数器
		SysTick->VAL =0X00;       //清空计数器
		bTaskOn = 0;
	}
	
	return re;
}

void TaskClose(void) {
	if(bTaskOn) {
		SysTick->CTRL=0x00;       //关闭计数器
		SysTick->VAL =0X00;       //清空计数器
		bTaskOn = 0;
	}
}


void PreDeal(void) {
	
	stopTheCar();
	while(getYaw(&dYaw));
	
	TaskStart(400);
	while(Tasking()) {
		Tracking(0);
		LightCheck();
	}
	TaskClose();
	
	stopTheCar();
	delay_ms(50);
}


void Finding(u8 _nD, u8 _nT) {
#ifdef _DEBUG_MODE
	printf(":light is seeked\n");
#endif
	
	stopTheCar();
	delay_ms(50);
	setSpeed(3-_nT, TS2, TS2);
	
	#define dAngle 7
	
	switch(_nT) {
		case 1:
			target = dYaw + 90 - 10;
			while(getYaw(&yaw));
			while(yaw < target) {
				getYaw(&yaw);
			}
			break;
			
		case 0:
			target = dYaw - 90 + 10;
			while(getYaw(&yaw));
			while(yaw > target) {
				getYaw(&yaw);
			}
			break;
	}
	
	setSpeed(1, DS1, DS1);
	delay_ms(600);
    setSpeed(1, 16, 16);
    delay_ms(200);
	
	setSpeed(0, DS1, DS1);
	delay_ms(200);
	while(!getBottom());
	
	setSpeed(2+_nT, TS2, TS2);
	
	switch(_nT) {
		case 1:
			target = dYaw + dAngle;
			while(getYaw(&yaw));
			while(yaw > target) {
				getYaw(&yaw);
			}
			break;
		
		case 0:
			target = dYaw - dAngle;
			while(getYaw(&yaw));
			while(yaw < target) {
				getYaw(&yaw);
			}
			break;
	}
	
	stopTheCar();
	bFind = 0;
	
#ifdef _DEBUG_MODE
	printf("light is finded\n");
#endif
	
	delay_ms(50);
}


void Seeking(void) {
	LightCheck();
	
	if(AD_Value[8] > iGrayF) {
		if(nGrayF < 20) nGrayF++;
	} else {
		nGrayF = 0;
	}
	if(AD_Value[9] > iGrayB) {
		if(nGrayB < 20) nGrayB++;
	} else {
		nGrayB = 0;
	}
		
	if(nToward == 2) {
		Tracking(0);
		u32 iDistance = multiTrig(GPIO_Pin_7);
		
#ifdef _DEBUG_MODE
		printf(":NO LIGHT\n");
		ADC_PrintValue();
		printf("::DISTANCE %u\n", iDistance);
#endif
		
		if(nGrayF > 10) {
			bSeek = 0;
			while(AD_Value[8] > 900);
		}
		
		return;
	}
	
	Tracking(nToward);
	
#ifdef _DEBUG_MODE
	if(nToward)
		printf(":BACK\n");
	else
		printf(":FRONT\n");
    ADC_PrintValue();
#endif
	
	if(getLeft()) {
		stopTheCar();
		delay_ms(5);
		if(getLeft()) {
			bFind = 1;
			nTurn = 0;
		}
	} else if(getRight()) {
		stopTheCar();
		delay_ms(5);
		if(getRight()) {
			bFind = 1;
			nTurn = 1;
		}
	}
	
	if(nToward == 0 && nGrayF > 5) {
		nGrayF = 0;
		if(AD_Value[0] > 1000 && AD_Value[0] > AD_Value[3]) {
			bFind = 1;
			nTurn = 1;
			setSpeed(1, DS2, DS2);
			delay_ms(100);
		} else if(AD_Value[3] > 1000 && AD_Value[3] > AD_Value[0]) {
			bFind = 1;
			nTurn = 0;
			setSpeed(1, DS2, DS2);
			delay_ms(100);
		}
	} else if(nToward == 1 && nGrayB > 5) {
		nGrayB = 0;
		if(AD_Value[4] > 1000 && AD_Value[4] > AD_Value[7]) {
			bFind = 1;
			nTurn = 0;
			setSpeed(1, DS2, DS2);
			delay_ms(10);
		} else if(AD_Value[7] > 1000 && AD_Value[7] > AD_Value[4]) {
			bFind = 1;
			nTurn = 1;
			setSpeed(1, DS2, DS2);
			delay_ms(10);
		}
	}
	
	if(bFind)
		Finding(nToward, nTurn);
}


float AutoControl(void) {
	if(!bStrat) {
		PreDeal();
		bStrat = 1;
		bSeek = 1;
	}
	while(bSeek) {
		Seeking();
	}
	
#ifdef _DEBUG_MODE
	printf("part two is ended\n");
#endif
	
	return dYaw;
}
