#ifndef _DNS_H_
#define _DNS_H_

/** DNS field TYPE used for "Resource Records" */
#define DNS_RRTYPE_A              1     /* a host address */
#define DNS_RRTYPE_NS             2     /* an authoritative name server */
#define DNS_RRTYPE_MD             3     /* a mail destination (Obsolete - use MX) */
#define DNS_RRTYPE_MF             4     /* a mail forwarder (Obsolete - use MX) */
#define DNS_RRTYPE_CNAME          5     /* the canonical name for an alias */
#define DNS_RRTYPE_SOA            6     /* marks the start of a zone of authority */
#define DNS_RRTYPE_MB             7     /* a mailbox domain name (EXPERIMENTAL) */
#define DNS_RRTYPE_MG             8     /* a mail group member (EXPERIMENTAL) */
#define DNS_RRTYPE_MR             9     /* a mail rename domain name (EXPERIMENTAL) */
#define DNS_RRTYPE_NULL           10    /* a null RR (EXPERIMENTAL) */
#define DNS_RRTYPE_WKS            11    /* a well known service description */
#define DNS_RRTYPE_PTR            12    /* a domain name pointer */
#define DNS_RRTYPE_HINFO          13    /* host information */
#define DNS_RRTYPE_MINFO          14    /* mailbox or mail list information */
#define DNS_RRTYPE_MX             15    /* mail exchange */
#define DNS_RRTYPE_TXT            16    /* text strings */

/** DNS field CLASS used for "Resource Records" */
#define DNS_RRCLASS_IN            1     /* the Internet */
#define DNS_RRCLASS_CS            2     /* the CSNET class (Obsolete - used only for examples in some obsolete RFCs) */
#define DNS_RRCLASS_CH            3     /* the CHAOS class */
#define DNS_RRCLASS_HS            4     /* Hesiod [Dyer 87] */
#define DNS_RRCLASS_FLUSH         0x800 /* Flush bit */

// dnsentry state
#define DNS_RESOLVING 1
#define DNS_RESOLVED   2

#define DNS_RESEND 2*1000         // DNS请求包重发时间间隔，毫秒
#define DNS_TIMEOUT 3600*1000  // DNS生命期，毫秒
#define DNS_MAXRETRY 8

typedef struct dnsentry {	/* format of entry in ARP cache		*/
    struct dnsentry *next;
	char 	name[32];		/* Hardware address 			*/
	ip_addr_t	ip;	/* Protocol address 			*/
    U16 id;                   // id for dns node
	U8	state;		     /* state of this entry (see below)	*/
	U8	tries;	         /* number of retries so far 		*/
	U32 timeout;	// 超时时间
} dns_t;

int dns_init();

// brief: 设置dns server IP地址
// parm in: server[], server IP地址数组
//                 num: server num
// return 0, ok, <0, error
int dns_set_server(ip_addr_t *server, int num);

// brief: 域名解析
// parm in: server_name: 待解析的域名
// parm out: 解析成功后输出的IP地址
// return ==0, 成功
// return NE_DNS_RESOLVING：正在解析中
//             其他<0, error
int dns_resolve(const char *server_name, ip_addr_t *addr);

// dns处理函数，由miIP线程调用
void dns_process();

// 打印dns模块信息
void dns_dump();

#endif //_DNS_H_

