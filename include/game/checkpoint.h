/**
 * checkpoint.h - Checkpoint and lap counting for Rush 2049 N64
 *
 * Based on arcade game/checkpoint.c
 * Handles checkpoint detection, lap counting, race positions
 */

#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include "types.h"

/* Maximum values */
#define MAX_CHECKPOINTS     20      /* Maximum checkpoints per track */
#define MAX_TRACKS          8       /* Number of tracks */
#define MAX_PATH_POINTS     1500    /* Maximum path points */

/**
 * Checkpoint Data - Single checkpoint definition
 * Based on arcade CheckPoint struct
 */
typedef struct CheckPoint {
    f32     pos[3];             /* World position (x, y, z) */
    f32     uvs[3][3];          /* Orientation matrix */
    f32     radius;             /* Trigger radius squared */
    f32     bonus_t[2];         /* Bonus time [first_lap, other_laps] */
    s16     track_cent;         /* Closest path center index */
    s16     logical_cp;         /* Logical checkpoint number (-1 = end) */
} CheckPoint;

/**
 * Track Checkpoint Data - Per-track checkpoint info
 * Based on arcade Track struct
 */
typedef struct Track {
    CheckPoint  *chk_point;     /* Pointer to checkpoint array */
    s16         num_checkpoints;/* Number of checkpoints */
    s16         direction;      /* Track direction (-1 or 1) */
} Track;

/**
 * Track Data - Per-track race configuration
 * Based on arcade Track_Data struct
 */
typedef struct TrackData {
    u32     start_time[8];      /* Start time per difficulty */
    u32     end_time[8];        /* End time per difficulty */
    s16     loop_chkpnt;        /* First checkpoint in lap loop */
    s16     finish_line;        /* Finish line checkpoint index */
    s16     before_finish;      /* Checkpoint before finish */
    s16     number_of_laps;     /* Laps in current race */
} TrackData;

/* Checkpoint system state */
extern u32 number_checkpoints;          /* Current track checkpoint count */
extern u32 last_checkpoint_time;        /* Time of last checkpoint hit */
extern s32 first_place_time;            /* Time entered first place */
extern s16 global_laps;                 /* Lead car lap count */
extern s16 global_last_cp;              /* Lead car last checkpoint */
extern s16 global_next_cp;              /* Lead car next checkpoint */
extern s16 lap_loop_index;              /* First checkpoint in loop */
extern u32 time_to_be_given;            /* Total bonus time available */
extern u32 total_time_given;            /* Total bonus time awarded */

/* Track/checkpoint arrays */
extern CheckPoint TrackCPs[MAX_TRACKS][MAX_CHECKPOINTS];
extern Track CP_Track[MAX_TRACKS];
extern TrackData track_data[MAX_TRACKS];

/* Path data for position calculation */
extern s16 path_dist_index[MAX_CHECKPOINTS];
extern u16 path_dist[MAX_PATH_POINTS];
extern s16 path_index[MAX_PATH_POINTS];
extern s16 path_to_maxpath[MAX_PATH_POINTS];
extern s16 num_path_points;

/* Initialization functions */
void init_cp_data(void);                /* Initialize checkpoint structures */
void InitCPS(void);                     /* Initialize for race start */
void init_cp_time(void);                /* Initialize checkpoint timing */

/* Path navigation */
s16 get_next_center(s16 t_index, s16 t_direction);
s16 get_prev_center(s16 t_index, s16 t_direction);
u32 get_next_checkpoint(u32 cur_checkpoint);

/* Checkpoint detection */
void CheckCPs(void);                    /* Check all cars for checkpoints */
void PassedCP(s32 car_index, s8 flag);  /* Car passed checkpoint */
void JumpToNextCP(void);                /* Skip to next checkpoint */

/* Position/timing */
void check_everyones_checkpoints(void); /* Update global checkpoint state */
void CarPassedCheckpoint(s32 slot, s32 checkpoint, s32 flag);
void CarReportsGameOver(s32 slot, u32 score, u32 flag);

/* Utility */
void set_game_time_to_one_second(void); /* End race quickly */
void update_lap_counter(s16 mode, s16 laps_left);
u32  get_checkpoint_bonus_time(s32 cp_index, s32 lap);
f32  calc_car_distance(s32 car_index);

/* Race position sorting */
void sort_race_positions(void);         /* Sort active cars by distance */
void sort_finished_positions(void);     /* Sort finished cars by time */
void update_all_positions(void);        /* Full position update */
s32  get_race_position(s32 car_index);  /* Get car's position */
s32  get_race_leader(void);             /* Get leader's car index */
s32  is_in_first_place(void);           /* Check if player is leading */
f32  get_distance_to_leader(s32 car_index);   /* Distance behind leader */
f32  get_distance_to_car(s32 car1, s32 car2); /* Distance between cars */

/* Mode values for update_lap_counter */
#define CP_INITIALIZE   0
#define CP_UPDATE       1
#define CP_CLEANUP      2

#endif /* CHECKPOINT_H */
