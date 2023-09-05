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
    COMM_RxFlush Flush;  // ˢ��CommBuf����

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

    // ���ؽ��ջ������е���Ч�ַ���
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


    // �ӽ��ջ�����peekһ���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
    char  PeekChar(int pos)
    {
        return ring_buf_peek_char(rb, pos);
    }

    // �ӽ��ջ�����peek n���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
    // ������Ч���ַ���
    int Peek(int pos, char *buf, int n)
    {
        return ring_buf_peek(rb, pos, (U8 *)buf, n);
    }

    // ���ջ���������n���ַ�
    void Skip(int n)
    {
        ring_buf_skip(rb, n);
    }

    // �Ƚ��ַ���. =0, ok. <0, ����.
    int Cmp(int pos, const char *str)
    {
        return ring_buf_strcmp(rb, pos, str);
    }

    // �Ƚ�n���ַ�. =0, ok. <0, ����.
    int Ncmp(int pos, const char *buf, int n)
    {
        return ring_buf_ncmp(rb, pos, buf, n);
    }

    // ����1���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
    int SearchChar(int pos, char c)
    {
        return ring_buf_search_char(rb, pos, c);
    }

    // ����n���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
    int Search(int pos, const char *buf, int n)
    {
        return ring_buf_search(rb, pos, buf, n);
    }
};

#endif // _COMMBUF_H_
