/**
 * resurrect.h - Car resurrection/respawn system for Rush 2049 N64
 *
 * Based on arcade resurrect.c
 * Handles car crashes, respawning, and invulnerability
 */

#ifndef RESURRECT_H
#define RESURRECT_H

#include "types.h"

/* Resurrection timing constants (milliseconds) */
#define CRASHDEL        3500    /* Delay after crash before respawn */
#define RESURRECT_TIME  1500    /* Duration of respawn animation */
#define INVULN_TIME     2000    /* Invulnerability duration after respawn */

/* Resurrection states */
#define RES_NONE        0       /* Normal driving */
#define RES_CRASHING    1       /* Currently crashing/exploding */
#define RES_DEAD        2       /* Waiting to respawn */
#define RES_RESPAWNING  3       /* Respawn animation in progress */
#define RES_INVULN      4       /* Invulnerable after respawn */

/* Resurrection data modes */
#define RES_MODE_CHECKPOINT  0  /* Save at checkpoint */
#define RES_MODE_START       1  /* Save at race start */
#define RES_MODE_MANUAL      2  /* Manual save point */

/**
 * ResurrectData - Per-car resurrection state
 * Tracks crash timing, respawn position, and invulnerability
 */
typedef struct ResurrectData {
    /* Last safe position (saved at checkpoints) */
    f32     save_pos[3];        /* Saved world position */
    f32     save_uvs[3][3];     /* Saved orientation matrix */

    /* Respawn target position */
    f32     respawn_pos[3];     /* Where to respawn */
    f32     respawn_uvs[3][3];  /* Respawn orientation */
    f32     velocity;           /* Speed at respawn */

    /* Timing */
    u32     crash_time;         /* When crash started (ms) */
    u32     resurrect_time;     /* When respawn started (ms) */

    /* State */
    s32     state;              /* Current resurrection state */
    s16     last_cp_index;      /* Last checkpoint passed */
    s16     next_cp_index;      /* Next checkpoint to pass */
    s16     pole_pos;           /* Grid position (0-7) */
    s16     pad;
} ResurrectData;

/* External resurrection data array */
extern ResurrectData res_data[];

/* Pole position offsets for starting grid */
extern const f32 pole_pos_offset[][2];

/* Initialization */
void resurrect_init(void);
void resurrect_init_car(s32 car_index);

/* Save/restore position */
void set_resurrection_data(s32 car_index, s32 mode);
void calculate_respawn_position(s32 car_index);
void get_start_position(s32 car_index, s32 pole_pos, f32 pos[3], f32 uvs[3][3]);

/* Crash handling */
void start_crash(s32 car_index, u32 crash_time);
void resurrect_car(s32 car_index);
void check_if_finished_resurrecting(void);
void abort_car(s32 car_index);

/* State queries */
s32 is_car_dead(s32 car_index);
s32 is_car_invulnerable(s32 car_index);
s32 is_car_respawning(s32 car_index);
u8 get_respawn_alpha(s32 car_index);

/* Damage */
void heal_damage(s32 car_index);

#endif /* RESURRECT_H */
