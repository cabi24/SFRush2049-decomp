/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* race_position_update = TB_EllipseText (MAT3 TB_text.c, GC 0x8004F668, size 0x190). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
extern s8 D_80149B70; /* gSpacing (HAL_text.c static) */
u32 object_manager_update(u8 *text, s16 numChars); /* HAL_GetTextWidth */
u8 *func_800BE4F0(u8 *str1, u8 *str2);             /* TB_Strcat */
u8 *func_800BE6A4(u8 *str1, u8 *str2);             /* TB_Strcpy */
u32 func_800BE744(u8 *s);                          /* TB_Strlen */
s8 func_800BDEB0(void);   /* HAL_GetFontSpacing (locked) */
u32 tw_width(u8 *s, s16 n) { return object_manager_update(s, n); }
void race_position_update(void *dst, void *src, s16 width)
{
    u8 *d = dst; s32 w; s32 dbl; s32 len; u8 eStr[16]; s32 ew; const u8 *ellipse = (const u8 *)"...";
    w = object_manager_update(src, -1);
    if (w <= width) func_800BE6A4(d, src);
    else {
        dbl = *d == 255;
        func_800BE6A4(d, src);
        len = func_800BE744(d);
        if (len <= 1) return;
        len -= 2;
        if (dbl) {
            eStr[0] = 255; func_800BE4F0(eStr, (u8 *)ellipse);
            eStr[7] = d[len*2+1]; eStr[8] = d[len*2+2];
            eStr[9] = d[len*2+3]; eStr[10] = d[len*2+4];
            eStr[11] = 0; eStr[12] = 0; d[len*2+1] = 0; d[len*2+2] = 0;
        } else {
            u8 *end;
            func_800BE6A4(eStr, (u8 *)ellipse);
            end = d + len;
            func_800BE4F0(eStr, end);
            *end = 0;
        }
        ew = width - object_manager_update(eStr, -1);
        while (w > ew - func_800BDEB0()) {
            len--;
            if (dbl) { d[len*2+1] = 0; d[len*2+2] = 0; } else d[len] = 0;
            if (len == 0) break;
            w = tw_width(d, -1);
        }
        func_800BE4F0(d, eStr);
    }
}
