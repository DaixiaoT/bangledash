#ifndef _MI_IP_H_
#define _MI_IP_H_

#include <sys_api.h>
#ifdef _MIIP_
// this configuration should be moved into user config.h
#define NETIF_NUM 2
#define NETIF_DEFAULT 0  // default netif idx
#define NETIF_MAX_HWADDR_LEN  6

// tcp configure
#define DEFAULT_TCP_IN_BUF_SIZE    (4*1024)
#define DEFAULT_TCP_OUT_BUF_SIZE (4*1024)

/* ??? raw tcb 数量 */
#define RAW_TCB_NUM   10

//#define ARP_ENTRY_NUM		20 	/* ARP entry 数量 */
#define	ARP_RESEND		500 	/* resend if no reply in 0.5 sec */
#define	ARP_MAXRETRY		20 	/* ARP 请求重复次数 */
#define ARP_TIMEOUT		30000	/* ARP RESOLVED状态的生命值, 超时重发, 单位:毫秒 */
#define ARP_CHECK_PERIOD	100	// 查看ARP表是否超时

#define MIIP_THREAD_PRIO 100

#define DNS_SERVER_NUM 2  // 最多2个DNS server

//#define RAW
//#define IGMP
//#define MIIP_DHCP

//STM32F4x7允许通过硬件识别和计算IP,UDP和ICMP的帧校验和

//#define CHECKSUM_BY_HARDWARE //定义CHECKSUM_BY_HARDWARE,使用硬件帧校验


#ifdef CHECKSUM_BY_HARDWARE
  //CHECKSUM_GEN_IP==0: 硬件生成IP数据包的帧校验和
  #define CHECKSUM_GEN_IP                 0
  //CHECKSUM_GEN_UDP==0: 硬件生成UDP数据包的帧校验和
  #define CHECKSUM_GEN_UDP                0
  //CHECKSUM_GEN_TCP==0: 硬件生成TCP数据包的帧校验和
  #define CHECKSUM_GEN_TCP                0
  //CHECKSUM_CHECK_IP==0: 硬件检查输入的IP数据包帧校验和
  #define CHECKSUM_CHECK_IP               0
  //CHECKSUM_CHECK_UDP==0: 硬件检查输入的UDP数据包帧校验和
  #define CHECKSUM_CHECK_UDP              0
  //CHECKSUM_CHECK_TCP==0: 硬件检查输入的TCP数据包帧校验和
  #define CHECKSUM_CHECK_TCP              0
#else
  /* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
  #define CHECKSUM_GEN_IP                 1
  /* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.*/
  #define CHECKSUM_GEN_UDP                1
  /* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.*/
  #define CHECKSUM_GEN_TCP                1
  /* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
  #define CHECKSUM_CHECK_IP               1
  /* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.*/
  #define CHECKSUM_CHECK_UDP              1
  /* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.*/
  #define CHECKSUM_CHECK_TCP              1
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <list.h>
#include <ringbuf.h>
//#include <mi_mem.h>
#include <neterr.h>
#include <packet.h>
#include <ipv4.h>
#include <icmpv4.h>
#include <netif.h>
#include <etharp.h>
#include <udp.h>
#include <tcp.h>
#include <dhcp.h>
#include <dns.h>
#include <lldp.h>

int miip_init();

// user lan driver api:
int lan_driver_init();
void lan_driver_start();
void lan_driver_process();
int lan_driver_reset_addr(U32 ip,U32 netmask,U32 gw);
int lan_driver_getset_addr(U32 *ip,U32 *netmask,U32 *gw);
int lan_driver_get_mac(U8 mac[6]);

#define MIIP_LOG

#ifdef MIIP_LOG
// miip debug functions
void packet_log(int pk_num, int ext_num, int pk_bytes);
void tcp_tcb_log(int tcb_num);
void tcp_seg_log(int num);
void miip_scan();
#else
#define packet_log(a, b, c)
#define tcp_tcb_log(a)
#define tcp_seg_log(a)
#define miip_scan()
#endif // MIIP_LOG

#ifdef __cplusplus
}
#endif

#endif//_MIIP_
#endif
