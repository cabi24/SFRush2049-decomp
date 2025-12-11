/**
 * carsel.c - Car/Track selection for Rush 2049 N64
 *
 * Based on arcade game/sselect.c (select.c)
 * Handles car selection, track selection, and pre-race setup.
 */

#include "game/carsel.h"
#include "game/game.h"

/* Selection timing constants (60 fps) */
#define SELECT_COUNTDOWN_TIME   (30 * 60)   /* 30 seconds to select */
#define INPUT_REPEAT_DELAY      12          /* Frames before input repeat */
#define INPUT_REPEAT_RATE       4           /* Frames between repeats */
#define TURNTABLE_SPEED         0.02f       /* Car preview rotation speed */
#define TRANSITION_TIME         60          /* Frames for transition */

/* Global state */
SelectState gSelect;
s32 gNumCars = MAX_SELECTABLE_CARS;
s32 gNumTracks = MAX_SELECTABLE_TRACKS;

/* Car list - stats for all selectable cars */
CarStats gCarList[MAX_SELECTABLE_CARS] = {
    /* Compact class */
    {"Compact 1",   CAR_CLASS_COMPACT, 60, 70, 80, 40, 75, 1, 0, 0, 0},
    {"Compact 2",   CAR_CLASS_COMPACT, 65, 75, 75, 45, 70, 1, 0, 1, 0},
    {"Compact 3",   CAR_CLASS_COMPACT, 70, 65, 85, 35, 80, 0, 0, 2, 0},
    /* Sports class */
    {"Sports 1",    CAR_CLASS_SPORTS,  80, 80, 70, 55, 65, 1, 0, 3, 0},
    {"Sports 2",    CAR_CLASS_SPORTS,  85, 75, 75, 50, 70, 1, 0, 4, 0},
    {"Sports 3",    CAR_CLASS_SPORTS,  90, 70, 65, 60, 60, 0, 0, 5, 0},
    /* Muscle class */
    {"Muscle 1",    CAR_CLASS_MUSCLE,  85, 90, 55, 75, 55, 1, 0, 6, 0},
    {"Muscle 2",    CAR_CLASS_MUSCLE,  90, 85, 50, 80, 50, 1, 0, 7, 0},
    {"Muscle 3",    CAR_CLASS_MUSCLE,  95, 80, 45, 85, 45, 0, 0, 8, 0},
    /* Exotic class */
    {"Exotic 1",    CAR_CLASS_EXOTIC,  95, 85, 70, 55, 60, 0, 0, 9, 0},
    {"Exotic 2",    CAR_CLASS_EXOTIC, 100, 80, 65, 50, 55, 0, 0, 10, 0},
    {"Exotic 3",    CAR_CLASS_EXOTIC, 100, 90, 60, 45, 50, 0, 0, 11, 0},
};

/* Track list - info for all selectable tracks */
TrackInfo gTrackList[MAX_SELECTABLE_TRACKS] = {
    {"Marina",      "Waterfront city circuit",     TRACK_EASY,   3, 8,  1, 0, 0, 0},
    {"Haight",      "Hilly San Francisco streets", TRACK_EASY,   3, 10, 1, 1, 0, 0},
    {"Metro",       "Downtown metropolitan rush",  TRACK_MEDIUM, 3, 12, 1, 2, 0, 0},
    {"Mission",     "Industrial district race",    TRACK_MEDIUM, 3, 10, 1, 3, 0, 0},
    {"Presidio",    "Military base course",        TRACK_HARD,   3, 14, 0, 4, 0, 0},
    {"Alcatraz",    "Island prison escape",        TRACK_HARD,   3, 16, 0, 5, 0, 0},
    {"Lombard",     "Crooked street challenge",    TRACK_EXPERT, 2, 12, 0, 6, 0, 0},
    {"The Rock",    "Ultimate challenge",          TRACK_EXPERT, 2, 18, 0, 7, 0, 0},
};

/*
 * carsel_init - Initialize car selection system
 */
void carsel_init(void)
{
    s32 i;

    /* Clear selection state */
    gSelect.track_index = 0;
    gSelect.track_locked = 0;
    gSelect.num_players = 1;
    gSelect.timer = SELECT_COUNTDOWN_TIME;
    gSelect.state_time = 0;
    gSelect.all_ready = 0;
    gSelect.countdown_active = 0;
    gSelect.transitioning = 0;

    /* Initialize camera */
    gSelect.camera_pos[0] = 0.0f;
    gSelect.camera_pos[1] = 100.0f;
    gSelect.camera_pos[2] = 300.0f;
    gSelect.camera_target[0] = 0.0f;
    gSelect.camera_target[1] = 0.0f;
    gSelect.camera_target[2] = 0.0f;

    /* Initialize all players */
    for (i = 0; i < MAX_PLAYERS; i++) {
        carsel_player_init(i);
    }

    /* First player is always active */
    gSelect.players[0].is_active = 1;
}

/*
 * carsel_reset - Reset selection state for new game
 */
void carsel_reset(void)
{
    carsel_init();
}

/*
 * carsel_player_init - Initialize a single player's selection state
 */
void carsel_player_init(s32 player)
{
    PlayerSelect *ps;

    if (player < 0 || player >= MAX_PLAYERS) {
        return;
    }

    ps = &gSelect.players[player];
    ps->car_index = 0;
    ps->color_index = player;  /* Default to player number color */
    ps->trans_mode = TRANS_AUTO;
    ps->state = SEL_STATE_INACTIVE;
    ps->is_active = 0;
    ps->is_ready = 0;
    ps->input_timer = 0;
    ps->turntable_angle = 0.0f;
}

/*
 * carsel_update - Main update for selection screen
 */
void carsel_update(void)
{
    s32 i;

    gSelect.state_time++;

    /* Update countdown timer if active */
    if (gSelect.countdown_active) {
        carsel_update_countdown();
    }

    /* Update each active player */
    for (i = 0; i < MAX_PLAYERS; i++) {
        if (gSelect.players[i].is_active) {
            carsel_player_update(i);
        }
    }

    /* Check if all players are ready */
    gSelect.all_ready = carsel_all_players_ready();

    /* Start countdown when all ready */
    if (gSelect.all_ready && !gSelect.countdown_active) {
        carsel_start_countdown();
    }

    /* Handle transition to race */
    if (gSelect.transitioning) {
        gSelect.state_time++;
        if (gSelect.state_time >= TRANSITION_TIME) {
            carsel_start_race();
        }
    }
}

/*
 * carsel_draw - Draw selection screen
 */
void carsel_draw(void)
{
    s32 i;

    /* Draw track preview */
    tracksel_draw();

    /* Draw each player's car selection */
    for (i = 0; i < MAX_PLAYERS; i++) {
        if (gSelect.players[i].is_active) {
            carsel_player_draw(i);
        }
    }

    /* Draw countdown overlay if active */
    if (gSelect.countdown_active) {
        /* Draw countdown number */
    }
}

/*
 * carsel_player_update - Update a single player's selection
 */
void carsel_player_update(s32 player)
{
    PlayerSelect *ps;

    if (player < 0 || player >= MAX_PLAYERS) {
        return;
    }

    ps = &gSelect.players[player];

    /* Update input timer */
    if (ps->input_timer > 0) {
        ps->input_timer--;
    }

    /* Update turntable rotation */
    ps->turntable_angle += TURNTABLE_SPEED;
    if (ps->turntable_angle > 6.28318f) {
        ps->turntable_angle -= 6.28318f;
    }

    /* State-specific updates */
    switch (ps->state) {
        case SEL_STATE_INACTIVE:
            /* Waiting for player to join */
            break;

        case SEL_STATE_CHOOSING:
            /* Player is selecting car/options */
            break;

        case SEL_STATE_LOCKED:
            /* Selection locked, waiting for others */
            break;

        case SEL_STATE_CONFIRMED:
            /* Ready to race */
            break;
    }
}

/*
 * carsel_player_draw - Draw a single player's selection UI
 */
void carsel_player_draw(s32 player)
{
    PlayerSelect *ps;
    CarStats *car;

    if (player < 0 || player >= MAX_PLAYERS) {
        return;
    }

    ps = &gSelect.players[player];
    car = carsel_get_car_stats(ps->car_index);

    if (car == NULL) {
        return;
    }

    /* Draw car model on turntable */
    /* Draw car stats bars */
    /* Draw transmission indicator */
    /* Draw ready/locked state */
}

/*
 * Track selection functions
 */

void tracksel_init(void)
{
    gSelect.track_index = 0;
    gSelect.track_locked = 0;
}

void tracksel_update(void)
{
    /* Handle track selection input */
}

void tracksel_draw(void)
{
    TrackInfo *track;

    track = carsel_get_track_info(gSelect.track_index);
    if (track == NULL) {
        return;
    }

    /* Draw track preview image */
    /* Draw track name */
    /* Draw difficulty indicator */
    /* Draw lap count */
}

void tracksel_select_next(void)
{
    if (gSelect.track_locked) {
        return;
    }

    gSelect.track_index++;
    if (gSelect.track_index >= gNumTracks) {
        gSelect.track_index = 0;
    }

    /* Skip locked tracks */
    while (!carsel_is_track_available(gSelect.track_index)) {
        gSelect.track_index++;
        if (gSelect.track_index >= gNumTracks) {
            gSelect.track_index = 0;
        }
    }
}

void tracksel_select_prev(void)
{
    if (gSelect.track_locked) {
        return;
    }

    gSelect.track_index--;
    if (gSelect.track_index < 0) {
        gSelect.track_index = gNumTracks - 1;
    }

    /* Skip locked tracks */
    while (!carsel_is_track_available(gSelect.track_index)) {
        gSelect.track_index--;
        if (gSelect.track_index < 0) {
            gSelect.track_index = gNumTracks - 1;
        }
    }
}

void tracksel_confirm(void)
{
    gSelect.track_locked = 1;
}

/*
 * Car selection functions
 */

void carsel_select_next_car(s32 player)
{
    PlayerSelect *ps;

    if (player < 0 || player >= MAX_PLAYERS) {
        return;
    }

    ps = &gSelect.players[player];

    if (ps->state != SEL_STATE_CHOOSING) {
        return;
    }

    if (ps->input_timer > 0) {
        return;
    }

    ps->car_index++;
    if (ps->car_index >= gNumCars) {
        ps->car_index = 0;
    }

    /* Skip unavailable cars */
    while (!carsel_is_car_available(ps->car_index)) {
        ps->car_index++;
        if (ps->car_index >= gNumCars) {
            ps->car_index = 0;
        }
    }

    ps->input_timer = INPUT_REPEAT_DELAY;
}

void carsel_select_prev_car(s32 player)
{
    PlayerSelect *ps;

    if (player < 0 || player >= MAX_PLAYERS) {
        return;
    }

    ps = &gSelect.players[player];

    if (ps->state != SEL_STATE_CHOOSING) {
        return;
    }

    if (ps->input_timer > 0) {
        return;
    }

    ps->car_index--;
    if (ps->car_index < 0) {
        ps->car_index = gNumCars - 1;
    }

    /* Skip unavailable cars */
    while (!carsel_is_car_available(ps->car_index)) {
        ps->car_index--;
        if (ps->car_index < 0) {
            ps->car_index = gNumCars - 1;
        }
    }

    ps->input_timer = INPUT_REPEAT_DELAY;
}

void carsel_select_next_color(s32 player)
{
    PlayerSelect *ps;

    if (player < 0 || player >= MAX_PLAYERS) {
        return;
    }

    ps = &gSelect.players[player];

    if (ps->state != SEL_STATE_CHOOSING) {
        return;
    }

    ps->color_index++;
    if (ps->color_index >= 8) {  /* 8 colors per car */
        ps->color_index = 0;
    }
}

void carsel_select_prev_color(s32 player)
{
    PlayerSelect *ps;

    if (player < 0 || player >= MAX_PLAYERS) {
        return;
    }

    ps = &gSelect.players[player];

    if (ps->state != SEL_STATE_CHOOSING) {
        return;
    }

    ps->color_index--;
    if (ps->color_index < 0) {
        ps->color_index = 7;
    }
}

void carsel_toggle_trans(s32 player)
{
    PlayerSelect *ps;

    if (player < 0 || player >= MAX_PLAYERS) {
        return;
    }

    ps = &gSelect.players[player];

    if (ps->state != SEL_STATE_CHOOSING) {
        return;
    }

    ps->trans_mode = (ps->trans_mode == TRANS_AUTO) ? TRANS_MANUAL : TRANS_AUTO;
}

void carsel_confirm(s32 player)
{
    PlayerSelect *ps;

    if (player < 0 || player >= MAX_PLAYERS) {
        return;
    }

    ps = &gSelect.players[player];

    if (ps->state == SEL_STATE_CHOOSING) {
        ps->state = SEL_STATE_LOCKED;
        ps->is_ready = 1;
    }
}

void carsel_cancel(s32 player)
{
    PlayerSelect *ps;

    if (player < 0 || player >= MAX_PLAYERS) {
        return;
    }

    ps = &gSelect.players[player];

    if (ps->state == SEL_STATE_LOCKED) {
        ps->state = SEL_STATE_CHOOSING;
        ps->is_ready = 0;
    }
}

/*
 * Query functions
 */

s32 carsel_get_selected_car(s32 player)
{
    if (player < 0 || player >= MAX_PLAYERS) {
        return 0;
    }
    return gSelect.players[player].car_index;
}

s32 carsel_get_selected_track(void)
{
    return gSelect.track_index;
}

s32 carsel_all_players_ready(void)
{
    s32 i;

    /* Check track is locked */
    if (!gSelect.track_locked) {
        return 0;
    }

    /* Check all active players are ready */
    for (i = 0; i < MAX_PLAYERS; i++) {
        if (gSelect.players[i].is_active && !gSelect.players[i].is_ready) {
            return 0;
        }
    }

    return 1;
}

s32 carsel_is_car_available(s32 car_index)
{
    if (car_index < 0 || car_index >= gNumCars) {
        return 0;
    }
    return gCarList[car_index].unlocked;
}

s32 carsel_is_track_available(s32 track_index)
{
    if (track_index < 0 || track_index >= gNumTracks) {
        return 0;
    }
    return gTrackList[track_index].unlocked;
}

/*
 * Info retrieval
 */

CarStats* carsel_get_car_stats(s32 car_index)
{
    if (car_index < 0 || car_index >= gNumCars) {
        return NULL;
    }
    return &gCarList[car_index];
}

TrackInfo* carsel_get_track_info(s32 track_index)
{
    if (track_index < 0 || track_index >= gNumTracks) {
        return NULL;
    }
    return &gTrackList[track_index];
}

/*
 * Unlock management
 */

void carsel_unlock_car(s32 car_index)
{
    if (car_index < 0 || car_index >= gNumCars) {
        return;
    }
    gCarList[car_index].unlocked = 1;
}

void carsel_unlock_track(s32 track_index)
{
    if (track_index < 0 || track_index >= gNumTracks) {
        return;
    }
    gTrackList[track_index].unlocked = 1;
}

void carsel_save_unlocks(void)
{
    /* Save to EEPROM/Controller Pak */
    /* Implementation depends on save system */
}

void carsel_load_unlocks(void)
{
    /* Load from EEPROM/Controller Pak */
    /* Implementation depends on save system */
}

/*
 * Countdown and transition
 */

void carsel_start_countdown(void)
{
    gSelect.countdown_active = 1;
    gSelect.timer = 3 * 60;  /* 3 second countdown */
}

void carsel_update_countdown(void)
{
    if (gSelect.timer > 0) {
        gSelect.timer--;
    }

    if (carsel_countdown_finished()) {
        gSelect.transitioning = 1;
        gSelect.state_time = 0;
    }
}

s32 carsel_countdown_finished(void)
{
    return (gSelect.countdown_active && gSelect.timer == 0);
}

void carsel_start_race(void)
{
    /* Transition to PREPLAY state */
    /* Set up selected cars for each player */
    /* Load selected track */
    gSelect.transitioning = 0;
}
