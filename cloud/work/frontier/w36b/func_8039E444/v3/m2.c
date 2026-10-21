typedef struct CompRec { s16 idx, level; f32 uvs[9]; f32 pos[3]; } CompRec;
#define gCarComponents ((CompRec (*)[11])D_803B2618)
typedef struct CompLightDir { u8 col[3], pad0, colc[3], pad1; s8 dir[3], pad2; u8 pad3[4]; } CompLightDir;
typedef struct CompLight { u8 col[3], pad0, colc[3], pad1; CompLightDir l[2]; } CompLight;
typedef struct ObjInstance { void *mat; u8 unknown04[64]; } ObjInstance;
typedef struct ZMatrix { f32 uvs[9]; f32 pos[3]; } ZMatrix;
CompLight D_803B2858[2] = {
    {{0, 128, 0}, 0, {0, 128, 0}, 0, {{{192, 192, 192}, 0, {192, 192, 192}, 0, {0, 100, 0}, 0}, {{0}}}},
    {{255, 0, 0}, 0, {255, 0, 0}, 0, {{{192, 192, 192}, 0, {192, 192, 192}, 0, {0, 100, 0}, 0}, {{0}}}},
};
extern s16 D_803B2600[11];
extern char *D_803B25EC[5];
extern char D_803B85CC[];
extern ObjInstance D_8012E708[];
extern void func_800A61B0(f32 *, f32 *, f32 *);
extern f32 func_800BEA30(void);
extern s32 engine_rpm_calc(s32, void *, s32, s32, s32);
extern s32 sprintf(char *, const char *, ...);
extern void car_mass_set(s32, s32);
extern void func_80090E9C(f32, void *);
extern void func_800A79B8(s32, s32, CompLight *);
extern ZMatrix *func_8008B3A0(s16);
void func_8039E444(s32 p)
{
    s32 i;
    f32 lightVec[3] = {-0.57f, 0.57f, -0.57f};
    f32 lightDir[3];
    CarHandles *car;
    s16 currentLevel;
    s32 whichLight;
    f32 compScale;
    char componentName[16];
    s32 mask;
    if (D_8014A108 > 1 || D_8014A110 == 2) return;
    if (D_803BA028[p] == 1 || (D_803BA060[p][D_803B28B8[p]] != 1 && D_803BA060[p][D_803B28B8[p]] != 2 && D_803BA060[p][D_803B28B8[p]] != 3 && D_803BA060[p][D_803B28B8[p]] != 4 && D_803BA060[p][D_803B28B8[p]] != 5)) {
        for (i = 0; i < 11; i++) {
            if (gCarComponents[p][i].idx != -1) {
                model_data_load(gCarComponents[p][i].idx, 1, 15);
                car_mass_set(gCarComponents[p][i].idx, -1);
            }
        }
        return;
    }
    car = &D_80139320[p * 13 + D_803B9FD0[p]];
    if (D_803B25C8 == -1) {
        D_803B25C8 = engine_rpm_calc(string_copy_format(D_803B85CC, 0, D_80140BDC - 1, 1), func_8008B3A0((s16)car->main), 15, -1, 0);
        model_data_load(D_803B25C8, 1, 15);
    } else {
        D_8012E708[D_803B25C8].mat = func_8008B3A0((s16)car->main);
    }
    D_803B2858[1].col[0] = 255.0f * func_800BEA30();
    func_800A61B0(lightVec, lightDir, func_8008B3A0((s16)car->main)->uvs);
    D_803B2858[0].l[0].dir[0] = lightVec[0] * 10.0f;
    D_803B2858[0].l[0].dir[1] = lightVec[1] * 10.0f;
    D_803B2858[0].l[0].dir[2] = lightVec[2] * 10.0f;
    D_803B2858[1].l[0].dir[0] = lightVec[0] * 10.0f;
    D_803B2858[1].l[0].dir[1] = lightVec[1] * 10.0f;
    D_803B2858[1].l[0].dir[2] = lightVec[2] * 10.0f;
    for (i = 0; i < 11; i++) {
        currentLevel = -1;
        compScale = 1.0f;
        whichLight = 0;
        switch (D_803B2600[i]) {
        case 0:
            if (D_803BA060[p][D_803B28B8[p]] == 3) {
                whichLight = 1;
                compScale = 0.1f * func_800BEA30() + 1.0f;
            }
            currentLevel = D_80111080[p+1][D_803B9FD0[p]] % 5;
            break;
        case 1:
            if (D_803BA060[p][D_803B28B8[p]] == 5) {
                whichLight = 1;
                compScale = 0.1f * func_800BEA30() + 1.0f;
            }
            currentLevel = D_80111230[p+1][D_803B9FD0[p]] % 3;
            break;
        case 2:
            if (D_803BA060[p][D_803B28B8[p]] == 2) {
                whichLight = 1;
                compScale = 0.1f * func_800BEA30() + 1.0f;
            }
            currentLevel = D_8011103C[p+1][D_803B9FD0[p]] % 3;
            break;
        case 3:
            if (D_803BA060[p][D_803B28B8[p]] == 4) {
                whichLight = 1;
                compScale = 0.1f * func_800BEA30() + 1.0f;
            }
            currentLevel = D_8011119C[p+1][D_803B9FD0[p]] % 4;
            break;
        case 4:
            if (D_803BA060[p][D_803B28B8[p]] == 1) {
                whichLight = 1;
                compScale = 0.1f * func_800BEA30() + 1.0f;
            }
            currentLevel = D_80110E78[p+1][D_803B9FD0[p]] % 2;
            break;
        }
        if (currentLevel != gCarComponents[p][i].level) {
            sprintf(componentName, D_803B25EC[D_803B2600[i]], currentLevel + 1);
            if (gCarComponents[p][i].idx != -1) {
                sound_call_minimal(gCarComponents[p][i].idx);
                gCarComponents[p][i].idx = -1;
                gCarComponents[p][i].level = -1;
            }
            gCarComponents[p][i].idx = sign_extend_call(string_copy_format(componentName, 0, D_80140BDC - 1, 1), gCarComponents[p][i].uvs, -1, 0);
            model_data_load(gCarComponents[p][i].idx, 1, 15);
            if (D_80151AD0 == 1) {
                gCarComponents[p][i].pos[0] = 0.0f;
                gCarComponents[p][i].pos[1] = 1.0f;
                gCarComponents[p][i].pos[2] = 15.0f;
            } else {
                gCarComponents[p][i].pos[0] = 0.0f;
                gCarComponents[p][i].pos[1] = 2.0f;
                gCarComponents[p][i].pos[2] = 15.0f;
            }
            gCarComponents[p][i].level = currentLevel;
        }
        switch (i) {
        case 0: case 1: case 10:
            func_8008B32C(D_8011418C, gCarComponents[p][i].uvs, compScale);
            gCarComponents[p][i].pos[0] = 0.f;
            gCarComponents[p][i].pos[1] = 0.f;
            gCarComponents[p][i].pos[2] = 0.f;
            gCarComponents[p][i].pos[1] += 0.1f;
            break;
        case 2: case 6:
            func_8008B32C(func_8008B3A0((s16)car->parts[3])->uvs, gCarComponents[p][i].uvs, compScale);
            gCarComponents[p][i].pos[0] = func_8008B3A0((s16)car->parts[3])->pos[0];
            gCarComponents[p][i].pos[1] = func_8008B3A0((s16)car->parts[3])->pos[1];
            gCarComponents[p][i].pos[2] = func_8008B3A0((s16)car->parts[3])->pos[2];
            func_80090E9C(3.1415927f, gCarComponents[p][i].uvs);
            break;
        case 3: case 7:
            func_8008B32C(func_8008B3A0((s16)car->parts[4])->uvs, gCarComponents[p][i].uvs, compScale);
            gCarComponents[p][i].pos[0] = func_8008B3A0((s16)car->parts[4])->pos[0];
            gCarComponents[p][i].pos[1] = func_8008B3A0((s16)car->parts[4])->pos[1];
            gCarComponents[p][i].pos[2] = func_8008B3A0((s16)car->parts[4])->pos[2];
            break;
        case 5: case 9:
            func_8008B32C(func_8008B3A0((s16)car->parts[5])->uvs, gCarComponents[p][i].uvs, compScale);
            gCarComponents[p][i].pos[0] = func_8008B3A0((s16)car->parts[5])->pos[0];
            gCarComponents[p][i].pos[1] = func_8008B3A0((s16)car->parts[5])->pos[1];
            gCarComponents[p][i].pos[2] = func_8008B3A0((s16)car->parts[5])->pos[2];
            func_80090E9C(3.1415927f, gCarComponents[p][i].uvs);
            break;
        case 4: case 8:
            func_8008B32C(func_8008B3A0((s16)car->parts[6])->uvs, gCarComponents[p][i].uvs, compScale);
            gCarComponents[p][i].pos[0] = func_8008B3A0((s16)car->parts[6])->pos[0];
            gCarComponents[p][i].pos[1] = func_8008B3A0((s16)car->parts[6])->pos[1];
            gCarComponents[p][i].pos[2] = func_8008B3A0((s16)car->parts[6])->pos[2];
            break;
        }
        car_mass_set(gCarComponents[p][i].idx, (s16)D_803B25C8);
        model_transform_setup(gCarComponents[p][i].idx, 0, mask = 1 << p);
        func_800A79B8(gCarComponents[p][i].idx, 1, &D_803B2858[whichLight]);
    }
}
