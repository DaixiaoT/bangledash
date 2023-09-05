#ifndef _UDP_H_
#define _UDP_H_
#ifdef _MIIP_

/* UDP header length in bytes	*/
#define	UDP_HLEN	8

/* maximum data in UDP packet	*/
#define	UDP_MAXLEN	(EP_DLEN - IPMHLEN - UDP_HLEN)

// UDP 包头
typedef struct	udp_hd {		/* message format of DARPA UDP	*/
	U16	u_src;		/* source UDP port number	*/
	U16	u_dst;		/* destination UDP port number	*/
	U16	u_len;		/* length of UDP data		*/
	U16	u_cksum;	/* UDP checksum (0 => none)	*/
}udp_hd_t;

#define UDP_MARK 0x9da53f0b

// udp 控制块
typedef struct udp_tcb
{
    U32 mark;            // should always be UDP_MARK
    struct udp_tcb *next;
    netif_t *nif;           // bind 的netif，主要用在dhcp

	U16 local_port;        /* 本地端口 	*/
	U16 remote_port;       /* 连接的远端端口 */
	ip_addr_t local_ip;          /* 本地IP */
	ip_addr_t remote_ip;         /* 连接的远端IP */

	/* 输入队列 */
	packet_t *in_head;
	packet_t *in_tail;
} udp_tcb_t;

static inline void udp_ntoh(udp_hd_t *p)
{
	p->u_src = ntohs(p->u_src);
	p->u_dst = ntohs(p->u_dst);
	p->u_len = ntohs(p->u_len);
}

static inline void udp_hton(udp_hd_t *p)
{
	p->u_src = htons(p->u_src);
	p->u_dst = htons(p->u_dst);
	p->u_len = htons(p->u_len);
}

void udp_init(void);

// 打印udp模块信息
void udp_dump();

// brief: udp输入包处理
// parm in: pk, 输入包; pip: 此包的ip协议头
int udp_in(packet_t *pk, ip_hd_t *pip);

// brief: 打开一个UDP连接
// parm out: handle: store the opend udp handle if success.
// return 0 success;
// return <0, error
int udp_open(U32 *handle);

// brief: 关闭UDP连接
// return 0 ok;
// return <0, error
int udp_close(U32 handle);

// brief: bind the udp handle to local ip:port，如果nif非0，同时bind到此nif；dhcp中需要
// parm in: local_ip, nif，都可以为0.
// return 0, ok, <0, error
int udp_bind_if(U32 handle, const ip_addr_t *local_ip, U16 local_port, netif_t *nif);
#define udp_bind(handle, local_ip, local_port) udp_bind_if(handle, local_ip, local_port, 0)

// brief: connect a udp handle to remote_ip:remote_port
// return 0, ok, <0, error
int udp_connect(U32 handle, const ip_addr_t *remote_ip, U16 remote_port);

// brief: break the connection of the udp handle
// return 0, ok, <0, error
int udp_disconnect(U32 handle);

static inline U16 udp_local_port(U32 handle)
{
	return ((udp_tcb_t *)handle)->local_port;
}

// brief: UDP发送数据的通用函数。可以指定发送的目标ip:port, 以及通过哪个netif发送。
// parm_in: dst_ip:dst_port: 目标ip:port, 可以为空。若为空则使用udp_connect()提供的目标ip:port
//                nif: 通过哪个netif发送；=0表示不指定netif，由miIP自动选择。仅在dhcp中会指定nif
// return >=0, the bytes send. <0, error
// nonblocking, multi thread safe
int udp_sendto_if(U32 handle, const U8 *buf, int len, const ip_addr_t *dst_ip, U16 dst_port, netif_t *nif);
#define udp_send(handle, buf, len) udp_sendto_if(handle, buf, len, 0, 0, 0)
#define udp_sendto(handle, buf, len, dst_ip, dst_port) udp_sendto_if(handle, buf, len, dst_ip, dst_port, 0)

// brief: 直接发送packet. 注意必须在packet中预留以太网头，IP头，UDP头的空间
// return 0, ok
//            <0, error, 此时pk未被处理；需要调用者自己删除packet
int udp_sendto_packet(U32 handle, packet_t *pk, const ip_addr_t *dst_ip, U16 dst_port);
#define udp_send_packet(handle, pk) udp_sendto_packet(handle, pk, 0, 0)

// brief: recv data from udp handle
//  parm_out: from_ip:from_port 如果这两个指针非空，存放此包的源ip:port。
// return >=0, the bytes recved.
//             NE_EMPTY_QUEUE 没有包可接收
//             other: error
// nonblocking, multi thread safe
int udp_recvfrom(U32 handle, U8 *buf, int len, ip_addr_t *from_ip, U16 *from_port);
int udp_recvfrom_if(U32 handle, U8 *buf, int len, ip_addr_t *from_ip, U16 *from_port,netif_t **nif);
#define udp_recv(handle, buf, len) udp_recvfrom(handle, buf, len, 0, 0)

// brief: recv a udp packet from udp handle
//  parm_out:
//             ppk: pointer to store the received packet if ret==0
//             from_ip:from_port 如果这两个指针非空，存放此包的源ip:port。
// return ==0, 收到一个包
//             NE_EMPTY_QUEUE 没有包可接收
//             other: error
// nonblocking, multi thread safe
int udp_recvfrom_packet(U32 handle, packet_t **ppk, ip_addr_t *from_ip, U16 *from_port);
#define udp_recv_packet(handle, ppk) udp_recvfrom_packet(handle, ppk, 0, 0)

// 是否提供ref接口？只读。
// 是否提供packet send/recv 接口？好像也没必要

#endif//_MIIP_	
#endif /* _UDP_H_ */
