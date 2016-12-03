#ifndef _AUTOCONTROL_H_
#define _AUTOCONTROL_H_

#include "stm32f10x.h"

#define ONE 350
#define DISTANCE 2200
#define COLOR_F 900
#define COLOR_B 900

#define DS1 20
#define DS2 24
#define TS1 22
#define TS2 90


extern float yaw, dYaw, target;

void AutoControlConfig(void);
void PreDeal(void);
void Finding(u8 _nD, u8 _nT);
void Seeking(void);

void TaskStart(u16 nms);
u8 Tasking(void);
void TaskClose(void);

float AutoControl(void);


#endif
