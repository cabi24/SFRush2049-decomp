/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef int s32;
typedef unsigned char u8;
typedef signed char s8;
typedef float f32;
#define M2C_FIELD(expr,type_ptr,offset) (*(type_ptr)((s8 *)(expr)+(offset)))
extern s32 D_80149868;
extern f32 D_80152748;
extern s32 func_80091BA8(s32);
extern s32 entity_state_check(s32);
extern void scheduler_recv(s32);
extern s32 entity_flags_apply(s32,s32,s32,u8);
extern void entity_flag_check(s32 *);
extern void audio_effect_setup(s32 *);

void func_800988D8(void) {
    s32 var_s3;
    s32 temp_s5;
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 idx;
    f32 temp_f0;
    f32 temp_f14;
    f32 var_f12;

    var_s3 = D_80149868;
    while ( 0 != var_s3 ) {
        temp_s5 = M2C_FIELD(var_s3, s32 *, 0);
        if (M2C_FIELD(var_s3, s8 *, 0xE) != 0) {
            scheduler_recv(M2C_FIELD(var_s3, s32 *, 0x38));
            entity_flag_check((s32 *)var_s3);
        } else {
            temp_a0 = M2C_FIELD(var_s3, s32 *, 0x38);
            if ( -1 == temp_a0 ) {
                temp_f0 = M2C_FIELD(var_s3, f32 *, 0x10);
                temp_f14 = temp_f0 + M2C_FIELD(var_s3, f32 *, 0x14);
                var_f12 = temp_f14;
                if ( temp_f0 > D_80152748 ) {
                    var_f12 = temp_f14 - 14400.0f;
                }
                if (var_f12 < D_80152748) {
                    scheduler_recv(temp_a0);
                    entity_flag_check((s32 *)var_s3);
                    var_s3 = temp_s5;
                    continue;
                }
            }
            temp_v0 = func_80091BA8(temp_a0);
            if ( 0 != temp_v0 ) {
                if (M2C_FIELD(temp_v0, s32 *, 0x10) == 2 && M2C_FIELD(temp_v0, u8 *, 0x1A) == 0 &&
                    entity_state_check(M2C_FIELD(temp_v0, s32 *, 0x3C)) == 0) {
                    audio_effect_setup((s32 *)temp_v0);
                } else {
                    var_s3 = temp_s5;
                    continue;
                }
            }
            idx = M2C_FIELD(var_s3, u8 *, 0x18);
            if (M2C_FIELD(var_s3, s32 *, 0x1C + idx * 4) == -1) {
                do {
                     idx = (u8)idx + 1; 
                    M2C_FIELD(var_s3, u8 *, 0x18) = (u8)idx;
                } while (idx < 4 && M2C_FIELD(var_s3, s32 *, 0x1C + idx * 4) == -1);
            }
            if (idx >= 4) {
                entity_flag_check((s32 *)var_s3);
            } else {
                M2C_FIELD(var_s3, u8 *, 0x18) = (u8)(idx + 1);
                temp_v0_2 = entity_flags_apply(M2C_FIELD(var_s3, s32 *, 0x1C + idx * 4), M2C_FIELD(var_s3, s32 *, 0x2C), M2C_FIELD(var_s3, s32 *, 0x30), M2C_FIELD(var_s3, u8 *, 0x34));
                M2C_FIELD(var_s3, s32 *, 0x38) = temp_v0_2;
                M2C_FIELD(func_80091BA8(temp_v0_2), s32 *, 0x40) = var_s3;
            }
        }
        var_s3 = temp_s5;
    }
}

/* stand-in caller (provisional): keeps func_800988D8 outlined; stands for the unmatched real caller func_80098FB8 */
void __standin_func_80098FB8(void) {
    func_800988D8();
    func_800988D8();
}
