#include "ac_api.h"


AC_PARM g_parm;

void ac_init()
{
}
void ac_set_ad(int ch, S16 ad)
{
}

void ac_control(U32 run_sec)
{
#ifndef USE_MODBUS_THEARD
    HMI_Uart4_task(NULL); // 485 Ω” ’
#endif                    // USE_MODBUS_THEARD
}
