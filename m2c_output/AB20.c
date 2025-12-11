? func_80007270(s32, ?, ?);                         /* extern */
? func_8000E4B0(?, void *, ? *, s32);               /* extern */
? func_8000E5B0(s32);                               /* extern */
? func_8000E5F4();                                  /* extern */
s32 func_8000E850(void *, ?);                       /* extern */
s32 func_8000E8D0(s32, s32, ?, ? *);                /* extern */
s32 func_8000F124();                                /* extern */
s32 func_8000F9F0(?, s32);                          /* extern */
extern s8 D_80037AE0;
extern ? D_80037B30;

s32 func_80009F20(void *arg0, s32 arg1) {
    void *sp24;
    s32 temp_t2;
    s32 var_s0;
    s32 var_v1;
    void *temp_t0;
    void *temp_t0_2;
    void *var_v0;

    temp_t0 = (arg0->unk8 << 6) + &D_80037B30;
    if (!(arg0->unk0 & 8)) {
        return 5;
    }
    sp24 = temp_t0;
    func_8000E5B0(arg1);
    (&D_80037B30 + (arg0->unk8 << 6))->unk3C = 1;
    var_v1 = 0;
    temp_t0_2 = temp_t0 + arg0->unk8;
    var_v0 = temp_t0_2;
    do {
        var_v1 += 4;
        var_v0->unk7 = (s8) arg1;
        var_v0->unk8 = (s8) arg1;
        var_v0->unk9 = (s8) arg1;
        var_v0 += 4;
        var_v0->unk2 = (s8) arg1;
    } while (var_v1 != 0x20);
    D_80037AE0 = 0xFE;
    sp24 = temp_t0_2;
    func_8000E4B0(1, &D_80037B30 + (arg0->unk8 << 6), &D_80037B30, arg1);
    func_80007270(arg0->unk4, 0, 1);
    func_8000E4B0(0, (arg0->unk8 << 6) + &D_80037B30);
    func_80007270(arg0->unk4, 0, 1);
    temp_t2 = temp_t0_2->unk2 & 0xC0;
    var_s0 = temp_t2;
    if (temp_t2 == 0) {
        if (arg1 == 0) {
            if (temp_t0_2->unk26 != 0) {
                goto block_9;
            }
        } else if (temp_t0_2->unk26 != 0xEB) {
block_9:
            var_s0 = 4;
        }
    }
    func_8000E5F4();
    return var_s0;
}

s32 func_8000A088(s32 arg0, u8 *arg1) {
    u8 *sp4C;
    s8 sp29;
    s8 sp28;
    s8 sp27;
    s8 sp26;
    s8 sp25;
    u8 sp24;
    s32 temp_a1;
    s32 temp_at;
    s32 var_v0;
    u8 *temp_t4;
    u8 *var_t2;
    u8 *var_t5;
    u8 *var_v1;

    sp24 = 0xFF;
    sp25 = 0x23;
    sp26 = 1;
    sp27 = 3;
    sp28 = 0xC0;
    sp4C = arg1;
    var_v0 = func_8000F9F0(0x600, arg0);
    sp29 = var_v0 | 0xC000;
    var_v1 = sp4C;
    if (arg0 != 0) {
        var_v0 = 0;
        if (arg0 > 0) {
            temp_a1 = arg0 & 3;
            if (temp_a1 != 0) {
                do {
                    var_v0 += 1;
                    *var_v1 = 0;
                    var_v1 += 1;
                } while (temp_a1 != var_v0);
                if (var_v0 != arg0) {
                    goto loop_7;
                }
                var_v0 += 4;
            } else {
                do {
loop_7:
                    var_v0 += 4;
                    var_v1->unk1 = 0;
                    var_v1->unk2 = 0;
                    var_v1->unk3 = 0;
                    var_v1 += 4;
                    var_v1->unk-4 = 0;
                } while (var_v0 != arg0);
            }
        }
    }
    var_t2 = &sp24;
    temp_t4 = var_t2 + 0x24;
    var_t5 = var_v1;
    do {
        temp_at = *var_t2;
        var_t2 += 0xC;
        var_t5 += 0xC;
        var_t5->unk-C = (unaligned s32) temp_at;
        var_t5->unk-8 = (unaligned s32) var_t2->unk-8;
        var_t5->unk-4 = (unaligned s32) var_t2->unk-4;
    } while (var_t2 != temp_t4);
    var_t5->unk0 = var_t2->unk0;
    var_t5->unk1 = (u8) var_t2->unk1;
    var_t5->unk2 = (u8) var_t2->unk2;
    var_v1->unk27 = 0xFE;
    return var_v0;
}

s32 func_8000A194(s32 arg0, void *arg1, s32 arg2) {
    ? sp24;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 var_v0;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;

    arg1->unk4 = arg0;
    arg1->unk65 = 0xFF;
    arg1->unk0 = 0;
    arg1->unk8 = arg2;
    var_v0 = func_8000E850(arg1, 0xFE);
    var_v1_4 = var_v0;
    if (var_v0 == 2) {
        var_v0 = func_8000E850(arg1, 0x80);
        var_v1_4 = var_v0;
    }
    if (var_v0 != 0) {
        return var_v1_4;
    }
    temp_v0 = func_8000E8D0(arg0, arg2, 0x400, &sp24);
    var_v1_3 = temp_v0;
    if (temp_v0 == 2) {
        var_v1_3 = 4;
    }
    if (var_v1_3 != 0) {
        return var_v1_3;
    }
    if (sp43 == 0xFE) {
        return 0xB;
    }
    temp_v0_2 = func_8000E850(arg1, 0x80);
    var_v1_2 = temp_v0_2;
    if (temp_v0_2 == 2) {
        var_v1_2 = 4;
    }
    if (var_v1_2 != 0) {
        return var_v1_2;
    }
    temp_v0_3 = func_8000E8D0(arg0, arg2, 0x400, &sp24);
    var_v1 = temp_v0_3;
    if (temp_v0_3 == 2) {
        var_v1 = 4;
    }
    if (var_v1 != 0) {
        return var_v1;
    }
    if (sp43 != 0x80) {
        return 0xB;
    }
    if (!(arg1->unk0 & 8)) {
        func_8000A088(arg2, (arg2 << 6) + &D_80037B30);
    }
    arg1->unk0 = 8;
    return 0;
}

s32 func_8000A2F0(s32 *arg0) {
    s32 temp_v0;
    s32 var_v1;

    var_v1 = 5;
    if (*arg0 & 5) {
        temp_v0 = func_8000F124();
        var_v1 = temp_v0;
        if (temp_v0 == 0) {
            *arg0 &= ~4;
        }
    }
    return var_v1;
}
