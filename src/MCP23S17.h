#ifndef __MCP23S17_h__
#define __MCP23S17_h__

#define ENABLE_CS_PIN(cs_port,cs_bit)   GPIO_ResetBits(cs_port,cs_bit)  //gpioSetValue(cs_port,cs_bit,0)//SetIOPort((((cs)&0xff00)>>8),((cs)&0xff),0)
#define DISABLE_CS_PIN(cs_port,cs_bit)  GPIO_SetBits(cs_port,cs_bit)  // gpioSetValue(cs_port,cs_bit,1) //SetIOPort((((cs)&0xff00)>>8),((cs)&0xff),1)

/*控制寄存器汇总（IOCON.BANK = 0）
		寄存器名称	地址（十六进制）*/
#define MCP_IODIRA		0x00
#define MCP_IODIRB		0x01
#define MCP_IPOLA		0x02
#define MCP_IPOLB		0x03
#define MCP_GPINTENA	0x04
#define MCP_GPINTENB	0x05
#define MCP_DEFVALA		0x06
#define MCP_DEFVALB		0x07
#define MCP_INTCONA		0x08
#define MCP_INTCONB		0x09
#define MCP_IOCONA		0x0A
#define MCP_IOCONB		0x0B
#define MCP_GPPUA		0x0C
#define MCP_GPPUB		0x0D
#define MCP_INTFA		0x0E
#define MCP_INTFB		0x0F
#define MCP_INTCAPA		0x10
#define MCP_INTCAPB		0x11
#define MCP_GPIOA		0x12
#define MCP_GPIOB		0x13
#define MCP_OLATA		0x14
#define MCP_OLATB		0x15



#define MCP_DIR_IN		0xffff
#define MCP_DIR_OUT		0


void MCP23S17_Init(void);
void MCP23S17_Write(U8 addr,U8 reg,U8 data, GPIO_TypeDef*  cs_prot,U16 cs_bit);

U8 MCP23S17_Read(U8 addr,U8 reg, GPIO_TypeDef*  cs_prot,U16 cs_bit);

#endif//__MCP23S17_h__
