#ifndef _MB85RS256B_
#define _MB85RS256B_

#ifdef _SPI_FRAM_
#define SPI_FRAM_SIZE 0x8000  // 32KB
#else
#define SPI_FRAM_SIZE 0x1000  // 4KB
#endif // _SPI_FRAM_

void SpiFramInit();
void SpiFramRead(U16 addr, U8 *buf, int len);
void SpiFramWrite(U16 addr, U8 *buf, int len);



#endif // _MB85RS256B_

