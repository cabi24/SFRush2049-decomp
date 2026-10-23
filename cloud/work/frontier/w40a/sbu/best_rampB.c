/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * sound_bank_unload -- NOT A MATCH (w3b best: 134/312 words in the whole-program unit,
 * exact length 312 and exact frame 192). Car palette builder (see
 * cloud/work/ipa-groups/dot_palette_generation_20261005/README.md for semantics); context for the
 * provisional internal interpolator func_800B0EA0, which is EQUAL here.
 * w3b changes against palette.c (262/312, 512 words emitted):
 *  - the two model-record stores go through func_800B0F60(id, descriptor), a deleted static
 *    inlined at both sites (its retail remains are the stub at 0x800B0F60, reproduced): gives the
 *    retail a0/a1/a2/a3/t0 colouring of the tail;
 *  - an unused `u16 *p = &colors[i];` in each brightening loop: the bigger ucode body makes uopt
 *    unroll by 2 as retail instead of 4 (312 words emitted). Not believed original: the real
 *    source probably has a bigger per-element body (see RESULTS.md);
 *  - `s32 pad[10]` after `handle`: frame 192 with name/colors/handle at 160/156/154 as retail.
 * Residual: ugen temp-ring phase in the brightening loop (first divergence at the `or` of
 * element 1), colouring of the ramp variables (retail ratio t0, cursor a3, colors t3, third
 * colour t4; ours a3, t0, t4, t3), descriptor spill slot 48 vs 44.
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
extern u8 D_8013FE90[];
extern u32 D_8013F300[];
extern Resource24 D_8013FEE0[];
extern Handles64 D_80139320[];
extern Model68 D_8012E700[];
extern volatile u8 D_80140BDC;
extern char D_80123498[];
extern s32 D_80152770;
extern void *audio_dma_sync(void *,u32);
extern Resource24 *sound_bank_load(char *,u16 *,s8,s8,s32);
extern s32 sprintf(char *,char *,...);
extern void *memcpy(void *,const void *,u32);
extern s32 osRecvMesg(void *,void **,s32);
extern s32 osJamMesg(void *,void *,s32);
extern void audio_reverb_update(u32,s32);
void func_80095CF4(void);   /* _GetSemaphore@memsys.c */
void func_80095CFC(void);   /* _ReleaseSemaphore@memsys.c */
void DeallocateMemory_SCC(u32 address);

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


void SetTexPal_SCC(s32 id, Resource24 *descriptor)
{
    D_8012E700[id].palette=descriptor;
}



static u16 *func_800B0F60(u16 *colors, u32 first_color, u32 second_color, u32 third_color)
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
    u16 *cursor;
    s32 i, ratio, value;
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
    DeallocateMemory_SCC((u32)colors);
    SetTexPal_SCC(D_80139320[index].root.halves.id,descriptor);
    SetTexPal_SCC(D_80139320[index].secondary.halves.id,descriptor);
}


void DeallocateMemory_SCC(u32 address)     /* MAT3 DeallocateMemory@memsys.c */
{
    func_80095CF4();
    audio_reverb_update(address, 0);
    func_80095CFC();
}
