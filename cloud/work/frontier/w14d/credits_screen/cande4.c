/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u32 state_word_a;
extern s32 D_80142728;
extern s32 D_801427A8;
extern s32 D_801102B4;
extern s32 D_801105B4;
extern s8 D_8011062C;
extern s32 D_80110630;
extern s8 D_80110634;
extern s32 D_80110648;
extern void osRecvMesg(void *mq, void *msg, s32 flags);
extern void osJamMesg(void *mq, void *msg, s32 flags);
extern s32 *func_80091B00(void);
extern void sound_handles_clear(s32 arg0);
extern void sound_stop(s32 sound_id);
extern void ambient_sounds_clear(void);
extern void entity_spawn_callback(s16 arg0, s32 arg1, s32 arg2);

void credits_screen(void) {
    s32 *msg;
    s32 *slot;

    osRecvMesg(&D_80142728, 0, 1);
    msg = func_80091B00();
    *((s8 *)msg + 2) = 1;
    osJamMesg(&D_80142728, 0, 0);
    osJamMesg(&D_801427A8, msg, 0);
    if (D_8011062C) {
        slot = &D_801102B4;
        do {
            if (slot[1] != -1) {
                entity_spawn_callback((s16)slot[1], 0, 0);
                slot[1] = -1;
            }
            slot += 16;
        } while ((s32)slot != (s32)&D_801105B4);
        D_801105B4 = 0;
        if (!(state_word_a & 0x7C03FFFE)) {
            sound_handles_clear(0);
            D_80110648 = 0;
        }
        if (D_80110630 != 0) {
            sound_stop(D_80110630);
            D_80110630 = 0;
        }
        ambient_sounds_clear();
        D_80110634 = 0;
        D_8011062C = 0;
    }
}

/* stand-in caller (not retail) so the callee can be internal */
void standin_caller_c(void) {
    credits_screen();
    credits_screen();
}
