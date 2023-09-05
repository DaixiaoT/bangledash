#ifndef  _NETIF_H_
#define _NETIF_H_

/** must be the maximum of all used hardware address lengths
    across all types of interfaces in use */
//#define NETIF_MAX_HWADDR_LEN 6U

/** Whether the network interface is 'up'. This is
 * a software flag used to control whether this network
 * interface is enabled and processes traffic.
 * It is set by the startup code (for static IP configuration) or
 * by dhcp/autoip when an address has been assigned.
 */
#define NETIF_FLAG_UP           0x01U
/** If set, the netif has broadcast capability.
 * Set by the netif driver in its init function. */
#define NETIF_FLAG_BROADCAST    0x02U
/** If set, the netif is one end of a point-to-point connection.
 * Set by the netif driver in its init function. */
#define NETIF_FLAG_POINTTOPOINT 0x04U
/** If set, the interface is configured using DHCP.
 * Set by the DHCP code when starting or stopping DHCP. */
#define NETIF_FLAG_DHCP         0x08U
/** If set, the interface has an active link
 *  (set by the network interface driver).
 * Either set by the netif driver in its init function (if the link
 * is up at that time) or at a later point once the link comes up
 * (if link detection is supported by the hardware). */
#define NETIF_FLAG_LINK_UP      0x10U
/** If set, the netif is an ethernet device using ARP.
 * Set by the netif driver in its init function.
 * Used to check input packet types and use of DHCP. */
#define NETIF_FLAG_ETHARP       0x20U
/** If set, the netif is an ethernet device. It might not use
 * ARP or TCP/IP if it is used for PPPoE only.
 */
#define NETIF_FLAG_ETHERNET     0x40U
/** If set, the netif has IGMP capability.
 * Set by the netif driver in its init function. */
#define NETIF_FLAG_IGMP         0x80U

struct netif;
typedef int (*netif_output_fn)(struct netif *netif, packet_t *pk, ip_addr_t *next_hop);
typedef int (*netif_linkoutput_fn)(struct netif *netif, packet_t *pk);
typedef void (*netif_status_callback_fn)(struct netif *netif);

struct dhcp;
typedef struct netif {
    sema_t lock;
    struct netif *next;
    int channel;

    // IP address configuration in network byte order
    ip_addr_t ip_addr;
    ip_addr_t netmask;
    ip_addr_t gw;

    /** This function is called by the IP module when it wants
    *  to send a packet on the interface. This function typically
    *  first resolves the hardware address, then sends the packet. */
    netif_output_fn output;
    /** This function is called by the ARP module when it wants
    *  to send a packet on the interface. This function outputs
    *  the pbuf as-is on the link medium. */
    netif_linkoutput_fn linkoutput;

    netif_status_callback_fn status_callback;
    //netif_status_callback_fn link_callback;

    /** the DHCP client state information for this netif */
    struct dhcp *dhcp;

    /** maximum transfer unit (in bytes) */
    U16 mtu;
    /** flags (see NETIF_FLAG_ above) */
    U8 flags;
    /** number of bytes used in hwaddr */
    U8 hwaddr_len;
    /** link level hardware address of this interface */
    U8 hwaddr[NETIF_MAX_HWADDR_LEN];
    /** descriptive abbreviation */
    char name[2];

    char*  hostname;
    void *state;
} netif_t;

extern sema_t netif_sema;  // netif list lock
extern netif_t *netif_list;       // netif list
extern netif_t *default_netif;  // the default netif

#define netif_list_lock()         p_sema(&netif_sema, -1)
#define netif_list_unlock()    v_sema(&netif_sema)

#define netif_lock(netif) p_sema(&(netif)->lock, -1)
#define netif_unlock(netif) v_sema(&(netif)->lock)

int netif_init();

// add a netif into miIP stack
static inline void netif_add(netif_t *netif)
{
    netif_list_lock();
    netif->next = netif_list;
    netif_list = netif;
    if(!default_netif)
        default_netif = netif;
    netif_list_unlock();
}

// set the default netif
static inline void netif_set_default(netif_t *netif)
{
    netif_lock(netif);
    default_netif = netif;
    netif_unlock(netif);
}

#define netif_is_up(netif)  (netif->flags & NETIF_FLAG_UP)
void locked_netif_set_up(netif_t *netif);
void locked_netif_set_down( netif_t *netif);

// set the state of netif to up. (working)
static inline void netif_set_up(netif_t *netif)
{
    netif_lock(netif);
    locked_netif_set_up(netif);
    netif_unlock(netif);
}

// set the state of netif to down (not working)
static inline void netif_set_down( netif_t *netif)
{
    netif_lock(netif);
    locked_netif_set_down(netif);
    netif_unlock(netif);
}

//void netif_set_link_up(netif_t *netif);
//void netif_set_link_down(netif_t *netif);

// get the ip/mask/gw of a netif, in netif locked state
static inline void locked_netif_get_addr(netif_t *netif, ip_addr_t *ip, ip_addr_t *netmask, ip_addr_t *gw)
{
    *ip = netif->ip_addr;
    *netmask = netif->netmask;
    *gw = netif->gw;
}

// set the ip/mask/gw of a netif, in netif locked state
static inline void locked_netif_set_addr(netif_t *netif, const ip_addr_t *ip, const ip_addr_t *netmask, const ip_addr_t *gw)
{
    mi_printf("netif[%c%c] ip=%08x %08x %08x\r\n", netif->name[0], netif->name[1], ip->addr, netmask->addr, gw->addr);
    netif->ip_addr = *ip;
    netif->netmask = *netmask;
    netif->gw = *gw;
}

// get the ip/mask/gw of a netif
// parm out: ip/netmask/gw: store the output ip/mask/gw
static inline void netif_get_addr(netif_t *netif, ip_addr_t *ip, ip_addr_t *netmask, ip_addr_t *gw)
{
    netif_lock(netif);
    locked_netif_get_addr(netif, ip, netmask, gw);
    netif_unlock(netif);
}

// set the ip/mask/gw of a netif
static inline void netif_set_addr(netif_t *netif, const ip_addr_t *ip, const ip_addr_t *netmask, const ip_addr_t *gw)
{
    netif_lock(netif);
    locked_netif_set_addr(netif, ip, netmask, gw);
    netif_unlock(netif);
}

#define NETIF_STATUS_CALLBACK(n) do{ if (n->status_callback) { (n->status_callback)(n); }}while(0)
static inline void netif_set_status_callback(netif_t *netif, netif_status_callback_fn status_callback)
{
    if (netif) {
        netif_lock(netif);
        netif->status_callback = status_callback;
        netif_unlock(netif);
    }
}

/*
//#define NETIF_LINK_CALLBACK(n) do{ if (n->link_callback) { (n->link_callback)(n); }}while(0)
static inline void netif_set_link_callback(netif_t *netif, netif_status_callback_fn link_callback)
{
    if (netif) {
        netif_lock(netif);
        netif->link_callback = link_callback;
        netif_unlock(netif);
    }
}
*/

#endif //_NETIF_H_
