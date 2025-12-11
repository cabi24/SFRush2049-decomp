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
            f32 rpm = 1000.0f + car->speed * 30.0f;
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
