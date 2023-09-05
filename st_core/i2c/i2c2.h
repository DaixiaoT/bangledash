#ifndef __I2C2_H__
#define __I2C2_H__

#include "i2c_error.h"

void I2C2_Init(U32 clockSpeed);
void I2C2_Lock();
void I2C2_Unlock();
int I2C2_Wait_Busy();
int Locked_I2C2_Request(uint8_t devAddr, uint8_t *buf, int len, int bRead, int bStop, int retry, int needAck = 1);

static inline int I2C2_Write(U8 devAddr, U8 *buf, int len, int retry, int needAck)
{
	int ret = I2C2_Wait_Busy();
	if(ret)
        return ret;

	I2C2_Lock();
	ret = Locked_I2C2_Request(devAddr, buf, len, 0, 1, retry, needAck);
	I2C2_Unlock();
	return ret;
}

static inline int I2C2_Read(U8 devAddr, U8 *buf, int len, int retry, int needAck)
{
	int ret = I2C2_Wait_Busy();
	if(ret)
        return ret;

	I2C2_Lock();
	ret = Locked_I2C2_Request(devAddr, buf, len, 1, 1, retry, needAck);
	I2C2_Unlock();
	return ret;
}

// len <= 256
// 返回>=0, 读/写的字节数，<0, 错误
// !!!! 注意：调用者必须在buf前面至少保留2字节空间给WriteEEPROM使用
//int WriteEEPROM0(U16 addr, U8 *buf, int len);

// len <= 256
// 返回>=0, 读/写的字节数，<0, 错误
//int ReadEEPROM0(U16 addr, U8 *buf, int len);

//int read_panel(U8* buf);

#endif//__I2C2_H__
