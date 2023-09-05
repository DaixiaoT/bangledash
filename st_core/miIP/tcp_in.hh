
static int tcp_listen_in(tcp_tcb_listen_t *pcb, packet_t *pk, ip_hd_t *iph, tcp_hd_t *tcph);
static int tcp_tw_in(tcp_tcb_t *pcb, packet_t *pk, ip_hd_t *iph, tcp_hd_t *tcph);
static int tcp_active_in(tcp_tcb_t *pcb, packet_t *pk, ip_hd_t *iph, tcp_hd_t *tcph);
static int tcp_receive(tcp_tcb_t *pcb, packet_t *pk, ip_hd_t *iph, tcp_hd_t *tcph);
static void tcp_parseopt(tcp_tcb_t *pcb, tcp_hd_t *tcph);

int tcp_in(packet_t *pk, ip_hd_t *iph)
{
    tcp_tcb_t *pcb, **pp;
    tcp_tcb_listen_t *lpcb;
    tcp_hd_t *tcph;

    // drop multicast/broadcast packets
    //mi_printf("tcp_in\r\n");

    // verify checksum
    //mi_printf("tcp_in: ip_dst=%08x src=%08x pk_len=%d\r\n", iph->ip_dst.addr, iph->ip_src.addr, pk->total_len);
    if(inet_chksum_pseudo(pk, &iph->ip_dst, &iph->ip_src, IPT_TCP, pk->total_len)) {
        mi_printf("tcp_in chksum error\r\n");
        packet_free(pk);
        return 0;
    }

    // Convert fields in TCP header to host byte order.
    tcph = (tcp_hd_t *)packet_data(pk);
    tcph->src = ntohs(tcph->src);
    tcph->dst = ntohs(tcph->dst);
    tcph->seqno = ntohl(tcph->seqno);
    tcph->ackno = ntohl(tcph->ackno);
    tcph->wnd = ntohs(tcph->wnd);

    packet_skip(pk, TCP_HLEN(tcph));
    //mi_printf("tcph(%08x): src=%04x dst=%04x seqno=%08x ackno=%08x wnd=%04x flags=%02x\r\n",
       // tcph, tcph->src, tcph->dst, tcph->seqno, tcph->ackno, tcph->wnd, tcph->flag);

    tcp_lock();

    // scan in active tcb list
  for(pcb = s_active_tcb, pp=&s_active_tcb; pcb !=0; pp = &pcb->next, pcb = pcb->next) {
    if (pcb->remote_port == tcph->src &&
       pcb->local_port == tcph->dst &&
       ip_addr_equ(&(pcb->remote_ip), &iph->ip_src) &&
       ip_addr_equ(&(pcb->local_ip), &iph->ip_dst)) {
            //mi_printf("active_in: dst_port=%d \r\n", tcph->dst);
            tcp_active_in(pcb, pk, iph, tcph);
            tcp_output(pcb);
            if(pcb->state==TCPS_CLOSED || pcb->state==TCPS_TIME_WAIT) {
                // remove from active list
                *pp = pcb->next;
                if(pcb->state==TCPS_TIME_WAIT) {
                    // add into timewait list
                    pcb->next = s_tw_tcb;
                    s_tw_tcb = pcb;
                }
                else
                if(pcb->state==TCPS_CLOSED && !(pcb->flags&TF_USER))
                      tcb_free(pcb);
            }
            goto end_l;
        }
    }

    // scan in time wait tcb list
  for(pcb = s_tw_tcb; pcb !=0; pcb = pcb->next) {
    if (pcb->remote_port == tcph->src &&
       pcb->local_port == tcph->dst &&
       ip_addr_equ(&(pcb->remote_ip), &iph->ip_src) &&
       ip_addr_equ(&(pcb->local_ip), &iph->ip_dst)) {
            tcp_tw_in(pcb, pk, iph, tcph);
            goto end_l;
        }
    }

//mi_printf("before lookup listen list\r\n");
  for(lpcb = s_listen_tcb; lpcb !=0; lpcb = lpcb->next) {
    //mi_printf("lpcb->port=%d inport=%d\r\n", lpcb->local_port, tcph->dst);
    if (lpcb->local_port == tcph->dst/* && ip_addr_equ(&(lpcb->local_ip), &iph->ip_dst)*/) {
            tcp_listen_in(lpcb, pk, iph, tcph);
            goto end_l;
        }
    }

end_l:
    tcp_unlock();
    packet_free(pk);
    //mi_printf("tcp_in exit\r\n");
    return 0;
}

static int tcp_listen_in(tcp_tcb_listen_t *pcb, packet_t *pk, ip_hd_t *iph, tcp_hd_t *tcph)
{
    //mi_printf("tcp_listen_in\r\n");
    U8 flags = tcph->flag;
    if(flags & TCP_RST)
        return 0;
    if (flags & TCP_ACK) {
        tcp_rst(tcph->ackno, tcph->seqno, &iph->ip_dst, &iph->ip_src, tcph->dst, tcph->src);
    } else if (flags & TCP_SYN) {
        // allocate a tcb, init it, put into active list
        tcp_tcb_t *npcb = (tcp_tcb_t *)tcb_new(0, 0);
        if(!npcb)
            return OE_MEMOUT;
        npcb->local_ip = iph->ip_dst;
        npcb->local_port = pcb->local_port;
        npcb->remote_ip = iph->ip_src;
        npcb->remote_port = tcph->src;
        npcb->state = TCPS_SYN_RCVD;
        npcb->life = 0;
        npcb->rcv_nxt = tcph->seqno + 1;
        npcb->rcv_ann_right_edge = npcb->rcv_nxt + npcb->in_buf.buf_size;
        npcb->snd_wnd = tcph->wnd;
        npcb->snd_wnd_max = tcph->wnd;
        npcb->snd_wl1 = tcph->seqno - 1; /* initialise to seqno - 1 to force window update */
        npcb->ssthresh = npcb->snd_wnd;
        //npcb->flags |= TF_SERVER;
        //npcb->mss = tcp_eff_send_mss(npcb->mss, &(npcb->remote_ip));
        npcb->listen_pcb = pcb;
        tcp_parseopt(npcb, tcph);
        npcb->next = s_active_tcb;
        s_active_tcb = npcb;
        tcp_send_syn_ack(npcb);
        //mi_printf("         send syn+ack, stat to SYN_RCVD\r\n");
    }
    return 0;
}

static int tcp_tw_in(tcp_tcb_t *pcb, packet_t *pk, ip_hd_t *iph, tcp_hd_t *tcph)
{
    //mi_printf("tcp_tw_in\r\n");
    U8 flags = tcph->flag;
    if(flags & TCP_RST)
        return 0;
  if (flags & TCP_SYN) {
    /* If an incoming segment is not acceptable, an acknowledgment
       should be sent in reply */
    if (TCP_SEQ_BETWEEN(tcph->seqno, pcb->rcv_nxt, pcb->rcv_ann_right_edge)) {
      /* If the SYN is in the window it is an error, send a reset */
        tcp_rst(tcph->ackno, tcph->seqno, &iph->ip_dst, &iph->ip_src, tcph->dst, tcph->src);
        return 0;
    }
  } else if (flags & TCP_FIN) {
    /* - eighth, check the FIN bit: Remain in the TIME-WAIT state.
         Restart the 2 MSL time-wait timeout.*/
    pcb->life = 0;
  }

  U16 tcplen = pk->total_len + ((flags & (TCP_FIN | TCP_SYN)) ? 1 : 0);
  if ((tcplen > 0))  {
    /* Acknowledge data, FIN or out-of-window SYN */
    tcp_ack_now(pcb);
    return tcp_output(pcb);
  }
  return 0;
}

static int tcp_active_in(tcp_tcb_t *pcb, packet_t *pk, ip_hd_t *iph, tcp_hd_t *tcph)
{
    //mi_printf("tcp_active_in\r\n");
    U8 flags = tcph->flag;

  /* Process incoming RST segments. */
    if (flags & TCP_RST) {
        mi_printf("active in: rcv reset\r\n");
        if (pcb->state == TCPS_SYN_SENT) {
            if (tcph->ackno == pcb->snd_max) {
                pcb->state = TCPS_CLOSED;
                if(pcb->connect_ret)
                    *pcb->connect_ret = NE_TCP_RST;  // tell tcp_connect() failed.
            }
        } else {
            if (TCP_SEQ_BETWEEN(tcph->seqno, pcb->rcv_nxt,  pcb->rcv_ann_right_edge))
                pcb->state = TCPS_CLOSED;
        }
        mi_printf("pcb state=%d flags=%04x\r\n", pcb->state, pcb->flags);
        return 0;
    }

    if ((flags & TCP_SYN) && (pcb->state != TCPS_SYN_SENT && pcb->state != TCPS_SYN_RCVD)) {
        /* Cope with new connection attempt after remote end crashed */
        mi_printf("active in: rcv SYN in stat=%d\r\n", pcb->state);
        tcp_ack_now(pcb);  // !!!! ?
        return 0;
    }

    pcb->life_idle = 0;

    tcp_parseopt(pcb, tcph);

    switch (pcb->state) {
    case TCPS_SYN_SENT:
        //mi_printf("active in: syn_sent\r\n");
        if ((flags & TCP_ACK) && (flags & TCP_SYN) && tcph->ackno==pcb->snd_uak+1) {
            // pcb init
            pcb->rcv_nxt = tcph->seqno +1;
            pcb->rcv_ann_right_edge = pcb->rcv_nxt + pcb->in_buf.buf_size;
            pcb->snd_uak = tcph->ackno;
            pcb->snd_wnd = tcph->wnd;
            pcb->snd_wnd_max = tcph->wnd;
            pcb->snd_wl1 = tcph->seqno - 1; /* initialise to seqno - 1 to force window update */
            //pcb->mss = tcp_eff_send_mss(pcb->mss, &(pcb->remote_ip));
            pcb->ssthresh = pcb->mss * 10;
            pcb->cwnd = ((pcb->cwnd == 1) ? (pcb->mss * 2) : pcb->mss);

            pcb->state = TCPS_ESTABLISHED;
            pcb->life = 0;
            if(pcb->connect_ret)
                *pcb->connect_ret = 1;  // tell tcp_connect() ok.

            mi_printf("=======establised, seqno=%08x ackno=%08x lport=%d\r\n", tcph->seqno, tcph->ackno, pcb->local_port);
            // send ack
            //tcp_ack_now(pcb);
            tcp_send_empty_ack(pcb);    // 需要启动重传; ACK包可能会丢失
            //mi_printf("SYN_SENT -> ESTABLISHED, send ack\r\n");
        }
        else if (flags & TCP_ACK) {
            tcp_rst(tcph->ackno, tcph->seqno, &iph->ip_dst, &iph->ip_src, tcph->dst, tcph->src);
        }
        break;
  case TCPS_SYN_RCVD:
        //mi_printf("active in: syn_rcvd\r\n");
        if (flags & TCP_ACK && !(flags & TCP_SYN)) {
            /* expected ACK number? */
            if (TCP_SEQ_BETWEEN(tcph->ackno, pcb->snd_uak+1, pcb->snd_max)) {
                //mi_printf("SYN_RCVD -> ESTABLISHED\r\n");
                pcb->state = TCPS_ESTABLISHED;
                pcb->life  = 0;
                tcp_receive(pcb, pk, iph, tcph);
                pcb->cwnd = ((pcb->cwnd == 1) ? (pcb->mss * 2) : pcb->mss);

                if (pcb->flags & TF_RX_FIN) {
                    tcp_ack_now(pcb);
                    pcb->state = TCPS_CLOSE_WAIT;
                    pcb->life  = 0;
                }
            } else {
                /* incorrect ACK number, send RST */
                tcp_rst(tcph->ackno, tcph->seqno, &iph->ip_dst, &iph->ip_src, tcph->dst, tcph->src);
            }
        } else if ((flags & TCP_SYN) && (tcph->seqno == pcb->rcv_nxt - 1)) {
            /* Looks like another copy of the SYN - retransmit our SYN-ACK */
            tcp_send_syn_ack(pcb);
        }
        break;
  case TCPS_CLOSE_WAIT:
    /* FALLTHROUGH */
  case TCPS_ESTABLISHED:
        tcp_receive(pcb, pk, iph, tcph);
        if (pcb->flags & TF_RX_FIN) { /* passive close */
            tcp_ack_now(pcb);
            pcb->state = TCPS_CLOSE_WAIT;
            pcb->life  = 0;
            //mi_printf("ESTABLISHED -> CLOSE_WAIT\r\n");
        }
    break;
  case TCPS_FIN_WAIT_1:
        //mi_printf("fin_wait1 %d flags=%02x len=%d seqno=%08x ackno=%08x\r\n",
           // pcb->local_port, flags, tcph->len, tcph->seqno, tcph->ackno);
        tcp_receive(pcb, pk, iph, tcph);
        if (pcb->flags & TF_RX_FIN) {
            if ((flags & TCP_ACK) && (tcph->ackno == pcb->snd_max)) {
                tcp_ack_now(pcb);
                //tcp_pcb_purge(pcb);
                pcb->state = TCPS_TIME_WAIT;
                pcb->life  = 0;
                //mi_printf("FIN_WAIT_1 -> TIME_WAIT\r\n");
            } else {
                tcp_ack_now(pcb);
                pcb->state = TCPS_CLOSING;
                pcb->life  = 0;
                //mi_printf("FIN_WAIT_1 -> CLOSING\r\n");
            }
        } else if ((flags & TCP_ACK) && (tcph->ackno == pcb->snd_max)) {
            pcb->state = TCPS_FIN_WAIT_2;
            pcb->life  = 0;
            //mi_printf("FIN_WAIT_1 -> FIN_WAIT_2\r\n");
        }
        break;
  case TCPS_FIN_WAIT_2:
        //mi_printf("fin_wait2 %d flags=%02x len=%d seqno=%08x ackno=%08x\r\n",
           // pcb->local_port, flags, tcph->len, tcph->seqno, tcph->ackno);
        tcp_receive(pcb, pk, iph, tcph);
        if (pcb->flags & TF_RX_FIN) {
            tcp_ack_now(pcb);
            //tcp_pcb_purge(pcb);
            pcb->state = TCPS_TIME_WAIT;
            pcb->life  = 0;
            //mi_printf("FIN_WAIT_1 -> TIME_WAIT\r\n");
        }
        break;
  case TCPS_CLOSING:
        //mi_printf("tcp_closing %d flags=%02x len=%d seqno=%08x ackno=%08x\r\n",
           // pcb->local_port, flags, tcph->len, tcph->seqno, tcph->ackno);
        tcp_receive(pcb, pk, iph, tcph);
        if (flags & TCP_ACK && tcph->ackno == pcb->snd_max) {
            //tcp_pcb_purge(pcb);
            pcb->state = TCPS_TIME_WAIT;
            pcb->life  = 0;
            //mi_printf("CLOSING -> TIME_WAIT\r\n");
        }
        break;
  case TCPS_LAST_ACK:
        tcp_receive(pcb, pk, iph, tcph);
        if (flags & TCP_ACK && tcph->ackno == pcb->snd_max) {
            /* bugfix #21699: don't set pcb->state to CLOSED here or we risk leaking segments */
            pcb->state = TCPS_CLOSED;  // !!!! ?
            //mi_printf("LAST_ACK -> CLOSED\r\n");
        }
        break;
    }

    return 0;
}

//
static int add_seg(tcp_tcb_t *pcb, U32 seq_l, U32 seq_r)
{
    tcp_seg_t *seg = pcb->segs;
    tcp_seg_t **pp = &pcb->segs;
    tcp_seg_t *seg2;

    while(seg) {
        if(TCP_SEQ_LT(seq_r, seg->L)) {
            // seq_r < seg->L, insert before seg
            seg2 = (tcp_seg_t *)mi_malloc(sizeof(tcp_seg_t));
            if(!seg2)
                return OE_MEMOUT;
            seg2->L = seq_l;
            seg2->R = seq_r;
            // insert before seg
            seg2->next = seg;
            *pp = seg2;
            tcp_seg_log(1);
            return 0;
        }
        else if(TCP_SEQ_GT(seq_l, seg->R)) {   // seq_l > seg->R, continue
            pp = &seg->next;
            seg = seg->next;
            continue;
        }

        // 2. merge seq with seg
        if(TCP_SEQ_LT(seq_l, seg->L))   // seq_l < seg->L, update seg->L
            seg->L = seq_l;
        if(TCP_SEQ_GT(seq_r, seg->R))   // seq_r > seg->R, update seg->R
            seg->R = seq_r;

        // if seq_r is intersected with the next segs, merge then all
        for(pp = &seg->next, seg2 = seg->next; seg2!=0; seg2=*pp) {
            if(TCP_SEQ_LT(seq_r, seg2->L))
                return 0;

            // merge seg and seg2
            if(TCP_SEQ_LT(seg->R, seg2->R))  // seg->R < seg2->R, update seg->R
                seg->R = seg2->R;

            // delete seg2
            *pp = seg2->next;
            mi_free(seg2);
            tcp_seg_log(-1);
        }

        return 0;
    }

    // add tail
    seg2 = (tcp_seg_t *)mi_malloc(sizeof(tcp_seg_t));
    if(!seg2)
        return OE_MEMOUT;
    seg2->L = seq_l;
    seg2->R = seq_r;
    seg2->next = 0;
    *pp = seg2;
    tcp_seg_log(1);
    return 0;
}

static void scan_segs(tcp_tcb_t *pcb)
{
    tcp_seg_t *seg, **pp;

    for(seg=pcb->segs, pp=&pcb->segs; seg!=0; seg=*pp) {
        if(TCP_SEQ_LT(pcb->rcv_nxt, seg->L)) // rcv_nxt < first seg.L, do nothing
            return;
        if(TCP_SEQ_LT(pcb->rcv_nxt, seg->R)) {  // rcv_nxt < seg->R, in_buf.num += seg->R - rcv_nxt
            ring_buf_fix_num(&pcb->in_buf, (int)(seg->R - pcb->rcv_nxt));
            pcb->rcv_nxt = seg->R;
            *pp = seg->next;
            mi_free(seg);
            tcp_seg_log(-1);
            return;
        }
        *pp = seg->next;
        mi_free(seg);
        tcp_seg_log(-1);
    }
}

static int tcp_receive(tcp_tcb_t *pcb, packet_t *pk, ip_hd_t *iph, tcp_hd_t *tcph)
{
    U8 flags = tcph->flag;
    if(flags & TCP_ACK) {
        U32 right_wnd_edge = pcb->snd_wnd + pcb->snd_wl2;

        //mi_printf("process ack: ackno=%08x snd_uak=%08x snd_nxt=%08x\r\n", tcph->ackno, pcb->snd_uak, pcb->snd_nxt);

        /* Update window. */
        if (TCP_SEQ_LT(pcb->snd_wl1, tcph->seqno) ||
            (pcb->snd_wl1 == tcph->seqno && TCP_SEQ_LT(pcb->snd_wl2, tcph->ackno)) ||
            (pcb->snd_wl2 == tcph->ackno && tcph->wnd > pcb->snd_wnd))
        {
            pcb->snd_wnd = tcph->wnd;
            /* keep track of the biggest window announced by the remote host to calculate
            the maximum segment size */
            if (pcb->snd_wnd_max < tcph->wnd) {
                pcb->snd_wnd_max = tcph->wnd;
            }
            pcb->snd_wl1 = tcph->seqno;
            pcb->snd_wl2 = tcph->ackno;
            if (pcb->snd_wnd == 0) {
                if (pcb->persist_backoff == 0) {
                    /* start persist timer */
                    mi_printf("+++++++++++++++++++start persist timer\r\n");
                    pcb->persist_cnt = 0;
                    pcb->persist_backoff = 1;
                }
            } else if (pcb->persist_backoff > 0) {
                /* stop persist timer */
                pcb->persist_backoff = 0;
            }
        }

        /* (From Stevens TCP/IP Illustrated Vol II, p970.) Its only a
         * duplicate ack if:
         * 1) It doesn't ACK new data
         * 2) length of received packet is zero (i.e. no payload)
         * 3) the advertised window hasn't changed
         * 4) There is outstanding unacknowledged data (retransmission timer running)
         * 5) The ACK is == biggest ACK sequence number so far seen (snd_una)
         *
         * If it passes all five, should process as a dupack:
         * a) dupacks < 3: do nothing
         * b) dupacks == 3: fast retransmit
         * c) dupacks > 3: increase cwnd */
        if (TCP_SEQ_LEQ(tcph->ackno, pcb->snd_uak)) {  // Clause 1
            U16 tcplen = pk->total_len + ((flags & (TCP_FIN | TCP_SYN)) ? 1 : 0);
            if (tcplen == 0 && // Clause 2
                pcb->snd_wl2 + pcb->snd_wnd == right_wnd_edge && // Clause 3
                pcb->rtime >= 0 && // Clause 4
                pcb->snd_uak == tcph->ackno)  // Clause 5
            {
                    if(pcb->dupacks < 4)
                        pcb->dupacks++;
                    if (pcb->dupacks == 3)
                        tcp_rexmit_fast(pcb);
                    else if (pcb->dupacks > 3) {
                        /* Inflate the congestion window, but not if it means that
                        the value overflows. */
                        if ((U16)(pcb->cwnd + pcb->mss) > pcb->cwnd)
                            pcb->cwnd += pcb->mss;
                    }
             }
             else {
                pcb->dupacks = 0;  // clean dupacks
                //mi_printf("dup ack: tcplen=%d rtime=%d snd_uak=%08x ackno=%08x wl2=%d snd_wnd=%d redge=%d\r\n",
                   // tcplen, pcb->rtime, pcb->snd_uak, tcph->ackno, pcb->snd_wl2, pcb->snd_wnd, right_wnd_edge);
            }
        } else if (TCP_SEQ_BETWEEN(tcph->ackno, pcb->snd_uak+1, pcb->snd_max)) {
            /* Reset the "IN Fast Retransmit" flag, since we are no longer
            in fast retransmit. Also reset the congestion window to the
            slow start threshold. */
            if (pcb->flags & TF_INFR) {
                pcb->flags &= ~TF_INFR;
                pcb->cwnd = pcb->ssthresh;
            }

            /* Reset the number of retransmissions. */
            pcb->nrtx = 0;
            /* Reset the retransmission time-out. */
            pcb->rto = (pcb->sa >> 3) + pcb->sv;

            /* Reset the fast retransmit variables. */
            pcb->dupacks = 0;

            /* Update the congestion control variables (cwnd and ssthresh). */
            if (pcb->state >= TCPS_ESTABLISHED) {
                if (pcb->cwnd < pcb->ssthresh) {
                    if ((U16)(pcb->cwnd + pcb->mss) > pcb->cwnd)
                        pcb->cwnd += pcb->mss;
                } else {
                    U16 new_cwnd = (pcb->cwnd + pcb->mss * pcb->mss / pcb->cwnd);
                    if (new_cwnd > pcb->cwnd)
                        pcb->cwnd = new_cwnd;
                }
            }

            // new valid ack arrived, process it
            int acked = (int)(tcph->ackno - pcb->snd_uak);
            //mi_printf("acked = %d\r\n", acked);
            pcb->snd_uak = tcph->ackno;
            ring_buf_skip(&pcb->out_buf, acked);

            pcb->rtime = (tcph->ackno==pcb->snd_max) ? -1 : 0;  // reset retransmit timer

            /* RTT estimation calculations. This is done by checking if the
            incoming segment acknowledges the segment we use to take a
            round-trip time measurement. */
            if (pcb->rttest && TCP_SEQ_LT(pcb->rtseq, tcph->ackno)) {
                /* diff between this shouldn't exceed 32K since this are tcp timer ticks
                and a round-trip shouldn't be that long... */
                S16 m = (S16)(tcp_ticks - pcb->rttest);
                //mi_printf("m=%d\r\n", m);
                /* This is taken directly from VJs original code in his paper */
                m = m - (pcb->sa >> 3);
                pcb->sa += m;
                if (m < 0)
                    m = -m;

                m = m - (pcb->sv >> 2);
                pcb->sv += m;
                pcb->rto = (pcb->sa >> 3) + pcb->sv;

                //mi_printf("~~~~~~~~~~~~~~~~~~~~~~rttest end. sa=%d sv=%d rto=%d\r\n", pcb->sa, pcb->sv, pcb->rto);
                pcb->rttest = 0;
            }
        }
    }

    if(flags & TCP_FIN) {
        U32 fin_seq = tcph->seqno + pk->total_len;
        //mi_printf("tcph->seqno=%08x total_len=%d data_len=%d\r\n", tcph->seqno, pk->total_len, pk->data_len);
        //mi_printf("got fin packet, fin_seq=%08x, rcv_nxt=%08x, rcv_ann_right_edge=%08x\r\n", fin_seq, pcb->rcv_nxt, pcb->rcv_ann_right_edge);
        //mi_printf("%d %d %d %d\r\n", (int)(fin_seq - pcb->rcv_nxt), (int)(fin_seq - pcb->rcv_ann_right_edge), TCP_SEQ_LT(fin_seq, pcb->rcv_ann_right_edge),
           //         TCP_SEQ_BETWEEN(fin_seq, pcb->rcv_nxt,  pcb->rcv_ann_right_edge));

        if(TCP_SEQ_BETWEEN(fin_seq, pcb->rcv_nxt,  pcb->rcv_ann_right_edge)) { // fin_seq is in recv window, mark it
            pcb->flags |= TF_FIN_SEQ;
            pcb->fin_seq = fin_seq;
            //mi_printf("pcb set TF_FIN_SEQ\r\n");
        }
    }

    // 如果seqno<pcb->rcv_nxt, 但此包有部分数据是新的，也要接收
    //mi_printf("recv_nxt=%08x seqno=%08x pk_len=%d flags=%02x\r\n", pcb->rcv_nxt, tcph->seqno, pk->total_len, flags);
    if(TCP_SEQ_BETWEEN(pcb->rcv_nxt, tcph->seqno, tcph->seqno + pk->total_len)) {
        // 连续数据
        int len = ring_buf_put_from_packet(&pcb->in_buf, pk, (int)(pcb->rcv_nxt - tcph->seqno));
        if(len > 0) {
            pcb->rcv_nxt += len;
            // send ack?
            scan_segs(pcb);
        }
        tcp_ack(pcb);           // 延时ACK
    }
    else {
        ring_buf_t *rb = &pcb->in_buf;
        int pos = (int)(tcph->seqno - pcb->rcv_nxt);

        if(pos>0 && rb->num + pos < rb->buf_size) {
            // ooseq packet, peek put
            int len = ring_buf_put_seg_from_packet(rb, pos, pk);
            if(len > 0)
                add_seg(pcb, tcph->seqno, tcph->seqno+len);
        }
        //mi_printf("*************************tcp_receive ack_now\r\n");
        tcp_ack_now(pcb);
    }

    if((pcb->flags & TF_FIN_SEQ) && TCP_SEQ_GEQ(pcb->rcv_nxt+1, pcb->fin_seq)) {
        pcb->flags |= TF_RX_FIN;
        pcb->rcv_nxt++;
        //mi_printf("pcb set TF_RX_FIN\r\n");
    }
    return 0;
}

static void tcp_parseopt(tcp_tcb_t *pcb, tcp_hd_t *tcph)
{
  U16 c, max_c;
  U16 mss;
  U8 *opts, opt;
#if LWIP_TCP_TIMESTAMPS
  U32 tsval;
#endif

  opts = (U8 *)tcph + TCP_MHLEN;

  /* Parse the TCP MSS option, if present. */
  if(TCP_HLEN(tcph) > 0x5) {
    max_c = (TCP_HLEN(tcph) - 5) << 2;
    for (c = 0; c < max_c; ) {
      opt = opts[c];
      switch (opt) {
      case 0x00:
        /* End of options. */
        //LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: EOL\r\n"));
        return;
      case 0x01:
        /* NOP option. */
        ++c;
        //LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: NOP\r\n"));
        break;
      case 0x02:
        //LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: MSS\r\n"));
        if (opts[c + 1] != 0x04 || c + 0x04 > max_c) {
          /* Bad length */
          //LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: bad length\r\n"));
          return;
        }
        /* An MSS option with the right option length. */
        mss = (opts[c + 2] << 8) | opts[c + 3];
        //mi_printf("**********mss=%d*************\r\n", mss);
        /* Limit the mss to the configured TCP_MSS and prevent division by zero */
        pcb->mss = ((mss > TCP_MSS) || (mss == 0)) ? TCP_MSS : mss;
        /* Advance to next option */
        c += 0x04;
        break;
#if LWIP_TCP_TIMESTAMPS
      case 0x08:
        LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: TS\r\n"));
        if (opts[c + 1] != 0x0A || c + 0x0A > max_c) {
          /* Bad length */
          LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: bad length\r\n"));
          return;
        }
        /* TCP timestamp option with valid length */
        tsval = (opts[c+2]) | (opts[c+3] << 8) |
          (opts[c+4] << 16) | (opts[c+5] << 24);
        if (flags & TCP_SYN) {
          pcb->ts_recent = ntohl(tsval);
          pcb->flags |= TF_TIMESTAMP;
        } else if (TCP_SEQ_BETWEEN(pcb->ts_lastacksent, tcph->seqno, tcph->seqno+tcplen)) {
          pcb->ts_recent = ntohl(tsval);
        }
        /* Advance to next option */
        c += 0x0A;
        break;
#endif
      default:
        //LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: other\r\n"));
        if (opts[c + 1] == 0) {
          //LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_parseopt: bad length\r\n"));
          /* If the length field is zero, the options are malformed
             and we don't process them further. */
          return;
        }
        /* All other options have a length field, so that we easily
           can skip past them. */
        c += opts[c + 1];
      }
    }
  }
}
