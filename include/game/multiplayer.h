/**
 * multiplayer.h - Multiplayer and split-screen system for Rush 2049 N64
 *
 * Manages multiple human players, split-screen viewports,
 * controller assignments, and local multiplayer modes.
 */

#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include "types.h"

/* Player limits */
#define MP_MAX_PLAYERS          4       /* Maximum human players */
#define MP_MAX_CONTROLLERS      4       /* Maximum controllers */
#define MP_MAX_CARS             8       /* Total cars including AI */

/* Multiplayer modes */
#define MP_MODE_SINGLE          0       /* Single player */
#define MP_MODE_SPLIT_2H        1       /* 2 player horizontal split */
#define MP_MODE_SPLIT_2V        2       /* 2 player vertical split */
#define MP_MODE_SPLIT_4         3       /* 4 player quad split */
#define MP_MODE_BATTLE          4       /* Battle mode (any player count) */
#define MP_MODE_STUNT           5       /* Stunt mode */
#define NUM_MP_MODES            6

/* Viewport positions */
#define VP_FULL                 0       /* Full screen */
#define VP_TOP                  1       /* Top half */
#define VP_BOTTOM               2       /* Bottom half */
#define VP_LEFT                 3       /* Left half */
#define VP_RIGHT                4       /* Right half */
#define VP_TOP_LEFT             5       /* Top-left quarter */
#define VP_TOP_RIGHT            6       /* Top-right quarter */
#define VP_BOTTOM_LEFT          7       /* Bottom-left quarter */
#define VP_BOTTOM_RIGHT         8       /* Bottom-right quarter */
#define NUM_VP_POSITIONS        9

/* Player states */
#define PLAYER_STATE_INACTIVE   0       /* Not participating */
#define PLAYER_STATE_WAITING    1       /* Waiting to join */
#define PLAYER_STATE_READY      2       /* Ready to race */
#define PLAYER_STATE_RACING     3       /* In race */
#define PLAYER_STATE_FINISHED   4       /* Finished race */
#define PLAYER_STATE_DNF        5       /* Did not finish */

/* Join states */
#define JOIN_STATE_NONE         0       /* No controller detected */
#define JOIN_STATE_DETECTED     1       /* Controller detected */
#define JOIN_STATE_PRESSED      2       /* Start pressed to join */
#define JOIN_STATE_JOINED       3       /* Successfully joined */
#define JOIN_STATE_DECLINED     4       /* Player declined to join */

/* Screen resolution */
#define SCREEN_WIDTH            320
#define SCREEN_HEIGHT           240

/* Viewport structure */
typedef struct Viewport {
    s16     x;                  /* Left edge */
    s16     y;                  /* Top edge */
    s16     width;              /* Viewport width */
    s16     height;             /* Viewport height */
    f32     aspect;             /* Aspect ratio */
    u8      position;           /* VP_* position code */
    u8      active;             /* Viewport in use */
    u8      player;             /* Player index using this viewport */
    u8      pad;
} Viewport;

/* Controller mapping */
typedef struct ControllerSlot {
    u8      connected;          /* Controller connected */
    u8      player;             /* Assigned player (-1 if none) */
    u8      type;               /* Controller type (standard, etc.) */
    u8      pak_type;           /* Accessory type (rumble, memory) */
} ControllerSlot;

/* Per-player multiplayer state */
typedef struct MPPlayer {
    /* Identity */
    u8      active;             /* Player slot active */
    u8      human;              /* Human (1) or AI (0) */
    u8      player_index;       /* Player number (0-3) */
    u8      car_index;          /* Car slot in race */

    /* Controller */
    s8      controller;         /* Controller index (-1 if AI) */
    u8      join_state;         /* JOIN_STATE_* */
    u8      state;              /* PLAYER_STATE_* */
    u8      ready;              /* Ready to start */

    /* Viewport */
    Viewport viewport;          /* Player's viewport */

    /* Car selection */
    u8      car_type;           /* Selected car */
    u8      car_color;          /* Car color index */
    u8      transmission;       /* Auto/manual */
    u8      pad;

    /* Race state */
    s32     position;           /* Race position (1st, 2nd, etc.) */
    s32     lap;                /* Current lap */
    u32     race_time;          /* Total race time */
    u32     best_lap_time;      /* Best lap */
    s32     checkpoint;         /* Last checkpoint passed */

    /* Statistics */
    s32     wins;               /* Session wins */
    s32     points;             /* Session points */

} MPPlayer;

/* Multiplayer session state */
typedef struct MultiplayerState {
    /* Mode */
    u8      mode;               /* MP_MODE_* */
    u8      active;             /* Session active */
    u8      in_race;            /* Currently racing */
    u8      paused;             /* Game paused */

    /* Players */
    MPPlayer players[MP_MAX_PLAYERS];
    s32     num_players;        /* Human players */
    s32     num_active;         /* Active (human + AI) */

    /* Controllers */
    ControllerSlot controllers[MP_MAX_CONTROLLERS];

    /* Viewports */
    Viewport viewports[MP_MAX_PLAYERS];
    s32     num_viewports;

    /* Join screen */
    u8      in_join_screen;     /* Showing join screen */
    u8      join_countdown;     /* Countdown to start */
    u8      min_players;        /* Minimum players required */
    u8      max_players;        /* Maximum players allowed */
    u32     join_timer;         /* Join screen timer */

    /* Race settings */
    s32     num_laps;           /* Laps for race */
    s32     num_ai_cars;        /* AI opponents */
    s32     difficulty;         /* AI difficulty */
    s32     track_id;           /* Selected track */
    u32     flags;              /* Track flags (mirror, etc.) */

    /* Session scores */
    s32     races_completed;    /* Races in session */
    s32     total_races;        /* Planned races (tournament) */

    /* Winner tracking */
    s32     race_winner;        /* Winner of current race */
    s32     session_leader;     /* Overall session leader */

} MultiplayerState;

/* Global multiplayer state */
extern MultiplayerState gMultiplayer;

/* Initialization */
void mp_init(void);
void mp_reset(void);
void mp_shutdown(void);

/* Mode setup */
s32 mp_start_session(s32 mode, s32 max_players);
void mp_end_session(void);
s32 mp_get_mode(void);
void mp_set_mode(s32 mode);

/* Player management */
s32 mp_add_player(s32 controller);
s32 mp_remove_player(s32 player);
s32 mp_get_num_players(void);
s32 mp_get_num_human_players(void);
MPPlayer* mp_get_player(s32 index);
s32 mp_is_player_active(s32 player);
s32 mp_is_player_human(s32 player);

/* Controller management */
void mp_detect_controllers(void);
s32 mp_get_controller_for_player(s32 player);
s32 mp_get_player_for_controller(s32 controller);
s32 mp_assign_controller(s32 player, s32 controller);
void mp_unassign_controller(s32 controller);
s32 mp_is_controller_connected(s32 controller);

/* Join screen */
void mp_start_join_screen(void);
void mp_update_join_screen(void);
void mp_draw_join_screen(void);
s32 mp_all_players_ready(void);
void mp_set_player_ready(s32 player, s32 ready);

/* Viewport management */
void mp_setup_viewports(void);
Viewport* mp_get_viewport(s32 player);
void mp_set_viewport(s32 player);
void mp_get_viewport_rect(s32 player, s16 *x, s16 *y, s16 *w, s16 *h);
void mp_apply_viewport(s32 player);
void mp_reset_viewport(void);

/* Race management */
void mp_start_race(void);
void mp_end_race(void);
void mp_update_race(void);
void mp_update_positions(void);
s32 mp_get_race_position(s32 player);
void mp_set_race_position(s32 player, s32 position);
void mp_player_finished(s32 player, u32 time);
s32 mp_all_finished(void);

/* Pause handling */
void mp_pause(s32 player);
void mp_unpause(void);
s32 mp_is_paused(void);
s32 mp_who_paused(void);

/* Results */
void mp_calculate_results(void);
s32 mp_get_winner(void);
void mp_award_points(void);
s32 mp_get_points(s32 player);
s32 mp_get_session_leader(void);

/* Car selection */
void mp_set_car_choice(s32 player, s32 car_type, s32 color);
void mp_set_transmission(s32 player, s32 auto_trans);
s32 mp_get_car_choice(s32 player);
s32 mp_get_car_color(s32 player);

/* Queries */
s32 mp_is_active(void);
s32 mp_is_split_screen(void);
s32 mp_get_split_type(void);
s32 mp_in_race(void);

/* AI opponents */
void mp_add_ai_opponents(s32 count, s32 difficulty);
void mp_remove_ai_opponents(void);

#endif /* MULTIPLAYER_H */
