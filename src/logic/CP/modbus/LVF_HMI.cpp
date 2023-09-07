#include "LVF_HMI.h"

#define MAX(A, B) ((A) >= (B) ? (A) : (B))
#define MIN(A, B) ((A) <= (B) ? (A) : (B))

DEV_UART4 dev_uart4;

U16 wEidn_Buf[100];

SLINK_UART4 sLink_uart4_rcv;
SLINK_UART4 sLink_uart4_snd;

static U32 s_current_meter_rx_tick[METER_NUM] = {0, 0, 0, 0};
HMI_RD_REG hmi_rd_reg;
HMI_WR_REG hmi_wr_reg;
static U32 s_hmi_rx_tick = 0;

CURRENT_METER power_meter[METER_NUM];

BOOL current_err(U8 idx)
{

    U32 AVal = 0;
    U32 MAXval = 0;
    U32 MINval = 0;

    static U32 current_time[4] = {0, 0, 0, 0};
    static U32 curent_lock[4] = {1, 1, 1, 1};
    switch (idx)
    {
    case 0: // 取最大的单相电流，与最小的电向电流
        AVal = power_meter[idx].wVal[12];
        MAXval = MAX(MAX(power_meter[idx].wVal[9], power_meter[idx].wVal[10]), power_meter[idx].wVal[11]);
        MINval = MIN(MIN(power_meter[idx].wVal[9], power_meter[idx].wVal[10]), power_meter[idx].wVal[11]);
        break;

    case 1:
        AVal = power_meter[idx].wVal[12];
        MAXval = MAX(MAX(power_meter[idx].wVal[9], power_meter[idx].wVal[10]), power_meter[idx].wVal[11]);
        MINval = MIN(MIN(power_meter[idx].wVal[9], power_meter[idx].wVal[10]), power_meter[idx].wVal[11]);
        break;

    case 2:
        AVal = power_meter[idx].wVal[12];
        MAXval = MAX(MAX(power_meter[idx].wVal[9], power_meter[idx].wVal[10]), power_meter[idx].wVal[11]);
        MINval = MIN(MIN(power_meter[idx].wVal[9], power_meter[idx].wVal[10]), power_meter[idx].wVal[11]);
        break;

    case 3:
        AVal = power_meter[idx].wVal[12];
        MAXval = MAX(MAX(power_meter[idx].wVal[9], power_meter[idx].wVal[10]), power_meter[idx].wVal[11]);
        MINval = MIN(MIN(power_meter[idx].wVal[9], power_meter[idx].wVal[10]), power_meter[idx].wVal[11]);
        break;
    }
    // 平均电流超过7A,或小于3.6A，或单相电流大于最大电流的百分之120，或单相电小于最小电流的百分之80
    if ((AVal > 700) || ((AVal > 0) && (AVal < 360)) || (MAXval > (AVal * 12 / 10)) || ((MINval > 0) && (MINval < (AVal * 8 / 10))))
    {
        if (curent_lock[idx] == 1)
        {
            current_time[idx] = sys_time();
        }
        if (TimeGap(current_time[idx]) > 3000)
        {

            return TRUE;
        }
        else
        {
            curent_lock[idx] = 0;
            return FALSE;
        }
    }
    else
    {

        curent_lock[idx] = 1;
        return FALSE;
    }
}

void PickDev_uart4(void)
{
    static U8 dev_num = 1;
    switch (dev_uart4.cState)
    {
    case 0:
        dev_uart4.cDev = HMI_WR;
        dev_uart4.cState = 1;
        break;
    case 1:
        dev_uart4.cDev = HMI_RD;
        dev_num = dev_num + 1;
        dev_uart4.cState = dev_num;
        break;
    case 2:
        dev_uart4.cDev = DEV_METER1;
        dev_uart4.cState = 0;
        break;
    case 3:
        dev_uart4.cDev = DEV_METER2;
        dev_uart4.cState = 0;
        break;
    case 4:
        dev_uart4.cDev = DEV_METER3;
        dev_uart4.cState = 0;
        break;
    case 5:
        dev_uart4.cDev = DEV_METER4;
        dev_uart4.cState = 0;
        dev_num = 1;
        break;
    default:
        dev_uart4.cDev = HMI_RD;
        break;
    }
}

void Prepare_fix_adu_req(INT8U dev)
{
    INT16U addr, reg_addr, reg_num;
    INT16U crc;
    INT8U cNext = 0;

    if (HMI_RD == dev)
    {
        addr = MB_ADDR_HMI;
        reg_addr = REG_ADDR_RD_HMI;
        reg_num = REG_NUM_RD_HMI;
    }

    else if (DEV_METER1 == dev)
    {
        addr = MB_ADDR_METER1;
        reg_addr = REG_ADDR_METER;
        reg_num = REG_NUM_METER;
    }
    else if (DEV_METER2 == dev)
    {
        addr = MB_ADDR_METER2;
        reg_addr = REG_ADDR_METER;
        reg_num = REG_NUM_METER;
    }
    else if (DEV_METER3 == dev)
    {
        addr = MB_ADDR_METER3;
        reg_addr = REG_ADDR_METER;
        reg_num = REG_NUM_METER;
    }
    else if (DEV_METER4 == dev)
    {
        addr = MB_ADDR_METER4;
        reg_addr = REG_ADDR_METER;
        reg_num = REG_NUM_METER;
    }

    sLink_uart4_snd.data[cNext++] = addr;
    sLink_uart4_snd.data[cNext++] = MB_CODE_RD;
    sLink_uart4_snd.data[cNext++] = HI(reg_addr);
    sLink_uart4_snd.data[cNext++] = LO(reg_addr);

    sLink_uart4_snd.data[cNext++] = HI(reg_num);
    sLink_uart4_snd.data[cNext++] = LO(reg_num);

    crc = crc16(sLink_uart4_snd.data, cNext);

    sLink_uart4_snd.data[cNext++] = HI(crc);
    sLink_uart4_snd.data[cNext++] = LO(crc);
    sLink_uart4_snd.wNum = cNext;
}


//主要目的是将要发送的数据按照485通信协议进行打包，然后通过UART4（串口通信）发送出去。
void Send_msg(INT16U add, INT16U *pData, INT16U wNUM) 
{
    INT16U ENext = 0;
    INT16U Ecrc = 0;

    sLink_uart4_snd.data[ENext++] = MB_ADDR_HMI;
    sLink_uart4_snd.data[ENext++] = MB_CODE_WR;
    sLink_uart4_snd.data[ENext++] = HI(add);
    sLink_uart4_snd.data[ENext++] = LO(add);
    sLink_uart4_snd.data[ENext++] = HI(wNUM);
    sLink_uart4_snd.data[ENext++] = LO(wNUM);
    sLink_uart4_snd.data[ENext++] = wNUM * 2;

    memcpy((U8 *)&sLink_uart4_snd.data[ENext], (U8 *)pData, wNUM * 2);
    ENext += (wNUM * 2);
    Ecrc = crc16(sLink_uart4_snd.data, ENext);
    sLink_uart4_snd.data[ENext++] = HI(Ecrc);
    sLink_uart4_snd.data[ENext++] = LO(Ecrc);
    sLink_uart4_snd.wNum = ENext;

    comm_uart4.Write((char *)sLink_uart4_snd.data, sLink_uart4_snd.wNum);
}

U8 HMI_mode(U8 set_mode, U8 set)
{
}

void Refresh_HVAC_HMIState(void)
{
}

void Refresh_HVAC_OpInfo(void)
{
    Refresh_HVAC_HMIState();
}

void Prepare_wr_adu_req(void)
{
}

void Prepare_adu_req(void)
{

    if (HMI_WR == dev_uart4.cDev)
    {
        Prepare_wr_adu_req();
    }
    else
    {
        Prepare_fix_adu_req(dev_uart4.cDev);
        comm_uart4.Write((char *)sLink_uart4_snd.data, sLink_uart4_snd.wNum);
    }
}

void Prepare_Send_uart4(void)
{
    PickDev_uart4();   // 用于选择要发送的设备以及相关的通信参数
    Prepare_adu_req(); // 用于准备ADU请求，也就是要发送到UART4通信端口的数据包
}


//用于解析接收到的仪表数据帧，并将数据存储在相应的数据结构中，同时记录接收时间戳。
void Update_Meter(int idx, INT8U *pData, INT16U wNum)
{
    INT8U byteNum = pData[2];
    INT8U i = 0;
    if (byteNum != sizeof(CURRENT_METER))
    {
        //数据帧中的数据长度与 CURRENT_METER 结构体的长度不匹配
        rt_kprintf("power meter response frame data num error: %d\r\n", byteNum);
    }
    else
    {
        s_current_meter_rx_tick[idx] = sys_time();//记录仪表数据的接收时间戳。

        //将数据帧中的数据（从第3个字节开始）复制到 wEidn_Buf 数组中。wEidn_Buf 可能是一个用于临时存储数据的缓冲区。
        memcpy((U8 *)wEidn_Buf, (U8 *)&pData[3], sizeof(CURRENT_METER));

        //使用一个循环，将 wEidn_Buf 中的数据存储到 power_meter[idx].wVal 数组中。
        for (i = 0; i < REG_NUM_METER; i++)
        {
            power_meter[idx].wVal[i] = hton16(wEidn_Buf[i]);
        }
        current_err(idx);
    }
}

void Update_HMI(INT8U *pData, INT16U wNum) // 读取显示屏数据
{
}

// 根据接收到的数据帧中的地址和功能码，将数据分派给不同的处理函数，以便进一步处理和解析通信数据。
void ParseInfo_UART4(U8 *pData, U16 wNum)
{
    U8 addr, fun_code;
    addr = pData[0];//地址
    fun_code = pData[1];//功能码

#ifdef _DEBUG
    if (MB_ADDR_AIRMON == addr)
    {
        Update_AirQuality(pData, wNum);
    }
    else if (MB_ADDR_METER == addr)
    {
        Update_Meter(pData, wNum);
    }
    else
#endif
        if (MB_ADDR_HMI == addr && MB_CODE_RD == fun_code)
    {

        Update_HMI(pData, wNum);
    }

    else if ((MB_ADDR_METER1 <= addr) && (addr <= MB_ADDR_METER4))
    {

        int idx = addr - MB_ADDR_METER1;
        Update_Meter(idx, pData, wNum);
    }
    else
    {
    }
}

// 目的是检查接收到的数据帧是否有效，并根据验证结果来更新通信和错误计数器。
// 验证通过的数据帧可以进一步解析和处理，而验证失败的数据帧可能需要被丢弃或进行错误处理。
U8 ValidateFrame(U8 *pData, U16 wNum)
{
    U16 crc = 0;
    if (wNum <= MIN_RSP_LEN_MB)
    {
        // 表示数据帧过短，无法进行有效验证
        dev_uart4.wCntErr++;
        return (FALSE);
    }
    else
    {
        crc = crc16(pData, wNum - 2);
        // 检查计算得到的CRC校验值的高位字节和低位字节是否与数据帧中的倒数第二个字节和最后一个字节相匹配。
        // 如果匹配成功，表示数据帧未被损坏，验证通过。
        if (HI(crc) == pData[wNum - 2] && LO(crc) == pData[wNum - 1])
        {
            if (dev_uart4.wCntComu < 0xFFFFFFFF)
            {
                // 表示通信成功，然后返回 TRUE 表示验证通过。
                dev_uart4.wCntComu++;
            }
            return (TRUE);
        }
        else
        {
            // 返回 FALSE 表示验证失败
            dev_uart4.wCntErr++;
            return (FALSE);
        }
    }
}

void HMI_Uart4_task(void *)
{
#ifdef USE_MODBUS_THEARD
    while (true)
    {
        Prepare_Send_uart4(); // 准备要发送的数据

        comm_uart4.Write((char *)sLink_uart4_snd.data, sLink_uart4_snd.wNum); // 将数据发送到UART4通信端口
        rt_thread_delay(200);                                                 // 延时

        // 尝试读取UART4接收缓冲区中的数据，将读取的数据存储在sLink_uart4_rcv缓冲区中，同时记录数据长度
        sLink_uart4_rcv.wNum = comm_uart4.TryRead((char *)sLink_uart4_rcv.data, 256);

        // 验证接收到的数据帧的有效性
        if (ValidateFrame(sLink_uart4_rcv.data, sLink_uart4_rcv.wNum))
        {
            // 如有效，再对数据进行处理和解析
            ParseInfo_UART4(sLink_uart4_rcv.data, sLink_uart4_rcv.wNum);
        }
    }
#else

    U8 SING_NUM[16];
    U16 crctext;
    U8 TNext = 0;
    static U32 s_time = 0;
    static BOOL s_frist = TRUE; // 首次标志
    if (TimeGap(s_time) < 250)
        return;
    s_time = sys_time();

    if (!s_frist)
    {
        // 尝试读取UART4接收缓冲区中的数据，将读取的数据存储在sLink_uart4_rcv缓冲区中，同时记录数据长度
        sLink_uart4_rcv.wNum = comm_uart4.TryRead((char *)sLink_uart4_rcv.data, 256);

        // 验证接收到的数据帧的有效性
        if (ValidateFrame(sLink_uart4_rcv.data, sLink_uart4_rcv.wNum))
        {
            // 如有效，再对数据进行处理和解析
            ParseInfo_UART4(sLink_uart4_rcv.data, sLink_uart4_rcv.wNum);
        }
    }

    // 准备要发送的数据
    Prepare_Send_uart4();

    s_frist = FALSE;
#endif
}

#ifdef USE_MODBUS_THEARD
#define HMI_TASK_STACK_SIZE (1024 * 2)
static U32 hmi_task_stack[HMI_TASK_STACK_SIZE >> 2];
#endif