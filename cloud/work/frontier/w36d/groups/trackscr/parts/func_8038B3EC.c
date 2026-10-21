/* w36e final body (cloud/work/frontier/w36e/groups/best_ts) */
void func_8038B3EC(void)
{
    static f32 cityPos[3] = { -100.0f, 30.0f, 200.0f };
    static f32 mapPos[3] = { 50.0f, -15.0f, 100.0f };
    static f32 rotAngle = 0.0f;
    rotAngle += D_8002EB94 * 6.2831854820251465f / 10.0f;
    if (rotAngle > 6.2831854820251465f) rotAngle -= 6.2831854820251465f;
    if (D_803BAC78 != D_803BAC90) {
        D_803BACCC -= D_8002EB94 * 4.0f;
        if (D_803BACCC < 0.0f) {
            D_803BACCC = 0.0f;
            func_8038B388();
            func_8038AEB0();
        }
    } else {
        D_803BACCC += D_8002EB94 * 4.0f;
        if (D_803BACCC > 1.0f) D_803BACCC = 1.0f;
    }
    D_803B7CE4[19].position[0] = cityPos[0];
    D_803B7CE4[19].position[1] = cityPos[1];
    D_803B7CE4[19].position[2] = cityPos[2];
    D_803B7CE4[19].position[0] += D_803B7CE4[D_803BAC78].offset_x;
    D_803B7CE4[19].position[1] += D_803B7CE4[D_803BAC78].offset_y;
    D_803B7CE4[19].position[0] *= 1.0f - D_803BACCC;
    D_803B7CE4[19].position[1] *= 1.0f - D_803BACCC;
    D_803B7CE4[19].position[2] *= 1.0f - D_803BACCC;
    D_803B7CE4[19].position[0] += mapPos[0] * D_803BACCC;
    D_803B7CE4[19].position[1] += mapPos[1] * D_803BACCC;
    D_803B7CE4[19].position[2] += mapPos[2] * D_803BACCC;
    D_803BAC30[9] = D_803B7CE4[19].position[0];
    D_803BAC30[10] = D_803B7CE4[19].position[1];
    D_803BAC30[11] = D_803B7CE4[19].position[2];
    func_8008B32C((f32 (*)[3])D_8011418C, (f32 (*)[3])D_803BAC30,
        D_803BACCC * D_803B7CE4[D_803BAC78].scale_to + D_803B7CE4[D_803BAC78].scale_from * (1.0f - D_803BACCC));
    func_8008B32C((f32 (*)[3])D_8011418C, (f32 (*)[3])D_803B8254,
        D_803BACCC * D_803B7CE4[D_803BAC78].scale_to + D_803B7CE4[D_803BAC78].scale_from * (1.0f - D_803BACCC));
    if ((D_8014A110 == 3 && D_801543D8[D_80154628].mirror == 1)
        || (D_8014A110 != 3 && D_8014A110 != 4 && D_8014A110 != 6 && D_8014A110 != 5 && D_80146108[28])) {
        func_800FD754((f32 (*)[3])D_803BAC30, (f32 (*)[3])D_803BAC30, -1.0f, 1.0f, 1.0f);
        func_800FD754((f32 (*)[3])D_803B8254, (f32 (*)[3])D_803B8254, -1.0f, 1.0f, 1.0f);
    }
    gfx_setup_fc(rotAngle * D_803BACCC + D_803B7CE4[D_803BAC78].angle * (1.0f - D_803BACCC), (f32 (*)[3])D_803BAC30);
    func_800B5898(-0.5235987901687622f * D_803BACCC + (1.0f - D_803BACCC) * -1.5707963705062866f, (f32 (*)[3])D_803BAC30);
    gfx_setup_fc(rotAngle * D_803BACCC + D_803B7CE4[D_803BAC78].angle * (1.0f - D_803BACCC), (f32 (*)[3])D_803B8254);
    func_800B5898(-0.5235987901687622f * D_803BACCC + (1.0f - D_803BACCC) * -1.5707963705062866f, (f32 (*)[3])D_803B8254);
}
