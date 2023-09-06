#ifndef __ADS1113_H__
#define __ADS1113_H__

typedef enum 
{
	BASE_VOL_4096 = 1,
	BASE_VOL_2048 = 2,
	BASE_VOL_256 = 5,
}AD_BASE_VOL;

BOOL ads_AD(U8 temp,AD_BASE_VOL BASE, U16 *ad);


#endif//__ADS1113_H__

