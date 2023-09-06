#ifndef _ADIO_H_
#define _ADIO_H_
#include "ac_include.h"


#define MAX_DIO_NUM (DI_NUM > DO_NUM ? DI_NUM : DO_NUM)
#define MAX_DIO_BYTES ((MAX_DIO_NUM+7)>>3)


class DIO
{
public:
    int m_num; // ʵ��DIO����

private:
    U8 m_v[MAX_DIO_BYTES]; // IO���飬ÿλ��ʾһ��IO��
    U8 m_f[MAX_DIO_BYTES]; // ǿ�Ƹ�ֵ���飬ĳλ=1������m_fv[]�ĸ�λȡ��m_v[]�Ķ�Ӧλ
    U8 m_fv[MAX_DIO_BYTES];
    U32 m_updateTime; // ����ʱ��(��)

public:
    int Get(int n);                     // ȡ��nλ��ֵ
    int GetNotForce(int n);             // ȡ��nλ��ֵ
    void Set(int n, int v);             // ���nλ = v
    void SetForce(int n, int f, int v); // if(f) ǿ�����õ�nλ = v

    BOOL SetAll(U8 *buf, int len);             // ȫ������
    BOOL SetForceAll(int f, U8 *buf, int len); // ȫ������
    int GetAll(U8 *buf, int len);              // ȡȫ��λ���������ֽ���
    int GetAllMap(U8 *buf, int len, U8 *map);  // �����ض�λ��ȡȫ��λ

    void dump();
};
#define E_IO_BUF_LEN -6

#define DO_SET(n)  g_DO.Set(n, 1)
#define DO_CLR(n)  g_DO.Set(n, 0)


extern DIO g_DI, g_DO, g_LED;
#endif //_ADIO_H_



