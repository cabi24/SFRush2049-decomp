void sound_position_update(f32 arg0, f32 arg1) {
    f32 temp_f0;
    f32 var_f12;
    f32 var_f14;
    f32 var_f2;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 var_v0;
    void *temp_a2;
    void *temp_a3;
    void *temp_t6;
    void *temp_t9;
    void *temp_v0;
    void *temp_v0_2;
    void *var_a0;

    var_f12 = arg0;
    var_f14 = arg1;
    var_a0 = arg0;
    temp_a3 = var_a0->unk8;
    temp_a2 = var_a0;
    temp_v1 = temp_a3->unk10;
    if (temp_v1 != 2) {

    }
    if (temp_v1 != 1) {
        if (!(((f32) *(s32 *)0x80146200 * 0.75f) <= (f32) arg1)) {
            var_a0 = (void *) temp_a3->unk19;
            if ((var_a0 != NULL) && (temp_v1 == 2) && (temp_a3->unk24 <= 0.0f)) {
                var_f14 = *(f32 *)0x80152748;
                temp_f0 = temp_a3->unk20;
                var_f12 = temp_f0 + *(f32 *)0x80123A64;
                var_f2 = var_f12;
                if (var_f14 < temp_f0) {
                    var_f2 = var_f12 - 14400.0f;
                }
                var_v0 = 0;
                if (var_f2 < var_f14) {
                    var_v0 = 1;
                }
                if (var_v0 != 0) {
                    goto block_11;
                }
                goto block_13;
            }
block_13:
            if ((temp_v1 != 2) && ((*(f32 *)0x80123A68 < temp_a3->unk24) || ((var_a0 == NULL) && (temp_a3->unk14 == 1)))) {
                temp_v0 = (void *(*)(f32, f32, void *, void *, void *))0x80091B00(var_f12, var_f14, var_a0, temp_a2, temp_a3);
                temp_v1_2 = *(s32 *)0x8011024C;
                ((temp_v1_2 * 4) + 0x80140000)->unk3AE8 = temp_v0;
                *(s32 *)0x8011024C = temp_v1_2 + 1;
                temp_v0->unk2 = 3;
                temp_t9 = M2C_ERROR(/* Read from unset register $a2 */)->unk8;
                temp_v0->unk4 = temp_t9;
                temp_t9->unk1A = (u8) (temp_t9->unk1A + 1);
            }
        } else {
block_11:
            if (temp_v1 == 2) {
                temp_v0_2 = (void *(*)(f32, f32, void *, void *, void *))0x80091B00(var_f12, var_f14, var_a0, temp_a2, temp_a3);
                temp_v1_3 = *(s32 *)0x80110250;
                ((temp_v1_3 * 4) + 0x80140000)->unk3CF0 = temp_v0_2;
                *(s32 *)0x80110250 = temp_v1_3 + 1;
                temp_v0_2->unk2 = 5;
                temp_t6 = M2C_ERROR(/* Read from unset register $a2 */)->unk8;
                temp_v0_2->unk4 = temp_t6;
                temp_t6->unk1A = (u8) (temp_t6->unk1A + 1);
            }
        }
    }
}
