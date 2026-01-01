/**
 * model.c - Catch-up and model control for Rush 2049 N64
 *
 * Based on arcade: game/model.c
 * Copyright 1996 Time Warner Interactive (original arcade source)
 *
 * Implements rubber-band catch-up algorithm to keep races competitive.
 */

#include "types.h"
#include "game/checkpoint.h"
#include "game/structs.h"
#include "game/physics.h"
#include "game/drone.h"
#include "game/model.h"

/* ========================================================================
 * Catch-up Constants
 * Based on arcade model.c
 * ======================================================================== */

/**
 * cuzone - Catch-up zone in feet behind leader
 * Track:    1      2      3      4      5      6      7      8
 */
const f32 cuzone[MAX_TRACKS] = {
    400.0f, 800.0f, 800.0f, 600.0f, 900.0f, 600.0f, 800.0f, 600.0f
};

/**
 * cuscale - Maximum catch-up as fraction
 * Track:    1      2      3      4      5      6      7      8
 */
const f32 cuscale[MAX_TRACKS] = {
    0.300f, 0.200f, 0.100f, 0.200f, 0.200f, 0.200f, 0.200f, 0.250f
};

/**
 * cusolo - Solo player catch-up compensation
 */
const f32 cusolo = 0.0f;

/* ========================================================================
 * Catch-up Implementation
 * ======================================================================== */

#ifdef NON_MATCHING

/**
 * set_catchup - Rubber-band catch-up algorithm
 */
void set_catchup(void) {
    s32 i;
    s32 high_index;
    s32 num_players;
    f32 firstdist;
    f32 lagdist;
    f32 max_catchup;
    f32 target_catchup;
    s32 track_idx;

    /* Clamp track index to valid range */
    track_idx = trackno;
    if (track_idx < 0) {
        track_idx = 0;
    }
    if (track_idx >= MAX_TRACKS) {
        track_idx = MAX_TRACKS - 1;
    }

    /* Find leading human player */
    num_players = 0;
    high_index = -1;

    for (i = 0; i < MAX_LINKS; i++) {
        if (model[i].in_game && model[i].drone_type == DRONE_TYPE_HUMAN) {
            num_players++;
            if (high_index == -1) {
                high_index = i;
            } else if (car_array[i].distance > car_array[high_index].distance) {
                high_index = i;
            }
        }
    }

    /* Solo player gets compensating boost */
    if (num_players == 1) {
        model[gThisNode].catchup = 1.0f + (cuscale[track_idx] * cusolo);
        return;
    }

    /* Multiple players: spread catch-up based on distance behind leader */
    if (high_index < 0) {
        return;  /* No players found */
    }

    max_catchup = cuscale[track_idx];
    firstdist = car_array[high_index].distance;

    for (i = 0; i < MAX_LINKS; i++) {
        if (model[i].in_game && model[i].drone_type == DRONE_TYPE_HUMAN) {
            /* Calculate distance behind leader */
            lagdist = firstdist - car_array[i].distance;

            /* Determine target catch-up */
            if (lagdist > cuzone[track_idx]) {
                /* Beyond catch-up zone: full boost */
                target_catchup = max_catchup + 1.0f;
            } else {
                /* Within zone: linear interpolation */
                target_catchup = (lagdist * max_catchup / cuzone[track_idx]) + 1.0f;
            }

            /* Smooth transition (97% old, 3% new) */
            model[i].catchup = 0.97f * model[i].catchup + 0.03f * target_catchup;
        }
    }
}

/**
 * no_catchup - Disable catch-up for tournament mode
 */
void no_catchup(void) {
    s32 i;

    /* Set all controlled cars to base speed */
    for (i = 0; i < MAX_LINKS; i++) {
        if (model[i].we_control_flag) {
            model[i].catchup = 1.0f;
        }
    }
}

/**
 * avg_catchup - Apply average catch-up for fair scoring
 */
void avg_catchup(void) {
    s32 track_idx;
    f32 avg_boost;

    track_idx = trackno;
    if (track_idx < 0) {
        track_idx = 0;
    }
    if (track_idx >= MAX_TRACKS) {
        track_idx = MAX_TRACKS - 1;
    }

    /* Average boost is half of maximum */
    avg_boost = 1.0f + (cuscale[track_idx] * 0.5f);
    model[gThisNode].catchup = avg_boost;
}

#else

/* Forward to drone subsystem implementation */
void set_catchup(void) {
    drone_set_catchup();
}

void no_catchup(void) {
    drone_no_catchup();
}

void avg_catchup(void) {
    s32 track_idx;
    f32 avg_boost;

    track_idx = trackno;
    if (track_idx < 0) {
        track_idx = 0;
    }
    if (track_idx >= MAX_TRACKS) {
        track_idx = MAX_TRACKS - 1;
    }

    avg_boost = 1.0f + (cuscale[track_idx] * 0.5f);
    model[gThisNode].catchup = avg_boost;
}

#endif /* NON_MATCHING */

/* Wrapper functions */
void model_set_catchup(void) {
    set_catchup();
}

void model_no_catchup(void) {
    no_catchup();
}
