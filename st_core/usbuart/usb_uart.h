#ifndef _USB_UART_H_
#define _USB_UART_H_

void USB_Uart_Init();

void USB_Uart_Write(const char *buf, int num);

int USB_Uart_TryRead(char *buf, int num);
void USB_Uart_Read(char *buf, int num);

void USB_LOG_PRINT(const char *fmt, ... );

extern CommObj comm_usb;

#define usb_isr_lock()   __set_BASEPRI(((NVIC_USB_FS_PRIO) << (8 - __NVIC_PRIO_BITS)) & 0xff)
#define usb_isr_unlock() __set_BASEPRI(0)  // 0 表示不关闭任何中断

// =============以下函数必须在usb_isr_lock()和usb_isr_unlock()之间调用===================

// 从DMA buffer中刷新到uart rx_ringbuf中
//void USB_Uart_RxFlush();

// 返回接收缓冲区中的有效字符数
int USB_Uart_RxNum();

// 从接收缓冲区peek一个字符. pos=0, 表示第一个有效字符
char  USB_Uart_PeekChar(int pos);

// 从接收缓冲区peek n个字符. pos=0, 表示第一个有效字符
// 返回有效的字符数
int USB_Uart_Peek(int pos, char *buf, int n);

// 接收缓冲区跳过n个字符
void USB_Uart_Skip(int n);

// 比较n个字符. =0, ok. <0, 错误.
int USB_Uart_ncmp(int pos, const char *buf, int n);

// 比较字符串. =0, ok. <0, 错误.
int USB_Uart_cmp(int pos, const char *str);

// 搜索1个字符, 返回>=0, 字符相对第一个有效字符的位置. <0, 错误
int USB_Uart_SearchChar(int pos, char c);

// 搜索n个字符, 返回>=0, 字符相对第一个有效字符的位置. <0, 错误
int USB_Uart_Search(int pos, const char *buf, int n);

#endif // _USB_UART_H_
