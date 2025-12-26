/**
 * hud.c - Heads-Up Display for Rush 2049 N64
 *
 * Based on arcade game/hud.c
 * Displays speedometer, laps, position, timer, and radar during races.
 */

#include "types.h"
#include "game/hud.h"
#include "game/structs.h"

/* External game state */
extern u8 gstate;
extern u32 frame_counter;
extern CarData car_array[];
extern s32 num_active_cars;
extern s32 this_car;

/* External functions */
extern f32 sqrtf(f32 x);

/* HUD state */
HudState gHud;
CarMarker hud_markers[MAX_CARS];

/* Frame rate constant */
#define FRAMES_PER_SEC  60

/**
 * hud_init - Initialize HUD system
 */
void hud_init(void) {
    s32 i;

    gHud.flags = HUD_SHOW_ALL;
    gHud.speed_display = 0;
    gHud.target_speed = 0;
    gHud.current_lap = 1;
    gHud.total_laps = 3;
    gHud.position = 1;
    gHud.total_cars = 8;
    gHud.elapsed_time = 0;
    gHud.checkpoint_time = 0;
    gHud.checkpoint_flash = 0;
    gHud.wrong_way = 0;
    gHud.markers_init = 0;

    /* Clear markers */
    for (i = 0; i < MAX_CARS; i++) {
        hud_markers[i].x = 0.0f;
        hud_markers[i].y = 0.0f;
        hud_markers[i].color = 0;
        hud_markers[i].visible = 0;
        hud_markers[i].is_player = 0;
        hud_markers[i].is_flashing = 0;
    }
}

/**
 * hud_reset - Reset HUD for new race
 */
void hud_reset(void) {
    gHud.speed_display = 0;
    gHud.target_speed = 0;
    gHud.current_lap = 1;
    gHud.elapsed_time = 0;
    gHud.checkpoint_time = 0;
    gHud.checkpoint_flash = 0;
    gHud.wrong_way = 0;
}

/**
 * hud_update - Update HUD state each frame
 */
void hud_update(void) {
    CarData *car;

    car = &car_array[this_car];

    /* Update speed display (smooth toward target) */
    gHud.target_speed = (s16)(car->speed * 0.6818f);  /* fps to mph approx */
    if (gHud.speed_display < gHud.target_speed) {
        gHud.speed_display += 2;
        if (gHud.speed_display > gHud.target_speed) {
            gHud.speed_display = gHud.target_speed;
        }
    } else if (gHud.speed_display > gHud.target_speed) {
        gHud.speed_display -= 2;
        if (gHud.speed_display < gHud.target_speed) {
            gHud.speed_display = gHud.target_speed;
        }
    }

    /* Update lap count */
    gHud.current_lap = car->laps + 1;

    /* Update position */
    gHud.position = car->place + 1;

    /* Update timer */
    gHud.elapsed_time++;

    /* Decrement checkpoint flash timer */
    if (gHud.checkpoint_flash > 0) {
        gHud.checkpoint_flash--;
    }

    /* Update radar markers */
    hud_update_markers();
}

/**
 * hud_draw - Draw all HUD elements
 */
void hud_draw(void) {
    if (gHud.flags & HUD_SHOW_SPEED) {
        hud_draw_speed();
    }
    if (gHud.flags & HUD_SHOW_LAPS) {
        hud_draw_laps();
    }
    if (gHud.flags & HUD_SHOW_POSITION) {
        hud_draw_position();
    }
    if (gHud.flags & HUD_SHOW_TIMER) {
        hud_draw_timer();
    }
    if (gHud.flags & HUD_SHOW_RADAR) {
        hud_draw_radar();
    }
    if (gHud.flags & HUD_SHOW_WRONG_WAY && gHud.wrong_way) {
        hud_draw_wrong_way();
    }
    if (gHud.flags & HUD_SHOW_CHECKTIME && gHud.checkpoint_flash > 0) {
        hud_draw_checkpoint_bonus();
    }
    if (gHud.flags & HUD_SHOW_MARKERS) {
        hud_draw_markers();
    }
}

/**
 * hud_draw_speed - Draw speedometer
 */
void hud_draw_speed(void) {
    /* Would use N64 graphics to draw speed number */
    /* In arcade, uses blit functions for number sprites */
}

/**
 * hud_draw_laps - Draw lap counter
 */
void hud_draw_laps(void) {
    /* Draw "LAP X/Y" text */
}

/**
 * hud_draw_position - Draw race position
 */
void hud_draw_position(void) {
    /* Draw position with suffix (1st, 2nd, etc) */
}

/**
 * hud_draw_timer - Draw race timer
 */
void hud_draw_timer(void) {
    /* Draw elapsed time in MM:SS.SS format */
}

/**
 * hud_draw_radar - Draw mini-map/radar
 */
void hud_draw_radar(void) {
    /* Draw track outline and car positions */
}

/**
 * hud_draw_wrong_way - Draw wrong way warning
 */
void hud_draw_wrong_way(void) {
    /* Flash "WRONG WAY" text on screen */
}

/**
 * hud_draw_checkpoint_bonus - Draw checkpoint time bonus
 */
void hud_draw_checkpoint_bonus(void) {
    /* Show "+XX.XX" time bonus when hitting checkpoint */
}

/**
 * hud_update_markers - Update car markers on radar
 * Based on arcade: hud.c marker code
 */
void hud_update_markers(void) {
    s32 i;
    f32 x, y;

    for (i = 0; i < num_active_cars; i++) {
        hud_calc_marker_pos(i, &x, &y);

        hud_markers[i].x = x;
        hud_markers[i].y = y;
        hud_markers[i].visible = 1;
        hud_markers[i].is_player = (i == this_car);
        hud_markers[i].is_flashing = 0;  /* Would check resurrection state */

        /* Set color based on player/drone */
        if (i == this_car) {
            hud_markers[i].color = 1;  /* Player color */
        } else {
            hud_markers[i].color = 2;  /* Other cars */
        }
    }

    gHud.markers_init = 1;
}

/**
 * hud_draw_markers - Draw car markers on radar
 */
void hud_draw_markers(void) {
    s32 i;

    if (!gHud.markers_init) {
        return;
    }

    for (i = 0; i < num_active_cars; i++) {
        if (!hud_markers[i].visible) {
            continue;
        }

        /* Skip flashing markers on odd frames */
        if (hud_markers[i].is_flashing && (frame_counter & 1)) {
            continue;
        }

        /* Draw marker dot at (x, y) with appropriate color */
        /* Would use N64 graphics primitives */
    }
}

/**
 * hud_calc_marker_pos - Calculate screen position for car marker
 *
 * @param car_index Car to get position for
 * @param out_x Output X coordinate
 * @param out_y Output Y coordinate
 */
void hud_calc_marker_pos(s32 car_index, f32 *out_x, f32 *out_y) {
    CarData *car;
    f32 world_x, world_z;
    f32 rel_x, rel_z;
    f32 scale;

    car = &car_array[car_index];

    /* Get world position */
    world_x = car->dr_pos[0];
    world_z = car->dr_pos[2];

    /* Convert to radar coordinates */
    /* This is simplified - arcade uses track-specific scaling */
    scale = 0.1f;  /* Scale factor */

    rel_x = world_x * scale;
    rel_z = world_z * scale;

    /* Clamp to radar bounds */
    if (rel_x < -HUD_RADAR_SIZE/2) rel_x = -HUD_RADAR_SIZE/2;
    if (rel_x > HUD_RADAR_SIZE/2) rel_x = HUD_RADAR_SIZE/2;
    if (rel_z < -HUD_RADAR_SIZE/2) rel_z = -HUD_RADAR_SIZE/2;
    if (rel_z > HUD_RADAR_SIZE/2) rel_z = HUD_RADAR_SIZE/2;

    /* Convert to screen coords */
    *out_x = HUD_RADAR_X + HUD_RADAR_SIZE/2 + rel_x;
    *out_y = HUD_RADAR_Y + HUD_RADAR_SIZE/2 - rel_z;  /* Z is flipped */
}

/**
 * hud_show - Show HUD elements
 *
 * @param flags Elements to show
 */
void hud_show(u16 flags) {
    gHud.flags |= flags;
}

/**
 * hud_hide - Hide HUD elements
 *
 * @param flags Elements to hide
 */
void hud_hide(u16 flags) {
    gHud.flags &= ~flags;
}

/**
 * hud_toggle - Toggle HUD elements
 *
 * @param flags Elements to toggle
 */
void hud_toggle(u16 flags) {
    gHud.flags ^= flags;
}

/**
 * hud_checkpoint_hit - Handle checkpoint crossing
 *
 * @param time_bonus Time added in frames
 */
void hud_checkpoint_hit(u32 time_bonus) {
    gHud.checkpoint_time = time_bonus;
    gHud.checkpoint_flash = FRAMES_PER_SEC * 2;  /* Flash for 2 seconds */
}

/**
 * hud_lap_complete - Handle lap completion
 *
 * @param lap_num Completed lap number
 */
void hud_lap_complete(s32 lap_num) {
    gHud.current_lap = lap_num + 1;
}

/**
 * hud_set_wrong_way - Set wrong way indicator
 *
 * @param wrong Non-zero if going wrong way
 */
void hud_set_wrong_way(s32 wrong) {
    gHud.wrong_way = wrong ? 1 : 0;
}

/**
 * hud_format_time - Format frame count as time string
 *
 * @param frames Time in frames
 * @param buffer Output buffer (at least 10 chars)
 */
void hud_format_time(u32 frames, char *buffer) {
    u32 total_secs;
    u32 mins, secs, hundredths;

    total_secs = frames / FRAMES_PER_SEC;
    mins = total_secs / 60;
    secs = total_secs % 60;
    hundredths = (frames % FRAMES_PER_SEC) * 100 / FRAMES_PER_SEC;

    /* Format as MM:SS.HH */
    buffer[0] = '0' + (mins / 10);
    buffer[1] = '0' + (mins % 10);
    buffer[2] = ':';
    buffer[3] = '0' + (secs / 10);
    buffer[4] = '0' + (secs % 10);
    buffer[5] = '.';
    buffer[6] = '0' + (hundredths / 10);
    buffer[7] = '0' + (hundredths % 10);
    buffer[8] = '\0';
}

/**
 * hud_format_speed - Format speed as string
 *
 * @param speed Speed in mph
 * @param buffer Output buffer (at least 5 chars)
 */
void hud_format_speed(s32 speed, char *buffer) {
    s32 hundreds, tens, ones;

    if (speed > 999) {
        speed = 999;
    }
    if (speed < 0) {
        speed = 0;
    }

    hundreds = speed / 100;
    tens = (speed / 10) % 10;
    ones = speed % 10;

    if (hundreds > 0) {
        buffer[0] = '0' + hundreds;
        buffer[1] = '0' + tens;
        buffer[2] = '0' + ones;
        buffer[3] = '\0';
    } else if (tens > 0) {
        buffer[0] = '0' + tens;
        buffer[1] = '0' + ones;
        buffer[2] = '\0';
    } else {
        buffer[0] = '0' + ones;
        buffer[1] = '\0';
    }
}

/******* ARCADE-COMPATIBLE TIME CONVERSION FUNCTIONS *******/

/**
 * cvt_time - Convert milliseconds to time format
 * Based on arcade: hud.c:cvt_time()
 *
 * @param t Time in milliseconds
 * @param dest Output array (at least 5 bytes)
 * @param format 's' = MM:SS, 'h' = MM:SS.hh, 'c' = SSS countdown
 * @return Number of digits set
 */
u8 cvt_time(s32 t, u8 *dest, char format) {
    s32 sec, min;
    u8 ret = 0;

    if (t < 0) {
        t = 0;
    }

    switch (format) {
        case 'c':
            /* Countdown timer (seconds only, max 999) */
            sec = t / 1000;  /* Convert ms to seconds */
            if (sec > 999) {
                sec = 999;
            }
            dest[0] = (u8)((sec / 100) % 10);
            dest[1] = (u8)((sec / 10) % 10);
            dest[2] = (u8)(sec % 10);
            ret = 3;
            break;

        case 'h':
            /* Hundredths display */
            dest[3] = (u8)((t / 100) % 10);
            dest[4] = (u8)((t / 10) % 10);
            ret = 5;
            /* Fall through to do seconds */

        case 's':
            /* Seconds and minutes only */
            sec = t / 1000;
            min = (sec / 60) % 60;
            sec %= 60;
            dest[0] = (u8)(min % 10);
            dest[1] = (u8)(sec / 10);
            dest[2] = (u8)(sec % 10);
            if (ret == 0) {
                ret = 3;
            }
            break;
    }

    return ret;
}

/**
 * cvt_time_frames - Convert frame count to time format
 * N64-specific version using 60fps timing
 *
 * @param frames Frame count
 * @param dest Output array (at least 5 bytes)
 * @param format 's' = MM:SS, 'h' = MM:SS.hh, 'c' = SSS countdown
 * @return Number of digits set
 */
u8 cvt_time_frames(u32 frames, u8 *dest, char format) {
    /* Convert frames to milliseconds (assuming 60fps) */
    u32 ms = (frames * 1000) / FRAMES_PER_SEC;
    return cvt_time((s32)ms, dest, format);
}

/**
 * hud_format_position - Format race position with suffix
 *
 * @param position Position (1 = first)
 * @param buffer Output buffer (at least 5 chars)
 */
void hud_format_position(s32 position, char *buffer) {
    if (position < 1) {
        position = 1;
    }
    if (position > 8) {
        position = 8;
    }

    buffer[0] = '0' + position;

    /* Add appropriate suffix */
    switch (position) {
        case 1:
            buffer[1] = 's';
            buffer[2] = 't';
            break;
        case 2:
            buffer[1] = 'n';
            buffer[2] = 'd';
            break;
        case 3:
            buffer[1] = 'r';
            buffer[2] = 'd';
            break;
        default:
            buffer[1] = 't';
            buffer[2] = 'h';
            break;
    }
    buffer[3] = '\0';
}

/**
 * hud_format_lap - Format lap counter
 *
 * @param current Current lap (1-based)
 * @param total Total laps
 * @param buffer Output buffer (at least 8 chars)
 */
void hud_format_lap(s32 current, s32 total, char *buffer) {
    buffer[0] = 'L';
    buffer[1] = 'A';
    buffer[2] = 'P';
    buffer[3] = ' ';
    buffer[4] = '0' + current;
    buffer[5] = '/';
    buffer[6] = '0' + total;
    buffer[7] = '\0';
}

/**
 * hud_get_speed_mph - Get current player speed in MPH
 *
 * @return Speed in MPH
 */
s32 hud_get_speed_mph(void) {
    return gHud.speed_display;
}

/**
 * hud_get_speed_kph - Get current player speed in KPH
 *
 * @return Speed in KPH
 */
s32 hud_get_speed_kph(void) {
    return (gHud.speed_display * 161) / 100;  /* 1.61 km per mile */
}

/**
 * hud_get_elapsed_frames - Get elapsed race time in frames
 *
 * @return Elapsed frames since race start
 */
u32 hud_get_elapsed_frames(void) {
    return gHud.elapsed_time;
}

/**
 * hud_get_elapsed_ms - Get elapsed race time in milliseconds
 *
 * @return Elapsed time in milliseconds
 */
u32 hud_get_elapsed_ms(void) {
    return (gHud.elapsed_time * 1000) / FRAMES_PER_SEC;
}

/**
 * hud_set_countdown_time - Set remaining countdown time
 *
 * @param ms Time remaining in milliseconds
 */
void hud_set_countdown_time(u32 ms) {
    gHud.countdown_time = ms;
}

/**
 * hud_get_countdown_time - Get remaining countdown time
 *
 * @return Time remaining in milliseconds
 */
u32 hud_get_countdown_time(void) {
    return gHud.countdown_time;
}

/**
 * hud_set_metric - Enable/disable metric units (KPH vs MPH)
 *
 * @param metric Non-zero for metric
 */
void hud_set_metric(s32 metric) {
    gHud.metric_mode = metric ? 1 : 0;
}

/**
 * hud_is_metric - Check if metric mode is enabled
 *
 * @return Non-zero if metric
 */
s32 hud_is_metric(void) {
    return gHud.metric_mode;
}

/******* RADAR MAP FUNCTIONS *******/

/* Radar mapping variables */
static f32 radar_min_x, radar_max_x;
static f32 radar_min_z, radar_max_z;
static f32 radar_scale_x, radar_scale_z;

/**
 * set_radar_mapping - Set radar coordinate mapping
 * Based on arcade: hud.c:set_radar_mapping()
 *
 * @param min_x World minimum X
 * @param max_x World maximum X
 * @param min_z World minimum Z
 * @param max_z World maximum Z
 */
void set_radar_mapping(f32 min_x, f32 max_x, f32 min_z, f32 max_z) {
    radar_min_x = min_x;
    radar_max_x = max_x;
    radar_min_z = min_z;
    radar_max_z = max_z;

    /* Calculate scale factors */
    if (max_x != min_x) {
        radar_scale_x = (f32)HUD_RADAR_SIZE / (max_x - min_x);
    } else {
        radar_scale_x = 1.0f;
    }

    if (max_z != min_z) {
        radar_scale_z = (f32)HUD_RADAR_SIZE / (max_z - min_z);
    } else {
        radar_scale_z = 1.0f;
    }
}

/**
 * world_to_radar - Convert world position to radar position
 *
 * @param world_x World X coordinate
 * @param world_z World Z coordinate
 * @param radar_x Output radar X
 * @param radar_y Output radar Y
 */
void world_to_radar(f32 world_x, f32 world_z, f32 *radar_x, f32 *radar_y) {
    f32 rel_x, rel_z;

    /* Convert to relative position */
    rel_x = (world_x - radar_min_x) * radar_scale_x;
    rel_z = (world_z - radar_min_z) * radar_scale_z;

    /* Clamp to radar bounds */
    if (rel_x < 0.0f) rel_x = 0.0f;
    if (rel_x > (f32)HUD_RADAR_SIZE) rel_x = (f32)HUD_RADAR_SIZE;
    if (rel_z < 0.0f) rel_z = 0.0f;
    if (rel_z > (f32)HUD_RADAR_SIZE) rel_z = (f32)HUD_RADAR_SIZE;

    /* Convert to screen coords */
    *radar_x = (f32)HUD_RADAR_X + rel_x;
    *radar_y = (f32)HUD_RADAR_Y + (f32)HUD_RADAR_SIZE - rel_z;  /* Flip Z */
}
