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
