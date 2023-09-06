
#define IO_FLT_LENTH ((U16)15)
#define IO_FLT_VALIDCOUNTS ((U16)13)

typedef struct DS_INPUTF_
{
    U8 gpioVal;       // �˿�ԭʼֵ
    U8 inVal;         // �˲��󣬶˿�ֵ
    U16 ioHighLevels; // �ߵ�ƽ����
    U16 ioLowLevels;  // �͵�ƽ����
    U16 FlLenth;      // �˲�������
    U16 validCounts;  // ��Ч��ƽ����
} DS_INPUTF, *P_DS_INPUTF;

static DS_INPUTF sysInputF[DI_NUM_MAINBOARD] = {0};

static void di_filer_init() // ��ʼ����������ͨ�����˲�������
{
    int i;
    for (i = 0; i < DI_NUM_MAINBOARD; i++)
    {
        sysInputF[i].gpioVal = 0;                      // DIͨ����GPIOֵ
        sysInputF[i].inVal = 0;                        // DIͨ��������ֵ
        sysInputF[i].ioHighLevels = 0;                 // DIͨ���ĸߵ�ƽˮƽ
        sysInputF[i].ioLowLevels = 0;                  // DIͨ���ĵ͵�ƽˮƽ
        sysInputF[i].FlLenth = IO_FLT_LENTH;           // DIͨ�����˲������ȣ�ָ���˲�����ʱ�䴰�ڳ���
        sysInputF[i].validCounts = IO_FLT_VALIDCOUNTS; // DIͨ������Ч������ָ���ڶ��ٸ���Ч�������϶������ź���Ч
    }
}

//���ڴ�һ���ֽ���������ȡ�ض�λ��ֵ��`get_bit` ���������������`buf` ���ֽ����飬`n` ��Ҫ��ȡ��λ����������ʹ����λ��������ȡָ��λ��ֵ��
#define get_bit(buf, n) GetBit8(buf[(n) >> 3], (n)&7)

//ͨ��һ��ѭ����������DIͨ����`DI_NUM_MAINBOARD` ���������������������е�λ��������Щͨ����״̬��
static void di_filer_update(U8 *input)
{
    int i;
    for (i = 0; i < DI_NUM_MAINBOARD; i++)
    {
        sysInputF[i].gpioVal = get_bit(input, i);//ʹ�� `get_bit` ������������л�ȡ�ض�DIͨ����״̬���������״̬�洢�� `sysInputF[i].gpioVal` ��
        if (sysInputF[i].gpioVal)
        {
            //��� `sysInputF[i].gpioVal` Ϊ�棨���ߵ�ƽ״̬��
            //���� `ioHighLevels` ��������ͬʱ�ݼ� `ioLowLevels` ��������������Ǵ���0��
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
        // ��������״̬
        //----------------------------------------------------------------------------------
        //��� `ioHighLevels` ���ڵ��� `validCounts`���� `inVal` ����Ϊ1����ʾ������Ч�ߵ�ƽ��
        if (sysInputF[i].ioHighLevels >= sysInputF[i].validCounts)
            sysInputF[i].inVal = 1;
        else if (sysInputF[i].ioLowLevels >= sysInputF[i].validCounts)
            sysInputF[i].inVal = 0;

        g_DI.Set(i, sysInputF[i].inVal);
    }
}
