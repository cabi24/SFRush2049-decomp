s64 func_8000D8F0(u64 arg0, ? arg1, s64 arg2, ? arg3) {
    return (s64) (arg0 >> arg2) >> 0x20;
}

s64 func_8000D91C(u64 arg0, ? arg1, u64 arg2, ? arg3) {
    return (s64) (arg0 % arg2) >> 0x20;
}

s64 func_8000D958(u64 arg0, ? arg1, u64 arg2, ? arg3) {
    return (s64) (arg0 / arg2) >> 0x20;
}

s64 func_8000D994(s64 arg0, ? arg1, s64 arg2, ? arg3) {
    return (s64) (arg0 << arg2) >> 0x20;
}

s64 func_8000D9C0(u64 arg0, ? arg1, u64 arg2, ? arg3) {
    return (s64) (arg0 % arg2) >> 0x20;
}

s64 func_8000D9FC(s64 arg0, ? arg1, s64 arg2, ? arg3) {
    if ((arg2 == -1) && (arg0 == (1 << 0x3F))) {
        M2C_BREAK(6);
    }
    return (s64) (arg0 / arg2) >> 0x20;
}

s64 func_8000DA58(s64 arg0, ? arg1, s64 arg2, ? arg3) {
    return (s64) (arg0 * arg2) >> 0x20;
}

void func_8000DA88(u64 *arg0, u64 *arg1, u64 arg2, ? arg3, s16 arg4) {
    *arg0 = arg2 / (u64) arg4;
    *arg1 = arg2 % (u64) arg4;
}

s32 func_8000DAE8(s64 arg0, ? arg1, s64 arg2, ? arg3) {
    s64 sp0;
    s64 temp_hi;

    temp_hi = arg0 % arg2;
    if ((arg2 == -1) && (arg0 == (1 << 0x3F))) {
        M2C_BREAK(6);
    }
    sp0 = temp_hi;
    if (((temp_hi < 0) && (arg2 > 0)) || ((sp0 > 0) && (arg2 < 0))) {
        sp0 += arg2;
    }
    return (s32) sp0;
}

s64 func_8000DB84(s64 arg0, ? arg1, s64 arg2, ? arg3) {
    return (s64) (arg0 >> arg2) >> 0x20;
}

s32 func_8000DBB0(void) {
    return M2C_ERROR(/* mfc0 $13 */);
}
