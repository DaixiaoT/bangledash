#ifndef  _TCP_TCB_H_
#define _TCP_TCB_H_

/* 序列号比较 */
#define TCP_SEQ_LT(a, b)	((int)((a) - (b))<0)
#define TCP_SEQ_LEQ(a, b)	((int)((a) - (b))<=0)
#define TCP_SEQ_GT(a, b)	((int)((a) - (b))>0)
#define TCP_SEQ_GEQ(a, b)	((int)((a) - (b))>=0)
#define TCP_SEQ_BETWEEN(a,b,c) (TCP_SEQ_GEQ(a,b) && TCP_SEQ_LEQ(a,c))

/* TCP state */
#define TCPS_NUM		            11
#define TCPS_CLOSED	             0
//#define TCPS_LISTEN	             1
#define TCPS_SYN_SENT	         2
#define TCPS_SYN_RCVD       	 3
#define TCPS_ESTABLISHED	     4
#define TCPS_CLOSE_WAIT	     5
#define TCPS_FIN_WAIT_1	         6
#define TCPS_CLOSING	             7
#define TCPS_FIN_WAIT_2	         8
#define TCPS_LAST_ACK	         9
#define TCPS_TIME_WAIT	         10

#define TCP_LISTEN_MARK 0xb0ef7c2a

typedef struct tcp_tcb_listen {
    U32 mark;
    struct tcp_tcb_listen *next;

	U16 local_port;
	//U16 remote_port;
	ip_addr_t local_ip;
	//ip_addr_t remote_ip;

} tcp_tcb_listen_t;

typedef struct tcp_seg {
    struct tcp_seg *next;
    U32 L, R;
} tcp_seg_t;

#define TCP_MARK 0xe1f923d6

typedef struct tcp_tcb {
    U32 mark;
    struct tcp_tcb *next;
    //struct tcp_tcb *alloc_next;  // just for test mem leak

	U16 local_port;
	U16 remote_port;
	ip_addr_t local_ip;
	ip_addr_t remote_ip;

    /* receiver variables */
    U32 rcv_nxt;   /* next seqno expected */
    U32 rcv_ann_right_edge; /* announced right edge of window */

    /* RTT (round trip time) estimation variables */
    U32 rttest; /* RTT estimate in 500ms ticks */
    U32 rtseq;  /* sequence number being timed */
    S16 sa, sv; /* @todo document this */

    /* sender variables */
    U32 snd_uak;  /* Highest acknowledged seqno. */
    U32 snd_nxt;   /* next seqno to be sent */
    U32 snd_max;  /* the max seqno+1 that has been sent */
    U32 snd_wl1, snd_wl2; /* Sequence and acknowledgement numbers of last
                             window update. */
    U16 snd_wnd;   /* sender window */
    U16 snd_wnd_max; /* the maximum sender window announced by the remote host */

    U32 fin_seq;     // the remote last seq
    /* congestion avoidance/control variables */
    U16 cwnd;
    U16 ssthresh;

    U16 mss;   /* maximum segment size */

    U16 flags;
#define TF_ACK_DELAY   ((U16)0x01U)   /* Delayed ACK. */
#define TF_ACK_NOW     ((U16)0x02U)   /* Immediate ACK. */
#define TF_INFR        ((U16)0x04U)   /* In fast recovery. */
#define TF_TIMESTAMP   ((U16)0x08U)   /* Timestamp option enabled */
#define TF_RX_FIN          ((U16)0x10U)   /* rx closed by tcp_shutdown */
#define TF_TX_FIN         ((U16)0x20U)   /* Connection was closed locally (FIN segment enqueued). */
#define TF_NODELAY     ((U16)0x40U)   /* Disable Nagle algorithm */
#define TF_NAGLEMEMERR ((U16)0x80U)   /* nagle enabled, memerr, try to output to prevent delayed ACK to happen */
#define TF_USER             ((U16)0x100U)   /* this pcb is used by app */
#define TF_FIN_SEQ          ((U16)0x400U)   /* received a fin packet, set fin_seq */
//#define TF_SERVER           ((U16)0x800U)   /* this tcb is created from tcp_listen(), just used for debug */

	U8   state;		/* tcp state */

    /* fast retransmit/recovery */
    U8 dupacks;

    /* Persist timer counter */
    U8 persist_cnt;
    /* Persist timer back-off */
    U8 persist_backoff;

    /* Retransmission timer. */
    U8 nrtx;    /* number of retransmissions */
    S16 rtime;
    S16 rto;    /* retransmission time-out */

    ring_buf_t in_buf;        // in buffer
    ring_buf_t out_buf;     // out buffer

    tcp_seg_t *segs;

    tcp_tcb_listen_t *listen_pcb;  // 如果listen_pcb!=0, 表示此pcb还未被tcp_accept()调用。
    volatile int *connect_ret;
    int life, life_idle;
    U32 delay_ack_time;

    //U32 snd_pk_count;
} tcp_tcb_t;

#define TCP_OPT_MSS     (U8)0x01U /* Include MSS option. */
#define TCP_OPT_TS        (U8)0x02U /* Include timestamp option. */

#endif //_TCP_TCB_H_

