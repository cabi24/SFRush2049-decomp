/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;
typedef signed short s16;typedef unsigned short u16;
typedef signed int s32;typedef unsigned int u32;
typedef struct Rgba8 {u8 r,g,b,a;} Rgba8;
typedef struct ResourceRecord8 {u8 bytes[8];} ResourceRecord8;
typedef struct ResourceBank8 {u8 *base;u32 count;} ResourceBank8;
extern Rgba8 D_801226C0[];
extern u32 D_8013F300[];
extern u8 D_80138664[],D_80139320[],D_8013FE90[],D_80140BDC;
extern u32 state_word_a;
extern ResourceRecord8 D_80153E88[];
extern s16 D_80142B08[];
extern u16 D_801427C0[],D_801428F8[],D_80161368[];
extern u8 *D_8011B3A0[];
extern void *D_80143F68[];
extern ResourceBank8 D_80151AE8[];
void *memset(void *,s32,u32);
void func_80391470(void);
void sfx_volume_set(s16,s16,s32);
void *func_800B24EC(u8 *,u16 *,s32,s8,s32);
s32 string_copy_format(u8 *,s32,s8,s32);
void sfx_position_3d(s32 reset)
{
    Rgba8 *color;
    u32 *packed;
    s32 i;
    u16 *half;
    void **output;
    u8 **names;
    u16 resource;
    color=D_801226C0;
    packed=D_8013F300;
    do {
        *packed++=(u16)(((color->b>>2)&0x3e)|((color->r<<8)&0xf800)|((color->g<<3)&0x7c0))|1;
        color++;
    } while(color<D_801226C0+32);
    D_80138664[0]=11;
    D_80138664[1]=5;
    D_80138664[2]=26;
    D_80138664[3]=16;
    if(reset) {
        memset(D_80139320,0,3328);
        if(state_word_a&0x100) {
            func_80391470();
            half=D_801427C0;
            do {
                half[1]=0;
                half[2]=0;
                half[3]=0;
                half[0]=0;
                half+=4;
            } while(half!=D_801427C0+156);
        } else {
            for(i=0;i<13;i++) {
                sfx_volume_set((s16)i,D_80153E88[i].bytes[1],D_80142B08[i]);
                D_8013FE90[i]=0;
            }
        }
        names=D_8011B3A0;
        output=D_80143F68;
        do {
            if(func_800B24EC(*names,&resource,0,(s8)(D_80140BDC-1),0))
                *output=D_80151AE8[resource>>10].base+(resource&0x3ff)*36;
            else *output=0;
            output++;
            names++;
        } while(output<D_80143F68+24);
        half=D_801428F8;
        do {
            *half++=string_copy_format(*names++,0,(s8)(D_80140BDC-1),1);
        } while(half!=D_801428F8+1);
        half=D_80161368;
        do {
            func_800B24EC(*names++,half,0,(s8)(D_80140BDC-1),0);
            half++;
        } while(half!=D_80161368+10);
    }
}
