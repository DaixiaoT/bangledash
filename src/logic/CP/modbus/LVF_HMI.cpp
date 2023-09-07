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
    case 0: // ȡ���ĵ������������С�ĵ������
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
    // ƽ����������7A,��С��3.6A��������������������İٷ�֮120�������С����С�����İٷ�֮80
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


//��ҪĿ���ǽ�Ҫ���͵����ݰ���485ͨ��Э����д����Ȼ��ͨ��UART4������ͨ�ţ����ͳ�ȥ��
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
    PickDev_uart4();   // ����ѡ��Ҫ���͵��豸�Լ���ص�ͨ�Ų���
    Prepare_adu_req(); // ����׼��ADU����Ҳ����Ҫ���͵�UART4ͨ�Ŷ˿ڵ����ݰ�
}


//���ڽ������յ����Ǳ�����֡���������ݴ洢����Ӧ�����ݽṹ�У�ͬʱ��¼����ʱ�����
void Update_Meter(int idx, INT8U *pData, INT16U wNum)
{
    INT8U byteNum = pData[2];
    INT8U i = 0;
    if (byteNum != sizeof(CURRENT_METER))
    {
        //����֡�е����ݳ����� CURRENT_METER �ṹ��ĳ��Ȳ�ƥ��
        rt_kprintf("power meter response frame data num error: %d\r\n", byteNum);
    }
    else
    {
        s_current_meter_rx_tick[idx] = sys_time();//��¼�Ǳ����ݵĽ���ʱ�����

        //������֡�е����ݣ��ӵ�3���ֽڿ�ʼ�����Ƶ� wEidn_Buf �����С�wEidn_Buf ������һ��������ʱ�洢���ݵĻ�������
        memcpy((U8 *)wEidn_Buf, (U8 *)&pData[3], sizeof(CURRENT_METER));

        //ʹ��һ��ѭ������ wEidn_Buf �е����ݴ洢�� power_meter[idx].wVal �����С�
        for (i = 0; i < REG_NUM_METER; i++)
        {
            power_meter[idx].wVal[i] = hton16(wEidn_Buf[i]);
        }
        current_err(idx);
    }
}

void Update_HMI(INT8U *pData, INT16U wNum) // ��ȡ��ʾ������
{
}

// ���ݽ��յ�������֡�еĵ�ַ�͹����룬�����ݷ��ɸ���ͬ�Ĵ��������Ա��һ������ͽ���ͨ�����ݡ�
void ParseInfo_UART4(U8 *pData, U16 wNum)
{
    U8 addr, fun_code;
    addr = pData[0];//��ַ
    fun_code = pData[1];//������

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

// Ŀ���Ǽ����յ�������֡�Ƿ���Ч����������֤���������ͨ�źʹ����������
// ��֤ͨ��������֡���Խ�һ�������ʹ�������֤ʧ�ܵ�����֡������Ҫ����������д�����
U8 ValidateFrame(U8 *pData, U16 wNum)
{
    U16 crc = 0;
    if (wNum <= MIN_RSP_LEN_MB)
    {
        // ��ʾ����֡���̣��޷�������Ч��֤
        dev_uart4.wCntErr++;
        return (FALSE);
    }
    else
    {
        crc = crc16(pData, wNum - 2);
        // ������õ���CRCУ��ֵ�ĸ�λ�ֽں͵�λ�ֽ��Ƿ�������֡�еĵ����ڶ����ֽں����һ���ֽ���ƥ�䡣
        // ���ƥ��ɹ�����ʾ����֡δ���𻵣���֤ͨ����
        if (HI(crc) == pData[wNum - 2] && LO(crc) == pData[wNum - 1])
        {
            if (dev_uart4.wCntComu < 0xFFFFFFFF)
            {
                // ��ʾͨ�ųɹ���Ȼ�󷵻� TRUE ��ʾ��֤ͨ����
                dev_uart4.wCntComu++;
            }
            return (TRUE);
        }
        else
        {
            // ���� FALSE ��ʾ��֤ʧ��
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
        Prepare_Send_uart4(); // ׼��Ҫ���͵�����

        comm_uart4.Write((char *)sLink_uart4_snd.data, sLink_uart4_snd.wNum); // �����ݷ��͵�UART4ͨ�Ŷ˿�
        rt_thread_delay(200);                                                 // ��ʱ

        // ���Զ�ȡUART4���ջ������е����ݣ�����ȡ�����ݴ洢��sLink_uart4_rcv�������У�ͬʱ��¼���ݳ���
        sLink_uart4_rcv.wNum = comm_uart4.TryRead((char *)sLink_uart4_rcv.data, 256);

        // ��֤���յ�������֡����Ч��
        if (ValidateFrame(sLink_uart4_rcv.data, sLink_uart4_rcv.wNum))
        {
            // ����Ч���ٶ����ݽ��д���ͽ���
            ParseInfo_UART4(sLink_uart4_rcv.data, sLink_uart4_rcv.wNum);
        }
    }
#else

    U8 SING_NUM[16];
    U16 crctext;
    U8 TNext = 0;
    static U32 s_time = 0;
    static BOOL s_frist = TRUE; // �״α�־
    if (TimeGap(s_time) < 250)
        return;
    s_time = sys_time();

    if (!s_frist)
    {
        // ���Զ�ȡUART4���ջ������е����ݣ�����ȡ�����ݴ洢��sLink_uart4_rcv�������У�ͬʱ��¼���ݳ���
        sLink_uart4_rcv.wNum = comm_uart4.TryRead((char *)sLink_uart4_rcv.data, 256);

        // ��֤���յ�������֡����Ч��
        if (ValidateFrame(sLink_uart4_rcv.data, sLink_uart4_rcv.wNum))
        {
            // ����Ч���ٶ����ݽ��д���ͽ���
            ParseInfo_UART4(sLink_uart4_rcv.data, sLink_uart4_rcv.wNum);
        }
    }

    // ׼��Ҫ���͵�����
    Prepare_Send_uart4();

    s_frist = FALSE;
#endif
}

#ifdef USE_MODBUS_THEARD
#define HMI_TASK_STACK_SIZE (1024 * 2)
static U32 hmi_task_stack[HMI_TASK_STACK_SIZE >> 2];
#endif