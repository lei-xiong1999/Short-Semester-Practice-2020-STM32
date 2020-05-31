#ifndef __CONNETONENET_H
#define __CONNETONENET_H
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "time.h"
//协议文件
#include "edpkit.h"
//C库
#include <string.h>
#include <stdio.h>
#define   Wifi_OK    0
#define   Wifi_wait  1
//以下都是AT指令，具体功能看ESP8266的手册
#define   AT          "AT\r\n"	      
#define   CWMODE      "AT+CWMODE=1\r\n"
#define		CWMODE_TCP  "AT+CWMODE=3\r\n"
#define   RST         "AT+RST\r\n"
#define   CIFSR       "AT+CIFSR\r\n"
#define   CWJAP       "AT+CWJAP=\"ONENET1\",\"17041126\"\r\n"
#define   CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.39\",876\r\n"
#define   CIPMODE     "AT+CIPMODE=1\r\n"
#define   CIPSEND     "AT+CIPSEND\r\n"
#define   CIPSTATUS   "AT+CIPSTATUS\r\n"
#define		CWSAP				"AT+CWSAP=\"ESP_Exclusive\",\"17041126\",5,3\r\n"
#define		CIPSTARTPC	"AT+CIPSTART=\"TCP\",\"192.168.4.2\",8010\r\n"

//以下都是发送AT指令后wifi模块返回的值
#define   WIFICONNECT  "WIFI CONNECT"
#define   CONNECTING  "STATUS:3"
#define   UNCONNECT1  "STATUS:2"
#define   UNCONNECT2  "STATUS:4"
#define   answer1     "OK"
#define   answer2     "no change"
#define   answer3     "+++"
#define   answer4     "ready"
#define   CONNECT     "CONNECT"
#define   CLOSE       "CIPCLOSE"

#define DEVID  	"598895065"        //设备号 可以改
#define APIKEY	"zqTObjjohlbG3wP8Gt=GSCVnVKE=" //API钥匙 可以改
 

void ESP8266_Clear(void);

u8 ESP8266_WaitRecive(void);

u8 ESP8266_SendCmd(u8 *cmd, u8*res);

void ESP8266_init(void);

_Bool OneNet_EDPKitCmd(unsigned char *data);


_Bool OneNet_DevLink(const char* devid, const char* auth_key);

void OneNet_FillBuf(char *buf,char *buf1,float number);

void OneNet_SendData(char *buf,char *buf1,float number);

void ESP8266_QuitTrans(void);

//系统软复位   
void Sys_Soft_Reset(void);

void ESP8266_SendData(unsigned char *data, int len);



extern u8 Wifi_receive[128];
extern u8 Wifi_cnt;
extern u8 Wifi_lastcnt;
extern u8 connect_out;
#endif	 



