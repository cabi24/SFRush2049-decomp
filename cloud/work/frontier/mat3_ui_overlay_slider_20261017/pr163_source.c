/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete image-A A3DA4 projected gauge callback, 912 bytes.
 * Standalone source. No caller, inlined helper, deleted-static stub or own data.
 * Direct indexed slot reads and quarter-height expressions avoid declared
 * caches; canonical IDO common-subexpression elimination preserves the native
 * derived address and height lifetimes without artificial local storage.
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
typedef struct ProjectContext {
    float rotation[3][3], position[3]; u8 opaque[104];
} ProjectContext;
extern s16 D_8014A108;
extern s8 D_803BA028[], D_803B9FD0[];
extern MenuSlot D_803AF9A8[][44];
extern float D_803B97A8, D_803B97AC, D_803B97B0, D_803B97B4, D_803B97B8;
extern float D_80111310[][13], D_80111414[][13];
extern ProjectContext D_80150B70[];
extern s8 input_new_data_wrapper(Blit *,s32);
extern void brake_light_update(s32,float *,ProjectContext *,void *,s16 *);
extern void Input_ApplyPadConfig(Blit *);
s32 func_803A3DA4(Blit *blt)
{
    s32 segment=blt->AnimID&15;
    s32 player=(blt->AnimID>>4)&15;
    s32 marker=blt->AnimID>>16;
    s16 position[2];
    if (player>=D_8014A108) {
        input_new_data_wrapper(blt,1);
        blt->AnimFunc=0;
        return 1;
    }
    if (D_803BA028[player]) {
        input_new_data_wrapper(blt,1);
        return 1;
    }
    if (input_new_data_wrapper(blt,
        (D_803B97A8<D_803AF9A8[player][marker].state.depth && D_803AF9A8[player][marker].state.depth<D_803B97AC) || !D_803AF9A8[player][marker].state.alpha))
        return 1;
    brake_light_update(player,&D_803AF9A8[player][marker+19].matrix[12],&D_80150B70[player],0,position);
    blt->X=position[0]-blt->Width/2;
    blt->Y=position[1]-(blt->Height/4)/2;
    blt->Left=0;
    blt->Right=blt->Width-1;
    blt->Top=0;
    blt->Bot=(blt->Height/4)-1;
    blt->Alpha=D_803AF9A8[player][marker].state.alpha;
    if (D_803B97B0 < D_803AF9A8[player][marker].state.depth) {
        blt->Top += blt->Height/2;
        blt->Bot += blt->Height/2;
    }
    if (segment==1) {
        blt->Top+=(blt->Height/4);
        blt->Bot+=(blt->Height/4);
        switch (marker) {
        case 13:
            blt->Right=(D_80111310[player][D_803B9FD0[player]]-.75f)/D_803B97B4*(blt->Width-1)/15.0f;
            break;
        case 14:
            blt->Right=(D_80111414[player][D_803B9FD0[player]]-.75f)/D_803B97B8*(blt->Width-1)/15.0f;
            break;
        }
    }
    Input_ApplyPadConfig(blt);
    return 1;
}
