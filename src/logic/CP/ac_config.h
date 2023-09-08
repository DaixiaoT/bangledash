#ifndef __AC_CONFIG_H__
#define __AC_CONFIG_H__
#include "ac_include.h"

#ifdef _SPI_FRAM_
#define AC_PARM_ADDR     (0)
#define AC_RUN_INFO_ADDR (0x200)
#endif // _SPI_FRAM_

// 控制器全局配置参数
#pragma pack(push, 1)
struct AC_PARM
{
    
};

struct AC_RUN_INFO{


    U8 rtc[6];
    U16 crc;
};

struct AC_ERR_Record {//目前定义为故障数据
	U16 crc;
};

#pragma pack(pop)


#define DI_NUM_MAINBOARD (76)
#define DO_NUM_MAINBOARD (48)
#define AI_NUM_MAINBOARD (6)

#define DI_NUM (DI_NUM_MAINBOARD)
#define DO_NUM (DO_NUM_MAINBOARD)
#define AI_NUM (AI_NUM_MAINBOARD)

#define DI_BYTES ((DI_NUM + 7) >> 3)
#define DO_BYTES ((DO_NUM + 7) >> 3)

extern AC_PARM g_parm;
extern AC_RUN_INFO g_runInfo;
extern AC_ERR_Record g_err_record;
#endif // !__AC_CONFIG_H__
