#ifndef  _DHCP_H_
#define _DHCP_H_

#define DHCP_CLIENT_PORT  68
#define DHCP_SERVER_PORT  67

#define DHCP_BOOTREQUEST  1
#define DHCP_BOOTREPLY    2

/** DHCP message types */
#define DHCP_DISCOVER 1
#define DHCP_OFFER    2
#define DHCP_REQUEST  3
#define DHCP_DECLINE  4
#define DHCP_ACK      5
#define DHCP_NAK      6
#define DHCP_RELEASE  7
#define DHCP_INFORM   8

/** DHCP hardware type, currently only ethernet is supported */
#define DHCP_HTYPE_ETH 1

#define DHCP_MAGIC_COOKIE 0x63825363UL

/** BootP options */
#define DHCP_OPTION_PAD 0
#define DHCP_OPTION_SUBNET_MASK 1 /* RFC 2132 3.3 */
#define DHCP_OPTION_ROUTER 3
#define DHCP_OPTION_DNS_SERVER 6
#define DHCP_OPTION_HOSTNAME 12
#define DHCP_OPTION_IP_TTL 23
#define DHCP_OPTION_MTU 26
#define DHCP_OPTION_BROADCAST 28
#define DHCP_OPTION_TCP_TTL 37
#define DHCP_OPTION_END 255

/** DHCP options */
#define DHCP_OPTION_REQUESTED_IP 50 /* RFC 2132 9.1, requested IP address */
#define DHCP_OPTION_LEASE_TIME 51 /* RFC 2132 9.2, time in seconds, in 4 bytes */
#define DHCP_OPTION_OVERLOAD 52 /* RFC2132 9.3, use file and/or sname field for options */

#define DHCP_OPTION_MESSAGE_TYPE 53 /* RFC 2132 9.6, important for DHCP */

#define DHCP_OPTION_SERVER_ID 54 /* RFC 2132 9.7, server IP address */
#define DHCP_OPTION_PARAMETER_REQUEST_LIST 55 /* RFC 2132 9.8, requested option types */

#define DHCP_OPTION_MAX_MSG_SIZE 57 /* RFC 2132 9.10, message size accepted >= 576 */

#define DHCP_OPTION_T1 58 /* T1 renewal time */
#define DHCP_OPTION_T2 59 /* T2 rebinding time */
#define DHCP_OPTION_US 60
#define DHCP_OPTION_CLIENT_ID 61
#define DHCP_OPTION_TFTP_SERVERNAME 66
#define DHCP_OPTION_BOOTFILE 67

/** DHCP client states */
#define DHCPS_OFF                   0
#define DHCPS_BACKING_OFF  1
#define DHCPS_SELECTING       2
#define DHCPS_REQUESTING   3
#define DHCPS_CHECKING       4
#define DHCPS_BOUND            5
#define DHCPS_RENEWING      6
#define DHCPS_REBINDING     7
#define DHCPS_REBOOTING    8

#define DHCP_OPTION_LEN 128

typedef struct dhcp_msg {
    U8 op;
    U8 htype;
    U8 hlen;
    U8 hops;
    U32 xid;
    U16 segs;
    U16 flags;
    ip_addr_t ciaddr;
    ip_addr_t yiaddr;
    ip_addr_t siaddr;
    ip_addr_t giaddr;
    U8 chaddr[16];
    U8 sname[64];
    char file[128];
    U32 cookie;
    U8 options[DHCP_OPTION_LEN];
} dhcp_msg_t;

// dhcp control block
typedef struct dhcp {
    U32 xid;
    U32 handle;  // udp handle, 0=none
    U8   state;
    U8   tries;    // 在当前状态下，发送包的次数
    U16 option_len;      // 发送包或接收包的option字节数

    U32 request_timeout;        // 超时，
    U32 t1_timeout;
    U32 t2_timeout;

    //U32 offered_t0_lease; /* lease period (in seconds) */
    U32 offered_t1_renew; /* recommended renew time (usually 50% of lease period) */
    U32 offered_t2_rebind; /* recommended rebind time (usually 66% of lease period)  */

    //S16  send_life;      // 上次发送到现在的时间，单位200ms;
    //int life;        // 处在当前状态的时间，单位: 200ms
    ip_addr_t server_ip_addr; /* dhcp server address that offered this lease */
    ip_addr_t offered_ip_addr;
    ip_addr_t offered_sn_mask;
    ip_addr_t offered_gw_addr;
    //ip_addr_t offered_dns[2];

    dhcp_msg_t msg;
} dhcp_t;

// dhcp init
void dhcp_init();

// dhcp处理，由miIP线程调用
void dhcp_process();

// brief: ethernet arp 收到ARP包后，会调用此函数检测IP地址是否有冲突
// called from eth_arp_in(): by tcpip thread
void dhcp_arp_reply(netif_t *netif, ip_addr_t *addr);

// 启动动态IP地址分配
int dhcp_start(netif_t *netif);
// 停止动态IP地址分配
int dhcp_stop(netif_t *netif);

// 网络状态发生了变化
void dhcp_network_changed(struct netif *netif);

#endif //_DHCP_H_

