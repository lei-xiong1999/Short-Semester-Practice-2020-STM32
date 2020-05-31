#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2011/6/14
//版本：V1.4
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			500  	//定义最大接收字节数 500
#define USART_TX_LEN  			500  	//定义最大接收字节数 500
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[100]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART2_RX_BUF[USART_REC_LEN];
extern u16 USART_RX_STA;         		//接收状态标记	
extern u16 USART2_RX_STA;
extern u16 NETWORK_RX_STA;
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void usartx_send(USART_TypeDef * USARTX,u8 *string);
void usartx_puchar(USART_TypeDef * USARTX,u8 *buff, u32 len);
void uart2_init(u32 bound);
u8 USART2_SendString(u8 *p);
void USART2_SendRaw(u8 *p, u8 len);
void USART2_Response(void);
void USART1_SendCommand(void);
void u2_printf(char* fmt,...);
#endif


