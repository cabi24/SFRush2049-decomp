/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete per-player timer-widget sizing and opacity callback. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u8 r,g,b,a; } Color4;
typedef struct Blit {
    s32 texture;
    Color4 *color;
    u32 field08;
    u16 field0C;
    s16 x,y;
    u16 field12;
    s16 width,height;
    u8 alpha,field19;
    s8 hidden;
    u8 field1B;
    s16 field1C,field1E,field20,field22;
    s32 field24,callback_state,player;
} Blit;
typedef struct { u8 prefix[0xEF]; s8 mode; u8 suffix[0x3B8-0xF0]; } Player952;
typedef struct { s32 x,y; } Point;
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_801461D0;
extern s32 D_801174B4;
extern s8 D_8015723C;
extern s16 D_80151AD0;
extern f32 D_80142740[];
extern Player952 player_array[];
extern Color4 D_80116188[];
extern Point D_80115EA8[];
void Input_ApplyPadConfig(Blit *);
s32 osRecvMesg(OSMesgQueue *,void **,s32);
s32 osJamMesg(OSMesgQueue *,void *,s32);
s32 slot_state_setup(s32);
s32 camera_shake_update(u16);
s8 object_byte9_set(s8);
s16 object_bytes_sum_global(void);
static void gfx_lock(void) { osRecvMesg(&D_801461D0,0,1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0,0,0); }
static s32 font_set(s32 font) {
    s32 old;
    gfx_lock(); old=slot_state_setup(font); gfx_unlock();
    return old;
}
s32 func_80108154(Blit *blit)
{
    s32 player;
    s32 hidden;
    s32 digit_width,colon_width,width;
    s8 old_mode;
    f32 time;
    Color4 *color;
    player = blit->player;
    if (player >= D_80151AD0) {
        blit->callback_state = 0;
        if (blit->hidden != 1) {
            blit->hidden = 1;
            Input_ApplyPadConfig(blit);
        }
        return 1;
    }
    hidden = (D_801174B4 & 8) != 0 || D_8015723C == 0 ||
    D_80142740[player] <= 0.0f || player_array[player].mode == 1;
    if (hidden != blit->hidden) {
        blit->hidden = hidden;
        Input_ApplyPadConfig(blit);
    }
    if (blit->hidden) return 1;
    if (D_80151AD0 == 1) font_set(1);
    else font_set(2);
    digit_width = camera_shake_update(56)+1;
    old_mode = object_byte9_set(0);
    colon_width = camera_shake_update(58)+1;
    object_byte9_set(old_mode);
    time = D_80142740[player];
    color = &D_80116188[player];
    if (time < 3.0f) color->a = (u32)((time*192.0f)/3.0f);
    else color->a = 192;
    width = digit_width*7+colon_width*2;
    blit->x = D_80115EA8[(D_80151AD0-1)*4+player].x-width/2;
    blit->width = width;
    blit->y = D_80115EA8[(D_80151AD0-1)*4+player].y;
    blit->height = object_bytes_sum_global();
    blit->field12 = 100;
    blit->color = color;
    blit->alpha = color->a;
    Input_ApplyPadConfig(blit);
    return 1;
}
