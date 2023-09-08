#ifndef _ADIO_H_
#define _ADIO_H_
#include "ac_include.h"

#define MAX_DIO_NUM (DI_NUM > DO_NUM ? DI_NUM : DO_NUM)
#define MAX_DIO_BYTES ((MAX_DIO_NUM + 7) >> 3)

class DIO
{
public:
    int m_num; // 实际DIO个数

private:
    U8 m_value[MAX_DIO_BYTES]; // IO数组，每位表示一个IO量
    U8 m_force[MAX_DIO_BYTES]; // 强制赋值数组，某位=1，则由m_force_value[]的该位取代m_value[]的对应位
    U8 m_force_value[MAX_DIO_BYTES];
    U32 m_updateTime; // 更新时间(秒)

public:
    U8 Get(U8 index);                            // 取第index位的值
    U8 GetNotForce(U8 index);                    // 取第index位的值
    void Set(U8 index, U8 value);                // 设第index位 = value
    void SetForce(U8 index, U8 force, U8 value); // if(force) 强制设置第index位 = value

    BOOL SetAll(U8 *buf, U8 len);                // 全部设置
    BOOL SetForceAll(U8 force, U8 *buf, U8 len); // 全部设置
    U8 GetAll(U8 *buf, U8 len);                  // 取全部位，返回总字节数
    U8 GetAllMap(U8 *buf, U8 len, U8 *map);      // 根据重定位表取全部位

    void dump();
};
#define E_IO_BUF_LEN -6

#define DO_SET(n) g_DO.Set(n, 1)
#define DO_CLR(n) g_DO.Set(n, 0)
#define DI_STAT(n) g_DI.Get(n)


extern DIO g_DI, g_DO, g_LED;
#endif //_ADIO_H_
