#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

extern volatile u16 CNT_MS;
extern volatile u16 CNT_MS_MOTOR;
extern u16 Loop_10Ms;
extern u8 Alert_Flag;

//typedef struct
//{
//	int msec;
//	int  sec;
//	int  min;
//	int  hour;
//}system_time_type_struct;

//extern volatile system_time_type_struct system_time;	

void TIM3_PWM_Init(void);
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM1_Init(u16 arr, u16 psc);
 
#endif
