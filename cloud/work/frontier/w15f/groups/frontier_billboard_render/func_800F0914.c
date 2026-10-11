/* func_800F0914 (0x800F0914, 1584 B): best-times table for the results / save-ghost screen.
 * Reconstruction from the retail disassembly (w15f). For each of the 5 table rows: a row whose
 * source slot is -1 gets a random default name (LCG 0x41C64E6D/12345 seeded from the track's par
 * time * 6969) and a random time a little above par; otherwise the stored name (built-in index < 21,
 * else the record's name at +20) and stored time. Times print as m:ss.mmm. Then (re)builds the
 * screen's title box (same sequence as func_800F1210 state 5).
 * Status: genuine reconstruction used as IPA context (it clobbers s0-s8 and $f20/$f22 like retail);
 * not claimed. Its callee slot_state_setup takes $s2 as a register parameter in retail and is
 * unmatched, so this body cannot be exact until slot_state_setup is in the unit. */
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct TextInfo { char unknown[54]; u16 index; } TextInfo;
typedef struct TextState { s32 unknown[3]; TextInfo *info; char **strings; } TextState;
typedef struct BestTimes { f32 unknown; f32 time[3][5]; f32 rest[8]; } BestTimes;   /* 96 bytes */

extern TextState countdown_state;
extern s8 D_80152570;               /* mirrored-track flag: tables offset by 6 / 19 */
extern s8 D_8014978C;               /* track */
extern s16 D_80142528;              /* table row (track + mirror offset) */
extern s8 D_801148D0;               /* which list is shown (toggled by billboard_render) */
extern s32 D_80151690[][3][5];      /* entry source: -1 none, < 21 built-in name, else record */
extern BestTimes D_80150F88[];
extern f32 D_8002E870[];            /* par time per track */
extern char *D_801147C8[];          /* 55 default names */
extern char D_80151968[][13];       /* built-in names */
extern char *D_80143FE0[5];         /* row names */
extern char D_80143F20[5][10];      /* row time strings */
extern s8 *D_80149778[2];
extern char D_801461D0[];
extern s32 sprintf(char *, const char *, ...);
extern s32 osRecvMesg(void *, void *, s32), osJamMesg(void *, void *, s32);
extern s32 slot_state_setup(s32);
extern s32 object_manager_update(char *, s32), object_bytes_sum_global(void);
extern void crowd_cheer_play(void *, s32, s32, s32, s32);

void func_800F0914(void)
{
    s32 *src;
    f32 *best;
    f32 t;
    u32 n;
    s32 i;
    s32 previous;
    s32 width, height;

    D_80142528 = (D_80152570 ? 6 : 0) + D_8014978C;
    src = D_80151690[D_80142528][D_801148D0];
    best = D_80150F88[D_80142528].time[D_801148D0];
    t = D_8002E870[(D_80152570 ? 19 : 0) + D_8014978C];
    n = t * 6969.0f;
    if (D_801148D0 == 1) {
        n = n * 0x41C64E6D + 12345;
        t += 1.0f + (f32)(((s32)(n >> 16) & 0x7FFF) % 2000) / 1000.0f;
    }
    for (i = 0; i < 5; i++) {
        if (src[i] == -1) {
            n = n * 0x41C64E6D + 12345;
            D_80143FE0[i] = D_801147C8[((s32)(n >> 16) & 0x7FFF) % 55];
            n = n * 0x41C64E6D + 12345;
            t += (f32)(((s32)(n >> 16) & 0x7FFF) % 1000) / 1000.0f;
            sprintf(D_80143F20[i], "%01d:%02d.%03d", (u32)t / 60, (u32)t % 60, (u32)(t * 1000.0f) % 1000);
        } else {
            if ((u32)src[i] < 21) {
                D_80143FE0[i] = D_80151968[src[i]];
            } else {
                D_80143FE0[i] = *(char **)src[i] + 20;
            }
            t = best[i];
            n = t;
            sprintf(D_80143F20[i], "%01d:%02d.%03d", n / 60, n % 60, (u32)(t * 1000.0f) % 1000);
        }
    }
    if (D_80149778[0] != 0) {
        osRecvMesg(D_801461D0, 0, 1);
        previous = slot_state_setup(13);
        osJamMesg(D_801461D0, 0, 0);
        width = object_manager_update(countdown_state.strings[countdown_state.info->index + D_801148D0], -1);
        height = object_bytes_sum_global();
        crowd_cheer_play(D_80149778[0], 152 - width / 2, 6, 168 + width / 2, 14 + height);
    }
}
