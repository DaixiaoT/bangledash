#ifndef _CAN_IAP_H_
#define _CAN_IAP_H_

#include "iap_base.h"

/******************�����APP IAP��Ϣ����Ҫ�޸�*******************/

#ifdef _CP_V7_1_
#define IAP_ID (IAP_CHIP_STM32F407 | IAP_SYSTEM_AC | IAP_HVAC_V7_1 | IAP_HVAC_DEV_CPU)
#define IAP_VERSION 20210426


#endif





#define APP_ADDR     0x08010000
#define APP_OK_MARK  0x66778899


/************************************************************/

#ifdef __cplusplus
void iap_set_buf(U8 *buf);
void iap_set_mac(int bus, U8 mac);

//int iap_uart0_send(U32 id, U8 *buf, int len);

#define CHECK_IAP_USB 1
#define CHECK_IAP_CAN 2
#define CHECK_IAP_UDP 4

void CheckUart0Bus();

static inline void LoopCheckIAPBus(U32 flag,U32 timeout)
{
	U32 time=0;
	int start_time=sys_time();
	while(1) {
		if ((TimeGap(start_time)>timeout) && (timeout!=0))
			break;

		CheckUart0Bus();	
		sleep(2);
		if (TimeGap(time)>=2000)
		{
			time = sys_time();
			// LOG_PRINT("LoopCheckIAPBus %d\r\n",TimeGap(start_time));
		}
	}
}


#ifdef _USB_
void CheckUSBBus();
#endif
// ���ش��� ��CAN ���ߴ�����֡
void iap_process_up_bus(int bus, U32 iap_id, U8 *buf, int len);

// ���ش��� �����ϵ�IAP֡, bufǰ�������4�ֽڡ�
void iap_process_sub_bus(int bus, U32 iap_id, U8 *buf, int len);

// �豸������IAP֡
void dev_iap_process(int bus, U32 iap_id, U8 *buf, int len);

void wait_uart0_update();
void wait_uart0_can_update();

#endif // __cplusplus

#endif //_CAN_IAP_H_

