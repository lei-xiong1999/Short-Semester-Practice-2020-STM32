#include "app.h"

long timestamp = 0;

void TIM3_PWM_Init(void)
{
        TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;//TIM定时器的配置
        TIM_OCInitTypeDef        TIM_OCInitStructure;//TIMPWM的
        
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
        //使能TIM3的时钟线;
        
        //复位定时器
        TIM_DeInit(TIM3);
    
        //TIM_Prescaler:72,TIM_Period:20000周期为20ms
        TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;                        //定时器周期
        TIM_TimeBaseInitStructure.TIM_Period = 99;                        //定时器更新的重装载值0-65535
        TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);            //初始化TIM3      
        
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = 0;                                //预设值
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    
        
        TIM_OC1Init(TIM3,&TIM_OCInitStructure);
        TIM_OC2Init(TIM3,&TIM_OCInitStructure);
        TIM_OC3Init(TIM3,&TIM_OCInitStructure);
        TIM_OC4Init(TIM3,&TIM_OCInitStructure);

        TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
        TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
        TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
        TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
        TIM_CtrlPWMOutputs(TIM3, ENABLE);
        TIM_ITConfig(TIM3,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);
        //使能标志位CC1,CC2,CC3,CC4
        TIM_Cmd(TIM3,ENABLE);
        //使能TIM;
}


void TIM2_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2, ENABLE);

}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		++timestamp;
		if(app.remote_switch && STATE == 1) {
			if(timestamp % 1000 == 0) {
				++app.remote_second;
			}
		}
	}
}




