#ifndef _SST25VF064B_H_
#define _SST25VF064B_H_
#include "ac_include.h"

#define SPI_FLASH_SIZE 0x800000
#define SPI_FLASH_TEST_SIZE 0x800
#define SPI_FLASH_MASK (SPI_FLASH_SIZE-1)

void SpiFlashInit(void);
void SpiFlashRead(U32 addr, U8 *buf, int len);
void SpiFlashWrite(U32 addr, U8 *buf, int len);
void SpiFlashErase4K(U32 addr);
void SpiFlashErase32K(U32 addr);
void SpiFlashErase64K(U32 addr);

void SpiFlashEraseFull(void);



#endif //_SST25VF064B_H_

