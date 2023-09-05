#ifndef _TCP_H_
#define _TCP_H_

#define TCP_MAXRTX  12
#define TCP_MSS         1400
#define TCP_MSL          20  // 10秒

typedef struct tcp_header {
	U16	src;	 // src port
	U16	dst;	 // dest port
	U32	seqno;		// sequence no，发送序列号
	U32	ackno;	   // ack no,  确认序列号

	U8	len;		/* TCP报头长度: 使用高4位 (& 0xF0) */
	U8	flag;		/* control flags: 使用低6位 (& 0x3F) */

	U16	wnd;	// recv window size
	U16	cksum;   // check sum
	U16	urgp;	   // urgent poinger
}tcp_hd_t;

/* TCP Control Bits */
#define TCP_URG	0x20    /* urgent pointer is valid */
#define TCP_ACK	0x10    /* acknowledgment field is valid */
#define TCP_PSH	0x08    /* this segment requests a push */
#define TCP_RST	0x04    /* reset the connection */
#define TCP_SYN	0x02    /* synchronize sequence numbers */
#define TCP_FIN	0x01    /* sender is finished sending */

#define TCP_KEEPALIVE 0x40  // local used

/* minimum TCP header length */
#define TCP_MHLEN	20

/* TCP header length */
#define TCP_HLEN(tcph)	(((tcph)->len & 0xf0) >> 2)

void tcp_init(void);

// brief: udp输入包处理
// parm in: pk, 输入包; pip: 此包的ip协议头
int tcp_in(packet_t *pk, ip_hd_t *pip);

// tcp 协议处理函数，由miIP线程调用
void tcp_process ();

// 打印tcp模块信息
void tcp_dump();

// brief: open a client tcp handle
// parm in:  in_buf_size: tcp input buffer size in bytes, 0=default(4096 bytes)
//                 out_buf_size: tcp output buffer size in bytes, 0=default(4096 bytes)
// parm out: handle: pointer to store the client tcp handle if success
// return =0, success.
//             <0, error
int tcp_open (U32 *handle, int in_buf_size, int out_buf_size);

// brief: close a client tcp handle
// return =0, ok. <0, error
int tcp_close(U32 handle);

// brief: connect a tcp client handle to remote ip:port
// return =0, ok. <0, error
// if ret==0, this tcp handle is in connecting state, use wait_flag(connect_ret) to wait, if connect_ret==1, tcp is connected. else error.
int tcp_connect_async(U32 handle, ip_addr_t *remote_ip, U16 remote_port, volatile int *connect_ret);

// brief: connect a tcp client handle to remote ip:port
// return =0, ok. <0, error
int tcp_connect (U32 handle, ip_addr_t *remote_ip, U16 remote_port);

// brief: open a tcp listen handle
// parm in: local_port: the tcp port to bind to
// parm out: handle: pointer to store the listen tcp handle if success
// return =0, success.
//             <0, error
int tcp_open_listen(U32 *handle, /*ip_addr_t *local_ip,*/ U16 local_port);

// brief: close a listen tcp handle
// return =0, ok. <0, error
int tcp_close_listen(U32 handle);

//  brief: accept a tcp connection
// parm in: handle: the listen tcp handle
// parm out:
//           accept_handle: pointer to store the accepted tcp client handle if return 1
//            remote_ip:remote_port: if not null, store where the tcp connection comes from
// return 1,  accepted a handle, the handle is stored in accept_handle
// return 0, no handle accepted
// return <0, error
// nonblocking, multi thread safe
int tcp_accept (U32 handle, U32 *accept_handle, ip_addr_t *remote_ip, U16 *remote_port);

// brief: send data to tcp client  handle.
// return >=0, number of bytes write.
// return <0, error
// nonblocking, multi thread safe
int tcp_send (U32 handle, const U8 *buf, int buf_len);

// 同步发送函数，直到把所有数据发完，或出错才返回
// return >=0, number of bytes write.
// return <0, error
// blocking, multi thread safe
int tcp_send_wait (U32 handle, const U8 *buf, int buf_len);

// brief: read data from tcp client handle.
// return >=0, number of bytes read.
// return = NE_TCP_FIN, means got the FIN flag. no more data can be read from the connection.
//            other: error
// nonblocking, multi thread safe
int tcp_recv (U32 handle, U8 *buf, int buf_len);

// 同步接收函数，直到buf_len字节，或遇到FIN, 或出错才返回; 遇到FIN时返回已收到的字节数
// return >=0, number of bytes read. if ret>=0 && ret!=buf_len,  got FIN.
// return <0, error
// blocking, multi thread safe
int tcp_recv_wait (U32 handle, U8 *buf, int buf_len);

// brief: 返回tcp可发送字节数
// 返回>=0, tcp out buf free bytes.
//          <0, err
// nonblocking, multi thread safe
int tcp_can_send(U32 handle);

// brief: 返回tcp可读取字节数
// 返回>=0, tcp in buf ready bytes
//          <0, err
// nonblocking, multi thread safe
int tcp_can_recv(U32 handle);

#endif // _TCP_H_
