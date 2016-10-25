/**
  ******************************************************************************
  * @author  			 Yuuki_Dach
  * @version 			 V1.0.5
  * @date          25-October-2016
  * @description   Functions of our car's tires(motors).
  ******************************************************************************
  * @attention
  *
  * OUR CAR USES TIM3 TO OUTPUT PWM WAVE. AND THERE ARE ONLY TWO MOTORS IN OUR
	* CAR. THE CONTROLLERI IS PS2. INTHIS PROGRAM, WE USE IT'S BUTTONS TO CONTROLL
  * ITS DIRECTIONS.
  *
  * <h2><center>&copy; COPYRIGHT 2016 Yuuki_Dach</center></h2>
  ******************************************************************************
  */

#include "movement.h"
#include "controller.h"
#include "usart1.h"
#include "pca9685.h"
#include "mechanical_arm.h"

void Tire_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LEFT_TIRE_IN1 | LEFT_TIRE_IN2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init( LEFT_TIRE_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RIGHT_TIRE_IN1 | RIGHT_TIRE_IN2;
	GPIO_Init( RIGHT_TIRE_GPIO, &GPIO_InitStructure);
	
  Arm_Config();
 
//	TIM3_GPIO_Config();
//	TIM3_Mode_Config();
  
  stopTheCar();
}

/*
void TIM3_GPIO_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LEFT_TIRE_IN1 | LEFT_TIRE_IN2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init( LEFT_TIRE_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RIGHT_TIRE_IN1 | RIGHT_TIRE_IN2;
	GPIO_Init( RIGHT_TIRE_GPIO, &GPIO_InitStructure);
}


void TIM3_Mode_Config(void){
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
	
  TIM_TimeBaseStructure.TIM_Period = 999;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure);
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
  TIM_OC1Init( TIM3, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig( TIM3, TIM_OCPreload_Enable);
	
  TIM_OC2Init( TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig( TIM3, TIM_OCPreload_Enable);
	
  TIM_OC3Init( TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig( TIM3, TIM_OCPreload_Enable);
	
  TIM_OC4Init( TIM3, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig( TIM3, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig( TIM3, ENABLE);
  TIM_Cmd( TIM3, ENABLE);
}
*/

/**
 * @brief	Sets speed of motors
 * @param	towards: FORWARDS or BACKWARDS
 * @param	PWM_Set: The PWM of motors(from 0 to 100);
 * @retval	None
 */
void setSpeed(int towards, int PWM_Setl, int PWM_Setr){
  switch (towards) {
    case FORWARDS : setForwards();  break;
    case BACKWARDS: setBackwards(); break;
    case TURNLEFT : setTurnLeft();  break;
    case TURNRIGHT: setTurnRight(); break;
  }
  PCA9685_SetOutput(PCA_ADDRESS, LEFT_TIRE_ENA , 0, 0xfff*PWM_Setl/100);
  PCA9685_SetOutput(PCA_ADDRESS, RIGHT_TIRE_ENA, 0, 0xfff*PWM_Setr/100);
}


void setForwards(void) {
  GPIO_SetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN1);
  GPIO_SetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN1);
  GPIO_ResetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN2);
  GPIO_ResetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN2);
}


void setBackwards(void) {
  GPIO_SetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN2);
  GPIO_SetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN2);
  GPIO_ResetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN1);
  GPIO_ResetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN1);
}


void setTurnLeft(void) {
  GPIO_SetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN1);
  GPIO_SetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN2);
  GPIO_ResetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN2);
  GPIO_ResetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN1);
}


void setTurnRight(void) {
  GPIO_SetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN2);
  GPIO_SetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN1);
  GPIO_ResetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN1);
  GPIO_ResetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN2);
}


void stopTheCar(void) {
  GPIO_ResetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN1);
  GPIO_ResetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN1);
  GPIO_ResetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN2);
  GPIO_ResetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN2);
}


void tireGoto(uint8_t dir){
  switch (dir) {
    case PSB_PAD_UP   : setSpeed(FORWARDS , 70, 70); break;
    case PSB_PAD_DOWN : setSpeed(BACKWARDS, 70, 70); break;
    case PSB_PAD_RIGHT: setSpeed(TURNRIGHT, 70, 70); break;
    case PSB_PAD_LEFT : setSpeed(TURNLEFT , 70, 70); break;
    default: stopTheCar();
  }
}

/******************* (C) COPYRIGHT 2016 Yuuki_Dach *************END OF FILE****/

