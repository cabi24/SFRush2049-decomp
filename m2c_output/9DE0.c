void func_800091E0(? *arg0, void *arg1) {
    ? *var_a3;
    ? *var_t0;
    f32 var_ft4;
    f32 var_ft5;
    s32 temp_fa0;
    s32 temp_fa0_2;
    s32 temp_fa1;
    s32 temp_fa1_2;
    s32 var_a0;
    s32 var_a2;
    void *var_v0;
    void *var_v1;

    var_v0 = arg1;
    var_v1 = arg1 + 0x20;
    var_a2 = 0;
    var_a3 = arg0;
    do {
        var_t0 = var_a3;
        var_a0 = 1;
        var_ft5 = var_t0->unk0;
        var_ft4 = var_t0->unk4 * 65536.0f;
        if (1 != 2) {
            do {
                var_a0 += 1;
                var_v0 += 4;
                var_v1 += 4;
                var_t0 += 8;
                temp_fa0 = (s32) var_ft4;
                temp_fa1 = (s32) (var_ft5 * 65536.0f);
                var_v0->unk-4 = (s32) ((temp_fa1 & 0xFFFF0000) | ((temp_fa0 >> 0x10) & 0xFFFF));
                var_v1->unk-4 = (s32) (((temp_fa1 << 0x10) & 0xFFFF0000) | (temp_fa0 & 0xFFFF));
                var_ft5 = var_t0->unk0;
                var_ft4 = var_t0->unk4 * 65536.0f;
            } while (var_a0 != 2);
        }
        var_v0 += 4;
        var_v1 += 4;
        temp_fa0_2 = (s32) var_ft4;
        temp_fa1_2 = (s32) (var_ft5 * 65536.0f);
        var_v0->unk-4 = (s32) ((temp_fa1_2 & 0xFFFF0000) | ((temp_fa0_2 >> 0x10) & 0xFFFF));
        var_v1->unk-4 = (s32) (((temp_fa1_2 << 0x10) & 0xFFFF0000) | (temp_fa0_2 & 0xFFFF));
        var_a2 += 1;
        var_a3 += 0x10;
    } while (var_a2 != 4);
}

void func_800092E0(void *arg0, s32 *arg1) {
    s32 sp4;
    s32 sp0;
    s32 *var_v0;
    s32 temp_a3;
    s32 var_a0;
    s32 var_a2;
    u32 *var_v1;
    void *var_t0;
    void *var_t1;

    var_v0 = arg1;
    var_v1 = arg1 + 0x20;
    var_a2 = 0;
    var_t0 = arg0;
    do {
        var_a0 = 0;
        var_t1 = var_t0;
loop_2:
        var_a0 += 1;
        sp4 = (((u32) *var_v1 >> 0x10) & 0xFFFF) | (*var_v0 & 0xFFFF0000);
        temp_a3 = (*var_v1 & 0xFFFF) | ((*var_v0 << 0x10) & 0xFFFF0000);
        sp0 = temp_a3;
        var_t1 += 8;
        var_v0 += 4;
        var_v1 += 4;
        var_t1->unk-8 = (f32) ((f32) sp4 * 0.000015258789f);
        var_t1->unk-4 = (f32) ((f32) temp_a3 * 0.000015258789f);
        if (var_a0 != 2) {
            goto loop_2;
        }
        var_a2 += 1;
        var_t0 += 0x10;
    } while (var_a2 != 4);
}

void func_80009394(? *arg0) {
    ? *var_v1;
    s32 var_v0;

    var_v1 = arg0;
    var_v0 = 0;
    do {
        if (var_v0 == 0) {
            var_v1->unk0 = 1.0f;
        } else {
            var_v1->unk0 = 0.0f;
        }
        if (var_v0 == 1) {
            var_v1->unk4 = 1.0f;
        } else {
            var_v1->unk4 = 0.0f;
        }
        if (var_v0 == 2) {
            var_v1->unk8 = 1.0f;
        } else {
            var_v1->unk8 = 0.0f;
        }
        if (var_v0 == 3) {
            var_v1->unkC = 1.0f;
        } else {
            var_v1->unkC = 0.0f;
        }
        var_v0 += 1;
        var_v1 += 0x10;
    } while (var_v0 != 4);
}

void func_8000941C(void *arg0) {
    ? sp18;

    func_80009394(&sp18);
    func_800091E0(&sp18, arg0);
}
