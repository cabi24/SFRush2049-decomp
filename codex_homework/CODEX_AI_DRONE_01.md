# Codex Homework: AI/Drone System - Part 1 of 6
## Path Data Structures & Initialization

**Estimated Time**: 2-3 hours
**Difficulty**: Medium
**Prerequisites**: Build system working, arcade source available

---

## Objective

Implement the path data structures and initialization functions for the AI/drone system. This forms the foundation that all other drone code depends on.

---

## Background

The Rush 2049 AI system uses pre-recorded "maxpath" waypoints to guide drone cars around the track. Each path consists of:
- 3D positions along the racing line
- Target speeds at each point
- Behavior hints (stay on path, allow passing, etc.)

The arcade source files are:
- `reference/repos/rushtherock/game/maxpath.h` - Data structures
- `reference/repos/rushtherock/game/maxpath.c` - Path system (~3,730 lines)
- `reference/repos/rushtherock/game/drones.c` - Drone control (~945 lines)

---

## Tasks

### Task 1: Define Path Data Structures

Create/update `include/game/drone.h` with the MPATH and MPHEADER structures.

**Arcade reference** (`maxpath.h`):
```c
typedef struct MPATH {
    F32 pos[3];         // World position (x, y, z)
    F32 speed;          // Target speed at this point
    S32 hints;          // Behavior hints (enum HINT_TYPES)
    S16 base_to_path;   // Link to base path
    S16 path_to_base;   // Link back to base
} MPATH;

typedef struct MPHEADER {
    S32 num_points;     // Number of path points
    S32 mpath_active;   // Is this path set active?
    S8  priority;       // Path priority (0-10)
    S16 lap_start;      // Index of lap start
    S16 lap_end;        // Index of lap end
    S16 num_in_lap;     // Points in one lap
} MPHEADER;
```

**N64 types**: Use `f32`, `s32`, `s16`, `s8` instead of `F32`, `S32`, etc.

### Task 2: Define Hint Types Enum

```c
typedef enum {
    STAY_WITHIN_WIDTH = 0,  // Stay within road width
    GET_ON_MPATH = 1,       // Actively return to path
    STAY_ON_MPATH = 2,      // Strictly follow path
    STAY_PARALLEL = 3,      // Stay parallel (allow passing)
    RESET_PARALLEL = 4,     // Reset parallel mode
    EASE_ON_MPATH = 5,      // Gradually return to path
    NUM_HINT_TYPES = 6
} HintType;
```

### Task 3: Implement InitMaxPath()

**N64 function to match**: `func_80086A50` area (game code start)
**Arcade reference**: `maxpath.c:421` - `InitMaxPath()`

The function should:
1. Load path data from ROM/track data
2. Initialize path headers for each path set
3. Set initial path assignments for drones

**Stub to implement** in `src/game/drone.c`:
```c
void drone_init_maxpath(s32 record_mode) {
    // TODO: Load path data from track
    // TODO: Initialize MPHEADER for each path
    // TODO: Set mpath_active flags
}
```

### Task 4: Implement assign_default_paths()

**Arcade reference**: `maxpath.c:513`

Assign path sets to each drone based on:
- Number of paths available for track
- Drone's starting position
- Difficulty level

### Task 5: Define Path System Constants

```c
#define MAXMPATH        2000    // Max path points per set
#define MAX_MPATHS      8       // Max number of path sets
#define MAX_LINKS       8       // Max cars in race

// Look-ahead for steering (arcade: g_lookahd = 80.0)
#define PATH_LOOKAHEAD  80.0f

// Speed scaling
#define PATH_SPEED_SCALE 1.0f   // g_MPspdscale
```

---

## Verification

After implementation:

1. Build must pass:
```bash
make VERSION=us NON_MATCHING=1 -j20
```

2. Path structures should match arcade sizes:
   - `MPATH` = 24 bytes (3 floats + 1 float + 1 int + 2 shorts)
   - `MPHEADER` = 16 bytes

3. Path array should be allocated (can be static for now)

---

## Files to Modify

| File | Changes |
|------|---------|
| `include/game/drone.h` | Add MPATH, MPHEADER, HintType, constants |
| `src/game/drone.c` | Implement drone_init_maxpath(), drone_assign_default_paths() |

---

## Arcade Source References

Read these sections carefully:

1. `reference/repos/rushtherock/game/maxpath.h` - Full file (113 lines)
2. `reference/repos/rushtherock/game/maxpath.c:421-550` - InitMaxPath and assign_default_paths
3. `reference/repos/rushtherock/game/drones.c:227-275` - InitDrones

---

## N64 Function Hints

From disassembly analysis:
- Path data likely stored in compressed game code section
- Look for arrays of floats with position/speed patterns
- Track-specific path data referenced via trackno global

---

## Success Criteria

- [ ] MPATH and MPHEADER structures defined
- [ ] HintType enum defined
- [ ] Constants defined matching arcade values
- [ ] drone_init_maxpath() has working stub
- [ ] drone_assign_default_paths() has working stub
- [ ] Build passes with no errors
- [ ] Git commit with descriptive message

---

## Next Homework

After completing this, proceed to **CODEX_AI_DRONE_02.md** which covers the path interval finding and position interpolation.
