/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80109F54 (0x80109F54, 1504 B) = GC hud.c animFlag (MAT3 decomp/src/GameSource/src/hud.c animFlag,
 * GC 0x80021DE4; GC locals x, y, width, height, w, h). Checkered-flag blit drawn on the track map at the
 * finish checkpoint (gCheckpoints.points[finishPt]), with the laps-left digit printed over it on the last lap.
 * Globals: gNumWindows, gNumLapsLeft = D_80152014, gRace = D_8014A110, gShowTrackMap = D_80156BDC, gPathMax/Min =
 * D_801407B4/D_801407D4, kMapSize = D_801161C4, mapPos = D_801160A8, gMirrorWaves = D_80140A04, gCheckpoints =
 * D_80151CE8, gGameState = D_801174B4, gPauseState = D_801170FC, gFrameSeconds = D_8002EB94 (volatile, as MAT3),
 * TB_SetJust pair = D_80118E20; UpdateBlit = Input_ApplyPadConfig, HAL_SetFont = slot_state_setup (s2 IPA
 * parameter), HAL_SetTextDepth = render_helper, TB_SetTextColor = dispatch_handler, TB_Printf =
 * music_tempo_adjust, SelectBlit = stat_race_update.
 * N64 differences from GC: no gGameState 0x400000 term in the first Hidden; the map-centre offsets are two more
 * named locals (xoff, yoff: frame 136 = two slots more than GC's six; computed before mapPos) without GC's outer
 * /2; the finish offset is not halved; blt->x/y subtract width/2 once.
 * Levers: operand order `(pos - min) * w / width`, `gFrameSeconds * 1000.0f`; TB_SetJust stores .h then .v.
 * Shaping (disclosed): `(0 + width)` / `(0 + height)` on the variable side of the two aspect multiplies (no code;
 * uopt puts a plain variable second in a commutative op, retail multiplies with the variable first). Helpers
 * gfx_lock/gfx_unlock/font_set/TB_SetJust/Hidden_9F54 are inlined, non-static, not on keep (w17j stub rule).
 * w36m (wave 36).
 */typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct Blit {
    /* 0x00 */ char *name;
    /* 0x04 */ void *palette;
    /* 0x08 */ void *texture;
    /* 0x0C */ u16 texIndex;
    /* 0x0E */ s16 x;
    /* 0x10 */ s16 y;
    /* 0x12 */ u16 z;
    /* 0x14 */ s16 width;
    /* 0x16 */ s16 height;
    /* 0x18 */ u8 alpha;
    /* 0x19 */ u8 flip;
    /* 0x1A */ s8 hide;
    /* 0x1B */ s8 init;
    /* 0x1C */ s16 top, bot, left, right;
    /* 0x24 */ s16 color;
    /* 0x28 */ void *animFunc;
    /* 0x2C */ u32 animID;
    /* 0x30 */ s32 animDTA;
} Blit;

typedef struct CheckPoint {
    f32 pos[3];
    f32 normal[3];
    s32 radius;
    s16 flags;
    s16 time[2];
    s16 pathPts[20];
    f32 distToNext;
} CheckPoint;

typedef struct Checkpoints {
    s16 startTime;
    s16 loopPt;
    s16 finishPt;
    s16 startPt;
    s16 numPoints;
    CheckPoint points[10];
} Checkpoints;

typedef struct Pair {
    s32 h, v;
} Pair;

extern OSMesgQueue D_801461D0;
extern u32 D_801174B4;          /* gGameState */
extern s32 D_8014A110;          /* gRace */
extern s16 gNumWindows;
extern s8 D_80152014;           /* gNumLapsLeft */
extern s8 D_80156BDC;           /* gShowTrackMap */
extern s16 D_801407B4[3];       /* gPathMax */
extern s16 D_801407D4[3];       /* gPathMin */
extern s32 D_801161C4;          /* kMapSize */
extern s32 D_801160A8[4][2];    /* mapPos */
extern s8 D_80140A04;           /* gMirrorWaves */
extern Checkpoints D_80151CE8;  /* gCheckpoints */
extern s32 D_801170FC;          /* gPauseState */
extern volatile f32 D_8002EB94; /* gFrameSeconds */
extern Pair D_80118E20;

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void Input_ApplyPadConfig(Blit *);
void render_helper(f32);
void dispatch_handler(s32);
void music_tempo_adjust(s16, s16, char *, ...);
void stat_race_update(Blit *blt, s32 index, s32 w, s32 h);

void gfx_lock_9F54(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_9F54(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_9F54(s32 font)
{
    s32 old;

    gfx_lock_9F54();
    old = slot_state_setup(font);
    gfx_unlock_9F54();
    return old;
}

void TB_SetJust_9F54(s32 h, s32 v)
{
    D_80118E20.h = h;
    D_80118E20.v = v;
}

s32 Hidden_9F54(Blit *blt, s32 hide)
{
    if (hide != blt->hide) {
        blt->hide = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->hide;
}

s32 func_80109F54(Blit *blt)
{
    s32 x, y, width, height, w, h, xoff, yoff;

    if (Hidden_9F54(blt, gNumWindows > 4 || D_80152014 > 1 || D_8014A110 == 6 || D_8014A110 == 4 ||
                         D_8014A110 == 5 || !D_80156BDC))
        return 1;
    width = D_801407B4[0] - D_801407D4[0];
    height = D_801407B4[2] - D_801407D4[2];
    if (width > height) {
        w = D_801161C4 - 8;
        h = (0 + height) * (D_801161C4 - 8) / width;
    } else {
        h = D_801161C4 - 8;
        w = (0 + width) * (D_801161C4 - 8) / height;
    }
    xoff = (D_801161C4 - 8 - w) / 2 + 4;
    yoff = (D_801161C4 - 8 - h) / 2 + 4;
    x = D_801160A8[gNumWindows - 1][0] + xoff - D_801161C4 / 2;
    y = D_801160A8[gNumWindows - 1][1] + yoff - D_801161C4 / 2;
    if (D_80140A04)
        x += (D_80151CE8.points[D_80151CE8.finishPt].pos[0] - D_801407D4[0]) * w / width;
    else
        x += w - 1 - (D_80151CE8.points[D_80151CE8.finishPt].pos[0] - D_801407D4[0]) * w / width;
    y += (D_80151CE8.points[D_80151CE8.finishPt].pos[2] - D_801407D4[2]) * h / height;
    blt->x = x - blt->width / 2;
    blt->y = y - blt->width / 2;
    if (Hidden_9F54(blt, D_8014A110 == 1 || D_80152014 > 1 || (D_801174B4 & 8))) {
        if (D_8014A110 != 1 && !(D_801174B4 & 8)) {
            if (gNumWindows < 4)
                font_set_9F54(6);
            else
                font_set_9F54(7);
            render_helper(0.0f);
            TB_SetJust_9F54(1, 1);
            dispatch_handler(0);
            music_tempo_adjust(x + 1, y + 1, "%d", D_80152014);
            dispatch_handler(11);
            music_tempo_adjust(x, y, "%d", D_80152014);
            TB_SetJust_9F54(0, 3);
            render_helper(-1.0f);
        }
        return 1;
    }
    if (D_80140A04)
        blt->flip = 1;
    blt->alpha = 128;
    if (!D_801170FC)
        blt->animDTA -= D_8002EB94 * 1000.0f;
    if (blt->animDTA <= 0) {
        blt->animDTA = 100;
        blt->animID = (blt->animID + 1) % 5;
    }
    stat_race_update(blt, blt->animID, blt->width, blt->width);
    return 1;
}
