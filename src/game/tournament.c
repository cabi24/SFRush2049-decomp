/**
 * tournament.c - Tournament/Championship mode for Rush 2049 N64
 *
 * Manages multi-race championships with standings and progression.
 */

#include "game/tournament.h"

/* Global tournament state */
TournamentState gTournament;

/* Points tables */
static const s32 sPointsStandard[8] = { 10, 8, 6, 5, 4, 3, 2, 1 };
static const s32 sPointsF1[10] = { 25, 18, 15, 12, 10, 8, 6, 4, 2, 1 };
static const s32 sPointsLinear[8] = { 8, 7, 6, 5, 4, 3, 2, 1 };
static const s32 sPointsWTA[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };

/* Pre-defined circuits */
const CircuitDef gCircuits[NUM_CIRCUITS] = {
    /* Bronze Circuit - Easy */
    {
        "Bronze Circuit",
        { 0, 1, 2, 3 },     /* Track IDs */
        4,                   /* Number of races */
        DIFFICULTY_EASY,
        UNLOCK_CAR_5         /* Unlocks car 5 */
    },
    /* Silver Circuit - Medium */
    {
        "Silver Circuit",
        { 1, 2, 3, 4, 5 },
        5,
        DIFFICULTY_MEDIUM,
        UNLOCK_CAR_6 | UNLOCK_TRACK_5
    },
    /* Gold Circuit - Hard */
    {
        "Gold Circuit",
        { 2, 3, 4, 5, 6, 7 },
        6,
        DIFFICULTY_HARD,
        UNLOCK_CAR_7 | UNLOCK_TRACK_6 | UNLOCK_TRACK_7
    },
    /* Platinum Circuit - Expert */
    {
        "Platinum Circuit",
        { 0, 1, 2, 3, 4, 5, 6, 7 },
        8,
        DIFFICULTY_EXPERT,
        UNLOCK_CAR_8 | UNLOCK_MIRROR_MODE
    }
};

/* Default player names */
static const char *sDefaultNames[8] = {
    "Player 1", "Player 2", "Player 3", "Player 4",
    "CPU 1", "CPU 2", "CPU 3", "CPU 4"
};

/* Unlock flags */
#define UNLOCK_CAR_5            (1 << 4)
#define UNLOCK_CAR_6            (1 << 5)
#define UNLOCK_CAR_7            (1 << 6)
#define UNLOCK_CAR_8            (1 << 7)
#define UNLOCK_TRACK_5          (1 << 12)
#define UNLOCK_TRACK_6          (1 << 13)
#define UNLOCK_TRACK_7          (1 << 14)
#define UNLOCK_MIRROR_MODE      (1 << 20)

/* -------------------------------------------------------------------------- */
/* Initialization                                                              */
/* -------------------------------------------------------------------------- */

/**
 * Initialize tournament system
 */
void tournament_init(void) {
    s32 i, j;

    /* Clear all state */
    for (i = 0; i < (s32)sizeof(TournamentState); i++) {
        ((u8*)&gTournament)[i] = 0;
    }

    gTournament.state = TOURNEY_STATE_INACTIVE;
    gTournament.points_system = POINTS_STANDARD;
    gTournament.ai_difficulty = 1;

    /* Initialize players */
    for (i = 0; i < MAX_TOURNAMENT_PLAYERS; i++) {
        for (j = 0; sDefaultNames[i][j] && j < MAX_TOURNAMENT_NAME - 1; j++) {
            gTournament.players[i].name[j] = sDefaultNames[i][j];
        }
        gTournament.players[i].name[j] = '\0';
        gTournament.players[i].player_index = (u8)i;
    }
}

/**
 * Reset tournament
 */
void tournament_reset(void) {
    tournament_init();
}

/* -------------------------------------------------------------------------- */
/* Setup                                                                       */
/* -------------------------------------------------------------------------- */

/**
 * Start a tournament of given type
 */
s32 tournament_start(s32 type) {
    if (type < 0 || type >= NUM_TOURNAMENT_TYPES) {
        return 0;
    }

    tournament_reset();

    gTournament.type = (u8)type;
    gTournament.state = TOURNEY_STATE_SETUP;
    gTournament.current_race = 0;

    /* Set default races based on type */
    switch (type) {
        case TOURNAMENT_SINGLE_RACE:
            gTournament.num_races = 1;
            break;
        case TOURNAMENT_CIRCUIT:
            gTournament.num_races = 4;
            break;
        case TOURNAMENT_ELIMINATOR:
            gTournament.num_races = 4;
            gTournament.elimination = 1;
            gTournament.elim_mode = ELIM_LAST_PLACE;
            gTournament.elim_count = 1;
            break;
        case TOURNAMENT_TIME_ATTACK:
            gTournament.num_races = 4;
            break;
        case TOURNAMENT_BATTLE_CUP:
            gTournament.num_races = 3;
            break;
        case TOURNAMENT_STUNT_CUP:
            gTournament.num_races = 3;
            break;
    }

    return 1;
}

/**
 * Start a pre-defined circuit
 */
s32 tournament_start_circuit(s32 circuit) {
    s32 i;
    const CircuitDef *def;

    if (circuit < 0 || circuit >= NUM_CIRCUITS) {
        return 0;
    }

    tournament_start(TOURNAMENT_CIRCUIT);

    def = &gCircuits[circuit];
    gTournament.circuit = (u8)circuit;
    gTournament.num_races = def->num_races;
    gTournament.ai_difficulty = def->difficulty;

    /* Set up races */
    for (i = 0; i < def->num_races; i++) {
        gTournament.races[i].track_id = def->races[i];
        gTournament.races[i].laps = 3;
        gTournament.races[i].completed = 0;
        gTournament.races[i].current = (i == 0) ? 1 : 0;
    }

    gTournament.state = TOURNEY_STATE_PRE_RACE;

    return 1;
}

/**
 * End tournament normally
 */
void tournament_end(void) {
    tournament_check_unlocks();
    gTournament.state = TOURNEY_STATE_COMPLETE;
}

/**
 * Abandon tournament
 */
void tournament_abandon(void) {
    gTournament.state = TOURNEY_STATE_ABANDONED;
}

/* -------------------------------------------------------------------------- */
/* Player Management                                                           */
/* -------------------------------------------------------------------------- */

/**
 * Add player to tournament
 */
s32 tournament_add_player(s32 player_index, s32 human) {
    TourneyPlayer *player;

    if (player_index < 0 || player_index >= MAX_TOURNAMENT_PLAYERS) {
        return 0;
    }

    player = &gTournament.players[player_index];
    player->active = 1;
    player->human = human ? 1 : 0;
    player->total_points = 0;
    player->current_position = gTournament.num_players + 1;
    player->wins = 0;

    gTournament.num_players++;
    if (human) {
        gTournament.num_humans++;
    }
    gTournament.num_remaining = gTournament.num_players;

    return 1;
}

/**
 * Remove player from tournament
 */
s32 tournament_remove_player(s32 player_index) {
    TourneyPlayer *player;

    if (player_index < 0 || player_index >= MAX_TOURNAMENT_PLAYERS) {
        return 0;
    }

    player = &gTournament.players[player_index];
    if (!player->active) {
        return 0;
    }

    if (player->human) {
        gTournament.num_humans--;
    }
    gTournament.num_players--;
    player->active = 0;

    return 1;
}

/**
 * Get number of active players
 */
s32 tournament_get_player_count(void) {
    return gTournament.num_players;
}

/**
 * Get player by index
 */
TourneyPlayer* tournament_get_player(s32 index) {
    if (index < 0 || index >= MAX_TOURNAMENT_PLAYERS) {
        return NULL;
    }
    return &gTournament.players[index];
}

/**
 * Get player by current standing position
 */
TourneyPlayer* tournament_get_player_by_position(s32 position) {
    s32 i;

    for (i = 0; i < MAX_TOURNAMENT_PLAYERS; i++) {
        if (gTournament.players[i].active &&
            gTournament.players[i].current_position == position) {
            return &gTournament.players[i];
        }
    }

    return NULL;
}

/* -------------------------------------------------------------------------- */
/* Race Progression                                                            */
/* -------------------------------------------------------------------------- */

/**
 * Advance to next race
 */
s32 tournament_next_race(void) {
    if (gTournament.current_race >= gTournament.num_races - 1) {
        tournament_end();
        return 0;
    }

    gTournament.races[gTournament.current_race].completed = 1;
    gTournament.races[gTournament.current_race].current = 0;
    gTournament.current_race++;
    gTournament.races[gTournament.current_race].current = 1;
    gTournament.races_completed++;

    gTournament.state = TOURNEY_STATE_PRE_RACE;

    return 1;
}

/**
 * Get current race index
 */
s32 tournament_get_current_race(void) {
    return gTournament.current_race;
}

/**
 * Get track ID for current race
 */
s32 tournament_get_current_track(void) {
    if (gTournament.current_race >= gTournament.num_races) {
        return 0;
    }
    return gTournament.races[gTournament.current_race].track_id;
}

/**
 * Get number of races remaining
 */
s32 tournament_get_races_remaining(void) {
    return gTournament.num_races - gTournament.races_completed;
}

/**
 * Get race info
 */
TourneyRace* tournament_get_race(s32 index) {
    if (index < 0 || index >= MAX_TOURNAMENT_RACES) {
        return NULL;
    }
    return &gTournament.races[index];
}

/* -------------------------------------------------------------------------- */
/* Results                                                                     */
/* -------------------------------------------------------------------------- */

/**
 * Submit race result for a player
 */
void tournament_submit_result(s32 player, s32 position, u32 time, u32 best_lap) {
    TourneyPlayer *p;
    TourneyRaceResult *result;
    s32 race;

    if (player < 0 || player >= MAX_TOURNAMENT_PLAYERS) {
        return;
    }

    p = &gTournament.players[player];
    if (!p->active) {
        return;
    }

    race = gTournament.current_race;
    result = &p->races[race];

    result->position = position;
    result->time = time;
    result->best_lap = best_lap;
    result->finished = 1;
    result->dnf = 0;

    /* Update overall stats */
    p->total_time += time;
    if (best_lap < p->best_lap_overall || p->best_lap_overall == 0) {
        p->best_lap_overall = best_lap;
    }
    if (position < p->best_position || p->best_position == 0) {
        p->best_position = position;
    }
    if (position == 1) {
        p->wins++;
    }
}

/**
 * Calculate points for current race
 */
void tournament_calculate_points(void) {
    s32 i;
    TourneyPlayer *p;
    TourneyRaceResult *result;
    s32 race;
    s32 points;

    race = gTournament.current_race;

    for (i = 0; i < MAX_TOURNAMENT_PLAYERS; i++) {
        p = &gTournament.players[i];
        if (!p->active) {
            continue;
        }

        result = &p->races[race];
        if (!result->finished) {
            continue;
        }

        points = tournament_get_points_for_position(result->position);
        result->points = points;
        p->total_points += points;
    }

    tournament_update_standings();
}

/**
 * Update standings based on total points
 */
void tournament_update_standings(void) {
    tournament_sort_standings();
}

/**
 * Check for eliminations
 */
void tournament_check_elimination(void) {
    s32 i;
    TourneyPlayer *p;
    s32 eliminated = 0;

    if (!gTournament.elimination) {
        return;
    }

    /* Sort standings first */
    tournament_sort_standings();

    /* Eliminate based on mode */
    for (i = 0; i < MAX_TOURNAMENT_PLAYERS && eliminated < gTournament.elim_count; i++) {
        p = tournament_get_player_by_position(gTournament.num_remaining - eliminated);
        if (p != NULL && p->active && !p->races[gTournament.current_race].eliminated) {
            switch (gTournament.elim_mode) {
                case ELIM_LAST_PLACE:
                    tournament_eliminate_player(p->player_index);
                    eliminated++;
                    break;

                case ELIM_BOTTOM_HALF:
                    if (p->current_position > gTournament.num_remaining / 2) {
                        tournament_eliminate_player(p->player_index);
                        eliminated++;
                    }
                    break;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Points                                                                      */
/* -------------------------------------------------------------------------- */

/**
 * Get player's total points
 */
s32 tournament_get_points(s32 player) {
    if (player < 0 || player >= MAX_TOURNAMENT_PLAYERS) {
        return 0;
    }
    return gTournament.players[player].total_points;
}

/**
 * Get player's current position in standings
 */
s32 tournament_get_position(s32 player) {
    if (player < 0 || player >= MAX_TOURNAMENT_PLAYERS) {
        return 0;
    }
    return gTournament.players[player].current_position;
}

/**
 * Get points awarded for a position
 */
s32 tournament_get_points_for_position(s32 position) {
    const s32 *table;
    s32 max_positions;

    if (position < 1) {
        return 0;
    }

    switch (gTournament.points_system) {
        case POINTS_F1:
            table = sPointsF1;
            max_positions = 10;
            break;
        case POINTS_LINEAR:
            table = sPointsLinear;
            max_positions = 8;
            break;
        case POINTS_WINNER_TAKES_ALL:
            table = sPointsWTA;
            max_positions = 8;
            break;
        default:
            table = sPointsStandard;
            max_positions = 8;
            break;
    }

    if (position <= max_positions) {
        return table[position - 1];
    }

    return 0;
}

/**
 * Set points system
 */
void tournament_set_points_system(s32 system) {
    if (system >= 0 && system < 4) {
        gTournament.points_system = (u8)system;
    }
}

/* -------------------------------------------------------------------------- */
/* Elimination                                                                 */
/* -------------------------------------------------------------------------- */

/**
 * Configure elimination mode
 */
void tournament_set_elimination(s32 enabled, s32 mode, s32 count) {
    gTournament.elimination = enabled ? 1 : 0;
    gTournament.elim_mode = (u8)mode;
    gTournament.elim_count = (u8)count;
}

/**
 * Check if player is eliminated
 */
s32 tournament_is_eliminated(s32 player) {
    s32 i;
    TourneyPlayer *p;

    if (player < 0 || player >= MAX_TOURNAMENT_PLAYERS) {
        return 0;
    }

    p = &gTournament.players[player];
    for (i = 0; i < gTournament.races_completed; i++) {
        if (p->races[i].eliminated) {
            return 1;
        }
    }

    return 0;
}

/**
 * Eliminate a player
 */
void tournament_eliminate_player(s32 player) {
    TourneyPlayer *p;

    if (player < 0 || player >= MAX_TOURNAMENT_PLAYERS) {
        return;
    }

    p = &gTournament.players[player];
    if (!p->active) {
        return;
    }

    p->races[gTournament.current_race].eliminated = 1;
    p->active = 0;
    gTournament.num_remaining--;
}

/* -------------------------------------------------------------------------- */
/* Queries                                                                     */
/* -------------------------------------------------------------------------- */

/**
 * Check if tournament is active
 */
s32 tournament_is_active(void) {
    return (gTournament.state != TOURNEY_STATE_INACTIVE &&
            gTournament.state != TOURNEY_STATE_COMPLETE &&
            gTournament.state != TOURNEY_STATE_ABANDONED);
}

/**
 * Check if this is the final race
 */
s32 tournament_is_final_race(void) {
    return (gTournament.current_race >= gTournament.num_races - 1);
}

/**
 * Check if tournament is complete
 */
s32 tournament_is_complete(void) {
    return (gTournament.state == TOURNEY_STATE_COMPLETE);
}

/**
 * Get current leader
 */
s32 tournament_get_leader(void) {
    return tournament_get_player_by_position(1) ?
           tournament_get_player_by_position(1)->player_index : -1;
}

/**
 * Get final winner
 */
s32 tournament_get_winner(void) {
    if (gTournament.state != TOURNEY_STATE_COMPLETE) {
        return -1;
    }
    return tournament_get_leader();
}

/* -------------------------------------------------------------------------- */
/* Standings                                                                   */
/* -------------------------------------------------------------------------- */

/**
 * Sort standings by points (descending)
 */
void tournament_sort_standings(void) {
    s32 i, j;
    s32 positions[MAX_TOURNAMENT_PLAYERS];
    TourneyPlayer *p1, *p2;
    s32 temp;

    /* Initialize position array */
    for (i = 0; i < MAX_TOURNAMENT_PLAYERS; i++) {
        positions[i] = i;
    }

    /* Bubble sort by points */
    for (i = 0; i < MAX_TOURNAMENT_PLAYERS - 1; i++) {
        for (j = 0; j < MAX_TOURNAMENT_PLAYERS - 1 - i; j++) {
            p1 = &gTournament.players[positions[j]];
            p2 = &gTournament.players[positions[j + 1]];

            /* Inactive players go to end */
            if (!p1->active && p2->active) {
                temp = positions[j];
                positions[j] = positions[j + 1];
                positions[j + 1] = temp;
            } else if (p1->active && p2->active) {
                /* Compare points (higher is better) */
                if (p2->total_points > p1->total_points) {
                    temp = positions[j];
                    positions[j] = positions[j + 1];
                    positions[j + 1] = temp;
                } else if (p2->total_points == p1->total_points) {
                    /* Tiebreaker: more wins */
                    if (p2->wins > p1->wins) {
                        temp = positions[j];
                        positions[j] = positions[j + 1];
                        positions[j + 1] = temp;
                    }
                }
            }
        }
    }

    /* Assign positions */
    for (i = 0; i < MAX_TOURNAMENT_PLAYERS; i++) {
        gTournament.players[positions[i]].current_position = i + 1;
    }
}

/**
 * Draw standings table
 */
void tournament_draw_standings(void) {
    /* Would draw standings UI */
}

/**
 * Draw race results
 */
void tournament_draw_race_results(void) {
    /* Would draw race results UI */
}

/**
 * Draw final results
 */
void tournament_draw_final_results(void) {
    /* Would draw final results UI with winner celebration */
}

/* -------------------------------------------------------------------------- */
/* Unlocks                                                                     */
/* -------------------------------------------------------------------------- */

/**
 * Check for unlocks based on results
 */
void tournament_check_unlocks(void) {
    s32 winner;
    const CircuitDef *circuit;

    if (gTournament.type != TOURNAMENT_CIRCUIT) {
        return;
    }

    /* Check if human player won */
    winner = tournament_get_winner();
    if (winner < 0 || !gTournament.players[winner].human) {
        return;
    }

    /* Award circuit unlocks */
    circuit = &gCircuits[gTournament.circuit];
    gTournament.unlocks_earned |= circuit->unlock_flags;
    gTournament.new_unlocks = 1;
}

/**
 * Get earned unlocks
 */
u32 tournament_get_unlocks(void) {
    return gTournament.unlocks_earned;
}

/**
 * Check if there are new unlocks
 */
s32 tournament_has_new_unlocks(void) {
    return gTournament.new_unlocks;
}

/**
 * Clear new unlocks flag
 */
void tournament_clear_new_unlocks(void) {
    gTournament.new_unlocks = 0;
}

/* -------------------------------------------------------------------------- */
/* Save/Load                                                                   */
/* -------------------------------------------------------------------------- */

/**
 * Save tournament progress
 */
s32 tournament_save_progress(void) {
    /* Would save to Controller Pak */
    return 1;
}

/**
 * Load tournament progress
 */
s32 tournament_load_progress(void) {
    /* Would load from Controller Pak */
    return 0;
}

/**
 * Check if can continue saved tournament
 */
s32 tournament_can_continue(void) {
    return 0;  /* Would check saved data */
}

/* -------------------------------------------------------------------------- */
/* State Queries                                                               */
/* -------------------------------------------------------------------------- */

s32 tournament_get_state(void) {
    return gTournament.state;
}

s32 tournament_get_type(void) {
    return gTournament.type;
}

s32 tournament_get_circuit(void) {
    return gTournament.circuit;
}

/* ========================================================================== */
/* Arcade Tourney Functions (from rushtherock/game/tourney.c)                 */
/* ========================================================================== */

/* Global arcade tourney state */
ArcadeTourney gArcadeTourney;
s32 gTourneyLaps[TOURNEY_MAX_TRACKS];
s8 gTourneyInTourney[TOURNEY_MAX_LINKS];

/* Menu state for arcade tourney setup */
static s8 sTourneyCurMenu;
static s8 sTourneyCurSelect[TOURNEY_NUM_MENUS];

/* Number of choices per menu item */
static const s8 sTourneyNumMenuChoices[TOURNEY_NUM_MENUS] = {
    2,  /* COIN: Normal, Free */
    8,  /* CAB: 8 cabinets */
    3,  /* JOIN: Any, Remote, Quick */
    4,  /* TSEL: T1-T3 + Remote */
    9,  /* CSEL: Any + 8 cars */
    3,  /* TRANS: Any, Manual, Auto */
    5,  /* LEN1: Norm, -5, -1, +1, +5 */
    5,  /* LEN2: Norm, -5, -1, +1, +5 */
    5,  /* LEN3: Norm, -5, -1, +1, +5 */
    3,  /* DRONE: Norm, Off, On */
    3,  /* SPEED: Norm, Off, On */
    3   /* EXIT: Cancel, Restore, Save */
};

#ifdef NON_MATCHING

/**
 * Check if tourney data is valid
 * Arcade: BadTourneyData (tourney.c:236)
 */
s32 tourney_bad_data(ArcadeTourney *t) {
    if (t->sanity != TOURNEY_SANITY_VALUE) {
        return 1;
    }
    if (t->ext_start > 2 || t->ext_start < 0) {
        return 1;
    }
    if (t->cabinets >= (1 << TOURNEY_MAX_LINKS)) {
        return 1;
    }
    if (t->laps > TOURNEY_MAX_LAPS || t->laps < 0) {
        return 1;
    }
    if (t->track >= TOURNEY_MAX_TRACKS || t->track < -1) {
        return 1;
    }
    if (t->car >= TOURNEY_MAX_CARS || t->car < -1) {
        return 1;
    }
    if (t->tranny > 1 || t->tranny < -1) {
        return 1;
    }
    if (t->drones > 1 || t->drones < -1) {
        return 1;
    }
    if (t->catchup > 1 || t->catchup < -1) {
        return 1;
    }
    return 0;
}

/**
 * Initialize arcade tourney system
 * Arcade: InitTourney (tourney.c:223)
 */
void tourney_init_arcade(void) {
    sTourneyCurMenu = 0;
    /* TourneyBlit = nil in arcade */
    tourney_load_settings();
}

/**
 * Load tourney settings from NVRAM
 * Arcade: LoadTourneySettings (tourney.c:269)
 */
void tourney_load_settings(void) {
    s32 i;

    /* Check for bad data and reset to defaults */
    if (tourney_bad_data(&gArcadeTourney)) {
        gArcadeTourney.free = 0;        /* Pay each time */
        gArcadeTourney.ext_start = 0;   /* Player select */
        gArcadeTourney.track = -1;      /* Any track */
        gArcadeTourney.car = -1;        /* Any car */
        gArcadeTourney.tranny = -1;     /* Any transmission */
        gArcadeTourney.laps = 0;        /* Normal laps */
        gArcadeTourney.drones = -1;     /* Normal drones */
        gArcadeTourney.catchup = -1;    /* Normal catchup */
        gArcadeTourney.cabinets = 0;    /* No cabinets */
        tourney_save_settings();
    }

    /* Set up menu with current values */
    sTourneyCurSelect[TOURNEY_MENU_COIN] = (s8)gArcadeTourney.free;
    sTourneyCurSelect[TOURNEY_MENU_CAB] = 0; /* This node */
    sTourneyCurSelect[TOURNEY_MENU_JOIN] = gArcadeTourney.ext_start;
    sTourneyCurSelect[TOURNEY_MENU_TSEL] = (gArcadeTourney.track < 0) ? 3 : gArcadeTourney.track;
    sTourneyCurSelect[TOURNEY_MENU_CSEL] = gArcadeTourney.car + 1;
    sTourneyCurSelect[TOURNEY_MENU_TRANS] = gArcadeTourney.tranny + 1;
    sTourneyCurSelect[TOURNEY_MENU_DRONE] = gArcadeTourney.drones + 1;
    sTourneyCurSelect[TOURNEY_MENU_SPEED] = gArcadeTourney.catchup + 1;
    sTourneyCurSelect[TOURNEY_MENU_EXIT] = 0;
    sTourneyCurSelect[TOURNEY_MENU_LEN1] = (gArcadeTourney.laps <= 0) ? 0 : 3;
    sTourneyCurSelect[TOURNEY_MENU_LEN2] = (gArcadeTourney.laps <= 0) ? 0 : 3;
    sTourneyCurSelect[TOURNEY_MENU_LEN3] = (gArcadeTourney.laps <= 0) ? 0 : 3;

    for (i = 0; i < TOURNEY_MAX_LINKS; i++) {
        gTourneyInTourney[i] = ((1 << i) & gArcadeTourney.cabinets) != 0;
    }
    for (i = 0; i < TOURNEY_MAX_TRACKS; i++) {
        gTourneyLaps[i] = gArcadeTourney.laps;
    }
}

/**
 * Save tourney settings to NVRAM
 * Arcade: SaveTourneySettings (tourney.c:341)
 */
void tourney_save_settings(void) {
    gArcadeTourney.sanity = TOURNEY_SANITY_VALUE;
    /* Would write to EEPROM/Controller Pak on N64 */
}

/**
 * Show or hide tourney setup screen
 * Arcade: ShowTourneySetup (tourney.c:358)
 */
void tourney_show_setup(s32 show) {
    if (show) {
        /* Would create blit objects and menu text */
        tourney_load_settings();
    } else {
        /* Would remove blit objects */
    }
}

/**
 * Handle tourney setup input
 * Arcade: HandleTourneySetup (tourney.c:500)
 */
void tourney_handle_setup(void) {
    /* Would handle:
     * - Up/down to change menu item
     * - Left/right to change selection
     * - Abort to toggle cabinet or save/cancel
     * In arcade this handles SW_VIEW1, SW_VIEW2, SW_VIEW3, SW_MUSIC, SW_ABORT
     */
}

/**
 * Check if tourney mode is active for this node
 * Arcade: TourneyOn (tourney.c:624)
 */
s32 tourney_is_on(void) {
    return tourney_is_node_active(0); /* gThisNode in arcade */
}

/**
 * Check if tourney mode is active for a specific node
 * Arcade: TourneyNode (tourney.c:633)
 */
s32 tourney_is_node_active(u32 node) {
    return ((gArcadeTourney.cabinets & (1 << node)) != 0);
}

/**
 * Set tourney status for a cabinet
 * Arcade: ResetTourneyStatus (tourney.c:642)
 */
void tourney_reset_status(u32 node, s32 val) {
    if (val) {
        gArcadeTourney.cabinets |= (1 << node);
    } else {
        gArcadeTourney.cabinets &= ~(1 << node);
    }
}

/**
 * Check external tourney box switches
 * Arcade: CheckTourneyBox (tourney.c:654)
 */
void tourney_check_box(void) {
    /* Would check SW_JOIN, SW_TRACK1-4 for linked cabinet control */
}

/**
 * Parse network-coded tourney flags
 * Arcade: ParseTourneyFlags (tourney.c:690)
 */
void tourney_parse_flags(u32 flags, s16 laps) {
    u32 cab;

    cab = (flags >> 16) & 0xff;

    /* Parse the flags, save settings */
    gArcadeTourney.free = 0; /* Always 0 in late arcade versions */
    gArcadeTourney.ext_start = (s8)((flags >> 1) & 3);

    gArcadeTourney.track = (s8)((flags >> 3) & 7);
    if (gArcadeTourney.track >= TOURNEY_MAX_TRACKS) {
        gArcadeTourney.track = -1;
    }

    gArcadeTourney.car = (s8)((flags >> 6) & 15);
    if (gArcadeTourney.car >= TOURNEY_MAX_CARS) {
        gArcadeTourney.car = -1;
    }

    gArcadeTourney.tranny = (s8)((flags >> 10) & 3);
    if (gArcadeTourney.tranny > 1) {
        gArcadeTourney.tranny = -1;
    }

    gArcadeTourney.drones = (s8)((flags >> 12) & 3);
    if (gArcadeTourney.drones > 1) {
        gArcadeTourney.drones = -1;
    }

    gArcadeTourney.catchup = (s8)((flags >> 14) & 3);
    if (gArcadeTourney.catchup > 1) {
        gArcadeTourney.catchup = -1;
    }

    gArcadeTourney.cabinets = cab;

    /* Range check laps */
    if (laps < 0) {
        gArcadeTourney.laps = 0;
    } else if (laps > TOURNEY_MAX_LAPS) {
        gArcadeTourney.laps = TOURNEY_MAX_LAPS;
    } else {
        gArcadeTourney.laps = laps;
    }

    tourney_save_settings();
}

#endif /* NON_MATCHING */
