typedef unsigned int u32;
void *func_800A47C0(void *dst, const void *src, u32 n) {
    char *d = (char *)dst;
    const char *s = (const char *)src;
    if (s < d && d < s + n) {
        d += n;
        s += n;
        if (((u32)d & 3) == 0 && ((u32)s & 3) == 0) {
            while (n >= 4) {
                d -= 4;
                s -= 4;
                *(u32 *)d = *(const u32 *)s;
                n -= 4;
            }
        }
        while (n-- != 0) {
            *--d = *--s;
        }
    } else {
        if ((((u32)d | (u32)s) & 3) == 0) {
            while (n >= 4) {
                *(u32 *)d = *(const u32 *)s;
                d += 4;
                s += 4;
                n -= 4;
            }
        }
        while (n-- != 0) {
            *d++ = *s++;
        }
    }
    return dst;
}
