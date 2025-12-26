/**
 * tournament.h - Tournament/Championship mode for Rush 2049 N64
 *
 * Manages multi-race championship series with point standings,
 * unlockable content, and progression systems.
 */

#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include "types.h"

/* Tournament types */
#define TOURNAMENT_SINGLE_RACE      0   /* Single race (quick race) */
#define TOURNAMENT_CIRCUIT          1   /* Circuit championship */
#define TOURNAMENT_ELIMINATOR       2   /* Elimination tournament */
#define TOURNAMENT_TIME_ATTACK      3   /* Time attack series */
#define TOURNAMENT_BATTLE_CUP       4   /* Battle mode cup */
#define TOURNAMENT_STUNT_CUP        5   /* Stunt challenge cup */
#define NUM_TOURNAMENT_TYPES        6

/* Circuit types */
#define CIRCUIT_BRONZE              0   /* Easy circuit */
#define CIRCUIT_SILVER              1   /* Medium circuit */
#define CIRCUIT_GOLD                2   /* Hard circuit */
#define CIRCUIT_PLATINUM            3   /* Expert circuit */
#define NUM_CIRCUITS                4

/* Tournament states */
#define TOURNEY_STATE_INACTIVE      0
#define TOURNEY_STATE_SETUP         1   /* Selecting options */
#define TOURNEY_STATE_PRE_RACE      2   /* Before race */
#define TOURNEY_STATE_RACING        3   /* In race */
#define TOURNEY_STATE_RESULTS       4   /* Race results */
#define TOURNEY_STATE_STANDINGS     5   /* Overall standings */
#define TOURNEY_STATE_COMPLETE      6   /* Tournament finished */
#define TOURNEY_STATE_ABANDONED     7   /* Player quit */

/* Tournament limits */
#define MAX_TOURNAMENT_RACES        8   /* Max races in tournament */
#define MAX_TOURNAMENT_PLAYERS      8   /* Max participants */
#define MAX_TOURNAMENT_NAME         16  /* Name length */

/* Elimination modes */
#define ELIM_LAST_PLACE             0   /* Last place eliminated */
#define ELIM_BELOW_TIME             1   /* Below time threshold */
#define ELIM_BOTTOM_HALF            2   /* Bottom half eliminated */

/* Points systems */
#define POINTS_STANDARD             0   /* 10,8,6,5,4,3,2,1 */
#define POINTS_F1                   1   /* 25,18,15,12,10,8,6,4,2,1 */
#define POINTS_LINEAR               2   /* 8,7,6,5,4,3,2,1 */
#define POINTS_WINNER_TAKES_ALL     3   /* 1,0,0,0... */

/* Per-race results */
typedef struct TourneyRaceResult {
    s32     position;           /* Final position */
    u32     time;               /* Race time (frames) */
    u32     best_lap;           /* Best lap time */
    s32     points;             /* Points earned */
    u8      finished;           /* Completed race */
    u8      dnf;                /* Did not finish */
    u8      eliminated;         /* Eliminated from tournament */
    u8      pad;
} TourneyRaceResult;

/* Player tournament entry */
typedef struct TourneyPlayer {
    char    name[MAX_TOURNAMENT_NAME];
    u8      active;             /* In tournament */
    u8      human;              /* Human player */
    u8      player_index;       /* Player slot */
    u8      car_type;           /* Car selection */

    s32     total_points;       /* Accumulated points */
    s32     current_position;   /* Position in standings */
    s32     best_position;      /* Best finish */
    s32     wins;               /* Race wins */

    /* Per-race results */
    TourneyRaceResult races[MAX_TOURNAMENT_RACES];

    /* Stats */
    u32     total_time;         /* Total race time */
    u32     best_lap_overall;   /* Best lap across all races */
    s32     poles;              /* Pole positions (best qualifier) */
    s32     fastest_laps;       /* Fastest laps in race */

} TourneyPlayer;

/* Race in tournament */
typedef struct TourneyRace {
    s32     track_id;           /* Track for this race */
    s32     laps;               /* Number of laps */
    u32     flags;              /* Track flags (mirror, etc.) */
    u8      completed;          /* Race finished */
    u8      current;            /* Current race */
    u8      pad[2];
    u32     race_time;          /* Total race duration */
} TourneyRace;

/* Circuit definition */
typedef struct CircuitDef {
    char    name[32];           /* Circuit name */
    s32     races[MAX_TOURNAMENT_RACES];    /* Track IDs */
    s32     num_races;          /* Number of races */
    s32     difficulty;         /* Difficulty level */
    u32     unlock_flags;       /* What this circuit unlocks */
} CircuitDef;

/* Tournament state */
typedef struct TournamentState {
    /* Type and mode */
    u8      type;               /* TOURNAMENT_* type */
    u8      circuit;            /* CIRCUIT_* if circuit mode */
    u8      state;              /* TOURNEY_STATE_* */
    u8      points_system;      /* POINTS_* system */

    /* Races */
    TourneyRace races[MAX_TOURNAMENT_RACES];
    s32     num_races;          /* Total races */
    s32     current_race;       /* Current race index */
    s32     races_completed;    /* Races finished */

    /* Players */
    TourneyPlayer players[MAX_TOURNAMENT_PLAYERS];
    s32     num_players;        /* Active players */
    s32     num_humans;         /* Human players */
    s32     num_remaining;      /* Non-eliminated */

    /* Elimination mode */
    u8      elimination;        /* Elimination enabled */
    u8      elim_mode;          /* ELIM_* mode */
    u8      elim_count;         /* How many eliminated per race */
    u8      pad;

    /* Difficulty */
    s32     ai_difficulty;      /* AI skill level */
    s32     rubber_band;        /* Rubber-banding strength */

    /* Unlock tracking */
    u32     unlocks_earned;     /* What's been unlocked */
    u8      new_unlocks;        /* New content unlocked */
    u8      pad2[3];

    /* Timer */
    u32     tournament_time;    /* Total elapsed time */

} TournamentState;

/* Global tournament state */
extern TournamentState gTournament;

/* Pre-defined circuits */
extern const CircuitDef gCircuits[NUM_CIRCUITS];

/* Initialization */
void tournament_init(void);
void tournament_reset(void);

/* Setup */
s32 tournament_start(s32 type);
s32 tournament_start_circuit(s32 circuit);
void tournament_end(void);
void tournament_abandon(void);

/* Player management */
s32 tournament_add_player(s32 player_index, s32 human);
s32 tournament_remove_player(s32 player_index);
s32 tournament_get_player_count(void);
TourneyPlayer* tournament_get_player(s32 index);
TourneyPlayer* tournament_get_player_by_position(s32 position);

/* Race progression */
s32 tournament_next_race(void);
s32 tournament_get_current_race(void);
s32 tournament_get_current_track(void);
s32 tournament_get_races_remaining(void);
TourneyRace* tournament_get_race(s32 index);

/* Results */
void tournament_submit_result(s32 player, s32 position, u32 time, u32 best_lap);
void tournament_calculate_points(void);
void tournament_update_standings(void);
void tournament_check_elimination(void);

/* Points */
s32 tournament_get_points(s32 player);
s32 tournament_get_position(s32 player);
s32 tournament_get_points_for_position(s32 position);
void tournament_set_points_system(s32 system);

/* Elimination */
void tournament_set_elimination(s32 enabled, s32 mode, s32 count);
s32 tournament_is_eliminated(s32 player);
void tournament_eliminate_player(s32 player);

/* Queries */
s32 tournament_is_active(void);
s32 tournament_is_final_race(void);
s32 tournament_is_complete(void);
s32 tournament_get_leader(void);
s32 tournament_get_winner(void);

/* Standings display */
void tournament_sort_standings(void);
void tournament_draw_standings(void);
void tournament_draw_race_results(void);
void tournament_draw_final_results(void);

/* Unlocks */
void tournament_check_unlocks(void);
u32 tournament_get_unlocks(void);
s32 tournament_has_new_unlocks(void);
void tournament_clear_new_unlocks(void);

/* Save/Load */
s32 tournament_save_progress(void);
s32 tournament_load_progress(void);
s32 tournament_can_continue(void);

/* State queries */
s32 tournament_get_state(void);
s32 tournament_get_type(void);
s32 tournament_get_circuit(void);

#endif /* TOURNAMENT_H */
