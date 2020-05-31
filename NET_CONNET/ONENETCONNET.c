#include "onenetconnet.h"
#include "sys.h" 
#include "app.h"

u8 Wifi_receive[128];
u8 Wifi_cnt=0;
u8 Wifi_lastcnt=0;
u8 connect_out=0;
EdpPacket *send_pkg,*send_pkg1;	//协议包
//清楚串口接收的字符串
void ESP8266_Clear(void)
{
	memset(Wifi_receive, 0, sizeof(Wifi_receive));
	Wifi_cnt = 0;
}
//	函数功能：	等待接收完成
//	入口参数：	无
//	返回参数：	Wifi_OK-接收完成		Wifi_wait-接收超时未完成
//	说明：		循环调用检测是否接收完成
//==========================================================


u8 ESP8266_WaitRecive(void)
{
	if(Wifi_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
	  return Wifi_wait;
	if(Wifi_cnt == Wifi_lastcnt)
	{			//如果上一次的值和这次相同，则说明接收完毕
		Wifi_cnt = 0;							    //清0接收计数
		return Wifi_OK;								//返回接收完成标志
	}
  Wifi_lastcnt=Wifi_cnt;					  //置为相同
	  return Wifi_wait;							//返回接收未完成标志
}



//	函数功能：	发送命令
//	入口参数：	cmd：命令
//	res：需要检查的返回指令
//	返回参数：	0-成功	1-失败
u8 ESP8266_SendCmd(u8 *cmd, u8 *res)
{
	unsigned char timeOut = 50;
  ESP8266_Clear();	
  usartx_puchar(USART2, (u8*)cmd, strlen((const char *)cmd));   //串口2发送AT指令的
	usartx_puchar(USART1, (u8*)cmd, strlen((const char *)cmd));   //方便调试用，通过串口1查看
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == Wifi_OK){						             	//如果收到数据
	  	usartx_puchar(USART1, (u8*)Wifi_receive, strlen((const char *)Wifi_receive));
			if(strstr((const char *)Wifi_receive, (const char *)res) != NULL)
		  {	//如果检索到关键词
				ESP8266_Clear();									                      //清空缓存			
				return 0;
			}
			 ESP8266_Clear();	
		}
		delay_ms(10);
	}

	
	return 1;
}


void ESP8266_init(){
	while(ESP8266_SendCmd((u8*)AT, (u8*)answer1))        //通过串口3发送AT指令
	delay_ms(500);
	while(ESP8266_SendCmd((u8*)CWMODE,(u8*)answer1))     //通过串口3发送AT指令
	delay_ms(500);
	while(ESP8266_SendCmd((u8*)RST,(u8*)answer1))        //通过串口3发送AT指令
	delay_ms(500);
	while(ESP8266_SendCmd((u8*)CIFSR,(u8*)answer1))      //通过串口3发送AT指令
	delay_ms(500);
	while(ESP8266_SendCmd((u8*)CWJAP,(u8*)WIFICONNECT))  //通过串口3发送AT指令
	delay_ms(500);
	while(ESP8266_SendCmd((u8*)CIPSTART,(u8*)answer1))   //通过串口3发送AT指令
	delay_ms(500);
	usartx_send(USART1, "WIFI Init OK\r\n");
}


//==========================================================
//	函数名称：	A7_EnterTrans
//
//	函数功能：	进入透传模式
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_EnterTrans(void)
{
	ESP8266_SendCmd("AT+CIPMODE=1\r\n", "OK");//使能透传
	delay_ms(200); 
  ESP8266_SendCmd("AT+CIPSEND\r\n", ">");  //通过串口2发送AT指令
}

//==========================================================
//	函数名称：	A7_QuitTrans
//
//	函数功能：	退出透传模式
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		连续发送三个‘+’，然后关闭透传模式
//==========================================================
void ESP8266_QuitTrans(void)
{
  delay_ms(500); 
	while((USART2->SR & 0X40) == 0);	//等待发送空
	USART2->DR = '+';
	delay_ms(15); 					//大于串口组帧时间(10ms)
	
	while((USART2->SR & 0X40) == 0);	//等待发送空
	USART2->DR = '+';        
	delay_ms(15); 					//大于串口组帧时间(10ms)
	
	while((USART2->SR & 0X40) == 0);	//等待发送空
	USART2->DR = '+';        
	delay_ms(500); 					//大于串口组帧时间(10ms)

	usartx_puchar(USART1,"+++\r\n",5);  //方便调试用，通过串口1查看

}


//==========================================================
//	函数名称：	A7_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_SendData(unsigned char *data, int len)
{
	ESP8266_Clear();								//清空接收缓存
  ESP8266_EnterTrans();
	usartx_puchar(USART1, data, len);	//发送设备连接请求数据
	usartx_puchar(USART2, data, len);	//发送设备连接请求数据
	usartx_send(USART1,"\r\n");
	ESP8266_QuitTrans();
}


_Bool OneNet_EDPKitCmd(unsigned char *data)
{
	if(data[0] == CONNRESP) //连接响应
	{		
		usartx_send(USART1,"DevLink: ");   
		//0		连接成功
		//1		验证失败：协议错误
		//2		验证失败：设备ID鉴权失败
		//3		验证失败：服务器失败
		//4		验证失败：用户ID鉴权失败
		//5		验证失败：未授权
		//6		验证失败：激活码未分配
		//7		验证失败：该设备已被激活
		//8		验证失败：重复发送连接请求包
		//9		验证失败：重复发送连接请求包
		if(data[3] == 0)
			return 1;
		else
			return 0;
	}
	
	return 0;

}
//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenCIPTMODEet平台建立连接
//==========================================================
_Bool OneNet_DevLink(const char* devid, const char* auth_key)
{
	_Bool status = 0;
	send_pkg = PacketConnect1(devid, auth_key);					                         	   //根据devid 和 apikey封装协议包     
	ESP8266_Clear();			                                                           //清空缓存
	ESP8266_SendData(send_pkg->_data, send_pkg->_write_pos);                         //发送数据到平台
	delay_ms(100);	
	usartx_send(USART1,"\r\n{");
  usartx_puchar(USART1, (u8*)Wifi_receive, strlen((const char *)Wifi_receive));usartx_send(USART1,"}\r\n");   
	DeleteBuffer(&send_pkg);										//删包
	ESP8266_Clear();
	return status;
}

//把数据设置为平台可识别的格式
void OneNet_FillBuf(char *buf,char *buf1,float number)  
{
	char text[25] = {0};
	memset(buf, 0, sizeof(buf));
	strcat((char *)buf, ",;");	
	strcat((char *)buf, buf1);
	strcat((char *)buf, ",");
	sprintf(text,"%.2f",number);
  strcat((char *)buf, text);
	strcat((char *)buf, ";");
}

//==========================================================
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_SendData(char *buf,char *buf1,float number)
{
  OneNet_FillBuf(buf,buf1,number);														//封装数据流	
	
	send_pkg = PacketSavedataSimpleString(NULL,(const char *) buf);							//封包-Type5
	
	ESP8266_SendData(send_pkg->_data, send_pkg->_write_pos);
	
	DeleteBuffer(&send_pkg);		 		//删包
	
}

//	OneNet_SendData(send_but,"shidu",humidity);
//	OneNet_SendData(send_but,"wendu",temperature);



















