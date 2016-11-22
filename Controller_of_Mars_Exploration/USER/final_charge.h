#ifndef _FINAL_CHARGE_H_
#define _FINAL_CHARGE_H_

#include "precompile.h"

#define MIDDLE_TRIGGER GPIO_Pin_7

#define TURN_PWM 60

#define INNER_TIME 100
#define FORWARD_I_TIME 700
#define OUTER_TIME_I 800
#define FORWARD_II_TIME 870

#define CORRECT_DIS 30000
#define INNER_DIS 15000
#define FORWARD_I_DIS 2500
#define FORWARD_II_DIS 4100

#define INTERVAL 85

void dirDef(uint8_t dir);
void turnInner(void);
void turnOuter(void);
void upwardSlope(void);
void Final_Charge (uint8_t _direction);

#endif
