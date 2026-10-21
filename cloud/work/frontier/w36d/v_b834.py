C='ctx1.c'
OLD="""s32 func_8038B834(s32 index)
{
    s8 *option = &D_803B8314[index];
    if (!*option) return 0;
    if (!func_800F7644(index)) return 0;
    if (D_8014A110 == 4) return option >= D_803B8314 + 14 && option < D_803B8314 + 18;
    if (D_8014A110 == 5) return option >= D_803B8314 + 18 && option < D_803B8314 + 19;
    if (D_8014A110 == 6) return option >= D_803B8314 + 6 && option < D_803B8314 + 14;
    return option >= D_803B8314 && option < D_803B8314 + 6;
}"""
NAT="""s32 func_8038B834(s32 track)
{
    if (!D_803B8314[track]) return 0;
    if (!func_800F7644(track)) return 0;
    if (D_8014A110 == 4) return track >= 14 && track <= 17;
    if (D_8014A110 == 5) return track >= 18 && track <= 18;
    if (D_8014A110 == 6) return track >= 6 && track <= 13;
    return track >= 0 && track <= 5;
}"""
VARIANTS = {
 'base': [],
 'natural': [(C, OLD, NAT)],
}
