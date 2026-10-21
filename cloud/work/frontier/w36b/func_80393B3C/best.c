/* func_80393B3C = initRecordScreen@recordscr.c (MAT3 reference/repos/mat3/decomp/src/GameSource/src/recordscr.c, GC 0x8008077C)
 * with initButtons@recordscr.c (GC 0x80084A5C) inlined; w36b direct typed port. The caller-less `jr ra; nop` at 0x80393B34
 * is the deleted procedure of the inlined initButtons: it is defined here as the non-static func_80393B34 (not on keep, so
 * uld/umerge inline it exactly as a static) so that its stub carries a symbol and cannot be counted as extra words of a
 * neighbouring member (w17j packaging rule). Names: gSelectMenuSelect D_803BA808, gStatPlayer D_803BA800,
 * setRecordState func_803931C0, createBG particle_lifetime_set, buttonInfo D_803B5F28 (23 x 64 B), buttonNames D_803B5EF0,
 * MBOX_NewObject sign_extend_call, MBOX_FindObject_WORK string_copy_format, ZOID_SetColor func_8008E06C, MBOX_HideObject
 * model_data_load, MBOX_ShowObject model_transform_setup, MBOX_FindTexture_WORK func_800B24EC, ZOID_SetTexture func_8008D870,
 * WPitchUV/WRollUV func_800B5898/func_800B5940, ScaleUV func_8008B32C, buttonsInited/JustInited D_803BA7D0/D_803BA7D4,
 * ZeroCameras particle_velocity_set, InitPane func_800B3B4C (viewPane D_803B6530), NewMultiBlit sound_control (blit
 * D_803B652C, mblit D_803B6504), gDeleteConfirm/gDeleteNotify/gYes/gSelectMenuClear D_803BA84D/854/850/80C, inited D_803B6528.
 * N64 differences from the GC bodies (from the retail words): no gSelectPakIdx store; the button y is 55.2f (GC 59.2f);
 * where the GC object has its "no-op copies" (records 17, 21, 22) the N64 multiplies all three position coordinates by a
 * constant 1.0: written `*= (1.0f / 1.0f)`, a constant expression that cfe folds to 1.0f but does not treat as the literal
 * 1.0f (a plain `*= 1.0f` is deleted; a propagated `f32 scale = 1.0f` keeps the mul but stores through absolute addresses,
 * retail stores through the s0 base). initRecordScreen is defined before initButtons, as in the MAT3 file: as1 breaks
 * scheduling ties by source line and the &buttonColor address (caller line) must win over the loop preheader.
 * Own data: .data white colour template {255,255,255,255} (retail 0x803B65A8 = FFFFFFFF) and 7 .rodata floats
 * (-1.5707964f 55.2f 75.1f pi 87.1f -75.1f -87.1f at 0x803B9454..0x803B946C), all checked against image A (tools/rocheck.py). */
void func_80393B34(void);

void func_80393B3C(void)
{
    D_803BA808 = 0;
    D_803BA800 = 0;
    func_803931C0(0);
    particle_lifetime_set();
    func_80393B34();
    D_803B6530 = func_800B3B4C(-20, 0, -10, 10, 192, 0, 0, 0);
    D_803B652C = sound_control(0, 0, D_803B6504, 1);
    D_803BA84D = 0;
    D_803BA854 = 0;
    D_803BA850 = 0;
    D_803BA80C = 0;
    D_803B6528 = 1;
}

void func_80393B34(void)
{
    Color4 buttonColor = {255, 255, 255, 255};
    void *tex;
    s16 texIdx;
    s32 buttonIdx;
    for (buttonIdx = 0; buttonIdx < 23; buttonIdx++) {
        if (D_803B5F28[buttonIdx].fields[1] == -1) {
            memcpy(D_803B5F28[buttonIdx].matrix, D_8011418C, 36);
            D_803B5F28[buttonIdx].fields[1] = sign_extend_call(
                string_copy_format(D_803B5EF0[D_803B5F28[buttonIdx].fields[0]], 0, D_80140BDC - 1, 1),
                D_803B5F28[buttonIdx].matrix, -1,
                (D_803B5F28[buttonIdx].fields[0] == 0 || D_803B5F28[buttonIdx].fields[0] == 1 ||
                 D_803B5F28[buttonIdx].fields[0] == 2 || D_803B5F28[buttonIdx].fields[0] == 7 ||
                 D_803B5F28[buttonIdx].fields[0] == 8 || D_803B5F28[buttonIdx].fields[0] == 9 ||
                 D_803B5F28[buttonIdx].fields[0] == 10 || D_803B5F28[buttonIdx].fields[0] == 11) ? 0x42000 : 0);
            func_8008E06C(D_803B5F28[buttonIdx].fields[1], &buttonColor);
            model_data_load(D_803B5F28[buttonIdx].fields[1], 1, 15);
            model_transform_setup(D_803B5F28[buttonIdx].fields[1], 0, 15);
        }
    }
    tex = func_800B24EC(D_803B8C10, &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B5F28[17].fields[1], tex, -1);
    memcpy(D_803B5F28[17].matrix, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B5F28[17].matrix);
    D_803B5F28[17].position[0] = 0.0f;
    D_803B5F28[17].position[1] = 115.0f;
    D_803B5F28[17].position[2] = 200.0f;
    D_803B5F28[17].position[0] *= (1.0f / 1.0f);
    D_803B5F28[17].position[1] *= (1.0f / 1.0f);
    D_803B5F28[17].position[2] *= (1.0f / 1.0f);
    tex = func_800B24EC(D_803B8C20, &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B5F28[18].fields[1], tex, -1);
    tex = func_800B24EC(D_803B8C28, &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B5F28[19].fields[1], tex, -1);
    memcpy(D_803B5F28[19].matrix, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B5F28[19].matrix);
    D_803B5F28[19].position[0] = 75.1f;
    D_803B5F28[19].position[1] = 55.2f;
    D_803B5F28[19].position[2] = 100.0f;
    func_8008B32C(D_803B5F28[19].matrix, D_803B5F28[19].matrix, 0.5f);
    memcpy(D_803B5F28[20].matrix, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B5F28[20].matrix);
    func_800B5940(3.1415927f, D_803B5F28[20].matrix);
    D_803B5F28[20].position[0] = 87.1f;
    D_803B5F28[20].position[1] = 55.2f;
    D_803B5F28[20].position[2] = 100.0f;
    tex = func_800B24EC(D_803B8C2C, &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B5F28[21].fields[1], tex, -1);
    memcpy(D_803B5F28[21].matrix, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B5F28[21].matrix);
    D_803B5F28[21].position[0] = -75.1f;
    D_803B5F28[21].position[1] = 55.2f;
    D_803B5F28[21].position[2] = 100.0f;
    D_803B5F28[21].position[0] *= (1.0f / 1.0f);
    D_803B5F28[21].position[1] *= (1.0f / 1.0f);
    D_803B5F28[21].position[2] *= (1.0f / 1.0f);
    func_8008B32C(D_803B5F28[21].matrix, D_803B5F28[21].matrix, 0.5f);
    memcpy(D_803B5F28[22].matrix, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B5F28[22].matrix);
    D_803B5F28[22].position[0] = -87.1f;
    D_803B5F28[22].position[1] = 55.2f;
    D_803B5F28[22].position[2] = 100.0f;
    D_803B5F28[22].position[0] *= (1.0f / 1.0f);
    D_803B5F28[22].position[1] *= (1.0f / 1.0f);
    D_803B5F28[22].position[2] *= (1.0f / 1.0f);
    D_803BA7D0 = 1;
    D_803BA7D4 = 1;
    particle_velocity_set();
}
