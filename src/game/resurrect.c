/**
 * resurrect.c - Car resurrection/respawn system for Rush 2049 N64
 *
 * Based on arcade game/resurrect.c (Copyright 1996 Time Warner Interactive)
 * Handles car respawn after crash, position calculation, quaternion
 * interpolation for smooth rotation, and invulnerability blinking.
 *
 * Key concepts from arcade source:
 * - Crash detection triggers death state
 * - After delay, car respawns on track using "hand of god" animation
 * - Respawn position calculated from checkpoint and path data
 * - Quaternions used for smooth orientation interpolation
 * - Car blinks during invulnerability period
 */

#include "types.h"
#include "game/resurrect.h"
#include "game/structs.h"

#ifdef NON_MATCHING
/* Stubs for functions not yet implemented */
#define STUB_FUNC(name) /* name is stub */
#endif

/*
 * External game state
 */
extern u8 gstate;
extern u32 frame_counter;
extern CarData car_array[];
extern s32 num_active_cars;
extern s32 this_node;           /* Local player car index */
extern s32 trackno;
extern s32 crash_delay;
extern s32 demo_game;

/* External functions */
extern f32 sqrtf(f32 x);
extern f32 fabsf(f32 x);
extern void crossprod(f32 *v1, f32 *v2, f32 *result);
extern f32 magnitude(f32 *v);

/*
 * Constants
 */

/* Axis lookup table for cross product edge cases */
const s16 major_axis[3][2] = {
    {1, 2},
    {2, 0},
    {0, 1}
};

/**
 * Pole position offsets - starting grid and respawn positions
 * First 8 entries (0-7) are for race start staggered grid
 * Last 8 entries (8-15) are for resurrection spread across track
 */
const f32 pole_pos_offset[16][3] = {
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

/*
 * Global data
 */

/* Intermediate save positions for hand-of-god animation (per car) */
f32 save_pos[MAX_CARS][3];

/* Per-car resurrection state */
static ResurrectData res_data[MAX_CARS];

/*
 * Math helper functions
 */

/**
 * dot_prod - Dot product of two 3-element vectors
 * Based on arcade: resurrect.c:dot_prod()
 */
f32 dot_prod(f32 *vec1, f32 *vec2)
{
    return (vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2]);
}

/**
 * get_major_axis - Find index of largest component in vector
 * Based on arcade: resurrect.c:get_major_axis()
 */
s32 get_major_axis(f32 *vec)
{
    f32 abs0, abs1, abs2;

    abs0 = fabsf(vec[0]);
    abs1 = fabsf(vec[1]);
    abs2 = fabsf(vec[2]);

    if ((abs0 > abs1) && (abs0 > abs2)) {
        return 0;
    } else if (abs1 > abs2) {
        return 1;
    } else {
        return 2;
    }
}

/**
 * interpolate - Linear interpolation between two point arrays
 * Based on arcade: resurrect.c:interpolate()
 *
 * @param pointa First point array
 * @param pointb Second point array
 * @param p_size Number of elements to interpolate
 * @param vala Value associated with pointa
 * @param valb Value associated with pointb
 * @param inab Interpolation parameter (between vala and valb)
 * @param pointab Output interpolated point
 */
void interpolate(f32 *pointa, f32 *pointb, s32 p_size,
                 f32 vala, f32 valb, f32 inab, f32 *pointab)
{
    f32 scale;
    s32 i;

    /* Get scaling factor */
    scale = valb - vala;

    /* Get % of dist: vala=0.0 and valb=1.0 */
    if (scale != 0.0f) {
        scale = (inab - vala) / scale;
    }

    for (i = 0; i < p_size; i++) {
        pointab[i] = (pointb[i] - pointa[i]) * scale + pointa[i];
    }
}

/**
 * get_normal_vector - Cross product with angle calculation
 * Based on arcade: resurrect.c:get_normal_vector()
 *
 * Finds normal vector to two vectors (cross product) and returns
 * angle between the two original vectors.
 */
#ifdef NON_MATCHING
f32 get_normal_vector(f32 *vec1, f32 *vec2, f32 *nvec)
{
    f32 dx, angle;
    f32 mag1, mag2;
    s32 index, uv1, uv2;

    mag1 = magnitude(vec1);
    mag2 = magnitude(vec2);

    if (mag1 > 0.0f && mag2 > 0.0f) {
        dx = dot_prod(vec1, vec2) / (mag1 * mag2);
    } else {
        dx = 0.0f;
    }

    /* Clamp to valid range */
    if (dx > 1.0f) {
        dx = 1.0f;
    } else if (dx < -1.0f) {
        dx = -1.0f;
    }

    if (dx < -0.99f) {
        /* Vectors are nearly opposite, generate cooked cross product */
        index = get_major_axis(vec1);
        uv1 = major_axis[index][0];
        uv2 = major_axis[index][1];

        nvec[index] = vec1[uv1];
        nvec[uv1] = vec1[uv2];
        if (vec1[index] != 0.0f) {
            nvec[uv2] = -(vec1[uv1] + vec1[uv2]) / vec1[index];
        } else {
            nvec[uv2] = 0.0f;
        }
    } else {
        /* Get cross product (vector normal to original vectors) */
        crossprod(vec1, vec2, nvec);
    }

    /* Calculate angle (simplified - arcade uses facos) */
    angle = 1.5708f - dx * 1.5708f; /* Approximate acos using linear interp */

    return angle;
}
#endif

/**
 * non_linear_scale_setup - Setup parabolic scale coefficients
 * Based on arcade: resurrect.c:non_linear_scale_setup()
 *
 * Sets up data for non-linear scale of form y = ax^2 + bx + c
 */
void non_linear_scale_setup(f32 a, f32 *b, f32 *c,
                            f32 x1, f32 y1, f32 x2, f32 y2)
{
    f32 dx;

    dx = x2 - x1;

    if (dx < 0.00001f && dx > -0.00001f) {
        *b = 0.0f;
        *c = 0.0f;
        return;
    }

    *b = (y2 - y1) / dx - a * (x1 + x2);
    *c = y2 - a * x2 * x2 - (*b) * x2;
}

/*
 * Quaternion functions
 */

/**
 * make_quat_from_uvs - Convert rotation matrix to quaternion
 * Based on arcade: resurrect.c:make_quat_from_uvs()
 */
void make_quat_from_uvs(f32 uvs[3][3], f32 q[4])
{
    if (uvs[0][0] >= 0.0f) {
        if (uvs[1][1] + uvs[2][2] >= 0.0f) {
            q[0] = 1.0f + uvs[0][0] + uvs[1][1] + uvs[2][2];
            q[1] = uvs[2][1] - uvs[1][2];
            q[2] = uvs[0][2] - uvs[2][0];
            q[3] = uvs[1][0] - uvs[0][1];
        } else {
            q[0] = uvs[2][1] - uvs[1][2];
            q[1] = 1.0f + uvs[0][0] - uvs[1][1] - uvs[2][2];
            q[2] = uvs[1][0] + uvs[0][1];
            q[3] = uvs[0][2] + uvs[2][0];
        }
    } else {
        if (uvs[1][1] - uvs[2][2] >= 0.0f) {
            q[0] = uvs[0][2] - uvs[2][0];
            q[1] = uvs[1][0] + uvs[0][1];
            q[2] = 1.0f - uvs[0][0] + uvs[1][1] - uvs[2][2];
            q[3] = uvs[2][1] + uvs[1][2];
        } else {
            q[0] = uvs[1][0] - uvs[0][1];
            q[1] = uvs[0][2] + uvs[2][0];
            q[2] = uvs[2][1] + uvs[1][2];
            q[3] = 1.0f - uvs[0][0] - uvs[1][1] + uvs[2][2];
        }
    }
}

/**
 * make_uvs_from_quat - Convert quaternion to rotation matrix
 * Based on arcade: resurrect.c:make_uvs_from_quat()
 */
void make_uvs_from_quat(f32 q[4], f32 uvs[3][3])
{
    f32 Nq, q1, q2, q3, q4;
    f32 q11, q22, q33, q44;
    f32 q12, q13, q14, q23, q24, q34;
    f32 s;

    q1 = q[0];
    q2 = q[1];
    q3 = q[2];
    q4 = q[3];

    q11 = q1 * q1;
    q22 = q2 * q2;
    q33 = q3 * q3;
    q44 = q4 * q4;
    q14 = q1 * q4;
    q13 = q1 * q3;
    q12 = q1 * q2;
    q24 = q2 * q4;
    q23 = q2 * q3;
    q34 = q3 * q4;

    Nq = q11 + q22 + q33 + q44;
    s = (Nq > 0.0f) ? (1.0f / Nq) : 0.0f;

    uvs[0][0] = s * (q11 + q22 - q33 - q44);
    uvs[0][1] = s * (2.0f * (q23 - q14));
    uvs[0][2] = s * (2.0f * (q24 + q13));
    uvs[1][0] = s * (2.0f * (q23 + q14));
    uvs[1][1] = s * (q11 - q22 + q33 - q44);
    uvs[1][2] = s * (2.0f * (q34 - q12));
    uvs[2][0] = s * (2.0f * (q24 - q13));
    uvs[2][1] = s * (2.0f * (q34 + q12));
    uvs[2][2] = s * (q11 - q22 - q33 + q44);
}

/**
 * find_best_quat - Choose shortest rotation path
 * Based on arcade: resurrect.c:find_best_quat()
 *
 * Makes two quaternions close to each other by negating q2 if needed.
 * Note: q and -q represent the same rotation.
 */
void find_best_quat(f32 *q1, f32 *q2)
{
    s32 i;
    f32 dq1, dq2;
    f32 diff1, diff2;

    /* Test both q2[0-3] and -q2[0-3] to find closer one */
    dq1 = 0.0f;
    dq2 = 0.0f;

    for (i = 0; i < 4; i++) {
        diff1 = q1[i] - q2[i];
        diff2 = q1[i] + q2[i];
        if (diff1 < 0.0f) diff1 = -diff1;
        if (diff2 < 0.0f) diff2 = -diff2;
        dq1 += diff1;
        dq2 += diff2;
    }

    /* If -q2 is closer to q1, negate q2 */
    if (dq2 < dq1) {
        for (i = 0; i < 4; i++) {
            q2[i] = -q2[i];
        }
    }
}

/*
 * Core resurrection functions
 */

/**
 * resurrect_init - Initialize resurrection system for all cars
 */
void resurrect_init(void)
{
    s32 i;

    for (i = 0; i < MAX_CARS; i++) {
        resurrect_init_car(i);
    }
}

/**
 * resurrect_init_car - Initialize resurrection state for one car
 */
void resurrect_init_car(s32 car_index)
{
    ResurrectData *res;
    s32 j;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    res = &res_data[car_index];
    res->moving_state = RES_NONE;
    res->crash_time = -1;
    res->resurrect_time = 0;
    res->velocity = 0.0f;

    for (j = 0; j < 3; j++) {
        res->save_pos[j] = 0.0f;
        res->pos[j] = 0.0f;
        save_pos[car_index][j] = 0.0f;
    }

    for (j = 0; j < 4; j++) {
        res->quat_start[j] = 0.0f;
        res->quat_end[j] = 0.0f;
    }
    res->quat_start[0] = 1.0f; /* Identity quaternion */
    res->quat_end[0] = 1.0f;
}

/**
 * set_resurrection_data - Save current position for respawn
 * Based on arcade: resurrect.c:set_resurrection_data()
 *
 * @param mode Initialize (0) or Do_it (1)
 * @param car_index Car index
 */
#ifdef NON_MATCHING
void set_resurrection_data(s32 mode, s32 car_index)
{
    ResurrectData *res;
    CarData *car;
    u32 current_time;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    res = &res_data[car_index];
    car = &car_array[car_index];
    current_time = frame_counter * 16; /* Approximate ms */

    switch (mode) {
        case Initialize:
            /* Set resurrection data to checkpoint position */
            /* Would copy from checkpoint data structure */
            res->crash_time = -1;
            break;

        case Do_it:
            /* Save current position for respawn calculation */
            /* Arcade stores in (x,y,z) format */
            res->save_pos[0] = car->dr_pos[1];  /* Y -> X */
            res->save_pos[1] = -car->dr_pos[2]; /* -Z -> Y */
            res->save_pos[2] = car->dr_pos[0];  /* X -> Z */

            /* Save current time */
            res->crash_time = (s32)current_time;
            break;
    }
}
#endif

/**
 * CheckCrash - Check if car crashed and start explosion
 * Based on arcade: resurrect.c:CheckCrash()
 */
#ifdef NON_MATCHING
void CheckCrash(s32 car_index)
{
    CarData *car;
    ResurrectData *res;
    u32 current_time;
    s32 elapsed;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    car = &car_array[car_index];
    res = &res_data[car_index];
    current_time = frame_counter * 16;

    if (car->crashflag) {
        if (car->crashtime == 0) {
            /* Just crashed - save position and start explosion */
            set_resurrection_data(Do_it, car_index);

            car->just_crashed = 1;
            car->crashtime = current_time;
            /* StartBlast(car_index); - explosion effect */
        }

        /* Check if crash delay expired */
        elapsed = (s32)(current_time - car->crashtime);
        if (elapsed > crash_delay) {
            car->we_died = 1;
        }
    } else {
        car->crashtime = 0;
    }
}
#endif

/**
 * resurrect_car - Main respawn logic
 * Based on arcade: resurrect.c:resurrect_car()
 *
 * Calculates respawn position and orientation from saved crash data,
 * sets up quaternion interpolation for smooth rotation.
 */
#ifdef NON_MATCHING
void resurrect_car(s32 car_index)
{
    ResurrectData *res;
    CarData *car;
    f32 pos[3];
    s32 i, j;
    s32 pole_idx;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    res = &res_data[car_index];
    car = &car_array[car_index];

    /* If initializing, just copy saved position */
    if (res->crash_time == -1) {
        /* Copy position and orientation from checkpoint */
        for (i = 0; i < 3; i++) {
            car->dr_pos[i] = res->save_pos[i];
            for (j = 0; j < 3; j++) {
                car->dr_uvs[i][j] = res->uvs[i][j];
            }
        }

        /* Disable hand-of-god animation */
        res->moving_state = RES_NONE;

        /* Apply pole position offset */
        pole_idx = car_index & 0x7;
        car->dr_pos[0] += pole_pos_offset[pole_idx][0];
        car->dr_pos[1] += pole_pos_offset[pole_idx][1];
        car->dr_pos[2] += pole_pos_offset[pole_idx][2];

        return;
    }

    /* Calculate resurrection point from crash position */
    car->collidable = 0;

    /* Get current orientation as starting quaternion */
    make_quat_from_uvs(car->dr_uvs, res->quat_start);

    /* Calculate respawn position (simplified) */
    /* Full arcade version uses path/checkpoint data */
    pole_idx = 8 + (car_index & 0x7); /* Use resurrection offsets */
    res->pos[0] = res->save_pos[0] + pole_pos_offset[pole_idx][0];
    res->pos[1] = res->save_pos[1] + pole_pos_offset[pole_idx][1];
    res->pos[2] = res->save_pos[2] + pole_pos_offset[pole_idx][2] - 2.0f;

    /* Get target orientation quaternion (would use path direction) */
    make_quat_from_uvs(res->uvs, res->quat_end);

    /* Choose shortest rotation path */
    find_best_quat(res->quat_start, res->quat_end);

    /* Initialize hand-of-god animation */
    res->moving_state = RES_STARTING;

    /* Set respawn velocity (would use maxpath speed) */
    res->velocity = 50.0f;

    /* Store current position for animation */
    for (i = 0; i < 3; i++) {
        pos[i] = car->RWV[i];
    }

    /* Clear crash state for tires */
    car->crashflag = 0;

    /* Restore velocity for camera */
    for (i = 0; i < 3; i++) {
        car->RWV[i] = pos[i];
    }
}
#endif

/**
 * update_resurrecting_car - Hand-of-god animation update
 * Based on arcade: resurrect.c:update_resurrecting_car()
 *
 * Moves resurrecting car toward destination with smooth interpolation.
 */
#ifdef NON_MATCHING
void update_resurrecting_car(s32 car_index)
{
    ResurrectData *res;
    CarData *car;
    f32 pos[3], pos2[3], quat[4], uvs[3][3];
    f32 f32_state, temp, scale;
    s32 s16_state;
    u32 current_time;
    s32 i, j;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    res = &res_data[car_index];
    car = &car_array[car_index];
    current_time = frame_counter * 16;

    /* Initialize hand-of-god on first frame */
    if (res->moving_state == RES_STARTING) {
        /* Calculate high point for arc */
        pos2[0] = car->dr_pos[0] - res->pos[0];
        pos2[1] = car->dr_pos[1] - res->pos[1];
        pos2[2] = car->dr_pos[2] - res->pos[2];
        temp = magnitude(pos2) / 8.0f;
        if (temp > 100.0f) {
            temp = 100.0f;
        }

        /* Get midpoint of animation */
        interpolate(car->dr_pos, res->pos, 3,
                   0.0f, (f32)RESURRECT_TIME, (f32)RESURRECT_TIME / 2.0f,
                   save_pos[car_index]);

        /* Raise midpoint for arc effect */
        save_pos[car_index][2] -= temp;

        /* Make car translucent */
        car->appearance |= App_M_TRANSLUCENT;
    }

    res->moving_state++;

    /* Calculate time offset */
    s16_state = (s32)(current_time - (u32)res->resurrect_time);
    f32_state = (f32)s16_state;

    /* Check if animation complete */
    if (s16_state > RESURRECT_TIME) {
        res->moving_state = RES_FINISHED;

        /* Show car again */
        car->appearance &= ~App_M_TRANSLUCENT;
        return;
    }

    /* Turn off hulk bit during animation */
    car->appearance &= ~App_M_HULK;

    if (s16_state < RESURRECT_TIME / 2) {
        /* First half: move up to midpoint with easing */
        scale = 2.0f * f32_state * f32_state / (f32)RESURRECT_TIME;

        interpolate(car->dr_pos, save_pos[car_index], 3,
                   0.0f, (f32)RESURRECT_TIME / 2.0f, scale, pos);
        interpolate(res->quat_start, res->quat_end, 4,
                   0.0f, (f32)RESURRECT_TIME / 2.0f, scale, quat);

        /* Convert interpolated quaternion to matrix */
        make_uvs_from_quat(quat, uvs);

        /* Update car orientation */
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                car->dr_uvs[i][j] = uvs[i][j];
            }
        }
    } else {
        /* Second half: move down to destination */
        interpolate(save_pos[car_index], res->pos, 3,
                   (f32)RESURRECT_TIME / 2.0f, (f32)RESURRECT_TIME,
                   f32_state, pos);
    }

    /* Update car position */
    for (i = 0; i < 3; i++) {
        car->RWR[i] = pos[i];
    }
}
#endif

/**
 * check_if_finished_resurrecting - State machine update
 * Based on arcade: resurrect.c:check_if_finished_resurrecting()
 */
#ifdef NON_MATCHING
void check_if_finished_resurrecting(void)
{
    ResurrectData *res;
    CarData *car;
    s32 i, j;

    for (i = 0; i < num_active_cars; i++) {
        res = &res_data[i];
        car = &car_array[i];

        if (res->moving_state == RES_FINISHED) {
            res->moving_state = RES_NONE;

            /* Copy final position */
            for (j = 0; j < 3; j++) {
                car->dr_pos[j] = res->pos[j];
            }

            /* Set initial velocity */
            car->RWV[0] = res->velocity * 0.4f;

            /* Make car collidable again after delay */
            car->collidable = 1;
        }
    }
}
#endif

/**
 * blink_resurrecting_cars - Update transparency for invulnerability
 * Based on arcade: resurrect.c:blink_resurrecting_cars()
 */
#ifdef NON_MATCHING
void blink_resurrecting_cars(void)
{
    ResurrectData *res;
    CarData *car;
    s32 delta_time;
    u32 current_time;
    s32 i;

    current_time = frame_counter * 16;

    for (i = 0; i < MAX_CARS; i++) {
        res = &res_data[i];
        car = &car_array[i];

        if (!car->data_valid) {
            continue;
        }

        if (car->collidable) {
            /* Car is collidable - make solid */
            car->appearance &= ~(App_M_HULK | App_M_TRANSLUCENT | App_M_ANY_DAMAGE);
        } else {
            /* Get time since we became non-collidable */
            delta_time = (s32)(current_time - car->collide_time);

            /* If being resurrected, reset timer */
            if (res->moving_state > RES_NONE) {
                car->collide_time = current_time;
                delta_time = 0;
            }

            /* After 5 seconds, make collidable again */
            if (delta_time > INVULN_TIME) {
                if (i == this_node) {
                    car->collidable = 1;
                    car->collide_time = 0;
                    car->appearance &= ~App_M_TRANSLUCENT;
                }
            } else {
                /* Blink effect based on time */
                if (car->collide_count <= 0) {
                    car->collide_state++;
                    if (car->collide_state & 1) {
                        /* Odd state: solid */
                        car->collide_count = 0;
                        car->appearance &= ~App_M_TRANSLUCENT;
                    } else {
                        /* Even state: transparent */
                        /* Blink faster as time goes on */
                        if (delta_time < 1000) {
                            car->collide_count = 33;
                        } else if (delta_time < 2000) {
                            car->collide_count = 8;
                        } else if (delta_time < 3000) {
                            car->collide_count = 5;
                        } else if (delta_time < 4000) {
                            car->collide_count = 2;
                        } else {
                            car->collide_count = 0;
                        }
                        car->appearance |= App_M_TRANSLUCENT;
                    }
                } else {
                    car->collide_count--;
                }
            }
        }
    }
}
#endif

/**
 * abort_car - Manual abort (player pressed abort button)
 * Based on arcade: resurrect.c:abort_car()
 */
#ifdef NON_MATCHING
void abort_car(s32 car_index)
{
    CarData *car;
    u32 elapsed_time;
    u32 current_time;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    car = &car_array[car_index];
    current_time = frame_counter * 16;

    /* Do nothing if race started less than 5 seconds ago */
    elapsed_time = current_time; /* Would use race start time */
    if (elapsed_time < 5 * ONE_SEC) {
        return;
    }

    /* Clear damage on abort */
    car->appearance &= ~App_M_ANY_DAMAGE;

    /* Save position and trigger respawn */
    set_resurrection_data(Do_it, car_index);
    car->we_died = 1;
}
#endif

/**
 * jump_to_next_checkpoint - Debug teleport to next checkpoint
 * Based on arcade: resurrect.c:jump_to_next_checkpoint()
 */
#ifdef NON_MATCHING
void jump_to_next_checkpoint(void)
{
    CarData *car;
    s32 car_index;

    car_index = this_node;
    car = &car_array[car_index];

    /* Make car non-collidable and translucent */
    car->collidable = 0;
    car->appearance |= App_M_TRANSLUCENT;
    car->collide_time = frame_counter * 16;
    car->collide_state = -1;
    car->collide_count = -1;

    /* Set resurrection data to next checkpoint */
    /* JumpToNextCP(); - would update checkpoint index */
    set_resurrection_data(Initialize, car_index);

    car->we_died = 1;
}
#endif

/**
 * detect_drone_abort - Auto-abort stuck drones
 * Based on arcade: resurrect.c:detect_drone_abort()
 */
#ifdef NON_MATCHING
void detect_drone_abort(void)
{
    /* Stub - full implementation would check drone velocity */
    /* and auto-abort if stuck for too long */
}
#endif

/*
 * State query functions
 */

/**
 * are_we_dead_or_dying - Check if car is in death state
 * Based on arcade: resurrect.c:are_we_dead_or_dying()
 */
s32 are_we_dead_or_dying(s32 car_index)
{
    CarData *car;
    ResurrectData *res;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }

    car = &car_array[car_index];
    res = &res_data[car_index];

    return (car->we_died ||
            car->crashflag ||
            res->moving_state > RES_NONE);
}

/**
 * are_we_solid - Check if car is collidable
 * Based on arcade: resurrect.c:are_we_solid()
 */
s32 are_we_solid(s32 car_index)
{
    CarData *car;
    ResurrectData *res;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }

    car = &car_array[car_index];
    res = &res_data[car_index];

    return (car->we_died == 0 &&
            car->crashflag == 0 &&
            res->moving_state == RES_NONE &&
            car->collidable != 0);
}

/*
 * Compatibility wrappers
 */

/**
 * is_car_dead - Check if car is dead (wrapper)
 */
s32 is_car_dead(s32 car_index)
{
    return are_we_dead_or_dying(car_index);
}

/**
 * is_car_invulnerable - Check if car is invulnerable
 */
s32 is_car_invulnerable(s32 car_index)
{
    CarData *car;
    ResurrectData *res;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }

    car = &car_array[car_index];
    res = &res_data[car_index];

    /* Invulnerable during respawn animation or collision countdown */
    return (res->moving_state > RES_NONE ||
            car->collidable == 0);
}

/**
 * get_respawn_alpha - Get transparency for fade effect
 */
u8 get_respawn_alpha(s32 car_index)
{
    ResurrectData *res;
    CarData *car;
    u32 current_time;
    s32 elapsed;
    f32 alpha;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return 255;
    }

    res = &res_data[car_index];
    car = &car_array[car_index];
    current_time = frame_counter * 16;

    if (res->moving_state > RES_NONE) {
        /* Fade in during respawn animation */
        elapsed = (s32)(current_time - (u32)res->resurrect_time);
        alpha = (f32)elapsed / (f32)RESURRECT_TIME;
        if (alpha > 1.0f) {
            alpha = 1.0f;
        }
        return (u8)(alpha * 255.0f);
    }

    if (car->collidable == 0) {
        /* Blink effect during invulnerability */
        if ((current_time / 100) & 1) {
            return 200;
        }
    }

    return 255;
}
