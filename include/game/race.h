/**
 * race.h - Race timing and management for Rush 2049 N64
 *
 * Based on arcade game/game.c and game/checkpoint.c
 * Handles lap timing, positions, checkpoints, and race state.
 */

#ifndef RACE_H
#define RACE_H

#include "types.h"

/* Maximum values */
#define MAX_LAPS            10
#define MAX_CHECKPOINTS     32
#define MAX_RACE_CARS       8

/* Race state flags */
#define RACE_FLAG_STARTED       0x01
#define RACE_FLAG_FINISHED      0x02
#define RACE_FLAG_PAUSED        0x04
#define RACE_FLAG_COUNTDOWN     0x08
#define RACE_FLAG_EXTENDED      0x10    /* Time extended */
#define RACE_FLAG_FINAL_LAP     0x20    /* On final lap */

/* Countdown states */
#define COUNTDOWN_READY     0
#define COUNTDOWN_3         1
#define COUNTDOWN_2         2
#define COUNTDOWN_1         3
#define COUNTDOWN_GO        4
#define COUNTDOWN_DONE      5

/* Race result codes */
#define RESULT_NONE         0
#define RESULT_WIN          1
#define RESULT_FINISH       2
#define RESULT_DNF          3       /* Did not finish */
#define RESULT_TIMEOUT      4

/* Frame rate */
#define RACE_FRAMES_PER_SEC     60
#define RACE_TIME_EXTENDED      (30 * RACE_FRAMES_PER_SEC)  /* 30 sec extension */

/* Lap time entry */
typedef struct LapTime {
    u32     frames;         /* Time in frames */
    u8      valid;          /* Is this a valid time? */
    u8      best;           /* Is this the best lap? */
    u8      pad[2];
} LapTime;

/* Checkpoint state per car */
typedef struct CheckpointState {
    u16     last_cp;            /* Last checkpoint passed */
    u16     next_cp;            /* Next expected checkpoint */
    u8      cp_hit[MAX_CHECKPOINTS / 8];  /* Bitmap of CPs hit this lap */
    u8      wrong_way;          /* Going wrong way */
    u8      on_shortcut;        /* On shortcut path */
    u8      pad;
} CheckpointState;

/* Per-car race state */
typedef struct CarRaceState {
    /* Timing */
    u32     start_time;         /* Frame race started */
    u32     finish_time;        /* Frame finished race */
    u32     total_time;         /* Total race time */
    LapTime lap_times[MAX_LAPS];
    u32     best_lap_time;      /* Best single lap */
    u32     current_lap_start;  /* Frame current lap started */

    /* Position */
    u8      position;           /* Current race position (1-8) */
    u8      current_lap;        /* Current lap number (1-based) */
    u8      finished;           /* Has finished race */
    u8      result;             /* Race result code */

    /* Checkpoint tracking */
    CheckpointState cp_state;

    /* Progress */
    f32     track_progress;     /* 0.0-1.0 around track */
    f32     total_progress;     /* Including laps */

} CarRaceState;

/* Race configuration */
typedef struct RaceConfig {
    s32     num_laps;           /* Total laps */
    s32     num_checkpoints;    /* Checkpoints per lap */
    s32     start_time;         /* Initial time (frames) */
    s32     checkpoint_bonus;   /* Time bonus per checkpoint */
    s32     difficulty;         /* Difficulty level */
    u8      flags;              /* Race flags */
    u8      track_id;           /* Track identifier */
    u8      pad[2];
} RaceConfig;

/* Global race state */
typedef struct RaceState {
    RaceConfig      config;
    CarRaceState    cars[MAX_RACE_CARS];
    s32             num_cars;
    u32             race_time;          /* Current race time */
    u32             countdown_timer;    /* Countdown frames remaining */
    u8              countdown_state;    /* Current countdown state */
    u8              race_flags;         /* Race state flags */
    u8              leader;             /* Current leader index */
    u8              num_finished;       /* Cars that have finished */
    s32             time_remaining;     /* Time left (checkpoint mode) */
} RaceState;

/* Global race state */
extern RaceState gRace;

/* Race initialization */
void race_init(void);
void race_reset(void);
void race_start(s32 num_laps, s32 num_cars);
void race_configure(RaceConfig *config);

/* Per-frame update */
void race_update(void);
void race_update_positions(void);
void race_update_checkpoints(void);

/* Countdown */
void race_start_countdown(void);
void race_update_countdown(void);
s32 race_countdown_finished(void);

/* Lap management */
void race_car_crossed_start(s32 car_index);
void race_car_finished_lap(s32 car_index);
void race_car_finished_race(s32 car_index);
u32 race_get_lap_time(s32 car_index, s32 lap);
u32 race_get_best_lap(s32 car_index);

/* Checkpoint management */
void race_car_hit_checkpoint(s32 car_index, s32 checkpoint_id);
s32 race_is_wrong_way(s32 car_index);
void race_reset_checkpoint_state(s32 car_index);

/* Position tracking */
s32 race_get_position(s32 car_index);
s32 race_get_leader(void);
f32 race_get_progress(s32 car_index);

/* Time management */
u32 race_get_time(void);
u32 race_get_car_time(s32 car_index);
s32 race_get_time_remaining(void);
void race_extend_time(s32 frames);

/* Race state queries */
s32 race_is_started(void);
s32 race_is_finished(void);
s32 race_is_paused(void);
s32 race_car_has_finished(s32 car_index);

/* Time formatting */
void race_format_time(u32 frames, char *buffer);
void race_format_time_diff(s32 diff_frames, char *buffer);

#endif /* RACE_H */
