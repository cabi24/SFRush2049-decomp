/**
 * drone.h - AI/Drone car control for Rush 2049 N64
 *
 * Based on arcade game/drones.c
 * Handles AI-controlled cars (drones) that race against the player.
 *
 * Key features:
 * - Path following using maxpath waypoints
 * - Catch-up/rubber-banding for competitive racing
 * - Difficulty scaling based on player position
 */

#ifndef DRONE_H
#define DRONE_H

#include "types.h"
#include "game/maxpath.h"

/* Drone types */
#define DRONE_TYPE_HUMAN    0   /* Player-controlled */
#define DRONE_TYPE_DRONE    1   /* AI-controlled */

/* Drone personality/behavior */
#define DRONE_PERS_NORMAL   0   /* Normal racing */
#define DRONE_PERS_AGGRESSIVE 1 /* More aggressive */
#define DRONE_PERS_PASSIVE  2   /* Hangs back */

/* Catch-up constants */
#define CATCHUP_DISTANCE        300.0f /* Distance for full catchup effect */
#define CATCHUP_MAX_BOOST       1.15f  /* Maximum speed boost */
#define CATCHUP_MIN_BOOST       0.85f  /* Maximum slowdown */
#define CATCHUP_SMOOTHING       0.03f  /* Smoothing factor per frame */
#define CATCHUP_SCALE_DEFAULT   0.10f  /* Default catchup strength */
#define CATCHUP_ZONE_DEFAULT    500.0f /* Distance zone for full effect */
#define CATCHUP_SOLO_BOOST      1.05f  /* Solo player boost */

/* Maxpath constants */
#define PATH_LOOKAHEAD  80.0f   /* Look-ahead for steering */
#define PATH_SPEED_SCALE 1.0f   /* Speed scaling factor */

/* Arcade-compatible aliases */
typedef MaxPathPoint MPATH;
typedef MaxPathHeader MPHEADER;

/* Drone control structure */
typedef struct DroneControl {
    s32     car_index;          /* Which car this controls */
    s32     drone_type;         /* HUMAN or DRONE */
    s32     personality;        /* Behavior style */
    s32     difficulty;         /* 0=hard, 7=easy */
    s32     target_car;         /* Car this drone is targeting */
    s32     weight_index;       /* Position in drone order */
    f32     drone_scale;        /* Speed scaling factor */
    f32     time_boost;         /* Time-based speed boost */
    f32     catchup_boost;      /* Catch-up speed boost */
    s32     mpath_index;        /* Current maxpath waypoint */
    s32     mpath_segment;      /* Current path segment */
    f32     xrel;               /* Forward position on segment */
    f32     yrel;               /* Lateral offset from path */
    f32     path_speed;         /* Target speed from path */
    f32     path_curvature;     /* Path curvature at current point */
    f32     steer_target;       /* Target steering angle */
    f32     throttle_target;    /* Target throttle (0-1) */
    f32     brake_target;       /* Target brake (0-1) */
    s8      we_control;         /* Do we control this car? */
    s8      is_active;          /* Is drone active? */
    u8      pad[2];
} DroneControl;

/* External drone data */
extern DroneControl drone_ctl[];
extern s32 num_drones;
extern s32 num_humans;

/* Initialization */
void drone_init(void);
void drone_init_car(s32 car_index);
void drone_init_maxpath(s32 record_mode);

/* Per-frame update */
void drone_update_all(void);
void drone_update(s32 car_index);
void drone_do_maxpath(s32 car_index);
/* Path helpers */
s32 drone_find_interval(s32 car_index);
void drone_interval_pos(s32 car_index, f32 *xrel_out, f32 *yrel_out);
void drone_find_path_dist(s32 car_index, f32 dist, f32 *pos_out);
/* Path controls */
f32 drone_target_speed(s32 car_index);
void drone_target_steer_pos(s32 car_index, f32 *target_pos);
void drone_adjust_speed(s32 car_index, f32 target_speed, f32 *throttle_out, f32 *brake_out);
f32 drone_adjust_steer(s32 car_index, f32 *target_pos);
void drone_maxpath_controls(s32 car_index);
void drone_avoid_areas(s32 car_index);
f32 drone_get_avoid_strength(HintType hint);
void drone_avoid_walls(s32 car_index);

/* Catch-up system */
void drone_set_catchup(void);
void drone_no_catchup(void);
void drone_calc_catchup(s32 car_index);
f32 drone_scale_catchup_by_difficulty(s32 car_index, f32 base_catchup);
void drone_assign_targets(void);
f32 drone_get_speed_multiplier(s32 car_index);
void drone_apply_inputs(s32 car_index);
void drone_activate_for_race(void);
void drone_deactivate(void);
void drone_set_difficulty_level(s32 difficulty);

/* Drone assignment */
void drone_assign_all(void);
void drone_assign_default_paths(void);
void drone_place_in_order(void);

/* Queries */
s32  drone_get_type(s32 car_index);
s32  drone_get_target(s32 car_index);
f32  drone_get_scale(s32 car_index);
s32  drone_is_human(s32 car_index);

/* Utility */
void drone_set_difficulty(s32 car_index, s32 difficulty);
void drone_set_target(s32 car_index, s32 target);
void drone_end(void);

/**
 * Linear interpolation utility
 * Based on arcade: drones.c:linear_interp()
 *
 * Maps input from range [in_bound1, in_bound2] to [out_bound1, out_bound2]
 * Clamps output to bounds if input is outside range.
 *
 * @param in_bound1  First input boundary
 * @param in_bound2  Second input boundary
 * @param out_bound1 Output value when input equals in_bound1
 * @param out_bound2 Output value when input equals in_bound2
 * @param input      Value to interpolate
 * @return Interpolated output value
 */
f32 linear_interp(f32 in_bound1, f32 in_bound2, f32 out_bound1, f32 out_bound2, f32 input);

#endif /* DRONE_H */
