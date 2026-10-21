/* OptionValid: MAT3 trackscr.c OptionValid (GC 0x8008a324) direct port; N64 adds the track-5 and D_803AF980 tests. */
s32 func_8038ACB0(s32 opt)
{
    s32 valid;
    valid = D_803B77D4[D_8014A110][opt];
    if (D_8014A110 == 0 && opt == 17) valid = D_8014A108 < 3;
    if (D_8014A110 == 4) {
        if (opt == 5) valid = D_80146108[32] == 1;
        if (opt == 6) valid = D_80146108[32] == 0;
        if (opt == 7) valid = D_80146108[32] == 2;
    }
    if (D_8014A110 == 6) {
        if (opt == 9) valid = D_80146108[37] == 0;
        if (opt == 11) valid = D_80146108[37] == 0;
        if (opt == 10) valid = D_80146108[37] == 1;
    }
    if (opt == 19 && D_8014A108 == 1) valid = 0;
    if (opt == 12) valid = D_8014978C >= 0 && D_8014978C <= 5;
    if (!D_80156994 && D_8014978C == 5 && opt != 0) valid = 0;
    if (!D_803AF980 && opt == 1) valid = 0;
    return valid;
}
