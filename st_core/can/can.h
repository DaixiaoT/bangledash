#ifndef _CAN_H_
#define _CAN_H_

struct CanPacket {
	U32 id;
	union {
		U8  buf[8];
		U32 data[2];
	};
	int len;
};

#define CAN_NUM 2

//不能超过tCAN, iCAN允许的最大包长度；并且必须<=64*7, 分片乱序重组允许的最大包长。这也是6位iCan最大包长。
#define CAN_SEG_NUM  35
#define CAN_MAX_PACKET_LEN (CAN_SEG_NUM * 8) // =280


// 发送一个大包或单包
// return value:
// =0, 成功发送
// <0, 错误码
int can_send_packet(int can, U32 id, U8 *buf, int len);

// 发送一个包，选一个空闲的TxBUF发送
// return value:
// =0, 成功启动发送，如果done参数!=0，发送完成后，ISR会设置done标志，1=成功,<0, 错误
//     发送程序可以用wait_flag()来等待这个标志
// <0, 错误码
//int can_send_packet(int can, U32 id, U8 *buf, int len, volatile int *done);

// 同步发送包，等待发送完成
// return value:
// =0, 成功发送
// <0, 错误码
int can_send_packet_wait(int can, U32 id, U8 *buf, int len, int timeout);

// return >=0，收到包的数据长度
//        <0, 错误
// 函数不会等待，立即返回
// 要求: buf必须在4字节对齐(低2位=0)，buf_len=8或>=CAN_MAX_PACKET_LEN，=8只收单包，>=CAN_MAX_PACKET_LEN全收
int can_recv_packet(int can, U32 *id, U8 *buf, int buf_len);

void can_reset(int can);

// 设置CAN总线波特率，必须在can_init()前调用
//void can_set_bps(int can, U32 bps);
// 取总线波特率
//U32 can_bps(int can);

#define CAN1_ENABLE (1<<0)
#define CAN2_ENABLE (1<<1)

#define CAN1_SEG_ENABLE (1<<0)
#define CAN2_SEG_ENABLE (1<<1)

int can_init(U32 can_en_flag, U32 seg_flag);

// 设置本CAN口的过滤条件：只有符合以下条件的包才能被接收：
// (recvID&mask)==local || (recvID&mask)==local2 || (recvID&mask)==mask  //broadcast
// 由于can driver并不知道上层是如何定义mac的，所以用mask来屏蔽更灵活。
void can_set_filter(int can, U32 mask, U32 local, U32 local2); //, U32 broadcast)

#define E_CAN_TX           -2
#define E_CAN_PACKET_LEN   -3
#define E_CAN_TX_BUF_FULL  -4
#define E_CAN_RX_BUF_EMPTY -5
#define E_CAN_BUF_LEN      -6
#define E_CAN_BUF_ALIGN    -7
#define E_CAN_PACKET_NOTEXIST  -8
#define E_CAN_TIMEOUT      -15
#define E_CAN_BUS          -16
#define E_CAN_OTHER        -17
#define E_CAN_PORT_IS_USED_BY_SEG_FLAG -18
#define E_CAN_INIT         -19

#endif //_CAN_H_

