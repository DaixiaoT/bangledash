#define MVB_PORT_NUM	6

#define FUNC_EQU_TEST 0x7C
#define FUNC_G0_DATA 0x7B

#define FUNC_MVB_GATEWAY 0x7B


// PA8, 运行灯绿色：=1亮，=0灭
#define LED_RUN_ON()		gpioSetValue(GPIOG, 8, 1)
#define LED_RUN_OFF()		gpioSetValue(GPIOG, 8, 0)
#define	LED_RUN_SWITCH()	gpioInvert(GPIOG, 8)
#define LED_RUN_SWITCH_TIME (1*1000)

// PA3, 运行灯绿色：=1亮，=0灭
#define PWRON24_ON()		gpioSetValue(GPIOA, 0, 0)
#define PWRON24_OFF()		gpioSetValue(GPIOA, 0, 1)
#define	PWRON24_SWITCH()	gpioInvert(GPIOA, 0)

// PD3, 故障灯红色：=1亮，=0灭
#define LED_FAULT_ON()		gpioSetValue(GPIOD, 3, 1)
#define LED_FAULT_OFF()		gpioSetValue(GPIOD, 3, 0)
#define	LED_FAULT_SWITCH()	gpioInvert(GPIOD, 3)
#define LED_FAULT_SWITCH_TIME (2*1000)

// PC12, 通讯灯绿色：=1亮，=0灭
#define LED_COMM_ON()		gpioSetValue(GPIOF, 10, 1)
#define LED_COMM_OFF()		gpioSetValue(GPIOF, 10, 0)
#define	LED_COMM_SWITCH()	gpioInvert(GPIOF, 10)
#define LED_COMM_SWITCH_TIME (2*1000)


// PF6 0=允许，1=禁止
#define WDEX_ON()  gpioSetValue(GPIOF, 6, 0)
#define WDEX_OFF() gpioSetValue(GPIOF, 6, 1)

// PF7, 喂狗信号，正常为小于1.6s脉冲
#define WDEX_FEED() gpioInvert(GPIOF, 7)

//PD2 蜂鸣器：=0不响，=1响
#define BEEP_ON()  gpioSetValue(GPIOD, 2, 1)
#define BEEP_OFF() gpioSetValue(GPIOD, 2, 0)
#define BEEP_INVERT() gpioInvert(GPIOD, 2)
#define BEEP(ms)    {BEEP_ON();sleep(ms);BEEP_OFF();}

// PE1, MVB网卡复位信号，低复位
#define TRDP_RST_LOW()  gpioSetValue(GPIOE, 1, 0)
#define TRDP_RST_HIGH() gpioSetValue(GPIOE, 1, 1)
//#define MVB_UART_BASE  0x68000000

// PD10, RS485 RS485接口方向控制：0=发送，1=接收
#define RS485_TX_ON()  gpioSetValue(GPIOD, 10, 0)
#define RS485_TX_OFF() gpioSetValue(GPIOD, 10, 1)

// PE3, MCP23S17_CS_IN1, 低有效
#define MCP23S17_CS_IN1_LOW()  gpioSetValue(GPIOE, 5, 0)
#define MCP23S17_CS_IN1_HIGH() gpioSetValue(GPIOE, 5, 1)

// PE4, MCP23S17_CS_IN2, 低有效
#define MCP23S17_CS_IN2_LOW()  gpioSetValue(GPIOE, 5, 0)
#define MCP23S17_CS_IN2_HIGH() gpioSetValue(GPIOE, 5, 1)

// PE5, MCP23S17_CS_IN3, 低有效
#define MCP23S17_CS_IN3_LOW()  gpioSetValue(GPIOE, 5, 0)
#define MCP23S17_CS_IN3_HIGH() gpioSetValue(GPIOE, 5, 1)

// PE6, MCP23S17_CS_IN4, 低有效
#define MCP23S17_CS_IN4_LOW()  gpioSetValue(GPIOE, 6, 0)
#define MCP23S17_CS_IN4_HIGH() gpioSetValue(GPIOE, 6, 1)

// PE6, MCP23S17_CS_IN5, 低有效
#define MCP23S17_CS_IN5_LOW()  gpioSetValue(GPIOE, 6, 0)
#define MCP23S17_CS_IN5_HIGH() gpioSetValue(GPIOE, 6, 1)


// PD11, MCP23S17_CS_OUT1, 低有效
#define MCP23S17_CS_OUT1_LOW()  gpioSetValue(GPIOD, 11, 0)
#define MCP23S17_CS_OUT1_HIGH() gpioSetValue(GPIOD, 11, 1)

// PD12, MCP23S17_CS_OUT2, 低有效
#define MCP23S17_CS_OUT2_LOW()  gpioSetValue(GPIOD, 12, 0)
#define MCP23S17_CS_OUT2_HIGH() gpioSetValue(GPIOD, 12, 1)

// PD13, MCP23S17_CS_OUT3, 低有效
#define MCP23S17_CS_OUT3_LOW()  gpioSetValue(GPIOD, 13, 0)
#define MCP23S17_CS_OUT3_HIGH() gpioSetValue(GPIOD, 13, 1)


// PA15 SPI1 FLASH 片选，低有效
#define SPI_FLASH1_CS_LOW()		gpioSetValue(GPIOA, 15, 0)
#define SPI_FLASH1_CS_HIGH()	gpioSetValue(GPIOA, 15, 1)

// PB6 SPI1 W5500 片选，低有效
#define SPI_W5500_CS_LOW()		gpioSetValue(GPIOB, 6, 0)
#define SPI_W5500_CS_HIGH()	    gpioSetValue(GPIOB, 6, 1)
//#define SPI_W5500_CS_HIGH()	  SPI_W5500_CS_LOW()		

// PG15 SPI1 FLASH 写保护，低保护
#define SPI_FLASH1_WRITE_PROTECT()  gpioSetValue(GPIOG, 15, 0)
#define SPI_FLASH1_WRITE_ENABLE()	gpioSetValue(GPIOG, 15, 1)

// PG12 SPI1 FRAM 片选，低有效
#define SPI_FRAM_CS_LOW()		gpioSetValue(GPIOG, 12, 0)
#define SPI_FRAM_CS_HIGH()	    gpioSetValue(GPIOG, 12, 1)

// PC13 W25 FLASH 片选，低有效
#define SPI_FLASH_W25_CS_LOW()		gpioSetValue(GPIOC, 13, 0)
#define SPI_FLASH_W25_CS_HIGH()		gpioSetValue(GPIOC, 13, 1)

// PG114 W25 FLASH 写保护，低保护
#define SPI_FLASH_W25_WRITE_PROTECT()   gpioSetValue(GPIOG, 14, 0)
#define SPI_FLASH_W25_WRITE_ENABLE()	gpioSetValue(GPIOG, 14, 1)

// PG13 SPI1 FRAM 写保护，低保护
#define SPI_FRAM_WRITE_PROTECT()  gpioSetValue(GPIOG, 13, 0)
#define SPI_FRAM_WRITE_ENABLE()   gpioSetValue(GPIOG, 13, 1)

#define TEMP100_1 0x4c
#define TEMP100_2 0x4e

// PC8, RS485 RS485接口方向控制：0=发送，1=接收
#define RS485_UART6_TX_ON()  gpioSetValue(GPIOC, 8, 0)
#define RS485_UART6_TX_OFF() gpioSetValue(GPIOC, 8, 1)

// PE2, RS485 RS485接口方向控制：0=发送，1=接收
#define RS485_UART4_TX_ON()  gpioSetValue(GPIOE,2, 0)
#define RS485_UART4_TX_OFF() gpioSetValue(GPIOE, 2, 1)


#ifdef __cplusplus
static inline U16 MCP23S17_GET()
{
    U16 dataL =  (U16)(GPIOA->IDR & 0xFF);
    U16 dataH =  (U16)(GPIOB->IDR & 0xFF);
    return (dataH<<8) | dataL;
}

static inline void MCP23S17_PUT(U16 data)
{
    U16 dataL = data & 0xFF;
    U16 dataH = (data>>8) & 0xFF;

    GPIOA->BSRRL = dataL;      // SET
    GPIOA->BSRRH = dataL^0xFF; // CLR

    GPIOB->BSRRL = dataH;      // SET
    GPIOB->BSRRH = dataH^0xFF; // CLR
}
#endif
