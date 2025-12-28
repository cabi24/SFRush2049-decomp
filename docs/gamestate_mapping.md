# Game State Machine Mapping: Arcade to N64

This document maps the arcade "Rush The Rock" game state machine to the N64 "Rush 2049" implementation.

## 1. State Diagram

### Arcade State Flow (GState enum)
```
                            +----------+
                            | ATTRACT  |<--------------------------------+
                            +----+-----+                                  |
                                 |                                        |
                         [coin inserted]                                  |
                                 |                                        |
                                 v                                        |
                            +----------+                                  |
                            | TRKSEL   |                                  |
                            +----+-----+                                  |
                                 |                                        |
                         [track selected]                                 |
                                 |                                        |
                                 v                                        |
                            +----------+                                  |
                            | CARSEL   |                                  |
                            +----+-----+                                  |
                                 |                                        |
                         [car selected]                                   |
                                 |                                        |
                                 v                                        |
                            +----------+                                  |
                            | PREPLAY  |  <-- one-time per-race setup     |
                            +----+-----+                                  |
                                 |                                        |
                         [track loaded, cars init]                        |
                                 |                                        |
                                 v                                        |
                            +-----------+                                 |
                            | COUNTDOWN |  <-- 3..2..1..GO!               |
                            +----+------+                                 |
                                 |                                        |
                         [countdown ends]                                 |
                                 |                                        |
                                 v                                        |
                            +-----------+                                 |
                            | PREPLAY2  |  <-- final pre-race setup       |
                            +----+------+                                 |
                                 |                                        |
                         [race starts]                                    |
                                 |                                        |
                                 v                                        |
                            +----------+                                  |
                            | PLAYGAME |  <-- active racing               |
                            +----+-----+                                  |
                                 |                                        |
                     [race ends / time out]                               |
                                 |                                        |
                                 v                                        |
                            +----------+                                  |
                            | ENDGAME  |  <-- race cleanup                |
                            +----+-----+                                  |
                                 |                                        |
                         [cleanup done]                                   |
                                 |                                        |
                                 v                                        |
                            +----------+                                  |
                            | GAMEOVER |  <-- wait for abort/timeout      |
                            +----+-----+                                  |
                                 |                                        |
                     [abort or timeout]                                   |
                                 |                                        |
                                 v                                        |
                            +----------+                                  |
                            | HISCORE  |  <-- high score entry            |
                            +----+-----+                                  |
                                 |                                        |
                         [score entered]                                  |
                                 +----------------------------------------+
```

### N64 State System

The N64 uses a **bitmask-based state system** rather than a simple enum. States are encoded as bit flags in `D_801174B4` (current state) and `D_801174B8` (pending/requested state).

```
N64 State Bitmask Layout:
  Bit 0     (0x0001): INIT - Initial boot state
  Bit 1     (0x0002): SETUP - Setup/configuration
  Bit 2     (0x0004): MENU - Main menu
  Bit 3     (0x0008): SELECT - Track/car selection
  Bit 4     (0x0010): COUNTDOWN_PREP
  Bit 5     (0x0020): COUNTDOWN
  Bit 6     (0x0040): RACE_ACTIVE
  Bit 7     (0x0080): RACE_PAUSE
  Bit 8     (0x0100): RACE_END
  Bits 9-12 (0x1E00): Reserved
  Bit 13    (0x2000): REPLAY
  Bit 14    (0x4000): BATTLE mode flag
  Bits 15-17: High score bits (0x03FC0000)
  Bit 21    (0x200000): ATTRACT flag
  Bit 22    (0x400000): RACE flag
  Bit 26    (0x04000000): Sound disable flag
```

## 2. State Transition Table

### Arcade State Transitions

| From State | To State | Condition | Trigger |
|------------|----------|-----------|---------|
| ATTRACT | TRKSEL | `chk_start()` returns true | Coin inserted + Start pressed |
| ATTRACT | ATR_JOIN | Other players in TRKSEL | Network join opportunity |
| ATTRACT | ATR_TOURNEY | `tourney_setup` enabled | Tournament mode active |
| TRKSEL | CARSEL | Track selected | Timer expires or selection confirmed |
| CARSEL | PREPLAY | Car selected | Timer expires or selection confirmed |
| PREPLAY | COUNTDOWN | Track loaded | `loadTrack()` completes successfully |
| COUNTDOWN | PREPLAY2 | All cars ready + countdown timer | `all_in_cdown && CountDown()` finishes |
| PREPLAY2 | PLAYGAME | Immediate | Final setup complete |
| PLAYGAME | ENDGAME | Race complete or timeout | `coast_flag && are_all_cars_stopped()` |
| ENDGAME | GAMEOVER | Cleanup done | `do_endgame()` completes |
| GAMEOVER | HISCORE | Timeout or abort | `TimeOut()` or `edges & SW_ABORT` |
| HISCORE | ATTRACT | High score saved | `EnterHighScore()` completes |

### N64 State Transitions (via game_loop)

| Current Bitmask | Next Bitmask | Condition |
|-----------------|--------------|-----------|
| `0x00200000` (ATTRACT) | `0x00000008` (SELECT) | Start pressed, credits available |
| `0x00000008` (SELECT) | `0x00000010` (COUNTDOWN_PREP) | Selection complete |
| `0x00000010` | `0x00000020` (COUNTDOWN) | Prep complete |
| `0x00000020` | `0x00400000` (RACE) | Countdown ends |
| `0x00400000` | `0x00000100` (RACE_END) | Race finishes |
| `0x00000100` | `0x00040000` (HISCORE) | End cleanup done |
| `0x00040000` | `0x00200000` (ATTRACT) | High score complete |

## 3. Per-State Handler Mapping

### State Handler Functions

| Arcade State | Arcade Function | N64 Address | N64 Function Name | Notes |
|--------------|-----------------|-------------|-------------------|-------|
| ATTRACT | `attract()` | `0x800DB81C` | `attract_handler` | Demo/title screen loop |
| TRKSEL | `TrackSel()` | (in state machine) | `state_menu_handler` | Track selection UI |
| CARSEL | `CarSel()` | (in state machine) | `state_menu_handler` | Car selection UI |
| PREPLAY | `preplay()` | `0x800FBC30` | `preplay_setup` | One-time race setup |
| COUNTDOWN | `CountDown()` | (inline) | `countdown_handler` | Race countdown |
| PREPLAY2 | (inline in game()) | (inline) | (part of state change) | Final setup |
| PLAYGAME | `playgame()` | `0x800CA3B4` | `playgame_handler` | Active racing logic |
| ENDGAME | `do_endgame()` | (in game_loop) | (inline) | Race cleanup |
| GAMEOVER | `do_gameover()` | (in game_loop) | (inline) | Wait for next action |
| HISCORE | `EnterHighScore()` | `0x800FBF88` | `hiscore_handler` | High score entry |

### N64 State Handler Addresses (from symbol analysis)

| N64 Address | Handler Name | Handles Bitmask |
|-------------|--------------|-----------------|
| `0x800FBF90` | `state_init_handler` | `0x00040000` (GSTATE_INIT) |
| `0x800FBFE4` | `state_setup_handler` | `0x00080000` (GSTATE_SETUP) |
| `0x800FC0EC` | `state_menu_handler` | `0x00100000` (GSTATE_MENU) |
| `0x800FC038` | `state_camera_handler` | `0x00200000` (GSTATE_CAMERA) |
| `0x800FC228` | `state_ready_handler` | `0x00400000` (GSTATE_READY) |
| `0x800FC25C` | `state_play_handler` | `0x00800000` (GSTATE_PLAY) |
| `0x800FC89C` | `state_cars_handler` | `0x01000000` (GSTATE_CARS) |
| `0x800FC960` | `state_finish_handler` | `0x02000000` (GSTATE_FINISH) |

## 4. Key State Variables

### Arcade Variables (globals.h)

| Variable | Type | Purpose | Source Location |
|----------|------|---------|-----------------|
| `gstate` | `GState` | Current game state enum | `reference/repos/rushtherock/game/globals.h` |
| `edges` | `U32` | Button edge detection (just pressed) | globals.h |
| `levels` | `U32` | Current button states | globals.h |
| `IRQTIME` | `VS32` | Game millisecond counter | globals.h |
| `demo_game` | `S16` | Demo mode flag (0=disabled, 1=demo, 2=stop) | globals.h |
| `trackno` | `S16` | Current track number (0-6) | globals.h |
| `gThisNode` | `U8` | Local player slot number | globals.h |
| `attract_effects` | `BOOL` | Attract mode sound effects enabled | globals.h |
| `attract_music` | `BOOL` | Attract mode music enabled | globals.h |
| `tourney_setup` | `S8` | Tournament mode state | globals.h |
| `coast_flag` | `S8` | Set when time runs out, cars coasting | globals.h |
| `end_game_flag` | `S8` | Set when someone crosses finish | globals.h |
| `playitagain_time` | `S32` | Timer for "play it again" music | game.c |
| `whats_your_name_time` | `S32` | Timer for name entry music | game.c |

### N64 Variables

| Address | Name | Type | Purpose |
|---------|------|------|---------|
| `0x801146EC` | `gstate` | `s8` | Simple game state byte (0-15) |
| `0x801174B4` | `D_801174B4` | `u32` | Current state bitmask |
| `0x801174B8` | `D_801174B8` | `u32` | Pending/requested state bitmask |
| `0x80142AFC` | `frame_counter` | `u32` | Frame counter |
| `0x801146E8` | `D_801146E8` | `s32` | Sound handle |
| `0x801170FC` | `D_801170FC` | `s32` | Render enable flag |
| `0x80152818` | `car_array` | array | Car data array |
| `0x801461D0` | `game_struct` | struct | Main game structure |

### N64 gstate Values (byte at 0x801146EC)

| Value | State |
|-------|-------|
| 0 | ATTRACT |
| 1 | MENU/SELECT |
| 2 | TRACK_SELECT |
| 3 | CAR_SELECT |
| 4 | PREPLAY |
| 5 | COUNTDOWN |
| 6 | PLAYGAME |
| 7 | ENDGAME |
| 8 | GAMEOVER |
| 9 | HISCORE |
| 10 | REPLAY |
| 11+ | Reserved/special modes |

## 5. Race Flow Sequence

### Arcade Race Flow (from game.c)

```
1. ATTRACT mode:
   - attract() loops: title screens, demos, high scores
   - Checks for coin + start via chk_start()
   - On start: sets gstate = TRKSEL

2. TRKSEL (Track Select):
   - TrackSel() runs UI
   - Player selects track
   - Sets trackno, transitions to CARSEL

3. CARSEL (Car Select):
   - CarSel() runs UI
   - Player selects car type
   - Sets car data, transitions to PREPLAY

4. PREPLAY (Pre-Play Setup):
   - preplay() called with (useHud=true, canAbort=false)
   - Initializes moobs, clears old car objects
   - Loads track via loadTrack()
   - Initializes checkpoints (InitCPS)
   - Initializes cars (init_cars, InitDrones)
   - Initializes physics (Init_MDrive)
   - Sets gstate = COUNTDOWN

5. COUNTDOWN:
   - SetCountdownTimer(10 * ONE_SEC)
   - Waits for all players to be in COUNTDOWN state (all_in_cdown)
   - Update_MDrive() runs physics
   - CountDown() decrements timer, displays 3..2..1
   - When timer done: gstate = PREPLAY2

6. PREPLAY2 (Final Setup):
   - SetCountdownTimerAt0() with track start time
   - Plays S_RUSH sound
   - init_cp_time() for checkpoint timing
   - StartRadio() for in-race music
   - Sets gstate = PLAYGAME

7. PLAYGAME (Active Racing):
   - playgame() main loop
   - Update_MDrive() - physics
   - DoDrones() - AI
   - CheckCPs() - checkpoint detection
   - CheckCrash() - collision
   - UpdateVisuals() - graphics
   - TargetCheck() - target hit detection
   - Respawn handling on car death
   - Coast handling when time runs out
   - On timeout + stopped: gstate = ENDGAME

8. ENDGAME:
   - do_endgame() called
   - End_MDrive() for all cars
   - modelstop() - stop physics sim
   - freewheel() / gSWforce = 0 - release wheel
   - StopEngineSound()
   - ShowGameOver(true)
   - SetCountdownTimer() for GAMEOVER timeout
   - gstate = GAMEOVER

9. GAMEOVER:
   - do_gameover() called
   - Waits for TimeOut() or abort button
   - On exit: CleanupAfterGame()
   - reinit_zoid() for attract model
   - gstate = HISCORE

10. HISCORE:
    - ResetGameLink()
    - EnterHighScore() with track, score, flags
    - Turn off all lights
    - gstate = ATTRACT (loop back)
```

### N64 Race Flow (from game_loop)

```
1. game_loop() called every frame:
   - First time: clear frame counter, set init flag

2. State change check:
   - Compare D_801174B4 (current) vs D_801174B8 (pending)
   - If different: state transition in progress

3. During state change:
   - attract_or_transition() for render state
   - process_inputs() for PDU/input handling
   - Sound start/stop based on state flags
   - playgame_handler() for main state logic

4. State-specific processing:
   - If ATTRACT bits set (0x00200000 or 0x00400000):
     - attract_handler() for demo/title
     - If render enabled: objects_update, physics_update, effects_update, render_scene
   - If HISCORE bits set (0x03FC0000):
     - hiscore_entry()
   - If steady state (current == pending):
     - countdown_start()
     - objects_update()
     - physics_update()
     - effects_update()
     - render_scene()

5. Frame counter increment at end
```

## 6. Attract Mode Sub-States

### Arcade AttractMode enum (attract.c)

| Value | State | Purpose |
|-------|-------|---------|
| ATR_HSENTRY | 0 | High score name entry |
| ATR_HISCORE1-7 | 1-7 | High score display per track |
| ATR_CREDITS | 8 | Developer credits |
| ATR_OCREDITS | 9 | Old credits screen |
| ATR_MIRROR | 10 | Mirror mode promo |
| ATR_CARS | 11 | Car showcase |
| ATR_3DFXLOGO | 12 | 3dfx logo |
| ATR_TEAM | 13 | Team credits |
| ATR_OTEAM | 14 | Old team screen |
| ATR_ATARILOGO/2/3 | 15-17 | Atari/publisher logos |
| ATR_MOVIE2 | 18 | Car 1 rotating movie |
| ATR_MOVIE3 | 19 | Car 2 rotating movie |
| ATR_MOVIE4 | 20 | Attract movie |
| ATR_MOVIE5/52/53 | 21-23 | Rush logo movies |
| ATR_ADVISORY | 24 | Legal/advisory screen |
| ATR_ADVERT | 25 | Advertisement screen |
| ATR_DEMO/DEMO2 | 26-27 | Demo gameplay |
| ATR_TRANSIT | 28 | Transition screen |
| ATR_TOURNEY | 29 | Tournament setup |
| ATR_JOIN | 30 | "Join Now" prompt |
| ATR_GAMESTAT | 31 | Game statistics |

### N64 Attract States (attract.h)

| Value | State | Purpose |
|-------|-------|---------|
| ATTRACT_STATE_INIT | 0 | Initial state |
| ATTRACT_STATE_LOGO | 1 | Title logo display |
| ATTRACT_STATE_CREDITS | 2 | Game credits |
| ATTRACT_STATE_HISCORE | 3 | High score table |
| ATTRACT_STATE_DEMO | 4 | Demo gameplay |
| ATTRACT_STATE_JOIN | 5 | "Press Start" prompt |
| ATTRACT_STATE_TRANSITION | 6 | State transition |

## 7. Key Differences: Arcade vs N64

| Aspect | Arcade | N64 |
|--------|--------|-----|
| State encoding | Simple enum (0-9) | Bitmask + byte |
| Main loop | `game()` switch statement | `game_loop()` with flag checks |
| Network | Multi-cabinet link support | Single-player or 4-player split |
| Attract screens | 31+ different screens | Simplified ~7 states |
| High scores | Per-track tables, 7 tracks | Per-track tables, 4 tracks |
| Tournament mode | Full support | Reduced/removed |
| Sound system | SOUND() macro | `sound_start()`/`sound_stop()` |
| Rendering | 3dfx Glide | RDP/RSP microcode |
| Physics | Model task | Inline in game loop |
| Frame timing | IRQTIME (1ms resolution) | Frame counter (60fps) |

## 8. Source File Mapping

| Arcade Source | N64 Equivalent | Purpose |
|---------------|----------------|---------|
| `game/game.c` | `src/game/game.c` | Main game loop and state machine |
| `game/attract.c` | `src/game/attract.c` | Attract mode |
| `game/select.c` | `src/game/menu.c` | Track/car selection |
| `game/checkpoint.c` | `src/game/checkpoint.c` | Checkpoint logic |
| `game/cars.c` | `src/game/car.c` | Car physics |
| `game/drones.c` | `src/game/drone.c` | AI drones |
| `game/camera.c` | `src/game/camera.c` | Camera system |
| `game/mdrive.c` | `src/game/physics.c` | Physics model |
| `game/hiscore.c` | `src/game/hiscore.c` | High score entry |
| `game/globals.h` | `include/game/game.h` | Global definitions |
| `game/game.h` | `include/game/game.h` | GState enum |

## 9. References

- Arcade source: `reference/repos/rushtherock/game/game.c` (lines 605-907)
- Arcade GState enum: `reference/repos/rushtherock/game/game.h` (lines 104-117)
- Arcade globals: `reference/repos/rushtherock/game/globals.h`
- N64 game loop: `src/game/game.c:game_loop()` (func_800FD464)
- N64 state handlers: `src/game/game.c` (various)
- Symbol addresses: `docs/all_symbols.txt`
