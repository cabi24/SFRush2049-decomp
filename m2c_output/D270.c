s32 func_8000D5C0(s32, void *, void *, u32);        /* extern */
extern s32 D_8002C34C;
extern void *D_8002C460;
extern void *D_8002C464;
extern s32 VI_BURST_REG;
extern s32 VI_CURRENT_REG;
extern s32 VI_DRAM_ADDR_REG;
extern u32 VI_H_START_REG;
extern s32 VI_H_SYNC_REG;
extern s32 VI_INTR_REG;
extern s32 VI_V_BURST_REG;
extern s32 VI_V_START_REG;
extern s32 VI_V_SYNC_REG;
extern s32 VI_WIDTH_REG;
extern s32 VI_X_SCALE_REG;

void func_8000C670(void) {
    void *sp44;
    u32 sp38;
    s32 sp34;
    s32 sp2C;
    void *sp24;
    f32 var_ft2;
    s32 temp_t1;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 var_a0;
    u16 temp_v1;
    u32 temp_v0;
    u32 var_a3;
    void *temp_a1;
    void *temp_s0;
    void *temp_t2;
    void *var_a1;
    void *var_a2;

    temp_s0 = D_8002C464;
    temp_a1 = temp_s0->unk8;
    sp2C = VI_CURRENT_REG & 1;
    sp44 = temp_a1;
    var_a1 = temp_a1;
    var_a2 = var_a1 + (sp2C * 0x14);
    var_a0 = var_a2->unk28 + func_8000D5C0(temp_s0->unk4, temp_a1);
    if (temp_s0->unk0 & 2) {
        temp_s0->unk20 = (s32) (temp_s0->unk20 | (var_a1->unk20 & ~0xFFF));
    } else {
        temp_s0->unk20 = (s32) var_a1->unk20;
    }
    if (temp_s0->unk0 & 4) {
        temp_t1 = var_a2->unk2C & 0xFFF;
        var_ft2 = (f32) temp_t1;
        if (temp_t1 < 0) {
            var_ft2 += 4294967296.0f;
        }
        temp_v0 = (u32) (temp_s0->unk24 * var_ft2);
        temp_s0->unk2C = temp_v0;
        temp_s0->unk2C = (u32) (temp_v0 | (var_a2->unk2C & ~0xFFF));
    } else {
        temp_s0->unk2C = (u32) var_a2->unk2C;
    }
    sp34 = (var_a2->unk30 - (D_8002C34C << 0x10)) + D_8002C34C;
    temp_v1 = temp_s0->unk0;
    var_a3 = var_a1->unk1C;
    if (temp_v1 & 0x20) {
        var_a3 = 0;
    }
    if (temp_v1 & 0x40) {
        temp_s0->unk2C = 0U;
        sp38 = var_a3;
        sp24 = var_a2;
        sp44 = var_a1;
        temp_v0_2 = func_8000D5C0(temp_s0->unk4, var_a1, var_a2, var_a3);
        var_a0 = temp_v0_2;
    }
    if (temp_s0->unk0 & 0x80) {
        temp_s0->unk2C = (u32) ((temp_s0->unk28 << 0x10) & 0x03FF0000);
        sp38 = var_a3;
        sp24 = var_a2;
        sp44 = var_a1;
        temp_v0_3 = func_8000D5C0(temp_s0->unk4, var_a1, var_a2, var_a3);
        var_a0 = temp_v0_3;
    }
    VI_DRAM_ADDR_REG = var_a0;
    VI_WIDTH_REG = var_a1->unk8;
    VI_BURST_REG = var_a1->unkC;
    VI_V_SYNC_REG = var_a1->unk10;
    VI_H_SYNC_REG = var_a1->unk14;
    VI_DRAM_ADDR_REG = var_a1->unk18;
    VI_H_START_REG = var_a3;
    VI_V_START_REG = sp34;
    VI_V_BURST_REG = var_a2->unk34;
    VI_INTR_REG = var_a2->unk38;
    VI_X_SCALE_REG = temp_s0->unk20;
    VI_H_START_REG = temp_s0->unk2C;
    VI_V_START_REG = temp_s0->unkC;
    temp_t2 = D_8002C460;
    D_8002C460 = temp_s0;
    D_8002C464 = temp_t2;
    M2C_MEMCPY_ALIGNED(temp_t2, temp_s0, 0x30);
}
