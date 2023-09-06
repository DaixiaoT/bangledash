#include "ADIO.h"

// ���ַ�������ȡ�ض�λ
static inline int get_bit(U8 *buf, int n)
{
    U8 mask = 0x01 << (n & 7);
    if (buf[n / 8] & mask)
        return 1;
    return 0;
}

// �����ַ������е��ض�λ
static inline void set_bit(U8 *buf, int n, int v)
{
    U8 mask = 0x01 << (n & 7);
    if (v)
        buf[n / 8] |= mask;
    else
        buf[n / 8] &= ~mask;
}

int DIO::Get(int n) // ȡ��nλ��ֵ
{
    if (n < 0 || n >= m_num)
    {
        PRINT_ERR_INT(n);
        // PRINT_ERR_INT(m_num);
        return 0;
    }

    int f = get_bit(m_f, n);

    return get_bit((f) ? m_fv : m_v, n);
}

int DIO::GetNotForce(int n) // ȡ��nλ��ֵ
{
    if (n < 0 || n >= m_num)
        return 0;

    return get_bit(m_v, n);
}

void DIO::Set(int n, int v) // ���nλ = v
{
    if (n < 0 || n >= m_num)
    {
        PRINT_ERR_INT(n);
        return;
    }

    set_bit(m_v, n, v);
}

void DIO::SetForce(int n, int f, int v) // ǿ�����õ�nλ = v
{
    if (n < 0 || n >= m_num)
    {
        PRINT_ERR_INT(n);
        return;
    }

    if (f)
    {
        set_bit(m_f, n, 1);
        set_bit(m_fv, n, v);
        //         LOG_PRINT("DIO::SetForce() %d ,%d\n",n,v);
        //         LOG_PRINT("DIO::SetForce() 0x%x ,0x%x\n",m_f[0],m_fv[0]);
    }
    else
        set_bit(m_f, n, 0);
}

#define DIO_BYTES ((m_num + 7) >> 3)
BOOL DIO::SetAll(U8 *buf, int len) // ȫ������
{
    if (len < DIO_BYTES)
    {
        LOG_PRINT("err DIO::SetAll len=%d m_num=%d\n", len, m_num);
        return FALSE;
    }
    mem_copy(m_v, buf, DIO_BYTES);
    m_updateTime = sys_time();
    return TRUE;
}

BOOL DIO::SetForceAll(int f, U8 *buf, int len) // ȫ������
{
    if (len < DIO_BYTES)
    {
        LOG_PRINT("err DIO::SetForceAll len=%d m_num=%d\n", len, m_num);
        return FALSE;
    }

    if (f)
    {
        mem_set(m_f, 0xff, DIO_BYTES);
        mem_copy(m_fv, buf, DIO_BYTES);
    }
    else
        mem_set(m_f, 0, DIO_BYTES);

    m_updateTime = sys_time();
    return TRUE;
}

int DIO::GetAll(U8 *buf, int len) //��ȡһ����������/�����DIO��ͨ���ĵ�ǰ״̬��������洢��һ����������
{
    if (len < DIO_BYTES)//��鴫�ݸ������Ļ�����buf�ĳ���len�Ƿ�С��DIOͨ�������ֽ���DIO_BYTES
        return E_IO_BUF_LEN;//��ʾ���������Ȳ�������������DIOͨ����״̬

    for (int i = 0; i < m_num; i++)
    {
        set_bit(buf, i, Get(i));//����Get(i)����ȡ��i��DIOͨ���ĵ�ǰ״̬����ʹ��set_bit��������״̬���õ�������buf�е���Ӧλ�á�
    }

    return DIO_BYTES;//�ɹ�������������DIOͨ�������ֽ���
}

// �����ض�λ��
int DIO::GetAllMap(U8 *buf, int len, U8 *map)
{
    if (len < DIO_BYTES)
        return E_IO_BUF_LEN;

    for (int i = 0; i < m_num; i++)
    {
        set_bit(buf, i, Get(map[i]));
    }

    return DIO_BYTES;
}

void DIO::dump()
{
    LOG_PRINT("DIO::dump()");
    memdump(m_v, MAX_DIO_BYTES);
    memdump(m_f, MAX_DIO_BYTES);
    memdump(m_fv, MAX_DIO_BYTES);
}

// ȫ�ֱ�������
DIO g_DI, g_DO, g_LED;
