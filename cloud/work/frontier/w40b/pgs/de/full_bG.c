/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* playgame_state_change (0x800CA3B4) = updateGameState@state.c with initGameState (Dreamcast text + N64 additions)
 * inlined; display_enable (0x800C8FA4) = LoadFrontEndOverlay@state.c (N64 body). w40b draft c1. */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct PSFile { u8 pad0[8]; void *fileh; } PSFile;
typedef struct PadState { u8 pad0[72]; PSFile **save; } PadState;   /* gPlayers, 76 bytes */
typedef struct EnvObj { struct EnvObj **next; } EnvObj;
typedef struct { u32 opaque[6]; } OSMesgQueue;
s32 osRecvMesg(OSMesgQueue *mq, void **msg, s32 flag);
s32 osSendMesg(OSMesgQueue *mq, void *msg, s32 flag);

extern u32 state_word_a;                /* gGameState */
extern u32 state_word_b;                /* gNewGameState */
extern u32 D_801174BC;                  /* gSkipToGameState */
extern s8 D_801461F8, D_80146205, D_80149414, D_80149774, D_80149794, D_801497C4;
extern s8 D_80156994;
extern s8 D_80146108[];                 /* gOptions */
extern s16 active_player_count;         /* gNumPlayers */
extern s16 gNumWindows;
extern PadState input_rec0[];           /* gPlayers */
extern s8 D_80151AD8;                   /* gUpsideDownTrack */
extern s8 D_80114650;                   /* gRestartGame */
extern s32 gameplay_mode;               /* gRace */
extern EnvObj **D_8012E6E0;
extern u8 playgame_settings[][3];       /* gFogColors */
extern s8 D_8011ED00;                   /* overlay "Start" (image A) loaded */
extern s8 D_8011ED04;                   /* overlay "Extra" (image B) loaded */

void state_change_preprocess(void);     /* uninitGameState */
void tire_compound_set(void);           /* InitWaves */
void hud_setup(u8 a0, u8 a1, u8 a2, s32 numA, s32 numB, f32 a5, f32 a6, s32 a7); /* AudioInit */
void func_800C885C(void);
void init_state_begin(void);            /* SetOptions */
void viScheduleTick(f32 t);             /* SetCountdownTimer */
void game_mode_handler(void);
void func_800C9480(void);               /* InitRecords */
void func_800B5570(u32 state);          /* setGameState */
void player_cleanup_slots(void);        /* _UnloadFontWaves */
void scene_cleanup_slots(void);         /* _UnloadFrontEndWaves */
void resource_slots_clear_multiple(void); /* _LoadFrontEndWaves */
void speed_mode0_wrapper(f32 vol, f32 time); /* AudSetMusicVol */
void speed_mode1_wrapper(f32 vol, f32 time); /* AudSetFxVol */
void player_state_set(s32 p, s32 on);   /* SetControllerActive */
void player_mode_set(s32 p, s32 on);    /* SetControllerStickEdges */
void func_800A3424(void *fileh, s32 lock); /* PS_LockFile */
void sync_entry_register(s32 song, s32 a1); /* AudPlaySong */
void object_render_cleanup(EnvObj **n);
void InitMaxPath(void);                 /* load overlay "Extra" */
void sync_maxpath_to_checkpoint(void);  /* load overlay "Start" */
void MP_TargetSpeed(void);              /* unload "Extra" */
void assign_default_paths(void);        /* unload "Start" */
s32 audio_frame_sync(s32 wave, s32 a1, s32 a2, s32 a3, void *a4); /* LoadWave */
void visual_objects_update(s32 a);      /* SaveFlush */
void hud_speed_display(s32 numA, s32 numB, f32 a2, f32 a3, s32 a4);
s32 object_create(s32 font);            /* HAL_SetFont */
void SaveServer(s32 forceUpdate, s32 rumbleOnly);
void *func_800A7480(s16 a0, s16 a1, u8 r, u8 g, u8 b, u8 a, u32 win); /* ZOID_SetFog */

extern u8 D_8038A400[];
extern s32 D_80152770[];                /* heap semaphore queue */
void audio_reverb_update(u32 address, s32 tag); /* _DeallocateMemory (internal, IPA a1/a2) */

void display_enable(s32 load)           /* LoadFrontEndOverlay */
{
    static s32 sLoaded = 0;
    static s8 sStart = 0;
    static s8 sExtra = 0;
    if (load) {
        if (!sLoaded) {
            if (sExtra) {
                osRecvMesg((OSMesgQueue *)D_80152770, (void **)0, 1);
                audio_reverb_update((u32)D_8038A400, 0);
                osSendMesg((OSMesgQueue *)D_80152770, (void *)0, 0);
                sExtra = 0;
            }
            sync_maxpath_to_checkpoint();
            sLoaded = 1;
        }
    } else if (sLoaded) {
        if (sStart) {
            osRecvMesg((OSMesgQueue *)D_80152770, (void **)0, 1);
            audio_reverb_update((u32)D_8038A400, 0);
            osSendMesg((OSMesgQueue *)D_80152770, (void *)0, 0);
            sStart = 0;
        }
        sLoaded = 0;
    }
}

void func_800C9150(void);
void func_800CA2F8(void);

static void initGameState(void)
{
    s32 i;

    if (state_word_a & 0x1) {
        func_800CA2F8();
        D_801461F8 = 0;
        D_80146205 = 0;
        D_80149414 = 0;
        D_80149774 = 0;
        D_80149794 = 0;
        D_801497C4 = 0;
        tire_compound_set();
        if (!D_80156994) {
            hud_setup(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
            func_800C885C();
        }
        display_enable(1);
        active_player_count = 1;
        gNumWindows = 1;
        init_state_begin();
        viScheduleTick(30.0f);
        game_mode_handler();
        game_mode_handler();
        game_mode_handler();
        func_800C9480();
        func_800B5570(0x2);
    } else if (state_word_a & 0x2) {
        func_800CA2F8();
        player_cleanup_slots();
        tire_compound_set();
        if (!D_80156994) {
            hud_setup(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
            func_800C885C();
        }
        speed_mode1_wrapper(D_80146108[13] / 10.0f, 0.1f);
        display_enable(1);
        resource_slots_clear_multiple();
        player_state_set(-1, 0);
        player_state_set(0, 1);
        player_mode_set(-1, 1);
    } else if (state_word_a & 0x40000000) {
    } else if (state_word_a & 0x4) {
        player_cleanup_slots();
        tire_compound_set();
        if (!D_80156994) {
            hud_setup(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
            func_800C885C();
        }
        speed_mode1_wrapper(D_80146108[13] / 10.0f, 0.1f);
        display_enable(1);
        resource_slots_clear_multiple();
        if (!(D_801174BC & 0x7cdfc0)) {
            for (i = 0; i < active_player_count; i++) {
                if (input_rec0[i].save) {
                    func_800A3424((*input_rec0[i].save)->fileh, 0);
                }
            }
        }
        player_state_set(-1, 0);
        player_state_set(0, 1);
        player_mode_set(-1, 1);
        if (D_80156994 && !(D_801174BC & 0x7c0000) && D_80146108[14] < 13) {
            speed_mode0_wrapper(D_80146108[12] / 10.0f, 0.05f);
            sync_entry_register(6, 1);
        }
        D_80151AD8 = 0;
    } else if (state_word_a & 0x8) {
        EnvObj **n;
        for (n = D_8012E6E0; n; n = (*n)->next) {
            object_render_cleanup(n);
        }
        scene_cleanup_slots();
        player_cleanup_slots();
        tire_compound_set();
        if (!D_80156994) {
            hud_setup(8, 8, 0, 0, 0, 1.0f, 200.0f, 0);
        }
        display_enable(0);
        if (D_80156994) {
            InitMaxPath();
        }
    } else if (state_word_a & 0x10) {
        player_state_set(-1, 0);
        player_state_set(0, 1);
        player_mode_set(-1, 1);
    } else if (state_word_a & 0x20) {
        s32 p;

        for (p = 0; p < active_player_count; p++) {
            player_state_set(p, 1);
            player_mode_set(p, 1);
        }
        for (; p < 4; p++) {
            player_state_set(p, 0);
            if (p > 1) {
            }
        }
    } else if (state_word_a & 0x40) {
        player_state_set(-1, 0);
        player_state_set(0, 1);
    } else if (state_word_a & 0x200) {
    } else if (state_word_a & 0x400) {
    } else if (state_word_a & 0x800) {
    } else if (state_word_a & 0x1000) {
    } else if (state_word_a & 0x2000) {
    } else if (state_word_a & 0x8000000) {
    } else if (state_word_a & 0x4000) {
        audio_frame_sync(57, 0, 0, 0, 0);
        func_800C9150();
    } else if (state_word_a & 0x100) {
        s32 p;

        audio_frame_sync(56, 0, 0, 0, 0);
        for (p = 0; p < active_player_count; p++) {
            player_state_set(p, 1);
            player_mode_set(p, 1);
        }
        for (; p < 4; p++) {
            player_state_set(p, 0);
            if (p > 1) {
            }
        }
    } else if (state_word_a & 0x80) {
        audio_frame_sync(60, 0, 0, 0, 0);
        player_state_set(-1, 0);
        player_state_set(0, 1);
        func_800C9150();
    } else if (state_word_a & 0x40000) {
        if (!D_80114650) {
            scene_cleanup_slots();
            player_cleanup_slots();
            tire_compound_set();
        }
        display_enable(0);
        if (!D_80114650) {
            if (!D_80156994 && gameplay_mode != 6 && gameplay_mode != 4 && gameplay_mode != 5) {
                hud_setup(8, 8, 0, 0, 0, 1.0f, 200.0f, 0);
            }
            if (!D_80156994 && gameplay_mode == 6) {
                hud_speed_display(5, 64, 1.0f, 200.0f, 0);
            }
            visual_objects_update(1);
        }
        if (gameplay_mode == 6 || gameplay_mode == 4 || gameplay_mode == 5 || D_80156994) {
            InitMaxPath();
        }
        if (!D_80114650) {
            object_create(13);
            object_create(10);
            object_create(11);
            object_create(12);
            object_create(4);
            object_create(1);
            object_create(2);
            object_create(3);
            object_create(9);
            object_create(8);
            object_create(5);
            object_create(6);
            object_create(7);
        }
        player_mode_set(-1, 0);
    } else if (state_word_a & 0x1000000) {
        player_mode_set(-1, 1);
    } else if (state_word_a & 0x800000) {
        SaveServer(1, 0);
        visual_objects_update(1);
    } else if (state_word_a & 0x20000000) {
        scene_cleanup_slots();
        tire_compound_set();
        audio_frame_sync(55, 0, 0, 0, 0);
    }
    if (state_word_a & 0x7c03fffe) {
        func_800A7480(1000, 999, playgame_settings[19][0], playgame_settings[19][1], playgame_settings[19][2], 0xff, 0);
        func_800A7480(1000, 999, playgame_settings[19][0], playgame_settings[19][1], playgame_settings[19][2], 0xff, 1);
    }
}

void playgame_state_change(void)        /* updateGameState */
{
    if (state_word_b != state_word_a) {
        state_change_preprocess();
        state_word_a = state_word_b;
        initGameState();
    }
}

void func_800C9150(void)
{
}

void func_800CA2F8(void)
{
}
