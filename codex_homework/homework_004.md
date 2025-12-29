# Homework 004: Analyze Arcade Collision Detection System

## Objective
Document the arcade collision detection and response system to guide N64 physics implementation.

## Context
The N64 has collision-related functions in game.c. The arcade uses a sophisticated collision system for car-to-car, car-to-wall, and car-to-object interactions.

## Tasks
1. Analyze arcade collision files:
   - `reference/repos/rushtherock/game/collision.c` (if exists)
   - `reference/repos/rushtherock/game/cars.c` (CheckCrash, etc.)
   - `reference/repos/rushtherock/game/drivsym.c` (physics response)

2. Search for collision-related code:
   ```bash
   grep -rn "collision\|crash\|impact\|bounce" reference/repos/rushtherock/game/*.c | head -50
   grep -rn "CheckCrash\|DoCollision\|HitWall" reference/repos/rushtherock/game/*.c
   ```

3. Document collision types:
   - Car-to-car collisions
   - Car-to-wall/barrier collisions
   - Car-to-object (cones, signs, etc.)
   - Ground/surface collisions
   - Wing/flip detection

4. Extract collision response:
   - How velocity is modified on impact
   - Damage calculation
   - Bounce/deflection angles
   - Spin-out conditions

5. Find N64 equivalents:
   ```bash
   grep -n "collision\|crash\|impact\|bounce" src/game/game.c | head -30
   ```

## Deliverables
Create `docs/physics_collision_mapping.md`:

```markdown
# Collision Detection System

## Overview
[How collision detection works at high level]

## Collision Types

### Car-to-Car
- Detection method: [bounding box, sphere, etc.]
- Response: [velocity exchange, damage]
- Arcade function: [name @ file:line]

### Car-to-Wall
[same format]

### Car-to-Object
[same format]

### Ground Contact
[same format]

## Arcade Functions
| Function | File:Line | Purpose |
|----------|-----------|---------|
| CheckCrash | cars.c:XXX | Main collision check |

## Collision Response Math
[Key formulas for bounce, deflection, damage]

## N64 Function Mapping
| N64 Function | Address | Arcade Equivalent |
|--------------|---------|-------------------|

## Damage System
| Damage Type | Threshold | Effect |
|-------------|-----------|--------|

## Special Cases
- Flip detection
- Rollover handling
- Water/reset triggers
```

## Reference Files
- `reference/repos/rushtherock/game/cars.c`
- `reference/repos/rushtherock/game/drivsym.c`
- `reference/repos/rushtherock/game/modeldat.h`
- `src/game/game.c`

## Success Criteria
- All collision types documented
- At least 5 arcade collision functions identified
- Collision response formulas extracted
- N64 collision functions mapped
