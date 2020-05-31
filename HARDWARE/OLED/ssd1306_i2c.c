#include "ssd1306_i2c.h"
#include "delay.h"
#include "bsp.h"


#define OLED_SCL		GPIO_Pin_10
#define OLED_SDA		GPIO_Pin_11
#define OLED_GPIO		GPIOB

#define OLED_MPU6050_SCL_H         	GPIO_WriteBit(OLED_GPIO, OLED_SCL, Bit_SET)
#define OLED_MPU6050_SCL_L         	GPIO_WriteBit(OLED_GPIO, OLED_SCL, Bit_RESET)

#define OLED_MPU6050_SDA_H         	GPIO_WriteBit(OLED_GPIO, OLED_SDA, Bit_SET)
#define OLED_MPU6050_SDA_L         	GPIO_WriteBit(OLED_GPIO, OLED_SDA, Bit_RESET)

#define OLED_SCL_read     	 GPIO_ReadInputDataBit(OLED_GPIO, OLED_SCL)
#define OLED_SDA_read      	GPIO_ReadInputDataBit(OLED_GPIO, OLED_SDA)

void OLED_IIC_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = OLED_SCL | OLED_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(OLED_GPIO, &GPIO_InitStruct);
}

void OLED_IIC_Start(void)
{
	OLED_MPU6050_SCL_H;
	OLED_MPU6050_SDA_H;
	
	OLED_MPU6050_SDA_L;
	OLED_MPU6050_SCL_L;
}
void OLED_IIC_Stop(void)
{
	OLED_MPU6050_SCL_L;
	OLED_MPU6050_SDA_L;
	OLED_MPU6050_SDA_H;
	OLED_MPU6050_SCL_H;	
}
void OLED_IIC_SendByte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		OLED_MPU6050_SCL_L;
		if(dat&0x80)
			OLED_MPU6050_SDA_H;
		else
			OLED_MPU6050_SDA_L;
		dat<<=1;
		OLED_MPU6050_SCL_H;
			
	}
	OLED_MPU6050_SCL_L;
	OLED_MPU6050_SCL_H;
}