#ifndef _PACKET_H_
#define _PACKET_H_

#define PACKET_EXT_BUF_NUM 2

#define PACKET_OWN_EXT_BUF 0x80  // free ext_buf[] in pk_free()

/* packet结构：
    第一块数据区，紧跟在packet后面
    其他数据，在buf[]中。
    第一块数据区由buf_len, data_pos, data_len控制: buf_len表示第一块数据区总长度，
    data_pos表示第一块数据区中有效数据的开始偏移；有效数据长度=data_len
    ext_buf中没有data_len, data_pos等控制字段。
    ext_buf中的数据总长度=total_len - data_len;
    ext_buf[0]中的数据长度=min(total_len - data_len, ext_buf_len[0])，如果ext_buf[0]满了，则
    ext_buf[1]中的数据长度=min(total_len - data_len - ext_buf_len[0]，ext_buf_len[1])。
    nif: 指向netif; 接收时，表示包从哪个netif来；发送时，表示发往哪个netif
    flag:  PACKET_OWN_EXT_BUF，此位有效，表示ext_buf由packet拥有，释放packet时，要释放ext_buf
*/
struct netif;
typedef struct packet {
    struct packet *next; //
    U16 buf_len;                //  data buffer len
    U16 data_pos;             //  offset in data buffer
    U16 data_len;              //  data len in data buffer
	U16 total_len;             //   total data len in packet
    U8 *ext_buf[PACKET_EXT_BUF_NUM];  // extended buffer to store data
    U16 ext_buf_len[PACKET_EXT_BUF_NUM];
	struct netif  *nif;                       //   netif index
	U8   flag;
} packet_t;

#define packet_buf(pk)   ((U8 *)(pk) + sizeof(packet_t))   // 取第一块数据区的地址
#define packet_data(pk) ((U8 *)(pk) + sizeof(packet_t) + (pk)->data_pos)   // // 取第一块数据区中有效数据的地址

#define packet_skip(pk, len)  (pk)->data_pos += (len);  (pk)->data_len -= (len);  (pk)->total_len -= (len);    // packet有效数据减少len字节(如果len<0，则是增加)

// alloc a packet，size表示要分配包的总大小；必须要考虑各层协议占用的空间
packet_t *packet_alloc(int size);
// free the packet
void packet_free(packet_t *pk);

// brief:     把包内容读入buf中，
// return:  >=0, 读取的字节数；<0, 错误码
int packet_read(packet_t *pk, int skip_bytes, U8 *buf, int buf_len);

// brief: 把buf内容写入packet中，
// return:  >=0,写入的字节数；<0, 错误码
int packet_write(packet_t *pk, const U8 *buf, int buf_len);

// append data into packet
// return >=0, bytes write, <0, err
int packet_append(packet_t *pk, const U8 *buf, int buf_len);

// brief: 把一个包数据拷入ring buf；此函数主要在tcp接收时调用
// parm:  skip_bytes: 包中需跳过的字节数；这样可以跳过包中的部分字节再拷贝；skip_bytes=0则拷贝整包
// return >=0, bytes copyed, <0, err
int ring_buf_put_from_packet(ring_buf_t *rb, packet_t *pk, int skip_bytes);

// brief：把一个包数据拷入ring buf；此函数主要在tcp接收到乱序的包(ooseq)时调用，乱序的包，仍然把数据考入ring buf中
// parm:  pos: ring buf中空白部分跳过pos字节再开始拷贝
// return >=0, bytes copyed, <0, err
int ring_buf_put_seg_from_packet(ring_buf_t *rb, int pos, packet_t *pk);

// 连接buf0, buf1到packet；packet 释放时会释放buf0, buf1
static inline void packet_chain(packet_t *pk, U8 *buf0, int len0, U8 *buf1, int len1)
{
    pk->ext_buf[0] = buf0;
    pk->ext_buf_len[0] = len0;
    pk->ext_buf[1] = buf1;
    pk->ext_buf_len[1] = len1;
    pk->total_len += len0 + len1;
    pk->flag |= PACKET_OWN_EXT_BUF;
}

// packet引用buf0, buf1；packet 释放时不会释放buf0, buf1
// tcp输出时会调用此函数
static inline void packet_ref(packet_t *pk, U8 *buf0, int len0, U8 *buf1, int len1)
{
    pk->ext_buf[0] = buf0;
    pk->ext_buf_len[0] = len0;
    pk->ext_buf[1] = buf1;
    pk->ext_buf_len[1] = len1;
    pk->total_len += len0 + len1;
    pk->flag &= ~PACKET_OWN_EXT_BUF;
}

#endif //_PACKET_H_

