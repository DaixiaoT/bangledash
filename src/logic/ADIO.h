#ifndef _ADIO_H_
#define _ADIO_H_
#include "ac_include.h"

#define MAX_DIO_NUM (DI_NUM > DO_NUM ? DI_NUM : DO_NUM)
#define MAX_DIO_BYTES ((MAX_DIO_NUM + 7) >> 3)

class DIO
{
public:
    int m_num; // ʵ��DIO����

private:
    U8 m_value[MAX_DIO_BYTES]; // IO���飬ÿλ��ʾһ��IO��
    U8 m_force[MAX_DIO_BYTES]; // ǿ�Ƹ�ֵ���飬ĳλ=1������m_force_value[]�ĸ�λȡ��m_value[]�Ķ�Ӧλ
    U8 m_force_value[MAX_DIO_BYTES];
    U32 m_updateTime; // ����ʱ��(��)

public:
    U8 Get(U8 index);                            // ȡ��indexλ��ֵ
    U8 GetNotForce(U8 index);                    // ȡ��indexλ��ֵ
    void Set(U8 index, U8 value);                // ���indexλ = value
    void SetForce(U8 index, U8 force, U8 value); // if(force) ǿ�����õ�indexλ = value

    BOOL SetAll(U8 *buf, U8 len);                // ȫ������
    BOOL SetForceAll(U8 force, U8 *buf, U8 len); // ȫ������
    U8 GetAll(U8 *buf, U8 len);                  // ȡȫ��λ���������ֽ���
    U8 GetAllMap(U8 *buf, U8 len, U8 *map);      // �����ض�λ��ȡȫ��λ

    void dump();
};
#define E_IO_BUF_LEN -6

#define DO_SET(n) g_DO.Set(n, 1)
#define DO_CLR(n) g_DO.Set(n, 0)
#define DI_STAT(n) g_DI.Get(n)


extern DIO g_DI, g_DO, g_LED;
#endif //_ADIO_H_
