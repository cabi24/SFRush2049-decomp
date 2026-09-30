typedef signed char s8; typedef unsigned char u8; typedef float f32;
typedef struct { f32 f; u8 pad[0x28]; } F2C;
extern f32 D_80111274[];
extern f32 D_80110DD8[];
extern F2C D_801116D4[];
extern f32 D_80124150, D_80124154;
extern s8 D_80143F1C;
typedef struct { u8 p0[8]; u8 b8; u8 p1[2]; s8 b11; u8 p2[2]; s8 b14; } Q;

f32 func_800D0B14(void *arg0)
{
f32 t, u, v;
 t = D_80124150 * (D_80111274[((s8 *) arg0)[0xE]] - D_80124154); return (D_80110DD8[((u8 *) arg0)[8]] + t + (f32) D_80143F1C * D_80124150) * D_801116D4[((s8 *) arg0)[0xB]].f;
}