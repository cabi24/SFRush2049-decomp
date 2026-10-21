/* TrackValid: MAT3 trackscr.c TrackValid (GC 0x8008a248) direct port; IDO rewrites the range tests as pointer compares on &trackReady[track]. */
s32 func_8038B834(s32 track)
{
    if (!D_803B8314[track]) return 0;
    if (!func_800F7644(track)) return 0;
    if (D_8014A110 == 4) return track >= 14 && track <= 17;
    if (D_8014A110 == 5) return track >= 18 && track <= 18;
    if (D_8014A110 == 6) return track >= 6 && track <= 13;
    return track >= 0 && track <= 5;
}
