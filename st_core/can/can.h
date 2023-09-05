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

//���ܳ���tCAN, iCAN������������ȣ����ұ���<=64*7, ��Ƭ�����������������������Ҳ��6λiCan��������
#define CAN_SEG_NUM  35
#define CAN_MAX_PACKET_LEN (CAN_SEG_NUM * 8) // =280


// ����һ������򵥰�
// return value:
// =0, �ɹ�����
// <0, ������
int can_send_packet(int can, U32 id, U8 *buf, int len);

// ����һ������ѡһ�����е�TxBUF����
// return value:
// =0, �ɹ��������ͣ����done����!=0��������ɺ�ISR������done��־��1=�ɹ�,<0, ����
//     ���ͳ��������wait_flag()���ȴ������־
// <0, ������
//int can_send_packet(int can, U32 id, U8 *buf, int len, volatile int *done);

// ͬ�����Ͱ����ȴ��������
// return value:
// =0, �ɹ�����
// <0, ������
int can_send_packet_wait(int can, U32 id, U8 *buf, int len, int timeout);

// return >=0���յ��������ݳ���
//        <0, ����
// ��������ȴ�����������
// Ҫ��: buf������4�ֽڶ���(��2λ=0)��buf_len=8��>=CAN_MAX_PACKET_LEN��=8ֻ�յ�����>=CAN_MAX_PACKET_LENȫ��
int can_recv_packet(int can, U32 *id, U8 *buf, int buf_len);

void can_reset(int can);

// ����CAN���߲����ʣ�������can_init()ǰ����
//void can_set_bps(int can, U32 bps);
// ȡ���߲�����
//U32 can_bps(int can);

#define CAN1_ENABLE (1<<0)
#define CAN2_ENABLE (1<<1)

#define CAN1_SEG_ENABLE (1<<0)
#define CAN2_SEG_ENABLE (1<<1)

int can_init(U32 can_en_flag, U32 seg_flag);

// ���ñ�CAN�ڵĹ���������ֻ�з������������İ����ܱ����գ�
// (recvID&mask)==local || (recvID&mask)==local2 || (recvID&mask)==mask  //broadcast
// ����can driver����֪���ϲ�����ζ���mac�ģ�������mask�����θ���
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

