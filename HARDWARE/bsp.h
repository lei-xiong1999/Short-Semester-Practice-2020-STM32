#ifndef __BSP_H
#define __BSP_H

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "stmflash.h"
#include "adc.h"
#include "ssd1306_i2c.h"
#include "ssd1306.h"
#include "STM32_I2C.h"
#include "MPU6050.h"
#include "led.h"
#include "GUI.h"
#include "key.h"
#include "bluetooth.h"
#include "esp8266.h"

#define __DEBUG

#define __system_lock()         do {                                                     \
                                        if (FLASH_GetReadOutProtectionStatus() == RESET) \
                                        {                                                \
                                                FLASH_Unlock();                          \
                                                FLASH_ReadOutProtection(ENABLE);         \
                                        }                                                \
                                }while(0)			
				
void bsp_init(void);
				

#endif
				
