#ifndef _BUZZERCONTROL_H_
#define _BUZZERCONTROL_H_

#include <stm32f10x.h>

#define BUZZER_RCC RCC_APB2Periph_GPIOA
#define BUZZER_GPIO GPIOC
#define BUZZER_PIN GPIO_Pin_13

void BuzzerConfig(void);
void BuzzerOn(void);
void BuzzerOff(void);


#endif
