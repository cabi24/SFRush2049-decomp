/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image A:803A3A6C..803A3D9C; full select-screen marker color callback.
 * Native-authoritative reconstruction. Projection-reaching descriptors must
 * identify player 0..3 and marker 7..9. Earlier hide-only exits are wider.
 * This is a 64-byte slot view, not a claim of recovered original N64 types.
 * Each player has a native 2816-byte stride, 44 slots. Projection observes
 * three floats at slot +19, float +12; visibility is +8/+60 in this slot.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Blit {
    char *Name; void *Image; void *Info;
    s16 TexIndex, X, Y; u16 State; s16 Width, Height;
    u8 Alpha, Flip; s8 Hide; u8 Init;
    s16 Top, Bot, Left, Right, Color; u16 reserved26;
    s32 (*AnimFunc)(struct Blit *); u32 AnimID;
} Blit;
typedef union MenuSlot {
    float matrix[16];
    struct { float x,y,depth; u8 rest[48]; u8 alpha; u8 tail[3]; } state;
} MenuSlot;
typedef struct Color { u8 r,g,b,a; } Color;
typedef struct ProjectContext {
    float rotation[3][3], position[3]; u8 opaque[104];
} ProjectContext;
extern s16 D_8014A108;
extern s8 D_803BA028[], D_803B9FD0[];
extern MenuSlot D_803AF9A8[][44];
extern float D_803B97A0, D_803B97A4;
extern ProjectContext D_80150B70[];
extern s8 D_80111611[][13],D_80111655[][13],D_80111699[][13];
extern Color D_803B9B60[][3],D_801226C0[];
extern s8 input_new_data_wrapper(Blit *,s32);
extern void brake_light_update(s32,float *,ProjectContext *,void *,s16 *);
extern void Input_ApplyPadConfig(Blit *);
s32 func_803A3A6C(Blit *blt)
{
    s32 player=blt->AnimID&15;
    s32 marker=(blt->AnimID>>16)&255;
    MenuSlot *slot;
    s32 palette;
    s16 position[2];
    s32 channel;

    if (player>=D_8014A108) {
        input_new_data_wrapper(blt,1);
        blt->AnimFunc=0;
        return 1;
    }
    if (D_803BA028[player]) {
        input_new_data_wrapper(blt,1);
        return 1;
    }
    slot=&D_803AF9A8[player][marker];
    if (input_new_data_wrapper(blt,
        (D_803B97A0<slot->state.depth && slot->state.depth<D_803B97A4) || !slot->state.alpha))
        return 1;
    brake_light_update(player,&slot[19].matrix[12],&D_80150B70[player],0,position);
    blt->X=position[0]-blt->Width/2;
    blt->Y=position[1]-blt->Height/2;
    blt->Alpha=slot->state.alpha;
    if (marker==7) {
        palette=D_80111611[player][D_803B9FD0[player]];
        channel=0;
    } else if (marker==8) {
        palette=D_80111655[player][D_803B9FD0[player]];
        channel=1;
    } else if (marker==9) {
        palette=D_80111699[player][D_803B9FD0[player]];
        channel=2;
    }
    D_803B9B60[player][channel].r=D_801226C0[palette].r;
    D_803B9B60[player][channel].g=D_801226C0[palette].g;
    D_803B9B60[player][channel].b=D_801226C0[palette].b;
    D_803B9B60[player][channel].a=blt->Alpha;
    blt->Image=&D_803B9B60[player][channel];
    blt->AnimID=(blt->AnimID&0x0fffffff)|((u32)palette<<28);
    Input_ApplyPadConfig(blt);
    return 1;
}
