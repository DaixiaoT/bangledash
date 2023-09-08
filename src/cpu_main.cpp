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
static U8 __ALIGN4__ s_buf0[IAP_H_LEN + S_BUF0_MAX + IAP_T_LEN]; // 接收CAN
static U8 *s_buf = s_buf0 + IAP_H_LEN;

//初始化总线通信系统
void InitCanBus(U32 id)
{
#ifdef _CAN_
    can_set_bps(CAN_FLOOR0, CAN_FLOOR_BTR);//设置总线波特率
    can_set_bps(CAN_FLOOR1, CAN_FLOOR_BTR);

    can_init(CAN1_ENABLE | CAN2_ENABLE, CAN1_SEG_ENABLE | CAN2_SEG_ENABLE);//初始化CAN1和CAN2通信通道

	can_set_mac(CAN_FLOOR0, id, IAP_PC_MAC);//设置CAN通信物理地址
	iap_set_mac(CAN_FLOOR0, id);

	can_set_mac(CAN_FLOOR1, id, IAP_PC_MAC); // 设PC_ADDR，用来接收COMM卡的IAP应答包
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
	LED_RUN_ON(); // 打开指示灯
	Time_t t;
	RTC_Init(&t);			 // 初始化实时时钟，并将初始化后的信息存储在t变量中
	RTC_print(&t);			 // 打印RTC的时间信息
	wait_break();
	int ret = mi_mem_init(); // 内存管理初始化，
	if (ret)
		dead_loop(); // 用来处理内存初始化失败的情况

#ifdef _MIIP_
	ret = miip_init(); // 初始化以太网通信组件
	if (ret)
		dead_loop();
	iap_udp_init();							   // 初始化UDP通信
	g_ptu_udp.miip_udp_info_reg(UDP_PTU_PORT); // 注册UDP通信信息
#endif
	SpiFlashInit(); // 初始化SPI Flash存储器
#ifdef _SPI_FRAM_

	SpiFramInit(); // 初始化SPI FRAM存储器
#endif
	filter_init();	  // 初始化过滤器
	BSP_IODrv_init(); // 初始化输入输出驱动

#ifdef _HW_TEST_
	hardware_test(); // 执行硬件测试
#endif

	sys_log_init(); // 初始化系统日志
	ac_init();		// 初始化空调设备

	BEEP(100);

#ifdef _WDT_
	WDEX_ON(); // 打开看门狗定时器
#endif

	U32 wdt_time = 0;
	U32 lastTime = sys_time();
	U32 run_sec = 0;

	while (1)
	{
		sleep(1);
#ifdef _WDT_
		if (TimeGap(wdt_time) >= 100) // 检查自上次喂狗以来的时间间隔是否大于100，防止看门狗定时器重置系统
		{
			wdt_time = sys_time();
			WDEX_FEED();
		}
#endif
		CheckUart0Bus(); // 检查串口总线状态

#ifdef _MIIP_
		iap_udp_poll();		  // 轮询UDP通信
#endif						  // _MIIP_
		BSP_IODrv_update();	  // 更新输入输出驱动状态
		ai_update();		  // 模拟量输入更新
		maintenance_update(); // 给ptu发送数据

		if (TimeGap(lastTime) >= LED_RUN_SWITCH_TIME)
		{
			run_sec++;
			set_led(run_sec & 0x0F);
			LED_RUN_SWITCH();
			lastTime = sys_time();
			LOG_PRINT("CPU run...%d\r\n", run_sec);
			os_stack_check();
		}

		//iap_udp_poll(); // 检查和处理与UDP通信相关的任务，例如发送或接收UDP数据包

		if ((ret = g_ptu_udp.check_udp_data(s_buf, S_BUF0_MAX)) > 0) // 用于检查是否有UDP数据包到达
		{
			// BEEP(50);
			maintenance_uart_recv(MAINTENANCE_NET, s_buf, ret); // 接收ptu请求，发送对应数据
		}
		
		
		if (TimeGap(g_down_log_tick) > 5000){
			ac_control(run_sec);
			
		}
			
			
	}
}
