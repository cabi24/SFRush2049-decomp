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

#endif /* SOUND_H */
