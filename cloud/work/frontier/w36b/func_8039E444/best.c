/* func_8039E444 = updateCarComponents@carscr.c (MAT3 reference/repos/mat3/decomp/src/GameSource/src/carscr.c, GC 0x800910F8,
 * size 0x830), w36b: direct typed port of the MAT3 body, replacing w35b's context stand-in. Private-ABI member: p arrives in s6
 * from its caller func_8039EF08 (showCarSelect); every callee-saved register is clobbered.
 * Names: gCarComponents = D_803B2618 ([4][11] x 52 B: idx, level, mat), compLight = D_803B2858 (2 x 40 B, Lights2 layout),
 * componentToName = D_803B2600 (s16[11]), carComponentNames = D_803B25EC, gCarPartFrame = D_803B25C8, gCarParts = D_80139320
 * (CarHandles; frame = main, tires[k] = parts[3+k]), ObjInstanceList = D_8012E708 (68 B), MBOX_HideObject model_data_load,
 * ZOID_SetObjectParent car_mass_set, MBOX_NewObjectCam engine_rpm_calc, MBOX_NewObject sign_extend_call, MBOX_FindObject_WORK
 * string_copy_format, ZOID_FindMatrix func_8008B3A0, TB_GetFlashTextCycle func_800BEA30, BodyVector func_800A61B0,
 * ZOID_RemoveObject sound_call_minimal, ScaleUV func_8008B32C, YawUV func_80090E9C, MBOX_ShowObject model_transform_setup,
 * ZOID_SetObjLights func_800A79B8, gThisCar D_803B9FD0, gOpts D_803BA060, gActiveOption D_803B28B8, gCarLocked D_803BA028.
 * N64 differences from the GC body (all from the retail words):
 *  - compLight is the N64 Lights2 (bytes): the flash writes compLight[1] ambient col[0] = 255.0f * cycle (u8), and both
 *    lights' dir bytes are (s8)(lightVec[k] * 10.0f) computed from lightVec itself; BodyVector still writes a scratch
 *    vector (lightDir, never read: the retail reloads lightVec after the call). No ZOID_SetColor / buttonColor, no printf.
 *  - compLight is DEFINED here (initialiser = image A bytes at 0x803B2858): the shared `lui at` of the six dir-byte stores
 *    only appears when the symbol is defined in the unit (w15d rule); as extern every store gets its own lui.
 *  - the early-out is one condition (`locked || option not in 1..5`); the MAT3 goto chain gives other register webs.
 *  - MBOX_FindObject_WORK's 4th parameter is int (MAT3 rush_protos.h): retail passes the shared constant-1 register
 *    unnarrowed. This changed the shared prototype of string_copy_format (see RESULTS.md); all old members still MATCH.
 *  - the case 0/1/10 zero is spelled 0.f (its own constant, retail $f30; the gNumWindows block zero is 0.0f in $f26),
 *    the two YawUV arguments 3.1415927f / 3.14159265f (two literals, neither hoisted, as retail's two rodata words).
 *  - loop top order currentLevel, whichLight, compScale (as1 tie), and `mask = 1 << p;` is a named local assigned just
 *    before MBOX_ShowObject: its declared slot is the one 4-byte local the retail frame has beyond the MAT3 list
 *    (frame 144; without it 136, every home 4/8 bytes low). The variable is used, not padding.
 * Own data checked against image A (tools/owncheck2.py): lightVec template 0x803B303C, compLight 0x803B2858 (80 B),
 * 0.1f 0x803B9628, pi 0x803B966C/0x803B9670, jump tables 0x803B962C (5) and 0x803B9640 (11), all entries equal. */
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
        whichLight = 0;
        compScale = 1.0f;
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
        mask = 1 << p;
        model_transform_setup(gCarComponents[p][i].idx, 0, mask);
        func_800A79B8(gCarComponents[p][i].idx, 1, &D_803B2858[whichLight]);
    }
}
