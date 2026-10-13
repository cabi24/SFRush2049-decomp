/*
 * func_80107EDC (0x80107EDC, 632 B): per-window lap counter text (HUD). Shown unless D_801174B4 bit 3,
 * when bit 22 is set, the race state D_8014A110 is not 1/5 and D_8016137C is set; font 11 (one window) or
 * 12; for each window whose lap D_8014A250[i].lap (+2024, stride 2056) differs from D_80152734 and whose
 * car D_80152818[i].f239 != 1: drop-shadowed text (colour 0 at x+1,y+1 then colour 1) of the format at
 * StrTable header +0x338 with lap+1 and D_80152734 (music_tempo_adjust is the formatted-text print).
 * Shaping (disclosed): gfx_lock/gfx_unlock/font_set are the family's inlined helpers (non-static so umerge
 * leaves no deleted-procedure stubs); the two `continue` guards give retail's s0..s3 order (nested ifs
 * colour the strength-reduced lap pointer first).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct TblHdr {
    u8 pad0[0x338];
    char *text;
} TblHdr;

typedef struct StrTable {
    u8 pad0[4];
    TblHdr *hdr;
} StrTable;

typedef struct Pos {
    s32 x, y;
} Pos;

typedef struct Pair {
    s32 a, b;
} Pair;

typedef struct Lap {
    u8 pad0[2024];
    s8 lap;
    u8 pad7E9[2056 - 2025];
} Lap;

typedef struct Car {
    u8 pad0[239];
    s8 f239;
    u8 padF0[952 - 240];
} Car;

extern OSMesgQueue D_801461D0;
extern u32 D_801174B4;
extern s32 D_8014A110;
extern s8 D_8016137C;
extern Pair D_80118E20;
extern StrTable D_8017A4E0;
extern s16 gNumWindows;
extern s16 D_80152734;
extern Lap D_8014A250[];
extern Car D_80152818[];
extern Pos D_80115DA8[][4];

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void render_helper(f32);
void dispatch_handler(s32);
void music_tempo_adjust(s16, s16, char *, s32, s32);

void gfx_lock_EDC(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_EDC(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_EDC(s32 font)
{
    s32 old;

    gfx_lock_EDC();
    old = slot_state_setup(font);
    gfx_unlock_EDC();
    return old;
}

s32 func_80107EDC(void *item)
{
    s32 i;
    s32 x;
    s32 y;

    if ((D_801174B4 & 8) || !(D_801174B4 & 0x400000) || D_8014A110 == 1 || D_8014A110 == 5 || !D_8016137C) {
        return 1;
    }
    render_helper(0.0f);
    D_80118E20.b = 3;
    D_80118E20.a = 1;
    if (gNumWindows == 1) {
        font_set_EDC(11);
    } else {
        font_set_EDC(12);
    }
    for (i = 0; i < gNumWindows; i++) {
        if (D_80152734 == D_8014A250[i].lap) {
            continue;
        }
        if (D_80152818[i].f239 == 1) {
            continue;
        }
        x = D_80115DA8[gNumWindows - 1][i].x;
        y = D_80115DA8[gNumWindows - 1][i].y;
        dispatch_handler(0);
        music_tempo_adjust(x + 1, y + 1, D_8017A4E0.hdr->text, D_8014A250[i].lap + 1, D_80152734);
        dispatch_handler(1);
        music_tempo_adjust(x, y, D_8017A4E0.hdr->text, D_8014A250[i].lap + 1, D_80152734);
    }
    D_80118E20.b = 3;
    D_80118E20.a = 0;
    render_helper(-1.0f);
    return 1;
}
