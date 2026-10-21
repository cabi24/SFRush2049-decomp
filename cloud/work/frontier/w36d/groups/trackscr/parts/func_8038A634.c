void func_8038A634(s32 initialize)
{
    u8 color[3];
    if (D_801427A0 < 19)
        D_801427A0 = D_8014978C;
    color[0] = D_80114658[D_801427A0].r;
    color[1] = D_80114658[D_801427A0].g;
    color[2] = D_80114658[D_801427A0].b;
    if (initialize) {
        D_803BA1E0[0] = color[0];
        D_803BA1E0[1] = color[1];
        D_803BA1E0[2] = color[2];
    } else {
        D_803BA1E0[0] += (color[0] - D_803BA1E0[0]) / 10;
        D_803BA1E0[1] += (color[1] - D_803BA1E0[1]) / 10;
        D_803BA1E0[2] += (color[2] - D_803BA1E0[2]) / 10;
    }
    D_803BA1E0[3] = D_8013F1D8 * 192 / 3;
    if (D_803B828C) {
        D_803BA1FC = D_803BA1E0;
        D_803BA1E0[4] = 0;
        D_803BA1E0[5] = 0;
        D_803BA1E0[6] = 0;
        D_803BA1E0[7] = 0;
        D_803B828C->Z = 0x7E00;
        D_803B828C->Alpha = D_803BA1E0[3];
        D_803B828C->Image = D_803BA1E8;
        Input_ApplyPadConfig(D_803B828C);
    }
}
