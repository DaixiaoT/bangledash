#ifndef _IP_H_
#define _IP_H_

typedef struct ip_addr {
  U32 addr;
} ip_addr_t;

extern const ip_addr_t ip_addr_any;
extern const ip_addr_t ip_addr_broadcast;

//#define IP_ROUTE_MASK   0xff00

/* 以下route互斥 */
//#define IP_ROUTE_NONE   0x0000        //       0: 无路由
//#define IP_ROUTE_LOCAL  0x0100        //       1: 输出到上层协议
//#define IP_ROUTE_NIC    0x0200        //       2: 输出的网络接口，低8位为NIC下标
//#define IP_ROUTE_BCAST  0x0400        //       3: 向所有NIC发送广播包
//#define IP_ROUTE_MCAST  0x0800        //       3: 向所有允许muticast的NIC发送mcast包

#ifdef BIG_ENDIAN
#define	IP_CLASSA(x) (((x)->addr & 0x80000000) == 0)		/* IP Class A */
#define	IP_CLASSB(x) (((x)->addr & 0xc0000000) == 0x80000000)	/* IP Class B */
#define	IP_CLASSC(x) (((x)->addr & 0xe0000000) == 0xc0000000)	/* IP Class C */
#define	IP_CLASSD(x) (((x)->addr & 0xf0000000) == 0xe0000000)	/* IP Class D */
#define	IP_CLASSE(x) (((x)->addr & 0xf8000000) == 0xf0000000)	/* IP Class E */
#define IP4_ADDR_LOOPBACK  0x7f000001
#define IP4_NETMASK_LOOPBACK 0x7f000000
#else
#define	IP_CLASSA(x) (((x)->addr & 0x80) == 0)		/* IP Class A */
#define	IP_CLASSB(x) (((x)->addr & 0xc0) == 0x80)	/* IP Class B */
#define	IP_CLASSC(x) (((x)->addr & 0xe0) == 0xc0)	/* IP Class C */
#define	IP_CLASSD(x) (((x)->addr & 0xf0) == 0xe0)	/* IP Class D */
#define	IP_CLASSE(x) (((x)->addr & 0xf8) == 0xf0)	    /* IP Class E */
#define IP4_ADDR_LOOPBACK  0x0100007f
#define IP4_NETMASK_LOOPBACK 0x7f
#endif
#define IP_LOOPBACK(a)    (((a)->addr & IP4_NETMASK_LOOPBACK) == IP4_NETMASK_LOOPBACK)

#define ip_addr_mask_equ(a, net, mask) ( ((a)->addr & (mask)->addr) == ((net)->addr & (mask)->addr) )

#define ip_addr_equ(a, b) ((a)->addr == (b)->addr)

// subnet broadcast
#define IP_SUB_BROADCAST(a, net, mask) (((a)->addr&~(mask)->addr)==(0xffffffff&~(mask)->addr) && ip_addr_mask_equ(a, net, mask))

// broadcast
#define IP_BROADCAST(a, net, mask) ( (a)->addr==0 || (a)->addr==0xffffffff || IP_SUB_BROADCAST(a, net, mask) )

#define AF_INET 2 

#define IP4_ADDR_ANY       0
#define IP4_ADDR_BROADCAST 0xffffffff
//#define NET_LOOPBACK      0xff000000 //!!!! 0x7f000000

#define IP_ANY(a) ((a)->addr == IP4_ADDR_ANY)

#define IP_SET_ANY(a) (a)->addr = IP4_ADDR_ANY;


/* protocal type in IP packet */
#define	IPT_ICMP	1	/* protocol type for ICMP packets	*/
#define	IPT_IGMP	2	/* protocol type for IGMP packets	*/
#define	IPT_TCP		6	/* protocol type for TCP packets	*/
#define IPT_EGP		8	/* protocol type for EGP packets	*/
#define	IPT_UDP		17	/* protocol type for UDP packets	*/
#define	IPT_OSPF	89	/* protocol type for OSPF packets	*/

/* ip header */
typedef struct ip_hd {
	U8    ip_verlen;	/* IP version & header length (in longs)*/
	U8    ip_tos;		/* type of service			*/
	U16  ip_len;		/* total packet length (in octets)	*/
	S16   ip_id;		/* datagram id				*/
	S16   ip_fragoff;	/* fragment offset (in 8-octet's)	*/
	U8    ip_ttl;		/* time to live, in gateway hops	*/
	U8    ip_proto;		/* IP protocol (see IPT_* above)	*/
	S16   ip_checksum;	/* header checksum 			*/
	ip_addr_t  ip_src;		/* IP address of source			*/
	ip_addr_t  ip_dst;		/* IP address of destination		*/
}ip_hd_t;

#define	IP_VERSION	4	/* current version value		*/
#define	IP_MINHLEN	5	/* minimum IP header length (in longs)	*/
#define	IP_TTL		64	/* Initial time-to-live value		*/

#define	IP_MF		0x2000	/* more fragments bit			*/
#define	IP_DF		0x4000	/* don't fragment bit			*/
#define	IP_FRAGOFF	0x1fff	/* fragment offset mask			*/
#define	IP_PREC		0xe0	/* precedence portion of TOS		*/

/* IP Precedence values */

#define	IPP_NETCTL	0xe0	/* network control			*/
#define	IPP_INCTL	0xc0	/* internet control			*/
#define	IPP_CRIT	0xa0	/* critical				*/
#define	IPP_FLASHO	0x80	/* flash over-ride			*/
#define	IPP_FLASH	0x60	/* flash 				*/
#define	IPP_IMMED	0x40	/* immediate				*/
#define	IPP_PRIO	0x20	/* priority				*/
#define	IPP_NORMAL	0x00	/* normal				*/

/* macro to compute a datagram's header length (in bytes)		*/
#define	IP_HLEN(p)	((p->ip_verlen & 0xf)<<2)
#define	IPMHLEN		20	/* minimum IP header length (in bytes)	*/

/* IP options */
#define	IPO_COPY	0x80	/* copy on fragment mask		*/
#define IPO_CLASS	0x60	/* option class				*/
#define	IPO_NUM		0x17	/* option number			*/

#define	IPO_EOOP	0x00	/* end of options			*/
#define	IPO_NOP		0x01	/* no operation				*/
#define	IPO_SEC		0x82	/* DoD security/compartmentalization	*/
#define	IPO_LSRCRT	0x83	/* loose source routing			*/
#define	IPO_SSRCRT	0x89	/* strict source routing		*/
#define	IPO_RECRT	0x07	/* record route				*/
#define	IPO_STRID	0x88	/* stream ID				*/
#define	IPO_TIME	0x44	/* internet timestamp			*/


static inline void ip_hton(ip_hd_t *p)
{
	// NOTE: does not include IP options
	p->ip_len = htons(p->ip_len);
	p->ip_id = htons(p->ip_id);
	p->ip_fragoff = htons(p->ip_fragoff);
	//p->ip_dst.addr = htonl(p->ip_dst.addr);
	//p->ip_src.addr = htonl(p->ip_src.addr);
}

static inline void ip_ntoh(ip_hd_t *p)
{
	// NOTE: does not include IP options
	p->ip_len = ntohs(p->ip_len);
	p->ip_id = ntohs(p->ip_id);
	p->ip_fragoff = ntohs(p->ip_fragoff);
	//p->ip_dst.addr = ntohl(p->ip_dst.addr);
	//p->ip_src.addr = ntohl(p->ip_src.addr);
}

struct netif;
struct netif *ip_route(ip_addr_t *dst_ip);

int local_out(struct netif *nif, packet_t * pk, ip_addr_t *dst);

// 底层收到一个IP包，调用此函数把包交给IP模块处理
int ip_in(packet_t *pk);

// 上层(tcp/udp等)发送一个IP包到IP模块
int ip_send_queue(packet_t *pk, const ip_addr_t *dst_ip, U8 proto, U8 tos, U8 ttl);

int isbrc(ip_addr_t *ip);

// IP模块初始化
void ip_init();

// IP processing called in miIP thread
void ip_process();

// brief: 计算包的校验和. tcp/udp等层调用
// parm in: src, dest, proto, proto_len: 伪首部
// return: 校验和
U16 inet_chksum_pseudo(packet_t *pk, ip_addr_t *src, ip_addr_t *dest, U8 proto, U16 proto_len);


// brief: use this function to replace inet_aton();  convert a string ip into ip address
// return 1, ok. 0, fail
int miip_aton(const char *ip_string, ip_addr_t *addr);
#define inet_aton miip_aton

// brief: use this function to replace inet_ntoa();  convert a ip address into string
// return null if buf is not enough
char *miip_ntoa(const ip_addr_t *addr, char *buf, int buflen);
#define inet_ntoa miip_ntoa

#endif /* _IP_H_ */
