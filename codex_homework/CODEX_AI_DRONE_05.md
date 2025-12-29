# Codex Homework: AI/Drone System - Part 5 of 6
## Rubber-Band Catchup System

**Estimated Time**: 2-3 hours
**Difficulty**: Medium
**Prerequisites**: CODEX_AI_DRONE_04 completed

---

## Objective

Implement the rubber-band catchup system that keeps races competitive by speeding up trailing drones and slowing down leaders.

---

## Background

The arcade game uses sophisticated "rubber-banding" to ensure exciting races:
- Drones far behind the player get speed boosts
- Drones far ahead slow down
- The effect scales with difficulty setting
- Solo players get a compensating boost

This is critical for gameplay feel - without it, either the player would always win or always lose.

---

## N64 Functions to Match

| N64 Function | Address | Size | Arcade Equivalent |
|--------------|---------|------|-------------------|
| `ai_catch_up_logic()` | 0x80028265 | ~500 bytes | `set_catchup()` |
| `ai_rubber_band()` | 0x80041861 | ~200 bytes | Helper function |
| `ai_catch_up()` | 0x80042091 | ~150 bytes | `no_catchup()` fallback |

---

## Tasks

### Task 1: Define Catchup Constants

Add to `include/game/drone.h`:

```c
// Catchup system constants
#define CATCHUP_DISTANCE    300.0f  // Distance for full catchup effect
#define CATCHUP_MAX_BOOST   1.15f   // Maximum speed boost (15%)
#define CATCHUP_MIN_BOOST   0.85f   // Maximum slowdown (15%)
#define CATCHUP_SMOOTHING   0.03f   // Smoothing factor per frame

// Per-track catchup scaling (arcade: cuscale[], cuzone[])
// These would be loaded from track data in full implementation
#define CATCHUP_SCALE_DEFAULT   0.10f   // Default catchup strength
#define CATCHUP_ZONE_DEFAULT    500.0f  // Distance zone for full effect
#define CATCHUP_SOLO_BOOST      1.05f   // Solo player boost
```

### Task 2: Implement Full set_catchup()

**Arcade reference**: `model.c:121`

```c
/**
 * Calculate rubber-band catchup for all drones
 * Based on arcade: model.c:set_catchup()
 *
 * Makes trailing cars faster and leading cars slower
 * to keep races competitive.
 */
void drone_set_catchup(void) {
    s32 i;
    s32 leader_index = -1;
    f32 leader_dist = -999999.0f;
    f32 my_dist, delta_dist;
    f32 target_catchup;
    DroneControl *ctl;

    // Find the leading human player
    for (i = 0; i < num_active_cars; i++) {
        if (drone_ctl[i].drone_type == DRONE_TYPE_HUMAN) {
            if (car_array[i].distance > leader_dist) {
                leader_dist = car_array[i].distance;
                leader_index = i;
            }
        }
    }

    // If no humans (all drones), use overall leader
    if (leader_index < 0) {
        for (i = 0; i < num_active_cars; i++) {
            if (car_array[i].distance > leader_dist) {
                leader_dist = car_array[i].distance;
                leader_index = i;
            }
        }
    }

    // Solo player handling
    if (num_humans == 1) {
        // Give solo player a compensating boost
        // This counters the fact that drones will catch up
        drone_ctl[leader_index].catchup_boost = CATCHUP_SOLO_BOOST;
    }

    // Calculate catchup for each drone
    for (i = 0; i < num_active_cars; i++) {
        ctl = &drone_ctl[i];

        // Skip humans (they don't get AI catchup)
        if (ctl->drone_type == DRONE_TYPE_HUMAN) {
            continue;
        }

        // Skip inactive
        if (!ctl->is_active) {
            continue;
        }

        my_dist = car_array[i].distance;
        delta_dist = leader_dist - my_dist;

        // Calculate target catchup based on distance behind leader
        if (delta_dist > CATCHUP_ZONE_DEFAULT) {
            // Way behind - max boost
            target_catchup = CATCHUP_MAX_BOOST;
        } else if (delta_dist < -CATCHUP_ZONE_DEFAULT) {
            // Way ahead - max slowdown
            target_catchup = CATCHUP_MIN_BOOST;
        } else if (delta_dist > 0) {
            // Behind - proportional boost
            target_catchup = 1.0f + (delta_dist / CATCHUP_ZONE_DEFAULT) * (CATCHUP_MAX_BOOST - 1.0f);
        } else {
            // Ahead - proportional slowdown
            target_catchup = 1.0f + (delta_dist / CATCHUP_ZONE_DEFAULT) * (1.0f - CATCHUP_MIN_BOOST);
        }

        // Smooth the transition (arcade: 3% per frame)
        ctl->catchup_boost = ctl->catchup_boost * (1.0f - CATCHUP_SMOOTHING) +
                             target_catchup * CATCHUP_SMOOTHING;
    }
}
```

### Task 3: Implement Difficulty-Based Scaling

**Arcade reference**: `drones.c:277` (assign_drones)

The catchup strength varies with difficulty:

```c
/**
 * Apply difficulty-based catchup scaling
 *
 * @param car_index Drone index
 * @param base_catchup Base catchup value
 * @return Scaled catchup value
 */
f32 drone_scale_catchup_by_difficulty(s32 car_index, f32 base_catchup) {
    DroneControl *ctl = &drone_ctl[car_index];
    f32 diff_scale;

    // Difficulty: 0 = hardest (less catchup for player)
    //            7 = easiest (more catchup for player)
    diff_scale = (f32)ctl->difficulty / 7.0f;

    // On easy, drones get less boost (player advantage)
    // On hard, drones get more boost (challenging)
    if (base_catchup > 1.0f) {
        // Drone is boosting - reduce on easy
        return 1.0f + (base_catchup - 1.0f) * (1.0f - diff_scale * 0.5f);
    } else {
        // Drone is slowing - reduce slowdown on hard
        return 1.0f - (1.0f - base_catchup) * (diff_scale * 0.5f + 0.5f);
    }
}
```

### Task 4: Implement Target Assignment

Drones track specific human players:

```c
/**
 * Assign target human for each drone to track
 * Used for catchup calculations
 */
void drone_assign_targets(void) {
    s32 i, j;
    s32 human_indices[MAX_CARS];
    s32 num_human = 0;

    // Collect human player indices
    for (i = 0; i < num_active_cars; i++) {
        if (drone_ctl[i].drone_type == DRONE_TYPE_HUMAN) {
            human_indices[num_human++] = i;
        }
    }

    if (num_human == 0) {
        // No humans - drones target each other (shouldn't happen)
        return;
    }

    // Assign drones to track nearest human by race position
    j = 0;
    for (i = 0; i < num_active_cars; i++) {
        if (drone_ctl[i].drone_type == DRONE_TYPE_DRONE) {
            // Simple assignment: round-robin to humans
            drone_ctl[i].target_car = human_indices[j % num_human];
            j++;
        }
    }
}
```

### Task 5: Apply Catchup to Speed

Update the speed calculation to use catchup:

```c
/**
 * Get final speed multiplier for a drone
 * Combines path speed, catchup, and difficulty
 *
 * @param car_index Drone index
 * @return Speed multiplier
 */
f32 drone_get_speed_multiplier(s32 car_index) {
    DroneControl *ctl = &drone_ctl[car_index];
    f32 multiplier;

    // Base multiplier is catchup boost
    multiplier = ctl->catchup_boost;

    // Apply difficulty scaling
    multiplier = drone_scale_catchup_by_difficulty(car_index, multiplier);

    // Apply drone_scale (from assign_drones position-based scaling)
    multiplier *= ctl->drone_scale;

    // Apply time_boost (early race spreading)
    multiplier *= ctl->time_boost;

    return multiplier;
}
```

---

## Integration

Update `drone_update()` to apply the speed multiplier:

```c
void drone_update(s32 car_index) {
    DroneControl *ctl = &drone_ctl[car_index];
    f32 speed_mult;

    // ... existing path following code ...

    // Get final speed multiplier (catchup + difficulty + positioning)
    speed_mult = drone_get_speed_multiplier(car_index);

    // Apply to throttle
    ctl->throttle_target *= speed_mult;

    // Clamp
    if (ctl->throttle_target > 1.0f) ctl->throttle_target = 1.0f;
}
```

---

## Verification

1. Build passes:
```bash
make VERSION=us NON_MATCHING=1 -j20
```

2. Catchup behavior:
   - Drone behind player: catchup_boost > 1.0
   - Drone ahead of player: catchup_boost < 1.0
   - Values should change smoothly (not jump)

---

## Files to Modify

| File | Changes |
|------|---------|
| `include/game/drone.h` | Add catchup constants |
| `src/game/drone.c` | Implement catchup functions, integrate into main loop |

---

## Arcade Source References

1. `reference/repos/rushtherock/game/model.c:121-200` - set_catchup()
2. `reference/repos/rushtherock/game/drones.c:277-400` - assign_drones catchup logic

---

## Arcade Catchup Formula

From `model.c`:
```c
// Calculate target catchup
if (lagdist > cuzone[trackno]) {
    target_catchup = max_catchup + 1;  // Full boost
} else {
    target_catchup = lagdist * max_catchup / cuzone[trackno] + 1;
}

// Smooth transition (3% per frame)
model[i].catchup = 0.97 * model[i].catchup + 0.03 * target_catchup;
```

---

## Success Criteria

- [ ] Catchup constants defined
- [ ] drone_set_catchup() implemented with full logic
- [ ] drone_scale_catchup_by_difficulty() implemented
- [ ] drone_assign_targets() implemented
- [ ] drone_get_speed_multiplier() combines all factors
- [ ] Catchup applied in drone_update()
- [ ] Build passes
- [ ] Git commit

---

## Next Homework

Proceed to **CODEX_AI_DRONE_06.md** for the final integration and testing.
