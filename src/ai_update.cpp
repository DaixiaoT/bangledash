#include "ai_update.h"






void ai_update()
{
    static U8 s_channel = 0;
    static U32 s_time=0;
    if (TimeGap(s_time)<(AI_UPDATE_TIME/ AI_NUM_MAINBOARD))
        return;
    s_time = sys_time();

	short ad;
	if (!ads_AD(s_channel, BASE_VOL_4096, (U16 *)&ad))
		ad = 0x7f00;
    int filt_ad = get_filter_val(FILT_AI_BASE+s_channel, ad, FILT_MID);
    if(filt_ad<0)
        filt_ad = 0;
    if(filt_ad>0x7f00)
        filt_ad = 0x7f00;


    ac_set_ad(s_channel, filt_ad);


    s_channel++;
    if(s_channel >= AI_NUM_MAINBOARD)
        s_channel = 0;
}

