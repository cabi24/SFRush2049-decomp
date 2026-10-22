/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* RaceStateMachine_Update (0x800DB81C) = MAT3 pausescr.c updatePauseScreen (GC 0x80077AA8). NOT A MATCH:
 * 524/537 words, every other word identical; missing are retail's 13 `li t0,2` reloads (constant-2 web live over the
 * whole switch in retail, only PauseTargets + end chain here). Score with blob_unit (game unit, kept).
 * Levers: audio commands = kept functions over internal AudLock/AudUnlock/AudSend (inlined at all 5 sites, spill homes
 * 244/180/132/100/68 = retail); AudSend `if (osSendMesg(...)) {}` (retail's stray beqz v0 in the retire path; disclosed);
 * PauseGameMusic / PlaySelectSfx / SetPausePlayer kept wrappers (inline slots: frame 272 = retail; the choice of which
 * calls are wrappers is not unique); `pressed = (edges & 1) || !attached || 0;`; gControllerData volatile.
 * See cloud/work/frontier/w36p/RESULTS.md. */
/* RaceStateMachine_Update (0x800DB81C) = MAT3 pausescr.c updatePauseScreen (GC 0x80077AA8, size 0x3D0). */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Player76 { u8 slot; u8 pad01[3]; s32 edges; u8 pad08[68]; } Player76;
typedef struct Controller16 { s8 attached; u8 pad[15]; } Controller16;
typedef struct Target { struct Target *next; u8 pad04[92]; s32 f96; s8 f100; } Target;
typedef struct AudMsg { u8 pad0[2]; s8 type; } AudMsg;

extern s32 D_801170FC;          /* gPauseState */
extern s32 D_80149D98;          /* pauseMenuSelect */
extern s32 D_8015698C;          /* playerInPause */
extern s16 gNumWindows;
extern Player76 input_rec0[];   /* gPlayers */
extern volatile Controller16 D_80156CF0[]; /* gControllerData */
extern s8 D_80117350;           /* gControllerRemoved */
extern s8 D_80117354;           /* gYes */
extern s8 D_80114650;           /* gRestartGame */
extern s8 D_80142699;           /* gRetireGame */
extern f32 D_801543CC;          /* gModelTime */
extern s8 D_80157244;           /* gSoftReset */
extern u32 state_word_a;        /* gGameState */
extern Target *D_80143FD8;      /* target list */
extern s8 D_8015418C[];         /* control screen entered from pause, per player */
extern u8 D_80142728[];         /* audio lock queue */
extern u8 D_801427A8[];         /* audio command queue */

void SaveServer(s32 forceUpdate, s32 rumbleOnly);
void func_800B4FB0(s32 state);          /* setPauseState */
void viEnableAccum(void);               /* PauseTimer */
void viDisableAccum(void);              /* ResumeTimer */
AudMsg *func_80091B00(void);
s32 osRecvMesg(void *mq, void *msg, s32 flag);
s32 osSendMesg(void *mq, void *msg, s32 flag);
void speed_mode0_wrapper(f32 vol, f32 time); /* AudSetMusicVol */
void func_800D5374(void);               /* StopListeners */
void players_frame_update(void);        /* modelstop */
void func_800D6530(void);               /* StartListeners */
void func_800D60AC(void);               /* modelgo */
void physics_init_mode0(void);          /* StartGameMusic */
void physics_init_mode1(void);          /* ResumeGameMusic */
void player_mode_set(s32 player, s32 value); /* SetControllerStickEdges */
void visual_objects_update(s32 action); /* SaveFlush */
u32 entity_flags_apply(u32 fx, u32 sub, u32 replace, u8 prio); /* AudPlayEffect */
void func_800DB1E0(void);               /* updateOptionScreen */
void func_800D91A0(void);               /* updateControlScreen */
void func_800D7634(void);               /* updateAudioScreen */
void physics_sym(void);                 /* updateVideoScreen */
void audio_distance_atten(s32 edges);   /* PlayUDSound */
void audio_doppler(s32 edges);          /* PlayLRSound */
void resource_type_select(s32 edges);   /* PlayStartABSound */
void func_800D5A04(void);               /* AbortGame */
void voice_stop(void);

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

void AudPauseAllEffects(void)
{
    AudMsg *msg;
    AudLock();
    msg = func_80091B00();
    msg->type = 7;
    AudUnlock();
    AudSend(msg);
}

void AudResumeAllEffects(void)
{
    AudMsg *msg;
    AudLock();
    msg = func_80091B00();
    msg->type = 9;
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

void PauseGameMusic(void)
{
    speed_mode0_wrapper(0.0f, 0.05f);
}

void PlaySelectSfx(void)
{
    entity_flags_apply(37, 0, 1, 0);
}

void PlayPauseSfx(void)
{
    entity_flags_apply(46, 0, 1, 0);
}

void SetPausePlayer(s32 player)
{
    D_8015698C = player;
    D_80117350 = !D_80156CF0[D_8015698C].attached;
}

static void PauseTargets(void)
{
    Target *t;
    for (t = D_80143FD8; t; t = t->next) {
        t->f100 = 2;
        t->f96 = -1;
    }
}

void RaceStateMachine_Update(void)
{
    s32 i;
    s32 pressed;

    if (D_801170FC) SaveServer(0, 0);
    switch (D_801170FC) {
    case 0:
        for (i = 0; i < gNumWindows; i++) {
            pressed = (input_rec0[i].edges & 1) || !D_80156CF0[i].attached || 0;
            if (pressed) {
                SetPausePlayer(i);
                goto pause_game;
            }
        }
        return;
    pause_game:
        func_800B4FB0(1);
        D_80149D98 = 0;
        viEnableAccum();
        AudPauseAllEffects();
        PauseGameMusic();
        func_800D5374();
        players_frame_update();
        PauseTargets();
        player_mode_set(-1, 1);
        visual_objects_update(1);
        entity_flags_apply(46, 0, 1, 0);
        break;
    case 5: func_800DB1E0(); break;
    case 4: func_800D91A0(); break;
    case 2: func_800D7634(); break;
    case 3: physics_sym(); break;
    case 1:
        if (input_rec0[D_8015698C].edges & 2) {
            PlaySelectSfx();
            switch (D_80149D98) {
            case 0:
            resume_game:
                func_800B4FB0(0);
                viDisableAccum();
                func_800D6530();
                AudResumeAllEffects();
                physics_init_mode1();
                if (!(state_word_a & 0x200000)) physics_init_mode0();
                func_800D60AC();
                player_mode_set(-1, 0);
                return;
            case 6:
                if (D_801543CC < 15.0f) {
                    D_80114650 = 1;
                    goto abort_game;
                } else func_800B4FB0(7);
                break;
            case 7:
                if (D_801543CC < 15.0f) {
                    D_80114650 = 0;
                    if (state_word_a & 0x400000) goto retire_game;
                    goto abort_game;
                } else func_800B4FB0(8);
                break;
            case 3: func_800B4FB0(2); break;
            case 4: func_800B4FB0(3); break;
            case 1: func_800B4FB0(5); break;
            case 2:
                D_8015418C[D_8015698C] = 1;
                func_800B4FB0(4);
                break;
            case 5: func_800B4FB0(6); break;
            }
        } else if (input_rec0[D_8015698C].edges & 0x400) {
            audio_distance_atten(input_rec0[D_8015698C].edges);
            if (--D_80149D98 < 0) D_80149D98 = 7;
        } else if (input_rec0[D_8015698C].edges & 0x800) {
            audio_distance_atten(input_rec0[D_8015698C].edges);
            if (++D_80149D98 >= 8) D_80149D98 = 0;
        } else if (input_rec0[D_8015698C].edges & 5) {
            resource_type_select(input_rec0[D_8015698C].edges);
            goto resume_game;
        }
        break;
    case 7: case 8:
        if (input_rec0[D_8015698C].edges & 0x3000) {
            D_80117354 = !D_80117354;
            audio_doppler(input_rec0[D_8015698C].edges);
        }
        if (input_rec0[D_8015698C].edges & 2) {
            if (D_80117354) {
                if ((state_word_a & 0x400000) && D_801170FC == 8) {
                retire_game:
                    D_80142699 = 1;
                    func_800B4FB0(0);
                    viDisableAccum();
                    AudStopAllEffects();
                } else {
                    D_80114650 = D_801170FC == 7;
                abort_game:
                    func_800B4FB0(0);
                    viDisableAccum();
                    AudStopAllEffects();
                    func_800D5A04();
                }
            } else func_800B4FB0(1);
        } else if (input_rec0[D_8015698C].edges & 5) {
            resource_type_select(input_rec0[D_8015698C].edges);
            func_800B4FB0(1);
        }
        break;
    case 6:
        if (input_rec0[D_8015698C].edges & 7) {
            resource_type_select(input_rec0[D_8015698C].edges);
            func_800B4FB0(1);
        }
        break;
    }
    if (D_801170FC) {
        if (!D_80156CF0[D_8015698C].attached != D_80117350) {
            D_80117350 = !D_80156CF0[D_8015698C].attached;
            if (D_801170FC != 5 && D_801170FC != 4 && D_801170FC != 2 && D_801170FC != 3) voice_stop();
        }
    }
    if (D_80157244) {
        D_80114650 = 0;
        func_800B4FB0(0);
        viDisableAccum();
        AudStopAllEffects();
        func_800D5A04();
    }
}
