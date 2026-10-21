/* buildBattleTrackMaps (inlined) */
void func_8038CD04(void)
{
    s32 saveTrackNum;
    saveTrackNum = D_8014978C;
    for (D_8014978C = 6; D_8014978C <= 13; D_8014978C++) {
        if (!func_800F7644(D_8014978C)) continue;
        if (D_803B7CE4[D_8014978C].handle == -1) {
            D_803B7CE4[D_8014978C].handle = sign_extend_call(string_copy_format(D_803B7738[D_8014978C], 0, D_80140BDC - 1, 1), (u32)D_803B7CE4[D_8014978C].matrix, -1, 0);
            func_8008B32C((f32 (*)[3])D_8011418C, D_803B7CE4[D_8014978C].matrix, 1.0f);
            func_800B5898(-1.5707963705062866f, D_803B7CE4[D_8014978C].matrix);
            D_803B7CE4[D_8014978C].position[0] = D_803B837C[0];
            D_803B7CE4[D_8014978C].position[1] = D_803B837C[1];
            D_803B7CE4[D_8014978C].position[2] = D_803B837C[2];
        }
    }
    D_8014978C = saveTrackNum;
}
