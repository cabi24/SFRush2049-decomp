/**
 * music.c - Music playback system for Rush 2049 N64
 *
 * Based on arcade source: game/sounds.c music() function and radio system.
 * The arcade uses a "radio station" metaphor for in-game music selection.
 *
 * Arcade reference files:
 *   - sounds.c: music() function, radio station tables
 *   - carsnd.c: StartRadio(), StopRadio()
 *   - attract.c: demo music handling
 *   - game.c: radio_station state, flash_music_time
 *
 * N64 adaptation notes:
 *   - Uses sequence player for music (MIDI-style) vs arcade streaming
 *   - Radio stations map to different sequence banks
 *   - Music button mapped to controller vs arcade cabinet button
 */

#include "types.h"
#include "game/music.h"
#include "game/sound.h"

/* ========================================================================
 * Radio Station Tables (from arcade sounds.c)
 * ======================================================================== */

/**
 * Radio station off (kill) commands
 * Arcade: sounds.c demo_radio_off[]
 */
const s16 demo_radio_off[MAX_RADIO] = {
    S_KPULPATT,
    S_KAUG8ATT,
    S_KZETHNOATT,
    S_KTHEROCK,
    S_KSTLATT,
    S_KRAVERUSHATT,
    S_KBLUESATT
};

/**
 * Radio station on (start) commands
 * Arcade: sounds.c demo_radio_on[]
 */
const s16 demo_radio_on[MAX_RADIO] = {
    S_PULPATT,
    S_AUG8ATT,
    S_ZETHNOATT,
    S_THEROCK,
    S_STLATT,
    S_RAVERUSHATT,
    S_BLUESATT
};

/**
 * Maximum radio station constant
 * Arcade: sounds.c max_radio
 */
const s16 max_radio = MAX_RADIO;

/* ========================================================================
 * Music State Variables
 * ======================================================================== */

/* Current and next radio station */
u8 radio_station = 0;
u8 next_radio_station = 0;

/* Music timing (frame counters) */
s32 playitagain_time = 0;
s32 whats_your_name_time = 0;
s32 flash_music_time = 0;

/* Attract mode flags */
s32 attract_music = 1;
s32 attract_effects = 1;
s32 attract_music_save = 1;
s32 attract_effects_save = 1;

/* Demo station for attract mode */
s32 demo_station = 0;

/* Internal state */
static u8 music_muted = 0;
static u8 current_volume = 255;
static u32 fade_frames = 0;
static u8 fade_start_vol = 0;
static u8 fade_end_vol = 0;
static u8 fade_direction = 0;  /* 0 = out, 1 = in */
static s32 music_button_held_time = 0;

/* Frame counter (external) */
extern u32 frame_counter;
#ifndef IRQTIME
#define IRQTIME frame_counter
#endif
#ifndef ONE_SEC
#define ONE_SEC 60
#endif

/* Sound present flag (external) */
extern s32 sounds_are_present;

/* ========================================================================
 * Music Initialization
 * ======================================================================== */

/**
 * music_init_system - Initialize music system
 */
#ifdef NON_MATCHING
void music_init_system(void) {
    radio_station = 0;
    next_radio_station = 0;
    playitagain_time = 0;
    whats_your_name_time = 0;
    flash_music_time = 0;
    attract_music = 1;
    attract_effects = 1;
    attract_music_save = 1;
    attract_effects_save = 1;
    demo_station = 0;
    music_muted = 0;
    current_volume = 255;
    fade_frames = 0;
    music_button_held_time = 0;
}
#endif

/* ========================================================================
 * Core Music Control Functions
 * ======================================================================== */

/**
 * music_control - Arcade-style music control
 * Arcade: sounds.c:music()
 *
 * Controls radio station music playback. Handles both gameplay
 * stations (0-6) and demo/attract stations (7+).
 *
 * @param mode Action: Do_it (1) to start, Undo_it (0) to stop
 * @param station Radio station index
 */
#ifdef NON_MATCHING
void music_control(s16 mode, u8 station) {
    s16 sound_command;
    s16 flag;
    const s16 *radio_on_table;
    const s16 *radio_off_table;

    sound_command = S_KSTLATT;  /* Default kill command */
    flag = 1;

    /* Select table based on attract/gameplay mode */
    if (station > max_radio) {
        /* Attract mode station */
        station -= max_radio + 1;
        radio_off_table = demo_radio_off;
        radio_on_table = demo_radio_on;
    } else {
        /* Gameplay station */
        radio_off_table = demo_radio_off;
        radio_on_table = demo_radio_on;
    }

    switch (mode) {
        case MUSIC_UNDO_IT:
            /* Stop music */
            if (station >= max_radio) {
                station = 0;
            }
            sound_command = radio_off_table[station];
            flag = 0;
            break;

        case MUSIC_DO_IT:
            /* Start music */
            if (station >= max_radio) {
                /* Muted - don't start any music */
                flag = 0;
            } else {
                sound_command = radio_on_table[station];
            }
            break;
    }

    /* Update music indicator light (on arcade, this controlled cabinet LED) */
    /* ctl_mod_latch(flag ? LA_MUSIC : ~LA_MUSIC); */

    /* Send sound command */
    if (sounds_are_present) {
        SOUND((u16)sound_command);
    }
}
#endif

/**
 * music_start_radio - Start playing a radio station
 * Arcade: carsnd.c:StartRadio()
 */
#ifdef NON_MATCHING
void music_start_radio(u8 station) {
    music_control(MUSIC_DO_IT, station);
}
#endif

/**
 * music_stop_radio - Stop playing a radio station
 * Arcade: carsnd.c:StopRadio()
 */
#ifdef NON_MATCHING
void music_stop_radio(u8 station) {
    music_control(MUSIC_UNDO_IT, station);
}
#endif

/* ========================================================================
 * Radio Station Management
 * ======================================================================== */

/**
 * music_next_station - Advance to next radio station
 *
 * Cycles through stations: 0 -> 1 -> ... -> 6 -> 7 (mute) -> 0
 * Based on arcade: game.c music button handling
 */
#ifdef NON_MATCHING
void music_next_station(void) {
    /* Stop current station */
    music_stop_radio(radio_station);

    /* Move to next station */
    radio_station = next_radio_station;
    next_radio_station++;
    if (next_radio_station > max_radio) {
        next_radio_station = 0;
    }

    /* Start new station (unless muted) */
    if (radio_station < max_radio) {
        music_start_radio(radio_station);
    }
}
#endif

/**
 * music_set_station - Set radio station directly
 */
#ifdef NON_MATCHING
void music_set_station(u8 station) {
    if (station > max_radio) {
        station = max_radio;  /* Clamp to mute */
    }

    /* Stop current if playing */
    if (radio_station < max_radio) {
        music_stop_radio(radio_station);
    }

    radio_station = station;
    next_radio_station = station + 1;
    if (next_radio_station > max_radio) {
        next_radio_station = 0;
    }

    /* Start new station */
    if (station < max_radio) {
        music_start_radio(station);
    }
}
#endif

/**
 * music_get_station - Get current radio station
 */
#ifdef NON_MATCHING
u8 music_get_station(void) {
    return radio_station;
}
#endif

/**
 * music_mute - Mute/unmute music
 */
#ifdef NON_MATCHING
void music_mute(s32 mute) {
    if (mute) {
        if (!music_muted && radio_station < max_radio) {
            music_stop_radio(radio_station);
        }
        music_muted = 1;
    } else {
        if (music_muted && radio_station < max_radio) {
            music_start_radio(radio_station);
        }
        music_muted = 0;
    }
}
#endif

/**
 * music_is_muted - Check if music is muted
 */
#ifdef NON_MATCHING
s32 music_is_muted(void) {
    return music_muted || (radio_station >= max_radio);
}
#endif

/* ========================================================================
 * Attract Mode Music
 * ======================================================================== */

/**
 * music_start_attract - Start attract mode music
 *
 * Starts demo station music. Uses station index offset by max_radio+1
 * to indicate attract mode to the music() function.
 */
#ifdef NON_MATCHING
void music_start_attract(void) {
    if (attract_music && playitagain_time == 0) {
        music_start_radio(demo_station + max_radio + 1);
    }
}
#endif

/**
 * music_stop_attract - Stop attract mode music
 */
#ifdef NON_MATCHING
void music_stop_attract(void) {
    music_stop_radio(demo_station + max_radio + 1);
}
#endif

/**
 * music_next_attract - Advance to next attract mode track
 * Arcade: attract.c:next_demo_music()
 */
#ifdef NON_MATCHING
void music_next_attract(void) {
    if (attract_music && playitagain_time == 0) {
        demo_station++;
        if (demo_station >= max_radio) {
            demo_station = 0;
        }
        music_start_radio(demo_station + max_radio + 1);
    }
}
#endif

/**
 * music_set_attract_flags - Set attract mode sound flags
 * Arcade: game.c attract sound configuration
 *
 * Based on NVRAM attract sound option (0-3):
 *   0: effects=off, music=off
 *   1: effects=on,  music=off
 *   2: effects=on,  music=on
 *   3: effects=off, music=on
 */
#ifdef NON_MATCHING
void music_set_attract_flags(s32 effects, s32 music_flag) {
    attract_effects = effects ? 1 : 0;
    attract_music = music_flag ? 1 : 0;
}
#endif

/**
 * music_save_attract_flags - Save attract flags for restoration
 */
#ifdef NON_MATCHING
void music_save_attract_flags(void) {
    attract_effects_save = attract_effects;
    attract_music_save = attract_music;
}
#endif

/**
 * music_restore_attract_flags - Restore saved attract flags
 */
#ifdef NON_MATCHING
void music_restore_attract_flags(void) {
    attract_effects = attract_effects_save;
    attract_music = attract_music_save;
}
#endif

/* ========================================================================
 * Game Event Music
 * ======================================================================== */

/**
 * music_play_winner - Play winner music
 */
#ifdef NON_MATCHING
void music_play_winner(void) {
    SOUND(S_WINNER_MUSIC);
}
#endif

/**
 * music_stop_winner - Stop winner music
 */
#ifdef NON_MATCHING
void music_stop_winner(void) {
    SOUND(S_KWINNER_MUSIC);
}
#endif

/**
 * music_play_hiscore - Play high score entry music
 */
#ifdef NON_MATCHING
void music_play_hiscore(void) {
    playitagain_time = IRQTIME;
    SOUND(S_PLAYITAGAIN);
}
#endif

/**
 * music_stop_hiscore - Stop high score entry music
 */
#ifdef NON_MATCHING
void music_stop_hiscore(void) {
    SOUND(S_KPLAYITAGAIN);
    playitagain_time = 0;
}
#endif

/**
 * music_play_select - Play track selection music
 */
#ifdef NON_MATCHING
void music_play_select(void) {
    SOUND(S_SELECT_MUSIC);
}
#endif

/**
 * music_stop_select - Stop track selection music
 */
#ifdef NON_MATCHING
void music_stop_select(void) {
    SOUND(S_KSELECT_MUSIC);
}
#endif

/**
 * music_play_carselect - Play car selection music
 */
#ifdef NON_MATCHING
void music_play_carselect(void) {
    SOUND(S_CARSELECT);
}
#endif

/**
 * music_stop_carselect - Stop car selection music
 */
#ifdef NON_MATCHING
void music_stop_carselect(void) {
    SOUND(S_KCARSELECT);
}
#endif

/**
 * music_play_waiting - Play waiting for drivers music
 */
#ifdef NON_MATCHING
void music_play_waiting(s32 track_num) {
    static const u16 wait_sounds[9] = {
        S_WAIT1, S_WAIT2, S_WAIT3, S_WAIT4,
        S_WAIT5, S_WAIT6, S_WAIT7, S_WAIT8,
        S_WAITROCK
    };

    if (track_num >= 0 && track_num < 9) {
        SOUND(wait_sounds[track_num]);
    }
}
#endif

/**
 * music_play_track_announce - Play track selection announcement
 */
#ifdef NON_MATCHING
void music_play_track_announce(s32 track_num) {
    static const u16 you_sounds[9] = {
        S_YOU1, S_YOU2, S_YOU3, S_YOU4,
        S_YOU5, S_YOU6, S_YOU7, S_YOU8,
        S_YOUROCK
    };

    if (track_num >= 0 && track_num < 9) {
        SOUND(you_sounds[track_num]);
    }
}
#endif

/* ========================================================================
 * Volume and Fade Control
 * ======================================================================== */

/**
 * music_set_volume_level - Set music volume
 */
#ifdef NON_MATCHING
void music_set_volume_level(u8 volume) {
    current_volume = volume;
    music_set_volume(volume);
}
#endif

/**
 * music_fade_out_timed - Start fade out
 */
#ifdef NON_MATCHING
void music_fade_out_timed(u32 frames) {
    if (frames == 0) {
        music_set_volume_level(0);
        return;
    }
    fade_frames = frames;
    fade_start_vol = current_volume;
    fade_end_vol = 0;
    fade_direction = 0;
}
#endif

/**
 * music_fade_in_timed - Start fade in
 */
#ifdef NON_MATCHING
void music_fade_in_timed(u32 frames) {
    if (frames == 0) {
        music_set_volume_level(255);
        return;
    }
    fade_frames = frames;
    fade_start_vol = current_volume;
    fade_end_vol = 255;
    fade_direction = 1;
}
#endif

/* ========================================================================
 * Music Update (Per-Frame)
 * ======================================================================== */

/**
 * music_update_frame - Per-frame music update
 *
 * Handles:
 *   - Volume fades
 *   - Play it again timeout (high score entry music)
 *   - Music button flash timing
 */
#ifdef NON_MATCHING
void music_update_frame(void) {
    u32 elapsed;
    u8 new_vol;

    /* Handle volume fade */
    if (fade_frames > 0) {
        fade_frames--;
        if (fade_frames == 0) {
            music_set_volume_level(fade_end_vol);
        } else {
            /* Linear interpolation */
            if (fade_direction == 0) {
                /* Fade out */
                new_vol = (u8)(fade_start_vol -
                    ((fade_start_vol - fade_end_vol) *
                     (fade_frames) / (fade_frames + 1)));
            } else {
                /* Fade in */
                new_vol = (u8)(fade_start_vol +
                    ((fade_end_vol - fade_start_vol) *
                     (1) / (fade_frames + 1)));
            }
            music_set_volume_level(new_vol);
        }
    }

    /* Handle play it again timeout (23 seconds) */
    if (playitagain_time != 0) {
        if (IRQTIME - playitagain_time > 23 * ONE_SEC) {
            if (attract_music) {
                music_start_attract();
            }
        }
    }

    /* Handle music flash timing (10 seconds) */
    if (flash_music_time != 0) {
        if (IRQTIME - flash_music_time > 10 * ONE_SEC) {
            flash_music_time = 0;
        }
    }
}
#endif

/* ========================================================================
 * Music Button Handling
 * ======================================================================== */

/**
 * music_button_pressed - Handle music button press
 *
 * Arcade: game.c music button handling
 * On arcade, holding music button for 2+ seconds shows credits.
 * On N64, just cycles through stations.
 */
#ifdef NON_MATCHING
void music_button_pressed(void) {
    music_next_station();
    flash_music_time = IRQTIME;
}
#endif

/**
 * music_button_held - Check if music button is being held
 */
#ifdef NON_MATCHING
s32 music_button_held(void) {
    return (music_button_held_time != 0 &&
            (IRQTIME - music_button_held_time) > 0);
}
#endif

/**
 * music_flash_enabled - Check if music indicator should flash
 *
 * Returns true for 10 seconds after music button press,
 * or if currently on mute station.
 */
#ifdef NON_MATCHING
s32 music_flash_enabled(void) {
    if (flash_music_time != 0) {
        if (IRQTIME - flash_music_time < 10 * ONE_SEC) {
            if (radio_station == max_radio) {
                return 1;  /* Flash on mute */
            }
        }
    }
    return 0;
}
#endif
