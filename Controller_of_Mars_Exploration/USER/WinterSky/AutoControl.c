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

//#define _DEBUG_MODE


float yaw = 0, dYaw = 0, target;	//欧拉角

u8 bStrat = 0, bSeek = 0, bFind, bPreEnd = 0;
u8 nToward = 2, nTurn = 2;

u8 nGrayF = 0, nGrayB = 0;

void AutoControlConfig(void) {
	TrackConfig();
	BuzzerConfig();
}


#define iInfrared    1000
#define iInfraredOut 3000
#define iInfraredIn  1000

void LightCheck(void) {
	u8 nF = 0, nB = 0;
	for(int i = 0; i < 4; i++) {
		if(AD_Value[i] > iInfrared) nF++;
	}
	for(int i = 4; i < 8; i++) {
		if(AD_Value[i] > iInfrared) nB++;
	}
	if(nF*nB > 0) {
		if(AD_Value[0] > iInfraredOut || AD_Value[3] > iInfraredOut) {
			nToward = 0;
		}
		if(AD_Value[4] > iInfraredOut || AD_Value[7] > iInfraredOut) {
			nToward = 1;
		}
	} else if(nF > 0) {
		nToward = 0;
	} else if(nB > 0) {
		nToward = 1;
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
	
	TaskStart(450);
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
	
    #define dAngleOL 8
    #define dAngleOR 8
	#define dAngleIL 6
	#define dAngleIR 6
	
	switch(_nT) {
		case 1:
			target = dYaw + 90 - dAngleOL;
			while(getYaw(&yaw));
			while(yaw < target) {
				getYaw(&yaw);
			}
			break;
			
		case 0:
			target = dYaw - 90 + dAngleOR;
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
	
	u8 bTimeOut = 0;
	TaskStart(600);
	while(!getBottom()) {
		if(!Tasking()) {
			bTimeOut = 1;
			break;
		}
	}
	TaskClose();
	
	setSpeed(2+_nT, TS2, TS2);
	
	switch(_nT) {
		case 1:
			target = dYaw + dAngleIR;
			while(getYaw(&yaw));
			while(yaw > target) {
				getYaw(&yaw);
			}
			break;
		
		case 0:
			target = dYaw - dAngleIL;
			while(getYaw(&yaw));
			while(yaw < target) {
				getYaw(&yaw);
			}
			break;
	}
	
	if(bTimeOut) {
		
	}
	
	stopTheCar();
	bFind = 0;
	
#ifdef _DEBUG_MODE
	printf("light is finded\n");
#endif
	
	nToward = nTurn = 2;
	nGrayF = nGrayB = 0;
	
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
		
#ifdef _DEBUG_MODE
		printf(":NO LIGHT\n");
		ADC_PrintValue();
#endif
		
		if(!bPreEnd && nGrayF > 10) {
			bPreEnd = 1;
            TaskStart(1500);
		}
		if(bPreEnd && (AD_Value[8] < iGrayF || !Tasking())) {
            bSeek = 0;
            TaskClose();
		}
		
		return;
	}
	bPreEnd = 0;
	
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
		if(AD_Value[4] > 1000 && AD_Value[4] > AD_Value[7]) {
			bFind = 1;
			nTurn = 0;
			setSpeed(0, DS2, DS2);
			delay_ms(50);
		} else if(AD_Value[7] > 1000 && AD_Value[7] > AD_Value[4]) {
			bFind = 1;
			nTurn = 1;
			setSpeed(0, DS2, DS2);
			delay_ms(50);
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
