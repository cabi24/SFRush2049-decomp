/**
 * timer.h - Game timer and timing utilities
 */

#ifndef TIMER_H
#define TIMER_H

#include "types.h"

/* Race timer functions */
void timer_init(void);
void timer_update(void);
f32  timer_get_elapsed(void);
u32  timer_get_elapsed_ms(void);

/* Lap timing */
void timer_lap_start(s32 car_index);
f32  timer_get_lap_time(s32 car_index);

/* Time formatting */
void timer_format_time(f32 seconds, char *buffer);

/* Countdown timer */
void countdown_init(s32 start_value);
s32  countdown_update(void);
s32  countdown_get_value(void);

/* Frame timing / delta time */
void frame_time_init(void);
void frame_time_update(void);
f32  frame_time_get_delta(void);
f32  frame_time_get_fps(void);

/******* ARCADE-COMPATIBLE TIMING FUNCTIONS *******/
/* Based on arcade game/sselect.c timing system */
/* Uses millisecond time base (ONE_SEC = 1000) */

#define ONE_SEC     1000    /* Milliseconds per second (arcade constant) */

/* Timer constants from arcade (used in game logic) */
#define TRK_SELECT_TIME     (20 * ONE_SEC)  /* Track select timeout */
#define CAR_SELECT_TIME     (30 * ONE_SEC)  /* Car select timeout */
#define QCK_SELECT_TIME     (10 * ONE_SEC)  /* Quick select timeout */
#define CLUTCH_SEL_TIME     (5 * ONE_SEC)   /* Clutch select timeout */
#define GAMEOVER_TIME       (4 * ONE_SEC)   /* Game over screen time */
#define TOURNEYOVER_TIME    (30 * ONE_SEC)  /* Tournament over time */
#define QUICKOVER_TIME      (10 * ONE_SEC)  /* Quick game over time */

/**
 * GetElapsedTime - Get elapsed time in milliseconds
 * Arcade: game/sselect.c
 */
u32 GetElapsedTime(void);

/**
 * GetCountdownTime - Get remaining countdown time in milliseconds
 * Arcade: game/sselect.c
 */
s32 GetCountdownTime(void);

/**
 * SetCountdownTimer - Set countdown timer in milliseconds
 * Arcade: game/sselect.c
 */
void SetCountdownTimer(s32 msec);

/**
 * SetCountdownTimerAt0 - Set countdown preserving elapsed offset
 * Arcade: game/select.c
 */
void SetCountdownTimerAt0(s32 msec);

/**
 * TimeOut - Check if countdown timer has expired
 * Arcade: game/sselect.c
 */
s32 TimeOut(void);

/**
 * arcade_timer_init - Initialize arcade-style timer system
 */
void arcade_timer_init(void);

#endif /* TIMER_H */
