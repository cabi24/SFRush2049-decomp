/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * sound_bank_unload (0x800B10D4, 1248 B; historical label) = N64 SetupCarColor: builds a car's 128-entry RGBA5551
 * palette and installs it on the car's two models. Semantics: cloud/work/ipa-groups/dot_palette_generation_20261005/
 * README.md. w40a (wave 40); earlier drafts w3b/w11d (134-135/312 words).
 *   1. mark the slot's paint job inited, allocate 512 bytes, load the palette texture named by `palette` and copy it;
 *   2. first == 0: brighten entries 1..31 (red channel x8 x1.25, clamp 255, repack grey); else second == 0: 33..63;
 *   3. three colour-table entries -> endpoints 32/64/96 and ramps 33..63, 65..95, 97..126 toward black (alpha set),
 *      through the internal interpolator func_800B0EA0 (MakeRamps = func_800B0F60, deleted after inlining);
 *   4. copy into the slot's palette descriptor, free the buffer (heap lock / internal audio_reverb_update(a1, a2) /
 *      unlock), point the two models' palette at the descriptor.
 * Structure facts (each one measured, see w40a RESULTS.md):
 *  - func_800B0EA0's parameter order is (left, right, ratio). IPA still passes ratio in a0 (the callee's code is
 *    identical for every order) but the call's argument order puts `move a0,t0` in the jal delay slot as retail.
 *  - The ramps are the single-site helper func_800B0F60 (its retail stub sits right after func_800B0EA0); brightening
 *    is written in place. A brightening helper makes `colors` the highest-priority web (its in-loop uses count) and
 *    breaks the t1/t2/t3/t4 colouring of first/second/colors/third; in place plus the ramp helper gives retail's.
 *  - MakeRamps returns its palette pointer (unused by the caller): the extra occurrence of `colors` puts its priority
 *    below second_color's (44/11 = 4.0 tie -> 45/12), so colors takes t3 as retail.
 *  - Separate r/g/b channel statements: the bigger loop body makes uopt unroll the brightening loops by 2 (not 4).
 *    `(u16)(r | g | b) | 1`: ugen emits the u16 mask (one temp-ring pop) that as1 then deletes; this is retail's
 *    t7/t9 temp phase in every brightened element.
 *  - second/third colour loads on one source line: as1 breaks the tie on line number and schedules the third load
 *    before the second, as retail.
 *  - Heap lock/unlock are the memsys helpers func_80095CF4/func_80095CFC called in place (both inlined, 8 frame bytes
 *    each), and SetModelPalette keeps a `Model68 *` local: together they make the inline area end at 140 so the
 *    descriptor's spill temp is sp+48 with frame 192.
 * No unused locals, no compiled-out tests.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct Resource24 { u8 directory_fields[20]; u16 *data; } Resource24;
typedef union Handle { s32 value; struct { s16 bank, id; } halves; } Handle;
typedef struct Handles64 { Handle root; u32 unknown04[3]; Handle secondary, body; u32 unknown18[10]; } Handles64;
typedef struct Model68 { u8 model_fields[56]; Resource24 *palette; u32 texture0, texture1; } Model68;
extern u8 D_8013FE90[];          /* gCarPaintJobInited */
extern u32 D_8013F300[];         /* paint colour table */
extern Resource24 D_8013FEE0[];  /* per-slot palette descriptors */
extern Handles64 D_80139320[];   /* per-slot model handles */
extern Model68 D_8012E700[];     /* models */
extern volatile u8 D_80140BDC;   /* gNumModels */
extern char D_80123498[];        /* palette texture name format */
extern void *audio_dma_sync(void *, u32);                          /* AllocateMemory */
extern Resource24 *sound_bank_load(char *, u16 *, s8, s8, s32);    /* texture lookup */
extern s32 sprintf(char *, char *, ...);
extern void *memcpy(void *, const void *, u32);
extern void audio_reverb_update(u32, s32);                         /* _DeallocateMemory (internal, IPA a1/a2) */
void func_80095CF4(void);   /* _GetSemaphore@memsys.c */
void func_80095CFC(void);   /* _ReleaseSemaphore@memsys.c */

u32 func_800B0EA0(u32 left, u32 right, s32 ratio)
{
    u32 inverse, red, green, blue;
    if (ratio == 0) return left;
    if (ratio >= 255) return right;
    inverse = 255U - (u32)ratio;
    red = ((s32)((left & 0xF800) * inverse + (right & 0xF800) * (u32)ratio) >> 8) & 0xF800;
    green = ((s32)((left & 0x7C0) * inverse + (right & 0x7C0) * (u32)ratio) >> 8) & 0x7C0;
    blue = ((s32)((left & 0x3E) * inverse + (right & 0x3E) * (u32)ratio) >> 8) & 0x3E;
    return red | green | blue | 1;
}


void SetModelPalette(s32 id, Resource24 *descriptor)
{
    Model68 *m;
    m = &D_8012E700[id];
    m->palette = descriptor;
}



u16 *func_800B0F60(u16 *colors, u32 first_color, u32 second_color, u32 third_color)   /* MakeRamps */
{
    s32 i;

    colors[32]=first_color;
    for(i=33;i<64;i++) {
        colors[i]=func_800B0EA0(first_color, 1,(i-33)*8);
    }
    colors[64]=second_color;
    for(i=65;i<96;i++) {
        colors[i]=func_800B0EA0(second_color, 1,(i-65)*8);
    }
    colors[96]=third_color;
    for(i=97;i<127;i++) {
        colors[i]=func_800B0EA0(third_color, 1,(i-97)*8);
    }
    return colors;
}

void sound_bank_unload(s16 index,s16 palette,u8 first,u8 second,u8 third)
{
    char name[32];
    u16 *colors;
    u16 handle;
    s32 i, value;
    s32 r, g, b;
    u32 first_color, second_color, third_color;
    Resource24 *descriptor;
    D_8013FE90[index]=1;
    colors=audio_dma_sync(0,512);
    sprintf(name,D_80123498,palette+1);
    memcpy(colors,sound_bank_load(name,&handle,0,D_80140BDC-1,1)->data,512);
    if (first == 0) {
        for (i = 1; i < 32; i++) {
            value = (s32)((((colors[i] >> 11) & 0x7f) << 3) * 1.25f);
            if (value > 255) value = 255;
            r = (value << 8) & 0xf800;
            g = (value << 3) & 0x7c0;
            b = (value >> 2) & 0x3e;
            colors[i] = (u16)(r | g | b) | 1;
        }
    } else if (second == 0) {
        for (i = 33; i < 64; i++) {
            value = (s32)((((colors[i] >> 11) & 0x7f) << 3) * 1.25f);
            if (value > 255) value = 255;
            r = (value << 8) & 0xf800;
            g = (value << 3) & 0x7c0;
            b = (value >> 2) & 0x3e;
            colors[i] = (u16)(r | g | b) | 1;
        }
    }
    first_color=D_8013F300[first];
    second_color=D_8013F300[second]; third_color=D_8013F300[third];
    func_800B0F60(colors, first_color, second_color, third_color);
    descriptor=&D_8013FEE0[index];
    memcpy(descriptor->data,colors,512);
    func_80095CF4();
    audio_reverb_update((u32)colors, 0);
    func_80095CFC();
    SetModelPalette(D_80139320[index].root.halves.id,descriptor);
    SetModelPalette(D_80139320[index].secondary.halves.id,descriptor);
}
