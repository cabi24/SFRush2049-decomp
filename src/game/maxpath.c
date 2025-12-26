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

/* External math */
extern f32 sqrtf(f32 x);

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
    if (car->velocity > 5.0f) {
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

    if (car->velocity > suggested_speed * 1.2f) {
        /* Going too fast, brake */
        *throttle = 0.3f;
        *brake = 0.3f;
    } else if (car->velocity < suggested_speed * 0.5f) {
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
