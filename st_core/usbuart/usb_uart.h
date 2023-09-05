#ifndef _USB_UART_H_
#define _USB_UART_H_

void USB_Uart_Init();

void USB_Uart_Write(const char *buf, int num);

int USB_Uart_TryRead(char *buf, int num);
void USB_Uart_Read(char *buf, int num);

void USB_LOG_PRINT(const char *fmt, ... );

extern CommObj comm_usb;

#define usb_isr_lock()   __set_BASEPRI(((NVIC_USB_FS_PRIO) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define usb_isr_unlock() __set_BASEPRI(0)  // 0 ��ʾ���ر��κ��ж�

// =============���º���������usb_isr_lock()��usb_isr_unlock()֮�����===================

// ��DMA buffer��ˢ�µ�uart rx_ringbuf��
//void USB_Uart_RxFlush();

// ���ؽ��ջ������е���Ч�ַ���
int USB_Uart_RxNum();

// �ӽ��ջ�����peekһ���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
char  USB_Uart_PeekChar(int pos);

// �ӽ��ջ�����peek n���ַ�. pos=0, ��ʾ��һ����Ч�ַ�
// ������Ч���ַ���
int USB_Uart_Peek(int pos, char *buf, int n);

// ���ջ���������n���ַ�
void USB_Uart_Skip(int n);

// �Ƚ�n���ַ�. =0, ok. <0, ����.
int USB_Uart_ncmp(int pos, const char *buf, int n);

// �Ƚ��ַ���. =0, ok. <0, ����.
int USB_Uart_cmp(int pos, const char *str);

// ����1���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int USB_Uart_SearchChar(int pos, char c);

// ����n���ַ�, ����>=0, �ַ���Ե�һ����Ч�ַ���λ��. <0, ����
int USB_Uart_Search(int pos, const char *buf, int n);

#endif // _USB_UART_H_
