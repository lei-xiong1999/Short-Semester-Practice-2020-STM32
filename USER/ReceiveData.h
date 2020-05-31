#ifndef __RCVDATA_H
#define __RCVDATA_H

#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define CONNECT_START								0x01
#define CONNECT_ALREADY								0x02
#define CONNECT_DENIED								0x03
#define CONNECT_LOST								0x04

#define USR_STA_GET_OK							        0x01
#define USR_STA_GET_ERROR							0x02

#define SHIPMENT_MAX_QUEUE                                                      10
                                       

enum shipment_source {
        from_server = 1,
        from_rfidcard = 2
};

struct shipment_info {
        u8      rcv_sta;
        u8      shipment_sta;
        int     num;
        int     len;
        enum    shipment_source source;
        u8      service_id[40];
        u8      msg_id[2];
};

struct shipment_queue {
        u16                      nwait;
        struct shipment_info    *head;
        struct shipment_info    *tail;
};

extern struct shipment_info new_order;
extern struct shipment_queue ship_queue;

extern int reconnect_time;
extern u8 system_staus;
extern volatile u8 rcv_sta;

void shipment_queue_init(void);
void shipment_enqueue(const struct shipment_info *new_order);
void shipment_dequeue(void);
struct shipment_info shipment_gethead(void);
void ReceiveDataPacket(void);
void DealWithDataPacket(void);

#endif



