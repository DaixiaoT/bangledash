#ifndef __AC_CRTL_H__
#define __AC_CRTL_H__
#include "ac_include.h"
class ACControl
{
public:
    void ProcessMode(U32 run);
    BOOL AdjustRunTimeInfo(BOOL force);
    // BOOL m_set_temp; // ����������¶ȿ���
};
extern ACControl g_car;

#endif //__AC_CRTL_H__
