#ifndef _CAN_ID_H_
#define _CAN_ID_H_

#define TID_20A  0x80000000

#ifdef _ICAN_
/*********************************
 ICAN Э��
**********************************/
// 29λID���䣺
// 0-7λ�� Port
// 8-11λ��func
// 12λ:   ack
// 13-20λ��dst mac
// 21-28λ��src mac

#define TID_PORT_POS  0
#define TID_PORT_MASK 0xFF

#define TID_FUNC_POS   8
#define TID_FUNC_MASK 0xF

#define TID_ACK_POS   12

#define TID_DST_POS   13
#define TID_SRC_POS   21
#define TID_MAC_MASK  0xFF

#define CAN_FIRST_SEG  0x40 // ��һ����Ƭ��־ 0100 0000
#define CAN_MIDDLE_SEG 0x80 // �м��Ƭ��־   1000 0000
#define CAN_LAST_SEG   0xC0 // ����Ƭ��־   1100 0000

#define CAN_SEG_FLAG_MASK  0xC0  // ��Ƭ��־mask, ��2λ

#define CAN_SEG_MASK       0x3F // ��Ƭmask, ��6λ

/* FUNC_ID */
#define FUNC_WRITE			0x01
#define FUNC_READ			0x02
#define FUNC_HEART 			0x03
#define FUNC_CONNECT		0x04
#define FUNC_DEL_CONNECT	0x05
#define FUNC_RESET			0x06
#define FUNC_CHECK_MACID	0x07

#define FUNC_IAP 			0x0A        //��������0x08~0x0e������IAP���
#define FUNC_ERROR			0x0f        //�ظ��쳣

//#define TID_IAP_BIT (1<<15)
//#define IAP_PC_MAC  0x3E // PC IAP �����ַ���̶�Ϊ0x3E (6λ��ַ)
#define IAP_PC_MAC  0xFE // PC IAP �����ַ���̶�Ϊ0xFE

#else // _ICAN_

/*********************************
 TCAN Э�飺
**********************************/
// 29λID���䣺
// 0-7λ�� ��16λ=1ʱ����Ϊ��Ƭ��־������user�Զ���, ���Դ�������
// 8-15λ��func����15λ=1����IAP��������7λ����4λ=IAP_CMD*����3λ��bus����
//               ��15λ=0������7λuser�Զ���
// 17-22λ��dst mac
// 23-28λ��src mac
// 16λ��  ��Ƭ��־��1=��Ƭ

#define TID_PORT_POS  0
#define TID_PORT_MASK 0xFF

#define TID_SEG_MASK   0x7F // ��Ƭ��mask, ��7λ
#define TID_LAST_SEG   0x80 // ����Ƭ��־   1000 0000

#define TID_FUNC_POS   8
#define TID_FUNC_MASK 0xFF

#define TID_IAP_POS   15
#define TID_IAP_BIT (1<<TID_IAP_POS)

#define TID_SEG_POS   16
#define TID_SEG_BIT (1<<TID_SEG_POS)

#define TID_DST_POS   17
#define TID_SRC_POS   23
#define TID_MAC_MASK  0x3F

#define IAP_PC_MAC  0x3E // PC IAP �����ַ���̶�Ϊ0x3E (6λ��ַ)

#endif // _ICAN_

#define TID_MAC_BROADCAST TID_MAC_MASK


static inline U32 MakeTID(U32 src, U32 dst, U32 func, U32 port=0)
{
    return ((src&TID_MAC_MASK)<<TID_SRC_POS) | \
           ((dst&TID_MAC_MASK)<<TID_DST_POS) | \
           ((func&TID_FUNC_MASK)<<TID_FUNC_POS) | \
           ((port&TID_PORT_MASK)<<TID_PORT_POS);
}

#define TID_Src(id)  (((id)>>TID_SRC_POS)&TID_MAC_MASK)
#define TID_Dst(id)  (((id)>>TID_DST_POS)&TID_MAC_MASK)
#define TID_Func(id) (((id)>>TID_FUNC_POS)&TID_FUNC_MASK)
#define TID_Port(id) (((id)>>TID_PORT_POS)&TID_PORT_MASK)

static inline void can_set_mac(int can, U32 mac1, U32 mac2=0)
{
    if(mac2==0)
        mac2 = mac1;

    if (0xffffffff==mac1)
    {
		can_set_filter(can,
					   0,
					   (mac1 & TID_MAC_MASK) << TID_DST_POS,
					   (mac2 & TID_MAC_MASK) << TID_DST_POS);
    }
    else
    {
		can_set_filter(can,
					   TID_MAC_MASK << TID_DST_POS,
					   (mac1 & TID_MAC_MASK) << TID_DST_POS,
					   (mac2 & TID_MAC_MASK) << TID_DST_POS);
    }
}


#endif //_CAN_ID_H_

