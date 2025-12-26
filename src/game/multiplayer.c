/**
 * multiplayer.c - Multiplayer and split-screen system for Rush 2049 N64
 *
 * Manages local multiplayer sessions, split-screen viewports,
 * and controller assignments.
 */

#include "game/multiplayer.h"

/* External controller functions */
extern s32 osContGetQuery(void *status);
extern s32 input_get_button_press(s32 controller, u16 button);
extern s32 input_is_connected(s32 controller);

/* External graphics functions */
extern void gfx_set_viewport(s32 x, s32 y, s32 w, s32 h);
extern void gfx_set_scissor(s32 x, s32 y, s32 w, s32 h);

/* Button constants */
#define BUTTON_START    0x1000

/* Global multiplayer state */
MultiplayerState gMultiplayer;

/* Viewport layouts for different split modes */
static const s16 sViewportLayouts[NUM_MP_MODES][MP_MAX_PLAYERS][4] = {
    /* Single player */
    { {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} },

    /* 2 player horizontal */
    { {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT/2},
      {0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2},
      {0,0,0,0}, {0,0,0,0} },

    /* 2 player vertical */
    { {0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT},
      {SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT},
      {0,0,0,0}, {0,0,0,0} },

    /* 4 player quad */
    { {0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
      {SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
      {0, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
      {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2} },

    /* Battle mode (same as 4 player) */
    { {0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
      {SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
      {0, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
      {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2} },

    /* Stunt mode (same as 4 player) */
    { {0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
      {SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
      {0, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
      {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2} }
};

/* Points awarded per position */
static const s32 sPositionPoints[8] = {
    10, 8, 6, 5, 4, 3, 2, 1
};

/* -------------------------------------------------------------------------- */
/* Initialization                                                              */
/* -------------------------------------------------------------------------- */

/**
 * Initialize multiplayer system
 */
void mp_init(void) {
    s32 i;

    /* Clear all state */
    for (i = 0; i < (s32)sizeof(MultiplayerState); i++) {
        ((u8*)&gMultiplayer)[i] = 0;
    }

    /* Initialize players */
    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        gMultiplayer.players[i].player_index = (u8)i;
        gMultiplayer.players[i].controller = -1;
        gMultiplayer.players[i].state = PLAYER_STATE_INACTIVE;
    }

    /* Initialize controllers */
    for (i = 0; i < MP_MAX_CONTROLLERS; i++) {
        gMultiplayer.controllers[i].player = 0xFF;  /* No player */
    }

    /* Detect connected controllers */
    mp_detect_controllers();

    /* Default to single player */
    gMultiplayer.mode = MP_MODE_SINGLE;
    gMultiplayer.min_players = 1;
    gMultiplayer.max_players = MP_MAX_PLAYERS;
    gMultiplayer.num_laps = 3;
    gMultiplayer.difficulty = 1;
}

/**
 * Reset multiplayer state
 */
void mp_reset(void) {
    s32 i;

    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        gMultiplayer.players[i].state = PLAYER_STATE_INACTIVE;
        gMultiplayer.players[i].active = 0;
        gMultiplayer.players[i].ready = 0;
        gMultiplayer.players[i].position = 0;
        gMultiplayer.players[i].lap = 0;
        gMultiplayer.players[i].race_time = 0;
    }

    gMultiplayer.in_race = 0;
    gMultiplayer.paused = 0;
    gMultiplayer.race_winner = -1;
}

/**
 * Shutdown multiplayer
 */
void mp_shutdown(void) {
    mp_end_session();
}

/* -------------------------------------------------------------------------- */
/* Mode Setup                                                                  */
/* -------------------------------------------------------------------------- */

/**
 * Start a multiplayer session
 */
s32 mp_start_session(s32 mode, s32 max_players) {
    if (mode < 0 || mode >= NUM_MP_MODES) {
        mode = MP_MODE_SINGLE;
    }

    mp_reset();

    gMultiplayer.mode = (u8)mode;
    gMultiplayer.max_players = (u8)max_players;
    gMultiplayer.active = 1;

    /* Set minimum players based on mode */
    switch (mode) {
        case MP_MODE_SINGLE:
            gMultiplayer.min_players = 1;
            gMultiplayer.max_players = 1;
            break;
        case MP_MODE_SPLIT_2H:
        case MP_MODE_SPLIT_2V:
            gMultiplayer.min_players = 2;
            gMultiplayer.max_players = 2;
            break;
        case MP_MODE_SPLIT_4:
        case MP_MODE_BATTLE:
        case MP_MODE_STUNT:
            gMultiplayer.min_players = 2;
            gMultiplayer.max_players = 4;
            break;
    }

    /* Auto-add player 1 if controller connected */
    if (gMultiplayer.controllers[0].connected) {
        mp_add_player(0);
    }

    return 1;
}

/**
 * End multiplayer session
 */
void mp_end_session(void) {
    s32 i;

    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        gMultiplayer.players[i].active = 0;
    }

    gMultiplayer.active = 0;
    gMultiplayer.num_players = 0;
    gMultiplayer.in_race = 0;
}

/**
 * Get current mode
 */
s32 mp_get_mode(void) {
    return gMultiplayer.mode;
}

/**
 * Set multiplayer mode
 */
void mp_set_mode(s32 mode) {
    if (mode >= 0 && mode < NUM_MP_MODES) {
        gMultiplayer.mode = (u8)mode;
        mp_setup_viewports();
    }
}

/* -------------------------------------------------------------------------- */
/* Player Management                                                           */
/* -------------------------------------------------------------------------- */

/**
 * Add a player to the session
 */
s32 mp_add_player(s32 controller) {
    s32 i;
    MPPlayer *player;

    if (controller < 0 || controller >= MP_MAX_CONTROLLERS) {
        return -1;
    }

    if (!gMultiplayer.controllers[controller].connected) {
        return -1;
    }

    /* Find free player slot */
    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        if (!gMultiplayer.players[i].active) {
            player = &gMultiplayer.players[i];

            player->active = 1;
            player->human = 1;
            player->player_index = (u8)i;
            player->controller = (s8)controller;
            player->car_index = (u8)i;
            player->state = PLAYER_STATE_WAITING;
            player->join_state = JOIN_STATE_JOINED;

            /* Assign controller to player */
            gMultiplayer.controllers[controller].player = (u8)i;

            gMultiplayer.num_players++;

            /* Update viewports */
            mp_setup_viewports();

            return i;
        }
    }

    return -1;  /* No free slots */
}

/**
 * Remove a player
 */
s32 mp_remove_player(s32 player) {
    MPPlayer *p;

    if (player < 0 || player >= MP_MAX_PLAYERS) {
        return 0;
    }

    p = &gMultiplayer.players[player];
    if (!p->active) {
        return 0;
    }

    /* Unassign controller */
    if (p->controller >= 0) {
        gMultiplayer.controllers[p->controller].player = 0xFF;
    }

    p->active = 0;
    p->human = 0;
    p->controller = -1;
    p->state = PLAYER_STATE_INACTIVE;

    gMultiplayer.num_players--;

    mp_setup_viewports();

    return 1;
}

/**
 * Get number of total players
 */
s32 mp_get_num_players(void) {
    return gMultiplayer.num_active;
}

/**
 * Get number of human players
 */
s32 mp_get_num_human_players(void) {
    return gMultiplayer.num_players;
}

/**
 * Get player by index
 */
MPPlayer* mp_get_player(s32 index) {
    if (index < 0 || index >= MP_MAX_PLAYERS) {
        return NULL;
    }
    return &gMultiplayer.players[index];
}

/**
 * Check if player is active
 */
s32 mp_is_player_active(s32 player) {
    if (player < 0 || player >= MP_MAX_PLAYERS) {
        return 0;
    }
    return gMultiplayer.players[player].active;
}

/**
 * Check if player is human
 */
s32 mp_is_player_human(s32 player) {
    if (player < 0 || player >= MP_MAX_PLAYERS) {
        return 0;
    }
    return gMultiplayer.players[player].human;
}

/* -------------------------------------------------------------------------- */
/* Controller Management                                                       */
/* -------------------------------------------------------------------------- */

/**
 * Detect connected controllers
 */
void mp_detect_controllers(void) {
    s32 i;

    for (i = 0; i < MP_MAX_CONTROLLERS; i++) {
        gMultiplayer.controllers[i].connected = input_is_connected(i) ? 1 : 0;
    }
}

/**
 * Get controller assigned to player
 */
s32 mp_get_controller_for_player(s32 player) {
    if (player < 0 || player >= MP_MAX_PLAYERS) {
        return -1;
    }
    return gMultiplayer.players[player].controller;
}

/**
 * Get player using a controller
 */
s32 mp_get_player_for_controller(s32 controller) {
    if (controller < 0 || controller >= MP_MAX_CONTROLLERS) {
        return -1;
    }
    if (gMultiplayer.controllers[controller].player == 0xFF) {
        return -1;
    }
    return gMultiplayer.controllers[controller].player;
}

/**
 * Assign controller to player
 */
s32 mp_assign_controller(s32 player, s32 controller) {
    if (player < 0 || player >= MP_MAX_PLAYERS) {
        return 0;
    }
    if (controller < 0 || controller >= MP_MAX_CONTROLLERS) {
        return 0;
    }

    /* Remove any existing assignment */
    mp_unassign_controller(controller);

    gMultiplayer.players[player].controller = (s8)controller;
    gMultiplayer.controllers[controller].player = (u8)player;

    return 1;
}

/**
 * Unassign a controller
 */
void mp_unassign_controller(s32 controller) {
    s32 player;

    if (controller < 0 || controller >= MP_MAX_CONTROLLERS) {
        return;
    }

    player = gMultiplayer.controllers[controller].player;
    if (player != 0xFF && player < MP_MAX_PLAYERS) {
        gMultiplayer.players[player].controller = -1;
    }

    gMultiplayer.controllers[controller].player = 0xFF;
}

/**
 * Check if controller is connected
 */
s32 mp_is_controller_connected(s32 controller) {
    if (controller < 0 || controller >= MP_MAX_CONTROLLERS) {
        return 0;
    }
    return gMultiplayer.controllers[controller].connected;
}

/* -------------------------------------------------------------------------- */
/* Join Screen                                                                 */
/* -------------------------------------------------------------------------- */

/**
 * Start join screen
 */
void mp_start_join_screen(void) {
    gMultiplayer.in_join_screen = 1;
    gMultiplayer.join_timer = 0;
    gMultiplayer.join_countdown = 0;

    /* Detect controllers again */
    mp_detect_controllers();
}

/**
 * Update join screen
 */
void mp_update_join_screen(void) {
    s32 i;

    if (!gMultiplayer.in_join_screen) {
        return;
    }

    gMultiplayer.join_timer++;

    /* Check for new players pressing start */
    for (i = 0; i < MP_MAX_CONTROLLERS; i++) {
        if (!gMultiplayer.controllers[i].connected) {
            continue;
        }

        if (gMultiplayer.controllers[i].player != 0xFF) {
            continue;  /* Already assigned */
        }

        if (gMultiplayer.num_players >= gMultiplayer.max_players) {
            continue;  /* Full */
        }

        /* Check for start button press */
        if (input_get_button_press(i, BUTTON_START)) {
            mp_add_player(i);
        }
    }

    /* Start countdown when enough players ready */
    if (mp_all_players_ready() && gMultiplayer.num_players >= gMultiplayer.min_players) {
        gMultiplayer.join_countdown++;
        if (gMultiplayer.join_countdown >= 180) {  /* 3 seconds at 60fps */
            gMultiplayer.in_join_screen = 0;
        }
    } else {
        gMultiplayer.join_countdown = 0;
    }
}

/**
 * Draw join screen
 */
void mp_draw_join_screen(void) {
    /* Would draw join screen UI */
}

/**
 * Check if all players are ready
 */
s32 mp_all_players_ready(void) {
    s32 i;

    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        if (gMultiplayer.players[i].active && !gMultiplayer.players[i].ready) {
            return 0;
        }
    }

    return (gMultiplayer.num_players > 0);
}

/**
 * Set player ready state
 */
void mp_set_player_ready(s32 player, s32 ready) {
    if (player >= 0 && player < MP_MAX_PLAYERS) {
        gMultiplayer.players[player].ready = ready ? 1 : 0;
    }
}

/* -------------------------------------------------------------------------- */
/* Viewport Management                                                         */
/* -------------------------------------------------------------------------- */

/**
 * Setup viewports based on current mode and player count
 */
void mp_setup_viewports(void) {
    s32 i;
    s32 mode;
    const s16 *layout;
    MPPlayer *player;

    mode = gMultiplayer.mode;

    /* Adjust mode based on actual player count */
    if (gMultiplayer.num_players == 1) {
        mode = MP_MODE_SINGLE;
    } else if (gMultiplayer.num_players == 2) {
        if (mode != MP_MODE_SPLIT_2V) {
            mode = MP_MODE_SPLIT_2H;
        }
    } else if (gMultiplayer.num_players >= 3) {
        mode = MP_MODE_SPLIT_4;
    }

    gMultiplayer.num_viewports = gMultiplayer.num_players;
    if (gMultiplayer.num_viewports > MP_MAX_PLAYERS) {
        gMultiplayer.num_viewports = MP_MAX_PLAYERS;
    }

    /* Apply layouts */
    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        player = &gMultiplayer.players[i];
        layout = sViewportLayouts[mode][i];

        if (player->active && player->human) {
            player->viewport.x = layout[0];
            player->viewport.y = layout[1];
            player->viewport.width = layout[2];
            player->viewport.height = layout[3];
            player->viewport.active = 1;
            player->viewport.player = (u8)i;

            /* Calculate aspect ratio */
            if (player->viewport.height > 0) {
                player->viewport.aspect = (f32)player->viewport.width /
                                          (f32)player->viewport.height;
            } else {
                player->viewport.aspect = 1.33f;
            }
        } else {
            player->viewport.active = 0;
        }
    }
}

/**
 * Get viewport for player
 */
Viewport* mp_get_viewport(s32 player) {
    if (player < 0 || player >= MP_MAX_PLAYERS) {
        return NULL;
    }
    return &gMultiplayer.players[player].viewport;
}

/**
 * Set current viewport to player's viewport
 */
void mp_set_viewport(s32 player) {
    Viewport *vp;

    vp = mp_get_viewport(player);
    if (vp == NULL || !vp->active) {
        return;
    }

    gfx_set_viewport(vp->x, vp->y, vp->width, vp->height);
    gfx_set_scissor(vp->x, vp->y, vp->width, vp->height);
}

/**
 * Get viewport rectangle
 */
void mp_get_viewport_rect(s32 player, s16 *x, s16 *y, s16 *w, s16 *h) {
    Viewport *vp;

    vp = mp_get_viewport(player);
    if (vp == NULL) {
        *x = 0;
        *y = 0;
        *w = SCREEN_WIDTH;
        *h = SCREEN_HEIGHT;
        return;
    }

    *x = vp->x;
    *y = vp->y;
    *w = vp->width;
    *h = vp->height;
}

/**
 * Apply viewport for rendering
 */
void mp_apply_viewport(s32 player) {
    mp_set_viewport(player);
}

/**
 * Reset to full screen viewport
 */
void mp_reset_viewport(void) {
    gfx_set_viewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gfx_set_scissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

/* -------------------------------------------------------------------------- */
/* Race Management                                                             */
/* -------------------------------------------------------------------------- */

/**
 * Start race
 */
void mp_start_race(void) {
    s32 i;
    MPPlayer *player;

    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        player = &gMultiplayer.players[i];
        if (player->active) {
            player->state = PLAYER_STATE_RACING;
            player->position = i + 1;
            player->lap = 1;
            player->race_time = 0;
            player->checkpoint = 0;
        }
    }

    gMultiplayer.in_race = 1;
    gMultiplayer.race_winner = -1;
}

/**
 * End race
 */
void mp_end_race(void) {
    gMultiplayer.in_race = 0;
    mp_calculate_results();
}

/**
 * Update race state
 */
void mp_update_race(void) {
    s32 i;

    if (!gMultiplayer.in_race) {
        return;
    }

    /* Update race time for active players */
    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        if (gMultiplayer.players[i].active &&
            gMultiplayer.players[i].state == PLAYER_STATE_RACING) {
            gMultiplayer.players[i].race_time++;
        }
    }

    /* Update positions */
    mp_update_positions();

    /* Check if all finished */
    if (mp_all_finished()) {
        mp_end_race();
    }
}

/**
 * Update race positions
 */
void mp_update_positions(void) {
    s32 i, j;
    s32 positions[MP_MAX_PLAYERS];
    MPPlayer *p1, *p2;

    /* Initialize positions */
    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        positions[i] = i;
    }

    /* Sort by lap and checkpoint (bubble sort) */
    for (i = 0; i < MP_MAX_PLAYERS - 1; i++) {
        for (j = 0; j < MP_MAX_PLAYERS - 1 - i; j++) {
            p1 = &gMultiplayer.players[positions[j]];
            p2 = &gMultiplayer.players[positions[j + 1]];

            if (!p1->active) continue;
            if (!p2->active) continue;

            /* Compare lap first, then checkpoint */
            if (p2->lap > p1->lap ||
                (p2->lap == p1->lap && p2->checkpoint > p1->checkpoint)) {
                /* Swap */
                s32 temp = positions[j];
                positions[j] = positions[j + 1];
                positions[j + 1] = temp;
            }
        }
    }

    /* Assign positions */
    j = 1;
    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        if (gMultiplayer.players[positions[i]].active) {
            gMultiplayer.players[positions[i]].position = j++;
        }
    }
}

/**
 * Get race position
 */
s32 mp_get_race_position(s32 player) {
    if (player < 0 || player >= MP_MAX_PLAYERS) {
        return 0;
    }
    return gMultiplayer.players[player].position;
}

/**
 * Set race position
 */
void mp_set_race_position(s32 player, s32 position) {
    if (player >= 0 && player < MP_MAX_PLAYERS) {
        gMultiplayer.players[player].position = position;
    }
}

/**
 * Mark player as finished
 */
void mp_player_finished(s32 player, u32 time) {
    MPPlayer *p;

    if (player < 0 || player >= MP_MAX_PLAYERS) {
        return;
    }

    p = &gMultiplayer.players[player];
    p->state = PLAYER_STATE_FINISHED;
    p->race_time = time;

    /* First to finish is winner */
    if (gMultiplayer.race_winner < 0) {
        gMultiplayer.race_winner = player;
    }
}

/**
 * Check if all players finished
 */
s32 mp_all_finished(void) {
    s32 i;

    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        if (gMultiplayer.players[i].active &&
            gMultiplayer.players[i].state == PLAYER_STATE_RACING) {
            return 0;
        }
    }

    return 1;
}

/* -------------------------------------------------------------------------- */
/* Pause Handling                                                              */
/* -------------------------------------------------------------------------- */

static s32 sPausePlayer = -1;

void mp_pause(s32 player) {
    if (!gMultiplayer.paused) {
        gMultiplayer.paused = 1;
        sPausePlayer = player;
    }
}

void mp_unpause(void) {
    gMultiplayer.paused = 0;
    sPausePlayer = -1;
}

s32 mp_is_paused(void) {
    return gMultiplayer.paused;
}

s32 mp_who_paused(void) {
    return sPausePlayer;
}

/* -------------------------------------------------------------------------- */
/* Results                                                                     */
/* -------------------------------------------------------------------------- */

/**
 * Calculate final results
 */
void mp_calculate_results(void) {
    mp_update_positions();
    mp_award_points();

    gMultiplayer.races_completed++;
}

/**
 * Get race winner
 */
s32 mp_get_winner(void) {
    return gMultiplayer.race_winner;
}

/**
 * Award points based on positions
 */
void mp_award_points(void) {
    s32 i;
    MPPlayer *player;

    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        player = &gMultiplayer.players[i];
        if (player->active && player->position > 0 && player->position <= 8) {
            player->points += sPositionPoints[player->position - 1];
        }
    }

    /* Update session leader */
    mp_get_session_leader();
}

/**
 * Get player's points
 */
s32 mp_get_points(s32 player) {
    if (player < 0 || player >= MP_MAX_PLAYERS) {
        return 0;
    }
    return gMultiplayer.players[player].points;
}

/**
 * Get session leader
 */
s32 mp_get_session_leader(void) {
    s32 i;
    s32 best = -1;
    s32 best_points = -1;

    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        if (gMultiplayer.players[i].active &&
            gMultiplayer.players[i].points > best_points) {
            best_points = gMultiplayer.players[i].points;
            best = i;
        }
    }

    gMultiplayer.session_leader = best;
    return best;
}

/* -------------------------------------------------------------------------- */
/* Car Selection                                                               */
/* -------------------------------------------------------------------------- */

void mp_set_car_choice(s32 player, s32 car_type, s32 color) {
    if (player >= 0 && player < MP_MAX_PLAYERS) {
        gMultiplayer.players[player].car_type = (u8)car_type;
        gMultiplayer.players[player].car_color = (u8)color;
    }
}

void mp_set_transmission(s32 player, s32 auto_trans) {
    if (player >= 0 && player < MP_MAX_PLAYERS) {
        gMultiplayer.players[player].transmission = auto_trans ? 1 : 0;
    }
}

s32 mp_get_car_choice(s32 player) {
    if (player < 0 || player >= MP_MAX_PLAYERS) {
        return 0;
    }
    return gMultiplayer.players[player].car_type;
}

s32 mp_get_car_color(s32 player) {
    if (player < 0 || player >= MP_MAX_PLAYERS) {
        return 0;
    }
    return gMultiplayer.players[player].car_color;
}

/* -------------------------------------------------------------------------- */
/* Queries                                                                     */
/* -------------------------------------------------------------------------- */

s32 mp_is_active(void) {
    return gMultiplayer.active;
}

s32 mp_is_split_screen(void) {
    return (gMultiplayer.num_players > 1);
}

s32 mp_get_split_type(void) {
    return gMultiplayer.mode;
}

s32 mp_in_race(void) {
    return gMultiplayer.in_race;
}

/* -------------------------------------------------------------------------- */
/* AI Opponents                                                                */
/* -------------------------------------------------------------------------- */

void mp_add_ai_opponents(s32 count, s32 difficulty) {
    s32 i;
    s32 added = 0;
    MPPlayer *player;

    gMultiplayer.difficulty = difficulty;
    gMultiplayer.num_ai_cars = count;

    /* Add AI players to remaining slots */
    for (i = 0; i < MP_MAX_CARS && added < count; i++) {
        if (i < MP_MAX_PLAYERS && gMultiplayer.players[i].active) {
            continue;  /* Slot taken by human */
        }

        if (i < MP_MAX_PLAYERS) {
            player = &gMultiplayer.players[i];
            player->active = 1;
            player->human = 0;
            player->controller = -1;
            player->state = PLAYER_STATE_RACING;
            player->car_index = (u8)(gMultiplayer.num_players + added);
            added++;
        }
    }

    gMultiplayer.num_active = gMultiplayer.num_players + added;
}

void mp_remove_ai_opponents(void) {
    s32 i;

    for (i = 0; i < MP_MAX_PLAYERS; i++) {
        if (gMultiplayer.players[i].active && !gMultiplayer.players[i].human) {
            gMultiplayer.players[i].active = 0;
        }
    }

    gMultiplayer.num_ai_cars = 0;
    gMultiplayer.num_active = gMultiplayer.num_players;
}
