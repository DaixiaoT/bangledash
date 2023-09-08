#include "BSP_IODrv.h"

#define GETA(n) ((U8)(n)&0xff)
#define GETB(n) ((U8)((n) >> 8))

#define CLOSE 0
#define OPEN 1

typedef struct DIOInfo
{
    U8 m_addr;              // 7位地址
    U16 m_dir;              // 1 = 引脚配置为输入,0 = 引脚配置为输出
    U16 m_ipol;             // 1 = GPIO寄存器位将反映输入引脚的相反逻辑状态,0 = GPIO寄存器位将反映输入引脚的相同逻辑状态
    GPIO_TypeDef *m_csprot; // 片选端口
    U16 m_csbit;            // 片选管脚
    U16 m_io;               // IO值
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
    // 这一行代码将配置硬件地址引脚，其中pDio->m_addr表示MCP23S17芯片的地址，MCP_IOCONA表示要配置的寄存器，
    //(1 << 3)设置了引脚的某个位，pDio->m_csprot和pDio->m_csbit是用于芯片通信的控制参数。
    // 使能硬件地址引脚，第一次要设置两次，不知道为什么
    MCP23S17_Write(pDio->m_addr, MCP_IOCONA, (1 << 3), pDio->m_csprot, pDio->m_csbit);
    MCP23S17_Write(pDio->m_addr, MCP_IOCONA, (1 << 3), pDio->m_csprot, pDio->m_csbit);

    MCP23S17_Write(pDio->m_addr, MCP_IOCONB, (1 << 3), pDio->m_csprot, pDio->m_csbit);
    // 设置方向
    MCP23S17_Write(pDio->m_addr, MCP_IODIRA, GETA(pDio->m_dir), pDio->m_csprot, pDio->m_csbit);
    MCP23S17_Write(pDio->m_addr, MCP_IODIRB, GETB(pDio->m_dir), pDio->m_csprot, pDio->m_csbit);

    // 设置输入极性端口
    MCP23S17_Write(pDio->m_addr, MCP_IPOLA, GETA(pDio->m_ipol), pDio->m_csprot, pDio->m_csbit);
    MCP23S17_Write(pDio->m_addr, MCP_IPOLB, GETB(pDio->m_ipol), pDio->m_csprot, pDio->m_csbit);

    if (pDio->m_dir == MCP_DIR_OUT)
    {
        // 默认输出为低
        MCP23S17_Write(pDio->m_addr, MCP_GPIOA, 0, pDio->m_csprot, pDio->m_csbit);
        MCP23S17_Write(pDio->m_addr, MCP_GPIOB, 0, pDio->m_csprot, pDio->m_csbit);
    }
}

void BSP_IODrv_init()
{
    g_DO.m_num = DO_NUM;
    g_DI.m_num = DI_NUM;

    MCP23S17_Init(); // 初始化MCP23S17芯片，这是一种I/O扩展器芯片，可以通过SPI接口与嵌入式系统通信，用于扩展输入/输出引脚。

    BSP_IODrv_Config(&s_di[0]);
    BSP_IODrv_Config(&s_di[1]);
    BSP_IODrv_Config(&s_di[2]);
    BSP_IODrv_Config(&s_di[3]);
    BSP_IODrv_Config(&s_di[4]);

    BSP_IODrv_Config(&s_do[0]);
    BSP_IODrv_Config(&s_do[1]);
    BSP_IODrv_Config(&s_do[2]);

    di_filer_init(); // 初始化数字输入的滤波器。
}

//控制具有24V或110V电源的数字输出通道，根据传入的状态来控制通道的开关状态
void out_di_110V(BOOL s) // DI加上24V或110V
{
    gpioSetValue(GPIOE, 11, s);
    gpioSetValue(GPIOE, 12, s);
    gpioSetValue(GPIOE, 3, s);
    gpioSetValue(GPIOE, 4, s);
}
//用于控制分时采集相关的数字输出通道
void out_di_dtc(BOOL s)
{
    gpioSetValue(GPIOE, 13, s);
    gpioSetValue(GPIOE, 15, s);
    gpioSetValue(GPIOG, 6, s);
    gpioSetValue(GPIOG, 7, s);
}

void out_di_dtc_open()
{ // 断开
    out_di_dtc(OPEN);
}

void out_di_dtc_close()
{ // 闭合
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

    if (pDio->m_dir == MCP_DIR_IN)//检查通道的方向
    {
        pDio->m_io = DI_Read_Verify(pDio);//如果方向为输入，则读取DIO通道状态，并将结果存储在pDio->m_io中
    }
    else // 输出
    {
        DO_Write_Verify(pDio);//如果方向为输出，则写入DIO通道的状态
    }
}
//更新数字输入（DI）通道的状态
static void DI_Update()
{
    MCP23S17_CS_IN4_HIGH();//设置片选引脚为高电平
    DioUpdate(&s_di[0]);//更新三个不同DI通道的状态
    DioUpdate(&s_di[1]);
    DioUpdate(&s_di[2]);
}

//更新数字输出（DO）通道的状态
static void DO_Update()
{
    U8 output[DO_BYTES];//定义了一个长度为DO_BYTES的output数组，用于存储DO通道的状态信息。
    mem_set(output, 0, DO_BYTES);//元素初始化为0，清除之前的状态。
    g_DO.GetAll(output, DO_BYTES);//获取DO通道的当前状态，并将其存储在output数组中。

    U16 io;
    io = (output[1] << 8) | output[0];//将output数组中的数据合并成一个16位整数io，以便与DI通道的状态进行比较。
    if (io != s_do[0].m_io)//如果不相等，表示DO通道的状态发生了变化，
    {                       
        s_do[0].m_io = io;//然后将io的值赋给s_do[0].m_io，
        DioUpdate(&s_do[0]);//然后调用DioUpdate(&s_do[0])函数来更新DI通道的状态。
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

//用于更新数字输入/输出（DI/DO）通道的状态
void BSP_IODrv_update(void)
{
    static U8 s_time = 0;
    static int s_di_step = 0;
    DO_Update();//更新数字输出（DO）通道的状态
    out_di_110V(OPEN);//用于控制某个DI通道的状态，将其设置为"OPEN"（打开）状态。
    switch (s_di_step)
    {
    case 0:
        // 等90ms
        if (TimeGap(s_time) >= (DOI_UPDATE_TIME - DOI_UPDATE_SHARING_TIME))
        {
            out_di_dtc_close();//关闭某个DI通道
            s_time = sys_time();//更新s_time和s_di_step的值。
            s_di_step = 1;
        }
        break;

    case 1:
        // 等10ms
        if (TimeGap(s_time) >= (DOI_UPDATE_SHARING_TIME))
        {
            s_time = sys_time();//更新s_time和s_di_step的值。
            s_di_step = 2;
        }
        break;
    case 2:
        s_time = sys_time();
        s_di_step = 0;

        DI_Update();//更新DI通道状态
        out_di_dtc_open();//打开某个DI通道
        U8 input[DI_BYTES];//获取多个DI通道的输入值并存储在input数组中
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

        di_filer_update(input);//更新DI通道的滤波器，将之前获取的DI通道输入值input传递给这个函数进行处理

        break;
    default:
        break;
    }
}
