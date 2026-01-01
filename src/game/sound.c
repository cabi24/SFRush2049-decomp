/**
 * sound.c - Sound system for Rush 2049 N64
 *
 * Provides interface to N64 audio library for sound effects and music.
 * Engine sounds are handled specially with RPM-based pitch modulation.
 */

#include "types.h"
#include "game/sound.h"
#include "game/structs.h"

/* External game state */
extern u8 gstate;
extern u32 frame_counter;
extern CarData car_array[];
extern s32 num_active_cars;

/* External N64 audio functions */
extern void alSeqpPlay(void *seqp);
extern void alSeqpStop(void *seqp);
extern void alSndpPlay(s32 channel);
extern void alSndpStop(s32 channel);
extern void alSndpSetPitch(s32 channel, f32 pitch);
extern void alSndpSetVol(s32 channel, s16 vol);
extern void alSndpSetPan(s32 channel, u8 pan);

/* Sound channel count */
#define NUM_SFX_CHANNELS    16
#define NUM_ENGINE_CHANNELS 8

/* Sound state */
SoundChannel sound_channels[NUM_SFX_CHANNELS];
EngineSoundState engine_sounds[NUM_ENGINE_CHANNELS];
u8 music_volume = 100;
u8 sfx_volume = 100;
u8 sound_enabled = 1;

/* Current music state */
static u16 current_music = 0xFFFF;
static u8 music_playing = 0;
static u32 fade_frames = 0;
static u8 fade_target_vol = 0;

/**
 * sound_init - Initialize sound system
 */
void sound_init(void) {
    s32 i;

    /* Initialize sound channels */
    for (i = 0; i < NUM_SFX_CHANNELS; i++) {
        sound_channels[i].sound_id = 0;
        sound_channels[i].priority = 0;
        sound_channels[i].volume = 127;
        sound_channels[i].pan = 0;
        sound_channels[i].playing = 0;
        sound_channels[i].looping = 0;
    }

    /* Initialize engine sounds */
    for (i = 0; i < NUM_ENGINE_CHANNELS; i++) {
        engine_sounds[i].rpm = 0.0f;
        engine_sounds[i].pitch = 1.0f;
        engine_sounds[i].channel = i;
        engine_sounds[i].active = 0;
    }

    sound_enabled = 1;
    music_volume = 100;
    sfx_volume = 100;
    current_music = 0xFFFF;
    music_playing = 0;
}

/**
 * sound_reset - Reset sound system
 */
void sound_reset(void) {
    sound_stop_all();
    music_stop();
}

/**
 * sound_play - Play a sound effect
 *
 * @param sound_id Sound effect ID
 */
void sound_play(u16 sound_id) {
    sound_play_vol(sound_id, 127);
}

/**
 * sound_play_vol - Play a sound effect with volume
 *
 * @param sound_id Sound effect ID
 * @param volume Volume (0-127)
 */
void sound_play_vol(u16 sound_id, u8 volume) {
    s32 i;
    s32 best_channel = -1;
    u8 lowest_priority = 255;

    if (!sound_enabled) {
        return;
    }

    /* Scale volume by master SFX volume */
    volume = (u8)((volume * sfx_volume) / 100);

    /* Find free channel or lowest priority channel */
    for (i = 0; i < NUM_SFX_CHANNELS; i++) {
        if (!sound_channels[i].playing) {
            best_channel = i;
            break;
        }
        if (sound_channels[i].priority < lowest_priority) {
            lowest_priority = sound_channels[i].priority;
            best_channel = i;
        }
    }

    if (best_channel < 0) {
        return;  /* No channel available */
    }

    /* Set up channel */
    sound_channels[best_channel].sound_id = sound_id;
    sound_channels[best_channel].volume = volume;
    sound_channels[best_channel].priority = SND_PRIORITY_NORMAL;
    sound_channels[best_channel].playing = 1;
    sound_channels[best_channel].looping = 0;

    /* Play sound via N64 audio */
    /* alSndpPlay(best_channel); */
    /* alSndpSetVol(best_channel, volume); */
}

/**
 * sound_play_3d - Play positional sound effect
 *
 * @param sound_id Sound effect ID
 * @param pos World position of sound source
 */
void sound_play_3d(u16 sound_id, f32 pos[3]) {
    /* Calculate volume and pan based on distance to camera */
    /* For now, just play at normal volume */
    sound_play(sound_id);
}

/**
 * sound_stop - Stop a specific sound
 *
 * @param sound_id Sound effect ID to stop
 */
void sound_stop(u16 sound_id) {
    s32 i;

    for (i = 0; i < NUM_SFX_CHANNELS; i++) {
        if (sound_channels[i].sound_id == sound_id && sound_channels[i].playing) {
            sound_channels[i].playing = 0;
            /* alSndpStop(i); */
        }
    }
}

/**
 * sound_stop_all - Stop all sound effects
 */
void sound_stop_all(void) {
    s32 i;

    for (i = 0; i < NUM_SFX_CHANNELS; i++) {
        if (sound_channels[i].playing) {
            sound_channels[i].playing = 0;
            /* alSndpStop(i); */
        }
    }

    /* Stop engine sounds */
    for (i = 0; i < NUM_ENGINE_CHANNELS; i++) {
        if (engine_sounds[i].active) {
            engine_sounds[i].active = 0;
        }
    }
}

/**
 * music_play - Start playing a music track
 *
 * @param track_id Music track ID
 */
void music_play(u16 track_id) {
    if (!sound_enabled) {
        return;
    }

    if (current_music == track_id && music_playing) {
        return;  /* Already playing */
    }

    /* Stop current music */
    if (music_playing) {
        music_stop();
    }

    current_music = track_id;
    music_playing = 1;

    /* Start N64 sequence player */
    /* alSeqpPlay(sequence_ptr); */
}

/**
 * music_stop - Stop current music
 */
void music_stop(void) {
    if (music_playing) {
        /* alSeqpStop(sequence_ptr); */
        music_playing = 0;
    }
    fade_frames = 0;
}

/**
 * music_pause - Pause current music
 */
void music_pause(void) {
    if (music_playing) {
        /* Pause sequence player */
    }
}

/**
 * music_resume - Resume paused music
 */
void music_resume(void) {
    if (music_playing) {
        /* Resume sequence player */
    }
}

/**
 * music_fade_out - Fade out music over time
 *
 * @param frames Number of frames to fade
 */
void music_fade_out(u32 frames) {
    fade_frames = frames;
    fade_target_vol = 0;
}

/**
 * music_set_volume - Set music volume
 *
 * @param volume Volume (0-127)
 */
void music_set_volume(u8 volume) {
    music_volume = volume;
    /* Apply to sequence player */
}

/**
 * sound_update_engine - Update engine sound for a car
 *
 * Engine sound pitch is modulated based on RPM.
 *
 * @param car_index Car index
 * @param rpm Current engine RPM
 * @param throttle Throttle position (0-1)
 */
void sound_update_engine(s32 car_index, f32 rpm, f32 throttle) {
    EngineSoundState *eng;
    f32 pitch;
    f32 volume;

    if (car_index < 0 || car_index >= NUM_ENGINE_CHANNELS) {
        return;
    }

    eng = &engine_sounds[car_index];

    if (!eng->active) {
        return;
    }

    /* Calculate pitch from RPM (normalized to 1.0 at idle RPM) */
    /* Typical idle ~1000 RPM, redline ~8000 RPM */
    pitch = 0.5f + (rpm / 8000.0f) * 1.5f;

    /* Clamp pitch */
    if (pitch < 0.5f) pitch = 0.5f;
    if (pitch > 2.0f) pitch = 2.0f;

    eng->rpm = rpm;
    eng->pitch = pitch;

    /* Calculate volume based on throttle */
    volume = 80.0f + throttle * 47.0f;  /* 80-127 range */

    /* Apply to N64 audio */
    /* alSndpSetPitch(eng->channel, pitch); */
    /* alSndpSetVol(eng->channel, (s16)volume); */
}

/**
 * sound_start_engine - Start engine sound for a car
 *
 * @param car_index Car index
 */
void sound_start_engine(s32 car_index) {
    if (car_index < 0 || car_index >= NUM_ENGINE_CHANNELS) {
        return;
    }

    if (!engine_sounds[car_index].active) {
        engine_sounds[car_index].active = 1;
        engine_sounds[car_index].rpm = 1000.0f;
        engine_sounds[car_index].pitch = 1.0f;

        /* Start looping engine sound */
        /* alSndpPlay(engine_sounds[car_index].channel); */
    }
}

/**
 * sound_stop_engine - Stop engine sound for a car
 *
 * @param car_index Car index
 */
void sound_stop_engine(s32 car_index) {
    if (car_index < 0 || car_index >= NUM_ENGINE_CHANNELS) {
        return;
    }

    if (engine_sounds[car_index].active) {
        engine_sounds[car_index].active = 0;
        /* alSndpStop(engine_sounds[car_index].channel); */
    }
}

/**
 * sound_tire_screech - Play tire screech sound
 *
 * @param car_index Car index
 * @param slip Tire slip amount (0-1)
 */
void sound_tire_screech(s32 car_index, f32 slip) {
    u8 volume;

    /* Scale volume by slip amount */
    volume = (u8)(slip * 127.0f);
    if (volume < 30) {
        return;  /* Too quiet */
    }

    sound_play_vol(SFX_TIRE_SCREECH, volume);
}

/**
 * sound_tire_skid - Play tire skid sound (on dirt/grass)
 *
 * @param car_index Car index
 * @param slip Tire slip amount
 */
void sound_tire_skid(s32 car_index, f32 slip) {
    u8 volume;

    volume = (u8)(slip * 100.0f);
    if (volume < 20) {
        return;
    }

    sound_play_vol(SFX_TIRE_SKID, volume);
}

/**
 * sound_collision - Play collision sound based on impact force
 *
 * @param car_index Car that collided
 * @param force Collision force magnitude
 */
void sound_collision(s32 car_index, f32 force) {
    if (force > 1000.0f) {
        sound_play_vol(SFX_CRASH_HEAVY, 127);
    } else if (force > 500.0f) {
        sound_play_vol(SFX_CRASH_LIGHT, 100);
    } else if (force > 100.0f) {
        sound_play_vol(SFX_CRASH_LIGHT, 60);
    }
}

/**
 * sound_wall_hit - Play wall collision sound
 *
 * @param car_index Car that hit wall
 * @param force Impact force
 */
void sound_wall_hit(s32 car_index, f32 force) {
    u8 volume;

    volume = (u8)(force / 20.0f);
    if (volume > 127) volume = 127;
    if (volume < 30) return;

    sound_play_vol(SFX_CRASH_WALL, volume);
}

/**
 * sound_update - Per-frame sound update
 */
void sound_update(void) {
    s32 i;

    /* Handle music fade */
    if (fade_frames > 0) {
        fade_frames--;
        if (fade_frames == 0) {
            if (fade_target_vol == 0) {
                music_stop();
            }
        } else {
            /* Interpolate volume */
            /* ... */
        }
    }

    /* Update engine sounds based on car data */
    for (i = 0; i < num_active_cars && i < NUM_ENGINE_CHANNELS; i++) {
        if (engine_sounds[i].active) {
            CarData *car = &car_array[i];
            /* Engine RPM would come from car physics */
            /* For now, estimate from speed */
            f32 rpm = 1000.0f + car->mph * 30.0f;
            sound_update_engine(i, rpm, 1.0f);
        }
    }
}

/**
 * sound_set_master_volume - Set overall volume
 *
 * @param volume Volume (0-100)
 */
void sound_set_master_volume(u8 volume) {
    if (volume > 100) volume = 100;
    sfx_volume = volume;
    music_volume = volume;
}

/**
 * sound_set_sfx_volume - Set sound effects volume
 *
 * @param volume Volume (0-100)
 */
void sound_set_sfx_volume(u8 volume) {
    if (volume > 100) volume = 100;
    sfx_volume = volume;
}

/**
 * sound_set_music_volume - Set music volume
 *
 * @param volume Volume (0-100)
 */
void sound_set_music_volume(u8 volume) {
    if (volume > 100) volume = 100;
    music_volume = volume;
}

/**
 * sound_enable - Enable or disable all sound
 *
 * @param enable Non-zero to enable
 */
void sound_enable(s32 enable) {
    sound_enabled = enable ? 1 : 0;

    if (!sound_enabled) {
        sound_stop_all();
        music_stop();
    }
}

/******* ARCADE-COMPATIBLE SOUND INTERFACE *******/

/* Attract mode sound flag */
static u8 attract_mode_sound = 0;
static u8 attract_effects = 1;
static u8 attract_music = 1;

/**
 * SOUND - Arcade-style sound command
 * Based on arcade: sounds.c SOUND macro
 *
 * This wraps the arcade's SOUND() macro for compatibility.
 * Commands >= 0x8000 are GUTS commands, others are sound IDs.
 *
 * @param cmd Sound command or ID
 */
void SOUND(u16 cmd) {
    /* Check for attract mode restrictions */
    if (attract_mode_sound) {
        if (cmd < 0x8000 && !attract_effects) {
            return;  /* Effects disabled in attract */
        }
        /* Music commands handled separately */
    }

    /* GUTS system commands (0x8000+) */
    if (cmd >= 0x8000) {
        switch (cmd) {
            case S_STOP_ALL:
                sound_stop_all();
                break;

            case S_ATTRACT_MODE:
                attract_mode_sound = 1;
                break;

            case S_GAME_MODE:
                attract_mode_sound = 0;
                break;

            /* Kill sound commands */
            case S_KSELECT:
            case S_KCARSELECT:
            case S_KTURNTABLE:
            case S_KBOG:
            case S_KLEADERLIGHT:
            case S_KSCRAPELOOP:
            case S_KWINNER:
            case S_KEXPLO:
                /* Stop the corresponding sound */
                sound_stop(cmd - 1);  /* Kill cmd is always sound cmd + 1 */
                break;

            default:
                /* Assume it's a regular sound command */
                sound_play(cmd);
                break;
        }
    } else {
        /* Regular sound ID */
        sound_play(cmd);
    }
}

/**
 * SOUNDS - Send sound with arguments
 * Based on arcade: sounds.c multi-argument sound call
 *
 * @param cmd Sound command
 * @param nargs Number of arguments
 * @param ... Variable arguments
 */
void SOUNDS(u16 cmd, s32 nargs, ...) {
    /* For N64, we simplify this to just play the sound */
    /* On arcade, this would send multiple arguments to sound system */
    SOUND(cmd);
}

/**
 * set_attract_sound - Set attract mode sound flags
 * Based on arcade: sounds.c attract sound control
 *
 * @param effects Enable effects in attract
 * @param music Enable music in attract
 */
void set_attract_sound(s32 effects, s32 music) {
    attract_effects = effects ? 1 : 0;
    attract_music = music ? 1 : 0;
}

/**
 * sound_checkpoint - Play checkpoint sound
 */
void sound_checkpoint(void) {
    SOUND(S_CHKPNTSTATIC);
}

/**
 * sound_lap_complete - Play lap completion sound
 */
void sound_lap_complete(void) {
    /* Play lap complete announcement */
    sound_play(SFX_LAP_COMPLETE);
}

/**
 * sound_race_start - Play race start sounds
 *
 * @param countdown 3, 2, 1, or 0 for GO
 */
void sound_race_start(s32 countdown) {
    switch (countdown) {
        case 3:
            SOUND(S_THREE);
            break;
        case 2:
            SOUND(S_TWO);
            break;
        case 1:
            SOUND(S_ONE);
            break;
        case 0:
            SOUND(S_GO);
            break;
    }
}

/**
 * sound_first_place - Play first place sound effect
 */
void sound_first_place(void) {
    SOUND(S_LEADERLIGHT);
}

/**
 * sound_kill_first_place - Stop first place sound
 */
void sound_kill_first_place(void) {
    SOUND(S_KLEADERLIGHT);
}

/**
 * sound_winner - Play winner music
 */
void sound_winner(void) {
    SOUND(S_WINNER);
}

/**
 * sound_kill_winner - Stop winner music
 */
void sound_kill_winner(void) {
    SOUND(S_KWINNER);
}

/**
 * sound_explosion - Play explosion sound at position
 *
 * @param pos World position
 */
void sound_explosion(f32 pos[3]) {
    sound_play_3d(S_EXPLO, pos);
}

/**
 * sound_car_lands - Play landing sound after jump
 *
 * @param car_index Car that landed
 */
void sound_car_lands(s32 car_index) {
    SOUND(S_CAR_LANDS);
}

/**
 * sound_coin - Play coin insert sound
 *
 * @param full Non-zero if full credit
 */
void sound_coin(s32 full) {
    SOUND(full ? S_COIN2 : S_COIN1);
}

/**
 * sound_menu_select - Play menu selection sound
 */
void sound_menu_select(void) {
    SOUND(S_TRANSELECT);
}

/**
 * sound_menu_browse - Play menu browse sound
 */
void sound_menu_browse(void) {
    SOUND(S_TRACKBROWSE);
}

/**
 * start_track_music - Start race music for track
 *
 * @param track_num Track number
 */
void start_track_music(s32 track_num) {
    /* Each track has its own music */
    /* For now, just start generic race music */
    music_play(MUSIC_RACE_BASE + track_num);
}

/**
 * stop_track_music - Stop race music
 */
void stop_track_music(void) {
    music_stop();
}

/**
 * start_attract_music - Start attract mode music
 *
 * @param track_num Attract music track
 */
void start_attract_music(s32 track_num) {
    if (attract_music) {
        music_play(MUSIC_ATTRACT_BASE + track_num);
    }
}

/**
 * stop_attract_music - Stop attract mode music
 */
void stop_attract_music(void) {
    music_stop();
}

/**
 * start_select_music - Start selection screen music
 */
void start_select_music(void) {
    SOUND(S_SELECT);
}

/**
 * stop_select_music - Stop selection screen music
 */
void stop_select_music(void) {
    SOUND(S_KSELECT);
}

/**
 * start_car_select_music - Start car selection music
 */
void start_car_select_music(void) {
    SOUND(S_CARSELECT);
}

/**
 * stop_car_select_music - Stop car selection music
 */
void stop_car_select_music(void) {
    SOUND(S_KCARSELECT);
}

/* ========================================================================
 * Arcade-compatible function implementations (sounds.c)
 * ======================================================================== */

/**
 * sndUpdate - Per-frame sound queue update
 * Arcade: sounds.c:sndUpdate()
 */
void sndUpdate(void) {
    sound_update();
}

/**
 * sndListenerUpdate - Update listener (camera) position
 * Arcade: sounds.c:sndListenerUpdate()
 */
s32 sndListenerUpdate(s32 x, s32 y, u16 velocity, u16 vel_angle, u16 facing_angle) {
    /* On N64, 3D audio positioning is handled differently */
    /* For now, just acknowledge the update */
    return 0;
}

/**
 * sndStartStaticUnpitched - Start static unpitched sound
 * Arcade: sounds.c:sndStartStaticUnpitched()
 */
s32 sndStartStaticUnpitched(u16 objID, u8 handle, u8 priority, s16 x, s16 y) {
    f32 pos[3];
    pos[0] = (f32)x;
    pos[1] = (f32)y;
    pos[2] = 0.0f;
    sound_play_3d(objID, pos);
    return 0;
}

/**
 * sndStartStaticPitched - Start static pitched sound
 * Arcade: sounds.c:sndStartStaticPitched()
 */
s32 sndStartStaticPitched(u16 objID, u8 handle, u8 priority,
                          s32 x, s32 y, u16 pitch, u8 filter, u8 Q) {
    f32 pos[3];
    pos[0] = (f32)x;
    pos[1] = (f32)y;
    pos[2] = 0.0f;
    sound_play_3d(objID, pos);
    /* Pitch and filter would be applied via N64 audio */
    return 0;
}

/**
 * sndStartDopplerUnpitched - Start doppler unpitched sound
 * Arcade: sounds.c:sndStartDopplerUnpitched()
 */
s32 sndStartDopplerUnpitched(u16 objID, u8 handle, u8 priority,
                             s32 x, s32 y, u16 velocity, u16 vel_angle) {
    f32 pos[3];
    pos[0] = (f32)x;
    pos[1] = (f32)y;
    pos[2] = 0.0f;
    sound_play_3d(objID, pos);
    return 0;
}

/**
 * sndStartDopplerPitched - Start doppler pitched sound
 * Arcade: sounds.c:sndStartDopplerPitched()
 */
s32 sndStartDopplerPitched(u16 objID, u8 handle, u8 priority,
                           s32 x, s32 y, u16 velocity, u16 vel_angle,
                           u16 pitch, u8 filter, u8 Q) {
    f32 pos[3];
    pos[0] = (f32)x;
    pos[1] = (f32)y;
    pos[2] = 0.0f;
    sound_play_3d(objID, pos);
    return 0;
}

/**
 * sndPositionSound - Position a sound in space
 * Arcade: sounds.c:sndPositionSound()
 */
s32 sndPositionSound(u16 objId, u16 angle, u16 volume) {
    sound_play_vol(objId, (u8)(volume >> 1));
    return 0;
}

/**
 * sndChangePosition - Change sound position
 * Arcade: sounds.c:sndChangePosition()
 */
s32 sndChangePosition(u8 handle, s16 x, s16 y) {
    /* Position changes not directly supported on N64 */
    return 0;
}

/**
 * sndChangeVelocity - Change sound velocity (for doppler)
 * Arcade: sounds.c:sndChangeVelocity()
 */
s32 sndChangeVelocity(u8 handle, u16 velocity, u16 vel_angle) {
    /* Velocity changes not directly supported on N64 */
    return 0;
}

/**
 * sndChangePitch - Change sound pitch
 * Arcade: sounds.c:sndChangePitch()
 */
s32 sndChangePitch(u8 handle, u16 pitch) {
    /* Would apply pitch via alSndpSetPitch */
    return 0;
}

/**
 * sndChangeFilter - Change sound filter
 * Arcade: sounds.c:sndChangeFilter()
 */
s32 sndChangeFilter(u8 handle, u8 filter, u8 Q) {
    /* Filters not directly supported on N64 */
    return 0;
}

/**
 * sndStartEngine - Start engine sound
 * Arcade: sounds.c:sndStartEngine()
 */
s32 sndStartEngine(u16 engineID, u16 pitch, u8 filter_frequency, u8 filter_Q) {
    sound_start_engine(0);  /* Start engine for car 0 */
    return 0;
}

/**
 * sndChangeEngine_PF - Change engine pitch and filter
 * Arcade: sounds.c:sndChangeEngine_PF()
 */
s32 sndChangeEngine_PF(u16 pitch, u8 filter_frequency, u8 filter_Q) {
    /* Engine pitch change via N64 audio */
    return 0;
}

/**
 * sndStartEngineWithVolume - Start engine with volume
 * Arcade: sounds.c:sndStartEngineWithVolume()
 */
s32 sndStartEngineWithVolume(u16 engineID, u16 pitch,
                             u8 filter_frequency, u8 filter_Q, u8 volume) {
    sound_start_engine(0);
    return 0;
}

/**
 * sndChangeEngineWithVolume - Change engine with volume
 * Arcade: sounds.c:sndChangeEngineWithVolume()
 */
s32 sndChangeEngineWithVolume(u16 pitch, u8 filter_frequency, u8 filter_Q, u8 volume) {
    return 0;
}

/**
 * sndUpdateAutoEngine - Update automatic engine model
 * Arcade: sounds.c:sndUpdateAutoEngine()
 */
s32 sndUpdateAutoEngine(u16 speed) {
    /* Convert speed to RPM-like value */
    f32 rpm = (f32)speed * 50.0f + 1000.0f;
    sound_update_engine(0, rpm, 1.0f);
    return 0;
}

/**
 * sndStartManualEngine - Start manual engine model
 * Arcade: sounds.c:sndStartManualEngine()
 */
s32 sndStartManualEngine(u16 engineID, u16 rpm, u16 etorque) {
    sound_start_engine(0);
    sound_update_engine(0, (f32)rpm, (f32)etorque / 32768.0f);
    return 0;
}

/**
 * sndUpdateManualEngine - Update manual engine model
 * Arcade: sounds.c:sndUpdateManualEngine()
 */
s32 sndUpdateManualEngine(u16 rpm, s16 etorque) {
    f32 throttle;
    throttle = (etorque > 0) ? ((f32)etorque / 32768.0f) : 0.0f;
    sound_update_engine(0, (f32)rpm, throttle);
    return 0;
}

/**
 * sndKillSound - Kill a sound by handle
 * Arcade: sounds.c kill sound functionality
 */
s32 sndKillSound(u8 handle) {
    /* On N64, we don't track handles the same way */
    return 0;
}

/******* ARCADE-COMPATIBLE CAR SOUND FUNCTIONS (carsnd.c) *******/

/* ========================================================================
 * Car Sound State Variables
 * ======================================================================== */

/* Road/wind noise state */
snd_state rdnoise_state;
snd_state wind_state;

/* Per-car screech state (4 tire positions per car) */
snd_state scrch_state[MAX_LINKS][4];

/* Collision bump state (4 corners + 1 for car-to-car) */
bump_state car_bump[5];

/* Scrape state and timing */
s16 scrape_state;
s32 scrape_time;
s32 bump_time;

/* Skid and smoke intensities [car][tire][left/right pair] */
s16 skid_intensity[MAX_LINKS][4][2];
s16 smoke_intensity[MAX_LINKS][4][2];

/* Tunnel/reverb state */
s32 in_tunnel;

/* Current player's car index (set by game code) */
s32 this_node;

/* Engine sound state */
static f32 rpm_fix;
static f32 torque_fix;

/* Demo/attract flags (external) */
extern s32 demo_game;
extern s16 which_engine;
extern s32 coast_flag;

/* Velocity squared (from physics, for wind calculations) */
static s32 velsq;

/* ========================================================================
 * Tire Smoke/Skid Constants (from arcade carsnd.c)
 * ======================================================================== */

/* Tire smoke appearance flags by tire index */
static const u32 tire_smoke_tab[4] = {
    App_M_LR_SMOKE,
    App_M_RR_SMOKE,
    App_M_LF_SMOKE | App_M_RF_SMOKE,
    App_M_LR_SMOKE | App_M_RR_SMOKE
};

/* Tire skid appearance flags by tire index */
static const u32 tire_skid_tab[4] = {
    App_M_LF_SKID | App_M_LR_SKID,
    App_M_RF_SKID | App_M_RR_SKID,
    App_M_LF_SKID | App_M_RF_SKID,
    App_M_LR_SKID | App_M_RR_SKID
};

/* Tire pair index lookup [tire_position][0=primary, 1=secondary] */
static const u32 tire_tab[4][2] = {
    {1, 3}, {2, 0}, {0, 1}, {3, 2}
};

/* Target sound IDs by object type */
static const s16 target_snd[7] = {
    S_CONES, S_GLASS, S_PMETER, S_BUSH,
    S_LIGHTPOLE, S_TREE, S_FENCE
};

/* Target volume by object type */
static const s16 target_volume[6] = {
    255, 255, 255, 255, 255, 230
};

/* Bump position table - maps corner index to angle (0-512 = 0-360 degrees) */
static const s16 thump_tab[16] = {
    0, 64, 448, 0, 192, 128, 0, 64, 320, 0, 384, 448, 256, 192, 320, 0
};

/* Bump direction table - maps force direction to angle */
static const s16 bump_tab[16] = {
    0, 0, 256, 0, 128, 64, 192, 128, 384, 448, 320, 384, 0, 0, 256, 0
};

/* Skid data per tire position (left, right, front, rear) */
static const skid_data skid_tab[4] = {
    /* Left side */
    {
        {1500, 1700, 1900, 2200, 2600},
        1240, 800,  /* pitch_h, pitch_l */
        255, 1,     /* vol_h, vol_l */
        160, 250    /* skid thresh, smoke thresh */
    },
    /* Right side */
    {
        {1500, 1700, 1900, 2200, 2600},
        1240, 800,
        255, 1,
        160, 250
    },
    /* Front tires */
    {
        {800, 975, 1250, 1325, 1500},
        1340, 800,
        255, 1,
        128, 250
    },
    /* Rear tires */
    {
        {2400, 2225, 2050, 1875, 1700},
        1140, 800,
        255, 1,
        120, 190
    }
};

/* ========================================================================
 * Helper Macros
 * ======================================================================== */

#define get_max(a, b)       ((a) > (b) ? (a) : (b))
#define range(a, b, c)      ((a) < (b) ? (b) : ((a) > (c) ? (c) : (a)))
#define sign(a)             ((a) < 0 ? -1 : 1)

/* Absolute value for float */
#ifndef fabs
static f32 fabs_local(f32 x) {
    return x < 0.0f ? -x : x;
}
#define fabs(x) fabs_local(x)
#endif

/* Absolute value for int */
#ifndef abs
#define abs(x) ((x) < 0 ? -(x) : (x))
#endif

/* Square root stub (would use libm in full build) */
#ifndef fsqrt
static f32 fsqrt_local(f32 x) {
    /* Newton-Raphson approximation */
    f32 guess;
    s32 i;
    if (x <= 0.0f) return 0.0f;
    guess = x / 2.0f;
    for (i = 0; i < 10; i++) {
        guess = (guess + x / guess) / 2.0f;
    }
    return guess;
}
#define fsqrt(x) fsqrt_local(x)
#endif

/* ========================================================================
 * Car Sound Initialization Functions
 * ======================================================================== */

/**
 * InitSndState - Initialize a sound state structure
 * Arcade: carsnd.c:InitSndState()
 */
void InitSndState(snd_state *sndst) {
    sndst->volume = 0;
    sndst->pitch = 0;
    sndst->on = 0;
    sndst->state_var = 0;
}

/**
 * InitCarSnds - Initialize car sound state for a car
 * Arcade: carsnd.c:InitCarSnds()
 */
void InitCarSnds(s16 drone_index) {
    InitSndState(&rdnoise_state);

    if (drone_index == this_node) {
        sndGravelNoise(0, 0);
        sndRoadNoise(0, 0);
        sndSplashNoise(0, 0);
    }

    InitSndState(&wind_state);

    if (drone_index == this_node) {
        sndWindNoise(0, 0);
    }

    init_skids(drone_index);
}

/**
 * init_bump_sounds - Initialize car bump/collision sounds
 * Arcade: carsnd.c:init_bump_sounds()
 */
void init_bump_sounds(void) {
    s16 i;

    for (i = 0; i < 5; i++) {
        init_bump(i);
    }

    kill_scrape_sound();
    bump_time = 0;
}

/**
 * init_bump - Initialize a single bump state slot
 * Arcade: carsnd.c:init_bump()
 */
void init_bump(s16 index) {
    car_bump[index].peak = 0.0f;
    car_bump[index].time = 0;
    car_bump[index].bump_it = 0;
}

/**
 * init_skids - Stop all skid sounds for a car
 * Arcade: carsnd.c:init_skids()
 */
void init_skids(s16 drone_index) {
    s16 i;

    for (i = 0; i < 4; i++) {
        if (scrch_state[drone_index][i].volume != 0 ||
            scrch_state[drone_index][i].pitch != 0 ||
            scrch_state[drone_index][i].on != 0 ||
            scrch_state[drone_index][i].state_var != 0) {
            InitSndState(&scrch_state[drone_index][i]);
        }

        skid_intensity[drone_index][tire_tab[i][0]][0] = 0;
        skid_intensity[drone_index][tire_tab[i][1]][1] = 0;
        smoke_intensity[drone_index][tire_tab[i][0]][0] = 0;
        smoke_intensity[drone_index][tire_tab[i][1]][1] = 0;

        if (drone_index == this_node) {
            sndDoSkid(i, 0, 0);
        }
    }
}

/* ========================================================================
 * Engine Sound Functions
 * ======================================================================== */

/**
 * StartEngineSound - Start engine sound for player's car
 * Arcade: carsnd.c:StartEngineSound()
 */
void StartEngineSound(void) {
    if (demo_game == 0) {
        sndStartManualEngine(which_engine, 250, 100);
    }

    rpm_fix = 0.8f;
    torque_fix = 0.3f;
}

/**
 * DoEngineSound - Update engine sound
 * Arcade: carsnd.c:DoEngineSound()
 *
 * Called every model iteration to update engine RPM and torque sounds.
 */
void DoEngineSound(void) {
    u16 clip_rpm;
    s16 clip_torque;
    s32 rpm_val;
    s32 torque_val;
    f32 engine_torque;

    /* Get current RPM (would come from model) */
    rpm_val = 2000;  /* Placeholder - arcade uses abs(rpm) */

    if (rpm_val > 9000) {
        clip_rpm = 9000;
    } else {
        clip_rpm = (u16)rpm_val;
    }

    /* Get engine torque (would come from model[this_node].engtorque) */
    engine_torque = 1000.0f;  /* Placeholder */

    if (engine_torque > 0.0f) {
        torque_val = (s32)engine_torque;
    } else {
        torque_val = (s32)engine_torque;
    }

    if (torque_val > 0x7FFF) {
        clip_torque = 0x7FFF;
    } else if (torque_val < -0x7FFF) {
        clip_torque = -0x7FFF;
    } else {
        clip_torque = (s16)torque_val;
    }

    if (demo_game == 0) {
        if (coast_flag) {
            sndUpdateManualEngine(0, 0);
        } else {
            sndUpdateManualEngine(clip_rpm, clip_torque);
        }
    }
}

/**
 * DoIntCarSounds - High-priority car sounds (every model frame)
 * Arcade: carsnd.c:DoIntCarSounds()
 */
void DoIntCarSounds(void) {
    DoEngineSound();
}

/**
 * StopEngineSound - Stop engine sound
 * Arcade: carsnd.c:StopEngineSound()
 */
void StopEngineSound(void) {
    sndStopEngine();
}

/**
 * sndStopEngine - Stop engine sound (low-level)
 */
void sndStopEngine(void) {
    sound_stop_engine(0);
}

/* ========================================================================
 * Road Surface Sound Functions
 * ======================================================================== */

/**
 * sndGravelNoise - Play gravel/dirt road noise
 * Arcade: sounds.c:sndGravelNoise()
 */
void sndGravelNoise(s16 pitch, s16 volume) {
    /* N64 implementation - would trigger dirt surface sound */
    if (volume > 0) {
        sound_play_vol(SFX_TIRE_SKID, (u8)(volume >> 1));
    }
}

/**
 * sndRoadNoise - Play pavement road noise
 * Arcade: sounds.c:sndRoadNoise()
 */
void sndRoadNoise(s16 pitch, s16 volume) {
    /* N64 implementation - road noise at low volume */
    /* Usually just engine noise dominates on pavement */
}

/**
 * sndSplashNoise - Play water splash noise
 * Arcade: sounds.c:sndSplashNoise()
 */
void sndSplashNoise(s16 pitch, s16 volume) {
    /* N64 implementation - water splash sound */
    if (volume > 0) {
        sound_play_vol(S_SPLASH, (u8)(volume >> 1));
    }
}

/**
 * sndWindNoise - Play wind noise
 * Arcade: sounds.c:sndWindNoise()
 */
void sndWindNoise(s16 pitch, s16 volume) {
    /* N64 implementation - wind rushing sound */
    /* Typically only audible at high speed or airborne */
}

/* ========================================================================
 * Tire Sound Functions
 * ======================================================================== */

/**
 * sndDoSkid - Play tire skid sound
 * Arcade: sounds.c:sndDoSkid()
 */
void sndDoSkid(s16 tire_num, s16 pitch, s16 volume) {
    if (volume > 0) {
        sound_tire_screech(this_node, (f32)volume / 255.0f);
    }
}

/**
 * DoTireSqueals - Update tire squeal sounds for a car
 * Arcade: carsnd.c:DoTireSqueals()
 *
 * Calculates tire forces and generates appropriate sounds.
 */
void DoTireSqueals(s16 drone_index) {
    s16 tnum;
    s16 index;
    u16 force;
    s32 force_h, force_l;
    s32 pitch_h, pitch_l, vol_h, vol_l, delta;
    s32 v_thresh, intensity;
    s32 make_noise;
    const skid_data *sk_tab;
    snd_state *screech_state;

    screech_state = &scrch_state[drone_index][0];

    make_noise = (drone_index == this_node) &&
                 (demo_game == 0 || (demo_game != 0 && attract_effects == 1));

    /* Process each tire position */
    for (tnum = 0; tnum < 4; tnum++) {
        sk_tab = &skid_tab[tnum];

        /* Get tire force (placeholder - would come from tire physics) */
        force = 500;  /* Placeholder value */

        if (force >= sk_tab->force_tab[0]) {
            screech_state[tnum].on = 1;

            /* Clamp to max */
            if (force > sk_tab->force_tab[4]) {
                force = sk_tab->force_tab[4];
            }

            /* Find interpolation segment */
            for (index = 0; index < 4; index++) {
                if (force <= sk_tab->force_tab[index + 1]) {
                    break;
                }
            }

            force_h = sk_tab->force_tab[index + 1];
            force_l = sk_tab->force_tab[index];

            /* Interpolate pitch */
            delta = (sk_tab->pitch_h - sk_tab->pitch_l) >> 2;
            pitch_l = sk_tab->pitch_l + delta * index;
            pitch_h = pitch_l + delta;

            /* Interpolate volume */
            delta = (sk_tab->vol_h - sk_tab->vol_l) >> 2;
            vol_l = sk_tab->vol_l + delta * index;
            vol_h = vol_l + delta;

            /* Linear interpolation */
            screech_state[tnum].pitch = (s16)((pitch_h - pitch_l) * force / (force_h - force_l) -
                                              (pitch_h - pitch_l) * force_l / (force_h - force_l) + pitch_l);
            screech_state[tnum].volume = (s16)((vol_h - vol_l) * force / (force_h - force_l) -
                                               (vol_h - vol_l) * force_l / (force_h - force_l) + vol_l);

            if (make_noise) {
                sndDoSkid(tnum, screech_state[tnum].pitch, screech_state[tnum].volume);
            }

            /* Update skid intensity */
            v_thresh = sk_tab->gr_skid_thresh;
            if (screech_state[tnum].volume > v_thresh) {
                intensity = (screech_state[tnum].volume - v_thresh) * 256 / (256 - v_thresh);
                skid_intensity[drone_index][tire_tab[tnum][0]][0] = (s16)intensity;
                skid_intensity[drone_index][tire_tab[tnum][1]][1] = (s16)intensity;
            } else {
                skid_intensity[drone_index][tire_tab[tnum][0]][0] = 0;
                skid_intensity[drone_index][tire_tab[tnum][1]][1] = 0;
            }

            /* Update smoke intensity */
            v_thresh = sk_tab->gr_smoke_thresh;
            if (screech_state[tnum].volume > v_thresh) {
                intensity = (screech_state[tnum].volume - v_thresh) * 256 / (256 - v_thresh);
                smoke_intensity[drone_index][tire_tab[tnum][0]][0] = (s16)intensity;
                smoke_intensity[drone_index][tire_tab[tnum][1]][1] = (s16)intensity;
            } else {
                smoke_intensity[drone_index][tire_tab[tnum][0]][0] = 0;
                smoke_intensity[drone_index][tire_tab[tnum][1]][1] = 0;
            }

        } else if (screech_state[tnum].on) {
            /* Fade out sound */
            screech_state[tnum].volume -= VOL_FADE_VAL;

            if (screech_state[tnum].volume <= 0) {
                screech_state[tnum].volume = 0;
                screech_state[tnum].on = 0;
            }

            if (make_noise) {
                sndDoSkid(tnum, screech_state[tnum].pitch, screech_state[tnum].volume);
            }

            /* Clear intensities when faded out */
            if (screech_state[tnum].volume <= 0) {
                skid_intensity[drone_index][tire_tab[tnum][0]][0] = 0;
                skid_intensity[drone_index][tire_tab[tnum][1]][1] = 0;
                smoke_intensity[drone_index][tire_tab[tnum][0]][0] = 0;
                smoke_intensity[drone_index][tire_tab[tnum][1]][1] = 0;
            }
        }
    }
}

/* ========================================================================
 * Collision Sound Functions
 * ======================================================================== */

/**
 * kill_scrape_sound - Kill scrape and locked sounds
 * Arcade: carsnd.c:kill_scrape_sound()
 */
void kill_scrape_sound(void) {
    if (scrape_state != 0 || scrape_time != 0) {
        sndPositionSound(S_KSCRAPELOOP, 0, 0);
    }
    scrape_state = 0;
    scrape_time = 0;
}

/**
 * get_force_and_peak - Calculate force magnitude and update peak state
 * Arcade: carsnd.c:get_force_and_peak()
 */
void get_force_and_peak(s16 index, f32 vec[3], f32 threshold) {
    f32 force;

    if (car_bump[index].bump_it == 0) {
        /* Check if force has cleared (time was set but peak is clear) */
        if (car_bump[index].time && car_bump[index].peak == 0.0f) {
            if (vec[0] == 0.0f && vec[1] == 0.0f && vec[2] == 0.0f) {
                if (IRQTIME - car_bump[index].time > ONE_SEC / 2) {
                    car_bump[index].time = 0;
                }
            } else {
                car_bump[index].time = IRQTIME;
            }
        } else {
            if (vec[0] != 0.0f || vec[1] != 0.0f || vec[2] != 0.0f) {
                force = fsqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);

                if (threshold < force) {
                    if (force > car_bump[index].peak) {
                        car_bump[index].peak = force;
                        car_bump[index].peak_vec[0] = vec[0];
                        car_bump[index].peak_vec[1] = vec[1];
                        car_bump[index].peak_vec[2] = vec[2];
                        car_bump[index].time = IRQTIME;
                    }
                }
            }

            if (car_bump[index].time && (IRQTIME - car_bump[index].time > ONE_SEC / 10)) {
                car_bump[index].bump_it = 1;
                car_bump[index].time = IRQTIME;
            }
        }
    }
}

/**
 * do_bump_sounds - Process collision sounds
 * Arcade: carsnd.c:do_bump_sounds()
 */
void do_bump_sounds(s16 update_car_sounds) {
    s16 i;
    s16 volume;
    s16 bump_index;
    s16 scrape_side;
    s16 scrape_snd;
    f32 high_force;

    if (update_car_sounds == 0) {
        init_bump_sounds();
        return;
    }

    /* Process wall/barrier bumps */
    bump_index = 0;
    volume = 0;
    high_force = 0.0f;

    if (bump_time != 0) {
        if (IRQTIME - bump_time > ONE_SEC) {
            bump_time = 0;
        }
    }

    /* Find highest force */
    for (i = 0; i < 4; i++) {
        if (car_bump[i].bump_it && car_bump[i].peak > high_force) {
            high_force = car_bump[i].peak;
        }
    }

    volume = (s16)range(high_force * 0.01f, 150, 235);

    /* Build bump index from corners that exceeded threshold */
    for (i = 0; i < 4; i++) {
        if (car_bump[i].bump_it) {
            if (car_bump[i].peak > high_force * 0.5f) {
                bump_index |= (1 << i);
            }
            car_bump[i].peak = 0.0f;
            car_bump[i].bump_it = 0;
        }
    }

    if (bump_index) {
        if (bump_time == 0) {
            sndPositionSound(S_CARBUMP, thump_tab[bump_index], (s16)(volume * 0.8f));
            sndPositionSound(S_BOOM, thump_tab[bump_index], volume);
            bump_time = IRQTIME;
        } else {
            sndPositionSound(S_CARSMASH, thump_tab[bump_index], volume);
            sndPositionSound(S_BOOM, thump_tab[bump_index], volume);
            bump_time = 0;
        }
    } else {
        /* Check car-to-car collision */
        if (car_bump[4].bump_it) {
            high_force = 0.0f;

            for (i = 0; i < 3; i++) {
                if (fabs(car_bump[4].peak_vec[i]) > high_force) {
                    high_force = fabs(car_bump[4].peak_vec[i]);
                }
            }

            volume = (s16)range(high_force * 0.001f, 190, 255);
            bump_index = 0;

            if (fabs(car_bump[4].peak_vec[0]) > high_force * 0.5f) {
                if (car_bump[4].peak_vec[0] < 0) {
                    bump_index |= 0x1;
                } else {
                    bump_index |= 0x2;
                }
            }

            if (fabs(car_bump[4].peak_vec[1]) > high_force * 0.5f) {
                if (car_bump[4].peak_vec[1] < 0) {
                    bump_index |= 0x4;
                } else {
                    bump_index |= 0x8;
                }
            }

            if (fabs(car_bump[4].peak_vec[2]) > high_force * 0.5f) {
                bump_index |= 0x1;
            }

            car_bump[4].peak = 0.0f;
            car_bump[4].bump_it = 0;

            if (bump_index) {
                sndPositionSound(S_CARSMASH, bump_tab[bump_index], volume);
                sndPositionSound(S_BOOM, bump_tab[bump_index], volume);
            }
        }
    }
}

/**
 * DoCarSounds - Main car sound update (per frame)
 * Arcade: carsnd.c:DoCarSounds()
 */
void DoCarSounds(s16 update_car_sounds, s8 skids_only) {
    if (update_car_sounds == 0) {
        InitCarSnds(this_node);
        return;
    }

    if (skids_only == 0) {
        /* Full sound update: wind, road noise, etc. */
        /* Would check surface type and airborne status */
    }

    /* Always update tire squeals */
    DoTireSqueals(this_node);
}

/* ========================================================================
 * Reverb Functions
 * ======================================================================== */

/**
 * init_reverb - Initialize reverb settings
 * Arcade: carsnd.c:init_reverb()
 */
void init_reverb(void) {
    u16 cmd_tab[2];

    /* Set reverb parameters */
    cmd_tab[0] = 6;
    sound_wparms(S_REVERB_PARMS, 1, cmd_tab);

    cmd_tab[0] = 0;
    cmd_tab[1] = 0;
    sound_wparms(S_REVERB_RETURN_VOL, 2, cmd_tab);

    in_tunnel = 0;
}

/**
 * handle_reverb - Update reverb based on car position
 * Arcade: carsnd.c:handle_reverb()
 */
void handle_reverb(void) {
    /* N64 stub - reverb handled differently on N64 */
}

/**
 * sound_wparms - Send sound command with parameters
 */
void sound_wparms(u16 cmd, s32 nargs, u16 *parms) {
    /* N64 stub - parameter-based sounds not directly supported */
}

/* ========================================================================
 * Radio Functions
 * ======================================================================== */

/**
 * music - Control radio music
 */
void music(s32 action, u8 station) {
    if (action == Do_it) {
        music_play(MUS_RACE_1 + station);
    } else {
        music_stop();
    }
}

/**
 * StartRadio - Start radio on given station
 * Arcade: carsnd.c:StartRadio()
 */
void StartRadio(u8 radio_station) {
    music(Do_it, radio_station);
}

/**
 * StopRadio - Stop radio
 * Arcade: carsnd.c:StopRadio()
 */
void StopRadio(u8 radio_station) {
    music(Undo_it, radio_station);
}

/******* DECOMPILED ROM FUNCTIONS *******/

/* N64 Sound handle structure (from ROM analysis) */
typedef struct N64SoundHandle {
    struct N64SoundHandle *next;  /* 0x00: Next in chain */
    void *field_04;               /* 0x04 */
    u8   pad_08[0x2C];            /* 0x08-0x33 */
    u16  state_index;             /* 0x34: Index into state array */
    u8   pad_36[6];               /* 0x36-0x3B */
    struct N64SoundHandle *linked;/* 0x3C: Linked sound handle */
} N64SoundHandle;

/* N64 Sound channel state */
typedef struct N64SoundState {
    u8   pad[22];                 /* 0x00-0x15 */
    u8   status;                  /* 0x16: 0=free, 1=playing, 2=stopped */
    u8   pad2[9];                 /* 0x17-0x1F */
} N64SoundState;  /* 0x20 bytes per entry */

/* External N64 sound data */
extern s32 active_sound_count;              /* Active sound count */
extern N64SoundHandle* active_sound_list[]; /* Active sound list */
extern N64SoundState sound_state_array[];   /* Sound state array */

/**
 * sound_handle_stop - Stop a sound handle (N64 ROM function)
 * (sound_stop @ 0x800B358C)
 * Address: 0x800B358C
 * Size: 156 bytes
 *
 * Stops a sound and removes it from the active sound list.
 * Processes all linked sounds in a chain.
 *
 * @param sound  Sound handle to stop (can be NULL)
 */
void sound_handle_stop(N64SoundHandle *sound) {
    s32 count;
    s32 i;
    s32 last_idx;
    N64SoundHandle *current;

    if (sound == NULL) {
        return;
    }

    current = sound;

    /* Process this sound and all linked sounds */
    do {
        count = active_sound_count;

        /* Find sound in active list */
        i = 0;
        if (count > 0) {
            while (i < count) {
                if (active_sound_list[i] == current) {
                    break;
                }
                i++;
            }
        }

        /* Mark sound state as stopped (2) */
        sound_state_array[current->state_index].status = 2;

        /* Remove from list by swapping with last entry */
        last_idx = count - 1;
        active_sound_list[i] = active_sound_list[last_idx];
        active_sound_list[last_idx] = current;
        active_sound_count = last_idx;

        /* Move to linked sound */
        current = current->linked;

    } while (current != NULL);
}
