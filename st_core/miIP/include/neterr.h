#ifndef _NETERR_H_
#define _NETERR_H_

#define NE_OK 0

#define NE_BASE           -20000


/* UDP ������� */
#define NE_UDP_TCB_FULL   (NE_BASE-1) // UDP TCB ��
#define NE_UDP_HANDLE     (NE_BASE-2) // UDP TCB �������
#define NE_UDP_MARK        (NE_BASE-3) // UDP TCB ״̬����
#define NE_UDP_DUP_PORT   (NE_BASE-4) // �ظ��Ķ˿�
#define NE_EMPTY_QUEUE    (NE_BASE-5) // ���п�
#define NE_MCAST_FULL     (NE_BASE-6)
#define NE_UDP_LEN        (NE_BASE-7)
#define NE_UDP_DST_IP     (NE_BASE-8)
#define NE_UDP_DST_PORT   (NE_BASE-9)
#define NE_UDP_NEW_PORT (NE_BASE-10)

/* IP ������� */
#define NE_IP_CKSUM       (NE_BASE-20) // ip checksum error
#define NE_IP_VER         (NE_BASE-21) // ip version error
#define NE_IP_ADDR        (NE_BASE-22)
#define NE_ARP_ENTRY_FULL (NE_BASE-23)
#define NE_PACKET_FULL    (NE_BASE-24) // �޷����������
#define NE_IP_RX_LEN      (NE_BASE-25)
#define NE_IP_ROUTE       (NE_BASE-26)
#define NE_IP_SRC_ADDR  (NE_BASE-27)

#define NE_RAW_PROTO_DUP  (NE_BASE-40) //
#define NE_RAW_TCB_FULL   (NE_BASE-41) //
#define NE_RAW_HANDLE     (NE_BASE-42) // RAW TCB �������
#define NE_RAW_STATE      (NE_BASE-43) // RAW TCB ״̬����

#define NE_BOOTP_NETMASK  (NE_BASE-50)
#define NE_DHCP_NAK       (NE_BASE-51)
#define NE_DHCP_DISCOVER  (NE_BASE-52)

// ����: >=0, �����ֽ���.
//       <0,   ������
#define NE_FFTP_RETRY_OUT     (NE_BASE-70)
#define NE_FFTP_LOAD_FILE     (NE_BASE-71)
#define NE_FFTP_BUF_TOO_SMALL (NE_BASE-72)  // ���ػ�����̫С
#define NE_FFTP_BUF_TOO_LARGE (NE_BASE-73)  // ���ػ�����̫С
#define NE_FFTP_OTHER         (NE_BASE-74)

#define NE_LAN_DRIVER_IDX     (NE_BASE-80)

#define NE_MCAST_DISABLED     (NE_BASE-90)

/* TCP ������� */
#define NE_TCP_TCB_FULL   (NE_BASE-101) /* TCP TCB �� 	    */
#define NE_TCP_HANDLE     (NE_BASE-102) /* TCP TCB ������� */
#define NE_TCP_STATE      (NE_BASE-103) /* TCP TCB ״̬���� */
#define NE_TCP_DUP_PORT   (NE_BASE-104) /* �ظ��Ķ˿�	    */
#define NE_TCP_DST_IP     (NE_BASE-105) /* Ŀ��IP����       */
#define NE_TCP_TIMEOUT (NE_BASE-106) /* TCP��ʱ */
#define NE_TCP_FIN              (NE_BASE-107)
#define NE_TCP_NEW_PORT (NE_BASE-108)
#define NE_TCP_MARK        (NE_BASE-109) // TCB ״̬����
#define NE_TCP_RST             (NE_BASE-110) // �Է�����reset��
#define NE_TCP_CLOSED    (NE_BASE-111)  // TCB�ѱ��ر�

#define NE_NIF_ETHARP  (NE_BASE-130)
#define NE_NIF_MTU         (NE_BASE-131)

// dns
#define NE_DNS_RESOLVING (NE_BASE-150)
#define NE_DNS_NO_SERVER (NE_BASE-151)

// phy
#define NE_EMAC_CLK       (NE_BASE-200)


#endif //_NETERR_H_
