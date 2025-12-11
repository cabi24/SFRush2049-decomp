u8 *func_80007C00(u8 *arg0, s32 arg1) {
    u8 *var_a0;
    u8 var_v1;

    var_a0 = arg0;
    var_v1 = *var_a0;
    if ((arg1 & 0xFF) != var_v1) {
loop_1:
        if (var_v1 == 0) {
            return NULL;
        }
        var_v1 = var_a0->unk1;
        var_a0 += 1;
        if ((arg1 & 0xFF) == var_v1) {
            /* Duplicate return node #4. Try simplifying control flow for better match */
            return var_a0;
        }
        goto loop_1;
    }
    return var_a0;
}

s32 func_80007C40(u8 *arg0) {
    u8 *var_v1;
    u8 temp_t7;

    var_v1 = arg0;
    if (*arg0 != 0) {
        do {
            temp_t7 = var_v1->unk1;
            var_v1 += 1;
        } while (temp_t7 != 0);
    }
    return var_v1 - arg0;
}

void *func_80007C68(void *arg0, u8 *arg1, s32 arg2) {
    s32 var_a2;
    u8 *var_v1;
    u8 temp_t6;
    void *var_v0;

    var_a2 = arg2;
    var_v0 = arg0;
    var_v1 = arg1;
    if (var_a2 != 0) {
        do {
            temp_t6 = *var_v1;
            var_a2 -= 1;
            var_v0 += 1;
            var_v1 += 1;
            var_v0->unk-1 = temp_t6;
        } while (var_a2 != 0);
    }
    return arg0;
}
