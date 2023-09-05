#ifndef __TOOLS_H__
#define __TOOLS_H__

#ifdef __cplusplus

//#ifndef max
#define max1(a,b) (((a)>(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))
//#endif

U16 CRC16(U8 *ptr,int len);
U16 CRC16_1(U8 *ptr, int len);
U16 CRC16_2(U8 *ptr, int len, U8 *ptr2, int len2);

static inline U16 Get16(U8 *ptr)
{
	U16 temp;
	temp = ptr[0];
	temp = (temp<<8)|ptr[1];
	return temp;
}

static inline S16 GetS16(U8 *ptr)
{
	U16 temp;
	temp = ptr[0];
	temp = (temp<<8)|ptr[1];
	return (S16)temp;
}

static inline U32 Get24(U8 *ptr)
{
	U32 v;
	v = ptr[0];
	v = (v<<8)|ptr[1];
	v = (v<<8)|ptr[2];
	return(v);
}

static inline U32 Get32(U8 *ptr)
{
	U32 temp;
	temp = ptr[0];
	temp = (temp<<8)|ptr[1];
	temp = (temp<<8)|ptr[2];
	temp = (temp<<8)|ptr[3];
	return temp;
}

static inline void Set16(U8 *ptr, U16 v)
{
	ptr[0] = (U8)((v>>8)&0xff);
	ptr[1] = (U8)(v&0xff);
}

static inline void Set24(U8 *ptr, U32 v)
{
	ptr[0] = (U8)((v>>16)&0xff);
	ptr[1] = (U8)((v>>8)&0xff);
	ptr[2] = (U8)(v&0xff);
}

static inline void Set32(U8 *ptr, U32 v)
{
	ptr[0] = (U8)((v>>24)&0xff);
	ptr[1] = (U8)((v>>16)&0xff);
	ptr[2] = (U8)((v>>8)&0xff);
	ptr[3] = (U8)(v&0xff);
}

static inline U16 Get16_LE(U8 *ptr)
{
	U16 temp;
	temp = ptr[1];
	temp = (temp<<8)|ptr[0];
	return temp;
}

static inline S16 GetS16_LE(U8 *ptr)
{
	U16 temp;
	temp = ptr[1];
	temp = (temp<<8)|ptr[0];
	return (S16)temp;
}

static inline U32 Get24_LE(U8 *ptr)
{
	U32 v;
	v = ptr[2];
	v = (v<<8)|ptr[1];
	v = (v<<8)|ptr[0];
	return(v);
}

static inline U32 Get32_LE(U8 *ptr)
{
	U32 temp;
	temp = ptr[3];
	temp = (temp<<8)|ptr[2];
	temp = (temp<<8)|ptr[1];
	temp = (temp<<8)|ptr[0];
	return temp;
}

static inline void Set16_LE(U8 *ptr, U16 v)
{
	ptr[1] = (U8)((v>>8)&0xff);
	ptr[0] = (U8)(v&0xff);
}

static inline void Set24_LE(U8 *ptr, U32 v)
{
	ptr[2] = (U8)((v>>16)&0xff);
	ptr[1] = (U8)((v>>8)&0xff);
	ptr[0] = (U8)(v&0xff);
}

static inline void Set32_LE(U8 *ptr, U32 v)
{
	ptr[3] = (U8)((v>>24)&0xff);
	ptr[2] = (U8)((v>>16)&0xff);
	ptr[1] = (U8)((v>>8)&0xff);
	ptr[0] = (U8)(v&0xff);
}

// Endian switch
static inline void Switch16(U8 *ptr)
{
    U8 t = ptr[0];
    ptr[0] = ptr[1];
    ptr[1] = t;
}

static inline void SetBit32(U32 *ptr, int bit, U8 v)
{
	if (v)
	{
		*ptr |=(1<<bit);
	}
	else
	{
		*ptr &=~(1<<bit);
	}
}

static inline int GetBit32(U32 ptr, int bit)
{
	return (ptr>>bit) & 0x01;
}

static inline void SetBit16(U16 *ptr, int bit, U8 v)
{
	if (v)
	{
		*ptr |=(1<<bit);
	}
	else
	{
		*ptr &=~(1<<bit);
	}
}

static inline int GetBit16(U16 ptr, int bit)
{
	return (ptr>>bit) & 0x01;
}


static inline void SetBit8(U8 *ptr, int bit, U8 v)
{
	if (v)
	{
		*ptr |=(1<<bit);
	}
	else
	{
		*ptr &=~(1<<bit);
	}
}

static inline int GetBit8(U8 ptr, int bit)
{
	return (ptr>>bit) & 0x01;
}

int char20x(char *p);

void FillTemp(U8 *buf, float t);

U8 BCD2Char(U8 c);
U8 Char2BCD(U8 c);

BOOL Hex2str(char *str, int str_len, U8 *hex_buf, int hex_buf_len);

#endif //__cplusplus
#endif // __TOOLS_H__
