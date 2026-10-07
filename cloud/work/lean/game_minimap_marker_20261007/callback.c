/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Blit {
    u8 field00[0xE];
    s16 x, y;
    u8 field12[2];
    s16 width, height;
    u8 alpha, mirrored;
    s8 hidden;
    u8 field1B[0x2C - 0x1B];
    u32 frame;
    s32 timer;
} Blit;
typedef struct { s16 x, y, z; } Bound;
typedef struct { s32 x, y; } Point;
typedef struct {
    s32 field00;
    s16 selected;
    u8 field06[6];
    f32 x, y, z;
    u8 remaining[0x50 - 0x18];
} MapRecord;
extern s16 D_80151AD0;
extern s8 D_80152014, D_80156BDC, D_80140A04;
extern s32 gameplay_mode, D_801161C4, D_801170FC;
extern u32 state_word_a;
extern Bound D_801407B4, D_801407D4;
extern MapRecord D_80151CE8[];
extern Point D_801160A8[];
extern s32 D_80118E20[2];
extern f32 D_8002EB94;
extern u8 D_801461D0[], D_80120E48[], D_80120E4C[];
extern void Input_ApplyPadConfig(Blit *);
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osJamMesg(void *, void *, s32);
extern s32 slot_state_setup(s32);
extern void render_helper(f32);
extern void dispatch_handler(s32);
extern void music_tempo_adjust(s16, s16, u8 *, ...);
extern void stat_race_update(Blit *, u32, s16, s16);
static void gfx_lock(void) { osRecvMesg(D_801461D0, 0, 1); }
static void gfx_unlock(void) { osJamMesg(D_801461D0, 0, 0); }
static s32 font_set(s32 font)
{
    s32 old;
    gfx_lock(); old = slot_state_setup(font); gfx_unlock();
    return old;
}
s32 func_80109F54(Blit *blit)
{
    s32 x, y;
    s32 map_width, map_height;
    s32 width, height;
    s8 hidden;
    MapRecord *record;

    hidden = D_80151AD0 >= 5 || D_80152014 >= 2 || gameplay_mode == 6 ||
             gameplay_mode == 4 || gameplay_mode == 5 || D_80156BDC == 0;
    if (hidden != blit->hidden) {
        blit->hidden = hidden;
        Input_ApplyPadConfig(blit);
    }
    if (blit->hidden) {
        return 1;
    }
    map_width = D_801407B4.x - D_801407D4.x;
    map_height = D_801407B4.z - D_801407D4.z;
    if (map_height < map_width) {
        width = D_801161C4 - 8;
        height = map_height * width / map_width;
    } else {
        height = D_801161C4 - 8;
        width = map_width * height / map_height;
    }
    x = D_801160A8[D_80151AD0 - 1].x + ((D_801161C4 - width - 8) / 2 + 4) - D_801161C4 / 2;
    y = D_801160A8[D_80151AD0 - 1].y + ((D_801161C4 - height - 8) / 2 + 4) - D_801161C4 / 2;
    if (D_80140A04) {
        record = &D_80151CE8[D_80151CE8[0].selected];
        x += (record->x - D_801407D4.x) * width / map_width;
    } else {
        record = &D_80151CE8[D_80151CE8[0].selected];
        x += (width - 1) - (record->x - D_801407D4.x) * width / map_width;
    }
    y += (record->z - D_801407D4.z) * height / map_height;
    blit->x = x - blit->width / 2;
    blit->y = y - blit->width / 2;
    hidden = gameplay_mode == 1 || D_80152014 >= 2 || (state_word_a & 8) != 0;
    if (hidden != blit->hidden) {
        blit->hidden = hidden;
        Input_ApplyPadConfig(blit);
    }
    if (blit->hidden) {
        if (gameplay_mode != 1 && !(state_word_a & 8)) {
            if (D_80151AD0 < 4) {
                font_set(6);
            } else {
                font_set(7);
            }
            render_helper(0.0f);
            D_80118E20[0] = 1;
            D_80118E20[1] = 1;
            dispatch_handler(0);
            music_tempo_adjust((s16)(x + 1), (s16)(y + 1), D_80120E48, D_80152014);
            dispatch_handler(11);
            music_tempo_adjust((s16)x, (s16)y, D_80120E4C, D_80152014);
            D_80118E20[1] = 3;
            D_80118E20[0] = 0;
            render_helper(-1.0f);
        }
    } else {
        if (D_80140A04) {
            blit->mirrored = 1;
        }
        blit->alpha = 128;
        if (D_801170FC == 0) {
            blit->timer -= D_8002EB94 * 1000.0f;
        }
        if (blit->timer <= 0) {
            blit->timer = 100;
            blit->frame = (blit->frame + 1) % 5U;
        }
        stat_race_update(blit, blit->frame, blit->width, blit->width);
    }
    return 1;
}
