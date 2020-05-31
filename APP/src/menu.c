#include "app.h"
#include <math.h>

static void menu_constructor_functions_1_static(void);
static void menu_constructor_functions_2_static(void);
static void menu_constructor_functions_3_static(void);
static void menu_constructor_functions_4_static(void);

static void menu_constructor_gesture_static(void);
static void menu_constructor_gesture_1_static(void);
static void menu_constructor_gesture_1_dynamic(void);

static void menu_constructor_transfer_1_static(void);
static void menu_constructor_transfer_2_static(void);

static void menu_constructor_dual_static(void);

static void menu_constructor_remote_1_static(void);
static void menu_constructor_remote_2_static(void);

static void menu_constructor_functions_1_dynamic(void);
static void menu_constructor_functions_2_dynamic(void);
static void menu_constructor_functions_3_dynamic(void);
static void menu_constructor_functions_4_dynamic(void);

static void menu_constructor_gesture_dynamic(void);

static void menu_constructor_transfer_1_dynamic(void);
static void menu_constructor_transfer_2_dynamic(void);

static void menu_constructor_dual_dynamic(void);

static void menu_constructor_remote_1_dynamic(void);
static void menu_constructor_remote_2_dynamic(void);

extern long timestamp;

u8 usart2_flag;

int time_line;
u8 xais = 7;
u8 yais;

menu_t menu;
app_status_t app = {
	{led_off, led_off, led_off, led_off},
	9,
	0,
	10,
};

void (*menu_constructors_static[5][4])() = {
	{menu_constructor_functions_1_static, 	menu_constructor_functions_2_static, 	menu_constructor_functions_3_static, 	menu_constructor_functions_4_static	},
	{menu_constructor_gesture_static, 	menu_constructor_gesture_1_static, 					NULL, 					NULL					},
	{menu_constructor_transfer_1_static,	menu_constructor_transfer_2_static,	NULL,					NULL					},
	{menu_constructor_dual_static, 		NULL, 					NULL, 					NULL					},
	{menu_constructor_remote_1_static, 	menu_constructor_remote_2_static,	NULL, 					NULL					}
};

void (*menu_constructors_dynamic[5][4])() = {
	{menu_constructor_functions_1_dynamic, 	menu_constructor_functions_2_dynamic, 	menu_constructor_functions_3_dynamic, 	menu_constructor_functions_4_dynamic	},
	{menu_constructor_gesture_dynamic, 	menu_constructor_gesture_1_dynamic, 					NULL, 					NULL					},
	{menu_constructor_transfer_1_dynamic,	menu_constructor_transfer_2_dynamic,	NULL,					NULL					},
	{menu_constructor_dual_dynamic, 	NULL, 					NULL, 					NULL					},
	{menu_constructor_remote_1_dynamic, 	menu_constructor_remote_2_dynamic,	NULL, 					NULL					}
};

void menu_init()
{
	menu.present_constructor_static = menu_constructors_static[0][0];
	menu.page_index = 0;
	menu.sub_page_index = 0;
	menu.flagIsMainMenu = 1;
	GUI_DispStringHCenterAt("2020专业实践综合设计", (GUI_GetXSize() + 0) / 2, 16);
	GUI_DispStringHCenterAt("学号: 17041126", (GUI_GetXSize() + 0) / 2, 32);
	GUI_DispStringHCenterAt("姓名: 熊磊", (GUI_GetXSize() + 0) / 2, 48);
	GUI_Update();
	//Led_Startup_1();
	Led_Startup_2();
	GUI_Clear();
	GUI_DrawBitmap(&bmhdulogo, (GUI_GetXSize() - bmhdulogo.XSize) / 2, (GUI_GetYSize() - bmhdulogo.YSize) / 2);
	GUI_Update();
	
}

void menu_constructor_main_static()
{
	uint8_t disp_buffer[20];
	
	GUI_Clear();
	
	GUI_DispStringAt("专业实践综合设计", 2, 2);
	
	if(menu.page_index < 3) {
		GUI_DispStringAt("→", 10, 20 + menu.page_index * 14);
		GUI_DispStringAt("1.系统测试", 25, 20);
		GUI_DispStringAt("2.姿态解算", 25, 34);
		GUI_DispStringAt("3.呼吸灯控制", 25, 48);
		GUI_DispStringAt("1/2", 106, 50);
	}
	else if(menu.page_index < 5) {
		GUI_DispStringAt("→", 10, 20 + (menu.page_index - 3) * 14);
		GUI_DispStringAt("4.WiFi通信", 25, 20);
		GUI_DispStringAt("5.拓展功能", 25, 34);
		GUI_DispStringAt("2/2", 106, 50);
	}
	
}

static void menu_constructor_functions_1_static()
{
	GUI_Clear();
	
	GUI_DispStringAt("系统测试", 2, 2);
	GUI_DispStringAt("1/4", 108, 2);
	
	GUI_DispStringAt("LED状态:", 8, 21);
	GUI_DispStringAt("按键状态:", 8, 39);
}


static void menu_constructor_functions_2_static()
{
	GUI_Clear();
	
	GUI_DispStringAt("系统测试", 2, 2);
	GUI_DispStringAt("2/4", 108, 2);
	
	GUI_DispStringAt("AD数据", 8, 21);
	GUI_DispStringAt("电压数据:", 72, 21);
}

static void menu_constructor_functions_3_static()
{
	GUI_Clear();
	
	GUI_DispStringAt("系统测试", 2, 2);
	GUI_DispStringAt("3/4", 108, 2);
	
	GUI_DispStringAt("AccX", 14, 16);
	GUI_DispStringAt("AccY", 49, 16);
	GUI_DispStringAt("AccZ", 90, 16);
}
static void menu_constructor_functions_4_static()
{
	GUI_Clear();
	
	GUI_DispStringAt("系统测试", 2, 2);
	GUI_DispStringAt("4/4", 108, 2);
	
	GUI_DispStringAt("GyroX", 10, 16);
	GUI_DispStringAt("GyroY", 52, 16);
	GUI_DispStringAt("GyroZ", 86, 16);
}

static void menu_constructor_gesture_static()
{
	GUI_Clear();
	
	GUI_DispStringAt("姿态解算", 2, 2);
	GUI_DispStringAt("1/2", 108, 2);
	
	GUI_DispStringAt("Pitch:", 8, 21);
	GUI_DispStringAt("Roll:", 8, 34);
	GUI_DispStringAt("Yaw:", 8, 47);
}

static void menu_constructor_gesture_1_static()
{
	GUI_Clear();
	
	GUI_DispStringAt("姿态解算", 2, 2);
	GUI_DispStringAt("2/2", 108, 2);
	
}

static void menu_constructor_transfer_1_static()
{
	GUI_Clear();
	
	GUI_DispStringAt("呼吸灯控制", 2, 2);
	GUI_DispStringAt("1/1", 108, 2);
	
	GUI_DispStringAt("亮灯个数:", 8, 20);
	GUI_DispStringAt("呼吸频率:", 8, 34);
	GUI_DispStringAt("最大亮度:", 8, 48);
}

static void menu_constructor_transfer_2_static()
{
	GUI_Clear();
	
	GUI_DispStringAt("呼吸灯控制", 2, 2);
	GUI_DispStringAt("1/1", 108, 2);
	
	GUI_DispStringAt("亮灯个数:", 8, 20);
	GUI_DispStringAt("呼吸频率:", 8, 34);
	GUI_DispStringAt("最大亮度:", 8, 48);
}


static void menu_constructor_dual_static()
{
	GUI_Clear();
	
	GUI_DispStringAt("WiFi通信", 2, 2);
	GUI_DispStringAt("1/1", 108, 2);
	
	GUI_DispStringAt("回调状态:", 8, 20);
	GUI_DispStringAt("IP:", 8, 34);
	GUI_DispStringAt("SSID:", 8, 48);
	
	//ESP_Init();
}

static void menu_constructor_remote_1_static()
{
	GUI_Clear();
	
	GUI_DispStringAt("Onenet上传", 2, 2);
	GUI_DispStringAt("1/2", 108, 2);
	
	GUI_DispStringAt("连接状态:", 8, 20);
	GUI_DispStringAt("数据条数:", 8, 34);
}

static void menu_constructor_remote_2_static()
{
	GUI_Clear();
	
	GUI_DispStringAt("电压图线", 2, 2);
	GUI_DispStringAt("2/2", 108, 2);
	
	GUI_DispStringAt("↑V", 2, 20);
	GUI_DrawLine(7, 30, 7, 57);
	
	GUI_DrawLine(7, 57, 104, 57);
	GUI_DispStringAt("→t", 104, 52);
	
}

static void menu_constructor_main_dynamic()
{
	uint8_t disp_buffer[20];
	
	GUI_Clear();
	
	GUI_DispStringAt("数字系统课程设计", 2, 2);
	
	if(menu.page_index < 3) {
		GUI_DispStringAt("→", 10, 20 + menu.page_index * 14);
		GUI_DispStringAt("1.系统测试", 25, 20);
		GUI_DispStringAt("2.姿态解算", 25, 34);
		GUI_DispStringAt("3.呼吸灯控制", 25, 48);
		GUI_DispStringAt("1/2", 106, 50);
	}
	else if(menu.page_index < 5) {
		GUI_DispStringAt("→", 10, 20 + (menu.page_index - 3) * 14);
		GUI_DispStringAt("4.WiFi通信", 25, 20);
		GUI_DispStringAt("5.拓展功能", 25, 34);
		GUI_DispStringAt("2/2", 106, 50);
	}
	
}


static void menu_constructor_functions_1_dynamic()
{
	char disp_buffer[20];

	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%s%s%s%s", ((LED1 == 0) ? "●" : "○"), ((LED2 == 0) ? "●" : "○"), ((LED3 == 0) ? "●" : "○"), ((LED4 == 0) ? "●" : "○"));

	GUI_DispStringAt(disp_buffer, 68, 21);
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%s%s%s%s", ((KEY1 == 0) ? "▂" : "▉"), ((KEY2 == 0) ? "▂" : "▉"), ((KEY3 == 0) ? "▂" : "▉"), ((KEY4 == 0) ? "▂" : "▉"));
		
	GUI_DispStringAt(disp_buffer, 68, 39);
}

static void menu_constructor_functions_2_dynamic()
{
	uint16_t column_height = 0;
	char disp_buffer[20];
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%d", app.ad_value);
	GUI_DispStringAt(disp_buffer, 12, 39);
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%.2f", (app.ad_value / 4096.0) * 3.3);
	GUI_DispStringAt(disp_buffer, 80, 39);
	
	GUI_ClearRect(49, 24, 20, 24);
	column_height = (uint16_t)(((app.ad_value / 4096.0) * 120) / 5);
	GUI_FillRect(49, 48 - column_height, 68, 48);
	
	

}

static void menu_constructor_functions_3_dynamic()
{
	uint16_t column_height = 0;
	char disp_buffer[20];
	
	GUI_ClearRect(0, 51, 128, 12);
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%5d", ax);
	GUI_DispStringAt(disp_buffer, 10, 51);
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%5d", ay);
	GUI_DispStringAt(disp_buffer, 52, 51);
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%5d", az);
	GUI_DispStringAt(disp_buffer, 86, 51);
	
	GUI_ClearRect(17, 30, 18, 20);
	column_height = (uint16_t)((((ax + 32767) / 65535.0) * 100) / 5);
	GUI_FillRect(17, 50 - column_height, 17 + 18, 50);
	
	GUI_ClearRect(55, 30, 18, 20);
	column_height = (uint16_t)((((ay + 32767) / 65535.0) * 100) / 5);
	GUI_FillRect(55, 50 - column_height, 55 + 18, 50);
	
	GUI_ClearRect(93, 30, 18, 20);
	column_height = (uint16_t)((((az + 32767) / 65535.0) * 100) / 5);
	GUI_FillRect(93, 50 - column_height, 93 + 18, 50);
	
}

static void menu_constructor_functions_4_dynamic()
{
	int16_t column_height = 0;
	char disp_buffer[20];
	
	GUI_ClearRect(0, 51, 128, 12);
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%5d", gx);
	GUI_DispStringAt(disp_buffer, 10, 51);
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%5d", gy);
	GUI_DispStringAt(disp_buffer, 52, 51);
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%5d", gz);
	GUI_DispStringAt(disp_buffer, 86, 51);
	
	GUI_ClearRect(17, 30, 18, 20);
	column_height = (uint16_t)((((gx + 32767) / 65535.0) * 100) / 5);
	GUI_FillRect(17, 50 - column_height, 17 + 18, 50);
	
	GUI_ClearRect(55, 30, 18, 20);
	column_height = (uint16_t)((((gy + 32767) / 65535.0) * 100) / 5);
	GUI_FillRect(55, 50 - column_height, 55 + 18, 50);
	
	GUI_ClearRect(93, 30, 18, 20);
	column_height = (uint16_t)((((gz + 32767) / 65535.0) * 100) / 5);
	GUI_FillRect(93, 50 - column_height, 93 + 18, 50);
}

static void menu_constructor_gesture_dynamic()
{
	char disp_buffer[20];
	
	GUI_ClearRect(58, 21, 70, 12);
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%.2f°", fAX);
	GUI_DispStringAt(disp_buffer, 58, 21);
	memset(disp_buffer, 0, 20);
//	sprintf(disp_buffer, "%d%%", (int)(((fAX + 180) / 360) * 100));
//	GUI_DispStringAt(disp_buffer, 110, 21);
	
	GUI_ClearRect(58, 34, 70, 12);
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%.2f°", fAY);
	GUI_DispStringAt(disp_buffer, 58, 34);
	memset(disp_buffer, 0, 20);
//	sprintf(disp_buffer, "%d%%", (int)(((fAY + 180) / 360) * 100));
//	GUI_DispStringAt(disp_buffer, 110, 34);
	
	GUI_ClearRect(58, 47, 70, 12);
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%.2f°", fAZ);
	GUI_DispStringAt(disp_buffer, 58, 47);
	memset(disp_buffer, 0, 20);
//	sprintf(disp_buffer, "%d%%", (int)(((fAZ + 180) / 360) * 100));
//	GUI_DispStringAt(disp_buffer, 110, 47);
}

static void menu_constructor_gesture_1_dynamic()
{
	u8 x0 = 64;
	u8 y0 = 42;
	u8 x1,x2,x3,x4,y1,y2,y3,y4;
	float column_x = 0;
	float column_y = 0;
	uint16_t column_height = 0;
	char disp_buffer[20];
	
	column_x = (float)(((fAX + 180) / 360.0) * 25.0);
	column_y = (float)(((fAY + 180) / 360.0) * 25.0);
	
	x1 = x0 + (int)((column_x+3)*sin((fAZ/180.0)*3.1415926));
	y1 = y0 - (int)((column_x+3)*cos((fAZ/180.0)*3.1415926));
	
	x2 = x0 + (int)((column_y+3)*cos((fAZ/180.0)*3.1415926));
	y2 = y0 + (int)((column_y+3)*sin((fAZ/180.0)*3.1415926));
	
	x3 = x0 - (int)((column_x+3)*sin((fAZ/180.0)*3.1415926));
	y3 = y0 + (int)((column_x+3)*cos((fAZ/180.0)*3.1415926));
	
	x4 = x0 - (int)((column_y+3)*cos((fAZ/180.0)*3.1415926));
	y4 = y0 - (int)((column_y+3)*sin((fAZ/180.0)*3.1415926));
	
	//printf("%d\r\n", x1);
	GUI_ClearRect(0, 20, 128, 64);
	
	GUI_DrawLine(x1, y1, x2, y2);
	GUI_DrawLine(x2, y2, x3, y3);
	GUI_DrawLine(x3, y3, x4, y4);
	GUI_DrawLine(x4, y4, x1, y1);
	
	
	
	//GUI_ClearRect(0, 20, 128, 64);
	

	//column_height = (uint16_t)(((app.ad_value / 4096.0) * 120) / 5);
//	GUI_FillRect(17, 50 - column_y, 25 + column_x, 58);
	
	
	
//	
//	memset(disp_buffer, 0, 20);
//	sprintf(disp_buffer, "x:%d", column_x+8);
//	GUI_DispStringAt(disp_buffer, 80, 32);
//	memset(disp_buffer, 0, 20);
//	sprintf(disp_buffer, "y:%d", column_y+8);
//	GUI_DispStringAt(disp_buffer, 80, 46);
//	
	
}
	


static void menu_constructor_transfer_1_dynamic()
{
	char disp_buffer[20];
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%d个", led_num);
	GUI_ClearRect(64, 20, 64, 12);
	GUI_DispStringAt(disp_buffer, 64, 20);	
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%d", led_freq);
	GUI_ClearRect(64, 34, 64, 12);
	GUI_DispStringAt(disp_buffer, 64, 34);

	memset(disp_buffer, 0, 20);	
	sprintf(disp_buffer, "%d%%", led_lumi);
	GUI_ClearRect(64, 48, 64, 12);
	GUI_DispStringAt(disp_buffer, 64, 48);
}

static void menu_constructor_transfer_2_dynamic()
{
	
	char disp_buffer[20];
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%d", app.recev_count);
	GUI_ClearRect(64, 20, 78, 12);
	GUI_DispStringAt(disp_buffer, 64, 20);	
	
}

static void menu_constructor_dual_dynamic()
{
	char disp_buffer[20];
	char temp_buffer[20];
	u8 i;
	
	if(wifi_status == 0)
	{
		memset(disp_buffer, 0, 20);
		sprintf(disp_buffer, "初始化中...");
		if(wifi_err == 1)
		{
			memset(disp_buffer, 0, 20);
			sprintf(disp_buffer, "检查模块！");
		}
	}
	else if(wifi_status == 1)
	{
		memset(disp_buffer, 0, 20);
		sprintf(disp_buffer, "STA+AP");
	}
	else if(wifi_status == 2)
	{
		memset(disp_buffer, 0, 20);
		sprintf(disp_buffer, "连接中...");
		if(wifi_err == 2)
		{
			memset(disp_buffer, 0, 20);
			sprintf(disp_buffer, "连接超时！");
		}
	}
	else if(wifi_status == 3)
	{
		memset(disp_buffer, 0, 20);
		sprintf(disp_buffer, "TCP已连接");
	}
	else if(wifi_status == 4 )
	{
		memset(disp_buffer, 0, 20);
		sprintf(disp_buffer, "透传退出");
	}
	
	if(Wifi_receive[0] != 0 && wifi_status == 3)
	{
		sprintf(disp_buffer, "%s", Wifi_receive);	
		Wifi_cnt = 0;
	}
	GUI_ClearRect(60, 20, 100, 12);
	GUI_DispStringAt(disp_buffer, 60, 20);
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "192.168.4.2-8010");
	GUI_DispStringAt(disp_buffer, 26, 34);
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "ESP_Exclusive");
	GUI_DispStringAt(disp_buffer, 40, 48);
	
}

static void menu_constructor_remote_1_dynamic()
{ 
	char disp_buffer[20];
	
	memset(disp_buffer, 0, 20);
	if(onenet_init == 0)
	{
		sprintf(disp_buffer, "未连接");
	}
	else if(onenet_init == 1)
	{
		sprintf(disp_buffer, "EDP已连接");
	}
	else if(onenet_init == 2)
	{
		sprintf(disp_buffer, "连接中...");
	}
	GUI_ClearRect(65, 20, 63, 12);
	GUI_DispStringAt(disp_buffer, 65, 20);
	
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "%d", onenet_number);
	GUI_ClearRect(65, 34, 63, 26);
	GUI_DispStringAt(disp_buffer, 65, 34);

}

static void menu_constructor_remote_2_dynamic()
{
	char disp_buffer[20];
	time_line++;
	memset(disp_buffer, 0, 20);
	sprintf(disp_buffer, "= %.2fV", (app.ad_value / 4096.0) * 3.3);
	GUI_DispStringAt(disp_buffer, 24, 20);
	yais = (app.ad_value / 4096.0) * 25;	
	printf("%d\r\n",yais);
	if(time_line%1 == 0)
	{
		GUI_DrawPixel(xais, 56 - yais);
		xais++;
		if(time_line == 93)
		{
			GUI_ClearRect(8, 30, 100, 26);
			xais = 7;
			time_line = 0;
		}
	}
	
	//time_line = 7;
	//GUI_ClearRect(8, 30, 100, 25);
	
	

}




