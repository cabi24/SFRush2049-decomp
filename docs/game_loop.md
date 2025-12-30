# Rush 2049 N64 - Main Game Loop Documentation

This document describes the main game loop structure of San Francisco Rush 2049 for N64, based on analysis of the decompiled `func_800FD464` (`game_loop`) in `src/game/game.c` and cross-referenced with the arcade source at `reference/repos/rushtherock/game/game.c`.

## Table of Contents

1. [Overview](#overview)
2. [Thread Architecture](#thread-architecture)
3. [State Machine](#state-machine)
4. [Per-Frame Update Order](#per-frame-update-order)
5. [Subsystem Calls](#subsystem-calls)
6. [Timing and VSync](#timing-and-vsync)
7. [State Transitions](#state-transitions)
8. [Arcade vs N64 Comparison](#arcade-vs-n64-comparison)

---

## Overview

The Rush 2049 N64 game loop follows a state machine architecture similar to the arcade version but adapted for the N64's multi-threaded OS. The main game loop runs in **Thread 7** and is called every frame from `game_thread_entry()`.

### Key Files

| File | Purpose |
|------|---------|
| `src/game/game.c` | Main game loop and state handlers |
| `src/game/init.c` | Thread creation and initialization |
| `src/game/state.c` | State machine logic |
| `include/game/gstate.h` | State definitions |
| `reference/repos/rushtherock/game/game.c` | Arcade reference |

### Key Functions

| N64 Function | Address | Arcade Equivalent | Purpose |
|--------------|---------|-------------------|---------|
| `game_loop` | 0x800FD464 | `game()` | Main per-frame loop |
| `game_mode_handler` | 0x800C9AE0 | Part of `game()` | Steady-state handler |
| `playgame_handler` | 0x800CA3B4 | `playgame()` | Active gameplay handler |
| `game_init` (init.c) | 0x80002238 | N/A (OS-specific) | Thread/queue setup |

---

## Thread Architecture

The N64 version uses a multi-threaded architecture:

```
+------------------+     +------------------+     +------------------+
|   Thread 1       |     |   Thread 6       |     |   Thread 7       |
|   (Idle)         |---->|   (Init)         |---->|   (Game Loop)    |
|   Priority: 2    |     |   Priority: 4    |     |   Priority: 5    |
+------------------+     +------------------+     +------------------+
                                  |
                                  v
                         +------------------+     +------------------+
                         |   Thread 8       |     |   Thread 5       |
                         |   (Audio)        |     |   (Render)       |
                         |   Priority: 3    |     |   Priority: 7    |
                         +------------------+     +------------------+
```

**Thread 7 (Main Game)** runs `game_thread_entry()` which contains an infinite loop calling `game_loop()`:

```c
static void game_thread_entry(void *arg) {
    __setfpcsr(0x1000E00);  /* Enable FPU exceptions */
    for (;;) {
        game_loop();        /* func_800FD464 */
    }
}
```

---

## State Machine

### N64 State Implementation

The N64 version uses a **bitmask-based** state system instead of the arcade's simple enum. State flags are stored in `gstate_mask` (32-bit):

```c
/* include/game/gstate.h */
#define GSTATE_INIT     0x00040000
#define GSTATE_SETUP    0x00080000
#define GSTATE_MENU     0x00100000
#define GSTATE_CAMERA   0x00200000
#define GSTATE_READY    0x00400000
#define GSTATE_PLAY     0x00800000
#define GSTATE_CARS     0x01000000
#define GSTATE_FINISH   0x02000000
```

### Arcade State Enum (for reference)

The arcade uses a simple enum:

```c
typedef enum GState {
    ATTRACT,    /* 0 - Attract mode / demo */
    TRKSEL,     /* 1 - Track selection */
    CARSEL,     /* 2 - Car selection */
    PLAYGAME,   /* 3 - Active racing */
    ENDGAME,    /* 4 - Race complete */
    GAMEOVER,   /* 5 - Game over screen */
    HISCORE,    /* 6 - High score entry */
    PREPLAY,    /* 7 - Pre-race setup */
    PREPLAY2,   /* 8 - Final race prep */
    COUNTDOWN,  /* 9 - 3-2-1 countdown */
    NUM_GAME_STATES
} GState;
```

### State Machine Diagram

```
                              +---------------+
                              |    ATTRACT    |<--------------+
                              | (Demo Mode)   |               |
                              +-------+-------+               |
                                      |                       |
                           [COIN INSERT / START]              |
                                      |                       |
                                      v                       |
                              +---------------+               |
                              |    TRKSEL     |               |
                              | Track Select  |               |
                              +-------+-------+               |
                                      |                       |
                                [SELECT TRACK]                |
                                      |                       |
                                      v                       |
                              +---------------+               |
                              |    CARSEL     |               |
                              | Car Select    |               |
                              +-------+-------+               |
                                      |                       |
                                 [SELECT CAR]                 |
                                      |                       |
                                      v                       |
                              +---------------+               |
                              |    PREPLAY    |               |
                              | Load Track    |               |
                              +-------+-------+               |
                                      |                       |
                              [TRACK LOADED]                  |
                                      |                       |
                                      v                       |
                              +---------------+               |
                              |   COUNTDOWN   |               |
                              |   3...2...1   |               |
                              +-------+-------+               |
                                      |                       |
                                 [COUNTDOWN=0]                |
                                      |                       |
                                      v                       |
                              +---------------+               |
                              |   PREPLAY2    |               |
                              | Final Setup   |               |
                              +-------+-------+               |
                                      |                       |
                                  [READY]                     |
                                      |                       |
                                      v                       |
                              +---------------+               |
                              |   PLAYGAME    |               |
                              |   Racing!     |<----+         |
                              +-------+-------+     |         |
                                      |             |         |
                      +---------------+------+      |         |
                      |                      |      |         |
               [CHECKPOINT]            [TIME OUT]   |         |
               [LAP COMPLETE]                |      |         |
                      |                      |      |         |
                      +---[ADD TIME]---------+      |         |
                                      |             |         |
                               [RACE FINISHED]      |         |
                               [NO TIME LEFT]       |         |
                                      |             |         |
                                      v             |         |
                              +---------------+     |         |
                              |    ENDGAME    |     |         |
                              | Race Results  |     |         |
                              +-------+-------+     |         |
                                      |             |         |
                                [SHOW RESULTS]      |         |
                                      |             |         |
                                      v             |         |
                              +---------------+     |         |
                              |   GAMEOVER    |-----+         |
                              | Continue?     |  [CONTINUE]   |
                              +-------+-------+               |
                                      |                       |
                              [NO CONTINUE]                   |
                              [HIGH SCORE]                    |
                                      |                       |
                                      v                       |
                              +---------------+               |
                              |    HISCORE    |               |
                              | Enter Name    |---------------+
                              +---------------+
```

---

## Per-Frame Update Order

### N64 game_loop() Frame Structure

The `game_loop()` function (func_800FD464) executes the following steps each frame:

```
Frame Start
    |
    v
+---------------------------+
| 1. First-Time Init Check  |  if (gstate_byte == 0) { init frame_counter }
+---------------------------+
    |
    v
+---------------------------+
| 2. State Change Check     |  if (gstate_cur == gstate_next) goto steady_state
+---------------------------+
    |
    v (if state changing)
+---------------------------+
| 3. Wait for Resources     |  while (wait_flag != 0) { spin }
+---------------------------+
    |
    v
+---------------------------+
| 4. Clear Render State     |  attract_render_state = 0
+---------------------------+
    |
    v
+---------------------------+
| 5. Transition Handler     |  attract_or_transition()
+---------------------------+
    |
    v
+---------------------------+
| 6. Process Inputs/PDUs    |  process_inputs()
+---------------------------+
    |
    v
+---------------------------+
| 7. Timing Check           |  elapsed_time = timing_tick - timing_constant
+---------------------------+
    |
    v
+---------------------------+
| 8. Sound Management       |  Start/stop sounds based on state
+---------------------------+
    |
    v
+---------------------------+
| 9. State Handler          |  playgame_handler() - handles transitions
+---------------------------+
    |
    v
+---------------------------+
| 10. Active State Updates  |  state_dispatch() for active states
+---------------------------+
    |
    v
+---------------------------+
| 11. Race Updates          |  attract_handler() for race states
|    - objects_update()     |
|    - physics_update()     |
|    - effects_update()     |
|    - render_scene()       |
+---------------------------+
    |
    v
+---------------------------+
| 12. High Score Updates    |  hiscore_entry() if needed
+---------------------------+
    |
    v
+---------------------------+
| 13. Steady-State Updates  |  countdown_start(), update functions
+---------------------------+
    |
    v
+---------------------------+
| 14. Increment Frame       |  frame_counter++
+---------------------------+
    |
    v
Frame End
```

### Arcade game() Frame Structure (for comparison)

```c
void game(void) {
    /* Performance timing init */
    DBG_perf_frame_init();

    /* First-time mode initialization */
    if (in_game_mode == false) {
        test_sounds();
        SOUND(S_ATTRACT_MODE);
        in_game_mode = true;
        MarkAlive(gThisNode, IDLE);
    }

    /* Network PDU processing */
    ProcessPDUs(false);

    /* Input handling */
    get_switches(Do_it);
    CheckTourneyBox();

    /* Heartbeat (1Hz) */
    if ((IRQTIME - gHeartTime) > ONE_SEC) {
        gHeartTime = IRQTIME;
        Heartbeat(true);
    }

    /* Credits/coins */
    get_credits();

    /* Volume control */
    aud_dyn_adjvol(gstate == ATTRACT);

    /* Stats logging */
    game_stats();

    /* Main state switch */
    switch (gstate) {
        case ATTRACT:   attract();      break;
        case TRKSEL:    TrackSel();     break;
        case CARSEL:    CarSel();       break;
        case PREPLAY:   preplay(...);   break;
        case COUNTDOWN: CountDown();    break;
        case PREPLAY2:  /* final */     break;
        case PLAYGAME:  playgame();     break;
        case ENDGAME:   do_endgame();   break;
        case GAMEOVER:  do_gameover();  break;
        case HISCORE:   EnterHighScore(...); break;
    }

    /* End-of-frame rendering */
    DrawBlits();
    UpdateEnvirons();
    ZOID_EndFrame();

    /* Debug output */
    DBG_print_debug();
}
```

---

## Subsystem Calls

### Per-Frame Subsystem Update Order

| Order | Subsystem | N64 Function | Arcade Function | Description |
|-------|-----------|--------------|-----------------|-------------|
| 1 | Input | `process_inputs()` | `get_switches()` | Read controller/buttons |
| 2 | Network | (in process_inputs) | `ProcessPDUs()` | Network sync (arcade) |
| 3 | Timer | `timer_sync()` | Heartbeat/IRQTIME | Time management |
| 4 | Sound | `sound_start/stop()` | `SOUND()` macro | Audio triggers |
| 5 | State | `playgame_handler()` | state switch | State machine |
| 6 | Objects | `objects_update()` | per-state handlers | Game objects |
| 7 | Physics | `physics_update()` | `Update_MDrive()` | Car physics |
| 8 | Drones | (in physics) | `DoDrones()` | AI cars |
| 9 | Checkpoints | (in physics) | `CheckCPs()` | Race progress |
| 10 | Effects | `effects_update()` | `UpdateVisuals()` | Particles/VFX |
| 11 | Camera | (in render_scene) | per-state | Camera update |
| 12 | Render | `render_scene()` | `ZOID_EndFrame()` | Display list |
| 13 | VSync | `vsync_wait()` | implicit | Wait for VBlank |

### Subsystem Call Graph

```
game_loop()
    |
    +-- game_mode_handler()          [Steady state]
    |       |
    |       +-- state_set/get()      [Memory operations]
    |       +-- timer_sync()         [Timing]
    |       +-- state_handler()      [Input processing]
    |
    +-- attract_or_transition()      [State change]
    |
    +-- process_inputs()             [Controller input]
    |
    +-- playgame_handler()           [State transitions]
    |       |
    |       +-- state_change_preprocess()
    |       +-- camera_set_pos()
    |       +-- hud_setup()
    |       +-- hud_init()
    |       +-- display_enable()
    |       +-- game_init_state()
    |       +-- player_state_set()
    |       +-- player_mode_set()
    |       +-- visual_objects_update()
    |
    +-- attract_handler()            [Race states]
    |       |
    |       +-- objects_update()
    |       +-- physics_update()
    |       +-- effects_update()
    |       +-- render_scene()
    |
    +-- hiscore_entry()              [High score]
    |
    +-- countdown_display()          [Countdown timer]
```

---

## Timing and VSync

### Frame Rate

- **Target**: 60 Hz (NTSC) or 50 Hz (PAL)
- **VBlank sync**: Uses `osRecvMesg()` on VI message queue
- **Timer resolution**: CPU counter at 46.875 MHz

### Timing Constants

```c
#define TIMER_HZ        60          /* Frames per second */
#define ONE_SEC         60          /* Frames (N64) or 1000ms (arcade) */
#define COUNT_PER_SEC   46875000    /* N64 CPU counter frequency */
```

### VSync Implementation

```c
void vsync_wait(void) {
    extern OSMesgQueue vi_message_queue;
    OSMesg msg;

    /* Block until VI interrupt signals VBlank */
    osRecvMesg(&vi_message_queue, &msg, OS_MESG_BLOCK);
}
```

### Frame Timing Flow

```
+-------------------+
|   Frame Start     |
+-------------------+
         |
         v
+-------------------+
| game_loop() runs  |
| - State machine   |
| - Physics         |
| - Rendering       |
+-------------------+
         |
         v
+-------------------+
| display_list_     |
| flush()           |
| - Submit to RSP   |
+-------------------+
         |
         v
+-------------------+
| vsync_wait()      |
| - Block on        |
|   VI message      |
+-------------------+
         |
         v (VBlank interrupt)
+-------------------+
| osViSwapBuffer()  |
| - Buffer swap     |
+-------------------+
         |
         v
+-------------------+
|   Next Frame      |
+-------------------+
```

---

## State Transitions

### Transition Triggers

| From State | To State | Trigger |
|------------|----------|---------|
| ATTRACT | TRKSEL | Coin insert + Start button |
| TRKSEL | CARSEL | Track selected (confirm button) |
| CARSEL | PREPLAY | Car selected (confirm button) |
| PREPLAY | COUNTDOWN | Track loaded successfully |
| COUNTDOWN | PREPLAY2 | Countdown reaches 0 |
| PREPLAY2 | PLAYGAME | Immediate (final setup done) |
| PLAYGAME | ENDGAME | Race finished OR time expired |
| ENDGAME | GAMEOVER | Results displayed |
| GAMEOVER | HISCORE | No continue OR high score achieved |
| GAMEOVER | TRKSEL | Continue pressed (with credits) |
| HISCORE | ATTRACT | Name entry complete |

### Transition Code Examples

**PREPLAY to COUNTDOWN** (Arcade):
```c
case PREPLAY:
    preplay(true, false);
    SetCountdownTimer(10 * ONE_SEC);
    check_sound_time(Initialize);
    flash_music_time = IRQTIME;
    countdown_state = 2;
    gstate = COUNTDOWN;
    break;
```

**COUNTDOWN to PLAYGAME** (Arcade):
```c
case PREPLAY2:
    SetCountdownTimerAt0(track_data[trackno].start_time[difficulty[trackno]]);
    SOUND(S_RUSH);
    init_cp_time();
    gstate = PLAYGAME;
    StartRadio(radio_station);
    EnableRadioHUD();
    break;
```

**PLAYGAME to ENDGAME** (Arcade):
```c
if (coast_flag) {
    if (are_all_cars_stopped()) {
        PutEventReportPDU(Event_GameOver, ...);
        gstate = ENDGAME;
    }
}
```

---

## Arcade vs N64 Comparison

### Key Differences

| Aspect | Arcade | N64 |
|--------|--------|-----|
| **State Storage** | Simple enum (`GState gstate`) | Bitmask (`u32 gstate_mask`) |
| **Threading** | Single-threaded with interrupts | Multi-threaded (5 threads) |
| **Graphics** | 3dfx Voodoo (ZOID library) | N64 RDP (F3DEX2 microcode) |
| **Timing Base** | IRQTIME (milliseconds) | Frame counter (60fps ticks) |
| **Network** | DIS protocol, multi-cabinet | Single-player/local only |
| **Audio** | Arcade sound board | N64 audio subsystem |
| **Memory** | Flat memory model | TLB-mapped segments |

### Preserved Concepts

Despite platform differences, these core concepts are preserved:

1. **State machine structure** - Same logical states (ATTRACT, PLAYGAME, etc.)
2. **Per-frame update order** - Input -> Logic -> Physics -> Render
3. **Countdown timer** - 10 second pre-race countdown
4. **Checkpoint timing** - Time bonus on checkpoint/lap
5. **Car physics** - MDrive physics model (Update_MDrive)
6. **AI drones** - Same pathfinding (maxpath) concepts

### Function Mapping

| Arcade Function | N64 Equivalent | Notes |
|-----------------|----------------|-------|
| `game()` | `game_loop()` | Main per-frame function |
| `game_init()` | `game_init()` (init.c) | Different scope (OS vs gameplay) |
| `playgame()` | `playgame_handler()` | Active race logic |
| `preplay()` | Part of state handlers | Track/race setup |
| `attract()` | `attract_handler()` | Demo mode |
| `CountDown()` | `countdown_display()` | 3-2-1 display |
| `Update_MDrive()` | `physics_update()` | Car physics |
| `DoDrones()` | `drone_update_all()` | AI cars |
| `CheckCPs()` | Checkpoint functions | Race progress |
| `ZOID_EndFrame()` | `display_list_flush()` | Graphics submit |

---

## References

- `/home/cburnes/projects/rush2049-decomp/src/game/game.c` - N64 decompiled source
- `/home/cburnes/projects/rush2049-decomp/src/game/init.c` - Thread setup
- `/home/cburnes/projects/rush2049-decomp/src/game/state.c` - State machine
- `/home/cburnes/projects/rush2049-decomp/include/game/gstate.h` - State definitions
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/game.c` - Arcade source
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/game.h` - Arcade definitions
