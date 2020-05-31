#include "key.h"
#include "delay.h"
#include "usart.h"

void Key_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = K1_Pin | K2_Pin | K3_Pin | K4_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int Key_Scan()
{
	int key_press_count = 0;
	
	if(KEY1 == 0) {
		delay_ms(50);
		while(KEY1 == 0) {
			++key_press_count;
			if(key_press_count > 10) {
				while(KEY1 == 0)
					delay_ms(20);
				key_press_count = 0;
//				printf("key 5 pressed\r\n");
				return 5;
			}
			else;
			delay_ms(20);
		}
//		printf("key 1 pressed\r\n");
		return 1;
	}
	
	else if(KEY2 == 0) {
		delay_ms(50);
		while(KEY2 == 0) {
			++key_press_count;
			if(key_press_count > 10) {
				while(KEY2 == 0)
					delay_ms(20);
				key_press_count = 0;
//				printf("key 6 pressed\r\n");
				return 6;
			}
			else;
			delay_ms(20);
		}
//		printf("key 2 pressed\r\n");
		return 2;
	}
	
	else if(KEY3 == 0) {
		delay_ms(50);
		while(KEY3 == 0)
			delay_ms(20);
//		printf("key 3 pressed\r\n");
		return 3;
	}
	
	else if(KEY4 == 0) {
		delay_ms(50);
		while(KEY4 == 0)
			delay_ms(20);
//		printf("key 4 pressed\r\n");
		return 4;
	}
	else
		return 0;
}

