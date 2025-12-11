/**
 * carsel.h - Car/Track selection for Rush 2049 N64
 *
 * Based on arcade game/sselect.c
 * Handles car selection, track selection, and pre-race setup.
 */

#ifndef CARSEL_H
#define CARSEL_H

#include "types.h"

/* Maximum values */
#define MAX_SELECTABLE_CARS     12
#define MAX_SELECTABLE_TRACKS   8
#define MAX_PLAYERS             4

/* Selection states */
#define SEL_STATE_INACTIVE      0
#define SEL_STATE_CHOOSING      1
#define SEL_STATE_LOCKED        2
#define SEL_STATE_CONFIRMED     3

/* Car classes/types */
#define CAR_CLASS_COMPACT       0
#define CAR_CLASS_SPORTS        1
#define CAR_CLASS_MUSCLE        2
#define CAR_CLASS_EXOTIC        3

/* Track difficulty */
#define TRACK_EASY              0
#define TRACK_MEDIUM            1
#define TRACK_HARD              2
#define TRACK_EXPERT            3

/* Transmission modes */
#define TRANS_AUTO              0
#define TRANS_MANUAL            1

/* Car stats structure */
typedef struct CarStats {
    char    name[16];           /* Car name */
    u8      car_class;          /* Car class */
    u8      speed;              /* Max speed rating (0-100) */
    u8      accel;              /* Acceleration rating (0-100) */
    u8      handling;           /* Handling rating (0-100) */
    u8      weight;             /* Weight rating (0-100) */
    u8      traction;           /* Traction rating (0-100) */
    u8      unlocked;           /* Is car unlocked */
    u8      pad;
    u32     model_id;           /* 3D model ID */
    u32     preview_texture;    /* Preview image */
} CarStats;

/* Track info structure */
typedef struct TrackInfo {
    char    name[24];           /* Track name */
    char    description[64];    /* Track description */
    u8      difficulty;         /* Track difficulty */
    u8      num_laps;           /* Default lap count */
    u8      num_checkpoints;    /* Checkpoint count */
    u8      unlocked;           /* Is track unlocked */
    u32     track_id;           /* Track data ID */
    u32     preview_texture;    /* Preview image */
    u32     map_texture;        /* Mini-map texture */
} TrackInfo;

/* Player selection state */
typedef struct PlayerSelect {
    s32     car_index;          /* Selected car index */
    s32     color_index;        /* Selected car color */
    s32     trans_mode;         /* Transmission mode */
    u8      state;              /* Selection state */
    u8      is_active;          /* Player is active */
    u8      is_ready;           /* Player confirmed selection */
    u8      pad;
    u32     input_timer;        /* Input repeat timer */
    f32     turntable_angle;    /* Car preview rotation */
} PlayerSelect;

/* Selection screen state */
typedef struct SelectState {
    /* Track selection */
    s32     track_index;        /* Currently selected track */
    s32     track_locked;       /* Track selection locked */

    /* Player selections */
    PlayerSelect players[MAX_PLAYERS];
    s32     num_players;

    /* Timing */
    u32     timer;              /* Selection countdown timer */
    u32     state_time;         /* Time in current state */

    /* Display */
    f32     camera_pos[3];      /* Camera position */
    f32     camera_target[3];   /* Camera look-at */

    /* State flags */
    u8      all_ready;          /* All players ready */
    u8      countdown_active;   /* Countdown started */
    u8      transitioning;      /* Transitioning to race */
    u8      pad;

} SelectState;

/* Global state */
extern SelectState gSelect;
extern CarStats gCarList[];
extern TrackInfo gTrackList[];
extern s32 gNumCars;
extern s32 gNumTracks;

/* Initialization */
void carsel_init(void);
void carsel_reset(void);

/* State updates */
void carsel_update(void);
void carsel_draw(void);

/* Track selection */
void tracksel_init(void);
void tracksel_update(void);
void tracksel_draw(void);
void tracksel_select_next(void);
void tracksel_select_prev(void);
void tracksel_confirm(void);

/* Car selection */
void carsel_player_init(s32 player);
void carsel_player_update(s32 player);
void carsel_player_draw(s32 player);
void carsel_select_next_car(s32 player);
void carsel_select_prev_car(s32 player);
void carsel_select_next_color(s32 player);
void carsel_select_prev_color(s32 player);
void carsel_toggle_trans(s32 player);
void carsel_confirm(s32 player);
void carsel_cancel(s32 player);

/* Queries */
s32 carsel_get_selected_car(s32 player);
s32 carsel_get_selected_track(void);
s32 carsel_all_players_ready(void);
s32 carsel_is_car_available(s32 car_index);
s32 carsel_is_track_available(s32 track_index);

/* Car/Track info */
CarStats* carsel_get_car_stats(s32 car_index);
TrackInfo* carsel_get_track_info(s32 track_index);

/* Unlock management */
void carsel_unlock_car(s32 car_index);
void carsel_unlock_track(s32 track_index);
void carsel_save_unlocks(void);
void carsel_load_unlocks(void);

/* Countdown and transition */
void carsel_start_countdown(void);
void carsel_update_countdown(void);
s32 carsel_countdown_finished(void);
void carsel_start_race(void);

#endif /* CARSEL_H */
