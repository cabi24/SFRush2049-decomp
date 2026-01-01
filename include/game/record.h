/**
 * record.h - Track and Lap Record System for Rush 2049 N64
 *
 * Handles best lap times, track records, and split times.
 * Works alongside hiscore.c for complete time tracking.
 *
 * Based on arcade checkpoint.c and hiscore.c patterns.
 */

#ifndef RECORD_H
#define RECORD_H

#include "types.h"

/* ========================================================================
 * ARCADE-COMPATIBLE CONSTANTS
 * ======================================================================== */

/* Time constants (in milliseconds for arcade compatibility) */
#define RECORD_ONE_SEC          1000            /* 1 second in ms */
#define RECORD_MIN_LAP_TIME     (5 * RECORD_ONE_SEC)    /* 5 second minimum lap */
#define RECORD_MAX_LAP_TIME     (10 * 60 * RECORD_ONE_SEC)  /* 10 minute max lap */
#define RECORD_MAX_RACE_TIME    (100 * 60 * RECORD_ONE_SEC) /* 99:59.99 max race */

/* Track limits */
#define RECORD_MAX_TRACKS       8               /* Maximum tracks */
#define RECORD_MAX_LAPS         20              /* Maximum laps per race */
#define RECORD_MAX_CHECKPOINTS  20              /* Maximum checkpoints per track */
#define RECORD_MAX_SPLITS       RECORD_MAX_CHECKPOINTS

/* Record categories */
#define RECORD_CAT_LAP          0               /* Best lap time */
#define RECORD_CAT_RACE         1               /* Best race time */
#define RECORD_CAT_SPLIT        2               /* Best split time */
#define RECORD_CAT_COUNT        3

/* Record flags */
#define RECORD_FLAG_VALID       0x01            /* Record is valid */
#define RECORD_FLAG_MIRROR      0x02            /* Set in mirror mode */
#define RECORD_FLAG_SAVED       0x04            /* Saved to Controller Pak */
#define RECORD_FLAG_NEW         0x08            /* New record this session */

/* Display constants */
#define RECORD_NAME_LENGTH      4               /* 3 chars + null */

/* ========================================================================
 * STRUCTURES
 * ======================================================================== */

/**
 * RecordEntry - Single record entry (lap or race time)
 */
typedef struct RecordEntry {
    char    name[RECORD_NAME_LENGTH];   /* Player initials */
    u32     time;                       /* Time in milliseconds */
    u8      car_type;                   /* Car used */
    u8      flags;                      /* RECORD_FLAG_* */
    u8      track_id;                   /* Track this record is for */
    u8      lap_num;                    /* Lap number (for lap records) */
} RecordEntry;

/**
 * LapRecord - Best lap time for a track
 */
typedef struct LapRecord {
    RecordEntry     best;               /* Best lap ever */
    RecordEntry     current;            /* Best lap this race */
    u32             lap_times[RECORD_MAX_LAPS]; /* All lap times this race */
    u8              num_laps;           /* Number of laps recorded */
    u8              best_lap_num;       /* Which lap was fastest */
    u8              pad[2];
} LapRecord;

/**
 * SplitRecord - Best split times (checkpoint to checkpoint)
 */
typedef struct SplitRecord {
    u32     best_splits[RECORD_MAX_SPLITS]; /* Best split times ever */
    u32     current_splits[RECORD_MAX_SPLITS]; /* Current race splits */
    u8      num_splits;                 /* Number of splits for track */
    u8      flags;
    u8      pad[2];
} SplitRecord;

/**
 * TrackRecord - All records for a single track
 */
typedef struct TrackRecord {
    RecordEntry     best_race;          /* Best total race time */
    LapRecord       lap;                /* Lap time records */
    SplitRecord     splits;             /* Split time records */
    u8              track_id;
    u8              mirror_mode;        /* 0 = normal, 1 = mirror */
    u8              valid;              /* Track records are valid */
    u8              pad;
} TrackRecord;

/**
 * RecordState - Complete record system state
 */
typedef struct RecordState {
    TrackRecord     tracks[RECORD_MAX_TRACKS];  /* Per-track records */

    /* Current race state */
    u8              current_track;      /* Track being raced */
    u8              current_lap;        /* Current lap (0-based) */
    u8              racing;             /* Currently in a race */
    u8              mirror_mode;        /* Mirror mode active */

    /* Timing */
    u32             race_start_time;    /* When race started */
    u32             lap_start_time;     /* When current lap started */
    u32             last_split_time;    /* Time at last checkpoint */
    u8              last_checkpoint;    /* Last checkpoint crossed */
    u8              pad[3];

    /* Player info */
    char            player_name[RECORD_NAME_LENGTH];
    u8              car_type;
    u8              pad2[3];
} RecordState;

/* ========================================================================
 * GLOBAL STATE
 * ======================================================================== */

extern RecordState gRecordState;

/* ========================================================================
 * ARCADE-COMPATIBLE FUNCTIONS
 * Based on arcade checkpoint.c and hiscore.c patterns
 * ======================================================================== */

/* Initialization */
void record_init(void);
void record_reset(void);
void record_init_track(u8 track_id);

/* Race lifecycle */
void record_start_race(u8 track_id, u8 car_type, u8 mirror_mode);
void record_end_race(u32 final_time);
void record_abort_race(void);

/* Lap tracking */
void record_start_lap(void);
void record_end_lap(u32 lap_time);
u32 record_get_current_lap_time(void);

/* Checkpoint/split tracking */
void record_checkpoint(u8 checkpoint_id, u32 split_time);
u32 record_get_split_delta(u8 checkpoint_id);

/* Record queries */
u32 record_get_best_lap(u8 track_id);
u32 record_get_best_race(u8 track_id);
u32 record_get_best_split(u8 track_id, u8 split_id);
s32 record_is_new_lap_record(u8 track_id, u32 lap_time);
s32 record_is_new_race_record(u8 track_id, u32 race_time);

/* Record management */
void record_save_lap(u8 track_id, u32 lap_time, u8 lap_num);
void record_save_race(u8 track_id, u32 race_time);
void record_save_split(u8 track_id, u8 split_id, u32 split_time);

/* Display */
void record_draw_lap_time(s16 x, s16 y, u32 time);
void record_draw_lap_delta(s16 x, s16 y, s32 delta);
void record_draw_split_delta(s16 x, s16 y, s32 delta);

/* Time utilities (arcade compatible) */
void record_time_to_string(u32 time_ms, char *buffer, char format);
void record_delta_to_string(s32 delta_ms, char *buffer);
u32 record_frames_to_ms(u32 frames);
u32 record_ms_to_frames(u32 ms);

/* Persistence */
s32 record_save(void);
s32 record_load(void);
s32 record_save_track(u8 track_id);
s32 record_load_track(u8 track_id);
void record_clear_all(void);
void record_clear_track(u8 track_id);

/* Validation */
s32 record_validate_time(u32 time);
s32 record_validate_lap(u32 lap_time);

/* ========================================================================
 * ARCADE-COMPATIBLE HELPER MACROS
 * ======================================================================== */

/* Convert between time units */
#define RECORD_FRAMES_TO_MS(f)  (((f) * 1000) / 60)
#define RECORD_MS_TO_FRAMES(ms) (((ms) * 60) / 1000)

/* Time comparison helpers */
#define RECORD_IS_BETTER(new_time, old_time) \
    ((new_time) > 0 && ((old_time) == 0 || (new_time) < (old_time)))

/* Format helpers */
#define RECORD_TIME_MINUTES(ms)     (((ms) / 1000) / 60)
#define RECORD_TIME_SECONDS(ms)     (((ms) / 1000) % 60)
#define RECORD_TIME_HUNDREDTHS(ms)  (((ms) % 1000) / 10)

#endif /* RECORD_H */
