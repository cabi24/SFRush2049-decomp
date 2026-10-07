typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern s32 slot_state_setup(s32);
extern u32 D_801174B4;
extern s32 D_8014A110;
extern s8 D_8016137C;
extern s16 D_80151AD0;
extern s32 D_80118E20[2];
extern s16 D_80152734;
extern u8 D_801461D0[];

typedef struct { u8 pad[2024]; s8 side; u8 pad2[31]; } Car;
typedef struct { u8 pad[0xEF]; s8 state; u8 pad2[0x3B8 - 0xF0]; } Racer;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 pad[0x338]; u8 *format; } TextBank;
typedef struct { s32 a; TextBank *text; } TextBankRef;

extern Car D_8014A250[];
extern Racer D_80152818[];
extern Pos D_80115DA8[][4];
extern TextBankRef D_8017A4E0;

extern void render_helper(f32);
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osJamMesg(void *, void *, s32);
extern void dispatch_handler(s32);
extern void music_tempo_adjust(s16, s16, u8 *, ...);

static void gfx_lock(void) { osRecvMesg(D_801461D0, 0, 1); }
static void gfx_unlock(void) { osJamMesg(D_801461D0, 0, 0); }
static s32 font_set(s32 font)
{
    s32 old;
    gfx_lock(); old = slot_state_setup(font); gfx_unlock();
    return old;
}

s32 func_80107EDC(s32 arg0)
{
    s32 i;
    s32 x, y;

    if ((D_801174B4 & 8) || !(D_801174B4 & 0x400000) || D_8014A110 == 1 ||
        D_8014A110 == 5 || D_8016137C == 0) {
        return 1;
    }
    render_helper(0.0f);
    D_80118E20[1] = 3;
    D_80118E20[0] = 1;
    if (D_80151AD0 == 1) {
        font_set(11);
    } else {
        font_set(12);
    }
    for (i = 0; i < D_80151AD0; i++) {
        if (D_80152734 == D_8014A250[i].side) {
            continue;
        }
        if (D_80152818[i].state == 1) {
            continue;
        }
        x = D_80115DA8[D_80151AD0 - 1][i].x;
        y = D_80115DA8[D_80151AD0 - 1][i].y;
        dispatch_handler(0);
        music_tempo_adjust(x + 1, y + 1, D_8017A4E0.text->format, D_8014A250[i].side + 1, D_80152734);
        dispatch_handler(1);
        music_tempo_adjust(x, y, D_8017A4E0.text->format, D_8014A250[i].side + 1, D_80152734);
    }
    D_80118E20[1] = 3;
    D_80118E20[0] = 0;
    render_helper(-1.0f);
    return 1;
}


