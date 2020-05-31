#ifndef __EDP_KIT_H__
#define __EDP_KIT_H__

#include "Common.h"

/*---------------------------------------------------------------------------*/
#define MOSQ_MSB(A)         (uint8)((A & 0xFF00) >> 8)
#define MOSQ_LSB(A)         (uint8)(A & 0x00FF)
#define BUFFER_SIZE         (0x100) 
#define PROTOCOL_NAME       "EDP"
#define PROTOCOL_VERSION    1

/*----------------------------错误码-----------------------------------------*/
#define ERR_UNPACK_CONNRESP_REMAIN              -1000
#define ERR_UNPACK_CONNRESP_FLAG                -1001
#define ERR_UNPACK_CONNRESP_RTN                 -1002
#define ERR_UNPACK_PUSHD_REMAIN                 -1010
#define ERR_UNPACK_PUSHD_DEVID                  -1011
#define ERR_UNPACK_PUSHD_DATA                   -1012
#define ERR_UNPACK_SAVED_REMAIN                 -1020
#define ERR_UNPACK_SAVED_TANSFLAG               -1021
#define ERR_UNPACK_SAVED_DEVID                  -1022
#define ERR_UNPACK_SAVED_DATAFLAG               -1023
#define ERR_UNPACK_SAVED_JSON                   -1024
#define ERR_UNPACK_SAVED_PARSEJSON              -1025
#define ERR_UNPACK_SAVED_BIN_DESC               -1026
#define ERR_UNPACK_SAVED_PARSEDESC              -1027
#define ERR_UNPACK_SAVED_BINLEN                 -1028
#define ERR_UNPACK_SAVED_BINDATA                -1029
#define ERR_UNPACK_PING_REMAIN                  -1030
#define ERR_UNPACK_CMDREQ                       -1031
#define ERR_UNPACK_ENCRYPT_RESP                 -1032
#define ERR_UNPACK_SAVEDATA_ACK                 -1033

/*----------------------------消息类型---------------------------------------*/
/* 连接请求 */
#define CONNREQ             0x10
/* 连接响应 */
#define CONNRESP            0x20
/* 转发(透传)数据 */
#define PUSHDATA            0x30
/* 存储(转发)数据 */
#define SAVEDATA            0x80
/* 存储确认 */
#define SAVEACK             0x90
/* 命令请求 */
#define CMDREQ              0xA0
/* 命令响应 */
#define CMDRESP             0xB0
/* 心跳请求 */
#define PINGREQ             0xC0
/* 心跳响应 */
#define PINGRESP            0xD0
/* 加密请求 */
#define ENCRYPTREQ          0xE0
/* 加密响应 */
#define ENCRYPTRESP         0xF0

#ifndef NULL
#define NULL (void*)0
#endif

/* SAVEDATA消息支持的格式类型 */
typedef enum {
    kTypeFullJson = 0x01,
    kTypeBin = 0x02,
    kTypeSimpleJsonWithoutTime = 0x03,
    kTypeSimpleJsonWithTime = 0x04,
    kTypeString = 0x05
}SaveDataType;

/*-------------发送buffer, 接收buffer, EDP包结构定义-------------------------*/
typedef struct Buffer
{
    uint8*  _data;          /* buffer数据 */
    uint32  _write_pos;     /* buffer写入位置 */
    uint32  _read_pos;      /* buffer读取位置 */
    uint32  _capacity;      /* buffer容量 */
}Buffer, SendBuffer, RecvBuffer, EdpPacket;
/*-----------------------------操作Buffer的接口------------------------------*/
/* 
 * 函数名:  NewBuffer
 * 功能:    生成Buffer
 * 说明:    一般情况下, NewBuffer和DeleteBuffer应该成对出现
 * 参数:    无
 * 返回值:  类型 (Buffer*)
 *          返回值非空 生成Buffer成功, 返回这个Buffer的指针
 *          返回值为空 生成Buffer失败, 内存不够
 */
Buffer* NewBuffer(void);
/* 
 * 函数名:  DeleteBuffer
 * 功能:    销毁Buffer
 * 说明:    一般情况下, NewBuffer和DeleteBuffer应该成对出现
 * 参数:    buf     一个Buffer的指针的指针
 * 返回值:  无
 */
void DeleteBuffer(Buffer** buf);
/* 
 * 函数名:  CheckCapacity
 * 功能:    检查Buffer是否能够写入长度为len的字节流, 
 *          如果Buffer的容量不够, 自动成倍扩展Buffer的容量(不影响Buffer数据)
 * 参数:    buf     需要写入Buffer的指针
 *          len     期望写入的长度
 * 返回值:  类型 (int32)
 *          <0      失败, 内存不够
 *          =0      成功
 */
int32 CheckCapacity(Buffer* buf, uint32 len);


/*-----------------------------客户端操作的接口------------------------------*/
/* 
 * 函数名:  EdpPacketType 
 * 功能:    获取一个EDP包的消息类型, 客户程序根据消息类型做不同的处理
 * 相关函数:Unpack***类函数
 * 参数:    pkg         EDP协议包
 * 返回值:  类型 (uint8)
 *          值          消息类型(详细参见本h的消息类型定义)
 */
/* 例子:
 * ...
 * int8 mtype = EdpPacketType(pkg);
 * switch(mtype)
 * {
 *  case CONNRESP:
 *      UnpackConnectResp(pkg);
 *      break;
 *  case PUSHDATA:
 *      UnpackPushdata(pkg, src_devid, data, data_len);
 *      break;
 *  case SAVEDATA:
 *      UnpackSavedata(pkg, src_devid, flag, data);
 *      break;
 *  case PINGRESP:
 *      UnpackPingResp(pkg); 
 *      break;
 *  ...
 * }
 */
uint8 EdpPacketType(EdpPacket* pkg);
/* 
 * 函数名:  PacketConnect1 
 * 功能:    打包 由设备到设备云的EDP协议包, 连接设备云的请求(登录认证方式1)
 * 说明:    返回的EDP包发送给设备云后, 需要客户程序删除该包
 *          设备云会回复连接响应给设备
 * 相关函数:UnpackConnectResp
 * 参数:    devid       设备ID, 申请设备时平台返回的ID
 *          auth_key    鉴权信息(api-key), 在平台申请的可以操作该设备的api-key字符串
 * 返回值:  类型 (EdpPacket*) 
 *          非空        EDP协议包
 *          为空        EDP协议包生成失败 
 */
EdpPacket* PacketConnect1(const char* devid, const char* auth_key);

/* 
 * 函数名:  PacketConnect2 
 * 功能:    打包 由设备到设备云的EDP协议包, 连接设备云的请求(登录认证方式2)
 * 说明:    返回的EDP包发送给设备云后, 需要客户程序删除该包
 *          设备云会回复连接响应给设备
 * 相关函数:UnpackConnectResp
 * 参数:    userid      用户ID, 在平台注册账号时平台返回的用户ID
 *          auth_info   鉴权信息, 在平台申请设备时填写设备的auth_info属性
 *                      (json对象字符串), 该属性需要具备唯一性
 * 返回值:  类型 (EdpPacket*) 
 *          非空        EDP协议包
 *          为空        EDP协议包生成失败 
 */
EdpPacket* PacketConnect2(const char* userid, const char* auth_info);

/* 
 * 函数名:  PacketPushdata
 * 功能:    打包 设备到设备云的EDP协议包, 设备与设备之间转发数据
 * 说明:    返回的EDP包发送给设备云后, 需要删除这个包
 * 相关函数:UnpackPushdata
 * 参数:    dst_devid   目的设备ID
 *          data        数据
 *          data_len    数据长度
 * 返回值:  类型 (EdpPacket*) 
 *          非空        EDP协议包
 *          为空        EDP协议包生成失败 
 */
EdpPacket* PacketPushdata(const char* dst_devid, 
        const char* data, uint32 data_len);

/* 
 * 函数名:  UnpackPushdata
 * 功能:    解包 由设备云到设备的EDP协议包, 设备与设备之间转发数据
 * 说明:    接收设备云发来的数据, 通过函数GetEdpPacket和EdpPacketType判断出是pushdata后, 
 *          将整个响应EDP包作为参数, 由该函数进行解析 
 *          返回的源设备ID(src_devid)和数据(data)都需要客户端释放
 * 相关函数:PacketPushdata, GetEdpPacket, EdpPacketType
 * 参数:    pkg         EDP包, 必须是pushdata包
 *          src_devid   源设备ID
 *          data        数据
 *          data_len    数据长度
 * 返回值:  类型 (int32) 
 *          =0          解析成功
 *          <0          解析失败, 具体失败原因见本h文件的错误码
 */
int32 UnpackPushdata(EdpPacket* pkg, char** src_devid, 
        char** data, uint32* data_len);


/* 
 * 函数名:  PacketSavedataSimpleString
 * 功能:    打包 设备到设备云的EDP协议包, 存储数据(以分号分隔的简单字符串形式)
 * 说明:    返回的EDP包发送给设备云后, 需要删除这个包
 * 相关函数:UnpackSavedataSimpleString
 * 参数:    dst_devid   目的设备ID
 *          input       以分号分隔的简单字符串形式，
 *                      详见《设备终端接入协议2-EDP.docx》
 * 返回值:  类型 (EdpPacket*) 
 *          非空        EDP协议包
 *          为空        EDP协议包生成失败 
 */
EdpPacket* PacketSavedataSimpleString(const char* dst_devid, const char* input);

/* 
 * 函数名:  PacketCmdResp
 * 功能:    向接入机发送命令响应
 * 说明:    返回的EDP包发送给设备云后, 需要客户程序删除该包
 *          
 * 相关函数:UnpackCmdReq
 * 参数:    cmdid       命令id
 *          cmdid_len   命令id长度
 *          resp        响应的消息
 *          resp_len    响应消息长度
 * 返回值:  类型 (EdpPacket*) 
 *          非空        EDP协议包
 *          为空        EDP协议包生成失败 
 */
EdpPacket* PacketCmdResp(const char* cmdid, uint16 cmdid_len, 
        const char* resp, uint32 resp_len);

/* 
 * 函数名:  UnpackCmdReq
 * 功能:    解包 由设备云到设备的EDP协议包, 命令请求消息
 * 说明:    接收设备云发来的数据, 解析命令请求消息包
 *          获取的cmdid以及req需要在使用后释放。
 * 相关函数:PacketCmdResp
 * 参数:    pkg         EDP包
 *          cmdid       获取命令id
 *          cmdid_len   cmdid的长度
 *          req         用户命令的起始位置
 *          req_len     用户命令的长度
 * 返回值:  类型 (int32) 
 *          =0          解析成功
 *          <0          解析失败, 具体失败原因见本h文件的错误码
 */
int32 UnpackCmdReq(EdpPacket* pkg, char** cmdid, uint16* cmdid_len, 
			      char** req, uint32* req_len);


EdpPacket* PacketSaveJson(const char* dst_devid, char* json_obj, SaveDataType type);

EdpPacket* PacketSaveBin(const char* dst_devid, char* desc_obj, unsigned int bin_len);


#endif /* __EDP_KIT_H__ */
