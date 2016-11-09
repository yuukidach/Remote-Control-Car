#include "AutoControl.h"

#include "ADCConfig.h"
#include "WifiControl.h"
#include "delay.h"
#include "movement.h"
#include <stdio.h>


u8 bSeek = 1, bFind = 0;
s8 toward = 2, turn = 0;
u8 nAddCount = 0;
u16 adValue[10], front, back, all;


void AutoControlConfig(void) {
	RCC_APB2PeriphClockCmd(TRACK_RCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//配置GPIO为模拟输入
	GPIO_InitStructure.GPIO_Pin = TRACK_FL | TRACK_FR | TRACK_BL | TRACK_BR;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(TRACK_GPIO, &GPIO_InitStructure);
}


u8 TrackCheck(void) {
	u8 re = 0;
	re += GPIO_ReadInputDataBit(TRACK_GPIO, TRACK_FL)<<3;
	re += GPIO_ReadInputDataBit(TRACK_GPIO, TRACK_FR)<<2;
	re += GPIO_ReadInputDataBit(TRACK_GPIO, TRACK_BR)<<1;
	re += GPIO_ReadInputDataBit(TRACK_GPIO, TRACK_BL);
	return re;
}


void Tracking(u8 _toward) {
	switch(TrackCheck()) {
		case 0:
			setSpeed(_toward, 10, 10);
			break;
		
		case 1:	//to left
		case 4:
		case 6:
			setSpeed(2, _toward?25:20, _toward?20:25);
			break;
		
		case 2:	//to right
		case 8:
		case 9:
			setSpeed(3, _toward?20:25, _toward?25:20);
			break;
		
		case 5: //turn left
			setSpeed(2, 25, 25);
			break;
		
		case 10:	//turn right
			setSpeed(3, 25, 25);
			break;
		
		case 3:
		case 7:
		case 11:
			if(bSeek) {
				stopTheCar();
				bFind = 1;
			} else {
				setSpeed(0, 10, 10);
			}
			break;
		
		case 12:
		case 13:
		case 14:
			if(bSeek) {
				stopTheCar();
				bFind = 1;
			} else {
				setSpeed(0, 10, 10);
			}
			break;
			
		case 15:
			stopTheCar();
			break;
	}
}


void PreDeal(void) {
	u32 count = 0;
	while(count++ < 0x000fffff) {
		Tracking(1);
	}
}


void Finding(s8 _nToward, s8 _nTurn) {
#ifdef _DEBUG_MODE
	printf("pre seek\n");
	ADC_PrintValue();
#endif
	if(_nTurn < 0) {
#ifdef _DEBUG_MODE
		printf("seek left\n");
#endif
		setSpeed(2, 25, 25);
		while(AD_Value[4+5*_nToward]+AD_Value[3+5*_nToward] > AD_Value[1+5*_nToward]+AD_Value[0+5*_nToward]);
	} else {
#ifdef _DEBUG_MODE
		printf("seek right\n");
#endif
		setSpeed(3, 25, 25);
		while(AD_Value[4+5*_nToward]+AD_Value[3+5*_nToward] < AD_Value[1+5*_nToward]+AD_Value[0+5*_nToward]);
	}
	setSpeed(_nToward, 15, 15);
	
#ifdef _DEBUG_MODE
	printf("\nSEEKING\n");
#endif
	while(AD_Value[2+5*_nToward] > 500) {
#ifdef _DEBUG_MODE
		ADC_PrintValue();
#endif
		u8 bFlag = AD_Value[4+5*_nToward]+AD_Value[3+5*_nToward] > AD_Value[1+5*_nToward]+AD_Value[0+5*_nToward];
		if(_nToward) {
			setSpeed(1, 15+2*bFlag, 15-2*bFlag);
		} else {
			setSpeed(0, 15-2*bFlag, 15+2*bFlag);
		}
	}
#ifdef _DEBUG_MODE
	printf("\nSEEK OK\n");
#endif
	ADC_PrintValue();
	setSpeed(1-_nToward, 15, 15);
	delay_ms(800);
	setSpeed(2+(_nTurn>0), 25, 25);
	delay_ms(700);
}


void Seeking(void) {
	for(int i = 0; i < 10; i++)
		adValue[i] += AD_Value[i];
	if(nAddCount++ < 16) return;
	
	//数据预处理
	for(int i = 0; i < 5; i++) {
		adValue[i] = adValue[i]>>4;
		front += adValue[i];
	}
	for(int i = 5; i < 10; i++) {
		adValue[i] = adValue[i]>>4;
		back += adValue[i];
	}
	all = front + back;
	
	//数据分析
	if(all < 1000) {	//场地上没有灯亮着
		stopTheCar();
		toward = 2;
		turn = 0;
		
#ifdef _DEBUG_MODE
		printf("fuck\n");
#endif
	} else {
		if(front > back) {	//灯在前面
			setSpeed(0, 10, 10);
			delay_ms(10);
			toward = 0;
			
#ifdef _DEBUG_MODE
			printf("front light\n");
			ADC_PrintValue();
#endif
			
		} else {						//灯在后面
			setSpeed(1, 10, 10);
			delay_ms(10);
			toward = 1;
			
#ifdef _DEBUG_MODE
			printf("back light\n");
			ADC_PrintValue();
#endif
			
		}
		if(adValue[4+5*toward] > 800) {
			Finding(toward, -1);
		} else if(adValue[0+5*toward] > 800) {
			Finding(toward, 1);
		}
	}
	
	//数据清除
	for(int i = 0; i < 10; i++) {
		adValue[i] = 0;
	}
	front = back = 0;
	nAddCount = 0;
}


void AutoControl(void) {
	
}


