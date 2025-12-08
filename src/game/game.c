/**
 * game.c - Main game state machine and game loop
 *
 * Based on arcade game/game.c
 * N64 main loop at func_800FD464, state handlers at 0x800FB* area
 */

#include "types.h"
#include "game/structs.h"

/* External function declarations */
extern void func_800C9AE0(void);       /* Input/init handling */
extern void func_800EDDC0(void);       /* Rendering/game logic */
extern void func_800C997C(void);       /* Screen/state update */
extern void func_800B37E8(void);       /* Audio/sound control */
extern void func_800CA3B4(void);       /* Main gameplay (playgame) */
extern void func_800DB81C(void);       /* Attract mode */
extern void func_800FBF88(void);       /* High score entry */
extern void func_800FBC30(void);       /* Countdown timer */
extern void func_800A04C4(void);       /* Viewport/camera setup */

/* State handler declarations */
extern void func_800FBF90(u32 flags);  /* GSTATE_INIT handler */
extern void func_800FBFE4(void);       /* GSTATE_SETUP handler */
extern void func_800FC0EC(u32 flags);  /* GSTATE_MENU handler */

/* External data */
extern u8 gstate;                      /* 0x801146EC - game state byte */
extern u8 gstate_prev;                 /* Previous game state */
extern CarData car_array[];            /* 0x80152818 - array of car data */
extern GameStruct game_struct;         /* 0x801461D0 - main game structure */
extern u32 frame_counter;              /* 0x80142AFC - frame counter */

/**
 * game_loop - Main per-frame game logic
 * Address: 0x800FD464
 *
 * This is the N64 equivalent of arcade game.c:game()
 * Called every frame from the main thread.
 *
 * Handles the game state machine:
 * - ATTRACT: Demo/title screen
 * - TRKSEL: Track selection
 * - CARSEL: Car selection
 * - PREPLAY: Pre-race setup
 * - COUNTDOWN: Race countdown
 * - PLAYGAME: Active racing
 * - ENDGAME: Race finished
 * - GAMEOVER: Game over screen
 * - HISCORE: High score entry
 */
void game_loop(void) {
    u32 state_flags = gstate;

    /* Handle state changes */
    if (gstate != gstate_prev) {
        func_800C9AE0();  /* Process state change */
        gstate_prev = gstate;
    }

    /* Process based on state flags */
    /* N64 uses bitmask states unlike arcade's enum */

    if (state_flags & GSTATE_INIT) {
        func_800FBF90(state_flags);
        return;
    }

    if (state_flags & GSTATE_SETUP) {
        func_800FBFE4();
        return;
    }

    if (state_flags & GSTATE_MENU) {
        func_800FC0EC(state_flags);
        return;
    }

    if (state_flags & GSTATE_CAMERA) {
        func_800A04C4();  /* Camera/viewport setup */
    }

    if (state_flags & GSTATE_READY) {
        func_800FBC30();  /* Countdown */
        return;
    }

    if (state_flags & GSTATE_PLAY) {
        func_800CA3B4();  /* Main gameplay - playgame() */
    }

    if (state_flags & GSTATE_CARS) {
        func_800EDDC0();  /* Update car visuals/rendering */
    }

    if (state_flags & GSTATE_FINISH) {
        func_800FBF88();  /* High score/finish handling */
    }

    /* Audio processing */
    func_800B37E8();

    /* Rendering */
    func_800C997C();

    /* Increment frame counter */
    frame_counter++;
}

/**
 * playgame - Active racing state handler
 * Address: 0x800CA3B4
 *
 * Called when GSTATE_PLAY is active.
 * Updates all active cars, checks collisions, handles AI.
 *
 * Based on arcade game.c:playgame()
 */
#if 0  /* TODO: Decompile from assembly */
void playgame(void) {
    s32 i;

    /* Update timer */
    update_timer();

    /* Process each car */
    for (i = 0; i < MAX_CARS; i++) {
        CarData *car = &car_array[i];

        if (car->data_valid) {
            /* Update physics */
            update_car_physics(car);

            /* Check collisions */
            check_collisions(car);

            /* Update AI for drones */
            if (car->difficulty != DRONE_HUMAN) {
                update_car_ai(car);
            }
        }
    }

    /* Check checkpoints */
    CheckCPs();

    /* Update race positions */
    update_positions();

    /* Check for race end */
    if (race_finished()) {
        gstate = GSTATE_FINISH;
    }
}
#endif

/**
 * attract - Demo/title screen handler
 * Address: 0x800DB81C
 *
 * Handles the attract mode when no one is playing.
 * Shows demo races and title screens.
 */
#if 0  /* TODO: Decompile from assembly */
void attract(void) {
    /* Switch on attract sub-state */
    /* Uses jump table at 0x80124284 */
}
#endif
