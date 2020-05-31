#include "bsp.h"

void bsp_init()
{
        
	delay_init();	    	 															
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 	
	Adc_Init();
	uart_init(115200);																
	uart2_init(115200);
	TIM2_Int_Init(99,719);
	TIM3_PWM_Init();
	OLED_IIC_GPIO_Init();
	MPU6050_IIC_GPIO_Init();
	Led_PWM_GPIO_Init();
	Key_GPIO_Init();
	ESP_GPIO_Init();
	while(!MPU_init());
	GUI_Init();
	
}



