/**
  ******************************************************************************
  * @author  			 Yuuki_Dach
  * @version 			 V1.0.1
  * @date          14-October-2016
  * @description   Header file of movement.c 
  ******************************************************************************
  * @attention
  *
  * THERE ARE DEFINITIONS OF THE VALUES THAT THE MOVEMENT.C WILL NEED.
  *
  * <h2><center>&copy; COPYRIGHT 2016 Yuuki_Dach</center></h2>
  ******************************************************************************
  */
	
#ifndef __MOVEMENT_H
#define __MOVEMENT_H

#include "stm32f10x.h"
#include "delay.h"

#define RIGHT_TIRE_GPIO  GPIOA
#define LEFT_TIRE_GPIO   GPIOB

#define RIGHT_TIRE_ENA   GPIO_Pin_5
#define RIGHT_TIRE_IN1   GPIO_Pin_6
#define RIGHT_TIRE_IN2	 GPIO_Pin_7

#define LEFT_TIRE_ENA    GPIO_Pin_2
#define LEFT_TIRE_IN1    GPIO_Pin_0
#define LEFT_TIRE_IN2    GPIO_Pin_1

#define tireEnable(x,y)  GPIO_SetBits  (x, y)
#define tireDisable(x,y) GPIO_ResetBits(x, y)

void Tire_Config(void);
void TIM3_GPIO_Config(void);
void TIM3_Mode_Config(void);
void setSpeed(int L1, int L2, int R1, int R2);
void tireGoto(uint8_t direction);

#endif /* __MOVEMENT_H */

/******************* (C) COPYRIGHT 2016 Yuuki_Dach *************END OF FILE****/

