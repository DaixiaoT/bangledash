
// LAN_MAC1:
#define MAC1_RESET          (1<<15)
#define MAC1_LOOPBACK       (1<<4)
#define MAC1_TX_FLOW_CTL    (1<<3)
#define MAC1_RX_FLOW_CTL    (1<<2)
#define MAC1_RECV_ALL_FRAME (1<<1)
#define MAC1_RECV_EN        (1<<0)

// LAN_MAC2:
#define MAC2_FULL_DUPLEX    (1<<0)
#define MAC2_FRAME_LENGTH_CHECKING (1<<1)
#define MAC2_PURE_PREAMBLE_ENFORCEMENT (1<<8)
#define MAC2_CRC_EN         0x00000010  /* Append CRC to every Frame         */
#define MAC2_PAD_EN         0x00000020  /* Pad all Short Frames              */

// LAN_MCMD
#define MCMD_MII_READ       (1<<0)

// PHY support
#define SMII_PHY            (1<<12)
#define PHY_SPEED100        (1<<8)

/* MII Management Configuration Register */
#define EMAC_MCFG_SCAN_INC       0x00000001  	/**< Scan Increment PHY Address        */
#define EMAC_MCFG_SUPP_PREAM     0x00000002  	/**< Suppress Preamble                 */
#define EMAC_MCFG_CLK_SEL(n)     ((n&0x0F)<<2)  /**< Clock Select Field                 */
#define EMAC_MCFG_RES_MII        0x00008000  	/**< Reset MII Management Hardware     */
#define EMAC_MCFG_MII_MAXCLK	 2500000UL		/**< MII Clock max */

/* Back-to-Back Inter-Packet-Gap Register */
#define IPGT_FULL_DUP       0x00000015  /* Recommended value for Full Duplex */
#define IPGT_HALF_DUP       0x00000012  /* Recommended value for Half Duplex */

/* Non Back-to-Back Inter-Packet-Gap Register */
#define IPGR_DEF            0x00000012  /* Recommended value                 */

/* Collision Window/Retry Register */
#define CLRT_DEF            0x0000370F  /* Default value                     */

/* PHY Support Register */
#define SUPP_SPEED_100      0x00000100  /* Reduced MII Logic Current Speed   */
//#define SUPP_RES_RMII       0x00000800  /* Reset Reduced MII Logic           */

// LAN_CMD
#define LAN_FULL_DUPLEX         (1<<10)
#define LAN_RMII                (1<<9)
#define LAN_RECV_ALL_FRAME      (1<<7)
#define LAN_RX_RESET            (1<<5)
#define LAN_TX_RESET            (1<<4)
#define LAN_REG_RESET           (1<<3)
#define LAN_TX_RT_EN            (1<<2)
#define LAN_TX_EN               (1<<1)
#define LAN_RX_EN               (1<<0)

// LAN_RX_FILTER_CTL
#define LAN_ACCEPT_PERFECT         (1<<5)
#define LAN_ACCEPT_MULTICAST_HASH  (1<<4)
#define LAN_ACCEPT_UNICAST_HASH    (1<<3)
#define LAN_ACCEPT_MULTICAST       (1<<2)
#define LAN_ACCEPT_BROADCAST       (1<<1)
#define LAN_ACCEPT_UNICAST         (1<<0)

/* Interrupt Status/Enable/Clear/Set Registers */
#define INT_RX_OVERRUN      0x00000001  /* Overrun Error in RX Queue         */
#define INT_RX_ERR          0x00000002  /* Receive Error                     */
#define INT_RX_FIN          0x00000004  /* RX Finished Process Descriptors   */
#define INT_RX_DONE         0x00000008  /* Receive Done                      */
#define INT_TX_UNDERRUN     0x00000010  /* Transmit Underrun                 */
#define INT_TX_ERR          0x00000020  /* Transmit Error                    */
#define INT_TX_FIN          0x00000040  /* TX Finished Process Descriptors   */
#define INT_TX_DONE         0x00000080  /* Transmit Done                     */
#define INT_SOFT_INT        0x00001000  /* Software Triggered Interrupt      */
#define INT_WAKEUP          0x00002000  /* Wakeup Event Interrupt            */

/* RX Descriptor Control Word */
#define RCTRL_SIZE          0x000007FF  /* Buffer size mask                  */
#define RCTRL_INT           0x80000000  /* Generate RxDone Interrupt         */

/* RX Status Hash CRC Word */
#define RHASH_SA            0x000001FF  /* Hash CRC for Source Address       */
#define RHASH_DA            0x001FF000  /* Hash CRC for Destination Address  */

/* RX Status Information Word */
#define RINFO_SIZE          0x000007FF  /* Data size in bytes                */
#define RINFO_CTRL_FRAME    0x00040000  /* Control Frame                     */
#define RINFO_VLAN          0x00080000  /* VLAN Frame                        */
#define RINFO_FAIL_FILT     0x00100000  /* RX Filter Failed                  */
#define RINFO_MCAST         0x00200000  /* Multicast Frame                   */
#define RINFO_BCAST         0x00400000  /* Broadcast Frame                   */
#define RINFO_CRC_ERR       0x00800000  /* CRC Error in Frame                */
#define RINFO_SYM_ERR       0x01000000  /* Symbol Error from PHY             */
#define RINFO_LEN_ERR       0x02000000  /* Length Error                      */
#define RINFO_RANGE_ERR     0x04000000  /* Range Error (exceeded max. size)  这个错误正常的. IP包类型0800, ARP 0806, 都超长了 */
#define RINFO_ALIGN_ERR     0x08000000  /* Alignment Error                   */
#define RINFO_OVERRUN       0x10000000  /* Receive overrun                   */
#define RINFO_NO_DESCR      0x20000000  /* No new Descriptor available       */
#define RINFO_LAST_FLAG     0x40000000  /* Last Fragment in Frame            */
#define RINFO_ERR           0x80000000  /* Error Occured (OR of all errors)  */

#define RINFO_ERR_MASK     (RINFO_FAIL_FILT | RINFO_CRC_ERR   | RINFO_SYM_ERR | \
                            RINFO_LEN_ERR   | RINFO_ALIGN_ERR | RINFO_OVERRUN)

/* TX Descriptor Control Word */
#define TCTRL_SIZE          0x000007FF  /* Size of data buffer in bytes      */
#define TCTRL_OVERRIDE      0x04000000  /* Override Default MAC Registers    */
#define TCTRL_HUGE          0x08000000  /* Enable Huge Frame                 */
#define TCTRL_PAD           0x10000000  /* Pad short Frames to 64 bytes      */
#define TCTRL_CRC           0x20000000  /* Append a hardware CRC to Frame    */
#define TCTRL_LAST          0x40000000  /* Last Descriptor for TX Frame      */
#define TCTRL_INT           0x80000000  /* Generate TxDone Interrupt         */

/* TX Status Information Word */
#define TINFO_COL_CNT       0x01E00000  /* Collision Count                   */
#define TINFO_DEFER         0x02000000  /* Packet Deferred (not an error)    */
#define TINFO_EXCESS_DEF    0x04000000  /* Excessive Deferral                */
#define TINFO_EXCESS_COL    0x08000000  /* Excessive Collision               */
#define TINFO_LATE_COL      0x10000000  /* Late Collision Occured            */
#define TINFO_UNDERRUN      0x20000000  /* Transmit Underrun                 */
#define TINFO_NO_DESCR      0x40000000  /* No new Descriptor available       */
#define TINFO_ERR           0x80000000  /* Error Occured (OR of all errors)  */

#define ETH_MAX_FLEN        1536        /* Max. Ethernet Frame Size          */
