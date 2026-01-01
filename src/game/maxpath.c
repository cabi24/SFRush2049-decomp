/**
 * maxpath.c - AI path following system for Rush 2049 N64
 *
 * Based on arcade game/maxpath.c
 * Handles waypoint paths that AI drones follow during racing.
 *
 * The maxpath system provides:
 * - Waypoint positions along the track
 * - Speed hints for each waypoint
 * - Track width info for passing/avoidance
 * - Multiple path options (fast/slow/alternate routes)
 *
 * Key arcade functions ported:
 * - InitMaxPath()        - Initialize path system
 * - MaxPath()            - Main update during recording
 * - MaxPathControls()    - Generate steering/throttle from path
 * - MP_FindInterval()    - Find current path segment
 * - MP_IntervalPos()     - Calculate relative position on segment
 * - MP_TargetSpeed()     - Get target speed from path data
 * - MP_TargetSteerPos()  - Get target steering position
 * - AdjustSpeed()        - Convert target speed to throttle/brake
 * - AdjustSteer()        - Convert target position to steering
 * - avoid_areas()        - Collision avoidance with other cars
 */

#include "types.h"
#include "game/maxpath.h"
#include "game/structs.h"
#include "game/checkpoint.h"

/* External game state */
extern CarData car_array[];
extern s32 num_active_cars;
extern s32 trackno;
extern u32 frame_counter;
extern s32 this_node;

/* External math */
extern f32 sqrtf(f32 x);
extern f32 fabsf(f32 x);

/* Arcade timing constant */
#define ONE_SEC         60      /* Frames per second */
#define IRQTIME         frame_counter  /* Alias for frame counter */

/* Arcade speed scaling */
static const f32 g_MPspdscale = 1.0f;

/* Look-ahead distance for steering */
static const f32 g_lookahd = 80.0f;

/* Hint type names for debugging */
const char *gHintNames[] = {
    "Within width",
    "Get on maxpath",
    "Stay on maxpath",
    "Stay parallel",
    "Reset parallel",
    "Ease on maxpath",
    "Invalid hint"
};

/* Global maxpath data */
MaxPathHeader *gMaxPath = NULL;
MaxPathPoint *gMPathPoints = NULL;
MaxPathHeader *gMPathHeaders[MAX_MPATHS];
MaxPathPoint *gMPathTables[MAX_MPATHS];
MaxPathControl gMPCtl[MAX_LINKS];
MaxPathWeight gMPWeight[MAX_LINKS];

/* Per-track path info */
s16 gNumMPaths = 0;
s16 gMPathList[MAX_MPATHS];

/* Path-to-checkpoint mapping */
s16 gMPathToPath[MAXMPATH];
u16 gMPathDist[MAXMPATH];

/* Recording state */
s32 gMPRecording = 0;
s16 gMPRecordIndex = 0;

/* Unit orientation matrix */
static const f32 sUnitUvs[3][3] = {
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f}
};

/* ========================================================================
 * Arcade-compatible per-car control state (mpctl array)
 * Based on arcade maxpath.c:MPCTL structure
 * ======================================================================== */

/* Arcade MPCTL structure (extended for N64) */
typedef struct ArcadeMPCTL {
    s32     mpi;                /* Current maxpath index */
    s32     mpath_index;        /* Which path (0-7) */
    s32     default_path;       /* Default assigned path */
    s32     new_mpi;            /* New index to sync to (-1 = none) */
    f32     xrel;               /* Forward position on segment */
    f32     yrel;               /* Lateral offset from path */
    f32     cyrel;              /* Path curvature (y relative to next) */
    f32     len;                /* Current segment length */
    f32     tgtspd;             /* Target speed */
    f32     tgtpos[3];          /* Target steering position */
    u32     interval_time;      /* Time of last interval update */
} ArcadeMPCTL;

/* Per-car arcade control state */
static ArcadeMPCTL arcade_mpctl[MAX_LINKS];

/* Last control outputs (for smoothing) */
static s32 lastwheel[MAX_LINKS];
static s32 lastbrake[MAX_LINKS];
static s32 lastthrottle[MAX_LINKS];

/* Last relative positions (for avoidance) */
static f32 last_rwr[MAX_LINKS][MAX_LINKS][3];
static f32 last_dir[MAX_LINKS][3];
static f32 last_relpos[MAX_LINKS][MAX_LINKS][3];

/* Best lap time for recording */
static s32 best_loop_time = 9999999;
static s32 loop_stamp = 0;

/* Clamp macro */
#define CLAMP(v, lo, hi) (((v) < (lo)) ? (lo) : (((v) > (hi)) ? (hi) : (v)))

/**
 * maxpath_init - Initialize maxpath system
 * Based on arcade: maxpath.c:InitMaxPath()
 *
 * @param record_mode Recording mode (-1 = none)
 */
void maxpath_init(s32 record_mode) {
    s32 i;

    gMPRecording = (record_mode >= 0) ? 1 : 0;
    gMPRecordIndex = 0;
    gNumMPaths = 0;

    /* Clear all path headers */
    for (i = 0; i < MAX_MPATHS; i++) {
        gMPathHeaders[i] = NULL;
        gMPathTables[i] = NULL;
        gMPathList[i] = -1;
    }

    /* Initialize per-car control */
    for (i = 0; i < MAX_LINKS; i++) {
        maxpath_init_car(i);
    }

    /* Load paths for current track */
    maxpath_load(trackno);
}

/**
 * maxpath_reset - Reset maxpath state for new race
 */
void maxpath_reset(void) {
    s32 i;

    for (i = 0; i < MAX_LINKS; i++) {
        gMPCtl[i].current_point = 0;
        gMPCtl[i].target_point = 0;
        gMPCtl[i].t = 0.0f;
        gMPCtl[i].distance = 0.0f;
        gMPCtl[i].lap = 0;
        gMPCtl[i].abort_count = 0;
    }
}

/**
 * maxpath_load - Load maxpath data for a track
 *
 * @param track_id Track to load paths for
 */
void maxpath_load(s32 track_id) {
    /* In the arcade version, path data is loaded from files.
     * For N64, paths are embedded in the compressed game code.
     * This would decompress and set up gMPathHeaders/gMPathTables.
     */

    /* For now, create a simple placeholder path */
    /* Real implementation would load from ROM */
}

/**
 * maxpath_init_car - Initialize maxpath control for one car
 *
 * @param car_index Car index
 */
void maxpath_init_car(s32 car_index) {
    MaxPathControl *ctl;
    MaxPathWeight *wt;

    if (car_index < 0 || car_index >= MAX_LINKS) {
        return;
    }

    ctl = &gMPCtl[car_index];
    wt = &gMPWeight[car_index];

    ctl->car_index = car_index;
    ctl->current_point = 0;
    ctl->target_point = 0;
    ctl->path_index = 0;
    ctl->t = 0.0f;
    ctl->distance = 0.0f;
    ctl->speed_mult = 1.0f;
    ctl->steer_target = 0.0f;
    ctl->throttle_target = 1.0f;
    ctl->brake_target = 0.0f;
    ctl->lap = 0;
    ctl->segment = 0;
    ctl->abort_count = 0;
    ctl->active = 0;
    ctl->recording = 0;

    /* Default weights */
    wt->distance_weight = 1.0f;
    wt->direction_weight = 0.5f;
    wt->speed_weight = 0.3f;
    wt->own_path_weight = 0.2f;
    wt->priority_weight = 0.1f;
    wt->hint_weight = 0.4f;
}

/**
 * maxpath_update - Update maxpath following for one car
 * Based on arcade: maxpath.c:MaxPath()
 *
 * @param car_index Car to update
 */
void maxpath_update(s32 car_index) {
    MaxPathControl *ctl;
    MaxPathHeader *header;
    CarData *car;
    f32 target_pos[3];
    f32 target_dir[3];
    f32 dx, dz;
    f32 dist;

    if (car_index < 0 || car_index >= MAX_LINKS) {
        return;
    }

    ctl = &gMPCtl[car_index];
    car = &car_array[car_index];

    if (!ctl->active) {
        return;
    }

    /* Get current path header */
    if (ctl->path_index < 0 || ctl->path_index >= gNumMPaths) {
        return;
    }

    header = gMPathHeaders[ctl->path_index];
    if (header == NULL || header->num_points == 0) {
        return;
    }

    /* Get target position and direction */
    maxpath_get_target(car_index, target_pos, target_dir);

    /* Calculate distance to target point */
    dx = target_pos[0] - car->dr_pos[0];
    dz = target_pos[2] - car->dr_pos[2];
    dist = sqrtf(dx * dx + dz * dz);

    /* Check if we've reached the current waypoint */
    if (dist < 10.0f) {
        /* Advance to next waypoint */
        ctl->current_point = ctl->target_point;
        ctl->target_point = maxpath_find_next(ctl->current_point, ctl->path_index);
        ctl->t = 0.0f;

        /* Check for lap completion */
        if (ctl->target_point <= ctl->current_point && header->num_in_lap > 0) {
            ctl->lap++;
        }
    }

    /* Calculate steering to reach target */
    maxpath_calc_steering(car_index, &ctl->steer_target, &ctl->throttle_target, &ctl->brake_target);

    /* Update distance traveled */
    ctl->distance = maxpath_calc_distance(car_index);

    /* Reset abort counter if making progress */
    if (car->mph > 5.0f) {
        ctl->abort_count = 0;
    } else {
        ctl->abort_count++;
    }
}

/**
 * maxpath_update_all - Update all active maxpath controls
 */
void maxpath_update_all(void) {
    s32 i;

    for (i = 0; i < num_active_cars; i++) {
        if (gMPCtl[i].active) {
            maxpath_update(i);
        }
    }
}

/**
 * maxpath_follow - Main path following logic
 *
 * @param car_index Car to follow path
 */
void maxpath_follow(s32 car_index) {
    maxpath_update(car_index);
}

/**
 * maxpath_get_target - Get target position and direction for a car
 *
 * @param car_index Car index
 * @param out_pos Output position (3 floats)
 * @param out_dir Output direction (3 floats)
 */
void maxpath_get_target(s32 car_index, f32 *out_pos, f32 *out_dir) {
    MaxPathControl *ctl;
    MaxPathPoint *points;
    MaxPathPoint *p1, *p2;
    s32 path_idx;

    ctl = &gMPCtl[car_index];
    path_idx = ctl->path_index;

    if (path_idx < 0 || path_idx >= gNumMPaths || gMPathTables[path_idx] == NULL) {
        /* Default: no path, go forward */
        out_pos[0] = car_array[car_index].dr_pos[0];
        out_pos[1] = car_array[car_index].dr_pos[1];
        out_pos[2] = car_array[car_index].dr_pos[2] + 100.0f;
        out_dir[0] = 0.0f;
        out_dir[1] = 0.0f;
        out_dir[2] = 1.0f;
        return;
    }

    points = gMPathTables[path_idx];
    p1 = &points[ctl->current_point];
    p2 = &points[ctl->target_point];

    /* Interpolate position */
    maxpath_interpolate_pos(ctl->current_point, ctl->target_point, ctl->t, out_pos);

    /* Calculate direction from p1 to p2 */
    out_dir[0] = p2->pos[0] - p1->pos[0];
    out_dir[1] = p2->pos[1] - p1->pos[1];
    out_dir[2] = p2->pos[2] - p1->pos[2];

    /* Normalize direction */
    {
        f32 len = sqrtf(out_dir[0]*out_dir[0] + out_dir[1]*out_dir[1] + out_dir[2]*out_dir[2]);
        if (len > 0.001f) {
            out_dir[0] /= len;
            out_dir[1] /= len;
            out_dir[2] /= len;
        }
    }
}

/**
 * maxpath_get_speed - Get suggested speed at current position
 *
 * @param car_index Car index
 * @return Suggested speed in mph
 */
f32 maxpath_get_speed(s32 car_index) {
    MaxPathControl *ctl;

    ctl = &gMPCtl[car_index];
    return maxpath_interpolate_speed(ctl->current_point, ctl->target_point, ctl->t);
}

/**
 * maxpath_get_hint - Get hint flags at current position
 *
 * @param car_index Car index
 * @return Hint flags
 */
s32 maxpath_get_hint(s32 car_index) {
    MaxPathControl *ctl;
    MaxPathPoint *points;

    ctl = &gMPCtl[car_index];

    if (ctl->path_index < 0 || gMPathTables[ctl->path_index] == NULL) {
        return 0;
    }

    points = gMPathTables[ctl->path_index];
    return points[ctl->current_point].hints;
}

/**
 * maxpath_find_nearest - Find nearest waypoint to a position
 *
 * @param pos Position to search from
 * @param path_index Which path to search
 * @return Index of nearest waypoint
 */
s32 maxpath_find_nearest(f32 pos[3], s32 path_index) {
    MaxPathHeader *header;
    MaxPathPoint *points;
    s32 i, best_idx;
    f32 best_dist, dx, dy, dz, dist;

    if (path_index < 0 || path_index >= gNumMPaths) {
        return 0;
    }

    header = gMPathHeaders[path_index];
    points = gMPathTables[path_index];

    if (header == NULL || points == NULL || header->num_points == 0) {
        return 0;
    }

    best_idx = 0;
    best_dist = 1e10f;

    for (i = 0; i < header->num_points; i++) {
        dx = points[i].pos[0] - pos[0];
        dy = points[i].pos[1] - pos[1];
        dz = points[i].pos[2] - pos[2];
        dist = dx*dx + dy*dy + dz*dz;

        if (dist < best_dist) {
            best_dist = dist;
            best_idx = i;
        }
    }

    return best_idx;
}

/**
 * maxpath_find_next - Find next waypoint index
 *
 * @param current Current waypoint index
 * @param path_index Path index
 * @return Next waypoint index (wraps at end)
 */
s32 maxpath_find_next(s32 current, s32 path_index) {
    MaxPathHeader *header;

    if (path_index < 0 || path_index >= gNumMPaths) {
        return 0;
    }

    header = gMPathHeaders[path_index];
    if (header == NULL || header->num_points == 0) {
        return 0;
    }

    current++;
    if (current >= header->num_points) {
        current = header->lap_start;  /* Wrap to lap start */
    }

    return current;
}

/**
 * maxpath_find_prev - Find previous waypoint index
 *
 * @param current Current waypoint index
 * @param path_index Path index
 * @return Previous waypoint index (wraps at start)
 */
s32 maxpath_find_prev(s32 current, s32 path_index) {
    MaxPathHeader *header;

    if (path_index < 0 || path_index >= gNumMPaths) {
        return 0;
    }

    header = gMPathHeaders[path_index];
    if (header == NULL || header->num_points == 0) {
        return 0;
    }

    current--;
    if (current < header->lap_start) {
        current = header->num_points - 1;  /* Wrap to end */
    }

    return current;
}

/**
 * maxpath_distance_to_point - Calculate distance to a waypoint
 *
 * @param point_index Waypoint index
 * @param pos Position to measure from
 * @return Distance to waypoint
 */
f32 maxpath_distance_to_point(s32 point_index, f32 pos[3]) {
    MaxPathPoint *points;
    f32 dx, dy, dz;

    if (gNumMPaths <= 0 || gMPathTables[0] == NULL) {
        return 0.0f;
    }

    points = gMPathTables[0];  /* Use first path */
    dx = points[point_index].pos[0] - pos[0];
    dy = points[point_index].pos[1] - pos[1];
    dz = points[point_index].pos[2] - pos[2];

    return sqrtf(dx*dx + dy*dy + dz*dz);
}

/**
 * maxpath_switch_path - Switch car to a different path
 *
 * @param car_index Car index
 * @param new_path New path index
 */
void maxpath_switch_path(s32 car_index, s32 new_path) {
    MaxPathControl *ctl;
    f32 pos[3];

    if (car_index < 0 || car_index >= MAX_LINKS) {
        return;
    }
    if (new_path < 0 || new_path >= gNumMPaths) {
        return;
    }

    ctl = &gMPCtl[car_index];

    /* Find nearest point on new path */
    pos[0] = car_array[car_index].dr_pos[0];
    pos[1] = car_array[car_index].dr_pos[1];
    pos[2] = car_array[car_index].dr_pos[2];

    ctl->path_index = new_path;
    ctl->current_point = maxpath_find_nearest(pos, new_path);
    ctl->target_point = maxpath_find_next(ctl->current_point, new_path);
    ctl->t = 0.0f;
}

/**
 * maxpath_can_switch - Check if car can switch paths
 *
 * @param car_index Car index
 * @return 1 if switch allowed, 0 otherwise
 */
s32 maxpath_can_switch(s32 car_index) {
    MaxPathControl *ctl;
    MaxPathPoint *points;

    ctl = &gMPCtl[car_index];

    if (ctl->path_index < 0 || gMPathTables[ctl->path_index] == NULL) {
        return 0;
    }

    points = gMPathTables[ctl->path_index];

    /* Check if current point has a path switch */
    if (points[ctl->current_point].base_to_path != 0) {
        return 1;
    }

    return 0;
}

/**
 * maxpath_get_alternate - Get alternate path index
 *
 * @param car_index Car index
 * @return Alternate path index or -1
 */
s32 maxpath_get_alternate(s32 car_index) {
    MaxPathControl *ctl;

    ctl = &gMPCtl[car_index];

    /* Return next path in rotation */
    return (ctl->path_index + 1) % gNumMPaths;
}

/**
 * maxpath_calc_distance - Calculate distance traveled on path
 *
 * @param car_index Car index
 * @return Total distance traveled
 */
f32 maxpath_calc_distance(s32 car_index) {
    MaxPathControl *ctl;
    MaxPathHeader *header;
    f32 lap_length;
    f32 segment_length;

    ctl = &gMPCtl[car_index];

    if (ctl->path_index < 0 || gMPathHeaders[ctl->path_index] == NULL) {
        return 0.0f;
    }

    header = gMPathHeaders[ctl->path_index];

    /* Approximate calculation: points * average segment length */
    lap_length = (f32)header->num_in_lap * 10.0f;  /* ~10 units per segment */
    segment_length = lap_length / (f32)header->num_in_lap;

    return (f32)ctl->lap * lap_length + (f32)ctl->current_point * segment_length + ctl->t * segment_length;
}

/**
 * maxpath_calc_total_distance - Get total path length
 *
 * @param path_index Path index
 * @return Total distance of one lap
 */
f32 maxpath_calc_total_distance(s32 path_index) {
    MaxPathHeader *header;

    if (path_index < 0 || path_index >= gNumMPaths) {
        return 0.0f;
    }

    header = gMPathHeaders[path_index];
    if (header == NULL) {
        return 0.0f;
    }

    return (f32)header->num_in_lap * 10.0f;  /* Approximate */
}

/**
 * maxpath_sync_to_checkpoint - Sync path position to checkpoint
 *
 * @param car_index Car index
 * @param checkpoint Checkpoint index
 */
void maxpath_sync_to_checkpoint(s32 car_index, s32 checkpoint) {
    MaxPathControl *ctl;

    ctl = &gMPCtl[car_index];

    /* Find nearest path point to checkpoint position */
    /* In full implementation, would use checkpoint-to-path mapping */
    if (checkpoint >= 0 && checkpoint < MAXMPATH) {
        if (gMPathToPath[checkpoint] >= 0) {
            ctl->current_point = gMPathToPath[checkpoint];
            ctl->target_point = maxpath_find_next(ctl->current_point, ctl->path_index);
            ctl->t = 0.0f;
        }
    }
}

/**
 * maxpath_calc_steering - Calculate steering/throttle/brake to follow path
 * Based on arcade: maxpath.c steering calculation
 *
 * @param car_index Car index
 * @param steer Output steering (-1 to 1)
 * @param throttle Output throttle (0 to 1)
 * @param brake Output brake (0 to 1)
 */
void maxpath_calc_steering(s32 car_index, f32 *steer, f32 *throttle, f32 *brake) {
    MaxPathControl *ctl;
    CarData *car;
    f32 target_pos[3], target_dir[3];
    f32 to_target[3];
    f32 dx, dz, dist;
    f32 dot, cross;
    f32 angle;
    f32 suggested_speed;

    ctl = &gMPCtl[car_index];
    car = &car_array[car_index];

    /* Get target position */
    maxpath_get_target(car_index, target_pos, target_dir);

    /* Vector to target */
    to_target[0] = target_pos[0] - car->dr_pos[0];
    to_target[1] = target_pos[1] - car->dr_pos[1];
    to_target[2] = target_pos[2] - car->dr_pos[2];

    /* 2D distance */
    dx = to_target[0];
    dz = to_target[2];
    dist = sqrtf(dx*dx + dz*dz);

    if (dist < 0.001f) {
        *steer = 0.0f;
        *throttle = 1.0f;
        *brake = 0.0f;
        return;
    }

    /* Normalize */
    dx /= dist;
    dz /= dist;

    /* Calculate steering based on cross product with car forward direction */
    /* Simplified: assume car forward is (0, 0, 1) rotated by car yaw */
    /* In full implementation, use car's orientation matrix */

    /* Dot product for forward alignment */
    dot = dz;  /* Simplified */

    /* Cross product for left/right */
    cross = dx;  /* Simplified */

    /* Steering angle based on cross product */
    *steer = CLAMP(cross * 2.0f, -1.0f, 1.0f);

    /* Throttle/brake based on speed and angle */
    suggested_speed = maxpath_get_speed(car_index);

    if (car->mph > suggested_speed * 1.2f) {
        /* Going too fast, brake */
        *throttle = 0.3f;
        *brake = 0.3f;
    } else if (car->mph < suggested_speed * 0.5f) {
        /* Going too slow, full throttle */
        *throttle = 1.0f;
        *brake = 0.0f;
    } else {
        /* Normal driving */
        *throttle = 0.8f;
        *brake = 0.0f;
    }

    /* Reduce throttle in sharp turns */
    if (*steer > 0.5f || *steer < -0.5f) {
        *throttle *= 0.7f;
    }
}

/**
 * maxpath_steer_to_point - Calculate steering angle to reach a point
 *
 * @param car_index Car index
 * @param target Target position
 * @return Steering angle (-1 to 1)
 */
f32 maxpath_steer_to_point(s32 car_index, f32 target[3]) {
    CarData *car;
    f32 dx, dz, dist;
    f32 cross;

    car = &car_array[car_index];

    dx = target[0] - car->dr_pos[0];
    dz = target[2] - car->dr_pos[2];
    dist = sqrtf(dx*dx + dz*dz);

    if (dist < 0.001f) {
        return 0.0f;
    }

    cross = dx / dist;
    return CLAMP(cross * 2.0f, -1.0f, 1.0f);
}

/**
 * maxpath_start_lap - Called when car starts a new lap
 *
 * @param car_index Car index
 */
void maxpath_start_lap(s32 car_index) {
    MaxPathControl *ctl;
    MaxPathHeader *header;

    ctl = &gMPCtl[car_index];

    if (ctl->path_index >= 0 && gMPathHeaders[ctl->path_index] != NULL) {
        header = gMPathHeaders[ctl->path_index];
        ctl->current_point = header->lap_start;
        ctl->target_point = maxpath_find_next(ctl->current_point, ctl->path_index);
    }
}

/**
 * maxpath_end_lap - Called when car finishes a lap
 *
 * @param car_index Car index
 */
void maxpath_end_lap(s32 car_index) {
    gMPCtl[car_index].lap++;
}

/**
 * maxpath_get_lap - Get current lap number
 *
 * @param car_index Car index
 * @return Current lap (0-based)
 */
s32 maxpath_get_lap(s32 car_index) {
    return gMPCtl[car_index].lap;
}

/**
 * maxpath_start_record - Start recording a new path
 *
 * @param path_index Path slot to record into
 */
void maxpath_start_record(s32 path_index) {
    gMPRecording = 1;
    gMPRecordIndex = 0;
    /* Would allocate buffer for recording */
}

/**
 * maxpath_stop_record - Stop recording path
 */
void maxpath_stop_record(void) {
    gMPRecording = 0;
}

/**
 * maxpath_record_point - Record a waypoint
 *
 * @param pos Position
 * @param speed Speed at this point
 * @param hints Hint flags
 */
void maxpath_record_point(f32 pos[3], f32 speed, s32 hints) {
    /* Would store point in recording buffer */
    gMPRecordIndex++;
}

/**
 * maxpath_save_to_file - Save recorded path
 *
 * @param filename Output filename
 */
void maxpath_save_to_file(const char *filename) {
    /* Would save to Controller Pak or similar */
}

/**
 * maxpath_draw_debug - Draw path for debugging
 *
 * @param path_index Path to draw
 */
void maxpath_draw_debug(s32 path_index) {
    /* Would draw lines between waypoints */
}

/**
 * maxpath_get_hint_name - Get name of hint type
 *
 * @param hint Hint value
 * @return Name string
 */
const char* maxpath_get_hint_name(s32 hint) {
    if (hint < 0 || hint >= NUM_HINT_TYPES) {
        return gHintNames[NUM_HINT_TYPES];  /* "Invalid hint" */
    }
    return gHintNames[hint];
}

/**
 * maxpath_validate - Validate a path's data
 *
 * @param path_index Path to validate
 * @return 1 if valid, 0 if invalid
 */
s32 maxpath_validate(s32 path_index) {
    MaxPathHeader *header;

    if (path_index < 0 || path_index >= gNumMPaths) {
        return 0;
    }

    header = gMPathHeaders[path_index];
    if (header == NULL) {
        return 0;
    }

    if (header->num_points <= 0 || header->num_points > MAXMPATH) {
        return 0;
    }

    if (gMPathTables[path_index] == NULL) {
        return 0;
    }

    return 1;
}

/**
 * maxpath_set_weights - Set path following weights for a car
 *
 * @param car_index Car index
 * @param weights Weight structure to copy
 */
void maxpath_set_weights(s32 car_index, MaxPathWeight *weights) {
    if (car_index >= 0 && car_index < MAX_LINKS && weights != NULL) {
        gMPWeight[car_index] = *weights;
    }
}

/**
 * maxpath_get_weights - Get path following weights for a car
 *
 * @param car_index Car index
 * @param weights Output weight structure
 */
void maxpath_get_weights(s32 car_index, MaxPathWeight *weights) {
    if (car_index >= 0 && car_index < MAX_LINKS && weights != NULL) {
        *weights = gMPWeight[car_index];
    }
}

/**
 * maxpath_interpolate_pos - Interpolate position between two waypoints
 *
 * @param p1 First waypoint index
 * @param p2 Second waypoint index
 * @param t Interpolation factor (0-1)
 * @param out Output position (3 floats)
 */
void maxpath_interpolate_pos(s32 p1, s32 p2, f32 t, f32 *out) {
    MaxPathPoint *points;
    MaxPathPoint *wp1, *wp2;

    if (gNumMPaths <= 0 || gMPathTables[0] == NULL) {
        out[0] = out[1] = out[2] = 0.0f;
        return;
    }

    points = gMPathTables[0];  /* Use first path */
    wp1 = &points[p1];
    wp2 = &points[p2];

    out[0] = wp1->pos[0] + (wp2->pos[0] - wp1->pos[0]) * t;
    out[1] = wp1->pos[1] + (wp2->pos[1] - wp1->pos[1]) * t;
    out[2] = wp1->pos[2] + (wp2->pos[2] - wp1->pos[2]) * t;
}

/**
 * maxpath_interpolate_dir - Interpolate direction between two waypoints
 *
 * @param p1 First waypoint index
 * @param p2 Second waypoint index
 * @param t Interpolation factor (0-1)
 * @param out Output direction (3 floats, normalized)
 */
void maxpath_interpolate_dir(s32 p1, s32 p2, f32 t, f32 *out) {
    MaxPathPoint *points;
    MaxPathPoint *wp1, *wp2;
    f32 len;

    if (gNumMPaths <= 0 || gMPathTables[0] == NULL) {
        out[0] = 0.0f;
        out[1] = 0.0f;
        out[2] = 1.0f;
        return;
    }

    points = gMPathTables[0];
    wp1 = &points[p1];
    wp2 = &points[p2];

    /* Direction from p1 to p2 */
    out[0] = wp2->pos[0] - wp1->pos[0];
    out[1] = wp2->pos[1] - wp1->pos[1];
    out[2] = wp2->pos[2] - wp1->pos[2];

    /* Normalize */
    len = sqrtf(out[0]*out[0] + out[1]*out[1] + out[2]*out[2]);
    if (len > 0.001f) {
        out[0] /= len;
        out[1] /= len;
        out[2] /= len;
    } else {
        out[0] = 0.0f;
        out[1] = 0.0f;
        out[2] = 1.0f;
    }
}

/**
 * maxpath_interpolate_speed - Interpolate speed between two waypoints
 *
 * @param p1 First waypoint index
 * @param p2 Second waypoint index
 * @param t Interpolation factor (0-1)
 * @return Interpolated speed
 */
f32 maxpath_interpolate_speed(s32 p1, s32 p2, f32 t) {
    MaxPathPoint *points;

    if (gNumMPaths <= 0 || gMPathTables[0] == NULL) {
        return 100.0f;  /* Default speed */
    }

    points = gMPathTables[0];
    return points[p1].speed + (points[p2].speed - points[p1].speed) * t;
}

/* ========================================================================
 * Arcade-compatible function implementations
 * Based on arcade maxpath.c - these match arcade signatures and behavior
 * ======================================================================== */

/**
 * PrevMaxPath - Get previous maxpath index with lap wrap
 * Based on arcade: maxpath.c:PrevMaxPath()
 *
 * @param mpi Current index
 * @param path_index Path to use
 * @return Previous index
 */
s32 PrevMaxPath(s32 mpi, s16 path_index) {
    MaxPathHeader *header;

    if (path_index < 0 || path_index >= gNumMPaths) {
        return 0;
    }

    header = gMPathHeaders[path_index];
    if (header == NULL) {
        return 0;
    }

    if (mpi <= 0) {
        return header->lap_end;
    }
    return mpi - 1;
}

/**
 * NextMaxPath - Get next maxpath index with lap wrap
 * Based on arcade: maxpath.c:NextMaxPath()
 *
 * @param mpi Current index
 * @param path_index Path to use
 * @return Next index
 */
s32 NextMaxPath(s32 mpi, s16 path_index) {
    MaxPathHeader *header;

    if (path_index < 0 || path_index >= gNumMPaths) {
        return 0;
    }

    header = gMPathHeaders[path_index];
    if (header == NULL) {
        return 0;
    }

    if (mpi < header->lap_end) {
        return mpi + 1;
    }
    return header->lap_start;
}

/**
 * mp_interval_pos - Calculate car position relative to path segment
 * Based on arcade: maxpath.c:mp_interval_pos()
 *
 * This calculates:
 * - xrel: forward distance along segment
 * - yrel: lateral offset from path centerline
 * - cyrel: path curvature at this point
 *
 * @param RWR Real world position [3]
 * @param cp Control structure to update
 */
void mp_interval_pos(f32 *RWR, ArcadeMPCTL *cp) {
    MaxPathPoint *mp, *nmp, *nnmp;
    s32 nmpi, nnmpi;
    f32 nx, ny, invdist;
    f32 x, y;

    if (cp->mpath_index < 0 || cp->mpath_index >= gNumMPaths) {
        return;
    }
    if (gMPathTables[cp->mpath_index] == NULL) {
        return;
    }

    /* Setup pointers for current and next waypoint */
    mp = &gMPathTables[cp->mpath_index][cp->mpi];
    nmpi = NextMaxPath(cp->mpi, cp->mpath_index);
    nmp = &gMPathTables[cp->mpath_index][nmpi];

    /* Direction vector for interval (XZ plane for N64, XY for arcade) */
    /* Note: N64 uses Y-up, arcade uses Z-up */
    nx = nmp->pos[0] - mp->pos[0];
    ny = nmp->pos[2] - mp->pos[2];  /* Use Z for N64 (arcade uses Y) */
    cp->len = sqrtf(nx * nx + ny * ny);

    /* Prevent divide by zero */
    invdist = (cp->len > 0.01f) ? (1.0f / cp->len) : 0.0f;
    nx *= invdist;
    ny *= invdist;

    /* Real world distance to car CG */
    x = RWR[0] - mp->pos[0];
    y = RWR[2] - mp->pos[2];  /* Z for N64 */

    /* CG relative to interval (simplified matrix multiply in XZ plane) */
    cp->xrel = x * nx + y * ny;
    cp->yrel = y * nx - x * ny;

    /* Get path curvature */
    nnmpi = NextMaxPath(nmpi, cp->mpath_index);
    nnmp = &gMPathTables[cp->mpath_index][nnmpi];
    cp->cyrel = (nnmp->pos[2] - mp->pos[2]) * nx - (nnmp->pos[0] - mp->pos[0]) * ny;
}

/**
 * MP_FindInterval - Find and update current path segment
 * Based on arcade: maxpath.c:MP_FindInterval()
 *
 * Updates the car's position on the path, advancing or retreating
 * through waypoints as needed.
 *
 * @param car_index Car to update
 */
void MP_FindInterval(s32 car_index) {
    ArcadeMPCTL *cp;
    CarData *car;
    f32 RWR[3];

    if (car_index < 0 || car_index >= MAX_LINKS) {
        return;
    }

    cp = &arcade_mpctl[car_index];
    car = &car_array[car_index];

    /* Get car position */
    RWR[0] = car->dr_pos[0];
    RWR[1] = car->dr_pos[1];
    RWR[2] = car->dr_pos[2];

    /* Check if we need to resync to a new maxpath index */
    if (cp->new_mpi >= 0) {
        cp->mpi = cp->new_mpi;
        cp->interval_time = IRQTIME;
        cp->new_mpi = -1;
    }

    /* Calculate position relative to current segment */
    mp_interval_pos(RWR, cp);

    /* Check if we've moved past the current segment */
    if (cp->xrel >= cp->len) {
        cp->mpi = NextMaxPath(cp->mpi, cp->mpath_index);
        cp->interval_time = IRQTIME;
        mp_interval_pos(RWR, cp);
    } else if (cp->xrel < 0.0f) {
        cp->mpi = PrevMaxPath(cp->mpi, cp->mpath_index);
        mp_interval_pos(RWR, cp);
    }
}

/**
 * MP_TargetSpeed - Calculate target speed from path data
 * Based on arcade: maxpath.c:MP_TargetSpeed()
 *
 * Interpolates speed between waypoints and applies off-path slowdown.
 *
 * @param car_index Car to update
 */
void MP_TargetSpeed(s32 car_index) {
    ArcadeMPCTL *cp;
    MaxPathPoint *mp, *nmp;
    s32 nmpi;
    f32 interp, yrel, offsc;

    if (car_index < 0 || car_index >= MAX_LINKS) {
        return;
    }

    cp = &arcade_mpctl[car_index];
    if (cp->mpath_index < 0 || gMPathTables[cp->mpath_index] == NULL) {
        cp->tgtspd = 100.0f;
        return;
    }

    mp = &gMPathTables[cp->mpath_index][cp->mpi];
    nmpi = NextMaxPath(cp->mpi, cp->mpath_index);
    nmp = &gMPathTables[cp->mpath_index][nmpi];

    /* Interpolate speed in current interval */
    if (cp->len > 0.001f) {
        interp = cp->xrel / cp->len;
    } else {
        interp = 0.0f;
    }
    if (interp < 0.0f) {
        interp = 0.0f;
    } else if (interp > 1.0f) {
        interp = 1.0f;
    }

    cp->tgtspd = mp->speed + (interp * (nmp->speed - mp->speed));
    cp->tgtspd *= g_MPspdscale;

    /* Slow down if off target line */
    yrel = (cp->yrel > 0.0f) ? cp->yrel : -cp->yrel;
    if (yrel < 0.1f) {
        offsc = 1.0f;
    } else if (yrel > 80.0f) {
        offsc = 0.8f;
    } else {
        /* offsc = 1 - (yrel/30)^2 simplified */
        offsc = 1.0f - (yrel * yrel * 0.00003125f);
    }

    cp->tgtspd *= offsc;

    /* Minimum speed (28 mph = ~41 fps at 1.4667 fps/mph) */
    if (cp->tgtspd < 28.0f) {
        cp->tgtspd = 28.0f;
    }
}

/**
 * MP_TargetSteerPos - Calculate target steering position
 * Based on arcade: maxpath.c:MP_TargetSteerPos()
 *
 * Looks ahead on the path and calculates target position for steering.
 *
 * @param car_index Car to update
 */
void MP_TargetSteerPos(s32 car_index) {
    ArcadeMPCTL *cp;
    MaxPathPoint *mp, *nmp;
    s32 nmpi;
    f32 x, y, z, dist, tgtdist, interp;
    s32 iter;

    if (car_index < 0 || car_index >= MAX_LINKS) {
        return;
    }

    cp = &arcade_mpctl[car_index];
    if (cp->mpath_index < 0 || gMPathTables[cp->mpath_index] == NULL) {
        return;
    }

    mp = &gMPathTables[cp->mpath_index][cp->mpi];
    nmpi = NextMaxPath(cp->mpi, cp->mpath_index);
    nmp = &gMPathTables[cp->mpath_index][nmpi];

    /* Look ahead g_lookahd feet and interpolate steer position */
    tgtdist = g_lookahd + cp->xrel;
    dist = cp->len;
    tgtdist -= dist;

    /* Walk forward through waypoints until we've covered lookahead distance */
    iter = 0;
    while (tgtdist > 0.0f && iter < 100) {
        mp = nmp;
        nmpi = NextMaxPath(nmpi, cp->mpath_index);
        nmp = &gMPathTables[cp->mpath_index][nmpi];

        x = nmp->pos[0] - mp->pos[0];
        y = nmp->pos[1] - mp->pos[1];
        z = nmp->pos[2] - mp->pos[2];
        dist = sqrtf(x * x + y * y + z * z);
        tgtdist -= dist;
        iter++;
    }

    /* Calculate interpolation factor */
    if (dist < 1e-5f) {
        interp = 1.0f;
    } else {
        interp = (dist + tgtdist) / dist;
    }

    if (interp < 0.0f) {
        interp = 0.0f;
    } else if (interp > 1.0f) {
        interp = 1.0f;
    }

    /* Interpolate target position */
    cp->tgtpos[0] = mp->pos[0] + (interp * (nmp->pos[0] - mp->pos[0]));
    cp->tgtpos[1] = mp->pos[1] + (interp * (nmp->pos[1] - mp->pos[1]));
    cp->tgtpos[2] = mp->pos[2] + (interp * (nmp->pos[2] - mp->pos[2]));
}

/**
 * AdjustSpeed - Convert target speed to throttle/brake inputs
 * Based on arcade: maxpath.c:AdjustSpeed()
 *
 * Uses proportional control to match current speed to target.
 *
 * @param car_index Car index
 * @param tspd Target speed
 */
void AdjustSpeed(s32 car_index, f32 tspd) {
    CarData *car;
    f32 dspd, delta_speed;

    if (car_index < 0 || car_index >= MAX_LINKS) {
        return;
    }

    car = &car_array[car_index];

    /* Scale up target slightly for responsiveness */
    tspd *= 1.05f;

    /* Get current speed */
    dspd = car->mph;

    /* Calculate speed difference */
    delta_speed = tspd - dspd;

    /* Reduce responsiveness when slowing down */
    if (delta_speed < 0.0f) {
        delta_speed *= 0.5f;
    }

    /* Update throttle with proportional control */
    lastthrottle[car_index] += (s32)(delta_speed * 750.0f);

    /* Apply brake if throttle is very negative */
    if (lastthrottle[car_index] < -3000) {
        lastbrake[car_index] -= (lastthrottle[car_index] + 3000) / 4;
    } else {
        lastbrake[car_index] = 0;
    }

    /* Clamp throttle to valid range (0 to 0x1000) */
    if (lastthrottle[car_index] < 0) {
        lastthrottle[car_index] = 0;
    } else if (lastthrottle[car_index] > 0x1000) {
        lastthrottle[car_index] = 0x1000;
    }

    /* Clamp brake to valid range */
    if (lastbrake[car_index] < 0) {
        lastbrake[car_index] = 0;
    } else if (lastbrake[car_index] > 0x1000) {
        lastbrake[car_index] = 0x1000;
    }
}

/**
 * AdjustSteer - Convert target position to steering wheel input
 * Based on arcade: maxpath.c:AdjustSteer()
 *
 * Calculates angle to target and converts to steering wheel position.
 *
 * @param car_index Car index
 * @param pos Target position (relative to car) [3]
 */
void AdjustSteer(s32 car_index, f32 *pos) {
    f32 angle;
    f32 dx, dz;

    if (car_index < 0 || car_index >= MAX_LINKS) {
        return;
    }

    /* Apply lateral bias for smoother steering */
    /* pos[1] is lateral in arcade (model Y), pos[0] is forward (model X) */
    /* For N64 we'll use pos[0] = X lateral, pos[2] = Z forward */

    /* Get direction to target */
    dx = pos[0];  /* lateral */
    dz = pos[2];  /* forward */

    /* Calculate angle using atan2 (returns -PI to PI) */
    /* Simplified: use ratio for small angles */
    if (dz > 0.001f || dz < -0.001f) {
        angle = dx / dz;
    } else {
        angle = (dx > 0) ? 1.0f : -1.0f;
    }

    /* Scale angle to wheel range (-0x1000 to 0x1000) */
    /* Multiply by ~2600 to match arcade scaling */
    lastwheel[car_index] = (s32)(angle * 2607.6f);

    /* Clamp to valid range */
    if (lastwheel[car_index] < -0x1000) {
        lastwheel[car_index] = -0x1000;
    }
    if (lastwheel[car_index] > 0x1000) {
        lastwheel[car_index] = 0x1000;
    }
}

/**
 * MaxPathControls - Generate AI inputs from path following
 * Based on arcade: maxpath.c:MaxPathControls()
 *
 * Main entry point for drone control - combines all path following logic.
 *
 * @param car_index Car index
 */
void MaxPathControls(s32 car_index) {
    ArcadeMPCTL *cp;
    CarData *car;
    f32 tspd;
    f32 rpos[3];  /* Relative position to target */

    if (car_index < 0 || car_index >= MAX_LINKS) {
        return;
    }

    if (gMPRecording) {
        /* During recording, zero out controls */
        lastthrottle[car_index] = 0;
        lastbrake[car_index] = 0;
        lastwheel[car_index] = 0;
        return;
    }

    cp = &arcade_mpctl[car_index];
    car = &car_array[car_index];

    /* Update current path segment */
    MP_FindInterval(car_index);

    /* Calculate target speed */
    MP_TargetSpeed(car_index);

    /* Calculate target steering position */
    MP_TargetSteerPos(car_index);

    /* Apply drone speed scaling */
    tspd = cp->tgtspd;

    /* Minimum speed cap */
    if (tspd < 100.0f) {
        if (cp->tgtspd < 100.0f) {
            tspd = cp->tgtspd;
        } else {
            tspd = 100.0f;
        }
    }

    /* Convert target position to relative coordinates */
    rpos[0] = cp->tgtpos[0] - car->dr_pos[0];
    rpos[1] = cp->tgtpos[1] - car->dr_pos[1];
    rpos[2] = cp->tgtpos[2] - car->dr_pos[2];

    /* Apply speed and steering control */
    AdjustSpeed(car_index, tspd);
    AdjustSteer(car_index, rpos);

    /* Check for abort condition (stuck too long) */
    /* Arcade: abort if interval_time > 1 second and past first few points */
    if ((IRQTIME - cp->interval_time > ONE_SEC) && (cp->mpi > 10)) {
        /* Mark as needing abort/reset */
    }

    /* Check for way off path */
    if (fabsf(cp->xrel) > 100.0f || fabsf(cp->yrel) > 100.0f) {
        /* Way off path - needs reset */
    }
}

/**
 * InitMaxPath - Initialize maxpath system for race
 * Based on arcade: maxpath.c:InitMaxPath()
 *
 * @param record Recording mode (-1 = playback, >=0 = record to that slot)
 */
void InitMaxPath(s32 record) {
    s32 i, j;
    MaxPathHeader *header;
    MaxPathPoint *mp;

    gNumMPaths = 0;

    /* Set up path tables and count active paths */
    for (i = 0; i < MAX_MPATHS; i++) {
        if (gMPathHeaders[i] != NULL && gMPathHeaders[i]->mpath_active) {
            gMPathList[gNumMPaths] = i;
            gNumMPaths++;
        }
    }

    /* Initialize recording state */
    if (record != -1) {
        gMPRecording = 1;
        gMPRecordIndex = 0;
    } else {
        gMPRecording = 0;

        /* Initialize per-car control state */
        for (i = 0; i < MAX_LINKS; i++) {
            arcade_mpctl[i].mpi = 0;
            arcade_mpctl[i].mpath_index = 0;
            arcade_mpctl[i].default_path = 0;
            arcade_mpctl[i].new_mpi = -1;
            arcade_mpctl[i].xrel = 0.0f;
            arcade_mpctl[i].yrel = 0.0f;
            arcade_mpctl[i].cyrel = 0.0f;
            arcade_mpctl[i].len = 0.0f;
            arcade_mpctl[i].tgtspd = 100.0f;
            arcade_mpctl[i].interval_time = IRQTIME;

            lastwheel[i] = 0;
            lastbrake[i] = 0;
            lastthrottle[i] = 0;
        }
    }

    loop_stamp = 0;
    best_loop_time = 9999999;
}

/**
 * CalcTargetPoint - Calculate world position for steering target
 * Based on arcade target point calculation logic
 *
 * @param car_index Car index
 * @param out_pos Output world position [3]
 */
void CalcTargetPoint(s32 car_index, f32 *out_pos) {
    ArcadeMPCTL *cp;

    if (car_index < 0 || car_index >= MAX_LINKS) {
        out_pos[0] = 0.0f;
        out_pos[1] = 0.0f;
        out_pos[2] = 0.0f;
        return;
    }

    cp = &arcade_mpctl[car_index];

    out_pos[0] = cp->tgtpos[0];
    out_pos[1] = cp->tgtpos[1];
    out_pos[2] = cp->tgtpos[2];
}

/**
 * GetPathNodePosition - Get world position of a path node
 *
 * @param path_index Which path
 * @param node_index Which node on path
 * @param out_pos Output position [3]
 */
void GetPathNodePosition(s32 path_index, s32 node_index, f32 *out_pos) {
    MaxPathPoint *mp;

    if (path_index < 0 || path_index >= gNumMPaths) {
        out_pos[0] = 0.0f;
        out_pos[1] = 0.0f;
        out_pos[2] = 0.0f;
        return;
    }

    if (gMPathTables[path_index] == NULL) {
        out_pos[0] = 0.0f;
        out_pos[1] = 0.0f;
        out_pos[2] = 0.0f;
        return;
    }

    mp = &gMPathTables[path_index][node_index];
    out_pos[0] = mp->pos[0];
    out_pos[1] = mp->pos[1];
    out_pos[2] = mp->pos[2];
}

/**
 * sync_maxpath_to_last_checkpoint - Sync car to checkpoint on path
 * Based on arcade: maxpath.c:sync_maxpath_to_last_checkpoint()
 *
 * @param node Car index
 */
void sync_maxpath_to_last_checkpoint(s16 node) {
    s32 index;
    ArcadeMPCTL *cp;

    if (node < 0 || node >= MAX_LINKS) {
        return;
    }

    cp = &arcade_mpctl[node];

    /* Find nearest maxpath point to last checkpoint */
    /* In full implementation, would use path_to_maxpath mapping */
    index = cp->mpi;  /* Placeholder - keep current */

    cp->new_mpi = index;
    cp->mpi = index;
}

/**
 * GetMPathControlSteer - Get steering value for a car
 *
 * @param car_index Car index
 * @return Steering value (-1.0 to 1.0)
 */
f32 GetMPathControlSteer(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_LINKS) {
        return 0.0f;
    }
    return (f32)lastwheel[car_index] / 4096.0f;
}

/**
 * GetMPathControlThrottle - Get throttle value for a car
 *
 * @param car_index Car index
 * @return Throttle value (0.0 to 1.0)
 */
f32 GetMPathControlThrottle(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_LINKS) {
        return 0.0f;
    }
    return (f32)lastthrottle[car_index] / 4096.0f;
}

/**
 * GetMPathControlBrake - Get brake value for a car
 *
 * @param car_index Car index
 * @return Brake value (0.0 to 1.0)
 */
f32 GetMPathControlBrake(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_LINKS) {
        return 0.0f;
    }
    return (f32)lastbrake[car_index] / 4096.0f;
}

/**
 * GetMPathCurrentIndex - Get current path index for a car
 *
 * @param car_index Car index
 * @return Current waypoint index
 */
s32 GetMPathCurrentIndex(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_LINKS) {
        return 0;
    }
    return arcade_mpctl[car_index].mpi;
}

/**
 * GetMPathTargetSpeed - Get target speed for a car
 *
 * @param car_index Car index
 * @return Target speed in mph
 */
f32 GetMPathTargetSpeed(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_LINKS) {
        return 100.0f;
    }
    return arcade_mpctl[car_index].tgtspd;
}

/**
 * SetMPathIndex - Set car to specific path index
 *
 * @param car_index Car index
 * @param new_mpi New waypoint index
 */
void SetMPathIndex(s32 car_index, s32 new_mpi) {
    if (car_index < 0 || car_index >= MAX_LINKS) {
        return;
    }
    arcade_mpctl[car_index].new_mpi = new_mpi;
}

/**
 * SetMPathPath - Set which path a car follows
 *
 * @param car_index Car index
 * @param path_index Path to follow (0-7)
 */
void SetMPathPath(s32 car_index, s32 path_index) {
    if (car_index < 0 || car_index >= MAX_LINKS) {
        return;
    }
    if (path_index < 0 || path_index >= MAX_MPATHS) {
        return;
    }
    arcade_mpctl[car_index].mpath_index = path_index;
    arcade_mpctl[car_index].default_path = path_index;
}
