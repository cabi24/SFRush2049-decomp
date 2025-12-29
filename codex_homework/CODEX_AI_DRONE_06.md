# Codex Homework: AI/Drone System - Part 6 of 6
## Main Loop Integration & N64 Function Matching

**Estimated Time**: 4-5 hours
**Difficulty**: Hard
**Prerequisites**: CODEX_AI_DRONE_01 through 05 completed

---

## Objective

Integrate all drone systems into the main game loop and match the actual N64 functions from the disassembly. This is the final step that makes drones actually work in-game.

---

## Background

Now that we have all the subsystems implemented:
1. Path data structures (Part 1)
2. Path interval finding (Part 2)
3. Steering/speed control (Part 3)
4. Collision avoidance (Part 4)
5. Rubber-band catchup (Part 5)

We need to:
1. Wire them into the main game loop
2. Match actual N64 function signatures
3. Test and debug

---

## N64 Functions to Match

The main AI function in game.c:

| N64 Function | Address | Size | Purpose |
|--------------|---------|------|---------|
| `drone_ai_update()` | 0x80009387 | 3440 bytes | Main per-frame drone update |
| `ai_difficulty_adjust()` | 0x80028107 | ~400 bytes | Difficulty scaling |
| `ai_difficulty_set()` | 0x80041904 | ~200 bytes | Set difficulty params |

---

## Tasks

### Task 1: Implement Main DoDrones() Function

**Arcade reference**: `drones.c:253`

```c
/**
 * Main per-frame drone update
 * Called from game loop every frame during racing
 *
 * Based on arcade: drones.c:DoDrones()
 */
void drone_update_all(void) {
    s32 i;

    // Skip if race not active
    if (gstate != GSTATE_PLAYGAME && gstate != GSTATE_COUNTDOWN) {
        return;
    }

    // Update catchup system
    if (use_catchup) {
        drone_set_catchup();
    } else {
        drone_no_catchup();
    }

    // Update path tracking for player (for distance/position calculations)
    for (i = 0; i < num_humans; i++) {
        s32 human_idx = human_indices[i];
        drone_do_maxpath(human_idx);
    }

    // Assign drone behaviors based on positions
    drone_assign_all();

    // Update each active drone
    for (i = 0; i < num_active_cars; i++) {
        if (drone_ctl[i].drone_type == DRONE_TYPE_DRONE &&
            drone_ctl[i].is_active &&
            drone_ctl[i].we_control) {

            // Generate AI inputs
            drone_maxpath_controls(i);

            // Apply collision avoidance
            drone_avoid_areas(i);
            drone_avoid_walls(i);

            // Apply catchup speed multiplier
            f32 speed_mult = drone_get_speed_multiplier(i);
            drone_ctl[i].throttle_target *= speed_mult;

            // Write to car input structure
            drone_apply_inputs(i);
        }
    }
}
```

### Task 2: Implement Input Application

Connect drone outputs to the actual car input system:

```c
/**
 * Apply drone control outputs to car input structure
 *
 * @param car_index Drone car index
 */
void drone_apply_inputs(s32 car_index) {
    DroneControl *ctl = &drone_ctl[car_index];
    CarData *car = &car_array[car_index];

    // Convert normalized inputs to car input format
    // Arcade uses 0-4096 (0x1000), N64 may differ

    // Steering: -1.0 to 1.0 -> input format
    car->steer_input = (s16)(ctl->steer_target * 127.0f);

    // Throttle: 0.0 to 1.0 -> input format
    car->throttle_input = (s16)(ctl->throttle_target * 255.0f);

    // Brake: 0.0 to 1.0 -> input format
    car->brake_input = (s16)(ctl->brake_target * 255.0f);

    // Set flags indicating AI is controlling
    car->flags |= CAR_FLAG_AI_CONTROL;
}
```

### Task 3: Hook into Game Loop

Find where to call `drone_update_all()` in game.c.

**Look for**:
- The main game update function (around `func_800FD464` / `game_loop`)
- Physics update calls
- Input processing

**Integration point** (in `src/game/game.c`):
```c
// In the main game loop, after input and before physics:
void game_update_frame(void) {
    // ... input processing ...

    // Update drone AI (generates inputs for AI cars)
    drone_update_all();

    // ... physics update ...
    // ... rendering ...
}
```

### Task 4: Match N64 drone_ai_update()

The N64 function at 0x80009387 is ~3440 bytes. Analyze the disassembly and match:

```bash
# Generate disassembly for this function
./tools/m2c.py func_80009387
```

Key things to look for:
1. Loop over cars (MAX_CARS iterations)
2. Calls to path following functions
3. Catchup calculations
4. Input application

### Task 5: Add Drone Activation Logic

Drones need to be activated at race start:

```c
/**
 * Activate drones for race start
 * Called when transitioning to COUNTDOWN state
 */
void drone_activate_for_race(void) {
    s32 i;

    // Reset all drone states
    for (i = 0; i < MAX_CARS; i++) {
        drone_init_car(i);
    }

    // Count players and assign types
    num_humans = 0;
    num_drones = 0;

    for (i = 0; i < num_active_cars; i++) {
        if (car_is_human(i)) {
            drone_ctl[i].drone_type = DRONE_TYPE_HUMAN;
            drone_ctl[i].we_control = 0;
            human_indices[num_humans++] = i;
        } else {
            drone_ctl[i].drone_type = DRONE_TYPE_DRONE;
            drone_ctl[i].we_control = 1;
            drone_ctl[i].is_active = 1;
            num_drones++;
        }
    }

    // Assign initial paths
    drone_assign_default_paths();

    // Assign targets
    drone_assign_targets();

    // Set initial difficulty
    for (i = 0; i < num_active_cars; i++) {
        if (drone_ctl[i].drone_type == DRONE_TYPE_DRONE) {
            drone_ctl[i].difficulty = game_difficulty;  // Global difficulty setting
        }
    }
}
```

### Task 6: Add Race End Cleanup

```c
/**
 * Deactivate drones at race end
 */
void drone_deactivate(void) {
    s32 i;

    for (i = 0; i < MAX_CARS; i++) {
        drone_ctl[i].is_active = 0;
        drone_ctl[i].we_control = 0;
    }

    drone_end();
}
```

---

## N64 Difficulty Settings

From game.c analysis (line 34309):

```c
// Match N64 difficulty settings
void drone_set_difficulty_level(s32 difficulty) {
    switch (difficulty) {
        case 0:  // Easy
            ai_max_speed_percent = 70;
            ai_aggression_level = 50;
            use_catchup = 1;
            break;
        case 1:  // Medium
            ai_max_speed_percent = 85;
            ai_aggression_level = 70;
            use_catchup = 1;
            break;
        case 2:  // Hard
            ai_max_speed_percent = 95;
            ai_aggression_level = 85;
            use_catchup = 0;  // No rubber-banding on hard
            break;
        case 3:  // Expert
            ai_max_speed_percent = 100;
            ai_aggression_level = 100;
            use_catchup = 0;
            break;
    }
}
```

---

## Testing Checklist

After implementation, verify:

1. **Build passes**:
```bash
make VERSION=us NON_MATCHING=1 -j20
```

2. **Function linkage**:
   - drone_update_all() called from game loop
   - drone_activate_for_race() called at race start
   - drone_deactivate() called at race end

3. **Behavior checks** (if testing in emulator):
   - Drones start at correct positions
   - Drones follow track (don't drive into walls)
   - Drones maintain reasonable speed
   - Drones avoid player car
   - Catchup keeps race competitive

---

## Files to Modify

| File | Changes |
|------|---------|
| `src/game/drone.c` | Complete all integration functions |
| `src/game/game.c` | Add calls to drone system |
| `include/game/drone.h` | Add any missing prototypes |
| `include/game/structs.h` | Add input fields if needed |

---

## Arcade Source References

1. `reference/repos/rushtherock/game/drones.c:253-300` - DoDrones main loop
2. `reference/repos/rushtherock/game/game.c` - Integration points
3. `reference/repos/rushtherock/game/modeldat.h` - Input structure

---

## Final Commit

After all parts work together:

```bash
git add src/game/drone.c include/game/drone.h src/game/game.c
git commit -m "Implement complete AI/drone system

- Path following using maxpath waypoints
- Steering and speed control
- Collision avoidance with other cars
- Rubber-band catchup system
- Difficulty scaling
- Full integration with game loop

Based on arcade drones.c, maxpath.c, model.c
"
```

---

## Success Criteria

- [ ] drone_update_all() fully implemented
- [ ] drone_apply_inputs() connects to car input
- [ ] drone_activate_for_race() initializes drones
- [ ] drone_deactivate() cleans up
- [ ] drone_set_difficulty_level() matches N64 settings
- [ ] Game loop calls drone system
- [ ] Build passes
- [ ] All 6 homework parts integrated
- [ ] Final git commit

---

## Congratulations!

If you've completed all 6 parts, you've implemented a complete AI/drone racing system:

1. **Part 1**: Path data structures
2. **Part 2**: Path interval finding
3. **Part 3**: Steering and speed control
4. **Part 4**: Collision avoidance
5. **Part 5**: Rubber-band catchup
6. **Part 6**: Integration and testing

The drones should now race against the player with competitive, fun behavior that matches the arcade game!
