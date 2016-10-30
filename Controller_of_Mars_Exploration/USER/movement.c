/**
  ******************************************************************************
  * @author  			 Yuuki_Dach
  * @version 			 V1.1.0
  * @date          30-October-2016
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
  
  RCC_APB2PeriphClockCmd(LEFT_TIRE_CLK_GPIO | RIGHT_TIRE_CLK_GPIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LEFT_TIRE_IN1 | LEFT_TIRE_IN2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(LEFT_TIRE_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = RIGHT_TIRE_IN1 | RIGHT_TIRE_IN2;
	GPIO_Init(RIGHT_TIRE_GPIO, &GPIO_InitStructure);
	
  Arm_Config();
  
  stopTheCar();
}


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
  if (PWM_Setl <= 100 && PWM_Setr <= 100){
    PCA9685_SetOutput(PCA_ADDRESS, LEFT_TIRE_ENA , 0, 0xfff*PWM_Setl/100);
    PCA9685_SetOutput(PCA_ADDRESS, RIGHT_TIRE_ENA, 0, 0xfff*PWM_Setr/100);
  }
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
  GPIO_SetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN2);
  GPIO_SetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN1);
  GPIO_ResetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN1);
  GPIO_ResetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN2);
}


void setTurnRight(void) {
  GPIO_SetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN1);
  GPIO_SetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN2);
  GPIO_ResetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN2);
  GPIO_ResetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN1);
}


void stopTheCar(void) {
  GPIO_ResetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN1);
  GPIO_ResetBits(LEFT_TIRE_GPIO, LEFT_TIRE_IN2);
  GPIO_ResetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN1);
  GPIO_ResetBits(RIGHT_TIRE_GPIO, RIGHT_TIRE_IN2);
  PCA9685_SetOutput(PCA_ADDRESS, LEFT_TIRE_ENA , 0xfff, 0);
  PCA9685_SetOutput(PCA_ADDRESS, RIGHT_TIRE_ENA, 0xfff, 0);
}


void tireGoto(uint8_t dir){
  switch (dir) {
    case PSB_PAD_UP   : setSpeed(FORWARDS , 50, 50); break;
    case PSB_PAD_DOWN : setSpeed(BACKWARDS, 50, 50); break;
    case PSB_PAD_RIGHT: setSpeed(TURNRIGHT, 50, 50); break;
    case PSB_PAD_LEFT : setSpeed(TURNLEFT , 50, 50); break;
    default: stopTheCar();
  }
}

/******************* (C) COPYRIGHT 2016 Yuuki_Dach *************END OF FILE****/

