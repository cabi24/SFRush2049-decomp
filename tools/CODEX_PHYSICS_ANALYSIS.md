# Codex Task: Physics System Deep Analysis

## Objective
Create a comprehensive mapping between the N64 physics functions in `src/game/game.c` and the arcade physics system in `reference/repos/rushtherock/game/`.

## Background
The N64 Rush 2049 uses a Milliken-style tire model and rigid body physics similar to the arcade version. The arcade source has well-documented physics code that can inform our decompilation.

## Task 1: Tire Physics Mapping

### Arcade Files to Analyze
- `reference/repos/rushtherock/game/tires.c` - Main tire model
- `reference/repos/rushtherock/game/initiali.c` - Tire constants
- `reference/repos/rushtherock/game/drivsym.c` - Physics integration

### N64 Functions to Map (from game.c)
Search for these renamed functions and map them to arcade equivalents:
- `tire_friction_calc` / `grip_calculate`
- `tire_slip_angle` / `calcalpha` equivalent
- `suspension_force` / spring/damper calculations
- `downforce_calculate`
- `drag_calculate`

### Deliverable
Create `docs/physics_tire_mapping.md` with:
1. Function-by-function mapping table
2. Key differences between arcade and N64 implementations
3. Data structure comparison (tire parameters, suspension settings)
4. Suggested implementations for stub functions

---

## Task 2: Drivetrain System Analysis

### Arcade Files
- `reference/repos/rushtherock/game/drivetra.c` - Drivetrain simulation
- `reference/repos/rushtherock/game/cars.c` - Car parameter tables

### N64 Functions to Analyze
- `engine_torque_get`
- `transmission_shift`
- `throttle_apply`
- `brake_apply`
- `handbrake_apply`

### Deliverable
Create `docs/physics_drivetrain_mapping.md` with:
1. Torque curve analysis
2. Gear ratio tables
3. Engine RPM calculations
4. Clutch/transmission logic

---

## Task 3: Collision System Mapping

### Arcade Files
- `reference/repos/rushtherock/game/collision.c` - Car-car and car-world collision
- `reference/repos/rushtherock/game/resurrect.c` - Crash detection and respawn

### N64 Functions
- `collision_check` / `collision_resolve`
- `car_collision` / `track_collision`
- `car_wreck` / `death_check`
- `car_respawn` / `recovery_timer_update`

### Deliverable
Create `docs/physics_collision_mapping.md` with:
1. Collision detection algorithm comparison
2. Impact force calculations
3. Damage system analysis
4. Respawn logic flowchart

---

## Task 4: Physics Integration

### Arcade Files
- `reference/repos/rushtherock/game/drivsym.c` - Main physics loop
- `reference/repos/rushtherock/game/model.c` - Model state management

### Key Functions to Trace
- `sym()` - Main physics tick
- `regular()` - Forces/torques/integration
- `forces()`, `forces1()`, `forces2()` - Force aggregation
- `torques()`, `accelerations()`, `velocities()`, `positions()` - Integration pipeline

### Deliverable
Create `docs/physics_integration_mapping.md` with:
1. Physics timestep analysis
2. Integration method (Euler, RK4, etc.)
3. Force accumulation order
4. State update sequence

---

## Output Format

For each mapping document, use this structure:

```markdown
# [System] Mapping: Arcade → N64

## Summary
[Brief overview of the system]

## Function Mapping Table
| Arcade Function | Arcade File | N64 Equivalent | Confidence | Notes |
|-----------------|-------------|----------------|------------|-------|
| calctireuv() | tires.c | tire_uv_calc? | High | Same algorithm |

## Data Structures
[Compare arcade structs to N64 equivalents]

## Implementation Notes
[Key insights for decompiling the N64 code]

## Recommended Stub Implementations
[For each stub function, provide pseudo-code based on arcade]
```

## Success Criteria
- At least 20 function mappings per document
- Confidence ratings for each mapping
- Actionable implementation suggestions for stub functions
- Cross-references to specific line numbers in arcade source
