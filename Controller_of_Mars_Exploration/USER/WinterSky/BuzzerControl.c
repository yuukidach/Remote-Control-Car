#include "BuzzerControl.h"


void BuzzerConfig(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(BUZZER_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BUZZER_GPIO, &GPIO_InitStructure);
}

void BuzzerOn(void) {
	GPIO_SetBits(BUZZER_GPIO, BUZZER_PIN);
}

void BuzzerOff(void) {
	GPIO_ResetBits(BUZZER_GPIO, BUZZER_PIN);
}

