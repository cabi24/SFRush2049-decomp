/**
 * maxpath.h - AI path following system for Rush 2049 N64
 *
 * Based on arcade game/maxpath.c
 * Handles waypoint paths that AI drones follow during racing.
 *
 * The maxpath system provides:
 * - Waypoint positions along the track
 * - Speed hints for each waypoint
 * - Track width info for passing/avoidance
 * - Multiple path options (fast/slow/alternate routes)
 */

#ifndef MAXPATH_H
#define MAXPATH_H

#include "types.h"

/* Maximum waypoints and paths */
#define MAXMPATH        2000    /* Max waypoints per track */
#define MAX_MPATHS      8       /* Max different paths per track */
#define MAX_LINKS       8       /* Max linked cars (players + drones) */

/* Hint types for waypoints */
typedef enum HintType {
    HINT_STAY_WITHIN_WIDTH,     /* Stay within track width */
    HINT_GET_ON_MPATH,          /* Get onto the maxpath */
    HINT_STAY_ON_MPATH,         /* Stay exactly on maxpath */
    HINT_STAY_PARALLEL,         /* Stay parallel to maxpath */
    HINT_RESET_PARALLEL,        /* Reset parallel tracking */
    HINT_EASE_ON_MPATH,         /* Ease onto maxpath gradually */
    NUM_HINT_TYPES
} HintType;

/* Hint names for debugging */
extern const char *gHintNames[];

/* Maxpath header structure */
typedef struct MaxPathHeader {
    s32     num_points;         /* Number of waypoints */
    s32     mpath_active;       /* Is path active? */
    s8      priority;           /* Path priority (0=highest) */
    s8      dummy;
    s16     lap_start;          /* Index of lap start point */
    s16     lap_end;            /* Index of lap end point */
    s16     num_in_lap;         /* Points in one lap */
    s32     spare[2];
} MaxPathHeader;

/* Single maxpath waypoint */
typedef struct MaxPathPoint {
    f32     pos[3];             /* World position */
    f32     speed;              /* Suggested speed (mph) */
    s32     hints;              /* Hint flags */
    s16     base_to_path;       /* Index offset to alternate path */
    s16     path_to_base;       /* Index offset back to main path */
} MaxPathPoint;

/* Drone weight factors for path following */
typedef struct MaxPathWeight {
    f32     distance_weight;    /* Weight for distance to path */
    f32     direction_weight;   /* Weight for direction alignment */
    f32     speed_weight;       /* Weight for speed matching */
    f32     own_path_weight;    /* Weight for staying on own path */
    f32     priority_weight;    /* Weight for path priority */
    f32     hint_weight;        /* Weight for following hints */
} MaxPathWeight;

/* Per-car maxpath control state */
typedef struct MaxPathControl {
    s32     car_index;          /* Which car this controls */
    s32     current_point;      /* Current waypoint index */
    s32     target_point;       /* Target waypoint index */
    s32     path_index;         /* Which path (0-7) we're on */
    f32     t;                  /* Interpolation between points (0-1) */
    f32     distance;           /* Distance traveled on path */
    f32     speed_mult;         /* Speed multiplier */
    f32     steer_target;       /* Target steering (-1 to 1) */
    f32     throttle_target;    /* Target throttle (0 to 1) */
    f32     brake_target;       /* Target brake (0 to 1) */
    s32     lap;                /* Current lap on path */
    s32     segment;            /* Current segment index */
    s32     abort_count;        /* Frames since stuck */
    u8      active;             /* Is control active? */
    u8      recording;          /* Recording new path? */
    u8      pad[2];
} MaxPathControl;

/* Global maxpath data */
extern MaxPathHeader *gMaxPath;
extern MaxPathPoint *gMPathPoints;
extern MaxPathHeader *gMPathHeaders[MAX_MPATHS];
extern MaxPathPoint *gMPathTables[MAX_MPATHS];
extern MaxPathControl gMPCtl[MAX_LINKS];
extern MaxPathWeight gMPWeight[MAX_LINKS];

/* Per-track path count */
extern s16 gNumMPaths;
extern s16 gMPathList[MAX_MPATHS];

/* Path-to-checkpoint mapping */
extern s16 gMPathToPath[MAXMPATH];
extern u16 gMPathDist[MAXMPATH];

/* Recording state */
extern s32 gMPRecording;
extern s16 gMPRecordIndex;

/* Initialization */
void maxpath_init(s32 record_mode);
void maxpath_reset(void);
void maxpath_load(s32 track_id);
void maxpath_init_car(s32 car_index);

/* Per-frame update */
void maxpath_update(s32 car_index);
void maxpath_update_all(void);

/* Path following */
void maxpath_follow(s32 car_index);
void maxpath_get_target(s32 car_index, f32 *out_pos, f32 *out_dir);
f32 maxpath_get_speed(s32 car_index);
s32 maxpath_get_hint(s32 car_index);

/* Path navigation */
s32 maxpath_find_nearest(f32 pos[3], s32 path_index);
s32 maxpath_find_next(s32 current, s32 path_index);
s32 maxpath_find_prev(s32 current, s32 path_index);
f32 maxpath_distance_to_point(s32 point_index, f32 pos[3]);

/* Path switching */
void maxpath_switch_path(s32 car_index, s32 new_path);
s32 maxpath_can_switch(s32 car_index);
s32 maxpath_get_alternate(s32 car_index);

/* Distance calculation */
f32 maxpath_calc_distance(s32 car_index);
f32 maxpath_calc_total_distance(s32 path_index);
void maxpath_sync_to_checkpoint(s32 car_index, s32 checkpoint);

/* Steering calculation */
void maxpath_calc_steering(s32 car_index, f32 *steer, f32 *throttle, f32 *brake);
f32 maxpath_steer_to_point(s32 car_index, f32 target[3]);

/* Lap handling */
void maxpath_start_lap(s32 car_index);
void maxpath_end_lap(s32 car_index);
s32 maxpath_get_lap(s32 car_index);

/* Recording (for creating new paths) */
void maxpath_start_record(s32 path_index);
void maxpath_stop_record(void);
void maxpath_record_point(f32 pos[3], f32 speed, s32 hints);
void maxpath_save_to_file(const char *filename);

/* Debug/utility */
void maxpath_draw_debug(s32 path_index);
const char* maxpath_get_hint_name(s32 hint);
s32 maxpath_validate(s32 path_index);

/* Weight table access */
void maxpath_set_weights(s32 car_index, MaxPathWeight *weights);
void maxpath_get_weights(s32 car_index, MaxPathWeight *weights);

/* Interpolation helpers */
void maxpath_interpolate_pos(s32 p1, s32 p2, f32 t, f32 *out);
void maxpath_interpolate_dir(s32 p1, s32 p2, f32 t, f32 *out);
f32 maxpath_interpolate_speed(s32 p1, s32 p2, f32 t);

#endif /* MAXPATH_H */
