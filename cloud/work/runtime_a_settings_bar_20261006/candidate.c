/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image A:803AE63C..803AE940, complete projected settings-bar callback.
 * Native-authoritative 64-byte slot views; the accessed record is slot+21.
 * BLIT callback contract is witnessed by NewMultiBlit and UpdateActiveObjects.
 * No exact arcade donor is claimed. */
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
extern MenuSlot D_803B6B14[];
extern float D_803B99D4, D_803B99D8, D_803B99DC;
extern ProjectContext D_80150B70[];
extern s8 D_8013F1D8, D_80142726, D_80152030, D_80150EFC;
extern s8 input_new_data_wrapper(Blit *,s32);
extern void brake_light_update(s32,float *,ProjectContext *,void *,s16 *);
extern void Input_ApplyPadConfig(Blit *);
s32 func_803AE63C(Blit *blt)
{
    s32 segment=blt->AnimID&15;
    s32 marker=blt->AnimID>>16;
    s16 position[2];
    MenuSlot *slot=&D_803B6B14[marker];

    if (input_new_data_wrapper(blt,
        (D_803B99D4<slot[21].state.depth && slot[21].state.depth<D_803B99D8)
        || !slot[21].state.alpha))
        return 1;
    brake_light_update(0,&slot[21].matrix[12],&D_80150B70[0],0,position);
    blt->X=position[0]-blt->Width/2;
    blt->Y=position[1]-(blt->Height/4)/2;
    blt->Top=0;
    blt->Bot=blt->Height/4-1;
    blt->Left=0;
    blt->Right=blt->Width-1;
    blt->Alpha=slot[21].state.alpha;
    if (blt->Alpha==255)
        blt->Alpha=254;
    if (D_803B99DC<slot[21].state.depth) {
        blt->Top+=blt->Height/2;
        blt->Bot+=blt->Height/2;
    }
    if (segment==1) {
        blt->Top+=blt->Height/4;
        blt->Bot+=blt->Height/4;
        switch (marker) {
        case 14: blt->Right=(blt->Width-1)*D_8013F1D8/3; break;
        case 15: blt->Right=(blt->Width-1)*D_80142726/4; break;
        case 18: blt->Right=(blt->Width-1)*D_80152030/5; break;
        case 19: blt->Right=(blt->Width-1)*D_80150EFC/2; break;
        }
    }
    Input_ApplyPadConfig(blt);
    return 1;
}
