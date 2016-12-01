#include "GrayscaleControl.h"

#include "ADCConfig.h"
#include "Delay.h"
#include "BuzzerControl.h"

#include "mpu6050.h" 
#include "mpuiic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


u16 iGrayF_1 = 0, iGrayF_2, iGrayB_1 = 0, iGrayB_2;
u16 iGrayF = 0, iGrayB = 0;


void grayLoadValue(void) {
	float pYaw, lYaw;
	while(getYaw(&pYaw));
	
	BuzzerOn();
	delay_ms(200);
	BuzzerOff();
	
	u32 sumF = 0, sumB = 0;
	for(int i = 0; i < 1024; i++) {
		sumF += AD_Value[8];
		sumB += AD_Value[9];
		delay_ms(1);
	}
	iGrayF_1 = sumF >> 10;
	iGrayB_2 = sumB >> 10;
	
	BuzzerOn();
	delay_ms(100);
	BuzzerOff();
	delay_ms(200);
	BuzzerOn();
	delay_ms(100);
	BuzzerOff();
	
	while(lYaw > pYaw - 70 && lYaw < pYaw + 70) {
		getYaw(&lYaw);
	}
	
	BuzzerOn();
	delay_ms(200);
	BuzzerOff();
	
	delay_ms(1500);
	
	BuzzerOn();
	delay_ms(200);
	BuzzerOff();
	
	
	sumF = sumB = 0;
	for(int i = 0; i < 1024; i++) {
		sumF += AD_Value[8];
		sumB += AD_Value[9];
		delay_ms(1);
	}
	iGrayF_2 = sumF >> 10;
	iGrayB_1 = sumB >> 10;
	
	BuzzerOn();
	delay_ms(100);
	BuzzerOff();
	delay_ms(200);
	BuzzerOn();
	delay_ms(100);
	BuzzerOff();
	
	iGrayF = (iGrayF_1 + iGrayF_2)/2;
	iGrayB = (iGrayB_1 + iGrayB_2)/2;
	
}

