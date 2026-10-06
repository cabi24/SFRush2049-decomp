/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete per-player warning text callback. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef struct Player952 {
    u8 prefix[0xEF];
    s8 mode;
    u8 middle[0x364-0xF0];
    f32 direction;
    u8 suffix[0x3B8-0x368];
} Player952;
typedef struct { s32 x,y; } Point;
typedef struct OSMesgQueue OSMesgQueue;
extern Player952 player_array[];
extern s8 D_8013F2F8[], D_801613A0;
extern f32 D_801543B8[];
extern f32 D_8002EB90, D_801248C4, D_801248C8, D_8017A630;
extern s32 gameplay_mode, state_word_a, D_80118E20, D_80118E24;
extern s16 D_80151AD0;
extern Point D_80115F28[];
extern void **countdown_object;
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32);
void render_helper(f32);
void func_800ED66C(f32);
void dispatch_handler(s32);
void state_utility(s16,s16,void *);
static void gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0, 0, 0); }
static s32 font_set(s32 font)
{
    s32 old;
    gfx_lock(); old=slot_state_setup(font); gfx_unlock();
    return old;
}
s32 func_80108AB0(void *blit)
{
    s32 player;
    s32 x, y;
    Player952 *car;
    f32 *start_time;
    s8 *warning;
    f32 direction;
    if (gameplay_mode == 5) return 1;
    render_helper(0.0f);
    D_80118E20 = 1;
    D_80118E24 = 1;
    for (player=0,car=player_array,start_time=D_801543B8,warning=D_8013F2F8;
         player<D_80151AD0; player++,car++,start_time++,warning++) {
        direction = car->direction;
        if ((D_801248C4 <= direction && direction <= D_801248C8) ||
            !(state_word_a & 0x400000) || car->mode == 1 ||
            D_801613A0 == 0 || gameplay_mode == 1) {
            *warning = 0;
            *start_time = D_8002EB90;
        } else {
            *warning = 1;
            if (!(D_8002EB90 - *start_time < 2.0f)) {
                if (D_80151AD0 == 1) font_set(10);
                else font_set(11);
                func_800ED66C(D_8017A630 * 255.0f);
                x = D_80115F28[(D_80151AD0-1)*4+player].x;
                y = D_80115F28[(D_80151AD0-1)*4+player].y;
                dispatch_handler(0);
                state_utility((s16)(x+1),(s16)(y+1),countdown_object[26]);
                dispatch_handler(14);
                state_utility((s16)x,(s16)y,countdown_object[26]);
            }
        }
    }
    D_80118E24 = 3;
    D_80118E20 = 0;
    func_800ED66C(-1.0f);
    render_helper(-1.0f);
    return 1;
}
