#ifndef _STRINGH_H_
#define _STRINGH_H_

static inline char *str_chr(char *s, int c)
{
    while (*s != (char)c) {
        if (!*s)
            return NULL;
        s++;
    }
    return (char *)s;
}

static inline char *str_cpy(char *dst, const char *src)
{
    char *q = dst;
    const char *p = src;
    char ch;
    do {
        *q++ = ch = *p++;
    } while (ch);
    return dst;
}

static inline int str_cmp(const char *s1, const char *s2)
{
    const unsigned char *c1 = (const unsigned char *)s1;
    const unsigned char *c2 = (const unsigned char *)s2;
    unsigned char ch;
    int d = 0;
    while (1) {
        d = (int)(ch = *c1++) - (int)*c2++;
        if (d || !ch)
            break;
    }
    return d;
}

static inline int str_len(const char *s)
{
    const char *ss = s;
    while (*ss)
        ss++;
    return (int)(ss - s);
}

#endif //_STRINGH_H_

