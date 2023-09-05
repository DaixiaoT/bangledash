#ifndef __COMM_BASE_H__
#define __COMM_BASE_H__
/*************** IAP UART ******************************/
#define IAP_UART_PACKET_START_CODE		0xF8
#define IAP_UART_PACKET_END_CODE		0xF9
// IAP包在串口中封装的格式：0xF8 + len(2B) + ID(4B) + data + CRC(2B) + 0xF9
// CRC = len+id+data的CRC
// MSB格式

/*************** IAP CMD ******************************/
#ifdef _ICAN_
#define IAP_CMD_SEARCH    0xF0 // 扫描设备帧
#define IAP_CMD_DEVINFO   0x10 // 设备信息帧
#define IAP_CMD_UPDATE    0x20 // 升级设备帧
#define IAP_CMD_READ_DATA 0x30 // 读数据块帧
#define IAP_CMD_DATA      0x40 // 发送数据块帧
#define IAP_CMD_END       0x50 // 升级结束帧
#define IAP_CMD_CLEAR_VECTOR 0xD0  // 清vector
#define IAP_CMD_APP       0xA0    // 走IAP协议的应用包, PC->设备
#define IAP_CMD_APP_ACK   0xB0    // 设备对IAP_CMD_APP的应答包
#define IAP_CMD_UPDATE_DATA 0xC0  // 写FLASH，类似于IAP_CMD_UPDATE
#else
// 高5位，IAP_CMD，并且第7位必须为1。低3位，bus层数
#define IAP_CMD_SEARCH    0xF8    // 扫描设备帧
#define IAP_CMD_DEVINFO   0x88    // 设备信息帧
#define IAP_CMD_UPDATE    0x90    // 升级设备帧
#define IAP_CMD_READ_DATA 0x98    // 读数据块帧
#define IAP_CMD_DATA      0xA0    // 发送数据块帧
#define IAP_CMD_END       0xA8    // 升级结束帧
#define IAP_CMD_CLEAR_VECTOR 0xB0 // 清vector
#define IAP_CMD_APP       0xB8    // 走IAP协议的应用包, PC->设备
#define IAP_CMD_APP_ACK   0xC0    // 设备对IAP_CMD_APP的应答包
#define IAP_CMD_UPDATE_DATA 0xC8  // 写FLASH，类似于IAP_CMD_UPDATE
#endif // _ICAN_

/*************** IAP BASE ******************************/
// IAP buffer前面、后面必须保留的空间；用于在串口、网口封装CAN IAP帧时使用
#define IAP_H_LEN 16
#define IAP_T_LEN 16

//#define IAP_PC_MAC  0xFE // PC IAP 软件地址，固定为0xFE
#define IAP_MARK         0x59a2f803

// ID 最高位=1，BOOT, =0, APP
#define IAP_ID_MASK 0x7FFFFFFF
// ID 第30位=1， BigEndian, =0, LittleEndian
#define IAP_ID_BigEndianBit 0x40000000
#define IAP_ID_BootBit      0x80000000

#define IAP_ID_BOOT (IAP_ID | IAP_ID_BootBit)

/*************** IAP CHIP ******************************/
// 24-29位，为设备的芯片型号
enum IAP_CHIP
{
	IAP_CHIP_STM32F407 = (0x10 << 24),

};

/*************** IAP SYSTEM ****************************/
enum IAP_SYSTEM
{

	IAP_SYSTEM_AC = (0x1 << 16),	// 空调系统
	
};


/*************** IAP HVAC DEVICE**********************/
enum HVAC_DEVICE
{
	IAP_HVAC_DEV_HMI = 4,					// HMI
	IAP_HVAC_DEV_CPU = 0,					// 主板

};

/*************** IAP HVAC PROJECT ********************/
enum HVAC_PROJECT
{
	IAP_HVAC_V7_1 = (0x10 << 8),  //坦桑尼亚和孟加拉
};


#endif//__COMM_BASE_H__





