#ifndef __ESP8266_H__
#define __ESP8266_H__	 

#include "app.h"

#define EN		PBin(3)
#define STATE		PBin(4)

void ESP_At_Response(u8 mode);
void ESP_Init(void);
void ESP_GPIO_Init(void);
void ESP_Enter_Trans(void);
void ESP_Quit_Trans(void);
void ESP_TCP(void);
void ESP_TCP_quit(void);
	
uint8_t ESP_Send_Cmd(u8 *cmd, u8 *ack, u16 waittime);
uint8_t ESP_Wait_Ack(uint8_t *ack, uint16_t waittime);
uint8_t* ESP_Check_Cmd(u8 *str);


#endif

