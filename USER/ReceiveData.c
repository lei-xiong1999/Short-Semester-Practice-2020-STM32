#include "bsp.h"

extern TaskHandle_t MotorTask_Handler;
extern TaskHandle_t ReconnectTask_Handler;

struct shipment_info new_order;
struct shipment_info shipment_content[SHIPMENT_MAX_QUEUE];
struct shipment_queue ship_queue;
u8 system_staus;
int reconnect_time;
volatile u8 rcv_sta;

void shipment_queue_init()
{
        memset(&ship_queue, 0, sizeof(struct shipment_queue));
        ship_queue.head = shipment_content;
        ship_queue.tail = shipment_content;
        xSemaphoreShipment = xSemaphoreCreateMutex();
}

void shipment_enqueue(const struct shipment_info *new_order)
{
        xSemaphoreTake(xSemaphoreShipment, portMAX_DELAY);
        if(ship_queue.nwait < SHIPMENT_MAX_QUEUE) {
                memcpy((void *)ship_queue.tail, (const void *)new_order, sizeof(struct shipment_info));
                ++ship_queue.tail;
                ++ship_queue.nwait;
        }
        else;
        xSemaphoreGive(xSemaphoreShipment);
}

void shipment_dequeue()
{
        u8 i;
        
        if(ship_queue.nwait > 0) {
                xSemaphoreTake(xSemaphoreShipment, portMAX_DELAY);
                for(i = 0; i < ship_queue.nwait; i++) {
                        if(i < SHIPMENT_MAX_QUEUE-1)
                                shipment_content[i] = shipment_content[i+1];
                        else
                                memset(&shipment_content[i], 0, sizeof(struct shipment_info));
                }
                --ship_queue.nwait;
                --ship_queue.tail;
                xSemaphoreGive(xSemaphoreShipment);        
        }
        else;
}

struct shipment_info shipment_gethead()
{
        return *(ship_queue.head);
}
                
void ReceiveDataPacket()
{
	char *index_head = NULL;
        char *index_tail = NULL;
        char buf[5] = {0, 0, 0, 0, 0};
        if(strstr((const char *)network.buffer, (const char *)card_num) && strstr((const char *)network.buffer, "num") && strstr((const char *)network.buffer, "len") && strstr((const char *)network.buffer, ",\"os\"")) {
                delay_ms(10);
                printf("get shipment, index : %d\r\n", NETWORK_RX_STA);
                index_head = strstr((const char *)network.buffer, (const char *)card_num);
                index_tail = strstr((const char *)network.buffer, "{");
                if(16 == (index_tail - index_head)) {
                         new_order.msg_id[0] = 0;
                         new_order.msg_id[1] = index_head[15];
                }
                else {
                        new_order.msg_id[0] = index_head[15];
                        new_order.msg_id[1] = index_head[16];
                }
        
                index_head = strstr((const char *)network.buffer, "num");
                index_tail = strstr((const char *)network.buffer, ",\"len\"");

                index_head += 5;
                strncpy(buf, index_head, index_tail-index_head);
                new_order.num = atoi(buf);
                
                index_head = strstr((const char *)index_tail, "len");
                index_tail = strstr((const char *)index_tail, ",\"os\"");
          
                index_head += 5;
                strncpy(buf, index_head, index_tail-index_head);
                new_order.len = atoi(buf);
               
                index_head = strstr((const char *)index_tail, ",\"os\"");
                index_head += 7;
                strncpy(new_order.service_id, index_head, 32);
                printf("shipment number : %s\r\n", new_order.service_id);
                new_order.rcv_sta = USR_STA_GET_OK;
                memset(network.buffer, 0, NETWORK_BUFFER_SIZE);
                NETWORK_RX_STA = 0;

        }       

}





