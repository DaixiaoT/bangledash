
#define IO_FLT_LENTH ((U16)15)
#define IO_FLT_VALIDCOUNTS ((U16)13)

typedef struct DS_INPUTF_
{
    U8 gpioVal;       // 端口原始值
    U8 inVal;         // 滤波后，端口值
    U16 ioHighLevels; // 高电平次数
    U16 ioLowLevels;  // 低电平次数
    U16 FlLenth;      // 滤波区长度
    U16 validCounts;  // 有效电平次数
} DS_INPUTF, *P_DS_INPUTF;

static DS_INPUTF sysInputF[DI_NUM_MAINBOARD] = {0};

static void di_filer_init() // 初始化数字输入通道的滤波器配置
{
    int i;
    for (i = 0; i < DI_NUM_MAINBOARD; i++)
    {
        sysInputF[i].gpioVal = 0;                      // DI通道的GPIO值
        sysInputF[i].inVal = 0;                        // DI通道的输入值
        sysInputF[i].ioHighLevels = 0;                 // DI通道的高电平水平
        sysInputF[i].ioLowLevels = 0;                  // DI通道的低电平水平
        sysInputF[i].FlLenth = IO_FLT_LENTH;           // DI通道的滤波器长度，指定滤波器的时间窗口长度
        sysInputF[i].validCounts = IO_FLT_VALIDCOUNTS; // DI通道的有效计数，指定在多少个有效计数内认定输入信号有效
    }
}

//用于从一个字节数组中提取特定位的值。`get_bit` 宏接受两个参数，`buf` 是字节数组，`n` 是要提取的位的索引。它使用了位运算来提取指定位的值。
#define get_bit(buf, n) GetBit8(buf[(n) >> 3], (n)&7)

//通过一个循环来处理多个DI通道（`DI_NUM_MAINBOARD` 个），并根据输入数据中的位来更新这些通道的状态。
static void di_filer_update(U8 *input)
{
    int i;
    for (i = 0; i < DI_NUM_MAINBOARD; i++)
    {
        sysInputF[i].gpioVal = get_bit(input, i);//使用 `get_bit` 宏从输入数据中获取特定DI通道的状态，并将这个状态存储在 `sysInputF[i].gpioVal` 中
        if (sysInputF[i].gpioVal)
        {
            //如果 `sysInputF[i].gpioVal` 为真（即高电平状态）
            //递增 `ioHighLevels` 计数器，同时递减 `ioLowLevels` 计数器（如果它们大于0）
            if (sysInputF[i].ioHighLevels < sysInputF[i].FlLenth)
                sysInputF[i].ioHighLevels++;
            if (sysInputF[i].ioLowLevels > 0)
                sysInputF[i].ioLowLevels--;
        }
        else
        {
            if (sysInputF[i].ioLowLevels < sysInputF[i].FlLenth)
                sysInputF[i].ioLowLevels++;
            if (sysInputF[i].ioHighLevels > 0)
                sysInputF[i].ioHighLevels--;
        }
        //----------------------------------------------------------------------------------
        // 更新输入状态
        //----------------------------------------------------------------------------------
        //如果 `ioHighLevels` 大于等于 `validCounts`，则将 `inVal` 设置为1，表示输入有效高电平。
        if (sysInputF[i].ioHighLevels >= sysInputF[i].validCounts)
            sysInputF[i].inVal = 1;
        else if (sysInputF[i].ioLowLevels >= sysInputF[i].validCounts)
            sysInputF[i].inVal = 0;

        g_DI.Set(i, sysInputF[i].inVal);
    }
}
