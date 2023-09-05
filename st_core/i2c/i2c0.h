#ifndef __I2C0_H__
#define __I2C0_H__

#include "i2c_error.h"

void I2C0_Init();
void I2C0_Lock();
void I2C0_Unlock();
int Locked_I2C0_Request(uint8_t devAddr, uint8_t *buf, int len, int bRead, int bStop, int retry, int needAck = 1);

static inline int I2C0_Write(U8 devAddr, U8 *buf, int len, int retry, int needAck)
{
	int ret;
	I2C0_Lock();
	ret = Locked_I2C0_Request(devAddr, buf, len, 0, 1, retry, needAck);
	I2C0_Unlock();
	return ret;
}

static inline int I2C0_Read(U8 devAddr, U8 *buf, int len, int retry, int needAck)
{
	int ret;
	I2C0_Lock();
	ret = Locked_I2C0_Request(devAddr, buf, len, 1, 1, retry, needAck);
	I2C0_Unlock();
	return ret;
}

// len <= 256
// ����>=0, ��/д���ֽ�����<0, ����
// !!!! ע�⣺�����߱�����bufǰ�����ٱ���2�ֽڿռ��WriteEEPROMʹ��
//int WriteEEPROM0(U16 addr, U8 *buf, int len);

// len <= 256
// ����>=0, ��/д���ֽ�����<0, ����
//int ReadEEPROM0(U16 addr, U8 *buf, int len);

//int read_panel(U8* buf);

#endif//__I2C0_H__
