#ifndef _LVF_HMI_H
#define _LVF_HMI_H
#include "ac_include.h"
#include "LVF_PJ.h"
#include "RTC.h"
#include "ac_ctrl.h"
#define MB_ADDR_HMI (49)
#define MB_ADDR_METER1 (0x03)
#define MB_ADDR_METER2 (0x04)
#define MB_ADDR_METER3 (0x05)
#define MB_ADDR_METER4 (0x06)

#define MB_CODE_WR (0x10)
#define MB_CODE_RD (0x03)

#define REG_ADDR_RD_HMI (0x7D0)
#define REG_NUM_RD_HMI (9)
#define REG_POS_HMI (0x02)

#define REG_ADDR_WR_HMI (100)
#define REG_NUM_WR_HMI (81)

#define REG_ADDR_METER (0x130)
#define REG_NUM_METER (51)

#pragma pack(1)
typedef struct
{

} HMI_RD_REG;
typedef struct
{
    U16 wVal;
} BIT_WORD;

typedef struct
{

} HMI_WR_REG;

#pragma pack()

typedef enum
{
    HMI_RD = 0,
    HMI_WR = 1,
    DEV_METER1 = 3,
    DEV_METER2 = 4,
    DEV_METER3 = 5,
    DEV_METER4 = 6,
} device_uart4;

typedef struct
{
    device_uart4 cDev;//表示当前选择执行的操作
    U8 bPosUpdateReady;//表示位置更新是否准备好
    U8 bPosUpdated;//表示位置是否已经更新
    U8 cState;//表示当前执行的状态
    U16 wCarPos;
    U32 wCntErr;
    U32 wCntComu;
} DEV_UART4;
typedef struct
{
    U16 wVal[REG_NUM_METER];
} CURRENT_METER;

typedef struct
{
    U16 wNum;
    U8 data[256];
} SLINK_UART4;

extern SLINK_UART4 sLink_uart4_rcv;
extern SLINK_UART4 sLink_uart4_snd;

#define MIN_RSP_LEN_MB (5 + 1)
#define METER_NUM 4
void HMI_Uart4_task(void *);

#endif
