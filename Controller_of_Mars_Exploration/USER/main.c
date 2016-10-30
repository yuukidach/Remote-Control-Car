/**
  ******************************************************************************
  * @author  			 Yuuki_Dach
  * @version 			 V1.0.3
  * @date          01-September-2016
  * @description   Main.c
  ******************************************************************************
  * @attention
  *
  * THIS IS THE MAIN.C FILE IN THIS PROJECTS. IT CONTIANS MAIN FUNCTION. OUR CAR
	* NEED TO BE CONTROLLED BY OURSELVES AT FIRST. AFTER WE COMPLETE THE TASK
	* WE NEED TO DO. IT WILL BE AUTO-CONTROLLED AND DO THE REST WORK. AS FOR MORE
	* SPECIFIC DESCRIPTIONS OF THE TASKS, YOU CAN FIND THEM IN "´óÈüÕÂ³Ì.pdf".
  *
  * <h2><center>&copy; COPYRIGHT 2016 Yuuki_Dach</center></h2>
  ******************************************************************************
  */

#include "controller.h"
#include "delay.h"
#include "usart1.h"
#include "movement.h"
#include "mechanical_arm.h"

int main(void){
	delay_init(72);
	//USART1_Config();
	Controller_Config();
	Tire_Config();
  Arm_Config();
	while(1){
    if(!isAutoControl()){
      tireGoto(getButtonData());
    } else if (isAutoControl()){
      for(int i = 1; i <= 0x444; ++i)
      PCA9685_SetOutput(PCA_ADDRESS, 0, 0, i);
      delay_ms(1000);
      for(int i = 0x444; i > 0; --i)
      PCA9685_SetOutput(PCA_ADDRESS, 0, 0, i);
      delay_ms(1000); 
    }    
	}
}

/******************* (C) COPYRIGHT 2016 Yuuki_Dach *************END OF FILE****/

