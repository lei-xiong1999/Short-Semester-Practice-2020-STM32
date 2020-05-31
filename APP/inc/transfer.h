#ifndef __TRANSFER_H
#define __TRANSFER_H

#include "app.h"

uint8_t uploadBasicStatus(void);
uint8_t uploadSamplingData(uint16_t adval, short mpu_ax, short mpu_ay, short mpu_az, short mpu_gx, short mpu_gy, short mpu_gz);
uint8_t uploadProcessedData2App(uint16_t adval, float mpu_fAX, float mpu_fAY, float mpu_fAZ);
uint8_t uploadProcessedData2Slave(uint16_t adval, float mpu_fAX, float mpu_fAY, float mpu_fAZ);
uint8_t uploadBlueToothName(uint8_t *name);
uint8_t requestBlueToothName(void);
uint8_t rcvDataParse(uint8_t *rcv_buff);

#endif

