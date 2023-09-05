#include "sys_api.h"

#include "LVF_PJ.h"




const INT16U crctable[256] = {
(INT16U)0x0000, (INT16U)0xc1c0, (INT16U)0x81c1, (INT16U)0x4001, (INT16U)0x01c3, (INT16U)0xc003, (INT16U)0x8002, (INT16U)0x41c2, (INT16U)0x01c6, (INT16U)0xc006,
(INT16U)0x8007, (INT16U)0x41c7, (INT16U)0x0005, (INT16U)0xc1c5, (INT16U)0x81c4, (INT16U)0x4004, (INT16U)0x01cc, (INT16U)0xc00c, (INT16U)0x800d, (INT16U)0x41cd,
(INT16U)0x000f, (INT16U)0xc1cf, (INT16U)0x81ce, (INT16U)0x400e, (INT16U)0x000a, (INT16U)0xc1ca, (INT16U)0x81cb, (INT16U)0x400b, (INT16U)0x01c9, (INT16U)0xc009,
(INT16U)0x8008, (INT16U)0x41c8, (INT16U)0x01d8, (INT16U)0xc018, (INT16U)0x8019, (INT16U)0x41d9, (INT16U)0x001b, (INT16U)0xc1db, (INT16U)0x81da, (INT16U)0x401a,
(INT16U)0x001e, (INT16U)0xc1de, (INT16U)0x81df, (INT16U)0x401f, (INT16U)0x01dd, (INT16U)0xc01d, (INT16U)0x801c, (INT16U)0x41dc, (INT16U)0x0014, (INT16U)0xc1d4,
(INT16U)0x81d5, (INT16U)0x4015, (INT16U)0x01d7, (INT16U)0xc017, (INT16U)0x8016, (INT16U)0x41d6, (INT16U)0x01d2, (INT16U)0xc012, (INT16U)0x8013, (INT16U)0x41d3,
(INT16U)0x0011, (INT16U)0xc1d1, (INT16U)0x81d0, (INT16U)0x4010, (INT16U)0x01f0, (INT16U)0xc030, (INT16U)0x8031, (INT16U)0x41f1, (INT16U)0x0033, (INT16U)0xc1f3,
(INT16U)0x81f2, (INT16U)0x4032, (INT16U)0x0036, (INT16U)0xc1f6, (INT16U)0x81f7, (INT16U)0x4037, (INT16U)0x01f5, (INT16U)0xc035, (INT16U)0x8034, (INT16U)0x41f4,
(INT16U)0x003c, (INT16U)0xc1fc, (INT16U)0x81fd, (INT16U)0x403d, (INT16U)0x01ff, (INT16U)0xc03f, (INT16U)0x803e, (INT16U)0x41fe, (INT16U)0x01fa, (INT16U)0xc03a,
(INT16U)0x803b, (INT16U)0x41fb, (INT16U)0x0039, (INT16U)0xc1f9, (INT16U)0x81f8, (INT16U)0x4038, (INT16U)0x0028, (INT16U)0xc1e8, (INT16U)0x81e9, (INT16U)0x4029,
(INT16U)0x01eb, (INT16U)0xc02b, (INT16U)0x802a, (INT16U)0x41ea, (INT16U)0x01ee, (INT16U)0xc02e, (INT16U)0x802f, (INT16U)0x41ef, (INT16U)0x002d, (INT16U)0xc1ed,
(INT16U)0x81ec, (INT16U)0x402c, (INT16U)0x01e4, (INT16U)0xc024, (INT16U)0x8025, (INT16U)0x41e5, (INT16U)0x0027, (INT16U)0xc1e7, (INT16U)0x81e6, (INT16U)0x4026,
(INT16U)0x0022, (INT16U)0xc1e2, (INT16U)0x81e3, (INT16U)0x4023, (INT16U)0x01e1, (INT16U)0xc021, (INT16U)0x8020, (INT16U)0x41e0, (INT16U)0x01a0, (INT16U)0xc060,
(INT16U)0x8061, (INT16U)0x41a1, (INT16U)0x0063, (INT16U)0xc1a3, (INT16U)0x81a2, (INT16U)0x4062, (INT16U)0x0066, (INT16U)0xc1a6, (INT16U)0x81a7, (INT16U)0x4067,
(INT16U)0x01a5, (INT16U)0xc065, (INT16U)0x8064, (INT16U)0x41a4, (INT16U)0x006c, (INT16U)0xc1ac, (INT16U)0x81ad, (INT16U)0x406d, (INT16U)0x01af, (INT16U)0xc06f,
(INT16U)0x806e, (INT16U)0x41ae, (INT16U)0x01aa, (INT16U)0xc06a, (INT16U)0x806b, (INT16U)0x41ab, (INT16U)0x0069, (INT16U)0xc1a9, (INT16U)0x81a8, (INT16U)0x4068,
(INT16U)0x0078, (INT16U)0xc1b8, (INT16U)0x81b9, (INT16U)0x4079, (INT16U)0x01bb, (INT16U)0xc07b, (INT16U)0x807a, (INT16U)0x41ba, (INT16U)0x01be, (INT16U)0xc07e,
(INT16U)0x807f, (INT16U)0x41bf, (INT16U)0x007d, (INT16U)0xc1bd, (INT16U)0x81bc, (INT16U)0x407c, (INT16U)0x01b4, (INT16U)0xc074, (INT16U)0x8075, (INT16U)0x41b5,
(INT16U)0x0077, (INT16U)0xc1b7, (INT16U)0x81b6, (INT16U)0x4076, (INT16U)0x0072, (INT16U)0xc1b2, (INT16U)0x81b3, (INT16U)0x4073, (INT16U)0x01b1, (INT16U)0xc071,
(INT16U)0x8070, (INT16U)0x41b0, (INT16U)0x0050, (INT16U)0xc190, (INT16U)0x8191, (INT16U)0x4051, (INT16U)0x0193, (INT16U)0xc053, (INT16U)0x8052, (INT16U)0x4192,
(INT16U)0x0196, (INT16U)0xc056, (INT16U)0x8057, (INT16U)0x4197, (INT16U)0x0055, (INT16U)0xc195, (INT16U)0x8194, (INT16U)0x4054, (INT16U)0x019c, (INT16U)0xc05c,
(INT16U)0x805d, (INT16U)0x419d, (INT16U)0x005f, (INT16U)0xc19f, (INT16U)0x819e, (INT16U)0x405e, (INT16U)0x005a, (INT16U)0xc19a, (INT16U)0x819b, (INT16U)0x405b,
(INT16U)0x0199, (INT16U)0xc059, (INT16U)0x8058, (INT16U)0x4198, (INT16U)0x0188, (INT16U)0xc048, (INT16U)0x8049, (INT16U)0x4189, (INT16U)0x004b, (INT16U)0xc18b,
(INT16U)0x818a, (INT16U)0x404a, (INT16U)0x004e, (INT16U)0xc18e, (INT16U)0x818f, (INT16U)0x404f, (INT16U)0x018d, (INT16U)0xc04d, (INT16U)0x804c, (INT16U)0x418c,
(INT16U)0x0044, (INT16U)0xc184, (INT16U)0x8185, (INT16U)0x4045, (INT16U)0x0187, (INT16U)0xc047, (INT16U)0x8046, (INT16U)0x4186, (INT16U)0x0182, (INT16U)0xc042,
(INT16U)0x8043, (INT16U)0x4183, (INT16U)0x0041, (INT16U)0xc181, (INT16U)0x8180, (INT16U)0x4040,
};



INT16U crc16(INT8U* pStart,INT16U size)
{
    INT16U crc = 0xFFFF;
    INT16U dat;

    while (size--) {
       dat = (unsigned short ) *pStart++;
       crc = (unsigned short )((crc << 8) ^ crctable[((crc >> 8) ^ dat) & (unsigned short )0xFF]);
    }

    return crc;
}

