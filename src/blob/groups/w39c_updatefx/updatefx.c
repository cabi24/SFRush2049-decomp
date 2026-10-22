/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * sound_position_update (0x800951E0, 444 B; historical label) = _UpdateFx@audiomgr.c (MAT3 GC 0x800A3080, DC
 * 8C014C68): the WalkBST callback of _ProcessPriorities (its address is loaded by func_80098FB8). For a sorted fx tag
 * that is not pending (state 1): when it is beyond the voice budget (elNum >= gMaxSfx * 0.75f) or is a silent looping
 * playing tag whose 0.2 s timeout has passed, queue an interrupt command (5) for a playing tag in gStopCmds; else, for
 * a tag not playing that is audible (vol > 0.1f) or a one-shot of type 1, queue a start command (3) in gStartCmds.
 * Commands come from _GetCmd (func_80091B00, IPA context: retail keeps stag in a2 across the call). AudioTimeout
 * (func_80095198, kept out of line at 0x80095198) is inlined from its own file (timeout.c), as in retail.
 * MAT3 typed port. Differences from the MAT3/DC text, each measured (cloud/work/frontier/w39c/RESULTS.md):
 *  - no `tag` local: every access is `stag->tag->...` (retail CSEs the state load into one register and colours
 *    stag a2 / tag a3 / the constant 2 t0; with MAT3's `tag` local the tag web outranks stag: 19 rows);
 *  - `gMaxSfx * 0.75f` (MAT3 `0.75f * gMaxSfx`): retail's mul.s has the converted gMaxSfx first.
 * SHAPING (disclosed, compiles to no code): the first line. Retail starts with `beq t0(2),v1(state),next; nop`, a
 *  branch to the next instruction. IDO leaves one only when the guarded block still holds a computation when ugen
 *  runs (uopt sinks `t` into it; ugen drops the empty `if (t)`s; as1 deletes the dead set). An empty `if` alone is
 *  removed (24 rows). A compiled-out debug/assert macro is the natural candidate. Same device family as the
 *  installed func_80096288 (slot_sound group). `!elNum` instead of `elNum == 2` gives the same code.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct FxTag {
    void *next, *prev;      /* 0x00 ll */
    void *list;             /* 0x08 */
    u32 id;                 /* 0x0C */
    s32 state;              /* 0x10 */
    s32 type;               /* 0x14 */
    s8 paused;              /* 0x18 */
    s8 loop;                /* 0x19 */
    u8 numCmds;             /* 0x1A */
    u8 priority;            /* 0x1B */
    s32 wpriority;          /* 0x1C */
    f32 startTime;          /* 0x20 */
    f32 vol;                /* 0x24 */
} FxTag;
typedef struct SortedTag {
    void *left, *right;     /* 0x00 tBSTE */
    FxTag *tag;             /* 0x08 */
} SortedTag;
typedef struct AudioMsg {
    s16 type;               /* 0x00 */
    s8 cmd;                 /* 0x02 */
    s8 used;                /* 0x03 */
    FxTag *tag;             /* 0x04 */
} AudioMsg;
extern s32 D_80146200;              /* gMaxSfx */
extern s32 D_80110250;              /* gNumStopCmds */
extern s32 D_8011024C;              /* gNumStartCmds */
extern AudioMsg *D_80143CF0[];      /* gStopCmds */
extern AudioMsg *D_80143AE8[];      /* gStartCmds */
AudioMsg *func_80091B00(void);      /* _GetCmd */
s32 func_80095198(f32 startTime, f32 interval);   /* AudioTimeout */

void sound_position_update(void *element, int elNum)
{
    SortedTag *stag = element;
    AudioMsg *cmd;
    { s32 t = elNum == 2; if (stag->tag->state != 2) { if (t) {} if (t) {} } }
    if (stag->tag->state != 1) {
        if (elNum >= D_80146200 * 0.75f ||
            (stag->tag->loop && stag->tag->state == 2 && stag->tag->vol <= 0.0f &&
             func_80095198(stag->tag->startTime, 0.2f))) {
            if (stag->tag->state == 2) {
                cmd = func_80091B00();
                D_80143CF0[D_80110250++] = cmd;
                cmd->cmd = 5;
                cmd->tag = stag->tag;
                cmd->tag->numCmds++;
            }
        } else if (stag->tag->state != 2 &&
                   (stag->tag->vol > 0.1f || (!stag->tag->loop && stag->tag->type == 1))) {
            cmd = func_80091B00();
            D_80143AE8[D_8011024C++] = cmd;
            cmd->cmd = 3;
            cmd->tag = stag->tag;
            cmd->tag->numCmds++;
        }
    }
}
