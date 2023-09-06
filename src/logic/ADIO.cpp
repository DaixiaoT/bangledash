#include "ADIO.h"

// 从字符数组中取特定位
static inline int get_bit(U8 *buf, int n)
{
    U8 mask = 0x01 << (n & 7);
    if (buf[n / 8] & mask)
        return 1;
    return 0;
}

// 设置字符数字中的特定位
static inline void set_bit(U8 *buf, int n, int v)
{
    U8 mask = 0x01 << (n & 7);
    if (v)
        buf[n / 8] |= mask;
    else
        buf[n / 8] &= ~mask;
}

int DIO::Get(int n) // 取第n位的值
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

int DIO::GetNotForce(int n) // 取第n位的值
{
    if (n < 0 || n >= m_num)
        return 0;

    return get_bit(m_v, n);
}

void DIO::Set(int n, int v) // 设第n位 = v
{
    if (n < 0 || n >= m_num)
    {
        PRINT_ERR_INT(n);
        return;
    }

    set_bit(m_v, n, v);
}

void DIO::SetForce(int n, int f, int v) // 强制设置第n位 = v
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
BOOL DIO::SetAll(U8 *buf, int len) // 全部设置
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

BOOL DIO::SetForceAll(int f, U8 *buf, int len) // 全部设置
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

int DIO::GetAll(U8 *buf, int len) //获取一组数字输入/输出（DIO）通道的当前状态，并将其存储在一个缓冲区中
{
    if (len < DIO_BYTES)//检查传递给函数的缓冲区buf的长度len是否小于DIO通道的总字节数DIO_BYTES
        return E_IO_BUF_LEN;//表示缓冲区长度不足以容纳所有DIO通道的状态

    for (int i = 0; i < m_num; i++)
    {
        set_bit(buf, i, Get(i));//调用Get(i)来获取第i个DIO通道的当前状态，并使用set_bit函数将该状态设置到缓冲区buf中的相应位置。
    }

    return DIO_BYTES;//成功操作并返回了DIO通道的总字节数
}

// 根据重定位表
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

// 全局变量定义
DIO g_DI, g_DO, g_LED;
