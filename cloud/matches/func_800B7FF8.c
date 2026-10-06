/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* N64 frame dispatch. The VI-derived frame-delta clock is observed through the
 * same volatile view supported by repeated native loads in func_8010E694.
 * Its writers are viTickStart and viUpdateTime; see the companion proof.
 * The original N64 header is not recovered. No arcade donor is claimed.
 * D_80123DE0 is the external protected single-precision pi constant.
 * The overlay mode must be reread after particles_update may change it.
 */
typedef int s32;
typedef unsigned int u32;
typedef float f32;
extern s32 state_word_a, D_801170FC, gameplay_mode;
extern f32 D_80123DE0, D_80116178;
extern volatile f32 D_8002EB94;
extern void particles_update(s32);
extern void func_80391B00(void);
void func_800B7FF8(void) {
    if (state_word_a != -1 && ((state_word_a & 0x200000) || (state_word_a & 0x400000))) {
        if (!D_801170FC) D_80116178 += D_80123DE0 * D_8002EB94;
        if (gameplay_mode == 0 || gameplay_mode == 1 || gameplay_mode == 2 || gameplay_mode == 3 || gameplay_mode == 4) particles_update(0);
        if (gameplay_mode == 6 || gameplay_mode == 4) func_80391B00();
    }
}
