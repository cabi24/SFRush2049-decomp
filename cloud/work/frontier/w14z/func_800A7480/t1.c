/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;

/* 72-byte record; only the fields written by func_800A7480 are named. */
typedef struct {
    u8 opaque[64];
    s16 x;      /* 0x40 */
    s16 y;      /* 0x42 */
    u8 r;       /* 0x44 */
    u8 g;       /* 0x45 */
    u8 b;       /* 0x46 */
    u8 a;       /* 0x47 */
} Descriptor72;

extern Descriptor72 D_8017A510[];
extern u32 D_80124FC8;

void func_800A7480(s16 x, s16 y, /*@{rt*/u8 r, u8 g/*@| s8 r, s8 g @}*/, u8 b, u8 a, s32 index)
{
    /*@{dc*/Descriptor72 *e;
    u16 px;

    e = &D_8017A510[index];/*@| Descriptor72 *e;
    u16 px;

    e = (Descriptor72 *)(u32)&D_8017A510[index]; @}*/
    /*@{st*/e->x = x;
    e->y = y;
    e->r = r;
    e->g = g;
    e->b = b;
    e->a = a;/*@| e->y = y;
    e->x = x;
    e->r = r;
    e->g = g;
    e->b = b;
    e->a = a; @}*/
    /*@{pk*/px = ((r << 8) & 0xf800) | ((g << 3) & 0x7c0) | ((b >> 2) & 0x3e) | 1;/*@| px = ((r << 8) & 0xf800) | ((g << 3) & 0x7c0) | ((b >> 2) & 0x3e) | 1; @}*/
    /*@{ot*/D_80124FC8 = ((u32)px << 16) | px;/*@| D_80124FC8 = ((u32)px << 16) | (u32)px; @}*/
}
