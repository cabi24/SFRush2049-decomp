/**
 * stunt.h - Stunt mode system for Rush 2049 N64
 *
 * Rush 2049's signature feature: deployable wings and trick scoring.
 * Players can extend wings for aerial control and perform tricks
 * off ramps and jumps to earn points.
 *
 * Trick Types:
 * - Spins (horizontal rotation)
 * - Flips (forward/backward rotation)
 * - Barrel Rolls (combined spin + flip)
 * - Wing tricks (extend/retract timing)
 */

#ifndef STUNT_H
#define STUNT_H

#include "types.h"
#include "game/structs.h"  /* For MAX_CARS */

/* Stunt mode states */
#define STUNT_STATE_DISABLED    0   /* Stunt mode not active */
#define STUNT_STATE_GROUNDED    1   /* On ground, ready */
#define STUNT_STATE_AIRBORNE    2   /* In the air, can trick */
#define STUNT_STATE_TRICKING    3   /* Performing a trick */
#define STUNT_STATE_LANDING     4   /* About to land */
#define STUNT_STATE_CRASHED     5   /* Crashed during trick */

/* Wing states */
#define WING_STATE_RETRACTED    0   /* Wings hidden */
#define WING_STATE_EXTENDING    1   /* Wings deploying */
#define WING_STATE_EXTENDED     2   /* Wings fully out */
#define WING_STATE_RETRACTING   3   /* Wings folding back */

/* Trick types */
#define TRICK_NONE              0
#define TRICK_SPIN_LEFT         1   /* 360 spin left */
#define TRICK_SPIN_RIGHT        2   /* 360 spin right */
#define TRICK_FLIP_FORWARD      3   /* Front flip */
#define TRICK_FLIP_BACKWARD     4   /* Back flip */
#define TRICK_BARREL_LEFT       5   /* Barrel roll left */
#define TRICK_BARREL_RIGHT      6   /* Barrel roll right */
#define TRICK_CORKSCREW         7   /* Corkscrew (advanced) */
#define TRICK_WING_FLASH        8   /* Quick wing extend/retract */
#define NUM_TRICK_TYPES         9

/* Trick multipliers */
#define TRICK_SPIN_POINTS       100     /* Base points for spin */
#define TRICK_FLIP_POINTS       150     /* Base points for flip */
#define TRICK_BARREL_POINTS     250     /* Base points for barrel roll */
#define TRICK_CORKSCREW_POINTS  500     /* Base points for corkscrew */
#define TRICK_WING_BONUS        50      /* Bonus for wing timing */

/* Trick combo multipliers */
#define COMBO_MULT_2            1.5f    /* 2 tricks = 1.5x */
#define COMBO_MULT_3            2.0f    /* 3 tricks = 2.0x */
#define COMBO_MULT_4            3.0f    /* 4 tricks = 3x */
#define COMBO_MULT_5_PLUS       5.0f    /* 5+ tricks = 5x */

/* Wing physics constants */
#define WING_EXTEND_TIME        15      /* Frames to extend */
#define WING_LIFT_FACTOR        0.3f    /* Lift multiplier */
#define WING_DRAG_FACTOR        0.1f    /* Drag multiplier */
#define WING_CONTROL_FACTOR     0.5f    /* Air control with wings */

/* Height thresholds */
#define MIN_TRICK_HEIGHT        3.0f    /* Minimum height to start trick */
#define SAFE_LANDING_ANGLE      30.0f   /* Max angle for safe landing */
#define CRASH_LANDING_ANGLE     60.0f   /* Angle that causes crash */

/* Rotation thresholds (degrees) */
#define SPIN_THRESHOLD          330.0f  /* Degrees for spin to count */
#define FLIP_THRESHOLD          330.0f  /* Degrees for flip to count */

/* Per-trick tracking */
typedef struct TrickData {
    u8      type;               /* TRICK_* type */
    u8      completed;          /* Trick completed? */
    u8      pad[2];
    f32     rotation;           /* Rotation accumulated */
    f32     start_angle;        /* Starting angle */
    s32     start_frame;        /* Frame trick started */
    s32     points;             /* Points earned */
} TrickData;

/* Stunt state for one car */
typedef struct StuntState {
    /* Mode state */
    u8      state;              /* STUNT_STATE_* */
    u8      wing_state;         /* WING_STATE_* */
    u8      wing_input;         /* Wing button held? */
    u8      grounded;           /* Is car on ground? */

    /* Wing deployment */
    f32     wing_extension;     /* 0.0 = retracted, 1.0 = extended */
    f32     wing_target;        /* Target extension */
    f32     wing_speed;         /* Extension speed */

    /* Air state */
    f32     air_time;           /* Time in air (seconds) */
    f32     max_height;         /* Maximum height reached */
    f32     launch_height;      /* Height at takeoff */
    f32     launch_speed;       /* Speed at takeoff */

    /* Rotation tracking */
    f32     spin_total;         /* Total horizontal rotation */
    f32     flip_total;         /* Total vertical rotation */
    f32     roll_total;         /* Total roll rotation */
    f32     last_yaw;           /* Previous yaw for delta */
    f32     last_pitch;         /* Previous pitch for delta */
    f32     last_roll;          /* Previous roll for delta */

    /* Current trick */
    TrickData current_trick;

    /* Combo tracking */
    s32     combo_count;        /* Tricks in current combo */
    s32     combo_points;       /* Points in current combo */
    f32     combo_multiplier;   /* Current multiplier */
    u32     combo_timer;        /* Frames since last trick */

    /* Session stats */
    u32     total_score;        /* Total stunt points */
    u32     best_combo;         /* Best combo achieved */
    s32     tricks_landed;      /* Total tricks landed */
    s32     crashes;            /* Crash count */

    /* Stunt arena specific */
    s32     targets_hit;        /* Targets collected */
    s32     coins_collected;    /* Coins collected */

} StuntState;

/* Global stunt state per car */
extern StuntState gStunt[MAX_CARS];

/* Stunt mode enabled flag */
extern s32 gStuntModeEnabled;

/* Trick names for display */
extern const char *gTrickNames[];

/* Initialization */
void stunt_init(void);
void stunt_reset(void);
void stunt_init_car(s32 car_index);

/* Per-frame update */
void stunt_update(s32 car_index);
void stunt_update_all(void);

/* Wing control */
void stunt_wing_input(s32 car_index, s32 pressed);
void stunt_extend_wings(s32 car_index);
void stunt_retract_wings(s32 car_index);
void stunt_update_wings(s32 car_index);
f32 stunt_get_wing_extension(s32 car_index);

/* Wing physics effects */
void stunt_apply_wing_lift(s32 car_index, f32 *lift);
void stunt_apply_wing_drag(s32 car_index, f32 *drag);
void stunt_apply_air_control(s32 car_index, f32 steer_input, f32 *steer_force);

/* Trick detection */
void stunt_check_airborne(s32 car_index);
void stunt_check_landing(s32 car_index);
void stunt_update_rotation(s32 car_index);
s32 stunt_detect_trick(s32 car_index);

/* Trick execution */
void stunt_start_trick(s32 car_index, s32 trick_type);
void stunt_complete_trick(s32 car_index);
void stunt_fail_trick(s32 car_index);

/* Scoring */
s32 stunt_calc_trick_points(s32 trick_type, f32 rotation, f32 height, f32 speed);
void stunt_add_points(s32 car_index, s32 points);
void stunt_update_combo(s32 car_index);
f32 stunt_get_combo_multiplier(s32 combo_count);

/* Landing */
s32 stunt_check_landing_safe(s32 car_index);
void stunt_land(s32 car_index);
void stunt_crash(s32 car_index);

/* Queries */
s32 stunt_is_airborne(s32 car_index);
s32 stunt_is_tricking(s32 car_index);
s32 stunt_wings_extended(s32 car_index);
u32 stunt_get_score(s32 car_index);
s32 stunt_get_combo(s32 car_index);
f32 stunt_get_air_time(s32 car_index);

/* Stunt arena */
void stunt_hit_target(s32 car_index, s32 target_id);
void stunt_collect_coin(s32 car_index, s32 coin_value);
void stunt_arena_init(void);
void stunt_arena_update(void);

/* Display */
void stunt_draw_hud(s32 car_index);
void stunt_draw_trick_popup(s32 car_index);
void stunt_draw_combo_meter(s32 car_index);
const char* stunt_get_trick_name(s32 trick_type);

#endif /* STUNT_H */
