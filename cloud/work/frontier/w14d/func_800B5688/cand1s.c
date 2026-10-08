/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern s32 D_8011A994;
extern s32 D_8011AC94;
extern s8 D_8011AD30;
extern s32 D_8011AD34;
extern s8 D_8011AD38;
extern s32 D_8011AD68;
extern void visual_objects_update(s32 arg0);
extern void sound_handles_clear(s32 arg0);
extern void sound_stop(s32 sound_id);
extern void ambient_sounds_clear(void);
extern void entity_spawn_callback(s16 arg0, s32 arg1, s32 arg2);

void func_800B5688(void) {
    s32 *slot;

    if (D_8011AD30 != 0) {
        visual_objects_update(1);
        slot = &D_8011A994;
        do {
            if (slot[1] != -1) {
                entity_spawn_callback((s16)slot[1], 0, 0);
                slot[1] = -1;
            }
            slot += 16;
        } while ((s32)slot != (s32)&D_8011AC94);
        D_8011AC94 = 0;
        sound_handles_clear(0);
        D_8011AD68 = 0;
        if (D_8011AD34 != 0) {
            sound_stop(D_8011AD34);
            D_8011AD34 = 0;
        }
        ambient_sounds_clear();
        D_8011AD38 = 0;
        D_8011AD30 = 0;
    }
}

/* stand-in caller (not retail) so the callee can be internal */
void standin_caller_b(void) {
    func_800B5688();
    func_800B5688();
}
