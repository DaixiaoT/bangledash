#ifndef __COMM_BASE_H__
#define __COMM_BASE_H__
/*************** IAP UART ******************************/
#define IAP_UART_PACKET_START_CODE		0xF8
#define IAP_UART_PACKET_END_CODE		0xF9
// IAP���ڴ����з�װ�ĸ�ʽ��0xF8 + len(2B) + ID(4B) + data + CRC(2B) + 0xF9
// CRC = len+id+data��CRC
// MSB��ʽ

/*************** IAP CMD ******************************/
#ifdef _ICAN_
#define IAP_CMD_SEARCH    0xF0 // ɨ���豸֡
#define IAP_CMD_DEVINFO   0x10 // �豸��Ϣ֡
#define IAP_CMD_UPDATE    0x20 // �����豸֡
#define IAP_CMD_READ_DATA 0x30 // �����ݿ�֡
#define IAP_CMD_DATA      0x40 // �������ݿ�֡
#define IAP_CMD_END       0x50 // ��������֡
#define IAP_CMD_CLEAR_VECTOR 0xD0  // ��vector
#define IAP_CMD_APP       0xA0    // ��IAPЭ���Ӧ�ð�, PC->�豸
#define IAP_CMD_APP_ACK   0xB0    // �豸��IAP_CMD_APP��Ӧ���
#define IAP_CMD_UPDATE_DATA 0xC0  // дFLASH��������IAP_CMD_UPDATE
#else
// ��5λ��IAP_CMD�����ҵ�7λ����Ϊ1����3λ��bus����
#define IAP_CMD_SEARCH    0xF8    // ɨ���豸֡
#define IAP_CMD_DEVINFO   0x88    // �豸��Ϣ֡
#define IAP_CMD_UPDATE    0x90    // �����豸֡
#define IAP_CMD_READ_DATA 0x98    // �����ݿ�֡
#define IAP_CMD_DATA      0xA0    // �������ݿ�֡
#define IAP_CMD_END       0xA8    // ��������֡
#define IAP_CMD_CLEAR_VECTOR 0xB0 // ��vector
#define IAP_CMD_APP       0xB8    // ��IAPЭ���Ӧ�ð�, PC->�豸
#define IAP_CMD_APP_ACK   0xC0    // �豸��IAP_CMD_APP��Ӧ���
#define IAP_CMD_UPDATE_DATA 0xC8  // дFLASH��������IAP_CMD_UPDATE
#endif // _ICAN_

/*************** IAP BASE ******************************/
// IAP bufferǰ�桢������뱣���Ŀռ䣻�����ڴ��ڡ����ڷ�װCAN IAP֡ʱʹ��
#define IAP_H_LEN 16
#define IAP_T_LEN 16

//#define IAP_PC_MAC  0xFE // PC IAP �����ַ���̶�Ϊ0xFE
#define IAP_MARK         0x59a2f803

// ID ���λ=1��BOOT, =0, APP
#define IAP_ID_MASK 0x7FFFFFFF
// ID ��30λ=1�� BigEndian, =0, LittleEndian
#define IAP_ID_BigEndianBit 0x40000000
#define IAP_ID_BootBit      0x80000000

#define IAP_ID_BOOT (IAP_ID | IAP_ID_BootBit)

/*************** IAP CHIP ******************************/
// 24-29λ��Ϊ�豸��оƬ�ͺ�
enum IAP_CHIP
{
	IAP_CHIP_STM32F407 = (0x10 << 24),

};

/*************** IAP SYSTEM ****************************/
enum IAP_SYSTEM
{

	IAP_SYSTEM_AC = (0x1 << 16),	// �յ�ϵͳ
	
};


/*************** IAP HVAC DEVICE**********************/
enum HVAC_DEVICE
{
	IAP_HVAC_DEV_HMI = 4,					// HMI
	IAP_HVAC_DEV_CPU = 0,					// ����

};

/*************** IAP HVAC PROJECT ********************/
enum HVAC_PROJECT
{
	IAP_HVAC_V7_1 = (0x10 << 8),  //̹ɣ���Ǻ��ϼ���
};


#endif//__COMM_BASE_H__





