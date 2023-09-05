#ifndef _SPI_H_
#define _SPI_H_

void SPI_Init_Core(SPI_TypeDef *SPIx);

/* �ȴ�æ��־λΪ��ͷ��ͻ���Ϊδ�� */
#define WAIT_TX(SPIx,retry)  while( (((SPIx)->SR & (SPI_I2S_FLAG_TXE|SPI_I2S_FLAG_BSY)) != SPI_I2S_FLAG_TXE ) && ((retry--)>0))

/* �ȴ�æ��־λΪ��ͽ��ջ������ǿ� */
#define WAIT_RX(SPIx,retry)  while ( ((SPIx)->SR & (SPI_I2S_FLAG_BSY|SPI_I2S_FLAG_RXNE)) != SPI_I2S_FLAG_RXNE && ((retry--)>0) )



#endif //_SPI_H_

