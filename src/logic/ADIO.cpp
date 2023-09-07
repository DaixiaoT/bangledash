#include "ADIO.h"

// ���ַ�������ȡ�ض�λ
static inline U8 get_bit(U8 *buf, U8 index)
{
    U8 mask = 0x01 << (index & 7);
    if (buf[index / 8] & mask)
        return 1;
    return 0;
}

// �����ַ������е��ض�λ
static inline void set_bit(U8 *buf, U8 index, U8 value)
{
    U8 mask = 0x01 << (index & 7);
    if (value)
        buf[index / 8] |= mask;
    else
        buf[index / 8] &= ~mask;
}

U8 DIO::Get(U8 index) // ȡ��indexλ��ֵ
{
    if (index < 0 || index >= m_num)
    {
        return 0;
    }

    U8 force = get_bit(m_force, index);

    return get_bit((force) ? m_force_value : m_value, index);
}

U8 DIO::GetNotForce(U8 index) // ȡ��indexλ��ֵ
{
    if (index < 0 || index >= m_num)
        return 0;

    return get_bit(m_value, index);
}

void DIO::Set(U8 index, U8 value) // ���indexλ = value
{
    if (index < 0 || index >= m_num)
    {
        return;
    }

    set_bit(m_value, index, value);
}

void DIO::SetForce(U8 index, U8 force, U8 value) // ǿ�����õ�indexλ = value
{
    if (index < 0 || index >= m_num)
    {
        return;
    }

    if (force)
    {
        set_bit(m_force, index, 1);
        set_bit(m_force_value, index, value);
    }
    else
        set_bit(m_force, index, 0);
}

#define DIO_BYTES ((m_num + 7) >> 3)
BOOL DIO::SetAll(U8 *buf, U8 len) // ȫ������
{
    if (len < DIO_BYTES)
    {
        return FALSE;
    }
    mem_copy(m_value, buf, DIO_BYTES);
    m_updateTime = sys_time();
    return TRUE;
}

BOOL DIO::SetForceAll(U8 force, U8 *buf, U8 len) // ȫ������
{
    if (len < DIO_BYTES)
    {
        return FALSE;
    }

    if (force)
    {
        mem_set(m_force, 0xff, DIO_BYTES);
        mem_copy(m_force_value, buf, DIO_BYTES);
    }
    else
        mem_set(m_force, 0, DIO_BYTES);

    m_updateTime = sys_time();
    return TRUE;
}

U8 DIO::GetAll(U8 *buf, U8 len) // ��ȡһ����������/�����DIO��ͨ���ĵ�ǰ״̬��������洢��һ����������
{
    if (len < DIO_BYTES)     // ��鴫�ݸ������Ļ�����buf�ĳ���len�Ƿ�С��DIOͨ�������ֽ���DIO_BYTES
        return E_IO_BUF_LEN; // ��ʾ���������Ȳ�������������DIOͨ����״̬

    for (U8 i = 0; i < m_num; i++)
    {
        set_bit(buf, i, Get(i)); // ����Get(i)����ȡ��i��DIOͨ���ĵ�ǰ״̬����ʹ��set_bit��������״̬���õ�������buf�е���Ӧλ�á�
    }

    return DIO_BYTES; // �ɹ�������������DIOͨ�������ֽ���
}

// �����ض�λ��
U8 DIO::GetAllMap(U8 *buf, U8 len, U8 *map)
{
    if (len < DIO_BYTES)
        return E_IO_BUF_LEN;

    for (U8 i = 0; i < m_num; i++)
    {
        set_bit(buf, i, Get(map[i]));
    }

    return DIO_BYTES;
}

void DIO::dump()
{
    memdump(m_value, MAX_DIO_BYTES);
    memdump(m_force, MAX_DIO_BYTES);
    memdump(m_force_value, MAX_DIO_BYTES);
}

// ȫ�ֱ�������
DIO g_DI, g_DO, g_LED;
