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
s8 func_800BDEB0(void);
s16 sound_pitch_diff_calc(void *arg0, s16 arg1);

static s32 all_chars(void)
{
    return -1;
}

void race_position_update(u8 *dst, u8 *src, s16 maxw)
{
    u8 *p;
    u8 tail[20];
    s32 w, avail, n, wide;
    s32 lim;

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
    avail = object_manager_update(tail, -1);
    lim = all_chars();
    while (maxw - avail - func_800BDEB0() < w) {
        n--;
        if (wide) {
            dst[n * 2 + 1] = 0;
            dst[n * 2 + 2] = 0;
        } else {
            dst[n] = 0;
        }
        if (n == 0)
            break;
        w = object_manager_update(dst, lim);
    }
    func_800BE4F0(dst, tail);
}
