#ifndef _MIIP_UDP_H__
#define _MIIP_UDP_H__

BOOL miip_udp_stat();
class MIIP_UDP
{
public:
    BOOL m_isInit;//初始化标志
    U16 m_port;//存储UDP通信的端口号
    U32 udp_handle;//存储UDP通信的句柄，句柄通常用于引用打开的通信通道
    ip_addr_t from_ip;//存储UDP数据包的源IP地址
    U16 from_port;//存储UDP数据包的源端口号

public:
    MIIP_UDP()
    {
        m_isInit = FALSE;
        m_port = 0;
        from_port = 0;
        from_ip.addr = 0;
    }

    //注册UDP信息，并绑定到指定的UDP端口
    BOOL miip_udp_info_reg(U16 port)
    {
        int ret = udp_open(&udp_handle);//打开一个UDP套接字，并将返回的句柄存储在udp_handle中
        if (ret < 0)
        {
            LOG_PRINT("udp_open err=%d\r\n", ret);
            return FALSE;
        }

        LOG_PRINT("udp handle: %d\r\n", udp_handle);

        ret = udp_bind(udp_handle, 0, port);//将UDP套接字绑定到指定端口
        if (ret)
        {
            LOG_PRINT("udp_bind err=%d\r\n", ret);
            return FALSE;
        }

        m_isInit = TRUE;//初始化成功
        return m_isInit;
    }

    //检查是否有UDP数据可用，并接收UDP数据
    int check_udp_data(U8 *buf, int len)
    {
        
        if (!miip_udp_stat() || (!m_isInit))
            return 0;//无法接收

        int rx_len = udp_recvfrom(udp_handle, buf, len, &from_ip, &from_port);//接收UDP数据，存储在buf中

        if (rx_len <= 0)
            return 0;

        return rx_len;//返回接收到的数据长度
    }

    //用于发送UDP数据
    int send_udp_data(U8 *buf, int len)
    {
        if (!miip_udp_stat() || (!m_isInit))
            return 0;//无法发送

        if (from_ip.addr == 0)
            return -1;//目标IP地址未设置，无法发送

        return udp_sendto(udp_handle, buf, len, &from_ip, from_port);//发送UDP数据到指定IP地址和端口，并返回发送的字节数
    }
};
#define UDP_PTU_PORT 6957
extern MIIP_UDP g_ptu_udp;
#endif //_MIIP_UDP_H__
