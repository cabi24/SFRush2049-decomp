/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800F0914 = GC `_PrintScores` (highscorescr.c, confident pair, w28g): fills the five high-score rows
 * of the current track/table. Empty rows (-1) get a random default name and a time derived from the
 * track's default start time (seed = defTime * 6969); otherwise the name comes from the temp-name table
 * (index 0..20) or the player record (+0x14), and the time from gTotalRaceStats[track].scores[table][i].
 * Then the title pane (GC namePanes+0x1b0, N64 D_80149450+0x228) is resized around the table's title.
 * Internal (no callee saves); real callers billboard_render and func_800F1210. Rand_r (func_800D50E4) and
 * object_create (font switch) are locked and inlined by umerge. No shaping devices.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct Player {                 /* GC struct __anon_3bdf (0x4c) */
    u8 ll[8]; void *fileh; u32 id[2];
    char name[13];                      /* +0x14 */
} Player;
typedef struct RaceStats {              /* GC struct __anon_2dfd (0x60) */
    u32 checksum;
    f32 scores[3][5];                   /* +0x04 */
    u8 rest[0x60 - 0x40];
} RaceStats;
typedef struct TextIndices { u8 other[54]; u16 tableNames; } TextIndices;
typedef struct TextState { void *word0; char **fixed; void *word8; TextIndices *indices; char **strings; } TextState;
typedef struct ScoreScreen { u8 other[0x228]; void *titlePane; } ScoreScreen;

extern s8 D_80152570;                   /* gBackwards */
extern s8 D_8014978C;                   /* gTrackNum */
extern s8 D_801148D0;                   /* gTable */
extern s16 D_80142528;                  /* gThisTrack */
extern Player **D_80151690[][3][5];     /* gTotalNames */
extern RaceStats D_80150F88[];          /* gTotalRaceStats */
extern f32 gDefStartTimes[];                /* gDefStartTimes */
extern char *D_801147C8[];              /* gDefaultNames (55 on N64) */
extern char D_80151968[][13];           /* gTempNames */
extern char D_80143F20[5][10];          /* score strings */
extern char *D_80143FE0[5];             /* name pointers */
extern void *D_80149778;              /* GC namePanes title pane */
extern TextState countdown_state;       /* gText */
extern OSMesgQueue D_801461D0;
extern char D_8012060C[], D_8012061C[];

extern void sprintf(char *, char *, ...);
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32);
extern s32 object_create(s32);          /* font_set wrapper, locked (credits_scroll_grp) */
extern s32 object_manager_update(void *, s32);
extern s16 object_bytes_sum_global(void);
extern void crowd_cheer_play(void *, s32, s32, s32, s32);

extern s32 func_800D50E4(u32 *seed);   /* rand_r, locked, inlined by umerge */

void PrintScores(void)
{
    s32 i;
    f32 *s;
    Player ***p;
    f32 defTime;
    u32 seed;
    s32 width;
    s32 height;

    D_80142528 = (D_80152570 ? 6 : 0) + D_8014978C;
    p = D_80151690[D_80142528][D_801148D0];
    s = D_80150F88[D_80142528].scores[D_801148D0];
    defTime = gDefStartTimes[(D_80152570 ? 19 : 0) + D_8014978C];
    seed = defTime * 6969.0f;
    if (D_801148D0 == 1) {
        defTime += 1.0f + (f32)(func_800D50E4(&seed) % 2000) / 1000.0f;
    }
    for (i = 0; i < 5; i++) {
        if (*p == (Player **)-1) {
            D_80143FE0[i] = D_801147C8[func_800D50E4(&seed) % 55];
            defTime += (f32)(func_800D50E4(&seed) % 1000) / 1000.0f;
            sprintf(D_80143F20[i], D_8012060C, (u32)defTime / 60, (u32)defTime % 60,
                    (u32)(defTime * 1000.0f) % 1000);
        } else {
            if ((u32)*p <= 20) {
                D_80143FE0[i] = D_80151968[(s32)*p];
            } else {
                D_80143FE0[i] = (**p)->name;
            }
            defTime = *s;
            seed = defTime;
            sprintf(D_80143F20[i], D_8012061C, (u32)defTime / 60, (u32)defTime % 60, (u32)(defTime * 1000.0f) % 1000);
        }
        s++;
        p++;
    }
    if (D_80149778 != 0) {
        object_create(13);
        width = object_manager_update(countdown_state.strings[countdown_state.indices->tableNames + D_801148D0], -1);
        height = object_bytes_sum_global();
        crowd_cheer_play(D_80149778, 160 - width / 2 - 8, 6, width / 2 + 168, height + 14);
    }
}
