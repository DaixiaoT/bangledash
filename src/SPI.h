#ifndef _SPI_H_
#define _SPI_H_

void SPI_Init_Core(SPI_TypeDef *SPIx);

/* 等待忙标志位为零和发送缓冲为未满 */
#define WAIT_TX(SPIx,retry)  while( (((SPIx)->SR & (SPI_I2S_FLAG_TXE|SPI_I2S_FLAG_BSY)) != SPI_I2S_FLAG_TXE ) && ((retry--)>0))

/* 等待忙标志位为零和接收缓冲器非空 */
#define WAIT_RX(SPIx,retry)  while ( ((SPIx)->SR & (SPI_I2S_FLAG_BSY|SPI_I2S_FLAG_RXNE)) != SPI_I2S_FLAG_RXNE && ((retry--)>0) )



#endif //_SPI_H_

