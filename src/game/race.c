/**
 * race.c - Race timing and management for Rush 2049 N64
 *
 * Based on arcade game/game.c and game/checkpoint.c
 * Handles lap timing, positions, checkpoints, and race state.
 */

#include "types.h"
#include "game/race.h"
#include "game/structs.h"

/* External game state */
extern u8 gstate;
extern u32 frame_counter;
extern CarData car_array[];
extern s32 num_active_cars;
extern s32 this_car;

/* External functions */
extern f32 sqrtf(f32 x);

/* Global race state */
RaceState gRace;

/**
 * race_init - Initialize race system
 */
void race_init(void) {
    s32 i, j;

    gRace.config.num_laps = 3;
    gRace.config.num_checkpoints = 8;
    gRace.config.start_time = 90 * RACE_FRAMES_PER_SEC;  /* 90 seconds */
    gRace.config.checkpoint_bonus = 30 * RACE_FRAMES_PER_SEC;  /* 30 sec bonus */
    gRace.config.difficulty = 1;
    gRace.config.flags = 0;
    gRace.config.track_id = 0;

    gRace.num_cars = 0;
    gRace.race_time = 0;
    gRace.countdown_timer = 0;
    gRace.countdown_state = COUNTDOWN_READY;
    gRace.race_flags = 0;
    gRace.leader = 0;
    gRace.num_finished = 0;
    gRace.time_remaining = gRace.config.start_time;

    for (i = 0; i < MAX_RACE_CARS; i++) {
        gRace.cars[i].start_time = 0;
        gRace.cars[i].finish_time = 0;
        gRace.cars[i].total_time = 0;
        gRace.cars[i].best_lap_time = 0xFFFFFFFF;
        gRace.cars[i].current_lap_start = 0;
        gRace.cars[i].position = i + 1;
        gRace.cars[i].current_lap = 1;
        gRace.cars[i].finished = 0;
        gRace.cars[i].result = RESULT_NONE;
        gRace.cars[i].track_progress = 0.0f;
        gRace.cars[i].total_progress = 0.0f;

        for (j = 0; j < MAX_LAPS; j++) {
            gRace.cars[i].lap_times[j].frames = 0;
            gRace.cars[i].lap_times[j].valid = 0;
            gRace.cars[i].lap_times[j].best = 0;
        }

        race_reset_checkpoint_state(i);
    }
}

/**
 * race_reset - Reset race for new start
 */
void race_reset(void) {
    race_init();
}

/**
 * race_start - Start a new race
 *
 * @param num_laps Number of laps
 * @param num_cars Number of cars in race
 */
void race_start(s32 num_laps, s32 num_cars) {
    s32 i;

    gRace.config.num_laps = num_laps;
    gRace.num_cars = num_cars;
    gRace.race_time = 0;
    gRace.race_flags = RACE_FLAG_STARTED;
    gRace.leader = 0;
    gRace.num_finished = 0;
    gRace.time_remaining = gRace.config.start_time;

    for (i = 0; i < num_cars; i++) {
        gRace.cars[i].start_time = frame_counter;
        gRace.cars[i].current_lap_start = frame_counter;
        gRace.cars[i].current_lap = 1;
        gRace.cars[i].finished = 0;
        gRace.cars[i].position = i + 1;
        race_reset_checkpoint_state(i);
    }
}

/**
 * race_configure - Set race configuration
 *
 * @param config Race configuration
 */
void race_configure(RaceConfig *config) {
    gRace.config = *config;
    gRace.time_remaining = config->start_time;
}

/**
 * race_update - Per-frame race update
 */
void race_update(void) {
    if (!(gRace.race_flags & RACE_FLAG_STARTED)) {
        return;
    }

    if (gRace.race_flags & RACE_FLAG_PAUSED) {
        return;
    }

    /* Update race time */
    gRace.race_time++;

    /* Update countdown if active */
    if (gRace.race_flags & RACE_FLAG_COUNTDOWN) {
        race_update_countdown();
    }

    /* Decrement time remaining */
    if (gRace.time_remaining > 0) {
        gRace.time_remaining--;
    }

    /* Update car positions */
    race_update_positions();

    /* Update checkpoint states */
    race_update_checkpoints();

    /* Check for race completion */
    if (gRace.num_finished >= gRace.num_cars) {
        gRace.race_flags |= RACE_FLAG_FINISHED;
    }
}

/**
 * race_update_positions - Update race positions based on progress
 */
void race_update_positions(void) {
    s32 i, j;
    f32 progress[MAX_RACE_CARS];
    s32 positions[MAX_RACE_CARS];
    f32 temp_prog;
    s32 temp_pos;

    if (gRace.num_cars <= 0) {
        return;
    }

    /* Calculate total progress for each car */
    for (i = 0; i < gRace.num_cars; i++) {
        if (gRace.cars[i].finished) {
            /* Finished cars get maximum progress plus finish time bonus */
            progress[i] = (f32)(gRace.config.num_laps + 1) * 100.0f -
                         (f32)(gRace.cars[i].finish_time - gRace.cars[i].start_time) * 0.0001f;
        } else {
            progress[i] = ((f32)(gRace.cars[i].current_lap - 1) +
                          gRace.cars[i].track_progress) * 100.0f;
        }
        positions[i] = i;
        gRace.cars[i].total_progress = progress[i];
    }

    /* Sort by progress (descending) - bubble sort for simplicity */
    for (i = 0; i < gRace.num_cars - 1; i++) {
        for (j = 0; j < gRace.num_cars - i - 1; j++) {
            if (progress[j] < progress[j + 1]) {
                /* Swap progress */
                temp_prog = progress[j];
                progress[j] = progress[j + 1];
                progress[j + 1] = temp_prog;
                /* Swap position indices */
                temp_pos = positions[j];
                positions[j] = positions[j + 1];
                positions[j + 1] = temp_pos;
            }
        }
    }

    /* Assign positions */
    for (i = 0; i < gRace.num_cars; i++) {
        gRace.cars[positions[i]].position = i + 1;
    }

    /* Update leader */
    gRace.leader = positions[0];
}

/**
 * race_update_checkpoints - Update checkpoint detection
 */
void race_update_checkpoints(void) {
    s32 i;
    CarData *car;
    CheckpointState *cp;

    for (i = 0; i < gRace.num_cars; i++) {
        car = &car_array[i];
        cp = &gRace.cars[i].cp_state;

        /* Check if car passed next expected checkpoint */
        /* This would be done by comparing car position to checkpoint triggers */

        /* Update track progress based on checkpoint */
        if (gRace.config.num_checkpoints > 0) {
            gRace.cars[i].track_progress =
                (f32)cp->last_cp / (f32)gRace.config.num_checkpoints;
        }
    }
}

/**
 * race_start_countdown - Start pre-race countdown
 */
void race_start_countdown(void) {
    gRace.countdown_timer = 4 * RACE_FRAMES_PER_SEC;  /* 4 seconds */
    gRace.countdown_state = COUNTDOWN_READY;
    gRace.race_flags |= RACE_FLAG_COUNTDOWN;
}

/**
 * race_update_countdown - Update countdown state
 */
void race_update_countdown(void) {
    if (gRace.countdown_timer > 0) {
        gRace.countdown_timer--;

        /* Update countdown state based on timer */
        if (gRace.countdown_timer > 3 * RACE_FRAMES_PER_SEC) {
            gRace.countdown_state = COUNTDOWN_READY;
        } else if (gRace.countdown_timer > 2 * RACE_FRAMES_PER_SEC) {
            gRace.countdown_state = COUNTDOWN_3;
        } else if (gRace.countdown_timer > RACE_FRAMES_PER_SEC) {
            gRace.countdown_state = COUNTDOWN_2;
        } else if (gRace.countdown_timer > 0) {
            gRace.countdown_state = COUNTDOWN_1;
        } else {
            gRace.countdown_state = COUNTDOWN_GO;
        }
    } else {
        gRace.countdown_state = COUNTDOWN_DONE;
        gRace.race_flags &= ~RACE_FLAG_COUNTDOWN;
    }
}

/**
 * race_countdown_finished - Check if countdown is complete
 *
 * @return Non-zero if countdown done
 */
s32 race_countdown_finished(void) {
    return (gRace.countdown_state == COUNTDOWN_DONE ||
            gRace.countdown_state == COUNTDOWN_GO);
}

/**
 * race_car_crossed_start - Handle car crossing start/finish line
 *
 * @param car_index Car that crossed
 */
void race_car_crossed_start(s32 car_index) {
    CarRaceState *car;
    u32 lap_time;

    if (car_index < 0 || car_index >= gRace.num_cars) {
        return;
    }

    car = &gRace.cars[car_index];

    /* Don't process if already finished */
    if (car->finished) {
        return;
    }

    /* Check if going wrong way */
    if (car->cp_state.wrong_way) {
        return;
    }

    /* Calculate lap time */
    lap_time = frame_counter - car->current_lap_start;

    /* Record lap time */
    if (car->current_lap > 0 && car->current_lap <= MAX_LAPS) {
        car->lap_times[car->current_lap - 1].frames = lap_time;
        car->lap_times[car->current_lap - 1].valid = 1;

        /* Check for best lap */
        if (lap_time < car->best_lap_time) {
            car->best_lap_time = lap_time;
            car->lap_times[car->current_lap - 1].best = 1;
        }
    }

    /* Check if race complete */
    if (car->current_lap >= gRace.config.num_laps) {
        race_car_finished_race(car_index);
    } else {
        /* Start next lap */
        car->current_lap++;
        car->current_lap_start = frame_counter;
        race_reset_checkpoint_state(car_index);

        /* Check for final lap */
        if (car->current_lap == gRace.config.num_laps) {
            gRace.race_flags |= RACE_FLAG_FINAL_LAP;
        }
    }
}

/**
 * race_car_finished_lap - Handle lap completion
 *
 * @param car_index Car that finished lap
 */
void race_car_finished_lap(s32 car_index) {
    race_car_crossed_start(car_index);
}

/**
 * race_car_finished_race - Handle race completion
 *
 * @param car_index Car that finished
 */
void race_car_finished_race(s32 car_index) {
    CarRaceState *car;

    if (car_index < 0 || car_index >= gRace.num_cars) {
        return;
    }

    car = &gRace.cars[car_index];

    if (car->finished) {
        return;
    }

    car->finished = 1;
    car->finish_time = frame_counter;
    car->total_time = car->finish_time - car->start_time;

    /* Determine result */
    if (gRace.num_finished == 0) {
        car->result = RESULT_WIN;
    } else {
        car->result = RESULT_FINISH;
    }

    gRace.num_finished++;
}

/**
 * race_get_lap_time - Get lap time for car
 *
 * @param car_index Car index
 * @param lap Lap number (1-based)
 * @return Lap time in frames or 0
 */
u32 race_get_lap_time(s32 car_index, s32 lap) {
    if (car_index < 0 || car_index >= gRace.num_cars) {
        return 0;
    }
    if (lap < 1 || lap > MAX_LAPS) {
        return 0;
    }

    return gRace.cars[car_index].lap_times[lap - 1].frames;
}

/**
 * race_get_best_lap - Get best lap time for car
 *
 * @param car_index Car index
 * @return Best lap time in frames
 */
u32 race_get_best_lap(s32 car_index) {
    if (car_index < 0 || car_index >= gRace.num_cars) {
        return 0;
    }

    return gRace.cars[car_index].best_lap_time;
}

/**
 * race_car_hit_checkpoint - Handle checkpoint crossing
 *
 * @param car_index Car that hit checkpoint
 * @param checkpoint_id Checkpoint identifier
 */
void race_car_hit_checkpoint(s32 car_index, s32 checkpoint_id) {
    CarRaceState *car;
    CheckpointState *cp;

    if (car_index < 0 || car_index >= gRace.num_cars) {
        return;
    }

    car = &gRace.cars[car_index];
    cp = &car->cp_state;

    /* Check if this is the expected checkpoint */
    if (checkpoint_id == cp->next_cp) {
        /* Mark checkpoint as hit */
        cp->cp_hit[checkpoint_id / 8] |= (1 << (checkpoint_id % 8));
        cp->last_cp = checkpoint_id;
        cp->next_cp = (checkpoint_id + 1) % gRace.config.num_checkpoints;
        cp->wrong_way = 0;

        /* Add time bonus */
        gRace.time_remaining += gRace.config.checkpoint_bonus;
    } else {
        /* Check for wrong way */
        s32 expected = cp->next_cp;
        s32 prev = (expected - 1 + gRace.config.num_checkpoints) % gRace.config.num_checkpoints;

        if (checkpoint_id == prev) {
            cp->wrong_way = 1;
        }
    }

    /* Update progress */
    car->track_progress = (f32)cp->last_cp / (f32)gRace.config.num_checkpoints;
}

/**
 * race_is_wrong_way - Check if car is going wrong way
 *
 * @param car_index Car index
 * @return Non-zero if wrong way
 */
s32 race_is_wrong_way(s32 car_index) {
    if (car_index < 0 || car_index >= gRace.num_cars) {
        return 0;
    }

    return gRace.cars[car_index].cp_state.wrong_way;
}

/**
 * race_reset_checkpoint_state - Reset checkpoint state for new lap
 *
 * @param car_index Car index
 */
void race_reset_checkpoint_state(s32 car_index) {
    s32 i;
    CheckpointState *cp;

    if (car_index < 0 || car_index >= MAX_RACE_CARS) {
        return;
    }

    cp = &gRace.cars[car_index].cp_state;

    cp->last_cp = 0;
    cp->next_cp = 1;  /* Expecting first checkpoint */
    cp->wrong_way = 0;
    cp->on_shortcut = 0;

    for (i = 0; i < MAX_CHECKPOINTS / 8; i++) {
        cp->cp_hit[i] = 0;
    }
}

/**
 * race_get_position - Get race position for car
 *
 * @param car_index Car index
 * @return Position (1-based)
 */
s32 race_get_position(s32 car_index) {
    if (car_index < 0 || car_index >= gRace.num_cars) {
        return 0;
    }

    return gRace.cars[car_index].position;
}

/**
 * race_get_leader - Get current race leader
 *
 * @return Leader car index
 */
s32 race_get_leader(void) {
    return gRace.leader;
}

/**
 * race_get_progress - Get track progress for car
 *
 * @param car_index Car index
 * @return Progress (0.0-1.0)
 */
f32 race_get_progress(s32 car_index) {
    if (car_index < 0 || car_index >= gRace.num_cars) {
        return 0.0f;
    }

    return gRace.cars[car_index].track_progress;
}

/**
 * race_get_time - Get current race time
 *
 * @return Race time in frames
 */
u32 race_get_time(void) {
    return gRace.race_time;
}

/**
 * race_get_car_time - Get race time for specific car
 *
 * @param car_index Car index
 * @return Car's race time in frames
 */
u32 race_get_car_time(s32 car_index) {
    if (car_index < 0 || car_index >= gRace.num_cars) {
        return 0;
    }

    if (gRace.cars[car_index].finished) {
        return gRace.cars[car_index].total_time;
    }

    return frame_counter - gRace.cars[car_index].start_time;
}

/**
 * race_get_time_remaining - Get time remaining
 *
 * @return Time remaining in frames
 */
s32 race_get_time_remaining(void) {
    return gRace.time_remaining;
}

/**
 * race_extend_time - Add time to remaining
 *
 * @param frames Frames to add
 */
void race_extend_time(s32 frames) {
    gRace.time_remaining += frames;
    gRace.race_flags |= RACE_FLAG_EXTENDED;
}

/**
 * race_is_started - Check if race has started
 *
 * @return Non-zero if started
 */
s32 race_is_started(void) {
    return (gRace.race_flags & RACE_FLAG_STARTED) != 0;
}

/**
 * race_is_finished - Check if race is finished
 *
 * @return Non-zero if finished
 */
s32 race_is_finished(void) {
    return (gRace.race_flags & RACE_FLAG_FINISHED) != 0;
}

/**
 * race_is_paused - Check if race is paused
 *
 * @return Non-zero if paused
 */
s32 race_is_paused(void) {
    return (gRace.race_flags & RACE_FLAG_PAUSED) != 0;
}

/**
 * race_car_has_finished - Check if car has finished
 *
 * @param car_index Car index
 * @return Non-zero if finished
 */
s32 race_car_has_finished(s32 car_index) {
    if (car_index < 0 || car_index >= gRace.num_cars) {
        return 0;
    }

    return gRace.cars[car_index].finished;
}

/**
 * race_format_time - Format frame count as time string
 *
 * @param frames Time in frames
 * @param buffer Output buffer (at least 12 chars)
 */
void race_format_time(u32 frames, char *buffer) {
    u32 total_secs;
    u32 mins, secs, hundredths;

    total_secs = frames / RACE_FRAMES_PER_SEC;
    mins = total_secs / 60;
    secs = total_secs % 60;
    hundredths = (frames % RACE_FRAMES_PER_SEC) * 100 / RACE_FRAMES_PER_SEC;

    /* Format as M:SS.HH or MM:SS.HH */
    if (mins >= 10) {
        buffer[0] = '0' + (mins / 10);
        buffer[1] = '0' + (mins % 10);
        buffer[2] = ':';
        buffer[3] = '0' + (secs / 10);
        buffer[4] = '0' + (secs % 10);
        buffer[5] = '.';
        buffer[6] = '0' + (hundredths / 10);
        buffer[7] = '0' + (hundredths % 10);
        buffer[8] = '\0';
    } else {
        buffer[0] = '0' + mins;
        buffer[1] = ':';
        buffer[2] = '0' + (secs / 10);
        buffer[3] = '0' + (secs % 10);
        buffer[4] = '.';
        buffer[5] = '0' + (hundredths / 10);
        buffer[6] = '0' + (hundredths % 10);
        buffer[7] = '\0';
    }
}

/**
 * race_format_time_diff - Format time difference
 *
 * @param diff_frames Time difference in frames (can be negative)
 * @param buffer Output buffer (at least 10 chars)
 */
void race_format_time_diff(s32 diff_frames, char *buffer) {
    u32 abs_frames;
    u32 secs, hundredths;
    s32 i = 0;

    /* Handle sign */
    if (diff_frames < 0) {
        buffer[i++] = '-';
        abs_frames = (u32)(-diff_frames);
    } else {
        buffer[i++] = '+';
        abs_frames = (u32)diff_frames;
    }

    secs = abs_frames / RACE_FRAMES_PER_SEC;
    hundredths = (abs_frames % RACE_FRAMES_PER_SEC) * 100 / RACE_FRAMES_PER_SEC;

    /* Format as +S.HH or +SS.HH */
    if (secs >= 10) {
        buffer[i++] = '0' + (secs / 10);
    }
    buffer[i++] = '0' + (secs % 10);
    buffer[i++] = '.';
    buffer[i++] = '0' + (hundredths / 10);
    buffer[i++] = '0' + (hundredths % 10);
    buffer[i] = '\0';
}
