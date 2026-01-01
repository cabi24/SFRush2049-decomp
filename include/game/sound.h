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
s32 sndKillSound(u8 handle);

#endif /* SOUND_H */
