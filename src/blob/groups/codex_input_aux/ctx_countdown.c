/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* countdown (0x800FBF88, 668 words) = MAT3 game.c _UpdateGame (GC 0x80014340, size 0x9E0), the per-frame
 * race state machine called by game_loop (= GameLoop) when gGameState & 0x3FC0000. The N64 label is WRONG
 * (proposed name: _UpdateGame). Direct typed port of the MAT3 body (statement order, gotos preplay/countdown,
 * locals i/allSolid/p/numDeaths, static startCountdown, static _EndGame defined after it).
 * N64 differences from GC: no gPakInserted loop; no SwitchFrameMode/DisableDrawFace/EnableDrawFace/
 * UpdateStationaryEmitters; SaveServer(0, 1) after the countdown frame and a 3-second SaveServer tick
 * (D_8014401C vs game time D_801543CC) at the start of the race state; gRetireGame heads the race-state
 * else-if chain; kCircuit (3) joins the one-race/practice branch; N64 sound ids 78/2/22; the second battle
 * loop indexes with i (GC: p).
 * Callee map (N64 -> GC): see cloud/work/frontier/w36p/RESULTS.md "Identities proven".
 *
 * Matching levers (w36p, all natural; no unused locals, no compiled-out code):
 *  - setGameState is the kept 1-statement func_800B5570 (as in GC). umerge inlines its 7 calls; they use up
 *    countdown's inline budget, so PlayGameSound (func_800B61A8) and _CheckButtons (func_800FBE30) stay
 *    `jal` calls as in retail with no inline blockers (writing `state_word_b = X` inlines one
 *    PlayGameSound and _CheckButtons).
 *  - startCountdown is a function-local `static s8 = 0` (.data 0x801146F0); as an extern its address web is
 *    coloured into s0.
 *  - _EndGame is a static helper defined after countdown (umerge inlines it; its later line numbers give as1
 *    retail's store-before-reload order at the end of the 0x1000000 state).
 *  - SetCountdownTimer(3.5) and `3.5f * gRetraceRate`: two spellings, two constants (retail rematerialises
 *    3.5 after the call; one spelling keeps it in $f20).
 *  - coast loop `if (gDeathMode) continue;` (retail reloads gNumPlayers only on the AreWeSolid path; the
 *    `&&` form reloads at the join and colours &gNumPlayers into s3).
 *  - the two-store blocks (gCoastFlag = 1; gOutOfTime = 1;) on separate lines in stunt case 2 and battle
 *    case 1 (as1 store order).
 * Needs to be INTERNAL (not kept): retail uses s0-s6/$f20/$f22 unsaved (IPA; game_loop saves them).
 */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Player76 { u8 slot; u8 pad01[63]; u16 numCrashes; u8 pad42[10]; } Player76;
typedef struct Car952 {
    u8 pad000[232];
    s32 appearance;         /* 0x0E8 */
    u8 pad0EC[3];
    s8 place_locked;        /* 0x0EF */
    u8 pad0F0[857 - 240];
    s8 dead;                /* 0x359 */
    u8 pad35A;
    s8 slot;                /* 0x35B */
    u8 pad35C[896 - 860];
    Player76 *player;       /* 0x380 */
    u8 pad384[931 - 900];
    s8 battlePoints;        /* 0x3A3 */
    u8 pad3A4[952 - 932];
} Car952;
typedef struct Model2056 {
    u8 pad000[1990];
    s16 slot;               /* 0x7C6 */
    u8 pad7C8[2024 - 1992];
    s8 laps;                /* 0x7E8 */
    u8 pad7E9[2056 - 2025];
} Model2056;
typedef struct Stunt120 { u8 pad00[20]; s32 totalPoints; u8 pad18[96]; } Stunt120;
typedef struct Cam152 { u8 pad00[36]; f32 pos[3]; u8 pad30[104]; } Cam152;
typedef struct Inner5 { u8 pad0[5]; s8 flag; } Inner5;
typedef struct Ghost40 { u8 pad0[40]; Inner5 **header; } Ghost40;

extern u32 state_word_a;        /* gGameState */
extern u32 state_word_b;        /* gNewGameState */
extern u32 D_801174BC;          /* gSkipToGameState */

extern s8 D_80114650;           /* gRestartGame */
extern s8 D_80114654;           /* gGamePointPlayed */
extern s32 gameplay_mode;       /* gRace */
extern s16 active_player_count; /* gNumPlayers */
extern Player76 input_rec0[];   /* gPlayers */
extern Model2056 D_8014A250[];  /* model */
extern Car952 player_array[];   /* gGameCar */
extern s8 D_80150F14;           /* gUseCatchup */
extern s8 D_80150EFC;           /* gCatchup */
extern s32 D_801407BC;          /* gStuntMode */
extern s32 D_80140804;          /* gStuntModeMins */
extern s32 D_80140B08;          /* gBattleMode */
extern s32 D_80140BD8;          /* gBattleModeMins */
extern s32 D_80140A00;          /* gStuntModePoints */
extern s32 D_80140AD8;          /* gStuntModeDeaths */
extern s32 D_80142510;          /* gBattleModePoints */
extern f32 D_8014401C;          /* last SaveServer tick time */
extern f32 D_801543CC;          /* game time */
extern s8 D_8013FECB;           /* gCoastFlag */
extern s8 D_80142699;           /* gRetireGame */
extern s8 D_80142690;           /* gOutOfTime */
extern s8 D_80142760;           /* gDeathMode */
extern Stunt120 D_80152038[];   /* gStuntPlayer */
extern s8 D_8015256C[4];        /* gTeamPoints */
extern s8 D_8012E67C[];         /* gCarTeam */
extern s32 D_80143F10;          /* gAPlayerReallyFinished */
extern f32 D_801525F4;          /* gDeathWaitEnd */
extern s16 D_80152734;          /* gNumLaps */
extern Ghost40 **D_80152698[];  /* gGhosts */
extern s8 D_80152744;           /* gNumActiveCars */
extern s16 gNumWindows;
extern Cam152 gCamera[];
extern f32 D_8002AFB4;          /* gRetraceRate */

void init_state_begin(void);            /* SetOptions */
void init_state_continue(void);         /* _CalcPolePositions */
s32 display_list_flush(s32 useHud, s32 cancel); /* PrePlay */
void entity_audio_update(void);         /* ClearRushControls */
void SaveServer(s32 forceUpdate, s32 rumbleOnly);
void camera_race_setup(void);           /* CarSetup */
void race_init_helper(void);            /* Update_MDrive */
void race_setup_1(void);                /* TextureAnimation */
void race_setup_2(s16 slot);            /* CheckTargetCollision */
void viScheduleTick(f32 time);          /* SetCountdownTimer */
s32 viDeadlinePassed(void);             /* CountdownTimeOut */
void func_800E762C(s32 ticks);          /* init_model_task */
void func_800FBF2C(void);               /* CountDown */
void func_800FBE30(void);               /* _CheckButtons */
f32 func_800FBED8(void);                /* CalcRaceTime */
s32 func_800B61A8(s32 sound, s32 slot, s32 a2, u8 a3); /* PlayGameSound */
void physics_init_mode0(void);          /* StartGameMusic */
void func_800FBE60(void);               /* InitCPTime */
void render_viewport_init(void);        /* PlayGame */
void func_800F7F3C(void);               /* FindWinner */
void ghost_race_setup(void);            /* GhostEnd */
void records_screen(void);              /* RemoveHUD */
s32 func_800CF604(s16 p);               /* AreWeSolid */
s32 func_800E7DD0(void);                /* are_all_cars_stopped */
void speed_mode0_wrapper(f32 vol, f32 time); /* AudSetMusicVol */
void *memset(void *, s32, u32);
void func_800D5828(s16 slot);           /* End_MDrive */
void cpak_read(s16 slot);               /* UpdateVisuals */
void players_frame_update(void);        /* modelstop */
void func_800D5374(void);               /* StopListeners */
void billboard_render(void);            /* updateHighScoreScreen */
void finish_state_alt(void);            /* updateGameOverScreen */
void func_800AB18C(s32 i, f32 *pos);    /* WorldFogDensity */
void func_800B5570(u32 state);          /* setGameState */

void func_800FBE20(void);  /* GC _EndGame: w36w2 names the static after its retail deleted-procedure stub */

void countdown(void)
{
    s32 i;
    s8 allSolid;
    s32 p;
    s32 numDeaths;
    static s8 startCountdown = 0;

    if (state_word_a & 0x40000) {
        D_801174BC = 1;
        init_state_begin();
        init_state_continue();
        D_80150F14 = D_80150EFC;
        func_800B5570(0x80000);
        goto preplay;
    } else if (state_word_a & 0x80000) {
    preplay:
        if (display_list_flush(1, 0)) {
            startCountdown = 1;
            func_800B5570(0x200000);
            entity_audio_update();
            goto countdown;
        } else {
            SaveServer(0, 1);
            goto done;
        }
    } else if (state_word_a & 0x200000) {
    countdown:
        if (D_80114650) goto done;
        camera_race_setup();
        race_init_helper();
        race_setup_1();
        race_setup_2(D_8014A250[0].slot);
        if (startCountdown) {
            viScheduleTick(3.5);
            func_800E762C(3.5f * D_8002AFB4);
            startCountdown = 0;
        }
        func_800FBF2C();
        func_800FBE30();
        SaveServer(0, 1);
    } else if (state_word_a & 0x100000) {
        if (gameplay_mode == 1) viScheduleTick(1800.0f);
        else if (gameplay_mode == 4) {
            if (D_801407BC == 0) viScheduleTick(D_80140804 * 60);
            else viScheduleTick(600.0f);
        } else if (gameplay_mode == 5) viScheduleTick(300.0f);
        else if (gameplay_mode == 6) {
            if (D_80140B08 == 1) viScheduleTick(D_80140BD8 * 60);
            else viScheduleTick(1200.0f);
        } else viScheduleTick(func_800FBED8());
        func_800B61A8(78, 0, 1, 0);
        physics_init_mode0();
        func_800FBE60();
        func_800B5570(0x400000);
        D_8014401C = D_801543CC;
    } else if (state_word_a & 0x400000) {
        if (D_8014401C + 3.0f < D_801543CC) {
            SaveServer(0, 1);
            D_8014401C = D_801543CC;
        }
        render_viewport_init();
        func_800FBE30();
        if (D_80142699) {
            func_800F7F3C();
            func_800B5570(0x1000000);
            ghost_race_setup();
            records_screen();
        } else if (D_8013FECB) {
            allSolid = 1;
            for (p = 0; p < active_player_count; p++) {
                if (D_80142760) continue;
                if (!func_800CF604(p)) allSolid = 0;
            }
            if (func_800E7DD0() && allSolid) {
                if (gameplay_mode == 4) func_800F7F3C();
                func_800B5570(0x1000000);
                ghost_race_setup();
                records_screen();
            }
            if (!viDeadlinePassed() && gameplay_mode != 4 && gameplay_mode != 5 && gameplay_mode != 6) {
                D_8013FECB = 0;
                D_80142690 = 0;
            }
        } else if (gameplay_mode == 4) {
            switch (D_801407BC) {
            case 0:
                if (viDeadlinePassed()) { D_8013FECB = 1; D_80142690 = 1; }
                break;
            case 1:
                for (i = 0; i < active_player_count; i++) {
                    if (D_80152038[i].totalPoints >= D_80140A00) { D_8013FECB = 1; D_80142690 = 1; }
                }
                break;
            case 2:
                numDeaths = 0;
                for (i = 0; i < active_player_count; i++) {
                    if (player_array[i].player->numCrashes >= D_80140AD8) { player_array[i].dead = 1; numDeaths++; }
                }
                if (numDeaths == active_player_count) {
                    D_8013FECB = 1;
                    D_80142690 = 1;
                }
                break;
            }
        } else if (gameplay_mode == 6) {
            switch (D_80140B08) {
            case 1:
                if (viDeadlinePassed()) {
                    func_800F7F3C();
                    D_8013FECB = 1;
                    D_80142690 = 1;
                }
                break;
            case 0:
                memset(D_8015256C, 0, 4);
                for (i = 0; i < active_player_count; i++) D_8015256C[D_8012E67C[i]] += player_array[i].battlePoints;
                for (i = 0; i < 4; i++) {
                    if (!D_80114654 && D_8015256C[i] == D_80142510 - 1) {
                        D_80114654 = 1;
                        func_800B61A8(2, 0, 2, 0);
                    }
                    if (D_8015256C[i] >= D_80142510) {
                        func_800F7F3C(); D_8013FECB = 1; D_80142690 = 1;
                    }
                }
                break;
            }
        } else if (gameplay_mode == 0 || gameplay_mode == 1 || gameplay_mode == 3) {
            for (i = 0; i < active_player_count; i++) if (!player_array[input_rec0[i].slot].place_locked) goto done;
            D_80143F10 = 0;
            for (i = 0; i < active_player_count; i++) if (D_80152734 == D_8014A250[i].laps) D_80143F10 = 1;
            if (!D_8013FECB) {
                if (D_80142760) D_801525F4 = 0.0f;
                D_8013FECB = 1;
                func_800F7F3C();
                speed_mode0_wrapper(0.0f, 3.0f);
            }
        } else {
            if (viDeadlinePassed()) {
                if (!D_8013FECB) {
                    D_8013FECB = 1;
                    func_800F7F3C();
                    speed_mode0_wrapper(0.0f, 3.0f);
                }
                for (i = 0; i < active_player_count; i++) {
                    if (D_80152734 != D_8014A250[input_rec0[i].slot].laps) {
                        if (gameplay_mode != 2 || !D_80152698[input_rec0[i].slot] || (*D_80152698[input_rec0[i].slot])->header[0]->flag < 0)
                            func_800B61A8(22, 0, 1, 0);
                        D_80142690 = 1;
                        break;
                    }
                }
            }
        }
    } else if (state_word_a & 0x1000000) {
        func_800FBE20();
    } else if (state_word_a & 0x2000000) billboard_render();
    else if (state_word_a & 0x800000) finish_state_alt();
done:
    if (state_word_a & 0x600000) {
        for (i = 0; i < gNumWindows; i++) func_800AB18C(i, gCamera[i].pos);
    }
}

void func_800FBE20(void)
{
    s16 i;
    for (i = 0; i < D_80152744; i++) {
        func_800D5828(D_8014A250[i].slot);
        player_array[i].appearance &= ~8;
        cpak_read(player_array[i].slot);
    }
    players_frame_update();
    func_800D5374();
    viScheduleTick(4.0f);
    func_800B5570(0x2000000);
}
