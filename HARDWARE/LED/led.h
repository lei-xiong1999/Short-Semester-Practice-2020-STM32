#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED1		PAin(6)
#define LED2		PAin(7)
#define LED3		PBin(0)
#define LED4		PBin(1)

enum led_status {
	led_on = 1,
	led_off
};

void Led_PWM_GPIO_Init(void);
void Led_Set_Status(uint8_t leds, enum led_status status);
void Led_Set_Luminance(uint8_t leds, uint8_t luminance);
void Led_Startup_1(void);
void Led_Startup_2(void);
void Led_Switch(uint8_t luminance, enum led_status led[]);
void Led_Effect(uint8_t type, uint8_t speed, uint8_t luminance);

#endif

