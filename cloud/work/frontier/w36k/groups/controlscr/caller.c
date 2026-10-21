/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_800D91A0 = updateControlScreen (controlscr.c, the CONTROLS screen; w36n map: proven).
 * Provenance: direct typed port of MAT3 controlscr.c updateControlScreen (GC 0x8006AFCC; locals and their
 * order from rush_d_dwarf.c: forcedExit, p, i, j, menuItem, maxFirstItem, controls + block-scope skipInit,
 * rumble, rumble, conflict, playerCont), built on Astra's PR #322 packet
 * (cloud/work/frontier/dot_d816c_stock_delay_20261011, itself on w17e/PR #317), which supplied every N64
 * offset and the private-ABI group context. N64 differences from the GC body, all read from the retail words:
 * ten controls (GC eleven), option 10 = rumble and 11 = defaults (GC 11/12); the conflict reset runs before the
 * profile copy; the defaults option does not reset rumble and the rumble options do not set gControlsSet; the
 * 21->8 / 22->9 fold loop is absent and SetControls(0) follows the memcmp.
 * PS_PakRumble / PS_PakInserted = the locked kept getters func_80094FC4 / func_800A1A3C, inlined by umerge
 * (their per-site areas give retail's frame 240). validOption = func_800D8154, called by jal here (umerge's
 * budget) and inlined in updateButtons / calcControlMenuMaxSize.
 * Shaping (disclosed): `j`, dead after the duplicate scan, carries gPlayers[p].edges through the 0x1000/0x2000
 * else-if chain and the &5 test (retail keeps both loads in a0; PR #322 found the &5 half). The two clear loops
 * put their two stores on separate lines (as1 store order). memcmp's count is size_t (u32): the locked
 * declaration says s32; with s32 the 10 shares the t1 constant web. No unused locals, no compiled-out reads.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct { u8 pad0[16]; u8 pak; } PSFile;                /* tPS_File: pak 0x10 */
typedef struct {
    u8 ll[8];
    PSFile **fileh;      /* 0x08 */
    u8 padC[21];
    s8 controls[10];     /* 0x21 */
    s8 rumble;           /* 0x2B */
} Profile;               /* tPlayerProfile */
typedef struct {
    u8 slot;
    u8 controller;       /* 0x01 */
    u8 pad2[2];
    u32 edges;           /* 0x04 */
    u8 pad8[64];
    Profile **save;      /* 0x48 */
} PadState;              /* tPadState, 0x4C */
typedef struct { s32 x, y, w, h, cx, cy; } PlayerRect;        /* 0x18 */

s32 *ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);   /* InitPane */
void drone_target_update(s32);                                  /* SetControls */
s32 entity_flags_apply(s32, s32, s32, u8);                       /* AudPlayEffect */
s32 func_800A1910(u8 *, u8 *, u32);                              /* memcmp (size_t count) */
void func_800B4FB0(s32);                                         /* setPauseState */
void func_800C77C4(void *);                                      /* SetDefaultControls */
s32 func_800D8078(s8);                                           /* validControl */
s32 func_800D8154(s8, s32);                                      /* validOption */
void func_800D9058(void);                                        /* calcControlMenuMaxSize */
void *memcpy(void *, const void *, u32);
void render_results_screen(void);                                /* cleanupControlScreen */
void resource_type_select(s32);                                  /* PlayStartABSound */
void *sound_control(s16, s16, void *, s16);                      /* NewMultiBlit */
void sound_handles_clear(s32);                                   /* RemovePanes */
void drone_throttle_calc(s8);                                    /* initButtons */
void func_800D816C(s8);                                          /* updateButtons */
void render_replay_ui(s8);                                       /* uninitButtons */

extern u32 state_word_a;                 /* gGameState */
extern PadState input_rec0[4];           /* gPlayers */
extern s16 D_80151AD0;                   /* gNumWindows */
extern s8 D_8015418C[4];                 /* controlScrActive */
extern PlayerRect D_80113EE0[4][4];      /* gPlayerRects */
extern s8 D_8014A10C[4];                 /* prevControlScrActive */
extern s16 D_8014A0F8[4];                /* gControlSelect */
extern s16 D_8014A100[4];                /* gControlFirst */
extern s16 D_8014A10A;                   /* gControlMenuSize */
extern s8 D_80149DA8[4][10];             /* gConflict */
extern s8 D_80149DD4[4];                 /* gConflictExists */
extern s8 D_8013C068[4][10];             /* gNewControls */
extern Profile *D_80146150[4];           /* gJustPlayers */
extern s8 D_80139300[4];                 /* gControlsSet */
extern s8 D_80157244;                    /* gSoftReset */
extern s8 D_80113ED4;                    /* inited */
extern s32 D_80113ED8;                   /* blit */
extern s32 D_80113EDC;                   /* exitControlScreen */
extern f32 D_80112A98, D_80112A9C, D_80112AA0, D_80112AA4, D_80112AA8, D_80112AAC, D_80112AB0, D_80112AB4;
extern u8 D_80112AB8[], D_8011361C[], D_80113910[];  /* buttonsFrontEnd, buttonsPauseSingle, buttonsPauseMulti */

s8 func_80094FC4(s32 pak);                                       /* PS_PakRumble (locked, menu_save_options) */
s8 func_800A1A3C(s32 pak);                                       /* PS_PakInserted (locked, menu_save_options) */

void func_800D91A0(void) {
    s32 forcedExit;
    s32 p;
    s32 i;
    s32 j;
    s32 menuItem;
    s32 maxFirstItem;
    s8 *controls;

    if (!D_80113ED4) {
        if (state_word_a & 0x7C03FFFE) {
            s8 skipInit = 1;
            for (p = 0; p < D_80151AD0; p++) {
                if (D_8015418C[p] == 1) skipInit = 0;
            }
            if (skipInit == 1) return;
        } else {
            for (p = 0; p < D_80151AD0; p++) {
                ambient_sound_set(D_80113EE0[D_80151AD0 - 1][p].x,
                    D_80113EE0[D_80151AD0 - 1][p].y,
                    D_80113EE0[D_80151AD0 - 1][p].x + D_80113EE0[D_80151AD0 - 1][p].w,
                    D_80113EE0[D_80151AD0 - 1][p].y + D_80113EE0[D_80151AD0 - 1][p].h,
                    176, 0, 0, 0);
            }
            for (p = D_80151AD0; p < 4; p++) D_8014A10C[p] = 0;
        }
        func_800D9058();
        if (state_word_a & 0x7C03FFFE)
            D_80113ED8 = (s32)sound_control(0, 0, D_80112AB8, 81);
        else if (D_80151AD0 == 1)
            D_80113ED8 = (s32)sound_control(0, 0, D_8011361C, 21);
        else
            D_80113ED8 = (s32)sound_control(0, 0, D_80113910, 41);
        if (D_80151AD0 == 1) {
            D_80112A98 = 1.0f;
            D_80112A9C = 0.9f;
            D_80112AA0 = 40.0f;
            D_80112AA4 = 0.0f;
            D_80112AA8 = 120.0f;
            D_80112AAC = -120.0f;
            D_80112AB0 = 300.0f;
            D_80112AB4 = 225.0f;
        } else if (D_80151AD0 == 2) {
            D_80112A98 = 1.0f;
            D_80112A9C = 1.3f;
            D_80112AA0 = 40.0f;
            D_80112AA4 = 0.0f;
            D_80112AA8 = 95.0f;
            D_80112AAC = -120.0f;
            D_80112AB0 = 300.0f;
            D_80112AB4 = 225.0f;
        } else {
            D_80112A98 = 1.0f;
            D_80112A9C = 1.3f;
            D_80112AA0 = 40.0f;
            D_80112AA4 = -30.0f;
            D_80112AA8 = 95.0f;
            D_80112AAC = -120.0f;
            D_80112AB0 = 300.0f;
            D_80112AB4 = 225.0f;
        }
        D_80113EDC = 0;
        D_80113ED4 = 1;
    }
    for (p = 0; p < 4; p++) {
        if (input_rec0[p].controller == 5) continue;
        if (!D_8015418C[p]) {
            if ((state_word_a & 0x7C0000) && (input_rec0[p].edges & 2)) D_8015418C[p] = 1;
            continue;
        }
        if (!D_8014A10C[p]) {
            D_8014A0F8[p] = 0;
            D_8014A100[p] = 0;
            for (i = 0; i < 10; i++) {
                D_80149DA8[p][i] = 0;
                D_80149DD4[p] = 0;
            }
            if (input_rec0[p].save)
                memcpy(D_8013C068[input_rec0[p].controller], (*input_rec0[p].save)->controls, 10);
            else
                memcpy(D_8013C068[input_rec0[p].controller], D_80146150[input_rec0[p].controller]->controls, 10);
            if (state_word_a & 0x7C03FFFE) drone_throttle_calc(p);
        }
        func_800D816C(p);
        controls = D_8013C068[input_rec0[p].controller];
        while ((D_80149DD4[p] == 1 && !D_80149DA8[p][D_8014A0F8[p]] && D_8014A0F8[p] != 11) ||
            !func_800D8154(p, D_8014A0F8[p]) || (!func_80094FC4(input_rec0[p].controller) && D_8014A0F8[p] == 10)) {
            D_8014A0F8[p]++;
            if (D_8014A0F8[p] > 11) D_8014A0F8[p] = 0;
        }
        if (input_rec0[p].edges & 0x400) {
            entity_flags_apply(40, 0, 1, 0);
            do {
                D_8014A0F8[p]--;
                if (D_8014A0F8[p] < 0) D_8014A0F8[p] = 11;
            } while ((D_80149DD4[p] == 1 && !D_80149DA8[p][D_8014A0F8[p]] && D_8014A0F8[p] != 11) ||
                !func_800D8154(p, D_8014A0F8[p]) || (!func_80094FC4(input_rec0[p].controller) && D_8014A0F8[p] == 10));
        }
        if (input_rec0[p].edges & 0x800) {
            entity_flags_apply(39, 0, 1, 0);
            do {
                D_8014A0F8[p]++;
                if (D_8014A0F8[p] > 11) D_8014A0F8[p] = 0;
            } while ((D_80149DD4[p] == 1 && !D_80149DA8[p][D_8014A0F8[p]] && D_8014A0F8[p] != 11) ||
                !func_800D8154(p, D_8014A0F8[p]) || (!func_80094FC4(input_rec0[p].controller) && D_8014A0F8[p] == 10));
        }
        maxFirstItem = 0;
        menuItem = 0;
        for (i = 11; i >= 0; i--) {
            if (func_800D8154(p, i)) {
                menuItem++;
                if (menuItem == D_8014A10A) {
                    maxFirstItem = i;
                    break;
                }
            }
        }
        menuItem = 0;
        for (i = 0; i < D_8014A0F8[p]; i++) {
            if (func_800D8154(p, i)) menuItem++;
        }
        if (D_8014A100[p] < menuItem - (D_8014A10A - 2)) D_8014A100[p] = menuItem - (D_8014A10A - 2);
        if (D_8014A100[p] > menuItem - 1) D_8014A100[p] = menuItem - 1;
        if (D_8014A100[p] > maxFirstItem) D_8014A100[p] = maxFirstItem;
        if (D_8014A100[p] < 0) D_8014A100[p] = 0;
        if (D_8014A0F8[p] == 11) {
            if (input_rec0[p].edges & 2) {
                entity_flags_apply(43, 0, 1, 0);
                func_800C77C4(controls);
            }
        } else if ((j = input_rec0[p].edges) & 0x1000) {
            entity_flags_apply(41, 0, 1, 0);
            if (D_8014A0F8[p] == 10) {
                s8 rumble;
                rumble = (*input_rec0[p].save)->rumble;
                rumble--;
                if (rumble < 0) rumble = 2;
                (*input_rec0[p].save)->rumble = rumble;
            } else {
                do {
                    controls[D_8014A0F8[p]]--;
                    if (controls[D_8014A0F8[p]] < 0) controls[D_8014A0F8[p]] = 25;
                } while (!func_800D8078(p));
            }
        } else if (j & 0x2000) {
            entity_flags_apply(44, 0, 1, 0);
            if (D_8014A0F8[p] == 10) {
                s8 rumble;
                rumble = (*input_rec0[p].save)->rumble;
                rumble++;
                if (rumble > 2) rumble = 0;
                (*input_rec0[p].save)->rumble = rumble;
            } else {
                do {
                    controls[D_8014A0F8[p]]++;
                    if (controls[D_8014A0F8[p]] >= 26) controls[D_8014A0F8[p]] = 0;
                } while (!func_800D8078(p));
            }
        }
        for (i = 0; i < 10; i++) {
            D_80149DA8[p][i] = 0;
            D_80149DD4[p] = 0;
        }
        for (i = 0; i < 9; i++) {
            for (j = i + 1; j < 10; j++) {
                if ((controls[i] == controls[j] ||
                    (controls[i] == 21 && controls[j] == 8) ||
                    (controls[i] == 8 && controls[j] == 21) ||
                    (controls[i] == 22 && controls[j] == 9) ||
                    (controls[i] == 9 && controls[j] == 22)) && controls[i] != 19 && controls[i] != 25) {
                    D_80149DA8[p][i] = 1;
                    D_80149DA8[p][j] = 1;
                    D_80149DD4[p] = 1;
                }
            }
        }
        forcedExit = 0;
        if ((state_word_a & 0x7C03FFFE) && input_rec0[p].save && (*input_rec0[p].save)->fileh &&
            !func_800A1A3C((*(*input_rec0[p].save)->fileh)->pak)) forcedExit = 1;
        if (forcedExit) {
            D_8015418C[p] = 0;
            render_replay_ui(p);
            D_80113EDC = 2;
        } else if ((j = input_rec0[p].edges) & 5) {
            s8 conflict = 0;
            for (i = 0; i < 10; i++) {
                if (D_80149DA8[p][i] == 1) conflict = 1;
            }
            if (conflict == 1) entity_flags_apply(42, 0, 1, 0);
            else {
                s32 playerCont = input_rec0[p].controller;
                resource_type_select(j);
                if (playerCont != 5) {
                    if (input_rec0[p].save) {
                        if (func_800A1910((u8 *)D_8013C068[playerCont], (u8 *)(*input_rec0[p].save)->controls, 10)) D_80139300[playerCont] = 1;
                    } else {
                        if (func_800A1910((u8 *)D_8013C068[playerCont], (u8 *)D_80146150[playerCont]->controls, 10)) D_80139300[playerCont] = 1;
                    }
                }
                drone_target_update(0);
                D_8015418C[p] = 0;
                render_replay_ui(p);
                D_80113EDC = 2;
            }
        }
        D_8014A10C[p] = D_8015418C[p];
    }
    for (p = 0; p < D_80151AD0; p++) {
        if (D_8015418C[p] == 1) D_80113EDC = 0;
    }
    if (D_80157244 || D_80113EDC) {
        render_results_screen();
        if (!(state_word_a & 0x7C03FFFE)) {
            sound_handles_clear(0);
            func_800B4FB0(1);
        }
    }
}
