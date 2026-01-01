/**
 * @file init.h
 * @brief Game initialization declarations
 *
 * Based on reference/repos/rushtherock/game/init.c
 * Contains initialization routine declarations and related types.
 *
 * Copyright 1996 Time Warner Interactive / Atari Games.
 * Decompilation for educational and preservation purposes.
 */

#ifndef _INIT_H
#define _INIT_H

#include "types.h"
#include "game/game.h"

/*===========================================================================*/
/*                              CONSTANTS                                     */
/*===========================================================================*/

/* Memory allocation sizes (arcade values) */
#define NET_MEM_BYTES       (30 * 1024)
#define GUTS_STACK_BYTES    (32 * 1024)

/* Model task AST level */
#define MODEL_AST_LEVEL     7   /* MAX_AST_QUEUES - 1 */

/* ROM addresses for compressed data (N64 specific) */
#define ROM_COMPRESSED_DATA 0xB0CB10

/* FPU control/status register value */
#define FPCSR_INIT          0x1000E00

/*===========================================================================*/
/*                              TIMING                                        */
/*===========================================================================*/

/* Timer interrupt interval (microseconds) */
#define MODEL_INITIAL_DELAY     50000   /* 50ms initial delay */
#define MODEL_PERIOD_USEC       5000    /* 5ms model update period */
#define MODEL_MIN_RESCHEDULE    500     /* 0.5ms minimum reschedule */

/*===========================================================================*/
/*                              GLOBALS                                       */
/*===========================================================================*/

/* Time tracking variables (from arcade init.c) */
extern f32 gCurGameTime;        /* Current game time */
extern f32 gLastGameTime;       /* Last frame's game time */
extern f32 gGameLoopTime;       /* Time for one game loop iteration */
extern f32 gGameExecTime;       /* Execution time for game logic */

/* Frame tracking (arcade) */
extern volatile u32 frames;     /* Frame counter */
extern volatile u32 gotvb;      /* VBlank received flag */

/* Memory pool (arcade) */
extern u32 mem_pool_start;      /* Start of memory pool */
extern u32 net_mem_start;       /* Start of network memory */

/* Model task variables (arcade) */
extern s32 game_block;          /* Model blocked count */
extern u32 mast_period;         /* Model AST period */
extern u32 last_mast_time;      /* Last model AST time */

/* Last track loaded index */
extern s16 last_track_loaded;

/*===========================================================================*/
/*                         FUNCTION PROTOTYPES                               */
/*===========================================================================*/

/**
 * Game entry point (reset vector)
 * Called from boot code after system initialization.
 * Arcade: Sets up threads and enters main game loop.
 * N64: Already implemented in src/game/init.c
 */
void start(void);

/**
 * Main game loop
 * Infinite loop that calls game() each frame.
 * Handles timing and VBlank synchronization.
 */
void game_loop(void);

/**
 * Powerup initialization
 * Initializes all game subsystems at power-on.
 * Called once before entering the main game loop.
 */
void init(void);

/**
 * Game state initialization
 * Initializes game options, high scores, and state machine.
 * Called each time a new game session starts.
 */
void game_init(void);

/**
 * Late game initialization
 * Called after threads are started but before main loop.
 * Sets up remaining subsystems.
 */
void game_late_init(void);

/**
 * Re-initialize graphics/rendering parameters
 * Resets view angle, ambient levels, and light direction.
 * Called from init() and when graphics need reset.
 */
void reinit_graphics(void);

/**
 * Start the model update task
 * Schedules the first model iteration.
 * @param time Initial delay in microseconds
 * @param task_param Task parameters (unused)
 */
void model_act_start(s32 time, void *task_param);

/**
 * Initialize model task data
 * Called once at powerup to set up model subsystem.
 */
void init_model_task(void);

/**
 * Model iteration
 * Called periodically to update physics/simulation.
 */
void model_iteration(void);

/**
 * Initialize timer interrupt
 * Sets up the timer IRQ for model updates.
 */
void init_timer_irq(void);

/**
 * Initialize global variables
 * Resets all global game state to defaults.
 */
void init_glob(void);

/**
 * Initialize track selection
 * Sets up track selection menu state.
 */
void init_track_sel(void);

/**
 * Initialize camera
 * Resets camera position and orientation.
 */
void zero_camera(void);

/**
 * Initialize blitter structures
 * Sets up 2D blitting system.
 */
void init_blits(void);

/**
 * Initialize attract mode
 * Sets up attract mode demo playback.
 */
void init_attract(void);

/**
 * Initialize mobile objects (MOOBS)
 * Sets up mobile object system for sounds/effects.
 */
void init_moobs(void);

/**
 * Initialize drone sound handles
 * Allocates sound channels for AI drones.
 */
void init_drone_handles(void);

/**
 * Initialize car sound handles
 * Allocates sound channels for player cars.
 */
void init_car_sound_handles(void);

/**
 * Initialize hint table
 * Sets up the loading hints system.
 */
void init_hint_table(void);

/**
 * Initialize game statistics
 * Resets all game statistics counters.
 */
void init_game_stats(void);

/**
 * Load initial data from storage
 * Loads startup data from ROM/NVRAM.
 */
void load_init(void);

/**
 * Initialize debug features
 * Sets up debug display and logging.
 */
void dbg_init_debug(void);

/**
 * Initialize memory handler
 * Sets up heap memory management.
 * @param pool_start Start address of memory pool
 * @param stack_size Size of GUTS stack
 */
void init_mem_handler(u32 pool_start, u32 stack_size);

/**
 * Initialize error handling
 * Sets up error recovery and reporting.
 */
void init_error(void);

/**
 * Initialize string system
 * Sets up string tables and localization.
 */
void init_strings(void);

/**
 * Initialize potentiometer inputs
 * Calibrates analog inputs (gas, brake, wheel).
 */
void pots_init(void);

/**
 * Initialize model subsystem
 * Performs powerup initialization of physics model.
 */
void model_init(void);

/*===========================================================================*/
/*                      N64-SPECIFIC FUNCTIONS                               */
/*===========================================================================*/

/**
 * Main entry point (N64)
 * Called from entrypoint after BSS clear.
 * Sets up OS and spawns idle thread.
 */
void main(void *arg);

/**
 * Audio thread entry point
 * Handles audio processing in separate thread.
 */
void audio_thread_proc(void *arg);

/**
 * Render thread entry point
 * Handles graphics rendering in separate thread.
 */
void render_thread_entry(void *arg);

/**
 * Sound initialization
 * Initializes audio subsystem.
 */
void sound_init(void);

/**
 * Start audio playback
 * Begins audio processing after initialization.
 */
void audio_start(void);

/**
 * Per-frame game update
 * Called each frame to update game state.
 */
void game_frame_update(void);

#endif /* _INIT_H */
