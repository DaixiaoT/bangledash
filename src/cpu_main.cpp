// #ifdef _CPU_
#include "sys_api.h"
#include "can_iap.h"
#include "BSP_IODrv.h"
#include "ai_update.h"
#include "filter.h"
#include "build_defs.h"
#include "RTC.h"

#include "MB85RS256B.h"
#include "miIP.h"
#include "miip_udp.h"
#define S_BUF0_MAX 2048
static U8 __ALIGN4__ s_buf0[IAP_H_LEN + S_BUF0_MAX + IAP_T_LEN]; // 接收CAN
static U8 *s_buf = s_buf0 + IAP_H_LEN;

// static U32 s_can_time[CAN_NUM];
// static BOOL s_can_ok[CAN_NUM];

void InitCanBus(U32 id)
{
#ifdef _CAN_
	can_set_bps(CAN_FLOOR0, CAN_FLOOR_BTR);
	can_set_bps(CAN_FLOOR1, CAN_FLOOR_BTR);

	can_init(CAN1_ENABLE | CAN2_ENABLE, CAN1_SEG_ENABLE | CAN2_SEG_ENABLE);

	can_set_mac(CAN_FLOOR0, id, IAP_PC_MAC);
	iap_set_mac(CAN_FLOOR0, id);

	can_set_mac(CAN_FLOOR1, id, IAP_PC_MAC); // 设PC_ADDR，用来接收COMM卡的IAP应答包
	iap_set_mac(CAN_FLOOR1, id);
#endif

	iap_set_buf(s_buf);
}

#ifndef _BOOT_

void dead_loop();
int iap_udp_init();
void iap_udp_poll();

static U8 s_log_buf[4096];

static int uart3_timer_proc(U32 data)
{
	RS485_TX_OFF(); //!!!!
	return 0;
}

void UART3_TX_END()
{
	Locked_SetTimer(UART3_TIMER, 4000, uart3_timer_proc, 0); // >=700
}

void UART3_TX_BEGIN()
{
	RS485_TX_ON();
}

static int uart6_timer_proc(U32 data)
{
	RS485_UART6_TX_OFF(); //!!!!

	return 0;
}

void UART6_TX_END()
{
	Locked_SetTimer(UART6_TIMER, 4000, uart6_timer_proc, 0); // >=700
}

void UART6_TX_BEGIN()
{
	RS485_UART6_TX_ON();
}

static int uart4_timer_proc(U32 data)
{
	RS485_UART4_TX_OFF(); //!!!!
	return 0;
}

void UART4_TX_END()
{
	Locked_SetTimer(UART4_TIMER, 4000 /*UART_LAST_BYTE_TIME*100*/, uart4_timer_proc, 0); // >=700
}

void UART4_TX_BEGIN()
{
	RS485_UART4_TX_ON();
	// LOG_PRINT("UART4_TX_BEGIN()\r\n");
}

static void wait_break()
{
	
}

void cpu_root(void *)
{


}

#endif //_BOOT_
