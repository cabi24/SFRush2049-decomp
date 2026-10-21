/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * particle_lifetime_set (historical label) = GC createBG@front.c (MAT3 front.c createBG, GC 0x80013C00, size 0x220):
 * builds the four scrolling menu-background polys.
 * w37b: D_801391E4 (only this function references it) is a function-local static (N64 BG_DIST source, an int 1025
 * converted to float; GC #define BG_DIST 16777000.0f). Unit: 51 of 183 words differ (w36q best: 65). Residual: as1
 * order of the static store vs `sw ra`, and the ugen temp ring two registers behind retail from the gBackCamera load
 * on. NOT a match.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef union {
    u8 c[4];
    u32 rgba;
} Color;

typedef struct {
    f32 minX;            /* 0x00 */
    f32 maxX;            /* 0x04 */
    f32 minY;            /* 0x08 */
    f32 maxY;            /* 0x0C */
    f32 delX;            /* 0x10 */
    f32 delY;            /* 0x14 */
    f32 offX[4];         /* 0x18 */
    f32 offY[4];         /* 0x28 */
} BackRect; /* 0x38 */

typedef struct {
    s16 tc[2];
    u8 pad4[16];
} PolyVert; /* 0x14 */

typedef struct {
    s16 nverts;          /* 0x00 */
    u16 flags;           /* 0x02 */
    u8 pad4[16];
    PolyVert verts[4];   /* 0x14 */
} Poly;

typedef struct SoundClearRecord SoundClearRecord;
typedef struct SoundState SoundState;

extern BackRect D_80114264[4][4];        /* gBackRects */
extern s32 D_801145E4[4][4];             /* gBackCamera */
extern Poly *D_80114628[4];              /* backPolys */
extern void *D_80114638;                 /* background texture (N64) */
extern s32 D_8011463C[4];                /* offsetsValid */
extern SoundState *D_80114624;           /* backBlit */
extern SoundClearRecord D_801141D4[];    /* frontBack */
extern SoundClearRecord D_8011421C[];    /* trackBack */
extern s16 gNumWindows;
extern volatile u8 gNumModels;
extern u32 state_word_a;                 /* gGameState */

void *func_800B24EC(char *name, u16 *index, s8 firstModel, s8 lastModel, s32 flag);   /* MBOX_FindTexture */
Poly *func_800A78BC(s32 nverts, f32 (*xyz)[3], u16 texidx, Color *color, u16 flags, s32 useTidx);  /* ZOID_NewPoly */
extern SoundState *sound_control(s16 ulx, s16 uly, SoundClearRecord *mblit, s16 nblits);           /* NewMultiBlit */

void ZOID_ShowPoly_bg(Poly *poly)
{
    poly->flags &= ~0x8000;
}

void particle_lifetime_set(void)
{
    static s32 D_801391E4;
    s32 polyIdx;
    f32 polyVerts[4][3];
    u16 texIdx;

    D_801391E4 = 1025;
    for (polyIdx = 0; polyIdx < 4; polyIdx++) {
        Color backColor = { { 0x00, 0x00, 0xC0, 0xFF } };

        D_80114638 = func_800B24EC("BACKGROUND_SFX", &texIdx, 0, gNumModels - 1, 1);
        polyVerts[0][0] = (f32)D_801391E4 * D_80114264[gNumWindows - 1][polyIdx].maxX;
        polyVerts[0][1] = (f32)D_801391E4 * D_80114264[gNumWindows - 1][polyIdx].maxY;
        polyVerts[0][2] = (f32)D_801391E4;
        polyVerts[1][0] = (f32)D_801391E4 * D_80114264[gNumWindows - 1][polyIdx].minX;
        polyVerts[1][1] = (f32)D_801391E4 * D_80114264[gNumWindows - 1][polyIdx].maxY;
        polyVerts[1][2] = (f32)D_801391E4;
        polyVerts[2][0] = (f32)D_801391E4 * D_80114264[gNumWindows - 1][polyIdx].minX;
        polyVerts[2][1] = (f32)D_801391E4 * D_80114264[gNumWindows - 1][polyIdx].minY;
        polyVerts[2][2] = (f32)D_801391E4;
        polyVerts[3][0] = (f32)D_801391E4 * D_80114264[gNumWindows - 1][polyIdx].maxX;
        polyVerts[3][1] = (f32)D_801391E4 * D_80114264[gNumWindows - 1][polyIdx].minY;
        polyVerts[3][2] = (f32)D_801391E4;
        D_80114628[polyIdx] = func_800A78BC(4, polyVerts, texIdx, &backColor,
                                            D_801145E4[gNumWindows - 1][polyIdx] | 0x1210, 1);
        ZOID_ShowPoly_bg(D_80114628[polyIdx]);
        if (D_8011463C[gNumWindows - 1] == 1) {
            Poly *backPoly;
            s32 vertIdx;

            backPoly = D_80114628[polyIdx];
            for (vertIdx = 0; vertIdx < backPoly->nverts; vertIdx++) {
                backPoly->verts[vertIdx].tc[0] = D_80114264[gNumWindows - 1][polyIdx].offX[vertIdx];
                backPoly->verts[vertIdx].tc[1] = D_80114264[gNumWindows - 1][polyIdx].offY[vertIdx];
            }
        }
    }
    if (state_word_a & 0x80) {
        D_80114624 = sound_control(0, 0, D_8011421C, 2);
    } else {
        D_80114624 = sound_control(0, 0, D_801141D4, 2);
    }
}
