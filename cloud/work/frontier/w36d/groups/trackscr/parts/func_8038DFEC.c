/* _InitScreen (MAT3 trackscr.c, GC 0x800887ac). N64: 17-entry multiblit; BlowFog and setCamera inlined. */
void func_8038DFEC(void)
{
    s32 i;
    s32 j;
    f32 pos;
    D_803BA85A = 0;
    D_803BA878 = 0;
    D_803BA910 = 0;
    D_803BAC78 = -1;
    D_803BA898 = 0; for (i = 0; i < 21; i++) if (func_8038ACB0(i)) D_803BA8B0[D_803BA898++] = i;
    func_8038CD14();
    if (D_8014A110 == 3) D_803BA84E = D_8014978C = D_801543D8[D_80154628].trackNum;
    else {
        if (D_8014A110 == 4) D_803BA84E = D_8014978C = func_800CDC3C(D_8014A118[0].save, D_8014A110) + 14;
        else if (D_8014A110 == 5) D_803BA84E = D_8014978C = func_800CDC3C(D_8014A118[0].save, D_8014A110) + 18;
        else if (D_8014A110 == 6) D_803BA84E = D_8014978C = func_800CDC3C(D_8014A118[0].save, D_8014A110) + 6;
        else D_803BA84E = D_8014978C = func_800CDC3C(D_8014A118[0].save, D_8014A110) + 0;
        if (!func_8038B834(D_8014978C)) {
            if (D_8014A110 == 4) D_803BA84E = D_8014978C = 14;
            else if (D_8014A110 == 5) D_803BA84E = D_8014978C = 18;
            else if (D_8014A110 == 6) D_803BA84E = D_8014978C = 6;
            else D_803BA84E = D_8014978C = 0;
        }
        for (i = 0; i < D_8014A108; i++) {
            if (D_8014A110 == 4) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 14);
            else if (D_8014A110 == 5) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 18);
            else if (D_8014A110 == 6) menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C - 6);
            else menu_text_input(D_8014A118[i].save, D_8014A110, D_8014978C);
        }
        for (i = 21; i < 41; i++)
            D_80146108[i] = reverb_setup(D_8014A118[0].save, i);
    }
    init_state_begin();
    particle_lifetime_set();
    func_8038C93C();
    D_803B7CD0 = sound_control(0, 0, D_803B7A6C, 17);
    i = 0;
    for (j = 0; j < 19; j++) {
        if (func_8038B834(j)) {
            if (j == D_8014978C) { D_803BA856 = i; break; }
            i++;
        }
    }
    D_803BA852 = 19;
    i = 0;
    pos = 0.0f;
    for (j = 0; j < 19; j++) {
        if (func_8038B834(j)) {
            D_803BA918[i].gTrackNum = j;
            D_803BA918[i].slot = i - D_803BA856;
            if (D_803BA918[i].slot < 0) D_803BA918[i].slot += D_803BA852;
            if (D_803BA856 == i) D_803BA918[i].targetPos = 0.0f;
            else if (D_803BA856 >= D_803BA852/2) D_803BA918[i].targetPos = pos - (D_803BA852 - D_803BA856)*100;
            else D_803BA918[i].targetPos = pos + D_803BA856*100;
            D_803BA918[i].pos[0] = D_803BA918[i].targetPos;
            D_803BA918[i].pos[1] = -5.0f;
            D_803BA918[i].pos[2] = 0.0f;
            pos -= 100.0f;
            if (pos < -(D_803BA852*100/2)) pos += D_803BA852*100;
            i++;
        }
    }
    D_803BAE10[0] = 0.0f;
    D_803BAE10[1] = D_803B7A20[D_803BA918[D_803BA856].gTrackNum] - 5.0f;
    D_803BAE10[2] = 0.0f;
    func_8038A634(1);
    func_8038C934();
    func_8038A62C();
    entity_audio_update();
    D_803B7CD4 = 1;
    D_803BAC78 = -1;
    D_803BAC90 = D_8014978C;
    D_803BACCC = 0;
}
