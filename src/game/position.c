/**
 * position.c - Race position calculation for Rush 2049 N64
 *
 * Based on arcade game logic from checkpoint.c and maxpath.c
 * Handles race position (1st, 2nd, etc.) calculation based on
 * checkpoint progress and lap count.
 *
 * Position is determined by:
 * 1. Lap count (higher = better)
 * 2. Checkpoint progress (higher = better)
 * 3. Distance along current segment (higher = better)
 */

#include "types.h"
#include "game/position.h"
#include "game/checkpoint.h"
#include "game/structs.h"

/* External game state */
extern u8 gstate;
extern CarData car_array[];
extern s32 num_active_cars;
extern u32 number_checkpoints;
extern s16 lap_loop_index;

/* External checkpoint data */
extern CheckPoint TrackCPs[][MAX_CHECKPOINTS];
extern Track CP_Track[];
extern TrackData track_data[];
extern s32 trackno;

/* Position data for all cars */
RacePosition race_positions[MAX_CARS];

/* Suffix strings for place display */
static const char* place_suffixes[] = {
    "st",   /* 1st */
    "nd",   /* 2nd */
    "rd",   /* 3rd */
    "th"    /* 4th, 5th, etc */
};

/**
 * position_init - Initialize position tracking for all cars
 */
void position_init(void) {
    s32 i;

    for (i = 0; i < MAX_CARS; i++) {
        position_init_car(i);
    }
}

/**
 * position_init_car - Initialize position tracking for one car
 *
 * @param car_index Car index
 */
void position_init_car(s32 car_index) {
    RacePosition *pos;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    pos = &race_positions[car_index];

    pos->car_index = car_index;
    pos->laps = 0;
    pos->checkpoint = 0;
    pos->distance = 0.0f;
    pos->total_distance = 0.0f;
    pos->place = car_index;  /* Initial place is starting grid position */
    pos->place_locked = 0;
}

/**
 * position_calc_total_distance - Calculate total race distance for sorting
 *
 * Total distance is:
 *   laps * track_length + checkpoint_distance + segment_distance
 *
 * This gives a single number that can be sorted to determine positions.
 *
 * @param car_index Car index
 * @return Total distance (higher = further along in race)
 */
f32 position_calc_total_distance(s32 car_index) {
    CarData *car;
    RacePosition *pos;
    f32 total;
    f32 lap_distance;
    f32 checkpoint_distance;

    car = &car_array[car_index];
    pos = &race_positions[car_index];

    /* Estimate lap distance (checkpoints * avg distance) */
    /* This is a simplified calculation; arcade uses path_dist arrays */
    lap_distance = (f32)number_checkpoints * 1000.0f;

    /* Distance from checkpoints */
    checkpoint_distance = (f32)car->checkpoint * (lap_distance / (f32)number_checkpoints);

    /* Total = laps * full_lap + checkpoint progress */
    total = (f32)car->laps * lap_distance + checkpoint_distance;

    /* Add distance within current segment */
    total += car->distance;

    return total;
}

/**
 * position_sort_by_distance - Sort cars by race distance to determine positions
 *
 * Uses simple bubble sort since we only have 8 cars max.
 */
void position_sort_by_distance(void) {
    RacePosition *pos;
    f32 distances[MAX_CARS];
    s32 order[MAX_CARS];
    s32 i, j;
    s32 temp;
    f32 temp_dist;

    /* Calculate distances for all active cars */
    for (i = 0; i < num_active_cars; i++) {
        order[i] = i;
        distances[i] = position_calc_total_distance(i);
        race_positions[i].total_distance = distances[i];
    }

    /* Bubble sort by distance (descending - furthest first) */
    for (i = 0; i < num_active_cars - 1; i++) {
        for (j = 0; j < num_active_cars - i - 1; j++) {
            /* Skip if either car's place is locked */
            if (race_positions[order[j]].place_locked ||
                race_positions[order[j+1]].place_locked) {
                continue;
            }

            /* Swap if next car is further along */
            if (distances[j] < distances[j+1]) {
                temp = order[j];
                order[j] = order[j+1];
                order[j+1] = temp;

                temp_dist = distances[j];
                distances[j] = distances[j+1];
                distances[j+1] = temp_dist;
            }
        }
    }

    /* Assign places based on sorted order */
    for (i = 0; i < num_active_cars; i++) {
        pos = &race_positions[order[i]];
        if (!pos->place_locked) {
            pos->place = i;
            car_array[order[i]].place = i;
        }
    }
}

/**
 * position_update_car - Update position data from car data
 *
 * @param car_index Car index
 */
void position_update_car(s32 car_index) {
    CarData *car;
    RacePosition *pos;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    car = &car_array[car_index];
    pos = &race_positions[car_index];

    /* Copy from car data */
    pos->laps = car->laps;
    pos->checkpoint = car->checkpoint;
    pos->distance = car->distance;

    /* Recalculate total distance */
    pos->total_distance = position_calc_total_distance(car_index);
}

/**
 * position_update_all - Update all car positions
 *
 * Called each frame during gameplay to update race standings.
 */
void position_update_all(void) {
    s32 i;

    /* Update individual car data */
    for (i = 0; i < num_active_cars; i++) {
        position_update_car(i);
    }

    /* Re-sort to determine current positions */
    position_sort_by_distance();
}

/**
 * position_get_place - Get current race position for a car
 *
 * @param car_index Car index
 * @return Place (0=1st, 1=2nd, etc), or -1 if invalid
 */
s32 position_get_place(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return -1;
    }
    return race_positions[car_index].place;
}

/**
 * position_get_car_in_place - Get car index that is in given place
 *
 * @param place Place to look up (0=1st, 1=2nd, etc)
 * @return Car index, or -1 if not found
 */
s32 position_get_car_in_place(s32 place) {
    s32 i;

    for (i = 0; i < num_active_cars; i++) {
        if (race_positions[i].place == place) {
            return i;
        }
    }
    return -1;
}

/**
 * position_is_finished - Check if car has finished the race
 *
 * @param car_index Car index
 * @return 1 if finished, 0 otherwise
 */
s32 position_is_finished(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0;
    }
    return race_positions[car_index].place_locked;
}

/**
 * position_get_distance_behind - Get distance behind leader
 *
 * @param car_index Car index
 * @return Distance behind leader (0 if in 1st place)
 */
f32 position_get_distance_behind(s32 car_index) {
    s32 leader;
    f32 leader_dist;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return 0.0f;
    }

    leader = position_get_car_in_place(0);
    if (leader < 0 || leader == car_index) {
        return 0.0f;
    }

    leader_dist = race_positions[leader].total_distance;
    return leader_dist - race_positions[car_index].total_distance;
}

/**
 * position_lock_place - Lock a car's finishing position
 *
 * Called when a car crosses the finish line to prevent
 * their position from changing.
 *
 * @param car_index Car index
 * @param place Final position
 */
void position_lock_place(s32 car_index, s32 place) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    race_positions[car_index].place = place;
    race_positions[car_index].place_locked = 1;
    car_array[car_index].place = place;
    car_array[car_index].place_locked = 1;
}

/**
 * position_unlock_all - Unlock all positions (for race restart)
 */
void position_unlock_all(void) {
    s32 i;

    for (i = 0; i < MAX_CARS; i++) {
        race_positions[i].place_locked = 0;
        car_array[i].place_locked = 0;
    }
}

/**
 * position_get_suffix - Get suffix for place display ("st", "nd", "rd", "th")
 *
 * @param place Place (0=1st, 1=2nd, etc)
 * @return Suffix string
 */
const char* position_get_suffix(s32 place) {
    /* Convert 0-indexed place to 1-indexed for display */
    s32 display_place = place + 1;

    /* Handle special cases for 11th, 12th, 13th */
    if (display_place >= 11 && display_place <= 13) {
        return place_suffixes[3];  /* "th" */
    }

    /* Get suffix based on last digit */
    switch (display_place % 10) {
        case 1:
            return place_suffixes[0];  /* "st" */
        case 2:
            return place_suffixes[1];  /* "nd" */
        case 3:
            return place_suffixes[2];  /* "rd" */
        default:
            return place_suffixes[3];  /* "th" */
    }
}
