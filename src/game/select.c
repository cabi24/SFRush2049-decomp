/**
 * select.c - Track and car selection for Rush 2049 N64
 *
 * Handles track selection, car selection, and transmission choice.
 * Based on arcade sselect.c patterns.
 */

#include "game/select.h"
#include "game/game.h"
#include "game/input.h"

/* Global state */
SelectState gSelect;

/* Track data */
const TrackInfo gTrackData[NUM_TRACKS] = {
    { "Marina",      "San Francisco", 0, 2, 3, TRACK_FLAG_UNLOCKED, 0, 8, 0 },
    { "Haight",      "San Francisco", 1, 3, 3, TRACK_FLAG_UNLOCKED, 0, 10, 0 },
    { "Metro",       "San Francisco", 2, 3, 3, TRACK_FLAG_UNLOCKED, 0, 9, 0 },
    { "Mission",     "San Francisco", 3, 4, 3, TRACK_FLAG_UNLOCKED, 0, 11, 0 },
    { "Presidio",    "San Francisco", 4, 4, 3, 0, 0, 10, 0 },  /* Locked initially */
    { "Downtown",    "San Francisco", 5, 5, 3, 0, 0, 12, 0 },
    { "Alcatraz",    "San Francisco", 6, 5, 4, TRACK_FLAG_HIDDEN, 0, 14, 0 },
    { "Stunt Arena", "San Francisco", 7, 1, 0, TRACK_FLAG_BONUS, 0, 0, 0 },
};

/* Car data */
const CarInfo gCarData[NUM_CARS] = {
    { "Rocket",     0, CAR_FLAG_UNLOCKED, 8, 6, 7, 5, 0, 0 },
    { "Venom",      1, CAR_FLAG_UNLOCKED, 7, 7, 6, 6, 0, 0 },
    { "Crusher",    2, CAR_FLAG_UNLOCKED, 6, 5, 5, 9, 0, 0 },
    { "Prototype",  3, CAR_FLAG_UNLOCKED, 9, 5, 8, 4, 0, 0 },
    { "Velocity",   4, 0, 8, 8, 7, 5, 0, 0 },  /* Locked initially */
    { "Hot Rod",    5, 0, 7, 6, 9, 6, 0, 0 },
    { "Concept",    6, CAR_FLAG_HIDDEN, 10, 7, 9, 4, 0, 0 },
    { "Stunt Car",  7, CAR_FLAG_SPECIAL, 6, 9, 6, 5, 0, 0 },
};

/*
 * select_init - Initialize selection system
 */
void select_init(void)
{
    select_reset();
}

/*
 * select_reset - Reset selection state
 */
void select_reset(void)
{
    gSelect.phase = 0;
    gSelect.complete = 0;
    gSelect.cancelled = 0;
    gSelect.player = 0;

    /* Reset track selection */
    gSelect.track.state = SELECT_STATE_IDLE;
    gSelect.track.sub_state = TRKSEL_BROWSING;
    gSelect.track.current_track = 0;
    gSelect.track.selected_track = 0;
    gSelect.track.mirror_mode = 0;
    gSelect.track.timer = 0;
    gSelect.track.lock_delay = 0;
    gSelect.track.cursor.position = 0;
    gSelect.track.cursor.target = 0;
    gSelect.track.cursor.scroll_offset = 0.0f;
    gSelect.track.cursor.scroll_speed = 0.0f;
    gSelect.track.cursor.moving = 0;
    gSelect.track.preview_rotation = 0.0f;
    gSelect.track.show_preview = 1;

    /* Reset car selection */
    gSelect.car.state = SELECT_STATE_IDLE;
    gSelect.car.sub_state = CARSEL_BROWSING;
    gSelect.car.current_car = 0;
    gSelect.car.selected_car = 0;
    gSelect.car.current_color = 0;
    gSelect.car.selected_color = 0;
    gSelect.car.show_stats = 1;
    gSelect.car.timer = 0;
    gSelect.car.lock_delay = 0;
    gSelect.car.cursor.position = 0;
    gSelect.car.cursor.target = 0;
    gSelect.car.cursor.scroll_offset = 0.0f;
    gSelect.car.cursor.scroll_speed = 0.0f;
    gSelect.car.cursor.moving = 0;
    gSelect.car.car_rotation = 0.0f;
    gSelect.car.rotation_speed = 0.5f;
    gSelect.car.auto_rotate = 1;
    gSelect.car.transmission = TRANS_AUTO;
    gSelect.car.trans_locked = 0;

    /* Reset final selections */
    gSelect.final_track = 0;
    gSelect.final_car = 0;
    gSelect.final_color = 0;
    gSelect.final_trans = TRANS_AUTO;
    gSelect.final_mirror = 0;
    gSelect.final_laps = 3;
}

/*
 * select_start - Begin selection process
 */
void select_start(void)
{
    select_reset();
    gSelect.phase = 0;
    track_select_init();
}

/*
 * select_update - Update selection state
 */
void select_update(void)
{
    switch (gSelect.phase) {
        case 0:
            track_select_update();
            if (track_select_is_locked()) {
                select_next_phase();
            }
            break;

        case 1:
            car_select_update();
            if (car_select_is_locked()) {
                select_next_phase();
            }
            break;

        case 2:
            trans_select_update();
            /* Transition handled by game state machine */
            break;
    }
}

/*
 * select_draw - Draw selection screen
 */
void select_draw(void)
{
    switch (gSelect.phase) {
        case 0:
            track_select_draw();
            break;
        case 1:
            car_select_draw();
            break;
        case 2:
            trans_select_draw();
            break;
    }
}

/*
 * select_next_phase - Advance to next selection phase
 */
void select_next_phase(void)
{
    switch (gSelect.phase) {
        case 0:
            /* Track -> Car */
            gSelect.final_track = gSelect.track.selected_track;
            gSelect.final_mirror = gSelect.track.mirror_mode;
            gSelect.final_laps = gTrackData[gSelect.final_track].laps;
            gSelect.phase = 1;
            car_select_init();
            break;

        case 1:
            /* Car -> Transmission */
            gSelect.final_car = gSelect.car.selected_car;
            gSelect.final_color = gSelect.car.selected_color;
            gSelect.phase = 2;
            trans_select_init();
            break;

        case 2:
            /* Complete */
            gSelect.final_trans = gSelect.car.transmission;
            gSelect.complete = 1;
            break;
    }
}

/*
 * select_prev_phase - Go back to previous phase
 */
void select_prev_phase(void)
{
    if (gSelect.phase > 0) {
        gSelect.phase--;

        switch (gSelect.phase) {
            case 0:
                track_select_init();
                gSelect.track.sub_state = TRKSEL_BROWSING;
                break;
            case 1:
                car_select_init();
                gSelect.car.sub_state = CARSEL_BROWSING;
                break;
        }
    }
}

/*
 * select_is_complete - Check if selection is done
 */
s32 select_is_complete(void)
{
    return gSelect.complete;
}

/*
 * Track Selection
 */

void track_select_init(void)
{
    gSelect.track.state = SELECT_STATE_ACTIVE;
    gSelect.track.sub_state = TRKSEL_BROWSING;
    gSelect.track.timer = TRK_SELECT_TIME;
    gSelect.track.lock_delay = TRK_DELAY_TIME;
    gSelect.track.preview_rotation = 0.0f;

    /* Find first available track */
    while (gSelect.track.current_track < NUM_TRACKS &&
           !track_is_available(gSelect.track.current_track)) {
        gSelect.track.current_track++;
    }

    gSelect.track.cursor.position = gSelect.track.current_track;
    gSelect.track.cursor.target = gSelect.track.current_track;
}

void track_select_update(void)
{
    /* Update timer */
    if (gSelect.track.timer > 0) {
        gSelect.track.timer--;
    }

    /* Update lock delay */
    if (gSelect.track.lock_delay > 0) {
        gSelect.track.lock_delay--;
    }

    /* Handle timeout */
    if (gSelect.track.timer == 0 &&
        gSelect.track.sub_state != TRKSEL_LOCKED) {
        /* Auto-select current track on timeout */
        track_select_confirm();
        return;
    }

    /* Handle input */
    track_select_input();

    /* Update preview rotation */
    gSelect.track.preview_rotation += 0.5f;
    if (gSelect.track.preview_rotation >= 360.0f) {
        gSelect.track.preview_rotation -= 360.0f;
    }

    /* Smooth cursor animation */
    if (gSelect.track.cursor.moving) {
        f32 diff = (f32)gSelect.track.cursor.target -
                   gSelect.track.cursor.scroll_offset;
        if (diff > 0.1f || diff < -0.1f) {
            gSelect.track.cursor.scroll_offset += diff * 0.2f;
        } else {
            gSelect.track.cursor.scroll_offset = (f32)gSelect.track.cursor.target;
            gSelect.track.cursor.moving = 0;
        }
    }
}

void track_select_input(void)
{
    if (gSelect.track.sub_state == TRKSEL_LOCKED) {
        return;
    }

    /* Left/Right to change track */
    /* TODO: Read actual controller input */
    /* For now, placeholder input handling */

    /* A button to confirm */
    /* B button for mirror mode toggle */
}

void track_select_confirm(void)
{
    if (gSelect.track.lock_delay > 0) {
        return;
    }

    if (!track_is_available(gSelect.track.current_track)) {
        return;
    }

    gSelect.track.selected_track = gSelect.track.current_track;
    gSelect.track.sub_state = TRKSEL_LOCKED;
    gSelect.track.state = SELECT_STATE_LOCKED;

    /* Play confirmation sound */
    /* sound_play(SFX_CONFIRM); */
}

void track_select_cancel(void)
{
    gSelect.cancelled = 1;
}

s32 track_select_is_locked(void)
{
    return (gSelect.track.sub_state == TRKSEL_LOCKED);
}

void track_select_draw(void)
{
    /* TODO: Draw track selection UI */
    /* - Track preview (3D or image) */
    /* - Track name and info */
    /* - Timer countdown */
    /* - Mirror mode indicator */
    select_draw_countdown();
}

/*
 * Car Selection
 */

void car_select_init(void)
{
    gSelect.car.state = SELECT_STATE_ACTIVE;
    gSelect.car.sub_state = CARSEL_BROWSING;
    gSelect.car.timer = CAR_SELECT_TIME;
    gSelect.car.lock_delay = CAR_DELAY_TIME;
    gSelect.car.car_rotation = 0.0f;

    /* Find first available car */
    while (gSelect.car.current_car < NUM_CARS &&
           !car_is_available(gSelect.car.current_car)) {
        gSelect.car.current_car++;
    }

    gSelect.car.cursor.position = gSelect.car.current_car;
    gSelect.car.cursor.target = gSelect.car.current_car;
}

void car_select_update(void)
{
    /* Update timer */
    if (gSelect.car.timer > 0) {
        gSelect.car.timer--;
    }

    /* Update lock delay */
    if (gSelect.car.lock_delay > 0) {
        gSelect.car.lock_delay--;
    }

    /* Handle timeout */
    if (gSelect.car.timer == 0 &&
        gSelect.car.sub_state != CARSEL_LOCKED) {
        car_select_confirm();
        return;
    }

    /* Handle input */
    car_select_input();

    /* Update car rotation */
    if (gSelect.car.auto_rotate) {
        gSelect.car.car_rotation += gSelect.car.rotation_speed;
        if (gSelect.car.car_rotation >= 360.0f) {
            gSelect.car.car_rotation -= 360.0f;
        }
    }

    /* Smooth cursor animation */
    if (gSelect.car.cursor.moving) {
        f32 diff = (f32)gSelect.car.cursor.target -
                   gSelect.car.cursor.scroll_offset;
        if (diff > 0.1f || diff < -0.1f) {
            gSelect.car.cursor.scroll_offset += diff * 0.2f;
        } else {
            gSelect.car.cursor.scroll_offset = (f32)gSelect.car.cursor.target;
            gSelect.car.cursor.moving = 0;
        }
    }
}

void car_select_input(void)
{
    if (gSelect.car.sub_state == CARSEL_LOCKED) {
        return;
    }

    /* Left/Right to change car */
    /* Up/Down to change color */
    /* A button to confirm */
    /* B button to go back */
    /* TODO: Read actual controller input */
}

void car_select_confirm(void)
{
    if (gSelect.car.lock_delay > 0) {
        return;
    }

    if (!car_is_available(gSelect.car.current_car)) {
        return;
    }

    gSelect.car.selected_car = gSelect.car.current_car;
    gSelect.car.selected_color = gSelect.car.current_color;
    gSelect.car.sub_state = CARSEL_LOCKED;
    gSelect.car.state = SELECT_STATE_LOCKED;

    /* Play confirmation sound */
    /* sound_play(SFX_CONFIRM); */
}

void car_select_cancel(void)
{
    select_prev_phase();
}

s32 car_select_is_locked(void)
{
    return (gSelect.car.sub_state == CARSEL_LOCKED);
}

void car_select_draw(void)
{
    /* TODO: Draw car selection UI */
    /* - 3D car model rotating */
    /* - Car name and stats */
    /* - Color selector */
    /* - Timer countdown */
    select_draw_countdown();
}

/*
 * Transmission Selection
 */

void trans_select_init(void)
{
    gSelect.car.trans_locked = 0;
}

void trans_select_update(void)
{
    /* Handle input */
    /* A to confirm auto, B to confirm manual */
    /* Or left/right to toggle, A to confirm */
    /* TODO: Read actual controller input */
}

void trans_select_toggle(void)
{
    if (!gSelect.car.trans_locked) {
        gSelect.car.transmission = (gSelect.car.transmission == TRANS_AUTO) ?
                                    TRANS_MANUAL : TRANS_AUTO;
    }
}

void trans_select_draw(void)
{
    /* TODO: Draw transmission selector */
    /* - AUTO / MANUAL options */
    /* - Current selection highlighted */
}

/*
 * Query functions
 */

u8 select_get_track(void)
{
    return gSelect.final_track;
}

u8 select_get_car(void)
{
    return gSelect.final_car;
}

u8 select_get_color(void)
{
    return gSelect.final_color;
}

u8 select_get_transmission(void)
{
    return gSelect.final_trans;
}

u8 select_get_mirror(void)
{
    return gSelect.final_mirror;
}

u8 select_get_laps(void)
{
    return gSelect.final_laps;
}

/*
 * Track/car availability
 */

s32 track_is_available(u8 track_id)
{
    if (track_id >= NUM_TRACKS) {
        return 0;
    }

    /* Check if unlocked */
    if (!(gTrackData[track_id].flags & TRACK_FLAG_UNLOCKED)) {
        return 0;
    }

    /* Hidden tracks require special unlock */
    if (gTrackData[track_id].flags & TRACK_FLAG_HIDDEN) {
        /* TODO: Check if hidden tracks unlocked */
        return 0;
    }

    return 1;
}

s32 car_is_available(u8 car_id)
{
    if (car_id >= NUM_CARS) {
        return 0;
    }

    /* Check if unlocked */
    if (!(gCarData[car_id].flags & CAR_FLAG_UNLOCKED)) {
        return 0;
    }

    /* Hidden cars require special unlock */
    if (gCarData[car_id].flags & CAR_FLAG_HIDDEN) {
        /* TODO: Check if hidden cars unlocked */
        return 0;
    }

    return 1;
}

void track_unlock(u8 track_id)
{
    /* TODO: Unlock track in save data */
    /* For now, this would modify gTrackData but it's const */
}

void car_unlock(u8 car_id)
{
    /* TODO: Unlock car in save data */
}

/*
 * Preview rendering (stubs)
 */

void track_preview_render(u8 track_id, f32 rotation)
{
    /* TODO: Render track preview */
}

void car_preview_render(u8 car_id, u8 color, f32 rotation)
{
    /* TODO: Render car preview model */
}

/*
 * Timer display
 */

void select_draw_timer(s32 x, s32 y, u32 frames)
{
    u32 seconds = frames / 60;
    /* TODO: Draw timer text at x,y */
}

void select_draw_countdown(void)
{
    u32 timer;

    switch (gSelect.phase) {
        case 0:
            timer = gSelect.track.timer;
            break;
        case 1:
            timer = gSelect.car.timer;
            break;
        default:
            return;
    }

    /* Draw timer in top corner */
    select_draw_timer(280, 20, timer);

    /* Blink when running low */
    if (timer < HURRY_BLINK_TIME * 5) {
        if ((timer / HURRY_BLINK_TIME) & 1) {
            /* TODO: Flash "HURRY UP" text */
        }
    }
}
