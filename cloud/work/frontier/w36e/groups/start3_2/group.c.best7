/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image A start screen (title "press start" menu): updateStartScreen (func_80390418, kept root) and its
 * IPA-internal updateButtons (func_8038F79C). Donor: MAT3 GameSource startscr.c, updateStartScreen (GC 0x8008624c)
 * and static updateButtons (GC 0x800871e4); direct typed port, N64 offsets/addresses from the retail words.
 *
 * N64 differences from MAT3 (all read from the retail code): gOptions[14] <= 12 (GC 18); AudPlaySong(6,1) (GC 19);
 * PlayLRSound() runs before the `gStartMenuSelect == 0` test; exit case 7 -> 0x10000000 (GC has no case 7);
 * lower angle limit in updateButtons is 0.0f (GC 0.08726646f); the gNumPlayers clamp is
 * `if (n > max) { if (max < 1) n = 1; else n = max; }`; the 17th button x is `gNumPlayers * 22.5f` (ugen operand order).
 * gFrameSeconds is volatile (MAT3 declares it volatile) and gNumModels is volatile u8 (every use is lui/addiu/lbu 0(reg)).
 * uninitScreen and uninitButtons are inlined statics (inlined twice into the root). func_8038F744 (optionValid) is the
 * installed body of group dot_runtime_a_option_eligibility_20261006, present only as IPA context (without it the
 * root keeps &gStartMenuSelect in s0 instead of a2).
 *
 * Frame shaping, disclosed (Wave 12/16 rule): updateButtons has retail frame 96. Without the three unused f32 locals
 * below the frame is 88 and the locals sit at sp+56/66/72 instead of retail's sp+52/62/68. Udef is 32 bytes with
 * the MAT3 locals (buttonIdx, prevPos[3], tex, texIdx, buttonY, selectedY) and 44 with three more 4-byte slots
 * (frame = align8(52 + Udef)). The three unused locals reproduce this exactly; their original use is unknown.
 */
typedef signed char s8;typedef unsigned char u8;
typedef short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
typedef struct Blit Blit;
typedef struct ButtonInfo { s32 buttonType; s32 objIdx; f32 angle; f32 uvs[4][3]; } ButtonInfo;
typedef union tRGBA { u8 c[4]; u32 w; } tRGBA;
typedef struct MultiBlit MultiBlit;
typedef struct Egg Egg;
typedef struct Controller { s8 attached; u8 rest[15]; } Controller;

extern volatile f32 D_8002EB94;            /* gFrameSeconds */
extern f32 D_8011418C[3][3];      /* unitmatrix */
extern volatile u8 D_80140BDC;    /* gNumModels */
extern s8 D_80146108[];           /* gOptions */
extern s8 D_801461F8;             /* gGameStarted */
extern s8 D_80157244;             /* gSoftReset */
extern u32 D_801174B4, D_801174BC;/* gGameState, gSkipToGameState */
extern u32 D_8015694C;            /* gActiveEdges */
extern s16 D_8014A108;            /* gNumPlayers */
extern Controller D_80156CF0[4];  /* gControllerData */
extern s8 D_801164A1;             /* gCheatActive[23] */
extern s8 D_803B65E4[];           /* validOption */
extern u32 D_80156944;            /* gActiveLevels */
extern ButtonInfo D_803B6610[18]; /* buttonInfo */
extern s8 D_803B65DC;             /* inited */
extern Blit *D_803B65E0;          /* blit */
extern s32 D_803B6A48;            /* buttonsInited */
extern s32 D_803B6A4C;            /* buttonsJustInited */
extern f32 D_803B6A50;            /* gButtonScale */
extern f32 D_803B6A54;            /* gButtonSpace */
extern f32 D_803B6A58;            /* buttonX */
extern f32 D_803B6A5C;            /* maxY */
extern f32 D_803B6A60;            /* buttonZ */
extern f32 D_803B6A64;            /* cursorZ */
extern s32 D_803B6A68;            /* gStartMenuSelect */
extern s32 D_803B6A6C;            /* gMaxPlayers */
extern s32 D_803B6A70;            /* exitStartScreen */
extern f32 D_803BAC1C;            /* cursorSpeed */
extern f32 D_803BAC60;            /* playerCursorSpeed */
extern char *D_803B65F0[];        /* buttonNames */
extern MultiBlit D_803B65B8[];    /* mblit */
extern Egg D_803B41D4[];          /* gEggs + 23 entries is the start-screen egg */
extern Egg *D_803B4278;           /* gEgg */
extern f32 D_803B427C;            /* gNextTimeOut */
extern s32 D_803B4280;            /* gEggIdx */

extern f32 fabsf(f32);
#pragma intrinsic (fabsf)
extern void *memcpy(void *, const void *, u32);
extern void particle_lifetime_set(void);      /* createBG */
extern s32 string_copy_format(char *, s8, s8, s8);  /* MBOX_FindObject_WORK */
extern s32 sign_extend_call(s32, f32 (*)[3], s32, s32); /* MBOX_NewObject */
extern void func_8008E06C(s16, tRGBA *);      /* ZOID_SetColor */
extern void particle_velocity_set(void);      /* ZeroCameras */
extern Blit *sound_control(s16, s16, MultiBlit *, s16); /* NewMultiBlit */
extern void entity_audio_update(void);        /* ClearRushControls */
extern s8 func_8038F648(void);                /* CheckForEgg */
extern void func_800B5570(s32);               /* setGameState */
extern void speed_mode0_wrapper(f32, f32);    /* AudSetMusicVol */
extern void sync_entry_register(s32, s32);    /* AudPlaySong */
extern void resource_type_select(u32);        /* PlayStartABSound */
extern void audio_doppler(u32);               /* PlayLRSound */
extern void audio_distance_atten(u32);        /* PlayUDSound */
extern void sound_stop(Blit *);               /* RemoveBlit */
extern void ambient_sounds_clear(void);       /* removeBG */
extern void sound_call_minimal(s16);          /* ZOID_RemoveObject */
extern void object_type1_create(void);        /* AudKillSong */
extern void object_type7_create(void);        /* AudStopAllEffects */
extern void func_800EE7AC(void);              /* ExitSystem */
extern void model_data_load(s32, s32, s32);   /* MBOX_HideObject */
extern void model_transform_setup(s32, s32, s32); /* MBOX_ShowObject */
extern void *func_800B24EC(char *, u16 *, s8, s8, s32); /* MBOX_FindTexture_WORK */
extern void func_8008D870(s16, void *, s32);  /* ZOID_SetTexture */
extern void func_8008B32C(f32 *, f32 *, f32); /* ScaleUV */
extern void func_800B5898(f32, f32 *);        /* WPitchUV */
extern void func_800B5940(f32, f32 *);        /* WRollUV */
extern void func_800FD754(f32 *, f32 *, f32, f32, f32); /* ScaleUVXYZ */

void InitEgg(s32 egg)
{
    D_803B4278 = &D_803B41D4[egg - 23];
    D_803B427C = 0;
    D_803B4280 = 0;
}

/* optionValid: installed body (group dot_runtime_a_option_eligibility_20261006), IPA context only */
s8 func_8038F744(s32 index)
{
    s8 *entry = &D_803B65E4[index];
    s32 value = *entry;

    if (index == 7 && !(D_80156944 & 0x100)) {
        value = 0;
    }
    if (index == 6) {
        value = D_801164A1;
    }
    return value;
}

void func_8038FFD0(void) /* initButtons */
{
    tRGBA buttonColor = {255, 255, 255, 255};
    void *tex;
    u16 texIdx;
    s32 buttonIdx;
    for (buttonIdx = 0; buttonIdx < 18; buttonIdx++)
        if (D_803B6610[buttonIdx].objIdx == -1) {
            memcpy(D_803B6610[buttonIdx].uvs, D_8011418C, 48);
            D_803B6610[buttonIdx].objIdx = sign_extend_call(
                string_copy_format(D_803B65F0[D_803B6610[buttonIdx].buttonType], 0, D_80140BDC - 1, 1),
                D_803B6610[buttonIdx].uvs, -1,
                (D_803B6610[buttonIdx].buttonType == 0 || D_803B6610[buttonIdx].buttonType == 1) ? 0x42000 : 0);
            func_8008E06C(D_803B6610[buttonIdx].objIdx, &buttonColor);
        }
    tex = func_800B24EC("BUTTON_TITLE", &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B6610[11].objIdx, tex, -1);
    memcpy(D_803B6610[11].uvs, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B6610[11].uvs);
    D_803B6610[11].uvs[3][0] = 0.0f;
    D_803B6610[11].uvs[3][1] = 115.0f;
    D_803B6610[11].uvs[3][2] = 200.0f;
    tex = func_800B24EC("A1", &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B6610[12].objIdx, tex, -1);
    memcpy(D_803B6610[12].uvs, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B6610[12].uvs);
    D_803B6610[12].uvs[3][0] = 75.1f;
    D_803B6610[12].uvs[3][1] = 55.2f;
    D_803B6610[12].uvs[3][2] = 100.0f;
    func_8008B32C(D_803B6610[12].uvs[0], D_803B6610[12].uvs[0], 0.5f);
    memcpy(D_803B6610[13].uvs, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B6610[13].uvs);
    func_800B5940(3.1415927f, D_803B6610[13].uvs);
    D_803B6610[13].uvs[3][0] = 87.1f;
    D_803B6610[13].uvs[3][1] = 55.2f;
    D_803B6610[13].uvs[3][2] = 100.0f;
    tex = func_800B24EC("B1", &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B6610[14].objIdx, tex, -1);
    memcpy(D_803B6610[14].uvs, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B6610[14].uvs);
    D_803B6610[14].uvs[3][0] = -75.1f;
    D_803B6610[14].uvs[3][1] = 55.20f;
    D_803B6610[14].uvs[3][2] = 100.0f;
    func_8008B32C(D_803B6610[14].uvs[0], D_803B6610[14].uvs[0], 0.5f);
    memcpy(D_803B6610[15].uvs, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_803B6610[15].uvs);
    D_803B6610[15].uvs[3][0] = -87.1f;
    D_803B6610[15].uvs[3][1] = 55.20f;
    D_803B6610[15].uvs[3][2] = 100.0f;
    tex = func_800B24EC("CURSOR", &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B6610[16].objIdx, tex, -1);
    D_803B6A48 = 1;
    D_803B6A4C = 1;
    particle_velocity_set();
}

void func_8038FFD8(void)
{
    if (D_80146108[14] <= 12) {
        speed_mode0_wrapper(D_80146108[12] / 10.0f, 0.05f);
        sync_entry_register(6, 1);
    }
    particle_lifetime_set();
    func_8038FFD0();
    D_803B65E0 = sound_control(0, 0, D_803B65B8, 1);
    D_803B6A70 = 0;
    entity_audio_update();
    if (!D_801164A1)
        InitEgg(23);
    D_803B65DC = 1;
}

void func_8038F79C(void)
{
    f32 unused0, unused1, unused2;
    s32 buttonIdx;
    f32 prevPos[3];
    void *tex;
    u16 texIdx;
    f32 buttonY, selectedY;
    if (!D_803B6A48)
        return;
    buttonY = D_803B6A5C;
    selectedY = D_803B6A5C;
    for (buttonIdx = 0; buttonIdx < 10; buttonIdx++) {
        if (!func_8038F744(buttonIdx)) {
            model_data_load(D_803B6610[buttonIdx].objIdx, 0, 15);
            continue;
        }
        model_transform_setup(D_803B6610[buttonIdx].objIdx, 0, 15);
        if (buttonIdx == D_803B6A68) {
            if (D_803B6610[buttonIdx].angle < 3.1415927f)
                D_803B6610[buttonIdx].angle += 12.566371f * D_8002EB94;
            if (D_803B6610[buttonIdx].angle > 3.1415927f || D_803B6A4C == 1)
                D_803B6610[buttonIdx].angle = 3.1415927f;
            selectedY = buttonY;
        } else {
            if (D_803B6610[buttonIdx].angle > 0.0f)
                D_803B6610[buttonIdx].angle -= 12.566371f * D_8002EB94;
            if (D_803B6610[buttonIdx].angle < 0.0f || D_803B6A4C == 1)
                D_803B6610[buttonIdx].angle = 0.0f;
        }
        D_803B6610[buttonIdx].uvs[3][0] = D_803B6A58;
        D_803B6610[buttonIdx].uvs[3][1] = buttonY;
        D_803B6610[buttonIdx].uvs[3][2] = D_803B6A60;
        func_8008B32C(D_8011418C[0], D_803B6610[buttonIdx].uvs[0], D_803B6A50);
        func_800B5898(D_803B6610[buttonIdx].angle - 1.5707964f, D_803B6610[buttonIdx].uvs[0]);
        if (D_803B6610[buttonIdx].angle > 1.5707964f)
            tex = func_800B24EC("BUTTON_SELECT", &texIdx, 0, D_80140BDC - 1, 1);
        else
            tex = func_800B24EC("BUTTON", &texIdx, 0, D_80140BDC - 1, 1);
        func_8008D870(D_803B6610[buttonIdx].objIdx, tex, -1);
        if (buttonIdx == 0) {
            D_803B6610[10].angle = D_803B6610[buttonIdx].angle;
            D_803B6610[10].uvs[3][0] = D_803B6610[0].uvs[3][0] + 160.0f * D_803B6A50;
            D_803B6610[10].uvs[3][1] = D_803B6610[0].uvs[3][1];
            D_803B6610[10].uvs[3][2] = D_803B6610[0].uvs[3][2];
            D_803B6610[10].uvs[3][0] = 1.02f * D_803B6610[10].uvs[3][0];
            D_803B6610[10].uvs[3][1] = 1.02f * D_803B6610[10].uvs[3][1];
            D_803B6610[10].uvs[3][2] = 1.02f * D_803B6610[10].uvs[3][2];
            func_8008B32C(D_8011418C[0], D_803B6610[10].uvs[0], D_803B6A50);
            func_800B5898(D_803B6610[10].angle - 1.5707964f, D_803B6610[10].uvs[0]);
            if (D_803B6610[10].angle > 1.5707964f)
                tex = func_800B24EC("BUTTON_SELECT", &texIdx, 0, D_80140BDC - 1, 1);
            else
                tex = func_800B24EC("BUTTON", &texIdx, 0, D_80140BDC - 1, 1);
            func_8008D870(D_803B6610[10].objIdx, tex, -1);
        }
        buttonY -= D_803B6A54 * D_803B6A50;
    }
    memcpy(prevPos, D_803B6610[16].uvs[3], 12);
    D_803B6610[16].angle += 6.2831855f * D_8002EB94;
    if (D_803B6610[16].angle > 6.2831855f)
        D_803B6610[16].angle -= 6.2831855f;
    D_803B6610[16].uvs[3][0] = D_803B6A58 * D_803B6A64 / D_803B6A60 + (-60.0f) * D_803B6A50;
    D_803B6610[16].uvs[3][1] = selectedY * D_803B6A64 / D_803B6A60;
    D_803B6610[16].uvs[3][2] = D_803B6A64;
    if (!D_803B6A4C) {
        if (60.0f * fabsf(D_803B6610[16].uvs[3][1] - prevPos[1]) / 10.0f > D_803BAC1C) {
            D_803BAC1C = 60.0f * fabsf(D_803B6610[16].uvs[3][1] - prevPos[1]) / 10.0f;
            D_803BAC1C = D_803BAC1C < 540.0f * D_803B6A50 ? D_803BAC1C : 540.0f * D_803B6A50;
            D_803BAC1C = D_803BAC1C > 180.0f * D_803B6A50 ? D_803BAC1C : 180.0f * D_803B6A50;
        }
        if (D_803B6610[16].uvs[3][1] > prevPos[1] + D_803BAC1C * D_8002EB94)
            D_803B6610[16].uvs[3][1] = prevPos[1] + D_803BAC1C * D_8002EB94;
        else if (D_803B6610[16].uvs[3][1] < prevPos[1] - D_803BAC1C * D_8002EB94)
            D_803B6610[16].uvs[3][1] = prevPos[1] - D_803BAC1C * D_8002EB94;
        else
            D_803BAC1C = 180.0f * D_803B6A50;
    }
    func_8008B32C(D_8011418C[0], D_803B6610[16].uvs[0], 0.4f * D_803B6A50);
    func_800B5940(D_803B6610[16].angle, D_803B6610[16].uvs[0]);
    func_800B5898(1.5707964f, D_803B6610[16].uvs[0]);
    memcpy(prevPos, D_803B6610[17].uvs[3], 12);
    D_803B6610[17].uvs[3][0] = D_803B6A58 * D_803B6A64 / D_803B6A60 + (72.0f + D_8014A108 * 22.5f) * D_803B6A50;
    D_803B6610[17].uvs[3][1] = D_803B6A5C * D_803B6A64 / D_803B6A60;
    D_803B6610[17].uvs[3][2] = D_803B6A64;
    if (!D_803B6A4C) {
        if (60.0f * fabsf(D_803B6610[17].uvs[3][0] - prevPos[0]) / 10.0f > D_803BAC60) {
            D_803BAC60 = 60.0f * fabsf(D_803B6610[17].uvs[3][0] - prevPos[0]) / 10.0f;
            D_803BAC60 = D_803BAC60 < 540.0f * D_803B6A50 ? D_803BAC60 : 540.0f * D_803B6A50;
            D_803BAC60 = D_803BAC60 > 180.0f * D_803B6A50 ? D_803BAC60 : 180.0f * D_803B6A50;
        }
        if (D_803B6610[17].uvs[3][0] > prevPos[0] + D_803BAC60 * D_8002EB94)
            D_803B6610[17].uvs[3][0] = prevPos[0] + D_803BAC60 * D_8002EB94;
        else if (D_803B6610[17].uvs[3][0] < prevPos[0] - D_803BAC60 * D_8002EB94)
            D_803B6610[17].uvs[3][0] = prevPos[0] - D_803BAC60 * D_8002EB94;
        else
            D_803BAC60 = 180.0f * D_803B6A50;
    }
    if (D_803B6610[0].angle > 1.5707964f)
        tex = func_800B24EC("CURSOR", &texIdx, 0, D_80140BDC - 1, 1);
    else
        tex = func_800B24EC("CURSOR_B", &texIdx, 0, D_80140BDC - 1, 1);
    func_8008D870(D_803B6610[17].objIdx, tex, -1);
    func_800FD754(D_8011418C[0], D_803B6610[17].uvs[0], 0.6f, 0.4f, 0.4f);
    func_8008B32C(D_803B6610[17].uvs[0], D_803B6610[17].uvs[0], D_803B6A50);
    func_800B5898(1.5707964f + D_803B6610[0].angle, D_803B6610[17].uvs[0]);
    D_803B6A4C = 0;
}

static void uninitButtons(void)
{
    s32 buttonIdx;
    for (buttonIdx = 0; buttonIdx < 18; buttonIdx++)
        if (D_803B6610[buttonIdx].objIdx != -1) {
            sound_call_minimal(D_803B6610[buttonIdx].objIdx);
            D_803B6610[buttonIdx].objIdx = -1;
        }
    D_803B6A48 = 0;
}

static void uninitScreen(void)
{
    if (D_803B65E0) {
        sound_stop(D_803B65E0);
        D_803B65E0 = 0;
    }
    ambient_sounds_clear();
    uninitButtons();
    D_803B65DC = 0;
}

void func_80390418(void)
{
    s32 ctrlIdx;
    if (!D_80157244)
        D_801461F8 = 1;
    if (D_801174BC != 1) {
        if (D_801174BC != D_801174B4) {
            if (!(D_801174BC & 0x7c0000) && D_80146108[14] <= 12) {
                speed_mode0_wrapper(D_80146108[12] / 10.0f, 0.05f);
                sync_entry_register(6, 1);
            }
            func_800B5570(32);
            return;
        }
        D_801174BC = 1;
    }
    if (!D_803B65DC)
        func_8038FFD8();
    while (!func_8038F744(D_803B6A68)) {
        if (++D_803B6A68 >= 10)
            D_803B6A68 = 0;
    }
    D_803B6A6C = 0;
    for (ctrlIdx = 0; ctrlIdx < 4; ctrlIdx++) {
        if (D_80156CF0[ctrlIdx].attached != 1)
            break;
        D_803B6A6C++;
    }
    if (D_8014A108 > D_803B6A6C) {
        if (D_803B6A6C < 1)
            D_8014A108 = 1;
        else
            D_8014A108 = D_803B6A6C;
    }
    if (D_8015694C & 1) {
        resource_type_select(D_8015694C);
        D_803B6A68 = 0;
        D_803B6A70 = 1;
    } else if (D_8015694C & 2) {
        resource_type_select(D_8015694C);
        D_803B6A70 = 1;
    } else if (D_8015694C & 4) {
        resource_type_select(D_8015694C);
        D_803B6A70 = 2;
    } else if (D_8015694C & 0x1000) {
        audio_doppler(D_8015694C);
        if (D_803B6A68 == 0) {
            if (--D_8014A108 < 1)
                D_8014A108 = D_803B6A6C;
        }
    } else if (D_8015694C & 0x2000) {
        audio_doppler(D_8015694C);
        if (D_803B6A68 == 0) {
            if (++D_8014A108 > D_803B6A6C)
                D_8014A108 = 1;
        }
    } else if (D_8015694C & 0x400) {
        audio_distance_atten(D_8015694C);
        do {
            if (--D_803B6A68 < 0)
                D_803B6A68 = 9;
        } while (!func_8038F744(D_803B6A68));
    } else if (D_8015694C & 0x800) {
        audio_distance_atten(D_8015694C);
        do {
            if (++D_803B6A68 >= 10)
                D_803B6A68 = 0;
        } while (!func_8038F744(D_803B6A68));
    }
    func_8038F79C();
    if (D_80157244) {
        uninitScreen();
        func_800B5570(4);
    } else if (D_803B6A70) {
        uninitScreen();
        switch (D_803B6A70) {
            case 1:
                switch (D_803B6A68) {
                    case 0: func_800B5570(32); break;
                    case 1: func_800B5570(0x2000); break;
                    case 2: func_800B5570(0x200); break;
                    case 3: func_800B5570(0x800); break;
                    case 4: func_800B5570(0x1000); break;
                    case 6: func_800B5570(0x8000000); break;
                    case 7: func_800B5570(0x10000000); break;
                    case 9:
                        object_type1_create();
                        object_type7_create();
                        func_800EE7AC();
                        break;
                }
                break;
            case 2:
                func_800B5570(4);
                break;
        }
        D_803B6A70 = 0;
    }
    if (!D_801164A1)
        D_801164A1 = func_8038F648();
}
