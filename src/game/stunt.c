/**
 * stunt.c - Stunt mode system for Rush 2049 N64
 *
 * Rush 2049's signature feature: deployable wings and trick scoring.
 * Players can extend wings for aerial control and perform tricks
 * off ramps and jumps to earn points.
 */

#include "types.h"
#include "game/stunt.h"
#include "game/structs.h"
#include "game/sound.h"

/* External game state */
extern CarData car_array[];
extern s32 num_active_cars;
extern u32 frame_counter;

/* External math */
extern f32 sqrtf(f32 x);
extern f32 fabsf(f32 x);
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);

/* Global stunt state */
StuntState gStunt[MAX_CARS];
s32 gStuntModeEnabled = 0;

/* Trick names */
const char *gTrickNames[] = {
    "",
    "SPIN LEFT",
    "SPIN RIGHT",
    "FRONT FLIP",
    "BACK FLIP",
    "BARREL ROLL L",
    "BARREL ROLL R",
    "CORKSCREW",
    "WING FLASH"
};

/* Absolute value macro */
#define ABS(x) (((x) < 0) ? -(x) : (x))
#define CLAMP(v, lo, hi) (((v) < (lo)) ? (lo) : (((v) > (hi)) ? (hi) : (v)))

/**
 * stunt_init - Initialize stunt system
 */
void stunt_init(void) {
    s32 i;

    gStuntModeEnabled = 0;

    for (i = 0; i < MAX_CARS; i++) {
        stunt_init_car(i);
    }
}

/**
 * stunt_reset - Reset stunt state for new race
 */
void stunt_reset(void) {
    s32 i;

    for (i = 0; i < MAX_CARS; i++) {
        stunt_init_car(i);
    }
}

/**
 * stunt_init_car - Initialize stunt state for one car
 *
 * @param car_index Car index
 */
void stunt_init_car(s32 car_index) {
    StuntState *st;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    st = &gStunt[car_index];

    st->state = STUNT_STATE_GROUNDED;
    st->wing_state = WING_STATE_RETRACTED;
    st->wing_input = 0;
    st->grounded = 1;

    st->wing_extension = 0.0f;
    st->wing_target = 0.0f;
    st->wing_speed = 1.0f / (f32)WING_EXTEND_TIME;

    st->air_time = 0.0f;
    st->max_height = 0.0f;
    st->launch_height = 0.0f;
    st->launch_speed = 0.0f;

    st->spin_total = 0.0f;
    st->flip_total = 0.0f;
    st->roll_total = 0.0f;
    st->last_yaw = 0.0f;
    st->last_pitch = 0.0f;
    st->last_roll = 0.0f;

    st->current_trick.type = TRICK_NONE;
    st->current_trick.completed = 0;
    st->current_trick.rotation = 0.0f;
    st->current_trick.start_angle = 0.0f;
    st->current_trick.start_frame = 0;
    st->current_trick.points = 0;

    st->combo_count = 0;
    st->combo_points = 0;
    st->combo_multiplier = 1.0f;
    st->combo_timer = 0;

    st->total_score = 0;
    st->best_combo = 0;
    st->tricks_landed = 0;
    st->crashes = 0;

    st->targets_hit = 0;
    st->coins_collected = 0;
}

/**
 * stunt_update - Update stunt state for one car
 *
 * @param car_index Car index
 */
void stunt_update(s32 car_index) {
    StuntState *st;
    CarData *car;

    if (!gStuntModeEnabled) {
        return;
    }

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    st = &gStunt[car_index];
    car = &car_array[car_index];

    /* Update wing animation */
    stunt_update_wings(car_index);

    /* Check ground/air state */
    stunt_check_airborne(car_index);

    /* Update based on state */
    switch (st->state) {
        case STUNT_STATE_GROUNDED:
            /* Reset rotation tracking */
            st->spin_total = 0.0f;
            st->flip_total = 0.0f;
            st->roll_total = 0.0f;
            st->air_time = 0.0f;

            /* Decay combo timer */
            if (st->combo_timer > 0) {
                st->combo_timer--;
                if (st->combo_timer == 0) {
                    /* Combo ended, finalize points */
                    stunt_update_combo(car_index);
                }
            }
            break;

        case STUNT_STATE_AIRBORNE:
            /* Track time in air */
            st->air_time += 1.0f / 60.0f;

            /* Track maximum height */
            if (car->dr_pos[1] > st->max_height) {
                st->max_height = car->dr_pos[1];
            }

            /* Update rotation tracking */
            stunt_update_rotation(car_index);

            /* Check for completed tricks */
            stunt_detect_trick(car_index);
            break;

        case STUNT_STATE_TRICKING:
            /* Continue tracking rotation */
            stunt_update_rotation(car_index);
            break;

        case STUNT_STATE_LANDING:
            /* Check landing angle */
            stunt_check_landing(car_index);
            break;

        case STUNT_STATE_CRASHED:
            /* Wait for recovery */
            break;
    }
}

/**
 * stunt_update_all - Update all cars' stunt state
 */
void stunt_update_all(void) {
    s32 i;

    if (!gStuntModeEnabled) {
        return;
    }

    for (i = 0; i < num_active_cars; i++) {
        stunt_update(i);
    }
}

/**
 * stunt_wing_input - Process wing button input
 *
 * @param car_index Car index
 * @param pressed Button pressed (1) or released (0)
 */
void stunt_wing_input(s32 car_index, s32 pressed) {
    StuntState *st;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    st = &gStunt[car_index];
    st->wing_input = (u8)pressed;

    if (pressed) {
        stunt_extend_wings(car_index);
    } else {
        stunt_retract_wings(car_index);
    }
}

/**
 * stunt_extend_wings - Start extending wings
 *
 * @param car_index Car index
 */
void stunt_extend_wings(s32 car_index) {
    StuntState *st;

    st = &gStunt[car_index];

    if (st->wing_state != WING_STATE_EXTENDED) {
        st->wing_state = WING_STATE_EXTENDING;
        st->wing_target = 1.0f;
        sound_play(SFX_WING_EXTEND);
    }
}

/**
 * stunt_retract_wings - Start retracting wings
 *
 * @param car_index Car index
 */
void stunt_retract_wings(s32 car_index) {
    StuntState *st;

    st = &gStunt[car_index];

    if (st->wing_state != WING_STATE_RETRACTED) {
        st->wing_state = WING_STATE_RETRACTING;
        st->wing_target = 0.0f;
    }
}

/**
 * stunt_update_wings - Update wing extension animation
 *
 * @param car_index Car index
 */
void stunt_update_wings(s32 car_index) {
    StuntState *st;
    f32 delta;

    st = &gStunt[car_index];

    /* Move toward target */
    delta = st->wing_target - st->wing_extension;

    if (delta > 0.01f) {
        st->wing_extension += st->wing_speed;
        if (st->wing_extension >= st->wing_target) {
            st->wing_extension = st->wing_target;
            st->wing_state = WING_STATE_EXTENDED;
        }
    } else if (delta < -0.01f) {
        st->wing_extension -= st->wing_speed;
        if (st->wing_extension <= st->wing_target) {
            st->wing_extension = st->wing_target;
            st->wing_state = WING_STATE_RETRACTED;
        }
    }
}

/**
 * stunt_get_wing_extension - Get current wing extension
 *
 * @param car_index Car index
 * @return Wing extension (0.0 to 1.0)
 */
f32 stunt_get_wing_extension(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0.0f;
    }
    return gStunt[car_index].wing_extension;
}

/**
 * stunt_apply_wing_lift - Apply wing lift force
 *
 * @param car_index Car index
 * @param lift Output lift force
 */
void stunt_apply_wing_lift(s32 car_index, f32 *lift) {
    StuntState *st;
    CarData *car;
    f32 speed_factor;
    f32 wing_factor;

    st = &gStunt[car_index];
    car = &car_array[car_index];

    /* Lift depends on speed and wing extension */
    speed_factor = car->velocity / 100.0f;
    if (speed_factor > 1.0f) speed_factor = 1.0f;

    wing_factor = st->wing_extension * WING_LIFT_FACTOR;

    *lift = speed_factor * wing_factor;
}

/**
 * stunt_apply_wing_drag - Apply wing drag force
 *
 * @param car_index Car index
 * @param drag Output drag force
 */
void stunt_apply_wing_drag(s32 car_index, f32 *drag) {
    StuntState *st;

    st = &gStunt[car_index];

    /* Drag proportional to wing extension */
    *drag = st->wing_extension * WING_DRAG_FACTOR;
}

/**
 * stunt_apply_air_control - Apply air control from wings
 *
 * @param car_index Car index
 * @param steer_input Steering input (-1 to 1)
 * @param steer_force Output steering force
 */
void stunt_apply_air_control(s32 car_index, f32 steer_input, f32 *steer_force) {
    StuntState *st;

    st = &gStunt[car_index];

    /* Air control proportional to wing extension */
    *steer_force = steer_input * st->wing_extension * WING_CONTROL_FACTOR;
}

/**
 * stunt_check_airborne - Check if car became airborne
 *
 * @param car_index Car index
 */
void stunt_check_airborne(s32 car_index) {
    StuntState *st;
    CarData *car;
    f32 height;

    st = &gStunt[car_index];
    car = &car_array[car_index];

    /* Get height above ground */
    height = car->dr_pos[1] - car->ground_height;

    if (st->grounded) {
        /* Check if we've left the ground */
        if (height > MIN_TRICK_HEIGHT) {
            st->grounded = 0;
            st->state = STUNT_STATE_AIRBORNE;
            st->launch_height = car->dr_pos[1];
            st->launch_speed = car->velocity;
            st->max_height = car->dr_pos[1];
            st->air_time = 0.0f;

            /* Reset rotation tracking */
            st->spin_total = 0.0f;
            st->flip_total = 0.0f;
            st->roll_total = 0.0f;
            /* Store current angles */
            /* In full implementation, read from car orientation */
            st->last_yaw = 0.0f;
            st->last_pitch = 0.0f;
            st->last_roll = 0.0f;
        }
    } else {
        /* Check if we've landed */
        if (height < 0.5f) {
            st->grounded = 1;
            st->state = STUNT_STATE_LANDING;
            stunt_check_landing(car_index);
        }
    }
}

/**
 * stunt_check_landing - Check landing conditions
 *
 * @param car_index Car index
 */
void stunt_check_landing(s32 car_index) {
    StuntState *st;
    s32 safe;

    st = &gStunt[car_index];

    safe = stunt_check_landing_safe(car_index);

    if (safe) {
        stunt_land(car_index);
    } else {
        stunt_crash(car_index);
    }
}

/**
 * stunt_update_rotation - Update rotation tracking
 *
 * @param car_index Car index
 */
void stunt_update_rotation(s32 car_index) {
    StuntState *st;
    CarData *car;
    f32 yaw, pitch, roll;
    f32 delta_yaw, delta_pitch, delta_roll;

    st = &gStunt[car_index];
    car = &car_array[car_index];

    /* Get current angles from car orientation */
    /* In full implementation, extract from dr_uvs matrix */
    /* For now, use placeholder calculation */
    yaw = car->yaw;
    pitch = car->pitch;
    roll = car->roll;

    /* Calculate rotation deltas */
    delta_yaw = yaw - st->last_yaw;
    delta_pitch = pitch - st->last_pitch;
    delta_roll = roll - st->last_roll;

    /* Handle wraparound */
    if (delta_yaw > 180.0f) delta_yaw -= 360.0f;
    if (delta_yaw < -180.0f) delta_yaw += 360.0f;
    if (delta_pitch > 180.0f) delta_pitch -= 360.0f;
    if (delta_pitch < -180.0f) delta_pitch += 360.0f;
    if (delta_roll > 180.0f) delta_roll -= 360.0f;
    if (delta_roll < -180.0f) delta_roll += 360.0f;

    /* Accumulate totals */
    st->spin_total += delta_yaw;
    st->flip_total += delta_pitch;
    st->roll_total += delta_roll;

    /* Store for next frame */
    st->last_yaw = yaw;
    st->last_pitch = pitch;
    st->last_roll = roll;
}

/**
 * stunt_detect_trick - Detect if a trick was completed
 *
 * @param car_index Car index
 * @return Trick type detected, or TRICK_NONE
 */
s32 stunt_detect_trick(s32 car_index) {
    StuntState *st;
    s32 trick = TRICK_NONE;
    f32 abs_spin, abs_flip, abs_roll;

    st = &gStunt[car_index];

    abs_spin = ABS(st->spin_total);
    abs_flip = ABS(st->flip_total);
    abs_roll = ABS(st->roll_total);

    /* Check for barrel roll (combined spin and roll) */
    if (abs_spin > SPIN_THRESHOLD && abs_roll > SPIN_THRESHOLD) {
        if (st->spin_total > 0) {
            trick = TRICK_BARREL_RIGHT;
        } else {
            trick = TRICK_BARREL_LEFT;
        }
        /* Reset counters */
        st->spin_total -= (st->spin_total > 0) ? 360.0f : -360.0f;
        st->roll_total -= (st->roll_total > 0) ? 360.0f : -360.0f;
    }
    /* Check for spin */
    else if (abs_spin > SPIN_THRESHOLD) {
        if (st->spin_total > 0) {
            trick = TRICK_SPIN_RIGHT;
        } else {
            trick = TRICK_SPIN_LEFT;
        }
        st->spin_total -= (st->spin_total > 0) ? 360.0f : -360.0f;
    }
    /* Check for flip */
    else if (abs_flip > FLIP_THRESHOLD) {
        if (st->flip_total > 0) {
            trick = TRICK_FLIP_FORWARD;
        } else {
            trick = TRICK_FLIP_BACKWARD;
        }
        st->flip_total -= (st->flip_total > 0) ? 360.0f : -360.0f;
    }

    /* If trick detected, award points */
    if (trick != TRICK_NONE) {
        stunt_start_trick(car_index, trick);
        stunt_complete_trick(car_index);
    }

    return trick;
}

/**
 * stunt_start_trick - Start a trick
 *
 * @param car_index Car index
 * @param trick_type Trick type
 */
void stunt_start_trick(s32 car_index, s32 trick_type) {
    StuntState *st;

    st = &gStunt[car_index];

    st->current_trick.type = (u8)trick_type;
    st->current_trick.completed = 0;
    st->current_trick.start_frame = (s32)frame_counter;
    st->current_trick.rotation = 0.0f;
    st->state = STUNT_STATE_TRICKING;
}

/**
 * stunt_complete_trick - Complete current trick
 *
 * @param car_index Car index
 */
void stunt_complete_trick(s32 car_index) {
    StuntState *st;
    s32 points;

    st = &gStunt[car_index];

    if (st->current_trick.type == TRICK_NONE) {
        return;
    }

    /* Calculate points */
    points = stunt_calc_trick_points(
        st->current_trick.type,
        st->current_trick.rotation,
        st->max_height - st->launch_height,
        st->launch_speed
    );

    st->current_trick.points = points;
    st->current_trick.completed = 1;

    /* Add to combo */
    st->combo_count++;
    st->combo_points += points;
    st->combo_timer = 60;  /* 1 second to continue combo */

    /* Update multiplier */
    st->combo_multiplier = stunt_get_combo_multiplier(st->combo_count);

    /* Play sound */
    sound_play(SFX_CHECKPOINT);  /* Placeholder */

    /* Return to airborne state */
    st->state = STUNT_STATE_AIRBORNE;
    st->current_trick.type = TRICK_NONE;
}

/**
 * stunt_fail_trick - Fail current trick (crash)
 *
 * @param car_index Car index
 */
void stunt_fail_trick(s32 car_index) {
    StuntState *st;

    st = &gStunt[car_index];

    st->current_trick.type = TRICK_NONE;
    st->current_trick.completed = 0;

    /* Reset combo */
    st->combo_count = 0;
    st->combo_points = 0;
    st->combo_multiplier = 1.0f;

    st->crashes++;
}

/**
 * stunt_calc_trick_points - Calculate points for a trick
 *
 * @param trick_type Trick type
 * @param rotation Rotation amount
 * @param height Height above launch
 * @param speed Speed at launch
 * @return Points earned
 */
s32 stunt_calc_trick_points(s32 trick_type, f32 rotation, f32 height, f32 speed) {
    s32 base_points;
    f32 height_bonus;
    f32 speed_bonus;
    f32 rotation_bonus;
    s32 total;

    /* Base points by trick type */
    switch (trick_type) {
        case TRICK_SPIN_LEFT:
        case TRICK_SPIN_RIGHT:
            base_points = TRICK_SPIN_POINTS;
            break;
        case TRICK_FLIP_FORWARD:
        case TRICK_FLIP_BACKWARD:
            base_points = TRICK_FLIP_POINTS;
            break;
        case TRICK_BARREL_LEFT:
        case TRICK_BARREL_RIGHT:
            base_points = TRICK_BARREL_POINTS;
            break;
        case TRICK_CORKSCREW:
            base_points = TRICK_CORKSCREW_POINTS;
            break;
        default:
            base_points = 50;
            break;
    }

    /* Height bonus (1.0 to 2.0) */
    height_bonus = 1.0f + (height / 50.0f);
    if (height_bonus > 2.0f) height_bonus = 2.0f;

    /* Speed bonus (1.0 to 1.5) */
    speed_bonus = 1.0f + (speed / 200.0f);
    if (speed_bonus > 1.5f) speed_bonus = 1.5f;

    /* Rotation bonus for extra rotation */
    rotation_bonus = 1.0f + (ABS(rotation) - 360.0f) / 360.0f * 0.5f;
    if (rotation_bonus < 1.0f) rotation_bonus = 1.0f;
    if (rotation_bonus > 2.0f) rotation_bonus = 2.0f;

    total = (s32)((f32)base_points * height_bonus * speed_bonus * rotation_bonus);

    return total;
}

/**
 * stunt_add_points - Add points to score
 *
 * @param car_index Car index
 * @param points Points to add
 */
void stunt_add_points(s32 car_index, s32 points) {
    StuntState *st;

    st = &gStunt[car_index];
    st->total_score += (u32)points;
}

/**
 * stunt_update_combo - Finalize combo and add points
 *
 * @param car_index Car index
 */
void stunt_update_combo(s32 car_index) {
    StuntState *st;
    s32 final_points;

    st = &gStunt[car_index];

    if (st->combo_count > 0) {
        /* Apply multiplier */
        final_points = (s32)((f32)st->combo_points * st->combo_multiplier);
        stunt_add_points(car_index, final_points);

        /* Check for best combo */
        if ((u32)final_points > st->best_combo) {
            st->best_combo = (u32)final_points;
        }

        st->tricks_landed += st->combo_count;
    }

    /* Reset combo */
    st->combo_count = 0;
    st->combo_points = 0;
    st->combo_multiplier = 1.0f;
}

/**
 * stunt_get_combo_multiplier - Get multiplier for combo count
 *
 * @param combo_count Number of tricks in combo
 * @return Multiplier
 */
f32 stunt_get_combo_multiplier(s32 combo_count) {
    if (combo_count >= 5) return COMBO_MULT_5_PLUS;
    if (combo_count >= 4) return COMBO_MULT_4;
    if (combo_count >= 3) return COMBO_MULT_3;
    if (combo_count >= 2) return COMBO_MULT_2;
    return 1.0f;
}

/**
 * stunt_check_landing_safe - Check if landing is safe
 *
 * @param car_index Car index
 * @return 1 if safe, 0 if crash
 */
s32 stunt_check_landing_safe(s32 car_index) {
    CarData *car;
    f32 pitch, roll;
    f32 abs_pitch, abs_roll;

    car = &car_array[car_index];

    /* Get landing angle */
    pitch = car->pitch;
    roll = car->roll;

    abs_pitch = ABS(pitch);
    abs_roll = ABS(roll);

    /* Check if within safe limits */
    if (abs_pitch < SAFE_LANDING_ANGLE && abs_roll < SAFE_LANDING_ANGLE) {
        return 1;
    }

    /* Definitely crash if too extreme */
    if (abs_pitch > CRASH_LANDING_ANGLE || abs_roll > CRASH_LANDING_ANGLE) {
        return 0;
    }

    /* In between - check velocity direction */
    return 1;  /* Be lenient */
}

/**
 * stunt_land - Successfully land
 *
 * @param car_index Car index
 */
void stunt_land(s32 car_index) {
    StuntState *st;

    st = &gStunt[car_index];

    /* Finalize any pending combo */
    if (st->combo_count > 0 && st->air_time > 0.5f) {
        stunt_update_combo(car_index);
    }

    st->state = STUNT_STATE_GROUNDED;
    st->grounded = 1;
    st->combo_timer = 60;  /* Grace period to continue combo */
}

/**
 * stunt_crash - Crash landing
 *
 * @param car_index Car index
 */
void stunt_crash(s32 car_index) {
    StuntState *st;

    st = &gStunt[car_index];

    st->state = STUNT_STATE_CRASHED;
    st->crashes++;

    /* Lose combo */
    st->combo_count = 0;
    st->combo_points = 0;
    st->combo_multiplier = 1.0f;
    st->combo_timer = 0;

    /* Play crash sound */
    sound_play(SFX_CRASH_HEAVY);
}

/**
 * stunt_is_airborne - Check if car is airborne
 *
 * @param car_index Car index
 * @return 1 if airborne
 */
s32 stunt_is_airborne(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }
    return !gStunt[car_index].grounded;
}

/**
 * stunt_is_tricking - Check if performing trick
 *
 * @param car_index Car index
 * @return 1 if tricking
 */
s32 stunt_is_tricking(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }
    return gStunt[car_index].state == STUNT_STATE_TRICKING;
}

/**
 * stunt_wings_extended - Check if wings are extended
 *
 * @param car_index Car index
 * @return 1 if extended
 */
s32 stunt_wings_extended(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }
    return gStunt[car_index].wing_state == WING_STATE_EXTENDED;
}

/**
 * stunt_get_score - Get total stunt score
 *
 * @param car_index Car index
 * @return Total score
 */
u32 stunt_get_score(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }
    return gStunt[car_index].total_score;
}

/**
 * stunt_get_combo - Get current combo count
 *
 * @param car_index Car index
 * @return Combo count
 */
s32 stunt_get_combo(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }
    return gStunt[car_index].combo_count;
}

/**
 * stunt_get_air_time - Get time in air
 *
 * @param car_index Car index
 * @return Air time in seconds
 */
f32 stunt_get_air_time(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0.0f;
    }
    return gStunt[car_index].air_time;
}

/**
 * stunt_hit_target - Hit a stunt arena target
 *
 * @param car_index Car index
 * @param target_id Target ID
 */
void stunt_hit_target(s32 car_index, s32 target_id) {
    StuntState *st;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    st = &gStunt[car_index];
    st->targets_hit++;

    /* Award bonus points */
    stunt_add_points(car_index, 100 * target_id);

    sound_play(SFX_CHECKPOINT);
}

/**
 * stunt_collect_coin - Collect a stunt arena coin
 *
 * @param car_index Car index
 * @param coin_value Coin value
 */
void stunt_collect_coin(s32 car_index, s32 coin_value) {
    StuntState *st;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    st = &gStunt[car_index];
    st->coins_collected++;

    stunt_add_points(car_index, coin_value);
}

/**
 * stunt_arena_init - Initialize stunt arena mode
 */
void stunt_arena_init(void) {
    gStuntModeEnabled = 1;
    stunt_reset();
}

/**
 * stunt_arena_update - Update stunt arena
 */
void stunt_arena_update(void) {
    stunt_update_all();
}

/**
 * stunt_draw_hud - Draw stunt HUD
 *
 * @param car_index Car index
 */
void stunt_draw_hud(s32 car_index) {
    /* Would draw score, combo meter, etc. */
}

/**
 * stunt_draw_trick_popup - Draw trick name popup
 *
 * @param car_index Car index
 */
void stunt_draw_trick_popup(s32 car_index) {
    /* Would draw trick name when completed */
}

/**
 * stunt_draw_combo_meter - Draw combo meter
 *
 * @param car_index Car index
 */
void stunt_draw_combo_meter(s32 car_index) {
    /* Would draw combo meter */
}

/**
 * stunt_get_trick_name - Get trick name string
 *
 * @param trick_type Trick type
 * @return Trick name
 */
const char* stunt_get_trick_name(s32 trick_type) {
    if (trick_type < 0 || trick_type >= NUM_TRICK_TYPES) {
        return "";
    }
    return gTrickNames[trick_type];
}
