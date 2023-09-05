#ifndef _COMMBUF_H_
#define _COMMBUF_H_

#include "ringbuf.h"

typedef void (*COMM_Write)(const char *buf, int num);
typedef int  (*COMM_TryRead)(char *buf, int num);
typedef void (*COMM_Read)(char *buf, int num);
typedef void (*COMM_RxFlush)(void);
typedef void (*COMM_Lock)(void);

struct CommObj {
    ring_buf_t  *rb;
    COMM_Lock    Lock;
    COMM_Lock    Unlock;
    COMM_Write   Write;
    COMM_TryRead TryRead;
    COMM_Read    Read;
    COMM_RxFlush Flush;  // 刷新CommBuf函数

    void init(ring_buf_t  *rb_in,
              COMM_Lock    lock_in,
              COMM_Lock    unlock_in,
              COMM_Write   write_in,
              COMM_TryRead try_read_in,
              COMM_Read    read_in,
              COMM_RxFlush flush_in)
    {
        rb      = rb_in;
        Lock    = lock_in;
        Unlock  = unlock_in;
        Write   = write_in;
        TryRead = try_read_in;
        Read    = read_in;
        Flush   = flush_in;
    }

    // 返回接收缓冲区中的有效字符数
    int RxNum()
    {
        return rb->num;
    }

	int clear()
	{
		if (rb->num>0)
		{
			Lock();
			Flush();
			Skip(rb->num);
			Unlock;
		}
	}


    // 从接收缓冲区peek一个字符. pos=0, 表示第一个有效字符
    char  PeekChar(int pos)
    {
        return ring_buf_peek_char(rb, pos);
    }

    // 从接收缓冲区peek n个字符. pos=0, 表示第一个有效字符
    // 返回有效的字符数
    int Peek(int pos, char *buf, int n)
    {
        return ring_buf_peek(rb, pos, (U8 *)buf, n);
    }

    // 接收缓冲区跳过n个字符
    void Skip(int n)
    {
        ring_buf_skip(rb, n);
    }

    // 比较字符串. =0, ok. <0, 错误.
    int Cmp(int pos, const char *str)
    {
        return ring_buf_strcmp(rb, pos, str);
    }

    // 比较n个字符. =0, ok. <0, 错误.
    int Ncmp(int pos, const char *buf, int n)
    {
        return ring_buf_ncmp(rb, pos, buf, n);
    }

    // 搜索1个字符, 返回>=0, 字符相对第一个有效字符的位置. <0, 错误
    int SearchChar(int pos, char c)
    {
        return ring_buf_search_char(rb, pos, c);
    }

    // 搜索n个字符, 返回>=0, 字符相对第一个有效字符的位置. <0, 错误
    int Search(int pos, const char *buf, int n)
    {
        return ring_buf_search(rb, pos, buf, n);
    }
};

#endif // _COMMBUF_H_
