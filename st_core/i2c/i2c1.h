#ifndef __I2C1_H__
#define __I2C1_H__

#include "i2c_error.h"

void I2C1_Init(U32 clockSpeed);
void I2C1_Lock();
void I2C1_Unlock();
int I2C1_Wait_Busy();
int Locked_I2C1_Request(uint8_t devAddr, uint8_t *buf, int len, int bRead, int bStop, int retry, int needAck = 1);

static inline int I2C1_Write(U8 devAddr, U8 *buf, int len, int retry, int needAck)
{
	int ret = I2C1_Wait_Busy();
	if(ret)
        return ret;

	I2C1_Lock();
	ret = Locked_I2C1_Request(devAddr, buf, len, 0, 1, retry, needAck);
	I2C1_Unlock();
	return ret;
}

static inline int I2C1_Read(U8 devAddr, U8 *buf, int len, int retry, int needAck)
{
	int ret = I2C1_Wait_Busy();
	if(ret)
        return ret;

	I2C1_Lock();
	ret = Locked_I2C1_Request(devAddr, buf, len, 1, 1, retry, needAck);
	I2C1_Unlock();
	return ret;
}

// len <= 256
// ����>=0, ��/д���ֽ�����<0, ����
// !!!! ע�⣺�����߱�����bufǰ�����ٱ���2�ֽڿռ��WriteEEPROMʹ��
//int WriteEEPROM1(U16 addr, U8 *buf, int len);

// len <= 256
// ����>=0, ��/д���ֽ�����<0, ����
//int ReadEEPROM1(U16 addr, U8 *buf, int len);

//int read_panel(U8* buf);

#endif//__I2C1_H__
