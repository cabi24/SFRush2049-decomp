/**
 * crash.h - Crash and resurrect handling for Rush 2049 N64
 *
 * Based on arcade game/resurrect.c
 * Handles crash detection, respawning, and damage tracking.
 */

#ifndef CRASH_H
#define CRASH_H

#include "types.h"

/* Crash states */
#define CRASH_STATE_NONE        0   /* Normal driving */
#define CRASH_STATE_SPINNING    1   /* Spinning out */
#define CRASH_STATE_FLIPPING    2   /* Flipping over */
#define CRASH_STATE_AIRBORNE    3   /* Airborne tumble */
#define CRASH_STATE_EXPLODING   4   /* Explosion animation */
#define CRASH_STATE_WAITING     5   /* Waiting for respawn */
#define CRASH_STATE_RESPAWNING  6   /* Respawn fade-in */
#define NUM_CRASH_STATES        7

/* Crash causes */
#define CRASH_CAUSE_NONE        0
#define CRASH_CAUSE_COLLISION   1   /* Car-car collision */
#define CRASH_CAUSE_WALL        2   /* Wall impact */
#define CRASH_CAUSE_FLIP        3   /* Flipped over */
#define CRASH_CAUSE_FALL        4   /* Fell off track */
#define CRASH_CAUSE_WATER       5   /* Drove into water */
#define CRASH_CAUSE_HAZARD      6   /* Hit hazard */
#define CRASH_CAUSE_EXPLOSION   7   /* Battle mode weapon */
#define NUM_CRASH_CAUSES        8

/* Damage levels */
#define DAMAGE_LEVEL_NONE       0   /* No damage */
#define DAMAGE_LEVEL_SCRATCH    1   /* Minor scratches */
#define DAMAGE_LEVEL_DENT       2   /* Visible dents */
#define DAMAGE_LEVEL_HEAVY      3   /* Heavy damage */
#define DAMAGE_LEVEL_CRITICAL   4   /* About to crash */
#define DAMAGE_LEVEL_DESTROYED  5   /* Total destruction */
#define NUM_DAMAGE_LEVELS       6

/* Timing constants (from arcade resurrect.h) */
#define CRASH_DELAY_MS          3500    /* Delay before respawn (ms) */
#define RESURRECT_TIME_MS       1500    /* Respawn animation time (ms) */
#define CRASH_DELAY_FRAMES      210     /* 3.5 seconds at 60fps */
#define RESURRECT_FRAMES        90      /* 1.5 seconds at 60fps */
#define INVINCIBLE_FRAMES       120     /* 2 seconds post-respawn */

/* Respawn distances */
#define POLE_H_DIST             17.0f   /* Horizontal distance for pole search */
#define POLE_V_DIST             20.0f   /* Vertical distance for pole search */

/* Damage thresholds */
#define DAMAGE_THRESHOLD_CRASH  100.0f  /* Damage level that triggers crash */
#define DAMAGE_PER_IMPACT       10.0f   /* Base damage per collision */
#define DAMAGE_SPEED_MULT       0.5f    /* Damage multiplier for speed */

/* Crash physics */
typedef struct CrashPhysics {
    f32     velocity[3];        /* Crash velocity */
    f32     angular[3];         /* Angular velocity (tumble) */
    f32     position[3];        /* Current crash position */
    f32     rotation[3];        /* Current tumble rotation */
    f32     bounce_count;       /* Number of bounces */
    f32     ground_timer;       /* Time on ground */
} CrashPhysics;

/* Respawn point */
typedef struct RespawnPoint {
    f32     position[3];        /* World position */
    f32     rotation;           /* Facing angle */
    s32     checkpoint;         /* Associated checkpoint */
    u8      active;
    u8      pad[3];
} RespawnPoint;

/* Per-car crash state */
typedef struct CrashData {
    /* State */
    u8      state;              /* CRASH_STATE_* */
    u8      cause;              /* CRASH_CAUSE_* */
    u8      damage_level;       /* DAMAGE_LEVEL_* */
    u8      invincible;         /* Post-respawn invincibility */

    /* Damage tracking */
    f32     damage;             /* Current damage (0-100) */
    f32     max_damage;         /* Maximum damage before crash */
    f32     last_impact;        /* Last impact force */

    /* Timing */
    f32     state_timer;        /* Timer for current state */
    f32     crash_timer;        /* Total crash duration */
    f32     invincible_timer;   /* Remaining invincibility */

    /* Crash physics */
    CrashPhysics physics;

    /* Respawn data */
    RespawnPoint respawn;       /* Where to respawn */
    f32     respawn_alpha;      /* Fade-in alpha */

    /* Statistics */
    s32     crash_count;        /* Total crashes this race */
    s32     flip_count;         /* Times flipped */
    s32     fall_count;         /* Times fell off track */
    f32     total_damage;       /* Cumulative damage taken */

} CrashData;

/* Crash system state */
typedef struct CrashSystem {
    /* Per-car crash data */
    CrashData cars[8];
    s32     num_cars;

    /* Respawn points (from track) */
    RespawnPoint respawn_points[32];
    s32     num_respawn_points;

    /* Settings */
    u8      enabled;            /* Crash system enabled */
    u8      damage_enabled;     /* Damage accumulation enabled */
    u8      auto_respawn;       /* Automatic respawn */
    u8      instant_respawn;    /* Debug: instant respawn */

    f32     damage_mult;        /* Damage multiplier */
    f32     crash_delay;        /* Delay before respawn */

} CrashSystem;

/* Global crash system */
extern CrashSystem gCrash;

/* Initialization */
void crash_init(void);
void crash_reset(void);
void crash_init_car(s32 car);

/* Track respawn points */
void crash_load_respawn_points(void *track_data);
void crash_clear_respawn_points(void);
s32 crash_add_respawn_point(f32 *pos, f32 rotation, s32 checkpoint);
RespawnPoint* crash_get_nearest_respawn(f32 *pos);
RespawnPoint* crash_get_checkpoint_respawn(s32 checkpoint);

/* Update */
void crash_update(void);
void crash_update_car(s32 car);
void crash_update_physics(s32 car);
void crash_update_respawn(s32 car);

/* Crash triggering */
void crash_trigger(s32 car, s32 cause);
void crash_from_collision(s32 car, f32 impact_force, f32 *impact_dir);
void crash_from_flip(s32 car);
void crash_from_fall(s32 car);
void crash_from_water(s32 car);
void crash_from_explosion(s32 car);

/* Damage */
void crash_apply_damage(s32 car, f32 amount);
void crash_set_damage(s32 car, f32 damage);
f32 crash_get_damage(s32 car);
s32 crash_get_damage_level(s32 car);
void crash_repair(s32 car, f32 amount);
void crash_full_repair(s32 car);

/* Respawn */
void crash_begin_respawn(s32 car);
void crash_complete_respawn(s32 car);
void crash_set_respawn_point(s32 car, f32 *pos, f32 rotation);
void crash_force_respawn(s32 car);
s32 crash_is_respawning(s32 car);

/* Queries */
s32 crash_is_crashed(s32 car);
s32 crash_is_invincible(s32 car);
s32 crash_get_state(s32 car);
s32 crash_get_cause(s32 car);
f32 crash_get_alpha(s32 car);

/* Settings */
void crash_set_enabled(u8 enabled);
void crash_set_damage_enabled(u8 enabled);
void crash_set_auto_respawn(u8 auto_respawn);
void crash_set_damage_mult(f32 mult);
void crash_set_crash_delay(f32 delay);

/* Visual effects */
void crash_spawn_effects(s32 car, s32 cause);
void crash_draw_explosion(s32 car);
void crash_draw_respawn(s32 car);

/* Debug */
void crash_debug_print(s32 car);

#endif /* CRASH_H */
