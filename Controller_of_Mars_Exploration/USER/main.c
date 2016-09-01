#include "controller.h"
#include "delay.h"
#include "usart1.h"

uint8_t a;

int main(void){
	delay_init(72);
	USART1_Config();
	Controller_Config();
	
	while(1){
		a = getButtonData();
		printf("%d %d wocaocao\r\n", a, isStickMode());
		if(isStickMode()){
			delay_ms(50);
			printf("%5d%5d%5d%5d\n",getStickData(PSS_LX),getStickData(PSS_LY),
														  getStickData(PSS_RX),getStickData(PSS_RY));			
		} else {
			a = getButtonData();
			delay_ms(50);
			printf("%5d\n",a);
			
			if(a == 11){
				PS2_Vibration(0xFF,0x00);  //发出震动后必须有延时
				delay_ms(1000);
			} else if(a == 12){
				PS2_Vibration(0x00,0xFF);  //发出震动后必须有延时
				delay_ms(1000);
			}
		}
	}
}
