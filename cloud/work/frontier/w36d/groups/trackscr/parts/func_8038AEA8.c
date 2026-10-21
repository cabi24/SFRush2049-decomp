/* w36e body (cloud/work/frontier/w36e/groups/best_ts), merged 2026-10-21 */
/* loadTrackScreen (MAT3 trackscr.c, GC 0x8008a634): inlined into buildTrackMap (deleted stub 0x8038AEA8).
 * The GC poly locals stay declared (shared GameSource declarations); the N64 body makes blits. */
/* w36e final body (cloud/work/frontier/w36e/groups/best_ts) */
void func_8038AEA8(void)
{
    f32 polyVerts[4][3];
    u16 texIdx;
    Color backColor = D_803B8328;
    func_8038AE50();
    if (!((D_8014978C >= 0 && D_8014978C <= 5) ||
        (D_8014978C >= 6 && D_8014978C <= 13) ||
        (D_8014978C >= 14 && D_8014978C <= 17) ||
        (D_8014978C >= 18 && D_8014978C <= 18))) return;
    D_803B8288 = func_800B3704(D_803B7788[D_8014978C],176,32,0);
    D_803B8288->Z = 0x7F00;
    func_80094EC8(D_803B8288);
    D_80140BF0[D_803B8288->resource].flags |= 1;
    if (func_8038ACB0(14) == 1) {
        D_803B828C = func_800B3704(D_803B9260,176,32,0);
        D_803B828C->Z = 0x7E00;
        func_80094EC8(D_803B828C);
    }
}
