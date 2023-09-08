#include "BSP_IODrv.h"

#define GETA(n) ((U8)(n)&0xff)
#define GETB(n) ((U8)((n) >> 8))

#define CLOSE 0
#define OPEN 1

typedef struct DIOInfo
{
    U8 m_addr;              // 7λ��ַ
    U16 m_dir;              // 1 = ��������Ϊ����,0 = ��������Ϊ���
    U16 m_ipol;             // 1 = GPIO�Ĵ���λ����ӳ�������ŵ��෴�߼�״̬,0 = GPIO�Ĵ���λ����ӳ�������ŵ���ͬ�߼�״̬
    GPIO_TypeDef *m_csprot; // Ƭѡ�˿�
    U16 m_csbit;            // Ƭѡ�ܽ�
    U16 m_io;               // IOֵ
} DIOInfo_t;

DIOInfo_t s_di[5] = {
    {0x20, MCP_DIR_IN, 0xffff, GPIOE, GPIO_Pin_5, 0},
    {0x21, MCP_DIR_IN, 0xffff, GPIOE, GPIO_Pin_5, 0},
    {0x22, MCP_DIR_IN, 0xffff, GPIOE, GPIO_Pin_5, 0},
    {0x23, MCP_DIR_IN, 0xffff, GPIOE, GPIO_Pin_6, 0},
    {0x20, MCP_DIR_IN, 0xffff, GPIOE, GPIO_Pin_6, 0}};

DIOInfo_t s_do[3] = {
    {0x24, MCP_DIR_OUT, 0, GPIOD, GPIO_Pin_11, 0},
    {0x25, MCP_DIR_OUT, 0, GPIOD, GPIO_Pin_12, 0},
    {0x26, MCP_DIR_OUT, 0, GPIOD, GPIO_Pin_13, 0},
};

static void BSP_IODrv_Config(DIOInfo_t *pDio)
{
    // ��һ�д��뽫����Ӳ����ַ���ţ�����pDio->m_addr��ʾMCP23S17оƬ�ĵ�ַ��MCP_IOCONA��ʾҪ���õļĴ�����
    //(1 << 3)���������ŵ�ĳ��λ��pDio->m_csprot��pDio->m_csbit������оƬͨ�ŵĿ��Ʋ�����
    // ʹ��Ӳ����ַ���ţ���һ��Ҫ�������Σ���֪��Ϊʲô
    MCP23S17_Write(pDio->m_addr, MCP_IOCONA, (1 << 3), pDio->m_csprot, pDio->m_csbit);
    MCP23S17_Write(pDio->m_addr, MCP_IOCONA, (1 << 3), pDio->m_csprot, pDio->m_csbit);

    MCP23S17_Write(pDio->m_addr, MCP_IOCONB, (1 << 3), pDio->m_csprot, pDio->m_csbit);
    // ���÷���
    MCP23S17_Write(pDio->m_addr, MCP_IODIRA, GETA(pDio->m_dir), pDio->m_csprot, pDio->m_csbit);
    MCP23S17_Write(pDio->m_addr, MCP_IODIRB, GETB(pDio->m_dir), pDio->m_csprot, pDio->m_csbit);

    // �������뼫�Զ˿�
    MCP23S17_Write(pDio->m_addr, MCP_IPOLA, GETA(pDio->m_ipol), pDio->m_csprot, pDio->m_csbit);
    MCP23S17_Write(pDio->m_addr, MCP_IPOLB, GETB(pDio->m_ipol), pDio->m_csprot, pDio->m_csbit);

    if (pDio->m_dir == MCP_DIR_OUT)
    {
        // Ĭ�����Ϊ��
        MCP23S17_Write(pDio->m_addr, MCP_GPIOA, 0, pDio->m_csprot, pDio->m_csbit);
        MCP23S17_Write(pDio->m_addr, MCP_GPIOB, 0, pDio->m_csprot, pDio->m_csbit);
    }
}

void BSP_IODrv_init()
{
    g_DO.m_num = DO_NUM;
    g_DI.m_num = DI_NUM;

    MCP23S17_Init(); // ��ʼ��MCP23S17оƬ������һ��I/O��չ��оƬ������ͨ��SPI�ӿ���Ƕ��ʽϵͳͨ�ţ�������չ����/������š�

    BSP_IODrv_Config(&s_di[0]);
    BSP_IODrv_Config(&s_di[1]);
    BSP_IODrv_Config(&s_di[2]);
    BSP_IODrv_Config(&s_di[3]);
    BSP_IODrv_Config(&s_di[4]);

    BSP_IODrv_Config(&s_do[0]);
    BSP_IODrv_Config(&s_do[1]);
    BSP_IODrv_Config(&s_do[2]);

    di_filer_init(); // ��ʼ������������˲�����
}

//���ƾ���24V��110V��Դ���������ͨ�������ݴ����״̬������ͨ���Ŀ���״̬
void out_di_110V(BOOL s) // DI����24V��110V
{
    gpioSetValue(GPIOE, 11, s);
    gpioSetValue(GPIOE, 12, s);
    gpioSetValue(GPIOE, 3, s);
    gpioSetValue(GPIOE, 4, s);
}
//���ڿ��Ʒ�ʱ�ɼ���ص��������ͨ��
void out_di_dtc(BOOL s)
{
    gpioSetValue(GPIOE, 13, s);
    gpioSetValue(GPIOE, 15, s);
    gpioSetValue(GPIOG, 6, s);
    gpioSetValue(GPIOG, 7, s);
}

void out_di_dtc_open()
{ // �Ͽ�
    out_di_dtc(OPEN);
}

void out_di_dtc_close()
{ // �պ�
    out_di_dtc(CLOSE);
}

static U16 DI_Read_Verify(DIOInfo_t *pDio)
{
    U8 IOA = MCP23S17_Read(pDio->m_addr, MCP_GPIOA, pDio->m_csprot, pDio->m_csbit);
    U8 IOB = MCP23S17_Read(pDio->m_addr, MCP_GPIOB, pDio->m_csprot, pDio->m_csbit);
    U16 v = (IOB << 8) | IOA;
    U16 v1;

    // LOG_PRINT("DI_Read_Verify() pDio->m_csprot:0x%x , addr:0x%x ,0x%x\r\n", pDio->m_csprot,pDio->m_addr,v);

    for (int i = 0; i < 10; i++)
    {
        IOA = MCP23S17_Read(pDio->m_addr, MCP_GPIOA, pDio->m_csprot, pDio->m_csbit);
        IOB = MCP23S17_Read(pDio->m_addr, MCP_GPIOB, pDio->m_csprot, pDio->m_csbit);
        v1 = (IOB << 8) | IOA;
        if (v == v1)
            return v;

        v = v1;
        // sleep(2);
    }

    // LOG_PRINT("DI_Read_Verify failed.\r\n");
    return 0;
}
static void DO_Write_Verify(DIOInfo_t *pDio)
{
    for (int i = 0; i < 10; i++)
    {
        MCP23S17_Write(pDio->m_addr, MCP_GPIOA, GETA(pDio->m_io), pDio->m_csprot, pDio->m_csbit);
        MCP23S17_Write(pDio->m_addr, MCP_GPIOB, GETB(pDio->m_io), pDio->m_csprot, pDio->m_csbit);

        U8 IOA = MCP23S17_Read(pDio->m_addr, MCP_GPIOA, pDio->m_csprot, pDio->m_csbit);
        U8 IOB = MCP23S17_Read(pDio->m_addr, MCP_GPIOB, pDio->m_csprot, pDio->m_csbit);
        U16 v = (IOB << 8) | IOA;
        if (v == pDio->m_io)
        {
            return;
        }
        // LOG_PRINT("DO_Write_Verify failed addr:%x.[%04x] [%04x]\r\n", pDio->m_addr, v, pDio->m_io);
        // sleep(2);
    }
}
static void DioUpdate(DIOInfo_t *pDio)
{
    U8 IOA, IOB;

    if (pDio->m_dir == MCP_DIR_IN)//���ͨ���ķ���
    {
        pDio->m_io = DI_Read_Verify(pDio);//�������Ϊ���룬���ȡDIOͨ��״̬����������洢��pDio->m_io��
    }
    else // ���
    {
        DO_Write_Verify(pDio);//�������Ϊ�������д��DIOͨ����״̬
    }
}
//�����������루DI��ͨ����״̬
static void DI_Update()
{
    MCP23S17_CS_IN4_HIGH();//����Ƭѡ����Ϊ�ߵ�ƽ
    DioUpdate(&s_di[0]);//����������ͬDIͨ����״̬
    DioUpdate(&s_di[1]);
    DioUpdate(&s_di[2]);
}

//�������������DO��ͨ����״̬
static void DO_Update()
{
    U8 output[DO_BYTES];//������һ������ΪDO_BYTES��output���飬���ڴ洢DOͨ����״̬��Ϣ��
    mem_set(output, 0, DO_BYTES);//Ԫ�س�ʼ��Ϊ0�����֮ǰ��״̬��
    g_DO.GetAll(output, DO_BYTES);//��ȡDOͨ���ĵ�ǰ״̬��������洢��output�����С�

    U16 io;
    io = (output[1] << 8) | output[0];//��output�����е����ݺϲ���һ��16λ����io���Ա���DIͨ����״̬���бȽϡ�
    if (io != s_do[0].m_io)//�������ȣ���ʾDOͨ����״̬�����˱仯��
    {                       
        s_do[0].m_io = io;//Ȼ��io��ֵ����s_do[0].m_io��
        DioUpdate(&s_do[0]);//Ȼ�����DioUpdate(&s_do[0])����������DIͨ����״̬��
    }

#if DO_NUM_MAINBOARD_REAL > 16
    io = (output[3] << 8) | output[2];
    if (io != s_do[1].m_io)
    {
        s_do[1].m_io = io;
        DioUpdate(&s_do[1]);
    }
#endif

#if DO_NUM_MAINBOARD_REAL > 32
    io = (output[5] << 8) | output[4];
    if (io != s_do[2].m_io)
    {
        s_do[2].m_io = io;
        DioUpdate(&s_do[2]);
    }
#endif
}

//���ڸ�����������/�����DI/DO��ͨ����״̬
void BSP_IODrv_update(void)
{
    static U8 s_time = 0;
    static int s_di_step = 0;
    DO_Update();//�������������DO��ͨ����״̬
    out_di_110V(OPEN);//���ڿ���ĳ��DIͨ����״̬����������Ϊ"OPEN"���򿪣�״̬��
    switch (s_di_step)
    {
    case 0:
        // ��90ms
        if (TimeGap(s_time) >= (DOI_UPDATE_TIME - DOI_UPDATE_SHARING_TIME))
        {
            out_di_dtc_close();//�ر�ĳ��DIͨ��
            s_time = sys_time();//����s_time��s_di_step��ֵ��
            s_di_step = 1;
        }
        break;

    case 1:
        // ��10ms
        if (TimeGap(s_time) >= (DOI_UPDATE_SHARING_TIME))
        {
            s_time = sys_time();//����s_time��s_di_step��ֵ��
            s_di_step = 2;
        }
        break;
    case 2:
        s_time = sys_time();
        s_di_step = 0;

        DI_Update();//����DIͨ��״̬
        out_di_dtc_open();//��ĳ��DIͨ��
        U8 input[DI_BYTES];//��ȡ���DIͨ��������ֵ���洢��input������
        input[0] = GETA(s_di[0].m_io);
        input[1] = GETB(s_di[0].m_io);
        input[2] = GETA(s_di[1].m_io);
        input[3] = GETB(s_di[1].m_io);
        input[4] = GETA(s_di[2].m_io);
        input[5] = GETB(s_di[2].m_io);
        input[6] = GETA(s_di[3].m_io);
        input[7] = GETB(s_di[3].m_io);
        input[8] = GETA(s_di[4].m_io);
        input[9] = GETB(s_di[4].m_io);

        di_filer_update(input);//����DIͨ�����˲�������֮ǰ��ȡ��DIͨ������ֵinput���ݸ�����������д���

        break;
    default:
        break;
    }
}
