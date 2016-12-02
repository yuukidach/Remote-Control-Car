#include "TrackControl.h"

#include "movement.h"
#include "AutoControl.h"

#include "mpu6050.h" 
#include "mpuiic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

void TrackConfig(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(TRACK_RCC, ENABLE);
	//≈‰÷√GPIOŒ™∏°ø’ ‰»Î
	GPIO_InitStructure.GPIO_Pin = TRACK_L1 | TRACK_L2 | TRACK_L3 | TRACK_L4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(TRACK_GPIO, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(CHECK_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = CHECK_L1 | CHECK_L2 | CHECK_L3 | CHECK_L4;
	GPIO_Init(CHECK_GPIO, &GPIO_InitStructure);
}


u8 TrackCheck(void) {
	u8 re = 0;
	re += GPIO_ReadInputDataBit(TRACK_GPIO, TRACK_L1);
	re += GPIO_ReadInputDataBit(TRACK_GPIO, TRACK_L4)<<1;
	re += GPIO_ReadInputDataBit(TRACK_GPIO, TRACK_L3)<<2;
	re += GPIO_ReadInputDataBit(TRACK_GPIO, TRACK_L2)<<3;
	return re;
}


u8 getLeft(void) {
	return 1 - GPIO_ReadInputDataBit(CHECK_GPIO, CHECK_L1);
}


u8 getRight(void) {
	return 1 - GPIO_ReadInputDataBit(CHECK_GPIO, CHECK_L4);
}


u8 getBottom(void) {
	return GPIO_ReadInputDataBit(CHECK_GPIO, CHECK_L2);
}


u8 bYawCalibration = 0;

void Tracking(u8 _nD) {
	if(_nD) {
		switch(TrackCheck()) {
			case 0:
				setSpeed(1, DS2, DS2);
				bYawCalibration = 0;
				break;
			
			case 4:
				setSpeed(2, TS2, TS2);
				bYawCalibration = 0;
				break;
			
			case 6:
				if(!bYawCalibration) {
					stopTheCar();
					while(getYaw(&dYaw));
					bYawCalibration = 1;
				}
				setSpeed(2, TS2, TS2);
				break;
			
			case 8:
				setSpeed(3, TS2, TS2);
				bYawCalibration = 0;
				break;
			
			case 9:
				if(!bYawCalibration) {
					stopTheCar();
					while(getYaw(&dYaw));
					bYawCalibration = 1;
				}
				setSpeed(3, TS2, TS2);
				break;
			
			default:
				setSpeed(1, DS2, DS2);
				bYawCalibration = 0;
				break;
		}
	} else {
		switch(TrackCheck()) {
			case 0:
				setSpeed(0, DS2, DS2);
				bYawCalibration = 0;
				break;
			
			case 1:
				setSpeed(2, TS2, TS2);
				bYawCalibration = 0;
				break;
			
			case 9:
				if(!bYawCalibration) {
					stopTheCar();
					while(getYaw(&dYaw));
					bYawCalibration = 1;
				}
				setSpeed(2, TS2, TS2);
				break;
			
			case 2:
				setSpeed(3, TS2, TS2);
				bYawCalibration = 0;
				break;
			
			case 6:
				if(!bYawCalibration) {
					stopTheCar();
					while(getYaw(&dYaw));
					bYawCalibration = 1;
				}
				setSpeed(3, TS2, TS2);
				break;
			
			default:
				setSpeed(0, DS2, DS2);
				bYawCalibration = 0;
				break;
		}
	}
}
