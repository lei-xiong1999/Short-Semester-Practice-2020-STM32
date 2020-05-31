#ifndef __APP_H
#define __APP_H

#include "bsp.h"
#include "menu.h"
#include "transfer.h"
#include "receive.h"
#include "onenetconnet.h"

#define student_id	"1538"

typedef struct {
	enum led_status led[4];
	uint8_t led_speed;
	uint8_t led_type;
	uint8_t led_luminance;
	
	uint16_t ad_value;
	
	uint8_t present_upload_frame;
	uint16_t present_upload_interval;
	int 	upload_count;
	
	uint8_t present_recev_type;
	uint8_t present_recev_source;
	int 	recev_count;
	
	uint8_t pre_dual_mode;
	uint8_t dual_mode;
	char dual_name[20];
	int handshake_count;
	
	uint8_t remote_switch;
	uint16_t remote_second;
	uint8_t remote_dir;
	
	char remote_x_now;
	char remote_y_now;
	char remote_x_ex;
	char remote_y_ex;
}app_status_t;

extern u16 led_lumi, led_num, led_freq, led_slope_lumi, led_slope_num, led_slope_freq ;
extern u16 wifi_status;
extern u16 wifi_err;
extern menu_t menu;
extern app_status_t app;
extern u8 onenet_init;
extern int onenet_number;
extern u8 usart2_flag;

#endif
