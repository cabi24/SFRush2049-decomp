/**
 * hud.c - Heads-Up Display for Rush 2049 N64
 *
 * Based on arcade game/hud.c
 * Displays speedometer, laps, position, timer, radar, and indicators.
 *
 * Key arcade functions ported:
 * - InitHUD, ShowHUD, RemoveHUD
 * - cvt_time (time conversion)
 * - ShowCheckpointTime, EnableRadioHUD
 * - set_radar_mapping
 *
 * Copyright 1996-1999 Time Warner Interactive / Atari Games
 */

#include "types.h"
#include "game/hud.h"
#include "game/structs.h"

/*===============================  EXTERNS  =================================*/

/* External game state */
extern u8 gstate;
extern u32 frame_counter;
extern CarData car_array[];
extern s32 num_active_cars;
extern s32 this_car;

/* External functions */
extern f32 sqrtf(f32 x);

/*===============================  GLOBALS  =================================*/

/* HUD state */
HudState gHud;
CarMarker hud_markers[MAX_CARS];

/* Arcade-compatible globals */
s32 gHudMovie;
s8 wrong_way;

/* Time string buffers (arcade-compatible) */
static char TrackTimeStr[8];
static char CountdownStr[8];
static char CheckpointStr[8];

/* Radar mapping state */
static f32 radar_min_x, radar_max_x;
static f32 radar_min_z, radar_max_z;
static f32 radar_scale_x, radar_scale_z;

/* Track map state (arcade-compatible) */
static f32 gMTrackX, gMTrackY;
static f32 gTTrackX, gTTrackY;
static f32 gMRadarX, gMRadarY;
static f32 gTRadarX, gTRadarY;

/* Frame rate constant */
#define FRAMES_PER_SEC  60

/*===============================  FUNCTIONS  ===============================*/

/**
 * InitHUD - Initialize HUD system
 * Based on arcade: hud.c:InitHUD()
 */
void InitHUD(void) {
#ifdef NON_MATCHING
    /* Arcade: strcpy(TrackName, "TRACK1_1"); */
    gHud.radio_timeout = 0;

    /* Initialize radar mapping with default values */
    set_radar_mapping(-103.12f, 103.12f, 78.75f, -401.25f);
#endif
}

/**
 * ShowHUD - Show the HUD display
 * Based on arcade: hud.c:ShowHUD()
 */
void ShowHUD(void) {
#ifdef NON_MATCHING
    /* Arcade creates HudBlit if it doesn't exist:
     * if (!HudBlit) {
     *     InitDigitList();
     *     HudBlit = NewMultiBlit(0, 0, HudMultiBlit, NBLITS(HudMultiBlit));
     *     gFinishHid = true;
     *     gRadioTimeout = IRQTIME;
     *     if (demo_game) AddJoinNow(HudBlit, 0, true);
     *     markers_valid = false;
     * }
     */
    InitDigitList();
    gHud.finish_hidden = 1;
    gHud.markers_init = 0;
#endif
}

/**
 * RemoveHUD - Remove HUD from display
 * Based on arcade: hud.c:RemoveHUD()
 */
void RemoveHUD(void) {
#ifdef NON_MATCHING
    /* Arcade removes radio blit and main HUD blit:
     * if (RadioBlit) { RemoveBlit(RadioBlit); RadioBlit = nil; }
     * if (HudBlit) { RemoveBlit(HudBlit); gFinishHid = true; HudBlit = nil; }
     * RemoveMsgs();
     */
    gHud.finish_hidden = 1;
#endif
}

/**
 * InitDigitList - Initialize digit texture list
 * Based on arcade: hud.c:InitDigitList()
 */
void InitDigitList(void) {
#ifdef NON_MATCHING
    /* Arcade loads digit textures:
     * for (i = 0; i < NUM_DIGIT_TYPES; ++i)
     *     sprintf(gDigitList[i].Name, "DIGIT%1ld", i);
     * InitBlitList(gDigitList, NUM_DIGIT_TYPES);
     */
#endif
}

/**
 * hud_init - Initialize HUD system (N64-style wrapper)
 */
void hud_init(void) {
    s32 i;

    gHud.flags = HUD_SHOW_ALL;
    gHud.speed_display = 0;
    gHud.target_speed = 0;
    gHud.rpm_display = 0;
    gHud.current_lap = 1;
    gHud.total_laps = 3;
    gHud.position = 1;
    gHud.total_cars = 8;
    gHud.elapsed_time = 0;
    gHud.countdown_time = 0;
    gHud.checkpoint_time = 0;
    gHud.checkpoint_timeout = 0;
    gHud.checkpoint_timeout2 = 0;
    gHud.checkpoint_flash = 0;
    gHud.radio_timeout = 0;
    gHud.wrong_way = 0;
    gHud.markers_init = 0;
    gHud.metric_mode = 0;
    gHud.auto_trans = 0;
    gHud.man_gear = 0;
    gHud.mBlit_index = 0;
    gHud.stray_angle = 0.0f;
    gHud.finish_x = 0;
    gHud.finish_y = 0;
    gHud.finish_hidden = 1;
    gHud.laps_left = 0;

    /* Clear markers */
    for (i = 0; i < MAX_CARS; i++) {
        hud_markers[i].x = 0.0f;
        hud_markers[i].y = 0.0f;
        hud_markers[i].z = 0.0f;
        hud_markers[i].color = 0;
        hud_markers[i].visible = 0;
        hud_markers[i].is_player = 0;
        hud_markers[i].is_flashing = 0;
    }

    /* Initialize arcade-style state */
    InitHUD();
}

/**
 * hud_reset - Reset HUD for new race
 */
void hud_reset(void) {
    gHud.speed_display = 0;
    gHud.target_speed = 0;
    gHud.rpm_display = 0;
    gHud.current_lap = 1;
    gHud.elapsed_time = 0;
    gHud.checkpoint_time = 0;
    gHud.checkpoint_timeout = 0;
    gHud.checkpoint_timeout2 = 0;
    gHud.checkpoint_flash = 0;
    gHud.wrong_way = 0;
    gHud.stray_angle = 0.0f;
}

/**
 * hud_update - Update HUD state each frame
 */
void hud_update(void) {
    CarData *car;

    car = &car_array[this_car];

    /* Update speed display (smooth toward target) */
    gHud.target_speed = (s16)(car->mph * 0.6818f);
    if (gHud.metric_mode) {
        gHud.target_speed = (s16)(gHud.target_speed * 1.61f);
    }

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

    /* Update RPM */
    gHud.rpm_display = car->rpm;

    /* Update lap count */
    gHud.current_lap = car->laps + 1;

    /* Update position */
    gHud.position = car->place + 1;

    /* Update timer */
    gHud.elapsed_time += (1000 / FRAMES_PER_SEC);

    /* Decrement countdown if active */
    if (gHud.countdown_time > 0) {
        gHud.countdown_time -= (1000 / FRAMES_PER_SEC);
    }

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
    if ((gHud.flags & HUD_SHOW_WRONG_WAY) && gHud.wrong_way) {
        hud_draw_wrong_way();
    }
    if ((gHud.flags & HUD_SHOW_CHECKTIME) && (gHud.checkpoint_flash > 0)) {
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
#ifdef NON_MATCHING
    /* Would use N64 graphics to draw speed number */
    /* Arcade uses AnimateSpeed callback for blit updates */
#endif
}

/**
 * hud_draw_laps - Draw lap counter
 */
void hud_draw_laps(void) {
#ifdef NON_MATCHING
    /* Draw "LAP X/Y" text */
    /* Arcade uses AnimateLapDigit callback */
#endif
}

/**
 * hud_draw_position - Draw race position
 */
void hud_draw_position(void) {
#ifdef NON_MATCHING
    /* Draw position with suffix (1st, 2nd, etc) */
    /* Arcade uses AnimatePlace callback */
#endif
}

/**
 * hud_draw_timer - Draw race timer
 */
void hud_draw_timer(void) {
#ifdef NON_MATCHING
    /* Draw elapsed time in MM:SS.SS format */
    /* Arcade uses AnimateElapsed and AnimateCTimer callbacks */
#endif
}

/**
 * hud_draw_radar - Draw mini-map/radar
 */
void hud_draw_radar(void) {
#ifdef NON_MATCHING
    /* Draw track outline and car positions */
    /* Arcade uses AnimateRadar and AnimateTrackMap callbacks */
#endif
}

/**
 * hud_draw_wrong_way - Draw wrong way warning
 * Based on arcade: hud.c:AnimateWrongWay()
 */
void hud_draw_wrong_way(void) {
#ifdef NON_MATCHING
    /* Flash "WRONG WAY" text on screen */
    /* Arcade implementation handles animation and scaling */
#endif
}

/**
 * hud_draw_checkpoint_bonus - Draw checkpoint time bonus
 * Based on arcade: hud.c:AnimateCheckPt()
 */
void hud_draw_checkpoint_bonus(void) {
#ifdef NON_MATCHING
    /* Show elapsed or delta time when hitting checkpoint */
#endif
}

/**
 * hud_update_markers - Update car markers on radar
 * Based on arcade: hud.c marker code in AnimateTach()
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
        hud_markers[i].is_flashing = 0;

        /* Set color based on player/drone */
        if (i == this_car) {
            hud_markers[i].color = 1;
        } else {
            hud_markers[i].color = 2;
        }
    }

    gHud.markers_init = 1;
}

/**
 * hud_draw_markers - Draw car markers on radar
 * Based on arcade: hud.c:AnimateDot() and AnimateMarker()
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
#ifdef NON_MATCHING
        /* Would use N64 graphics primitives */
#endif
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

    car = &car_array[car_index];

    /* Get world position */
    world_x = car->dr_pos[0];
    world_z = car->dr_pos[2];

    /* Convert to radar coordinates */
    world_to_radar(world_x, world_z, &rel_x, &rel_z);

    *out_x = rel_x;
    *out_y = rel_z;
}

/**
 * hud_show - Show HUD elements
 */
void hud_show(u16 flags) {
    gHud.flags |= flags;
}

/**
 * hud_hide - Hide HUD elements
 */
void hud_hide(u16 flags) {
    gHud.flags &= ~flags;
}

/**
 * hud_toggle - Toggle HUD elements
 */
void hud_toggle(u16 flags) {
    gHud.flags ^= flags;
}

/**
 * hud_checkpoint_hit - Handle checkpoint crossing
 */
void hud_checkpoint_hit(u32 time_bonus) {
    gHud.checkpoint_time = time_bonus;
    gHud.checkpoint_flash = FRAMES_PER_SEC * 2;
}

/**
 * hud_lap_complete - Handle lap completion
 */
void hud_lap_complete(s32 lap_num) {
    gHud.current_lap = (s8)(lap_num + 1);
}

/**
 * hud_set_wrong_way - Set wrong way indicator
 */
void hud_set_wrong_way(s32 wrong) {
    gHud.wrong_way = wrong ? 1 : 0;
    wrong_way = gHud.wrong_way;
}

/**
 * ShowCheckpointTime - Show checkpoint delta time
 * Based on arcade: hud.c:ShowCheckpointTime()
 *
 * @param delta Time delta in milliseconds
 */
void ShowCheckpointTime(s32 delta) {
    s32 t;

    /* Set the go away time */
    gHud.checkpoint_timeout = gHud.elapsed_time + CHECKPT_TIME;

    /* Convert the value to a string */
    t = (s32)gHud.elapsed_time;
    cvt_time(t, (u8 *)CheckpointStr, 'h');
}

/**
 * ShowFinishFlag - Show/hide finish line flag
 * Based on arcade: hud.c:ShowFinishFlag()
 *
 * @param pos World position of finish line
 * @param hide Non-zero to hide
 */
void ShowFinishFlag(f32 pos[3], s32 hide) {
    gHud.finish_hidden = hide ? 1 : 0;

    /* Convert world position to track map coordinates */
    gHud.finish_x = (s16)(40.0f * pos[0] * gMTrackX + gTTrackX) - 14;
    gHud.finish_y = (s16)(40.0f * pos[2] * gMTrackY + gTTrackY) - 14;
}

/**
 * EnableRadioHUD - Show radio/CD player display
 * Based on arcade: hud.c:EnableRadioHUD()
 */
void EnableRadioHUD(void) {
#ifdef NON_MATCHING
    /* Arcade implementation:
     * if (gstate != PLAYGAME || mBlitIndex == MBLIT_PLACE) return;
     * if (!RadioBlit) {
     *     RadioBlit = NewMultiBlit(0, 0, RadioMultiBlit, NBLITS(RadioMultiBlit));
     *     mBlitIndex = MBLIT_CD;
     * }
     * gRadioTimeout = IRQTIME + ONE_SEC * 3;
     */
    gHud.radio_timeout = gHud.elapsed_time + (ONE_SEC * 3);
    gHud.mBlit_index = MBLIT_CD;
#endif
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
 */
void hud_format_speed(s32 speed, char *buffer) {
    s32 hundreds, tens, ones;

    if (speed > 999) speed = 999;
    if (speed < 0) speed = 0;

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
    u8 ret;

    ret = 0;

    if (t < 0) {
        t = 0;
    }

    switch (format) {
        case 'c':
            /* Countdown timer (seconds only, max 999) */
            sec = t / 1000;
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
            /* Fall through */

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

        default:
            break;
    }

    return ret;
}

/**
 * cvt_time_frames - Convert frame count to time format
 *
 * @param frames Frame count
 * @param dest Output array (at least 5 bytes)
 * @param format 's' = MM:SS, 'h' = MM:SS.hh, 'c' = SSS countdown
 * @return Number of digits set
 */
u8 cvt_time_frames(u32 frames, u8 *dest, char format) {
    u32 ms;
    ms = (frames * 1000) / FRAMES_PER_SEC;
    return cvt_time((s32)ms, dest, format);
}

/**
 * hud_format_position - Format race position with suffix
 */
void hud_format_position(s32 position, char *buffer) {
    if (position < 1) position = 1;
    if (position > 8) position = 8;

    buffer[0] = '0' + position;

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
 */
s32 hud_get_speed_mph(void) {
    return gHud.speed_display;
}

/**
 * hud_get_speed_kph - Get current player speed in KPH
 */
s32 hud_get_speed_kph(void) {
    return (gHud.speed_display * 161) / 100;
}

/**
 * hud_get_elapsed_frames - Get elapsed race time in frames
 */
u32 hud_get_elapsed_frames(void) {
    return (gHud.elapsed_time * FRAMES_PER_SEC) / 1000;
}

/**
 * hud_get_elapsed_ms - Get elapsed race time in milliseconds
 */
u32 hud_get_elapsed_ms(void) {
    return gHud.elapsed_time;
}

/**
 * hud_set_countdown_time - Set remaining countdown time
 */
void hud_set_countdown_time(u32 ms) {
    gHud.countdown_time = ms;
}

/**
 * hud_get_countdown_time - Get remaining countdown time
 */
u32 hud_get_countdown_time(void) {
    return gHud.countdown_time;
}

/**
 * hud_set_metric - Enable/disable metric units
 */
void hud_set_metric(s32 metric) {
    gHud.metric_mode = metric ? 1 : 0;
}

/**
 * hud_is_metric - Check if metric mode is enabled
 */
s32 hud_is_metric(void) {
    return gHud.metric_mode;
}

/**
 * set_radar_mapping - Set radar coordinate mapping
 * Based on arcade: hud.c:set_radar_mapping()
 *
 * @param lx World low X
 * @param ly World low Y (Z)
 * @param hx World high X
 * @param hy World high Y (Z)
 */
void set_radar_mapping(f32 lx, f32 hx, f32 ly, f32 hy) {
    radar_min_x = lx;
    radar_max_x = hx;
    radar_min_z = ly;
    radar_max_z = hy;

    /* Calculate scale factors (arcade formula) */
    gMRadarX = (RADAR_WIDE - RADAR_BWIDE * 2.0f) / (hx - lx);
    gMRadarY = (RADAR_HIGH - RADAR_BHIGH * 2.0f) / (hy - ly);
    gTRadarX = (f32)RADAR_X + RADAR_CENTER_X - TCAR_W / 2.0f;
    gTRadarY = (f32)RADAR_Y + RADAR_CENTER_Y - TCAR_H / 2.0f;

    /* Also set scale for world_to_radar */
    if (hx != lx) {
        radar_scale_x = (f32)HUD_RADAR_SIZE / (hx - lx);
    } else {
        radar_scale_x = 1.0f;
    }

    if (hy != ly) {
        radar_scale_z = (f32)HUD_RADAR_SIZE / (hy - ly);
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

    rel_x = (world_x - radar_min_x) * radar_scale_x;
    rel_z = (world_z - radar_min_z) * radar_scale_z;

    /* Clamp to radar bounds */
    if (rel_x < 0.0f) rel_x = 0.0f;
    if (rel_x > (f32)HUD_RADAR_SIZE) rel_x = (f32)HUD_RADAR_SIZE;
    if (rel_z < 0.0f) rel_z = 0.0f;
    if (rel_z > (f32)HUD_RADAR_SIZE) rel_z = (f32)HUD_RADAR_SIZE;

    *radar_x = (f32)HUD_RADAR_X + rel_x;
    *radar_y = (f32)HUD_RADAR_Y + (f32)HUD_RADAR_SIZE - rel_z;
}

/**
 * Hidden - Set blit hidden state
 * Based on arcade: hud.c:Hidden()
 *
 * @param blt Blit pointer (void* for N64 compatibility)
 * @param hide Non-zero to hide
 * @return Current hidden state
 */
s32 Hidden(void *blt, s32 hide) {
#ifdef NON_MATCHING
    /* Arcade implementation:
     * if (hide != blt->Hide) {
     *     blt->Hide = hide;
     *     UpdateBlit(blt);
     * }
     * return blt->Hide;
     */
#endif
    return hide;
}

/**
 * UpdateNumber - Update digit blit
 * Based on arcade: hud.c:UpdateNumber()
 *
 * @param blt Blit pointer
 * @param digit Digit value (0-9)
 * @return 1 on success
 */
s32 UpdateNumber(void *blt, u8 digit) {
#ifdef NON_MATCHING
    /* Arcade implementation handles:
     * - Font type changes
     * - Digit texture selection
     * - Blit size based on digit type
     */
#endif
    return 1;
}

/**
 * updateList - Update blit from value list
 * Based on arcade: hud.c:updateList()
 */
s32 updateList(void *blt, s32 val, s32 num, ...) {
#ifdef NON_MATCHING
    /* Arcade uses varargs to select from list of texture names */
#endif
    return 1;
}

/**
 * updateList2 - Update blit from value list (variant)
 * Based on arcade: hud.c:updateList2()
 */
s32 updateList2(void *blt, s32 val, s32 num, ...) {
#ifdef NON_MATCHING
    /* Similar to updateList but always updates */
#endif
    return 1;
}

/**
 * CropBlit - Crop blit to specified rectangle
 * Based on arcade: hud.c:CropBlit()
 *
 * @param blt Blit pointer
 * @param l Left edge
 * @param t Top edge
 * @param r Right edge
 * @param b Bottom edge
 */
void CropBlit(void *blt, s32 l, s32 t, s32 r, s32 b) {
#ifdef NON_MATCHING
    /* Arcade implementation:
     * blt->Top = blt->Info->Height - t;
     * blt->Bot = blt->Info->Height - b + 1;
     * blt->Height = b - t + 1;
     * blt->Width = r - l + 1;
     * blt->Left = l;
     * blt->Right = r + 1;
     * UpdateBlit(blt);
     */
#endif
}
