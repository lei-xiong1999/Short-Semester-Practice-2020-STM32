#include "app.h"

extern long timestamp;
extern uint8_t flagSendName;

#define START_TASK_PRIO		1
#define MPU6050_TASK_PRIO	6
#define LED_TASK_PRIO		2
#define KEY_TASK_PRIO		5
#define ADC_TASK_PRIO		3
#define GUI_TASK_PRIO		5
#define TRANS_TASK_PRIO		5
#define RECEV_TASK_PRIO		4

#define START_STK_SIZE 		128
#define MPU6050_STK_SIZE	256
#define LED_STK_SIZE		160
#define KEY_STK_SIZE		160
#define ADC_STK_SIZE		128
#define GUI_STK_SIZE		160
#define TRANS_STK_SIZE		256
#define RECEV_STK_SIZE		160

TaskHandle_t StartTask_Handler;
TaskHandle_t MPU6050Task_Handler;
TaskHandle_t LedTask_Handler;
TaskHandle_t KeyTask_Handler;
TaskHandle_t AdConvertTask_Handler;
TaskHandle_t GuiTask_Handler;
TaskHandle_t TransferTask_Handler;
TaskHandle_t ReceiveTask_Handler;

static void start_task(void *pvParameters);
static void mpu6050_task(void *pvParameters);
static void led_task(void *pvParameters);
static void key_task(void *pvParameters);
static void adconvert_task(void *pvParameters);
static void gui_task(void *pvParameters);
static void transfer_task(void *pvParameters);
static void receive_task(void *pvParameters);

extern void menu_constructor_main_static(void);
extern void (*menu_constructors_static[5][4])();
extern void (*menu_constructors_dynamic[5][4])();

extern menu_t menu;
extern float fAX, fAY, fAZ;
extern short ax, ay, az, gx, gy, gz;
char up_buffer[20];
u16 led0pwmval;
u8 len;	
u8 t;
u8 onenet_init = 0;
int onenet_number = 0;
u16 led_lumi = 0, led_num, led_freq, led_slope_lumi = 1, led_slope_num = 1, led_slope_freq = 1;
char  send_but[100];
char humidity,temperature,ad;


int main(void)
{
	bsp_init();

	GUI_SetFont(&GUI_FontHZ_SimSun_12);
	GUI_Clear();
	menu_init();
	
	while(Key_Scan()==0);
	
	xTaskCreate((TaskFunction_t )start_task,
	            (const char*    )"start_task",
	            (uint16_t       )START_STK_SIZE,
	            (void*          )NULL,
	            (UBaseType_t    )START_TASK_PRIO,
	            (TaskHandle_t*  )&StartTask_Handler);


	vTaskStartScheduler();
	while(1);
}


void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();

	xTaskCreate((TaskFunction_t )mpu6050_task,
	            (const char*    )"mpu6050_task",
	            (uint16_t       )MPU6050_STK_SIZE,
	            (void*          )NULL,
	            (UBaseType_t    )MPU6050_TASK_PRIO,
	            (TaskHandle_t*  )&MPU6050Task_Handler);


	xTaskCreate((TaskFunction_t )led_task,
	            (const char*    )"led_task",
	            (uint16_t       )LED_STK_SIZE,
	            (void*          )NULL,
	            (UBaseType_t    )LED_TASK_PRIO,
	            (TaskHandle_t*  )&LedTask_Handler);

	xTaskCreate((TaskFunction_t )key_task,
	            (const char*    )"key_task",
	            (uint16_t       )KEY_STK_SIZE,
	            (void*          )NULL,
	            (UBaseType_t    )KEY_TASK_PRIO,
	            (TaskHandle_t*  )&KeyTask_Handler);

	xTaskCreate((TaskFunction_t )adconvert_task,
	            (const char*    )"adconvert_task",
	            (uint16_t       )ADC_STK_SIZE,
	            (void*          )NULL,
	            (UBaseType_t    )ADC_TASK_PRIO,
	            (TaskHandle_t*  )&AdConvertTask_Handler);

	xTaskCreate((TaskFunction_t )gui_task,
	            (const char*    )"gui_task",
	            (uint16_t       )GUI_STK_SIZE,
	            (void*          )NULL,
	            (UBaseType_t    )GUI_TASK_PRIO,
	            (TaskHandle_t*  )&GuiTask_Handler);
		    
	xTaskCreate((TaskFunction_t )transfer_task,
	            (const char*    )"transfer_task",
	            (uint16_t       )TRANS_STK_SIZE,
	            (void*          )NULL,
	            (UBaseType_t    )TRANS_TASK_PRIO,
	            (TaskHandle_t*  )&TransferTask_Handler);
		    
	xTaskCreate((TaskFunction_t )receive_task,
	            (const char*    )"receive_task",
	            (uint16_t       )RECEV_STK_SIZE,
	            (void*          )NULL,
	            (UBaseType_t    )RECEV_TASK_PRIO,
	            (TaskHandle_t*  )&ReceiveTask_Handler);

	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();
}

static void mpu6050_task(void *pvParameters)
{
	while(1) {
		if(app.dual_mode != 2)
			MPU_getdata();
		vTaskDelay(50);
	}
}

static void led_task(void *pvParameters)
{
	uint8_t i = 1;
	uint8_t horse = 0;
	uint8_t dir = 0;
	
	app.led_speed = 9;
//	Led_Startup_1();
	while(1) {
		if(menu.page_index_depth == 1 && menu.page_index == 0 && menu.sub_page_index == 0) {
			if(Key_Scan()==3)
			{
				horse = !horse;
			}
			if(app.led_speed > 0 && app.led_speed <= 9 && horse == 1) {
				if(i == 4)
					dir = 1;
				else if(i == 0)
					dir = 0;
				else;
				Led_Set_Status(i, led_off);
				if(dir == 0) {
					Led_Set_Status(i + 1, led_on);
					++i;
				}
				else {
					Led_Set_Status(i - 1, led_on);
					--i;
				}
				delay_ms(1000 - app.led_speed * 100);
			}
			else if(horse == 0){
				Led_Set_Status(1, led_off);
				Led_Set_Status(2, led_off);
				Led_Set_Status(3, led_off);
				Led_Set_Status(4, led_off);
				i = 1;
			}
		}
//		else if(menu.page_index_depth == 1 && menu.page_index == 1) {
//			Led_Set_Luminance(2, (fAX + 180) / 360 * 100);
//			Led_Set_Luminance(3, (fAY + 180) / 360 * 100);
//			Led_Set_Luminance(4, (fAZ + 180) / 360 * 100);
//		}
		else if(menu.page_index_depth == 1 && menu.page_index == 2){
			//printf("%c\r\n",USART_RX_BUF[0]);
			if(USART_RX_BUF[0] == '5' && USART_RX_BUF[1] =='M' && USART_RX_BUF[2] == 'H' && USART_RX_BUF[3] == 'M')
			{
				led_lumi = (USART_RX_BUF[4]-'0') * 10;
				USART_RX_BUF[0] = 0;
			}
			else if(USART_RX_BUF[0] == '5' && USART_RX_BUF[1] =='M' && USART_RX_BUF[2] == 'H' && USART_RX_BUF[3] == 'S')
			{
				led_freq = (USART_RX_BUF[4]-'0');
				USART_RX_BUF[0] = 0;
			}
			for(led0pwmval = 0; led0pwmval < led_lumi ; led0pwmval+= led_freq) 
			{			
				switch(led_num){
					case 0: 
						Led_Set_Status(1, led_off);
						Led_Set_Status(2, led_off);
						Led_Set_Status(3, led_off);
						Led_Set_Status(4, led_off);
						
						break;
					case 1:
						Led_Set_Luminance(1, led0pwmval);
						Led_Set_Status(2, led_off);
						Led_Set_Status(3, led_off);
						Led_Set_Status(4, led_off);
					
						break;
					case 2:
						Led_Set_Luminance(1, led0pwmval);
						Led_Set_Luminance(2, led0pwmval);
						Led_Set_Status(3, led_off);
						Led_Set_Status(4, led_off);
					
						break;
					case 3:
						Led_Set_Luminance(1, led0pwmval);
						Led_Set_Luminance(2, led0pwmval);
						Led_Set_Luminance(3, led0pwmval);
						Led_Set_Status(4, led_off);
					
						break;
					case 4:
						Led_Set_Luminance(1, led0pwmval);
						Led_Set_Luminance(2, led0pwmval);
						Led_Set_Luminance(3, led0pwmval);
						Led_Set_Luminance(4, led0pwmval);
				}

				delay_ms(20);
			}
			for(led0pwmval = 0; led0pwmval < led_lumi ; led0pwmval+= led_freq) {
				switch(led_num){
					case 0: 
						Led_Set_Status(1, led_off);
						Led_Set_Status(2, led_off);
						Led_Set_Status(3, led_off);
						Led_Set_Status(4, led_off);
					
						break;
					case 1:
						Led_Set_Luminance(1, led_lumi - led0pwmval);
						Led_Set_Status(2, led_off);
						Led_Set_Status(3, led_off);
						Led_Set_Status(4, led_off);
					
						break;
					case 2:
						Led_Set_Luminance(1, led_lumi - led0pwmval);
						Led_Set_Luminance(2, led_lumi - led0pwmval);
						Led_Set_Status(3, led_off);
						Led_Set_Status(4, led_off);
					
						break;
					case 3:
						Led_Set_Luminance(1, led_lumi - led0pwmval);
						Led_Set_Luminance(2, led_lumi - led0pwmval);
						Led_Set_Luminance(3, led_lumi - led0pwmval);
						Led_Set_Status(4, led_off);
					
						break;
					case 4:
						Led_Set_Luminance(1, led_lumi - led0pwmval);
						Led_Set_Luminance(2, led_lumi - led0pwmval);
						Led_Set_Luminance(3, led_lumi - led0pwmval);
						Led_Set_Luminance(4, led_lumi - led0pwmval);
				}

				delay_ms(20);
			}
			
			
			
		}
		else if(app.led_type)
			Led_Effect(app.led_type, app.led_speed, app.led_luminance * 10);
		else
			Led_Switch(app.led_luminance * 10, app.led);
		vTaskDelay(20);
	}
}

static void key_task(void *pvParameters)
{
	uint8_t key_sta = 0;
	while(1) {
		key_sta = Key_Scan();
		if(key_sta != 0) {
			if(menu.flagIsMainMenu)  {
				switch(key_sta) {
				case 1:
					if(menu.page_index < 4)
						++menu.page_index;
					else;
					
					break;
				case 2:
					if(menu.page_index > 0)
						--menu.page_index;
					else;
					
					break;

				case 3:
					menu.flagIsMainMenu = 0;
					++menu.page_index_depth;
					break;
				case 4:
					menu.page_index = 0;
					menu.sub_page_index = 0;
					break;
				default:
					break;
				}
			} else {
				switch(key_sta) {
				case 1:
					switch(menu.page_index) {
						case 0 :
							if(menu.sub_page_index < 3)
								++menu.sub_page_index;
							else;
							break;
							
						case 2 :
							if(led_slope_num)
								led_num ++;
							else
								led_num --;
							if(led_num == 4)
								led_slope_num = 0;
							else if(led_num == 0)
								led_slope_num = 1;
							break;
						
						case 3:
								ESP_TCP();
							break;

						case 4 :
							if(menu.sub_page_index < 1)
								++menu.sub_page_index;
							else;
							break;
							
						default :
							break;
						
					}
					break;
				case 2:
					switch(menu.page_index) {
						case 0 :
							if(menu.sub_page_index > 0)
								--menu.sub_page_index;
							else;
							break;
							
						case 2 :
							if(led_slope_freq)
								led_freq ++;
							else
								led_freq --;
							if(led_freq == 10)
								led_slope_freq = 0;
							else if(led_freq == 0)
								led_slope_freq = 1;
							break;
							
						case 3:
								ESP_Send_Cmd("TEST\r\n", "", 500);
							break;
							
						case 4 :
							if(menu.sub_page_index > 0)
								--menu.sub_page_index;
							else;
							break;
						default :
							break;
						
					}
					break;

				case 3:
					switch(menu.page_index) {
						case 1 :
							if(menu.sub_page_index < 1)
								++menu.sub_page_index;
							else
								--menu.sub_page_index;
							
						case 2 :
							if(led_slope_lumi == 1)
								led_lumi += 10;
							else
								led_lumi -= 10;
							if(led_lumi == 100)
								led_slope_lumi = 0;
							else if(led_lumi == 0)
								led_slope_lumi = 1;
							break;
							
						case 3 :
							ESP_TCP_quit();
							break;
							
						case 4 :
							if(onenet_init == 0)
							{		
								onenet_init = 2;
								ESP8266_QuitTrans();
								ESP8266_init();                                      //初始化ESP8266模块，连接WIFI,接入平台IP
								OneNet_DevLink(DEVID,APIKEY);                        //接入平台设备。
								onenet_init = 1;
							}
							
							break;
						default :
							break;
					}
					break;
				case 4:
					menu.sub_page_index = 0;
					menu.flagIsMainMenu = 1;
					menu.page_index_depth = 0;
					break;
				
				case 5:
					switch(menu.page_index) {
						case 2 :
							if(app.present_upload_interval < 100)
								app.present_upload_interval += 10;
							else if(app.present_upload_interval < 1000)
								app.present_upload_interval += 100;
							else
								app.present_upload_interval = 10;
							break;
							
						case 3 :
							
							break;
							
						case 4 :
						
							break;
							
						default :
							break;
					}
					break;
				
				case 6:
					switch(menu.page_index) {
						case 2 :
							if(app.present_upload_frame < 2)
								++app.present_upload_frame;
							else
								app.present_upload_frame = 0;
							break;
							
						case 3 :
							
							break;
							
						case 4 :
							
							break;
							
						default :
							break;
					}
					break;
				default:
					break;
				}
			}
			xTaskNotifyGive(GuiTask_Handler);
		}
		vTaskDelay(20);
	}
}

static void adconvert_task(void *pvParameters)
{
	while(1) {
		if(app.dual_mode != 2)
			app.ad_value = Get_Adc_Average(1, 5);
		vTaskDelay(20);
	}
}

static void transfer_task(void *pvParameters)
{
	char send_buffer[15];
	char send_buffer1[47];
	char send_buffer2[29];
	u8 temp_buffer = 0;
	char trans_buffer[2];
	char ax_buffer[6], ay_buffer[6], az_buffer[6];
	char gx_buffer[6], gy_buffer[6], gz_buffer[6];
	char pitch_buffer[6], roll_buffer[6], yaw_buffer[6];
	char voltage_buffer[4];
	u16 i;
	
	while(1) {
		
		memset(send_buffer, 0, 15);
		memset(send_buffer1, 0, 47);
		memset(send_buffer2, 0, 29);
		memset(ax_buffer, 0, 6);
		memset(ax_buffer, 0, 6);
		memset(ay_buffer, 0, 6);
		memset(az_buffer, 0, 6);
		memset(gx_buffer, 0, 6);
		memset(gy_buffer, 0, 6);
		memset(gz_buffer, 0, 6);
		memset(pitch_buffer, 0, 6);
		memset(roll_buffer, 0, 6);
		memset(yaw_buffer, 0, 6);
		memset(voltage_buffer, 0, 4);
		memset(trans_buffer, 0, 2);
		
		if(menu.page_index == 0 && menu.page_index_depth != 0)
		{
			send_buffer[0] = '5';
			send_buffer[1] = 'M';
			send_buffer[2] = 'B';
			send_buffer[3] = 'D';
			
			send_buffer[4] = (LED1 == 0) ? '1': '0';
			send_buffer[5] = (LED2 == 0) ? '1': '0';
			send_buffer[6] = (LED3 == 0) ? '1': '0';
			send_buffer[7] = (LED4 == 0) ? '1': '0';
			
			send_buffer[8]  = (KEY1 == 0) ? 'D': 'U';
			send_buffer[9]  = (KEY2 == 0) ? 'D': 'U';
			send_buffer[10] = (KEY3 == 0) ? 'D': 'U';
			send_buffer[11] = (KEY4 == 0) ? 'D': 'U';
			
			for(i = 0; i < 12; i++)
			{
				temp_buffer += send_buffer[i];
			}
			temp_buffer &= 0xFF;
			sprintf(trans_buffer, "%02X", temp_buffer);
			strcat(send_buffer, trans_buffer);
			
			send_buffer[14] = '\n';
			printf("%s",send_buffer);
			
			memset(trans_buffer, 0, 2);
			temp_buffer = 0;
			
			
			send_buffer1[0] = '5';
			send_buffer1[1] = 'M';
			send_buffer1[2] = 'Y';
			send_buffer1[3] = 'D';
			
			send_buffer1[4] = (app.ad_value/1000)+'0';
			send_buffer1[5] = (app.ad_value%1000)/100+'0';
			send_buffer1[6] = (app.ad_value%100)/10+'0';
			send_buffer1[7] = (app.ad_value%10)+'0';
			
			sprintf(ax_buffer, "%6d", ax);
			sprintf(ay_buffer, "%6d", ay);
			sprintf(az_buffer, "%6d", az);
			sprintf(gx_buffer, "%6d", gx);
			sprintf(gy_buffer, "%6d", gy);
			sprintf(gz_buffer, "%6d", gz);
			
			strcat(send_buffer1, ax_buffer);
			strcat(send_buffer1, ay_buffer);
			strcat(send_buffer1, az_buffer);
			strcat(send_buffer1, gx_buffer);
			strcat(send_buffer1, gy_buffer);
			strcat(send_buffer1, gz_buffer);
			
			for(i = 0; i < 44; i++)
			{
				temp_buffer += send_buffer[i];
			}
			temp_buffer &= 0xFF;
			sprintf(trans_buffer, "%02X", temp_buffer);
			strcat(send_buffer1, trans_buffer);
			
			send_buffer1[46] = '\n';
			printf("%s",send_buffer1);
			
			memset(trans_buffer, 0, 2);
			temp_buffer = 0;
			
			send_buffer2[0] = '5';
			send_buffer2[1] = 'M';
			send_buffer2[2] = 'R';
			send_buffer2[3] = 'D';
			
			send_buffer2[4] = (int)((app.ad_value / 4096.0) * 3.3) + '0';
			send_buffer2[5] = '.';
			send_buffer2[6] = ((int)((app.ad_value / 4096.0) * 33)%10) + '0';
			send_buffer2[7] = ((int)((app.ad_value / 4096.0) * 330)%10) + '0';
			
			sprintf(pitch_buffer, "%6.2f", fAX);
			sprintf(roll_buffer, "%6.2f", fAY);
			sprintf(yaw_buffer, "%6.2f", fAZ);
			
			strcat(send_buffer2, pitch_buffer); //8-13
			strcat(send_buffer2, roll_buffer); //14-19
			strcat(send_buffer2, yaw_buffer); //20-25
			
			for(i = 0; i < 26; i++)
			{
				temp_buffer += send_buffer[i];
			}
			temp_buffer &= 0xFF;
			sprintf(trans_buffer, "%02X", temp_buffer);
			strcat(send_buffer2, trans_buffer);
			
			send_buffer2[28] = '\n';
			printf("%s",send_buffer2);

		}
		
		if(menu.page_index == 3 && menu.page_index_depth == 1 && wifi_status == 0)
		{
			ESP8266_QuitTrans();
			ESP_Init();
		}
		else if(menu.page_index == 4 && menu.page_index_depth == 1 && onenet_init == 1)
		{
			//delay_ms(2000);
			memset(up_buffer, 0, 20);
			sprintf(up_buffer, "%.2f°", fAX);
			OneNet_SendData(send_but,"fAX",fAX);
			onenet_number++;
			memset(up_buffer, 0, 20);
			sprintf(up_buffer, "%.2f°", fAY);
			OneNet_SendData(send_but,"fAY",fAY);
			onenet_number++;
			memset(up_buffer, 0, 20);
			sprintf(up_buffer, "%.2f°", fAZ);
			OneNet_SendData(send_but,"fAZ",fAZ);
			onenet_number++;
			memset(up_buffer, 0, 20);
			sprintf(up_buffer, "%.2f", (app.ad_value / 4096.0) * 3.3);
			OneNet_SendData(send_but,"Voltage",(app.ad_value / 4096.0) * 3.3);
			onenet_number++;
			//delay_ms(2000);
		}
		vTaskDelay(20);
	}
}

static void receive_task(void *pvParameters)
{
	uint8_t parseRetVal = 0;
	while(1) {
		if(USART_RX_STA & 0X8000) {
			USART_RX_BUF[USART_RX_STA & 0X3FFF] = 0;
			parseRetVal = rcvDataParse(USART_RX_BUF);
			if(parseRetVal == 1) {
				app.recev_count += USART_RX_STA & 0X3FFF;
				app.present_recev_source = 1;
				USART_RX_STA = 0;
			}
			else if (parseRetVal == 2)
				USART_RX_STA = 0;
		}
		vTaskDelay(20);
	}
}

static void gui_task(void *pvParameters)
{
	menu_constructor_main_static();
	while(1) {
		if(menu.flagIsMainMenu) {
		}
		else {
			menu.present_constructor_dynamic = menu_constructors_dynamic[menu.page_index][menu.sub_page_index];
			menu.present_constructor_dynamic();
		}
		if(ulTaskNotifyTake(pdTRUE, 0) > 0) {
			if(menu.flagIsMainMenu) {
				menu_constructor_main_static();
			} else {
				menu.present_constructor_static = menu_constructors_static[menu.page_index][menu.sub_page_index];
				menu.present_constructor_static();
				
			}
			
		}
		GUI_Update();
		vTaskDelay(20);
	}
}




