/**
 * checkpoint.c - Checkpoint and lap counting for Rush 2049 N64
 *
 * Based on arcade game/checkpoint.c
 * Handles checkpoint detection, lap counting, race positions
 *
 * Key functions mapped from arcade:
 * - init_cp_data -> checkpoint structure initialization
 * - InitCPS -> race start initialization
 * - CheckCPs -> per-frame checkpoint detection
 * - PassedCP -> checkpoint crossing handler
 * - get_next_checkpoint -> checkpoint sequence
 */

#include "types.h"
#include "game/checkpoint.h"
#include "game/structs.h"

/* External OS functions */
extern u32 osGetCount(void);

/* External game state */
extern u8 gstate;
extern u32 frame_counter;
extern CarData car_array[];
extern GameStruct game_struct;

/* External from vecmath */
extern f32 vec_dot(const f32 a[3], const f32 b[3]);
extern void vec_sub(const f32 a[3], const f32 b[3], f32 result[3]);

/* Track-related externals (from track data) */
extern s32 trackno;                 /* Current track index */
extern s32 num_active_cars;         /* Number of cars in race */
extern s32 this_node;               /* Local player index */

/* Timing externals */
#define ONE_SEC     60              /* Frames per second */
#define IRQTIME     frame_counter   /* Current time in frames */

/* Checkpoint system state */
u32 number_checkpoints;
u32 last_checkpoint_time;
s32 first_place_time;
s16 global_laps;
s16 global_last_cp;
s16 global_next_cp;
s16 lap_loop_index;
u32 time_to_be_given;
u32 total_time_given;
u32 total_time_box_time;

/* Track/checkpoint data */
CheckPoint TrackCPs[MAX_TRACKS][MAX_CHECKPOINTS];
Track CP_Track[MAX_TRACKS];
TrackData track_data[MAX_TRACKS];

/* Path data (arcade: extern in checkpoint.c) */
u16 path_start;                     /* Start index in path array */
u16 path_end;                       /* End index in path array */
u16 path_loop;                      /* Loop point for lap restart */
s16 path_dist_index[MAX_CHECKPOINTS];
u16 path_dist[MAX_PATH_POINTS];
s16 path_index[MAX_PATH_POINTS];
s16 path_to_maxpath[MAX_PATH_POINTS];
s16 num_path_points;

/* Local state */
static s8 in_first_place;
static s8 end_game_flag;

/**
 * get_next_checkpoint - Get next checkpoint in sequence
 * Based on arcade: checkpoint.c:get_next_checkpoint()
 *
 * @param cur_checkpoint Current checkpoint index
 * @return Next checkpoint index (wraps at lap boundary)
 */
u32 get_next_checkpoint(u32 cur_checkpoint) {
    cur_checkpoint++;

    if (cur_checkpoint >= number_checkpoints) {
        cur_checkpoint = lap_loop_index;
    }

    return cur_checkpoint;
}

/**
 * get_next_center - Get next path center point
 * Based on arcade: checkpoint.c:get_next_center()
 *
 * @param t_index Current path index
 * @param t_direction Track direction (-1 or 1)
 * @return Next path center index
 */
s16 get_next_center(s16 t_index, s16 t_direction) {
    /* Clamp to valid range */
    if (t_index < (s16)path_start) {
        t_index = path_start;
    } else if (t_index > (s16)path_end) {
        t_index = path_end;
    }

    switch (t_direction) {
        case -1:
            if (t_index <= (s16)path_start) {
                if (path_loop == path_start) {
                    t_index = path_end;
                } else {
                    t_index = path_loop;
                }
            } else {
                t_index--;
            }
            break;

        case 1:
            if (t_index >= (s16)path_end) {
                t_index = path_loop;
            } else {
                t_index++;
            }
            break;
    }

    return t_index;
}

/**
 * get_prev_center - Get previous path center point
 * Based on arcade: checkpoint.c:get_prev_center()
 *
 * @param t_index Current path index
 * @param t_direction Track direction (-1 or 1)
 * @return Previous path center index
 */
s16 get_prev_center(s16 t_index, s16 t_direction) {
    /* Clamp to valid range */
    if (t_index < (s16)path_start) {
        t_index = path_start;
    } else if (t_index > (s16)path_end) {
        t_index = path_end;
    }

    switch (t_direction) {
        case -1:
            if ((t_index == (s16)path_loop) && (path_loop != path_start)) {
                t_index = path_start;
            } else if (t_index < (s16)path_end) {
                t_index++;
            } else {
                t_index = path_end;
            }
            break;

        case 1:
            if (t_index == (s16)path_loop) {
                t_index = path_end;
            } else if (t_index > (s16)path_start) {
                t_index--;
            } else {
                t_index = path_start;
            }
            break;
    }

    return t_index;
}

/**
 * InitCPS - Initialize checkpoint system for race start
 * Based on arcade: checkpoint.c:InitCPS()
 *
 * Called at start of each race to reset checkpoint state.
 */
void InitCPS(void) {
    CheckPoint *chkpnt;
    TrackData *trck;
    s32 i;

    global_laps = 0;
    global_last_cp = 0;
    global_next_cp = 1;
    number_checkpoints = 0;
    end_game_flag = 0;
    total_time_given = 0;
    total_time_box_time = 0;

    chkpnt = CP_Track[trackno].chk_point;
    trck = &track_data[trackno];

    /* Count checkpoints until sentinel (-1) */
    while ((chkpnt[number_checkpoints].logical_cp != -1) &&
           (number_checkpoints < MAX_CHECKPOINTS)) {
        number_checkpoints++;
    }

    lap_loop_index = trck->loop_chkpnt;

    /* Initialize car checkpoint states */
    for (i = 0; i < MAX_CARS; i++) {
        car_array[i].checkpoint = 0;
        car_array[i].laps = 0;
    }
}

/**
 * init_cp_time - Initialize checkpoint timing
 * Based on arcade: checkpoint.c:init_cp_time()
 *
 * Note: arcade version has unused local variables (i, j, num, temp)
 * that were likely for removed debug/setup code.
 */
void init_cp_time(void) {
    last_checkpoint_time = IRQTIME;
    first_place_time = 0;
    in_first_place = 0;
}

/**
 * CheckCPs - Check all cars for checkpoint crossings
 * Based on arcade: checkpoint.c:CheckCPs()
 *
 * Called each frame to detect checkpoint crossings.
 * Uses plane test: car is past checkpoint if dot(car_pos - cp_pos, cp_zvec) > 0
 */
void CheckCPs(void) {
    CheckPoint *chkpnt;
    f32 obs_posn[3];
    f32 diff[3];
    f32 zvec[3];
    f32 cent_dist;
    s32 i, index;
    s32 next_cp;

    chkpnt = CP_Track[trackno].chk_point;

    /* Check each active car */
    for (index = 0; index < num_active_cars; index++) {
        /* Get car's next checkpoint */
        next_cp = car_array[index].checkpoint;
        next_cp = get_next_checkpoint(next_cp);

        /* Get car position */
        for (i = 0; i < 3; i++) {
            obs_posn[i] = car_array[index].dr_pos[i];
        }

        /* Calculate difference from checkpoint position */
        for (i = 0; i < 3; i++) {
            diff[i] = obs_posn[i] - chkpnt[next_cp].pos[i];
            zvec[i] = chkpnt[next_cp].uvs[2][i];
        }

        /* Check distance to checkpoint (XZ plane only) */
        cent_dist = diff[0] * diff[0] + diff[2] * diff[2];

        if (cent_dist < chkpnt[next_cp].radius) {
            /* Within radius - check if past the plane */
            if (vec_dot(diff, zvec) > 0.0f) {
                /* Passed checkpoint! */
                car_array[index].checkpoint = next_cp;

                /* Check for lap completion */
                if (next_cp == track_data[trackno].finish_line) {
                    car_array[index].laps++;
                }
            }
        }
    }

    /* Update race positions */
    check_everyones_checkpoints();

    /* Check first place status for sound */
    if (first_place_time && (IRQTIME - first_place_time) > (3 * ONE_SEC)) {
        first_place_time = 0;
    }

    if (car_array[this_node].place == 0) {
        if (in_first_place == 0 && first_place_time == 0) {
            first_place_time = IRQTIME;
            in_first_place = 1;
        }
    } else {
        in_first_place = 0;
    }
}

/**
 * check_everyones_checkpoints - Update global checkpoint state
 * Based on arcade: checkpoint.c:check_everyones_checkpoints()
 *
 * Tracks the lead car's progress and awards bonus time.
 */
void check_everyones_checkpoints(void) {
    CheckPoint *chkpnt;
    TrackData *trck;
    s32 i, slot;
    s32 cur_laps, cur_lcp;
    s32 next_lap, next_lcp, next_ncp;

    chkpnt = CP_Track[trackno].chk_point;
    trck = &track_data[trackno];

    /* Determine expected next checkpoint for time bonus */
    next_lap = global_laps;
    if (global_next_cp == trck->finish_line) {
        next_lap++;
    }
    next_lcp = global_next_cp;
    next_ncp = get_next_checkpoint(next_lcp);

    /* Check each car's progress */
    for (i = 0; i < num_active_cars; i++) {
        slot = i;  /* Simplified - arcade uses model[i].slot */

        cur_laps = car_array[slot].laps;
        cur_lcp = car_array[slot].checkpoint;

        /* Check if this car is ahead of the global leader */
        if ((cur_laps > next_lap) ||
            ((cur_laps == next_lap) &&
             (chkpnt[cur_lcp].logical_cp >= chkpnt[next_lcp].logical_cp))) {

            /* Update global leader position */
            global_laps = next_lap;
            global_last_cp = next_lcp;
            global_next_cp = next_ncp;

            /* Recalculate next expected checkpoint */
            next_lap = global_laps;
            if (global_next_cp == trck->finish_line) {
                next_lap++;
            }
            next_lcp = global_next_cp;
            next_ncp = get_next_checkpoint(next_lcp);
        }
    }
}

/**
 * CarPassedCheckpoint - Handle network checkpoint message
 * Based on arcade: checkpoint.c:CarPassedCheckpoint()
 *
 * @param slot Car slot that passed checkpoint
 * @param checkpoint Checkpoint data (lap << 16 | last_cp << 8 | next_cp)
 * @param flag Skip-ahead flag
 */
void CarPassedCheckpoint(s32 slot, s32 checkpoint, s32 flag) {
    s32 laps, last_cp, next_cp;

    /* Decode checkpoint info */
    laps = (checkpoint >> 16) & 0xFF;
    last_cp = (checkpoint >> 8) & 0xFF;
    next_cp = checkpoint & 0xFF;

    /* Update car state */
    car_array[slot].laps = laps;
    car_array[slot].checkpoint = last_cp;
}

/**
 * CarReportsGameOver - Handle network game over message
 * Based on arcade: checkpoint.c:CarReportsGameOver()
 *
 * @param slot Car slot that finished
 * @param score Finish time
 * @param flag Special flags
 */
void CarReportsGameOver(s32 slot, u32 score, u32 flag) {
    /* Lock the car's final position */
    car_array[slot].score = score;
    car_array[slot].place_locked = 1;

    if (flag) {
        gstate = 8;  /* ENDGAME state */
    }
}

/**
 * set_game_time_to_one_second - Force race to end quickly
 * Based on arcade: checkpoint.c:set_game_time_to_one_second()
 *
 * Used when skipping to next checkpoint or ending demo.
 */
void set_game_time_to_one_second(void) {
    s32 i;

    /* Mark all cars as not finishing (place = -1) */
    for (i = 0; i < num_active_cars; i++) {
        car_array[i].place_locked = -1;
    }
}

/**
 * update_lap_counter - Update lap display
 * Based on arcade: checkpoint.c:update_lap_counter()
 *
 * @param mode CP_INITIALIZE, CP_UPDATE, or CP_CLEANUP
 * @param laps_left Laps remaining
 */
void update_lap_counter(s16 mode, s16 laps_left) {
    static s16 saved_laps;

    switch (mode) {
        case CP_INITIALIZE:
            saved_laps = laps_left;
            break;

        case CP_UPDATE:
            saved_laps = laps_left;
            /* Display would be updated here via HUD system */
            break;

        case CP_CLEANUP:
            /* Clear display */
            break;
    }
}

/**
 * PassedCP - Handle checkpoint crossing for a car
 * Based on arcade: checkpoint.c:PassedCP()
 *
 * Called when a car crosses a checkpoint boundary.
 * Updates lap count, calculates bonus time, triggers audio.
 *
 * @param car_index Index of car that crossed
 * @param flag Non-zero if skipping ahead (cheat/debug)
 */
void PassedCP(s32 car_index, s8 flag) {
    CheckPoint *chkpnt;
    TrackData *trck;
    s32 node;
    s32 laps_remaining;
    s32 current_time, delta_time;
    s32 next_cp, last_cp;

    chkpnt = CP_Track[trackno].chk_point;
    trck = &track_data[trackno];
    node = car_index;

    last_cp = car_array[node].checkpoint;
    next_cp = get_next_checkpoint(last_cp);

    /* Bump lap count when crossing finish line */
    if (next_cp == trck->finish_line) {
        car_array[node].laps++;

        /* Calculate laps remaining for this car */
        laps_remaining = trck->number_of_laps - car_array[node].laps;

        if (node == this_node) {
            update_lap_counter(CP_UPDATE, laps_remaining);
        }
    }

    /* Update checkpoint indices */
    car_array[node].checkpoint = next_cp;

    /* If this is the local player, update timing */
    if (node == this_node) {
        current_time = IRQTIME;
        delta_time = current_time - last_checkpoint_time;
        last_checkpoint_time = current_time;

        /* Could display checkpoint split time here */
    }

    /* Handle finish line crossing */
    if (next_cp == trck->finish_line) {
        laps_remaining = trck->number_of_laps - car_array[node].laps;

        if (laps_remaining <= 0 && car_array[node].place_locked == 0) {
            /* Car finished the race! */
            car_array[node].place_locked = 1;
            end_game_flag = 1;
        }
    }
}

/**
 * JumpToNextCP - Skip to next checkpoint (debug/cheat)
 * Based on arcade: checkpoint.c:JumpToNextCP()
 */
void JumpToNextCP(void) {
    PassedCP(this_node, 1);
}

/**
 * get_checkpoint_bonus_time - Calculate bonus time for checkpoint
 * Based on arcade checkpoint bonus time logic
 *
 * @param cp_index Checkpoint index
 * @param lap Current lap number
 * @return Bonus time in milliseconds
 */
u32 get_checkpoint_bonus_time(s32 cp_index, s32 lap) {
    CheckPoint *chkpnt;
    u32 bonus;

    chkpnt = CP_Track[trackno].chk_point;

    if (lap == 0) {
        /* First lap uses bonus_t[0] */
        bonus = (u32)(chkpnt[cp_index].bonus_t[0] * 1000.0f);
    } else {
        /* Subsequent laps use bonus_t[1] */
        bonus = (u32)(chkpnt[cp_index].bonus_t[1] * 1000.0f);
    }

    return bonus;
}

/**
 * calc_car_distance - Calculate car's distance along track
 * Based on arcade: checkpoint.c path distance calculations
 *
 * Used for race position sorting and rubber-banding.
 *
 * @param car_index Car index
 * @return Distance value (higher = further along)
 */
f32 calc_car_distance(s32 car_index) {
    f32 distance;
    s32 cp, laps;

    cp = car_array[car_index].checkpoint;
    laps = car_array[car_index].laps;

    /* Base distance from laps completed */
    distance = (f32)laps * (f32)num_path_points;

    /* Add checkpoint progress within current lap */
    if (cp < MAX_CHECKPOINTS && cp >= 0) {
        distance += (f32)path_dist_index[cp];
    }

    return distance;
}

/**
 * sort_race_positions - Sort all cars by race position
 * Based on arcade: checkpoint.c:CheckCPs() position sorting
 *
 * Uses selection sort to rank cars by distance.
 * Cars with locked positions (finished) are excluded from sorting.
 */
void sort_race_positions(void) {
    f32 dist_tab[MAX_CARS];
    s16 place[MAX_CARS];
    s32 i, j, index;
    s32 num_left;
    s32 high_index;
    f32 high_dist;
    s16 temp;

    /* Build list of cars that are still racing (not locked) */
    num_left = 0;
    for (i = 0; i < num_active_cars; i++) {
        if (car_array[i].place_locked != 1) {
            place[num_left++] = i;
        }
    }

    if (num_left == 0) {
        return;
    }

    /* Calculate distances for sortable cars */
    for (i = 0; i < num_left; i++) {
        dist_tab[place[i]] = calc_car_distance(place[i]);
        car_array[place[i]].distance = dist_tab[place[i]];
    }

    /* Selection sort by distance (descending - furthest = first place) */
    for (i = 0; i < num_left; i++) {
        high_dist = dist_tab[place[i]];
        high_index = i;

        for (j = i + 1; j < num_left; j++) {
            index = place[j];
            if (dist_tab[index] > dist_tab[place[high_index]]) {
                high_dist = dist_tab[index];
                high_index = j;
            }
        }

        /* Assign position (0 = first, offset by already-finished cars) */
        car_array[place[high_index]].place = i + (num_active_cars - num_left);

        /* Swap to maintain sorted portion */
        temp = place[high_index];
        place[high_index] = place[i];
        place[i] = temp;
    }
}

/**
 * sort_finished_positions - Sort finished cars by score/time
 * Based on arcade: checkpoint.c final position sorting
 *
 * Sorts all cars that have finished the race by their finish time.
 */
void sort_finished_positions(void) {
    s16 place[MAX_CARS];
    s32 i, j;
    s32 num_locked;
    s16 temp;

    /* Build list of finished cars */
    num_locked = 0;
    for (i = 0; i < num_active_cars; i++) {
        if (car_array[i].place_locked == 1) {
            place[num_locked++] = i;
        }
    }

    if (num_locked <= 1) {
        return;
    }

    /* Bubble sort by score (ascending - lowest time = first place) */
    for (i = 0; i < num_locked - 1; i++) {
        for (j = 0; j < num_locked - i - 1; j++) {
            if (car_array[place[j]].score > car_array[place[j + 1]].score) {
                temp = place[j];
                place[j] = place[j + 1];
                place[j + 1] = temp;
            }
        }
    }

    /* Handle ties - lower car index wins */
    for (i = 1; i < num_locked; i++) {
        if ((car_array[place[i - 1]].score == car_array[place[i]].score) &&
            (place[i - 1] > place[i])) {
            temp = place[i];
            place[i] = place[i - 1];
            place[i - 1] = temp;
        }
    }

    /* Assign final positions */
    for (i = 0; i < num_locked; i++) {
        car_array[place[i]].place = i;
    }
}

/**
 * get_race_position - Get a car's current race position
 *
 * @param car_index Car index
 * @return Position (0 = first, 1 = second, etc.)
 */
s32 get_race_position(s32 car_index) {
    if (car_index < 0 || car_index >= MAX_CARS) {
        return -1;
    }
    return car_array[car_index].place;
}

/**
 * get_race_leader - Get the index of the race leader
 *
 * @return Car index of leader, or -1 if no active cars
 */
s32 get_race_leader(void) {
    s32 i;

    for (i = 0; i < num_active_cars; i++) {
        if (car_array[i].place == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * is_in_first_place - Check if local player is in first
 *
 * @return Non-zero if local player is leading
 */
s32 is_in_first_place(void) {
    return (car_array[this_node].place == 0) ? 1 : 0;
}

/**
 * get_distance_to_leader - Get distance behind the leader
 *
 * @param car_index Car to check
 * @return Distance behind leader (negative if leading)
 */
f32 get_distance_to_leader(s32 car_index) {
    s32 leader;
    f32 leader_dist, car_dist;

    leader = get_race_leader();
    if (leader < 0 || car_index == leader) {
        return 0.0f;
    }

    leader_dist = car_array[leader].distance;
    car_dist = car_array[car_index].distance;

    return leader_dist - car_dist;
}

/**
 * get_distance_to_car - Get distance between two cars
 *
 * @param car1 First car index
 * @param car2 Second car index
 * @return Distance (positive if car1 is behind car2)
 */
f32 get_distance_to_car(s32 car1, s32 car2) {
    return car_array[car2].distance - car_array[car1].distance;
}

/**
 * update_all_positions - Full position update (sorting + first place check)
 * Should be called each frame after CheckCPs
 */
void update_all_positions(void) {
    /* Sort racing cars by distance */
    sort_race_positions();

    /* Sort finished cars by time */
    sort_finished_positions();

    /* Check first place status for sound effects */
    if (first_place_time && (IRQTIME - first_place_time) > (3 * ONE_SEC)) {
        first_place_time = 0;
        /* Could play first place sound here */
    }

    if (car_array[this_node].place == 0) {
        if (in_first_place == 0 && first_place_time == 0) {
            first_place_time = IRQTIME;
            in_first_place = 1;
            /* Could play "you're in first" sound here */
        }
    } else {
        in_first_place = 0;
    }
}
