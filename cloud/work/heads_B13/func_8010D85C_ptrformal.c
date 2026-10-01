/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
#define NULL ((void *)0)
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern s32 D_801170FC, D_80118E08[], D_80121DA0, D_80121DA4, D_80121DA8, D_8012E738;
extern u8 D_80140BDC;
extern void entity_transform_apply(void *,s32);
extern void *func_800A464C(void *,void *);
extern s32 sound_bank_load(s32,u16 *,s8,s8,s32);
void func_8010D85C(void *arg0, s16 arg1) {
    u16 sp44;
    void *sp40;
    void *temp_a0;
    s32 var_v1;
    s8 var_v1_2;
    void *temp_s0;
    void *temp_v0;

    if (arg1 == 0) {
        entity_transform_apply(arg0, 1);
        return;
    }
    if (D_801170FC == 0) {
        temp_s0 = M2C_FIELD(arg0, void **, 0xC);
        if (!(M2C_FIELD(temp_s0, u8 *, 4) & 1)) {
            temp_a0 = M2C_FIELD(temp_s0, void **, 0x6C);
            M2C_FIELD(temp_s0, s16 *, 0x5A) = 0xA;
            sp40 = temp_a0;
            if (func_800A464C(temp_a0, &D_80121DA0) != NULL) {
                var_v1 = 2;
            } else if (func_800A464C(temp_a0, &D_80121DA4) != NULL) {
                var_v1 = 1;
            } else {
                var_v1 = 0;
            }
            M2C_FIELD(&D_8012E738, s32 *, M2C_FIELD(temp_s0, s16 *, 0xE) * 0x44) = sound_bank_load(D_80118E08[var_v1], &sp44, 0, (s8) (D_80140BDC - 1), 1);
            temp_v0 = func_800A464C(M2C_FIELD(temp_s0, void **, 0x6C), &D_80121DA8);
            if (temp_v0 != NULL) {
                var_v1_2 = M2C_FIELD(temp_v0, u8 *, 4) - 0x30;
            } else {
                var_v1_2 = 0;
            }
            M2C_FIELD(arg0, s16 *, 4) = (s16) var_v1_2;
            M2C_FIELD(temp_s0, u8 *, 4) = (u8) (M2C_FIELD(temp_s0, u8 *, 4) | 1);
        }
        M2C_FIELD(arg0, s32 *, 0x14) = 0x80094888;
        M2C_FIELD(arg0, f32 *, 0x10) = 0.0f;
    }
}

