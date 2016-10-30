/**
  ******************************************************************************
  * @author  			 Yuuki_Dach
  * @version 			 V1.1.0
  * @date          30-October-2016
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

#define FORWARDS  0
#define BACKWARDS 1
#define TURNLEFT  2
#define TURNRIGHT 3

#define RIGHT_TIRE_GPIO  GPIOD
#define LEFT_TIRE_GPIO   GPIOD

#define RIGHT_TIRE_CLK_GPIO  RCC_APB2Periph_GPIOD
#define LEFT_TIRE_CLK_GPIO   RCC_APB2Periph_GPIOD

#define LEFT_TIRE_ENA    4
#define LEFT_TIRE_IN1    GPIO_Pin_5
#define LEFT_TIRE_IN2    GPIO_Pin_4

#define RIGHT_TIRE_ENA   5
#define RIGHT_TIRE_IN1   GPIO_Pin_7
#define RIGHT_TIRE_IN2	 GPIO_Pin_6

void Tire_Config(void);
void setForwards(void);
void setBackwards(void);
void setTurnLeft(void);
void setTurnRight(void);
void setSpeed(int towards, int PWM_Setl, int PWM_Setr);
void stopTheCar(void);
void tireGoto(uint8_t direction);

#endif /* __MOVEMENT_H */

/******************* (C) COPYRIGHT 2016 Yuuki_Dach *************END OF FILE****/

