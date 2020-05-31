#ifndef __BSP_SYSTICK_H
#define __BSP_SYSTICK_H

#include "stm32f4xx.h"

#define SYSTICK_STA_INITIALIZED									1u
#define SYSTICK_STA_UNINITIALIZED								0u


//typedef struct {
//	__IO unsigned int 				xms;
//	__IO unsigned char 				xsec;
//	__IO unsigned char 				xmin;
//	__IO unsigned char 				xhour;
//	
//	uint8_t										status;
//	uint8_t 									**task_name;
//	uint8_t										task_num;
//	
//	void											(*init)();
//	uint32_t 									(*get_systime)();
//	void											(*install_timing_task)(unsigned char *handler_name, void (*pHandler)(), int time_tick, ...);
//	void										 	(*uninstall_timing_task)(unsigned char *handler_name);
//	
//}bsp_systime_struct;

//extern bsp_systime_struct systime;

void bsp_systick_init(void);
void bsp_delay_us(__IO uint32_t nTime);
void bsp_delay_ms(__IO uint32_t nTime);

#endif /* __BSP_SYSTICK_H */
