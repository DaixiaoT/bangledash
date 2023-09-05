
#define TCP_OPT_LENGTH(flags)              \
  (flags & TCP_OPT_MSS ? 4  : 0) +          \
  (flags & TCP_OPT_TS  ? 12 : 0)

/** This returns a TCP header option for MSS in an U32 */
#define TCP_BUILD_MSS_OPTION(mss) htonl((0x02040000 | ((mss) & 0xFFFF)))

// 发送reset包
static int tcp_rst(U32 seqno, U32 ackno,
                                ip_addr_t *local_ip, ip_addr_t *remote_ip,
                                U16 local_port, U16 remote_port)
{
    //mi_printf("tcp_rst\r\n");
    packet_t *pk = packet_alloc(EP_HLEN_A4 + IPMHLEN + TCP_MHLEN);
    if (! pk){
        PRINT_ERR_STR("tcp_rst");
        return OE_MEMOUT;
    }

    pk->data_pos = EP_HLEN_A4 + IPMHLEN;
    pk->data_len = pk->total_len = TCP_MHLEN;

    tcp_hd_t *tcph = (tcp_hd_t *)packet_data(pk);

    tcph->src = htons(local_port);
    tcph->dst = htons(remote_port);
    tcph->seqno = htonl(seqno);
    tcph->ackno = htonl(ackno);
    tcph->len = TCP_MHLEN<<2;     // means  len/4 << 4
    tcph->flag = TCP_RST | TCP_ACK;
    tcph->wnd = 0;
    tcph->cksum = 0;
    tcph->urgp = 0;

    tcph->cksum = inet_chksum_pseudo(pk, local_ip, remote_ip, IPT_TCP, pk->total_len);

	ip_send_queue(pk, remote_ip, IPT_TCP, IPP_NORMAL, IP_TTL);
	return 0;
}


// comm send function
static int tcp_send_comm(tcp_tcb_t *pcb , U8 opt_flags, U8 flags, int len)
{
    int optlen = TCP_OPT_LENGTH(opt_flags);
    //mi_printf("tcp_send_comm len=%d\r\n", EP_HLEN_A4 + IPMHLEN + TCP_MHLEN + optlen);
    packet_t *pk = packet_alloc(EP_HLEN_A4 + IPMHLEN + TCP_MHLEN + optlen);
    if (! pk){
        PRINT_ERR_STR("tcp_send_comm");
        return OE_MEMOUT;
    }

    pk->data_pos = EP_HLEN_A4 + IPMHLEN + TCP_MHLEN;
    pk->data_len = pk->total_len = optlen;

    // fill tcp options
    U32 *opts = (U32 *)packet_data(pk);
    if(opt_flags & TCP_OPT_MSS) {
        *opts = TCP_BUILD_MSS_OPTION(pcb->mss);
        opts += 1;
    }
    /*if (opt_flags & TCP_OPT_TS) {
        opts[0] = htonl(0x0101080A);
        opts[1] = htonl(sys_time());
        opts[2] = htonl(pcb->ts_recent);
        opts += 3;
    }*/

    packet_skip(pk, -TCP_MHLEN);
    //mi_printf("pk(%08x)->pos=%d, data_len=%d, total_len=%d\r\n", pk, pk->data_pos, pk->data_len, pk->total_len);

    tcp_hd_t *tcph = (tcp_hd_t *)packet_data(pk);
    tcph->src = htons(pcb->local_port);
    tcph->dst = htons(pcb->remote_port);
    tcph->ackno = htonl(pcb->rcv_nxt);
    tcph->len = (TCP_MHLEN + optlen)<<2;     // means  len/4 << 4
    tcph->flag = flags & 0x3F;

    U32 fin_seq = pcb->snd_uak + pcb->out_buf.num;
    if(flags & TCP_SYN) {
        // syn包始终发送snd_uak
        tcph->seqno = htonl(pcb->snd_uak);
        pcb->snd_nxt = pcb->snd_uak+1;
        len = 0;  // force don't send data
    }
    else if(pcb->flags & TF_TX_FIN && TCP_SEQ_GEQ(pcb->snd_nxt, fin_seq)) {
        // 单独的fin包，如果是KEEP_ALIVE包，重发ooseq, 否则始终发送fin_tx_seq
        if(flags & TCP_KEEPALIVE) {
            tcph->seqno = htonl(fin_seq - 1);
        }
        else {
            tcph->seqno = htonl(fin_seq);
            pcb->snd_nxt = fin_seq+1;
            tcph->flag |= TCP_FIN; // if snd_nxt reach the fin seq, mark FIN.
            //mi_printf("tcp(%d) send_comm: FIN\r\n", pcb->local_port);
        }
        len = 0;  // force don't send data
    } else if(flags & TCP_KEEPALIVE) {
        tcph->seqno = htonl(pcb->snd_nxt-1);  // send ooseq to force the other end ack now.
        len = 0;  // force don't send data
    } else
        tcph->seqno = htonl(pcb->snd_nxt);    // 普通包从snd_nxt开始发送

    int wnd = (int)(pcb->rcv_ann_right_edge - pcb->rcv_nxt);
    //short wnd = ring_buf_free_space()
    tcph->wnd = htons((U16)wnd);
    tcph->cksum = 0;
    tcph->urgp = 0;

    if (!(flags&TCP_SYN)) {  // rttest==0, 并且不是SYN包, 启动往返时间估计
        if(pcb->rttest == 0) {
            pcb->rttest = tcp_ticks;
            pcb->rtseq = ntohl(tcph->seqno);
        }
        if(pcb->rtime<0)
            pcb->rtime = 0;
        //mi_printf("rttest start\r\n");
    }

    if(len > 0) {
        ring_buf_t *rb = &pcb->out_buf;
        int skip_num = (int)(pcb->snd_nxt - pcb->snd_uak);
        len = my_min(rb->num - skip_num, len);
        len = my_min(len, pcb->mss);
        if(len > 0) {
            int pos = rb->start + skip_num;
            if(pos >= rb->buf_size)
                pos -= rb->buf_size;
            int len0 = my_min(len, rb->buf_size - pos);
            //mi_printf("ref out_buf pos=%d len0=%d len1=%d\r\n", pos, len0, len-len0);
            packet_ref(pk, rb->buf+pos, len0, rb->buf, len - len0);
            pcb->snd_nxt += len;
            // // if snd_nxt reached the fin seq, send FIN flag.
            if(pcb->flags & TF_TX_FIN && TCP_SEQ_GEQ(pcb->snd_nxt, fin_seq)) {
                tcph->flag |= TCP_FIN;
                pcb->snd_nxt++;
            }
        }
    }

    //mi_printf("tcp_send_comm: ip_dst=%08x src=%08x pk_len=%d\r\n", pcb->local_ip.addr, pcb->remote_ip.addr, pk->total_len);
    tcph->cksum = inet_chksum_pseudo(pk, &(pcb->local_ip), &(pcb->remote_ip), IPT_TCP, pk->total_len);

    //mi_printf("tcp_send_comm dst=%d seqno=%08x ackno=%08x wnd=%04x flags=%02x data_len=%d mss=%d\r\n",
       // ntohs(tcph->dst), ntohl(tcph->seqno), ntohl(tcph->ackno), ntohs(tcph->wnd), tcph->flag, len, pcb->mss);
    ip_send_queue(pk, &pcb->remote_ip, IPT_TCP, IPP_NORMAL, IP_TTL);
/*#if 1
    //mi_printf("tcp_send_comm send ip packet\r\n");
    if(len>0) {
        //mi_printf("tcp_send_comm: ip_dst=%08x src=%08x pk_len=%d\r\n", pcb->local_ip.addr, pcb->remote_ip.addr, pk->total_len);
        //mi_printf("tcp_send_comm dst=%d seqno=%08x ackno=%08x wnd=%04x flags=%02x data_len=%d\r\n",
           //     ntohs(tcph->dst), ntohl(tcph->seqno), ntohl(tcph->ackno), ntohs(tcph->wnd), tcph->flag, len);
        pcb->snd_pk_count++;
        if ((pcb->snd_pk_count&1)==0) {  // rttest==0, 并且不是SYN包, 启动往返时间估计
            ip_send_queue(pk, &pcb->remote_ip, IPT_TCP, IPP_NORMAL, IP_TTL);
        }
        else {
            mi_printf("drop packet seqno=%08x\r\n", ntohl(tcph->seqno));
            packet_free(pk);
        }
    }
    else
            ip_send_queue(pk, &pcb->remote_ip, IPT_TCP, IPP_NORMAL, IP_TTL);
#else
        packet_free(pk);
#endif*/
    pcb->flags &=~(TF_ACK_NOW | TF_ACK_DELAY);  // clean delay ack flags

    if(TCP_SEQ_GT(pcb->snd_nxt, pcb->snd_max))
        pcb->snd_max = pcb->snd_nxt;

	return len;
}

#define tcp_send_syn_ack(pcb)                 tcp_send_comm(pcb, TCP_OPT_MSS, TCP_SYN | TCP_ACK, 0)  // 发送SYN+ACK
#define tcp_send_syn(pcb)                         tcp_send_comm(pcb, TCP_OPT_MSS, TCP_SYN, 0)                     // 发送SYN
#define tcp_send_empty_ack(pcb)            tcp_send_comm(pcb, 0, TCP_ACK, 0)                                             // 发送ACK，无数据
#define tcp_send_data(pcb, len)               tcp_send_comm(pcb, 0, TCP_ACK, len)                                          // 发送数据
#define tcp_zero_window_probe(pcb)     tcp_send_comm(pcb, 0, TCP_ACK, 1)                                             // 发送1字节数据
#define tcp_keepalive(pcb)                        tcp_send_comm(pcb, 0, TCP_ACK | TCP_KEEPALIVE, 0)  // 发送ooseq，对方收到ooseq会立即应答

// 尽量发送数据
static int tcp_output(tcp_tcb_t *pcb)
{
    if(pcb->state < TCPS_ESTABLISHED) // 此函数只用来发送数据+ACK；未建立连接的状态不用此函数发送
        return 0;

    //mi_printf("tcp_output\r\n");
    // 计算有ACK/FIN或数据需要发送才发送。否则返回。
    // flags有ACK/DELAY_ACK标志，表示需要发送ACK。
    int wnd = my_min(pcb->snd_wnd, pcb->cwnd);
    int unsend = my_min(wnd, pcb->out_buf.num);
    int len = (int)(pcb->snd_uak + unsend - pcb->snd_nxt);
    //mi_printf("tcp_output: wnd=%d unsend=%d send_len=%d pcb_flags=%04x mss=%d\r\n", wnd, unsend,  len, pcb->flags, pcb->mss);

    while(len>0/*=pcb->mss*/ || (pcb->flags & TF_ACK_NOW)) {
        int ret = tcp_send_data(pcb, len);
        //mi_printf("tcp_output: tcp_send_data, pcb_flags=%04x\r\n", pcb->flags);
        if(ret<=0)
            break;
        len -= ret;
    }

    return 0;
}

static inline void tcp_rexmit_rto(tcp_tcb_t *pcb)
{
    mi_printf("tcp_rexmit_rto\r\n");
    pcb->snd_nxt = pcb->snd_uak;
    ++pcb->nrtx;
    /* Don't take any rtt measurements after retransmitting. */
    pcb->rttest = 0;
    tcp_output(pcb);
}

static inline void tcp_rexmit_fast(tcp_tcb_t *pcb)
{
    mi_printf("tcp_rexmit_fast\r\n");
    if (TCP_SEQ_LT(pcb->snd_uak, pcb->snd_nxt) && !(pcb->flags & TF_INFR)) {
        pcb->snd_nxt = pcb->snd_uak;
        ++pcb->nrtx;
        /* Don't take any rtt measurements after retransmitting. */
        pcb->rttest = 0;

        /* Set ssthresh to half of the minimum of the current
        * cwnd and the advertised window */
        if (pcb->cwnd > pcb->snd_wnd)
            pcb->ssthresh = pcb->snd_wnd / 2;
        else
            pcb->ssthresh = pcb->cwnd / 2;

        /* The minimum value for ssthresh should be 2 MSS */
        if (pcb->ssthresh < 2*pcb->mss) {
            pcb->cwnd = pcb->ssthresh + 3 * pcb->mss;
            pcb->flags |= TF_INFR;
        }
    }
}
