/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * state.c (N64): MAT3 state.c / Dreamcast state.c with the N64 additions read from retail. w40b.
 *
 * playgame_state_change (0x800CA3B4, 2,544 B) = updateGameState@state.c with initGameState (func_800CA2F8, umerge-
 *   inlined) and setGameState (func_800B5570) inlined. Base text: the Dreamcast initGameState (it keeps the N64 13-font
 *   preload in state 0x40000 and SaveFlush(1)); N64 additions: the AudioInit block (hud_setup + func_800C885C) after
 *   InitWaves, AudSetFxVol(gOptions[13] / 10, 0.1) before LoadFrontEndOverlay(1) (speed_mode1_wrapper, inlined),
 *   N64 waves 57/56/60/55, the gPlayers PurgePlayerData loop (object_render_cleanup) in state 0x8, the front-end music
 *   block (AudSetMusicVol + AudPlaySong(6, 1)) at the end of state 0x4, the N64 audio re-init in state 0x40000
 *   (AudioInit(8, 8, ...) / hud_speed_display for gRace 6), InitMaxPath (overlay "Extra") and ZOID_SetFog(1000, 999).
 * display_enable (0x800C8FA4, 316 B) = LoadFrontEndOverlay@state.c (GC: empty stub "called with an argument"):
 *   load: unload overlay "Extra" (flag D_8011ED04), load overlay "Start" (sync_maxpath_to_checkpoint, inlined:
 *   PrevMaxPath with its IPA s0-s4 register parameters); unload: unload "Start" (flag D_8011ED00). Its flag `loaded`
 *   is MAT3's function-local static (retail 0x801174C0 = state.c .data after gGameState/gNewGameState/
 *   gSkipToGameState, value 0); as a file-level or extern variable uopt colours its address web into s5 and the
 *   caller then saves s5 (retail saves s0-s4 only).
 *   CONTEXT ONLY (not code-identical): retail has no address webs for D_8011ED04 / D_8011ED00 either; ours colours
 *   them into s3 (inside the s0-s4 clobber set, so the caller is unaffected). Lane finding: the webs vanish when the
 *   two overlay flags are procedure-local statics (micro tests in w40b/pgs/micro), i.e. retail's overlay flags are
 *   most likely function-local statics of a deleted overlay helper (stub func_800A0F64 before MP_TargetSpeed) that
 *   is inlined into the overlay_loader.c wrappers and here. See RESULTS.md.
 *
 * Shaping and disclosed choices (playgame_state_change):
 *  - umerge's knapsack (lane-instrumented umerge, w40b/tools/patch_umerge.py) inlines a multi-caller callee at a
 *    site when size_incr * max(1, C >> 7) / benefit <= 3.0, C = the caller's size measure (+28). HAL_SetFont in the
 *    MAT3 two-level form (halfont.c) has size_incr 9, benefit 5: the font sites are visited from the last one, the
 *    first three (fonts 7, 6, 5) are inlined while C < 256, the fourth pushes C to 257 (factor 2) and every later
 *    candidate with cost > 1.5 (fonts 8..13, InitMaxPath, display_enable) stays a jal, as in retail. This needs
 *    C = 232..239 before the font sites: the four calls of the empty N64 helper func_800C9150 (umerge inlines them
 *    in its first pass, each adding 2 to C: 225 -> 233) do that, and their four 8-byte umerge inline reserves are the
 *    retail frame's 32 bytes between the font homes (132/108/84) and `i` (220): frame 232 (without them: 200 and four
 *    fonts inlined). func_800C9150's retail stub sits right before player_state_set (post-order layout, w38b), so its
 *    last call site is at the end of state 0x80; the four positions used here (before and after LoadWave in 0x4000 and
 *    0x80, MAT3's loading-effect slots) are not otherwise determined by the bytes: any four calls before the font
 *    block give the same code and frame.
 *  - initGameState is func_800CA2F8: written non-static (not kept) so umerge's deleted-procedure stub keeps the symbol;
 *    its retail stub lies where the post-order layout puts initGameState itself (after its last new callee
 *    init_state_begin, before uninitGameState = state_change_preprocess).
 *  - the two compiled-out `if (p > 1) {}` are the Dreamcast text's ("target retains empty per-controller check").
 */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct PSFile { u8 pad0[8]; void *fileh; } PSFile;            /* tPS_File: fileh at +8 */
typedef struct PadState { u8 pad0[72]; PSFile **save; } PadState;    /* gPlayers[], 76 bytes, save at +72 */
typedef struct PlayerData { struct PlayerData **next; } PlayerData;  /* handle list, next handle at +0 */
typedef struct { u32 opaque[6]; } OSMesgQueue;
s32 osRecvMesg(OSMesgQueue *mq, void **msg, s32 flag);
s32 osSendMesg(OSMesgQueue *mq, void *msg, s32 flag);

extern u32 state_word_a;                /* gGameState */
extern u32 state_word_b;                /* gNewGameState */
extern u32 D_801174BC;                  /* gSkipToGameState */
extern s8 D_801461F8, D_80146205, D_80149414, D_80149774, D_80149794, D_801497C4; /* state-1 flags */
extern s8 D_80156994;
extern s8 D_80146108[];                 /* gOptions: [12] music volume, [13] sfx volume, [14] music track */
extern s16 active_player_count;         /* gNumPlayers */
extern s16 gNumWindows;
extern PadState input_rec0[];           /* gPlayers */
extern s8 D_80151AD8;                   /* gUpsideDownTrack */
extern s8 D_80114650;                   /* gRestartGame */
extern s32 gameplay_mode;               /* gRace */
extern PlayerData **D_8012E6E0;
extern u8 playgame_settings[][3];       /* gFogColors */
extern s8 D_8011ED00;                   /* overlay "Start" (image A) loaded */
extern s8 D_8011ED04;                   /* overlay "Extra" (image B) loaded */
extern u8 D_8038A400[];                 /* runtime image base (A and B) */
extern OSMesgQueue D_80152770;          /* heap semaphore */

void state_change_preprocess(void);     /* uninitGameState */
void tire_compound_set(void);           /* InitWaves */
void hud_setup(u8 a0, u8 a1, u8 a2, s32 numA, s32 numB, f32 a5, f32 a6, s32 a7); /* AudioInit */
void func_800C885C(void);
void init_state_begin(void);            /* SetOptions */
void viScheduleTick(f32 t);             /* SetCountdownTimer */
void game_mode_handler(void);
void func_800C9480(void);               /* InitRecords */
void func_800B5570(s32 state);          /* setGameState */
void player_cleanup_slots(void);        /* _UnloadFontWaves */
void scene_cleanup_slots(void);         /* _UnloadFrontEndWaves */
void resource_slots_clear_multiple(void); /* _LoadFrontEndWaves */
void speed_mode0_wrapper(f32 vol, f32 time); /* AudSetMusicVol */
void speed_mode1_wrapper(f32 vol, f32 time); /* AudSetFxVol */
void player_state_set(s32 p, s32 on);   /* SetControllerActive */
void player_mode_set(s32 p, s32 on);    /* SetControllerStickEdges */
void func_800A3424(void *fileh, s32 lock); /* PS_LockFile */
void sync_entry_register(s32 song, s32 a1); /* AudPlaySong */
void object_render_cleanup(PlayerData **h); /* PurgePlayerData */
void InitMaxPath(void);                 /* load overlay "Extra" */
void sync_maxpath_to_checkpoint(void);  /* load overlay "Start" */
s32 audio_frame_sync(s32 wave, s32 a1, s32 a2, s32 a3, void *a4); /* LoadWave */
void visual_objects_update(s32 a);      /* SaveFlush */
void hud_speed_display(s32 numA, s32 numB, f32 a2, f32 a3, s32 a4);
s32 object_create(s32 fontID);          /* HAL_SetFont (halfont.c) */
void SaveServer(s32 forceUpdate, s32 rumbleOnly);
void *func_800A7480(s16 a0, s16 a1, u8 r, u8 g, u8 b, u8 a, u32 win); /* ZOID_SetFog */
void audio_reverb_update(u32 address, s32 tag); /* _DeallocateMemory (internal, IPA a1/a2) */
void func_800C9150(void);               /* empty N64 helper, deleted after inlining (stub 0x800C9150) */
void func_800CA2F8(void);               /* initGameState */

void func_800A0F64(s32 extra, s32 load);
void display_enable(s32 load)           /* LoadFrontEndOverlay */
{
    static s32 loaded = 0;
    if (load) {
        if (!loaded) {
            func_800A0F64(1, 0);
            func_800A0F64(0, 1);
            loaded = 1;
        }
    } else if (loaded) {
        func_800A0F64(0, 0);
        loaded = 0;
    }
}

void func_800CA2F8(void)                /* initGameState@state.c (static in MAT3; deleted after inlining, stub 0x800CA2F8) */
{
    s32 i;

    if (state_word_a & 0x1) {
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
        PlayerData **h;
        for (h = D_8012E6E0; h; h = (*h)->next) {
            object_render_cleanup(h);
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
        func_800C9150();
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
        func_800C9150();
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
        func_800CA2F8();
    }
}

void func_800C9150(void)
{
}
