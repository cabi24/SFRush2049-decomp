# Respawn/Resurrection System Documentation

This document covers the resurrection (respawn) system in Rush 2049 N64, based on analysis of the arcade source code at `reference/repos/rushtherock/game/resurrect.c` and the N64 decompiled code.

## Overview

The resurrection system handles what happens when a car is destroyed (crashed, flipped, fell off track, etc.) and needs to respawn back on the track. It manages:

1. **Death detection** - Determining when a car should be destroyed
2. **Respawn point selection** - Calculating where the car should reappear
3. **Car reset sequence** - The "hand of god" animation moving the car
4. **Invulnerability period** - Post-respawn protection from collisions
5. **Different modes** - Race mode vs Battle mode respawning

---

## 1. Death/Crash Detection

### Arcade Implementation (resurrect.c)

The arcade game uses several mechanisms to detect when a car should be destroyed:

#### Crash Flag Detection (CheckCrash)
```c
// From arcade resurrect.c:1196-1249
void CheckCrash(S16 drone_index) {
    if (game_car[drone_index].crashflag) {
        if (game_car[drone_index].crashtime == 0) {
            /* Just crashed - save resurrection data */
            set_resurrection_data(Do_it, &model[drone_index]);

            /* Play explosion sound */
            SOUND(S_EXPLO);
            sndPositionSound(S_BOOM, 0, 255);

            /* Start explosion visual */
            StartBlast(drone_index);
            game_car[drone_index].just_crashed = 1;
            game_car[drone_index].crashtime = IRQTIME;
        }

        /* Check if crash delay expired */
        if ((IRQTIME - game_car[drone_index].crashtime) > crash_delay) {
            game_car[drone_index].we_died = true;
        }
    }
}
```

#### Crash Causes
| Cause | Description | Arcade Flag |
|-------|-------------|-------------|
| `crashflag` | Car exceeded damage threshold | MODELDAT.crashflag |
| Flip | Car flipped upside down | Detected by orientation |
| Fall | Fell off track boundaries | Out-of-bounds check |
| Water | Drove into water hazard | Surface type check |
| Stuck | No forward progress for too long | `detect_drone_abort()` |
| Manual Abort | Player pressed abort button | SW_ABORT input |

#### Auto-Abort Detection (detect_drone_abort)
```c
// From arcade resurrect.c:1564-1694
void detect_drone_abort(void) {
    /* Wait 5 seconds after race start */
    if (GetElapsedTime() < 5 * ONE_SEC) return;

    for (i = 0; i < num_active_cars; i++) {
        if (m->magvel < 25 && m->stuck_time != 0) {
            wait_time = /* depends on yank flags and reverse */
            if (IRQTIME - m->stuck_time > 3*ONE_SEC * wait_time) {
                abort_me = true;  /* Force respawn */
            }
        }
    }
}
```

### N64 Implementation

#### Key Data Structures (include/game/structs.h)
```c
typedef struct CarData {
    s16     crashflag;          /* Crashed flag */
    s8      we_died;            /* Should resurrect */
    s8      just_crashed;       /* In death throes */
    s16     collidable;         /* Is collidable */
    u32     collide_time;       /* Collision timestamp */
    s8      collide_state;      /* Transparency state */
    // ...
} CarData;
```

#### N64 Function Mappings
| N64 Address | N64 Name | Arcade Equivalent | Description |
|-------------|----------|-------------------|-------------|
| `0x800A9390` | `car_crash_detect` | `CheckCrash` partial | Car crash detection |
| `0x800A99C8` | `car_crash_response` | `CheckCrash` partial | Crash impulse response |
| `0x80108DA8` | `car_wreck` | `CheckCrash`/`StartBlast` | Triggers destruction |
| `0x80109F5C` | `wrong_way_check` | `detect_drone_abort` | Wrong way detection |

---

## 2. Respawn Point Selection

### Arcade Algorithm (resurrect.c:385-866)

The arcade uses a sophisticated algorithm to calculate respawn position:

#### Step 1: Find Distance Along Path
```c
// Between last checkpoint (dist1) and next checkpoint (dist2)
dist1 = (F32)path_dist[path_dist_index[m->last_cp_index]];
dist2 = (F32)path_dist[path_dist_index[m->last_cp_index+1]];

// Find closest path point to crash position
for (i = path_dist_index[m->last_cp_index]; i <= j; i++) {
    k = path_index[i];
    dist = (path[k].pos - crash_pos)^2;
    if (dist < c_dist) {
        c_dist = dist;
        close_index = i;
    }
}
```

#### Step 2: Calculate Projected Position
```c
// Project forward based on crash time and expected velocity
crash_time = resurrect_time - crash_time + RESURRECT_TIME;
new_dist = crash_dist + velocity * 1.1 * crash_time * 0.001;

// Don't respawn past finish line
if (near_finish && new_dist > half_dist_to_finish) {
    new_dist = half_dist_to_finish;
}
```

#### Step 3: Get Track Position and Orientation
```c
// Interpolate position between path points
interpolate(path[t_back].pos, path[t_fwd].pos, ..., new_dist, respawn_pos);

// Get orientation from path data
for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
        respawn_uvs[i][j] = path[t_back].fuvs[i][j];
```

#### Step 4: Apply Pole Position Offset
```c
// Offset respawn horizontally across track
ipos[1] = pole_pos_offset[polepos][1] * track_width;
Transvec(ipos, temp, &path[t_back].uvs[0]);
respawn_pos += temp * 0.025;
```

### Pole Position Offsets
```c
// First 8 entries: Starting grid positions (staggered 2-wide)
// Last 8 entries: Resurrection positions (spread across track)
const F32 pole_pos_offset[16][3] = {
    /* Starting grid */
    {POLEVDIST *  0.0, POLEHDIST *  0.0, 0.0},  // P1: front center
    {POLEVDIST *  0.0, POLEHDIST *  1.0, 0.0},  // P2: front right
    {POLEVDIST * -1.0, POLEHDIST * -0.5, 0.0},  // P3: row2 left
    {POLEVDIST * -1.0, POLEHDIST *  0.5, 0.0},  // P4: row2 right
    // ... rows 3-4

    /* Resurrection - spread across track width */
    {POLEVDIST *  0.0, POLEHDIST * -1.4, 0.0},
    {POLEVDIST *  0.0, POLEHDIST * -1.0, 0.0},
    // ...
};
```

### N64 Function Mappings
| N64 Address | N64 Name | Arcade Equivalent |
|-------------|----------|-------------------|
| `0x801084D4` | `car_respawn` | `resurrect_car` |
| `0x80155400` | `gResurrectPos` | `resurrect.save_pos` |
| `0x8015540C` | `gResurrectUvs` | `resurrect.uvs` |

---

## 3. Car Reset Sequence ("Hand of God")

### Arcade Animation States (resurrect.c:1305-1412)

The arcade uses a smooth quaternion-interpolated animation:

```c
void update_resurrecting_car(MODELDAT *m) {
    // State 0: Initialize hand-of-god effect
    if (m->resurrect.moving_state == 0) {
        // Calculate high point for arc
        vecsub(m->initin.pos, m->resurrect.pos, pos2);
        temp = magnitude(pos2) / 8;
        if (temp > 100) temp = 100;

        // Make car translucent
        m->appearance |= App_M_TRANSLUCENT;
    }

    // Increment state
    m->resurrect.moving_state++;

    // Time offset
    f32_state = IRQTIME - m->resurrect.resurrect_time;

    // Animation complete?
    if (s16_state > RESURRECT_TIME) {
        m->resurrect.moving_state = -2;  // Done
        m->hide_car = false;
        return;
    }

    // First half: Lift from crash site with rotation interpolation
    if (s16_state < RESURRECT_TIME/2) {
        scale = 2 * f32_state * f32_state / RESURRECT_TIME;
        interpolate(initin.pos, save_pos, ..., scale, pos);
        interpolate(quat_start, quat_end, 4, ..., scale, quat);
        make_uvs_from_quat(quat, uvs);
    }
    // Second half: Lower to respawn point
    else {
        interpolate(save_pos, respawn_pos, ..., f32_state, pos);
    }
}
```

### Animation Timeline

| Phase | Time | Action |
|-------|------|--------|
| Crash | 0ms | Save position, start explosion |
| Explosion | 0-3500ms | Car burns, player watches |
| Lift | 3500-4250ms | Car lifted up, rotating to align with track |
| Lower | 4250-5000ms | Car lowered to respawn point |
| Invulnerable | 5000-10000ms | Car blinks, gradually becomes solid |

### Quaternion-Based Rotation

The arcade uses quaternions for smooth rotation during respawn:

```c
// Convert orientations to quaternions
make_quat_from_uvs(gc->dr_uvs, m->resurrect.quat_start);
make_quat_from_uvs(m->resurrect.uvs, m->resurrect.quat_end);

// Find shortest rotation path
find_best_quat(m->resurrect.quat_start, m->resurrect.quat_end);

// Interpolate during animation
interpolate(quat_start, quat_end, 4, 0.0, RESURRECT_TIME/2, scale, quat);
make_uvs_from_quat(quat, uvs);
```

### N64 Function Mappings
| N64 Address | N64 Name | Arcade Equivalent |
|-------------|----------|-------------------|
| `0x80108F40` | `recovery_timer_update` | `check_if_finished_resurrecting` |
| `0x80155438` | `gMovingState` | `resurrect.moving_state` |
| `0x80155430` | `gResurrectTime` | `resurrect.resurrect_time` |

---

## 4. Invulnerability Period

### Arcade Implementation (resurrect.c:1418-1521)

After respawning, cars are temporarily invulnerable with a visual blinking effect:

```c
void blink_resurrecting_cars() {
    for (i = 0; i < MAX_LINKS; i++) {
        gc = &game_car[i];
        m = &model[i];

        if (gc->collidable) {
            // Car is solid - make visible
            m->appearance &= ~(App_M_HULK | App_M_TRANSLUCENT);
        } else {
            delta_time = IRQTIME - gc->collide_time;

            // If resurrecting, reset timer
            if (m->resurrect.moving_state > -1) {
                gc->collide_time = IRQTIME;
                delta_time = 0;
            }

            // After 5 seconds, force collidable
            if (delta_time > 5 * ONE_SEC) {
                gc->collidable = true;
                m->collidable = true;
                m->appearance &= ~App_M_TRANSLUCENT;
            } else {
                // Blink effect - faster as time progresses
                switch (delta_time / 1000) {
                    case 0: gc->collide_count = 33; break;  // Slow blink
                    case 1: gc->collide_count = 8; break;
                    case 2: gc->collide_count = 5; break;
                    case 3: gc->collide_count = 2; break;
                    default: gc->collide_count = 0; break;  // Solid
                }

                // Toggle transparency
                if (gc->collide_state & 1)
                    m->appearance &= ~App_M_TRANSLUCENT;
                else
                    m->appearance |= App_M_TRANSLUCENT;
            }
        }
    }
}
```

### Blinking Schedule

| Time After Respawn | Blink Interval | Effect |
|-------------------|----------------|--------|
| 0-1 sec | 33 frames (~0.5s) | Slow fade in/out |
| 1-2 sec | 8 frames (~0.13s) | Medium blink |
| 2-3 sec | 5 frames (~0.08s) | Fast blink |
| 3-4 sec | 2 frames (~0.03s) | Very fast blink |
| 4-5 sec | 0 frames | Solid |
| 5+ sec | N/A | Force collidable |

### Key Flags

| Flag | Location | Purpose |
|------|----------|---------|
| `collidable` | CAR_DATA | Whether car can be hit |
| `collide_time` | CAR_DATA | When invulnerability started |
| `collide_state` | CAR_DATA | Current blink state (even=transparent) |
| `collide_count` | CAR_DATA | Frames until next blink toggle |
| `App_M_TRANSLUCENT` | appearance | Visual transparency flag |

---

## 5. Battle Mode Respawn vs Race Respawn

### Race Mode Respawn

Race mode respawning follows the checkpoint system:

1. **Position**: Near last checkpoint, projected forward
2. **Orientation**: Aligned with track direction
3. **Velocity**: Set to track's expected speed
4. **Delay**: Full 3.5 second crash delay
5. **Animation**: Full "hand of god" arc

```c
// Race respawn from arcade resurrect.c
void resurrect_car(MODELDAT *m) {
    // Get checkpoint-based respawn
    chkpnt = CP_Track[trackno].chk_point;
    dist1 = path_dist[path_dist_index[m->last_cp_index]];

    // Calculate projected position
    new_dist = crash_dist + vel * 1.1 * crash_time * 0.001;

    // Don't pass finish line
    if (near_finish) new_dist = min(new_dist, half_dist_to_finish);

    // Set respawn velocity
    m->resurrect.velocity = mpath[k].speed;
}
```

### Battle Mode Respawn

Battle mode uses fixed spawn points in the arena:

```c
// From battle.c
static const f32 sSpawnPoints[4][4][3] = {
    /* Arena 0 - corners */
    { {100.0f, 0.0f, 100.0f}, {-100.0f, 0.0f, 100.0f},
      {100.0f, 0.0f, -100.0f}, {-100.0f, 0.0f, -100.0f} },
    // ... more arenas
};

void battle_respawn_player(s32 player) {
    BattlePlayer *p = &gBattle.players[player];
    CarData *car = &car_array[player];

    p->alive = 1;
    p->health = p->max_health;
    p->weapon = WEAPON_NONE;
    p->shield_timer = 60;  /* Brief spawn protection (1 sec) */

    /* Move to preset spawn point */
    car->dr_pos[0] = p->spawn_pos[0];
    car->dr_pos[1] = p->spawn_pos[1];
    car->dr_pos[2] = p->spawn_pos[2];
    car->mph = 0.0f;  /* Zero velocity */
}
```

### Key Differences

| Aspect | Race Mode | Battle Mode |
|--------|-----------|-------------|
| **Position Source** | Checkpoint + projection | Fixed arena spawn points |
| **Respawn Delay** | 3.5 seconds | ~3 seconds (configurable) |
| **Initial Speed** | Track speed (maxpath) | Zero velocity |
| **Orientation** | Track direction | Arena spawn direction |
| **Invulnerability** | 5 seconds with blink | 1 second shield |
| **Animation** | Full "hand of god" | Instant teleport + fade |
| **Health Reset** | N/A (no health in race) | Full health restore |

### N64 Function Mappings
| N64 Address | N64 Name | Mode | Description |
|-------------|----------|------|-------------|
| `0x801084D4` | `car_respawn` | Race | Checkpoint-based respawn |
| `0x800D5C90` | `player_respawn` | Race | Player respawn after crash |
| `0x8010F0D0` | `battle_respawn` | Battle | Arena spawn point respawn |

---

## 6. N64 vs Arcade Differences

### Known Differences

| Aspect | Arcade | N64 |
|--------|--------|-----|
| **Frame Rate** | 30 Hz | 60 Hz |
| **Timing** | Milliseconds | Frame counts |
| **Crash Delay** | 3.5 seconds | ~3.5 sec (210 frames) |
| **Resurrect Time** | 1.5 seconds | ~1.5 sec (90 frames) |
| **Max Cars** | 8 (MAX_LINKS) | 8 (confirmed) |
| **Path Resolution** | High (40 units/foot) | Similar |

### N64 Simplifications

1. **Less sophisticated path projection**: The N64 may use simpler distance calculations
2. **Fixed invulnerability blink**: May use constant blink rate instead of acceleration
3. **Simplified quaternion math**: May use matrix interpolation instead of quaternions
4. **Memory constraints**: Fewer checkpoints stored, smaller path data

### Arcade Features Possibly Removed

1. **Shortcut respawn logic** - Complex shortcut position tracking
2. **Mirror mode adjustments** - `gMirrorMode` pole position flipping
3. **Demo game special handling** - Different respawn for attract mode

---

## 7. Key Global Variables

### N64 Globals (from symbol_addrs.us.txt)

| Address | Name | Size | Description |
|---------|------|------|-------------|
| `0x80155400` | `gResurrectPos` | 12 bytes | f32[3] respawn position |
| `0x8015540C` | `gResurrectUvs` | 36 bytes | f32[3][3] respawn orientation |
| `0x80155430` | `gResurrectTime` | 4 bytes | Respawn timer (s32) |
| `0x80155434` | `gCrashTime` | 4 bytes | Crash time (s32) |
| `0x80155438` | `gMovingState` | 2 bytes | Moving state (s16) |
| `0x8015543C` | `gResurrectVelocity` | 4 bytes | Target velocity (f32) |
| `0x80152B28` | `gCarCrashFlag` | 4 bytes | Global crash flag |
| `0x80152B2C` | `gCarCrashThreshold` | 4 bytes | Crash threshold |

### Arcade Equivalents

| Arcade Field | Location | Description |
|--------------|----------|-------------|
| `resurrect.save_pos` | MODELDAT | Crash position |
| `resurrect.uvs` | MODELDAT | Crash orientation |
| `resurrect.crash_time` | MODELDAT | When crash started |
| `resurrect.resurrect_time` | MODELDAT | When respawn started |
| `resurrect.moving_state` | MODELDAT | Animation state (-1, 0, 1+, -2) |
| `resurrect.velocity` | MODELDAT | Target respawn speed |
| `crashflag` | CAR_DATA | Car is crashed |
| `we_died` | CAR_DATA | Should respawn |
| `just_crashed` | CAR_DATA | In explosion phase |

---

## 8. Complete Function Mapping

### Resurrection Functions

| N64 Address | N64 Name | Arcade Function | File | Description |
|-------------|----------|-----------------|------|-------------|
| `0x801084D4` | `car_respawn` | `resurrect_car()` | resurrect.c:385 | Main respawn logic |
| `0x80108AB0` | `death_check` | Part of CheckCrash | resurrect.c:1196 | Death detection |
| `0x80108DA8` | `car_wreck` | `CheckCrash()`+`StartBlast()` | resurrect.c:1196 | Trigger destruction |
| `0x80108F40` | `recovery_timer_update` | `check_if_finished_resurrecting()` | resurrect.c:1256 | Update respawn timer |
| `0x80109468` | `car_reset_manual` | `abort_car()` | resurrect.c:1528 | Manual reset |
| `0x80109A60` | `shortcut_detect` | Shortcut handling | resurrect.c:494 | Shortcut zone detection |
| `0x80109F5C` | `wrong_way_check` | `detect_drone_abort()` | resurrect.c:1564 | Wrong way detection |
| `0x800D5C90` | `player_respawn` | Player-specific respawn | - | 220 bytes |

### Battle Mode Functions

| N64 Address | N64 Name | Description |
|-------------|----------|-------------|
| `0x8010F0D0` | `battle_respawn` | Arena spawn point respawn (280 bytes) |

### Helper Functions

| Arcade Function | File | Description |
|-----------------|------|-------------|
| `set_resurrection_data()` | resurrect.c:1108 | Save crash position |
| `update_resurrecting_car()` | resurrect.c:1305 | Animation update |
| `blink_resurrecting_cars()` | resurrect.c:1418 | Invulnerability effect |
| `make_quat_from_uvs()` | resurrect.c:907 | Orientation to quaternion |
| `make_uvs_from_quat()` | resurrect.c:874 | Quaternion to orientation |
| `find_best_quat()` | resurrect.c:951 | Shortest rotation path |
| `interpolate()` | resurrect.c:1058 | Linear interpolation |
| `are_we_dead_or_dying()` | resurrect.c:1702 | State query |
| `are_we_solid()` | resurrect.c:1718 | Collision query |

---

## 9. Timing Constants

```c
/* From arcade resurrect.h */
#define POLEHDIST       17.0    /* Horizontal grid spacing (feet) */
#define POLEVDIST       20.0    /* Vertical grid spacing (feet) */
#define CRASHDEL        3500    /* Crash delay (milliseconds) */
#define RESURRECT_TIME  1500    /* Respawn animation time (milliseconds) */

/* N64 frame-based equivalents */
#define CRASH_DELAY_FRAMES      210     /* 3.5 seconds at 60fps */
#define RESURRECT_FRAMES        90      /* 1.5 seconds at 60fps */
#define INVINCIBLE_FRAMES       120     /* 2 seconds post-respawn */
```

---

## 10. References

### Source Files

| File | Description |
|------|-------------|
| `reference/repos/rushtherock/game/resurrect.c` | Arcade resurrection implementation (1730 lines) |
| `reference/repos/rushtherock/game/resurrect.h` | Arcade constants and prototypes |
| `src/game/resurrect.c` | N64 decompiled resurrection code |
| `src/game/crash.c` | N64 crash detection and handling |
| `src/game/battle.c` | N64 battle mode (includes respawn) |
| `include/game/resurrect.h` | N64 resurrection header |
| `include/game/crash.h` | N64 crash system header |
| `include/game/structs.h` | CarData struct with crash fields |
| `symbol_addrs.us.txt` | Function address mappings |

### Related Documentation

| Document | Topic |
|----------|-------|
| `docs/checkpoint_system_mapping.md` | Checkpoint integration |
| `docs/collision_system.md` | Crash triggers |
| `docs/battle_mode_system.md` | Battle respawn details |
| `docs/physics_collision_mapping.md` | Impact detection |
