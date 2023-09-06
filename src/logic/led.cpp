#include "led.h"

static const U8 segTable[18] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d,
                                0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x00, 0xff};

#define LED_DO_OFFSET 40

void set_led(U8 code)
{
    if (code >= 18)
    {
        PRINT_ERR_INT(code);
        code = 0xf;
    }

    U8 v = segTable[code];

    int i;
    for (i = 0; i < 8; i++)
    {
        if ((v >> i) & 1)
            DO_SET(LED_DO_OFFSET + i);
        else
            DO_CLR(LED_DO_OFFSET + i);
    }
}