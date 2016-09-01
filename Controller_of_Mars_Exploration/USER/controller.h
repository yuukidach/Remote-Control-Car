#ifndef __CONTROLLER_H
#define __CONTROLLER_H

#include "stm32f10x.h"

#define YES 1
#define NO 0

#define PS2_START 0
#define PS2_REQUEST_DATA 1

#define PS2_DI GPIO_Pin_12
#define PS2_DO GPIO_Pin_13
#define PS2_CS GPIO_Pin_14
#define PS2_CLK GPIO_Pin_15

#define PSB_SELECT      0
#define PSB_L3          1
#define PSB_R3          2
#define PSB_START       3
#define PSB_PAD_UP      4
#define PSB_PAD_RIGHT   5
#define PSB_PAD_DOWN    6
#define PSB_PAD_LEFT    7
#define PSB_L2          8
#define PSB_R2          9
#define PSB_L1          10
#define PSB_R1          11
#define PSB_GREEN       12
#define PSB_RED         13
#define PSB_BLUE        14
#define PSB_PINK        15

#define PSB_TRIANGLE    12
#define PSB_CIRCLE      13
#define PSB_CROSS       14
#define PSB_SQUARE      15

#define PSS_RX 5                //��ҡ��X������
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

#define PRESSED 0

void Controller_Config(void);
void sendCmd2PS2(uint8_t command);
void sendCmd2MCU(void);
uint8_t isStickMode(void);
uint8_t getStickData(uint8_t button);
uint8_t getButtonData(void);
void clrPS2Buff(void);
void shortPoll(void);
void enterSettings(void);
void turnOnModeChange(void);
void turnOnVibrationMode(void);
void saveChangesAndExit(void);
void PS2_Vibration(u8 motor1, u8 motor2);

#endif /*__CONTROLLER_H*/
