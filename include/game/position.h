/**
 * position.h - Race position calculation for Rush 2049 N64
 *
 * Based on arcade game logic from checkpoint.c and maxpath.c
 * Handles race position (1st, 2nd, etc.) calculation based on
 * checkpoint progress and lap count.
 */

#ifndef POSITION_H
#define POSITION_H

#include "types.h"

/* Position tracking */
typedef struct RacePosition {
    s32     car_index;          /* Car this tracks */
    s32     laps;               /* Completed laps */
    s32     checkpoint;         /* Current checkpoint */
    f32     distance;           /* Distance along current segment */
    f32     total_distance;     /* Total race distance traveled */
    s8      place;              /* Race position (0=1st, 1=2nd, etc) */
    s8      place_locked;       /* Position locked (finished race) */
    u8      pad[2];
} RacePosition;

/* External position data */
extern RacePosition race_positions[];

/* Initialization */
void position_init(void);
void position_init_car(s32 car_index);

/* Position calculation */
void position_update_all(void);
void position_update_car(s32 car_index);
f32  position_calc_total_distance(s32 car_index);
void position_sort_by_distance(void);

/* Queries */
s32 position_get_place(s32 car_index);
s32 position_get_car_in_place(s32 place);
s32 position_is_finished(s32 car_index);
f32 position_get_distance_behind(s32 car_index);

/* Place locking (for finish) */
void position_lock_place(s32 car_index, s32 place);
void position_unlock_all(void);

/* Display helpers */
const char* position_get_suffix(s32 place);  /* "st", "nd", "rd", "th" */

#endif /* POSITION_H */
