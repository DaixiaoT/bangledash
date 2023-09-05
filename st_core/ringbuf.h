#ifndef _RINGBUF_H_
#define _RINGBUF_H_

typedef struct ring_buf {
    U16 start;     // start position of valid data bytes
    U16 num;       // valid data bytes
    U16 buf_size;  // buffer size
    U16 user;      // user defined data
    U8 *buf;       // buffer base address
} ring_buf_t;

#define ring_buf_inc(rb, idx) { (idx)++; if((idx)>=(rb)->buf_size) (idx)=0; }

#define ring_buf_not_full(rb) ((rb)->num < (rb)->buf_size)
#define ring_buf_not_full2(rb, pos) ((rb)->num + (pos) < (rb)->buf_size)

#define ring_buf_free_space(rb) ((rb)->buf_size - (rb)->num)

// brief: init a ring buffer
static inline void ring_buf_init(ring_buf_t *rb, U8 *buf, U16 buf_size)
{
    rb->buf = buf;
    rb->buf_size = buf_size;
    rb->start = rb->num = rb->user = 0;
}

// 往ring buffer中写数据；返回写的字节数.
static inline int ring_buf_put(ring_buf_t *rb, const U8 *buf, int len)
{
    int copy = my_min(rb->buf_size - rb->num, len);
    int k = rb->start + rb->num;
    int i;
    if(k >= rb->buf_size)
        k -= rb->buf_size;
    for(i=0; i<copy; i++) {
        rb->buf[k] = buf[i];
        ring_buf_inc(rb, k);
    }
    rb->num += copy;
    return copy;
}

// 往ring buffer中peek写数据；返回写的字节数.  peek写，不调整num.
static inline int ring_buf_put_peek(ring_buf_t *rb, int pos, const U8 *buf, int len)
{
    int copy = my_min(rb->buf_size - rb->num - pos, len);
    int k = rb->start + rb->num + pos;
    int i;
    if(k >= rb->buf_size)
        k -= rb->buf_size;
    for(i=0; i<copy; i++) {
        rb->buf[k] = buf[i];
        ring_buf_inc(rb, k);
    }
    //rb->num += copy;
    return copy;
}

// 调整ringbuf num
static inline void ring_buf_fix_num(ring_buf_t *rb, int len)
{
    if(len <= 0)
        return;
    rb->num += len;
    if(rb->num > rb->buf_size)
        rb->num = rb->buf_size;
}

// 从ring buffer中读数据；返回读取的字节数.
static inline int ring_buf_get(ring_buf_t *rb, U8 *buf, int len)
{
    int copy = my_min(rb->num, len);
    int k = rb->start;
    int i;
    for(i=0; i<copy; i++) {
        buf[i] = rb->buf[k];
        ring_buf_inc(rb, k);
    }
    rb->start = k;
    rb->num -= copy;
    return copy;
}

static inline U8 ring_buf_peek_char(ring_buf_t *rb, int pos)
{
    if(pos>=rb->num)
        return 0;

    int k = rb->start + pos;
    if(k >= rb->buf_size)
        k -= rb->buf_size;

    return rb->buf[k];
}

// 从ring buffer中Peek数据；从pos位置开始，返回读取的字节数. ring buffer中数据不变
static inline int ring_buf_peek(ring_buf_t *rb, int pos, U8 *buf, int len)
{
    int i, k;
    if((pos + len) > rb->num)
        len = rb->num - pos;
    k = rb->start + pos;
    if(k >= rb->buf_size)
        k -= rb->buf_size;
    for(i=0; i<len; i++) {
        buf[i] = rb->buf[k];
        ring_buf_inc(rb, k);
    }
    return len;
}

// ring buffer中丢弃n字节
static inline void ring_buf_skip(ring_buf_t *rb, int n)
{
    if(n >= rb->num) {
        rb->start = rb->num = 0;
    }
    else if(n>0) {
        rb->start += n;
        if(rb->start>=rb->buf_size)
            rb->start -= rb->buf_size;
        rb->num -= n;
    }
}

// get one byte from ring buffer
// this function must be called when num>0
static inline U8 ring_buf_get_byte(ring_buf_t *rb)
{
    U8 b = rb->buf[rb->start];
    ring_buf_inc(rb, rb->start);
    rb->num--;
    return b;
}

// put one byte into ring buffer, if buffer is already full, the oldest byte is dropped.
static inline void ring_buf_put_byte(ring_buf_t *rb, U8 b)
{
    int k = rb->start + rb->num;
    if(k >= rb->buf_size)
        k -= rb->buf_size;
    rb->buf[k] = b;
    if(rb->num==rb->buf_size) {
        ring_buf_inc(rb, rb->start);
    }
    else
        rb->num++;
}


// 比较字符串. =0, ok. <0, 错误.
static inline int ring_buf_strcmp(ring_buf_t *rb, int pos, const char *str)
{
	int i = (rb->start + pos);
    if(i >= rb->buf_size)
        i -= rb->buf_size;

	while(*str) {
		if(pos >= rb->num)
			return -1;
		if(*str != rb->buf[i])
			return -1;
		ring_buf_inc(rb, i);
		pos++;
		str++;
	}

	return 0;
}

// 比较字符串. =0, ok. <0, 错误.
static inline int ring_buf_ncmp(ring_buf_t *rb, int pos,
                                const char *str, int n)
{
	if(pos + n > rb->num)
		return -1;

	int i = (rb->start + pos);
    if(i >= rb->buf_size)
        i -= rb->buf_size;

	while(n-->0) {
		//if(pos >= rb->num)
			//return -1;
		if(*str != rb->buf[i])
			return -1;
		ring_buf_inc(rb, i);
		//pos++;
		str++;
	}

	return 0;
}

// 搜索1个字符, 返回>=0, 字符相对第一个有效字符的位置. <0, 错误
static inline int ring_buf_search_char(ring_buf_t *rb, int pos, char c)
{
	int i = (rb->start + pos);
    if(i >= rb->buf_size)
        i -= rb->buf_size;

	while(pos < rb->num) {
		if(c == rb->buf[i])
			return pos;
		ring_buf_inc(rb, i);
		pos++;
	}

	return -1;
}

// 搜索n个字符, 返回>=0, 字符相对第一个有效字符的位置. <0, 错误
static inline int ring_buf_search(ring_buf_t *rb, int pos,
                                  const char *buf, int n)
{
    int k, v;
	int i = (rb->start + pos);
    if(i >= rb->buf_size)
        i -= rb->buf_size;

	while(pos + n <= rb->num) {
        v = i;
    	for(k=0; k<n; k++) {
            if(buf[k] != rb->buf[v])
                break;
            ring_buf_inc(rb, v);
        }
        if(k==n)
            return pos;

		ring_buf_inc(rb, i);
		pos++;
	}

	return -1;
}

#endif // _RINGBUF_H_
