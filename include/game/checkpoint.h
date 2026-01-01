/**
 * checkpoint.h - Checkpoint and lap counting for Rush 2049 N64
 *
 * Based on arcade game/checkpoint.c (1996 Time Warner Interactive)
 * Handles checkpoint detection, lap counting, race positions
 */

#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include "types.h"

/* ========================================================================
 * Arcade-compatible defines (from rushtherock/game/checkpoint.h)
 * ======================================================================== */

/* Maximum values - arcade compatible */
#define MAX_CPS             20      /* Arcade: MAX_CPS */
#define MAX_CHECKPOINTS     20      /* Alias for MAX_CPS */
#define MAX_TRACKS          8       /* Arcade: MAX_TRACKS (globals.h) */
#define MAX_LINKS           8       /* Arcade: MAX_LINKS - Max cars active */
#define MAX_PATH_POINTS     1500    /* Maximum path points */

/* CP Editor flag (arcade: used to enable debug menus) */
#define CP_EDITOR           1

/* Arcade compile-time flags (from checkpoint.c) */
#define LAP_HACK            1       /* Alcatraz track special case */
#define GAMEOVER_RESEND     1       /* Resend gameover PDUs until ACKed */

/* Debug switch flags (arcade: dlevels bit flags) */
#define SW_DEBUG1           0x0001  /* Drone learning */
#define SW_DEBUG11          0x0400  /* Checkpoint path errors */
#define SW_DEBUG14          0x2000  /* Show checkpoint objects */

/* Arcade type aliases for portability */
typedef s8  S8;
typedef s16 S16;
typedef s32 S32;
typedef u8  U8;
typedef u16 U16;
typedef u32 U32;
typedef f32 F32;
typedef s8  BOOL;

/* ========================================================================
 * Arcade-compatible structures (from rushtherock/game/checkpoint.h)
 * ======================================================================== */

/**
 * InitCheckPoint - Initial checkpoint data from track definition
 * Arcade: InitCheckPoint struct (checkpoint.h:74-80)
 *
 * This is the ROM/static data format used in cpinit.c.
 * At runtime, data is copied to CheckPoint struct.
 */
typedef struct InitCheckPoint {
    f32     pos[3];             /* World position (x, y, z) */
    s32     radius;             /* Trigger radius (not squared) */
    s16     logical_cp;         /* Checkpoint order (-1 = end sentinel) */
    f32     best_t[2];          /* Par time [first_lap, other_laps] in seconds */
} InitCheckPoint;

/**
 * CheckPoint - Runtime checkpoint data
 * Arcade: CheckPoint struct (checkpoint.h:82-90)
 *
 * Used during gameplay. Populated from InitCheckPoint during init_cp_data.
 */
typedef struct CheckPoint {
    f32     pos[3];             /* World position (x, y, z) */
    f32     uvs[3][3];          /* Orientation matrix for plane test */
    s32     radius;             /* Trigger radius (not squared) - arcade: S32 */
    s16     track_cent;         /* Track center index over checkpoint */
    s16     logical_cp;         /* Checkpoint order (-1 = end sentinel) */
    f32     bonus_t[2];         /* Bonus time [first_lap, other_laps] in frames */
} CheckPoint;

/**
 * Init_Track_Data - Initial track data from ROM
 * Arcade: Init_Track_Data struct (checkpoint.h:92-99)
 *
 * Static data read from track definition.
 */
typedef struct Init_Track_Data {
    f32     start_time[8];      /* Start time per difficulty (seconds) */
    f32     end_time[8];        /* End time per difficulty (seconds) */
    s16     loop_chkpnt;        /* First checkpoint in lap loop */
    s16     finish_line;        /* Finish line checkpoint index */
    s16     before_finish;      /* Checkpoint before finish */
} Init_Track_Data;

/**
 * Track_Data - Runtime track data
 * Arcade: Track_Data struct (checkpoint.h:101-109)
 *
 * Populated from Init_Track_Data at race start.
 */
typedef struct Track_Data {
    f32     start_time[8];      /* Start time per difficulty (frames) */
    f32     end_time[8];        /* End time per difficulty (frames) */
    s16     loop_chkpnt;        /* First checkpoint in lap loop */
    s16     finish_line;        /* Finish line checkpoint index */
    s16     before_finish;      /* Checkpoint before finish */
    s16     number_of_laps;     /* Laps in current race */
} Track_Data;

/* N64 alias for Track_Data */
typedef Track_Data TrackData;

/**
 * Track - Per-track checkpoint info
 * Arcade: Track struct (checkpoint.h:111-116)
 */
typedef struct Track {
    CheckPoint  *chk_point;     /* Pointer to checkpoint array */
    s16         num_checkpoints;/* Number of checkpoints */
    s16         direction;      /* Track direction (-1 or 1) */
} Track;

/* ========================================================================
 * Checkpoint system state (arcade globals from checkpoint.c)
 * ======================================================================== */

/* Core state */
extern u32 number_checkpoints;          /* Current track checkpoint count */
extern u32 last_checkpoint_time;        /* Time of last checkpoint hit */
extern s32 first_place_time;            /* Time entered first place */
extern s16 global_laps;                 /* Lead car lap count */
extern s16 global_last_cp;              /* Lead car last checkpoint */
extern s16 global_next_cp;              /* Lead car next checkpoint */
extern s16 lap_loop_index;              /* First checkpoint in loop */
extern u32 time_to_be_given;            /* Total bonus time available */
extern u32 total_time_given;            /* Total bonus time awarded */
extern u32 total_time_box_time;         /* Time box time given */

/* Additional arcade globals */
extern u32 old_last_cp_index;           /* Previous last checkpoint */
extern u32 index1, index2;              /* Temp indices */
extern s16 closest_cp_index[MAX_CPS+2]; /* Closest checkpoint indices */
extern u32 post_object[8];              /* Checkpoint object handles */
extern s8  in_first_place;              /* Currently in first? (BOOL) */
extern s8  cancel_mpath_lap;            /* Cancel maxpath lap recording */
extern f32 lap_loop_distance;           /* Distance around loop */
extern s16 close_chkpnt;                /* Close checkpoint index */
extern s16 path_point_index;            /* Current path point index */
extern s8  end_game_flag;               /* End of game flag */
extern s8  lap_flag;                    /* Lap display flag */

/* Arcade laps_to_go sound array (from checkpoint.c) */
extern const s16 laps_to_go[10];

/* Arcade track length array (laps per track) */
extern s16 track_len[];
extern s16 difficulty[];

/* Track/checkpoint arrays */
extern CheckPoint TrackCPs[MAX_TRACKS][MAX_CPS];
extern Track CP_Track[MAX_TRACKS];
extern Track_Data track_data[MAX_TRACKS];

/* Init data arrays (defined in cpinit.c or track data) */
extern InitCheckPoint *InitCP_Track[];
extern Init_Track_Data init_track_data[];

/* Path data for position calculation */
extern u16 path_start, path_end, path_loop;  /* Path array bounds */
extern s16 path_dist_index[MAX_CPS];
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

/* ========================================================================
 * Arcade display/debug functions (from checkpoint.c)
 * These are stubs on N64 - no debug display available
 * ======================================================================== */
void FPutCheckpoints(s16 track);        /* Place checkpoint objects */
u32  FPlaceOneCP(CheckPoint *chkpnt, s32 cp_objnum);

/* Debug display functions (arcade only - stubs on N64) */
void display_chkpnts(s16 mode);         /* Display checkpoint table */
void add_chkpnt(s16 mode);              /* Add checkpoint (debug) */
void del_chkpnt(s16 mode);              /* Delete checkpoint (debug) */
void fwd_chkpnt(s16 mode);              /* Forward checkpoint (debug) */
void back_chkpnt(s16 mode);             /* Back checkpoint (debug) */
void display_path_points(s16 mode);     /* Display path points */
void fwd_path_points(s16 mode);         /* Forward path points */
void back_path_points(s16 mode);        /* Back path points */

/* ========================================================================
 * Mode values for update_lap_counter (arcade: Initialize, Do_it, Cleanup)
 * ======================================================================== */
#define CP_INITIALIZE   0       /* Arcade: Initialize */
#define CP_UPDATE       1       /* Arcade: Do_it */
#define CP_CLEANUP      2       /* Arcade: Cleanup */

/* Arcade name aliases */
#define Initialize      CP_INITIALIZE
#define Do_it           CP_UPDATE
#define Cleanup         CP_CLEANUP

/* ========================================================================
 * Arcade timing constants
 * ======================================================================== */
#define ONE_SEC             60      /* Frames per second (NTSC) */
#define CHECKPT_TIME        180     /* Checkpoint timeout (3 seconds) */

#endif /* CHECKPOINT_H */
