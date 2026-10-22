/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* playgame_state_change (0x800CA3B4) = MAT3 state.c updateGameState + initGameState. NOT A MATCH.
 * Unit: --internal FontLock --internal FontUnlock --block resource_slots_clear_multiple --block InitMaxPath
 * --block func_800C885C --keep object_create --internal display_enable  ->  ops 2, ~80 words (v0/v1 colour, frame 224/232,
 * s5 save from the display_enable reconstruction). The blockers break PrintScores/func_800C8918, so they stand in for
 * umerge's caller budget (retail inlines only the last 3 object_create calls). See cloud/work/frontier/w36p/RESULTS.md. */
/* playgame_state_change (0x800CA3B4) = MAT3 state.c updateGameState with initGameState inlined. */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct AudioCfg { u8 pad0[12]; s8 music; s8 sfx; s8 track; s8 mono; } AudioCfg;
typedef struct Handle { u8 pad0[8]; void *fileh; } Handle;
typedef struct Player76 { u8 pad00[72]; Handle **save; } Player76;
typedef struct Node { struct Node **next; } Node;

extern u32 state_word_a;        /* gGameState */
extern u32 state_word_b;        /* gNewGameState */
extern u32 D_801174BC;          /* gSkipToGameState */
extern s8 D_801461F8, D_80146205, D_80149414, D_80149774, D_80149794, D_801497C4;
extern s8 D_80156994;
extern AudioCfg D_80146108;     /* audio options */
extern s16 active_player_count; /* gNumPlayers */
extern s16 gNumWindows;
extern Player76 input_rec0[];   /* gPlayers */
extern s8 D_80151AD8;           /* gUpsideDownTrack */
extern s8 D_80114650;           /* gRestartGame */
extern s32 gameplay_mode;       /* gRace */
extern Node *D_8012E6E0;
extern u8 D_801461D0[];         /* font lock queue */
extern u8 playgame_settings[][3]; /* gFogColors */

void state_change_preprocess(void);     /* uninitGameState */
void tire_compound_set(void);           /* InitWaves */
void hud_setup(u8 a0, u8 a1, u8 a2, s32 numA, s32 numB, f32 a5, f32 a6, s32 a7);
void func_800C885C(void);
void display_enable(s32 load);          /* LoadFrontEndOverlay */
void init_state_begin(void);            /* SetOptions */
void viScheduleTick(f32 t);             /* SetCountdownTimer */
void game_mode_handler(void);
void func_800C9480(void);
void func_800B5570(u32 state);          /* setGameState */
void player_cleanup_slots(void);        /* _UnloadFontWaves */
void speed_mode0_wrapper(f32 vol, f32 time);
void speed_mode1_wrapper(f32 vol, f32 time);
void resource_slots_clear_multiple(void);
void player_state_set(s32 p, s32 on);   /* SetControllerActive */
void player_mode_set(s32 p, s32 on);    /* SetControllerStickEdges */
void func_800A3424(void *fileh, s32 lock); /* PS_LockFile */
void sync_entry_register(s32 song, s32 a1); /* AudPlaySong */
void object_render_cleanup(Node *n);
void scene_cleanup_slots(void);         /* _UnloadFrontEndWaves */
void InitMaxPath(void);
void audio_frame_sync(s32 wave, s32 a1, s32 a2, s32 a3, s32 a4); /* LoadWave */
void visual_objects_update(s32 a);      /* SaveFlush */
void hud_speed_display(s32 numA, s32 numB, f32 a2, f32 a3, s32 a4);
void object_create(s32 font);           /* HAL_SetFont */
s32 slot_state_setup(s32 font);
s32 osRecvMesg(void *mq, void *msg, s32 flag);
s32 osSendMesg(void *mq, void *msg, s32 flag);
void SaveServer(s32 forceUpdate, s32 rumbleOnly);
void func_800A7480(s32 a0, s32 a1, s32 r, s32 g, s32 b, s32 a, s32 win); /* ZOID_SetFog */


static s32 D_801174C0;          /* front-end overlay loaded (state.c) */
extern u8 D_8038A400[];         /* overlay load address */
extern s8 D_8011ED00;           /* overlay "Start" loaded */
extern s8 D_8011ED04;           /* overlay "Extra" loaded */
void func_800960CC(u32 address);        /* heap free (locked) */
void sync_maxpath_to_checkpoint(void);  /* load overlay "Start" */

void display_enable(s32 load)
{
    if (load) {
        if (!D_801174C0) {
            if (D_8011ED04) {
                func_800960CC((u32)D_8038A400);
                D_8011ED04 = 0;
            }
            sync_maxpath_to_checkpoint();
            D_801174C0 = 1;
        }
    } else if (D_801174C0) {
        if (D_8011ED00) {
            func_800960CC((u32)D_8038A400);
            D_8011ED00 = 0;
        }
        D_801174C0 = 0;
    }
}

void StartFrontEndMusic(void)
{
    speed_mode0_wrapper(D_80146108.music / 10.0f, 0.05f);
    sync_entry_register(6, 1);
}

void FontLock(void)
{
    osRecvMesg(D_801461D0, 0, 1);
}

void FontUnlock(void)
{
    osSendMesg(D_801461D0, 0, 0);
}

s32 LoadFontLocked(s32 font)
{
    s32 old;
    FontLock();
    old = slot_state_setup(font);
    FontUnlock();
    return old;
}

void FrontEndHeap(void)
{
    if (!D_80156994) {
        hud_setup(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
        func_800C885C();
    }
}

void SetSfxVolume(void)
{
    speed_mode1_wrapper(D_80146108.sfx / 10.0f, 0.1f);
}

void leStartLoadingEffect(void)
{
}

void lePrepareLoading(void)
{
}

void leEndLoadingEffect(void)
{
}

static void initGameState(void)
{
    s32 i;

    if (state_word_a & 1) {
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
        func_800B5570(2);
    } else if (state_word_a & 2) {
        player_cleanup_slots();
        tire_compound_set();
        if (!D_80156994) {
            hud_setup(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
            func_800C885C();
        }
        SetSfxVolume();
        display_enable(1);
        resource_slots_clear_multiple();
        player_state_set(-1, 0);
        player_state_set(0, 1);
        player_mode_set(-1, 1);
    } else if (state_word_a & 0x40000000) {
    } else if (state_word_a & 4) {
        player_cleanup_slots();
        tire_compound_set();
        if (!D_80156994) {
            hud_setup(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
            func_800C885C();
        }
        SetSfxVolume();
        display_enable(1);
        resource_slots_clear_multiple();
        if (!(D_801174BC & 0x7cdfc0)) {
            for (i = 0; i < active_player_count; i++) {
                if (input_rec0[i].save) func_800A3424((*input_rec0[i].save)->fileh, 0);
            }
        }
        player_state_set(-1, 0);
        player_state_set(0, 1);
        player_mode_set(-1, 1);
        if (D_80156994 && !(D_801174BC & 0x7c0000) && D_80146108.track < 13) StartFrontEndMusic();
        D_80151AD8 = 0;
    } else if (state_word_a & 8) {
        Node *n;
        for (n = D_8012E6E0; n; n = *n->next) object_render_cleanup(n);
        scene_cleanup_slots();
        player_cleanup_slots();
        tire_compound_set();
        if (!D_80156994) hud_setup(8, 8, 0, 0, 0, 1.0f, 200.0f, 0);
        display_enable(0);
        if (D_80156994) InitMaxPath();
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
        leStartLoadingEffect();
        lePrepareLoading();
        audio_frame_sync(57, 0, 0, 0, 0);
        leEndLoadingEffect();
    } else if (state_word_a & 0x100) {
        s32 p;
        leStartLoadingEffect();
        lePrepareLoading();
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
        leStartLoadingEffect();
        lePrepareLoading();
        audio_frame_sync(60, 0, 0, 0, 0);
        player_state_set(-1, 0);
        player_state_set(0, 1);
        leEndLoadingEffect();
    } else if (state_word_a & 0x40000) {
        if (!D_80114650) {
            scene_cleanup_slots();
            player_cleanup_slots();
            tire_compound_set();
        }
        display_enable(0);
        if (!D_80114650) {
            if (!D_80156994 && gameplay_mode != 6 && gameplay_mode != 4 && gameplay_mode != 5)
                hud_setup(8, 8, 0, 0, 0, 1.0f, 200.0f, 0);
            if (!D_80156994 && gameplay_mode == 6) hud_speed_display(5, 64, 1.0f, 200.0f, 0);
            visual_objects_update(1);
        }
        if (gameplay_mode == 6 || gameplay_mode == 4 || gameplay_mode == 5 || D_80156994) InitMaxPath();
        if (!D_80114650) {
            object_create(13); object_create(10); object_create(11); object_create(12);
            object_create(4); object_create(1); object_create(2); object_create(3);
            object_create(9); object_create(8);
            LoadFontLocked(5);
            LoadFontLocked(6);
            LoadFontLocked(7);
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
        func_800A7480(1000, 999, playgame_settings[19][0], playgame_settings[19][1], playgame_settings[19][2], 255, 0);
        func_800A7480(1000, 999, playgame_settings[19][0], playgame_settings[19][1], playgame_settings[19][2], 255, 1);
    }
}

void playgame_state_change(void)
{
    if (state_word_b != state_word_a) {
        state_change_preprocess();
        state_word_a = state_word_b;
        initGameState();
    }
}
