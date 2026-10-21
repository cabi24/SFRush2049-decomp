/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* display_list_flush (0x800FB2C8) = MAT3 game.c PrePlay (GC 0x80014D20). NOT A MATCH (55 ops rows, frame 256/88).
 * Score in the unit together with groups/frontier_update_game/{countdown,init_state_continue}.c (both --internal) and --internal InitBattlePoints --internal AudLock --internal AudUnlock --internal AudSend.
 * effect_system_init here is the MAT3-shaped InitStunt (EQUAL as its own body, and inlinable as in retail).
 * See cloud/work/frontier/w36p/RESULTS.md. */
/* display_list_flush (0x800FB2C8) = MAT3 game.c PrePlay(useHud, cancel) (GC 0x80014D20, size 0x624). */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Player76 { u8 slot; u8 pad01[63]; s16 numCrashes; u8 pad42[10]; } Player76;
typedef struct Car952 {
    u8 pad000[232];
    s32 appearance;         /* 0x0E8 */
    u8 pad0EC[3];
    s8 place_locked;        /* 0x0EF */
    u8 pad0F0[857 - 240];
    s8 dead;                /* 0x359 */
    u8 pad35A[896 - 858];
    void *player;           /* 0x380 */
    u8 pad384[902 - 900];
    s16 health;             /* 0x386 */
    u8 pad388[931 - 904];
    s8 points;              /* 0x3A3 */
    u8 pad3A4[940 - 932];
    f32 reloadingTime;      /* 0x3AC */
    u8 pad3B0[952 - 944];
} Car952;
typedef struct Model2056 {
    u8 pad000[8];
    u8 body_type;           /* 0x008 */
    u8 pad009;
    s8 autotrans;           /* 0x00A */
    u8 pad00B[1990 - 11];
    s16 slot;               /* 0x7C6 */
    u8 pad7C8[2];
    s16 we_control;         /* 0x7CA */
    s8 drone_type;          /* 0x7CC */
    u8 pad7CD[2004 - 1997];
    s32 appearance;         /* 0x7D4 */
    u8 pad7D8[2056 - 2008];
} Model2056;
typedef struct Link8 { u8 track; u8 pad1[4]; u8 polepos; u8 flags; u8 owner; } Link8;
typedef struct Cam152 { u8 pad00[36]; f32 pos[3]; u8 pad30[104]; } Cam152;
typedef struct AudMsg { u8 pad0[2]; s8 type; } AudMsg;

extern u32 state_word_a;        /* gGameState */
extern s8 D_80114650;           /* gRestartGame */
extern s8 D_80114654;           /* gGamePointPlayed */
extern s32 gameplay_mode;       /* gRace */
extern s16 active_player_count; /* gNumPlayers */
extern Player76 input_rec0[];   /* gPlayers */
extern Model2056 D_8014A250[];  /* model */
extern Car952 player_array[];   /* gGameCar */
extern Link8 D_80153E88[];      /* gLink */
extern s8 D_80152744;           /* gNumActiveCars */
extern s16 D_8015274C;          /* num_our_drones */
extern s16 D_80152768;          /* num_drones */
extern s16 D_80153FD2;          /* num_humans */
extern s16 D_801527D8[];        /* drones */
extern s16 D_80152808[];        /* our_drones */
extern s16 D_80153E84;          /* sched_to_use */
extern s16 D_80153F08;          /* new_sched_to_use */
extern s16 D_80153F24;          /* sched_index */
extern s16 D_80153F40;          /* cur_drone */
extern s8 D_8013FECB;           /* gCoastFlag */
extern s8 D_80142699;           /* gRetireGame */
extern s8 D_80142690;           /* gOutOfTime */
extern f32 D_80142764;          /* gGravity */
extern s8 D_801427A0;           /* gFogColor */
extern s8 D_8014978C;           /* gTrackNum */
extern f32 D_80154188;          /* HFOV */
extern s16 gNumWindows;
extern Cam152 gCamera[];
extern u8 playgame_settings[][3]; /* gFogColors */
extern s8 D_80143A08[6];        /* carsetup_flag */
extern s8 D_801543C8;           /* gGravityScale */
extern f32 D_801202F8[];        /* gravityArray */
extern u8 D_80142728[];         /* audio lock queue */
extern u8 D_801427A8[];         /* audio command queue */
extern s16 D_801164A4[];        /* gCheatState */
extern s8 D_80151AD8;           /* gUpsideDownTrack */

void init_state_begin(void);            /* SetOptions */
void vsync_wait(s32 on);                /* EnableGameSounds */
void music_tempo_set(s16 slot, u8 body, s32 apply); /* SetupCar */
void init_state_continue(void);         /* _CalcPolePositions */
void func_800FAF50(void);               /* InitCPS */
void func_800FAEE4(void);               /* InitDrones */
void func_800E543C(s32 a, s16 slot);    /* Init_MDrive */
void skid_mark_render(void);            /* ShowHUD */
void object_type1_create(void);         /* AudKillSong */
void object_type7_create(void);         /* AudStopAllEffects */
void func_800A5B3C(void);               /* ZOID_Reset */
void func_800A5488(void);               /* InitBlits */
void tire_compound_set(void);           /* InitWaves */
void func_800EC914(void);               /* munge_gLink_data */
void GhostStart(void);
void func_800F7EB0(void);               /* ResetHighScores */
void func_800A5A40(void);               /* ZOID_ResetWindows */
s32 wheel_render_full(s32 n);           /* ZOID_SplitScreen */
void player_flag_clear_process(void);   /* ReloadTrack */
s32 LoadTrack(s32 track, s32 a1, s32 a2);
void camera_shake_start(void);          /* InitTextureAnimation */
void func_800A7480(s32 a0, s32 a1, s32 r, s32 g, s32 b, s32 a, s32 win); /* ZOID_SetFog */
void func_800AB18C(s32 i, f32 *pos);    /* WorldFogDensity */
void world_physics_tick(void);          /* init_cars */
void music_control(void);               /* StartEnvironment */
void render_post_process(void);         /* init_views */
void func_8038CA24(s16 slot);           /* InitBattlePlayer (image B) */
void entity_audio_update(void);         /* ClearRushControls */
void func_800D6530(void);               /* StartListeners */
void InitBattlePoints(void);
void func_803908D0(void);               /* ShotInit (image B) */
void effect_system_init(void);          /* InitStunt */
void func_800A14FC(void);               /* PS_RumbleGo */
void *memset(void *, s32, u32);

extern s8 D_801494A8[16];       /* gBattlePoints */
extern s8 D_8015256C[4];        /* gTeamPoints */


extern s32 D_80156990, D_80156CE0, D_801613AC, D_80156BAC, D_801613B4;
extern s16 D_8015B254;
extern f32 D_801613B8;
extern s16 D_80140618;
extern s32 D_801406B8;
extern u8 D_8011EA18[];
AudMsg *func_80091B00(void);
s32 osRecvMesg(void *mq, void *msg, s32 flag);
s32 osSendMesg(void *mq, void *msg, s32 flag);

static void ZOID_Reset(void)
{
    D_80156990 = 0;
    D_80156CE0 = 0;
    D_801613AC = 0;
    D_80156BAC = 0;
    D_801613B4 = 0;
    D_8015B254 = -1;
    func_800A5A40();
    D_801613B8 = 1.0f;
    D_80140618 = 0;
    D_801406B8 = (s32)D_8011EA18;
}

void AudLock(void)
{
    osRecvMesg(D_80142728, 0, 1);
}

void AudUnlock(void)
{
    osSendMesg(D_80142728, 0, 0);
}

void AudSend(AudMsg *msg)
{
    if (osSendMesg(D_801427A8, msg, 0)) {
    }
}

void AudKillSong(void)
{
    AudMsg *msg;
    AudLock();
    msg = func_80091B00();
    msg->type = 1;
    AudUnlock();
    AudSend(msg);
}

void AudStopAllEffects(void)
{
    AudMsg *msg;
    AudLock();
    msg = func_80091B00();
    msg->type = 7;
    AudUnlock();
    AudSend(msg);
}

typedef struct StuntInfo { s32 active; u8 pad[120]; } StuntInfo;
extern u8 D_80152038[480];      /* gStuntPlayer */
extern StuntInfo D_801569B8[4]; /* gStuntInfo */
extern f32 D_8016139C;          /* eTime */
extern s32 D_80152738;          /* gStuntsDone */

static void ResetStunts(void)
{
    s32 playerIdx;
    for (playerIdx = 0; playerIdx < 4; playerIdx++) {
        memset(&D_801569B8[playerIdx], 0, sizeof(StuntInfo));
        D_801569B8[playerIdx].active |= 0x15000000;
    }
}

void effect_system_init(void)
{
    memset(D_80152038, 0, sizeof(D_80152038));
    ResetStunts();
    D_8016139C = 0.0f;
    D_80152738 = 0;
}

void InitBattlePoints(void)
{
    memset(D_801494A8, 0, 16);
    memset(D_8015256C, 0, 4);
}

s32 display_list_flush(s32 useHud, s32 cancel)
{
    static s8 inited = 0;
    s32 i, slot, rrr;

    if (cancel) { inited = 0; return 0; }
    if (!inited) {
        inited = 1;
        init_state_begin();
        vsync_wait(!(state_word_a & 8) && !(state_word_a & 0x2000000));
        if (state_word_a & 0x2000000) {
            D_8015274C = 0;
            *(volatile s16 *)&D_80152768 = D_8015274C;
            *(volatile s16 *)&D_80153FD2 = D_8015274C;
            for (i = 0; i < active_player_count; i++) {
                slot = input_rec0[i].slot;
                D_80153E88[slot].owner -= 6;
                player_array[slot].dead = 0;
                player_array[slot].place_locked = 0;
                player_array[slot].player = 0;
                D_8014A250[slot].drone_type = 1;
                D_8014A250[slot].we_control = 1;
                D_8014A250[slot].autotrans = 1;
                D_8014A250[slot].appearance = 0;
                player_array[slot].appearance = 0;
                music_tempo_set(slot, D_8014A250[slot].body_type, 1);
                D_801527D8[D_80152768++] = i;
                D_80152808[D_8015274C++] = i;
            }
            for (; i < D_80152744; i++) {
                player_array[i].dead = 0;
                player_array[i].place_locked = 0;
                player_array[i].appearance = 0;
                D_8014A250[i].appearance = 0;
                music_tempo_set(i, D_8014A250[i].body_type, 1);
                D_801527D8[D_80152768++] = i;
                D_80152808[D_8015274C++] = i;
            }
            D_80153F08 = D_80153E84 = D_8015274C;
            D_80153F24 = -1;
            D_80153F40 = 0;
            init_state_continue();
            func_800FAF50();
            func_800FAEE4();
            D_8013FECB = 0;
            D_80142699 = 0;
            D_80142764 = 32.2f;
            for (i = 0; i < D_80152744; i++) func_800E543C(0, D_8014A250[i].slot);
            skid_mark_render();
            inited = 0;
            return 1;
        }
        D_80154188 = 90.0f;
        if (!(state_word_a & 8)) AudKillSong();
        if (D_80114650) { ZOID_Reset(); func_800A5488(); }
        else if (!(state_word_a & 8)) tire_compound_set();
        func_800EC914();
        GhostStart();
        func_800F7EB0();
    }
    if (D_801427A0 < 19) D_801427A0 = D_8014978C;
    if (!(state_word_a & 8)) {
        D_80154188 = 90.0f;
        func_800A5A40();
        gNumWindows = gameplay_mode == 2 ? (s16)1 : active_player_count;
        wheel_render_full(gNumWindows);
    }
    if (D_80114650) player_flag_clear_process();
    else {
        rrr = LoadTrack(D_8014978C, 1, 0);
        if (!rrr) return 0;
    }
    camera_shake_start();
    D_80114650 = 0;
    for (i = 0; i < gNumWindows; i++) {
        func_800A7480(1000, 999, playgame_settings[D_801427A0][0], playgame_settings[D_801427A0][1], playgame_settings[D_801427A0][2], 255, i);
        func_800AB18C(i, gCamera[i].pos);
    }
    if (!(state_word_a & 8)) AudStopAllEffects();
    func_800FAF50();
    world_physics_tick();
    for (i = 0; i < 6; i++) D_80143A08[i] = 0;
    func_800FAEE4();
    D_8013FECB = 0; D_80142699 = 0; D_80142690 = 0;
    D_80142764 = 32.2f * D_801202F8[D_801543C8];
    for (i = 0; i < D_80152744; i++) {
        slot = D_8014A250[i].slot;
        if (D_80153E88[slot].owner == 0 || D_80153E88[slot].owner == 6) func_800E543C(0, slot);
    }
    music_control();
    render_post_process();
    if (useHud) skid_mark_render();
    for (i = 0; i < active_player_count; i++) {
        input_rec0[i].numCrashes = 0;
        player_array[i].health = 800;
        player_array[i].points = 0;
        player_array[i].reloadingTime = 0.0f;
        if (gameplay_mode == 6) func_8038CA24(i);
        D_80114654 = 0;
    }
    if (!(state_word_a & 8)) entity_audio_update();
    func_800D6530();
    if (gameplay_mode == 6) { InitBattlePoints(); func_803908D0(); }
    if (gameplay_mode == 4) effect_system_init();
    if (!(state_word_a & 8)) func_800A14FC();
    D_80151AD8 = D_801164A4[3];
    inited = 0;
    return 1;
}
