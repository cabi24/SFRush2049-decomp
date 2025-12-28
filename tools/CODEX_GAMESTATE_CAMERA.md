# Codex Task: Game State Machine & Camera System Analysis

## Objective
Document the complete game state machine and camera system, mapping N64 implementation to arcade source.

---

# Part A: Game State Machine

## Task 1: State Diagram Creation

### Arcade Reference
- `reference/repos/rushtherock/game/game.c` - Main state machine
- `reference/repos/rushtherock/game/globals.h` - GState enum

### Arcade State Machine
```c
enum GState {
    ATTRACT,    // Demo/attract mode
    TRKSEL,     // Track selection
    CARSEL,     // Car selection
    PREPLAY,    // Pre-race setup
    COUNTDOWN,  // 3-2-1-GO
    PREPLAY2,   // Final setup
    PLAYGAME,   // Racing
    ENDGAME,    // Race complete
    GAMEOVER,   // Game over
    HISCORE     // High score entry
};
```

### N64 Functions to Map
- `game_loop` (main dispatcher)
- `state_handler` / `state_change`
- `attract_handler` / `countdown_start`
- `race_start` / `race_end`
- `hiscore_entry` / `game_exit`

### Deliverable
Create `docs/gamestate_mapping.md` with:
1. Complete state transition diagram (Mermaid or ASCII)
2. Per-state handler functions
3. Transition trigger conditions
4. State variable locations (gstate equivalent)

---

## Task 2: Attract Mode Analysis

### Arcade Files
- `reference/repos/rushtherock/game/attract.c` (72,881 bytes)

### Key Functions
```c
void attract(void)              // Main attract handler
void demo_attract(void)         // Demo playback
void show_high_scores(void)     // High score display
```

### N64 Equivalent
- Functions in 0x800DB* range
- Demo playback system
- Title screen logic

### Deliverable
Create `docs/gamestate_attract.md` with:
1. Attract mode sequence
2. Demo playback system
3. High score display timing
4. Input detection for exit

---

## Task 3: Race Flow Analysis

### Arcade Flow
1. PREPLAY: Load track, spawn cars, init HUD
2. COUNTDOWN: 3-2-1-GO sequence
3. PLAYGAME: Active racing
4. ENDGAME: Cross finish, stop car
5. GAMEOVER: Show results

### N64 Functions
- `race_start` / `race_end`
- `countdown_start` / `countdown_display`
- `lap_complete` / `race_finish`
- `position_update` / `standings_display`

### Deliverable
Create `docs/gamestate_race_flow.md` with:
1. Complete race initialization sequence
2. Countdown timing
3. Finish detection logic
4. Results screen flow

---

# Part B: Camera System

## Task 4: Camera Modes

### Arcade Files
- `reference/repos/rushtherock/game/camera.c` (39,720 bytes)
- `reference/repos/rushtherock/game/camera.v3.c` (51,167 bytes)

### Camera Modes
1. Chase cam (behind car)
2. Bumper cam (hood view)
3. Cockpit cam
4. TV cam (static positions)
5. Replay cam (cinematic)
6. Death cam (crash view)

### Key Arcade Functions
```c
void init_view(void)
void SelectCam(void)
void UpdateCam(void)
void SetMCamera(S16 mode)
void DeathCam(F32 pos[3], F32 uvs[3][3])
void circle_camera_around_car(...)
```

### N64 Functions
- `camera_init` / `camera_update` / `camera_reset`
- `camera_mode_set` / `camera_follow`
- `camera_shake` / `camera_zoom`
- `camera_target_set` / `camera_distance_set`

### Deliverable
Create `docs/camera_modes_mapping.md` with:
1. All camera modes documented
2. Mode switching logic
3. Per-mode parameters (distance, height, lag)
4. Input bindings for camera change

---

## Task 5: Camera Math

### Arcade Reference
- Smooth follow algorithm
- Lag/spring constants
- Collision avoidance
- FOV calculations

### N64 Functions
- `camera_lerp` / `camera_fov_set`
- `lookat_set` / `projection_set`
- `viewport_set` / `frustum_cull`

### Deliverable
Create `docs/camera_math_mapping.md` with:
1. Follow algorithm (spring-damper?)
2. Position interpolation
3. Look-at target calculation
4. FOV and near/far planes

---

## Task 6: Replay Camera System

### Analyze
- How replay cameras are selected
- Cinematic camera positions
- Camera cuts/transitions
- Ghost car following

### N64 Functions
- `replay_start` / `replay_frame`
- `ghost_init` / `ghost_update`

### Deliverable
Create `docs/camera_replay_mapping.md` with:
1. Replay camera selection algorithm
2. Cinematic camera positions
3. Cut timing logic
4. Ghost car camera behavior

---

## Analysis Commands

```bash
# Find state-related functions
grep -n "state_\|gstate\|ATTRACT\|PLAYGAME" src/game/game.c | head -30

# Find camera functions
grep -n "camera_\|cam_\|view_\|lookat" src/game/game.c | head -30

# Check camera.c
cat src/game/camera.c

# Find viewport functions
grep -n "viewport_\|frustum_\|fov_" src/game/game.c
```

## Success Criteria
- Complete state transition diagram
- All 6+ camera modes documented
- Camera math formulas extracted
- State handler function table
- Replay system flow documented
