#ifndef  _ETHARP_H_
#define _ETHARP_H_

#define	EP_MINLEN	  60	/* minimum packet length		*/
#define	EP_DLEN		1500	/* length of data field ep		*/
#define	EP_HLEN		  14	/* size of (extended) Ether header	*/
#define	EP_HLEN_A4  16	// ehternet header, aligned to dword
#define	EP_CRC		   4	/* ether CRC (trailer)			*/
#define	EP_MAXLEN	(EP_HLEN+EP_DLEN)
#define	EP_ALEN	6		/* number of octets in physical address	*/
//typedef	unsigned char	Eaddr[EP_ALEN]; /* a physical Ethernet address	*/
#define	EP_RETRY	3	/* number of times to retry xmit errors	*/
#define	EP_BROADCAST "\377\377\377\377\377\377"/* Ethernet broadcast address	*/
#define EP_RTO 300		/* time out in seconds for reads	*/

/* ethernet protocol type */
#define ETHTYPE_RARP	  0x8035	/* type: Reverse ARP		*/
#define ETHTYPE_ARP       0x0806U
#define ETHTYPE_IP        0x0800U
#define ETHTYPE_VLAN      0x8100U
#define ETHTYPE_PPPOEDISC 0x8863U	/* PPP Over Ethernet Discovery Stage */
#define ETHTYPE_PPPOE     0x8864U	/* PPP Over Ethernet Session Stage */
#define ETHTYPE_EAPOL     0x888e	/* EAPOL */
#define ETHTYPE_LLDP      0x88CCU	/* LLDP */

/* 以太网包头 */
typedef struct ethernet_header {
	U8  dst[6];
	U8  src[6];
	U16 type;
} ethernet_t;

typedef struct eth_llc_hdr {
  U8  dsap;	/* destination SAP */
  U8  ssap;	    /* source SAP */
  U8  llc;		    /* LLC control field */
  U8  protid[3];	/* protocol id */
  U16 type;	/* ether type field */
} eth_llc_hdr_t;

#define SIZEOF_ETH_HDR sizeof(ethernet_t)
#define SIZEOF_ETH_LLC_HDR sizeof(eth_llc_hdr_t)

static inline void ethernet_ntoh(ethernet_t *peth)
{
	peth->type = ntohs(peth->type);
}

static inline void ethernet_hton(ethernet_t *peth)
{
	peth->type = htons(peth->type);
}

/*
 * ???
 */
static inline void ethernet_map_ip_multicast(U32 ip, U8 *mac)
{
	mac[0] = 0x01;
	mac[1] = 0x00;
	mac[2] = 0x5E;
	mac[3] = (U8)((ip >> 16) & 0x7F);
	mac[4] = (U8)((ip>>8) & 0xFF);
	mac[5] = (U8)(ip & 0xFF);
}


#define ARP_HW_ETH	1         /* 硬件类型: 以太网 */
#define ARP_IPV4	0x0800    /* 协议类型: IPv4 */

#define	AR_REQUEST	1	/* ARP request to resolve address	*/
#define	AR_REPLY	2	/* reply to a resolve request		*/

#define	RA_REQUEST	3	/* reverse ARP request (RARP packets)	*/
#define	RA_REPLY	4	/* reply to a reverse request (RARP ")	*/

#define IP_ADDR_LEN sizeof(ip_addr_t)

typedef struct eth_arp {
	U16	hwtype;		/* hardware type			*/
	U16	prtype;		/* protocol type			*/
	U8	hwlen;		/* hardware address length		*/
	U8	prlen;		/* protocol address length		*/
	U16	op;		/* ARP operation (see list above)	*/
	U8	src_mac[6];
	U8	src_ip[IP_ADDR_LEN];
	U8	dst_mac[6];
	U8	dst_ip[IP_ADDR_LEN];
} eth_arp_t;

#define ETH_ARP_LEN sizeof(eth_arp_t)

/************************************************************************
 * ARP缓冲信息结构
 ************************************************************************/
typedef struct arpentry {	/* format of entry in ARP cache		*/
    struct arpentry *next;
	U8	mac[6];		/* Hardware address 			*/
	U8	state;		/* state of this entry (see below)	*/
	U8	attempts;	/* number of retries so far 		*/
	int	life;	    	/* 生命期				*/
	ip_addr_t	ip;		/* Protocol address 			*/
	netif_t	*nif;   	    /* netif index 			*/
	/* queue of packets for this address	*/
	struct packet *wait_head;
	struct packet *wait_tail;
} arpentry_t;

/************************************************************************
 * 	ARP缓冲状态
 ************************************************************************/
//#define	AES_FREE	0	/* Entry is unused (initial value)	*/
#define	AES_PENDING	1	/* Entry is used but incomplete		*/
#define	AES_RESOLVED	2	/* Entry has been resolved		*/

static inline void eth_arp_ntoh(eth_arp_t *parp)
{
	parp->hwtype = ntohs(parp->hwtype);
	parp->prtype = ntohs(parp->prtype);
	parp->op = ntohs(parp->op);
}

static inline void eth_arp_hton(eth_arp_t *parp)
{
	parp->hwtype = htons(parp->hwtype);
	parp->prtype = htons(parp->prtype);
	parp->op = htons(parp->op);
}

static inline void copy_eth_mac(U8 *dst, U8 *src)
{
	dst[0]= src[0];
	dst[1]= src[1];
	dst[2]= src[2];
	dst[3]= src[3];
	dst[4]= src[4];
	dst[5]= src[5];
}

static inline void clear_eth_mac(U8 *mac)
{
	mac[0] = 0;
	mac[1] = 0;
	mac[2] = 0;
	mac[3] = 0;
	mac[4] = 0;
	mac[5] = 0;
}

static inline int compare_mac(U8 *mac1, U8 *mac2)
{
	return mac1[0]!=mac2[0] || mac1[1]!=mac2[1] || mac1[2]!=mac2[2] ||
		   mac1[3]!=mac2[3] || mac1[4]!=mac2[4] || mac1[5]!=mac2[5];
}

void ethernet_init();

// 打印ethernet arp模块信息
void etharp_dump();

// clear all arp table
void etharp_clear();

// clear all arp table related to nif
void etharp_clean_netif(netif_t *nif);

// brief: send a ethernet packet throw nif
// parm in: next_hop: the ip address send to
// return 0, ok, <0, error
int ethernet_send(netif_t *nif, packet_t *pk, ip_addr_t *next_hop);

// brief: low level driver received an ethernet packet send it to ethernet module
void ethernet_in(packet_t *pk);

// ethernet processing function called by miIP thread
void ethernet_process(void);

// 发送arp请求包
int arp_request(netif_t *nif, ip_addr_t *dst_ip);

#endif // _ETHARP_H_
