#include "transfer.h"

uint8_t flagSendName = 0;

uint8_t uploadBasicStatus()
{
	uint8_t dataBuffer[16];
	uint8_t ledStatusBuffer[5];
	uint8_t keyStatusBuffer[5];
	
	sprintf((char *)ledStatusBuffer, "%s%s%s%s", ((LED1 == 0) ? "0" : "1"), ((LED2 == 0) ? "0" : "1"), ((LED3 == 0) ? "0" : "1"), ((LED4 == 0) ? "0" : "1"));
	sprintf((char *)keyStatusBuffer, "%s%s%s%s", ((KEY1 == 0) ? "D" : "U"), ((KEY2 == 0) ? "D" : "U"), ((KEY3 == 0) ? "D" : "U"), ((KEY4 == 0) ? "D" : "U"));
	
	sprintf((char *)dataBuffer, "7MBD%4s%4s\n", ledStatusBuffer, keyStatusBuffer);
	USART2_SendString(dataBuffer);
	printf("%s", dataBuffer);
	return strlen((const char *)dataBuffer);
}

uint8_t uploadSamplingData(uint16_t adval, short mpu_ax, short mpu_ay, short mpu_az, short mpu_gx, short mpu_gy, short mpu_gz)
{
	uint8_t dataBuffer[50];
	
	sprintf((char *)dataBuffer, "7MY,%d,%d,%d,%d,%d,%d,%d\n", adval, mpu_ax, mpu_ay, mpu_az, mpu_gx, mpu_gy, mpu_gz);
	USART2_SendString(dataBuffer);
	printf("%s", dataBuffer);
	return strlen((const char *)dataBuffer);
}

uint8_t uploadProcessedData2App(uint16_t adval, float mpu_fAX, float mpu_fAY, float mpu_fAZ)
{
	uint8_t dataBuffer[50];
	
	USART2_RX_STA = 0;
	sprintf((char *)dataBuffer, "7MR,%.1f,%.1f,%.1f,%.1f\n", (3.3 * adval / 4096), mpu_fAX, mpu_fAY, mpu_fAZ);
	USART2_SendString(dataBuffer);
	printf("%s", dataBuffer);
	return strlen((const char *)dataBuffer);
}

uint8_t uploadProcessedData2Slave(uint16_t adval, float mpu_fAX, float mpu_fAY, float mpu_fAZ)
{
	uint8_t dataBuffer[50];
	
	USART2_RX_STA = 0;
	sprintf((char *)dataBuffer, "7MSR,%.2f,%.2f,%.2f,%.2f\n", (3.3 * adval / 4096), mpu_fAX, mpu_fAY, mpu_fAZ);
	app.upload_count += strlen((const char *)dataBuffer);
	USART2_SendString(dataBuffer);
	
	return HC05_Wait_Ack("O", 50);
}

uint8_t uploadBlueToothName(uint8_t *name)
{
	uint8_t dataBuffer[50];
	
	USART2_RX_STA = 0;
	sprintf((char *)dataBuffer, "7MBN%s\n", name);
	app.upload_count += strlen((const char *)dataBuffer);
	USART2_SendString(dataBuffer);
	
	return HC05_Wait_Ack("O", 50);
}

uint8_t requestBlueToothName(void)
{
	USART2_RX_STA = 0;
	USART2_SendString("BT+NAME?");
	
	return HC05_Wait_Ack("O", 50);
}

uint8_t rcvDataParse(uint8_t *rcv_buff)
{
	long long time_ms;
	uint8_t interval[4] = {0};
	uint8_t stampTime[11] = {0};
	uint8_t proData[4][7] = {0};
	uint8_t i = 0, j = 0;
	float ad_valtage = 0;
	
	uint8_t *pDataBuf = rcv_buff;
	/* ÅÐ¶ÏÖ¡Í· */
	if (strstr((char *)pDataBuf, "7M")) {
		pDataBuf += 2;
		
		/* ÅÐ¶ÏÖ¡Í· */
		if (*pDataBuf == '+') {
			++pDataBuf;
			app.led_type = 0;
			app.present_recev_type = 1;
			while(*pDataBuf != '\n') {
				if(*pDataBuf >= '1' && *pDataBuf <= '4')
					app.led[*pDataBuf - '1'] = led_on;
				++pDataBuf;
			}
		}
		else if (*pDataBuf == '-') {
			++pDataBuf;
			app.led_type = 0;
			app.present_recev_type = 1;
			while(*pDataBuf != '\n') {
				if(*pDataBuf >= '1' && *pDataBuf <= '4')
					app.led[*pDataBuf - '1'] = led_off;
				++pDataBuf;
			}
		}
		else if (*pDataBuf == '*') {
			++pDataBuf;
			app.led_type = 0;
			app.present_recev_type = 1;
			for(i = 0; i < 4 && pDataBuf[i] != '\n'; i++) {
				if(pDataBuf[i] == '1')
					app.led[i] = led_on;
				else if(pDataBuf[i] == '0')
					app.led[i] = led_off;
			}
		}
		else if ((*pDataBuf == 'L') && (*(pDataBuf + 1) == 'M')) {
			pDataBuf += 2;
			app.present_recev_type = 2;
			if(*pDataBuf >= '1' && *pDataBuf <= '3')
				app.led_type = *pDataBuf - '0';
			else;
		}
		else if ((*pDataBuf == 'L') && (*(pDataBuf + 1) == 'S')) {
			pDataBuf += 2;
			app.present_recev_type = 2;
			if(*pDataBuf >= '0' && *pDataBuf <= '9')
				app.led_speed = *pDataBuf - '0';
			else;
		}
		else if ((*pDataBuf == 'H') && (*(pDataBuf + 1) == 'M')) {
			pDataBuf += 2;
			app.present_recev_type = 3;
			if(*pDataBuf >= '1' && *pDataBuf <= '9')
				app.led_luminance = *pDataBuf - '0';
			else;
		}
		else if ((*pDataBuf == 'U') && (*(pDataBuf + 1) == 'S')) {
			pDataBuf += 2;
			app.present_recev_type = 4;
			for(i = 0; i < 3 && pDataBuf[i] != '\n'; i++) {
				if(pDataBuf[i] >= '0' && pDataBuf[i] <= '9')
					interval[i] = pDataBuf[i];
			}
			sscanf((const char *)interval, "%hd", &app.present_upload_interval);
			app.present_upload_interval *= 10;
			if(pDataBuf[i] != '\n') {
				switch(pDataBuf[i]) {
					case 'Y':
						app.present_upload_frame = 1;
						break;
					case 'R':
						app.present_upload_frame = 2;
						break;
					case 'B':
						if(pDataBuf[i + 1] == 'D')
							app.present_upload_frame = 0;
						break;
					default:
						break;
				}
			}
		}
		else if ((*pDataBuf == 'B') && (*(pDataBuf + 1) == 'N')) {
			pDataBuf += 2;
			++app.handshake_count;
			for(i = 0; i < 20 && pDataBuf[i] != '\n'; i++)
				app.dual_name[i] = pDataBuf[i];
			
			USART2_SendString("O");
		}
		else if((*pDataBuf == 'S') && (*(pDataBuf + 1) == 'R')) {
			pDataBuf += 3;
			++app.handshake_count;
			for(i = 0; i < 4; i++) {
				for(j = 0; j < 7; j++) {
					if(*pDataBuf != ',' && *pDataBuf != '\n') {
						proData[i][j] = *pDataBuf;
						++pDataBuf;
					}
					else {
						if(i == 0) {
							sscanf((const char *)proData[i], "%f", &ad_valtage);
							app.ad_value = (uint16_t)(ad_valtage / 3.3 * 4096);
						}
						else if(i == 1)
							sscanf((const char *)proData[i], "%f", &fAX);
						else if(i == 2)
							sscanf((const char *)proData[i], "%f", &fAY);
						else if(i == 3)
							sscanf((const char *)proData[i], "%f", &fAZ);
						++pDataBuf;
						break;
					}
				}
			}
			USART2_SendString("O");
		}
		else if ((*pDataBuf == 'B') && (*(pDataBuf + 1) == 'R')) {
			if(app.remote_switch == 1) {
				pDataBuf += 2;
				if(*pDataBuf != '\n') {
					switch(*pDataBuf) {
						case '1' :
							if(app.remote_y_ex - 5 > 0)
								app.remote_y_now -= 5;
							else
								app.remote_y_now = 0;
							break;
							
						case '2' :
							if(app.remote_y_ex + 5 < 28)
								app.remote_y_now += 5;
							else
								app.remote_y_now = 28;
							break;
							
						case '3' :
							if(app.remote_x_ex - 5 > 0)
								app.remote_x_now -= 5;
							else
								app.remote_x_now = 0;
							break;
							
						case '4' :
							if(app.remote_x_ex + 5 < 113)
								app.remote_x_now += 5;
							else
								app.remote_x_now = 113;
							break;
							
						default :
							break;
					}
				}
			}
		}
		else if ((*pDataBuf == 'S') && (*(pDataBuf + 1) == 'T')) {
			pDataBuf += 2;
			for(i = 0; i < 11 && pDataBuf[i] != '\n'; i++) 
				stampTime[i] = pDataBuf[i];
			sscanf((const char *)stampTime, "%lld", &time_ms);
		}
		return 1;
	}
	else if(strstr((char *)pDataBuf, "BT+")) {
		++app.handshake_count;
		USART2_SendString("O");
		return 2;
	}
	else if(strstr((char *)pDataBuf, "OK") || strstr((char *)pDataBuf, "O"))
		return 0;
	else
		return 2;
}


