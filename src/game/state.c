/**
 * state.c - Game state machine implementation for Rush 2049 N64
 *
 * Based on arcade game/game.c state machine
 * Handles game state transitions and per-state logic
 *
 * Key functions:
 * - func_800DB81C (attract/race_state_machine) - 2148 bytes
 * - func_800FBF88 (state_transition)
 *
 * Arcade state flow:
 *   ATTRACT -> TRKSEL -> CARSEL -> PREPLAY -> COUNTDOWN ->
 *   PREPLAY2 -> PLAYGAME -> ENDGAME -> GAMEOVER -> HISCORE -> ATTRACT
 */

#include "types.h"
#include "game/state.h"
#include "game/structs.h"

/* External OS functions */
extern u32 osGetCount(void);
extern u32 frame_counter;

/* External game state */
extern u8 gstate;
extern CarData car_array[];
extern GameStruct game_struct;

/* External checkpoint functions */
extern void InitCPS(void);
extern void init_cp_time(void);
extern void CheckCPs(void);

/* External functions - mapped from N64 addresses */
extern void func_800DB81C(void);    /* attract mode handler */
extern void func_800CA3B4(void);    /* playgame handler */
extern void func_800FBF88(void);    /* high score handler */
extern void func_800FBC30(void);    /* countdown handler */

/* Game state flags (N64 uses bitmask, not enum) */
#define GSTATE_INIT     0x40000
#define GSTATE_SETUP    0x80000
#define GSTATE_MENU     0x100000
#define GSTATE_CAMERA   0x200000
#define GSTATE_READY    0x400000
#define GSTATE_PLAY     0x800000
#define GSTATE_CARS     0x1000000
#define GSTATE_FINISH   0x2000000

/* Timing constants */
#define ONE_SEC         60          /* Frames per second */
#define COUNTDOWN_START 3           /* Start countdown at 3 */

/* State machine variables */
u8 gstate_prev;
u32 gstate_flags;
static GState current_state = GS_ATTRACT;
static s32 state_timer;
static s32 countdown_value;
static u32 countdown_start_time;
static u32 game_start_time;
static u32 countdown_time_limit;
static s8 coast_flag;

/* State name strings for debug */
static const char* state_names[] = {
    "ATTRACT",
    "TRKSEL",
    "CARSEL",
    "PREPLAY",
    "COUNTDOWN",
    "PREPLAY2",
    "PLAYGAME",
    "ENDGAME",
    "GAMEOVER",
    "HISCORE"
};

/**
 * state_init - Initialize game state machine
 *
 * Called at game start to set initial state.
 */
void state_init(void) {
    current_state = GS_ATTRACT;
    gstate = GS_ATTRACT;
    gstate_prev = GS_ATTRACT;
    gstate_flags = GSTATE_INIT;
    state_timer = 0;
    coast_flag = 0;
}

/**
 * state_set - Transition to new game state
 * Based on arcade state transitions
 *
 * @param new_state Target state
 */
void state_set(GState new_state) {
    if (new_state >= GS_NUM_STATES) {
        return;  /* Invalid state */
    }

    /* Call exit handler for old state */
    state_exit(current_state);

    /* Update state */
    gstate_prev = current_state;
    current_state = new_state;
    gstate = new_state;

    /* Update flags based on new state */
    switch (new_state) {
        case GS_ATTRACT:
            gstate_flags = GSTATE_INIT;
            break;

        case GS_TRKSEL:
        case GS_CARSEL:
            gstate_flags = GSTATE_MENU | GSTATE_CAMERA;
            break;

        case GS_PREPLAY:
        case GS_COUNTDOWN:
        case GS_PREPLAY2:
            gstate_flags = GSTATE_READY;
            break;

        case GS_PLAYGAME:
            gstate_flags = GSTATE_PLAY | GSTATE_CARS | GSTATE_CAMERA;
            break;

        case GS_ENDGAME:
        case GS_GAMEOVER:
        case GS_HISCORE:
            gstate_flags = GSTATE_FINISH | GSTATE_CARS;
            break;

        default:
            gstate_flags = 0;
            break;
    }

    /* Call entry handler for new state */
    state_enter(new_state);

    /* Reset state timer */
    state_timer = 0;
}

/**
 * state_get - Get current game state
 *
 * @return Current state enum value
 */
GState state_get(void) {
    return current_state;
}

/**
 * state_get_flags - Get current state flags
 *
 * @return Current state flag bitmask
 */
u32 state_get_flags(void) {
    return gstate_flags;
}

/**
 * state_enter - Called when entering a new state
 *
 * @param state State being entered
 */
void state_enter(GState state) {
    switch (state) {
        case GS_ATTRACT:
            /* Reset attract mode variables */
            break;

        case GS_PREPLAY:
            /* Initialize race - called once per game */
            InitCPS();
            init_cp_time();
            break;

        case GS_COUNTDOWN:
            /* Start countdown timer */
            countdown_value = COUNTDOWN_START;
            countdown_start_time = frame_counter;
            timer_set_countdown(10 * ONE_SEC);
            break;

        case GS_PREPLAY2:
            /* Final pre-race setup */
            break;

        case GS_PLAYGAME:
            /* Start actual gameplay */
            game_start_time = frame_counter;
            coast_flag = 0;
            break;

        case GS_ENDGAME:
            /* Race finished */
            break;

        case GS_GAMEOVER:
            /* Game over */
            break;

        case GS_HISCORE:
            /* High score entry */
            break;

        default:
            break;
    }
}

/**
 * state_exit - Called when leaving a state
 *
 * @param state State being exited
 */
void state_exit(GState state) {
    switch (state) {
        case GS_COUNTDOWN:
            /* Countdown finished */
            break;

        case GS_PLAYGAME:
            /* Race ended */
            break;

        default:
            break;
    }
}

/**
 * state_get_name - Get state name string for debug
 *
 * @param state State to get name for
 * @return State name string
 */
const char* state_get_name(GState state) {
    if (state < GS_NUM_STATES) {
        return state_names[state];
    }
    return "UNKNOWN";
}

/******* STATE HANDLERS *******/

/**
 * state_attract - Attract mode handler
 * Based on arcade: game.c:attract()
 *
 * Shows demo races and title screens when idle.
 */
void state_attract(void) {
    /* Call N64's attract handler */
    func_800DB81C();

    /* Check for coin insert / start button */
    /* Would transition to TRKSEL */
}

/**
 * state_track_select - Track selection handler
 * Based on arcade: game.c:TrackSel()
 */
void state_track_select(void) {
    /* Track selection UI handling */
    /* On selection: state_set(GS_CARSEL) */
}

/**
 * state_car_select - Car selection handler
 * Based on arcade: game.c:CarSel()
 */
void state_car_select(void) {
    /* Car selection UI handling */
    /* On selection: state_set(GS_PREPLAY) */
}

/**
 * state_preplay - Pre-race initialization
 * Based on arcade: game.c:preplay()
 */
void state_preplay(void) {
    /* One-time race initialization */
    /* Then transition to COUNTDOWN */
    state_set(GS_COUNTDOWN);
}

/**
 * state_countdown - Race countdown (3-2-1-GO)
 * Based on arcade: game.c COUNTDOWN case
 */
void state_countdown(void) {
    s32 elapsed_frames;
    s32 new_value;

    /* Calculate elapsed frames since countdown start */
    elapsed_frames = frame_counter - countdown_start_time;

    /* Each number lasts 1 second */
    new_value = COUNTDOWN_START - (elapsed_frames / ONE_SEC);

    if (new_value < -1) {
        new_value = -1;
    }

    /* Detect countdown transitions */
    if (new_value != countdown_value) {
        countdown_value = new_value;

        /* Play countdown sounds would go here */
        if (countdown_value == 0) {
            /* "GO!" - transition to PREPLAY2 then PLAYGAME */
            state_set(GS_PREPLAY2);
        }
    }

    /* Call countdown display handler */
    func_800FBC30();
}

/**
 * state_playgame - Active racing handler
 * Based on arcade: game.c PLAYGAME case + playgame()
 */
void state_playgame(void) {
    /* Call main gameplay function */
    func_800CA3B4();

    /* Check checkpoints */
    CheckCPs();

    /* Check for coast (time ran out) */
    if (coast_flag) {
        /* Check if all cars stopped */
        /* Would call are_all_cars_stopped() */
        /* If stopped: state_set(GS_ENDGAME) */
    } else if (timer_timeout()) {
        coast_flag = 1;
        /* Play sputter sound */
    }
}

/**
 * state_endgame - Race finished handler
 * Based on arcade: game.c:endgame()
 */
void state_endgame(void) {
    /* Handle race finish sequence */
    /* After delay: state_set(GS_GAMEOVER) */
}

/**
 * state_gameover - Game over handler
 * Based on arcade: game.c:gameover()
 */
void state_gameover(void) {
    /* Show game over screen */
    /* Check for high score: state_set(GS_HISCORE) */
    /* Otherwise: state_set(GS_ATTRACT) */
}

/**
 * state_hiscore - High score entry handler
 * Based on arcade: game.c:hiscore()
 */
void state_hiscore(void) {
    /* Call high score handler */
    func_800FBF88();

    /* After entry: state_set(GS_ATTRACT) */
}

/******* TIMER FUNCTIONS *******/

/**
 * timer_set_countdown - Set countdown timer
 *
 * @param time Time limit in frames
 */
void timer_set_countdown(u32 time) {
    countdown_time_limit = frame_counter + time;
}

/**
 * timer_get_countdown - Get remaining countdown time
 *
 * @return Remaining time in frames
 */
u32 timer_get_countdown(void) {
    if (frame_counter >= countdown_time_limit) {
        return 0;
    }
    return countdown_time_limit - frame_counter;
}

/**
 * timer_get_elapsed - Get elapsed game time
 *
 * @return Elapsed time since race start in frames
 */
u32 timer_get_elapsed(void) {
    return frame_counter - game_start_time;
}

/**
 * timer_timeout - Check if time ran out
 *
 * @return 1 if timeout, 0 otherwise
 */
s32 timer_timeout(void) {
    return (timer_get_countdown() == 0) ? 1 : 0;
}

/**
 * countdown_finished - Check if countdown is done
 *
 * @return 1 if countdown finished, 0 otherwise
 */
s32 countdown_finished(void) {
    return (countdown_value <= -1) ? 1 : 0;
}

/**
 * state_change_notify - Called when gstate changes
 *
 * Performs cleanup/setup for state transitions.
 */
void state_change_notify(void) {
    if (gstate != gstate_prev) {
        state_exit((GState)gstate_prev);
        state_enter((GState)gstate);
        gstate_prev = gstate;
    }
}
