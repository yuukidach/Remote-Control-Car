#ifndef _AUTOCONTROL_H_
#define _AUTOCONTROL_H_

#include "stm32f10x.h"


#define TRACK_RCC RCC_APB2Periph_GPIOA
#define TRACK_GPIO GPIOD
#define TRACK_FL GPIO_Pin_13
#define TRACK_FR GPIO_Pin_12
#define TRACK_BL GPIO_Pin_14
#define TRACK_BR GPIO_Pin_11


void AutoControlConfig(void);
u8 TrackCheck(void);
void Tracking(u8 _toward);
void PreDeal(void);
void Finding(s8 _nToward, s8 _nTurn);
void Seeking(void);
void AutoControl(void);


#endif
