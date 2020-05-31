#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "stm32f10x_dma.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "bsp.h"

#if SYSTEM_SUPPORT_UCOS
#include "includes.h"
#endif

extern volatile u8 rcv_sta;

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE {
	int handle;

};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
#ifdef __DEBUG
	while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕
	USART1->DR = (u8) ch;
#endif
	return ch;
}
#endif

/*使用microLib的方法*/
/*
int fputc(int ch, FILE *f)
{
USART_SendData(USART1, (uint8_t) ch);

while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}

   return ch;
}
int GetKey (void)  {

   while (!(USART1->SR & USART_FLAG_RXNE));

   return ((int)(USART1->DR & 0x1FF));
}
*/

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART_RX_BUF[100];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART2_RX_BUF[USART_REC_LEN];
u8 USART2_TX_BUF[USART_TX_LEN];
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0;     //接收状态标记
u16 USART2_RX_STA = 0;
u16 NETWORK_RX_STA = 0;
//初始化IO 串口1
//bound:波特率
void uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	USART_DeInit(USART1);  //复位串口1
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9

	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

	//Usart1 NVIC 配置

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口

}

void TIM4_Init(u16 arr, u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );


	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void TIM4_Set(u8 sta)
{
	if(sta) {
		TIM_SetCounter(TIM4, 0);
		TIM_Cmd(TIM4, ENABLE);
	} else TIM_Cmd(TIM4, DISABLE);
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		if(USART_RX_STA & 0x4000) {
			USART_RX_STA |= 1 << 15;	//标记接收完成
		}
		if(USART2_RX_STA & 0x4000) {
			USART2_RX_STA |= 1 << 15;	//标记接收完成
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		TIM4_Set(0);
	}
}

void uart2_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2，GPIOB时钟
	USART_DeInit(USART2);  //复位串口2
	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	//USART2 NVIC 配置

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式



	USART_Init(USART2, &USART_InitStructure); //初始化串口

	TIM4_Init(99, 7199);
	USART2_RX_STA = 0;
	TIM4_Set(0);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断

	USART_Cmd(USART2, ENABLE);                    //使能串口

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) { //接收中断(接收到的数据必须是0x0d 0x0a结尾)

		Res = USART_ReceiveData(USART1); //(USART1->DR);	//读取接收到的数据

		if((USART_RX_STA & 0x8000) == 0) { //接收未完成
			if(USART_RX_STA & 0x4000) { //接收到了0x0d
				if(Res != 0x0a)USART_RX_STA = 0; //接收错误,重新开始
				else USART_RX_STA |= 0x8000;	//接收完成了
			} else { //还没收到0X0D
				if(Res == 0x0d)USART_RX_STA |= 0x4000;
				else {
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res ;
					USART_RX_STA++;
					if(USART_RX_STA > (USART_REC_LEN - 1))USART_RX_STA = 0; //接收数据错误,重新开始接收
				}
			}
		}
	}
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();
#endif
}

void usartx_send(USART_TypeDef * USARTX,u8 *string)            //发送字符串
{
  USARTX->SR &=~(1<<6);
  while(*string!='\0')
	  {
	    USARTX->DR = *(string++);
	    while(!(USARTX->SR & (1<<6)));
	  }
}

//void uart_putbuff(USART_TypeDef * USARTX,,u8 *buff, u32 len)
//输入参数 
//USARTX    : USART1 | USART2| USART3   (选择串口)
//buff      :"sadsad"| str              (字符串或数组首地址)
//len       :strlen(str)                (字符串或数组长度)
void usartx_puchar(USART_TypeDef * USARTX,u8 *buff, u32 len)    //发送字符串
{  
	USARTX->SR &=~(1<<6);
	while(len--)
	  {
 	    USARTX->DR = *(buff++);
	    while(!(USARTX->SR & (1<<6)));
    }
}



void USART2_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART2->SR & (1<<5))
	{	
		if(Wifi_cnt >= sizeof(Wifi_receive))Wifi_cnt = 0;      //防止串口被刷爆
			 Wifi_receive[Wifi_cnt] = USART2->DR;                //把串口接收的数据存在数组Wifi_receive
			 if(Wifi_receive[Wifi_cnt]=='E'&&Wifi_cnt>=4)      //用于判断是否断开了onenet，断开了的话接收到close
					 if(strstr((const char *)Wifi_receive, "CLOSE") != NULL)
							 connect_out=1;
			 Wifi_cnt++;
			 while(USART2->SR & (1<<5));
	 }

}

u8 USART2_SendString(u8 *p)
{
	u8 i = 0;
	u8 SendByte = 0;
	printf("%s", p);
	while(p[i] != 0) {
		while((USART2->SR & 0X40) == 0);
		USART_SendData(USART2, p[i]);
		i++;
		SendByte ++;
	}
	return SendByte;
}

void USART2_SendRaw(u8 *p, u8 len)
{
	int i = 0;
	while(i < len) {
		while((USART2->SR & 0X40) == 0);
		USART_SendData(USART2, p[i]);
		i++;
	}
}


void USART2_Response()
{
	u8 i = 0;
	if(USART2_RX_BUF[0]) {
		printf("%s\r\n", USART2_RX_BUF);	//发送到串口
		USART2_RX_STA = 0;
		for(i = 0; i < USART_REC_LEN; i++)
			USART2_RX_BUF[i] = 0;
	}
}

void USART1_SendCommand()
{
	u8 i = 0;
	if(USART_RX_BUF[0]) {
		USART2_SendString(USART_RX_BUF);
		USART_RX_STA = 0;
		USART2_RX_STA = 0;
		for(i = 0; i < USART_REC_LEN; i++) {
			USART_RX_BUF[i] = 0;
			USART2_RX_BUF[i] = 0;
		}
	}
}
#endif

