#include "ADIO.h"

// 从字符数组中取特定位
static inline U8 get_bit(U8 *buf, U8 index)
{
    U8 mask = 0x01 << (index & 7);
    if (buf[index / 8] & mask)
        return 1;
    return 0;
}

// 设置字符数字中的特定位
static inline void set_bit(U8 *buf, U8 index, U8 value)
{
    U8 mask = 0x01 << (index & 7);
    if (value)
        buf[index / 8] |= mask;
    else
        buf[index / 8] &= ~mask;
}

U8 DIO::Get(U8 index) // 取第index位的值
{
    if (index < 0 || index >= m_num)
    {
        return 0;
    }

    U8 force = get_bit(m_force, index);

    return get_bit((force) ? m_force_value : m_value, index);
}

U8 DIO::GetNotForce(U8 index) // 取第index位的值
{
    if (index < 0 || index >= m_num)
        return 0;

    return get_bit(m_value, index);
}

void DIO::Set(U8 index, U8 value) // 设第index位 = value
{
    if (index < 0 || index >= m_num)
    {
        return;
    }

    set_bit(m_value, index, value);
}

void DIO::SetForce(U8 index, U8 force, U8 value) // 强制设置第index位 = value
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
BOOL DIO::SetAll(U8 *buf, U8 len) // 全部设置
{
    if (len < DIO_BYTES)
    {
        return FALSE;
    }
    mem_copy(m_value, buf, DIO_BYTES);
    m_updateTime = sys_time();
    return TRUE;
}

BOOL DIO::SetForceAll(U8 force, U8 *buf, U8 len) // 全部设置
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

U8 DIO::GetAll(U8 *buf, U8 len) // 获取一组数字输入/输出（DIO）通道的当前状态，并将其存储在一个缓冲区中
{
    if (len < DIO_BYTES)     // 检查传递给函数的缓冲区buf的长度len是否小于DIO通道的总字节数DIO_BYTES
        return E_IO_BUF_LEN; // 表示缓冲区长度不足以容纳所有DIO通道的状态

    for (U8 i = 0; i < m_num; i++)
    {
        set_bit(buf, i, Get(i)); // 调用Get(i)来获取第i个DIO通道的当前状态，并使用set_bit函数将该状态设置到缓冲区buf中的相应位置。
    }

    return DIO_BYTES; // 成功操作并返回了DIO通道的总字节数
}

// 根据重定位表
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

// 全局变量定义
DIO g_DI, g_DO, g_LED;
