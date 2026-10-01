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
extern s32 D_801170FC;
extern f32 D_8002EB94, D_801249CC;
extern s32 D_80117530, D_80121DDC, D_80143FC8;
extern void entity_transform_apply(void *,s32);
extern void entity_spawn_callback(s32,s32,s32);
extern void func_800AFA84(void *,void *);
extern void sound_position_set(f32 *,void *);
void func_8010E4E4(void *arg0, s16 arg1) {
    void *sp60;
    f32 pos[3];
    f32 *temp_a0;
    f32 *temp_v1;
    f32 temp_f4;
    f32 temp_f8;
    s16 var_a2;
    s32 temp_v0;
    void *temp_a1;
    void *temp_t1;
    void *temp_t2;

    if (arg1 == 0) {
        entity_transform_apply(arg0, 1);
        return;
    }
    if (D_801170FC == 0) {
        var_a2 = 0;
        temp_t1 = M2C_FIELD(arg0, void **, 0xC);
        temp_t2 = M2C_FIELD(temp_t1, void **, 0x6C);
        do {
            temp_v0 = var_a2 * 4;
            temp_a0 = (u8 *) &D_80121DDC + temp_v0;
            temp_v1 = (u8 *) temp_t2 + 0xC + temp_v0;
            temp_f4 = *temp_v1;
            temp_f8 = *temp_a0 * D_801249CC;
            temp_a1 = (u8 *) temp_t1 + temp_v0;
            var_a2 += 1;
            *temp_v1 = temp_f4 + temp_f8;
            M2C_FIELD(temp_a1, f32 *, 0x38) = (f32) (M2C_FIELD(temp_a1, f32 *, 0x38) + (*temp_v1 + (*temp_a0 * D_801249CC)));
        } while (var_a2 < 3);
        pos[0] = M2C_FIELD(temp_t2, f32 *, 0) * D_8002EB94;
        pos[1] = M2C_FIELD(temp_t2, f32 *, 4) * D_8002EB94;
        sp60 = temp_t1;
        pos[2] = M2C_FIELD(temp_t2, f32 *, 8) * D_8002EB94;
        sound_position_set(pos, (u8 *) temp_t1 + 0x14);
        M2C_FIELD(arg0, f32 *, 0x10) = (f32) (M2C_FIELD(arg0, f32 *, 0x10) - D_8002EB94);
        if (M2C_FIELD(arg0, f32 *, 0x10) <= 0.0f) {
            if (M2C_FIELD(((M2C_FIELD(sp60, s16 *, 0x10) * 0x30) + (u8 *) &D_80117530), u16 *, 0x12) & 0x2000) {
                entity_spawn_callback(M2C_FIELD(sp60, s16 *, 0xE), 0, 0);
            }
            func_800AFA84(&D_80143FC8, sp60);
            entity_transform_apply(arg0, 1);
        }
    }
}

