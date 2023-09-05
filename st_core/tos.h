#ifndef _TOS_H_
#define _TOS_H_

/////////////////////////////////////////////////////////////////////////////
//                        OS����
/////////////////////////////////////////////////////////////////////////////

#define THREAD_NUM  8          // �̸߳���, �����ã�����ʵ�������. 0=idle�߳�, 1=���߳�

void os_init();


// ����0, ok. ����: ��ջ������̺߳�
int os_stack_check(BOOL isPrint=TRUE);
void os_run();

/////////////////////////////////////////////////////////////////////////////
//                        �ź���
/////////////////////////////////////////////////////////////////////////////

struct sema_t {
	int count;    // >=0, ��ʾ�ź�������. <0, ��ʾ�ȴ����߳���
};

// return 0, ok. <0, err
// timeout=0, ���ȴ�, timeout<0, ���޵ȴ�, timeout>0, �ȴ���ʱ��(����)
int p_sema(sema_t *, int timeout);
void v_sema(sema_t *);
#define init_sema(s, num) ((s)->count = (num))


/////////////////////////////////////////////////////////////////////////////
//                        �߳�
/////////////////////////////////////////////////////////////////////////////

typedef void THREAD_ENTRY(void *data);

// ����>=0, �߳�id, <0, ����
int create_thread(THREAD_ENTRY *entry,
				  void *data,
				  U32 stack_base,
				  U32 stack_size,
				  int priority);

void sleep(int ms);

// return 0, ok. <0, err
int wait_flag(volatile int *flag, int timeout);

/////////////////////////////////////////////////////////////////////////////
//                        �����л�
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//                        �������
/////////////////////////////////////////////////////////////////////////////

// �����
#define OE_OK 0

#define OE_BASE           -21000

#define OE_TIMEOUT        (OE_BASE)

#define OE_MEMOUT         (OE_BASE - 1)

// system errors:
#define OE_SYS            (OE_BASE - 2)
#define OE_BAD_THREAD_ID  (OE_BASE - 3)
#define OE_THREAD_FULL    (OE_BASE - 4)
#define OE_THREAD_STACK_ALIGN    (OE_BASE - 5)
#define OE_THREAD_STACK_SIZE     (OE_BASE - 6)

#endif // _TOS_H_

