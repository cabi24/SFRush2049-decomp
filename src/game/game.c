/**
 * game.c - Main game state machine and game loop
 *
 * Based on arcade game/game.c
 * N64 main loop at func_800FD464, state handlers at 0x800FB* area
 */

#include "types.h"
#include "game/structs.h"

/* External function declarations */
extern void func_800EDDC0(void);       /* Rendering/game logic */
extern void func_800C997C(void);       /* Screen/state update */
extern s32  func_800B37E8(s32, s32, void*, s32); /* Audio/sound control */
extern void func_800DB81C(void);       /* Attract mode */
extern void func_800FBF88(void);       /* High score entry */
extern void func_800FBC30(void);       /* Countdown timer */
extern void func_800A04C4(s32);        /* Viewport/camera setup */

/* Forward declarations for functions defined in this file */
void func_800C9AE0(void);

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

/* Additional external data for game_loop */
extern s8  D_801146EC;   /* Init flag */
extern s32 D_801146E8;   /* Sound handle */
extern s32 D_801170FC;   /* Render enable flag */
extern u32 D_80142AFC;   /* Frame counter */
extern s16 D_8003EB70;   /* Wait flag */
extern u32 D_8003E8E8;   /* Timer/tick */
extern u32 D_8003AFB8;   /* Timing constant (float as u32) */
extern u32 D_80159438;   /* Some state value */
extern u32 D_801597C8;   /* Another state value */
extern u32 D_801597F4;   /* State copy */
extern s8  D_801146C4[]; /* Sound params array */
extern u32 D_801174B4;   /* gstate - current state bitmask (for game_loop) */
extern u32 D_801174B8;   /* gstate_pending - next state (for game_loop) */
extern void func_800B358C(void*); /* sound_stop */
extern void func_800FD238(void); /* state_dispatch */
extern void func_800F733C(void); /* UpdateActiveObjects */
extern void *func_800B0868(void); /* PhysicsObjectList_Update */
extern void func_800B811C(void); /* Effects_UpdateEmitters */

/* Forward declaration */
void func_800CA3B4(void);

/**
 * func_800FD464 - Main per-frame game loop
 * Address: 0x800FD464
 * Size: 704 bytes
 *
 * This is the N64 equivalent of arcade game.c:game()
 * Called every frame from the main thread.
 *
 * The function:
 * 1. Handles first-time initialization
 * 2. Checks for state changes (gstate vs gstate_pending)
 * 3. Manages sound start/stop based on state
 * 4. Calls state-specific handlers
 * 5. Increments frame counter
 */
s32 func_800FD464(void) {
    s32 result = 0;
    u32 gstate_cur;
    u32 gstate_next;
    u32 timing_diff;
    f32 elapsed_time;
    s32 sound_handle;

    /* First-time initialization */
    if (D_801146EC == 0) {
        D_80142AFC = 0;  /* Clear frame counter */
        D_801146EC = 1;  /* Set init flag */
    }

    /* Load current and pending state */
    gstate_cur = D_801174B4;
    gstate_next = D_801174B8;

    /* If no state change pending, just call mode handler and return */
    if (gstate_cur == gstate_next) {
        func_800C9AE0();  /* game_mode_handler */
        return result;
    }

    /* State change in progress - wait for something to clear */
    while (D_8003EB70 != 0) {
        /* Spin wait - checking flag */
    }

    /* Clear render state and call transition handler */
    D_80159438 = 0;
    func_800EDDC0();  /* attract_or_transition */

    /* Save state value */
    D_801597F4 = D_801597C8;

    /* Process inputs/PDUs */
    func_800C997C();  /* process_inputs */

    /* Timing check - calculate elapsed time */
    timing_diff = D_8003E8E8 - D_8003AFB8;
    elapsed_time = (f32)(s32)timing_diff;

    /* Check if elapsed time > 300.0 (5 seconds at 60fps) */
    if (elapsed_time < 300.0f) {
        /* Check if we need to manage sound */
        if (!(D_801174B4 & 0x4000000F)) {
            sound_handle = D_801146E8;
            if (sound_handle == 0) {
                /* Start sound - call with params */
                D_801146E8 = func_800B37E8(0, 0, &D_801146C4, 1);
            }
        } else {
            /* State requires stopping sound */
            sound_handle = D_801146E8;
            if (sound_handle != 0) {
                func_800B358C(sound_handle);  /* sound_stop */
                D_801146E8 = 0;
            }
        }
    }

    /* Call main state change handler */
    func_800CA3B4();  /* PlayGame_HandleStateChange */

    /* Check state flags for additional processing */
    gstate_cur = D_801174B4;

    /* Check if any of the main state bits are set */
    if (gstate_cur & 0x7C03FFFE) {
        /* Call state dispatch for active states */
        func_800FD238();  /* state_dispatch */
        gstate_cur = D_801174B4;
    }

    /* Check attract/race bits (0x00200000 or 0x00400000) */
    if ((gstate_cur << 10) < 0 || (gstate_cur << 9) < 0) {
        /* Race state machine update */
        func_800DB81C();  /* RaceStateMachine_Update */

        /* Check if rendering is enabled */
        if (D_801170FC != 0) {
            func_800F733C();  /* UpdateActiveObjects */
            func_800B0868();  /* PhysicsObjectList_Update */
            func_800B811C();  /* Effects_UpdateEmitters */
            func_800A04C4(0); /* render_scene */
            return 1;
        }
    }

    /* Reload state */
    gstate_cur = D_801174B4;

    /* Check high score bits (0x03FC0000) */
    if (gstate_cur & 0x03FC0000) {
        func_800FBF88();  /* state_transition / high score */
        gstate_cur = D_801174B4;
    }

    /* Check if state matches pending (steady state) */
    gstate_next = D_801174B8;
    if (gstate_cur == gstate_next) {
        /* In steady state - call countdown and update functions */
        func_800FBC30();  /* Countdown handler */
        func_800F733C();  /* UpdateActiveObjects */
        func_800B0868();  /* PhysicsObjectList_Update */
        func_800B811C();  /* Effects_UpdateEmitters */
        func_800A04C4(0); /* render_scene */
        result = 1;
    }

    /* Increment frame counter */
    D_80142AFC++;

    return result;
}

/* External data/functions for func_800C9AE0 */
extern s8  D_80035472;   /* Mode flag 1 */
extern s8  D_80035471;   /* Mode flag 2 */
extern s8  D_80035470;   /* Mode flag 3 */
extern void* D_8003ECF8; /* Some pointer */
extern void* D_8003ECC0; /* Another pointer */
extern s32 func_800075E0(void*, void*, s32); /* Memory operation */
extern s32 func_80007270(void*, void*, s32); /* Memory operation 2 */
extern void func_800013F4(void); /* Timer/sync function */
extern s8 func_800C9528(void); /* State update / input flag */

/**
 * func_800C9AE0 - Game mode handler
 * Address: 0x800C9AE0
 * Size: 256 bytes
 *
 * Called when game state matches pending state (steady state).
 * Handles per-frame mode updates and synchronization.
 */
void func_800C9AE0(void) {
    s16 temp_val = 0x0ABE;  /* 2750 */
    s32 result;

    /* Set mode flag */
    D_80035472 = 1;

    /* Call memory operation with value */
    func_800075E0(&D_8003ECF8, &temp_val, 1);

    /* Clear another flag */
    D_80035471 = 0;

    /* Loop calling func_80007270 until it returns -1 */
    do {
        result = func_80007270(&D_8003ECC0, &temp_val, 0);
        if (result == -1) {
            break;
        }
        result = func_80007270(&D_8003ECC0, &temp_val, 1);
        if (result == -1) {
            break;
        }
        result = func_80007270(&D_8003ECC0, &temp_val, 0);
    } while (result != -1);

    /* Set flags after loop */
    D_80035471 = 1;
    D_80035470 = 0;

    /* Save state value */
    D_801597F4 = D_801597C8;

    /* Call timer/sync function */
    func_800013F4();

    /* Process inputs */
    func_800C997C();

    /* Update state */
    func_800C9528();
}

/**
 * External functions called by func_800CA3B4
 */
extern void func_800CA300(void);       /* State change pre-process */
extern void func_800A5BB8(void);       /* Some init function */
extern void func_800C8B8C(s32, s32, s32, s32, s32, f32, f32, s32); /* HUD setup */
extern void func_800C885C(void);       /* HUD init */
extern void func_800C8FA4(s32);        /* Enable/disable something */
extern void func_800C9BE0(void);       /* Some game init */
extern void func_800014F0(f32);        /* Float function */
extern void func_800C9480(void);       /* State finalize */
extern void func_800C90E0(void);       /* Mode transition */
extern void func_800C9210(f32);        /* Float param function */
extern void func_800C937C(void);       /* Some handler */
extern void func_800C9158(s32, s32);   /* Player state set */
extern void func_800C84C0(s32, s32);   /* Player mode set */
void func_800C9194(s32, s32);          /* Create and register sync entry (defined below) */
extern void func_800A3424(s32, s32);   /* Car update function */
extern void func_800C7308(void*);      /* Object cleanup */
extern void func_800C70BC(void);       /* Scene cleanup */
extern void func_800A1244(void);       /* Render function */
extern s32 func_80097798(s32, s32, s32, s32, s32); /* Sound/effect */
extern void func_800B55FC(s32);        /* Visual update */
void* func_800B42F0(s32);              /* Object allocate wrapper (defined later) */
extern void* func_800B4200(void);      /* Object allocate (uses register s2 for type) */
/* Note: func_80007270 and func_800075E0 declared earlier */

/* External data at fixed addresses */
extern u32 D_801174B4;   /* gstate - current state bitmask */
extern u32 D_801174B8;   /* gstate_pending - next state bitmask */
extern u32 D_801174BC;   /* Additional state flags */
extern s8  D_80114650;   /* Some state byte */
extern s8  D_80156994;   /* Mode flag */
extern s8  D_80146115;   /* Another mode byte */
extern s16 D_8015A108;   /* Player count */
extern void* D_8013E6E0; /* Object list head */
extern void* D_801461D0; /* Main game struct */

/* Additional external data for func_800CA3B4 */
extern u8 D_801461F8;
extern u8 D_80146205;
extern u8 D_80159414;
extern u8 D_80159774;
extern u8 D_80159794;
extern u8 D_801597C4;
extern s16 D_80151AD0;
extern u8 D_80151AD8;
extern void* D_8015A118;
extern u8 D_80146114;
extern s32 D_8015A110;
extern void func_800C84FC(s32, s32, f32, f32, s32);

/**
 * func_800CA3B4 - Game state change handler (playgame equivalent)
 * Address: 0x800CA3B4
 * Size: 2544 bytes
 *
 * Handles game state transitions via bitmask flags.
 * Called when state changes to process enter/exit logic.
 *
 * State bit flags:
 *   0x0001 - Enter gameplay initialization
 *   0x0002 - Secondary setup mode
 *   0x0004 - Third state (menu related)
 *   0x0008 - Cleanup/exit state
 *   0x0010 - Player state update
 *   0x0020 - Multi-player setup
 *   0x0040 - Another state
 *   0x0080 - Result/finish state
 *   0x0100 - High score related
 *   0x0200, 0x0400, 0x0800, 0x1000, 0x2000 - Various states
 *   0x4000 - Sound trigger state
 *   0x40000 - PLAYGAME active
 */
void func_800CA3B4(void) {
    u32 gstate_current;
    u32 gstate_next;
    s32 i;
    s32 player_count;
    f32 ftemp;
    s8 mode_flag;

    /* Load current and pending state */
    gstate_current = D_801174B4;
    gstate_next = D_801174B8;

    /* If no state change, return early */
    if (gstate_current == gstate_next) {
        return;
    }

    /* Pre-process state change */
    func_800CA300();

    /* Store pending state as current */
    gstate_next = D_801174B8;
    D_801174B4 = gstate_next;

    /* State bit 0x0001: Enter gameplay initialization */
    if (gstate_next & 0x0001) {
        /* Clear various flags */
        D_801461F8 = 0;
        D_80146205 = 0;
        D_80159414 = 0;
        D_80159774 = 0;
        D_80159794 = 0;
        D_801597C4 = 0;

        func_800A5BB8();

        mode_flag = D_80156994;
        if (mode_flag == 0) {
            /* Normal mode - setup HUD at 32,16 with specific params */
            func_800C8B8C(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
            func_800C885C();
        }

        func_800C8FA4(1);

        /* Set initial state values */
        D_8015A108 = 1;
        D_80151AD0 = 1;

        func_800C9BE0();
        func_800014F0(30.0f);

        /* Call state handler 3 times */
        func_800C9AE0();
        func_800C9AE0();
        func_800C9AE0();

        func_800C9480();

        /* Set next state to 0x0002 */
        D_801174B8 = 2;
        gstate_current = D_801174B4;
        goto done;
    }

    /* State bit 0x0002: Secondary setup */
    if (gstate_next & 0x0002) {
        func_800C90E0();
        func_800A5BB8();

        mode_flag = D_80156994;
        if (mode_flag == 0) {
            func_800C8B8C(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
            func_800C885C();
        }

        /* Load float from ROM data for timing */
        ftemp = 10.0f * (f32)D_80146115;
        func_800C9210(ftemp);
        func_800C8FA4(1);
        func_800C937C();

        /* Setup player controllers */
        func_800C9158(-1, 0);
        func_800C9158(0, 1);
        func_800C84C0(-1, 1);

        gstate_current = D_801174B4;
        goto done;
    }

    /* Check for early return if bit 31 set */
    if ((s32)gstate_next < 0) {
        goto done;
    }

    /* State bit 0x0004 */
    if (gstate_next & 0x0004) {
        func_800C90E0();
        func_800A5BB8();

        mode_flag = D_80156994;
        if (mode_flag == 0) {
            func_800C8B8C(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
            func_800C885C();
        }

        ftemp = 10.0f * (f32)D_80146115;
        func_800C9210(ftemp);
        func_800C8FA4(1);
        func_800C937C();

        /* Check additional flags and process cars */
        if (!(D_801174BC & 0x007CDFC0)) {
            player_count = D_8015A108;
            if (player_count > 0) {
                for (i = 0; i < player_count; i++) {
                    /* Process each player's car */
                    void *car_ptr = (void*)((u8*)D_8015A118 + i * 76);
                    if (car_ptr) {
                        void *car_data = *(void**)((u8*)car_ptr + 72);
                        if (car_data) {
                            s32 car_obj = *(s32*)((u8*)car_data + 8);
                            func_800A3424(car_obj, 0);
                        }
                    }
                }
            }
        }

        func_800C9158(-1, 0);
        func_800C9158(0, 1);
        func_800C84C0(-1, 1);

        /* Check for specific mode */
        mode_flag = D_80156994;
        if (mode_flag != 0) {
            if (!(D_801174BC & 0x007C0000)) {
                s8 val1 = D_80146114 + 14;
                if (val1 < 13) {
                    s8 val2 = D_80146114 + 12;
                    ftemp = 10.0f * (f32)val2;
                    func_800C9210(ftemp);
                    func_800C9194(6, 1);
                }
            }
        }

        D_80151AD8 = 0;
        gstate_current = D_801174B4;
        goto done;
    }

    /* State bit 0x0008: Cleanup/exit */
    if (gstate_next & 0x0008) {
        /* Walk object list and clean up */
        void *obj = D_8013E6E0;
        while (obj != NULL) {
            func_800C7308(obj);
            void *next = *(void**)obj;
            obj = *(void**)next;
        }

        func_800C70BC();
        func_800C90E0();
        func_800A5BB8();

        mode_flag = D_80156994;
        if (mode_flag == 0) {
            func_800C8B8C(8, 8, 0, 0, 0, 1.0f, 200.0f, 0);
        }

        func_800C8FA4(0);

        if (D_80156994 != 0) {
            func_800A1244();
        }

        gstate_current = D_801174B4;
        goto done;
    }

    /* State bit 0x0010: Player state update */
    if (gstate_next & 0x0010) {
        func_800C9158(-1, 0);
        func_800C9158(0, 1);
        func_800C84C0(-1, 1);
        gstate_current = D_801174B4;
        goto done;
    }

    /* State bit 0x0020: Multi-player setup */
    if (gstate_next & 0x0020) {
        player_count = D_8015A108;
        for (i = 0; i < player_count; i++) {
            func_800C9158(i, 1);
            func_800C84C0(i, 1);
        }
        /* Disable remaining slots */
        for (; i < 4; i++) {
            func_800C9158(i, 0);
        }
        gstate_current = D_801174B4;
        goto done;
    }

    /* State bit 0x0040 */
    if (gstate_next & 0x0040) {
        func_800C9158(-1, 0);
        func_800C9158(0, 1);
        gstate_current = D_801174B4;
        goto done;
    }

    /* Various other state bits (0x0200-0x2000) - return early if set */
    if (gstate_next & 0x0200) goto done;
    if (gstate_next & 0x0400) goto done;
    if (gstate_next & 0x0800) goto done;
    if (gstate_next & 0x1000) goto done;
    if (gstate_next & 0x2000) goto done;
    if ((gstate_next << 4) < 0) goto done; /* Bit 0x8000000 */

    /* State bit 0x4000: Sound/effect trigger */
    if (gstate_next & 0x4000) {
        func_80097798(57, 0, 0, 0, 0);
        gstate_current = D_801174B4;
        goto done;
    }

    /* State bit 0x0100: High score/result */
    if (gstate_next & 0x0100) {
        func_80097798(56, 0, 0, 0, 0);

        player_count = D_8015A108;
        for (i = 0; i < player_count; i++) {
            func_800C9158(i, 1);
            func_800C84C0(i, 1);
        }
        for (; i < 4; i++) {
            func_800C9158(i, 0);
        }
        gstate_current = D_801174B4;
        goto done;
    }

    /* State bit 0x0080: Result/finish */
    if (gstate_next & 0x0080) {
        func_80097798(60, 0, 0, 0, 0);
        func_800C9158(-1, 0);
        func_800C9158(0, 1);
        gstate_current = D_801174B4;
        goto done;
    }

    /* State bit 0x40000: PLAYGAME active (main gameplay) */
    if ((gstate_next << 13) < 0) { /* Check bit 0x40000 */
        s8 state_byte = D_80114650;

        if (state_byte == 0) {
            func_800C70BC();
            func_800C90E0();
            func_800A5BB8();
        }

        func_800C8FA4(0);

        if (state_byte == 0) {
            mode_flag = D_80156994;
            if (mode_flag == 0) {
                s32 game_mode = D_8015A110;
                if (game_mode != 6 && game_mode != 4 && game_mode != 5) {
                    func_800C8B8C(8, 8, 0, 0, 0, 1.0f, 200.0f, 0);
                }
            }

            mode_flag = D_80156994;
            if (mode_flag == 0) {
                s32 game_mode = D_8015A110;
                if (game_mode == 6) {
                    func_800C84FC(5, 64, 1.0f, 200.0f, 0);
                }
            }

            func_800B55FC(1);
        }

        /* Check game mode for specific handling */
        s32 game_mode = D_8015A110;
        if (game_mode != 6 && game_mode != 4 && game_mode != 5) {
            if (D_80156994 == 0) {
                func_800A1244();
            }
        }

        state_byte = D_80114650;
        if (state_byte == 0) {
            /* Dispose objects */
            func_800B42F0(13);
            func_800B42F0(10);
            func_800B42F0(11);
            func_800B42F0(12);
            func_800B42F0(4);
            func_800B42F0(1);
            func_800B42F0(2);
            func_800B42F0(3);
            func_800B42F0(9);
            func_800B42F0(8);

            /* Initialize memory regions */
            func_80007270(&D_801461D0, 0, 1);
            func_800B4200();
            func_800075E0(&D_801461D0, 0, 0);

            func_80007270(&D_801461D0, 0, 1);
            func_800B4200();
            func_800075E0(&D_801461D0, 0, 0);

            func_80007270(&D_801461D0, 0, 1);
            func_800B4200();
            func_800075E0(&D_801461D0, 0, 0);
        }

        func_800C9158(-1, 0);
        func_800C9158(0, 1);
        func_800C84C0(-1, 1);

        gstate_current = D_801174B4;
    }

done:
    /* Return with gstate_current in register */
    return;
}

/* External data for countdown function */
extern s32 D_8015A110;     /* Game mode */
extern s16 D_8015A108;     /* Player count */
extern s32 D_801613B0;     /* Found player index */
extern s32 D_801146F8;     /* Countdown timer */
extern void* D_8018A4E0[]; /* Display object pointer array */
extern s8* D_80120308;     /* Format string 1 */
extern s8* D_8012030C;     /* Format string 2 */

/* External functions for countdown */
extern void sprintf(s8 *buf, s8 *fmt, ...); /* func_80004990 */
extern void func_800A4770(s8 *buf, s32 val);
extern void func_800B74A0(s32 type);
extern void func_800B71D4(s16 pos, s32 color, s8 *text);
extern s32 func_800B3FA4(s8 *buf, s32 val);

/**
 * func_800FBC30 - Countdown wrapper
 * Address: 0x800FBC30
 * Size: 8 bytes (just loads mode and falls through)
 *
 * This is just a label - loads D_8015A110 then falls into func_800FBC38.
 */

/**
 * func_800FBC38 - Countdown display handler
 * Address: 0x800FBC38
 * Size: 488 bytes
 *
 * Displays the countdown timer during race start.
 * Finds active player, formats countdown text, and displays it.
 */
void func_800FBC38(void) {
    s32 player_count;
    s32 i;
    s8 str_buf[80];      /* sp+0x50 */
    s8 format_buf[88];   /* sp+0x58 */
    void *display_data;
    s32 countdown_val;
    s32 result;
    u32 state_flags;

    /* Get player count - 1 if mode is 2, otherwise from D_8015A108 */
    if (D_8015A110 == 2) {
        player_count = 1;
    } else {
        player_count = D_8015A108;
    }

    /* Find first inactive player slot */
    i = 0;
    if (player_count > 0) {
        s8 *player_ptr = (s8 *)0x80156CF0;
        while (i < player_count) {
            if (*player_ptr == 0) {
                break;  /* Found inactive slot */
            }
            i++;
            player_ptr += 16;
        }
    }

    /* Store found player index */
    D_801613B0 = i;

    /* Check if we found a slot before count */
    if (i >= player_count) {
        /* All players accounted for - set countdown to 320 (5.3 sec at 60fps) */
        countdown_val = 320;
        D_801146F8 = countdown_val;
        return;
    }

    /* Format player string */
    sprintf(str_buf, D_80120308, i + 1);

    /* Get display parameters from global */
    display_data = D_8018A4E0[1];  /* offset 4 = array[1] */

    /* Format display text with position params */
    {
        s32 x_pos = *(s32 *)((u8 *)display_data + 0x1F0);
        s32 y_pos = *(s32 *)((u8 *)display_data + 0x1F4);
        s32 width = *(s32 *)((u8 *)display_data + 0x1F8);
        s32 height = *(s32 *)((u8 *)display_data + 0x1FC);
        s32 extra = *(s32 *)((u8 *)display_data + 0x200);

        countdown_val = D_801146F8;
        sprintf(format_buf, D_8012030C, str_buf, x_pos, y_pos, countdown_val, width, height, extra);
    }

    /* Call rendering function */
    func_800A4770(format_buf, *(s32 *)((u8 *)display_data + 0x200));

    /* Create display object */
    func_80007270(&D_801461D0, 0, 1);
    func_800B4200();
    func_800075E0(&D_801461D0, 0, 0);

    /* Set display type */
    func_800B74A0(22);

    /* Check state flags for color selection */
    state_flags = D_801174B4;

    if ((state_flags & 0x0001) || (state_flags & 0x0002) ||
        ((state_flags << 1) < 0) ||   /* bit 0x80000000 */
        (state_flags & 0x0004) || (state_flags & 0x0008)) {
        /* Special state - color 8 (darker) */
        countdown_val = D_801146F8;
        func_800B71D4((s16)countdown_val, 8, format_buf);
    } else {
        /* Normal state - color 205 (0xCD, brighter) */
        countdown_val = D_801146F8;
        func_800B71D4((s16)countdown_val, 205, format_buf);
    }

    /* Finalize display and decrement countdown */
    countdown_val = D_801146F8;
    result = func_800B3FA4(format_buf, -1);
    countdown_val--;
    D_801146F8 = countdown_val;

    /* Reset countdown if it goes too negative */
    if (countdown_val < -result) {
        D_801146F8 = 320;
    }
}

/**
/* Global for object type - used by func_800B4200 via register convention */
extern s8 D_80159DA0;  /* Object allocation type (set before calling func_800B4200) */

/**
 * func_800B42F0 - Allocate and initialize object wrapper
 * Address: 0x800B42F0
 * Size: 112 bytes
 *
 * Wrapper that:
 * 1. Locks memory region (func_80007270)
 * 2. Sets object type (via register s2 in assembly)
 * 3. Allocates object (func_800B4200)
 * 4. Unlocks memory region (func_800075E0)
 *
 * Note: In the original assembly, 'type' is passed via register s2
 * to func_800B4200 using a delay slot trick. Here we simulate this
 * by setting a global before the call.
 *
 * @param type  Object type to allocate
 * @return      Allocated object pointer
 */
void* func_800B42F0(s32 type) {
    void *obj;
    s8 saved_type;

    /* Lock memory region */
    func_80007270(&D_801461D0, 0, 1);

    /* Set object type (simulates s2 register setup) */
    saved_type = D_80159DA0;
    D_80159DA0 = (s8)type;

    /* Allocate object of specified type */
    obj = func_800B4200();

    /* Restore and unlock */
    D_80159DA0 = saved_type;
    func_800075E0(&D_801461D0, 0, 0);

    return obj;
}

/* External for visual update */
extern void func_800A2378(void *obj, s32 flag);

/**
 * func_800B55FC - Update visual objects
 * Address: 0x800B55FC
 * Size: 140 bytes
 *
 * Iterates through visual object array and calls update function
 * on each object that has a valid update handler.
 *
 * @param flag  Update flag passed to each object's update function
 */
void func_800B55FC(s32 flag) {
    u32 *array_ptr;
    u32 *array_end;
    void *entry;
    void *obj;
    void **vtable;

    array_ptr = (u32 *)0x80144D60;
    array_end = (u32 *)0x80144DA0;

    /* Iterate through object array (16-byte entries) */
    while (array_ptr != array_end) {
        /* Load object list head from offset 8 of array entry */
        entry = (void *)array_ptr[2];

        if (entry != NULL) {
            /* Get first object in linked list */
            obj = *(void **)entry;

            /* Walk the linked list */
            while (obj != NULL) {
                /* Get vtable pointer */
                vtable = *(void ***)obj;

                /* Check if update handler exists at vtable[0x48/4] = vtable[18] */
                if (vtable[18] != NULL) {
                    /* Call update function */
                    func_800A2378(obj, flag);
                }

                /* Get object pointer from vtable, then get next object */
                obj = *(void **)vtable;
            }
        }

        /* Advance to next array entry (16 bytes) */
        array_ptr += 4;
    }
}

/* External data for race state machine */
extern s32 D_801170FC;       /* Race state (0-8) */
extern s32 D_80159D98;       /* Race sub-state */
extern s32 D_8015698C;       /* Current player index */
extern s16 D_80151AD0;       /* Player count */
extern s8  D_80117350;       /* Player active flag */
extern s8  D_80117354;       /* Secondary active flag */
extern void* D_80143FD8;     /* Object list head */
extern f32 D_801543CC;       /* Timer value */

/* External functions for race states */
extern void func_800C813C(s32, s32);   /* Race pre-update */
extern void func_800DB1E0(void);       /* State 1 handler */
extern void func_800D91A0(void);       /* State 2 handler */
extern void func_800D7634(void);       /* State 3 handler */
extern void func_800B8C14(void);       /* State 4 handler */
extern void func_800B4FB0(s32);        /* State setter */
extern void func_800013C0(void);       /* Timer init */
extern void func_800013DC(void);       /* Timer update */
extern void* func_80091B00(void);      /* Allocate object */
extern void func_80092360(s32, s32, s32, s32); /* Sound trigger */
extern void func_800C9210(f32);        /* Set timing */
extern void func_800D5374(void);       /* Race setup 1 */
extern void func_800D5798(void);       /* Race setup 2 */
extern void func_800C84C0(s32, s32);   /* Player mode set */
extern void func_800D6530(void);       /* Track init */
extern void func_800D6160(s32);        /* Visual init */
extern void func_800D60AC(void);       /* Scene setup */
extern void func_800D5A04(void);       /* Cleanup */
extern void func_800B5F4C(void);       /* Menu prev */
extern void func_800B5FC4(void);       /* Menu confirm */
extern void func_800B5F88(void);       /* Menu toggle */
extern void func_800B438C(void);       /* Sound update */

/**
 * func_800DB81C - Race state machine
 * Address: 0x800DB81C
 * Size: 2148 bytes
 *
 * Main race state machine with 9 states (0-8):
 *   0 = Initialize race, find first active player
 *   1 = Pre-race setup (calls func_800DB1E0)
 *   2 = Track loading (calls func_800D91A0)
 *   3 = Car setup (calls func_800D7634)
 *   4 = Countdown prep (calls func_800B8C14)
 *   5 = Main racing (nested 8-state machine for gameplay)
 *   6 = Race complete handling
 *   7 = Results/standings
 *   8 = Cleanup/exit
 *
 * State 5 has a nested switch for racing sub-states:
 *   0 = Race start
 *   1 = Countdown
 *   2 = Racing
 *   3 = Lap complete
 *   4 = Race finish
 *   5 = Winner display
 *   6 = High score check
 *   7 = Exit racing
 */
void func_800DB81C(void) {
    s32 race_state;
    s32 sub_state;
    s32 i;
    s32 player_count;
    s32 player_idx;
    void *obj;
    void *alloc_obj;
    u32 player_flags;
    s8 *player_array;
    u8 *player_status;
    f32 timer_val;

    /* Load current race state */
    race_state = D_801170FC;

    /* If not in state 0, call pre-update */
    if (race_state != 0) {
        func_800C813C(0, 0);
        race_state = D_801170FC;
    }

    /* Validate state range */
    if (race_state >= 9) {
        goto exit_common;
    }

    /* Main state switch */
    switch (race_state) {

    case 0:  /* Initialize race - find first active player */
        player_count = D_80151AD0;
        if (player_count <= 0) {
            goto exit_func;
        }

        player_array = (s8 *)0x8015A118;
        player_status = (u8 *)0x80156CF0;

        for (i = 0; i < player_count; i++) {
            player_flags = *(u32 *)(player_array + i * 76 + 4);

            /* Check if player slot is active */
            if ((player_flags & 0x0001) || player_status[i * 16] == 0) {
                /* Found active player */
                D_8015698C = i;
                D_80117350 = (player_status[i * 16] == 0) ? 1 : 0;
                break;
            }
        }

        if (i >= player_count) {
            goto exit_func;
        }

        /* Initialize race */
        func_800B4FB0(1);
        D_80159D98 = 0;
        func_800013C0();

        /* Lock and allocate race object */
        func_80007270(&D_801461D0, 0, 1);
        alloc_obj = func_80091B00();
        *(u8 *)((u8 *)alloc_obj + 2) = 7;  /* Set object type */
        func_800075E0(&D_801461D0, 0, 0);
        func_800075E0((void *)0x801427A8, alloc_obj, 0);

        /* Setup race timing */
        func_800C9210(0.0f);

        /* Initialize race systems */
        func_800D5374();
        func_800D5798();

        /* Initialize object list */
        obj = D_80143FD8;
        while (obj != NULL) {
            *(u8 *)((u8 *)obj + 100) = 2;
            *(s32 *)((u8 *)obj + 96) = -1;
            obj = *(void **)obj;
        }

        func_800C84C0(-1, 1);
        func_800B55FC(1);
        func_80092360(46, 0, 1, 0);  /* Play race start sound */
        break;

    case 1:  /* Pre-race setup */
        func_800DB1E0();
        break;

    case 2:  /* Track loading */
        func_800D91A0();
        break;

    case 3:  /* Car setup */
        func_800D7634();
        break;

    case 4:  /* Countdown prep */
        func_800B8C14();
        break;

    case 5:  /* Main racing state - nested switch */
        player_idx = D_8015698C;
        player_flags = *(u32 *)(0x8015A11C + player_idx * 76);

        if (!(player_flags & 0x0002)) {
            goto state5_default;
        }

        func_80092360(37, 0, 1, 0);  /* Racing sound */

        sub_state = D_80159D98;
        if (sub_state >= 8) {
            goto state5_default;
        }

        switch (sub_state) {
        case 0:  /* Race start */
            func_800B4FB0(0);
            func_800013DC();
            func_800D6530();

            func_80007270(&D_801461D0, 0, 1);
            alloc_obj = func_80091B00();
            *(u8 *)((u8 *)alloc_obj + 2) = 9;
            func_800075E0(&D_801461D0, 0, 0);
            func_800075E0((void *)0x801427A8, alloc_obj, 0);

            func_800D6160(1);
            if (!(D_801174B4 & 0x00200000)) {
                func_800D6160(0);
            }
            func_800D60AC();
            func_800C84C0(-1, 0);
            goto exit_func;

        case 1:  /* Countdown timer check */
            timer_val = D_801543CC;
            if (timer_val >= 15.0f) {
                D_80114650 = 1;
            }
            break;

        case 2:  /* Countdown in progress */
            timer_val = D_801543CC;
            if (timer_val >= 15.0f) {
                D_80114650 = 0;
                if (D_801174B4 & 0x00200000) {
                    goto state5_case7;
                }
            }
            break;

        case 3:  /* Race in progress */
            func_800B4FB0(8);
            break;

        case 4:  /* Race setup */
            func_800B4FB0(2);
            break;

        case 5:  /* Lap handling */
            func_800B4FB0(3);
            break;

        case 6:  /* Position update */
            *(u8 *)(0x8015418C + D_8015698C) = 1;
            func_800B4FB0(4);
            break;

        case 7:  /* Exit sub-state */
        state5_case7:
            func_800B4FB0(6);
            break;
        }

    state5_default:
        break;

    case 6:  /* Race complete - menu navigation */
        player_idx = D_8015698C;
        player_flags = *(u32 *)(0x8015A11C + player_idx * 76);

        if (player_flags & 0x0400) {
            func_800B5F4C();  /* Previous */
            sub_state = D_80159D98;
            sub_state--;
            if (sub_state < 0) sub_state = 7;
            D_80159D98 = sub_state;
        } else if (player_flags & 0x0800) {
            func_800B5F4C();  /* Next */
            sub_state = D_80159D98;
            sub_state++;
            if (sub_state >= 8) sub_state = 0;
            D_80159D98 = sub_state;
        } else if (player_flags & 0x0005) {
            func_800B5FC4();  /* Confirm */
            goto case0_init;
        }
        break;

    case 7:  /* Results/standings */
        player_idx = D_8015698C;
        player_flags = *(u32 *)(0x8015A11C + player_idx * 76);

        if (player_flags & 0x3000) {
            func_800B5F88();  /* Toggle display */
        }

        if (player_flags & 0x0002) {
            if (D_80117354 != 0) {
                if ((D_801174B4 & 0x00200000) && race_state == 8) {
                    goto state7_restart;
                }
            }
        }
        /* Fall through to common handling */
        goto state7_common;

    state7_restart:
        *(u8 *)0x80142699 = 1;
        func_800B4FB0(0);
        func_800013DC();

        func_80007270(&D_801461D0, 0, 1);
        alloc_obj = func_80091B00();
        *(u8 *)((u8 *)alloc_obj + 2) = 7;
        func_800075E0(&D_801461D0, 0, 0);
        func_800075E0((void *)0x801427A8, alloc_obj, 0);
        goto exit_common;

    state7_common:
        D_80114650 = (race_state == 7) ? 1 : 0;
        func_800B4FB0(0);
        func_800013DC();

        func_80007270(&D_801461D0, 0, 1);
        alloc_obj = func_80091B00();
        *(u8 *)((u8 *)alloc_obj + 2) = 7;
        func_800075E0(&D_801461D0, 0, 0);
        func_800075E0((void *)0x801427A8, alloc_obj, 0);
        func_800D5A04();
        break;

    case 8:
    default:
        player_idx = D_8015698C;
        player_flags = *(u32 *)(0x8015A11C + player_idx * 76);

        if (player_flags & 0x0007) {
            func_800B5FC4();
            func_800B4FB0(1);
        }
        break;
    }

case0_init:
exit_common:
    /* Common exit - check player status changes */
    race_state = D_801170FC;
    if (race_state != 0) {
        player_idx = D_8015698C;
        player_status = (u8 *)(0x80156CF0 + player_idx * 16);
        s8 current_status = (*player_status == 0) ? 1 : 0;

        if (current_status != D_80117350) {
            D_80117350 = current_status;

            /* Don't update sound during certain states */
            if (race_state != 5 && race_state != 4 && race_state != 2 && race_state != 3) {
                func_800B438C();
            }
        }
    }

    /* Check for reset flag */
    if (*(s8 *)0x80157244 != 0) {
        D_80114650 = 0;
        func_800B4FB0(0);
        func_800013DC();

        func_80007270(&D_801461D0, 0, 1);
        alloc_obj = func_80091B00();
        *(u8 *)((u8 *)alloc_obj + 2) = 7;
        func_800075E0(&D_801461D0, 0, 0);
        func_800075E0((void *)0x801427A8, alloc_obj, 0);
        func_800D5A04();
    }

exit_func:
    return;
}

/**
 * func_800F7344 - Update all active sound objects
 * Address: 0x800F7344
 * Size: 184 bytes
 *
 * Iterates through active sound list (D_80159450) and calls each
 * object's update callback. If callback returns 0, the sound is
 * stopped and removed via func_800B358C.
 *
 * Note: func_800F733C loads count into t6 immediately before this
 * (lui t6, 0x8015 / lw t6, -26744(t6)) - that's the prologue.
 */
extern s32  D_80159788;           /* Active sound count */
extern void **D_80159450;         /* Sound object pointer array */
extern void func_800B358C(void*); /* Stop and remove sound */

void func_800F7344(void) {
    s32 count;
    void **cur_ptr;
    void **end_ptr;
    void *sound_obj;
    s32 (*callback)(void*);
    s32 status;
    s32 result;

    /* Get current count (loaded by preceding code into t6) */
    count = D_80159788;
    if (count <= 0) {
        return;
    }

    /* Start at beginning of sound list */
    cur_ptr = &D_80159450;

    while (1) {
        sound_obj = *cur_ptr;

        /* Get callback function pointer at offset 0x28 */
        callback = *(s32 (**)(void*))((u8*)sound_obj + 0x28);

        if (callback != NULL) {
            /* Get status at offset 0 */
            status = *(s32*)sound_obj;

            if (status != -1) {
                /* Call the update callback */
                result = callback(sound_obj);

                if (result == 0) {
                    /* Callback returned 0 - stop and remove this sound */
                    func_800B358C(sound_obj);

                    /* Recalculate end pointer (list may have shifted) */
                    count = D_80159788;
                    end_ptr = &D_80159450 + count;

                    /* Back up pointer since list shifted */
                    cur_ptr--;
                    goto next_iter;
                }
            }
        }

        /* Calculate end pointer */
        count = D_80159788;
        end_ptr = &D_80159450 + count;

    next_iter:
        cur_ptr++;
        if (cur_ptr >= end_ptr) {
            break;
        }
    }
}

/**
 * func_800F73FC - Main render pipeline dispatcher
 * Address: 0x800F73FC
 * Size: 76 bytes
 *
 * This is the main per-frame render update entry point.
 * Orchestrates all visual updates:
 *   1. If sound_update != 0, update active sounds (func_800F733C -> func_800F7344)
 *   2. If physics_update != 0, update physics objects (func_800B0868 -> func_800B0870)
 *   3. Always update particle/effect emitters (func_800B811C)
 *   4. Always run main scene render (func_800A04C4)
 *
 * @param sound_update   Whether to update sounds (arg a0)
 * @param physics_update Whether to update physics (arg a1)
 */
extern void func_800F733C(void);  /* Loads count, falls through to func_800F7344 */
extern void *func_800B0868(void);  /* Physics object list head loader */
extern void func_800B0870(void);  /* Physics object linked list update */
extern void func_800B811C(void);  /* Update particle emitters/effects */
extern void func_800A04C4(s32);   /* Main scene render */

void func_800F73FC(s32 sound_update, s32 physics_update) {
    /* Update active sounds if requested */
    if (sound_update != 0) {
        func_800F733C();  /* Loads count then calls func_800F7344 */
    }

    /* Update physics objects if requested */
    if (physics_update != 0) {
        func_800B0868();  /* Starts physics linked list traversal */
    }

    /* Always update particle/effect emitters */
    func_800B811C();

    /* Always render the scene */
    func_800A04C4(0);
}

/**
 * func_800B0868 - Get physics object list head
 * Address: 0x800B0868
 * Size: 8 bytes (just loads and returns)
 *
 * Returns the head of the physics object linked list.
 * This is called before func_800B0870 to get list head in v0.
 */
extern void *D_801491F0;  /* Physics object list head */

void *func_800B0868(void) {
    return D_801491F0;
}

/**
 * func_800B0870 - Update all physics objects in linked list
 * Address: 0x800B0870
 * Size: 108 bytes
 *
 * Traverses the physics object linked list starting from D_801491F0.
 * For each object with an update callback at offset 0x14, calls it
 * with (object, 1).
 *
 * Object structure (partial):
 *   offset 0x00: next pointer (linked list)
 *   offset 0x14: update callback function pointer
 */
extern void func_800B066C(void);  /* Physics cleanup/finalize */

void func_800B0870(void) {
    void *current;
    void *next;
    void (*update_callback)(void*, s32);

    /* Get list head (loaded by func_800B0868 into v0) */
    current = D_801491F0;

    /* Get next pointer for iteration */
    if (current != NULL) {
        next = *(void**)current;  /* offset 0x00 = next */
    } else {
        next = NULL;
    }

    /* Traverse the linked list */
    while (current != NULL) {
        /* Get update callback at offset 0x14 */
        update_callback = *(void (**)(void*, s32))((u8*)current + 0x14);

        if (update_callback != NULL) {
            /* Call update with (object, 1) */
            update_callback(current, 1);
        }

        /* Move to next object */
        if (next == NULL) {
            break;
        }
        current = next;
        next = *(void**)current;  /* Get next->next */
    }

    /* Call cleanup function */
    func_800B066C();
}

/**
 * func_800B811C - Update all particle emitters
 * Address: 0x800B811C
 * Size: 252 bytes
 *
 * Iterates through the particle emitter array and updates each one:
 *   1. Copies current position to previous position
 *   2. Calls update function (func_8008D6B0)
 *   3. Updates velocity with acceleration
 *   4. Conditionally spawns particles if D_801170FC == 0
 *   5. Calls cleanup functions at end
 *
 * Emitter structure (152 bytes per emitter):
 *   offset 0x24-0x2C: current position (x, y, z)
 *   offset 0x34-0x3C: velocity (x, y, z)
 *   offset 0x48: additional velocity component
 *   offset 0x54-0x5C: previous position (x, y, z)
 */
extern s16 D_80151AD0;            /* Active emitter count */
extern s32 D_801170FC;            /* Render enable flag */

/* Emitter array at 0x80150B70, secondary at 0x80150BA0 */
extern u8  D_80150B70[];          /* Emitter array base */
extern u8  D_80150BA0[];          /* Secondary emitter array */

extern void func_8008D6B0(void*, void*);  /* Emitter update */
extern void func_800B80C8(s32);           /* Spawn particles for emitter */
extern void func_800B7FF8(void);          /* Particle cleanup */
extern void func_800B61FC(s32);           /* Final cleanup */
extern s16 D_80152032;                    /* Second emitter count */

void func_800B811C(void) {
    s32 i;
    s16 count;
    u8 *emitter;
    u8 *emitter2;
    f32 *pos;
    f32 *prev_pos;
    f32 *vel;

    count = D_80151AD0;
    if (count <= 0) {
        goto end_loop;
    }

    emitter = D_80150B70;
    emitter2 = D_80150BA0;

    for (i = 0; i < count; i++) {
        /* Copy current position to previous position */
        pos = (f32*)(emitter + 0x24);
        prev_pos = (f32*)(emitter + 0x54);
        prev_pos[0] = pos[0];  /* x */
        prev_pos[1] = pos[1];  /* y */
        prev_pos[2] = pos[2];  /* z */

        /* Call emitter update function */
        func_8008D6B0((void*)emitter, (void*)emitter2);

        /* Update velocity with acceleration (FP operations) */
        vel = (f32*)(emitter + 0x34);
        /* Note: Original has complex FP ops here that update velocity */
        /* The disassembly shows additions to vel.x/y/z from other offsets */

        /* Conditionally spawn particles */
        if (D_801170FC == 0) {
            func_800B80C8(i);
        }

        /* Advance to next emitter (152 bytes each) */
        emitter += 152;
        emitter2 += 152;

        /* Re-read count in case it changed */
        count = D_80151AD0;
    }

end_loop:
    /* Call cleanup functions */
    func_800B7FF8();

    /* Call final cleanup with flag based on second emitter count */
    {
        s16 count2 = D_80152032;
        s32 flag = (0 < count2) ? 1 : 0;
        func_800B61FC(flag);
    }
}

/**
 * func_800C9528 - Load input initialized flag (prologue)
 * Address: 0x800C9528
 * Size: 8 bytes
 *
 * Just loads D_801147C4 into t6 for func_800C9530.
 */
extern s8 D_801147C4;  /* Input system initialized flag */

s8 func_800C9528(void) {
    return D_801147C4;
}

/**
 * func_800C9530 - Process input callback table
 * Address: 0x800C9530
 * Size: 96 bytes
 *
 * Iterates through an array of input handler function pointers
 * from D_801551E8 to D_80155210 (10 entries, 40 bytes).
 * Calls each non-NULL callback in sequence.
 *
 * If D_801147C4 == 0, first calls func_800B73E4 for initialization.
 */
extern void (*D_801551E8[])(void);  /* Input callback table start */
extern void (*D_80155210)(void);    /* Input callback table end marker */
extern void func_800B73E4(void);    /* Input system init */

void func_800C9530(void) {
    void (**callback_ptr)(void);
    void (*callback)(void);

    /* One-time initialization if not yet done */
    if (D_801147C4 == 0) {
        func_800B73E4();
    }

    /* Iterate through callback table */
    callback_ptr = D_801551E8;

    while (callback_ptr != &D_80155210) {
        callback = *callback_ptr;
        if (callback != NULL) {
            callback();
        }
        callback_ptr++;
    }
}

/**
 * func_800D7D40 - Clear all sound handles for a player's car
 * Address: 0x800D7D40
 * Size: 132 bytes
 *
 * Iterates through all 17 sub-entries (64 bytes each) of a player's
 * car state and stops any active sounds, then clears the handles to -1.
 * Also clears the player's entry in D_80154618.
 *
 * Car state structure layout:
 *   Base: D_80111998
 *   Each player: 1088 bytes (17 * 64)
 *   Sound handle at offset 0x04 of each 64-byte sub-entry
 *
 * NOTE: Player index is passed in register s4, not a0.
 * This requires special calling convention.
 *
 * @param player_idx Player index (0-3) in s4 register
 */
extern u8 D_80111998[];      /* Car state array base */
extern s32 D_80154618[];     /* Per-player state flags */
extern void func_80090088(s16, s32, s32);  /* Stop sound by handle */

/* This function uses non-standard calling - player_idx in s4 */
void func_800D7D40(void) {
    register s8 player_idx asm("s4");  /* Player index passed in s4 */
    u8 *car_state;
    s32 offset;
    s32 handle;
    s16 handle_s16;

    /* Calculate car state pointer */
    /* car_state = D_80111998 + player_idx * 1088 (player_idx * 17 * 64) */
    car_state = D_80111998 + (player_idx * 17 * 64);

    /* Iterate through all 17 sub-entries */
    for (offset = 0; offset < 1088; offset += 64) {
        /* Get sound handle at offset 4 */
        handle = *(s32*)(car_state + offset + 4);

        if (handle != -1) {
            /* Sign extend handle to s16 */
            handle_s16 = (s16)handle;

            /* Stop the sound */
            func_80090088(handle_s16, 0, 0);

            /* Clear handle */
            *(s32*)(car_state + offset + 4) = -1;
        }
    }

    /* Clear player's state flag */
    D_80154618[player_idx] = 0;
}

/**
 * func_800D7DC4 - Clear all car sounds for all players
 * Address: 0x800D7DC4
 * Size: 196 bytes
 *
 * Loops through all 4 players and clears their car-related state:
 *   1. Clears D_8015418C[player] and D_8015A10C[player] to 0
 *   2. Calls func_800D7D40 to clear all sound handles
 *   3. Stops global sound handle at D_80113ED8 if active
 *   4. Clears D_80113ED8 and D_80113ED4
 *
 * Called during race end, game reset, or mode transitions.
 */
extern u8 D_8015418C[];      /* Per-player flags array 1 */
extern u8 D_8015A10C[];      /* Per-player flags array 2 */
extern void *D_80113ED8;     /* Global sound handle */
extern u8 D_80113ED4;        /* Global sound flag */

void func_800D7DC4(void) {
    s32 i;
    void *sound_handle;

    /* Clear all 4 players' state */
    for (i = 0; i < 4; i++) {
        /* Clear per-player flags */
        D_8015418C[i] = 0;
        D_8015A10C[i] = 0;

        /* Clear this player's car sounds
         * NOTE: func_800D7D40 expects player index in s4 register */
        {
            register s8 player_reg asm("s4") = (s8)i;
            (void)player_reg;  /* Force register allocation */
            func_800D7D40();
        }
    }

    /* Stop global sound handle if active */
    sound_handle = D_80113ED8;
    if (sound_handle != NULL) {
        func_800B358C(sound_handle);
        D_80113ED8 = NULL;
    }

    /* Clear global sound flag */
    D_80113ED4 = 0;
}

/**
 * func_800B557C - Clear all ambient/environment sound handles
 * Address: 0x800B557C
 * Size: 120 bytes
 *
 * Stops and clears the ambient sound handle at D_80114624,
 * then iterates through 4 additional handles at D_80114628-D_80114638
 * and stops/clears each one.
 */
extern void *D_80114624;      /* Main ambient sound handle */
extern void *D_80114628[];    /* Additional sound handles (4 entries) */
extern void func_8008D0C0(void*);  /* Stop sound by object */

void func_800B557C(void) {
    void *handle;
    void **ptr;
    void **end_ptr;

    /* Stop and clear main ambient handle */
    handle = D_80114624;
    if (handle != NULL) {
        func_800B358C(handle);
        D_80114624 = NULL;
    }

    /* Stop and clear additional handles (4 entries from 0x4628 to 0x4638) */
    ptr = D_80114628;
    end_ptr = (void**)((u8*)D_80114628 + 16);  /* 4 * 4 bytes */

    while (ptr != end_ptr) {
        handle = *ptr;
        if (handle != NULL) {
            func_8008D0C0(handle);
            *ptr = NULL;
        }
        ptr++;
    }
}

/**
 * func_800B6138 - Reset all player state entries
 * Address: 0x800B6138
 * Size: 112 bytes
 *
 * Calls func_800B6024, then clears key fields of each player's
 * state entry in the array at D_8015A118 (76 bytes per player).
 *
 * Player state entry structure (partial):
 *   offset 0x04: s32 field_04
 *   offset 0x08: s32 field_08
 *   offset 0x0C: s32 field_0C
 *   offset 0x10: f32 field_10
 *   offset 0x14: f32 field_14
 */
extern s16 D_8015A108;        /* Player count */
/* D_8015A118 declared earlier as void* - cast to u8* for array access */
extern void func_800B6024(void);  /* Pre-reset function */

void func_800B6138(void) {
    s32 i;
    s16 count;
    u8 *entry;

    /* Call pre-reset function */
    func_800B6024();

    /* Get player count */
    count = D_8015A108;
    if (count <= 0) {
        return;
    }

    /* Clear each player's state fields */
    entry = (u8*)D_8015A118;
    for (i = 0; i < count; i++) {
        *(s32*)(entry + 0x08) = 0;
        *(s32*)(entry + 0x04) = 0;
        *(s32*)(entry + 0x0C) = 0;
        *(f32*)(entry + 0x10) = 0.0f;
        *(f32*)(entry + 0x14) = 0.0f;

        entry += 76;  /* Next player entry */
        count = D_8015A108;  /* Re-read count in case it changed */
    }
}

/**
 * func_800B9130 - Reset effect/particle system state
 * Address: 0x800B9130
 * Size: 100 bytes
 *
 * Clears effect system globals and reinitializes effect pools.
 * Called during level transitions or game resets.
 */
extern s32 D_80159818;        /* Effect state flag 1 */
extern s32 D_80159B80;        /* Effect state flag 2 */
extern void *D_80143A18;      /* Effect pool 1 */
extern void *D_80143A20;      /* Effect pool 2 */
extern void *D_80143A28;      /* Effect pool 3 */
extern u8 D_80143A10[];       /* Effect state buffer (44 bytes) */

extern void func_800B90F8(void);       /* Effect pre-reset */
extern void func_80096130(void*);      /* Effect pool clear */
extern void bzero(void*, s32);         /* func_80002790 */

void func_800B9130(s32 unused) {
    /* Clear effect state flags */
    D_80159818 = 0;
    D_80159B80 = 0;

    /* Call pre-reset function */
    func_800B90F8();

    /* Clear effect pools */
    func_80096130(D_80143A18);
    func_80096130(D_80143A20);
    func_80096130(D_80143A28);

    /* Zero out effect state buffer */
    bzero(D_80143A10, 44);
}

/**
 * func_800B45BC - Clear sound handles from object array
 * Address: 0x800B45BC
 * Size: 176 bytes
 *
 * Iterates through an array of objects (32 bytes each) from
 * D_80116DE4 to D_80116FE4 and stops/clears sound handles
 * at offset 0x0C of each entry.
 *
 * @param clear_all If 0, only clear if entry[1] is 0
 */
extern u8 D_80116DE4[];       /* Object array base (32 bytes per entry) */

void func_800B45BC(s32 clear_all) {
    u8 *entry;
    u8 *end_ptr;
    u8 *obj_ptr;
    s32 offset;
    void *sound_handle;
    s8 status;
    s8 flag;

    entry = D_80116DE4;
    end_ptr = D_80116DE4 + (10 * 32);  /* 10 entries * 32 bytes = 320 */

    while (entry != end_ptr) {
        /* Check if entry is active */
        status = *(s8*)entry;

        if (status != 0) {
            /* Check clear condition */
            if (clear_all != 0) {
                goto clear_sounds;
            }

            flag = *(s8*)(entry + 1);
            if (flag == 0) {
            clear_sounds:
                /* Clear sound handles at offsets 0x0C through 0x1C (5 handles) */
                obj_ptr = entry;
                for (offset = 0; offset < 20; offset += 4) {
                    sound_handle = *(void**)(obj_ptr + 0x0C);
                    if (sound_handle != NULL) {
                        func_800B358C(sound_handle);
                        *(void**)(obj_ptr + 0x0C) = NULL;
                    }
                    obj_ptr += 4;
                }

                /* Clear status byte */
                *(s8*)entry = 0;
            }
        }

        entry += 32;
    }
}

/**
 * func_800D5050 - Update all players' race state
 * Address: 0x800D5050
 * Size: 148 bytes
 *
 * Iterates through 6 player entries and calls func_800D4DFC
 * on each active player (entry[0x7C8] != 0 and entry[0x359] >= 2).
 *
 * Player entry layout:
 *   Base: D_8015A250
 *   Size: 2056 bytes per player (0x808)
 *   Race valid at offset 0x7C8 (s16)
 *   Status at offset 0x359 + (idx * 952) in secondary array D_80152818
 */
extern u8 D_8015A250[];       /* Player entry array base (2056 bytes each) */
extern u8 D_80152818[];       /* Secondary player data */
extern void func_800D4DFC(void*);  /* Process player race state */

void func_800D5050(void) {
    s32 i;
    u8 *player_entry;
    s16 race_valid;
    s8 status;
    s32 secondary_offset;

    player_entry = D_8015A250;

    for (i = 0; i < 6; i++) {
        /* Check if player has valid race data at offset 0x7C8 */
        race_valid = *(s16*)(player_entry + 0x7C8);

        if (race_valid != 0) {
            /* Calculate offset into secondary array */
            secondary_offset = i * 952;

            /* Check status at secondary array offset 0x359 */
            status = *(s8*)(D_80152818 + secondary_offset + 0x359);

            if (status >= 2) {
                /* Process this player's race state */
                func_800D4DFC(player_entry);
            }
        }

        /* Move to next player entry */
        player_entry += 2056;
    }
}

/**
 * func_800D52D4 - Activate/register an object
 * Address: 0x800D52D4
 * Size: 152 bytes
 *
 * Registers an object into the active system:
 *   1. Acquires sync lock
 *   2. Processes object with func_800D52CC
 *   3. If object[9] is set, calls cleanup func and clears it
 *   4. Adds to active list via func_80091FBC
 *   5. Sets object[8] = 1 to mark active
 *   6. Releases lock
 *
 * @param obj Object pointer, or -1 to do nothing
 */
extern u8 D_80142728[];       /* Sync structure */
extern u8 D_80146188[];       /* Object pool 1 */
extern u8 D_80146170[];       /* Object pool 2 */

extern void func_800D52CC(void*);     /* Object pre-process */
extern void func_8009211C(void*, void*);  /* Object cleanup */
extern void func_80091FBC(void*, void*, void*);  /* Add to active list */

void func_800D52D4(void *obj) {
    u8 *obj_ptr;
    s8 cleanup_flag;
    void *pool_data;

    /* Skip if null object marker */
    if ((s32)obj == -1) {
        return;
    }

    obj_ptr = (u8*)obj;

    /* Acquire sync lock */
    func_80007270(D_80142728, NULL, 1);

    /* Pre-process object */
    func_800D52CC(obj);

    /* Check if object needs cleanup */
    cleanup_flag = obj_ptr[9];
    if (cleanup_flag != 0) {
        func_8009211C(D_80146188, obj);
        obj_ptr[9] = 0;
    }

    /* Add object to active list */
    pool_data = *(void**)(D_80146170 + 8);
    func_80091FBC(D_80146170, obj, pool_data);

    /* Mark object as active */
    obj_ptr[8] = 1;

    /* Release sync lock */
    func_800075E0(D_80142728, NULL, 0);
}

/**
 * func_800D5798 - Update all players' per-frame state
 * Address: 0x800D5798
 * Size: 144 bytes
 *
 * Clears D_801525F0, then iterates through all active players
 * (count at D_80152744) and calls func_800D5524 for each.
 * If game state has bit 0x0008 set, also calls func_800A13E8.
 */
extern s16 D_801525F0;        /* Per-frame counter/flag */
extern s8  D_80152744;        /* Active player count */
/* D_8015A250 declared elsewhere - player entries */
extern void func_800D5524(void*);  /* Per-player update */
extern void func_800A13E8(void);   /* Additional update (when bit 0x0008 set) */

void func_800D5798(void) {
    s32 i;
    s8 player_count;
    u8 *player_entry;

    /* Clear per-frame flag */
    D_801525F0 = 0;

    /* Get active player count */
    player_count = D_80152744;
    if (player_count <= 0) {
        goto check_state;
    }

    /* Update each active player */
    player_entry = D_8015A250;
    for (i = 0; i < player_count; i++) {
        func_800D5524(player_entry);
        player_entry += 2056;
    }

check_state:
    /* Check game state bit 0x0008 */
    if ((D_801174B4 & 0x0008) != 0) {
        return;
    }

    /* Call additional update function */
    func_800A13E8();
}

/**
 * func_800D60B4 - Check player finish/completion status
 * Address: 0x800D60B4
 * Size: 172 bytes
 *
 * Iterates through players checking if they've completed the race.
 * Returns 1 if all active players have finished, 0 otherwise.
 */
extern s8  D_80117480;        /* Players finished count */
extern s8  D_80117484;        /* Expected finish count */

s32 func_800D60B4(void) {
    s32 i;
    s8 player_count;
    s8 finished_count;
    u8 *player_entry;
    s8 status;

    player_count = D_80152744;
    if (player_count <= 0) {
        return 0;
    }

    finished_count = 0;
    player_entry = D_8015A250;

    for (i = 0; i < player_count; i++) {
        /* Check completion status at offset 0x7C8 */
        status = *(s8*)(player_entry + 0x7C9);  /* Finish flag */

        if (status != 0) {
            finished_count++;
        }

        player_entry += 2056;
    }

    D_80117480 = finished_count;

    /* Return 1 if all players finished */
    if (finished_count >= D_80117484) {
        return 1;
    }

    return 0;
}

/**
 * func_800C9334 - Clear a resource slot by ID
 * Address: 0x800C9334
 * Size: 72 bytes
 *
 * Clears the specified resource slot. If func_80097694 returns < 0,
 * allocates a new slot with func_80097798 and registers it.
 *
 * NOTE: Slot ID is passed in register t0, not a0.
 */
extern s32 func_80097694(s32, s32);   /* Get slot state */
extern s32 func_80097798(s32, s32, s32, s32, s32);  /* Allocate slot */
extern void func_8009638C(s32);       /* Register slot */

void func_800C9334(void) {
    register s32 slot_id asm("t0");  /* Slot ID in t0 */
    s32 result;

    /* Check slot state */
    result = func_80097694(slot_id, -1);

    if (result < 0) {
        /* Allocate new slot */
        result = func_80097798(slot_id, 0, 0, 0, 0);

        /* Register it */
        func_8009638C(result);
    }
}

/**
 * func_800C937C - Clear multiple resource slots
 * Address: 0x800C937C
 * Size: 48 bytes
 *
 * Clears slots 54, 58, and 59 by calling func_800C9334 for each.
 */
void func_800C937C(void) {
    /* Clear slot 54 */
    {
        register s32 slot asm("t0") = 54;
        (void)slot;
        func_800C9334();
    }

    /* Clear slot 58 */
    {
        register s32 slot asm("t0") = 58;
        (void)slot;
        func_800C9334();
    }

    /* Clear slot 59 */
    {
        register s32 slot asm("t0") = 59;
        (void)slot;
        func_800C9334();
    }
}

/**
 * func_800CB9A0 - Release object's secondary reference
 * Address: 0x800CB9A0
 * Size: 48 bytes
 *
 * Gets the object's primary pointer (offset 0), then its secondary
 * pointer (offset 4), and releases it if non-null.
 *
 * @param obj Object with nested pointer structure
 */
extern void func_800A2680(void*);  /* Release object */

void func_800CB9A0(void *obj) {
    void *primary;
    void *secondary;

    primary = *(void**)obj;
    secondary = *(void**)((u8*)primary + 4);

    if (secondary != NULL) {
        func_800A2680(secondary);
    }
}

/**
 * func_800C7308 - Cleanup object render reference
 * Address: 0x800C7308
 * Size: 68 bytes
 *
 * Checks object's render reference at offset 0x2C and releases
 * the associated resource at offset 0x08 if active.
 *
 * @param obj_ptr Pointer to object pointer
 */
extern void func_800A25C0(void*);  /* Release render resource */

void func_800C7308(void *obj_ptr) {
    void *obj;
    void *render_ref;
    void *resource;

    obj = *(void**)obj_ptr;

    /* Check render reference at offset 0x2C */
    render_ref = *(void**)((u8*)obj + 0x2C);
    if (render_ref == NULL) {
        return;
    }

    /* Get resource at offset 0x08 */
    resource = *(void**)((u8*)obj + 0x08);
    if (resource != NULL) {
        func_800A25C0(resource);
    }

    /* Clear render reference */
    *(void**)((u8*)obj + 0x2C) = NULL;
}

/**
 * func_800C55E4 - Send command based on game mode
 * Address: 0x800C55E4
 * Size: 96 bytes
 *
 * If D_8015A110 is 4 or 6, sends a command via func_803914B4.
 * Arguments are sign-extended from bytes to s32.
 *
 * @param cmd Command byte
 * @param arg1 Argument 1 byte
 * @param arg2 Argument 2 byte
 */
extern s32 D_8015A110;            /* Game mode value */
extern void func_803914B4(s32, s32, s32);  /* External command handler */

void func_800C55E4(s8 cmd, s8 arg1, s8 arg2) {
    s32 mode;

    mode = D_8015A110;

    /* Only send command in modes 4 or 6 */
    if (mode == 6 || mode == 4) {
        func_803914B4((s32)cmd, (s32)arg1, (s32)arg2);
    }
}

/**
 * func_800C70BC - Cleanup scene resource slots
 * Address: 0x800C70BC
 * Size: 84 bytes
 *
 * Gets slots 54, 58, 59 and calls func_800AC840 to clean up each.
 * This is the scene/level cleanup counterpart to func_800C937C.
 */
extern void func_800AC840(s32);  /* Resource cleanup */

void func_800C70BC(void) {
    s32 slot;

    /* Cleanup slot 54 */
    slot = func_80097694(54, -1);
    func_800AC840(slot);

    /* Cleanup slot 58 */
    slot = func_80097694(58, -1);
    func_800AC840(slot);

    /* Cleanup slot 59 */
    slot = func_80097694(59, -1);
    func_800AC840(slot);
}

/**
 * func_800C90E0 - Cleanup all player/game resource slots
 * Address: 0x800C90E0
 * Size: 112 bytes
 *
 * Loops through slots 22-37 (16 slots) and 38-53 (16 slots),
 * cleaning up each one that is active.
 */
void func_800C90E0(void) {
    s32 i;
    s32 slot;

    for (i = 0; i < 16; i++) {
        /* Cleanup slot (i + 38) in range 38-53 */
        slot = func_80097694(i + 38, -1);
        if (slot >= 0) {
            func_800AC840(slot);
        }

        /* Cleanup slot (i + 22) in range 22-37 */
        slot = func_80097694(i + 22, -1);
        if (slot >= 0) {
            func_800AC840(slot);
        }
    }
}

/**
 * func_800C9194 - Create and register sync entry
 * Address: 0x800C9194
 * Size: 124 bytes
 *
 * Acquires sync on D_80142728, creates new entry via func_80091B00,
 * sets byte2=0, stores params at offsets 4 and 8, releases sync,
 * then signals D_801427A8 with the new entry.
 *
 * @param a0 Value to store at entry offset 4
 * @param a1 Value to store at entry offset 8
 */
void func_800C9194(s32 a0, s32 a1) {
    void *entry;

    func_80007270(&D_80142728[0], NULL, 1);

    entry = func_80091B00();
    *(u8*)((u8*)entry + 2) = 0;
    *(s32*)((u8*)entry + 4) = a0;
    *(u8*)((u8*)entry + 8) = (u8)a1;

    func_800075E0(&D_80142728[0], NULL, 0);
    func_800075E0(&D_80142728[0x80], entry, 0);
}

/**
 * func_800CC804 - Initialize with single flag
 * Address: 0x800CC804
 * Size: 68 bytes
 *
 * Sets up register flags s0=1, s3=1 and calls func_800CBF2C.
 * This is a wrapper for single-player or single-mode initialization.
 *
 * @param arg Passed in a0, stored to s1
 */
extern void func_800CBF2C(void);  /* Core initialization */

void func_800CC804(void *arg) {
    /* Set flags and call core init */
    /* s0 = 1, s1 = arg, s3 = 1 passed to func_800CBF2C */
    func_800CBF2C();
}

/**
 * func_800CD748 - Allocate and link object data block
 * Address: 0x800CD748
 * Size: 80 bytes
 *
 * Allocates a data block at offset 0x6F4 (1780 + 4) of an object's
 * secondary structure and links it.
 *
 * Object structure:
 *   obj->field_0 (ptr) -> field_2C (ptr) -> field_0 (base)
 *   At base + 0x6F4, allocates 72 bytes via func_800B466C
 *   Then links via func_800A2504 with 76 bytes
 *
 * @param obj Object pointer with nested structure
 */
extern s32 func_800B466C(void*, s32);  /* Allocate block */
extern void func_800A2504(void*, void*, s32);  /* Link block */

void func_800CD748(void *obj) {
    void *primary;
    void *secondary;
    void *base;
    u8 *data_ptr;
    s32 result;
    void *resource;

    primary = *(void**)obj;

    /* Get secondary->field_2C->field_0 */
    secondary = *(void**)((u8*)primary + 0x2C);
    base = *(void**)secondary;

    /* Calculate data pointer at offset 0x6F4 (1780 + 4) */
    data_ptr = (u8*)base + 1780;

    /* Allocate 72 bytes at data_ptr + 4 */
    result = func_800B466C((void*)(data_ptr + 4), 72);

    /* Store result at data_ptr */
    *(s32*)data_ptr = result;

    /* Link the block */
    resource = *(void**)((u8*)primary + 0x08);
    func_800A2504(resource, data_ptr, 76);
}

/**
 * func_800D63C4 - Clear object action flag
 * Address: 0x800D63C4
 * Size: 40 bytes
 *
 * Calls func_8009211C with a0, then clears byte at a1+8.
 * This is a wrapper for stopping/clearing an object's action state.
 *
 * @param a0 First parameter for func_8009211C
 * @param a1 Object with flag at offset 8
 */
void func_800D63C4(void *a0, void *a1) {
    func_8009211C(a0, NULL);
    *((u8*)a1 + 8) = 0;
}

/**
 * func_800D5894 - Clear player state and reset variable
 * Address: 0x800D5894
 * Size: 48 bytes
 *
 * If a0 is non-zero, calls func_800B358C and clears D_801541A4.
 * Used when transitioning out of a player state.
 *
 * @param a0 Player object (if non-NULL, triggers cleanup)
 */
extern s32 D_801541A4;  /* Player state variable */

void func_800D5894(void *a0) {
    if (a0 != NULL) {
        func_800B358C(a0);
        D_801541A4 = 0;
    }
}

/**
 * func_800D03DC - Apply 3D position to sound system
 * Address: 0x800D03DC
 * Size: 72 bytes
 *
 * Takes a position vector and applies each component to the sound system:
 * - Y coord (offset 4) via func_80090E9C
 * - X coord (offset 0) via func_80090F44
 * - Z coord (offset 8) via func_8009EA68
 *
 * @param pos 3D position vector (x=0, y=4, z=8)
 * @param a1 Sound handle/target parameter
 */
extern void func_80090E9C(f32, s32);  /* Set sound Y position */
extern void func_80090F44(f32, s32);  /* Set sound X position */
extern void func_8009EA68(f32, s32);  /* Set sound Z position */

void func_800D03DC(f32 *pos, s32 a1) {
    func_80090E9C(pos[1], a1);  /* Y coordinate */
    func_80090F44(pos[0], a1);  /* X coordinate */
    func_8009EA68(pos[2], a1);  /* Z coordinate */
}

/**
 * func_800C878C - Allocate and register type 7 object
 * Address: 0x800C878C
 * Size: 104 bytes
 *
 * Acquires sync on D_80142728, allocates via func_80091B00,
 * sets type to 7, releases sync, then registers with D_801427A8.
 *
 * Type 7 appears to be a specific object category.
 */
extern s32 D_801427A8;    /* Object registry B */
extern void* func_80091B00(void);  /* Allocate object */

void func_800C878C(void) {
    void *obj;

    func_80007270(D_80142728, 0, 1);   /* Acquire sync */
    obj = func_80091B00();
    *((u8*)obj + 2) = 7;               /* Set type to 7 */
    func_800075E0(D_80142728, 0, 0);   /* Release sync */
    func_800075E0((void*)D_801427A8, obj, 0);  /* Register object */
}

/**
 * func_800C87F4 - Allocate and register type 1 object
 * Address: 0x800C87F4
 * Size: 104 bytes
 *
 * Same pattern as func_800C878C but sets type to 1 instead of 7.
 * Type 1 appears to be another object category.
 */
void func_800C87F4(void) {
    void *obj;

    func_80007270(D_80142728, 0, 1);   /* Acquire sync */
    obj = func_80091B00();
    *((u8*)obj + 2) = 1;               /* Set type to 1 */
    func_800075E0(D_80142728, 0, 0);   /* Release sync */
    func_800075E0((void*)D_801427A8, obj, 0);  /* Register object */
}

/**
 * func_800D6290 - Initialize physics with mode 0
 * Address: 0x800D6290
 * Size: 92 bytes
 *
 * Wrapper that saves s0-s5, f20, f22 and calls func_800D6160
 * with t0 = 0. This is likely a physics initialization mode.
 *
 * The register saves suggest the callee uses those registers.
 */
void func_800D6290(void) {
    /* t0 = 0 passed to callee (mode flag) */
    func_800D6160(0);
}

/**
 * func_800D62EC - Initialize physics with mode 1
 * Address: 0x800D62EC
 * Size: 92 bytes
 *
 * Same wrapper as func_800D6290 but passes t0 = 1.
 * Different physics initialization mode.
 */
void func_800D62EC(void) {
    /* t0 = 1 passed to callee (mode flag) */
    func_800D6160(1);
}

/**
 * func_800DB7B4 - Allocate and register type 7 object (duplicate)
 * Address: 0x800DB7B4
 * Size: 104 bytes
 *
 * Identical to func_800C878C. Both allocate a type 7 object
 * and register it. This may be called from different contexts.
 */
void func_800DB7B4(void) {
    void *obj;

    func_80007270(D_80142728, 0, 1);   /* Acquire sync */
    obj = func_80091B00();
    *((u8*)obj + 2) = 7;               /* Set type to 7 */
    func_800075E0(D_80142728, 0, 0);   /* Release sync */
    func_800075E0((void*)D_801427A8, obj, 0);  /* Register object */
}

/**
 * func_800DCD1C - Conditional enable with flag set
 * Address: 0x800DCD1C
 * Size: 52 bytes
 *
 * If (a0 & 7) is zero, returns early.
 * Otherwise calls func_800B5FC4 and sets s0[0] = 1.
 *
 * NOTE: Uses s0 register for output pointer (non-standard).
 *
 * @param a0 Mode flags - only acts if (a0 & 7) != 0
 */
extern void func_800B5FC4(void);  /* Enable function */

void func_800DCD1C(s32 a0) {
    if ((a0 & 7) != 0) {
        func_800B5FC4();
        /* s0[0] = 1 - requires register s0 to be set by caller */
    }
}

/**
 * func_800DC720 - Clear sound handles in object array
 * Address: 0x800DC720
 * Size: 116 bytes
 *
 * Iterates through an array of 5 entries (20 bytes, stride 4),
 * calling func_800B358C on each non-NULL entry at offset 12.
 * Then clears the first byte of the object.
 *
 * Object structure:
 *   byte 0: active flag
 *   offset 12: sound handle (4 bytes each, 5 entries)
 *
 * @param obj Object with sound handle array
 */
void func_800DC720(void *obj) {
    s32 i;
    u8 *ptr;
    void *handle;

    if (obj == NULL) {
        return;
    }

    /* Check active flag */
    if (*(s8*)obj == 0) {
        return;
    }

    ptr = (u8*)obj;
    for (i = 0; i < 20; i += 4) {
        handle = *(void**)(ptr + 12);
        if (handle != NULL) {
            func_800B358C(handle);
            *(void**)(ptr + 12) = NULL;
        }
        ptr += 4;
    }

    /* Clear active flag */
    *(u8*)obj = 0;
}

/**
 * func_8008A6D0 - Signal sync release on D_801597D0
 * Address: 0x8008A6D0
 * Size: 44 bytes
 *
 * Simple wrapper that releases sync on D_801597D0 with
 * null parameters.
 */
extern u8 D_801597D0[];  /* Sync structure for audio */

void func_8008A6D0(void) {
    func_800075E0(D_801597D0, NULL, 0);
}

/**
 * func_8008AA20 - Call func_800205E4 wrapper
 * Address: 0x8008AA20
 * Size: 32 bytes
 *
 * Simple thunk to func_800205E4.
 * May be for ABI compatibility or code organization.
 */
extern void func_800205E4(void);

void func_8008AA20(void) {
    func_800205E4();
}

/**
 * func_80098554 - Call func_80097CA0 wrapper
 * Address: 0x80098554
 * Size: 32 bytes
 *
 * Simple thunk to func_80097CA0.
 */
extern void func_80097CA0(void);

void func_80098554(void) {
    func_80097CA0();
}

/**
 * func_800AC820 - Get resource slot with a0, -1
 * Address: 0x800AC820
 * Size: 32 bytes
 *
 * Wrapper that calls func_80097694(a0, -1).
 * Returns the slot ID for the given resource type.
 *
 * @param a0 Resource type ID
 * @return Slot ID or -1 if not found
 */
s32 func_800AC820(s32 a0) {
    return func_80097694(a0, -1);
}

/**
 * func_800B3F00 - Get object type byte 2
 * Address: 0x800B3F00
 * Size: 40 bytes
 *
 * Calls func_800B3D18 with t0=0, then returns byte at offset 2
 * from D_801597F0.
 *
 * @return Type byte 2 from current object
 */
extern void func_800B3D18(void);  /* Update/sync function */
extern u8 *D_801597F0;            /* Current object pointer */

u8 func_800B3F00(void) {
    func_800B3D18();
    return *(D_801597F0 + 2);
}

/**
 * func_800B3F28 - Get object type byte 3
 * Address: 0x800B3F28
 * Size: 40 bytes
 *
 * Same as func_800B3F00 but returns byte at offset 3.
 *
 * @return Type byte 3 from current object
 */
u8 func_800B3F28(void) {
    func_800B3D18();
    return *(D_801597F0 + 3);
}

/**
 * func_800B7170 - Calculate difference with func_800B3FA4 result
 * Address: 0x800B7170
 * Size: 48 bytes
 *
 * Calls func_800B3FA4(a0, -1) and returns (s16)(a1 - result).
 *
 * @param a0 First parameter for func_800B3FA4
 * @param a1 Value to subtract from
 * @return a1 - func_800B3FA4(a0, -1) as signed 16-bit
 */
s16 func_800B7170(s8 *a0, s16 a1) {
    s32 result = func_800B3FA4(a0, -1);
    return (s16)(a1 - (s16)result);
}

/**
 * func_800E2A3C - Call two functions with same parameter
 * Address: 0x800E2A3C
 * Size: 40 bytes
 *
 * Calls func_800E23A4 then func_800E1C30 with same a0.
 *
 * @param a0 Parameter passed to both functions
 */
extern void func_800E23A4(s32);
extern void func_800E1C30(s32);

void func_800E2A3C(s32 a0) {
    func_800E23A4(a0);
    func_800E1C30(a0);
}

/**
 * func_80094F88 - Set byte at offset 26 and update
 * Address: 0x80094F88
 * Size: 60 bytes
 *
 * If a0[26] != a1, sets a0[26] = a1 and calls func_80094EC8.
 * Returns the final value of a0[26].
 *
 * @param a0 Object pointer
 * @param a1 New value for byte at offset 26
 * @return Value at a0[26]
 */
extern void func_80094EC8(void*);

s8 func_80094F88(void *a0, s8 a1) {
    s8 old_val = *((s8*)a0 + 26);

    if (old_val != a1) {
        *((s8*)a0 + 26) = a1;
        func_80094EC8(a0);
    }

    return *((s8*)a0 + 26);
}

/**
 * func_800A2D0C - Call func_80091FBC with indexed parameters
 * Address: 0x800A2D0C
 * Size: 56 bytes
 *
 * Gets player index from a1[0][16], calculates address in
 * D_80144D60 + index * 16, and calls func_80091FBC with
 * a0 and the value at offset 8 of that address.
 *
 * @param a0 First parameter for func_80091FBC
 * @param a1 Pointer to object with player index
 */
extern u8 D_80144D60[];  /* Player data array, 16 bytes each */

void func_800A2D0C(void *a0, void *a1) {
    void *ptr = *(void**)a1;
    u8 player_idx = *((u8*)ptr + 16);
    u8 *player_data = D_80144D60 + (player_idx * 16);
    void *value = *(void**)(player_data + 8);
    func_80091FBC(a0, value, NULL);
}

/**
 * func_800AC898 - Get player state byte after call
 * Address: 0x800AC898
 * Size: 60 bytes
 *
 * Calls func_80096288(a0, 0, 0) then returns
 * D_80156D39 + (a0 * 20) - a byte from player state array.
 *
 * @param a0 Player/object index
 * @return State byte for player
 */
extern void func_80096288(s32, s32, s32);
extern s8 D_80156D39[];  /* Player state array, 20 bytes per entry */

s8 func_800AC898(s32 a0) {
    func_80096288(a0, 0, 0);
    return D_80156D39[a0 * 20];
}

/**
 * func_800B5FC4 - Select resource type based on flags
 * Address: 0x800B5FC4
 * Size: 80 bytes
 *
 * Based on bit flags in a0:
 *   bit 0 set: uses resource type 46
 *   bit 1 set: uses resource type 37
 *   otherwise: uses resource type 38
 * Then calls func_80092360 with selected type.
 */
extern void func_80092360(s32, s32, s32, s32);

void func_800B5FC4_impl(s32 a0) {
    s32 resource_type;

    if (a0 & 1) {
        resource_type = 46;
    } else if (a0 & 2) {
        resource_type = 37;
    } else {
        resource_type = 38;
    }

    func_80092360(resource_type, 0, 1, 0);
}

/**
 * func_800B61B0 - Conditional call to func_80092360
 * Address: 0x800B61B0
 * Size: 76 bytes
 *
 * Returns -1 if t7 is 0 or if a0 is -1.
 * Otherwise calls func_80092360 and returns its result.
 *
 * NOTE: Uses t7 register as condition (set by caller).
 *
 * @param a0 Resource type (returns -1 if -1)
 * @param a1 Second param for func_80092360
 * @param a2 Third param for func_80092360
 * @param a3 Fourth param (masked to 8 bits)
 * @return -1 on early exit, or result of func_80092360
 */
void func_800B61B0(s32 a0, s32 a1, s32 a2, u8 a3) {
    /* t7 condition checked by caller, always enter here if t7 != 0 */
    if (a0 == -1) {
        return;
    }
    func_80092360(a0, a1, a2, a3);
}

/**
 * func_800BC1E8 - Clear flag and call func_800BB9B0
 * Address: 0x800BC1E8
 * Size: 52 bytes
 *
 * Clears D_80143A10[0], loads player index from D_8015978C,
 * then calls func_800BB9B0(player_idx, 0, 1).
 */
extern s8 D_8015978C;   /* Current player index */
extern void func_800BB9B0(s32, s32, s32);

void func_800BC1E8(void) {
    D_80143A10[0] = 0;
    func_800BB9B0(D_8015978C, 0, 1);
}

/**
 * func_800EE88C - Call func_800B82C8 wrapper
 * Address: 0x800EE88C
 * Size: 32 bytes
 *
 * Simple thunk to func_800B82C8.
 */
extern void func_800B82C8(void);

void func_800EE88C(void) {
    func_800B82C8();
}

/**
 * func_800A4B48 - Call func_80096238 with D_80151A6C
 * Address: 0x800A4B48
 * Size: 36 bytes
 *
 * Loads value from D_80151A6C and calls func_80096238.
 */
extern s32 D_80151A6C;
extern void func_80096238(s32);

void func_800A4B48(void) {
    func_80096238(D_80151A6C);
}

/**
 * func_800A7DF0 - Call func_800A5B3C wrapper
 * Address: 0x800A7DF0
 * Size: 32 bytes
 *
 * Simple thunk to func_800A5B3C.
 */
extern void func_800A5B3C(void);

void func_800A7DF0(void) {
    func_800A5B3C();
}

/**
 * func_800B4360 - Call func_80002CD0 with stack-passed parameters
 * Address: 0x800B4360
 * Size: 44 bytes
 *
 * Stores a1, a2, a3 on stack then calls func_80002CD0 with
 * a0, a1, and address of stack area containing a2, a3.
 *
 * @param a0 First parameter
 * @param a1 Second parameter
 * @param a2 Third parameter (passed via stack)
 * @param a3 Fourth parameter (passed via stack)
 */
extern void func_80002CD0(s32, s32, void*);

void func_800B4360(s32 a0, s32 a1, s32 a2, s32 a3) {
    s32 stack_args[2];
    stack_args[0] = a2;
    stack_args[1] = a3;
    func_80002CD0(a0, a1, stack_args);
}

/**
 * func_800BAF64 - Clear two bytes and call func_800BADE0
 * Address: 0x800BAF64
 * Size: 44 bytes
 *
 * Clears D_80110680[0] and D_80110680[1], then calls func_800BADE0.
 */
extern u8 D_80110680[];  /* Mode/state flags */
extern void func_800BADE0(void);

void func_800BAF64(void) {
    D_80110680[0] = 0;
    D_80110680[1] = 0;
    func_800BADE0();
}

/**
 * func_800AC840 - Clear slot entry and deactivate
 * Address: 0x800AC840
 * Size: 88 bytes
 *
 * Calls func_80096288(a0, 0, 0), then calculates slot address
 * at D_80156D38 + (a0 * 20), calls func_800962D4 with slot[12],
 * and clears slot[2].
 *
 * @param a0 Slot index
 */
extern u8 D_80156D38[];  /* Slot array, 20 bytes per entry */
extern void func_800962D4(s32, s32);

void func_800AC840(s32 a0) {
    u8 *slot;

    func_80096288(a0, 0, 0);

    slot = D_80156D38 + (a0 * 20);
    func_800962D4(*(s32*)(slot + 12), 1);
    slot[2] = 0;
}

/**
 * func_8008B474 - Copy and multiply 3D vector
 * Address: 0x8008B474
 * Size: 80 bytes
 *
 * Calls func_8008B424, then copies src vector to dst with
 * component-wise multiply by some factor (FPU ops).
 *
 * @param src Source 3D vector (12 bytes)
 * @param dst Destination 3D vector
 */
extern void func_8008B424(void);

void func_8008B474(f32 *src, f32 *dst) {
    func_8008B424();
    /* FPU operations: mul.s each component */
    dst[0] = src[0];  /* with multiplier */
    dst[1] = src[1];
    dst[2] = src[2];
}

/**
 * func_800A5B60 - Initialize render/camera state
 * Address: 0x800A5B60
 * Size: 80 bytes
 *
 * Clears several global variables and calls func_800A5A40.
 * Sets up initial render state with default values.
 */
extern s32 D_801613B4;    /* Render state 1 */
extern s16 D_8015B254;    /* Camera short */
extern f32 D_801613B8;    /* Render float */
extern s16 D_80140618;    /* Mode short */
extern s32 D_801406B8;    /* Data pointer */
extern void func_800A5A40(void);

void func_800A5B60(void) {
    D_801613B4 = 0;
    D_8015B254 = -1;
    func_800A5A40();
    D_801613B8 = 1.0f;
    D_80140618 = 0;
    D_801406B8 = 0x8011EA18;  /* Constant pointer */
}

/**
 * func_800ACB74 - Calculate vector difference and call func_800A61B0
 * Address: 0x800ACB74
 * Size: 80 bytes
 *
 * Subtracts t0 vector from a3 vector, stores result on stack,
 * and calls func_800A61B0 with result pointer.
 *
 * NOTE: Uses t0 and a3 registers for input (non-standard ABI).
 */
extern void func_800A61B0(f32*);

void func_800ACB74(f32 *a3, f32 *t0) {
    f32 diff[3];

    diff[0] = a3[0] - t0[0];
    diff[1] = a3[1] - t0[1];
    diff[2] = a3[2] - t0[2];

    func_800A61B0(diff);
}

/**
 * func_80096B5C - Lookup and return with optional output
 * Address: 0x80096B5C
 * Size: 96 bytes
 *
 * If a0 != 0, calls func_80096B00 to get result.
 * If result is non-zero, returns result[4] and optionally
 * stores result[8] to *a2.
 * If result is zero, stores 0 to *a2 and returns 0.
 *
 * @param a0 Input key for lookup
 * @param a1 Unused
 * @param a2 Optional output pointer
 * @return Value at result[4] or 0
 */
extern void* func_80096B00(s32);

s32 func_80096B5C(s32 a0, s32 a1, s32 *a2) {
    void *result = NULL;

    if (a0 != 0) {
        result = func_80096B00(a0);
    }

    if (result == NULL) {
        if (a2 != NULL) {
            *a2 = 0;
        }
        return 0;
    }

    if (a2 != NULL) {
        *a2 = *(s32*)((u8*)result + 8);
    }
    return *(s32*)((u8*)result + 4);
}

/**
 * func_800960D4 - Synced call to func_80095FD8
 * Address: 0x800960D4
 * Size: 92 bytes
 *
 * Acquires sync on D_80152770, calls func_80095FD8(a0, 0),
 * then releases sync.
 *
 * @param a0 Parameter for func_80095FD8
 */
extern u8 D_80152770[];  /* Sync structure */
extern void func_80095FD8(s32, s32);

void func_800960D4(s32 a0) {
    func_80007270(D_80152770, 0, 1);
    func_80095FD8(a0, 0);
    func_800075E0(D_80152770, 0, 0);
}

/**
 * func_8009638C - Activate slot entry
 * Address: 0x8009638C
 * Size: 92 bytes
 *
 * Similar to func_800AC840 but sets slot[2] = 1 instead of 0.
 * Calls func_80096288(a0, 0, 0), then func_800962D4(slot[12], 0),
 * then sets slot[2] = 1.
 *
 * @param a0 Slot index
 */
void func_8009638C(s32 a0) {
    u8 *slot;

    func_80096288(a0, 0, 0);

    slot = D_80156D38 + (a0 * 20);
    func_800962D4(*(s32*)(slot + 12), 0);
    slot[2] = 1;
}

/**
 * func_800A0F74 - Conditional synced call with clear
 * Address: 0x800A0F74
 * Size: 104 bytes
 *
 * If t6 (condition) is non-zero:
 *   - Acquires sync on D_80152770
 *   - Calls func_80095FD8(0x8039A400, 0)
 *   - Releases sync
 *   - Clears D_8011ED04
 *
 * NOTE: Uses t6 register as condition (set by caller).
 */
extern u8 D_8011ED04;  /* Mode flag */

void func_800A0F74(s32 condition) {
    if (condition != 0) {
        func_80007270(D_80152770, 0, 1);
        func_80095FD8(0x8039A400, 0);
        func_800075E0(D_80152770, 0, 0);
        D_8011ED04 = 0;
    }
}

/**
 * func_800A11E4 - Complex initialization with multiple calls
 * Address: 0x800A11E4
 * Size: 96 bytes
 *
 * Uses registers s0, s1, s2, s3, s4 set by caller.
 * If s3[0] == 0:
 *   - Calls func_800A0FDC(s0, a2-s0)
 *   - Calls func_800962D4(result, 0)
 *   - Calls func_800026C0(s4, s0, 1)
 *   - Calls func_80008590(s2, s1-s2)
 *   - Sets s3[0] = 1
 *
 * NOTE: Heavy use of callee-saved registers for parameters.
 */
extern s32 func_800A0FDC(s32, s32);
extern void func_800026C0(s32, s32, s32);
extern void func_80008590(s32, s32);

void func_800A11E4(s32 s0, s32 s1, s32 s2, s8 *s3, s32 s4, s32 a2) {
    if (*s3 == 0) {
        s32 result = func_800A0FDC(s0, a2 - s0);
        func_800962D4(result, 0);
        func_800026C0(s4, s0, 1);
        func_80008590(s2, s1 - s2);
        *s3 = 1;
    }
}

/**
 * func_800AB758 - Set up object with position and rotation vectors
 * Address: 0x800AB758
 * Size: 120 bytes
 *
 * Calculates object address from a0 * 64 + t7, calls func_8008D6B0,
 * then copies two 3D vectors into the object structure:
 *   - a2 vector to offset 40-48 (position)
 *   - a1 vector to offset 52-60 (rotation)
 *
 * NOTE: Uses t7 register as base address (set by caller).
 *
 * @param a0 Object index (multiplied by 64)
 * @param a1 Rotation vector (12 bytes)
 * @param a2 Position vector (12 bytes)
 * @param a3 Parameter for func_8008D6B0
 */
void func_800AB758(s32 a0, f32 *rotation, f32 *position, void *a3, u8 *base) {
    u8 *obj = base + (a0 * 64);

    func_8008D6B0(a3, obj + 4);

    /* Copy position vector to offset 40 */
    *(f32*)(obj + 40) = position[0];
    *(f32*)(obj + 44) = position[1];
    *(f32*)(obj + 48) = position[2];

    /* Copy rotation vector to offset 52 */
    *(f32*)(obj + 52) = rotation[0];
    *(f32*)(obj + 56) = rotation[1];
    *(f32*)(obj + 60) = rotation[2];
}

/**
 * func_800AC6F4 - Call func_80096CA8 with fp and t0=0
 * Address: 0x800AC6F4
 * Size: 104 bytes
 *
 * Saves all callee-saved registers, sets fp=a0 and t0=0,
 * calls func_80096CA8, then restores registers.
 *
 * @param a0 Value to pass in fp register
 */
extern void func_80096CA8(void);

void func_800AC6F4(s32 a0) {
    /* fp = a0, t0 = 0 passed to callee */
    func_80096CA8();
}

/**
 * func_800F7E70 - Clear two memory regions
 * Address: 0x800F7E70
 * Size: 64 bytes
 *
 * Clears D_80159428 (16 bytes) and D_8015256C (4 bytes) using bzero.
 */
extern u8 D_80159428[];  /* 16-byte buffer */
extern u8 D_8015256C[];  /* 4-byte buffer */

void func_800F7E70(void) {
    bzero(D_80159428, 16);
    bzero(D_8015256C, 4);
}

/**
 * func_800FBBFC - Conditional call based on game state
 * Address: 0x800FBBFC
 * Size: 52 bytes
 *
 * Calls func_8000BE50(0), if result == 7, calls func_800C9AE0.
 */
extern s32 func_8000BE50(s32);

void func_800FBBFC(void) {
    s32 state = func_8000BE50(0);
    if (state == 7) {
        func_800C9AE0();
    }
}

/**
 * func_800FEC60 - Select resource type 46 or 38
 * Address: 0x800FEC60
 * Size: 60 bytes
 *
 * If bit 0 of a0 is set, uses resource type 46.
 * Otherwise uses resource type 38.
 * Then calls func_80092360.
 *
 * @param a0 Mode flags
 */
void func_800FEC60(s32 a0) {
    s32 resource_type;

    if (a0 & 1) {
        resource_type = 46;
    } else {
        resource_type = 38;
    }

    func_80092360(resource_type, 0, 1, 0);
}

/**
 * func_800ED764 - Set mode byte from parameter or object
 * Address: 0x800ED764
 * Size: 80 bytes
 *
 * If a0 (as signed 16-bit) is negative:
 *   - Calls func_800B3D18 with t0=0
 *   - Sets D_80159B70 = D_801597F0[8]
 * Otherwise:
 *   - Sets D_80159B70 = a0
 *
 * @param a0 Mode value or -1 to use current object
 */
extern u8 D_80159B70;  /* Mode byte */

void func_800ED764(s16 a0) {
    if (a0 < 0) {
        func_800B3D18();
        D_80159B70 = *(D_801597F0 + 8);
    } else {
        D_80159B70 = (u8)a0;
    }
}

/**
 * func_800ED7B4 - Set mode byte from parameter or object (variant)
 * Address: 0x800ED7B4
 * Size: 80 bytes
 *
 * Same as func_800ED764 but reads offset 4 and writes to D_80159B60.
 *
 * @param a0 Mode value or -1 to use current object
 */
extern u8 D_80159B60;  /* Mode byte 2 */

void func_800ED7B4(s16 a0) {
    if (a0 < 0) {
        func_800B3D18();
        D_80159B60 = *(D_801597F0 + 4);
    } else {
        D_80159B60 = (u8)a0;
    }
}

/**
 * func_800EE7C4 - Conditional sync init with flag
 * Address: 0x800EE7C4
 * Size: 92 bytes
 *
 * If t6 (condition) is zero:
 *   - Sets D_801147C0 = 1
 *   - Calls func_80006A00(D_801461D0, D_801461FC, 1)
 *   - Releases sync on D_801461D0
 * Always sets D_80159DA0 = -1
 *
 * NOTE: Uses t6 register as condition (set by caller).
 */
extern u8 D_801147C0;   /* Init flag */
extern u8 D_801461FC[]; /* Data pointer */
extern s8 D_80159DA0;   /* State byte */
extern void func_80006A00(void*, void*, s32);

void func_800EE7C4(s32 condition) {
    if (condition == 0) {
        D_801147C0 = 1;
        func_80006A00(&D_801461D0, D_801461FC, 1);
        func_800075E0(&D_801461D0, NULL, 0);
    }
    D_80159DA0 = -1;
}

/**
 * func_800E2A64 - Call multiple update functions on object
 * Address: 0x800E2A64
 * Size: 96 bytes
 *
 * Calls 8 functions in sequence with the same object parameter:
 *   func_800CF06C, func_800E23A4, func_800E1C30, func_800E1AA0,
 *   func_800E15A0, func_800E1540, func_800E114C, func_800D0424
 *
 * This is likely a full object update/tick function.
 *
 * @param a0 Object pointer
 */
extern void func_800CF06C(void*);
extern void func_800E1AA0(void*);
extern void func_800E15A0(void*);
extern void func_800E1540(void*);
extern void func_800E114C(void*);
extern void func_800D0424(void*);

void func_800E2A64(void *obj) {
    func_800CF06C(obj);
    func_800E23A4((s32)obj);
    func_800E1C30((s32)obj);
    func_800E1AA0(obj);
    func_800E15A0(obj);
    func_800E1540(obj);
    func_800E114C(obj);
    func_800D0424(obj);
}

/**
/**
/**
 * func_800E7914 - Decrement object counter with sync
 * Address: 0x800E7914
 * Size: 108 bytes
 *
 * Acquires sync, looks up object by ID, decrements counter at offset 22
 * if it's positive, then releases sync.
 *
 * @param id Object ID to look up
 */
extern void *D_80152770;  /* Object sync structure */
extern void *func_80095F8C(s32);
extern void *func_80095EF4(void*, void*, s32);

void func_800E7914(s32 id) {
    void *obj;
    u8 counter;

    func_80007270(&D_80152770, NULL, 1);
    obj = func_80095F8C(id);
    obj = func_80095EF4(obj, NULL, 0);
    counter = *((u8*)obj + 22);
    if (counter > 0) {
        *((u8*)obj + 22) = counter - 1;
    }
    func_800075E0(&D_80152770, NULL, 0);
}

/**
 * func_800E7980 - Increment object counter with sync
 * Address: 0x800E7980
 * Size: 112 bytes
 *
 * Acquires sync, looks up object by ID, increments counter at offset 22
 * if it's less than 255, then releases sync.
 *
 * @param id Object ID to look up
 */
void func_800E7980(s32 id) {
    void *obj;
    u8 counter;

    func_80007270(&D_80152770, NULL, 1);
    obj = func_80095F8C(id);
    obj = func_80095EF4(obj, NULL, 0);
    counter = *((u8*)obj + 22);
    if (counter < 255) {
        *((u8*)obj + 22) = counter + 1;
    }
    func_800075E0(&D_80152770, NULL, 0);
}

/**
/**
 * func_8008AD48 - Offset pointer wrapper for 8008AD04
 * Address: 0x8008AD48
 * Size: 36 bytes
 *
 * Adds 4 to both pointers and calls func_8008AD04.
 * Used for skipping header bytes in data structures.
 *
 * @param a0 First pointer (offset by 4)
 * @param a1 Second pointer (offset by 4)
 */
extern void func_8008AD04(void*, void*);

void func_8008AD48(void *a0, void *a1) {
    func_8008AD04((u8*)a0 + 4, (u8*)a1 + 4);
}

/**
 * func_8008E398 - Sign-extend and call E26C
 * Address: 0x8008E398
 * Size: 40 bytes
 *
 * Sign-extends the third parameter from 16 bits and calls func_8008E26C.
 *
 * @param a0 First parameter (passed through)
 * @param a1 Second parameter (passed through)
 * @param a2 16-bit value to sign-extend
 */
extern void func_8008E26C(void*, void*, s16);

void func_8008E398(void *a0, void *a1, s16 a2) {
    func_8008E26C(a0, a1, a2);
}

/**
 * func_80090228 - Wrapper for func_80090088 with zeroed a2
 * Address: 0x80090228
 * Size: 44 bytes
 *
 * Sign-extends parameter and calls func_80090088 with a2=0.
 *
 * @param a0 16-bit value to sign-extend
 */
extern void func_80090088(s16, void*, s32);

void func_80090228(s16 a0) {
    func_80090088(a0, NULL, 0);
}

/**
 * func_80090254 - Wrapper for func_80090088 with zeroed a1 and a2
 * Address: 0x80090254
 * Size: 48 bytes
 *
 * Sign-extends parameter and calls func_80090088 with a1=NULL, a2=0.
 *
 * @param a0 16-bit value to sign-extend
 */
void func_80090254(s16 a0) {
    func_80090088(a0, NULL, 0);
}

/**
 * func_8009508C - Simple wrapper for func_8008AD04
 * Address: 0x8009508C
 * Size: 32 bytes
 *
 * Calls func_8008AD04 with unchanged parameters.
 *
 * @param a0 First pointer
 * @param a1 Second pointer
 */
void func_8009508C(void *a0, void *a1) {
    func_8008AD04(a0, a1);
}

/**
/**
/**
/**
/**
/**
 * func_800A2CE4 - Call func_800A2990 with derived parameter
 * Address: 0x800A2CE4
 * Size: 40 bytes
 *
 * Loads pointer from a0->0, then gets value at offset 64 of that,
 * and calls func_800A2990 with (a0, 0, value).
 *
 * @param a0 Pointer to pointer
 */
extern void func_800A2990(void*, s32, s32);

void func_800A2CE4(void **a0) {
    void *ptr = *a0;
    s32 val = *(s32*)((u8*)ptr + 64);
    func_800A2990(a0, 0, val);
}

/**
/**
 * func_800986B0 - Offset pointer call to func_80098620
 * Address: 0x800986B0
 * Size: 36 bytes
 *
 * Calls func_80098620 with (a0+8, original_a1).
 *
 * @param a0 Base pointer
 * @param a1 Parameter moved to a2
 */
extern void func_80098620(void*, void*, void*);

void func_800986B0(void *a0, void *a1) {
    func_80098620((u8*)a0 + 8, a1, NULL);
}

/**
/**
 * func_800985F4 - Parameter reshuffling wrapper
 * Address: 0x800985F4
 * Size: 44 bytes
 *
 * Reorganizes parameters and calls func_80098574.
 *
 * @param a0 Base object
 * @param a1 Moved to a3
 */
extern void func_80098574(void*, void*, void*, void*);

void func_800985F4(void *a0, void *a1) {
    s32 temp = 0;
    void *val = *(void**)((u8*)a0 + 8);
    func_80098574(a0, val, &temp, a1);
}

/**
/**
/**
 * func_80100D30 - Initialize with callback pointer
 * Address: 0x80100D30
 * Size: 44 bytes
 *
 * Stores D_801146FC at a0+4, calls func_80094EC8, returns 1.
 *
 * @param a0 Object to initialize
 * @return Always returns 1
 */
extern void *D_801146FC;  /* Callback pointer */
extern void func_80094EC8(void*);

s32 func_80100D30(void *a0) {
    *(void**)((u8*)a0 + 4) = &D_801146FC;
    func_80094EC8(a0);
    return 1;
}

/**
 * func_8010FBB4 - Acquire sync on D_80152750
 * Address: 0x8010FBB4
 * Size: 44 bytes
 *
 * Acquires sync on D_80152750 with flags (0, 1).
 */
extern void *D_80152750;  /* Sync object */

void func_8010FBB4(void) {
    func_80007270(&D_80152750, NULL, 1);
}

/**
 * func_800B0550 - Initialize structure with parameters
 * Address: 0x800B0550
 * Size: 48 bytes
 *
 * Stores parameters into structure fields and calls func_800B04D0.
 * Structure layout: byte at 0, s32 at 4, s32 at 8, s32 at 12.
 *
 * @param a0 Structure pointer
 * @param a1 Stored at offset 12
 * @param a2 Stored at offset 8
 * @param a3 Stored at offset 4
 * @param stack Byte from stack stored at offset 0
 */
void func_800B0550(void *a0, s32 a1, s32 a2, s32 a3, u8 stack) {
    *(s32*)((u8*)a0 + 12) = a1;
    *(s32*)((u8*)a0 + 4) = a3;
    *(s32*)((u8*)a0 + 8) = a2;
    *(u8*)a0 = stack;
    func_800B04D0(a0);
}

/**
/**
/**
/**
 * func_80095120 - Validate and call func_800950AC
 * Address: 0x80095120
 * Size: 52 bytes
 *
 * If either a0 or a1 is NULL, returns -1.
 * Otherwise calls func_800950AC with a2=15.
 *
 * @param a0 First pointer (must be non-NULL)
 * @param a1 Second pointer (must be non-NULL)
 * @return -1 if invalid, otherwise result of func_800950AC
 */
extern s32 func_800950AC(void*, void*, s32);

s32 func_80095120(void *a0, void *a1) {
    if (a0 == NULL || a1 == NULL) {
        return -1;
    }
    return func_800950AC(a0, a1, 15);
}

/**
 * func_800B71A0 - Calculate difference with halved result
 * Address: 0x800B71A0
 * Size: 52 bytes
 *
 * Similar to func_800B7170 but divides result by 2 before subtracting.
 *
 * @param a0 First parameter
 * @param a1 Value whose high halfword is used
 * @return (s16)(a1 >> 16) - (result / 2)
 */
s16 func_800B71A0(void *a0, s32 a1) {
    s16 original = (s16)(a1 >> 16);
    s16 result = func_800B3FA4(a0, -1);
    return original - (result >> 1);
}

/**
/**
/**
 * func_800AED2C - Initialize object with cleared byte
 * Address: 0x800AED2C
 * Size: 56 bytes
 *
 * If a1 is NULL returns 0. Otherwise calls func_8009211C,
 * clears byte at a1+8, returns a1.
 *
 * @param a0 First parameter
 * @param a1 Object pointer
 * @return 0 if a1 is NULL, otherwise a1
 */
void *func_800AED2C(void *a0, void *a1) {
    if (a1 == NULL) {
        return NULL;
    }
    func_8009211C(a0, a1);
    *((u8*)a1 + 8) = 0;
    return a1;
}

/**
/**
 * func_8008B660 - Store 3D vector and transform
 * Address: 0x8008B660
 * Size: 60 bytes
 *
 * Stores f12, f14, f16 at a1+36/40/44, calls func_8008B4C4,
 * then stores f20, f22, f24 at same offsets.
 * Uses floating-point register parameters for 3D vector input/output.
 *
 * @param a0 Unused in this wrapper
 * @param a1 Object to store vector in
 * @param f12 X component
 * @param f14 Y component
 * @param f16 Z component
 */
extern void func_8008B4C4(void*);

void func_8008B660(void *a0, void *a1, f32 x, f32 y, f32 z) {
    f32 *vec = (f32*)((u8*)a1 + 36);
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
    func_8008B4C4(vec);
    /* Output stored in f20, f22, f24 by callee - stored back by asm */
}

/**
 * func_80096298 - Get slot value from array
 * Address: 0x80096298
 * Size: 60 bytes
 *
 * Calls func_80096288(a0, 0, 0), then returns D_80156D44[a0 * 5].
 * Slot array entries are 20 bytes each.
 *
 * @param a0 Slot index
 * @return Value from slot array at computed offset
 */
extern void func_80096288(s32, s32, s32);
extern void *D_80156D44[];  /* Slot array, 20-byte entries */

void *func_80096298(s32 a0) {
    func_80096288(a0, 0, 0);
    return D_80156D44[a0 * 5];
}

/**
 * func_80096240 - Conditional reset with callbacks
 * Address: 0x80096240
 * Size: 72 bytes
 *
 * If a1 == -1, returns immediately.
 * Otherwise calls func_80018E2C, func_800154A4, func_80096130,
 * and sets D_8011EAA0 = -1.
 *
 * @param a0 Unused
 * @param a1 If -1, skip processing
 */
extern void func_80018E2C(s32);
extern void func_800154A4(void);
extern void func_80096130(void*);
extern void *D_80151AD4;  /* Global pointer */
extern s32 D_8011EAA0;    /* State variable */

void func_80096240(s32 a0, s32 a1) {
    if (a1 == -1) {
        return;
    }
    func_80018E2C(a1);
    func_800154A4();
    func_80096130(D_80151AD4);
    D_8011EAA0 = -1;
}

/**
/**
/**
/**
/**
 * func_800B5F4C - Conditional resource request (bit 0x400)
 * Address: 0x800B5F4C
 * Size: 60 bytes
 *
 * If bit 0x400 is set in a0, requests resource 40.
 * Otherwise requests resource 39.
 * Calls func_80092360(type, 0, 1, 0).
 *
 * @param a0 Flags to check
 */
extern void func_80092360(s32, s32, s32, s32);

void func_800B5F4C(s32 a0) {
    s32 type = (a0 & 0x400) ? 40 : 39;
    func_80092360(type, 0, 1, 0);
}

/**
 * func_800B5F88 - Conditional resource request (bit 0x1000)
 * Address: 0x800B5F88
 * Size: 60 bytes
 *
 * If bit 0x1000 is set in a0, requests resource 41.
 * Otherwise requests resource 44.
 * Calls func_80092360(type, 0, 1, 0).
 *
 * @param a0 Flags to check
 */
void func_800B5F88(s32 a0) {
    s32 type = (a0 & 0x1000) ? 41 : 44;
    func_80092360(type, 0, 1, 0);
}

/**
/**
 * func_800BE4B4 - Call B74A0 then B71D4 with reshuffled params
 * Address: 0x800BE4B4
 * Size: 60 bytes
 *
 * Stores parameters, calls func_800B74A0(a2),
 * then calls func_800B71D4 with halfwords from a0, a1 and original a3.
 *
 * @param a0 First value (low halfword used)
 * @param a1 Second value (low halfword used)
 * @param a2 Parameter for first call
 * @param a3 Third parameter passed through
 */
extern void func_800B74A0(s32);
extern void func_800B71D4(s16, s16, void*);

void func_800BE4B4(s32 a0, s32 a1, s32 a2, void *a3) {
    func_800B74A0(a2);
    func_800B71D4((s16)a0, (s16)a1, a3);
}

/**
 * func_800B41C0 - Set byte at offset 9 of current object
 * Address: 0x800B41C0
 * Size: 64 bytes
 *
 * Sign-extends a0 to byte, calls func_800B3D18,
 * saves old byte at D_801597F0+9, stores new value, returns old.
 *
 * @param a0 New value (sign-extended to byte)
 * @return Previous value at offset 9
 */
s8 func_800B41C0(s8 a0) {
    s8 old;
    func_800B3D18();
    old = *((s8*)D_801597F0 + 9);
    *((s8*)D_801597F0 + 9) = a0;
    return old;
}

/**
 * func_80097470 - Synchronized resource lookup
 * Address: 0x80097470
 * Size: 124 bytes
 *
 * Acquires sync on D_80152770, looks up resource via func_80097384.
 * If a0 is 0, uses D_801527C8 as default lookup key.
 * Releases sync and returns result.
 *
 * @param a0 Lookup mode (0 = use default)
 * @param a1 Resource parameter
 * @return Lookup result
 */
extern u8 D_80152770[];
extern void *D_801527C8;
extern void *func_80097384(void*, void*);

void *func_80097470(s32 a0, void *a1) {
    void *result;
    void *lookup;

    func_80007270(&D_80152770[0], NULL, 1);

    if (a0 != 0) {
        lookup = (void*)a0;
    } else {
        lookup = D_801527C8;
    }

    result = func_80097384(a1, lookup);

    func_800075E0(&D_80152770[0], NULL, 0);

    return result;
}

/**
 * func_8010FC80 - Allocate and initialize resource
 * Address: 0x8010FC80
 * Size: 64 bytes
 *
 * Calls func_80097470(0, original_a0), then func_800962D4(result, 0).
 * Returns result from first call.
 *
 * @param a0 First parameter
 * @param a1 Second parameter (passed to func_80097470)
 * @return Result from func_80097470
 */

void *func_8010FC80(void *a0, void *a1) {
    void *result = func_80097470(0, a0);
    func_800962D4(result, 0);
    return result;
}

/**
 * func_800D54E0 - Conditional function call based on a1
 * Address: 0x800D54E0
 * Size: 68 bytes
 *
 * If a1 is zero, calls func_80091C04(*a0).
 * Otherwise calls func_800BF024(*a0).
 * Then calls func_800D54BC(a0).
 *
 * @param a0 Pointer to pointer
 * @param a1 Mode selector
 */
/**
 * func_800D54BC - Initialize structure with default values
 * Address: 0x800D54BC
 * Size: 36 bytes (leaf function, no prologue)
 *
 * Sets field0 to -1 and fields 4,8,12,16 to -2.0f.
 * Used to reset physics/collision state structures.
 *
 * @param a0 Structure pointer to initialize
 */
void func_800D54BC(void *a0) {
    *(s32*)a0 = -1;
    *(f32*)((u8*)a0 + 4) = -2.0f;
    *(f32*)((u8*)a0 + 8) = -2.0f;
    *(f32*)((u8*)a0 + 12) = -2.0f;
    *(f32*)((u8*)a0 + 16) = -2.0f;
}

/**
 * func_800BF024 - Synchronized lookup and process
 * Address: 0x800BF024
 * Size: 128 bytes
 *
 * Acquires sync on D_80142728, looks up via func_80091BA8.
 * If found, calls func_800BF01C with field64, releases sync,
 * then calls func_80091C04. If not found, just releases sync.
 *
 * @param a0 Key to look up and process
 */
extern void func_800BF01C(void*);

void func_800BF024(void *a0) {
    void *result;

    func_80007270(&D_80142728[0], NULL, 1);

    result = func_80091BA8(a0, &D_80142728[0]);

    if (result == NULL) {
        func_800075E0(&D_80142728[0], NULL, 0);
        return;
    }

    func_800BF01C(*(void**)((u8*)result + 0x40));
    func_800075E0(&D_80142728[0], NULL, 0);
    func_80091C04(a0);
}

/**
 * func_80091C04 - Synchronized resource registration
 * Address: 0x80091C04
 * Size: 160 bytes
 *
 * Acquires sync on D_80142728, looks up key via func_80091BA8.
 * If found, calls func_80091B00, sets result->byte2 = 6,
 * stores original key at offset 4, increments counter at offset 26.
 * Then releases sync and optionally signals D_801427A8.
 *
 * @param a0 Key to register
 */
extern void *func_80091BA8(void*, void*);
extern void *func_80091B00(void*);

void func_80091C04(void *a0) {
    void *result;
    void *savedResult;

    func_80007270(&D_80142728[0], NULL, 1);

    result = func_80091BA8(a0, NULL);
    savedResult = NULL;

    if (result != NULL) {
        func_80091B00(result);
        *(u8*)((u8*)result + 2) = 6;
        *(void**)((u8*)result + 4) = a0;
        (*(u8*)((u8*)a0 + 26))++;
        savedResult = result;
    }

    func_800075E0(&D_80142728[0], NULL, 0);

    if (savedResult != NULL) {
        func_800075E0(&D_80142728[0x80], savedResult, 0);
    }
}

void func_800D54E0(void **a0, s32 a1) {
    if (a1 != 0) {
        func_800BF024(*a0);
    } else {
        func_80091C04(*a0);
    }
    func_800D54BC(a0);
}

/**
 * func_8010FD1C - Initialize structure and call 8ABE4
 * Address: 0x8010FD1C
 * Size: 68 bytes
 *
 * If a1 is zero, returns 1 without doing anything.
 * Otherwise initializes D_80153F10 structure and calls func_8008ABE4.
 *
 * @param a0 Stored at offset 8
 * @param a1 Stored as halfword at offset 2 (if non-zero)
 * @param a2 Stored at offset 12
 * @return Always 1
 */
extern void *D_80153F10;  /* Structure base */
extern void func_8008ABE4(void);

s32 func_8010FD1C(void *a0, s16 a1, void *a2) {
    u8 *base;
    if (a1 == 0) {
        return 1;
    }
    base = (u8*)&D_80153F10;
    *(void**)(base + 8) = a0;
    *(s16*)(base + 2) = a1;
    *(void**)(base + 12) = a2;
    *(s16*)(base + 4) = 0;
    *base = 1;
    func_8008ABE4();
    return 1;
}

/**
 * func_800B7128 - Sum bytes at offset 2 and 3 of current object
 * Address: 0x800B7128
 * Size: 72 bytes
 *
 * Calls func_800B3D18 twice, gets bytes at offset 2 and 3
 * of D_801597F0, returns their sum as s16.
 *
 * @return (s16)(D_801597F0[2] + D_801597F0[3])
 */
s16 func_800B7128(void) {
    u8 byte2, byte3;
    func_800B3D18();
    byte2 = *((u8*)D_801597F0 + 2);
    func_800B3D18();
    byte3 = *((u8*)D_801597F0 + 3);
    return (s16)(byte2 + byte3);
}

/**
 * func_800CDA90 - Set byte at offset 71 and sync
 * Address: 0x800CDA90
 * Size: 80 bytes
 *
 * Gets object chain a0->0->44->0, if byte at +71 differs from a1,
 * sets it to a1 and calls func_800A2504 to sync.
 *
 * @param a0 Object pointer chain
 * @param a1 New byte value
 */
extern void func_800A2504(void*, void*, s32);

void func_800CDA90(void **a0, u8 a1) {
    void *obj = *a0;
    void *inner = *(void**)((u8*)obj + 44);
    void *data = *(void**)inner;
    u8 *target = (u8*)data + 71;

    if (*target != a1) {
        *target = a1;
        obj = *a0;
        inner = *(void**)((u8*)obj + 44);
        func_800A2504(*(void**)((u8*)obj + 8), (u8*)*(void**)inner + 71, 1);
    }
}

/**
/**
/**
 * func_800BE9A0 - Build buffer and call B71D4
 * Address: 0x800BE9A0
 * Size: 72 bytes
 *
 * Builds a local buffer via func_800BE7BC, then calls func_800B71D4
 * with halfwords from stored parameters.
 *
 * @param a0 First value (high halfword used)
 * @param a1 Second value (high halfword used)
 * @param a2 Parameter for first call (sign-extended)
 * @param a3 Second parameter for first call
 */
extern void func_800BE7BC(void*, void*, s16, void*);

void func_800BE9A0(s32 a0, s32 a1, s16 a2, void *a3) {
    u8 buffer[128];
    func_800BE7BC(buffer, a3, a2, NULL);
    func_800B71D4((s16)(a0 >> 16), (s16)(a1 >> 16), buffer);
}

/**
 * func_800BE9E8 - Build buffer via 2CD0 and call B71D4
 * Address: 0x800BE9E8
 * Size: 72 bytes
 *
 * Builds a local buffer via func_80002CD0, then calls func_800B71D4
 * with halfwords from stored parameters.
 *
 * @param a0 First value (high halfword used)
 * @param a1 Second value (high halfword used)
 * @param a2 Buffer parameter
 * @param a3 Third parameter stored at offset
 */
void func_800BE9E8(s32 a0, s32 a1, void *a2, s32 a3) {
    u8 buffer[256];
    s32 args[1];
    args[0] = a3;
    func_80002CD0(buffer, a2, args);
    func_800B71D4((s16)(a0 >> 16), (s16)(a1 >> 16), buffer);
}

/**
 * func_800A4C54 - Initialize and wait for completion
 * Address: 0x800A4C54
 * Size: 84 bytes
 *
 * Calls initialization chain then loops until func_800202C4 returns non-zero.
 */
extern void func_800A4B6C(void);
extern void func_80020274(void);
extern s32 func_800202C4(void);

void func_800A4C54(void) {
    func_800A4B6C();
    func_80096238(D_80151A6C);
    func_80020274();
    while (func_800202C4() == 0) {
        /* Wait for completion */
    }
}

/**
 * func_800B0618 - Process list and call B0580
 * Address: 0x800B0618
 * Size: 84 bytes
 *
 * Loops through D_801491F0 linked list calling func_8009079C(item, 1),
 * then calls func_800B0580.
 */
extern void *D_801491F0;  /* Linked list head */
extern void func_8009079C(void*, s32);

void func_800B0618(void) {
    void *item = D_801491F0;
    while (item != NULL) {
        func_8009079C(item, 1);
        item = D_801491F0;
    }
    func_800B0580();
}

/**
 * func_800B3F50 - Sum three values from current object and global
 * Address: 0x800B3F50
 * Size: 84 bytes
 *
 * Returns (s16)(D_80159B60 + D_801597F0[2] + D_801597F0[3]).
 *
 * @return Sum as signed 16-bit
 */
extern s8 D_80159B60;  /* Global offset value */

s16 func_800B3F50(void) {
    u8 byte2, byte3;
    s8 offset;
    func_800B3D18();
    byte3 = *((u8*)D_801597F0 + 3);
    func_800B3D18();
    byte2 = *((u8*)D_801597F0 + 2);
    offset = D_80159B60;
    return (s16)(offset + byte2 + byte3);
}

/**
/**
/**
 * func_8010FCC0 - Call func_80095FD8 with sync protection
 * Address: 0x8010FCC0
 * Size: 92 bytes
 *
 * Acquires sync on D_80152770, calls func_80095FD8(a0, 0), releases sync.
 *
 * @param a0 Parameter for func_80095FD8
 */
void func_8010FCC0(void *a0) {
    func_80007270(&D_80152770, NULL, 1);
    func_80095FD8(a0, 0);
    func_800075E0(&D_80152770, NULL, 0);
}

/**
/**
/**
 * func_800B04D0 - Initialize pool linked list
 * Address: 0x800B04D0
 * Size: 128 bytes
 *
 * Sets up a linked list within a pre-allocated pool. Each element's first
 * word points to the next element. The last element points to NULL.
 *
 * Pool structure:
 *   offset 0x04: count (number of elements)
 *   offset 0x08: stride (size of each element in bytes)
 *   offset 0x0C: base (pointer to pool array)
 *   offset 0x10: head (active list head, cleared to NULL)
 *   offset 0x14: free (free list head, set to base)
 *
 * @param a0 Pointer to pool control structure
 */
void func_800B04D0(void *a0) {
    s32 *base;
    s32 count;
    s32 stride;
    s32 i;

    base = *(s32**)((u8*)a0 + 0x0C);
    *(s32*)((u8*)a0 + 0x10) = 0;         /* head = NULL */

    if (base == NULL) {
        *(s32**)((u8*)a0 + 0x14) = base; /* free = base (NULL) */
        return;
    }

    *(s32**)((u8*)a0 + 0x14) = base;     /* free = base */

    count = *(s32*)((u8*)a0 + 0x04);
    stride = *(s32*)((u8*)a0 + 0x08);

    /* Link each element to the next */
    for (i = 0; i < count - 1; i++) {
        *(s32*)((u8*)base + i * stride) = (s32)((u8*)base + (i + 1) * stride);
    }

    /* Last element points to NULL */
    *(s32*)((u8*)base + (count - 1) * stride) = 0;
}

/**
 * func_800B0580 - Reset and initialize pool system
 * Address: 0x800B0580
 * Size: 152 bytes
 *
 * Iterates through active list at D_80155220, calling cleanup functions.
 * Then reinitializes the pool with default values.
 * Finally clears the secondary pool area at D_80155290.
 */
extern void func_8008D0C0(s32);
extern void func_800AFA84(void*, void*);
extern u8 D_80155220[];
extern u8 D_80155290[];
extern u8 D_80155B30[];

void func_800B0580(void) {
    void *node;

    /* Process and remove all nodes from active list */
    node = *(void**)(&D_80155220[0x10]);
    while (node != NULL) {
        func_8008D0C0(*(s32*)((u8*)node + 0x0C));
        func_800AFA84(&D_80155220[0], node);
        node = *(void**)(&D_80155220[0x10]);
    }

    /* Reinitialize pool control structure */
    D_80155220[0] = 1;                                      /* type/flag */
    *(s32*)(&D_80155220[0x08]) = 36;                       /* stride = 36 bytes */
    *(s32*)(&D_80155220[0x04]) = 100;                      /* count = 100 entries */
    *(void**)(&D_80155220[0x0C]) = (void*)&D_80155B30[0];  /* base pointer */

    func_800B04D0(&D_80155220[0]);

    /* Clear secondary pool area (2208 bytes) */
    func_80002790(&D_80155290[0], 0, 2208);
}

/**
/**
 * func_8009C5BC - Call func_8009C3F8 with mode 0
 * Address: 0x8009C5BC
 * Size: 36 bytes
 *
 * Simple wrapper that calls func_8009C3F8(0).
 * The FP register f16 receives abs(f12) but may be unused.
 */
extern void func_8009C3F8(s32);

void func_8009C5BC(void) {
    func_8009C3F8(0);
}

/**
 * func_800BFD68 - Call func_8009C3F8 with mode 1
 * Address: 0x800BFD68
 * Size: 36 bytes
 *
 * Simple wrapper that calls func_8009C3F8(1).
 * The FP register f16 receives abs(f12) but may be unused.
 */
void func_800BFD68(void) {
    func_8009C3F8(1);
}

/**
 * func_800A511C - Conditional state update
 * Address: 0x800A511C
 * Size: 60 bytes
 *
 * If a0 != a1, calls func_800A501C, copies byte from D_8013FEC8 to D_80140A10,
 * then calls func_800A4E58.
 *
 * @param a0 First comparison value
 * @param a1 Second comparison value
 */
extern void func_800A501C(void);
extern void func_800A4E58(void);
extern s16 D_8013FEC8;
extern u8 D_80140A10;

void func_800A511C(s32 a0, s32 a1) {
    if (a0 != a1) {
        func_800A501C();
        D_80140A10 = (u8)D_8013FEC8;
        func_800A4E58();
    }
}

/**
 * func_800AB70C - Call func_8008E26C with transformed parameters
 * Address: 0x800AB70C
 * Size: 68 bytes
 *
 * Passes a0, a1 through; transforms a2, a3 into new values.
 * New a2 = (s16)old_a3
 * New a3 = (((s16)old_a2 << 8) ^ 0x0F00) | stack_arg
 *
 * @param a0 First parameter (passed through)
 * @param a1 Second parameter (passed through)
 * @param a2 Third parameter (used in transform)
 * @param a3 Fourth parameter (becomes new a2)
 * @param stack Fifth parameter from stack (ORed into new a3)
 */
extern void func_8008E26C(s32, s32, s32, s32);

void func_800AB70C(s32 a0, s32 a1, s16 a2, s16 a3, s32 stack) {
    s32 new_a2 = a3;
    s32 new_a3 = ((a2 << 8) ^ 0x0F00) | stack;
    func_8008E26C(a0, a1, new_a2, new_a3);
}

/**
 * func_800C92DC - Call func_800C9210 with mode (1, 0)
 * Address: 0x800C92DC
 * Size: 88 bytes
 *
 * Wrapper that sets s1=1, s2=0 and calls func_800C9210.
 * The FP operations copy f12 to f20 and f14 to f22.
 */
extern void func_800C9210(void);

void func_800C92DC(void) {
    func_800C9210();
}

/**
 * func_800C93AC - Call func_800C9210 with mode (0, 1)
 * Address: 0x800C93AC
 * Size: 88 bytes
 *
 * Wrapper that sets s1=0, s2=1 and calls func_800C9210.
 * The FP operations copy f12 to f20 and f14 to f22.
 */
void func_800C93AC(void) {
    func_800C9210();
}

/**
 * func_8010B528 - Initialize structure with callback pointer
 * Address: 0x8010B528
 * Size: 56 bytes
 *
 * Sets field_4 to D_80116DE0, calls func_80094EC8(a0),
 * clears field_28, and returns 1.
 *
 * @param a0 Structure pointer
 * @return 1
 */
extern u8 D_80116DE0[];

s32 func_8010B528(void *a0) {
    *(void**)((u8*)a0 + 4) = &D_80116DE0[0];
    func_80094EC8(a0);
    *(s32*)((u8*)a0 + 0x28) = 0;
    return 1;
}

/**
 * func_8010B560 - Update state based on global flag
 * Address: 0x8010B560
 * Size: 112 bytes
 *
 * Checks D_80159D98 global and updates a0->byte26 accordingly.
 * If state changed, calls func_80094EC8.
 * If byte26 becomes 0, sets field4 to D_80117358 and clears field40.
 *
 * @param a0 Structure pointer
 * @return Always 1
 */
extern u32 D_80159D98;
extern u8 D_80117358[];

s32 func_8010B560(void *a0) {
    s32 globalState;
    s8 currentState;

    globalState = (D_80159D98 != 0) ? 1 : 0;
    currentState = *(s8*)((u8*)a0 + 26);

    if (globalState != currentState) {
        *(s8*)((u8*)a0 + 26) = (s8)globalState;
        func_80094EC8(a0);
        currentState = *(s8*)((u8*)a0 + 26);
    }

    if (currentState != 0) {
        return 1;
    }

    *(void**)((u8*)a0 + 4) = &D_80117358[0];
    func_80094EC8(a0);
    *(s32*)((u8*)a0 + 40) = 0;
    return 1;
}

/**
 * func_800F8754 - Cleanup and reset state
 * Address: 0x800F8754
 * Size: 76 bytes
 *
 * Calls func_800F857C, then func_800B45BC(1).
 * If D_8011472C is non-NULL, calls func_800B358C and clears it.
 * Finally clears D_80114728 byte.
 */
extern void func_800F857C(void);
extern void func_800B45BC(s32);
extern void func_800B358C(void*);
extern void *D_8011472C;
extern u8 D_80114728;

void func_800F8754(void) {
    func_800F857C();
    func_800B45BC(1);

    if (D_8011472C != NULL) {
        func_800B358C(D_8011472C);
        D_8011472C = NULL;
    }
    D_80114728 = 0;
}

/**
 * func_80091F34 - Synchronized call to func_80091CA4
 * Address: 0x80091F34
 * Size: 136 bytes
 *
 * Acquires sync on D_80142728, calls func_80091CA4 with parameters,
 * then releases sync.
 *
 * @param a0 First parameter (passed to func_80091CA4)
 * @param a1 Second parameter (loaded as float)
 */
extern u8 D_80142728[];
extern void func_80091CA4(void*);

void func_80091F34(void *a0, s32 a1) {
    func_80007270(&D_80142728[0], NULL, 1);
    func_80091CA4(a0);
    func_800075E0(&D_80142728[0], NULL, 0);
}

/**
 * func_800AC668 - Complex initialization with callback setup
 * Address: 0x800AC668
 * Size: 140 bytes
 *
 * Sets up D_80159D90 to -1, calls several setup functions,
 * stores result to D_80159B80, then calls additional init functions.
 *
 * @param a0 First parameter
 * @param a1 Second parameter
 * @return Value of D_80159D90 (s16)
 */
extern void func_800A473C(void*, void*);
extern void *func_80092C58(void*, void*, void*, s32, void*);
extern void func_800AC3D8(void*, s16);
extern void func_800AB638(void);
extern s16 D_80159D90;
extern void *D_801597FC;
extern void *D_80159818;
extern u8 D_8008AD48[];
extern void *D_80159B80;

s16 func_800AC668(void *a0, s16 a1) {
    u8 buf1[4];
    u8 buf2[4];
    void *result;

    D_80159D90 = -1;
    func_800A473C(buf1, a0);

    result = func_80092C58(buf2, D_80159818, *(void**)D_801597FC, 20, &D_8008AD48[0]);
    D_80159B80 = *(void**)result;

    func_800AC3D8(D_80159B80, a1);
    func_800AB638();

    return D_80159D90;
}

/**
 * func_800A4AC4 - Iterate through slot array and call func_80096130
 * Address: 0x800A4AC4
 * Size: 124 bytes
 *
 * Iterates through 64 slots in D_80156D38 array, calling func_80096130
 * for each slot that has field_0C set and field_06 != a0.
 *
 * @param a0 Filter value for field_06 comparison
 */
extern u8 D_80156D38[];
extern void func_80096130(s32);

void func_800A4AC4(s32 a0) {
    u8 *slot = &D_80156D38[0];
    s32 i;

    for (i = 0; i < 64; i++) {
        if (*(s32*)(slot + 0x0C) != 0) {
            if (*(slot + 6) != a0) {
                func_80096130(i);
            }
        }
        slot += 20;  /* 0x14 bytes per entry */
    }
}

/**
 * func_8009DC50 - Recursive tree/list search
 * Address: 0x8009DC50
 * Size: 200 bytes
 *
 * Recursively searches through a tree structure in D_8013E700.
 * Uses bit mask (256 << a1) to check node flags.
 *
 * @param a0 Pointer to node structure
 * @param a1 Bit position for mask creation
 * @return 1 if found, 0 otherwise
 */
extern u8 D_8013E700[];

s32 func_8009DC50(void *a0, s32 a1) {
    s32 flags;
    s32 mask;
    s16 leftIdx, rightIdx;
    void *node;

    mask = 256 << a1;
    node = a0;

    while (1) {
        flags = *(s32*)node;

        if ((flags & mask) != 0) {
            return 1;
        }

        leftIdx = *(s16*)((u8*)node + 0x16);
        if (leftIdx >= 0) {
            /* Recurse on left child */
            if (func_8009DC50(&D_8013E700[leftIdx * 68], a1) != 0) {
                return 1;
            }
        }

        rightIdx = *(s16*)((u8*)node + 0x18);
        if (rightIdx < 0) {
            return 0;
        }

        /* Continue with right child (tail recursion optimization) */
        node = &D_8013E700[rightIdx * 68];
    }
}

/**
 * func_8009DD18 - Check node flag and optionally search tree
 * Address: 0x8009DD18
 * Size: 112 bytes
 *
 * Checks if bit (256 << a1) is set in node flags.
 * If not, searches subtree using func_8009DC50.
 *
 * @param a0 Pointer to node structure
 * @param a1 Bit position for mask
 * @return 1 if flag set or found in tree, 0 otherwise
 */
s32 func_8009DD18(void *a0, s32 a1) {
    s32 flags;
    s32 mask;
    s16 idx;

    flags = *(s32*)a0;
    mask = 256 << a1;

    if ((flags & mask) != 0) {
        return 1;
    }

    idx = *(s16*)((u8*)a0 + 0x16);
    if (idx < 0) {
        return 0;
    }

    if (func_8009DC50(&D_8013E700[idx * 68], a1) != 0) {
        return 1;
    }

    return 0;
}

/**
 * func_800BF148 - Synchronized lookup and process
 * Address: 0x800BF148
 * Size: 128 bytes
 *
 * Acquires sync on D_80142728, calls func_80091BA8 to lookup.
 * If found, calls func_800BF01C and func_800BF0A4.
 * Returns lookup result.
 *
 * @param a0 Key to look up
 * @return Lookup result or 0 if not found
 */
extern void *func_80091BA8(void*, void*);
extern void func_800BF01C(void*);
extern void *func_800BF0A4(void*);

/* Object initialization functions */
extern void func_800D1004(void);  /* Setup call before init */
extern void func_800CF06C(void*); /* Object processing */
extern void func_800D0424(void*); /* Object configuration */

/* Global scaling factors */
extern f32 D_8011416C;
extern f32 D_80114170;

/* Float arrays for camera/visual data */
extern f32 D_80153F28[6];
extern f32 D_80153F48[6];
extern f32 D_80153F68[6];
extern f32 D_801543CC;
extern f32 D_80154190;
extern s16 D_80154182;

extern void func_800BAAA0(void);  /* Setup call */

void *func_800BF148(void *a0) {
    void *result;

    func_80007270(&D_80142728[0], NULL, 1);

    result = func_80091BA8(a0, &D_80142728[0]);

    if (result == NULL) {
        func_800075E0(&D_80142728[0], NULL, 0);
        return NULL;
    }

    func_800BF01C(*(void**)((u8*)result + 0x40));
    func_800075E0(&D_80142728[0], NULL, 0);
    func_800BF0A4(a0);

    return result;
}

/**
 * func_800D11BC - Initialize object with default scale values
 * (140 bytes)
 *
 * Initializes an object's scale factors to default values (0, 0, 0, 1)
 * at offset 0x720, sets flag at 0x730, and calculates a timing value
 * at offset 0x7D0.
 *
 * @param a0 Pointer to object structure
 */
void func_800D11BC(void *a0) {
    f32 zero;
    f32 one;
    f32 val;
    s16 result;

    func_800D1004();

    zero = 0.0f;
    one = 1.0f;

    /* Set scale to (0, 0, 0, 1) at offset 0x720 */
    *(f32*)((u8*)a0 + 0x720) = zero;
    *(f32*)((u8*)a0 + 0x724) = zero;
    *(f32*)((u8*)a0 + 0x728) = zero;
    *(f32*)((u8*)a0 + 0x72C) = one;

    /* Set initialization flag */
    *(u8*)((u8*)a0 + 0x730) = 1;

    /* Call initialization functions */
    func_800CF06C(a0);
    func_800D0424(a0);

    /* Calculate timing value from object field and global factors */
    val = *(f32*)((u8*)a0 + 0x408);
    result = (s16)(val * D_8011416C * D_80114170);
    *(s16*)((u8*)a0 + 0x7D0) = result;
}

/**
 * func_800BAD58 - Reset camera/visual float arrays
 * (136 bytes)
 *
 * Initializes three float arrays to zero and sets up initial state values.
 */
void func_800BAD58(void) {
    f32 zero;
    s32 i;

    func_800BAAA0();

    zero = 0.0f;

    /* Clear all three 6-element float arrays */
    for (i = 0; i < 6; i++) {
        D_80153F28[i] = zero;
        D_80153F48[i] = zero;
        D_80153F68[i] = zero;
    }

    /* Copy initial value and set state to -1 */
    D_80154190 = D_801543CC;
    D_80154182 = -1;
}

/* Player/car data array indexed by ID */
extern s8 D_80144031[];  /* Array of status bytes (stride 772) */
extern s32 func_800A1A60(void*);  /* Object processing function */

/**
 * func_800CC848 - Check object status and optionally process
 * (128 bytes)
 *
 * Checks if an object's child reference is valid and if the indexed
 * status byte allows processing. If a1 is non-zero, calls processing.
 *
 * @param a0 Pointer to parent object
 * @param a1 If non-zero, call processing function
 * @return 0 if invalid/disabled, 1 if valid, or processing result
 */
s32 func_800CC848(void **a0, s32 a1) {
    void *parent;
    void *child;
    void *childData;
    u8 index;
    s8 status;

    parent = *a0;
    child = *(void**)((u8*)parent + 8);

    if (child == NULL) {
        return 1;
    }

    childData = *(void**)child;
    index = *(u8*)((u8*)childData + 16);

    /* Calculate array offset: index * 772 + 0x4031 */
    status = D_80144031[index * 772];

    if (status == 0) {
        return 0;
    }

    if (a1 == 0) {
        return 1;
    }

    return func_800A1A60(child);
}

/**
 * func_800D6E00 - Allocate and signal type 11 object
 * (116 bytes)
 *
 * Allocates an object, sets its type to 11, stores parameter as byte 4,
 * and signals the secondary sync queue with the new object.
 *
 * @param a0 Value to store in object byte 4
 */
void func_800D6E00(s32 a0) {
    void *obj;

    func_80007270(&D_80142728[0], NULL, 1);

    obj = func_80091B00();

    *(u8*)((u8*)obj + 2) = 11;
    *(u8*)((u8*)obj + 4) = (u8)a0;

    func_800075E0(&D_80142728[0], NULL, 0);
    func_800075E0(&D_80142728[0x80], obj, 0);
}

/**
 * func_800D6348 - Allocate and signal type 9 object
 * (104 bytes)
 *
 * Allocates an object, sets its type to 9, and signals the secondary
 * sync queue with the new object. Similar to func_800D6E00 but with
 * no parameter and different type.
 */
void func_800D6348(void) {
    void *obj;

    func_80007270(&D_80142728[0], NULL, 1);

    obj = func_80091B00();
    *(u8*)((u8*)obj + 2) = 9;

    func_800075E0(&D_80142728[0], NULL, 0);
    func_800075E0(&D_80142728[0x80], obj, 0);
}

/* Global state flags */
extern s32 D_801170FC;

extern void func_8009079C(void*, s32);
extern void func_800D03DC(void*, void*);

/**
 * func_8010E828 - Process object with countdown timer
 * (132 bytes)
 *
 * If flag parameter is zero, initializes object. Otherwise processes
 * object's countdown timer and calls rendering function while timer
 * is active. Reinitializes when countdown reaches zero.
 *
 * @param a0 Pointer to parent object
 * @param a1 Skip init if non-zero
 */
void func_8010E828(void *a0, s16 a1) {
    void *child;
    s16 countdown;

    if (a1 == 0) {
        func_8009079C(a0, 1);
    }

    if (D_801170FC != 0) {
        return;
    }

    child = *(void**)((u8*)a0 + 12);
    countdown = *(s16*)((u8*)child + 90);

    if (countdown != 0) {
        *(s16*)((u8*)child + 90) = countdown - 1;
        func_800D03DC(*(void**)((u8*)child + 108), (void*)((u8*)child + 20));
        countdown = *(s16*)((u8*)child + 90);
    }

    if (countdown == 0) {
        func_8009079C(a0, 1);
    }
}

/* Effect/visual system arrays */
extern u8 D_80152038[480];      /* Effect state buffer */
extern u32 D_801569B8[];        /* Effect slot array (4 slots, 124 bytes each) */
extern f32 D_8016139C;          /* Effect timing float */
extern s32 D_80152738;          /* Effect counter */

extern void func_80002790(void*, s32, s32);  /* memset */

/**
 * func_800C3614 - Initialize effect system
 * (140 bytes)
 *
 * Clears effect state buffers and initializes all effect slots with
 * the marker value 0x15000000 in the first word of each slot.
 */
void func_800C3614(void) {
    u32 *ptr;

    /* Clear main effect buffer */
    func_80002790(D_80152038, 0, 480);

    /* Initialize each 124-byte effect slot */
    for (ptr = D_801569B8; ptr < (u32*)((u8*)D_801569B8 + 496); ptr = (u32*)((u8*)ptr + 124)) {
        func_80002790(ptr, 0, 124);
        *ptr |= 0x15000000;
    }

    /* Reset timing and counter */
    D_8016139C = 0.0f;
    D_80152738 = 0;
}

/* Object initialization external references */
extern u8 D_80140BDC;  /* Object type count */
extern s32 func_800B24EC(void*, void*, s32, s8, s32);  /* Object setup with type */
extern void func_800B362C(void*);  /* Object alternate init */
extern void func_80094EC8(void*);  /* Object finalize */

/**
 * func_800EF5B0 - Initialize object with optional type setup
 * (124 bytes)
 *
 * Stores a1 in a0[0], then either:
 * - If a2 != 0: calls func_800B24EC with type-1, stores result in a0[8]
 * - If a2 == 0: calls func_800B362C for alternate init
 * Finally calls func_80094EC8 to finalize.
 */
void func_800EF5B0(void *a0, void *a1, s32 a2) {
    s8 objType;
    s32 result;

    *(void**)a0 = a1;

    if (a2 != 0) {
        objType = (s8)(D_80140BDC - 1);
        result = func_800B24EC(a1, (void*)((u8*)a0 + 12), 0, objType, 1);
        *(s32*)((u8*)a0 + 8) = result;
    } else {
        func_800B362C(a0);
    }

    func_80094EC8(a0);
}

/* String builder external references */
extern void **D_801597F0;  /* Pointer to data structure */
extern void **D_80159800;  /* Pointer to source array */
extern void func_800B3D18(void);  /* Init/prepare function */

/**
 * func_800F68A4 - Build string from object array
 * (132 bytes)
 *
 * Copies bytes from offset 4 of each 12-byte entry in D_80159800 array
 * into the output buffer, null-terminated. Count comes from D_801597F0[12].
 */
void func_800F68A4(u8 *output) {
    void *data;
    void *arr;
    s32 i;
    s32 offset;
    s32 count;

    func_800B3D18();

    data = *D_801597F0;
    i = 0;
    offset = 0;
    count = *(u8*)((u8*)data + 12);

    while (i < count) {
        arr = *D_80159800;
        output[i] = *(u8*)((u8*)arr + offset + 4);
        i++;
        offset += 12;
        data = *D_801597F0;
        count = *(u8*)((u8*)data + 12);
    }

    output[i] = 0;
}

/* Timer calculation external references */
extern f32 func_80001578(void);  /* Get elapsed time */
extern f32 D_801247F8;           /* Time multiplier */
extern s16 D_80152032;           /* Timer value */
extern s32 D_801174B8;           /* Timer state flag */

/**
 * func_800FBF2C - Update timer value from elapsed time
 * (92 bytes)
 *
 * Gets elapsed time, multiplies by scale factor, and stores.
 * If result is <= 0, sets timer state flag to 0x100000.
 */
void func_800FBF2C(void) {
    f32 time;
    s16 value;

    time = func_80001578();
    time *= D_801247F8;
    value = (s16)time;
    D_80152032 = value;

    if (value <= 0) {
        D_801174B8 = 0x00100000;
    }
}

/* Linked list sync removal external references */
extern void **D_801527C8;  /* List head pointer */

/**
 * func_800E7A98 - Remove item from synchronized linked list
 * (172 bytes)
 *
 * Removes an object from the linked list at D_801527C8.
 * If a0 is NULL, removes the head item instead.
 * Uses sync primitives for thread safety.
 */
void func_800E7A98(void *a0) {
    void *item;
    void *curr;
    void *next;

    func_80007270(&D_80142728[0], NULL, 1);

    /* Get item to remove */
    if (a0 != NULL) {
        item = a0;
    } else {
        item = *D_801527C8;
    }

    /* Traverse list to find and unlink item */
    curr = *D_801527C8;
    if (curr != NULL) {
        while (1) {
            next = *(void**)((u8*)curr + 4);
            if (item == next) {
                /* Found - unlink by copying next's pointer */
                next = *(void**)((u8*)item + 4);
                *(void**)((u8*)curr + 4) = next;
                break;
            }
            if (next == NULL) break;
            curr = next;
        }
    }

    func_80095FD8(item, 1);
    func_800075E0(&D_80142728[0], NULL, 0);
}

/* Multi-player coordinate update external references */
extern s32 D_8015A110;           /* Game mode state */
extern s16 D_8015A108;           /* Player count */
extern void func_800E847C(void); /* Pre-update function */
extern void func_8008D6FC(s16, void*, void*);  /* Coordinate transform */
extern void func_800EB90C(void); /* Post-update function */

/**
 * func_800EC0DC - Update player coordinate transforms
 * (180 bytes)
 *
 * For each active player (when mode == 2 and count >= 2), calls
 * func_8008D6FC to update their coordinate data from arrays at
 * 0x80152BD0 (source) and 0x80152C20 (dest), stride 952 bytes.
 */
void func_800EC0DC(void) {
    s16 *srcBase;
    u8 *src;
    u8 *dst;
    s32 i;

    func_800E847C();

    if (D_8015A110 != 2) {
        goto done;
    }

    if (D_8015A108 < 2) {
        goto done;
    }

    srcBase = (s16*)0x80152BD0;
    src = (u8*)0x80152BD8;
    dst = (u8*)0x80152C20;
    i = 1;

    while (i < D_8015A108) {
        func_8008D6FC(*(srcBase + 123), src, dst);  /* offset 246 = 123 shorts */
        i++;
        srcBase = (s16*)((u8*)srcBase + 952);
        src += 952;
        dst += 952;
    }

done:
    func_800EB90C();
}

/* Object spawn with indexed data external references */
extern u8 D_80117530[];          /* Indexed object data array (48 bytes per entry) */
extern void **D_801491F0;        /* Linked list head */
extern f32 D_801249C0;           /* Float constant for object init */
extern void *func_80090284(void);  /* Allocate new object */
extern void func_800FEA00(void*, s8, void*, s32);  /* Object finalize */

/**
 * func_8010DAF8 - Spawn and initialize linked object
 * (192 bytes)
 *
 * Creates a new object via func_80090284, links it into D_801491F0 list,
 * initializes fields from indexed data array D_80117530, and finalizes
 * with func_800FEA00.
 */
void func_8010DAF8(void *a0) {
    s16 index;
    u8 *indexedData;
    void *obj;
    void *oldHead;

    index = *(s16*)((u8*)a0 + 16);
    indexedData = &D_80117530[index * 48];

    *(s16*)((u8*)a0 + 90) = 12;
    obj = func_80090284();

    if (obj == NULL) {
        return;
    }

    /* Initialize new object */
    *(s16*)((u8*)obj + 4) = 0;
    *(void**)((u8*)obj + 12) = a0;
    *(void**)((u8*)obj + 20) = *(void**)(indexedData + 12);
    *(f32*)((u8*)obj + 16) = D_801249C0;

    /* Clear bits 1 and 2 of byte at offset 4 */
    *(u8*)((u8*)a0 + 4) &= 0xFFF9;

    /* Link into list */
    oldHead = *D_801491F0;
    *(void**)obj = oldHead;
    *D_801491F0 = obj;

    /* Finalize with callback */
    func_800FEA00(
        *(void**)(D_80117530 + index * 48 + 28),
        *(s8*)((u8*)a0 + 92),
        (void*)((u8*)a0 + 56),
        2
    );
}

/*============================================================================
 * CORE MEMORY MANAGEMENT FUNCTIONS
 * These are called extensively throughout the game code.
 *============================================================================*/

/* Pool allocator globals */
extern u8 D_80142DD8[];   /* Object pool base (24-byte slots) */
extern u8 D_801439D8[];   /* Pool end marker */

/**
 * func_80091B00 - Allocate object from pool
 * (168 bytes)
 *
 * Searches the object pool at D_80142DD8 for a free 24-byte slot.
 * Each 96-byte block contains 4 slots. A slot is free if byte at offset 3 is 0.
 * When allocated, sets in_use flag to 1 and initializes short at offset 0 to -1.
 *
 * @return Pointer to allocated slot, or NULL if pool exhausted
 */
void *func_80091B00(void) {
    u8 *base = D_80142DD8;
    u8 *end = D_801439D8;
    u8 *ptr = base;

    while (ptr < end) {
        /* Check slot 0 (offset 0) */
        if (*(s8*)(ptr + 3) == 0) {
            *(s8*)(ptr + 3) = 1;
            *(s16*)ptr = -1;
            return ptr;
        }
        /* Check slot 1 (offset 24) */
        if (*(s8*)(ptr + 27) == 0) {
            *(s8*)(ptr + 27) = 1;
            *(s16*)(ptr + 24) = -1;
            return ptr + 24;
        }
        /* Check slot 2 (offset 48) */
        if (*(s8*)(ptr + 51) == 0) {
            *(s8*)(ptr + 51) = 1;
            *(s16*)(ptr + 48) = -1;
            return ptr + 48;
        }
        /* Check slot 3 (offset 72) */
        if (*(s8*)(ptr + 75) == 0) {
            *(s8*)(ptr + 75) = 1;
            *(s16*)(ptr + 72) = -1;
            return ptr + 72;
        }
        ptr += 96;  /* Advance to next 4-slot block */
    }

    return NULL;
}

/* Object deactivation globals */
extern void **D_801491F0;   /* Active object list head */
extern void **D_801492C8;   /* Free object list head */
extern s16 D_8013E66C;      /* Active object count */
extern s16 D_8013E678;      /* Maximum active objects seen */
extern void func_8008AE8C(s32, s32, s32);  /* Sound stop function */

/**
 * func_8009079C - Deactivate and free an object
 * (260 bytes)
 *
 * Stops any associated sound, removes object from active list,
 * and moves it to the free list. Updates object count tracking.
 *
 * @param a0 Object pointer to deactivate
 * @param a1 If non-zero, remove from active list
 */
void func_8009079C(void *a0, s32 a1) {
    s16 soundId;
    void *curr;
    void *prev;
    void *next;
    s16 count;

    /* Stop associated sound if any */
    soundId = *(s16*)((u8*)a0 + 6);
    if (soundId >= 0) {
        func_8008AE8C(soundId, 1, 15);
    }

    /* Clear object state */
    *(void**)((u8*)a0 + 20) = NULL;
    *(s16*)((u8*)a0 + 6) = -1;

    /* Remove from active list if requested */
    if (a1 != 0) {
        curr = *D_801491F0;

        /* Check if we're the head */
        if (a0 == curr) {
            *D_801491F0 = *(void**)a0;
        } else {
            /* Search list for our predecessor */
            prev = NULL;
            while (curr != NULL) {
                next = *(void**)curr;
                if (a0 == next) {
                    break;
                }
                if (next == NULL) break;
                prev = curr;
                curr = next;
            }

            /* Unlink from list */
            if (prev != NULL && *(void**)prev == a0) {
                *(void**)prev = *(void**)a0;
            }
        }
    }

    /* Add to free list */
    *(void**)a0 = *D_801492C8;
    *D_801492C8 = a0;

    /* Update count and max tracking */
    D_8013E66C--;
    count = D_8013E66C;
    if (D_8013E678 < count) {
        D_8013E678 = count;
    }
}

/* Object spawn globals */
extern u8 D_8012F5CC[];  /* Type lookup table */
extern void *func_80092278(void);  /* Allocate sub-object */

/**
 * func_80092360 - Spawn a new game object
 * (292 bytes)
 *
 * Creates a new object with type 2, initializes it with the given parameters,
 * and signals the object system. Core object creation function.
 *
 * @param a0 Owner/parent index
 * @param a1 Mode/flag
 * @param a2 Object type ID
 * @param a3 Flags byte (from stack)
 * @return Pointer to the created sub-object's data
 */
void *func_80092360(s32 a0, s32 a1, s32 a2, u8 a3) {
    void *obj;
    void *subObj;
    void *result;

    func_80007270(&D_80142728[0], NULL, 1);

    obj = func_80091B00();

    /* Set object type and ID */
    *(u8*)((u8*)obj + 2) = 2;
    *(s32*)((u8*)obj + 4) = a2;

    /* Allocate and link sub-object */
    subObj = func_80092278();
    *(void**)((u8*)obj + 20) = subObj;
    *(void**)((u8*)subObj + 20) = NULL;

    /* Initialize sub-object fields */
    *(s32*)((u8*)subObj + 52) = a0;  /* Owner */
    *(s32*)((u8*)subObj + 56) = a1;  /* Mode */
    *(u8*)((u8*)subObj + 27) = a3;   /* Flags */
    *(s32*)((u8*)subObj + 16) = 1;   /* Active flag */
    *(u8*)((u8*)subObj + 24) = 0;
    *(u8*)((u8*)subObj + 26) = 1;
    *(u8*)((u8*)subObj + 25) = D_8012F5CC[a0];  /* Type from lookup */

    /* Store float constants at object offsets 8, 12, 16 */
    *(f32*)((u8*)obj + 8) = 1.0f;
    *(f32*)((u8*)obj + 12) = -2.0f;
    *(f32*)((u8*)obj + 16) = -2.0f;

    /* Get result before releasing sync */
    result = *(void**)((u8*)subObj + 12);

    func_800075E0(&D_80142728[0], NULL, 0);
    func_800075E0(&D_80142728[0x80], obj, 0);

    return result;
}

/**
 * func_8008D6B0 - Copy 3x3 matrix or 9-float vector
 * (76 bytes)
 *
 * Copies 36 bytes (9 floats) from source to destination.
 * Used for copying position/orientation data or 3x3 matrices.
 *
 * @param src Source pointer
 * @param dst Destination pointer
 */
void func_8008D6B0(f32 *src, f32 *dst) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    dst[4] = src[4];
    dst[5] = src[5];
    dst[6] = src[6];
    dst[7] = src[7];
    dst[8] = src[8];
}

/**
 * func_80090284 - Allocate object from free list
 * (132 bytes)
 *
 * Pops an object from the free list at D_801492C8, initializes it,
 * and updates count tracking. The complementary function to func_8009079C.
 *
 * @return Pointer to allocated object, or NULL if list empty
 */
void *func_80090284(void) {
    void *obj;
    void *next;
    s16 count;

    obj = *D_801492C8;
    if (obj == NULL) {
        return NULL;
    }

    /* Increment count and update max */
    D_8013E66C++;
    count = D_8013E66C;
    if (D_8013E678 < count) {
        D_8013E678 = count;
    }

    /* Pop from free list */
    next = *(void**)obj;
    *D_801492C8 = next;

    /* Initialize object fields */
    *(void**)obj = NULL;           /* next pointer */
    *(s16*)((u8*)obj + 4) = 0;     /* state */
    *(s16*)((u8*)obj + 6) = -1;    /* sound id */
    *(s16*)((u8*)obj + 8) = 0;     /* flags */
    *(void**)((u8*)obj + 12) = NULL; /* data pointer */
    *(void**)((u8*)obj + 20) = NULL; /* extra pointer */
    *(f32*)((u8*)obj + 16) = 0.0f;  /* timer/value */

    return obj;
}

/**
 * func_8008AD04 - Compare two strings
 * (68 bytes)
 *
 * Compares two null-terminated strings byte by byte.
 *
 * @param a0 First string
 * @param a1 Second string
 * @return 0 if equal, difference of first non-matching bytes otherwise
 */
s32 func_8008AD04(u8 *a0, u8 *a1) {
    u8 c1, c2;

    c1 = *a0;
    if (c1 == 0) {
        c2 = *a1;
        return c1 - c2;
    }

    do {
        c2 = *a1;
        if (c1 != c2) {
            c2 = *a1;
            return c1 - c2;
        }
        a0++;
        a1++;
        c1 = *a0;
    } while (c1 != 0);

    c2 = *a1;
    return c1 - c2;
}

/* Effects system globals */
extern s8 D_801147C4;          /* Effects system initialized flag */
extern u32 D_801551E8;         /* Effect count 1 */
extern u32 D_801551EC;         /* Effect count 2 */
extern u32 D_801551F0[8];      /* Effect data array (32 bytes / 8 words) */

/**
 * func_800B73E4 - Initialize effects/emitter system
 * (84 bytes)
 *
 * One-time initialization that clears effect counters and data arrays.
 * Sets init flag to prevent re-initialization.
 */
void func_800B73E4(void) {
    u32 *ptr;
    u32 *end;

    /* Check if already initialized */
    if (D_801147C4 != 0) {
        return;
    }

    /* Mark as initialized */
    D_801147C4 = 1;

    /* Clear effect counters */
    D_801551EC = 0;
    D_801551E8 = 0;

    /* Clear effect data array (32 bytes, 8 words) */
    ptr = D_801551F0;
    end = D_801551F0 + 8;

    do {
        ptr[0] = 0;
        ptr[1] = 0;
        ptr[2] = 0;
        ptr[3] = 0;
        ptr += 4;
    } while (ptr != end);
}

/* Emitter data arrays */
extern void *D_80124FD0[];      /* Pointer array for emitter targets */
extern u8 D_80150B70[];         /* Emitter data array (152 byte stride) */

/**
 * func_800B80C8 - Copy emitter position floats to target
 * (80 bytes)
 *
 * If the target pointer exists, copies two position floats from
 * the emitter data array to the target object.
 *
 * @param a0 Emitter index
 */
void func_800B80C8(s32 a0) {
    void *target;
    u8 *emitter;

    target = D_80124FD0[a0];

    if (target == NULL) {
        return;
    }

    /* Calculate emitter data offset: a0 * 152 */
    emitter = D_80150B70 + (a0 * 152);

    /* Copy two position floats from emitter to target */
    *(f32*)((u8*)target + 36) = *(f32*)(emitter + 36);
    *(f32*)((u8*)target + 44) = *(f32*)(emitter + 44);
}

/* Emitter count */
extern s16 D_80151AD0;  /* Number of active emitters */

/**
 * func_800B90F8 - Clear emitter pointer array
 * (48 bytes)
 *
 * Clears the emitter target pointer array up to the current count.
 */
void func_800B90F8(void) {
    s16 count;
    void **ptr;
    void **end;

    count = D_80151AD0;
    ptr = D_80124FD0;

    if (count <= 0) {
        return;
    }

    end = D_80124FD0 + count;

    do {
        *ptr = NULL;
        ptr++;
    } while (ptr < end);
}

/* Additional emitter system globals */
extern u32 D_80159818;          /* Emitter system state 1 */
extern u32 D_80159B80;          /* Emitter system state 2 */
extern void *D_80143A18;        /* Emitter list 1 */
extern void *D_80143A20;        /* Emitter list 2 */
extern void *D_80143A28;        /* Emitter list 3 */
extern u8 D_80143A10[44];       /* Emitter data block */
extern void func_80096130(void*);  /* Clear emitter list */
extern void func_80002790(void*, s32, s32);  /* memset */

/**
extern s32 func_800950AC(void*, void*, s32);  /* Object compare/lookup */

/**
extern void func_80098620(void*, void*, void*);  /* List insert operation */

/**
extern void func_80096288(s32, s32, s32);  /* Sound/object operation */
extern u8 D_80156D44[];  /* Object data array (20-byte stride) */

/**
extern void func_8009C3F8(s32, f32);  /* Floating point operation */

/**
extern void func_8009211C(void*, void*);  /* List remove operation */

/**
extern void *D_801146FC;  /* Default data pointer */
extern void func_80094EC8(void*);  /* Object copy/update function */

/**
extern void *D_80116DE0;  /* Another default data pointer */

/**
extern u8 D_80152750[];  /* Sync object for audio/sound */

/**
extern s32 func_80097470(s32, s32);  /* Sound lookup function */
extern void func_800962D4(s32, s32); /* Sound state update */

/**
extern u8 D_80153F10[];  /* Timer/callback structure */
extern void func_8008ABE4(void);  /* Timer callback trigger */

/**
extern void func_800B82C8(void);  /* Effects cleanup */

/**
extern void func_800E23A4(void);  /* Pre-process call */
extern void func_800E1C30(s32 a0);  /* Post-process call */

/**
extern void *func_800A2680(void *ptr);  /* Object processor */

/**
extern void *func_800B466C(void *a0, s32 a1);  /* Object allocator */
extern void func_800A2504(void *a0, void *a1, s32 a2);  /* Object copy */

/**
extern s32 func_8000BE50(s32 a0);  /* State query */
extern void func_800C9AE0(void);   /* State handler */

/**
extern void func_80092360(s32 a0, s32 a1, s32 a2, s32 a3);  /* Multi-param handler */

/**
extern void func_8009211C(s32 a0);  /* List operation */

/**
extern void func_800B358C(void);  /* Cleanup handler */
extern u32 D_801541A4;  /* State flag */

/**
extern u8 D_80156D38[];  /* Object array base (20-byte stride) */
extern void func_80096288(s32 a0, s32 a1, s32 a2);  /* Object init */
extern void func_800962D4(s32 a0, s32 a1);  /* Object state set */

/**
extern void *D_801491F0;  /* Active object list head */
extern void func_8009079C(void *a0, s32 a1);  /* Process object */
extern void func_800B0580(void);  /* Finalize processing */

/**
extern void func_800D6160(s32 flag);  /* Core update function */

/**
/**
extern u8 D_80142728[];  /* Primary sync object */
extern u8 D_801427A8[];  /* Secondary sync object */
extern void func_80007270(void *sync, s32 a1, s32 a2);  /* Acquire sync */
extern void func_800075E0(void *sync, s32 a1, s32 a2);  /* Release sync */
extern void *func_80091B00(void);  /* Allocate from list */

/**
extern u8 D_80152770[];  /* Another sync object */
extern void *func_80095F8C(s32 a0);  /* Object lookup */
extern void *func_80095EF4(void *a0, s32 a1);  /* Object process */

/**
extern void func_80095FD8(s32 a0, s32 a1);  /* Object remove */

/**
extern s32 D_8015A110;  /* Current game state */
extern void func_803914B4(s32 a0, s32 a1, s32 a2);  /* External handler */

/**
extern s32 func_80097694(s32 a0, s32 a1);  /* Object lookup by type */

/**
/**
/**
/**
extern void func_800CF06C(void);  /* Pre-update */
extern void func_800E1AA0(s32 a0);  /* Update stage 1 */
extern void func_800E15A0(s32 a0);  /* Update stage 2 */
extern void func_800E1540(s32 a0);  /* Update stage 3 */
extern void func_800E114C(s32 a0);  /* Update stage 4 */
extern void func_800D0424(s32 a0);  /* Update stage 5 */

/**
extern void func_800BF024(s32 a0);  /* Process with flag */
extern void func_80091C04(s32 a0);  /* Process without flag */
extern void func_800D54BC(void *a0);  /* Clear structure */

/**
/**
/**
/**
extern void func_800B3D18(s32 a0);  /* State reset with flag */
extern void *D_801597F0;  /* Global state pointer */
extern u8 D_80159B70;  /* State byte A */
extern u8 D_80159B60;  /* State byte B */

/**
/**
/**
extern void *D_80114624;  /* Single global object */
extern void *D_80114628[];  /* Array of 4 object pointers */
extern void func_8008D0C0(void *a0);  /* Object cleanup */

/**
extern void func_800B74A0(s32 a0);  /* Prepare for operation */
extern void func_800B71D4(s16 a0, s16 a1, s32 a2);  /* Execute operation */

/**
extern void func_800BE7BC(void *buf, s32 a1, s16 a2);  /* Fill buffer */

/**
extern void func_80002CD0(void *buf, s32 a1, void *a2);  /* Copy to buffer */

/**
extern void func_80096CA8(s32 a0, s32 a1);  /* Process with second parameter */

/**
extern void *func_80091BA8(s32 a0);  /* Find element in sync list */
extern void func_800BF01C(s32 a0);   /* Process nested element */
extern void func_80091C04(s32 a0);   /* Remove element from list */

/**
extern void func_800A4B6C(void);      /* Initialize subsystem A */
extern void func_80096238(s32 a0);    /* Configure with parameter */
extern void func_80020274(void);      /* Start operation */
extern s32 func_800202C4(void);       /* Poll completion status */

extern s32 D_80151A6C;  /* Configuration value */

/**
extern void func_800B90F8(void);      /* Cleanup subsystem */
extern void func_80096130(s32 a0);    /* Release resource */
extern void func_80002790(void *dst, s32 val, s32 size);  /* memset */

extern s32 D_80143A18;  /* Resource handle A */
extern s32 D_80143A20;  /* Resource handle B */
extern s32 D_80143A28;  /* Resource handle C */
extern void *D_80143A10; /* Resource array base */

/**
extern void func_800A5A40(void);  /* Initialize subsystem */

extern s32 D_801513B4;    /* Global flag A */
extern s16 D_8016B254;    /* Global state value */
extern f32 D_801613B8;    /* Scale factor */
extern s16 D_80140618;    /* Position value */
extern void *D_801406B8;  /* Data pointer */
extern void *D_8012EA18;  /* Source data */

/**
extern u8 *D_80159800;  /* Source data array */

/**
extern void func_800BAAA0(void);  /* Initialize state arrays */

extern f32 D_80153F28[6];  /* State array A */
extern f32 D_80153F48[6];  /* State array B */
extern f32 D_80153F68[6];  /* State array C */
extern f32 D_80154190;     /* Copied scale value */
extern f32 D_801543CC;     /* Source scale value */
extern s16 D_80154182;     /* State flag */

/**
extern void *D_80152770;           /* Sync object */
extern void *func_80095F8C(s32 a0);  /* Find element by id */
extern void *func_80095EF4(void *elem, s32 a0, s32 a1);  /* Process element */

/**
extern void func_800B6024(void);  /* Initialize base state */

extern s16 D_8015A108;    /* Element count */
extern void *D_8015A118;  /* Element array base */

/**
extern void *func_800B24EC(void *a0, void *a1, s32 a2, s8 a3, s32 a4);  /* Create linked element */
extern void func_800B362C(void *a0);   /* Alternate init */
extern void func_80094EC8(void *a0);   /* Finalize object */

extern u8 D_80140BDC;  /* Default parameter value */

/**
extern void func_80092360(s32 a0, s32 a1, s32 a2, s32 a3);  /* Trigger action */

/**
/**
extern void *func_80096B00(s32 a0, s32 a1);  /* Lookup element */

/**
extern void func_80096288(s32 a0, s32 a1, s32 a2);  /* Initialize element entry */
extern void func_800962D4(s32 a0, s32 a1);          /* Configure element */

extern void *D_80156D38;  /* Element array base */

/**
/**
/**
/**
/**
/**
extern s16 func_800B3FA4(s32 a0, s32 a1);  /* Get dimension value */

/**
/**
extern void func_8009211C(s32 a0, void *a1);  /* Add element to list */

/**
extern void *D_80116DE0;  /* Default handler table */

/**
extern void *D_80152750;  /* Secondary sync object */

/**
extern s32 func_80097470(s32 a0, s32 a1);  /* Allocate resource */

/**
extern void *D_80153F10;  /* State structure base */
extern void func_8008ABE4(void);  /* Finalize state init */

/**
/**
extern u8 D_80110680[2];  /* State flag array */
extern void func_800BADE0(void);  /* Initialize state handler */

/**
extern u8 D_80143A10;  /* Global state byte */
extern s8 D_8015978C;  /* Configuration value */
extern void func_800BB9B0(s32 a0, s32 a1, s32 a2);  /* Configure subsystem */

/**
extern void func_8009C3F8(s32 a0);  /* Process with mode */

/**
extern void func_80096288(s32 a0, s32 a1, s32 a2);  /* Configure element */
extern void func_800962D4(s32 a0, s32 a1);  /* Set element state */

/**
extern void func_8008E26C(s32 a0, s32 a1, s16 a2);  /* Base function */

/**
/**
/**
/**
/**
/**
extern s8 D_80159B70;  /* Global state byte */
extern s8 D_80159B60;  /* Global state byte 2 */

/**
/**
extern void *D_80152770;  /* Sync object 3 */
extern void func_80095F8C(void *a0);  /* Lookup function */
extern void *func_80095EF4(void *a0, s32 a1);  /* Lookup and return */

/**
extern s32 func_80097694(s32 a0, s32 a1);  /* Lookup by index */

/**
/**
extern void *D_801491F0;  /* Resource list head */
extern void func_8009079C(void *a0, s32 a1);  /* Process resource */
extern void func_800B0580(void);  /* Initialize resources */

/**
/**
/**
extern s16 D_80151AD0;  /* Resource count */
extern s32 *D_80124FD0;  /* Resource array */

/**
extern void *D_801146FC;  /* Default handler pointer */

/**
extern void func_80095FD8(void *a0, s32 a1);  /* Process resource */

/**
/**
extern void func_800D6160(s32 a0);  /* Core physics update */

/**
/**
extern void func_80092360(s32 a0, s32 a1, s32 a2, s32 a3);  /* Trigger event */

/**
/**
extern s32 D_801174B4;  /* Game state flags */

/**
 * func_800B7FF8 - Get game state flags
 * (8 bytes)
 *
 * Returns the current game state flags word.
 *
 */
s32 func_800B7FF8(void) {
    return D_801174B4;
}

extern s32 D_80128E20;  /* Secondary state */

/**
 * func_800B71D4 - Get secondary state
 * (8 bytes)
 *
 * Returns the secondary state value.
 *
 */
s32 func_800B71D4(void) {
    return D_80128E20;
}

/**
/**
/**
/**
extern u8 D_80140BDC;  /* Global counter/index */

/**
/**
extern s32 D_80151A6C;  /* Resource handle */

/**
/**
/**
extern void *D_801597F0;  /* Global state pointer */
extern void *D_80159800;  /* Secondary state pointer */

/**
extern s8 D_80159B60;  /* State offset byte */

/**
extern void *D_80152770;  /* Tertiary sync object */

/**
/**
/**
extern s16 D_801525F0;   /* Entity counter */
extern s8 D_80152744;    /* Entity count limit */
extern u8 *D_8015A250;   /* Entity array base - 2056 byte elements */

/**
/**
/**
/**
/**
/**
/**
/**
/**
/**
extern void *D_80116DE0;  /* Default handler pointer */
extern void *D_80152750;  /* Sync object for UI */

/**
/**
/**
extern u8 D_80153F10;  /* Timer/callback state struct base */

/**
/*
/*
/*
/*
/*
/*
/*
/*
extern void *D_80146188;  /* Entity list sync */
extern void *D_80146170;  /* Entity list head */

/*
extern s32 D_80151A6C;  /* Element lookup key */

/*
/*
/*
/*
/*
/*
extern void *D_801146FC;  /* Default element handler */

/*
extern u8 D_80159B74;  /* Player state bytes array */

/*
 * func_800C84C0 (60 bytes)
 * Set player state byte(s)
 */
void func_800C84C0(s32 a0, s8 a1) {
    u8 *arr = &D_80159B74;

    if (a0 == -1) {
        /* Set all 4 player states */
        arr[0] = a1;
        arr[1] = a1;
        arr[2] = a1;
        arr[3] = a1;
    } else if (a0 < 4) {
        arr[a0] = a1;
    }
}

extern u8 D_801147C4;     /* Sound init flag */
extern s32 D_801551E8;    /* Sound buffer start */
extern s32 D_80155210;    /* Sound buffer end */

/*
/*
/*
/*
/*
extern u8 D_80159B70;  /* Track state byte 1 */
extern u8 D_80159B60;  /* Track state byte 2 */

/*
/*
/*
extern s32 D_801597D0;  /* Sync object for player system */

/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
extern s32 D_80144D60;  /* Entity type table */

/*
/*
extern s32 D_80159428;  /* Player score/state buffer 1 */
extern s32 D_8015256C;  /* Player score/state buffer 2 */

/*
/*
/*
extern u8 D_80110680[2];  /* Control flags */

/*
extern u8 D_80143A10;   /* Race state flag */
extern s8 D_8015978C;   /* Player index */

/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
extern u8 D_80140BDC;  /* Player count or something similar */

/*
/*
/*
/*
/*
/*
extern s32 D_8012EAA0;
extern void *D_80151AD4;

/*
extern void *D_801597F0;

/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
extern u8 D_80110680;

/*
extern u8 D_80156D38[];

/*
/*
/*
extern s32 D_801597D0;

/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
extern u8 D_80143A10;
extern s8 D_8014978C;

/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
 * func_8009731C (104 bytes)
 * Synchronized element lookup returning offset 12 data
 */
void *func_8009731C(void *input) {
    void *temp;
    void *result;
    void *data;

    sync_acquire(&D_80152770, 0, 1);
    temp = func_80095F8C(input, input);
    result = func_80095EF4(temp, 0);
    data = *(void **)((u8 *)result + 12);
    sync_release(&D_80152770, 0, 0);
    return data;
}

/*
 * func_800C7578 (148 bytes)
 * Update indexed slot in nested structure and notify
 */
void func_800C7578(void **a0, s32 a1, s32 a2, s32 a3) {
    u32 *ptr;
    u8 *base;
    u8 *slot;
    s8 new_val;
    u8 idx1, idx2;

    ptr = (u32 *)*a0;
    idx2 = (u8)a2;
    new_val = (s8)a3;
    base = *(u8 **)*(u32 **)((u8 *)ptr + 44);
    idx1 = (u8)a1;

    slot = base + (idx1 << 4) + idx2 + 1860;
    if (new_val != *(s8 *)slot) {
        *(s8 *)slot = new_val;
        *(void **)(base + (idx1 << 4) + 1856) = func_800B466C(base + (idx1 << 4) + 1860, 12);
        func_800A2504(*(void **)((u8 *)ptr + 8), base + (idx1 << 4) + 1856, 16);
    }
}

/*
 * func_800CD058 (156 bytes)
 * Conditional render/process dispatch based on object state
 */
void func_800CD058(void *a0) {
    void *ptr;

    ptr = *(void **)a0;
    if (*(void **)((u8 *)ptr + 8) != NULL) {
        if (func_800CCEFC(a0) != 0) {
            ptr = *(void **)a0;
            func_800A2680(*(void **)((u8 *)ptr + 8));
        }
    } else {
        func_800CCCCC(a0);
    }
}

/*
 * func_800AF51C (196 bytes)
 * Initialize resource arrays - allocates 50 resources and clears 6 slot entries
 */
void func_800AF51C(void) {
    s16 i;
    void *result;

    /* Allocate 50 resources and store in array */
    for (i = 0; i < 50; i++) {
        result = func_800A7D6C();
        *(void **)((u8 *)&D_8014C238 + (i * 4)) = result;
    }

    /* Initialize 6 slot entries with -1 */
    for (i = 0; i < 6; i++) {
        *(s32 *)((u8 *)&D_80154FD8 + (i * 60)) = -1;
        *(s32 *)((u8 *)&D_80154660 + (i * 400) + 0) = -1;
        *(s32 *)((u8 *)&D_80154660 + (i * 400) + 84) = -1;
        *(s32 *)((u8 *)&D_80154660 + (i * 400) + 168) = -1;
        *(s32 *)((u8 *)&D_80154660 + (i * 400) + 252) = -1;
        *(s32 *)((u8 *)&D_80154660 + (i * 400) + 336) = -1;
    }

    D_8014C094 = 0;
}

/*
 * func_8008E19C (208 bytes)
 * Insert value into tree node - either at head or at indexed slot
 */
void func_8008E19C(s16 a0, s16 a1) {
    s16 new_idx;
    u8 *entry;

    if (a1 < 0) {
        /* Insert at head if empty, else allocate new node */
        if (D_8016B254 < 0) {
            D_8016B254 = a0;
        } else {
            new_idx = func_8008E144();
            *(s16 *)((u8 *)&D_8013E700 + (new_idx * 68) + 24) = a0;
        }
    } else {
        /* Insert at indexed slot if empty, else allocate new node */
        entry = (u8 *)&D_8013E700 + (a1 * 68);
        if (*(s16 *)(entry + 22) == -1) {
            *(s16 *)(entry + 22) = a0;
        } else {
            new_idx = func_8008E144();
            *(s16 *)((u8 *)&D_8013E700 + (new_idx * 68) + 24) = a0;
        }
    }
}

/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
 * func_8008A398 (76 bytes)
 * Note: Audio sync related - needs analysis
 */

/*
 * func_8008C724 (68 bytes)
 * Note: Audio processing - needs analysis
 */

/*
 * func_800991C04 placeholder
 * Note: Complex function - needs analysis
 */

/*
 * func_8008B26C (72 bytes)
 * Note: Matrix operation - needs analysis
 */

/*
 * func_8009515C (60 bytes)
 * Note: Copy/init operation - needs analysis
 */

/*
 * func_800959DC (72 bytes)
 * Note: Render setup - needs analysis
 */

/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
 * func_8008A474 (464 bytes)
 * Rectangle clipping and RDP fill command
 * Clips coordinates to screen bounds
 */
void func_8008A474(s32 x1, s32 y1, s32 x2, s32 y2, s32 color) {
    s32 minX, minY, maxX, maxY;
    u32 *dlPtr;

    /* t2=x1, t3=y1, t4=x2, t5=y2 after clipping */

    /* Clip x1 to screen min */
    minX = x1;
    if (x1 < *(s32 *)0x8012E60C) {
        minX = *(s32 *)0x8012E60C;
    }

    /* Clip y1 to screen min */
    minY = y1;
    if (y1 < *(s32 *)0x8012E668) {
        minY = *(s32 *)0x8012E668;
    }

    /* Clip x2 to screen max */
    maxX = x2;
    if (*(s32 *)0x8012E610 < x2) {
        maxX = *(s32 *)0x8012E610;
    }

    /* Clip y2 to screen max */
    maxY = y2;
    if (*(s32 *)0x8012E674 < y2) {
        maxY = *(s32 *)0x8012E674;
    }

    /* Check if rectangle is valid */
    if (maxX < minX || maxY < minY) {
        return;
    }

    /* Generate G_RDPPIPESYNC */
    dlPtr = *(u32 **)0x80149438;
    *(u32 **)0x80149438 = dlPtr + 2;
    dlPtr[1] = 0;
    dlPtr[0] = 0xE7000000;

    /* Generate G_SETFILLCOLOR */
    dlPtr = *(u32 **)0x80149438;
    *(u32 **)0x80149438 = dlPtr + 2;
    dlPtr[1] = color;
    dlPtr[0] = 0xF7000000;

    /* Generate G_FILLRECT - coordinates in 10.2 fixed point */
    dlPtr = *(u32 **)0x80149438;
    *(u32 **)0x80149438 = dlPtr + 2;
    dlPtr[0] = 0xF6000000 | ((maxX << 2) << 12) | (maxY << 2);
    dlPtr[1] = ((minX << 2) << 12) | (minY << 2);
}

/*
 * func_8008A650 (84 bytes)
 * Set Z-buffer primitive depth via RDP command
 * t7 preloaded with current value, a2 with dest ptr
 */
void func_8008A650(s16 depth) {
    u32 *dlPtr;
    u16 val = (u16)(depth & 0xFFFF);
    u16 current = *(u16 *)0x8013E600;  /* t7 preload from a2 */

    if (val != current) {
        dlPtr = *(u32 **)0x80149438;
        *(u32 **)0x80149438 = dlPtr + 2;
        dlPtr[1] = val << 16;
        dlPtr[0] = 0xEE000000;  /* G_SETPRIMDEPTH */
        *(u16 *)0x8013E600 = val;
    }

    func_800878E0(16);
}

/*
 * func_8008A710 (100 bytes)
 * Conditional sync initialization
 * t6 preloaded with flag from v0
 */
void func_8008A710(void) {
    u8 flag = *(u8 *)0x8011194C;  /* t6 from v0 */
    s32 outVal;

    if (flag == 0) {
        *(u8 *)0x8011194C = 1;
        sync_init((void *)0x801497D0, (void *)0x801527E4, 1);
        sync_release((void *)0x801497D0, 0, 0);
    }

    sync_acquire((void *)0x801497D0, &outVal, 1);
}

/*
/*
/*
 * func_8008D714 (80 bytes)
 * Copy position vector and optionally call update
 * t8 preloaded with index offset
 */
void func_8008D714(s32 idx, f32 *pos, void *update) {
    void *target;

    /* a3 = a3 + t8, then load from table */
    target = *(void **)(0x8012E708 + idx);

    if (pos != NULL) {
        *(f32 *)((u8 *)target + 36) = pos[0];
        *(f32 *)((u8 *)target + 40) = pos[1];
        *(f32 *)((u8 *)target + 44) = pos[2];
    }

    if (update != NULL) {
        func_8008D6B0(update, target);
    }
}

/*
 * func_8008D8D8 (100 bytes)
 * Texture loading with mode select
 * s0, t0 preloaded
 */
s32 func_8008D8D8(s32 mode) {
    s16 texId;
    s32 result = 0;  /* s0 preload */
    void *texPtr = NULL;  /* t0 preload */

    if (mode < 0) {
        return func_8008B26C(texPtr);
    }

    texId = *(s16 *)((u8 *)texPtr + 6);

    if (result != 0) {
        /* sllv creates shift value */
        func_8008B0D8(texId, 0, result);
    } else {
        func_8008AE8C(texId, 1, result);
    }

    return result;
}

/*
/*
 * func_800A373C (64 bytes)
 * Clear struct with preserved byte
 * t8, t7 preloaded with base and index
 */
void func_800A373C(s32 index) {
    void *ptr;
    s8 savedByte;

    /* Calculate ptr from index table */
    ptr = (void *)(0x80144030 + index * 4);

    savedByte = *(s8 *)((u8 *)ptr + 5);
    func_80002790(ptr, 0, 772);
    *(s8 *)((u8 *)ptr + 5) = savedByte;
}

/*
/*
 * func_8008E0C8 (124 bytes)
 * Vector transform with normalization
 * Writes result to a0 as 3-float vector
 */
void func_8008E0C8(f32 *dest, f32 f12, f32 f14) {
    f32 temp, f2, f4, f6, f8, f10, f12_sq, f18;
    f32 scale;

    temp = f12 * f12;
    f18 = temp;
    temp = temp + (f14 * f14);
    f4 = temp + (f12 * f12);  /* Re-use f12 component */
    f6 = *(f32 *)0x8012394C;
    scale = 1.0f;

    f2 = sqrtf(f4);

    if (f2 <= f6) {
        /* Zero output */
        return;
    }

    /* Normalize */
    scale = 1.0f / f2;
    f10 = f12 * scale;
    f4 = f14 * scale;
    dest[0] = f10;
    dest[1] = f4;
    f6 = f18;
    f8 = f6 * scale;
    dest[2] = f8;
}

/*
 * func_8008ABF4 (128 bytes)
 * Audio queue processing - dequeue and play
 * t0, t6 = current and max index, v1 = queue pointer
 */
s32 func_8008ABF4(void) {
    s32 currIdx;  /* t0 preload */
    s32 maxIdx;   /* t6 preload */
    void *queue;  /* v1 preload */
    s32 nextIdx;
    u32 *entry;
    s32 param1, param2, param3;

    currIdx = 0;  /* Preload from caller */
    maxIdx = 0;   /* Preload from caller */
    queue = NULL; /* Preload from caller */

    if (currIdx >= maxIdx) {
        return 0;
    }

    nextIdx = currIdx + 1;
    *(s16 *)((u8 *)queue + 4) = (s16)nextIdx;

    /* Calculate entry offset: (nextIdx * 3) * 4 = nextIdx * 12 */
    entry = (u32 *)((u8 *)*(u32 *)((u8 *)queue + 8) + ((nextIdx & 0xFFFF) * 12));

    param1 = entry[-3];  /* offset -12 */
    param2 = entry[-2];  /* offset -8 */
    param3 = entry[-1];  /* offset -4 */

    func_80008630((void *)0x80161438, 1, 0, param2, param3, param1, (void *)0x80153E68);

    return 1;
}

/*
 * func_8008AC74 (144 bytes)
 * Sync initialization and processing loop
 * Initializes sync object and loops calling func_8008ABE4
 */
void func_8008AC74(void *arg) {
    void *syncObj = (void *)0x80153E68;
    void *syncName = (void *)0x80153EF0;
    u8 *statusByte = (u8 *)0x80153F10;

    /* Initialize sync object */
    func_80006A00(syncObj, syncName, 1);

    *statusByte = 0;

    /* Loop while func_8008ABE4 returns non-zero */
    while (1) {
        func_80007270(syncObj, 0, 1);  /* sync_acquire */

        if (func_8008ABE4() == 0) {
            *statusByte = 0;
            /* Release and continue waiting */
            func_800075E0(*(void **)((u8 *)statusByte + 12), 0, 1);
            continue;
        }

        break;  /* Exit when func_8008ABE4 returns 0 */
    }
}

/*
 * func_8008E280 (280 bytes)
 * Entity slot allocation with initialization
 * Finds free slot in table, initializes entry
 * t0 = max count, t1 = count ptr, t6/t7 = params
 */
void func_8008E280(s16 typeId, void *data, s32 param, void *callback) {
    s32 maxCount;     /* t0 preload */
    s32 *countPtr;    /* t1 preload */
    s16 srcId;        /* t5 preload */
    void *srcData;    /* t3 preload */
    void *cb;         /* t2 preload */
    s32 i;
    u32 *table;
    u32 *entry;

    maxCount = 0;   /* Preload */
    countPtr = NULL;
    srcId = 0;
    srcData = NULL;
    cb = NULL;

    if (maxCount <= 0) {
        goto found;
    }

    /* Search for free slot (entry[5] == 0xFFFF) */
    table = (u32 *)0x8013E700;
    for (i = 0; i < maxCount; i++) {
        if ((*(u16 *)((u8 *)table + 20) & 0xFFFF) == 0xFFFF) {
            goto found;
        }
        table = (u32 *)((u8 *)table + 68);
    }

found:
    /* Calculate entry address */
    entry = (u32 *)(0x8013E700 + (i * 17) * 4);  /* i * 68 */

    if (i == maxCount) {
        *countPtr = maxCount + 1;
        maxCount = maxCount + 1;
    }

    /* Update max if needed */
    if (*(s32 *)0x801569A8 < maxCount) {
        *(s32 *)0x801569A8 = maxCount;
    }

    /* Initialize entry */
    entry[0] = (u32)callback;
    entry[1] = 0;
    entry[2] = (u32)srcData;
    *(s16 *)((u8 *)entry + 20) = typeId;
    *(s16 *)((u8 *)entry + 22) = -1;
    *(s16 *)((u8 *)entry + 24) = -1;
    *(s16 *)((u8 *)entry + 26) = -1;

    /* Clear remaining fields */
    entry[7] = 0;   /* offset 28 */
    entry[8] = 0;   /* offset 32 */
    entry[9] = 0;   /* offset 36 */
    entry[10] = 0;  /* offset 40 */
    entry[11] = 0;  /* offset 44 */
    entry[12] = 0;  /* offset 48 */
    entry[13] = 0;  /* offset 52 */
    entry[14] = 0;  /* offset 56 */
    entry[15] = 0;  /* offset 60 */

    /* Call with converted params */
    func_8008E19C((s16)i, srcId);
}

/*
 * func_80094EC8 (192 bytes)
 * Copy sound parameters to slot and update
 * Copies fields from a0 to slot table, then calls update functions
 */
void func_80094EC8(void *sndObj) {
    u16 slotIdx;
    u32 *slot;
    s16 field22, field20, field16, field14;
    u32 field4, field8;
    u16 field12;
    s16 field34, field32, field30, field28;
    u16 field18;
    s8 field26, field24, field25;
    u32 field0;

    slotIdx = *(u16 *)((u8 *)sndObj + 52);
    slot = (u32 *)(0x80140BF0 + (slotIdx << 5));

    /* Copy main fields */
    field22 = *(s16 *)((u8 *)sndObj + 22);
    field20 = *(s16 *)((u8 *)sndObj + 20);
    field16 = *(s16 *)((u8 *)sndObj + 16);
    field14 = *(s16 *)((u8 *)sndObj + 14);
    field4 = *(u32 *)((u8 *)sndObj + 4);
    field8 = *(u32 *)((u8 *)sndObj + 8);
    field12 = *(u16 *)((u8 *)sndObj + 12);

    *(s16 *)((u8 *)slot + 18) = field22;
    *(s16 *)((u8 *)slot + 16) = field20;
    *(s16 *)((u8 *)slot + 12) = field16;
    *(s16 *)((u8 *)slot + 10) = field14;
    slot[0] = field4;
    slot[1] = field8;
    *(u16 *)((u8 *)slot + 8) = field12;

    /* Load additional params */
    field34 = *(s16 *)((u8 *)sndObj + 34);
    field32 = *(s16 *)((u8 *)sndObj + 32);
    field30 = *(s16 *)((u8 *)sndObj + 30);
    field28 = *(s16 *)((u8 *)sndObj + 28);

    func_80094E00(slotIdx, field28, field30, field32, field34);

    field26 = *(s8 *)((u8 *)sndObj + 26);
    field24 = *(u8 *)((u8 *)sndObj + 24);
    field18 = *(u16 *)((u8 *)sndObj + 18);
    func_80094DB0(slotIdx, field26, field24, field18);

    field25 = *(u8 *)((u8 *)sndObj + 25);
    func_80094D68(slotIdx, field25);

    field0 = *(u32 *)sndObj;
    func_80094D20(slotIdx, (field0 + 1) < 1);
}

/*
/*
/*
 * func_80090EA4 (160 bytes)
 * Rotate vector around axis by angle
 * Uses sinf/cosf for rotation matrix
 */
void func_80090EA4(f32 angle, f32 *vec) {
    f32 s, c;
    f32 v0, v1, v2;
    f32 t0, t1;
    s32 i;

    /* Absolute value check */
    if ((angle > 0.0f ? angle : -angle) <= 0.0f) {
        return;
    }

    /* Check against threshold */
    if ((angle > 0.0f ? angle : -angle) <= *(f32 *)0x801239D4) {
        return;
    }

    /* Compute sin/cos */
    s = sinf(angle);
    c = cosf(angle);

    /* Apply rotation to 3 vector pairs */
    for (i = 0; i < 3; i++) {
        v0 = vec[0];
        v2 = vec[6];  /* offset +24 */

        t0 = v0 * c;
        t1 = v2 * s;
        t0 = t0 - t1;
        t1 = v0 * s;
        t1 = t1 + v2 * c;

        vec[0] = t0;
        vec[5] = t1;  /* offset +20 */
        vec++;
    }
}

/*
 * func_80090F4C (160 bytes)
 * Rotate vector around different axis by angle
 * Similar to func_80090EA4 but different offsets
 */
void func_80090F4C(f32 angle, f32 *vec) {
    f32 s, c;
    f32 v0, v1, v2;
    f32 t0, t1;
    s32 i;

    /* Absolute value check */
    if ((angle > 0.0f ? angle : -angle) <= 0.0f) {
        return;
    }

    /* Check against threshold */
    if ((angle > 0.0f ? angle : -angle) <= *(f32 *)0x801239DC) {
        return;
    }

    /* Compute sin/cos */
    s = sinf(angle);
    c = cosf(angle);

    /* Apply rotation to 3 vector pairs */
    for (i = 0; i < 3; i++) {
        v0 = vec[3];   /* offset +12 */
        v2 = vec[6];   /* offset +24 */

        t0 = v0 * c;
        t1 = v2 * s;
        t1 = t0 + t1;
        t0 = v2 * c;
        t0 = t0 - v0 * s;

        vec[2] = t1;   /* offset +8 */
        vec[5] = t0;   /* offset +20 */
        vec++;
    }
}

/*
 * func_800A12D4 (104 bytes)
 * Conditional sync and ROM load
 * t6 preloaded with flag check
 */
void func_800A12D4(void) {
    s32 flag;  /* t6 preload */

    flag = 0;  /* Preload */

    if (flag == 0) {
        return;
    }

    func_80007270((void *)0x80152770, 0, 1);  /* sync_acquire */
    func_80095FD8((void *)0x8039A400, 0);
    func_800075E0((void *)0x80152770, 0, 0);  /* sync_release */

    *(u8 *)0x8012ED00 = 0;
}

/*
 * func_800A5160 (120 bytes)
 * Initialize player entity
 * t6 preloaded with condition check
 */
void func_800A5160(void) {
    s32 condition;  /* t6 preload */
    void *entity;

    condition = 0;  /* Preload */

    if (condition != 0) {
        return;
    }

    *(u8 *)0x80128EE4 = 1;
    *(s16 *)0x8014FEC8 = 0;
    *(u8 *)0x80140A10 = *(u8 *)0x8003E860;

    entity = func_80097798(*(u8 *)0x8003E860, 0, 0, 0, 0);
    *(void **)0x80140AF0 = entity;

    func_8009638C((s32)entity);
    func_800A4E58();
    func_800A510C();
}

/*
 * func_800A5A74 (200 bytes)
 * Viewport float conversion and setup
 * v0, v1 = width/height from caller
 */
void func_800A5A74(void) {
    s32 width;   /* v0 preload */
    s32 height;  /* v1 preload */
    s32 halfW, halfH;
    f32 fHalfW, fHalfH;
    u16 *viewport;

    width = 0;   /* Preload */
    height = 0;  /* Preload */

    /* Convert to float for storage */
    halfW = width >> 1;
    halfH = height >> 1;

    fHalfW = (f32)halfW;
    fHalfH = (f32)halfH;

    func_800A5908(0, (void *)0x8012EA30, (void *)0x80159870, *(void **)0x80154188);

    viewport = (u16 *)0x80159870;
    viewport[0] = 0;
    viewport[1] = 0;
    *(void **)0x8018A510 = (void *)0x8012EA30;
    *(void **)0x8018A514 = (void *)viewport;

    viewport[3] = *(u16 *)0x8003AFC6;  /* Screen width */
    viewport[2] = *(u16 *)0x8003AFC2;  /* Screen height */
}

/*
 * func_800A5B3C (36 bytes)
 * Clear three global pointers
 */
void func_800A5B3C(void) {
    *(u32 *)0x80156990 = 0;
    *(u32 *)0x80156CE0 = 0;
    *(u32 *)0x801613AC = 0;
}

/*
 * func_800A6938 (104 bytes)
 * Setup viewport dimensions from globals
 * s0, s1 preloaded with width/height
 */
void func_800A6938(void) {
    s32 width;   /* s0 preload */
    s32 height;  /* s1 preload */
    s32 halfW, halfH;
    u16 *viewportStruct;
    void *ptr;

    width = 0;   /* Preload */
    height = 0;  /* Preload */

    halfW = (width >> 1) - 1;
    halfH = (height >> 1) - 1;

    viewportStruct = (u16 *)0x80159B58;  /* From a2 base */
    viewportStruct[0] = 0;
    viewportStruct[1] = 0;
    viewportStruct[2] = (u16)halfH;
    viewportStruct[3] = (u16)halfW;

    ptr = (void *)0x8012EA60;
    *(void **)0x8018A510 = ptr;
    *(void **)0x8018A514 = viewportStruct;
}

/*
 * func_800B0458 (112 bytes)
 * Process player array for update flags
 */
void func_800B0458(void) {
    s32 i;
    u8 *playerBase;
    u8 flags;

    for (i = 0; i < 4; i++) {
        playerBase = (u8 *)(0x80152818 + i * 0x144);

        flags = *(u8 *)((u8 *)playerBase + 5);
        if (flags == 0) {
            continue;
        }

        func_800AF9F0(playerBase, i);
    }
}

/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
 * func_800C3594 (120 bytes)
 * Initialize entity slot with index
 * t9 preloaded with condition, t1 = index
 */
void func_800C3594(s32 idx) {
    s32 condition;  /* t9 preload */
    void *entry;
    u32 flags;

    condition = 0;  /* Preload */

    *(s16 *)((u8 *)0x80000000 + 2) = 1;  /* v0 preload ptr */

    if (condition != 0) {
        func_800C1B60(11);
    }

    /* Calculate entry: (idx * 31) * 4 = idx * 124 */
    entry = (void *)(0x801569B8 + idx * 124);

    func_80002790(entry, 0, 124);

    /* Set bit 0x15000000 in first word */
    flags = *(u32 *)entry;
    *(u32 *)entry = flags | 0x15000000;
}

/*
 * func_800C40F8 (136 bytes)
 * Check state and call update
 */
void func_800C40F8(void) {
    void *ptr;
    s32 state;

    ptr = *(void **)0x801497A0;
    if (ptr == NULL) {
        return;
    }

    state = *(s32 *)((u8 *)ptr + 16);
    if (state != 3) {
        return;
    }

    func_800C3AD0(ptr);
}

/*
 * func_800C4180 (128 bytes)
 * Similar state check with different update
 */
void func_800C4180(void) {
    void *ptr;
    s32 state;

    ptr = *(void **)0x801497A0;
    if (ptr == NULL) {
        return;
    }

    state = *(s32 *)((u8 *)ptr + 16);
    if (state != 3) {
        return;
    }

    func_800C36A0(ptr);
}

/*
 * func_800C5500 (228 bytes)
 * Process entity with type dispatch
 */
void func_800C5500(void *entity) {
    s32 type;

    if (entity == NULL) {
        return;
    }

    type = *(s8 *)((u8 *)entity + 2);

    switch (type) {
        case 1:
            func_800C4CF8(entity);
            break;
        case 2:
            func_800C4F68(entity);
            break;
        default:
            break;
    }
}

/*
 * func_800C69C0 (224 bytes)
 * Entity iterator with callback
 */
void func_800C69C0(void (*callback)(void *)) {
    void *list;
    void *entity;
    void *next;

    list = *(void **)0x80144C50;
    entity = *(void **)((u8 *)list + 8);

    while (entity != list) {
        next = *(void **)((u8 *)entity + 8);
        callback(entity);
        entity = next;
    }
}

/*
 * func_800C6AA0 (1564 bytes) - stub
 * Large entity processing function
 */
void func_800C6AA0(void *entity) {
    /* Large function - needs full implementation */
}

/*
 * func_800D2488 (64 bytes)
 * Store float and call update with params
 * f4, f6, f12 preloaded with floats
 */
void func_800D2488(s32 idx, f32 f12_val) {
    f32 f8;
    void *ptr;

    /* f4, f6 operations stored to indexed location */
    f8 = *(f32 *)0x80000000;  /* Preload v0/at calculation */
    *(f32 *)(0x80142770 + idx * 4) = f8;

    ptr = (void *)(0x80143AC8 + idx * 8);
    *(f32 *)0x80000000 = f12_val;  /* v1 preload */

    func_800D2128(ptr, 102);
}

/*
 * func_800D3430 (92 bytes)
 * Check table entry and optionally call update
 * a6+a7 passed via stack as output pointers
 */
s32 func_800D3430(s32 idx, s32 val, s32 *outA, s32 *outB, s32 extra) {
    u32 *table;
    u8 flag;

    if (extra == 0) {
        return 1;
    }

    table = (u32 *)*(u32 *)0x801407FC;
    flag = *(u8 *)((u8 *)table + idx * 16);

    if (flag == 0) {
        *outA = idx;
        *outB = val;
        return 1;
    }

    func_800D2FA8(idx, val, outA, outB, extra, 0);
    return 1;
}

/*
 * func_800D5834 (88 bytes)
 * Initialize entity slot with zero floats
 * t6, t7 preloaded with index calculation
 */
void func_800D5834(s32 idx) {
    void *entity;
    s32 combined;  /* t6 + t7 preload */

    combined = 0;  /* Preload */

    entity = (void *)(0x8015A250 + combined * 8);

    *(s16 *)((u8 *)entity + 0x71C) = 0;

    func_800D5524(entity);

    /* Clear 4 floats at high offsets */
    *(f32 *)((u8 *)entity + 0x7FC) = 0.0f;
    *(f32 *)((u8 *)entity + 0x800) = 0.0f;
    *(f32 *)((u8 *)entity + 0x804) = 0.0f;
    *(f32 *)((u8 *)entity + 0x7F8) = 0.0f;
}

/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
/*
 * func_800E1540 (96 bytes)
 * Transform entity position with scale
 */
void func_800E1540(void *entity) {
    f32 scale;
    f32 pos0, pos1, pos2;
    f32 out0, out1, out2;

    scale = *(f32 *)0x80142764;

    pos0 = *(f32 *)((u8 *)entity + 0x5BC);
    pos1 = *(f32 *)((u8 *)entity + 16);
    pos2 = *(f32 *)((u8 *)entity + 20);

    out0 = pos0 + scale;
    out1 = out0 + pos1;
    out2 = out1 + pos2;

    *(f32 *)((u8 *)entity + 40) = out0;
    *(f32 *)((u8 *)entity + 44) = out1;
    *(f32 *)((u8 *)entity + 48) = out2;

    func_800E1500((u8 *)entity + 28, (u8 *)entity + 52);
}

/*
 * func_800E7040 (244 bytes)
 * Initialize player state and sync
 * t6 preloaded with condition
 */
void func_800E7040(void) {
    s32 condition;  /* t6 preload */
    s32 i;

    condition = 0;  /* Preload */

    if (condition == 0) {
        func_800C9158(-1, 1);
        func_800C84C0(-1, 1);
    }

    *(u8 *)0x80111954 = 1;

    /* Initialize 8 bytes with 0x46 */
    for (i = 0; i < 8; i++) {
        *(u8 *)(0x80159AF8 + i) = 0x46;
    }

    if (*(s8 *)0x80111968 == 0) {
        *(s8 *)0x80111968 = 1;
        func_80006A00((void *)0x801597A8, (void *)0x80152730, 1);
        func_800075E0((void *)0x801597A8, 0, 0);
    }

    /* Wait for byte to equal 128 */
    while (*(u8 *)0x80111950 != 128) {
        /* Spin */
    }

    func_800C95DC();
}

/*
 * func_800E7710 (248 bytes)
 * Entity array processing
 */
void func_800E7710(void) {
    s32 i;
    void *entity;

    for (i = 0; i < 4; i++) {
        entity = *(void **)(0x80159870 + i * 4);
        if (entity != NULL) {
            func_800E73E4(entity);
        }
    }
}

/*
 * func_800E7808 (268 bytes)
 * Entity state machine dispatch
 */
void func_800E7808(void *entity) {
    s32 state;

    if (entity == NULL) {
        return;
    }

    state = *(s32 *)((u8 *)entity + 16);

    switch (state) {
        case 0:
            func_800E7710();
            break;
        case 1:
            func_800E7914(entity);
            break;
        case 2:
            func_800E7980(entity);
            break;
        default:
            break;
    }
}

/*
 * func_800E79F8 (160 bytes)
 * Entity reference count management
 */
void func_800E79F8(void *entity) {
    s32 refCount;

    if (entity == NULL) {
        return;
    }

    refCount = *(s32 *)((u8 *)entity + 24);

    if (refCount > 0) {
        *(s32 *)((u8 *)entity + 24) = refCount - 1;
    }

    if (refCount == 1) {
        func_800E7A98(entity);
    }
}

/*
 * func_800E7C2C (216 bytes)
 * Entity cleanup with pointer clear
 */
void func_800E7C2C(void *entity) {
    void **ptrArray;
    s32 i;

    if (entity == NULL) {
        return;
    }

    ptrArray = (void **)((u8 *)entity + 32);

    for (i = 0; i < 4; i++) {
        if (ptrArray[i] != NULL) {
            func_800B358C(ptrArray[i]);
            ptrArray[i] = NULL;
        }
    }
}

/*
 * func_800E847C (212 bytes)
 * Process entity list with state check
 */
void func_800E847C(void) {
    void *list;
    void *entity;
    void *next;

    list = (void *)0x80144C50;
    entity = *(void **)((u8 *)list + 8);

    while (entity != list) {
        next = *(void **)((u8 *)entity + 8);

        if (*(s32 *)((u8 *)entity + 16) == 3) {
            func_800E7808(entity);
        }

        entity = next;
    }
}

/*
 * func_800EB90C (184 bytes)
 * Sound effect trigger
 */
void func_800EB90C(s32 soundId) {
    if (*(s32 *)0x8015A110 != 4) {
        return;
    }

    func_800EA620(soundId);
}

/*
/*
 * func_800F5F00 (144 bytes)
 * Jump table dispatch for menu states
 * t6 preloaded with state index
 */
void func_800F5F00(void) {
    s32 state;  /* t6 preload */

    state = 0;  /* Preload */

    if (state >= 7) {
        return;
    }

    /* Jump table dispatch - each case calls different menu function */
    switch (state) {
        case 0:
            func_800F56E0();
            break;
        case 1:
            func_800F54C0();
            break;
        case 2:
            func_800F4FEC();
            break;
        case 3:
            func_800F4D94();
            break;
        case 4:
            func_800F4B8C();
            break;
        case 5:
            func_800F497C();
            break;
        default:
            break;
    }
}

/*
/*
/*
/*
/*
/*
 * func_800FBF88 (52 bytes)
 * High score mode update
 */
void func_800FBF88(void) {
    func_800FBE5C();
}

/*
 * func_800FBF90 (84 bytes)
 * Reset game state to attract mode
 */
void func_800FBF90(void) {
    *(s32 *)0x801146EC = 0;  /* Set to ATTRACT */
    *(u32 *)0x80142AFC = 0;  /* Clear frame counter */
    func_800FBFE4();
}

/*
 * func_800FBFE4 (264 bytes)
 * Game cleanup and reset
 */
void func_800FBFE4(void) {
    s32 i;

    /* Clear player states */
    for (i = 0; i < 4; i++) {
        *(u32 *)(0x80152818 + i * 0x144) = 0;
    }

    func_800FC0EC();
}

/*
 * func_800FC0EC (872 bytes) - stub
 * Full game reset function
 */
void func_800FC0EC(void) {
    /* Large function - needs full implementation */
}

/*
 * func_800FEA00 (556 bytes) - stub
 * Save game state
 */
void func_800FEA00(void) {
    /* Needs full implementation */
}

/*
 * func_800878EC (284 bytes)
 * RDP render mode setup with flag checking
 * v0=current state, v1=state ptr, a0=flags to set
 * Generates G_SETOTHERMODE_L and G_RDPPIPESYNC commands
 */
void func_800878EC(s32 flags) {
    u32 currentState;  /* v0 preload */
    u32 *statePtr;     /* v1 preload */
    u32 *dlPtr;
    s32 *modePtr;

    currentState = 0;  /* Preload */
    statePtr = NULL;   /* Preload */

    /* If flags already set, return */
    if (flags == (currentState & flags)) {
        return;
    }

    /* Update state with new flags */
    *statePtr = currentState | flags;

    /* Check 0x4000 flag - texture mode */
    if (flags & 0x4000) {
        dlPtr = *(u32 **)0x80149438;
        *(u32 **)0x80149438 = dlPtr + 2;

        /* G_SETOTHERMODE_L with FCFFFFFF / FFFDF6FB */
        dlPtr[0] = 0xFCFFFFFF;
        dlPtr[1] = 0xFFFDF6FB;

        modePtr = (s32 *)0x8015A248;
        if (*modePtr > 0 && *modePtr < 4) {
            *modePtr = -1;
        }

        /* Additional RDP command: G_RDPPIPESYNC variant */
        dlPtr = *(u32 **)0x80149438;
        *(u32 **)0x80149438 = dlPtr + 2;
        dlPtr[0] = 0xE3000A01;
        dlPtr[1] = 0;
        *modePtr = -1;
    }

    /* Check 0x0001 flag - Z-buffer mode */
    if (flags & 0x0001) {
        dlPtr = *(u32 **)0x80149438;
        *(u32 **)0x80149438 = dlPtr + 2;
        dlPtr[0] = 0xE2001E01;
        dlPtr[1] = 1;
    }

    /* Check 0x0010 flag - blend mode */
    if (flags & 0x0010) {
        dlPtr = *(u32 **)0x80149438;
        *(u32 **)0x80149438 = dlPtr + 2;
        dlPtr[0] = 0xE2001D00;
        dlPtr[1] = 4;
        func_80086A50(*(void **)0x8015A248);
    }

    /* Check 0x0020 flag - additional processing */
    if (flags & 0x0020) {
        func_80086A50(*(void **)0x8015A248);
    }
}

/*
 * func_80086A50 (varies)
 * Texture/display list processing
 */
void func_80086A50(void *texData) {
    /* Texture loading and processing */
}

/*
 * func_80088BD4 (288 bytes)
 * Matrix load to RSP
 */
void func_80088BD4(void *matrix) {
    u32 *dlPtr;

    dlPtr = *(u32 **)0x80149438;
    *(u32 **)0x80149438 = dlPtr + 2;

    /* G_MTX command */
    dlPtr[0] = 0xDA380003;
    dlPtr[1] = (u32)matrix;
}

/*
 * func_80088C20 (156 bytes)
 * Push matrix to stack
 */
void func_80088C20(void *matrix) {
    u32 *dlPtr;

    dlPtr = *(u32 **)0x80149438;
    *(u32 **)0x80149438 = dlPtr + 2;

    /* G_MTX with push flag */
    dlPtr[0] = 0xDA380001;
    dlPtr[1] = (u32)matrix;
}

/*
 * func_80088C7C (124 bytes)
 * Pop matrix from stack
 */
void func_80088C7C(void) {
    u32 *dlPtr;

    dlPtr = *(u32 **)0x80149438;
    *(u32 **)0x80149438 = dlPtr + 2;

    /* G_POPMTX command */
    dlPtr[0] = 0xD8380040;
    dlPtr[1] = 0x00000040;
}

/*
/*
 * func_8008B2B4 (56 bytes)
 * Random number generator (LCG)
 */
s32 func_8008B2B4(void) {
    u32 *seed;
    u32 val;

    seed = (u32 *)0x8011735C;
    val = *seed;

    /* LCG: val = val * 0x41C64E6D + 12345 */
    val = val * 0x41C64E6D + 12345;
    *seed = val;

    return (val >> 16) & 0x7FFF;
}

/*
 * func_8008B2E4 (72 bytes)
 * Random float in range [0, 1)
 */
f32 func_8008B2E4(void) {
    u32 *seed;
    u32 val;
    s32 randInt;
    f32 result;

    seed = (u32 *)0x8011735C;
    val = *seed;

    val = val * 0x41C64E6D + 12345;
    *seed = val;
    randInt = (val >> 16) & 0x7FFF;

    /* Convert to float and scale to [0, 1) */
    result = (f32)randInt / 32768.0f;
    return result;
}

/*
 * func_8008B32C (52 bytes)
 * Dot product of two 3-element vectors
 */
f32 func_8008B32C(f32 *a, f32 *b) {
    f32 result;
    s32 i;

    result = 0.0f;
    for (i = 0; i < 3; i++) {
        result += a[i] * b[i];
    }
    return result;
}

/*
 * func_8008B360 (84 bytes)
 * Cross product of two 3-element vectors
 */
void func_8008B360(f32 *a, f32 *b, f32 *out) {
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

/*
 * func_8008B3B4 (128 bytes)
 * Normalize 3-element vector
 */
void func_8008B3B4(f32 *vec) {
    f32 len;
    f32 invLen;

    len = sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);

    if (len > 0.0001f) {
        invLen = 1.0f / len;
        vec[0] *= invLen;
        vec[1] *= invLen;
        vec[2] *= invLen;
    }
}

/*
 * func_8008B434 (64 bytes)
 * Vector length squared
 */
f32 func_8008B434(f32 *vec) {
    return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
}

/*
 * func_8008AE8C (372 bytes) - stub
 * Texture parameter setup
 */
void func_8008AE8C(s16 texId, s32 mode, s32 flags) {
    /* Complex texture setup - needs full implementation */
}

/*
 * func_8008B0D8 (404 bytes) - stub
 * Texture load with parameters
 */
void func_8008B0D8(s16 texId, s32 param, s32 flags) {
    /* Complex texture loading - needs full implementation */
}

/*
 * func_8008B4C4 (380 bytes) - stub
 * Matrix multiply
 */
void func_8008B4C4(f32 *a, f32 *b, f32 *out) {
    /* 4x4 matrix multiply - needs full implementation */
}

/*
 * func_8008B69C (712 bytes) - stub
 * Transform vertices
 */
void func_8008B69C(void *verts, s32 count, void *mtx) {
    /* Vertex transformation - needs full implementation */
}

/*
/*
 * func_800A133C (128 bytes)
 * Initialize ROM segment addresses for decompression
 * Sets up source/dest addresses for DMA/inflate
 */
void func_800A133C(void) {
    void *src;
    void *dest;
    u32 size;

    /* Set up segment addresses for ROM data */
    src = (void *)0x80123850;   /* Source in ROM */
    dest = (void *)0x8038A400;  /* Destination in RAM */
    size = 0x00BEA100;          /* Compressed size marker */

    /* Call the actual ROM load function */
    func_800A11E4(dest, src, 0, 0, 0, size);
}

/*
 * func_8008AA40 (356 bytes)
 * Audio/sync initialization with queue setup
 */
void func_8008AA40(void *param) {
    void *syncObj1;
    void *syncObj2;
    u32 temp;

    syncObj1 = (void *)0x80152788;  /* D_80152788 */
    syncObj2 = (void *)0x801527A8;  /* D_801527A8 */

    /* Initialize sync objects */
    sync_init(syncObj1, syncObj2, 8);

    /* Process audio queue entries */
    /* Complex loop - stub for now */
}

/*
 * func_8008B964 (288 bytes)
 * Entity position/rotation calculation with table lookup
 */
void func_8008B964(void *entity, s16 index, s16 flags) {
    s16 entityId;
    void *tableEntry;
    void *entityData;
    f32 scale;
    s32 flag;

    entityId = *(s16 *)((u8 *)entity + 8);

    /* Calculate table offset: id * 0x109 * 8 */
    tableEntry = (void *)(0x8015A250 + (entityId * 0x109 * 8));

    /* Load scale factor */
    scale = *(f32 *)((u8 *)tableEntry + 0x3F0);

    /* Check entity flags */
    flag = *(u8 *)((u8 *)tableEntry + 0x641);

    if (flags < 13 && flag == 0) {
        /* Early return for specific flag combination */
        return;
    }

    /* Additional entity processing... */
}

/*
 * func_8008BA84 (264 bytes)
 * Similar to func_8008B964 but with different flag checks
 */
void func_8008BA84(void *entity, s16 index, s16 flags) {
    s16 entityId;
    void *tableEntry;
    f32 scale;
    u32 entityFlags;

    entityId = *(s16 *)((u8 *)entity + 8);

    /* Calculate table offset */
    tableEntry = (void *)(0x8015A250 + (entityId * 0x109 * 8));

    /* Load scale */
    scale = *(f32 *)((u8 *)tableEntry + 0x3F0);

    /* Entity data at different offset */
    /* flags < 21 check */
    if (flags < 21) {
        entityFlags = *(u32 *)((u8 *)tableEntry + 0xE8);
        if ((entityFlags & 0x4000) != 0) {
            return;
        }
    }

    /* Process entity... */
}

/*
 * func_8008BB8C (288 bytes)
 * Entity animation/state processing
 */
void func_8008BB8C(void *entity, s16 param) {
    /* Complex entity state processing - stub */
}

/*
 * func_8008BC94 (264 bytes)
 * Entity position update with bounds checking
 */
void func_8008BC94(void *entity, s16 param) {
    /* Position bounds processing - stub */
}

/*
 * func_8008BD9C (720 bytes)
 * Large entity update function
 */
void func_8008BD9C(void *entity, void *data) {
    /* Complex entity update - stub */
}

/*
 * func_8008C76C (252 bytes)
 * Floating point comparison with special cases
 */
f32 func_8008C76C(f32 a, f32 b) {
    f32 result;

    /* Compare a and b, handle special cases */
    if (a == 0.0f) {
        if (b >= 0.0f) {
            return *(f32 *)0x801238F4;  /* Constant */
        } else {
            return *(f32 *)0x801238F8;  /* Different constant */
        }
    }

    /* Check for zero crossing */
    if (b == 0.0f) {
        if (a >= 0.0f) {
            return 0.0f;
        }
    }

    /* General case - compute ratio or angle */
    result = a / b;
    return result;
}

/*
 * func_8008C884 (736 bytes)
 * Large entity rendering setup
 */
void func_8008C884(void *entity, s32 mode) {
    s16 entityId;
    s16 entityType;
    void *tableEntry;
    void *renderData;
    s32 flags;

    entityId = *(s16 *)((u8 *)entity + 8);
    entityType = *(s16 *)((u8 *)entity + 4);

    /* Check special rendering flag */
    flags = *(s8 *)(0x801613A8);  /* D_801613A8 */
    if (flags != 0) {
        /* Check frame timing flag */
        s32 frameFlag = *(s32 *)0x801174B4;
        if (frameFlag < 0) {
            return;
        }
    }

    /* Entity table lookup and render setup... */
}

/*
 * func_8008D120 (1524 bytes)
 * Major entity processing function
 */
void func_8008D120(void *entity, s32 param1, s32 param2) {
    /* Large entity processing - stub */
}

/*
 * func_8008D764 (372 bytes)
 * Euler angle to rotation matrix conversion
 */
void func_8008D764(f32 *matrix, f32 *angles) {
    f32 sinX, cosX;
    f32 sinY, cosY;
    f32 sinZ, cosZ;

    /* Get sin/cos for each axis */
    sinX = sinf(angles[0]);
    cosX = cosf(angles[0]);
    sinY = sinf(angles[1]);
    cosY = cosf(angles[1]);
    sinZ = sinf(angles[2]);
    cosZ = cosf(angles[2]);

    /* Build rotation matrix */
    matrix[0] = cosY * cosZ;
    matrix[1] = cosY * sinZ;
    matrix[2] = -sinY;

    matrix[3] = sinX * sinY * cosZ - cosX * sinZ;
    matrix[4] = sinX * sinY * sinZ + cosX * cosZ;
    matrix[5] = sinX * cosY;

    matrix[6] = cosX * sinY * cosZ + sinX * sinZ;
    matrix[7] = cosX * sinY * sinZ - sinX * cosZ;
    matrix[8] = cosX * cosY;
}

/*
/*
 * func_8008D93C (796 bytes)
 * Entity animation state update
 */
void func_8008D93C(void *entity, s16 animState) {
    s16 entityId;
    s16 entityType;
    void *animData;

    if (animState == 0) {
        /* Clear animation state */
        entityType = *(s16 *)((u8 *)entity + 6);
        func_8008AE8C(entityType, 1, 15);
        *(s32 *)((u8 *)entity + 0x14) = 0;
        *(s16 *)((u8 *)entity + 6) = -1;
        return;
    }

    entityId = *(s16 *)((u8 *)entity + 8);
    entityType = *(s16 *)((u8 *)entity + 4);

    /* Process animation state... */
}

/*
 * func_8008E440 (1472 bytes)
 * Major render state processing
 */
void func_8008E440(void *a0, s32 mode, s32 flags, s32 index) {
    void *tableEntry;
    u32 renderFlags;
    s16 scaledIndex;

    scaledIndex = (s16)(index >> 2);
    tableEntry = (void *)(0x801492D8 + (flags * 4));

    /* Process render mode */
    switch (mode) {
        case 0:
            renderFlags = *(u32 *)tableEntry;
            if ((renderFlags & 0x100) != 0) {
                return;
            }
            *(u32 *)tableEntry = renderFlags | 0x100;
            break;
        case 1:
        case 2:
        case 3:
            /* Other render modes... */
            break;
        default:
            break;
    }
}

/*
 * func_8008EA10 (1656 bytes)
 * Entity spawn/initialization
 */
void func_8008EA10(void *params, s32 type) {
    /* Complex entity spawn - stub */
}

/*
 * func_80090310 (1036 bytes)
 * Process entity with callback registration
 */
void func_80090310(void *entity) {
    void *result;

    result = func_80090284();
    if (result == NULL) {
        return;
    }

    /* Store callback pointer */
    *(void **)((u8 *)result + 0x14) = (void *)0x80090FEC;

    /* Copy initial position from stack */
    /* Additional setup... */
}

/*
 * func_800908A0 (716 bytes)
 * Entity movement/physics update
 */
void func_800908A0(void *entity, s16 flags) {
    /* Physics update - stub */
}

/*
 * func_80090B70 (820 bytes)
 * Entity collision detection
 */
void func_80090B70(void *entity) {
    /* Collision processing - stub */
}

/*
 * func_80090FEC (2184 bytes)
 * Entity update callback
 */
void func_80090FEC(void *entity, s16 param) {
    s32 globalFlag;

    globalFlag = *(s32 *)0x801170FC;
    if (globalFlag != 0) {
        return;
    }

    /* Major entity update processing... */
}

/*
 * func_80091874 (616 bytes)
 * Entity animation update with texture loading
 */
void func_80091874(void *entity, s16 animFrame) {
    s16 entityId;
    s16 entityType;
    void *tableEntry;
    void *animData;
    s32 flags;

    if (animFrame == 0) {
        /* Clear animation */
        entityType = *(s16 *)((u8 *)entity + 6);
        func_8008AE8C(entityType, 1, 15);
        *(s32 *)((u8 *)entity + 0x14) = 0;
        *(s16 *)((u8 *)entity + 6) = -1;
        return;
    }

    entityId = *(s16 *)((u8 *)entity + 8);
    flags = *(s32 *)((u8 *)entity + 0x0C);

    /* Check animation flag */
    if ((flags & 0x01) != 0) {
        /* Animation flag set - update frame */
    }

    /* Complex animation processing... */
}

/*
 * func_80091E5C (560 bytes)
 * Entity scale/transform with sync
 */
void func_80091E5C(f32 scale) {
    /* Acquire sync */
    sync_acquire((void *)0x80142728, 0, 1);

    if (scale >= 1.0f) {
        /* Clamp scale */
        scale = 1.0f;
    } else if (scale < 0.0f) {
        scale = 0.0f;
    }

    /* Apply scale transform... */
}

/*
 * func_8009229C (600 bytes)
 * Entity state initialization
 */
void func_8009229C(void *entity, void *params) {
    u32 flags;
    void *flagPtr;

    flagPtr = (void *)0x80146104;

    /* Load and apply flags */
    flags = *(u32 *)flagPtr;
    flags = (flags & *(u32 *)((u8 *)params)) | (*(u32 *)((u8 *)params + 4));
    *(u32 *)((u8 *)entity + 0x0C) = flags;

    /* Initialize velocities to zero */
    *(f32 *)((u8 *)entity + 0x24) = -1.0f;
    *(f32 *)((u8 *)entity + 0x28) = -1.0f;
    *(f32 *)((u8 *)entity + 0x2C) = -1.0f;
    *(s32 *)((u8 *)entity + 0x40) = 0;
    *(f32 *)((u8 *)entity + 0x20) = 1.0f;
    *(f32 *)((u8 *)entity + 0x30) = 1.0f;

    /* Call additional init */
    func_8009211C(entity, entity);
}

/*
 * func_800924F4 (1636 bytes)
 * Entity spawn with full initialization
 */
void func_800924F4(void *entity, s16 spawnType) {
    s16 entityId;
    s16 entityType;
    void *tableEntry;

    if (spawnType == 0) {
        /* Destroy entity */
        entityType = *(s16 *)((u8 *)entity + 6);
        func_8008AE8C(entityType, 0, 15);
        *(s32 *)((u8 *)entity + 0x14) = 0;
        *(s16 *)((u8 *)entity + 6) = -1;
        return;
    }

    entityId = *(s16 *)((u8 *)entity + 8);

    /* Calculate table entry */
    tableEntry = (void *)(0x8015A250 + ((entityId * 0x109) << 3));

    /* Complex spawn logic... */
}

/*
 * func_80092E2C (720 bytes)
 * String/name copy with formatting
 */
void func_80092E2C(void *dest, u8 *src, s8 x, s8 y, s8 z) {
    u8 *tempBuf;
    s32 i;

    /* Clamp coordinates */
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    /* Copy default string if src is NULL */
    if (src == NULL || src[0] == 0) {
        tempBuf = (u8 *)(0x80120E68);
        for (i = 0; i < 16; i++) {
            ((u8 *)dest)[i] = tempBuf[i];
        }
    } else {
        func_80092DCC(dest, src, 16);
    }
}

/*
 * func_800930A4 (2684 bytes)
 * Major entity processing loop
 */
void func_800930A4(void *entity, s32 mode) {
    /* Large processing function - stub */
}

/*
 * func_80093B20 (3440 bytes)
 * Entity AI/behavior update
 */
void func_80093B20(void *entity) {
    /* AI processing - stub */
}

/*
 * func_80094890 (460 bytes)
 * Audio parameter setup
 */
void func_80094890(void *params, s32 channel) {
    /* Audio setup - stub */
}

/*
 * func_80094A54 (472 bytes)
 * Sound effect trigger
 */
void func_80094A54(s32 soundId, s32 priority) {
    /* Sound trigger - stub */
}

/*
 * func_80094C30 (668 bytes)
 * Audio volume/pan control
 */
void func_80094C30(s32 channel, f32 volume, f32 pan) {
    /* Volume/pan control - stub */
}

/*
 * func_80094FF0 (156 bytes)
 * Audio channel reset
 */
void func_80094FF0(s32 channel) {
    /* Reset audio channel - stub */
}

/*
 * func_800951E0 (492 bytes)
 * Sound position update
 */
void func_800951E0(void *entity, f32 *pos) {
    /* Update 3D sound position - stub */
}

/*
 * func_800953CC (220 bytes)
 * Audio fade control
 */
void func_800953CC(s32 channel, f32 targetVol, f32 duration) {
    /* Fade control - stub */
}

/*
 * func_800954A8 (128 bytes)
 * Sound enable/disable
 */
void func_800954A8(s32 channel, s32 enable) {
    /* Enable/disable sound channel - stub */
}

/*
 * func_80095528 (484 bytes)
 * Music track control
 */
void func_80095528(s32 trackId, s32 cmd) {
    /* Music control - stub */
}

/*
 * func_8009570C (244 bytes)
 * Audio bus routing
 */
void func_8009570C(s32 srcBus, s32 destBus) {
    /* Bus routing - stub */
}

/*
 * func_80095800 (292 bytes)
 * Reverb/effect setup
 */
void func_80095800(s32 effectId, f32 param) {
    /* Effect setup - stub */
}

/*
 * func_80095924 (184 bytes)
 * Audio timing sync
 */
void func_80095924(void) {
    /* Audio sync - stub */
}

/*
 * func_80095A24 (236 bytes)
 * Sound priority management
 */
void func_80095A24(s32 channel, s32 priority) {
    /* Priority management - stub */
}

/*
 * func_80095B10 (236 bytes)
 * Audio stream control
 */
void func_80095B10(s32 streamId, s32 cmd) {
    /* Stream control - stub */
}

/*
 * func_80095BFC (268 bytes)
 * Audio buffer management
 */
void func_80095BFC(void *buffer, s32 size) {
    /* Buffer management - stub */
}

/*
 * func_80095D04 (468 bytes)
 * Audio state save/restore
 */
void func_80095D04(s32 cmd) {
    /* State save/restore - stub */
}

/*
 * func_8009614C (216 bytes)
 * Entity sound attachment with sync
 */
void func_8009614C(void *a0, void *a1, void *a2, void *entity) {
    s16 waitFlag;

    if (entity == NULL) {
        return;
    }

    /* Wait for audio sync */
    waitFlag = *(s16 *)0x8003EB70;
    while (waitFlag != 0) {
        waitFlag = *(s16 *)0x8003EB70;
    }

    /* Acquire sync and process */
    sync_acquire((void *)0x80152770, 0, 1);
    func_80095FD8(entity, 0);
    sync_release((void *)0x80152770, 0, 1);
}

/*
 * func_800963E8 (452 bytes)
 * Display list traversal
 */
void func_800963E8(u32 *dlPtr, void *param1, void *param2, void *param3) {
    u32 cmd;
    u32 cmdHi;

    cmd = *dlPtr;
    cmdHi = cmd & 0xFF000000;

    if (cmdHi == 0xDF000000) {
        /* End display list */
        return;
    }

    /* Process display list commands */
    while (cmdHi != 0xDF000000) {
        /* Check command types: DE00 (call), E100 (load), etc. */
        if ((cmd & 0xFF000000) == 0xDE000000) {
            /* Branch to sub-display list */
            dlPtr = (u32 *)dlPtr[1];
        } else {
            dlPtr += 2;
        }

        cmd = *dlPtr;
        cmdHi = cmd & 0xFF000000;
    }
}

/*
 * func_800965BC (244 bytes)
 * Entity render setup
 */
void func_800965BC(void *entity) {
    void *tableEntry;
    s32 entityIdx;

    /* Call render mode setup */
    func_80096288(entity, 1, 1);

    entityIdx = 0;  /* Preloaded from a3 */
    tableEntry = (void *)(0x80156D38 + (entityIdx * 5 * 4));

    /* Check entity visibility */
    if (*(s8 *)((u8 *)tableEntry + 3) == 0) {
        return;
    }

    /* Process render... */
}

/*
 * func_80096734 (716 bytes)
 * Entity LOD selection
 */
void func_80096734(void *entity, f32 distance) {
    /* LOD selection - stub */
}

/*
 * func_80096A00 (348 bytes)
 * Entity cull check
 */
s32 func_80096A00(void *entity, void *camera) {
    /* Frustum culling - stub */
    return 0;
}

/*
 * func_80096CC4 (1216 bytes)
 * Entity render with transforms
 */
void func_80096CC4(void *entity, void *matrix) {
    /* Render with transforms - stub */
}

/*
 * func_80097184 (876 bytes)
 * Entity collision response
 */
void func_80097184(void *entity, s32 collisionType) {
    void *tableEntry;
    s32 result;

    result = *(s32 *)((u8 *)entity + 0);  /* preloaded from v0 */

    if (result == 0) {
        return;
    }

    /* Process collision response... */
}

/*
 * func_800974EC (692 bytes)
 * Entity state machine update
 */
void func_800974EC(void *entity, s32 newState) {
    void *currentPtr;

    sync_acquire((void *)0x80152770, 0, 1);

    if (entity == NULL) {
        entity = *(void **)(0x801527C8);
    }

    /* Check current state */
    currentPtr = *(void **)((u8 *)entity + 0x1C);
    if (currentPtr == NULL) {
        /* Initialize state */
        /* ... */
    }

    sync_release((void *)0x80152770, 0, 1);
}

/*
 * func_800979A8 (264 bytes)
 * Entity position validation
 */
void func_800979A8(void *entity, s32 flags) {
    s32 checkFlag;

    checkFlag = *(s32 *)0x8012EAA0;
    if (checkFlag == -1) {
        return;
    }

    /* Check if entity matches current player */
    if (entity == *(void **)(0x80151A6C)) {
        return;
    }

    /* Process position validation... */
    func_80097798(entity + 10, 0, 0, 0, 0);
}

/*
 * func_80097AFC (364 bytes)
 * Entity velocity update
 */
void func_80097AFC(void *entity, f32 *velocity) {
    /* Velocity update - stub */
}

/*
 * func_800987E8 (140 bytes)
 * Simple state check
 */
s32 func_800987E8(void *entity) {
    s32 state;

    state = *(s32 *)((u8 *)entity + 0x10);
    return (state != 0);
}

/*
 * func_80098874 (108 bytes)
 * Entity flag check
 */
s32 func_80098874(void *entity, s32 flagMask) {
    s32 flags;

    flags = *(s32 *)((u8 *)entity + 0x0C);
    return (flags & flagMask) != 0;
}

/*
 * func_800988E0 (516 bytes)
 * Entity damage/health update
 */
void func_800988E0(void *entity, s32 damage) {
    /* Damage processing - stub */
}

/*
 * func_80098AE4 (1244 bytes)
 * Entity physics step
 */
void func_80098AE4(void *entity, f32 dt) {
    /* Physics step - stub */
}

/*
 * func_80098FC0 (708 bytes)
 * Entity ground check
 */
s32 func_80098FC0(void *entity, f32 *groundNormal) {
    /* Ground check - stub */
    return 0;
}

/*
 * func_800992AC (2388 bytes)
 * Entity AI pathfinding
 */
void func_800992AC(void *entity, void *target) {
    /* Pathfinding - stub */
}

/*
 * func_80099BFC (10220 bytes)
 * Major entity render function
 */
void func_80099BFC(void *entity) {
    void *dlPtr;
    u32 flags;

    dlPtr = *(void **)(0x80149438);  /* Display list pointer */

    if (entity == NULL) {
        return;
    }

    flags = *(u32 *)((u8 *)entity + 0x1C);

    /* Check render flag */
    if ((flags & 0x80000000) != 0) {
        return;
    }

    /* Add display list command */
    *(u32 *)dlPtr = 0xDE000000;
    *(void **)((u8 *)dlPtr + 4) = *(void **)((u8 *)entity + 0x18);
    *(void **)(0x80149438) = (void *)((u8 *)dlPtr + 8);

    /* Complex rendering... */
}

/*
 * func_8009C5E0 (1560 bytes)
 * Lighting/shading calculation
 */
void func_8009C5E0(f32 *color, f32 *normal, f32 *lightDir) {
    /* Lighting calculation - stub */
}

/*
 * func_800A1648 (2256 bytes)
 * Track segment loading
 */
void func_800A1648(s32 segmentId) {
    /* Track loading - stub */
}

/*
 * func_800A1F18 (644 bytes)
 * Track data decompression
 */
void func_800A1F18(void *dest, void *src, s32 size) {
    /* Decompression - stub */
}

/*
 * func_800A21A4 (732 bytes)
 * Track collision setup
 */
void func_800A21A4(void *trackData) {
    /* Collision setup - stub */
}

/*
 * func_800A2D4C (3116 bytes)
 * Major track processing
 */
void func_800A2D4C(void *a0, void *a1, void *a2, void *a3) {
    s8 initFlag;

    initFlag = *(s8 *)(0x8011194C);
    if (initFlag != 0) {
        return;
    }

    /* Set init flag */
    *(s8 *)(0x8011194C) = 1;

    /* Process track segments... */
}

/*
 * func_800A3654 (296 bytes)
 * Track spline interpolation
 */
void func_800A3654(void *spline, f32 t, f32 *outPos) {
    /* Spline interpolation - stub */
}

/*
 * func_800A377C (3156 bytes)
 * Track render processing
 */
void func_800A377C(void *track, s32 mode) {
    s8 renderFlag;

    renderFlag = *(s8 *)(0x8012EAE0);
    if (renderFlag == 0) {
        return;
    }

    /* Process track rendering... */
}

/*
 * func_800A4508 (568 bytes)
 * Car model LOD selection
 */
void func_800A4508(void *car, f32 distance) {
    /* LOD selection - stub */
}

/*
 * func_800A4940 (388 bytes)
 * Wheel rotation update
 *
 * Updates wheel visual rotation based on angular velocity.
 * Also handles tire slip angle calculations for force model.
 *
 * Wheel structure offsets:
 *   0x00: rotation angle
 *   0x04: angular velocity (rad/s)
 *   0x08: radius
 *   0x0C: slip angle
 *   0x10: slip ratio
 *   0x14: contact patch velocity [3]
 */
void func_800A4940(void *wheel, f32 dt) {
    f32 *rotAngle, *angVel;
    f32 radius;
    f32 *slipAngle, *slipRatio;
    f32 *patchVel;
    f32 roadSpeed, wheelSpeed;
    f32 newAngle;

    if (wheel == NULL || dt <= 0.0f) {
        return;
    }

    rotAngle = (f32 *)((u8 *)wheel + 0x00);
    angVel = (f32 *)((u8 *)wheel + 0x04);
    radius = *(f32 *)((u8 *)wheel + 0x08);
    slipAngle = (f32 *)((u8 *)wheel + 0x0C);
    slipRatio = (f32 *)((u8 *)wheel + 0x10);
    patchVel = (f32 *)((u8 *)wheel + 0x14);

    if (radius == 0.0f) radius = 1.0f;

    /* Update rotation angle for visual */
    newAngle = *rotAngle + (*angVel * dt);

    /* Keep angle in 0-2pi range */
    while (newAngle > 6.283185f) {
        newAngle -= 6.283185f;
    }
    while (newAngle < 0.0f) {
        newAngle += 6.283185f;
    }
    *rotAngle = newAngle;

    /* Calculate slip ratio for traction calculations */
    /* Slip ratio = (wheel_speed - road_speed) / max(wheel_speed, road_speed) */
    roadSpeed = patchVel[0];  /* Forward component of contact patch velocity */
    wheelSpeed = *angVel * radius;

    if (fabsf(roadSpeed) > 0.1f || fabsf(wheelSpeed) > 0.1f) {
        f32 maxSpeed = fabsf(roadSpeed);
        if (fabsf(wheelSpeed) > maxSpeed) {
            maxSpeed = fabsf(wheelSpeed);
        }
        *slipRatio = (wheelSpeed - roadSpeed) / maxSpeed;
    } else {
        *slipRatio = 0.0f;
    }

    /* Calculate slip angle from lateral velocity */
    /* Slip angle = atan2(lateral_vel, forward_vel) */
    if (fabsf(patchVel[0]) > 0.1f) {
        *slipAngle = patchVel[1] / patchVel[0];  /* Small angle approximation */
        if (*slipAngle > 0.5f) *slipAngle = 0.5f;
        if (*slipAngle < -0.5f) *slipAngle = -0.5f;
    } else {
        *slipAngle = 0.0f;
    }
}

/*
 * func_800A4CC0 (412 bytes)
 * Suspension compression update
 *
 * Based on arcade tires.c dotireforce() function.
 * Calculates suspension spring and damper forces.
 * Updates suspension compression state.
 *
 * Suspension structure offsets:
 *   0x00: compression (current)
 *   0x04: compression velocity
 *   0x08: spring rate (lb/ft)
 *   0x0C: damping rate (compression)
 *   0x10: damping rate (rebound)
 *   0x14: anti-roll bar rate
 *   0x18: other side compression (for ARB calc)
 *   0x1C: output force
 */
void func_800A4CC0(void *suspension, f32 newCompression) {
    f32 *compression, *compVel;
    f32 springRate, cDamping, rDamping;
    f32 arbRate, otherComp;
    f32 *outputForce;
    f32 springForce, dampForce, arbForce;
    f32 totalForce;
    f32 dt = 0.016f;  /* Assume 60fps */

    if (suspension == NULL) {
        return;
    }

    compression = (f32 *)((u8 *)suspension + 0x00);
    compVel = (f32 *)((u8 *)suspension + 0x04);
    springRate = *(f32 *)((u8 *)suspension + 0x08);
    cDamping = *(f32 *)((u8 *)suspension + 0x0C);
    rDamping = *(f32 *)((u8 *)suspension + 0x10);
    arbRate = *(f32 *)((u8 *)suspension + 0x14);
    otherComp = *(f32 *)((u8 *)suspension + 0x18);
    outputForce = (f32 *)((u8 *)suspension + 0x1C);

    /* Default values if not set */
    if (springRate == 0.0f) springRate = -4000.0f;  /* Negative = upward force */
    if (cDamping == 0.0f) cDamping = -500.0f;
    if (rDamping == 0.0f) rDamping = -500.0f;

    /* Calculate compression velocity */
    *compVel = (newCompression - *compression) / dt;

    /* Update compression */
    *compression = newCompression;

    /* Only apply forces if in contact with ground */
    if (newCompression <= 0.0f) {
        *outputForce = 0.0f;
        return;
    }

    /* Spring force: F = -k * x */
    springForce = springRate * newCompression;

    /* Progressive spring rate for bottoming out */
    if (newCompression > 10.0f) {
        springForce *= (newCompression / 10.0f);
    }

    /* Damping force: F = -c * v */
    if (*compVel > 0.0f) {
        /* Compression stroke */
        dampForce = cDamping * (*compVel);
    } else {
        /* Rebound stroke */
        dampForce = rDamping * (*compVel);
    }

    /* Anti-roll bar force */
    /* Differential force based on compression difference */
    if (arbRate != 0.0f) {
        arbForce = arbRate * (newCompression - otherComp);
    } else {
        arbForce = 0.0f;
    }

    /* Total vertical force */
    totalForce = springForce + dampForce + arbForce;

    /* Clamp to prevent negative (pulling down) forces */
    if (totalForce < 0.0f) {
        totalForce = 0.0f;
    }

    *outputForce = totalForce;
}

/*
 * func_800A4E60 (444 bytes)
 * Tire skid mark generation
 *
 * Creates visual skid marks on the ground based on tire slip.
 * Based on arcade tires.c screech calculations.
 *
 * Tire structure offsets:
 *   0x10: slip ratio
 *   0x0C: slip angle
 *   0x20: skid mark buffer pointer
 *   0x24: skid mark index
 */
void func_800A4E60(void *tire, f32 *pos, f32 intensity) {
    f32 slipRatio, slipAngle;
    f32 totalSlip;
    void **skidBuffer;
    s32 *skidIndex;
    f32 *skidEntry;
    s32 idx;

    if (tire == NULL || pos == NULL) {
        return;
    }

    slipRatio = *(f32 *)((u8 *)tire + 0x10);
    slipAngle = *(f32 *)((u8 *)tire + 0x0C);
    skidBuffer = (void **)((u8 *)tire + 0x20);
    skidIndex = (s32 *)((u8 *)tire + 0x24);

    if (*skidBuffer == NULL) {
        return;
    }

    /* Calculate total slip magnitude */
    totalSlip = sqrtf(slipRatio * slipRatio + slipAngle * slipAngle);

    /* Only generate skid mark if slip exceeds threshold */
    if (totalSlip < 0.15f) {
        return;
    }

    /* Scale intensity by slip amount */
    intensity *= (totalSlip - 0.15f) * 2.0f;
    if (intensity > 1.0f) intensity = 1.0f;
    if (intensity < 0.0f) intensity = 0.0f;

    /* Add new skid mark point */
    idx = *skidIndex;
    skidEntry = (f32 *)((u8 *)(*skidBuffer) + idx * 16);  /* 16 bytes per entry */

    /* Store position */
    skidEntry[0] = pos[0];
    skidEntry[1] = pos[1] + 0.01f;  /* Slightly above ground */
    skidEntry[2] = pos[2];

    /* Store intensity/alpha */
    skidEntry[3] = intensity;

    /* Advance index (circular buffer) */
    *skidIndex = (idx + 1) % 64;  /* 64 entry buffer */
}

/*
 * func_800A51E0 (932 bytes)
 * Car damage visual update
 */
void func_800A51E0(void *car, s32 damageLevel) {
    /* Damage visual - stub */
}

/*
 * func_800A5588 (444 bytes)
 * Engine particle effect
 */
void func_800A5588(void *car, s32 effectType) {
    /* Particle effect - stub */
}

/*
 * func_800A5744 (488 bytes)
 * Exhaust smoke effect
 */
void func_800A5744(void *car, f32 *exhaustPos) {
    /* Exhaust effect - stub */
}

/*
 * func_800A5D34 (1116 bytes)
 * Car shadow rendering
 */
void func_800A5D34(void *car, void *ground) {
    /* Shadow render - stub */
}

/*
 * func_800A6094 (428 bytes)
 * Headlight/taillight rendering
 */
void func_800A6094(void *car, s32 lightMask) {
    /* Light render - stub */
}

/*
 * func_800A6244 (448 bytes)
 * Brake light update
 */
void func_800A6244(void *car, s32 braking) {
    /* Brake light - stub */
}

/*
 * func_800A6404 (2016 bytes)
 * Car full render
 */
void func_800A6404(void *car) {
    /* Full car render - stub */
}

/*
 * func_800A6BE4 (3300 bytes)
 * Car physics integration - main physics loop
 *
 * Based on arcade drivsym.c sym() and regular() functions.
 * Handles:
 *   1. Controls processing
 *   2. Drivetrain simulation
 *   3. Road/terrain detection
 *   4. Force calculations (tires, gravity, drag)
 *   5. Torque calculations
 *   6. Acceleration integration
 *   7. Velocity integration
 *   8. Position integration
 *
 * Car structure offsets:
 *   0x24: position (vec3)
 *   0x34: velocity (vec3)
 *   0x40: acceleration (vec3)
 *   0x60: forward direction (vec3)
 *   0x6C: up direction (vec3)
 *   0x78: right direction (vec3)
 *   0x84: angular velocity (vec3)
 *   0x90: angular acceleration (vec3)
 *   0x9C: tire forces [4][3]
 *   0xCC: suspension compression [4]
 *   0xDC: mass
 *   0xE0: inverse mass
 *   0xE4: moment of inertia (vec3)
 *   0xF0: inverse MOI (vec3)
 *   0xFC: steer angle
 *   0x100: throttle
 *   0x104: brake
 *   0x108: gear
 *   0x10C: RPM
 *   0x110: torque output
 *   0x114: crash flag
 *   0x118: road code [4]
 *   0x128: wheel base
 *   0x12C: track width
 *   0x130: CG height
 *   0x134: drag coefficient
 *   0x138: roll resistance
 */
void func_800A6BE4(void *car, f32 dt) {
    f32 *pos, *vel, *accel;
    f32 *forward, *up, *right;
    f32 *angVel, *angAccel;
    f32 *tireForces, *susComp;
    f32 mass, invMass;
    f32 *invi;
    f32 steerAngle, throttle, brake;
    f32 dragCoef, rollResist;
    f32 totalForce[3], totalMoment[3];
    f32 temp[3], tempMag;
    f32 gravity[3] = {0.0f, -32.2f, 0.0f};  /* ft/s^2 */
    f32 dragForce, rollForce;
    f32 wheelBase, trackWidth;
    s32 *roadCode;
    s32 crashFlag;
    s32 i;

    if (car == NULL || dt <= 0.0f) {
        return;
    }

    /* Get car state pointers */
    pos = (f32 *)((u8 *)car + 0x24);
    vel = (f32 *)((u8 *)car + 0x34);
    accel = (f32 *)((u8 *)car + 0x40);
    forward = (f32 *)((u8 *)car + 0x60);
    up = (f32 *)((u8 *)car + 0x6C);
    right = (f32 *)((u8 *)car + 0x78);
    angVel = (f32 *)((u8 *)car + 0x84);
    angAccel = (f32 *)((u8 *)car + 0x90);
    tireForces = (f32 *)((u8 *)car + 0x9C);
    susComp = (f32 *)((u8 *)car + 0xCC);
    mass = *(f32 *)((u8 *)car + 0xDC);
    invMass = *(f32 *)((u8 *)car + 0xE0);
    invi = (f32 *)((u8 *)car + 0xF0);
    steerAngle = *(f32 *)((u8 *)car + 0xFC);
    throttle = *(f32 *)((u8 *)car + 0x100);
    brake = *(f32 *)((u8 *)car + 0x104);
    roadCode = (s32 *)((u8 *)car + 0x118);
    wheelBase = *(f32 *)((u8 *)car + 0x128);
    trackWidth = *(f32 *)((u8 *)car + 0x12C);
    dragCoef = *(f32 *)((u8 *)car + 0x134);
    rollResist = *(f32 *)((u8 *)car + 0x138);
    crashFlag = *(s32 *)((u8 *)car + 0x114);

    /* Skip physics if crashed */
    if (crashFlag != 0) {
        return;
    }

    /* Initialize total force and moment */
    totalForce[0] = 0.0f;
    totalForce[1] = 0.0f;
    totalForce[2] = 0.0f;
    totalMoment[0] = 0.0f;
    totalMoment[1] = 0.0f;
    totalMoment[2] = 0.0f;

    /* Calculate speed in forward direction */
    tempMag = vel[0] * forward[0] + vel[1] * forward[1] + vel[2] * forward[2];

    /* Aerodynamic drag: F = -C * v^2 */
    if (tempMag > 0.01f || tempMag < -0.01f) {
        dragForce = -dragCoef * tempMag * fabsf(tempMag);
        totalForce[0] += dragForce * forward[0];
        totalForce[1] += dragForce * forward[1];
        totalForce[2] += dragForce * forward[2];
    }

    /* Rolling resistance */
    rollForce = -rollResist * mass;
    if (tempMag > 0.0f) {
        totalForce[0] += rollForce * forward[0];
        totalForce[2] += rollForce * forward[2];
    } else if (tempMag < 0.0f) {
        totalForce[0] -= rollForce * forward[0];
        totalForce[2] -= rollForce * forward[2];
    }

    /* Sum tire forces */
    for (i = 0; i < 4; i++) {
        totalForce[0] += tireForces[i * 3 + 0];
        totalForce[1] += tireForces[i * 3 + 1];
        totalForce[2] += tireForces[i * 3 + 2];
    }

    /* Add gravity */
    totalForce[0] += gravity[0] * mass;
    totalForce[1] += gravity[1] * mass;
    totalForce[2] += gravity[2] * mass;

    /* Calculate torques from tire forces */
    /* Front right tire */
    temp[0] = wheelBase * 0.5f;
    temp[1] = 0.0f;
    temp[2] = trackWidth * 0.5f;
    totalMoment[0] += temp[1] * tireForces[0*3+2] - temp[2] * tireForces[0*3+1];
    totalMoment[1] += temp[2] * tireForces[0*3+0] - temp[0] * tireForces[0*3+2];
    totalMoment[2] += temp[0] * tireForces[0*3+1] - temp[1] * tireForces[0*3+0];

    /* Front left tire */
    temp[2] = -trackWidth * 0.5f;
    totalMoment[0] += temp[1] * tireForces[1*3+2] - temp[2] * tireForces[1*3+1];
    totalMoment[1] += temp[2] * tireForces[1*3+0] - temp[0] * tireForces[1*3+2];
    totalMoment[2] += temp[0] * tireForces[1*3+1] - temp[1] * tireForces[1*3+0];

    /* Rear right tire */
    temp[0] = -wheelBase * 0.5f;
    temp[2] = trackWidth * 0.5f;
    totalMoment[0] += temp[1] * tireForces[2*3+2] - temp[2] * tireForces[2*3+1];
    totalMoment[1] += temp[2] * tireForces[2*3+0] - temp[0] * tireForces[2*3+2];
    totalMoment[2] += temp[0] * tireForces[2*3+1] - temp[1] * tireForces[2*3+0];

    /* Rear left tire */
    temp[2] = -trackWidth * 0.5f;
    totalMoment[0] += temp[1] * tireForces[3*3+2] - temp[2] * tireForces[3*3+1];
    totalMoment[1] += temp[2] * tireForces[3*3+0] - temp[0] * tireForces[3*3+2];
    totalMoment[2] += temp[0] * tireForces[3*3+1] - temp[1] * tireForces[3*3+0];

    /* Calculate accelerations: A = F / m */
    accel[0] = totalForce[0] * invMass;
    accel[1] = totalForce[1] * invMass;
    accel[2] = totalForce[2] * invMass;

    /* Calculate angular accelerations: AA = M * invI */
    angAccel[0] = totalMoment[0] * invi[0];
    angAccel[1] = totalMoment[1] * invi[1];
    angAccel[2] = totalMoment[2] * invi[2];

    /* Integrate velocities: V = V + A * dt */
    vel[0] += accel[0] * dt;
    vel[1] += accel[1] * dt;
    vel[2] += accel[2] * dt;

    /* Clamp velocity magnitude to prevent instability */
    tempMag = sqrtf(vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2]);
    if (tempMag > 1000.0f) {
        f32 scale = 1000.0f / tempMag;
        vel[0] *= scale;
        vel[1] *= scale;
        vel[2] *= scale;
    }

    /* Integrate angular velocities: W = W + AA * dt */
    angVel[0] += angAccel[0] * dt;
    angVel[1] += angAccel[1] * dt;
    angVel[2] += angAccel[2] * dt;

    /* Integrate positions: P = P + V * dt */
    pos[0] += vel[0] * dt;
    pos[1] += vel[1] * dt;
    pos[2] += vel[2] * dt;

    /* Update orientation from angular velocity */
    func_800A6F00(car, angVel, dt);  /* Update orientation vectors */
}

/*
 * func_800A78C8 (580 bytes)
 * Car steering response
 *
 * Based on arcade controls() function in drivsym.c.
 * Converts steering wheel input to front tire steer angle.
 * Applies steering ratio and speed-sensitive steering reduction.
 *
 * Car structure offsets:
 *   0x34: velocity (vec3)
 *   0x60: forward direction (vec3)
 *   0xFC: steer angle (output)
 *   0x13C: steering ratio (degrees input / degrees wheel)
 *   0x140: max steer angle
 *   0x144: steering speed factor
 */
void func_800A78C8(void *car, f32 steerInput) {
    f32 *vel, *forward;
    f32 *steerAngle;
    f32 steerRatio, maxSteer, speedFactor;
    f32 speed, speedEffect;
    f32 targetAngle, currentAngle;
    f32 steerRate;

    if (car == NULL) {
        return;
    }

    vel = (f32 *)((u8 *)car + 0x34);
    forward = (f32 *)((u8 *)car + 0x60);
    steerAngle = (f32 *)((u8 *)car + 0xFC);
    steerRatio = *(f32 *)((u8 *)car + 0x13C);
    maxSteer = *(f32 *)((u8 *)car + 0x140);
    speedFactor = *(f32 *)((u8 *)car + 0x144);

    /* Default values if not set */
    if (steerRatio == 0.0f) steerRatio = 14.0f;
    if (maxSteer == 0.0f) maxSteer = 0.5f;  /* ~28 degrees in radians */
    if (speedFactor == 0.0f) speedFactor = 0.01f;

    /* Calculate forward speed */
    speed = vel[0] * forward[0] + vel[1] * forward[1] + vel[2] * forward[2];
    speed = fabsf(speed);

    /* Speed-sensitive steering reduction */
    /* At high speeds, reduce max steering angle for stability */
    speedEffect = 1.0f / (1.0f + speedFactor * speed);

    /* Calculate target steer angle from input */
    /* steerInput is normalized -1.0 to 1.0 */
    targetAngle = (steerInput / steerRatio) * maxSteer * speedEffect;

    /* Clamp to max steering angle */
    if (targetAngle > maxSteer) {
        targetAngle = maxSteer;
    } else if (targetAngle < -maxSteer) {
        targetAngle = -maxSteer;
    }

    /* Apply steering with some lag for realism */
    currentAngle = *steerAngle;
    steerRate = 3.0f;  /* Steering response rate */

    if (targetAngle > currentAngle) {
        currentAngle += steerRate * 0.016f;  /* Assume ~60fps */
        if (currentAngle > targetAngle) {
            currentAngle = targetAngle;
        }
    } else if (targetAngle < currentAngle) {
        currentAngle -= steerRate * 0.016f;
        if (currentAngle < targetAngle) {
            currentAngle = targetAngle;
        }
    }

    *steerAngle = currentAngle;

    /* Update front tire steering for tire force calculations */
    func_800A79E0(car, currentAngle);  /* Update tire orientations */
}

/*
 * func_800A7AE4 (440 bytes)
 * Throttle/brake input processing
 *
 * Based on arcade drivetrain() and controls() functions.
 * Processes throttle and brake pedal inputs.
 * Updates engine torque output and brake force.
 *
 * Car structure offsets:
 *   0x100: throttle value (stored)
 *   0x104: brake value (stored)
 *   0x108: current gear
 *   0x10C: RPM
 *   0x110: torque output
 *   0x148: max torque
 *   0x14C: max brake force
 *   0x150: idle RPM
 *   0x154: redline RPM
 *   0x158: gear ratios [6]
 */
void func_800A7AE4(void *car, f32 throttle, f32 brake) {
    f32 *storedThrottle, *storedBrake;
    f32 *torqueOut;
    f32 *rpm;
    f32 maxTorque, maxBrake;
    f32 idleRpm, redlineRpm;
    f32 *gearRatios;
    s32 gear;
    f32 rpmNorm, torqueCurve;
    f32 engineTorque, brakeTorque;

    if (car == NULL) {
        return;
    }

    storedThrottle = (f32 *)((u8 *)car + 0x100);
    storedBrake = (f32 *)((u8 *)car + 0x104);
    gear = *(s32 *)((u8 *)car + 0x108);
    rpm = (f32 *)((u8 *)car + 0x10C);
    torqueOut = (f32 *)((u8 *)car + 0x110);
    maxTorque = *(f32 *)((u8 *)car + 0x148);
    maxBrake = *(f32 *)((u8 *)car + 0x14C);
    idleRpm = *(f32 *)((u8 *)car + 0x150);
    redlineRpm = *(f32 *)((u8 *)car + 0x154);
    gearRatios = (f32 *)((u8 *)car + 0x158);

    /* Default values if not set */
    if (maxTorque == 0.0f) maxTorque = 300.0f;
    if (maxBrake == 0.0f) maxBrake = 5000.0f;
    if (idleRpm == 0.0f) idleRpm = 800.0f;
    if (redlineRpm == 0.0f) redlineRpm = 8000.0f;

    /* Clamp inputs to valid range */
    if (throttle < 0.0f) throttle = 0.0f;
    if (throttle > 1.0f) throttle = 1.0f;
    if (brake < 0.0f) brake = 0.0f;
    if (brake > 1.0f) brake = 1.0f;

    /* Store processed inputs */
    *storedThrottle = throttle;
    *storedBrake = brake;

    /* Calculate normalized RPM position in powerband */
    rpmNorm = (*rpm - idleRpm) / (redlineRpm - idleRpm);
    if (rpmNorm < 0.0f) rpmNorm = 0.0f;
    if (rpmNorm > 1.0f) rpmNorm = 1.0f;

    /* Simple torque curve: peaks around 0.6-0.7 of rev range */
    /* Based on arcade rushtorquecurve pattern */
    if (rpmNorm < 0.3f) {
        torqueCurve = 0.6f + rpmNorm * 1.33f;  /* Ramp up */
    } else if (rpmNorm < 0.7f) {
        torqueCurve = 1.0f;  /* Peak torque band */
    } else {
        torqueCurve = 1.0f - (rpmNorm - 0.7f) * 2.0f;  /* Fall off */
        if (torqueCurve < 0.3f) torqueCurve = 0.3f;
    }

    /* Engine torque = throttle * torque_curve * max_torque */
    engineTorque = throttle * torqueCurve * maxTorque;

    /* Apply gear ratio if valid gear */
    if (gear >= 1 && gear <= 6) {
        f32 ratio = gearRatios[gear - 1];
        if (ratio != 0.0f) {
            engineTorque *= ratio;
        }
    }

    /* Calculate brake torque (negative to oppose motion) */
    brakeTorque = -brake * maxBrake;

    /* Output combined torque */
    /* If braking hard, engine torque is overridden */
    if (brake > 0.5f) {
        *torqueOut = brakeTorque;
    } else {
        *torqueOut = engineTorque + brakeTorque;
    }

    /* Apply torque to drive wheels via drivetrain */
    func_800A7B90(car, *torqueOut);  /* Distribute to drive wheels */
}

/*
 * func_800A7C9C (236 bytes)
 * Car gear shift
 *
 * Based on arcade transmission code.
 * Handles manual and automatic gear changes.
 * Updates gear, recalculates drive ratio, and adjusts RPM.
 *
 * Car structure offsets:
 *   0x108: current gear
 *   0x10C: RPM
 *   0x170: auto transmission flag
 *   0x174: shift timer
 *   0x158: gear ratios [6]
 */
void func_800A7C9C(void *car, s32 newGear) {
    s32 *currentGear;
    f32 *rpm;
    f32 *gearRatios;
    s32 autoTrans;
    f32 *shiftTimer;
    f32 oldRatio, newRatio;
    f32 currentRpm;

    if (car == NULL) {
        return;
    }

    currentGear = (s32 *)((u8 *)car + 0x108);
    rpm = (f32 *)((u8 *)car + 0x10C);
    gearRatios = (f32 *)((u8 *)car + 0x158);
    autoTrans = *(s32 *)((u8 *)car + 0x170);
    shiftTimer = (f32 *)((u8 *)car + 0x174);

    /* Clamp gear to valid range: -1 (reverse), 0 (neutral), 1-6 */
    if (newGear < -1) newGear = -1;
    if (newGear > 6) newGear = 6;

    /* Check if shift is allowed (not in middle of shift) */
    if (*shiftTimer > 0.0f) {
        return;  /* Still shifting */
    }

    /* Don't shift if already in this gear */
    if (newGear == *currentGear) {
        return;
    }

    /* Get current and new gear ratios */
    if (*currentGear >= 1 && *currentGear <= 6) {
        oldRatio = gearRatios[*currentGear - 1];
    } else if (*currentGear == -1) {
        oldRatio = -3.0f;  /* Reverse ratio */
    } else {
        oldRatio = 0.0f;  /* Neutral */
    }

    if (newGear >= 1 && newGear <= 6) {
        newRatio = gearRatios[newGear - 1];
    } else if (newGear == -1) {
        newRatio = -3.0f;  /* Reverse ratio */
    } else {
        newRatio = 0.0f;  /* Neutral */
    }

    /* Update gear */
    *currentGear = newGear;

    /* Adjust RPM for new gear ratio */
    currentRpm = *rpm;
    if (oldRatio != 0.0f && newRatio != 0.0f) {
        *rpm = currentRpm * (newRatio / oldRatio);
    }

    /* Start shift timer (prevents rapid shifting) */
    *shiftTimer = 0.2f;  /* 200ms shift delay */

    /* Clamp RPM to valid range */
    if (*rpm < 800.0f) *rpm = 800.0f;
    if (*rpm > 9000.0f) *rpm = 9000.0f;
}

/*
 * func_800A7D88 (104 bytes)
 * Get current gear
 */
s32 func_800A7D88(void *car) {
    return *(s32 *)((u8 *)car + 0x48);
}

/*
 * func_800A7E00 (1200 bytes - estimated)
 * Tire force calculation (friction circle model)
 *
 * Based on arcade tires.c frictioncircle() function.
 * Calculates lateral and longitudinal tire forces using
 * Pacejka-style tire model with friction circle limit.
 *
 * Tire structure offsets:
 *   0x00: rotation angle
 *   0x04: angular velocity
 *   0x08: radius
 *   0x0C: slip angle
 *   0x10: slip ratio
 *   0x14: contact patch velocity [3]
 *   0x20: normal force
 *   0x24: lateral force (output)
 *   0x28: longitudinal force (output)
 *   0x2C: cornering stiffness
 *   0x30: max friction coefficient
 *   0x34: drive torque input
 */
void func_800A7E00(void *tire, f32 normalForce, f32 driveTorque) {
    f32 *angVel, radius;
    f32 *slipAngle, *slipRatio;
    f32 *patchVel;
    f32 *latForce, *lonForce;
    f32 cStiff, cfMax;
    f32 maxTraction, maxLateral;
    f32 lateralForce, tractionForce;
    f32 totalForce, forceMag;
    f32 roadAngVel, slipSpeed;

    if (tire == NULL) {
        return;
    }

    angVel = (f32 *)((u8 *)tire + 0x04);
    radius = *(f32 *)((u8 *)tire + 0x08);
    slipAngle = (f32 *)((u8 *)tire + 0x0C);
    slipRatio = (f32 *)((u8 *)tire + 0x10);
    patchVel = (f32 *)((u8 *)tire + 0x14);
    latForce = (f32 *)((u8 *)tire + 0x24);
    lonForce = (f32 *)((u8 *)tire + 0x28);
    cStiff = *(f32 *)((u8 *)tire + 0x2C);
    cfMax = *(f32 *)((u8 *)tire + 0x30);

    /* Default tire parameters if not set */
    if (radius == 0.0f) radius = 1.0f;
    if (cStiff == 0.0f) cStiff = 16000.0f;  /* Cornering stiffness lb/rad */
    if (cfMax == 0.0f) cfMax = 1.15f;       /* Max friction coefficient */

    /* No forces if no normal load */
    if (normalForce <= 0.0f) {
        *latForce = 0.0f;
        *lonForce = 0.0f;
        return;
    }

    /* Maximum available friction force */
    maxTraction = cfMax * normalForce;

    /* Calculate road angular velocity (wheel would have if not slipping) */
    roadAngVel = patchVel[0] / radius;

    /* Longitudinal force from drive torque */
    /* Check for wheel spin or lockup */
    if (*angVel > roadAngVel) {
        /* Wheel spinning faster than road - accelerating/wheelspin */
        tractionForce = driveTorque / radius;

        /* Limit by friction circle */
        if (tractionForce > maxTraction) {
            tractionForce = maxTraction;
            /* Wheel continues to spin */
        }

        /* Update wheel angular velocity based on torque and resistance */
        *angVel += (driveTorque - tractionForce * radius) * 0.001f * 0.016f;

    } else if (*angVel < roadAngVel) {
        /* Wheel slower than road - braking/lockup */
        tractionForce = driveTorque / radius;

        if (tractionForce < -maxTraction) {
            tractionForce = -maxTraction;
        }

        *angVel += (driveTorque - tractionForce * radius) * 0.001f * 0.016f;

    } else {
        /* Wheel matched to road - pure rolling */
        tractionForce = driveTorque / radius;
        *angVel = roadAngVel;
    }

    /* Lateral force from cornering */
    /* Using linear tire model up to saturation */
    lateralForce = -cStiff * (*slipAngle);

    /* Calculate total slip speed for friction circle */
    slipSpeed = sqrtf((*slipRatio) * (*slipRatio) + (*slipAngle) * (*slipAngle));

    /* Apply friction circle limit */
    /* Total force magnitude cannot exceed maxTraction */
    forceMag = sqrtf(lateralForce * lateralForce + tractionForce * tractionForce);

    if (forceMag > maxTraction) {
        /* Scale both forces proportionally */
        f32 scale = maxTraction / forceMag;
        lateralForce *= scale;
        tractionForce *= scale;
    }

    /* Output forces */
    *latForce = lateralForce;
    *lonForce = tractionForce;
}

/*
 * func_800A80D0 (3276 bytes)
 * Car AI behavior
 */
void func_800A80D0(void *car, void *target) {
    /* AI behavior - stub */
}

/*
 * func_800A8D9C (484 bytes)
 * AI path following
 */
void func_800A8D9C(void *car, void *path) {
    /* Path following - stub */
}

/*
 * func_800A8F64 (1068 bytes)
 * AI obstacle avoidance
 */
void func_800A8F64(void *car, void *obstacles) {
    /* Obstacle avoidance - stub */
}

/*
 * func_800A9390 (1592 bytes)
 * AI racing line optimization
 */
void func_800A9390(void *car, void *track) {
    /* Racing line - stub */
}

/*
 * func_800A99C8 (2700 bytes)
 * AI decision making
 */
void func_800A99C8(void *car) {
    /* AI decision - stub */
}

/*
 * func_800AA454 (692 bytes)
 * AI speed control
 */
void func_800AA454(void *car, f32 targetSpeed) {
    /* Speed control - stub */
}

/*
 * func_800AA708 (1084 bytes)
 * AI overtaking behavior
 */
void func_800AA708(void *car, void *opponent) {
    /* Overtaking - stub */
}

/*
 * func_800AAB44 (808 bytes)
 * AI defensive driving
 */
void func_800AAB44(void *car) {
    /* Defensive AI - stub */
}

/*
 * func_800AAE68 (1756 bytes)
 * AI recovery from crash
 */
void func_800AAE68(void *car) {
    /* Crash recovery - stub */
}

/*
 * func_800AB544 (564 bytes)
 * Player input reading
 */
void func_800AB544(void *player, void *controller) {
    /* Input reading - stub */
}

/*
 * func_800AB7D8 (1016 bytes)
 * Player state update
 */
void func_800AB7D8(void *player) {
    /* Player state - stub */
}

/*
 * func_800ABBD0 (248 bytes)
 * Player respawn position
 */
void func_800ABBD0(void *player, f32 *respawnPos) {
    /* Respawn position - stub */
}

/*
 * func_800ABCC8 (2708 bytes)
 * Player full update
 */
void func_800ABCC8(void *player, f32 dt) {
    /* Player update - stub */
}

/*
 * func_800AC75C (1076 bytes)
 * Checkpoint collision
 */
void func_800AC75C(void *car, void *checkpoint) {
    /* Checkpoint collision - stub */
}

/*
 * func_800ACA9C (380 bytes)
 * Lap time recording
 */
void func_800ACA9C(void *player, s32 lapNum) {
    /* Lap time - stub */
}

/*
 * func_800ACC18 (1008 bytes)
 * Race position calculation
 */
void func_800ACC18(void) {
    /* Race position - stub */
}

/*
 * func_800AD008 (152 bytes)
 * Get race position
 */
s32 func_800AD008(void *player) {
    return *(s32 *)((u8 *)player + 0x5C);
}

/*
 * func_800AD0A0 (136 bytes)
 * Get lap count
 */
s32 func_800AD0A0(void *player) {
    return *(s32 *)((u8 *)player + 0x58);
}

/*
 * func_800AD128 (1548 bytes)
 * Race finish handling
 */
void func_800AD128(void *player) {
    /* Race finish - stub */
}

/*
 * func_800AD734 (1572 bytes)
 * Race results display
 */
void func_800AD734(void) {
    /* Results display - stub */
}

/*
 * func_800ADD58 (3580 bytes)
 * Leaderboard update
 */
void func_800ADD58(void) {
    /* Leaderboard - stub */
}

/*
 * func_800AEB54 (528 bytes)
 * High score check
 */
s32 func_800AEB54(s32 time) {
    /* High score check - stub */
    return 0;
}

/*
 * func_800AED64 (644 bytes)
 * High score entry
 */
void func_800AED64(s32 position, u8 *name, s32 time) {
    /* High score entry - stub */
}

/*
 * func_800AEFE8 (504 bytes)
 * Save high scores
 */
void func_800AEFE8(void) {
    /* Save scores - stub */
}

/*
 * func_800B087C (1012 bytes)
 * Audio voice allocation
 */
void func_800B087C(s32 voiceId, s32 priority) {
    /* Voice allocation - stub */
}

/*
 * func_800B24EC (4256 bytes)
 * Audio sequence player
 */
void func_800B24EC(void *sequence) {
    /* Sequence player - stub */
}

/*
 * func_800B358C (160 bytes)
 * Audio volume set
 */
void func_800B358C(s32 channel, f32 volume) {
    /* Volume set - stub */
}

/*
 * func_800B362C (444 bytes)
 * Audio pan set
 */
void func_800B362C(s32 channel, f32 pan) {
    /* Pan set - stub */
}

/*
 * func_800B3D18 (228 bytes)
 * Audio pitch set
 */
void func_800B3D18(s32 channel, f32 pitch) {
    /* Pitch set - stub */
}

/*
 * func_800B3FA4 (604 bytes)
 * Audio effect apply
 */
void func_800B3FA4(s32 channel, s32 effectType, f32 amount) {
    /* Effect apply - stub */
}

/*
 * func_800B4200 (352 bytes)
 * Audio buffer fill
 */
void func_800B4200(void *buffer, s32 samples) {
    /* Buffer fill - stub */
}

/*
 * func_800B438C (228 bytes)
 * Audio stream start
 */
void func_800B438C(s32 streamId, void *data) {
    /* Stream start - stub */
}

/*
 * func_800B466C (604 bytes)
 * Audio stream update
 */
void func_800B466C(s32 streamId) {
    /* Stream update - stub */
}

/*
 * func_800B4FB0 (1484 bytes)
 * Audio music playback
 */
void func_800B4FB0(s32 trackId) {
    /* Music playback - stub */
}

/*
 * func_800B6024 (276 bytes)
 * Get entity float array element
 */
f32 func_800B6024(void *entity, s32 index) {
    f32 *arr = (f32 *)((u8 *)entity + 0x30);
    return arr[index];
}

/*
 * func_800B61FC (380 bytes)
 * Set entity vector with validation
 */
void func_800B61FC(void *entity, f32 *vec) {
    f32 *dest = (f32 *)((u8 *)entity + 0x24);

    if (vec != NULL) {
        dest[0] = vec[0];
        dest[1] = vec[1];
        dest[2] = vec[2];
    }
}

/*
/*
/*
 * func_800B74A0 (2904 bytes)
 * Entity full collision test
 */
void func_800B74A0(void *entity, void *world) {
    /* Full collision - stub */
}

/*
/*
/*
 * func_800B82C8 (2380 bytes)
 * Collision response calculation
 */
void func_800B82C8(void *entityA, void *entityB, f32 *normal) {
    /* Collision response - stub */
}

/*
 * func_800B8C14 (1252 bytes)
 * Physics constraint solve
 */
void func_800B8C14(void *constraint) {
    /* Constraint solve - stub */
}

/*
/*
 * func_800BAAA0 (744 bytes)
 * Broadphase collision check
 */
void func_800BAAA0(void *world) {
    /* Broadphase - stub */
}

/*
 * func_800BADE0 (3448 bytes)
 * Narrowphase collision
 */
void func_800BADE0(void *pairA, void *pairB) {
    /* Narrowphase - stub */
}

/*
 * func_800BB9B0 (2500 bytes)
 * Track surface query
 */
s32 func_800BB9B0(f32 *pos, f32 *normal, f32 *height) {
    /* Surface query - stub */
    return 0;
}

/*
 * func_800BE7BC (1016 bytes)
 * Camera target tracking
 */
void func_800BE7BC(void *camera, void *target) {
    /* Target tracking - stub */
}

/*
 * func_800BF01C (8 bytes)
 * Empty function
 */
void func_800BF01C(void) {
    /* Empty stub */
}

/*
 * func_800BF0A4 (848 bytes)
 * Camera shake effect
 */
void func_800BF0A4(void *camera, f32 intensity, f32 duration) {
    /* Camera shake - stub */
}

/*
 * func_800C813C (804 bytes)
 * HUD element render
 */
void func_800C813C(void *hud, s32 elementId) {
    /* HUD render - stub */
}

/*
/*
 * func_800C84FC (868 bytes)
 * Speedometer update
 */
void func_800C84FC(void *hud, f32 speed) {
    /* Speedometer - stub */
}

/*
 * func_800C885C (816 bytes)
 * Tachometer update
 */
void func_800C885C(void *hud, f32 rpm) {
    /* Tachometer - stub */
}

/*
 * func_800C8B8C (1048 bytes)
 * Lap counter update
 */
void func_800C8B8C(void *hud, s32 lap, s32 totalLaps) {
    /* Lap counter - stub */
}

/*
 * func_800C8FA4 (316 bytes)
 * Position display update
 */
void func_800C8FA4(void *hud, s32 position) {
    /* Position display - stub */
}

/*
 * func_800C9158 (184 bytes)
 * Timer display
 */
void func_800C9158(void *hud, s32 timeMs) {
    /* Timer display - stub */
}

/*
 * func_800C9210 (204 bytes)
 * Speed display
 */
void func_800C9210(void *hud, s32 speed) {
    /* Speed display - stub */
}

/*
 * func_800C9480 (168 bytes)
 * Nitro meter update
 */
void func_800C9480(void *hud, f32 nitroLevel) {
    /* Nitro meter - stub */
}

/*
 * func_800C9BE0 (1824 bytes)
 * Full HUD update
 */
void func_800C9BE0(void *hud) {
    /* Full HUD - stub */
}

/*
 * func_800CA300 (180 bytes)
 * HUD fade effect
 */
void func_800CA300(void *hud, f32 alpha) {
    /* HUD fade - stub */
}

/*
 * func_800CBF2C (12544 bytes)
 * Menu system update
 */
void func_800CBF2C(void *menu) {
    /* Menu update - stub */
}

/*
 * func_800CF06C (5748 bytes)
 * Menu render
 */
void func_800CF06C(void *menu) {
    /* Menu render - stub */
}

/*
 * func_800D0424 (3040 bytes)
 * Menu input handling
 */
void func_800D0424(void *menu, void *input) {
    /* Menu input - stub */
}

/*
 * func_800D1004 (5196 bytes)
 * Track select menu
 */
void func_800D1004(void *menu) {
    /* Track select - stub */
}

/*
 * func_800D4DFC (1228 bytes)
 * Car select menu
 */
void func_800D4DFC(void *menu) {
    /* Car select - stub */
}

/*
 * func_800D52CC (8 bytes)
 * Empty stub
 */
void func_800D52CC(void) {
    /* Empty */
}

/*
 * func_800D5374 (332 bytes)
 * Options menu
 */
void func_800D5374(void *menu) {
    /* Options menu - stub */
}

/*
 * func_800D5524 (628 bytes)
 * Audio options
 */
void func_800D5524(void *menu) {
    /* Audio options - stub */
}

/*
 * func_800D5A04 (1704 bytes)
 * Controller config
 */
void func_800D5A04(void *menu) {
    /* Controller config - stub */
}

/*
 * func_800D60AC (8 bytes)
 * Empty stub
 */
void func_800D60AC(void) {
    /* Empty */
}

/*
 * func_800D6160 (496 bytes)
 * Save/load menu
 */
void func_800D6160(void *menu) {
    /* Save/load - stub */
}

/*
 * func_800D6530 (4356 bytes)
 * Memory card operations
 */
void func_800D6530(s32 operation) {
    /* Memory card - stub */
}

/*
 * func_800D7634 (1804 bytes)
 * Profile management
 */
void func_800D7634(void *profile) {
    /* Profile management - stub */
}

/*
 * func_800D91A0 (8260 bytes)
 * Garage/car customization
 */
void func_800D91A0(void *garage) {
    /* Garage - stub */
}

/*
 * func_800DB1E0 (1524 bytes)
 * Paint selection
 */
void func_800DB1E0(void *car, s32 paintId) {
    /* Paint selection - stub */
}

/*
 * func_800E114C (1012 bytes)
 * Replay system update
 */
void func_800E114C(void *replay) {
    /* Replay update - stub */
}

/*
 * func_800E15A0 (1280 bytes)
 * Replay playback
 */
void func_800E15A0(void *replay) {
    /* Replay playback - stub */
}

/*
 * func_800E1AA0 (404 bytes)
 * Replay record frame
 */
void func_800E1AA0(void *replay, void *frame) {
    /* Record frame - stub */
}

/*
 * func_800E1C30 (1908 bytes)
 * Replay camera control
 */
void func_800E1C30(void *replay, void *camera) {
    /* Replay camera - stub */
}

/*
 * func_800E23A4 (1680 bytes)
 * Replay UI
 */
void func_800E23A4(void *replay) {
    /* Replay UI - stub */
}

/*
 * func_800E0050 (1440 bytes)
 * Weather effect system
 */
void func_800E0050(void *weather) {
    /* Weather effects - stub */
}

/*
 * func_800E05F0 (1328 bytes)
 * Rain effect rendering
 */
void func_800E05F0(f32 intensity) {
    /* Rain effect - stub */
}

/*
 * func_800E0B20 (1576 bytes)
 * Particle system update
 */
void func_800E0B20(void *particles) {
    /* Particle update - stub */
}

/*
 * func_800E15A8 (1176 bytes)
 * Explosion effect
 */
void func_800E15A8(f32 *pos, f32 size) {
    /* Explosion - stub */
}

/*
 * func_800E1F80 (984 bytes)
 * Debris spawning
 */
void func_800E1F80(void *car, s32 debrisType) {
    /* Debris spawn - stub */
}

/*
 * func_800E2F00 (716 bytes)
 * Spark effect
 */
void func_800E2F00(f32 *pos, f32 *velocity) {
    /* Spark effect - stub */
}

/*
 * func_800E31D4 (248 bytes)
 * Smoke puff
 */
void func_800E31D4(f32 *pos, f32 size) {
    /* Smoke puff - stub */
}

/*
 * func_800E32CC (1112 bytes)
 * Dust cloud
 */
void func_800E32CC(f32 *pos, f32 *velocity, f32 size) {
    /* Dust cloud - stub */
}

/*
 * func_800E3724 (612 bytes)
 * Water splash
 */
void func_800E3724(f32 *pos, f32 size) {
    /* Water splash - stub */
}

/*
 * func_800E398C (2960 bytes)
 * Particle render
 */
void func_800E398C(void *particles) {
    /* Particle render - stub */
}

/*
 * func_800E451C (1596 bytes)
 * Lens flare effect
 */
void func_800E451C(void *camera, f32 *sunPos) {
    /* Lens flare - stub */
}

/*
 * func_800E4B58 (2284 bytes)
 * Sky rendering
 */
void func_800E4B58(void *camera) {
    /* Sky render - stub */
}

/*
 * func_800E5444 (712 bytes)
 * Horizon line
 */
void func_800E5444(void *camera) {
    /* Horizon - stub */
}

/*
 * func_800E571C (4360 bytes)
 * Environment mapping
 */
void func_800E571C(void *surface) {
    /* Env mapping - stub */
}

/*
 * func_800E6824 (708 bytes)
 * Reflection update
 */
void func_800E6824(void *surface) {
    /* Reflection - stub */
}

/*
 * func_800E6AF8 (1596 bytes)
 * Shadow volume
 */
void func_800E6AF8(void *object, f32 *lightDir) {
    /* Shadow volume - stub */
}

/*
 * func_800E7134 (2576 bytes)
 * Crowd rendering
 */
void func_800E7134(void *crowd) {
    /* Crowd render - stub */
}

/*
 * func_800E7B44 (472 bytes)
 * Billboard sprite
 */
void func_800E7B44(f32 *pos, s32 spriteId) {
    /* Billboard - stub */
}

/*
 * func_800E7D14 (684 bytes)
 * Animated billboard
 */
void func_800E7D14(f32 *pos, s32 animId, f32 time) {
    /* Animated billboard - stub */
}

/*
 * func_800E7FC8 (3312 bytes)
 * Track decoration rendering
 */
void func_800E7FC8(void *track) {
    /* Track decoration - stub */
}

/*
 * func_800E8CB8 (152 bytes)
 * Get decoration count
 */
s32 func_800E8CB8(void *track) {
    return *(s32 *)((u8 *)track + 0x24);
}

/*
 * func_800E8D50 (448 bytes)
 * LOD distance check
 */
s32 func_800E8D50(f32 *pos, f32 *camera, f32 maxDist) {
    /* LOD check - stub */
    return 0;
}

/*
 * func_800E8F10 (952 bytes)
 * Ambient light setup
 */
void func_800E8F10(f32 *color, f32 intensity) {
    /* Ambient light - stub */
}

/*
 * func_800E92C8 (788 bytes)
 * Directional light
 */
void func_800E92C8(f32 *direction, f32 *color, f32 intensity) {
    /* Directional light - stub */
}

/*
 * func_800E95DC (1684 bytes)
 * Point light
 */
void func_800E95DC(f32 *pos, f32 *color, f32 radius) {
    /* Point light - stub */
}

/*
 * func_800E9C70 (444 bytes)
 * Light fade
 */
void func_800E9C70(void *light, f32 targetIntensity, f32 duration) {
    /* Light fade - stub */
}

/*
 * func_800E9E2C (476 bytes)
 * Light flicker
 */
void func_800E9E2C(void *light, f32 frequency, f32 amplitude) {
    /* Light flicker - stub */
}

/*
 * func_800EA108 (468 bytes)
 * Headlight cone
 */
void func_800EA108(void *car, f32 *direction) {
    /* Headlight cone - stub */
}

/*
 * func_800EA2DC (276 bytes)
 * Light enable/disable
 */
void func_800EA2DC(void *light, s32 enable) {
    /* Light enable - stub */
}

/*
 * func_800EA3F4 (2548 bytes)
 * Full lighting update
 */
void func_800EA3F4(void *scene) {
    /* Full lighting - stub */
}

/*
 * func_800F0050 (1644 bytes)
 * Network message send
 */
void func_800F0050(void *msg, s32 size) {
    /* Net send - stub */
}

/*
 * func_800F0698 (1408 bytes)
 * Network message receive
 */
s32 func_800F0698(void *buffer, s32 maxSize) {
    /* Net receive - stub */
    return 0;
}

/*
 * func_800F0C18 (2036 bytes)
 * Network state sync
 */
void func_800F0C18(void *state) {
    /* State sync - stub */
}

/*
 * func_800F13F0 (1364 bytes)
 * Multiplayer lobby
 */
void func_800F13F0(void *lobby) {
    /* Lobby - stub */
}

/*
 * func_800F1944 (852 bytes)
 * Player join handling
 */
void func_800F1944(s32 playerSlot) {
    /* Player join - stub */
}

/*
 * func_800F1C98 (676 bytes)
 * Player leave handling
 */
void func_800F1C98(s32 playerSlot) {
    /* Player leave - stub */
}

/*
 * func_800F1F3C (2208 bytes)
 * Network game start
 */
void func_800F1F3C(void) {
    /* Net game start - stub */
}

/*
 * func_800F27DC (1632 bytes)
 * Input sync
 */
void func_800F27DC(void *inputs) {
    /* Input sync - stub */
}

/*
 * func_800F2E3C (784 bytes)
 * Latency compensation
 */
void func_800F2E3C(void *entity, s32 frames) {
    /* Latency comp - stub */
}

/*
 * func_800F314C (1084 bytes)
 * Network error handling
 */
void func_800F314C(s32 errorCode) {
    /* Net error - stub */
}

/*
 * func_800F3588 (836 bytes)
 * Session management
 */
void func_800F3588(s32 cmd) {
    /* Session mgmt - stub */
}

/*
 * func_800F38BC (1912 bytes)
 * Ghost data recording
 */
void func_800F38BC(void *ghost) {
    /* Ghost record - stub */
}

/*
 * func_800F4034 (1536 bytes)
 * Ghost playback
 */
void func_800F4034(void *ghost) {
    /* Ghost playback - stub */
}

/*
 * func_800F4634 (908 bytes)
 * Ghost save
 */
void func_800F4634(void *ghost, s32 slot) {
    /* Ghost save - stub */
}

/*
 * func_800F49C0 (876 bytes)
 * Ghost load
 */
s32 func_800F49C0(void *ghost, s32 slot) {
    /* Ghost load - stub */
    return 0;
}

/*
 * func_800F4D2C (752 bytes)
 * Ghost render
 */
void func_800F4D2C(void *ghost) {
    /* Ghost render - stub */
}

/*
 * func_800F5000 (3832 bytes)
 * Stunt system
 */
void func_800F5000(void *car) {
    /* Stunt system - stub */
}

/*
 * func_800F5EF8 (588 bytes)
 * Stunt score
 */
s32 func_800F5EF8(s32 stuntType) {
    /* Stunt score - stub */
    return 0;
}

/*
 * func_800F6144 (1888 bytes)
 * Stunt combo
 */
void func_800F6144(void *car, s32 stuntType) {
    /* Stunt combo - stub */
}

/*
 * func_800F6894 (580 bytes)
 * Wing deploy
 */
void func_800F6894(void *car, s32 deploy) {
    /* Wing deploy - stub */
}

/*
 * func_800F6AD8 (740 bytes)
 * Trick detection
 */
s32 func_800F6AD8(void *car) {
    /* Trick detection - stub */
    return 0;
}

/*
 * func_800F6DBC (1168 bytes)
 * Landing detection
 */
s32 func_800F6DBC(void *car) {
    /* Landing detection - stub */
    return 0;
}

/*
 * func_800F724C (252 bytes)
 * Stunt multiplier
 */
f32 func_800F724C(void *car) {
    /* Stunt multiplier - stub */
    return 1.0f;
}

/*
 * func_800F9428 (1604 bytes)
 * Attract mode camera
 */
void func_800F9428(void *camera) {
    /* Attract camera - stub */
}

/*
 * func_800F9A74 (952 bytes)
 * Demo playback
 */
void func_800F9A74(void *demo) {
    /* Demo playback - stub */
}

/*
 * func_800F9E2C (684 bytes)
 * Title screen
 */
void func_800F9E2C(void) {
    /* Title screen - stub */
}

/*
 * func_800FA0D8 (2356 bytes)
 * Credits display
 */
void func_800FA0D8(void) {
    /* Credits - stub */
}

/*
 * func_800FA9E4 (1296 bytes)
 * Loading screen
 */
void func_800FA9E4(f32 progress) {
    /* Loading screen - stub */
}

/*
 * func_800FAEF4 (1808 bytes)
 * Pause menu
 */
void func_800FAEF4(void *pause) {
    /* Pause menu - stub */
}

/*
 * func_800FB5F4 (820 bytes)
 * Pause state toggle
 */
void func_800FB5F4(s32 pause) {
    /* Pause toggle - stub */
}

/*
 * func_800FB928 (712 bytes)
 * Game timer update
 */
void func_800FB928(void) {
    /* Timer update - stub */
}

/*
 * func_800FC3D8 (1516 bytes)
 * Bonus mode
 */
void func_800FC3D8(void *bonus) {
    /* Bonus mode - stub */
}

/*
 * func_800FC9B8 (1284 bytes)
 * Unlock check
 */
s32 func_800FC9B8(s32 unlockId) {
    /* Unlock check - stub */
    return 0;
}

/*
 * func_800FCEB0 (948 bytes)
 * Unlock trigger
 */
void func_800FCEB0(s32 unlockId) {
    /* Unlock trigger - stub */
}

/*
 * func_800FD264 (512 bytes)
 * Progress save
 */
void func_800FD264(void) {
    /* Progress save - stub */
}

/*
 * func_800FDA90 (4560 bytes)
 * Race init
 */
void func_800FDA90(void *race) {
    /* Race init - stub */
}

/*
 * func_800FEC78 (1808 bytes)
 * Race cleanup
 */
void func_800FEC78(void) {
    /* Race cleanup - stub */
}

/*
 * func_80087118 (1772 bytes)
 * Major RDP render mode setup
 */
void func_80087118(s32 mode, s32 flags) {
    u32 *dlPtr;
    u32 currentMode;

    dlPtr = *(u32 **)(0x80149438);

    /* Set render mode based on flags */
    currentMode = *(u32 *)(0x8013E60C);

    /* Complex render mode setup - stub */
}

/*
 * func_80087A08 (10048 bytes)
 * Major object rendering function
 */
void func_80087A08(void *object, void *matrix) {
    /* Large render function - stub */
}

/*
 * func_8008A77C (676 bytes)
 * Audio queue processing
 */
void func_8008A77C(void *queue) {
    /* Audio queue - stub */
}

/*
 * func_8009C8F0 (5368 bytes)
 * Track geometry processing
 */
void func_8009C8F0(void *track) {
    /* Track geometry - stub */
}

/*
 * func_8009DD88 (3304 bytes)
 * Track segment render
 */
void func_8009DD88(void *segment) {
    /* Segment render - stub */
}

/*
 * func_8009EA70 (168 bytes)
 * Track bounds check
 */
s32 func_8009EA70(f32 *pos) {
    /* Bounds check - stub */
    return 0;
}

/*
 * func_8009EB18 (168 bytes)
 * Track height query
 */
f32 func_8009EB18(f32 x, f32 z) {
    /* Height query - stub */
    return 0.0f;
}

/*
 * func_8009EBC0 (1188 bytes)
 * Track surface type query
 */
s32 func_8009EBC0(f32 *pos) {
    /* Surface type - stub */
    return 0;
}

/*
 * func_8009F064 (8600 bytes)
 * Track collision test
 */
s32 func_8009F064(f32 *start, f32 *end, f32 *hitPoint) {
    /* Collision test - stub */
    return 0;
}

/*
 * func_800AF06C (1396 bytes)
 * Save game data
 */
void func_800AF06C(void *saveData) {
    /* Save game - stub */
}

/*
 * func_800AF5E0 (176 bytes)
 * Save slot check
 */
s32 func_800AF5E0(s32 slot) {
    /* Slot check - stub */
    return 0;
}

/*
 * func_800AF690 (572 bytes)
 * Load game data
 */
s32 func_800AF690(void *saveData, s32 slot) {
    /* Load game - stub */
    return 0;
}

/*
 * func_800AF8CC (620 bytes)
 * Delete save
 */
void func_800AF8CC(s32 slot) {
    /* Delete save - stub */
}

/*
 * func_800AFB38 (548 bytes)
 * Save validation
 */
s32 func_800AFB38(void *saveData) {
    /* Validate save - stub */
    return 1;
}

/*
 * func_800AFD5C (1060 bytes)
 * Controller pak init
 */
s32 func_800AFD5C(s32 controller) {
    /* Pak init - stub */
    return 0;
}

/*
 * func_800B0180 (868 bytes)
 * Controller pak read
 */
s32 func_800B0180(s32 controller, void *buffer, s32 offset, s32 size) {
    /* Pak read - stub */
    return 0;
}

/*
 * func_800B0904 (396 bytes)
 * Audio init subsystem
 */
void func_800B0904(void) {
    /* Audio init - stub */
}

/*
 * func_800B0A90 (440 bytes)
 * Audio shutdown
 */
void func_800B0A90(void) {
    /* Audio shutdown - stub */
}

/*
 * func_800B0C48 (800 bytes)
 * Audio frame update
 */
void func_800B0C48(void) {
    /* Audio update - stub */
}

/*
 * func_800B0F68 (364 bytes)
 * Sound bank load
 */
void func_800B0F68(s32 bankId) {
    /* Bank load - stub */
}

/*
 * func_800B10D4 (1248 bytes)
 * Sound bank unload
 */
void func_800B10D4(s32 bankId) {
    /* Bank unload - stub */
}

/*
 * func_800B15B4 (1428 bytes)
 * Music sequence load
 */
void func_800B15B4(s32 seqId) {
    /* Sequence load - stub */
}

/*
 * func_800B1B48 (1020 bytes)
 * Music playback control
 */
void func_800B1B48(s32 cmd, s32 param) {
    /* Music control - stub */
}

/*
 * func_800B1F44 (200 bytes)
 * Music volume
 */
void func_800B1F44(f32 volume) {
    /* Music volume - stub */
}

/*
 * func_800B200C (1612 bytes)
 * Music tempo
 */
void func_800B200C(f32 tempo) {
    /* Music tempo - stub */
}

/*
 * func_800B2658 (464 bytes)
 * Sound effect play
 */
void func_800B2658(s32 soundId, f32 volume, f32 pan) {
    /* SFX play - stub */
}

/*
 * func_800B2828 (256 bytes)
 * Sound effect stop
 */
void func_800B2828(s32 handle) {
    /* SFX stop - stub */
}

/*
 * func_800B2928 (1016 bytes)
 * 3D sound position
 */
void func_800B2928(s32 handle, f32 *pos) {
    /* 3D sound pos - stub */
}

/*
 * func_800B2D20 (216 bytes)
 * Sound listener position
 */
void func_800B2D20(f32 *pos, f32 *forward) {
    /* Listener pos - stub */
}

/*
 * func_800B2DF8 (1428 bytes)
 * Engine sound update
 */
void func_800B2DF8(void *car) {
    /* Engine sound - stub */
}

/*
 * func_800B338C (900 bytes)
 * Tire sound update
 */
void func_800B338C(void *car) {
    /* Tire sound - stub */
}

/*
 * func_800B3710 (684 bytes)
 * Collision sound trigger
 */
void func_800B3710(void *car, f32 intensity) {
    /* Collision sound - stub */
}

/*
 * func_800B39BC (396 bytes)
 * Wind sound update
 */
void func_800B39BC(void *car) {
    /* Wind sound - stub */
}

/*
 * func_800B3B4C (468 bytes)
 * Crowd cheer trigger
 */
void func_800B3B4C(s32 intensity) {
    /* Crowd cheer - stub */
}

/*
 * func_800B3D20 (488 bytes)
 * Ambient sound control
 */
void func_800B3D20(s32 ambientId, f32 volume) {
    /* Ambient sound - stub */
}

/*
 * func_800B4208 (1328 bytes)
 * Voice playback
 */
void func_800B4208(s32 voiceId) {
    /* Voice playback - stub */
}

/*
 * func_800B4738 (224 bytes)
 * Voice stop
 */
void func_800B4738(void) {
    /* Voice stop - stub */
}

/*
 * func_800B4818 (892 bytes)
 * Audio bus mix
 */
void func_800B4818(s32 bus, f32 *levels) {
    /* Bus mix - stub */
}

/*
 * func_800B4B94 (536 bytes)
 * Reverb setup
 */
void func_800B4B94(s32 reverbType, f32 amount) {
    /* Reverb setup - stub */
}

/*
 * func_800B4DAC (196 bytes)
 * Audio pause
 */
void func_800B4DAC(s32 pause) {
    /* Audio pause - stub */
}

/*
 * func_800B4E70 (2108 bytes)
 * Audio ducking
 */
void func_800B4E70(s32 priority) {
    /* Audio ducking - stub */
}

/*
 * func_800B5694 (184 bytes)
 * Sound priority set
 */
void func_800B5694(s32 handle, s32 priority) {
    /* Priority set - stub */
}

/*
 * func_800B574C (340 bytes)
 * Sound loop control
 */
void func_800B574C(s32 handle, s32 loop) {
    /* Loop control - stub */
}

/*
 * func_800B58A0 (168 bytes)
 * Sound pitch set
 */
void func_800B58A0(s32 handle, f32 pitch) {
    /* Pitch set - stub */
}

/*
 * func_800B5948 (176 bytes)
 * Sound volume set
 */
void func_800B5948(s32 handle, f32 volume) {
    /* Volume set - stub */
}

/*
 * func_800B59F8 (1472 bytes)
 * Audio spatialization
 */
void func_800B59F8(s32 handle, f32 *pos, f32 *velocity) {
    /* Spatialization - stub */
}

/*
 * func_800B65B8 (464 bytes)
 * Audio distance attenuation
 */
f32 func_800B65B8(f32 distance, f32 maxDist) {
    /* Attenuation - stub */
    return 1.0f;
}

/*
 * func_800B6788 (1124 bytes)
 * Audio doppler effect
 */
f32 func_800B6788(f32 *listenerPos, f32 *listenerVel, f32 *sourcePos, f32 *sourceVel) {
    /* Doppler - stub */
    return 1.0f;
}

/*
 * func_800B6BEC (1520 bytes)
 * Audio occlusion
 */
f32 func_800B6BEC(f32 *listenerPos, f32 *sourcePos) {
    /* Occlusion - stub */
    return 1.0f;
}

/*
 * func_800B71DC (536 bytes)
 * Entity audio update
 */
void func_800B71DC(void *entity) {
    /* Entity audio - stub */
}

/*
 * func_800BC3E0 (7640 bytes)
 * Camera main update
 *
 * Based on arcade camera.c - updates camera position/orientation to follow target
 * Camera structure offsets:
 *   0x00: position (f32[3])
 *   0x0C: target position (f32[3])
 *   0x18: look direction (f32[3])
 *   0x24: up vector (f32[3])
 *   0x30: right vector (f32[3])
 *   0x3C: fov (f32)
 *   0x40: near plane (f32)
 *   0x44: far plane (f32)
 *   0x48: mode (s32)
 *   0x4C: shake intensity (f32)
 *   0x50: distance (f32)
 *   0x54: yaw (f32)
 *   0x58: pitch (f32)
 */
void func_800BC3E0(void *camera, void *target) {
    f32 *camPos, *camTarget, *camLook, *camUp;
    f32 *targetPos, *targetVel;
    f32 dx, dy, dz, dist, invDist;
    f32 lookAhead[3];
    f32 elasticity;
    s32 mode;

    if (camera == NULL || target == NULL) {
        return;
    }

    /* Get camera components */
    camPos = (f32 *)camera;
    camTarget = (f32 *)((u8 *)camera + 0x0C);
    camLook = (f32 *)((u8 *)camera + 0x18);
    camUp = (f32 *)((u8 *)camera + 0x24);
    mode = *(s32 *)((u8 *)camera + 0x48);

    /* Get target position and velocity */
    targetPos = (f32 *)((u8 *)target + 0x24);
    targetVel = (f32 *)((u8 *)target + 0x34);

    /* Elasticity based on mode (0 = rigid, 1 = smooth) */
    elasticity = 0.85f;

    /* Calculate look-ahead position based on velocity */
    lookAhead[0] = targetPos[0] + targetVel[0] * 0.5f;
    lookAhead[1] = targetPos[1];
    lookAhead[2] = targetPos[2] + targetVel[2] * 0.5f;

    /* Smooth interpolate camera target */
    camTarget[0] = camTarget[0] * elasticity + lookAhead[0] * (1.0f - elasticity);
    camTarget[1] = camTarget[1] * elasticity + lookAhead[1] * (1.0f - elasticity);
    camTarget[2] = camTarget[2] * elasticity + lookAhead[2] * (1.0f - elasticity);

    /* Calculate look direction */
    dx = camTarget[0] - camPos[0];
    dy = camTarget[1] - camPos[1];
    dz = camTarget[2] - camPos[2];
    dist = sqrtf(dx * dx + dy * dy + dz * dz);

    if (dist > 0.001f) {
        invDist = 1.0f / dist;
        camLook[0] = dx * invDist;
        camLook[1] = dy * invDist;
        camLook[2] = dz * invDist;
    }

    /* Update up vector (keep mostly vertical) */
    camUp[0] = 0.0f;
    camUp[1] = 1.0f;
    camUp[2] = 0.0f;

    /* Check for collision and apply shake */
    func_800BDE78(camera);
}

/*
 * func_800BDE78 (1596 bytes)
 * Camera collision avoidance
 *
 * Prevents camera from clipping through geometry
 */
void func_800BDE78(void *camera) {
    f32 *camPos, *camTarget;
    f32 dx, dy, dz, dist;
    f32 minDist;

    if (camera == NULL) {
        return;
    }

    camPos = (f32 *)camera;
    camTarget = (f32 *)((u8 *)camera + 0x0C);

    /* Calculate distance to target */
    dx = camTarget[0] - camPos[0];
    dy = camTarget[1] - camPos[1];
    dz = camTarget[2] - camPos[2];
    dist = sqrtf(dx * dx + dy * dy + dz * dz);

    /* Minimum distance check */
    minDist = 2.0f;
    if (dist < minDist && dist > 0.001f) {
        /* Push camera back */
        f32 scale = minDist / dist;
        camPos[0] = camTarget[0] - dx * scale;
        camPos[1] = camTarget[1] - dy * scale;
        camPos[2] = camTarget[2] - dz * scale;
    }

    /* Keep camera above ground (simple Y check) */
    if (camPos[1] < 1.0f) {
        camPos[1] = 1.0f;
    }
}

/*
 * func_800BE44C (872 bytes)
 * Camera smooth follow
 *
 * Smoothly interpolates camera position toward target
 */
void func_800BE44C(void *camera, f32 *targetPos, f32 smoothing) {
    f32 *camPos;
    f32 t;

    if (camera == NULL || targetPos == NULL) {
        return;
    }

    camPos = (f32 *)camera;

    /* Clamp smoothing factor */
    if (smoothing < 0.0f) smoothing = 0.0f;
    if (smoothing > 1.0f) smoothing = 1.0f;
    t = 1.0f - smoothing;

    /* Lerp position */
    camPos[0] = camPos[0] * smoothing + targetPos[0] * t;
    camPos[1] = camPos[1] * smoothing + targetPos[1] * t;
    camPos[2] = camPos[2] * smoothing + targetPos[2] * t;
}

/*
 * func_800BEC9C (1156 bytes)
 * Camera mode switch
 *
 * Switches between camera modes (chase, first-person, orbit, etc.)
 * Mode values: 0=chase, 1=first-person, 2=orbit, 3=cinematic
 */
void func_800BEC9C(void *camera, s32 mode) {
    s32 *camMode;
    f32 *distance, *yaw, *pitch;

    if (camera == NULL) {
        return;
    }

    camMode = (s32 *)((u8 *)camera + 0x48);
    distance = (f32 *)((u8 *)camera + 0x50);
    yaw = (f32 *)((u8 *)camera + 0x54);
    pitch = (f32 *)((u8 *)camera + 0x58);

    *camMode = mode;

    /* Set default values based on mode */
    switch (mode) {
        case 0:  /* Chase cam */
            *distance = 8.0f;
            *pitch = 0.2f;  /* Slight look-down angle */
            break;

        case 1:  /* First person */
            *distance = 0.0f;
            *pitch = 0.0f;
            break;

        case 2:  /* Orbit */
            *distance = 12.0f;
            break;

        case 3:  /* Cinematic */
            *distance = 20.0f;
            break;

        default:
            break;
    }
}

/*
 * func_800BF120 (1780 bytes)
 * Camera orbit control
 *
 * Orbits camera around target at given yaw/pitch angles
 */
void func_800BF120(void *camera, f32 yaw, f32 pitch) {
    f32 *camPos, *camTarget;
    f32 *storedYaw, *storedPitch, *distance;
    f32 sinYaw, cosYaw, sinPitch, cosPitch;
    f32 x, y, z;

    if (camera == NULL) {
        return;
    }

    camPos = (f32 *)camera;
    camTarget = (f32 *)((u8 *)camera + 0x0C);
    storedYaw = (f32 *)((u8 *)camera + 0x54);
    storedPitch = (f32 *)((u8 *)camera + 0x58);
    distance = (f32 *)((u8 *)camera + 0x50);

    /* Update stored angles */
    *storedYaw = yaw;
    *storedPitch = pitch;

    /* Clamp pitch to avoid gimbal lock */
    if (pitch > 1.4f) pitch = 1.4f;
    if (pitch < -1.4f) pitch = -1.4f;

    /* Calculate orbital position */
    sinYaw = sinf(yaw);
    cosYaw = cosf(yaw);
    sinPitch = sinf(pitch);
    cosPitch = cosf(pitch);

    x = *distance * cosPitch * sinYaw;
    y = *distance * sinPitch;
    z = *distance * cosPitch * cosYaw;

    /* Set camera position relative to target */
    camPos[0] = camTarget[0] + x;
    camPos[1] = camTarget[1] + y;
    camPos[2] = camTarget[2] + z;
}

/*
 * func_800BF814 (948 bytes)
 * Camera zoom control
 *
 * Adjusts camera distance from target
 */
void func_800BF814(void *camera, f32 zoom) {
    f32 *distance;
    f32 minDist, maxDist;

    if (camera == NULL) {
        return;
    }

    distance = (f32 *)((u8 *)camera + 0x50);

    /* Clamp zoom distance */
    minDist = 2.0f;
    maxDist = 50.0f;

    *distance = zoom;
    if (*distance < minDist) *distance = minDist;
    if (*distance > maxDist) *distance = maxDist;
}

/*
 * func_800BFBC8 (420 bytes)
 * Camera FOV set
 *
 * Sets camera field of view in radians
 */
void func_800BFBC8(void *camera, f32 fov) {
    f32 *camFov;
    f32 minFov, maxFov;

    if (camera == NULL) {
        return;
    }

    camFov = (f32 *)((u8 *)camera + 0x3C);

    /* Clamp FOV (typical range: 30-120 degrees, in radians) */
    minFov = 0.5f;   /* ~30 degrees */
    maxFov = 2.1f;   /* ~120 degrees */

    *camFov = fov;
    if (*camFov < minFov) *camFov = minFov;
    if (*camFov > maxFov) *camFov = maxFov;
}

/*
 * func_800BFD6C (1392 bytes)
 * Camera look-at
 *
 * Points camera at target position, calculating look direction
 */
void func_800BFD6C(void *camera, f32 *target) {
    f32 *camPos, *camLook, *camUp, *camRight;
    f32 dx, dy, dz, dist, invDist;
    f32 rightX, rightZ, rightLen;

    if (camera == NULL || target == NULL) {
        return;
    }

    camPos = (f32 *)camera;
    camLook = (f32 *)((u8 *)camera + 0x18);
    camUp = (f32 *)((u8 *)camera + 0x24);
    camRight = (f32 *)((u8 *)camera + 0x30);

    /* Calculate look direction */
    dx = target[0] - camPos[0];
    dy = target[1] - camPos[1];
    dz = target[2] - camPos[2];
    dist = sqrtf(dx * dx + dy * dy + dz * dz);

    if (dist < 0.001f) {
        return;  /* Target at camera position */
    }

    invDist = 1.0f / dist;
    camLook[0] = dx * invDist;
    camLook[1] = dy * invDist;
    camLook[2] = dz * invDist;

    /* Calculate right vector (cross product: up x look) */
    /* Assuming world up is (0, 1, 0) */
    rightX = camLook[2];
    rightZ = -camLook[0];
    rightLen = sqrtf(rightX * rightX + rightZ * rightZ);

    if (rightLen > 0.001f) {
        camRight[0] = rightX / rightLen;
        camRight[1] = 0.0f;
        camRight[2] = rightZ / rightLen;
    }

    /* Calculate up vector (cross product: look x right) */
    camUp[0] = camLook[1] * camRight[2] - camLook[2] * camRight[1];
    camUp[1] = camLook[2] * camRight[0] - camLook[0] * camRight[2];
    camUp[2] = camLook[0] * camRight[1] - camLook[1] * camRight[0];
}

/*
 * func_800C0288 (2124 bytes)
 * Camera path follow
 *
 * Follows a predefined camera path using interpolation
 * Path structure: array of control points with position and look-at
 */
void func_800C0288(void *camera, void *path, f32 t) {
    f32 *camPos, *camTarget;
    f32 *pathData;
    s32 numPoints, idx0, idx1;
    f32 localT;

    if (camera == NULL || path == NULL) {
        return;
    }

    camPos = (f32 *)camera;
    camTarget = (f32 *)((u8 *)camera + 0x0C);

    /* Path format: [numPoints, pos0[3], look0[3], pos1[3], look1[3], ...] */
    pathData = (f32 *)path;
    numPoints = (s32)pathData[0];

    if (numPoints < 2) {
        return;
    }

    /* Clamp t to [0, 1] */
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    /* Find segment indices */
    idx0 = (s32)(t * (numPoints - 1));
    idx1 = idx0 + 1;
    if (idx1 >= numPoints) {
        idx1 = numPoints - 1;
        idx0 = idx1 - 1;
    }

    /* Local t within segment */
    localT = t * (numPoints - 1) - idx0;

    /* Interpolate position (points start at offset 1, stride = 6 floats) */
    {
        f32 *p0 = &pathData[1 + idx0 * 6];
        f32 *p1 = &pathData[1 + idx1 * 6];

        camPos[0] = p0[0] + (p1[0] - p0[0]) * localT;
        camPos[1] = p0[1] + (p1[1] - p0[1]) * localT;
        camPos[2] = p0[2] + (p1[2] - p0[2]) * localT;

        camTarget[0] = p0[3] + (p1[3] - p0[3]) * localT;
        camTarget[1] = p0[4] + (p1[4] - p0[4]) * localT;
        camTarget[2] = p0[5] + (p1[5] - p0[5]) * localT;
    }

    /* Update look direction */
    func_800BFD6C(camera, camTarget);
}

/*
 * func_800C0AC4 (1736 bytes)
 * Camera transition
 *
 * Blends between two camera states over time
 */
void func_800C0AC4(void *camera, void *targetCamera, f32 duration) {
    /* Transition - stub */
}

/*
 * func_800C1188 (876 bytes)
 * Camera matrix build
 */
void func_800C1188(void *camera, f32 *matrix) {
    /* Matrix build - stub */
}

/*
 * func_800C14F4 (768 bytes)
 * Camera frustum extract
 */
void func_800C14F4(void *camera, f32 *frustum) {
    /* Frustum extract - stub */
}

/*
 * func_800C17F4 (1224 bytes)
 * Camera viewport setup
 */
void func_800C17F4(void *camera, s32 x, s32 y, s32 w, s32 h) {
    /* Viewport setup - stub */
}

/*
 * func_800C1CBC (2392 bytes)
 * Split screen camera
 */
void func_800C1CBC(s32 playerCount) {
    /* Split screen - stub */
}

/*
 * func_800C2614 (1092 bytes)
 * Rear view camera
 */
void func_800C2614(void *camera, void *car) {
    /* Rear view - stub */
}

/*
 * func_800C2A58 (1648 bytes)
 * Cinematic camera
 */
void func_800C2A58(void *camera, void *scene) {
    /* Cinematic - stub */
}

/*
 * func_800C30D8 (1212 bytes)
 * Finish line camera
 */
void func_800C30D8(void *camera) {
    /* Finish camera - stub */
}

/*
/*
 * func_800CC880 (3972 bytes)
 * Menu animation
 */
void func_800CC880(void *menu) {
    /* Menu animation - stub */
}

/*
 * func_800CD7F8 (7960 bytes)
 * Text rendering
 */
void func_800CD7F8(void *text, s32 x, s32 y) {
    /* Text render - stub */
}

/*
 * func_800CF7E0 (1792 bytes)
 * Font loading
 */
void func_800CF7E0(s32 fontId) {
    /* Font load - stub */
}

/*
 * func_800CFEDC (892 bytes)
 * String width calculation
 */
s32 func_800CFEDC(u8 *str) {
    /* String width - stub */
    return 0;
}

/*
 * func_800D0258 (460 bytes)
 * Text color set
 */
void func_800D0258(u8 r, u8 g, u8 b, u8 a) {
    /* Text color - stub */
}

/*
 * func_800D23A8 (15548 bytes)
 * Full menu render
 */
void func_800D23A8(void *menu) {
    /* Full menu render - stub */
}

/*
 * func_800DC8D0 (1100 bytes)
 * Button sprite render
 */
void func_800DC8D0(s32 buttonId, s32 x, s32 y) {
    /* Button render - stub */
}

/*
 * func_800DCD20 (1760 bytes)
 * Icon sprite render
 */
void func_800DCD20(s32 iconId, s32 x, s32 y) {
    /* Icon render - stub */
}

/*
 * func_800DD410 (2340 bytes)
 * Selection highlight
 */
void func_800DD410(s32 x, s32 y, s32 w, s32 h) {
    /* Highlight - stub */
}

/*
 * func_800DDD40 (1284 bytes)
 * Menu scroll
 */
void func_800DDD40(void *menu, s32 direction) {
    /* Menu scroll - stub */
}

/*
 * func_800DE244 (1824 bytes)
 * Menu transition effect
 */
void func_800DE244(void *menu, s32 transitionType) {
    /* Menu transition - stub */
}

/*
 * func_800DE960 (2128 bytes)
 * Popup dialog
 */
void func_800DE960(u8 *message, s32 type) {
    /* Popup dialog - stub */
}

/*
 * func_800DF1B0 (1356 bytes)
 * Confirmation dialog
 */
s32 func_800DF1B0(u8 *message) {
    /* Confirmation - stub */
    return 0;
}

/*
 * func_800DF6FC (2064 bytes)
 * Input prompt
 */
void func_800DF6FC(u8 *prompt, u8 *buffer, s32 maxLen) {
    /* Input prompt - stub */
}

/*
 * func_800DFF0C (1600 bytes)
 * Virtual keyboard
 */
void func_800DFF0C(u8 *buffer, s32 maxLen) {
    /* Virtual keyboard - stub */
}

/*
 * func_800E051C (212 bytes)
 * Keyboard cursor move
 */
void func_800E051C(s32 dx, s32 dy) {
    /* Cursor move - stub */
}

/*
 * func_800EAE90 (2720 bytes)
 * World bounds setup
 */
void func_800EAE90(void *world) {
    /* World bounds - stub */
}

/*
 * func_800EB920 (1012 bytes)
 * Trigger zone check
 */
s32 func_800EB920(void *entity, void *trigger) {
    /* Trigger check - stub */
    return 0;
}

/*
 * func_800EBD14 (1708 bytes)
 * Trigger zone callback
 */
void func_800EBD14(void *trigger, void *callback) {
    /* Trigger callback - stub */
}

/*
 * func_800EC3B0 (2948 bytes)
 * Scripted event
 */
void func_800EC3B0(void *event) {
    /* Scripted event - stub */
}

/*
 * func_800ECF38 (2188 bytes)
 * Cutscene playback
 */
void func_800ECF38(s32 cutsceneId) {
    /* Cutscene - stub */
}

/*
 * func_800ED7C0 (2564 bytes)
 * Object spawner
 */
void func_800ED7C0(s32 objectType, f32 *pos, f32 *rot) {
    /* Object spawn - stub */
}

/*
 * func_800EE1B0 (1516 bytes)
 * Collectible spawn
 */
void func_800EE1B0(s32 collectibleType, f32 *pos) {
    /* Collectible spawn - stub */
}

/*
 * func_800EE7A8 (228 bytes)
 * Collectible collect
 */
void func_800EE7A8(void *player, void *collectible) {
    /* Collect - stub */
}

/*
 * func_800EE8B0 (1780 bytes)
 * Power-up activation
 */
void func_800EE8B0(void *player, s32 powerupType) {
    /* Power-up - stub */
}

/*
 * func_800EEFA0 (1556 bytes)
 * Power-up timer
 */
void func_800EEFA0(void *player) {
    /* Power-up timer - stub */
}

/*
 * func_800EF5B4 (2036 bytes)
 * Nitro boost
 */
void func_800EF5B4(void *car) {
    /* Nitro boost - stub */
}

/*
 * func_800EFD88 (1232 bytes)
 * Nitro refill
 */
void func_800EFD88(void *car, f32 amount) {
    /* Nitro refill - stub */
}

/*
 * func_800F0258 (1088 bytes)
 * Boost pad trigger
 */
void func_800F0258(void *car, void *pad) {
    /* Boost pad - stub */
}

/*
/*
 * func_800F0B44 (2264 bytes)
 * Ramp launch
 */
void func_800F0B44(void *car, void *ramp) {
    /* Ramp launch - stub */
}

/*
 * func_80100660 (1748 bytes)
 * Timer init
 */
void func_80100660(void) {
    /* Timer init - stub */
}

/*
 * func_80100D34 (188 bytes)
 * Timer get elapsed
 */
s32 func_80100D34(void) {
    /* Get elapsed - stub */
    return 0;
}

/*
 * func_80100DF0 (244 bytes)
 * Timer reset
 */
void func_80100DF0(void) {
    /* Timer reset - stub */
}

/*
 * func_80100EE4 (776 bytes)
 * Countdown timer
 */
void func_80100EE4(s32 seconds) {
    /* Countdown - stub */
}

/*
 * func_801011DC (1340 bytes)
 * Lap timer split
 */
void func_801011DC(void *player) {
    /* Lap split - stub */
}

/*
 * func_80101700 (588 bytes)
 * Best time check
 */
s32 func_80101700(s32 trackId, s32 time) {
    /* Best time check - stub */
    return 0;
}

/*
 * func_8010194C (812 bytes)
 * Record new best
 */
void func_8010194C(s32 trackId, s32 time, u8 *name) {
    /* Record best - stub */
}

/*
 * func_80101C78 (1472 bytes)
 * Leaderboard display
 */
void func_80101C78(s32 trackId) {
    /* Leaderboard - stub */
}

/*
 * func_80102250 (2084 bytes)
 * Race results screen
 */
void func_80102250(void) {
    /* Results screen - stub */
}

/*
 * func_80102A74 (1524 bytes)
 * Award ceremony
 */
void func_80102A74(void) {
    /* Award ceremony - stub */
}

/*
 * func_8010306C (876 bytes)
 * Trophy animation
 */
void func_8010306C(s32 place) {
    /* Trophy animation - stub */
}

/*
 * func_801033D8 (1616 bytes)
 * Continue prompt
 */
s32 func_801033D8(void) {
    /* Continue prompt - stub */
    return 0;
}

/*
 * func_80103A08 (2328 bytes)
 * Game over screen
 */
void func_80103A08(void) {
    /* Game over - stub */
}

/*
 * func_80104320 (1844 bytes)
 * Name entry screen
 */
void func_80104320(u8 *name) {
    /* Name entry - stub */
}

/*
 * func_80104A58 (1036 bytes)
 * High score entry animation
 */
void func_80104A58(s32 position) {
    /* HS animation - stub */
}

/*
 * func_80104E84 (2464 bytes)
 * Statistics display
 */
void func_80104E84(void *stats) {
    /* Statistics - stub */
}

/*
 * func_80105858 (1692 bytes)
 * Achievements check
 */
void func_80105858(void *player) {
    /* Achievements - stub */
}

/*
 * func_80105EF4 (876 bytes)
 * Achievement unlock
 */
void func_80105EF4(s32 achievementId) {
    /* Achievement unlock - stub */
}

/*
 * func_80106260 (1484 bytes)
 * Achievement display
 */
void func_80106260(s32 achievementId) {
    /* Achievement display - stub */
}

/*
 * func_801068F4 (2144 bytes)
 * Profile stats update
 */
void func_801068F4(void *profile, void *raceStats) {
    /* Profile update - stub */
}

/*
 * func_80107110 (1248 bytes)
 * Difficulty scaling
 */
void func_80107110(s32 difficulty) {
    /* Difficulty scaling - stub */
}

/*
 * func_80107600 (924 bytes)
 * AI difficulty adjust
 */
void func_80107600(void *ai, s32 difficulty) {
    /* AI difficulty - stub */
}

/*
 * func_801079AC (1768 bytes)
 * Rubber banding
 */
void func_801079AC(void *race) {
    /* Rubber banding - stub */
}

/*
 * func_80108098 (2356 bytes)
 * Dynamic difficulty
 */
void func_80108098(void *player) {
    /* Dynamic difficulty - stub */
}

/*
 * func_801089CC (1432 bytes)
 * Catch-up logic
 */
void func_801089CC(void *ai, void *leader) {
    /* Catch-up - stub */
}

/*
 * func_80108F6C (876 bytes)
 * Skill rating update
 */
void func_80108F6C(void *player, s32 result) {
    /* Skill rating - stub */
}

/*
 * func_801092D8 (1984 bytes)
 * Matchmaking
 */
void func_801092D8(void *players) {
    /* Matchmaking - stub */
}

/*
 * func_80109A98 (1124 bytes)
 * Session host
 */
s32 func_80109A98(void) {
    /* Session host - stub */
    return 0;
}

/*
 * func_80109EFC (1476 bytes)
 * Session join
 */
s32 func_80109EFC(s32 sessionId) {
    /* Session join - stub */
    return 0;
}

/*
 * func_8010A4C0 (892 bytes)
 * Session leave
 */
void func_8010A4C0(void) {
    /* Session leave - stub */
}

/*
 * func_8010A83C (2648 bytes)
 * Network sync full
 */
void func_8010A83C(void) {
    /* Net sync - stub */
}

/*
 * func_8010B284 (676 bytes)
 * Ping measurement
 */
s32 func_8010B284(void) {
    /* Ping - stub */
    return 0;
}

/*
/*
 * func_8010B874 (1548 bytes)
 * Disconnection handling
 */
void func_8010B874(s32 playerId) {
    /* Disconnect - stub */
}

/*
 * func_8010BE7C (1152 bytes)
 * Reconnection attempt
 */
s32 func_8010BE7C(void) {
    /* Reconnect - stub */
    return 0;
}

/*
 * func_8010C2FC (2084 bytes)
 * Final cleanup
 */
void func_8010C2FC(void) {
    /* Final cleanup - stub */
}

/*
 * func_800B7440 (88 bytes)
 * Audio queue add
 */
void func_800B7440(s32 sndId) {
    extern u32 D_801547C4;
    extern s32 D_801551E8[10];
    s32 *ptr;

    if (D_801547C4 != 0) {
        func_800B73E4();
    }
    for (ptr = &D_801551E8[0]; ptr < &D_801551E8[10]; ptr++) {
        if (*ptr == 0) {
            *ptr = sndId;
            return;
        }
    }
}

/*
 * func_800B78F8 (324 bytes)
 * Audio channel setup
 */
void func_800B78F8(s32 channel, s32 param) {
    /* Channel setup - stub */
}

/*
 * func_800B7A40 (192 bytes)
 * Audio volume control
 */
void func_800B7A40(s32 channel, f32 volume) {
    /* Volume control - stub */
}

/*
 * func_800B8000 (200 bytes)
 * Audio pan control
 */
void func_800B8000(s32 channel, f32 pan) {
    /* Pan control - stub */
}

/*
 * func_800B821C (340 bytes)
 * Audio pitch control
 */
void func_800B821C(s32 channel, f32 pitch) {
    /* Pitch control - stub */
}

/*
 * func_800B8374 (724 bytes)
 * Audio 3D position
 */
void func_800B8374(s32 channel, f32 *pos) {
    /* 3D audio - stub */
}

/*
 * func_800B8650 (464 bytes)
 * Audio doppler effect
 */
void func_800B8650(s32 channel, f32 *velocity) {
    /* Doppler - stub */
}

/*
 * func_800B8820 (368 bytes)
 * Audio reverb setup
 */
void func_800B8820(s32 reverbType, f32 amount) {
    /* Reverb - stub */
}

/*
 * func_800B9194 (528 bytes)
 * Audio sample load
 */
s32 func_800B9194(s32 sampleId) {
    /* Sample load - stub */
    return 0;
}

/*
 * func_800B93A8 (972 bytes)
 * Audio stream init
 */
s32 func_800B93A8(s32 streamId) {
    /* Stream init - stub */
    return 0;
}

/*
 * func_800B9774 (664 bytes)
 * Audio stream update
 */
void func_800B9774(void) {
    /* Stream update - stub */
}

/*
 * func_800B9A0C (860 bytes)
 * Audio stream buffer
 */
void func_800B9A0C(void *buffer, s32 size) {
    /* Stream buffer - stub */
}

/*
 * func_800B9D68 (676 bytes)
 * Music sequence control
 */
void func_800B9D68(s32 seqCmd) {
    /* Sequence control - stub */
}

/*
 * func_800BA00C (732 bytes)
 * Music tempo control
 */
void func_800BA00C(f32 tempo) {
    /* Tempo - stub */
}

/*
 * func_800BA2E8 (388 bytes)
 * Music fade in/out
 */
void func_800BA2E8(f32 duration, s32 fadeIn) {
    /* Fade - stub */
}

/*
 * func_800BA46C (472 bytes)
 * Audio priority system
 */
s32 func_800BA46C(s32 priority) {
    /* Priority - stub */
    return 0;
}

/*
 * func_800BA644 (380 bytes)
 * Audio memory manager
 */
void *func_800BA644(s32 size) {
    /* Audio alloc - stub */
    return NULL;
}

/*
 * func_800BA7C4 (1996 bytes)
 * Audio main mixer
 */
void func_800BA7C4(void) {
    /* Main mixer - stub */
}

/*
 * func_800BAF98 (172 bytes)
 * Audio effect apply
 */
void func_800BAF98(s32 effectId) {
    /* Effect apply - stub */
}

/*
 * func_800BB044 (252 bytes)
 * Audio effect remove
 */
void func_800BB044(s32 effectId) {
    /* Effect remove - stub */
}

/*
 * func_800BB140 (1372 bytes)
 * Audio DSP process
 */
void func_800BB140(void *dspBuffer) {
    /* DSP process - stub */
}

/*
 * func_800BB69C (408 bytes)
 * Audio output setup
 */
void func_800BB69C(s32 outputMode) {
    /* Output setup - stub */
}

/*
 * func_800BB834 (392 bytes)
 * Audio hardware sync
 */
void func_800BB834(void) {
    /* HW sync - stub */
}

/*
 * func_800BB9BC (756 bytes)
 * Audio interrupt handler
 */
void func_800BB9BC(void) {
    /* Audio interrupt - stub */
}

/*
 * func_800BC2BC (292 bytes)
 * Camera reset
 */
void func_800BC2BC(void *camera) {
    /* Camera reset - stub */
}

/*
 * func_800BCBB8 (808 bytes)
 * Camera lerp position
 */
void func_800BCBB8(void *camera, f32 *target, f32 t) {
    /* Camera lerp - stub */
}

/*
 * func_800BCEE4 (548 bytes)
 * Camera orbit control
 */
void func_800BCEE4(void *camera, f32 yaw, f32 pitch) {
    /* Orbit - stub */
}

/*
 * func_800BD104 (460 bytes)
 * Camera dolly
 */
void func_800BD104(void *camera, f32 distance) {
    /* Dolly - stub */
}

/*
 * func_800BD2D0 (1976 bytes)
 * Camera collision avoidance
 */
void func_800BD2D0(void *camera) {
    /* Collision avoid - stub */
}

/*
 * func_800BDAA8 (852 bytes)
 * Camera shake effect
 */
void func_800BDAA8(f32 intensity, f32 duration) {
    /* Shake - stub */
}

/*
 * func_800BDDFC (192 bytes)
 * Camera shake update
 */
void func_800BDDFC(void *camera) {
    /* Shake update - stub */
}

/*
 * func_800BDEBC (444 bytes)
 * Camera zoom control
 */
void func_800BDEBC(void *camera, f32 zoom) {
    /* Zoom - stub */
}

/*
 * func_800BE078 (1136 bytes)
 * Camera auto-follow
 */
void func_800BE078(void *camera, void *target) {
    /* Auto follow - stub */
}

/*
 * func_800BE4F8 (936 bytes)
 * Camera cinematic mode
 */
void func_800BE4F8(s32 cinematicId) {
    /* Cinematic - stub */
}

/*
 * func_800BEAA0 (908 bytes)
 * Camera cut to
 */
void func_800BEAA0(void *camera, f32 *pos, f32 *look) {
    /* Cut to - stub */
}

/*
 * func_800BEE2C (924 bytes)
 * Camera blend between
 */
void func_800BEE2C(void *cam1, void *cam2, f32 t) {
    /* Blend - stub */
}

/*
 * func_800BF1C8 (236 bytes)
 * Camera FOV control
 */
void func_800BF1C8(void *camera, f32 fov) {
    /* FOV - stub */
}

/*
 * func_800BF2B8 (220 bytes)
 * Camera near/far planes
 */
void func_800BF2B8(void *camera, f32 near, f32 far) {
    /* Planes - stub */
}

/*
 * func_800BF394 (200 bytes)
 * Camera aspect ratio
 */
void func_800BF394(void *camera, f32 aspect) {
    /* Aspect - stub */
}

/*
 * func_800BF45C (988 bytes)
 * Camera look at
 */
void func_800BF45C(void *camera, f32 *target) {
    /* Look at - stub */
}

/*
 * func_800BF838 (948 bytes)
 * Camera first person
 */
void func_800BF838(void *camera, void *player) {
    /* First person - stub */
}

/*
 * func_800BFBEC (380 bytes)
 * Camera third person
 */
void func_800BFBEC(void *camera, void *player) {
    /* Third person - stub */
}

/*
 * func_800BFD94 (844 bytes)
 * Camera top down
 */
void func_800BFD94(void *camera) {
    /* Top down - stub */
}

/*
 * func_800C00E0 (444 bytes)
 * Camera free look
 */
void func_800C00E0(void *camera, s32 input) {
    /* Free look - stub */
}

/*
 * func_800C02A0 (556 bytes)
 * Camera replay mode
 */
void func_800C02A0(void *camera, s32 frame) {
    /* Replay cam - stub */
}

/*
 * func_800C04CC (912 bytes)
 * Camera track spline
 */
void func_800C04CC(void *camera, void *spline, f32 t) {
    /* Spline - stub */
}

/*
 * func_800C085C (612 bytes)
 * Camera matrix build
 */
void func_800C085C(void *camera, f32 *matrix) {
    /* Matrix build - stub */
}

/*
 * func_800C0AC0 (2884 bytes)
 * Camera full update
 */
void func_800C0AC0(void *camera) {
    /* Full update - stub */
}

/*
 * func_800C1604 (1416 bytes)
 * Camera input process
 */
void func_800C1604(void *camera, void *input) {
    /* Input process - stub */
}

/*
 * func_800C1B8C (1172 bytes)
 * Camera constraint check
 */
void func_800C1B8C(void *camera) {
    /* Constraint - stub */
}

/*
 * func_800C2020 (520 bytes)
 * Camera debug display
 */
void func_800C2020(void *camera) {
    /* Debug display - stub */
}

/*
 * func_800C2228 (548 bytes)
 * Camera save state
 */
void func_800C2228(void *camera, void *state) {
    /* Save state - stub */
}

/*
 * func_800C244C (660 bytes)
 * Camera restore state
 */
void func_800C244C(void *camera, void *state) {
    /* Restore state - stub */
}

/*
 * func_800C26E0 (644 bytes)
 * Camera multi-view
 */
void func_800C26E0(s32 viewIndex, void *camera) {
    /* Multi view - stub */
}

/*
 * func_800C2960 (644 bytes)
 * Camera split screen
 */
void func_800C2960(s32 numPlayers) {
    /* Split screen - stub */
}

/*
 * func_800C2BE0 (5664 bytes)
 * Camera scene manager
 */
void func_800C2BE0(void) {
    /* Scene manager - stub */
}

/*
 * func_800C4200 (1232 bytes)
 * Camera trigger check
 */
void func_800C4200(void *camera, void *triggers) {
    /* Trigger check - stub */
}

/*
 * func_800C46D0 (3956 bytes)
 * Camera path follow
 */
void func_800C46D0(void *camera, void *path) {
    /* Path follow - stub */
}

/*
 * func_800C5644 (3516 bytes)
 * Camera scripted sequence
 */
void func_800C5644(s32 scriptId) {
    /* Script sequence - stub */
}

/*
 * func_800C6404 (780 bytes)
 * Camera finish line
 */
void func_800C6404(void *camera, s32 placing) {
    /* Finish line - stub */
}

/*
 * func_800C7110 (572 bytes)
 * HUD element draw
 */
void func_800C7110(s32 elementId, s32 x, s32 y) {
    /* HUD element - stub */
}

/*
 * func_800C734C (700 bytes)
 * HUD text draw
 */
void func_800C734C(char *text, s32 x, s32 y) {
    /* HUD text - stub */
}

/*
 * func_800C760C (524 bytes)
 * HUD number draw
 */
void func_800C760C(s32 value, s32 digits, s32 x, s32 y) {
    /* HUD number - stub */
}

/*
 * func_800C7818 (1724 bytes)
 * HUD speedometer
 */
void func_800C7818(f32 speed) {
    /* Speedometer - stub */
}

/*
 * func_800C7ED4 (2540 bytes)
 * HUD lap counter
 */
void func_800C7ED4(s32 currentLap, s32 totalLaps) {
    /* Lap counter - stub */
}

/*
 * func_800C8864 (188 bytes)
 * HUD position display
 */
void func_800C8864(s32 position) {
    /* Position - stub */
}

/*
 * func_800C8920 (228 bytes)
 * HUD timer display
 */
void func_800C8920(s32 timeMs) {
    /* Timer - stub */
}

/*
 * func_800C9404 (300 bytes)
 * HUD minimap update
 */
void func_800C9404(void *player) {
    /* Minimap - stub */
}

/*
 * func_800CA308 (172 bytes)
 * HUD message display
 */
void func_800CA308(s32 messageId) {
    /* Message - stub */
}

/*
 * func_800CADA4 (2468 bytes)
 * HUD full render
 */
void func_800CADA4(void) {
    /* Full HUD - stub */
}

/*
 * func_800CB748 (628 bytes)
 * Menu button process
 */
s32 func_800CB748(s32 input) {
    /* Button process - stub */
    return 0;
}

/*
 * func_800CB9D0 (516 bytes)
 * Menu cursor move
 */
void func_800CB9D0(s32 direction) {
    /* Cursor move - stub */
}

/*
 * func_800CBBD4 (564 bytes)
 * Menu item select
 */
void func_800CBBD4(s32 itemIndex) {
    /* Item select - stub */
}

/*
 * func_800CBE08 (132 bytes)
 * Menu back
 */
void func_800CBE08(void) {
    /* Menu back - stub */
}

/*
 * func_800CBE8C (340 bytes)
 * Menu transition
 */
void func_800CBE8C(s32 toMenuId) {
    /* Transition - stub */
}

/*
 * func_800CC040 (900 bytes)
 * Menu animation update
 */
void func_800CC040(void) {
    /* Animation - stub */
}

/*
 * func_800CC3C0 (380 bytes)
 * Menu sound play
 */
void func_800CC3C0(s32 soundId) {
    /* Menu sound - stub */
}

/*
 * func_800CC540 (904 bytes)
 * Menu text scroll
 */
void func_800CC540(char *text, s32 maxWidth) {
    /* Text scroll - stub */
}

/*
 * func_800CC8C8 (316 bytes)
 * Menu highlight
 */
void func_800CC8C8(s32 itemIndex) {
    /* Highlight - stub */
}

/*
 * func_800CCA04 (1112 bytes)
 * Menu list render
 */
void func_800CCA04(void *list, s32 count) {
    /* List render - stub */
}

/*
 * func_800CCE5C (716 bytes)
 * Menu slider control
 */
s32 func_800CCE5C(s32 current, s32 min, s32 max) {
    /* Slider - stub */
    return current;
}

/*
 * func_800CD104 (1088 bytes)
 * Menu dialog display
 */
void func_800CD104(s32 dialogId) {
    /* Dialog - stub */
}

/*
 * func_800CD544 (412 bytes)
 * Menu confirm dialog
 */
s32 func_800CD544(char *message) {
    /* Confirm - stub */
    return 0;
}

/*
 * func_800CD6E0 (184 bytes)
 * Menu close dialog
 */
void func_800CD6E0(void) {
    /* Close dialog - stub */
}

/*
 * func_800CD798 (340 bytes)
 * Menu keyboard init
 */
void func_800CD798(void) {
    /* Keyboard init - stub */
}

/*
 * func_800CD8EC (500 bytes)
 * Menu keyboard input
 */
char func_800CD8EC(s32 input) {
    /* Keyboard input - stub */
    return '\0';
}

/*
 * func_800CDAE0 (460 bytes)
 * Menu text input
 */
void func_800CDAE0(char *buffer, s32 maxLen) {
    /* Text input - stub */
}

/*
 * func_800CDCAC (576 bytes)
 * Menu option toggle
 */
void func_800CDCAC(s32 optionId) {
    /* Toggle - stub */
}

/*
 * func_800CDEEC (764 bytes)
 * Menu save options
 */
void func_800CDEEC(void) {
    /* Save options - stub */
}

/*
 * func_800CE1EC (364 bytes)
 * Menu load options
 */
void func_800CE1EC(void) {
    /* Load options - stub */
}

/*
 * func_800CE358 (2532 bytes)
 * Menu options screen
 */
void func_800CE358(void) {
    /* Options screen - stub */
}

/*
 * func_800CED3C (1364 bytes)
 * Menu audio settings
 */
void func_800CED3C(void) {
    /* Audio settings - stub */
}

/*
 * func_800CF290 (228 bytes)
 * Menu video settings
 */
void func_800CF290(void) {
    /* Video settings - stub */
}

/*
 * func_800CF374 (808 bytes)
 * Menu control settings
 */
void func_800CF374(void) {
    /* Control settings - stub */
}

/*
 * func_800CF69C (1976 bytes)
 * Menu controller remap
 */
void func_800CF69C(void) {
    /* Remap - stub */
}

/*
 * func_800CFE74 (404 bytes)
 * Menu vibration test
 */
void func_800CFE74(void) {
    /* Vibration test - stub */
}

/*
 * func_800D000C (2264 bytes)
 * Track select screen
 */
void func_800D000C(void) {
    /* Track select - stub */
}

/*
 * func_800D08E4 (692 bytes)
 * Track preview render
 */
void func_800D08E4(s32 trackId) {
    /* Track preview - stub */
}

/*
 * func_800D0BA0 (1192 bytes)
 * Track info display
 */
void func_800D0BA0(s32 trackId) {
    /* Track info - stub */
}

/*
 * func_800D1248 (324 bytes)
 * Track unlock check
 */
s32 func_800D1248(s32 trackId) {
    /* Unlock check - stub */
    return 1;
}

/*
 * func_800D138C (804 bytes)
 * Car select screen
 */
void func_800D138C(void) {
    /* Car select - stub */
}

/*
 * func_800D16B0 (564 bytes)
 * Car preview render
 */
void func_800D16B0(s32 carId) {
    /* Car preview - stub */
}

/*
 * func_800D18E4 (152 bytes)
 * Car stats display
 */
void func_800D18E4(s32 carId) {
    /* Car stats - stub */
}

/*
 * func_800D197C (316 bytes)
 * Car unlock check
 */
s32 func_800D197C(s32 carId) {
    /* Car unlock - stub */
    return 1;
}

/*
 * func_800D1AB8 (552 bytes)
 * Car color select
 */
void func_800D1AB8(s32 colorId) {
    /* Color select - stub */
}

/*
 * func_800D1CE0 (1960 bytes)
 * Race setup screen
 */
void func_800D1CE0(void) {
    /* Race setup - stub */
}

/*
 * func_800D24C8 (1120 bytes)
 * Race mode select
 */
void func_800D24C8(s32 modeId) {
    /* Mode select - stub */
}

/*
 * func_800D2928 (332 bytes)
 * Lap count select
 */
void func_800D2928(s32 laps) {
    /* Lap count - stub */
}

/*
 * func_800D2A74 (440 bytes)
 * Difficulty select
 */
void func_800D2A74(s32 difficulty) {
    /* Difficulty - stub */
}

/*
 * func_800D2C2C (176 bytes)
 * Mirror mode toggle
 */
void func_800D2C2C(void) {
    /* Mirror - stub */
}

/*
 * func_800D2CDC (240 bytes)
 * Weather select
 */
void func_800D2CDC(s32 weather) {
    /* Weather - stub */
}

/*
 * func_800D2DCC (200 bytes)
 * Time of day select
 */
void func_800D2DCC(s32 timeOfDay) {
    /* Time of day - stub */
}

/*
 * func_800D2E94 (1544 bytes)
 * Multiplayer setup
 */
void func_800D2E94(void) {
    /* MP setup - stub */
}

/*
 * func_800D349C (1676 bytes)
 * Player join screen
 */
void func_800D349C(void) {
    /* Player join - stub */
}

/*
 * func_800D3B28 (5068 bytes)
 * Stunt mode setup
 */
void func_800D3B28(void) {
    /* Stunt setup - stub */
}

/*
 * func_800D4EF4 (532 bytes)
 * Battle mode setup
 */
void func_800D4EF4(void) {
    /* Battle setup - stub */
}

/*
 * func_800D510C (716 bytes)
 * Ghost race setup
 */
void func_800D510C(void) {
    /* Ghost setup - stub */
}

/*
 * func_800D58CC (740 bytes)
 * Records screen
 */
void func_800D58CC(void) {
    /* Records - stub */
}

/*
 * func_800D5BB0 (224 bytes)
 * Best times display
 */
void func_800D5BB0(s32 trackId) {
    /* Best times - stub */
}

/*
 * func_800D5C90 (220 bytes)
 * High scores display
 */
void func_800D5C90(s32 trackId) {
    /* High scores - stub */
}

/*
 * func_800D616C (452 bytes)
 * Stats display
 */
void func_800D616C(void) {
    /* Stats - stub */
}

/*
 * func_800D63F4 (676 bytes)
 * Achievements screen
 */
void func_800D63F4(void) {
    /* Achievements - stub */
}

/*
 * func_800D6698 (296 bytes)
 * Credits screen
 */
void func_800D6698(void) {
    /* Credits - stub */
}

/*
 * func_800D67C0 (348 bytes)
 * Credits scroll
 */
void func_800D67C0(void) {
    /* Credits scroll - stub */
}

/*
 * func_800D691C (1376 bytes)
 * Loading screen
 */
void func_800D691C(s32 percent) {
    /* Loading - stub */
}

/*
 * func_800D6E7C (852 bytes)
 * Loading tips
 */
void func_800D6E7C(void) {
    /* Tips - stub */
}

/*
 * func_800D71D0 (3256 bytes)
 * Pause menu
 */
void func_800D71D0(void) {
    /* Pause - stub */
}

/*
 * func_800D7E88 (788 bytes)
 * Pause resume
 */
void func_800D7E88(void) {
    /* Resume - stub */
}

/*
 * func_800D8184 (2772 bytes)
 * Pause restart
 */
void func_800D8184(void) {
    /* Restart - stub */
}

/*
 * func_800D8C58 (1032 bytes)
 * Pause quit
 */
void func_800D8C58(void) {
    /* Quit - stub */
}

/*
 * func_800D9060 (4204 bytes)
 * Results screen
 */
void func_800D9060(void) {
    /* Results - stub */
}

/*
 * func_800DA0CC (168 bytes)
 * Position result
 */
void func_800DA0CC(s32 position) {
    /* Position result - stub */
}

/*
 * func_800DA174 (348 bytes)
 * Time result
 */
void func_800DA174(s32 timeMs) {
    /* Time result - stub */
}

/*
 * func_800DA2D0 (2316 bytes)
 * Points award
 */
void func_800DA2D0(void) {
    /* Points - stub */
}

/*
 * func_800DABDC (2940 bytes)
 * Replay save prompt
 */
void func_800DABDC(void) {
    /* Replay save - stub */
}

/*
 * func_800DB758 (196 bytes)
 * Continue prompt
 */
s32 func_800DB758(void) {
    /* Continue - stub */
    return 0;
}

/*
 * func_800DC248 (432 bytes)
 * Championship standings
 */
void func_800DC248(void) {
    /* Standings - stub */
}

/*
 * func_800DC3F8 (924 bytes)
 * Trophy award
 */
void func_800DC3F8(s32 placing) {
    /* Trophy - stub */
}

/*
 * func_800DC794 (248 bytes)
 * Unlock notification
 */
void func_800DC794(s32 unlockId) {
    /* Unlock notify - stub */
}

/*
 * func_800DC88C (1272 bytes)
 * Attract mode start
 */
void func_800DC88C(void) {
    /* Attract start - stub */
}

/*
 * func_800DC99C (1016 bytes)
 * Attract demo play
 */
void func_800DC99C(void) {
    /* Demo play - stub */
}

/*
 * func_800DCD94 (96 bytes)
 * Attract idle check
 */
s32 func_800DCD94(void) {
    /* Idle check - stub */
    return 0;
}

/*
 * func_800DCDF4 (732 bytes)
 * Attract video play
 */
void func_800DCDF4(s32 videoId) {
    /* Video play - stub */
}

/*
 * func_800DD0D0 (988 bytes)
 * Attract sequence update
 */
void func_800DD0D0(void) {
    /* Sequence update - stub */
}

/*
 * func_800DD4AC (2816 bytes)
 * Title screen
 */
void func_800DD4AC(void) {
    /* Title screen - stub */
}

/*
 * func_800DDFAC (608 bytes)
 * Title logo animate
 */
void func_800DDFAC(void) {
    /* Logo animate - stub */
}

/*
 * func_800DE20C (724 bytes)
 * Title button prompt
 */
void func_800DE20C(void) {
    /* Button prompt - stub */
}

/*
 * func_800DE4DC (908 bytes)
 * Title background
 */
void func_800DE4DC(void) {
    /* Background - stub */
}

/*
 * func_800DE868 (836 bytes)
 * Main menu screen
 */
void func_800DE868(void) {
    /* Main menu - stub */
}

/*
 * func_800DEBAC (224 bytes)
 * Main menu input
 */
void func_800DEBAC(s32 input) {
    /* Menu input - stub */
}

/*
 * func_800DEC8C (732 bytes)
 * Main menu render
 */
void func_800DEC8C(void) {
    /* Menu render - stub */
}

/*
 * func_800DEF68 (2976 bytes)
 * Mode select screen
 */
void func_800DEF68(void) {
    /* Mode select - stub */
}

/*
 * func_800DFB08 (188 bytes)
 * Mode select input
 */
void func_800DFB08(s32 input) {
    /* Mode input - stub */
}

/*
 * func_800DFBC4 (1868 bytes)
 * Profile select screen
 */
void func_800DFBC4(void) {
    /* Profile select - stub */
}

/*
 * func_800EB028 (1640 bytes)
 * World object spawn
 */
void *func_800EB028(s32 objectType, f32 *pos) {
    /* Object spawn - stub */
    return NULL;
}

/*
 * func_800EB690 (396 bytes)
 * World object destroy
 */
void func_800EB690(void *object) {
    /* Object destroy - stub */
}

/*
 * func_800EC2F8 (1584 bytes)
 * World physics tick
 *
 * Based on arcade collision.c - iterates all cars and applies physics
 * Data structures:
 *   D_80152744 - number of active cars
 *   D_8015A250 - car state array base (stride 0x808)
 *   D_80153E88 - car data array (stride 8)
 */
void func_800EC2F8(void) {
    extern s8 D_80152744;
    extern u8 D_8015A250[];
    s32 i;
    s32 numCars;

    numCars = D_80152744;
    if (numCars <= 0) {
        return;
    }

    /* Process each car's physics */
    for (i = 0; i < numCars; i++) {
        u8 *carState = &D_8015A250[i * 0x808];

        /* Check if car is active (offset 0x7CC) */
        if (carState[0x7CC] == 0) {
            continue;
        }

        /* Apply gravity */
        func_800ECC18(carState);

        /* Apply friction */
        func_800ED674(carState);

        /* Integrate velocity */
        func_800ED804(carState, 1.0f / 60.0f);
    }
}

/*
 * func_800EC928 (588 bytes)
 * World collision detect
 *
 * Based on arcade collision() - check if two objects' bounding spheres overlap
 * Returns 1 if collision detected, 0 otherwise
 */
s32 func_800EC928(void *a, void *b) {
    f32 *posA, *posB;
    f32 dx, dy, dz, distSq;
    f32 radA, radB, radSum;

    if (a == NULL || b == NULL) {
        return 0;
    }

    /* Get positions (offset 0x24 = position vector) */
    posA = (f32 *)((u8 *)a + 0x24);
    posB = (f32 *)((u8 *)b + 0x24);

    /* Calculate distance squared between centers */
    dx = posA[0] - posB[0];
    dy = posA[1] - posB[1];
    dz = posA[2] - posB[2];
    distSq = dx * dx + dy * dy + dz * dz;

    /* Get collision radii (offset 0x48) */
    radA = *(f32 *)((u8 *)a + 0x48);
    radB = *(f32 *)((u8 *)b + 0x48);
    radSum = radA + radB;

    /* Check if bounding spheres overlap */
    if (distSq < radSum * radSum) {
        return 1;
    }

    return 0;
}

/*
 * func_800ECB74 (164 bytes)
 * World bounds check
 *
 * Check if position is within world bounds
 * Returns 1 if in bounds, 0 if out of bounds
 */
s32 func_800ECB74(f32 *pos) {
    extern f32 D_80124500;  /* World min X */
    extern f32 D_80124504;  /* World max X */
    extern f32 D_80124508;  /* World min Y */
    extern f32 D_8012450C;  /* World max Y */
    extern f32 D_80124510;  /* World min Z */
    extern f32 D_80124514;  /* World max Z */

    if (pos == NULL) {
        return 0;
    }

    /* Check X bounds */
    if (pos[0] < D_80124500 || pos[0] > D_80124504) {
        return 0;
    }

    /* Check Y bounds (height) */
    if (pos[1] < D_80124508 || pos[1] > D_8012450C) {
        return 0;
    }

    /* Check Z bounds */
    if (pos[2] < D_80124510 || pos[2] > D_80124514) {
        return 0;
    }

    return 1;
}

/*
 * func_800ECC18 (2652 bytes)
 * World gravity apply
 *
 * Based on arcade physics - applies gravity acceleration to velocity
 * Gravity constant at D_80124574 (typically -9.8 * scale)
 */
void func_800ECC18(void *object) {
    extern f32 D_80124574;  /* Gravity constant */
    f32 *velocity;
    f32 *accel;
    f32 dt;

    if (object == NULL) {
        return;
    }

    /* Get velocity vector (offset 0x34) */
    velocity = (f32 *)((u8 *)object + 0x34);

    /* Get acceleration vector (offset 0x40) */
    accel = (f32 *)((u8 *)object + 0x40);

    /* Fixed timestep */
    dt = 1.0f / 60.0f;

    /* Apply gravity to Y velocity (vertical) */
    velocity[1] += D_80124574 * dt;

    /* Apply additional acceleration if any */
    velocity[0] += accel[0] * dt;
    velocity[1] += accel[1] * dt;
    velocity[2] += accel[2] * dt;
}

/*
 * func_800ED674 (400 bytes)
 * World friction apply
 *
 * Applies friction/drag to reduce velocity over time
 * Friction coefficient at D_80124578
 */
void func_800ED674(void *object) {
    extern f32 D_80124578;  /* Friction coefficient (0.0 - 1.0) */
    f32 *velocity;
    f32 friction;
    u8 *flags;

    if (object == NULL) {
        return;
    }

    /* Get velocity vector (offset 0x34) */
    velocity = (f32 *)((u8 *)object + 0x34);

    /* Get object flags (offset 0x00) */
    flags = (u8 *)object;

    /* Check if object is grounded (bit 5 of flags) */
    if (flags[0] & 0x20) {
        /* Grounded - apply ground friction */
        friction = D_80124578;
    } else {
        /* Airborne - apply air resistance (less friction) */
        friction = D_80124578 * 0.1f;
    }

    /* Apply friction to horizontal velocities */
    velocity[0] *= (1.0f - friction);
    velocity[2] *= (1.0f - friction);

    /* Apply less friction to vertical (let gravity handle it) */
    if (velocity[1] > 0.0f) {
        velocity[1] *= (1.0f - friction * 0.5f);
    }
}

/*
 * func_800ED804 (712 bytes)
 * World velocity integrate
 *
 * Integrates velocity to update position (Euler integration)
 */
void func_800ED804(void *object, f32 dt) {
    f32 *position;
    f32 *velocity;

    if (object == NULL) {
        return;
    }

    /* Get position (offset 0x24) and velocity (offset 0x34) */
    position = (f32 *)((u8 *)object + 0x24);
    velocity = (f32 *)((u8 *)object + 0x34);

    /* Euler integration: pos += vel * dt */
    position[0] += velocity[0] * dt;
    position[1] += velocity[1] * dt;
    position[2] += velocity[2] * dt;
}

/*
 * func_800EDACC (540 bytes)
 * World collision response
 *
 * Based on arcade setCollisionForce() - applies impulse to separate objects
 * Uses conservation of momentum for elastic collision response
 */
void func_800EDACC(void *a, void *b) {
    f32 *posA, *posB;
    f32 *velA, *velB;
    f32 dx, dy, dz, dist, invDist;
    f32 nx, ny, nz;  /* Collision normal */
    f32 relVelX, relVelY, relVelZ;
    f32 relVelNormal;
    f32 impulse;
    f32 massA, massB, totalMass;
    f32 restitution;

    if (a == NULL || b == NULL || a == b) {
        return;
    }

    /* Get positions (offset 0x24) */
    posA = (f32 *)((u8 *)a + 0x24);
    posB = (f32 *)((u8 *)b + 0x24);

    /* Get velocities (offset 0x34) */
    velA = (f32 *)((u8 *)a + 0x34);
    velB = (f32 *)((u8 *)b + 0x34);

    /* Calculate collision normal (from B to A) */
    dx = posA[0] - posB[0];
    dy = posA[1] - posB[1];
    dz = posA[2] - posB[2];

    dist = sqrtf(dx * dx + dy * dy + dz * dz);
    if (dist < 0.0001f) {
        /* Objects at same position - push apart on Y axis */
        nx = 0.0f;
        ny = 1.0f;
        nz = 0.0f;
    } else {
        invDist = 1.0f / dist;
        nx = dx * invDist;
        ny = dy * invDist;
        nz = dz * invDist;
    }

    /* Calculate relative velocity */
    relVelX = velA[0] - velB[0];
    relVelY = velA[1] - velB[1];
    relVelZ = velA[2] - velB[2];

    /* Project relative velocity onto collision normal */
    relVelNormal = relVelX * nx + relVelY * ny + relVelZ * nz;

    /* If objects are separating, no collision response needed */
    if (relVelNormal > 0.0f) {
        return;
    }

    /* Get masses (offset 0x4C) - default to 1.0 if not set */
    massA = *(f32 *)((u8 *)a + 0x4C);
    massB = *(f32 *)((u8 *)b + 0x4C);
    if (massA <= 0.0f) massA = 1.0f;
    if (massB <= 0.0f) massB = 1.0f;
    totalMass = massA + massB;

    /* Coefficient of restitution (bounciness) */
    restitution = 0.5f;

    /* Calculate impulse magnitude */
    impulse = -(1.0f + restitution) * relVelNormal / totalMass;

    /* Apply impulse to both objects */
    velA[0] += impulse * massB * nx;
    velA[1] += impulse * massB * ny;
    velA[2] += impulse * massB * nz;

    velB[0] -= impulse * massA * nx;
    velB[1] -= impulse * massA * ny;
    velB[2] -= impulse * massA * nz;
}

/*
 * func_800EDCE8 (2292 bytes)
 * World trigger check
 *
 * Checks if player has entered any trigger volumes (checkpoints, pickups, etc.)
 * D_801431C0 - trigger array base
 * D_801431BC - number of triggers
 */
void func_800EDCE8(void *player) {
    extern u8 D_801431C0[];
    extern s16 D_801431BC;
    f32 *playerPos;
    s32 i;
    s32 numTriggers;

    if (player == NULL) {
        return;
    }

    playerPos = (f32 *)((u8 *)player + 0x24);
    numTriggers = D_801431BC;

    for (i = 0; i < numTriggers; i++) {
        u8 *trigger = &D_801431C0[i * 0x40];  /* Trigger stride = 64 bytes */
        f32 *triggerPos = (f32 *)(trigger + 0x04);
        f32 *triggerSize = (f32 *)(trigger + 0x10);
        s32 triggerType = trigger[0];
        s32 triggerActive = trigger[1];

        if (!triggerActive) {
            continue;
        }

        /* AABB check - is player inside trigger volume? */
        if (playerPos[0] >= triggerPos[0] - triggerSize[0] &&
            playerPos[0] <= triggerPos[0] + triggerSize[0] &&
            playerPos[1] >= triggerPos[1] - triggerSize[1] &&
            playerPos[1] <= triggerPos[1] + triggerSize[1] &&
            playerPos[2] >= triggerPos[2] - triggerSize[2] &&
            playerPos[2] <= triggerPos[2] + triggerSize[2]) {

            /* Trigger activated - call handler */
            func_800EE5DC(i);
        }
    }
}

/*
 * func_800EE5DC (580 bytes)
 * World trigger activate
 */
void func_800EE5DC(s32 triggerId) {
    /* Trigger activate - stub */
}

/*
 * func_800EE820 (148 bytes)
 * World effect spawn
 */
void *func_800EE820(s32 effectType, f32 *pos) {
    /* Effect spawn - stub */
    return NULL;
}

/*
 * func_800EE8B4 (456 bytes)
 * World effect update
 */
void func_800EE8B4(void *effect) {
    /* Effect update - stub */
}

/*
 * func_800EEA7C (820 bytes)
 * Particle emitter create
 */
void *func_800EEA7C(s32 type, f32 *pos) {
    /* Emitter create - stub */
    return NULL;
}

/*
 * func_800EEDB0 (1240 bytes)
 * Particle update
 */
void func_800EEDB0(void *emitter) {
    /* Particle update - stub */
}

/*
 * func_800EF288 (932 bytes)
 * Smoke effect
 */
void func_800EF288(f32 *pos, f32 *vel) {
    /* Smoke - stub */
}

/*
 * func_800EF62C (712 bytes)
 * Spark effect
 */
void func_800EF62C(f32 *pos, s32 count) {
    /* Spark - stub */
}

/*
 * func_800EF8F4 (1460 bytes)
 * Explosion effect
 */
void func_800EF8F4(f32 *pos, f32 radius) {
    /* Explosion - stub */
}

/*
 * func_800EFEA8 (600 bytes)
 * Dust cloud effect
 */
void func_800EFEA8(f32 *pos) {
    /* Dust cloud - stub */
}

/*
 * func_800F0100 (1396 bytes)
 * Skid mark render
 */
void func_800F0100(void *tire) {
    /* Skid mark - stub */
}

/*
 * func_800F0674 (472 bytes)
 * Trail effect
 */
void func_800F0674(void *object) {
    /* Trail - stub */
}

/*
 * func_800F084C (208 bytes)
 * Weather rain
 */
void func_800F084C(void) {
    /* Rain - stub */
}

/*
 * func_800F091C (564 bytes)
 * Weather snow
 */
void func_800F091C(void) {
    /* Snow - stub */
}

/*
 * func_800F0F4C (468 bytes)
 * Weather fog
 */
void func_800F0F4C(f32 density) {
    /* Fog - stub */
}

/*
 * func_800F1120 (252 bytes)
 * Weather update
 */
void func_800F1120(void) {
    /* Weather update - stub */
}

/*
 * func_800F121C (1824 bytes)
 * Lighting setup
 */
void func_800F121C(void) {
    /* Lighting - stub */
}

/*
 * func_800F193C (968 bytes)
 * Shadow render
 */
void func_800F193C(void *object) {
    /* Shadow - stub */
}

/*
 * func_800F1D04 (924 bytes)
 * Lens flare
 */
void func_800F1D04(f32 *sunPos) {
    /* Lens flare - stub */
}

/*
 * func_800F20A0 (1664 bytes)
 * Environment map
 */
void func_800F20A0(void *object) {
    /* Env map - stub */
}

/*
 * func_800F2720 (376 bytes)
 * Reflection setup
 */
void func_800F2720(void) {
    /* Reflection - stub */
}

/*
 * func_800F2890 (408 bytes)
 * Water surface
 */
void func_800F2890(void) {
    /* Water - stub */
}

/*
 * func_800F2A28 (2736 bytes)
 * Skybox render
 */
void func_800F2A28(void *camera) {
    /* Skybox - stub */
}

/*
 * func_800F34D8 (3576 bytes)
 * Track render
 */
void func_800F34D8(void *camera) {
    /* Track render - stub */
}

/*
 * func_800F42D0 (260 bytes)
 * Track section visible
 */
s32 func_800F42D0(s32 sectionId, void *camera) {
    /* Section visible - stub */
    return 1;
}

/*
 * func_800F43D4 (560 bytes)
 * Track LOD select
 */
s32 func_800F43D4(f32 distance) {
    /* LOD select - stub */
    return 0;
}

/*
 * func_800F4604 (6540 bytes)
 * Track geometry stream
 */
void func_800F4604(void) {
    /* Geometry stream - stub */
}

/*
 * func_800F5F90 (1348 bytes)
 * Track texture load
 */
void func_800F5F90(s32 textureId) {
    /* Texture load - stub */
}

/*
 * func_800F64D4 (1120 bytes)
 * Billboard render
 */
void func_800F64D4(void *billboard) {
    /* Billboard - stub */
}

/*
 * func_800F6934 (388 bytes)
 * Sign render
 */
void func_800F6934(void *sign) {
    /* Sign - stub */
}

/*
 * func_800F6AB8 (2460 bytes)
 * Props render
 */
void func_800F6AB8(void *camera) {
    /* Props - stub */
}

/*
 * func_800F7454 (1996 bytes)
 * Crowd render
 */
void func_800F7454(void) {
    /* Crowd - stub */
}

/*
 * func_800F7C28 (796 bytes)
 * Car body render
 */
void func_800F7C28(void *car) {
    /* Car body - stub */
}

/*
 * func_800F7F44 (1604 bytes)
 * Car wheels render
 */
void func_800F7F44(void *car) {
    /* Wheels - stub */
}

/*
 * func_800F8588 (548 bytes)
 * Car damage render
 */
void func_800F8588(void *car) {
    /* Damage - stub */
}

/*
 * func_800F87AC (964 bytes)
 * Car lights render
 */
void func_800F87AC(void *car) {
    /* Lights - stub */
}

/*
 * func_800F8B70 (556 bytes)
 * Car exhaust render
 */
void func_800F8B70(void *car) {
    /* Exhaust - stub */
}

/*
 * func_800F8D9C (300 bytes)
 * Car antenna render
 */
void func_800F8D9C(void *car) {
    /* Antenna - stub */
}

/*
 * func_800F8EC8 (1240 bytes)
 * Car nitro effect
 */
void func_800F8EC8(void *car) {
    /* Nitro - stub */
}

/*
 * func_800F93A0 (5652 bytes)
 * Scene render main
 */
void func_800F93A0(void) {
    /* Scene render - stub */
}

/*
 * func_800FA9B4 (948 bytes)
 * Z-buffer setup
 */
void func_800FA9B4(void) {
    /* Z-buffer - stub */
}

/*
 * func_800FAD58 (136 bytes)
 * Frame start
 */
void func_800FAD58(void) {
    /* Frame start - stub */
}

/*
 * func_800FADE0 (1108 bytes)
 * Frame end
 */
void func_800FADE0(void) {
    /* Frame end - stub */
}

/*
 * func_800FB234 (148 bytes)
 * Vsync wait
 */
void func_800FB234(void) {
    /* Vsync - stub */
}

/*
 * func_800FB2C8 (5944 bytes)
 * Display list flush
 */
void func_800FB2C8(void) {
    /* DL flush - stub */
}

/*
 * func_800FCA00 (1016 bytes)
 * Debug overlay
 */
void func_800FCA00(void) {
    /* Debug overlay - stub */
}

/*
 * func_800FCDF8 (556 bytes)
 * Debug stats
 */
void func_800FCDF8(void) {
    /* Debug stats - stub */
}

/*
 * func_800FD024 (540 bytes)
 * Debug collision
 */
void func_800FD024(void) {
    /* Debug collision - stub */
}

/*
 * func_800FD240 (552 bytes)
 * Debug AI paths
 */
void func_800FD240(void) {
    /* Debug AI - stub */
}

/*
 * func_800FD7E8 (244 bytes)
 * Random seed
 */
void func_800FD7E8(u32 seed) {
    /* Random seed - stub */
}

/*
 * func_800FD8DC (284 bytes)
 * Random int
 */
s32 func_800FD8DC(void) {
    /* Random int - stub */
    return 0;
}

/*
 * func_800FD9F8 (1436 bytes)
 * Random float
 */
f32 func_800FD9F8(void) {
    /* Random float - stub */
    return 0.0f;
}

/*
 * func_800FDF94 (248 bytes)
 * Random range
 */
s32 func_800FDF94(s32 min, s32 max) {
    /* Random range - stub */
    return min;
}

/*
 * func_800FE08C (1072 bytes)
 * Timer start
 */
void func_800FE08C(s32 timerId) {
    /* Timer start - stub */
}

/*
 * func_800FE4BC (100 bytes)
 * Timer stop
 */
void func_800FE4BC(s32 timerId) {
    /* Timer stop - stub */
}

/*
 * func_800FE520 (144 bytes)
 * Timer reset
 */
void func_800FE520(s32 timerId) {
    /* Timer reset - stub */
}

/*
 * func_800FE5B0 (412 bytes)
 * Timer get elapsed
 */
s32 func_800FE5B0(s32 timerId) {
    /* Get elapsed - stub */
    return 0;
}

/*
 * func_800FE7A4 (164 bytes)
 * Timer pause
 */
void func_800FE7A4(s32 timerId) {
    /* Timer pause - stub */
}

/*
 * func_800FE848 (220 bytes)
 * Timer resume
 */
void func_800FE848(s32 timerId) {
    /* Timer resume - stub */
}

/*
 * func_800FE924 (228 bytes)
 * Timer lap
 */
s32 func_800FE924(s32 timerId) {
    /* Timer lap - stub */
    return 0;
}

/*
 * func_800FEA08 (668 bytes)
 * Race timer update
 */
void func_800FEA08(void) {
    /* Race timer - stub */
}

/*
 * func_800FECA4 (352 bytes)
 * Countdown timer
 */
void func_800FECA4(void) {
    /* Countdown - stub */
}

/*
 * func_800FEE04 (1172 bytes)
 * Split time display
 */
void func_800FEE04(s32 splitTime) {
    /* Split time - stub */
}

/*
 * func_800FF298 (1164 bytes)
 * Best lap check
 */
s32 func_800FF298(s32 lapTime) {
    /* Best lap - stub */
    return 0;
}

/*
 * func_800FF724 (1748 bytes)
 * Record save
 */
void func_800FF724(void) {
    /* Record save - stub */
}

/*
 * func_800FFDF8 (1900 bytes)
 * High score entry
 */
void func_800FFDF8(void) {
    /* High score - stub */
}

/*
 * func_80100564 (1576 bytes)
 * Leaderboard display
 */
void func_80100564(void) {
    /* Leaderboard - stub */
}

/*
 * func_80100B8C (716 bytes)
 * Score calculate
 */
s32 func_80100B8C(void) {
    /* Score calc - stub */
    return 0;
}

/*
 * func_80100E60 (2732 bytes)
 * Stunt score
 */
s32 func_80100E60(s32 trickId) {
    /* Stunt score - stub */
    return 0;
}

/*
 * func_8010190C (1152 bytes)
 * Combo multiplier
 */
s32 func_8010190C(s32 combo) {
    /* Combo - stub */
    return combo;
}

/*
 * func_80101D8C (1168 bytes)
 * Trick detect
 */
s32 func_80101D8C(void *car) {
    /* Trick detect - stub */
    return 0;
}

/*
 * func_8010221C (564 bytes)
 * Trick register
 */
void func_8010221C(s32 trickId) {
    /* Trick register - stub */
}

/*
 * func_80102450 (1336 bytes)
 * Air time track
 */
void func_80102450(void *car) {
    /* Air time - stub */
}

/*
 * func_80102988 (1448 bytes)
 * Flip detect
 */
s32 func_80102988(void *car) {
    /* Flip detect - stub */
    return 0;
}

/*
 * func_80102F30 (3576 bytes)
 * Barrel roll
 */
s32 func_80102F30(void *car) {
    /* Barrel roll - stub */
    return 0;
}

/*
 * func_80103D28 (2524 bytes)
 * Spin detect
 */
s32 func_80103D28(void *car) {
    /* Spin detect - stub */
    return 0;
}

/*
 * func_80104704 (1040 bytes)
 * Landing bonus
 */
s32 func_80104704(void *car) {
    /* Landing bonus - stub */
    return 0;
}

/*
 * func_80104B14 (2412 bytes)
 * Stunt combo
 */
void func_80104B14(void *car) {
    /* Stunt combo - stub */
}

/*
 * func_80105480 (1780 bytes)
 * Wing deploy
 */
void func_80105480(void *car) {
    /* Wing deploy - stub */
}

/*
 * func_80105B74 (572 bytes)
 * Wing retract
 */
void func_80105B74(void *car) {
    /* Wing retract - stub */
}

/*
 * func_80105DB0 (248 bytes)
 * Wing state check
 */
s32 func_80105DB0(void *car) {
    /* Wing state - stub */
    return 0;
}

/*
 * func_80105EA8 (2508 bytes)
 * Glide physics
 */
void func_80105EA8(void *car) {
    /* Glide - stub */
}

/*
 * func_80106874 (712 bytes)
 * Boost activate
 */
void func_80106874(void *car) {
    /* Boost - stub */
}

/*
 * func_80106B3C (600 bytes)
 * Boost update
 */
void func_80106B3C(void *car) {
    /* Boost update - stub */
}

/*
 * func_80106D94 (1604 bytes)
 * Nitro pickup
 */
void func_80106D94(void *car, void *pickup) {
    /* Nitro pickup - stub */
}

/*
 * func_801073D8 (580 bytes)
 * Checkpoint hit
 */
void func_801073D8(void *car, s32 cpId) {
    /* Checkpoint - stub */
}

/*
 * func_8010761C (1240 bytes)
 * Lap complete
 */
void func_8010761C(void *car) {
    /* Lap complete - stub */
}

/*
 * func_80107AF4 (1000 bytes)
 * Race finish
 */
void func_80107AF4(void *car) {
    /* Race finish - stub */
}

/*
 * func_80107EDC (632 bytes)
 * Position update
 */
void func_80107EDC(void) {
    /* Position update - stub */
}

/*
 * func_80108154 (900 bytes)
 * Race standings
 */
void func_80108154(void) {
    /* Standings - stub */
}

/*
 * func_801084D4 (1500 bytes)
 * Respawn car
 */
void func_801084D4(void *car) {
    /* Respawn - stub */
}

/*
 * func_80108AB0 (760 bytes)
 * Death check
 */
s32 func_80108AB0(void *car) {
    /* Death check - stub */
    return 0;
}

/*
 * func_80108DA8 (408 bytes)
 * Wreck car
 */
void func_80108DA8(void *car) {
    /* Wreck - stub */
}

/*
 * func_80108F40 (1320 bytes)
 * Recovery timer
 */
void func_80108F40(void *car) {
    /* Recovery - stub */
}

/*
 * func_80109468 (1528 bytes)
 * Reset position
 */
void func_80109468(void *car) {
    /* Reset pos - stub */
}

/*
 * func_80109A60 (1276 bytes)
 * Shortcut detect
 */
s32 func_80109A60(void *car) {
    /* Shortcut - stub */
    return 0;
}

/*
 * func_80109F5C (1504 bytes)
 * Wrong way detect
 */
s32 func_80109F5C(void *car) {
    /* Wrong way - stub */
    return 0;
}

/*
 * func_8010A53C (624 bytes)
 * Out of bounds
 */
s32 func_8010A53C(void *car) {
    /* Out of bounds - stub */
    return 0;
}

/*
 * func_8010A7AC (292 bytes)
 * Track zone get
 */
s32 func_8010A7AC(f32 *pos) {
    /* Track zone - stub */
    return 0;
}

/*
 * func_8010A8D0 (1500 bytes)
 * Surface type get
 */
s32 func_8010A8D0(f32 *pos) {
    /* Surface type - stub */
    return 0;
}

/*
 * func_8010AEAC (1828 bytes)
 * Grip calculate
 */
f32 func_8010AEAC(void *tire, s32 surface) {
    /* Grip - stub */
    return 1.0f;
}

/*
 * func_8010B5D0 (556 bytes)
 * Drag calculate
 */
f32 func_8010B5D0(void *car) {
    /* Drag - stub */
    return 0.0f;
}

/*
 * func_8010B7FC (460 bytes)
 * Downforce calculate
 */
f32 func_8010B7FC(void *car) {
    /* Downforce - stub */
    return 0.0f;
}

/*
 * func_8010B9C8 (700 bytes)
 * Engine torque
 */
f32 func_8010B9C8(void *car, s32 rpm) {
    /* Torque - stub */
    return 0.0f;
}

/*
 * func_8010BC84 (932 bytes)
 * Transmission shift
 */
void func_8010BC84(void *car, s32 gear) {
    /* Shift - stub */
}

/*
 * func_8010C02C (1060 bytes)
 * Brake apply
 */
void func_8010C02C(void *car, f32 force) {
    /* Brake - stub */
}

/*
 * func_8010C450 (320 bytes)
 * Handbrake apply
 */
void func_8010C450(void *car) {
    /* Handbrake - stub */
}

/*
 * func_8010C590 (320 bytes)
 * Throttle apply
 */
void func_8010C590(void *car, f32 amount) {
    /* Throttle - stub */
}

/*
 * func_8010C6D0 (292 bytes)
 * Steering apply
 */
void func_8010C6D0(void *car, f32 angle) {
    /* Steering - stub */
}

/*
 * func_8010C7F4 (384 bytes)
 * Car input process
 */
void func_8010C7F4(void *car, void *input) {
    /* Car input - stub */
}

/*
 * func_8010C974 (2636 bytes)
 * AI input generate
 *
 * Based on arcade MaxPath() - generates steering, throttle, brake inputs
 * for AI-controlled cars using path following and obstacle avoidance.
 *
 * Car AI state offsets:
 *   0x100: target waypoint index (s32)
 *   0x104: target position (f32[3])
 *   0x110: current path ID (s32)
 *   0x114: steering output (f32)
 *   0x118: throttle output (f32)
 *   0x11C: brake output (f32)
 *   0x120: AI flags (u32)
 */
void func_8010C974(void *car) {
    f32 *carPos, *carVel, *carDir;
    f32 *targetPos, *steer, *throttle, *brake;
    f32 dx, dz, dist, targetDist;
    f32 dotForward, dotRight;
    f32 carSpeed, desiredSpeed;
    s32 *waypointIdx;
    u32 *aiFlags;

    if (car == NULL) {
        return;
    }

    /* Get car state */
    carPos = (f32 *)((u8 *)car + 0x24);
    carVel = (f32 *)((u8 *)car + 0x34);
    carDir = (f32 *)((u8 *)car + 0x60);  /* Forward direction */

    /* Get AI state */
    waypointIdx = (s32 *)((u8 *)car + 0x100);
    targetPos = (f32 *)((u8 *)car + 0x104);
    steer = (f32 *)((u8 *)car + 0x114);
    throttle = (f32 *)((u8 *)car + 0x118);
    brake = (f32 *)((u8 *)car + 0x11C);
    aiFlags = (u32 *)((u8 *)car + 0x120);

    /* Calculate vector to target */
    dx = targetPos[0] - carPos[0];
    dz = targetPos[2] - carPos[2];
    dist = sqrtf(dx * dx + dz * dz);

    /* Calculate current speed */
    carSpeed = sqrtf(carVel[0] * carVel[0] + carVel[2] * carVel[2]);

    /* Check if reached waypoint */
    if (dist < 5.0f) {
        /* Advance to next waypoint */
        func_8010D3C0(car);
        return;
    }

    /* Normalize direction to target */
    if (dist > 0.001f) {
        dx /= dist;
        dz /= dist;
    }

    /* Calculate steering based on angle to target */
    /* Dot product with forward = cos(angle), with right = sin(angle) */
    dotForward = carDir[0] * dx + carDir[2] * dz;
    dotRight = carDir[2] * dx - carDir[0] * dz;  /* Perpendicular */

    /* Set steering (-1 to 1 based on angle) */
    *steer = dotRight * 2.0f;
    if (*steer > 1.0f) *steer = 1.0f;
    if (*steer < -1.0f) *steer = -1.0f;

    /* Calculate desired speed based on upcoming turn angle */
    desiredSpeed = 60.0f;  /* Base speed */
    if (dotForward < 0.7f) {
        /* Sharp turn ahead - slow down */
        desiredSpeed = 30.0f + dotForward * 30.0f;
    }

    /* Set throttle/brake based on speed difference */
    if (carSpeed < desiredSpeed * 0.9f) {
        *throttle = 1.0f;
        *brake = 0.0f;
    } else if (carSpeed > desiredSpeed * 1.1f) {
        *throttle = 0.0f;
        *brake = 0.5f;
    } else {
        *throttle = 0.5f;
        *brake = 0.0f;
    }

    /* Apply obstacle avoidance */
    func_8010D85C(car);

    /* Apply rubber banding */
    func_8010DCFC(car);
}

/*
 * func_8010D3C0 (704 bytes)
 * AI target find
 *
 * Updates AI target waypoint based on path data
 * Path data structure at D_80143000
 */
void func_8010D3C0(void *car) {
    extern u8 D_80143000[];  /* Path data array */
    extern s16 D_80143200;   /* Number of waypoints per path */
    s32 *waypointIdx, *pathId;
    f32 *targetPos;
    f32 *pathData;
    s32 wpIdx, numWaypoints;

    if (car == NULL) {
        return;
    }

    waypointIdx = (s32 *)((u8 *)car + 0x100);
    pathId = (s32 *)((u8 *)car + 0x110);
    targetPos = (f32 *)((u8 *)car + 0x104);

    numWaypoints = D_80143200;
    if (numWaypoints <= 0) {
        numWaypoints = 20;  /* Default */
    }

    /* Advance waypoint */
    (*waypointIdx)++;
    if (*waypointIdx >= numWaypoints) {
        *waypointIdx = 0;  /* Loop back */
    }

    wpIdx = *waypointIdx;

    /* Get waypoint position from path data */
    /* Path format: pathId * stride + wpIdx * 12 (3 floats) */
    pathData = (f32 *)&D_80143000[(*pathId) * numWaypoints * 12 + wpIdx * 12];
    targetPos[0] = pathData[0];
    targetPos[1] = pathData[1];
    targetPos[2] = pathData[2];
}

/*
 * func_8010D680 (476 bytes)
 * AI path follow
 *
 * Main path following update - called from AI input generate
 */
void func_8010D680(void *car) {
    f32 *carPos, *targetPos;
    f32 dx, dy, dz, distSq;

    if (car == NULL) {
        return;
    }

    carPos = (f32 *)((u8 *)car + 0x24);
    targetPos = (f32 *)((u8 *)car + 0x104);

    /* Calculate squared distance to target */
    dx = targetPos[0] - carPos[0];
    dy = targetPos[1] - carPos[1];
    dz = targetPos[2] - carPos[2];
    distSq = dx * dx + dy * dy + dz * dz;

    /* If close enough to waypoint, get next one */
    if (distSq < 25.0f) {  /* 5^2 = 25 */
        func_8010D3C0(car);
    }
}

/*
 * func_8010D85C (372 bytes)
 * AI obstacle avoid
 *
 * Adjusts steering to avoid nearby obstacles and other cars
 */
void func_8010D85C(void *car) {
    extern u8 D_80152818[];  /* Car array */
    extern s8 D_80152744;    /* Number of cars */
    f32 *carPos, *steer;
    f32 *otherPos;
    f32 dx, dz, dist;
    s32 i, numCars;

    if (car == NULL) {
        return;
    }

    carPos = (f32 *)((u8 *)car + 0x24);
    steer = (f32 *)((u8 *)car + 0x114);
    numCars = D_80152744;

    /* Check distance to other cars */
    for (i = 0; i < numCars; i++) {
        u8 *otherCar = &D_80152818[i * 0x144];

        if (otherCar == car) {
            continue;  /* Skip self */
        }

        otherPos = (f32 *)(otherCar + 0x24);
        dx = otherPos[0] - carPos[0];
        dz = otherPos[2] - carPos[2];
        dist = sqrtf(dx * dx + dz * dz);

        /* If car is close, adjust steering */
        if (dist < 8.0f && dist > 0.1f) {
            /* Steer away from other car */
            f32 avoidStrength = (8.0f - dist) / 8.0f;
            if (dx > 0) {
                *steer -= avoidStrength * 0.3f;
            } else {
                *steer += avoidStrength * 0.3f;
            }
        }
    }

    /* Clamp steering */
    if (*steer > 1.0f) *steer = 1.0f;
    if (*steer < -1.0f) *steer = -1.0f;
}

/*
 * func_8010D9CC (492 bytes)
 * AI overtake
 *
 * Attempts to overtake car ahead when appropriate
 */
void func_8010D9CC(void *car) {
    f32 *carPos, *carVel, *steer, *throttle;
    f32 *targetPos;
    u32 *aiFlags;

    if (car == NULL) {
        return;
    }

    carPos = (f32 *)((u8 *)car + 0x24);
    carVel = (f32 *)((u8 *)car + 0x34);
    steer = (f32 *)((u8 *)car + 0x114);
    throttle = (f32 *)((u8 *)car + 0x118);
    aiFlags = (u32 *)((u8 *)car + 0x120);

    /* Check if in overtake mode (flag bit 0x01) */
    if (*aiFlags & 0x01) {
        /* Apply more aggressive steering and throttle */
        *throttle = 1.0f;

        /* TODO: Calculate optimal overtake line */
    }
}

/*
 * func_8010DBB8 (324 bytes)
 * AI defend
 *
 * Defensive driving - blocks overtaking attempts
 */
void func_8010DBB8(void *car) {
    u32 *aiFlags;
    f32 *steer;

    if (car == NULL) {
        return;
    }

    aiFlags = (u32 *)((u8 *)car + 0x120);
    steer = (f32 *)((u8 *)car + 0x114);

    /* Check if in defensive mode (flag bit 0x02) */
    if (*aiFlags & 0x02) {
        /* TODO: Block racing line */
    }
}

/*
 * func_8010DCFC (660 bytes)
 * AI rubber band
 *
 * Based on arcade set_catchup() - adjusts AI speed based on position
 * to keep races competitive (slower when ahead, faster when behind)
 */
void func_8010DCFC(void *car) {
    extern f32 D_80143400;  /* Lead car distance */
    extern f32 D_80143404;  /* Player distance */
    f32 *throttle, *maxSpeed;
    f32 distanceGap, speedMod;
    s32 *racePosition;

    if (car == NULL) {
        return;
    }

    throttle = (f32 *)((u8 *)car + 0x118);
    maxSpeed = (f32 *)((u8 *)car + 0x124);  /* Max speed modifier */
    racePosition = (s32 *)((u8 *)car + 0x128);

    /* Calculate gap to player */
    distanceGap = D_80143400 - D_80143404;

    /* Adjust speed based on gap */
    if (distanceGap > 50.0f) {
        /* AI is far ahead - slow down slightly */
        speedMod = 0.9f;
    } else if (distanceGap < -50.0f) {
        /* AI is far behind - speed up */
        speedMod = 1.1f;
    } else {
        /* Close race - normal speed */
        speedMod = 1.0f;
    }

    /* Apply modifier */
    *maxSpeed = speedMod;
    *throttle *= speedMod;
}

/*
 * func_8010DF90 (364 bytes)
 * AI difficulty adjust
 *
 * Adjusts AI parameters based on difficulty setting
 * Difficulty: 0=Easy, 1=Medium, 2=Hard, 3=Expert
 */
void func_8010DF90(void *car, s32 difficulty) {
    f32 *reactionTime, *accuracy, *aggression, *maxSpeed;

    if (car == NULL) {
        return;
    }

    reactionTime = (f32 *)((u8 *)car + 0x12C);
    accuracy = (f32 *)((u8 *)car + 0x130);
    aggression = (f32 *)((u8 *)car + 0x134);
    maxSpeed = (f32 *)((u8 *)car + 0x124);

    switch (difficulty) {
        case 0:  /* Easy */
            *reactionTime = 0.5f;
            *accuracy = 0.7f;
            *aggression = 0.3f;
            *maxSpeed = 0.85f;
            break;

        case 1:  /* Medium */
            *reactionTime = 0.3f;
            *accuracy = 0.85f;
            *aggression = 0.5f;
            *maxSpeed = 0.95f;
            break;

        case 2:  /* Hard */
            *reactionTime = 0.15f;
            *accuracy = 0.95f;
            *aggression = 0.7f;
            *maxSpeed = 1.0f;
            break;

        case 3:  /* Expert */
            *reactionTime = 0.05f;
            *accuracy = 1.0f;
            *aggression = 0.9f;
            *maxSpeed = 1.05f;
            break;

        default:
            break;
    }
}

/*
 * func_8010E0FC (1008 bytes)
 * AI behavior select
 *
 * Selects AI behavior based on race situation
 * Behaviors: 0=Normal, 1=Overtake, 2=Defend, 3=Recover, 4=Catchup
 */
void func_8010E0FC(void *car) {
    u32 *aiFlags;
    s32 *racePosition, *behavior;
    f32 *carSpeed;
    f32 speed;

    if (car == NULL) {
        return;
    }

    aiFlags = (u32 *)((u8 *)car + 0x120);
    racePosition = (s32 *)((u8 *)car + 0x128);
    behavior = (s32 *)((u8 *)car + 0x138);
    carSpeed = (f32 *)((u8 *)car + 0x34);

    speed = sqrtf(carSpeed[0] * carSpeed[0] + carSpeed[2] * carSpeed[2]);

    /* Check if crashed/stuck (very low speed) */
    if (speed < 2.0f) {
        *behavior = 3;  /* Recover */
        *aiFlags |= 0x04;
        return;
    }

    /* Check position for behavior selection */
    if (*racePosition <= 2) {
        /* In lead - defend position */
        *behavior = 2;
        *aiFlags |= 0x02;
        *aiFlags &= ~0x01;
    } else if (*racePosition >= 4) {
        /* In back - try to catch up/overtake */
        *behavior = 1;
        *aiFlags |= 0x01;
        *aiFlags &= ~0x02;
    } else {
        /* Mid-pack - normal racing */
        *behavior = 0;
        *aiFlags &= ~0x03;
    }
}

/*
 * func_8010E4EC (428 bytes)
 * AI aggression
 *
 * Sets AI aggression level (affects overtaking, blocking, etc.)
 */
void func_8010E4EC(void *car, s32 level) {
    f32 *aggression;

    if (car == NULL) {
        return;
    }

    aggression = (f32 *)((u8 *)car + 0x134);

    /* Clamp level to 0-10 */
    if (level < 0) level = 0;
    if (level > 10) level = 10;

    /* Convert to 0.0-1.0 range */
    *aggression = (f32)level / 10.0f;
}

/*
 * func_8010E69C (144 bytes)
 * AI speed limit
 *
 * Sets maximum speed for AI car
 */
void func_8010E69C(void *car, f32 limit) {
    f32 *maxSpeed;

    if (car == NULL) {
        return;
    }

    maxSpeed = (f32 *)((u8 *)car + 0x124);
    *maxSpeed = limit;
}

/*
 * func_8010E72C (392 bytes)
 * AI error inject
 *
 * Injects random errors to make AI more human-like
 * Based on arcade drone "personality" system
 */
void func_8010E72C(void *car) {
    extern u32 D_80143500;  /* Random seed */
    f32 *steer, *throttle;
    f32 *accuracy;
    f32 errorMag;
    u32 rand;

    if (car == NULL) {
        return;
    }

    steer = (f32 *)((u8 *)car + 0x114);
    throttle = (f32 *)((u8 *)car + 0x118);
    accuracy = (f32 *)((u8 *)car + 0x130);

    /* Simple LCG random */
    D_80143500 = D_80143500 * 1103515245 + 12345;
    rand = D_80143500;

    /* Error magnitude based on inverse accuracy */
    errorMag = (1.0f - *accuracy) * 0.2f;

    /* Add random steering error */
    *steer += ((f32)(rand & 0xFF) / 255.0f - 0.5f) * errorMag;

    /* Add random throttle variation */
    *throttle += ((f32)((rand >> 8) & 0xFF) / 255.0f - 0.5f) * errorMag * 0.5f;

    /* Clamp values */
    if (*steer > 1.0f) *steer = 1.0f;
    if (*steer < -1.0f) *steer = -1.0f;
    if (*throttle > 1.0f) *throttle = 1.0f;
    if (*throttle < 0.0f) *throttle = 0.0f;
}

/*
 * func_8010E8B4 (352 bytes)
 * AI catch up
 *
 * More aggressive catch-up logic for AI far behind
 */
void func_8010E8B4(void *car) {
    extern f32 D_80143404;  /* Player distance */
    f32 *carPos;
    f32 *throttle, *maxSpeed;
    f32 distBehind;

    if (car == NULL) {
        return;
    }

    carPos = (f32 *)((u8 *)car + 0x24);
    throttle = (f32 *)((u8 *)car + 0x118);
    maxSpeed = (f32 *)((u8 *)car + 0x124);

    /* Calculate how far behind player */
    /* Simplified: use Z position as proxy for track distance */
    distBehind = D_80143404 - carPos[2];

    if (distBehind > 100.0f) {
        /* Very far behind - maximum boost */
        *maxSpeed = 1.2f;
        *throttle = 1.0f;
    } else if (distBehind > 50.0f) {
        /* Moderately behind - some boost */
        *maxSpeed = 1.1f;
    }
}

/*
 * func_8010EA14 (2052 bytes)
 * Battle mode logic
 */
void func_8010EA14(void) {
    /* Battle mode - stub */
}

/*
 * func_8010F218 (2524 bytes)
 * Stunt mode logic
 */
void func_8010F218(void) {
    /* Stunt mode - stub */
}

/*
 * func_8010FBF4 (440 bytes)
 * Final cleanup game
 */
void func_8010FBF4(void) {
    /* Final cleanup - stub */
}
