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

#endif /* TIMER_H */
