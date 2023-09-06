#ifndef _ADIO_H_
#define _ADIO_H_
#include "ac_include.h"


#define MAX_DIO_NUM (DI_NUM > DO_NUM ? DI_NUM : DO_NUM)
#define MAX_DIO_BYTES ((MAX_DIO_NUM+7)>>3)


class DIO
{
public:
    int m_num; // 实际DIO个数

private:
    U8 m_v[MAX_DIO_BYTES]; // IO数组，每位表示一个IO量
    U8 m_f[MAX_DIO_BYTES]; // 强制赋值数组，某位=1，则由m_fv[]的该位取代m_v[]的对应位
    U8 m_fv[MAX_DIO_BYTES];
    U32 m_updateTime; // 更新时间(秒)

public:
    int Get(int n);                     // 取第n位的值
    int GetNotForce(int n);             // 取第n位的值
    void Set(int n, int v);             // 设第n位 = v
    void SetForce(int n, int f, int v); // if(f) 强制设置第n位 = v

    BOOL SetAll(U8 *buf, int len);             // 全部设置
    BOOL SetForceAll(int f, U8 *buf, int len); // 全部设置
    int GetAll(U8 *buf, int len);              // 取全部位，返回总字节数
    int GetAllMap(U8 *buf, int len, U8 *map);  // 根据重定位表取全部位

    void dump();
};
#define E_IO_BUF_LEN -6

#define DO_SET(n)  g_DO.Set(n, 1)
#define DO_CLR(n)  g_DO.Set(n, 0)


extern DIO g_DI, g_DO, g_LED;
#endif //_ADIO_H_



