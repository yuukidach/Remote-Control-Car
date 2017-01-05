#include "tim.h"

uint8_t u8_tim_sta;
uint32_t u32_tim_uprising;
uint32_t * u32p_tim_count;

void TIM3_Init(void) {
    
    u8_tim_sta = FALSE;
    u32_tim_uprising = 0;
    u32p_tim_count = NULL;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    
    TIM_TimeBaseStructure.TIM_Period = 0xffff;
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1 ;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    TIM_ARRPreloadConfig(TIM3 , DISABLE); 
    TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_CC2, ENABLE);
    TIM_Cmd(TIM3, DISABLE);
}


void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
        if (!u8_tim_sta) {
            u32_tim_uprising = TIM_GetCapture2(TIM3);
            u8_tim_sta = TRUE ;
            TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);
        } else {
            if (u32p_tim_count)
                * u32p_tim_count = TIM_GetCapture2(TIM3) - u32_tim_uprising;
            u8_tim_sta = FALSE ;
            TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);
            TIM_Cmd(TIM3, DISABLE);
        }
    }
    
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        if (u32p_tim_count) {
            if (!u8_tim_sta)
                *u32p_tim_count = 0;
            else
                *u32p_tim_count = 1;
        }
        u8_tim_sta = FALSE;
        TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);
        TIM_Cmd(TIM3, DISABLE);
    }
}




