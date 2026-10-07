/* DIAGNOSTIC ONLY (w13f): the inlined helper body is changed to `color * 0x10001`, which is NOT the locked func_800A5560 body. Removes the coloured v1 web (17 -> 14 words); not a source claim. */
/* flags: -g0 -O3 -mips2 -G 0 -non_shared -- NOT A MATCH: 17/34 words (score.py fn); see ../RESULTS.md */
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Descriptor72 {
    void *first, *bounds;
    s32 orthographic;
    f32 horizontal, vertical, tan_horizontal, tan_vertical, inverse_horizontal, inverse_vertical;
    f32 width, height, near_plane, far_plane, aspect, zoom, fog;
    s16 range_start, range_end;
    u8 red, green, blue, alpha;
} Descriptor72;
extern Descriptor72 D_8017A510[];
extern u32 D_80124FC8;

#define GPACK_RGBA5551(r, g, b, a) ((((r) << 8) & 0xf800) | (((g) << 3) & 0x7c0) | (((b) >> 2) & 0x3e) | ((a) & 1))

void func_800A5560(u16 color)
{
    D_80124FC8 = color * 0x10001;
}

void func_800A7480(s16 range_start, s16 range_end, s8 red, u8 green, u8 blue, u8 alpha, s32 index)
{
    Descriptor72 *d;

    d = (Descriptor72 *)(u32)&D_8017A510[index];
    d->range_start = range_start;
    d->range_end = range_end;
    d->red = red;
    d->green = green;
    d->blue = blue;
    d->alpha = alpha;
    func_800A5560(GPACK_RGBA5551(red, green, blue, 1));
}
