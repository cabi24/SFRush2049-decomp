/**
 * drone.c - AI/Drone car control for Rush 2049 N64
 *
 * Based on arcade game/drones.c
 * Handles AI-controlled cars (drones) that race against the player.
 *
 * Key features:
 * - Path following using maxpath waypoints
 * - Catch-up/rubber-banding for competitive racing
 * - Difficulty scaling based on player position
 */

#include "types.h"
#include "game/drone.h"
#include "game/structs.h"
#include "game/checkpoint.h"

/* External OS functions */
extern u32 osGetCount(void);
extern u32 frame_counter;

/* External game state */
extern u8 gstate;
extern CarData car_array[];
extern s32 num_active_cars;
extern s32 this_car;
extern s32 trackno;

/* External math */
extern f32 sqrtf(f32 x);

/* Drone control data */
DroneControl drone_ctl[MAX_CARS];
s32 num_drones;
s32 num_humans;

/* Catch-up state */
static s32 use_catchup = 1;

/* Old drone scale/boost for smoothing */
static f32 old_drone_scale[MAX_CARS];
static f32 old_boost[MAX_CARS];

/**
 * drone_init - Initialize drone system for all cars
 * Based on arcade: drones.c:InitDrones()
 */
void drone_init(void) {
    s32 i;

    /* Initialize maxpath system */
    drone_init_maxpath(-1);  /* -1 = no recording */

    for (i = 0; i < MAX_CARS; i++) {
        drone_init_car(i);
        old_drone_scale[i] = 1.0f;
        old_boost[i] = 1.0f;
    }

    num_drones = 0;
    num_humans = 0;
}

/**
 * drone_init_car - Initialize drone control for one car
 *
 * @param car_index Car index to initialize
 */
void drone_init_car(s32 car_index) {
    DroneControl *ctl;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    ctl = &drone_ctl[car_index];

    ctl->car_index = car_index;
    ctl->drone_type = DRONE_TYPE_HUMAN;  /* Default to human */
    ctl->personality = DRONE_PERS_NORMAL;
    ctl->difficulty = 0;
    ctl->target_car = car_index;
    ctl->weight_index = 0;
    ctl->drone_scale = 1.0f;
    ctl->time_boost = 1.0f;
    ctl->catchup_boost = 1.0f;
    ctl->mpath_index = 0;
    ctl->mpath_segment = 0;
    ctl->steer_target = 0.0f;
    ctl->throttle_target = 1.0f;
    ctl->brake_target = 0.0f;
    ctl->we_control = 0;
    ctl->is_active = 0;
}

/**
 * drone_init_maxpath - Initialize maxpath waypoint system
 * Based on arcade: maxpath.c:InitMaxPath()
 *
 * @param record_mode Recording mode (-1 = none)
 */
void drone_init_maxpath(s32 record_mode) {
    /* Maxpath waypoints are loaded from track data */
    /* For N64, these are in the compressed game code */
}

/**
 * drone_update_all - Update all drones each frame
 * Based on arcade: drones.c:DoDrones()
 */
void drone_update_all(void) {
    s32 i;

    /* Handle catch-up speed adjustments */
    if (!use_catchup) {
        drone_no_catchup();
    } else {
        drone_set_catchup();
    }

    /* Update path following for player's car (for waypoint tracking) */
    drone_do_maxpath(this_car);

    /* Assign drone behaviors */
    drone_assign_all();

    /* Update each active drone */
    for (i = 0; i < num_active_cars; i++) {
        if (drone_ctl[i].drone_type == DRONE_TYPE_DRONE && drone_ctl[i].is_active) {
            drone_update(i);
        }
    }
}

/**
 * drone_update - Update one drone's AI
 *
 * @param car_index Drone car index
 */
void drone_update(s32 car_index) {
    DroneControl *ctl;
    CarData *car;
    f32 target_steer;
    f32 target_throttle;

    ctl = &drone_ctl[car_index];
    car = &car_array[car_index];

    if (!ctl->is_active) {
        return;
    }

    /* Follow maxpath waypoints */
    drone_do_maxpath(car_index);

    /* Calculate catch-up boost */
    drone_calc_catchup(car_index);

    /* Apply speed scaling */
    /* In arcade, this modifies the car's torque curve */
    /* For N64, we'll scale the throttle input */
    target_throttle = ctl->throttle_target * ctl->drone_scale * ctl->time_boost;

    /* Smooth the steering */
    target_steer = ctl->steer_target;

    /* Apply drone inputs to car */
    /* These would be written to the car's input structure */
}

/**
 * drone_do_maxpath - Follow maxpath waypoints
 * Based on arcade: maxpath.c:MaxPath()
 *
 * @param car_index Car to update
 */
void drone_do_maxpath(s32 car_index) {
    DroneControl *ctl;
    CarData *car;
    f32 dx, dz;
    f32 dist_sq;

    ctl = &drone_ctl[car_index];
    car = &car_array[car_index];

    /* Check if we've reached current waypoint */
    /* In full implementation, would check distance to next waypoint */
    /* and update mpath_index when close enough */

    /* Calculate steering to reach next waypoint */
    /* This is a simplified version - arcade uses spline interpolation */
}

/**
 * drone_set_catchup - Enable catch-up mode
 * Based on arcade: model.c:set_catchup() and drones.c logic
 *
 * Catch-up makes trailing drones faster and leading drones slower
 * to keep the race competitive.
 */
void drone_set_catchup(void) {
    s32 i;
    s32 my_place;
    s32 target_car;
    f32 place_scale;
    f32 diff_scale;
    f32 max_boost, min_boost;
    f32 max_brake, min_brake;
    f32 delta_dist;
    f32 time_scale, scale;
    DroneControl *ctl;

    for (i = 0; i < num_active_cars; i++) {
        ctl = &drone_ctl[i];

        if (ctl->drone_type != DRONE_TYPE_DRONE) {
            continue;
        }

        if (!ctl->we_control) {
            continue;
        }

        my_place = car_array[i].place;
        target_car = ctl->target_car;

        /* Place scale: first place = 0, last place = 1 */
        place_scale = (f32)my_place / (f32)(num_active_cars - 1);

        /* Difficulty scale: 0 = hard, 1 = easy */
        diff_scale = (f32)ctl->difficulty / 7.0f;

        /* Calculate boost/brake limits based on place and difficulty */
        /* From arcade: complex formula based on place and difficulty */
        max_boost = (1.0f - 1.1f) * diff_scale + 1.1f - place_scale * ((0.15f - 0.05f) * diff_scale + 0.05f);
        min_boost = 1.0f - place_scale * ((0.06f - 0.02f) * diff_scale + 0.02f);
        max_brake = 0.9f - place_scale * ((0.6f - 0.1f) * diff_scale + 0.1f);
        min_brake = (0.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((0.05f - 0.02f) * diff_scale + 0.02f);

        /* Calculate distance to target */
        delta_dist = car_array[i].distance - car_array[target_car].distance;

        /* Apply catch-up based on distance to target human */
        if (target_car != i) {
            if (delta_dist < -20.0f) {
                /* Human in front of drone, speed up */
                time_scale = linear_interp(-300.0f, -60.0f, max_boost, min_boost, delta_dist);
                scale = min_brake;
            } else {
                /* Drone ahead of or near human, slow down */
                scale = linear_interp(200.0f, 60.0f, max_brake, min_brake, delta_dist);
                time_scale = min_boost;
            }
        } else {
            /* No target - use place-based scaling */
            if (my_place < num_active_cars / 2) {
                /* Leading - slow down */
                time_scale = min_boost;
                scale = min_brake;
            } else {
                /* Trailing - speed up */
                time_scale = max_boost;
                scale = min_brake;
            }
        }

        ctl->catchup_boost = time_scale;
        ctl->drone_scale = scale;
    }
}

/**
 * drone_no_catchup - Disable catch-up mode
 *
 * All drones run at base speed.
 */
void drone_no_catchup(void) {
    s32 i;

    for (i = 0; i < num_active_cars; i++) {
        drone_ctl[i].catchup_boost = 1.0f;
    }
}

/**
 * drone_calc_catchup - Calculate catch-up for one drone
 *
 * @param car_index Drone car index
 */
void drone_calc_catchup(s32 car_index) {
    DroneControl *ctl;
    s32 target;
    f32 my_dist, target_dist;
    f32 delta;

    ctl = &drone_ctl[car_index];
    target = ctl->target_car;

    if (target == car_index || !use_catchup) {
        ctl->drone_scale = 1.0f;
        return;
    }

    /* Get distances */
    my_dist = car_array[car_index].distance;
    target_dist = car_array[target].distance;

    /* Calculate distance delta */
    delta = target_dist - my_dist;

    /* Scale based on distance behind target */
    if (delta > CATCHUP_DISTANCE) {
        /* Way behind - max boost */
        ctl->drone_scale = CATCHUP_MAX_BOOST;
    } else if (delta < -CATCHUP_DISTANCE) {
        /* Way ahead - slow down */
        ctl->drone_scale = CATCHUP_MIN_BOOST;
    } else {
        /* Interpolate */
        ctl->drone_scale = 1.0f + (delta / CATCHUP_DISTANCE) * 0.1f;
    }

    /* Smooth the scaling */
    ctl->drone_scale = old_drone_scale[car_index] * 0.9f + ctl->drone_scale * 0.1f;
    old_drone_scale[car_index] = ctl->drone_scale;
}

/**
 * drone_assign_all - Assign drone behaviors and targets
 * Based on arcade: drones.c:assign_drones()
 */
void drone_assign_all(void) {
    s32 i;
    s32 cars_in_order[MAX_CARS];
    s32 humans[MAX_CARS];
    s32 drones[MAX_CARS];

    /* Count humans and drones, get race order */
    drone_place_in_order();

    /* Assign default paths to drones */
    drone_assign_default_paths();

    /* Early in race, spread drones out */
    if (frame_counter < 5 * 60) {  /* First 5 seconds */
        for (i = 0; i < num_drones; i++) {
            s32 idx = drones[i];
            s32 j = num_drones - i - 1;
            drone_ctl[idx].time_boost = 0.98f + (f32)j * 0.02f;
            drone_ctl[idx].drone_scale = 1.0f;
        }
    }
}

/**
 * drone_assign_default_paths - Assign default maxpath to drones
 */
void drone_assign_default_paths(void) {
    /* Each drone gets assigned a path segment based on their position */
    /* In arcade, there are multiple paths (fast/slow/medium) */
}

/**
 * drone_place_in_order - Sort cars by race position
 */
void drone_place_in_order(void) {
    s32 i;

    num_drones = 0;
    num_humans = 0;

    for (i = 0; i < num_active_cars; i++) {
        if (drone_ctl[i].drone_type == DRONE_TYPE_HUMAN) {
            num_humans++;
        } else {
            num_drones++;
        }
    }
}

/**
 * drone_get_type - Get drone type for a car
 *
 * @param car_index Car index
 * @return DRONE_TYPE_HUMAN or DRONE_TYPE_DRONE
 */
s32 drone_get_type(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return DRONE_TYPE_HUMAN;
    }
    return drone_ctl[car_index].drone_type;
}

/**
 * drone_get_target - Get target car for a drone
 *
 * @param car_index Drone car index
 * @return Target car index
 */
s32 drone_get_target(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }
    return drone_ctl[car_index].target_car;
}

/**
 * drone_get_scale - Get speed scale for a drone
 *
 * @param car_index Drone car index
 * @return Speed scale factor
 */
f32 drone_get_scale(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 1.0f;
    }
    return drone_ctl[car_index].drone_scale * drone_ctl[car_index].catchup_boost;
}

/**
 * drone_is_human - Check if car is human-controlled
 *
 * @param car_index Car index
 * @return 1 if human, 0 if drone
 */
s32 drone_is_human(s32 car_index) {
    return drone_get_type(car_index) == DRONE_TYPE_HUMAN;
}

/**
 * drone_set_difficulty - Set difficulty for a drone
 *
 * @param car_index Drone car index
 * @param difficulty Difficulty level (0=hard, 7=easy)
 */
void drone_set_difficulty(s32 car_index, s32 difficulty) {
    if (car_index >= 0 && car_index < MAX_CARS) {
        drone_ctl[car_index].difficulty = difficulty;
    }
}

/**
 * drone_set_target - Set target car for a drone
 *
 * @param car_index Drone car index
 * @param target Target car index
 */
void drone_set_target(s32 car_index, s32 target) {
    if (car_index >= 0 && car_index < MAX_CARS) {
        drone_ctl[car_index].target_car = target;
    }
}

/**
 * drone_end - Clean up drone system
 * Based on arcade: drones.c:EndDrones()
 */
void drone_end(void) {
    /* Nothing to clean up in arcade version */
}

/**
 * linear_interp - Linear interpolation utility
 * Based on arcade: drones.c:linear_interp()
 *
 * Maps input from range [in_bound1, in_bound2] to [out_bound1, out_bound2].
 * Clamps output to bounds if input is outside range.
 *
 * @param in_bound1  First input boundary
 * @param in_bound2  Second input boundary
 * @param out_bound1 Output value when input equals in_bound1
 * @param out_bound2 Output value when input equals in_bound2
 * @param input      Value to interpolate
 * @return Interpolated output value
 */
f32 linear_interp(f32 in_bound1, f32 in_bound2, f32 out_bound1, f32 out_bound2, f32 input) {
    f32 slope, offset;

    /* Handle clamping based on bound ordering */
    if (in_bound1 < in_bound2) {
        if (input < in_bound1) {
            return out_bound1;
        } else if (input > in_bound2) {
            return out_bound2;
        }
    } else {
        if (input < in_bound2) {
            return out_bound2;
        } else if (input > in_bound1) {
            return out_bound1;
        }
    }

    /* If input range is too small, return center of output */
    if ((in_bound1 - in_bound2 > -0.001f) && (in_bound1 - in_bound2 < 0.001f)) {
        return (out_bound1 + out_bound2) * 0.5f;
    }

    /* Linear interpolation: y = mx + b */
    slope = (out_bound1 - out_bound2) / (in_bound1 - in_bound2);
    offset = out_bound1 - slope * in_bound1;

    return slope * input + offset;
}
