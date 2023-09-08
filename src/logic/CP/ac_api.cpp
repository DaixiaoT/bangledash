#include "ac_api.h"

AC_PARM g_parm;
AC_RUN_INFO g_runInfo;
AC_ERR_Record g_err_record;

void ac_init()
{
}
void ac_set_ad(int channel, S16 ad)
{
    
}

static inline void get_rtc(U8 *buf)
{
    Time_t t;
    RTC_GetTime(&t);
    buf[0] = t.year;
    buf[1] = t.month;
    buf[2] = t.day;
    buf[3] = t.hour;
    buf[4] = t.minute;
    buf[5] = t.second;
} // 获取系统时钟

static BOOL StoreRunInfo(AC_RUN_INFO *p)
{
    get_rtc(p->rtc); // 获取当前时钟信息
    p->crc = CRC16((U8 *)p, sizeof(AC_RUN_INFO) - 2);

#ifdef _SPI_FRAM_
    SpiFramWrite(AC_RUN_INFO_ADDR, (U8 *)p, sizeof(AC_RUN_INFO));
#endif // _SPI_FRAM_
    return TRUE;
}
void save_run_log()
{
}

int ac_err_log_record(AC_ERR_Record *f)
{
}

BOOL ac_get_record(AC_ERR_Record *f)
{
}

void ac_control(U32 run_sec)
{
#ifndef USE_MODBUS_THEARD
    HMI_Uart4_task(NULL); // 485 接收
#endif                    // USE_MODBUS_THEARD
    static U32 s_time = 0;
    if (TimeGap(s_time) < 1000)
    {
        return;
    }
    s_time = sys_time();

    if (g_car.AdjustRunTimeInfo(FALSE))
    {
        StoreRunInfo(&g_runInfo);
    }

    g_car.ProcessMode(run_sec);

    if (ac_get_record(&g_err_record))
    {
        ac_err_log_record(&g_err_record);
    }

    save_run_log();
    LOG_PRINT("------------------------------------------------------------------------------------\n");
}
