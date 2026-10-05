/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research: N64 radar traffic callback. Arcade ancestor: game/hud.c
 * AnimateTraffic and Hidden at rushtherock 845329d7b36f5a384c5625ed9a0aef584ab46139.
 * Native changes: one blit per viewed/player pair, horizontal eight-icon atlas,
 * per-view origin, mirror, eligibility, and no direction-angle selection.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Blit {
    char *Name;
    void *Image;
    void *Info;
    s16 TexIndex, X, Y, Z, Width, Height;
    u8 Alpha, Flip;
    s8 Hide, Init;
    s16 Top, Bot, Left, Right;
    s32 field24;
    s32 (*AnimFunc)(struct Blit *);
    u32 AnimID;
} Blit;
typedef struct Car {
    u8 prefix[8];
    f32 dr_pos[3];
    u8 to_matrix[24];
    f32 dr_uvs[3][3];
    u8 to_mode[159];
    s8 mode;
    u8 tail[712];
} Car;
typedef struct Model {
    u8 prefix[1990];
    s16 index, in_game;
    u8 field7ca[2];
    s8 drone_type;
    u8 tail[59];
} Model;
typedef struct {s32 x,y;} Pos;
extern Car player_array[];
extern Model D_8014A250[];
extern s16 D_80151AD0,D_801543CA;
extern s32 state_word_a,D_8011617C,D_80116180;
extern s8 D_80156CE8,D_80140A04;
extern Pos D_80116028[][4];
extern void Input_ApplyPadConfig(Blit *);
extern s32 func_800CF604(s16);
extern void func_800A61B0(f32 *,f32 *,f32 *);
extern void stat_race_update(Blit *,s32,s32,s32);

static s32 Hidden(Blit *blt,s32 hide)
{
    if(hide != blt->Hide) {
        blt->Hide=hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->Hide;
}

s32 func_80108F40(Blit *blt)
{
    s32 view;
    s16 slot;
    f32 rpos[3],lpos[3];
    s32 width,height;
    f32 scale;
    Car *gc,*view_car;
    Model *m;

    view=(blt->AnimID>>8)&15;
    width=blt->Width*0.125f;
    height=blt->Height;
    if(view>=D_80151AD0 || (state_word_a&8) || D_801543CA<2) {
        blt->AnimFunc=0;
        return Hidden(blt,1);
    }
    slot=blt->AnimID&15;
    if(Hidden(blt,!D_80156CE8 || player_array[view].mode==1))
        return 1;
    m=&D_8014A250[slot];
    if(!m->in_game) {
        blt->AnimFunc=0;
        return Hidden(blt,1);
    }
    if(Hidden(blt,slot!=view && !func_800CF604(slot)))
        return 1;
    gc=&player_array[slot];
    view_car=&player_array[view];
    lpos[0]=gc->dr_pos[0]-view_car->dr_pos[0];
    lpos[1]=gc->dr_pos[1]-view_car->dr_pos[1];
    lpos[2]=gc->dr_pos[2]-view_car->dr_pos[2];
    func_800A61B0(lpos,rpos,view_car->dr_uvs[0]);
    blt->X=D_8011617C*0.5f;
    blt->Y=16;
    if(D_80140A04) {
        scale=1.0f/480.0f;
        blt->X+=-rpos[0]*scale*D_8011617C;
    } else {
        scale=1.0f/480.0f;
        blt->X+=rpos[0]*scale*D_8011617C;
    }
    blt->Y+=rpos[2]*-scale*D_80116180;
    if(Hidden(blt,blt->X<width || blt->X>D_8011617C-width ||
                  blt->Y<height || blt->Y>D_80116180-height))
        return 1;
    blt->X+=D_80116028[D_80151AD0-1][view].x;
    blt->Y+=D_80116028[D_80151AD0-1][view].y;
    if(m->drone_type==2)
        stat_race_update(blt,m->index,blt->Width*0.125f,blt->Height);
    else if(m->drone_type==1)
        stat_race_update(blt,5,blt->Width*0.125f,blt->Height);
    blt->X-=width/2;
    blt->Y-=height/2;
    Input_ApplyPadConfig(blt);
    return 1;
}
