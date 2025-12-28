# Codex Task: Data Structures & Stub Implementation Guide

## Objective
Analyze arcade data structures and create implementation guides for N64 stub functions.

---

# Part A: Data Structure Analysis

## Task 1: MODELDAT Structure

### Priority: HIGH
This is the core physics/car state structure.

### Arcade Location
- `reference/repos/rushtherock/game/modeldat.h`

### Key Fields to Document
```c
typedef struct MODELDAT {
    // Position and orientation
    F32 pos[3];           // World position
    F32 vel[3];           // Velocity vector
    F32 uvs[3][3];        // Orientation matrix
    F32 angvel[3];        // Angular velocity

    // Physics state
    F32 mass;
    F32 inertia[3][3];
    F32 forces[3];
    F32 torques[3];

    // Tire state (x4)
    struct tiredes tires[4];
    F32 suscomp[4];       // Suspension compression

    // Drivetrain
    F32 rpm;
    S16 gear;
    F32 throttle;
    F32 brake;
    F32 steer;

    // ... many more fields
} MODELDAT;
```

### Deliverable
Create `docs/struct_modeldat.md` with:
1. Complete field list with types and offsets
2. Field descriptions and valid ranges
3. Comparison to N64 memory layout
4. Initialization sequence

---

## Task 2: CAR_DATA Structure

### Arcade Location
- `reference/repos/rushtherock/game/modeldat.h`

### Purpose
Game-level car state (separate from physics MODELDAT)

### Key Fields
- Visual state (damage, effects)
- Score and lap data
- Player info
- HUD state

### Deliverable
Create `docs/struct_car_data.md`

---

## Task 3: Tire Description Structure

### Arcade Location
- `reference/repos/rushtherock/game/tiredes.h`

### Key Fields
```c
struct tiredes {
    F32 stiffness;        // Tire stiffness
    F32 friction;         // Friction coefficient
    F32 slip_ratio;       // Current slip
    F32 slip_angle;       // Slip angle
    F32 angvel;           // Wheel angular velocity
    // ...
};
```

### Deliverable
Create `docs/struct_tiredes.md`

---

## Task 4: Global Variable Mapping

### N64 Globals to Identify
From game.c, map these D_80XXXXXX variables:
```c
D_801461D0  // Main game struct (160 refs)
D_80146170  // Secondary state array
D_80152818  // Player/car state (89 refs)
D_80143500  // Random seed
D_801146EC  // gstate equivalent
```

### Deliverable
Create `docs/globals_mapping.md` with:
1. Address → Name mapping
2. Type and size
3. Purpose description
4. Arcade equivalent

---

# Part B: Stub Implementation Guides

## Task 5: Physics Stub Implementations

### Target Functions
Pick 10 physics-related stub functions and provide implementation guides.

Example format:
```markdown
## grip_calculate

**Address**: 0x8010AEAC
**Arcade Equivalent**: frictioncircle() in tires.c

### Purpose
Calculate tire grip based on normal force and slip.

### Arcade Implementation
```c
void frictioncircle(MODELDAT *m, F32 tirev[3], F32 normalforce,
                    F32 torque, struct tiredes *tire, F32 *sfp, F32 *trp) {
    // [paste relevant arcade code]
}
```

### N64 Adaptation Notes
- Replace F32 with f32
- Use N64 math functions
- Account for fixed-point where needed

### Suggested Implementation
```c
f32 grip_calculate(void *car, s32 wheel_idx) {
    // [pseudocode based on arcade]
}
```
```

### Deliverable
Create `docs/stubs_physics.md` with 10 implementations

---

## Task 6: AI Stub Implementations

### Target Functions
- `ai_waypoint_next`
- `ai_path_follow`
- `ai_obstacle_avoid`
- `ai_rubber_band`
- `ai_catch_up`

### Deliverable
Create `docs/stubs_ai.md` with implementations

---

## Task 7: Audio Stub Implementations

### Target Functions
- `engine_sound_update`
- `tire_squeal`
- `sound_position_set`

### Deliverable
Create `docs/stubs_audio.md` with implementations

---

## Task 8: Camera Stub Implementations

### Target Functions
- `camera_follow`
- `camera_shake`
- `camera_mode_set`

### Deliverable
Create `docs/stubs_camera.md` with implementations

---

# Part C: Memory Layout Analysis

## Task 9: N64 Memory Map

### Analyze
- Code segments (from splat.us.yaml)
- Data segments
- BSS locations
- Heap regions

### Cross-reference with
- Compressed game code destination (0x80086A50)
- Stack locations
- DMA buffers

### Deliverable
Create `docs/memory_map.md`

---

## Task 10: Compare Build Artifacts

### Compare
- N64 function sizes vs arcade
- Data segment sizes
- Code density differences

### Analysis
```bash
# Get N64 function sizes from symbol file
grep "size:" symbol_addrs.us.txt | sort -t: -k2 -rn | head -20

# Compare to arcade (use nm or objdump on arcade binary if available)
```

### Deliverable
Create `docs/size_comparison.md`

---

## Analysis Commands

```bash
# Find struct-like accesses
grep -n "offset\|->.*=" src/game/game.c | head -30

# Find global variable accesses
grep -oE 'D_80[0-9A-F]+' src/game/game.c | sort | uniq -c | sort -rn | head -20

# Arcade struct definitions
find reference/repos/rushtherock -name "*.h" -exec grep -l "typedef struct" {} \;
```

## Success Criteria
- MODELDAT fully documented (50+ fields)
- At least 30 stub implementation guides
- 20+ global variables identified
- Memory map with key regions
- Size comparison analysis
