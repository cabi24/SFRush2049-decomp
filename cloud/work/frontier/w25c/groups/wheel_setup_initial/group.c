/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * wheel_setup_initial (0x800A4AC4): release every slot of the 20-byte ResSlot table D_80156D38[64] that is
 * loaded and owned by `type` (func_80096130 = release slot i). Layout and predicate from the locked sibling
 * func_80097694 (codex_state_audio_a146): `p0C != 0 && type == slot->type && (sub < 0 || sub == slot->f5)`.
 * The per-slot test is a deleted helper inlined by umerge, called with sub = -1; it is defined non-static under its
 * retail stub name func_800A4B40 (the caller-less `jr ra; nop` right after the function, w17j packaging rule).
 * Its parameter homes are retail's 8-byte local area (frame 48) and the folded constant -1 is retail's codeless s4
 * web (w24f).
 * DISCLOSED SHAPING DEVICE (w25c): the compiled-out `if (type == 64) {}` in the loop. Constants -1 and 64 and the
 * parameter `type` tie at save 3.33 (nocs 3) and ties go to the lower web number: -1 (web 46) took s3, 64 (web 47)
 * s4. The codeless compare gives `type` and 64 one more use each, so the order becomes type s2, 64 s3, -1 s4 as in
 * retail. No code is emitted for it.
 * Provenance: w4d/w20f loop, w24f helper (cloud/work/frontier/w24f/RESULTS.md), w25c tie lever.
 */
typedef signed char s8; typedef unsigned char u8; typedef int s32; typedef unsigned int u32;
typedef struct ResSlot {s8 f0,loaded,f2,f3,f4,f5;u8 type,pad07;s32 pad08;u8 **p0C;u8 *w10;} ResSlot;
extern ResSlot D_80156D38[64];
void func_80096130(s32 index);
void func_800A4B40(ResSlot *slot, s32 i, s32 type, s8 sub);
void wheel_setup_initial(s32 type) {
    s32 i;
    for (i = 0; i < 64; i++) {
        if (type == 64) {}
        func_800A4B40(&D_80156D38[i], i, type, -1);
    }
}
void func_800A4B40(ResSlot *slot, s32 i, s32 type, s8 sub) {
    if ((slot->p0C != 0) && (type == slot->type) && ((sub < 0) || (sub == slot->f5))) func_80096130(i);
}
