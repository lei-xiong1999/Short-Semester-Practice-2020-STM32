#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__	 

#include "app.h"

#define EN		PBin(3)
#define STATE		PBin(4)

void HC05_At_Response(u8 mode);
void HC05_Init(void);
void HC05_GPIO_Init(void);
void HC05_Switch_2_Slave(void);
void HC05_Switch_2_Master(void);
void HC05_Enter_Trans(void);
void HC05_Quit_Trans(void);
uint8_t HC05_Send_Cmd(u8 *cmd, u8 *ack, u16 waittime);
uint8_t HC05_Wait_Ack(uint8_t *ack, uint16_t waittime);
uint8_t* HC05_Check_Cmd(u8 *str);


#endif





