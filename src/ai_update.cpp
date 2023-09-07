#include "ai_update.h"

//主要作用是定期更新模拟输入数据，将其滤波处理后存储，并将处理后的数据设置到相应的模拟控制器通道中。
//这个函数可能会在系统中周期性地调用，以确保模拟输入数据的更新和处理。
void ai_update()
{
    static U8 s_channel = 0;//`s_channel` 变量用于跟踪当前处理的模拟输入通道，
    static U32 s_time = 0;//`s_time` 用于记录上一次处理的时间。

    //用于控制处理模拟输入数据的更新频率。
    if (TimeGap(s_time) < (AI_UPDATE_TIME / AI_NUM_MAINBOARD))
        return;

    s_time = sys_time();

    short ad;//用于存储从模拟输入通道读取的原始模拟输入数据。

    //从模拟输入通道 `s_channel` 读取模拟输入数据，并将其存储在 `ad` 变量中。如果读取失败（返回值为非零），则将 `ad` 设置为 `0x7f00`。
    if (!ads_AD(s_channel, BASE_VOL_4096, (U16 *)&ad))
        ad = 0x7f00;

    //对读取的原始模拟输入数据 `ad` 进行滤波处理，结果存储在 `filt_ad` 变量中。
    U8 filt_ad = get_filter_val(FILT_AI_BASE + s_channel, ad, FILT_MID);
    if (filt_ad < 0)
        filt_ad = 0;
    if (filt_ad > 0x7f00)
        filt_ad = 0x7f00;

    //将经过滤波处理的模拟输入数据 `filt_ad` 设置到某个模拟控制器通道 `s_channel` 中，以供后续处理或使用。
    ac_set_ad(s_channel, filt_ad);

    //更新当前处理的模拟输入通道。`s_channel` 递增，如果超过了 `AI_NUM_MAINBOARD` 的数量，就会重新从第一个通道开始处理。
    s_channel++;
    if (s_channel >= AI_NUM_MAINBOARD)
        s_channel = 0;
}
