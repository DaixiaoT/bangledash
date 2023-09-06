#ifndef __MAINTENANCE_H__
#define __MAINTENANCE_H__
#include "ac_include.h"

enum MAINTENANCE_BUS
{
	MAINTENANCE_UART,//´®¿ÚÍ¨ÐÅ
	MAINTENANCE_USB,
	MAINTENANCE_NET,
};
extern U32 g_down_log_tick;
void maintenance_uart_recv(int bus, U8 *buf, int len);

void maintenance_update();

#endif//__MAINTENANCE_H__


