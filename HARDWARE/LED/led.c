#include "bsp.h"

void Led_PWM_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	//这里TIM3的通道1是GPIOA_Pin_6,通道2是GPIOA_Pin_7;通道3是GPIOB_Pin_0；
	//这里TIM3的通道4是GPIOB_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	//使用PWM的功能需要设置成AF_PP模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Led_Set_Status(uint8_t leds, enum led_status status)
{
	switch(leds) {
	case 1 :
		if(led_on == status)
			TIM_SetCompare1(TIM3, 0);
		else
			TIM_SetCompare1(TIM3, 100);
		break;

	case 2 :
		if(led_on == status)
			TIM_SetCompare2(TIM3, 0);
		else
			TIM_SetCompare2(TIM3, 100);
		break;

	case 3 :
		if(led_on == status)
			TIM_SetCompare3(TIM3, 0);
		else
			TIM_SetCompare3(TIM3, 100);
		break;

	case 4 :
		if(led_on == status)
			TIM_SetCompare4(TIM3, 0);
		else
			TIM_SetCompare4(TIM3, 100);
		break;
	
	default :
		break;
	}
}

void Led_Set_Luminance(uint8_t leds, uint8_t luminance)
{
	switch(leds) {
	case 1 :
		if(luminance >= 0 && luminance <= 100)
			TIM_SetCompare1(TIM3, 100 - luminance);
		else;
		
		break;

	case 2 :
		if(luminance >= 0 && luminance <= 100)
			TIM_SetCompare2(TIM3, 100 - luminance);
		else;
		
		break;
	case 3 :
		if(luminance >= 0 && luminance <= 100)
			TIM_SetCompare3(TIM3, 100 - luminance);
		else;
		
		break;

	case 4 :
		if(luminance >= 0 && luminance <= 100)
			TIM_SetCompare4(TIM3, 100 - luminance);
		else;
		
		break;
	
	default :
		break;
	
	}
}

void Led_Startup_1()
{
	int i = 0;	
	
	for(i = 0; i < 100; i++) {
		Led_Set_Luminance(1, 100 - i);
		Led_Set_Luminance(2, 100 - i);
		Led_Set_Luminance(3, 100 - i);
		Led_Set_Luminance(4, 100 - i);
		delay_ms(20);
	}
	
	Led_Set_Status(1, led_on);
	delay_ms(300);
	Led_Set_Status(2, led_on);
	delay_ms(300);
	Led_Set_Status(3, led_on);
	delay_ms(300);
	Led_Set_Status(4, led_on);
	delay_ms(300);
	
//	Led_Set_Status(1, led_on);
//	delay_ms(100);
//	Led_Set_Status(1, led_off);
//	Led_Set_Status(2, led_on);
//	delay_ms(100);
//	Led_Set_Status(2, led_off);
//	Led_Set_Status(3, led_on);
//	delay_ms(100);
//	Led_Set_Status(3, led_off);
//	Led_Set_Status(4, led_on);
//	delay_ms(100);
//	Led_Set_Status(4, led_off);
//	Led_Set_Status(3, led_on);
//	delay_ms(100);
//	Led_Set_Status(3, led_off);
//	Led_Set_Status(2, led_on);
//	delay_ms(100);
//	Led_Set_Status(2, led_off);
//	Led_Set_Status(1, led_on);
//	delay_ms(100);
	
}

void Led_Startup_2()
{
	int i = 0;	
	
//	for(i = 0; i < 100; i++) {
//		Led_Set_Luminance(1, i);
//		Led_Set_Luminance(2, i);
//		Led_Set_Luminance(3, i);
//		Led_Set_Luminance(4, i);
//		delay_ms(20);
//	}
	
//	Led_Set_Status(1, led_off);
//	delay_ms(100);
//	Led_Set_Status(1, led_on);
//	Led_Set_Status(2, led_off);
//	delay_ms(100);
//	Led_Set_Status(2, led_off);
//	Led_Set_Status(3, led_off);
//	delay_ms(100);
//	Led_Set_Status(3, led_on);
//	Led_Set_Status(4, led_off);
//	delay_ms(100);
//	Led_Set_Status(4, led_on);
//	Led_Set_Status(3, led_off);
//	delay_ms(100);
//	Led_Set_Status(3, led_on);
//	Led_Set_Status(2, led_off);
//	delay_ms(100);
//	Led_Set_Status(2, led_on);
//	Led_Set_Status(1, led_off);
//	delay_ms(100);
	for(i=0; i<5; i++)
	{
		Led_Set_Status(1, led_off);
		Led_Set_Status(2, led_off);
		Led_Set_Status(3, led_off);
		Led_Set_Status(4, led_off);
		delay_ms(200);
		Led_Set_Status(1, led_on);
		Led_Set_Status(2, led_on);
		Led_Set_Status(3, led_on);
		Led_Set_Status(4, led_on);
		delay_ms(200);
	}

		
}

void Led_Switch(uint8_t luminance, enum led_status led[])
{
	uint8_t i;
	for(i = 0; i < 4; i++) {
		if(led[i] == led_on)
			Led_Set_Luminance(i + 1, luminance);
		else
			Led_Set_Luminance(i + 1, 0);
	}
}

void Led_Effect_Type_1(uint8_t speed, uint8_t luminance)
{
	static uint8_t i = 1;
	if(speed) {
		Led_Set_Luminance(i, luminance);
		delay_ms(1000 - speed * 100);
		Led_Set_Luminance(i, 0);
		++i;
		if(i == 5)
			i = 1;
	}
	else
		Led_Set_Luminance(i, luminance);
}

void Led_Effect_Type_2(uint8_t speed, uint8_t luminance)
{
	static uint8_t i = 4;
	if(speed) {
		Led_Set_Luminance(i, luminance);
		delay_ms(1000 - speed * 100);
		Led_Set_Luminance(i, 0);
		--i;
		if(i == 0)
			i = 4;
	}
	else
		Led_Set_Luminance(i, luminance);
}

void Led_Effect_Type_3(uint8_t speed, uint8_t luminance)
{
	Led_Effect_Type_1(speed, luminance);
	Led_Effect_Type_2(speed, luminance);
}

void Led_Effect(uint8_t type, uint8_t speed, uint8_t luminance)
{
	if(speed > 9)
		speed = 9;
	else if(speed < 0)
		speed = 0;
	switch(type) {
		case 1:
			Led_Effect_Type_1(speed, luminance);
			break;
		case 2:
			Led_Effect_Type_2(speed, luminance);
			break;
		case 3:
			Led_Effect_Type_3(speed, luminance);
			break;
		default:
			break;
	}
}

