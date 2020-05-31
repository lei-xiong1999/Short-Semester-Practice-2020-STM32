#include "bsp.h"
#include "FreeRTOS.h"                      
#include "task.h"
#include "GUI.h"

#if	(__USE_DRIVER_SYSTICK)

extern void xPortSysTickHandler(void);
extern volatile GUI_TIMER_TIME OS_TimeMS;

static u16 fac_ms=0;    

/**
  * @brief  启动系统滴答定时器 SysTick
  * @param  无
  * @retval 无
  */
void bsp_systick_init(void)
{

	uint32_t reloadValue;
	
	/* SysTick时钟不需要系统时钟那么高，为了降低功耗将其8分频 */
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	
	/* 根据OSCfg_TickRate_Hz设定溢出时间 */
	reloadValue = (SystemCoreClock / 8) / 1000u;
	
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  /* 开启SYSTICK中断 */
	SysTick->LOAD = reloadValue;                /* 每1 / OS_TICKS_PER_SEC秒中断一次	*/
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   /* 开启SYSTICK */
}

/**
 * @brief   us延时程序,1us为一个单位
 * note     若SysTick时钟是系统时钟不分频，节拍数8000000改为1000000
 * @param  
 *		@arg nTime: 延时的时间
 * note     保证nTime在1 ~ 1000000 * 2^32 / SystemCoreClock之间
 *	         典型值 1 ~ 25565281（SystemCoreClock = 168MHz）
 * @retval  %NUL
 * add by Supersure 2018
 **/
void bsp_delay_us(__IO uint32_t nTime)
{
	uint32_t ticks;
	uint32_t told;
	uint32_t tnow;
	uint32_t tcnt;
	uint32_t reload;
	
	/* 刚进入时的计数器值 */
	told = SysTick->VAL;
	/* 重装再战 */
	reload = SysTick->LOAD;
	/* 清零计数值 */
	tcnt = 0;
	/* 需要的节拍数 */
	ticks = nTime * (SystemCoreClock / 8000000);

	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			/* SYSTICK是一个递减的计数器 */
			if (tnow < told)
			{
				tcnt += told - tnow;
			}
			/* 重新装载递减 */
			else
			{
				tcnt += reload - tnow + told;
			}
			told = tnow;

			/* 时间超过/等于要延迟的时间,则退出 */
			if (tcnt >= ticks)
			{
				break;
			}
		}
	}
	
}

void bsp_delay_ms(__IO uint32_t nTime)
{
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
	{       
			if(nTime>=fac_ms)                    
					vTaskDelay(nTime/fac_ms);         
			nTime%=fac_ms;                        
	}
	bsp_delay_us((uint32_t)(nTime * 1000));
}

void SysTick_Handler(void)
{
	
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
    xPortSysTickHandler();  

	
}

#endif



