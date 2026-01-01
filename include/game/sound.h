/**
 * sound.h - Sound system interface for Rush 2049 N64
 *
 * Provides hooks for sound effects and music.
 * N64 uses the audio library (AL) for sound playback.
 */

#ifndef SOUND_H
#define SOUND_H

#include "types.h"

/* Sound effect IDs */
#define SFX_ENGINE_IDLE     0x00
#define SFX_ENGINE_REV      0x01
#define SFX_ENGINE_ACCEL    0x02
#define SFX_TIRE_SCREECH    0x10
#define SFX_TIRE_SKID       0x11
#define SFX_CRASH_LIGHT     0x20
#define SFX_CRASH_HEAVY     0x21
#define SFX_CRASH_WALL      0x22
#define SFX_CHECKPOINT      0x30
#define SFX_LAP_COMPLETE    0x31
#define SFX_RACE_START      0x32
#define SFX_RACE_FINISH     0x33
#define SFX_COUNTDOWN_3     0x40
#define SFX_COUNTDOWN_2     0x41
#define SFX_COUNTDOWN_1     0x42
#define SFX_COUNTDOWN_GO    0x43
#define SFX_WRONG_WAY       0x50
#define SFX_MENU_SELECT     0x60
#define SFX_MENU_MOVE       0x61
#define SFX_MENU_BACK       0x62
#define SFX_BOOST           0x70
#define SFX_WING_EXTEND     0x71
#define SFX_WING_DEPLOY     0x71    /* Alias for extend */
#define SFX_WING_RETRACT    0x72
#define SFX_WING_WHOOSH     0x73
#define SFX_WING_DAMAGE     0x74

/* Music track IDs */
#define MUS_MENU            0x00
#define MUS_RACE_1          0x01
#define MUS_RACE_2          0x02
#define MUS_RACE_3          0x03
#define MUS_RESULTS         0x10
#define MUS_GAMEOVER        0x11

/* Sound priorities */
#define SND_PRIORITY_LOW    0
#define SND_PRIORITY_NORMAL 1
#define SND_PRIORITY_HIGH   2

/* Sound channel state */
typedef struct SoundChannel {
    u16     sound_id;           /* Current sound playing */
    u8      priority;           /* Channel priority */
    u8      volume;             /* Volume (0-127) */
    s8      pan;                /* Pan (-64 to 63) */
    u8      playing;            /* Is channel active? */
    u8      looping;            /* Is sound looping? */
    u8      pad;
} SoundChannel;

/* Engine sound state (per car) */
typedef struct EngineSoundState {
    f32     rpm;                /* Current RPM */
    f32     pitch;              /* Current pitch modifier */
    u8      channel;            /* Audio channel assigned */
    u8      active;             /* Is engine sound active? */
    u8      pad[2];
} EngineSoundState;

/* External sound state */
extern SoundChannel sound_channels[];
extern EngineSoundState engine_sounds[];
extern u8 music_volume;
extern u8 sfx_volume;
extern u8 sound_enabled;

/* Initialization */
void sound_init(void);
void sound_reset(void);

/* Sound effect playback */
void sound_play(u16 sound_id);
void sound_play_vol(u16 sound_id, u8 volume);
void sound_play_3d(u16 sound_id, f32 pos[3]);
void sound_stop(u16 sound_id);
void sound_stop_all(void);

/* Music control */
void music_play(u16 track_id);
void music_stop(void);
void music_pause(void);
void music_resume(void);
void music_fade_out(u32 frames);
void music_set_volume(u8 volume);

/* Engine sounds */
void sound_update_engine(s32 car_index, f32 rpm, f32 throttle);
void sound_start_engine(s32 car_index);
void sound_stop_engine(s32 car_index);

/* Tire sounds */
void sound_tire_screech(s32 car_index, f32 slip);
void sound_tire_skid(s32 car_index, f32 slip);

/* Collision sounds */
void sound_collision(s32 car_index, f32 force);
void sound_wall_hit(s32 car_index, f32 force);

/* Per-frame update */
void sound_update(void);

/* Volume control */
void sound_set_master_volume(u8 volume);
void sound_set_sfx_volume(u8 volume);
void sound_set_music_volume(u8 volume);
void sound_enable(s32 enable);

/******* ARCADE-COMPATIBLE SOUND INTERFACE *******/

/* ========================================================================
 * Car Sound Types and Constants (carsnd.c)
 * ======================================================================== */

/* Maximum cars/links for sound tracking */
#define MAX_LINKS           12      /* Max cars for sound tracking */
#define MAX_DRONES          12      /* Synonym for MAX_LINKS */

/* Surface/visual codes for tire sounds */
#define PAVEMENT            0       /* On paved road */
#define DIRT                1       /* Off-road (gravel) */
#define WATER               2       /* In water */
#define AIR                 3       /* Airborne */

/* Wind noise parameters */
#define WIND_MAX_VOL        255
#define WIND_PITCH          1024

/* Road noise parameters */
#define ROAD_MIN_PITCH      200
#define ROAD_MAX_PITCH      1600
#define ROAD_MAX_VOL        200

/* Skid sound decay rate */
#define VOL_FADE_VAL        16

/* Scrape volume */
#define SCRAPE_VOL          165

/* Engine RPM scaling */
#define MAX_ENG_RPM_SCL     0x1700  /* Max RPM to prevent CAGE overload */

/* Timer macros (N64 adaptation) */
#ifndef ONE_SEC
#define ONE_SEC             60      /* Frames per second on N64 */
#endif
#ifndef IRQTIME
extern u32 frame_counter;
#define IRQTIME             frame_counter
#endif

/* Target/object types for collision sounds */
typedef enum {
    OBJ_TARGET_CONE = 0,
    OBJ_TARGET_GLASS,
    OBJ_TARGET_PMETER,
    OBJ_TARGET_BUSH,
    OBJ_TARGET_LIGHTPOLE,
    OBJ_TARGET_TREE,
    OBJ_TARGET_FENCE,
    OBJ_NUM_TARGETS
} TargetSoundType;

/* Sound state structure (per sound channel) */
typedef struct snd_state {
    s16     volume;         /* Current volume (0-255) */
    s16     pitch;          /* Current pitch */
    s16     on;             /* Sound is active */
    s16     state_var;      /* State variable (surface type, etc.) */
} snd_state;

/* Bump/collision state structure */
typedef struct bump_state {
    f32     peak;           /* Peak force magnitude */
    f32     peak_vec[3];    /* Peak force vector */
    u32     time;           /* Time of peak */
    s32     bump_it;        /* Flag: ready to make sound */
} bump_state;

/* Skid data table entry (per tire position) */
typedef struct skid_data {
    s16     force_tab[5];       /* Force thresholds for interpolation */
    s16     pitch_h;            /* High pitch */
    s16     pitch_l;            /* Low pitch */
    s16     vol_h;              /* High volume */
    s16     vol_l;              /* Low volume */
    s16     gr_skid_thresh;     /* Graphic skid threshold */
    s16     gr_smoke_thresh;    /* Graphic smoke threshold */
} skid_data;

/* Sound ID constants (arcade S_* defines) */
#define S_CONES             0x8050
#define S_GLASS             0x8051
#define S_PMETER            0x8052
#define S_BUSH              0x8053
#define S_LIGHTPOLE         0x8054
#define S_TREE              0x8055
#define S_FENCE             0x8056
#define S_SPLASH            0x8057
#define S_CARBUMP           0x8058
#define S_BOOM              0x8059
#define S_CARSMASH          0x805A
#define S_CARSCRAPE         0x805B
#define S_CURBWHUMP         0x805C
#define S_BOTTOMOUT         0x805D
#define S_REVERB_PARMS      0x805E
#define S_REVERB_RETURN_VOL 0x805F

/* Model appearance flags (tire smoke/skid marks) */
#define App_M_LF_SKID       0x0001
#define App_M_RF_SKID       0x0002
#define App_M_LR_SKID       0x0004
#define App_M_RR_SKID       0x0008
#define App_M_LF_SMOKE      0x0010
#define App_M_RF_SMOKE      0x0020
#define App_M_LR_SMOKE      0x0040
#define App_M_RR_SMOKE      0x0080
#define App_M_TIRE_SMOKE    0x00F0
#define App_M_SKID_MARK     0x000F

/* Sound flags for reverb (from arcade) */
#define LF_REVERB_MASK      0x0100
#define RT_REVERB_MASK      0x0200
#define ALL_REVERB_MASK     0x0300
#define LF_DIST_MASK        0x0C00
#define RT_DIST_MASK        0x3000
#define ALL_DIST_MASK       0x3C00
#define LF_DIST_SHIFT       10
#define RT_DIST_SHIFT       12

/* ========================================================================
 * Car Sound External State (carsnd.c)
 * ======================================================================== */

/* Per-car sound states */
extern snd_state rdnoise_state;
extern snd_state wind_state;
extern snd_state scrch_state[MAX_LINKS][4];
extern bump_state car_bump[5];
extern s16 scrape_state;
extern s16 skid_intensity[MAX_LINKS][4][2];
extern s16 smoke_intensity[MAX_LINKS][4][2];
extern s32 scrape_time;
extern s32 bump_time;
extern s32 in_tunnel;
extern s32 this_node;

/* ========================================================================
 * Car Sound Function Prototypes (carsnd.c)
 * ======================================================================== */

/* Sound state initialization */
void InitSndState(snd_state *sndst);
void InitCarSnds(s16 drone_index);
void init_bump_sounds(void);
void init_bump(s16 index);
void init_skids(s16 drone_index);

/* Car sound updates (per frame) */
void DoIntCarSounds(void);
void DoCarSounds(s16 update_car_sounds, s8 skids_only);
void do_bump_sounds(s16 update_car_sounds);

/* Engine sounds */
void StartEngineSound(void);
void DoEngineSound(void);
void StopEngineSound(void);
void sndStopEngine(void);

/* Tire squeals */
void DoTireSqueals(s16 drone_index);

/* Collision sounds */
void check_forces_on_car(void *m);
void get_force_and_peak(s16 index, f32 vec[3], f32 threshold);
void kill_scrape_sound(void);
void target_sound(void *t, s16 slot);
s32 target_inside_box(s16 slot, void *t);

/* Road surface sounds */
void sndGravelNoise(s16 pitch, s16 volume);
void sndRoadNoise(s16 pitch, s16 volume);
void sndSplashNoise(s16 pitch, s16 volume);
void sndWindNoise(s16 pitch, s16 volume);

/* Tire skid sounds */
void sndDoSkid(s16 tire_num, s16 pitch, s16 volume);

/* Reverb control */
void init_reverb(void);
void handle_reverb(void);

/* Radio control */
void StartRadio(u8 radio_station);
void StopRadio(u8 radio_station);
void music(s32 action, u8 station);
#ifndef Do_it
#define Do_it       1
#endif
#ifndef Undo_it
#define Undo_it     0
#endif

/* Arcade helper function (external, for coordinate transform) */
void sound_wparms(u16 cmd, s32 nargs, u16 *parms);

/* Arcade coordinate transforms (sounds.c) */
#define X_COORD_TRANS(x)          ((s16)(((x)*3)/10))     /* feet to meters */
#define Y_COORD_TRANS(y)          ((s16)(((y)*3)/10))     /* feet to meters */
#define LX_COORD_TRANS(x)         ((s32)(((x)*3)/10))     /* feet to meters (long) */
#define LY_COORD_TRANS(y)         ((s32)(((y)*3)/10))     /* feet to meters (long) */
#define ANGLE_COORD_TRANS(t)      (128 - (0x1ff & ((t) >> 7)))
#define MOOB_VELOCITY_XFORM(v)    ((s16)(((v)*447)/120))  /* mph to mm/frame */
#define ENGINE_VELOCITY_XFORM(v)  ((v)<<1)

/* Sound queue constants */
#define SND_Q_SIZE          4096
#define SND_Q_MASK          (SND_Q_SIZE-1)

/* Engine and radio counts */
#define NUM_ENGINES         4
#define MAX_RADIO           7

/* GUTS system commands */
#define S_STOP_ALL          0x8000
#define S_ATTRACT_MODE      0x8001
#define S_GAME_MODE         0x8002
#define S_SET_GAME_VOL      0x8006
#define S_SET_ATTR_VOL      0x8007
#define S_MOOB_LISTENER     0x8010
#define S_ENGINE_CHANGE_PF  0x8020
#define S_ENGINE_CHANGE_PFV 0x8021
#define S_ENGINE_CHANGE_AUTO 0x8022

/* Music commands */
#define S_SELECT            0x8046
#define S_KSELECT           0x8047
#define S_CARSELECT         0x804B
#define S_KCARSELECT        0x804C
#define S_WINNER            0x803C
#define S_KWINNER           0x803D
#define S_EXPLO             0x8040
#define S_KEXPLO            0x8041

/* Game sounds */
#define S_CHKPNTSTATIC      0x8084
#define S_THREE             0x8092
#define S_TWO               0x8093
#define S_ONE               0x8094
#define S_GO                0x8095
#define S_LEADERLIGHT       0x8088
#define S_KLEADERLIGHT      0x8089
#define S_CAR_LANDS         0x8083
#define S_COIN1             0x8090
#define S_COIN2             0x8091
#define S_TRANSELECT        0x808B
#define S_TRACKBROWSE       0x808A
#define S_TURNTABLE         0x808E
#define S_KTURNTABLE        0x808F
#define S_BOG               0x8086
#define S_KBOG              0x8087
#define S_SCRAPELOOP        0x807D
#define S_KSCRAPELOOP       0x807E

/* Music base IDs */
#define MUSIC_RACE_BASE     0x100
#define MUSIC_ATTRACT_BASE  0x200

/* Arcade-style sound function */
void SOUND(u16 cmd);
void SOUNDS(u16 cmd, s32 nargs, ...);

/* Attract mode control */
void set_attract_sound(s32 effects, s32 music);

/* Game event sounds */
void sound_checkpoint(void);
void sound_lap_complete(void);
void sound_race_start(s32 countdown);
void sound_first_place(void);
void sound_kill_first_place(void);
void sound_winner(void);
void sound_kill_winner(void);
void sound_explosion(f32 pos[3]);
void sound_car_lands(s32 car_index);
void sound_coin(s32 full);
void sound_menu_select(void);
void sound_menu_browse(void);

/* Music control */
void start_track_music(s32 track_num);
void stop_track_music(void);
void start_attract_music(s32 track_num);
void stop_attract_music(void);
void start_select_music(void);
void stop_select_music(void);
void start_car_select_music(void);
void stop_car_select_music(void);

/* ========================================================================
 * Arcade-compatible function aliases (sounds.c)
 * ======================================================================== */

/* Per-frame update */
void sndUpdate(void);

/* Listener (camera) position */
s32 sndListenerUpdate(s32 x, s32 y, u16 velocity, u16 vel_angle, u16 facing_angle);

/* Static sound (non-moving source) */
s32 sndStartStaticUnpitched(u16 objID, u8 handle, u8 priority, s16 x, s16 y);
s32 sndStartStaticPitched(u16 objID, u8 handle, u8 priority,
                          s32 x, s32 y, u16 pitch, u8 filter, u8 Q);

/* Doppler sound (moving source) */
s32 sndStartDopplerUnpitched(u16 objID, u8 handle, u8 priority,
                             s32 x, s32 y, u16 velocity, u16 vel_angle);
s32 sndStartDopplerPitched(u16 objID, u8 handle, u8 priority,
                           s32 x, s32 y, u16 velocity, u16 vel_angle,
                           u16 pitch, u8 filter, u8 Q);

/* Sound modification */
s32 sndPositionSound(u16 objId, u16 angle, u16 volume);
s32 sndChangePosition(u8 handle, s16 x, s16 y);
s32 sndChangeVelocity(u8 handle, u16 velocity, u16 vel_angle);
s32 sndChangePitch(u8 handle, u16 pitch);
s32 sndChangeFilter(u8 handle, u8 filter, u8 Q);

/* Engine sounds */
s32 sndStartEngine(u16 engineID, u16 pitch, u8 filter_frequency, u8 filter_Q);
s32 sndChangeEngine_PF(u16 pitch, u8 filter_frequency, u8 filter_Q);
s32 sndStartEngineWithVolume(u16 engineID, u16 pitch,
                             u8 filter_frequency, u8 filter_Q, u8 volume);
s32 sndChangeEngineWithVolume(u16 pitch, u8 filter_frequency, u8 filter_Q, u8 volume);
s32 sndUpdateAutoEngine(u16 speed);
s32 sndStartManualEngine(u16 engineID, u16 rpm, u16 etorque);
s32 sndUpdateManualEngine(u16 rpm, s16 etorque);

/* Kill sound */
s32 sndKillMoob(u16 handle);
s32 sndKillAllMoobs(void);

/* Engine crash sound */
s32 sndEngineCrash(u16 speed);

/* Test sound (for debugging) */
s32 doSndTest(u16 snd_id, u16 pitch, u16 volume);

/* MOOB (Motion Object) update for drone engines */
s32 sndUpdateMoobEngine(u16 objID, u8 handle, u8 priority,
                        s32 x, s32 y,
                        u16 velocity, u16 vel_angle, u16 rpm, u16 etorque);

/* ========================================================================
 * Additional Arcade Sound Constants (sounds.c/sounds.h)
 * ======================================================================== */

/* Maximum volume constant */
#define MAX_VOLUME          255

/* Engine sound IDs (from arcade own_engine[] table) */
#define S_AUDREY            0x8030      /* VW engine */
#define S_GHR               0x8031      /* Standard engine */
#define S_NSX               0x8032      /* NSX engine */
#define S_TPM               0x8033      /* TPM engine */
#define S_M_MULTIIDLE       0x8034      /* Multi-car idle */

/* Tire squeal sound IDs */
#define S_SQUEAL_LEFT       0x8070
#define S_SQUEAL_RIGHT      0x8071
#define S_SKID_CUSTOM1      0x8072
#define S_PEELOUT           0x8073

/* Road surface sound IDs */
#define S_ROADNOISE         0x8074
#define S_GRAVELNOISE       0x8075
#define S_WINDNOISE         0x8076
#define S_WATERROAD         0x8077

/* Engine control commands */
#define S_ENGINE_STOP       0x8023
#define S_ENGINE_CRASH      0x8024

/* MOOB control commands */
#define S_MOOB_KILL_OBJECT  0x8011
#define S_MOOB_KILL_ALL     0x8012
#define S_MOOB_CHANGE_POSITION  0x8013
#define S_MOOB_CHANGE_VELOCITY  0x8014
#define S_MOOB_CHANGE_PITCH     0x8015
#define S_MOOB_CHANGE_FILTER    0x8016

/* Stop all sounds command */
#define S_STOP_ALL_SNDS     0x8000

/* ========================================================================
 * Sound Handle Management (sounds.c)
 * ======================================================================== */

/* Car sound handles (for managing car sound channels) */
extern u8 car_sound_handle[9];
extern s16 sound_index;
extern s32 sounds_are_present;

/* Sound handle initialization */
void init_car_sound_handles(void);
u8 get_next_handle(void);

/* Wait for sound queue to empty */
void wait_for_sounds(void);

/* Test if sounds are present */
void test_sounds(void);

/* Sound queue element */
typedef struct sndEl {
    s16 d[16];
    u8  cnt;
} sndEl;

/* Sound queue structure */
typedef struct sndQueue {
    u32 head;
    u32 tail;
    sndEl q[SND_Q_SIZE];
} sndQueue;

/* ========================================================================
 * Target Object Sound Types (carsnd.c)
 * ======================================================================== */

/* Object types for collision sounds (enum values match arcade target_types) */
#define OBJ_CONE1           1
#define OBJ_METER1          2
#define OBJ_TREEA1          3
#define OBJ_TREEB1          4
#define OBJ_TREEC1          5
#define OBJ_TREED1          6
#define OBJ_TREEE1          7
#define OBJ_WINDOWA1        8
#define OBJ_WINDOWB1        9
#define OBJ_FENCE1          10
#define OBJ_POLE1           11
#define OBJ_TLIGHT1         12
#define OBJ_SLIGHT1         13

/* External flags (from game state) */
extern s32 demo_game;
extern s32 coast_flag;

#endif /* SOUND_H */
