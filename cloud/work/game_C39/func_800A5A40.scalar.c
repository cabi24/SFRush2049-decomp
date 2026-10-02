/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef int s32;
typedef float f32;
typedef struct Viewport {s16 scale[4], translate[4];} Viewport;
typedef struct Record {void *first; Viewport *bounds; u8 rest[64];} Record;
extern s32 D_8002AFC0, D_8002AFC4;
extern u16 D_8002AFC2, D_8002AFC6;
extern u8 D_80146204, D_8017A63C;
extern Viewport D_8011EA30, D_80149870;
extern void *D_80154188;
extern Record D_8017A510;
extern void arb_rate_set(s32 index, void *first, Viewport *bounds, void *matrix, f32 z, f32 width, f32 height, f32 halfWidth, f32 halfHeight);
void func_800A5A40(void) {
    u16 width, height;
    D_80146204 = 1;
    D_8017A63C = 0;
    arb_rate_set(0, &D_8011EA30, &D_80149870, D_80154188, 0.0f, (f32)D_8002AFC0, (f32)D_8002AFC4, (f32)(D_8002AFC0 / 2), (f32)(D_8002AFC4 / 2));
    height = D_8002AFC6;
    width = D_8002AFC2;
    D_80149870.scale[0] = 0;
    D_80149870.scale[1] = 0;
    D_8017A510.first = &D_8011EA30;
    D_8017A510.bounds = &D_80149870;
    D_80149870.scale[3] = height;
    D_80149870.scale[2] = width;
}
