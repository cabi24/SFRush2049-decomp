/**
 * music.h - Music playback interface for Rush 2049 N64
 *
 * Based on arcade source: game/sounds.c music() function and radio system.
 * The arcade uses a "radio station" metaphor for in-game music selection,
 * with separate attract-mode music tracks.
 *
 * N64 adaptation: Uses sequence player for MIDI-style music playback,
 * rather than the arcade's streaming audio system.
 */

#ifndef MUSIC_H
#define MUSIC_H

#include "types.h"

/* ========================================================================
 * Music Mode Constants (from arcade sounds.c)
 * ======================================================================== */

/* Action modes for music() function */
#define MUSIC_DO_IT     1       /* Start playing music */
#define MUSIC_UNDO_IT   0       /* Stop playing music */

/* Alternate names (arcade compatibility) - defined in sound.h */
#ifndef Do_it
#define Do_it           MUSIC_DO_IT
#endif
#ifndef Undo_it
#define Undo_it         MUSIC_UNDO_IT
#endif

/* ========================================================================
 * Radio Station Constants (from arcade sounds.c)
 * ======================================================================== */

/* Maximum number of radio stations */
#define MAX_RADIO       7

/* Number of engines (for engine sound selection) */
#define NUM_ENGINES     4

/* Radio station indices (0-6 for gameplay, 7+ for attract) */
typedef enum {
    RADIO_PULP = 0,         /* Pulp Fiction style */
    RADIO_AUG8,             /* August 8th */
    RADIO_ZETHNO,           /* Z-Ethno */
    RADIO_THEROCK,          /* The Rock (Alcatraz theme) */
    RADIO_STL,              /* STL */
    RADIO_RAVERUSH,         /* Rave Rush */
    RADIO_BLUES,            /* Blues */
    RADIO_MUTE,             /* No music (station index == MAX_RADIO) */
    RADIO_COUNT
} RadioStation;

/* ========================================================================
 * Arcade Sound Commands for Music (from sounds.h)
 * ======================================================================== */

/* Music track start commands */
#define S_PULPATT       0x8052  /* Pulp attract music */
#define S_AUG8ATT       0x8050  /* August 8th attract music */
#define S_ZETHNOATT     0x8056  /* Z-Ethno attract music */
#define S_THEROCK       0x80D8  /* The Rock (Alcatraz) music */
#define S_STLATT        0x8054  /* STL attract music */
#define S_RAVERUSHATT   0x8058  /* Rave Rush attract music */
#define S_BLUESATT      0x805A  /* Blues attract music */

/* Music track stop (kill) commands */
#define S_KPULPATT      0x8053  /* Kill Pulp music */
#define S_KAUG8ATT      0x8051  /* Kill August 8th music */
#define S_KZETHNOATT    0x8057  /* Kill Z-Ethno music */
#define S_KTHEROCK      0x80D9  /* Kill The Rock music */
#define S_KSTLATT       0x8055  /* Kill STL music */
#define S_KRAVERUSHATT  0x8059  /* Kill Rave Rush music */
#define S_KBLUESATT     0x805B  /* Kill Blues music */

/* Other music-related sounds */
#define S_WINNER_MUSIC  0x803C  /* Winner music */
#define S_KWINNER_MUSIC 0x803D  /* Kill winner music */
#define S_PLAYITAGAIN   0x805C  /* Play it again (high score entry) */
#define S_KPLAYITAGAIN  0x805D  /* Kill play it again */
#define S_WHATSYERNAME  0x8042  /* What's your name (high score entry) */
#define S_KWHATSYERNAME 0x8043  /* Kill what's your name */
#define S_WHATSTAG      0x805E  /* Go to tag for what's your name */
#define S_WAVENET       0x80F5  /* Wavenet tune */
#define S_KWAVENET      0x80F6  /* Kill wavenet tune */

/* Selection/menu music */
#define S_SELECT_MUSIC  0x8046  /* Track selection music */
#define S_KSELECT_MUSIC 0x8047  /* Kill track selection music */
#define S_CARSELECT     0x804B  /* Car selection music */
#define S_KCARSELECT    0x804C  /* Kill car selection music */
#define S_GARAGEAMB     0x8049  /* Garage ambience */
#define S_KGARAGEAMB    0x804A  /* Kill garage ambience */

/* Waiting/countdown music */
#define S_WAIT1         0x80DC  /* Waiting for drivers track 1 */
#define S_WAIT2         0x80DD  /* Waiting for drivers track 2 */
#define S_WAIT3         0x80DE  /* Waiting for drivers track 3 */
#define S_WAIT4         0x80DF  /* Waiting for drivers track 4 */
#define S_WAIT5         0x80E0  /* Waiting for drivers track 5 */
#define S_WAIT6         0x80E1  /* Waiting for drivers track 6 */
#define S_WAIT7         0x80E2  /* Waiting for drivers track 7 */
#define S_WAIT8         0x80E3  /* Waiting for drivers track 8 */
#define S_WAITROCK      0x80E4  /* Waiting for drivers The Rock */

/* Track announcement music */
#define S_YOU1          0x80E5  /* You have selected track 1 */
#define S_YOU2          0x80E6  /* You have selected track 2 */
#define S_YOU3          0x80E7  /* You have selected track 3 */
#define S_YOU4          0x80E8  /* You have selected track 4 */
#define S_YOU5          0x80E9  /* You have selected track 5 */
#define S_YOU6          0x80EA  /* You have selected track 6 */
#define S_YOU7          0x80EB  /* You have selected track 7 */
#define S_YOU8          0x80EC  /* You have selected track 8 */
#define S_YOUROCK       0x80ED  /* You have selected The Rock */

/* Tournament/network music */
#define S_TOURNEY1      0x80F9  /* Tournament play music */
#define S_REGIONAL      0x80FA  /* Regional winner music */
#define S_KREGIONAL     0x80FB  /* Kill regional music */
#define S_REGISTER1     0x80FC  /* Register to win music */

/* ========================================================================
 * Music State Variables (external)
 * ======================================================================== */

/* Current radio station (0-6 = music, 7 = mute) */
extern u8 radio_station;

/* Next radio station (for queued changes) */
extern u8 next_radio_station;

/* Music timing */
extern s32 playitagain_time;
extern s32 whats_your_name_time;
extern s32 flash_music_time;

/* Attract mode music/effects flags */
extern s32 attract_music;
extern s32 attract_effects;
extern s32 attract_music_save;
extern s32 attract_effects_save;

/* Demo station for attract mode */
extern s32 demo_station;

/* ========================================================================
 * Music Function Prototypes
 * ======================================================================== */

/**
 * music_init - Initialize music system
 *
 * Called at game startup to set up music state.
 */
void music_init_system(void);

/**
 * music - Arcade-style music control
 * Based on arcade: sounds.c:music()
 *
 * Controls radio station music playback.
 *
 * @param mode Action: Do_it (1) to start, Undo_it (0) to stop
 * @param station Radio station index (0-6) or attract mode (7+)
 */
void music_control(s16 mode, u8 station);

/**
 * StartRadio - Start playing a radio station
 * Based on arcade: carsnd.c:StartRadio()
 *
 * @param station Radio station index
 */
void music_start_radio(u8 station);

/**
 * StopRadio - Stop playing a radio station
 * Based on arcade: carsnd.c:StopRadio()
 *
 * @param station Radio station index
 */
void music_stop_radio(u8 station);

/**
 * music_next_station - Advance to next radio station
 *
 * Called when player presses music button during gameplay.
 */
void music_next_station(void);

/**
 * music_set_station - Set radio station directly
 *
 * @param station Radio station index (0-7)
 */
void music_set_station(u8 station);

/**
 * music_get_station - Get current radio station
 *
 * @return Current station index
 */
u8 music_get_station(void);

/**
 * music_mute - Mute/unmute music
 *
 * @param mute Non-zero to mute
 */
void music_mute(s32 mute);

/**
 * music_is_muted - Check if music is muted
 *
 * @return Non-zero if muted
 */
s32 music_is_muted(void);

/* ========================================================================
 * Attract Mode Music Functions
 * ======================================================================== */

/**
 * music_start_attract - Start attract mode music
 *
 * Starts the demo station music for attract mode.
 */
void music_start_attract(void);

/**
 * music_stop_attract - Stop attract mode music
 */
void music_stop_attract(void);

/**
 * music_next_attract - Advance to next attract mode track
 * Based on arcade: attract.c:next_demo_music()
 */
void music_next_attract(void);

/**
 * music_set_attract_flags - Set attract mode sound flags
 * Based on arcade: game.c attract sound configuration
 *
 * @param effects Enable sound effects in attract
 * @param music_flag Enable music in attract
 */
void music_set_attract_flags(s32 effects, s32 music_flag);

/**
 * music_save_attract_flags - Save attract flags for restoration
 */
void music_save_attract_flags(void);

/**
 * music_restore_attract_flags - Restore saved attract flags
 */
void music_restore_attract_flags(void);

/* ========================================================================
 * Game Event Music Functions
 * ======================================================================== */

/**
 * music_winner - Play winner music
 */
void music_play_winner(void);

/**
 * music_stop_winner - Stop winner music
 */
void music_stop_winner(void);

/**
 * music_hiscore - Play high score entry music
 */
void music_play_hiscore(void);

/**
 * music_stop_hiscore - Stop high score entry music
 */
void music_stop_hiscore(void);

/**
 * music_play_select - Play track selection music
 */
void music_play_select(void);

/**
 * music_stop_select - Stop track selection music
 */
void music_stop_select(void);

/**
 * music_play_carselect - Play car selection music
 */
void music_play_carselect(void);

/**
 * music_stop_carselect - Stop car selection music
 */
void music_stop_carselect(void);

/**
 * music_play_waiting - Play waiting for drivers music
 *
 * @param track_num Track number (0-8)
 */
void music_play_waiting(s32 track_num);

/**
 * music_play_track_announce - Play track selection announcement
 *
 * @param track_num Track number (0-8)
 */
void music_play_track_announce(s32 track_num);

/* ========================================================================
 * Volume and Fade Functions
 * ======================================================================== */

/**
 * music_set_volume - Set music volume
 *
 * @param volume Volume level (0-255)
 */
void music_set_volume_level(u8 volume);

/**
 * music_fade_out - Start fade out
 *
 * @param frames Number of frames to fade
 */
void music_fade_out_timed(u32 frames);

/**
 * music_fade_in - Start fade in
 *
 * @param frames Number of frames to fade
 */
void music_fade_in_timed(u32 frames);

/**
 * music_update - Per-frame music update
 *
 * Handles fades, timing, and music button input.
 * Called from main game loop.
 */
void music_update_frame(void);

/* ========================================================================
 * Music Button Handling (arcade has physical music button)
 * ======================================================================== */

/**
 * music_button_pressed - Handle music button press
 *
 * On N64, this is mapped to a controller button.
 * On arcade, this was a dedicated cabinet button.
 */
void music_button_pressed(void);

/**
 * music_button_held - Check if music button is being held
 *
 * @return Non-zero if held
 */
s32 music_button_held(void);

/**
 * music_flash_enabled - Check if music indicator should flash
 *
 * @return Non-zero if flashing
 */
s32 music_flash_enabled(void);

/* ========================================================================
 * Radio Station Tables (from arcade sounds.c)
 * ======================================================================== */

/* Radio off commands (indexed by station) */
extern const s16 demo_radio_off[MAX_RADIO];

/* Radio on commands (indexed by station) */
extern const s16 demo_radio_on[MAX_RADIO];

/* Maximum radio constant (for bounds checking) */
extern const s16 max_radio;

#endif /* MUSIC_H */
