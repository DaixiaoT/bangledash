#ifndef  _MI_MEM_H_
#define _MI_MEM_H_

// 内存管理初始化
int mi_mem_init(); //(U32 mem_addr, U32 size)

//分配一块内存，返回指针。返回0，表示内存用尽
void *mi_malloc(int size);

void *mi_calloc(int n,int size);

// 释放一块内存
void mi_free(void *p);

// 内存扫描，检查错误
void mi_mem_scan();

// 内存块头部大小
int mi_mem_block_head_size();

#endif //_MI_MEM_H_

