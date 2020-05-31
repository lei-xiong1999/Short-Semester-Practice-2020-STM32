#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

#define KEY1		PAin(4)
#define KEY2		PAin(5)
#define KEY3		PAin(8)
#define KEY4		PAin(15)

#define K1_Pin 		GPIO_Pin_4
#define K1_GPIO_Port 	GPIOA

#define K2_Pin 		GPIO_Pin_5
#define K2_GPIO_Port 	GPIOA

#define K3_Pin 		GPIO_Pin_8
#define K3_GPIO_Port 	GPIOA

#define K4_Pin 		GPIO_Pin_15
#define K4_GPIO_Port 	GPIOA

void Key_GPIO_Init(void);
int Key_Scan(void);

#endif
