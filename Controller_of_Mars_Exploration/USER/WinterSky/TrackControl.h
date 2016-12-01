#ifndef _TRACKCONTROL_H_
#define _TRACKCONTROL_H_

#include <stm32f10x.h>

#define TRACK_RCC RCC_APB2Periph_GPIOA
#define TRACK_GPIO GPIOD
#define TRACK_L1 GPIO_Pin_11
#define TRACK_L2 GPIO_Pin_12
#define TRACK_L3 GPIO_Pin_13
#define TRACK_L4 GPIO_Pin_14

#define CHECK_RCC RCC_APB2Periph_GPIOE
#define CHECK_GPIO GPIOE
#define CHECK_L1 GPIO_Pin_11
#define CHECK_L2 GPIO_Pin_12
#define CHECK_L3 GPIO_Pin_13
#define CHECK_L4 GPIO_Pin_14

void TrackConfig(void);
u8 TrackCheck(void);
u8 getLeft(void);
u8 getRight(void);
u8 getBottom(void);
void Tracking(u8 _nD);


#endif
