#ifndef _AC_API_H_
#define _AC_API_H_
#include "ac_include.h"
#include "ac_ctrl.h"
#include "LVF_HMI.h"
#include "MB85RS256B.h"
void ac_init();

void ac_set_ad(int ch, S16 ad);

void ac_control(U32 run_sec);

#endif // _AC_API_H_