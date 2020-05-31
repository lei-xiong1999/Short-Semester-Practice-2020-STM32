#include "bluetooth.h"

void HC05_At_Response(u8 mode)
{
	if(USART2_RX_STA & 0X8000) {	//接收到一次数据了
		USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = 0; //添加结束符
		printf("%s", USART2_RX_BUF);	//发送到串口
		if(mode)
			USART2_RX_STA = 0;
	}
}

u8* HC05_Check_Cmd(u8 *str)
{
	char *strx = NULL;
	if(USART2_RX_STA & 0X8000) {	//接收到一次数据了
		USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = 0; //添加结束符
		strx = strstr((const char*)USART2_RX_BUF, (const char*)str);
	}
	return (u8*)strx;
}

uint8_t HC05_Send_Cmd(u8 *cmd, u8 *ack, u16 waittime)
{
	USART2_RX_STA = 0;
	printf("%s", cmd);	
	USART2_SendString(cmd);
	if(ack && waittime) {					//需要等待应答
		while(--waittime) {				//等待倒计时
			delay_ms(10);
			if(USART2_RX_STA & 0X8000) { 	//接收到期待的应答结果
				if(HC05_Check_Cmd(ack)) {
					HC05_At_Response(1);   	//打印AT指令返回信息
					return 1;		//得到有效数据
				}
				else
					USART2_RX_STA = 0;
			}
		}
		if(waittime == 0)
			return 0;
	} else;
	return 1;
}

uint8_t HC05_Wait_Ack(uint8_t *ack, uint16_t waittime)
{
	if(waittime) {				
		while(--waittime) {				
			delay_ms(10);
			if(USART2_RX_STA & 0X8000) { 	
				if(HC05_Check_Cmd(ack)) {
					++app.handshake_count;
					HC05_At_Response(1);  
					return 1;		
				}
				else
					USART2_RX_STA = 0;
			}
		}
		if(waittime == 0)
			return 0;
	} else;
	return 1;
}

void HC05_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	DBGMCU->CR  &= ~((uint32_t)1<<5);    
	AFIO->MAPR = (AFIO->MAPR & ~((uint32_t)0x7 << 24)) | (2 << 24);     /*  PA15 PB3 PB4 */

	/*EN*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
	
	/*STATE*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB,GPIO_Pin_3);	
}

void HC05_Init(void)
{
	HC05_Enter_Trans();
	while(!HC05_Send_Cmd("AT+DISC\r\n", "OK", 100));
	while(!HC05_Send_Cmd("AT+ORGL\r\n", "OK", 100));
	while(!HC05_Send_Cmd("AT+NAME=HC05\r\n", "OK", 100));
	while(!HC05_Send_Cmd("AT+CMODE=1\r\n", "OK", 100));
	while(!HC05_Send_Cmd("AT+RMAAD\r\n", "OK", 100));
	HC05_Quit_Trans();
}

void HC05_Enter_Trans(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_3);
	while(!HC05_Send_Cmd("AT\r\n", "OK", 100));
}

void HC05_Quit_Trans(void)	
{	
	HC05_Send_Cmd("AT+RESET\r\n", "OK", 100);
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);
}

void HC05_Switch_2_Slave(void)
{
	uint8_t pDataBuf[20] = {0};
	sprintf((char *)pDataBuf, "AT+NAME=%s\r\n", student_id);
	HC05_Enter_Trans();
	while(!HC05_Send_Cmd("AT+DISC\r\n", "OK", 100));
	while(!HC05_Send_Cmd(pDataBuf, "OK", 100));
	while(!HC05_Send_Cmd("AT+PSWD=1212\r\n", "OK", 100));
	while(!HC05_Send_Cmd("AT+ROLE=0\r\n", "OK", 100));
	while(!HC05_Send_Cmd("AT+ROLE=0\r\n", "OK", 100));
	HC05_Quit_Trans();
}

void HC05_Switch_2_Master(void)
{
	uint8_t pDataBuf[20] = {0};
	sprintf((char *)pDataBuf, "AT+NAME=%s\r\n", student_id);
	HC05_Enter_Trans();
	while(!HC05_Send_Cmd("AT+DISC\r\n", "OK", 100));
	while(!HC05_Send_Cmd(pDataBuf, "OK", 100));
	while(!HC05_Send_Cmd("AT+PSWD=1212\r\n", "OK", 100));
	while(!HC05_Send_Cmd("AT+ROLE=1\r\n", "OK", 100));
	while(!HC05_Send_Cmd("AT+ROLE=1\r\n", "OK", 100));
	HC05_Quit_Trans();
}

//void HC05_Get_Dev_Name(void)
//{
//	uint8_t i = 0;
//	uint8_t *pDataBuf = NULL;
//	HC05_Enter_Trans();
//	if(HC05_Send_Cmd("AT+NAME?\r\n", "OK", 100)) {
//		pDataBuf = HC05_Check_Cmd("NAME") + 5;
//		for(i = 0; pDataBuf[i] != '\r'; i++)
//			app.own_name[i] = pDataBuf[i];
//	}
//	HC05_Quit_Trans();
//}

