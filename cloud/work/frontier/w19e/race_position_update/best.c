/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef signed int s32;
extern u8 D_80121DE8[];   /* "..." */
extern s8 D_80149B70;     /* font letter spacing */
u8 *func_800BE4F0(u8 *destination, u8 *source);   /* strcat */
u8 *func_800BE6A4(u8 *destination, u8 *source);   /* strcpy */
int func_800BE744(u8 *string);                    /* strlen */
s32 object_manager_update(u8 *str, s16 maxlen);    /* text width */

void race_position_update(u8 *dst, u8 *src, s16 maxw)
{
    u8 tail[20];
    s32 w, avail, n, wide;
    u8 *p;

    w = object_manager_update(src, -1);
    if (w <= maxw) {
        func_800BE6A4(dst, src);
        return;
    }
    wide = (*dst == 255);
    func_800BE6A4(dst, src);
    n = func_800BE744(dst);
    if (n < 2)
        return;
    n -= 2;
    if (wide) {
        tail[0] = 255;
        func_800BE4F0(tail, D_80121DE8);
        p = &dst[n * 2];
        tail[7] = p[1];
        tail[8] = p[2];
        tail[9] = p[3];
        tail[10] = p[4];
        tail[11] = 0;
        tail[12] = 0;
        p[1] = 0;
        p[2] = 0;
    } else {
        func_800BE6A4(tail, D_80121DE8);
        func_800BE4F0(tail, dst + n);
        dst[n] = 0;
    }
    avail = maxw - object_manager_update(tail, -1);
    while (avail - D_80149B70 < w) {
        n--;
        if (wide) {
            p = &dst[n * 2];
            p[1] = 0;
            p[2] = 0;
        } else {
            dst[n] = 0;
        }
        if (n == 0)
            break;
        w = object_manager_update(dst, -1);
    }
    func_800BE4F0(dst, tail);
}
