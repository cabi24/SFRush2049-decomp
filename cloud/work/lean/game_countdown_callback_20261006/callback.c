/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* HUD countdown text callback; typed from its native field accesses. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Blit {
    s32 texture;
    u32 field04, field08;
    unsigned short field0C;
    s16 field0E, field10;
    unsigned short field12;
    s16 field14, field16;
    u8 field18, field19;
    s8 hidden;
    u8 field1B;
    s16 field1C, field1E, field20, field22;
    s32 field24;
    s32 callback_state;
} Blit;
typedef struct { u8 field00, car; u8 remaining[0x4A]; } PlayerInput;
typedef struct { u8 field00[6]; s8 inactive; u8 remaining[0x2FD]; } CarState;
typedef struct { s32 x, y; } Point;
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_801461D0;
extern s32 state_word_a;
extern s32 D_801170FC;
extern s16 D_80151AD0;
extern s32 D_80118E20, D_80118E24;
extern s32 D_801146AC[];
extern PlayerInput input_rec0[];
extern CarState D_80144030[];
extern Point D_80115FA8[];
extern char **countdown_object;
void Input_ApplyPadConfig(Blit *);
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32);
u32 object_manager_update(char *, s32);
void dispatch_handler(s32);
void state_utility(s16, s16, char *);
static void gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0, 0, 0); }
static s32 font_set(s32 font)
{
    s32 old;
    gfx_lock(); old = slot_state_setup(font); gfx_unlock();
    return old;
}
s32 func_80106B3C(Blit *blit)
{
    s32 player;
    s32 x, y;
    u32 width;
    s32 available;
    if ((state_word_a & 0x400000) != blit->hidden) {
        blit->hidden = state_word_a & 0x400000;
        Input_ApplyPadConfig(blit);
    }
    if (blit->hidden) {
        blit->callback_state = 0;
        return 0;
    }
    D_80118E20 = 1;
    D_80118E24 = 1;
    font_set(11);
    width = object_manager_update(countdown_object[226], -1);
    available = D_80151AD0 >= 3 ? 160 : D_80151AD0 >= 2 ? 240 : 320;
    if ((u32)(available + 8) < width) font_set(12);
    for (player = 0; player < D_80151AD0; player++) {
        if ((state_word_a & 0x200000) && D_801170FC == 0 &&
            D_80144030[input_rec0[player].car].inactive == 0) {
            x = D_80115FA8[(D_80151AD0 - 1) * 4 + player].x;
            y = D_80115FA8[(D_80151AD0 - 1) * 4 + player].y;
            dispatch_handler(D_801146AC[player]);
            state_utility((s16)x, (s16)y, countdown_object[226]);
        }
    }
    D_80118E24 = 3;
    D_80118E20 = 0;
    return 1;
}
