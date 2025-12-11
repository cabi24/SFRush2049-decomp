/**
 * timer.c - Game timer and timing utilities
 *
 * Based on arcade game timing concepts from game/game.c
 * Handles race timers, countdown, and time-based game logic
 */

#include "types.h"
#include "game/structs.h"

/* External OS timing functions */
extern u64 osGetTime(void);
extern u32 osGetCount(void);

/* External game state */
extern GameStruct game_struct;
extern u32 frame_counter;

/* Timer state - addresses TBD from analysis */
static u32 timer_start_count;
static f32 timer_elapsed;
static u32 lap_start_times[MAX_CARS];

/* Constants */
#define TIMER_HZ        60      /* Timer ticks per second (N64 frame rate) */
#define COUNT_PER_SEC   46875000  /* N64 CPU counter frequency */

/**
 * timer_init - Initialize game timers
 *
 * Called at race start to reset all timing state.
 */
void timer_init(void) {
    s32 i;

    timer_start_count = osGetCount();
    timer_elapsed = 0.0f;

    for (i = 0; i < MAX_CARS; i++) {
        lap_start_times[i] = timer_start_count;
    }
}

/**
 * timer_update - Update elapsed time
 *
 * Called each frame to update the game timer.
 * Uses high-resolution CPU counter for accuracy.
 */
void timer_update(void) {
    u32 current_count = osGetCount();
    u32 elapsed_counts = current_count - timer_start_count;

    /* Convert to seconds (float) */
    timer_elapsed = (f32)elapsed_counts / (f32)COUNT_PER_SEC;
}

/**
 * timer_get_elapsed - Get elapsed race time
 *
 * @return Elapsed time in seconds
 */
f32 timer_get_elapsed(void) {
    return timer_elapsed;
}

/**
 * timer_get_elapsed_ms - Get elapsed race time in milliseconds
 *
 * @return Elapsed time in milliseconds
 */
u32 timer_get_elapsed_ms(void) {
    return (u32)(timer_elapsed * 1000.0f);
}

/**
 * timer_lap_start - Record lap start time for a car
 *
 * @param car_index Index of car (0-7)
 */
void timer_lap_start(s32 car_index) {
    if (car_index >= 0 && car_index < MAX_CARS) {
        lap_start_times[car_index] = osGetCount();
    }
}

/**
 * timer_get_lap_time - Get current lap time for a car
 *
 * @param car_index Index of car (0-7)
 * @return Lap time in seconds
 */
f32 timer_get_lap_time(s32 car_index) {
    if (car_index >= 0 && car_index < MAX_CARS) {
        u32 current = osGetCount();
        u32 elapsed = current - lap_start_times[car_index];
        return (f32)elapsed / (f32)COUNT_PER_SEC;
    }
    return 0.0f;
}

/**
 * timer_format_time - Format time as MM:SS.mmm string
 * Based on arcade display formatting
 *
 * @param seconds Time in seconds
 * @param buffer Output buffer (at least 12 chars)
 */
void timer_format_time(f32 seconds, char *buffer) {
    s32 minutes = (s32)(seconds / 60.0f);
    s32 secs = (s32)seconds % 60;
    s32 ms = (s32)((seconds - (f32)(minutes * 60 + secs)) * 1000.0f);

    /* Simple formatting without sprintf */
    buffer[0] = '0' + (minutes / 10);
    buffer[1] = '0' + (minutes % 10);
    buffer[2] = ':';
    buffer[3] = '0' + (secs / 10);
    buffer[4] = '0' + (secs % 10);
    buffer[5] = '.';
    buffer[6] = '0' + (ms / 100);
    buffer[7] = '0' + ((ms / 10) % 10);
    buffer[8] = '0' + (ms % 10);
    buffer[9] = '\0';
}

/******* COUNTDOWN TIMER *******/

/* Countdown state */
static s32 countdown_value;      /* Current countdown number (3,2,1,GO) */
static u32 countdown_start;      /* Countdown start count */

#define COUNTDOWN_DURATION  (TIMER_HZ)  /* 1 second per number */

/**
 * countdown_init - Initialize race countdown
 *
 * @param start_value Starting number (usually 3)
 */
void countdown_init(s32 start_value) {
    countdown_value = start_value;
    countdown_start = osGetCount();
}

/**
 * countdown_update - Update countdown timer
 *
 * @return Current countdown value (3,2,1,0=GO, -1=done)
 */
s32 countdown_update(void) {
    if (countdown_value < 0) {
        return -1;  /* Already finished */
    }

    u32 current = osGetCount();
    u32 elapsed = current - countdown_start;
    s32 elapsed_frames = (s32)(elapsed / (COUNT_PER_SEC / TIMER_HZ));

    /* Each number lasts 1 second (60 frames) */
    s32 new_value = 3 - (elapsed_frames / TIMER_HZ);

    if (new_value < -1) {
        new_value = -1;
    }

    /* Detect transition */
    if (new_value != countdown_value) {
        countdown_value = new_value;
    }

    return countdown_value;
}

/**
 * countdown_get_value - Get current countdown value
 *
 * @return Current value (3,2,1,0,-1)
 */
s32 countdown_get_value(void) {
    return countdown_value;
}

/******* FRAME TIMING *******/

/* Delta time tracking */
static u32 last_frame_count;
static f32 delta_time;

/**
 * frame_time_init - Initialize frame timing
 */
void frame_time_init(void) {
    last_frame_count = osGetCount();
    delta_time = 1.0f / 60.0f;  /* Default to 60 FPS */
}

/**
 * frame_time_update - Update frame delta time
 *
 * Called at start of each frame to calculate delta time.
 */
void frame_time_update(void) {
    u32 current = osGetCount();
    u32 elapsed = current - last_frame_count;
    last_frame_count = current;

    /* Convert to seconds */
    delta_time = (f32)elapsed / (f32)COUNT_PER_SEC;

    /* Clamp to reasonable range (1/120 to 1/10 second) */
    if (delta_time < 0.008f) {
        delta_time = 0.008f;
    } else if (delta_time > 0.1f) {
        delta_time = 0.1f;
    }
}

/**
 * frame_time_get_delta - Get delta time for current frame
 *
 * @return Time since last frame in seconds
 */
f32 frame_time_get_delta(void) {
    return delta_time;
}

/**
 * frame_time_get_fps - Get estimated frames per second
 *
 * @return Current FPS estimate
 */
f32 frame_time_get_fps(void) {
    if (delta_time > 0.001f) {
        return 1.0f / delta_time;
    }
    return 60.0f;
}
