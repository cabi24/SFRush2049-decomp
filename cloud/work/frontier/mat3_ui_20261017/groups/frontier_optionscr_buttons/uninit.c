/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Options-screen teardown, func_800DA0BC, complete native extent 184 bytes.
 * Inherited w14ob C, now compiled with the genuine locked caller func_800DB1E0
 * instead of the old stand-in. GC optionscr.c names the operation uninitScreen.
 * Shaping retained and disclosed: slot assignment and do open on the SAME line.
 * This source-line tie preserves as1's order of the loop cursor/end addresses.
 * No invented caller, false prototype, new volatility or unused locals.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u32 state_word_a;
extern s32 D_80116DA4;
extern s32 D_80116D98;
extern s32 D_8011650C;
extern s32 D_80116D0C;
extern s8 D_80116D94;
extern void sound_handles_clear(s32 arg0);
extern void sound_stop(s32 sound_id);
extern void ambient_sounds_clear(void);
extern void entity_spawn_callback(s16 arg0, s32 arg1, s32 arg2);

void func_800DA0BC(void) {
    s32 *slot;

    if (!(state_word_a & 0x7C03FFFE)) {
        sound_handles_clear(0);
        D_80116DA4 = 0;
    }
    if (D_80116D98 != 0) {
        sound_stop(D_80116D98);
        D_80116D98 = 0;
    }
    ambient_sounds_clear();
    slot = &D_8011650C; do {
        if (slot[1] != -1) {
            entity_spawn_callback((s16)slot[1], 0, 0);
            slot[1] = -1;
        }
        slot += 16;
    } while ((s32)slot != (s32)&D_80116D0C);
    D_80116D0C = 0;
    D_80116D94 = 0;
}
