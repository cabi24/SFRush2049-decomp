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

