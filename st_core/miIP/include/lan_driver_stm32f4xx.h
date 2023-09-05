#ifndef _LAN_DRIVER_STM32F4xx_H_
#define _LAN_DRIVER_STM32F4xx_H_

int  lan_stm32f4xx_init(U8 *mac, netif_t *nif);
int  lan_stm32f4xx_start();

int lan_stm32f4xx_send(netif_t *nif, packet_t *pk);

void lan_stm32f4xx_poll();    // 定时调用

// 增加接收的组播IP. return 0, ok
//int lan_1788_add_multicast(U32 addr);
//int lan_1788_del_multicast(U32 addr);

#endif //_LAN_DRIVER_STM32F4xx_H_
