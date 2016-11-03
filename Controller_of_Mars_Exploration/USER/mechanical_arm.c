/**
  ******************************************************************************
  * @author  			 Yuuki_Dach
  * @version 			 V1.0.1
  * @date          3-November-2016
  * @description   Functions of the mechanical arm
  ******************************************************************************
  * @attention
  *
  * SERVOS OF THE MECHANICAL ARM ARE CONTROLLED BY PCA9685.
  *
  * <h2><center>&copy; COPYRIGHT 2016 Yuuki_Dach</center></h2>
  ******************************************************************************
  */
	
#include "mechanical_arm.h"

void Arm_Config(void) {
  PCA9685_I2C_Init();
  
  PCA9685_Init_TypeDef PCA_InitStructure;
  
  PCA_InitStructure.Address = PCA_ADDRESS;
  PCA_InitStructure.InvOutputs = PCA9685_NotInvOutputs;
  PCA_InitStructure.OutputDriver = PCA9685_OutputDriver_TotemPole;
  PCA_InitStructure.OutputNotEn = PCA9685_OutputNotEn_0;
  PCA_InitStructure.PWMFrequency = PCA9685_Frequency_50Hz;
  
  PCA9685_Init(&PCA_InitStructure);
}


void putArmHigh(void) {
  PCA9685_SetOutput(PCA_ADDRESS, 0, 0, 98); 
  PCA9685_SetOutput(PCA_ADDRESS, 1, 0, 470); 
  PCA9685_SetOutput(PCA_ADDRESS, 2, 0, 511); 
}


void putArmLow(void) {
  PCA9685_SetOutput(PCA_ADDRESS, 0, 0, 420); 
  PCA9685_SetOutput(PCA_ADDRESS, 1, 0, 230); //add the pwm will put the arm lower
  PCA9685_SetOutput(PCA_ADDRESS, 2, 0, 511);
}


void handGrab(void) {
  PCA9685_SetOutput(PCA_ADDRESS, 3, 0, 125); 
}


void handOpen(void) {
  PCA9685_SetOutput(PCA_ADDRESS, 3, 0, 270);
}

void armControl(uint8_t cmd) {
  switch (cmd) {
    case PSB_TRIANGLE : putArmHigh(); break;
    case PSB_CROSS    : putArmLow() ; break;
    case PSB_R1       : handGrab()  ; break;
    case PSB_R2       : handOpen()  ; break;
  }
}
/******************* (C) COPYRIGHT 2016 Yuuki_Dach *************END OF FILE****/
