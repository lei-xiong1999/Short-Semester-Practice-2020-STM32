#include "STM32_I2C.h"

#define MPU6050_SCL		GPIO_Pin_6
#define MPU6050_SDA		GPIO_Pin_7
#define MPU6050_I2C_GPIO	GPIOB

#define MPU6050_SCL_H         GPIO_WriteBit(MPU6050_I2C_GPIO, MPU6050_SCL, Bit_SET)
#define MPU6050_SCL_L         GPIO_WriteBit(MPU6050_I2C_GPIO, MPU6050_SCL, Bit_RESET)

#define MPU6050_SDA_H         GPIO_WriteBit(MPU6050_I2C_GPIO, MPU6050_SDA, Bit_SET)
#define MPU6050_SDA_L         GPIO_WriteBit(MPU6050_I2C_GPIO, MPU6050_SDA, Bit_RESET)

#define MPU6050_SCL_read      GPIO_ReadInputDataBit(MPU6050_I2C_GPIO, MPU6050_SCL)
#define MPU6050_SDA_read      GPIO_ReadInputDataBit(MPU6050_I2C_GPIO, MPU6050_SDA)

static void I2C_delay(void)
{
    volatile int i = 7;
    while (i)
        i--;
}

static bool I2C_Start(void)
{
    MPU6050_SDA_H;
    MPU6050_SCL_H;
    I2C_delay();
    if (!MPU6050_SDA_read)
        return false;
    MPU6050_SDA_L;
    I2C_delay();
    if (MPU6050_SDA_read)
        return false;
    MPU6050_SDA_L;
    I2C_delay();
    return true;
}

static void I2C_Stop(void)
{
    MPU6050_SCL_L;
    I2C_delay();
    MPU6050_SDA_L;
    I2C_delay();
    MPU6050_SCL_H;
    I2C_delay();
    MPU6050_SDA_H;
    I2C_delay();
}

static void I2C_Ack(void)
{
    MPU6050_SCL_L;
    I2C_delay();
    MPU6050_SDA_L;
    I2C_delay();
    MPU6050_SCL_H;
    I2C_delay();
    MPU6050_SCL_L;
    I2C_delay();
}

static void I2C_NoAck(void)
{
    MPU6050_SCL_L;
    I2C_delay();
    MPU6050_SDA_H;
    I2C_delay();
    MPU6050_SCL_H;
    I2C_delay();
    MPU6050_SCL_L;
    I2C_delay();
}

static bool I2C_WaitAck(void)
{
    MPU6050_SCL_L;
    I2C_delay();
    MPU6050_SDA_H;
    I2C_delay();
    MPU6050_SCL_H;
    I2C_delay();
    if (MPU6050_SDA_read) {
        MPU6050_SCL_L;
        return false;
    }
    MPU6050_SCL_L;
    return true;
}

static void I2C_SendByte(uint8_t byte)
{
    uint8_t i = 8;
    while (i--) {
        MPU6050_SCL_L;
        I2C_delay();
        if (byte & 0x80)
            MPU6050_SDA_H;
        else
            MPU6050_SDA_L;
        byte <<= 1;
        I2C_delay();
        MPU6050_SCL_H;
        I2C_delay();
    }
    MPU6050_SCL_L;
}

static uint8_t I2C_ReceiveByte(void)
{
    uint8_t i = 8;
    uint8_t byte = 0;

    MPU6050_SDA_H;
    while (i--) {
        byte <<= 1;
        MPU6050_SCL_L;
        I2C_delay();
        MPU6050_SCL_H;
        I2C_delay();
        if (MPU6050_SDA_read) {
            byte |= 0x01;
        }
    }
    MPU6050_SCL_L;
    return byte;
}

void MPU6050_IIC_GPIO_Init(void)
{
    	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = MPU6050_SCL | MPU6050_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MPU6050_I2C_GPIO, &GPIO_InitStruct);
}

/////////////////////////////////////////////////////////////////////////////////
int8_t i2cwrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
	if(i2cWriteBuffer(addr,reg,len,data))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	//return FALSE;
}
int8_t i2cread(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	if(i2cRead(addr,reg,len,buf))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	//return FALSE;
}
//////////////////////////////////////////////////////////////////////////////////
bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
{
    if (!I2C_Start())
        return false;
    I2C_SendByte(addr << 1 | I2C_Direction_Transmitter);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return false;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_SendByte(data);
    I2C_WaitAck();
    I2C_Stop();
    return true;
}

bool i2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
    int i;
    if (!I2C_Start())
        return false;
    I2C_SendByte(addr << 1 | I2C_Direction_Transmitter);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return false;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    for (i = 0; i < len; i++) {
        I2C_SendByte(data[i]);
        if (!I2C_WaitAck()) {
            I2C_Stop();
            return false;
        }
    }
    I2C_Stop();
    return true;
}

bool i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!I2C_Start())
        return false;
    I2C_SendByte(addr << 1 | I2C_Direction_Transmitter);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return false;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(addr << 1 | I2C_Direction_Receiver);
    I2C_WaitAck();
    while (len) {
        *buf = I2C_ReceiveByte();
        if (len == 1)
            I2C_NoAck();
        else
            I2C_Ack();
        buf++;
        len--;
    }
    I2C_Stop();
    return true;
}

uint16_t i2cGetErrorCounter(void)
{
    // TODO maybe fix this, but since this is test code, doesn't matter.
    return 0;
}
