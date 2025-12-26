/**
 * resurrect.c - Car resurrection/respawn system for Rush 2049 N64
 *
 * Based on arcade game/resurrect.c
 * Handles car respawn after crash, position calculation, and animations.
 *
 * Key concepts:
 * - Crash detection triggers death state
 * - After delay, car respawns on track near crash point
 * - Respawn position calculated from checkpoint and path data
 * - Car fades in with invulnerability period
 */

#include "types.h"
#include "game/structs.h"

/* External game state */
extern u8 gstate;
extern u32 frame_counter;
extern CarData car_array[];
extern s32 num_active_cars;
extern s32 this_car;
extern s32 trackno;

/* External checkpoint data */
extern u32 number_checkpoints;

/* External math functions */
extern f32 sqrtf(f32 x);

/******* CONSTANTS *******/
/* From arcade: resurrect.h */

#define POLEHDIST       17.0f   /* Horizontal distance between pole positions */
#define POLEVDIST       20.0f   /* Vertical distance between pole positions */
#define CRASHDEL        3500    /* Crash delay in milliseconds */
#define RESURRECT_TIME  1500    /* Resurrection animation time in ms */
#define INVULN_TIME     2000    /* Invulnerability time after respawn in ms */

/* Resurrection states */
#define RES_NONE        0       /* Normal driving */
#define RES_CRASHING    1       /* Currently crashing/exploding */
#define RES_DEAD        2       /* Waiting to respawn */
#define RES_RESPAWNING  3       /* Fading back in */
#define RES_INVULN      4       /* Invulnerable after respawn */

/******* DATA STRUCTURES *******/

/**
 * ResurrectData - Per-car resurrection state
 * Based on arcade: MODELDAT resurrection fields
 */
typedef struct ResurrectData {
    f32     save_pos[3];        /* Position when crashed */
    f32     save_uvs[3][3];     /* Orientation when crashed */
    f32     respawn_pos[3];     /* Calculated respawn position */
    f32     respawn_uvs[3][3];  /* Calculated respawn orientation */
    f32     velocity;           /* Speed at respawn */
    u32     crash_time;         /* Time of crash (ms) */
    u32     resurrect_time;     /* Time of respawn start */
    s32     state;              /* Current resurrection state */
    s16     last_cp_index;      /* Last checkpoint before crash */
    s16     next_cp_index;      /* Next checkpoint before crash */
    s16     pole_pos;           /* Pole position for start/respawn offset */
    s16     pad;
} ResurrectData;

/* Resurrection state for all cars */
ResurrectData resurrect_data[MAX_CARS];

/**
 * Pole position offsets - starting grid and respawn positions
 * Based on arcade: pole_pos_offset table
 * First 8 entries are for race start, next 8 for resurrection
 */
static const f32 pole_pos_offset[16][3] = {
    /* Starting grid positions (staggered 2-wide) */
    { POLEVDIST *  0.0f, POLEHDIST *  0.0f, 0.0f },
    { POLEVDIST *  0.0f, POLEHDIST *  1.0f, 0.0f },
    { POLEVDIST * -1.0f, POLEHDIST * -0.5f, 0.0f },
    { POLEVDIST * -1.0f, POLEHDIST *  0.5f, 0.0f },
    { POLEVDIST * -2.0f, POLEHDIST *  0.0f, 0.0f },
    { POLEVDIST * -2.0f, POLEHDIST *  1.0f, 0.0f },
    { POLEVDIST * -3.0f, POLEHDIST * -0.5f, 0.0f },
    { POLEVDIST * -3.0f, POLEHDIST *  0.5f, 0.0f },
    /* Resurrection positions (spread across track width) */
    { POLEVDIST *  0.0f, POLEHDIST * -1.4f, 0.0f },
    { POLEVDIST *  0.0f, POLEHDIST * -1.0f, 0.0f },
    { POLEVDIST *  0.0f, POLEHDIST * -0.6f, 0.0f },
    { POLEVDIST *  0.0f, POLEHDIST * -0.2f, 0.0f },
    { POLEVDIST *  0.0f, POLEHDIST *  0.2f, 0.0f },
    { POLEVDIST *  0.0f, POLEHDIST *  0.6f, 0.0f },
    { POLEVDIST *  0.0f, POLEHDIST *  1.0f, 0.0f },
    { POLEVDIST *  0.0f, POLEHDIST *  1.4f, 0.0f }
};

/******* FUNCTIONS *******/

/**
 * resurrect_init - Initialize resurrection system
 */
void resurrect_init(void) {
    s32 i, j;

    for (i = 0; i < MAX_CARS; i++) {
        resurrect_data[i].state = RES_NONE;
        resurrect_data[i].crash_time = 0;
        resurrect_data[i].resurrect_time = 0;
        resurrect_data[i].velocity = 0.0f;
        resurrect_data[i].last_cp_index = 0;
        resurrect_data[i].next_cp_index = 1;
        resurrect_data[i].pole_pos = i;

        for (j = 0; j < 3; j++) {
            resurrect_data[i].save_pos[j] = 0.0f;
            resurrect_data[i].respawn_pos[j] = 0.0f;
        }
    }
}

/**
 * resurrect_init_car - Initialize resurrection state for one car
 *
 * @param car_index Car index
 */
void resurrect_init_car(s32 car_index) {
    ResurrectData *res;
    s32 j;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    res = &resurrect_data[car_index];
    res->state = RES_NONE;
    res->crash_time = 0;
    res->resurrect_time = 0;
    res->velocity = 0.0f;
    res->pole_pos = car_index;

    for (j = 0; j < 3; j++) {
        res->save_pos[j] = 0.0f;
        res->respawn_pos[j] = 0.0f;
    }
}

/**
 * set_resurrection_data - Save current position for respawn calculation
 * Based on arcade: resurrect.c:set_resurrection_data()
 *
 * Called when car crashes to save position for respawn.
 *
 * @param car_index Car index
 * @param mode 0 = Initialize (race start), 1 = Do_it (crash)
 */
void set_resurrection_data(s32 car_index, s32 mode) {
    ResurrectData *res;
    CarData *car;
    s32 i, j;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    res = &resurrect_data[car_index];
    car = &car_array[car_index];

    if (mode == 0) {
        /* Initialize - set pole position */
        res->pole_pos = car_index;
    }

    /* Save current position and orientation */
    for (i = 0; i < 3; i++) {
        res->save_pos[i] = car->dr_pos[i];
        for (j = 0; j < 3; j++) {
            res->save_uvs[i][j] = car->dr_uvs[i][j];
        }
    }

    /* Save checkpoint indices */
    res->last_cp_index = car->checkpoint;
    res->next_cp_index = car->checkpoint + 1;
    if (res->next_cp_index >= (s16)number_checkpoints) {
        res->next_cp_index = 0;
    }
}

/**
 * start_crash - Begin crash/death sequence
 * Based on arcade: resurrect.c crash handling
 *
 * @param car_index Car that crashed
 * @param crash_time Current time in milliseconds
 */
void start_crash(s32 car_index, u32 crash_time) {
    ResurrectData *res;
    CarData *car;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    res = &resurrect_data[car_index];
    car = &car_array[car_index];

    /* Only start if not already crashing */
    if (res->state != RES_NONE) {
        return;
    }

    /* Save position for respawn calculation */
    set_resurrection_data(car_index, 1);

    /* Start crash sequence */
    res->state = RES_CRASHING;
    res->crash_time = crash_time;

    /* Mark car as crashed */
    car->crashflag = 1;
}

/**
 * calculate_respawn_position - Calculate where to respawn car
 * Based on arcade: resurrect.c:resurrect_car() position calculation
 *
 * Uses checkpoint and path data to find appropriate respawn point.
 *
 * @param car_index Car index
 */
void calculate_respawn_position(s32 car_index) {
    ResurrectData *res;
    CarData *car;
    f32 offset[3];
    s32 pole_idx;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    res = &resurrect_data[car_index];
    car = &car_array[car_index];

    /* Get pole position offset (use resurrection offsets, indices 8-15) */
    pole_idx = 8 + (res->pole_pos & 0x7);
    offset[0] = pole_pos_offset[pole_idx][0];
    offset[1] = pole_pos_offset[pole_idx][1];
    offset[2] = pole_pos_offset[pole_idx][2];

    /* For now, respawn near saved position with offset */
    /* Full implementation would use path/maxpath data */
    res->respawn_pos[0] = res->save_pos[0] + offset[0];
    res->respawn_pos[1] = res->save_pos[1] + offset[1];
    res->respawn_pos[2] = res->save_pos[2] + offset[2] + 5.0f; /* Raise up */

    /* Copy orientation (would calculate from path direction) */
    /* For now, keep saved orientation */
    res->respawn_uvs[0][0] = res->save_uvs[0][0];
    res->respawn_uvs[0][1] = res->save_uvs[0][1];
    res->respawn_uvs[0][2] = res->save_uvs[0][2];
    res->respawn_uvs[1][0] = res->save_uvs[1][0];
    res->respawn_uvs[1][1] = res->save_uvs[1][1];
    res->respawn_uvs[1][2] = res->save_uvs[1][2];
    res->respawn_uvs[2][0] = res->save_uvs[2][0];
    res->respawn_uvs[2][1] = res->save_uvs[2][1];
    res->respawn_uvs[2][2] = res->save_uvs[2][2];

    /* Set respawn velocity based on track speed */
    res->velocity = 50.0f;  /* Placeholder - would use maxpath speed */
}

/**
 * resurrect_car - Respawn car at calculated position
 * Based on arcade: resurrect.c:resurrect_car()
 *
 * @param car_index Car index
 */
void resurrect_car(s32 car_index) {
    ResurrectData *res;
    CarData *car;
    s32 i, j;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    res = &resurrect_data[car_index];
    car = &car_array[car_index];

    /* Calculate respawn position */
    calculate_respawn_position(car_index);

    /* Move car to respawn position */
    for (i = 0; i < 3; i++) {
        car->dr_pos[i] = res->respawn_pos[i];
        car->RWR[i] = res->respawn_pos[i];
        car->RWV[i] = 0.0f;  /* Zero velocity */

        for (j = 0; j < 3; j++) {
            car->dr_uvs[i][j] = res->respawn_uvs[i][j];
        }
    }

    /* Set forward velocity */
    car->RWV[0] = res->velocity;

    /* Clear crash state */
    car->crashflag = 0;

    /* Start respawn animation */
    res->state = RES_RESPAWNING;
}

/**
 * check_if_finished_resurrecting - Update resurrection state
 * Based on arcade: mdrive.c:check_if_finished_resurrecting()
 *
 * Called each frame to update resurrection animations.
 */
void check_if_finished_resurrecting(void) {
    ResurrectData *res;
    CarData *car;
    u32 current_time;
    u32 elapsed;
    s32 i;

    /* Would get current time from timer system */
    current_time = frame_counter * 16;  /* Approximate ms */

    for (i = 0; i < num_active_cars; i++) {
        res = &resurrect_data[i];
        car = &car_array[i];

        switch (res->state) {
            case RES_NONE:
                /* Normal state - nothing to do */
                break;

            case RES_CRASHING:
                /* Check if crash delay expired */
                elapsed = current_time - res->crash_time;
                if (elapsed >= CRASHDEL) {
                    res->state = RES_DEAD;
                }
                break;

            case RES_DEAD:
                /* Ready to respawn */
                resurrect_car(i);
                res->resurrect_time = current_time;
                break;

            case RES_RESPAWNING:
                /* Check if respawn animation done */
                elapsed = current_time - res->resurrect_time;
                if (elapsed >= RESURRECT_TIME) {
                    res->state = RES_INVULN;
                }
                break;

            case RES_INVULN:
                /* Check if invulnerability expired */
                elapsed = current_time - res->resurrect_time;
                if (elapsed >= RESURRECT_TIME + INVULN_TIME) {
                    res->state = RES_NONE;
                    /* Make car collidable again */
                    car->collidable = 1;
                }
                break;
        }
    }
}

/**
 * is_car_dead - Check if car is in death/respawn state
 *
 * @param car_index Car index
 * @return Non-zero if car is dead or respawning
 */
s32 is_car_dead(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }

    return (resurrect_data[car_index].state != RES_NONE);
}

/**
 * is_car_invulnerable - Check if car is in invulnerability period
 *
 * @param car_index Car index
 * @return Non-zero if car is invulnerable
 */
s32 is_car_invulnerable(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }

    return (resurrect_data[car_index].state == RES_INVULN ||
            resurrect_data[car_index].state == RES_RESPAWNING);
}

/**
 * get_respawn_alpha - Get transparency for respawn fade-in effect
 *
 * @param car_index Car index
 * @return Alpha value 0-255 (0=invisible, 255=solid)
 */
u8 get_respawn_alpha(s32 car_index) {
    ResurrectData *res;
    u32 current_time;
    u32 elapsed;
    f32 alpha;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return 255;
    }

    res = &resurrect_data[car_index];
    current_time = frame_counter * 16;

    if (res->state == RES_RESPAWNING) {
        elapsed = current_time - res->resurrect_time;
        alpha = (f32)elapsed / (f32)RESURRECT_TIME;
        if (alpha > 1.0f) alpha = 1.0f;
        return (u8)(alpha * 255.0f);
    }

    if (res->state == RES_INVULN) {
        /* Blink effect during invulnerability */
        if ((current_time / 100) & 1) {
            return 200;
        }
        return 255;
    }

    return 255;
}

/**
 * abort_car - Manually abort/reset car (abort button pressed)
 * Based on arcade: resurrect.c abort handling
 *
 * @param car_index Car index
 */
void abort_car(s32 car_index) {
    u32 current_time;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    current_time = frame_counter * 16;
    start_crash(car_index, current_time);
}

/**
 * get_start_position - Get starting grid position for car
 *
 * @param pole_position Pole position (0 = first place)
 * @param pos Output position array
 * @param uvs Output orientation matrix
 */
void get_start_position(s32 pole_position, f32 pos[3], f32 uvs[3][3]) {
    s32 idx;

    idx = pole_position & 0x7;  /* Clamp to 0-7 */

    /* Return offset from pole_pos_offset (first 8 entries) */
    pos[0] = pole_pos_offset[idx][0];
    pos[1] = pole_pos_offset[idx][1];
    pos[2] = pole_pos_offset[idx][2];

    /* Identity orientation (would be set from track start direction) */
    uvs[0][0] = 1.0f; uvs[0][1] = 0.0f; uvs[0][2] = 0.0f;
    uvs[1][0] = 0.0f; uvs[1][1] = 1.0f; uvs[1][2] = 0.0f;
    uvs[2][0] = 0.0f; uvs[2][1] = 0.0f; uvs[2][2] = 1.0f;
}

/**
 * heal_damage - Reset damage after resurrection
 * Based on arcade: resurrect.c damage reset
 *
 * @param car_index Car index
 */
void heal_damage(s32 car_index) {
    CarData *car;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    car = &car_array[car_index];

    /* Reset damage-related fields */
    car->damage_level = 0;
    car->crashflag = 0;
    car->just_crashed = 0;
    car->crashtime = 0;
}
