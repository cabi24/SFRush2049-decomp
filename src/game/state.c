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
extern void attract_handler(void);   /* func_800DB81C - attract mode handler */
extern void playgame_handler(void);  /* func_800CA3B4 - playgame handler */
extern void hiscore_handler(void);   /* func_800FBF88 - high score handler */
extern void countdown_handler(void); /* func_800FBC30 - countdown display */

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
    attract_handler();

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
    countdown_handler();
}

/**
 * state_playgame - Active racing handler
 * Based on arcade: game.c PLAYGAME case + playgame()
 */
void state_playgame(void) {
    /* Call main gameplay function */
    playgame_handler();

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
    hiscore_handler();

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

/******* ARCADE-COMPATIBLE GAME LOOP *******/

/* External sound functions */
extern void SOUND(u16 cmd);
extern void sound_stop_all(void);

/* Sound command IDs */
#define S_STOP_ALL      0x8000
#define S_FANFARE2      0x8048
#define S_RUSH          0x8095
#define S_SPUTTER       0x80A0
#define S_KSPUTTER      0x80A1

/* External game data */
extern s32 trackno;
extern s32 num_active_cars;
extern s32 this_node;
extern s32 demo_game;
extern s32 difficulty[];

/* External functions */
extern void ReadGasAndBrake(void);
extern void update_all_positions(void);
extern s32 are_all_cars_stopped(void);

/**
 * game - Main per-frame game function
 * Based on arcade: game.c:game()
 *
 * This is the central game loop dispatcher that handles
 * all state-specific logic and transitions.
 */
void game(void) {
    /* Handle state-specific logic */
    switch (current_state) {
        case GS_ATTRACT:
            state_attract();
            break;

        case GS_TRKSEL:
            state_track_select();
            break;

        case GS_CARSEL:
            state_car_select();
            break;

        case GS_PREPLAY:
            state_preplay();
            break;

        case GS_COUNTDOWN:
            state_countdown();
            break;

        case GS_PREPLAY2:
            state_preplay2();
            break;

        case GS_PLAYGAME:
            state_playgame();
            break;

        case GS_ENDGAME:
            state_endgame();
            break;

        case GS_GAMEOVER:
            state_gameover();
            break;

        case GS_HISCORE:
            state_hiscore();
            break;

        default:
            break;
    }

    /* Increment state timer */
    state_timer++;
}

/**
 * state_preplay2 - Final pre-race setup
 * Based on arcade: game.c PREPLAY2 case
 *
 * Called just before race starts to set up timer
 * and transition to PLAYGAME.
 */
void state_preplay2(void) {
    /* Set race time from track data */
    /* track_data[trackno].start_time[difficulty[trackno]] */

    /* Play "RUSH!" sound */
    SOUND(S_RUSH);

    /* Initialize checkpoint timing */
    init_cp_time();

    /* Transition to gameplay */
    state_set(GS_PLAYGAME);
}

/**
 * do_endgame - End of race handling
 * Based on arcade: game.c:do_endgame()
 */
void do_endgame(void) {
    /* Show race results */

    /* After delay, transition to GAMEOVER */
    if (state_timer > 3 * ONE_SEC) {
        state_set(GS_GAMEOVER);
    }
}

/**
 * do_gameover - Game over handling
 * Based on arcade: game.c:do_gameover()
 */
void do_gameover(void) {
    /* Check for high score */
    if (check_high_score()) {
        state_set(GS_HISCORE);
    } else {
        state_set(GS_ATTRACT);
    }
}

/**
 * check_high_score - Check if player achieved a high score
 *
 * @return Non-zero if high score achieved
 */
s32 check_high_score(void) {
    /* Compare player's score against high score table */
    /* For now, always return 0 (no high score) */
    return 0;
}

/**
 * are_all_cars_stopped - Check if all cars have stopped
 * Based on arcade: game.c:are_all_cars_stopped()
 *
 * Used to determine when to end the race after timeout.
 *
 * @return Non-zero if all cars stopped
 */
s32 are_all_cars_stopped_impl(void) {
    s32 i;
    f32 speed_threshold = 5.0f;

    for (i = 0; i < num_active_cars; i++) {
        if (car_array[i].mph > speed_threshold) {
            return 0;  /* At least one car still moving */
        }
    }

    return 1;  /* All cars stopped */
}

/**
 * preplay - Full pre-race initialization
 * Based on arcade: game.c:preplay()
 *
 * @param useHud Show HUD during race
 * @param canAbort Allow race abort
 * @return Non-zero if successful
 */
s32 preplay(s32 useHud, s32 canAbort) {
    s32 i;

    /* Clear car object numbers */
    for (i = 0; i < MAX_CARS; i++) {
        car_array[i].objnum = -1;
    }

    /* Load track would go here */
    /* loadTrack(trackno, useHud, canAbort) */

    /* Initialize checkpoints */
    InitCPS();

    /* Initialize cars */
    /* init_cars() */

    /* Initialize drones/AI */
    /* InitDrones() */

    /* Initialize physics for all cars */
    coast_flag = 0;
    for (i = 0; i < num_active_cars; i++) {
        /* Init_MDrive(Initialize, slot) */
    }

    /* Show HUD if requested */
    if (useHud) {
        /* ShowHUD() */
    }

    return 1;  /* Success */
}

/**
 * init_cars - Initialize all car data
 * Based on arcade: game.c:init_cars()
 */
void init_cars(void) {
    s32 i;

    for (i = 0; i < MAX_CARS; i++) {
        car_array[i].data_valid = 0;
        car_array[i].laps = 0;
        car_array[i].checkpoint = 0;
        car_array[i].place = i;
        car_array[i].place_locked = 0;
        car_array[i].score = 0;
        car_array[i].crashflag = 0;
    }
}

/**
 * SetupCar - Setup car visuals and physics
 * Based on arcade: game.c:SetupCar()
 *
 * @param node Car slot index
 * @param body_type Body type index
 * @param init Initialize physics
 */
void SetupCar(s32 node, s32 body_type, s32 init) {
    if (node < 0 || node >= MAX_CARS) {
        return;
    }

    car_array[node].body_type = body_type;
    car_array[node].data_valid = 1;

    if (init) {
        /* Initialize car physics */
        /* Init_MDrive(Initialize, node) */
    }
}

/**
 * check_buttons - Check for button presses during game
 * Based on arcade: game.c:check_buttons()
 */
void check_buttons(void) {
    /* Check for pause, abort, view change, etc. */
    /* Handled by input system */
}

/******* COUNTDOWN FUNCTIONS *******/

/**
 * CountDown - Handle race countdown sequence
 * Based on arcade: game.c:CountDown()
 *
 * Displays 3-2-1-GO and plays appropriate sounds.
 */
void CountDown(void) {
    s32 elapsed_frames;
    s32 current_count;
    static s32 last_count = -1;

    elapsed_frames = frame_counter - countdown_start_time;
    current_count = COUNTDOWN_START - (elapsed_frames / ONE_SEC);

    if (current_count < -1) {
        current_count = -1;
    }

    /* Play sound on count change */
    if (current_count != last_count) {
        last_count = current_count;

        switch (current_count) {
            case 3:
                SOUND(0x8092);  /* S_THREE */
                break;
            case 2:
                SOUND(0x8093);  /* S_TWO */
                break;
            case 1:
                SOUND(0x8094);  /* S_ONE */
                break;
            case 0:
                SOUND(S_RUSH);  /* "RUSH!" / GO */
                /* Transition to race */
                state_set(GS_PREPLAY2);
                break;
        }
    }

    countdown_value = current_count;
}

/**
 * ResetCountdownTimer - Reset the countdown to full
 * Based on arcade: game.c:ResetCountdownTimer()
 */
void ResetCountdownTimer(void) {
    countdown_start_time = frame_counter;
    countdown_value = COUNTDOWN_START;
}

/**
 * SetCountdownTimer - Set countdown duration
 * Based on arcade: game.c:SetCountdownTimer()
 *
 * @param time Time in frames
 */
void SetCountdownTimer(s32 time) {
    countdown_time_limit = frame_counter + time;
}

/**
 * SetCountdownTimerAt0 - Set countdown with time already at zero
 * Based on arcade: game.c:SetCountdownTimerAt0()
 *
 * @param time Time in frames
 */
void SetCountdownTimerAt0(s32 time) {
    countdown_time_limit = frame_counter + time;
    game_start_time = frame_counter;
}

/**
 * GetCountdownTime - Get remaining countdown time
 * Based on arcade: game.c:GetCountdownTime()
 *
 * @return Remaining time in frames (0 if expired)
 */
s32 GetCountdownTime(void) {
    if (frame_counter >= countdown_time_limit) {
        return 0;
    }
    return countdown_time_limit - frame_counter;
}

/**
 * GetElapsedTime - Get elapsed race time
 * Based on arcade: game.c:GetElapsedTime()
 *
 * @return Elapsed time in frames since race start
 */
u32 GetElapsedTime(void) {
    return frame_counter - game_start_time;
}

/**
 * TimeOut - Check if time has run out
 * Based on arcade: game.c:TimeOut()
 *
 * @return Non-zero if time expired
 */
s32 TimeOut(void) {
    return (GetCountdownTime() == 0) ? 1 : 0;
}

/******* STATE QUERY FUNCTIONS *******/

/**
 * is_playgame - Check if currently in gameplay state
 *
 * @return Non-zero if in PLAYGAME state
 */
s32 is_playgame(void) {
    return (current_state == GS_PLAYGAME) ? 1 : 0;
}

/**
 * is_countdown - Check if currently in countdown state
 *
 * @return Non-zero if in COUNTDOWN state
 */
s32 is_countdown(void) {
    return (current_state == GS_COUNTDOWN) ? 1 : 0;
}

/**
 * is_attract - Check if currently in attract mode
 *
 * @return Non-zero if in ATTRACT state
 */
s32 is_attract(void) {
    return (current_state == GS_ATTRACT) ? 1 : 0;
}

/**
 * is_demo_game - Check if running demo game
 *
 * @return Non-zero if demo game active
 */
s32 is_demo_game(void) {
    return demo_game;
}

/**
 * get_state_timer - Get frames spent in current state
 *
 * @return State timer value
 */
s32 get_state_timer(void) {
    return state_timer;
}
