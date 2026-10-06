/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Countdown panel callback; typed from native field accesses. */
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
extern PlayerInput input_rec0[];
extern CarState D_80144030[];
extern Point D_80115FA8[];
extern u8 **countdown_object;
void Input_ApplyPadConfig(Blit *);
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32);
s32 object_manager_update(u8 *, s16);
static void gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0, 0, 0); }
static s32 font_set(s32 font)
{
    s32 old;
    gfx_lock(); old = slot_state_setup(font); gfx_unlock();
    return old;
}
typedef struct FiveParts FiveParts;
typedef struct { u8 r, g, b, a; } Color4;
typedef struct { Color4 first, second; } ColorPair;
extern ColorPair D_80116198[];
extern FiveParts *D_80154350[];
s16 object_bytes_sum_global(void);
FiveParts *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
void tournament_unlock_check(FiveParts *, Color4 *, Color4 *);
void sound_handles_array_clear(FiveParts *);
s32 func_80106874(Blit *blit)
{
    s32 width, height;
    s32 available;
    s32 player;
    s32 x, y;
    FiveParts **panel;
    font_set(11);
    width = object_manager_update(countdown_object[226], -1);
    available = D_80151AD0 >= 3 ? 160 : D_80151AD0 >= 2 ? 240 : 320;
    if (available + 8 < width) {
        font_set(12);
        width = object_manager_update(countdown_object[226], -1);
    }
    height = object_bytes_sum_global();
    for (player = 0, panel = D_80154350; player < D_80151AD0; player++, panel++) {
        if ((state_word_a & 0x200000) && D_801170FC == 0 &&
            D_80144030[input_rec0[player].car].inactive == 0) {
            if (*panel == 0) {
                x = D_80115FA8[(D_80151AD0 - 1) * 4 + player].x;
                y = D_80115FA8[(D_80151AD0 - 1) * 4 + player].y;
                *panel = ambient_sound_set(x-width/2-8, y-height/2-4,
                                          width/2+x+8, height/2+y+4,
                                          255, 0, player, 1);
                tournament_unlock_check(*panel, &D_80116198[player].first,
                                        &D_80116198[player].second);
            }
        } else if (*panel != 0) {
            sound_handles_array_clear(*panel);
            *panel = 0;
        }
    }
    if (state_word_a & 0x400000) {
        blit->callback_state = 0;
        if (blit->hidden != 1) {
            blit->hidden = 1;
            Input_ApplyPadConfig(blit);
        }
        return 0;
    }
    return 1;
}
