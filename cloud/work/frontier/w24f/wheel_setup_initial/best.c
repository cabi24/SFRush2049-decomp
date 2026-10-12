/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * w24f best for wheel_setup_initial (NOT a match: 4 words, colour-only).
 * Release every resource slot of a given type: D_80156D38[64] is the 20-byte ResSlot table (layout from the
 * locked sibling func_80097694 in codex_state_audio_a146). The per-slot test is an inlined static helper
 * (retail stub func_800A4B40 follows the function) using func_80097694's predicate with sub = -1; its
 * parameters give retail's 8-byte inline area (frame 48) and the constant -1 is retail's codeless s4 web.
 * Residual: constants 64 and -1 tie at save 3.33 and -1 wins (s3); retail gives 64 s3.
 * force.sh wR5 wheel_setup_initial "p1:w47=c17,p1:w46=c18" -> differing rows 0.
 */
typedef signed char s8; typedef unsigned char u8; typedef int s32; typedef unsigned int u32;
typedef struct ResSlot {s8 f0,loaded,f2,f3,f4,f5;u8 type,pad07;s32 pad08;u8 **p0C;u8 *w10;} ResSlot;
extern ResSlot D_80156D38[64];
void func_80096130(s32 index);
static void release_if(ResSlot *slot, s32 i, s32 type, s8 sub);
void wheel_setup_initial(s32 type) {
    s32 i;
    for (i = 0; i < 64; i++) {
        release_if(&D_80156D38[i], i, type, -1);
    }
}
static void release_if(ResSlot *slot, s32 i, s32 type, s8 sub) {
    if ((slot->p0C != 0) && (type == slot->type) && ((sub < 0) || (sub == slot->f5))) func_80096130(i);
}
