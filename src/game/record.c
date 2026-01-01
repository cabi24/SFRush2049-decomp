/**
 * record.c - Track and Lap Record System for Rush 2049 N64
 *
 * Handles best lap times, track records, and split times.
 * Works alongside hiscore.c for complete time tracking.
 *
 * Based on arcade checkpoint.c and hiscore.c patterns.
 */

#include "game/record.h"
#include "types.h"

#ifdef NON_MATCHING

/* ========================================================================
 * GLOBAL STATE
 * ======================================================================== */

RecordState gRecordState;

/* Default best times per track (in milliseconds) */
static const u32 sDefaultBestLaps[RECORD_MAX_TRACKS] = {
    45000,  /* Track 1: 45 seconds */
    50000,  /* Track 2: 50 seconds */
    55000,  /* Track 3: 55 seconds */
    60000,  /* Track 4: 60 seconds */
    52000,  /* Track 5: 52 seconds */
    48000,  /* Track 6: 48 seconds */
    58000,  /* Track 7: 58 seconds */
    62000   /* Track 8: 62 seconds */
};

static const u32 sDefaultBestRaces[RECORD_MAX_TRACKS] = {
    180000, /* Track 1: 3:00 */
    200000, /* Track 2: 3:20 */
    220000, /* Track 3: 3:40 */
    240000, /* Track 4: 4:00 */
    210000, /* Track 5: 3:30 */
    190000, /* Track 6: 3:10 */
    230000, /* Track 7: 3:50 */
    250000  /* Track 8: 4:10 */
};

/* ========================================================================
 * INITIALIZATION
 * ======================================================================== */

/**
 * record_init - Initialize the record system
 * Based on arcade: InitGameScores() pattern
 */
void record_init(void) {
    s32 i;
    u8 *ptr;

    /* Clear all state */
    ptr = (u8 *)&gRecordState;
    for (i = 0; i < (s32)sizeof(RecordState); i++) {
        ptr[i] = 0;
    }

    /* Initialize default player name */
    gRecordState.player_name[0] = 'A';
    gRecordState.player_name[1] = 'A';
    gRecordState.player_name[2] = 'A';
    gRecordState.player_name[3] = '\0';

    /* Initialize all track records with defaults */
    for (i = 0; i < RECORD_MAX_TRACKS; i++) {
        record_init_track((u8)i);
    }
}

/**
 * record_reset - Reset current race state
 */
void record_reset(void) {
    gRecordState.racing = 0;
    gRecordState.current_lap = 0;
    gRecordState.race_start_time = 0;
    gRecordState.lap_start_time = 0;
    gRecordState.last_split_time = 0;
    gRecordState.last_checkpoint = 0;
}

/**
 * record_init_track - Initialize records for a single track
 */
void record_init_track(u8 track_id) {
    TrackRecord *track;
    s32 i;

    if (track_id >= RECORD_MAX_TRACKS) {
        return;
    }

    track = &gRecordState.tracks[track_id];

    /* Clear track record */
    track->track_id = track_id;
    track->valid = 1;
    track->mirror_mode = 0;

    /* Initialize best race time */
    track->best_race.name[0] = 'C';
    track->best_race.name[1] = 'P';
    track->best_race.name[2] = 'U';
    track->best_race.name[3] = '\0';
    track->best_race.time = sDefaultBestRaces[track_id];
    track->best_race.car_type = 0;
    track->best_race.flags = RECORD_FLAG_VALID;
    track->best_race.track_id = track_id;
    track->best_race.lap_num = 0;

    /* Initialize best lap time */
    track->lap.best.name[0] = 'C';
    track->lap.best.name[1] = 'P';
    track->lap.best.name[2] = 'U';
    track->lap.best.name[3] = '\0';
    track->lap.best.time = sDefaultBestLaps[track_id];
    track->lap.best.car_type = 0;
    track->lap.best.flags = RECORD_FLAG_VALID;
    track->lap.best.track_id = track_id;
    track->lap.best.lap_num = 1;

    /* Clear current race lap data */
    track->lap.current.time = 0;
    track->lap.current.flags = 0;
    track->lap.num_laps = 0;
    track->lap.best_lap_num = 0;

    for (i = 0; i < RECORD_MAX_LAPS; i++) {
        track->lap.lap_times[i] = 0;
    }

    /* Initialize splits */
    track->splits.num_splits = 0;
    track->splits.flags = 0;

    for (i = 0; i < RECORD_MAX_SPLITS; i++) {
        track->splits.best_splits[i] = 0;
        track->splits.current_splits[i] = 0;
    }
}

/* ========================================================================
 * RACE LIFECYCLE
 * ======================================================================== */

/**
 * record_start_race - Called when a race begins
 * Based on arcade: preplay() initialization pattern
 */
void record_start_race(u8 track_id, u8 car_type, u8 mirror_mode) {
    TrackRecord *track;
    s32 i;

    if (track_id >= RECORD_MAX_TRACKS) {
        return;
    }

    gRecordState.current_track = track_id;
    gRecordState.car_type = car_type;
    gRecordState.mirror_mode = mirror_mode;
    gRecordState.racing = 1;
    gRecordState.current_lap = 0;
    gRecordState.race_start_time = 0;  /* Will be set by timing system */
    gRecordState.lap_start_time = 0;
    gRecordState.last_split_time = 0;
    gRecordState.last_checkpoint = 0;

    /* Clear current race lap times */
    track = &gRecordState.tracks[track_id];
    track->lap.num_laps = 0;
    track->lap.best_lap_num = 0;
    track->lap.current.time = 0;
    track->lap.current.flags = 0;

    for (i = 0; i < RECORD_MAX_LAPS; i++) {
        track->lap.lap_times[i] = 0;
    }

    /* Clear current splits */
    for (i = 0; i < RECORD_MAX_SPLITS; i++) {
        track->splits.current_splits[i] = 0;
    }
}

/**
 * record_end_race - Called when a race finishes
 * Based on arcade: EnterHighScore() pattern
 */
void record_end_race(u32 final_time) {
    u8 track_id;

    if (!gRecordState.racing) {
        return;
    }

    track_id = gRecordState.current_track;
    if (track_id >= RECORD_MAX_TRACKS) {
        gRecordState.racing = 0;
        return;
    }

    /* Check for new race record */
    if (record_is_new_race_record(track_id, final_time)) {
        record_save_race(track_id, final_time);
    }

    gRecordState.racing = 0;
}

/**
 * record_abort_race - Called when a race is aborted
 */
void record_abort_race(void) {
    gRecordState.racing = 0;
    record_reset();
}

/* ========================================================================
 * LAP TRACKING
 * ======================================================================== */

/**
 * record_start_lap - Called at the start of each lap
 * Based on arcade checkpoint.c lap handling
 */
void record_start_lap(void) {
    if (!gRecordState.racing) {
        return;
    }

    gRecordState.lap_start_time = 0;  /* Will be set by timing system */
    gRecordState.last_checkpoint = 0;
}

/**
 * record_end_lap - Called when a lap is completed
 * Based on arcade checkpoint.c finish line crossing
 */
void record_end_lap(u32 lap_time) {
    TrackRecord *track;
    u8 track_id;
    u8 lap_num;

    if (!gRecordState.racing) {
        return;
    }

    track_id = gRecordState.current_track;
    if (track_id >= RECORD_MAX_TRACKS) {
        return;
    }

    track = &gRecordState.tracks[track_id];
    lap_num = gRecordState.current_lap;

    /* Store lap time */
    if (lap_num < RECORD_MAX_LAPS) {
        track->lap.lap_times[lap_num] = lap_time;
        track->lap.num_laps = lap_num + 1;

        /* Check if this is the best lap this race */
        if (track->lap.current.time == 0 || lap_time < track->lap.current.time) {
            track->lap.current.time = lap_time;
            track->lap.current.lap_num = lap_num + 1;
            track->lap.current.flags = RECORD_FLAG_VALID | RECORD_FLAG_NEW;
            track->lap.best_lap_num = lap_num + 1;
        }

        /* Check for new all-time lap record */
        if (record_is_new_lap_record(track_id, lap_time)) {
            record_save_lap(track_id, lap_time, lap_num + 1);
        }
    }

    /* Move to next lap */
    gRecordState.current_lap++;
    record_start_lap();
}

/**
 * record_get_current_lap_time - Get elapsed time for current lap
 */
u32 record_get_current_lap_time(void) {
    /* This should be called with actual elapsed time from timing system */
    /* Returning 0 as placeholder - actual implementation would query timer */
    return 0;
}

/* ========================================================================
 * CHECKPOINT/SPLIT TRACKING
 * ======================================================================== */

/**
 * record_checkpoint - Called when passing a checkpoint
 * Based on arcade checkpoint.c CheckCPs() pattern
 */
void record_checkpoint(u8 checkpoint_id, u32 split_time) {
    TrackRecord *track;
    u8 track_id;

    if (!gRecordState.racing) {
        return;
    }

    track_id = gRecordState.current_track;
    if (track_id >= RECORD_MAX_TRACKS) {
        return;
    }

    track = &gRecordState.tracks[track_id];

    if (checkpoint_id >= RECORD_MAX_SPLITS) {
        return;
    }

    /* Store current split */
    track->splits.current_splits[checkpoint_id] = split_time;

    /* Check for new split record */
    if (track->splits.best_splits[checkpoint_id] == 0 ||
        split_time < track->splits.best_splits[checkpoint_id]) {
        record_save_split(track_id, checkpoint_id, split_time);
    }

    gRecordState.last_checkpoint = checkpoint_id;
    gRecordState.last_split_time = split_time;

    /* Update num_splits if needed */
    if (checkpoint_id >= track->splits.num_splits) {
        track->splits.num_splits = checkpoint_id + 1;
    }
}

/**
 * record_get_split_delta - Get difference from best split
 */
u32 record_get_split_delta(u8 checkpoint_id) {
    TrackRecord *track;
    u8 track_id;
    u32 best, current;

    track_id = gRecordState.current_track;
    if (track_id >= RECORD_MAX_TRACKS || checkpoint_id >= RECORD_MAX_SPLITS) {
        return 0;
    }

    track = &gRecordState.tracks[track_id];
    best = track->splits.best_splits[checkpoint_id];
    current = track->splits.current_splits[checkpoint_id];

    if (best == 0 || current == 0) {
        return 0;
    }

    /* Return signed delta (negative = faster than record) */
    return current - best;
}

/* ========================================================================
 * RECORD QUERIES
 * ======================================================================== */

/**
 * record_get_best_lap - Get best lap time for a track
 */
u32 record_get_best_lap(u8 track_id) {
    if (track_id >= RECORD_MAX_TRACKS) {
        return 0;
    }

    return gRecordState.tracks[track_id].lap.best.time;
}

/**
 * record_get_best_race - Get best race time for a track
 */
u32 record_get_best_race(u8 track_id) {
    if (track_id >= RECORD_MAX_TRACKS) {
        return 0;
    }

    return gRecordState.tracks[track_id].best_race.time;
}

/**
 * record_get_best_split - Get best split time
 */
u32 record_get_best_split(u8 track_id, u8 split_id) {
    if (track_id >= RECORD_MAX_TRACKS || split_id >= RECORD_MAX_SPLITS) {
        return 0;
    }

    return gRecordState.tracks[track_id].splits.best_splits[split_id];
}

/**
 * record_is_new_lap_record - Check if lap time is a new record
 * Based on arcade: HiScoreRank() pattern
 */
s32 record_is_new_lap_record(u8 track_id, u32 lap_time) {
    TrackRecord *track;

    if (track_id >= RECORD_MAX_TRACKS) {
        return 0;
    }

    /* Validate time */
    if (!record_validate_lap(lap_time)) {
        return 0;
    }

    track = &gRecordState.tracks[track_id];

    /* Check if better than current best */
    if (track->lap.best.time == 0 || lap_time < track->lap.best.time) {
        return 1;
    }

    return 0;
}

/**
 * record_is_new_race_record - Check if race time is a new record
 */
s32 record_is_new_race_record(u8 track_id, u32 race_time) {
    TrackRecord *track;

    if (track_id >= RECORD_MAX_TRACKS) {
        return 0;
    }

    /* Validate time */
    if (!record_validate_time(race_time)) {
        return 0;
    }

    track = &gRecordState.tracks[track_id];

    /* Check if better than current best */
    if (track->best_race.time == 0 || race_time < track->best_race.time) {
        return 1;
    }

    return 0;
}

/* ========================================================================
 * RECORD MANAGEMENT
 * ======================================================================== */

/**
 * record_save_lap - Save a new lap record
 * Based on arcade: SaveHighScore() pattern
 */
void record_save_lap(u8 track_id, u32 lap_time, u8 lap_num) {
    TrackRecord *track;

    if (track_id >= RECORD_MAX_TRACKS) {
        return;
    }

    if (!record_validate_lap(lap_time)) {
        return;
    }

    track = &gRecordState.tracks[track_id];

    /* Update best lap record */
    track->lap.best.name[0] = gRecordState.player_name[0];
    track->lap.best.name[1] = gRecordState.player_name[1];
    track->lap.best.name[2] = gRecordState.player_name[2];
    track->lap.best.name[3] = '\0';
    track->lap.best.time = lap_time;
    track->lap.best.car_type = gRecordState.car_type;
    track->lap.best.flags = RECORD_FLAG_VALID | RECORD_FLAG_NEW;
    track->lap.best.track_id = track_id;
    track->lap.best.lap_num = lap_num;

    if (gRecordState.mirror_mode) {
        track->lap.best.flags |= RECORD_FLAG_MIRROR;
    }
}

/**
 * record_save_race - Save a new race record
 */
void record_save_race(u8 track_id, u32 race_time) {
    TrackRecord *track;

    if (track_id >= RECORD_MAX_TRACKS) {
        return;
    }

    if (!record_validate_time(race_time)) {
        return;
    }

    track = &gRecordState.tracks[track_id];

    /* Update best race record */
    track->best_race.name[0] = gRecordState.player_name[0];
    track->best_race.name[1] = gRecordState.player_name[1];
    track->best_race.name[2] = gRecordState.player_name[2];
    track->best_race.name[3] = '\0';
    track->best_race.time = race_time;
    track->best_race.car_type = gRecordState.car_type;
    track->best_race.flags = RECORD_FLAG_VALID | RECORD_FLAG_NEW;
    track->best_race.track_id = track_id;
    track->best_race.lap_num = 0;

    if (gRecordState.mirror_mode) {
        track->best_race.flags |= RECORD_FLAG_MIRROR;
    }
}

/**
 * record_save_split - Save a new split record
 */
void record_save_split(u8 track_id, u8 split_id, u32 split_time) {
    TrackRecord *track;

    if (track_id >= RECORD_MAX_TRACKS || split_id >= RECORD_MAX_SPLITS) {
        return;
    }

    track = &gRecordState.tracks[track_id];
    track->splits.best_splits[split_id] = split_time;
    track->splits.flags |= RECORD_FLAG_NEW;
}

/* ========================================================================
 * DISPLAY
 * ======================================================================== */

/**
 * record_draw_lap_time - Draw a lap time on screen
 * Note: N64 version is a stub - actual rendering uses RDP
 */
void record_draw_lap_time(s16 x, s16 y, u32 time) {
    /* Stub - actual implementation would use N64 graphics */
    (void)x;
    (void)y;
    (void)time;
}

/**
 * record_draw_lap_delta - Draw lap time delta (+/- from record)
 */
void record_draw_lap_delta(s16 x, s16 y, s32 delta) {
    /* Stub - actual implementation would use N64 graphics */
    (void)x;
    (void)y;
    (void)delta;
}

/**
 * record_draw_split_delta - Draw split time delta
 */
void record_draw_split_delta(s16 x, s16 y, s32 delta) {
    /* Stub - actual implementation would use N64 graphics */
    (void)x;
    (void)y;
    (void)delta;
}

/* ========================================================================
 * TIME UTILITIES
 * ======================================================================== */

/**
 * record_time_to_string - Convert time to string
 * Based on arcade: cvt_time_str()
 *
 * @param time_ms   Time in milliseconds
 * @param buffer    Output buffer (at least 10 bytes)
 * @param format    's' = MM:SS, 'h' = M:SS.hh, 'c' = SSS, 'f' = MM:SS.hh
 */
void record_time_to_string(u32 time_ms, char *buffer, char format) {
    u32 sec, min, hundredths;

    switch (format) {
        case 'c':
            /* Countdown format: SSS */
            sec = time_ms / 1000;
            if (sec > 999) {
                sec = 999;
            }
            buffer[0] = (char)('0' + (sec / 100) % 10);
            buffer[1] = (char)('0' + (sec / 10) % 10);
            buffer[2] = (char)('0' + sec % 10);
            buffer[3] = '\0';
            break;

        case 'f':
            /* Full format: MM:SS.hh */
            if (time_ms >= RECORD_MAX_RACE_TIME) {
                buffer[0] = '9'; buffer[1] = '9';
                buffer[2] = ':';
                buffer[3] = '5'; buffer[4] = '9';
                buffer[5] = '.';
                buffer[6] = '9'; buffer[7] = '9';
                buffer[8] = '\0';
                return;
            }
            hundredths = (time_ms % 1000) / 10;
            sec = (time_ms / 1000) % 60;
            min = (time_ms / 1000) / 60;
            buffer[0] = (char)('0' + (min / 10) % 10);
            buffer[1] = (char)('0' + min % 10);
            buffer[2] = ':';
            buffer[3] = (char)('0' + sec / 10);
            buffer[4] = (char)('0' + sec % 10);
            buffer[5] = '.';
            buffer[6] = (char)('0' + hundredths / 10);
            buffer[7] = (char)('0' + hundredths % 10);
            buffer[8] = '\0';
            break;

        case 's':
            /* Short format: M:SS */
            sec = (time_ms / 1000) % 60;
            min = (time_ms / 1000) / 60;
            buffer[0] = (char)('0' + min % 10);
            buffer[1] = ':';
            buffer[2] = (char)('0' + sec / 10);
            buffer[3] = (char)('0' + sec % 10);
            buffer[4] = '\0';
            break;

        case 'h':
        default:
            /* Hundredths format: M:SS.hh */
            if (time_ms >= RECORD_MAX_LAP_TIME) {
                buffer[0] = '9';
                buffer[1] = ':';
                buffer[2] = '9'; buffer[3] = '9';
                buffer[4] = '.';
                buffer[5] = '9'; buffer[6] = '9';
                buffer[7] = '\0';
                return;
            }
            hundredths = (time_ms % 1000) / 10;
            sec = (time_ms / 1000) % 60;
            min = (time_ms / 1000) / 60;
            buffer[0] = (char)('0' + min % 10);
            buffer[1] = ':';
            buffer[2] = (char)('0' + sec / 10);
            buffer[3] = (char)('0' + sec % 10);
            buffer[4] = '.';
            buffer[5] = (char)('0' + hundredths / 10);
            buffer[6] = (char)('0' + hundredths % 10);
            buffer[7] = '\0';
            break;
    }
}

/**
 * record_delta_to_string - Convert time delta to string
 * Output: "+0:01.23" or "-0:01.23"
 */
void record_delta_to_string(s32 delta_ms, char *buffer) {
    u32 abs_delta;
    u32 sec, hundredths;
    char sign;

    if (delta_ms < 0) {
        sign = '-';
        abs_delta = (u32)(-delta_ms);
    } else {
        sign = '+';
        abs_delta = (u32)delta_ms;
    }

    hundredths = (abs_delta % 1000) / 10;
    sec = abs_delta / 1000;

    if (sec > 59) {
        sec = 59;
        hundredths = 99;
    }

    buffer[0] = sign;
    buffer[1] = (char)('0' + sec / 10);
    buffer[2] = (char)('0' + sec % 10);
    buffer[3] = '.';
    buffer[4] = (char)('0' + hundredths / 10);
    buffer[5] = (char)('0' + hundredths % 10);
    buffer[6] = '\0';
}

/**
 * record_frames_to_ms - Convert frames (60fps) to milliseconds
 */
u32 record_frames_to_ms(u32 frames) {
    return (frames * 1000) / 60;
}

/**
 * record_ms_to_frames - Convert milliseconds to frames (60fps)
 */
u32 record_ms_to_frames(u32 ms) {
    return (ms * 60) / 1000;
}

/* ========================================================================
 * PERSISTENCE
 * ======================================================================== */

/**
 * record_save - Save all records to Controller Pak
 */
s32 record_save(void) {
    /* TODO: Implement Controller Pak save */
    /* For now, return success */
    return 1;
}

/**
 * record_load - Load all records from Controller Pak
 */
s32 record_load(void) {
    /* TODO: Implement Controller Pak load */
    /* For now, use defaults */
    record_init();
    return 1;
}

/**
 * record_save_track - Save single track records
 */
s32 record_save_track(u8 track_id) {
    /* TODO: Implement Controller Pak save */
    (void)track_id;
    return 1;
}

/**
 * record_load_track - Load single track records
 */
s32 record_load_track(u8 track_id) {
    /* TODO: Implement Controller Pak load */
    (void)track_id;
    return 1;
}

/**
 * record_clear_all - Clear all records
 * Based on arcade: ClearHighScores()
 */
void record_clear_all(void) {
    s32 i;

    for (i = 0; i < RECORD_MAX_TRACKS; i++) {
        record_init_track((u8)i);
    }
}

/**
 * record_clear_track - Clear records for a single track
 */
void record_clear_track(u8 track_id) {
    if (track_id < RECORD_MAX_TRACKS) {
        record_init_track(track_id);
    }
}

/* ========================================================================
 * VALIDATION
 * ======================================================================== */

/**
 * record_validate_time - Validate a race time
 */
s32 record_validate_time(u32 time) {
    /* Check bounds */
    if (time == 0) {
        return 0;
    }
    if (time >= RECORD_MAX_RACE_TIME) {
        return 0;
    }

    return 1;
}

/**
 * record_validate_lap - Validate a lap time
 */
s32 record_validate_lap(u32 lap_time) {
    /* Check bounds */
    if (lap_time < RECORD_MIN_LAP_TIME) {
        return 0;  /* Too fast - probably cheating */
    }
    if (lap_time >= RECORD_MAX_LAP_TIME) {
        return 0;
    }

    return 1;
}

#endif /* NON_MATCHING */
