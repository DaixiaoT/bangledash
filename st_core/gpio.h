#ifndef _GPIO_H_
#define _GPIO_H_

/*
static inline void gpioSetDir(int port, int pin, int val)
{
    if(val)
        LPC_GPIO(port)->DIR |= ((U32)1<<pin);
    else
        LPC_GPIO(port)->DIR &= ~((U32)1<<pin);
}
*/
static inline void gpioSetValue(GPIO_TypeDef* GPIOx, U16 pin, int val)
{
    if(val)
        //GPIOx->BSRRL = (uint16_t)(1<<pin); // SET
        GPIO_SetBits(GPIOx, (uint16_t)(1<<pin));
    else
        //GPIOx->BSRRH = (uint16_t)(1<<pin); // CLR
        GPIO_ResetBits(GPIOx, (uint16_t)(1<<pin));
}

//
static inline void gpioInvert(GPIO_TypeDef* GPIOx, U16 pin)
{
    GPIOx->ODR ^= (1<<pin);
}

static inline int gpioGetInValue(GPIO_TypeDef* GPIOx, U16 pin)
{
    return ((GPIOx->IDR)>>pin) & 1;
}

static inline int gpioGetOutValue(GPIO_TypeDef* GPIOx, U16 pin)
{
    return ((GPIOx->ODR)>>pin) & 1;
}

void GPIO_AF_Set(GPIO_TypeDef* GPIOx,U8 BITx, U8 AFx);
void GPIO_Set(GPIO_TypeDef* GPIOx,U32 BITx,U32 MODE,U32 OTYPE,U32 OSPEED,U32 PUPD);

#endif // _GPIO_H_
