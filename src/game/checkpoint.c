/**
 * checkpoint.c - Checkpoint and lap counting for Rush 2049 N64
 *
 * Based on arcade game/checkpoint.c (1996 Time Warner Interactive)
 * Handles checkpoint detection, lap counting, race positions
 *
 * Key functions mapped from arcade:
 * - init_cp_data -> checkpoint structure initialization
 * - InitCPS -> race start initialization
 * - CheckCPs -> per-frame checkpoint detection
 * - PassedCP -> checkpoint crossing handler
 * - get_next_checkpoint -> checkpoint sequence
 *
 * Arcade source: reference/repos/rushtherock/game/checkpoint.c
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
extern f32 vec_magnitude(const f32 v[3]);
extern void vec_direction(const f32 v[3], f32 result[3]);

/* Track-related externals (from track data) */
extern s32 trackno;                 /* Current track index */
extern s32 num_active_cars;         /* Number of cars in race */
extern s32 this_node;               /* Local player index */
extern s32 gThisNode;               /* Arcade: local node */
extern s32 gMirrorMode;             /* Mirror mode flag */
extern s32 demo_game;               /* Demo mode flag */
extern s32 attract_effects;         /* Attract mode sound effects */
extern s32 mpath_edit;              /* Maxpath editor mode */

/* Path data from track (arcade: extern in checkpoint.c) */
typedef struct PathPoint {
    s32 pos[3];                     /* Position (z, x, -y) in 1/40 units */
} PathPoint;
extern PathPoint *path;             /* Path point array */

/* External functions from other modules */
extern void init_stree(void);       /* Initialize stree data */
extern u32 lsqrt(u32 val);          /* Integer square root */
extern void find_maxpath_intervals(void);
extern void check_mpath_save(s8 cancel);
extern void set_maxpath_index(void *m, s32 index);

/* Timing externals */
#define ONE_SEC     60              /* Frames per second */
#define IRQTIME     frame_counter   /* Current time in frames */

/* Arcade sound codes (stubs) */
#define S_FINALL        0x100
#define S_2L            0x101
#define S_3L            0x102
#define S_4L            0x103
#define S_5L            0x104
#define S_6L            0x105
#define S_7L            0x106
#define S_8L            0x107
#define S_9L            0x108
#define S_CHKPNTSTATIC  0x110
#define S_WINNER        0x111
#define S_BEEP1         0x112
#define S_LEADERLIGHT   0x113
#define S_KLEADERLIGHT  0x114
#define S_KWINNER       0x115

/* Sound macro (stub on N64) */
#define SOUND(x)        /* No-op for now */

/* ========================================================================
 * Checkpoint system state (arcade globals from checkpoint.c)
 * ======================================================================== */
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

/* Additional arcade globals */
u32 old_last_cp_index;
u32 index1, index2;
s16 closest_cp_index[MAX_CPS + 2];
u32 post_object[8];
s8  in_first_place;
s8  cancel_mpath_lap;
f32 lap_loop_distance;
s16 close_chkpnt;
s16 path_point_index;
s8  end_game_flag;
s8  lap_flag;

/* Track/checkpoint data arrays */
CheckPoint TrackCPs[MAX_TRACKS][MAX_CHECKPOINTS];
Track CP_Track[MAX_TRACKS];
TrackData track_data[MAX_TRACKS];

/* Path data for position calculation */
u16 path_start;
u16 path_end;
u16 path_loop;
s16 path_dist_index[MAX_CHECKPOINTS];
u16 path_dist[MAX_PATH_POINTS];
s16 path_index[MAX_PATH_POINTS];
s16 path_to_maxpath[MAX_PATH_POINTS];
s16 num_path_points;

/* Arcade laps_to_go sound array (arcade: checkpoint.c:305) */
const s16 laps_to_go[10] = {
    S_FINALL, S_2L, S_3L, S_4L, S_5L,
    S_6L, S_7L, S_8L, S_9L, 0
};

/* Track lengths (laps) - placeholder until track data loaded */
s16 track_len[MAX_TRACKS] = { 3, 3, 3, 3, 3, 3, 3, 3 };
s16 difficulty[MAX_TRACKS] = { 0, 0, 0, 0, 0, 0, 0, 0 };

/* Init checkpoint data (extern, defined in track data) */
InitCheckPoint *InitCP_Track[MAX_TRACKS];
Init_Track_Data init_track_data[MAX_TRACKS];

/* ========================================================================
 * init_cp_data - Initialize all checkpoint structures
 * Based on arcade: checkpoint.c:init_cp_data() (lines 318-530)
 *
 * This is called once at game startup to:
 * 1. Link CP_Track pointers to TrackCPs arrays
 * 2. Copy Init_Track_Data to Track_Data (converting seconds to frames)
 * 3. Copy InitCheckPoint to CheckPoint for current track
 * 4. Calculate checkpoint UVS matrices for plane tests
 * 5. Find closest track center for each checkpoint
 * ======================================================================== */
void init_cp_data(void) {
    s16 i, j, k;
    s16 t_cent, t_fwd, t_back;
    s32 temp[3];
    f32 delta1[3], delta2[3], pos[3];
    f32 dist[MAX_CPS];
    f32 x, y, z, dx, dy, dz, cur_dist;
    f32 px, py, pz;
    CheckPoint *chkpnt;
    InitCheckPoint *initchkpnt;
    Track_Data *trck;
    s32 itrk;

    /* Link CP_Track checkpoint pointers to TrackCPs arrays */
    for (i = 0; i < MAX_TRACKS; i++) {
        CP_Track[i].chk_point = &TrackCPs[i][0];
    }

    trck = &track_data[trackno];

    /* Initialize data for target track (convert seconds to frames) */
    itrk = trackno;
    for (i = 0; i < 8; i++) {
        trck->start_time[i] = init_track_data[itrk].start_time[i] * ONE_SEC;
        trck->end_time[i] = init_track_data[itrk].end_time[i] * ONE_SEC;
    }

    trck->loop_chkpnt = init_track_data[itrk].loop_chkpnt;
    trck->finish_line = init_track_data[itrk].finish_line;
    trck->before_finish = init_track_data[itrk].before_finish;

    /* Initialize checkpoint structure for current track */
    initchkpnt = InitCP_Track[itrk];
    chkpnt = CP_Track[trackno].chk_point;

    /* Skip if init data not set up yet */
    if (initchkpnt == NULL) {
        CP_Track[trackno].num_checkpoints = 0;
        return;
    }

    /* Copy init checkpoint data to runtime checkpoint data */
    for (j = 0; j < MAX_CPS; j++) {
        chkpnt[j].logical_cp = initchkpnt[j].logical_cp;
        for (k = 0; k < 2; k++) {
            chkpnt[j].bonus_t[k] = initchkpnt[j].best_t[k] * ONE_SEC;
        }
        chkpnt[j].radius = initchkpnt[j].radius;

        if (chkpnt[j].logical_cp == -1) {
            break;
        }
    }

    CP_Track[trackno].num_checkpoints = j;

    /* Initialize important track data (path_end etc.) */
    init_stree();

    /* Reload pointers after init_stree may have modified them */
    initchkpnt = InitCP_Track[itrk];
    chkpnt = CP_Track[trackno].chk_point;

    /* Initialize distance array */
    for (i = 0; i < CP_Track[trackno].num_checkpoints; i++) {
        dist[i] = 9999999.0f;
    }

    px = py = pz = 99999.0f * 40.0f;

    /* Determine closest track center for each checkpoint (linear search) */
    for (i = (s16)path_start; i < (s16)path_end; i++) {
        /* Convert path coordinates (path is in 1/40 foot units, z/x/-y order) */
        x = (f32)path[i].pos[1] / 40.0f;
        y = (f32)(-path[i].pos[2]) / 40.0f;
        z = (f32)path[i].pos[0] / 40.0f;

        px = x;
        py = y;
        pz = z;

        for (j = 0; j < CP_Track[trackno].num_checkpoints; j++) {
            /* Swap checkpoint x for mirrored tracks */
            if (gMirrorMode) {
                dx = -initchkpnt[j].pos[0] - x;
            } else {
                dx = initchkpnt[j].pos[0] - x;
            }
            dy = initchkpnt[j].pos[1] - y;
            dz = initchkpnt[j].pos[2] - z;

            cur_dist = dx * dx + dy * dy + dz * dz;

            if (cur_dist < dist[j]) {
                dist[j] = cur_dist;
                chkpnt[j].track_cent = i;
            }
        }
    }

    /* Determine track direction from checkpoint ordering */
    k = 0;
    for (j = 0; j < CP_Track[trackno].num_checkpoints; j++) {
        i = chkpnt[j].track_cent;

        if (j != 0) {
            k += (i < chkpnt[j - 1].track_cent);
        }

        /* Save checkpoint position from nearest path point */
        chkpnt[j].pos[0] = (f32)path[i].pos[1] / 40.0f;
        chkpnt[j].pos[1] = (f32)(-path[i].pos[2]) / 40.0f;
        chkpnt[j].pos[2] = (f32)path[i].pos[0] / 40.0f;
    }

    /* Set track direction based on checkpoint ordering */
    if (k > 1) {
        CP_Track[trackno].direction = -1;
    } else {
        CP_Track[trackno].direction = 1;
    }

    /* Calculate UVS matrices for all checkpoints */
    for (k = 0; k < CP_Track[trackno].num_checkpoints; k++) {
        t_cent = chkpnt[k].track_cent;
        t_fwd = get_next_center(t_cent, CP_Track[trackno].direction);
        t_back = get_prev_center(t_cent, CP_Track[trackno].direction);

        /* delta1 = cent - back */
        temp[0] = path[t_cent].pos[0] - path[t_back].pos[0];
        temp[1] = path[t_cent].pos[1] - path[t_back].pos[1];
        temp[2] = path[t_cent].pos[2] - path[t_back].pos[2];
        delta1[0] = ((f32)temp[1]) / 40.0f;
        delta1[1] = 0.0f;
        delta1[2] = ((f32)temp[0]) / 40.0f;

        /* delta2 = forward - cent */
        temp[0] = path[t_fwd].pos[0] - path[t_cent].pos[0];
        temp[1] = path[t_fwd].pos[1] - path[t_cent].pos[1];
        temp[2] = path[t_fwd].pos[2] - path[t_cent].pos[2];
        delta2[0] = ((f32)temp[1]) / 40.0f;
        delta2[1] = 0.0f;
        delta2[2] = ((f32)temp[0]) / 40.0f;

        /* Normalize deltas - use simple normalization */
        {
            f32 mag1 = delta1[0] * delta1[0] + delta1[2] * delta1[2];
            f32 mag2 = delta2[0] * delta2[0] + delta2[2] * delta2[2];

            if (mag1 > 0.01f) {
                f32 inv_mag = 1.0f / (f32)lsqrt((u32)(mag1 * 1000000.0f)) * 1000.0f;
                delta1[0] *= inv_mag;
                delta1[2] *= inv_mag;
            }
            if (mag2 > 0.01f) {
                f32 inv_mag = 1.0f / (f32)lsqrt((u32)(mag2 * 1000000.0f)) * 1000.0f;
                delta2[0] *= inv_mag;
                delta2[2] *= inv_mag;
            }
        }

        /* Average two deltas to get forward direction */
        pos[0] = (delta1[0] + delta2[0]) / 2.0f;
        pos[1] = 0.0f;
        pos[2] = (delta1[2] + delta2[2]) / 2.0f;

        /* Normalize averaged direction */
        {
            f32 mag = pos[0] * pos[0] + pos[2] * pos[2];
            if (mag < 0.01f) {
                pos[0] = 0.0f;
                pos[2] = 1.0f;
            } else {
                f32 inv_mag = 1.0f / (f32)lsqrt((u32)(mag * 1000000.0f)) * 1000.0f;
                pos[0] *= inv_mag;
                pos[2] *= inv_mag;
            }
        }

        /* Build UVS matrix (X = right, Y = up, Z = forward) */
        /* X axis (perpendicular to forward, in XZ plane) */
        chkpnt[k].uvs[0][0] = pos[2];
        chkpnt[k].uvs[1][0] = 0.0f;
        chkpnt[k].uvs[2][0] = pos[0];

        /* Y axis (up) */
        chkpnt[k].uvs[0][1] = 0.0f;
        chkpnt[k].uvs[1][1] = 1.0f;
        chkpnt[k].uvs[2][1] = 0.0f;

        /* Z axis (forward - used for plane test) */
        chkpnt[k].uvs[0][2] = -pos[0];
        chkpnt[k].uvs[1][2] = 0.0f;
        chkpnt[k].uvs[2][2] = pos[2];
    }
}

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

/* ========================================================================
 * Arcade Debug/Display Functions (stubs for N64)
 * These are from arcade checkpoint.c but not needed on N64
 * ======================================================================== */

/**
 * FPutCheckpoints - Place checkpoint debug objects
 * Arcade: checkpoint.c:FPutCheckpoints() (line 1136)
 *
 * On arcade, this places checkpoint visualization objects.
 * Stub on N64 since we don't have the debug display system.
 */
void FPutCheckpoints(s16 track) {
    /* Stub - no debug checkpoint objects on N64 */
    (void)track;
}

/**
 * FPlaceOneCP - Place one checkpoint object
 * Arcade: checkpoint.c:FPlaceOneCP() (line 1157)
 *
 * @param chkpnt Checkpoint to place
 * @param cp_objnum Object index
 * @return Object handle (0 on N64 since not implemented)
 */
u32 FPlaceOneCP(CheckPoint *chkpnt, s32 cp_objnum) {
    /* Stub - would return MBOX_NewObject on arcade */
    (void)chkpnt;
    (void)cp_objnum;
    return 0;
}

/**
 * display_chkpnts - Display checkpoint table (debug)
 * Arcade: checkpoint.c:display_chkpnts() (line 1434)
 */
void display_chkpnts(s16 mode) {
    /* Stub - arcade debug display */
    (void)mode;
}

/**
 * add_chkpnt - Add checkpoint (debug editor)
 * Arcade: checkpoint.c:add_chkpnt() (line 1498)
 */
void add_chkpnt(s16 mode) {
    /* Stub - arcade checkpoint editor */
    (void)mode;
}

/**
 * del_chkpnt - Delete checkpoint (debug editor)
 * Arcade: checkpoint.c:del_chkpnt() (line 1503)
 */
void del_chkpnt(s16 mode) {
    /* Stub - arcade checkpoint editor */
    (void)mode;
}

/**
 * fwd_chkpnt - Forward checkpoint (debug editor)
 * Arcade: checkpoint.c:fwd_chkpnt() (line 1508)
 */
void fwd_chkpnt(s16 mode) {
    /* Stub - arcade checkpoint editor */
    (void)mode;
}

/**
 * back_chkpnt - Back checkpoint (debug editor)
 * Arcade: checkpoint.c:back_chkpnt() (line 1537)
 */
void back_chkpnt(s16 mode) {
    /* Stub - arcade checkpoint editor */
    (void)mode;
}

/**
 * display_path_points - Display path point table (debug)
 * Arcade: checkpoint.c:display_path_points() (line 1572)
 */
void display_path_points(s16 mode) {
    /* Stub - arcade debug display */
    (void)mode;
}

/**
 * fwd_path_points - Forward path points (debug)
 * Arcade: checkpoint.c:fwd_path_points() (line 1609)
 */
void fwd_path_points(s16 mode) {
    /* Stub - arcade path editor */
    (void)mode;
}

/**
 * back_path_points - Back path points (debug)
 * Arcade: checkpoint.c:back_path_points() (line 1625)
 */
void back_path_points(s16 mode) {
    /* Stub - arcade path editor */
    (void)mode;
}
