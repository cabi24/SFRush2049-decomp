/**
 * resurrect.h - Car resurrection/respawn system for Rush 2049 N64
 *
 * Based on arcade game/resurrect.c and game/resurrect.h
 * Handles car crashes, respawning, invulnerability, and quaternion interpolation.
 *
 * Arcade source functions implemented:
 * - resurrect_car()            - Main respawn logic
 * - set_resurrection_data()    - Save position for respawn
 * - CheckCrash()               - Detect crash and trigger explosion
 * - check_if_finished_resurrecting() - State machine update
 * - update_resurrecting_car()  - Hand-of-God animation
 * - blink_resurrecting_cars()  - Invulnerability blinking
 * - abort_car()                - Manual abort trigger
 * - jump_to_next_checkpoint()  - Debug teleport
 * - detect_drone_abort()       - Auto-abort stuck drones
 * - are_we_dead_or_dying()     - State query
 * - are_we_solid()             - Collidable query
 *
 * Quaternion helpers:
 * - make_quat_from_uvs()       - Convert rotation matrix to quaternion
 * - make_uvs_from_quat()       - Convert quaternion to rotation matrix
 * - find_best_quat()           - Choose shortest rotation path
 *
 * Math helpers:
 * - interpolate()              - Linear interpolation
 * - dot_prod()                 - Vector dot product
 * - get_normal_vector()        - Cross product with angle
 * - get_major_axis()           - Find largest component
 * - non_linear_scale_setup()   - Parabolic scaling
 */

#ifndef RESURRECT_H
#define RESURRECT_H

#include "types.h"

/*
 * Constants from arcade resurrect.h
 */

/* Pole position spacing (feet) */
#define POLEHDIST       17.0f   /* Horizontal distance between positions */
#define POLEVDIST       20.0f   /* Vertical distance between rows */

/* Resurrection timing constants (milliseconds) */
#define CRASHDEL        3500    /* Delay after crash before respawn */
#define RESURRECT_TIME  1500    /* Duration of hand-of-god animation */
#define INVULN_TIME     5000    /* Invulnerability after respawn (5 sec) */
#define ONE_SEC         1000    /* One second in milliseconds */

/* Resurrection states */
#define RES_NONE        (-1)    /* Normal driving (arcade uses -1) */
#define RES_STARTING    0       /* Starting hand-of-god animation */
#define RES_MOVING      1       /* Being moved by hand-of-god */
#define RES_FINISHED    (-2)    /* Animation complete */

/* Resurrection data modes (for set_resurrection_data) */
#define Initialize      0       /* Set data to checkpoint position */
#define Do_it           1       /* Save current position for respawn */

/* Appearance flags from arcade */
#define App_M_TRANSLUCENT   0x0001  /* Car is translucent */
#define App_M_HULK          0x0002  /* Car is destroyed hulk */
#define App_M_ANY_DAMAGE    0x00F0  /* Any damage bits */

/* Shortcut/yank flags */
#define S_CUT_MASK          0x0100  /* In shortcut */
#define SC_DIST_MASK        0x00FF  /* Shortcut distance bits */
#define YANK_MASK           0x3000  /* Yank time bits */
#define YANK_SHIFT          12      /* Shift for yank time */
#define NOWRONGWAY_MASK     0x4000  /* No wrong-way check */

/**
 * ResurrectData - Per-car resurrection state
 * Based on arcade MODELDAT.resurrect fields
 * Tracks crash timing, respawn position, quaternion animation,
 * and invulnerability state.
 */
typedef struct ResurrectData {
    /* Saved crash position (in world coords) */
    f32     save_pos[3];        /* Position when crashed */
    f32     uvs[3][3];          /* Orientation when crashed (or respawn target) */

    /* Respawn target position */
    f32     pos[3];             /* Calculated respawn position */
    f32     velocity;           /* Speed at respawn */

    /* Quaternion interpolation for smooth rotation */
    f32     quat_start[4];      /* Starting quaternion (crash orientation) */
    f32     quat_end[4];        /* Target quaternion (track orientation) */

    /* Timing (milliseconds) */
    s32     crash_time;         /* Time of crash (-1 = init mode) */
    s32     resurrect_time;     /* Time respawn started */

    /* State */
    s32     moving_state;       /* Animation state (RES_NONE, RES_STARTING, etc) */
} ResurrectData;

/* Global data */
extern f32 save_pos[][3];                   /* Intermediate positions during animation */
extern const f32 pole_pos_offset[][3];      /* Start/respawn grid positions */
extern const s16 major_axis[][2];           /* Axis lookup for cross product */

/*
 * Core resurrection functions (from arcade resurrect.c)
 */

/* Main resurrection logic - respawn car at calculated position */
void resurrect_car(s32 car_index);

/* Save current position for respawn calculation */
/* mode: Initialize (0) = checkpoint, Do_it (1) = crash */
void set_resurrection_data(s32 mode, s32 car_index);

/* Check for crash and start explosion sequence */
void CheckCrash(s32 car_index);

/* Update resurrection state machine (call each frame) */
void check_if_finished_resurrecting(void);

/* Move resurrecting car toward destination (hand-of-god animation) */
void update_resurrecting_car(s32 car_index);

/* Update transparency blinking for invulnerable cars */
void blink_resurrecting_cars(void);

/* Manual abort (player pressed abort button) */
void abort_car(s32 car_index);

/* Debug: teleport to next checkpoint */
void jump_to_next_checkpoint(void);

/* Check if drones need auto-abort due to lack of progress */
void detect_drone_abort(void);

/*
 * State query functions
 */

/* Is car dead, dying, or being resurrected? */
s32 are_we_dead_or_dying(s32 car_index);

/* Is car solid and collidable (not dead/respawning)? */
s32 are_we_solid(s32 car_index);

/*
 * Quaternion functions (for smooth orientation interpolation)
 */

/* Convert 3x3 rotation matrix to quaternion */
void make_quat_from_uvs(f32 uvs[3][3], f32 q[4]);

/* Convert quaternion to 3x3 rotation matrix */
void make_uvs_from_quat(f32 q[4], f32 uvs[3][3]);

/* Choose equivalent quaternion closest to first (shortest rotation) */
void find_best_quat(f32 *q1, f32 *q2);

/*
 * Math helper functions
 */

/* Linear interpolation between two point arrays */
void interpolate(f32 *pointa, f32 *pointb, s32 p_size,
                 f32 vala, f32 valb, f32 inab, f32 *pointab);

/* Dot product of two 3-element vectors */
f32 dot_prod(f32 *vec1, f32 *vec2);

/* Cross product with angle calculation between vectors */
f32 get_normal_vector(f32 *vec1, f32 *vec2, f32 *nvec);

/* Find index of largest component in vector */
s32 get_major_axis(f32 *vec);

/* Setup coefficients for parabolic scale: y = ax^2 + bx + c */
void non_linear_scale_setup(f32 a, f32 *b, f32 *c,
                            f32 x1, f32 y1, f32 x2, f32 y2);

/*
 * Initialization (N64-specific)
 */
void resurrect_init(void);
void resurrect_init_car(s32 car_index);

/*
 * Compatibility wrappers (match existing N64 code style)
 */
s32 is_car_dead(s32 car_index);
s32 is_car_invulnerable(s32 car_index);
u8 get_respawn_alpha(s32 car_index);

#endif /* RESURRECT_H */
