/* flags: -g0 -O3 -mips2 -G 0 -non_shared -- NOT A MATCH: 21/157 words in the whole-program unit
 * (score with blob_unit --with this file; it redefines the kept wrappers audio_effect_process,
 * object_type1_create, object_type7_create as __inline with four stand-in locals each, which reproduces
 * retail's per-instance frame slots exactly). Residual: the busy-wait START/END address webs. See ../RESULTS.md. Shaping: `volatile` on the busy-wait pointer `m` is load-bearing (without it 71 of 157 words differ); disclosed. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct ShutdownMsg { s16 id; s8 type, used; u8 payload[20]; } ShutdownMsg;
extern ShutdownMsg D_80142DD8[128];
extern ShutdownMsg D_801439D8[];
extern s8 D_8011023C;
extern u32 D_8011025C, D_80110260, D_80110244, D_80110248, D_80110270;
void init_wait_completion(void);
s32 osRecvMesg(void *, void *, s32);
s32 osJamMesg(void *, void *, s32);
extern s32 D_80152770, D_80142728, D_801427A8;
void audio_reverb_update(u32 address, s32 tag);
ShutdownMsg *func_80091B00(void);
__inline void audio_effect_process(u32 address) {
    s32 u0, u1, u2, u3;
    osRecvMesg(&D_80152770, 0, 1);
    audio_reverb_update(address, 0);
    osJamMesg(&D_80152770, 0, 0);
}
__inline void object_type1_create(void) {
    ShutdownMsg *sp1C;
    ShutdownMsg *temp_v0;
    s32 u0, u1, u2, u3;

    osRecvMesg(&D_80142728, 0, 1);
    temp_v0 = func_80091B00();
    sp1C = temp_v0;
    temp_v0->type = 1;
    osJamMesg(&D_80142728, 0, 0);
    osJamMesg(&D_801427A8, sp1C, 0);
}
__inline void object_type7_create(void) {
    ShutdownMsg *sp1C;
    ShutdownMsg *temp_v0;
    s32 u0, u1, u2, u3;

    osRecvMesg(&D_80142728, 0, 1);
    temp_v0 = func_80091B00();
    sp1C = temp_v0;
    temp_v0->type = 7;
    osJamMesg(&D_80142728, 0, 0);
    osJamMesg(&D_801427A8, sp1C, 0);
}

void func_800C8918(void) {
    volatile ShutdownMsg *m;
    ShutdownMsg *end;

    if (D_8011023C) {
        if (D_8011025C) {
            audio_effect_process(D_8011025C);
            D_8011025C = 0;
        }
        if (D_80110260) {
            audio_effect_process(D_80110260);
            D_80110260 = 0;
        }
        object_type1_create();
        object_type7_create();
        end = D_801439D8;
        do {
            m = D_80142DD8;
            do {
                if (m->used) {
                    break;
                }
                m++;
            } while (m != end);
        } while (m < end);
        D_8011023C = 0;
        init_wait_completion();
        audio_effect_process(D_80110244);
        audio_effect_process(D_80110248);
        audio_effect_process(D_80110270);
    }
}
