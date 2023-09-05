#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

#define BOOT_FLAG   0x397F

#define SYS_LOG_SECTOR_MASK   0x1FFFF  // 128K sector

#define SYS_LOG_START_SECTOR  FLASH_Sector_7
#define SYS_LOG_LAST_SECTOR   FLASH_Sector_11
#define SYS_LOG_START_ADDR    ADDR_FLASH_SECTOR_7
#define SYS_LOG_END_ADDR     (ADDR_FLASH_SECTOR_11 + 128*1024)

#define SYS_LOG_FLASH_SIZE   (5*128*1024)
#define SYS_LOG_SIZE         32  // ϵͳ��¼��С��16/32/64�ֽ�
#define SYS_LOG_NUM          (SYS_LOG_FLASH_SIZE / SYS_LOG_SIZE)

// filter����:
#define FILT_BUF_LEN	16
#define FILT_OBJ_NUM    20
#define FILT_AI_BASE    0 // 0~3, 4���¶�
//3.27 Req.92558.SRS.027-3.2.3
#define MY_IP4_ADDR    0xC0A80109
#define MY_IP4_NETMASK 0xFFFFFF00
#define MY_IP4_GW      0xC0A80101

// CAN ��������
#if 1
/* sys_clk=168M, APB1_clk=42 MHZ
1Mbps = 42/[(1+TS1+TS2)*BRP]
�̶�: TS1=9tq, TS2=4tq. ��: BRP = 3 * (1Mhz/bps)
500k=2, 250k=4, 125k=8, 40k=25
*/
#define CAN_BS1_val CAN_BS1_9tq
#define CAN_BS2_val CAN_BS2_4tq
#define CAN1_Prescaler_val (3*8) // 40k    (25=1000/40bps)
#define CAN2_Prescaler_val (3*8) // 40k	 (25=1000/40bps)
#endif

#define CAN_BUS_1	    0
#define CAN_BUS_2	    1



#ifdef _CAN_
#define CAN_RX_BUF_SIZE 128 // ������2��ָ��, 64/128/256
#define CAN_TX_BUF_SIZE 256 // ������2�ı���, 128/256/512
#define CAN_MERGE_CONTROL_NUM (CAN_NUM * 8)
#endif

#define UART0_RX_BUF_SIZE (512 * 4)
#define UART0_TX_BUF_SIZE (512 * 4)
//#define UART0_TX_DMA_SIZE (256)
#define UART0_RX_DMA_SIZE (128)

#ifdef _UART1_
#define UART1_RX_BUF_SIZE (512 * 1)
#define UART1_TX_BUF_SIZE (512 * 1)
#define UART1_USR_INIT()
#define UART1_TX_BEGIN()
#define UART1_TX_END()
#endif

#ifdef _UART2_
#define UART2_RX_BUF_SIZE (512 * 4)
#define UART2_TX_BUF_SIZE (512 * 1)
#define UART2_USR_INIT()
#define UART2_TX_BEGIN()
#define UART2_TX_END()
#endif

#ifdef _UART3_
#define UART3_RX_BUF_SIZE (512 * 1)
#define UART3_TX_BUF_SIZE (512 * 1)
#define UART3_RX_DMA_SIZE (128)
#define UART3_USR_INIT() InitTimer(UART3_TIMER, NVIC_UART3_PRIO)
void UART3_TX_BEGIN();
void UART3_TX_END();
#endif


#ifdef _UART4_
#define UART4_RX_BUF_SIZE (512 * 1)
#define UART4_TX_BUF_SIZE (512 * 1)
#define UART4_RX_DMA_SIZE (128)
#define UART4_USR_INIT() InitTimer(UART4_TIMER, NVIC_UART4_PRIO)
void UART4_TX_BEGIN();
void UART4_TX_END();

//#define UART4_TX_BEGIN()    RS485_UART4_TX_ON()//UART4_TX_BEGIN_DELAY(4)//gpioSetValue(2,6,1); //sleep(5)
//#define UART4_TX_BEGIN_DELAY(ms) {RS485_UART4_TX_ON(); sleep(ms);}
#endif


#ifdef _UART6_
#define UART6_RX_BUF_SIZE (512 * 1)
#define UART6_TX_BUF_SIZE (512 * 1)
#define UART6_RX_DMA_SIZE (128)
#define UART6_USR_INIT() InitTimer(UART6_TIMER, NVIC_UART6_PRIO)
void UART6_TX_BEGIN();
void UART6_TX_END();
#endif


#define RTC_ISL1208_WRITE		I2C1_Write
#define RTC_ISL1208_LOCK		I2C1_Lock
#define RTC_ISL1208_UnLOCK		I2C1_Unlock
#define	RTC_Locked_I2C_Request	Locked_I2C1_Request


/*
*3.5���ַ���ʱ������ֻ������RTUģʽ���棬��ΪRTUģʽû�п�ʼ���ͽ�������
�������ݰ�֮��ֻ�ܿ�ʱ���������֣�Modbus�����ڲ�ͬ�Ĳ������£����ʱ���ǲ�һ���ģ�
���Ծ���3.5���ַ���ʱ�䣬�����ʸߣ����ʱ������С�������ʵͣ����ʱ������Ӧ�ʹ�
*����ͨ�Ų������й�(Modbus�ǲ��ô���ͨ�ŵ�)����ÿ���źŲ��ΰ���һ��������λ������£��������ǵ��ڱ����ʵģ���1baud=1bit/s��
1���ַ���RTUģʽ������8λ��������ʽ����ģ���3.5���ַ�ʱ�����3.5*10(1bitstart + 8bitdata + 1bit end)��bit�����ʱ�䡣
������ѡ��Ĳ�����Ϊ9600baud��������ÿ���źŲ��ΰ���һ��������λ������9600bit/s����ô1bit����Ĵ���ʱ�����1/9600�룬����3.5���ַ�ʱ��=3.5*10*1/9600�롣
**/
#define UART_LAST_BYTE_TIME	(int)((100000 / COM4_BAUD_RATE) +1)	//��λ0.1ms   Լ0.3ms ���һ���ֽ�ʱ��    1000(����)/38400(������)*10(һ���ֽ�10bit  1 start +8 data +1 end)
#define MODBUS_3_5_BYTE_TIME	(int)((350000/COM4_BAUD_RATE)+1)  //��λ0.1ms Լ1ms


#define BKPSRAM_ADDR     0x40024000


// ����CPU����IO��ͨѶ����0
#define CAN_FLOOR0	    0
// ����CPU����IO��ͨѶ����1
#define CAN_FLOOR1	    1
//#define CAN_FLOOR_BTR   BPS_500K


#define I2C1_TIMER  4//_BTIM4_  ����
#define UART3_TIMER 2//_BTIM2_  ����
#define UART6_TIMER 3//_BTIM3_  ����
#define UART4_TIMER 5  //_BTIM5_  ����
#define TIMER4_SLOT_MODBUS_RX_TIMEOUT	6
//3.22 Req.92558.SRS.022-3.2.2
//3.23 Req.92558.SRS.023-3.2.2

//����0������
#define COM0_BAUD_RATE 115200 
//����0������  ISP
#define ISP_BAUD_RATE 115200

//����4�����ʣ�RS485
#define COM4_BAUD_RATE 19200 //Req.92558.SRS.003-3.1.1.4
#define USART4_Parity	USART_Parity_Even	// Req.92558.SRS.004-3.1.1.4

//����3�����ʣ�RS485
#define COM3_BAUD_RATE 9600
//#define COM3_LCR       0x02 // 7bit

#define COM6_BAUD_RATE 9600

#define ROOT_THREAD_PRIO 102

//extern const unsigned long  update_cfg[12];

#define PRINT_ERR_STR(str)  LOG_PRINT("ERROR: %s In %s at Line %u - %s\r\n",__FILE__ , __FUNCTION__, __LINE__,str);
#define PRINT_ERR_INT(n)  LOG_PRINT("ERROR: %s In %s at Line %u - %d\r\n",__FILE__ , __FUNCTION__, __LINE__,n);

//#ifdef _CPU_
#include "cpu_sys_config.hh"
//#endif

#define AI_UPDATE_TIME		100
#define DOI_UPDATE_TIME		100//ʵ����100ms��
#define DOI_UPDATE_SHARING_TIME		10//10msΪDI��ʱ�ɼ�

#endif // _SYS_CONFIG_H_
