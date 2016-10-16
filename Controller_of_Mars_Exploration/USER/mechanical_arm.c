/**
  ******************************************************************************
  * @author  			 Yuuki_Dach
  * @version 			 V1.0.1
  * @date          14-October-2016
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


/******************* (C) COPYRIGHT 2016 Yuuki_Dach *************END OF FILE****/
