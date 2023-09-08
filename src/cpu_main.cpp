#include "ac_include.h"
#include "BSP_IODrv.h"
#include "build_defs.h"
#include "RTC.h"
#include "SST26VF064B.h"
#include "MB85RS256B.h"
#include "miIP.h"
#include "miip_udp.h"
#include "filter.h"
#include "ac_api.h"
#include "ai_update.h"
#include "maintenance.h"
#include "led.h"
#include "comm_base.h"
#include "can_iap.h"
#define S_BUF0_MAX 2048
static U8 __ALIGN4__ s_buf0[IAP_H_LEN + S_BUF0_MAX + IAP_T_LEN]; // ����CAN
static U8 *s_buf = s_buf0 + IAP_H_LEN;

//��ʼ������ͨ��ϵͳ
void InitCanBus(U32 id)
{
#ifdef _CAN_
    can_set_bps(CAN_FLOOR0, CAN_FLOOR_BTR);//�������߲�����
    can_set_bps(CAN_FLOOR1, CAN_FLOOR_BTR);

    can_init(CAN1_ENABLE | CAN2_ENABLE, CAN1_SEG_ENABLE | CAN2_SEG_ENABLE);//��ʼ��CAN1��CAN2ͨ��ͨ��

	can_set_mac(CAN_FLOOR0, id, IAP_PC_MAC);//����CANͨ�������ַ
	iap_set_mac(CAN_FLOOR0, id);

	can_set_mac(CAN_FLOOR1, id, IAP_PC_MAC); // ��PC_ADDR����������COMM����IAPӦ���
	iap_set_mac(CAN_FLOOR1, id);
#endif

	iap_set_buf(s_buf);
}


void dead_loop();
int iap_udp_init();
void iap_udp_poll();
static int uart3_timer_proc(U32 data)
{
	RS485_TX_OFF();
	return 0;
}
static int uart4_timer_proc(U32 data)
{
	RS485_UART4_TX_OFF(); //!!!!
	return 0;
}
static int uart6_timer_proc(U32 data)
{
	RS485_UART6_TX_OFF(); //!!!!

	return 0;
}

void UART3_TX_BEGIN()
{
	RS485_TX_ON();
}
void UART3_TX_END()
{
	Locked_SetTimer(UART3_TIMER, 4000, uart3_timer_proc, 0);
}
void UART4_TX_END()
{
	Locked_SetTimer(UART4_TIMER, 4000, uart4_timer_proc, 0);
}

void UART4_TX_BEGIN()
{
	RS485_UART4_TX_ON();
}
void UART6_TX_BEGIN()
{
	RS485_UART6_TX_ON();
}
void UART6_TX_END()
{
	Locked_SetTimer(UART6_TIMER, 4000, uart6_timer_proc, 0); // >=700
}

static void wait_break()
{
	for(int i=0; i < 6; i++) {
		LOG_PRINT("*");
		sleep(200);
		char c;
		int ret = Uart0_TryRead(&c, 1);
		if(ret == 1 && (c == '\r' || c == '\n')) {
			LOG_PRINT("\r\n+++++++ac_base break++++++++\r\n");
			LoopCheckIAPBus(CHECK_IAP_USB,60*1000);
		}
	}
}

void cpu_root(void *)
{
	InitCanBus(0);
	LED_RUN_ON(); // ��ָʾ��
	Time_t t;
	RTC_Init(&t);			 // ��ʼ��ʵʱʱ�ӣ�������ʼ�������Ϣ�洢��t������
	RTC_print(&t);			 // ��ӡRTC��ʱ����Ϣ
	wait_break();
	int ret = mi_mem_init(); // �ڴ�����ʼ����
	if (ret)
		dead_loop(); // ���������ڴ��ʼ��ʧ�ܵ����

#ifdef _MIIP_
	ret = miip_init(); // ��ʼ����̫��ͨ�����
	if (ret)
		dead_loop();
	iap_udp_init();							   // ��ʼ��UDPͨ��
	g_ptu_udp.miip_udp_info_reg(UDP_PTU_PORT); // ע��UDPͨ����Ϣ
#endif
	SpiFlashInit(); // ��ʼ��SPI Flash�洢��
#ifdef _SPI_FRAM_

	SpiFramInit(); // ��ʼ��SPI FRAM�洢��
#endif
	filter_init();	  // ��ʼ��������
	BSP_IODrv_init(); // ��ʼ�������������

#ifdef _HW_TEST_
	hardware_test(); // ִ��Ӳ������
#endif

	sys_log_init(); // ��ʼ��ϵͳ��־
	ac_init();		// ��ʼ���յ��豸

	BEEP(100);

#ifdef _WDT_
	WDEX_ON(); // �򿪿��Ź���ʱ��
#endif

	U32 wdt_time = 0;
	U32 lastTime = sys_time();
	U32 run_sec = 0;

	while (1)
	{
		sleep(1);
#ifdef _WDT_
		if (TimeGap(wdt_time) >= 100) // ������ϴ�ι��������ʱ�����Ƿ����100����ֹ���Ź���ʱ������ϵͳ
		{
			wdt_time = sys_time();
			WDEX_FEED();
		}
#endif
		CheckUart0Bus(); // ��鴮������״̬

#ifdef _MIIP_
		iap_udp_poll();		  // ��ѯUDPͨ��
#endif						  // _MIIP_
		BSP_IODrv_update();	  // ���������������״̬
		ai_update();		  // ģ�����������
		maintenance_update(); // ��ptu��������

		if (TimeGap(lastTime) >= LED_RUN_SWITCH_TIME)
		{
			run_sec++;
			set_led(run_sec & 0x0F);
			LED_RUN_SWITCH();
			lastTime = sys_time();
			LOG_PRINT("CPU run...%d\r\n", run_sec);
			os_stack_check();
		}

		//iap_udp_poll(); // ���ʹ�����UDPͨ����ص��������緢�ͻ����UDP���ݰ�

		if ((ret = g_ptu_udp.check_udp_data(s_buf, S_BUF0_MAX)) > 0) // ���ڼ���Ƿ���UDP���ݰ�����
		{
			// BEEP(50);
			maintenance_uart_recv(MAINTENANCE_NET, s_buf, ret); // ����ptu���󣬷��Ͷ�Ӧ����
		}
		
		
		if (TimeGap(g_down_log_tick) > 5000){
			ac_control(run_sec);
			
		}
			
			
	}
}
