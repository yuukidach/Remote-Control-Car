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
	* WE NEED TO DO. IT WILL BE AUTO-CONTROLLED AND DO THE REST WORK. FOR MORE
	* SPECIFIC DESCRIPTIONS OF THE TASKS, YOU CAN FIND THEM IN "大赛章程.pdf".
  *
  * <h2><center>&copy; COPYRIGHT 2016 Yuuki_Dach</center></h2>
  ******************************************************************************
  */

#include "controller.h"
#include "delay.h"
#include "usart1.h"
#include "movement.h"
#include "mechanical_arm.h"

#define __ON__           1
#define __OFF__          0
#define __DEBUG__        (__OFF__)

uint8_t dir = PART3LEFT;

int main(void){
	delay_init(72);

	Controller_Config();
	Tire_Config();
  Arm_Config();
  
  #if(__DEBUG__ == __ON__)
    USART1_Config();
  #endif

  putArmHigh();

	while(1){
    if(!isAutoControl()){
      carGo(getButtonData());

      armControl(getButtonData());
  
      dir = getPart3Direction();
    } else if (isAutoControl()){
      delay_ms(1000);
    } 
	}
}

/******************* (C) COPYRIGHT 2016 Yuuki_Dach *************END OF FILE****/

