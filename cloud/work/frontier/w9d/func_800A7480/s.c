/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
    D_80124FC8 = (color << 16) | color;
}

void func_800A7480(s16 range_start, s16 range_end, u8 red, s8 green, u8 blue, u8 alpha, s32 index)
{
    D_8017A510[index].range_start = range_start;
    D_8017A510[index].range_end = range_end;
    D_8017A510[index].red = red;
    D_8017A510[index].green = green;
    D_8017A510[index].blue = blue;
    D_8017A510[index].alpha = alpha;
    func_800A5560(GPACK_RGBA5551(red, green, blue, 1));
}
