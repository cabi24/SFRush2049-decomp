# Collision Detection System

## Overview
Arcade collision is handled in `collision.c` using a two-stage test: broadphase bounding-sphere overlap (`colrad`) followed by per-corner bounding-box tests against the other car's body bounds. On collision, forces are computed in the other car's frame and applied to `CENTERFORCE`, which drives damage and response.

## Collision Types

### Car-to-Car
- Detection method: bounding sphere (`colrad`) then corner-in-box tests using `PointInBody()`.
- Response: `setFBCollisionForce()` computes force based on relative velocity and contact side; `ForceApart()` used when major overlap.
- Arcade functions: `collision()` + `setFBCollisionForce()` in `collision.c`.

### Car-to-Wall
- Detection method: not in `collision.c`; wall contacts are handled via road/track collision (see N64 `track_collision` call and `entity_collision_test`).
- Response: likely separate collision system for world geometry (track collision), with `CENTERFORCE`/`BODYFORCE` updates.
- Arcade functions: not explicitly in `collision.c` (see road/stree collision paths).

### Car-to-Object
- Detection method: sound/impact flags in `carsnd.c` indicate object hits (cones, glass, poles, fences).
- Response: sound triggers plus collision force in model; objects use `sound_flags`/`shortcut_flags` and target systems.
- Arcade functions: object collision sounds in `carsnd.c` and target hits in `targets.c` (not fully traced here).

### Ground Contact
- Detection method: handled by physics/road contact (tire forces and road interactions), not `collision.c`.
- Response: tire forces, `roadtorque`, and suspension compression in `drivsym.c`/`tires.c`.

### Flip/Rollover
- Detection method: crash/resurrect flow via `crashflag` and `CheckCrash()` (resurrection system).
- Response: explosion, death cam, and delayed resurrect in `resurrect.c`.

## Arcade Functions
| Function | File:Line | Purpose |
|----------|-----------|---------|
| `init_collision(MODELDAT *m)` | reference/repos/rushtherock/game/collision.c:138 | Reset per-car collision force accumulators and last touch pointers. |
| `collision(MODELDAT *m)` | reference/repos/rushtherock/game/collision.c:163 | Car-to-car collision detection and response. |
| `PointInBody(MODELDAT *m, F32 pt[3])` | reference/repos/rushtherock/game/collision.c:240 | Test if a point lies inside the car's collision box. |
| `setFBCollisionForce(...)` | reference/repos/rushtherock/game/collision.c:305 | Compute front/back/side forces based on relative velocity and contact side. |
| `ForceApart(...)` | reference/repos/rushtherock/game/collision.c:272 | High-overlap separation force. |
| `setCollisionDamage(MODELDAT *m)` | reference/repos/rushtherock/game/collision.c:373 | Apply damage flags based on force magnitude and quadrant. |
| `simpleCollForce(...)` | reference/repos/rushtherock/game/collision.c:419 | Simplified collision force along center-to-center vector. |
| `CheckCrash(S16)` | reference/repos/rushtherock/game/resurrect.c:1196 | Explosion and resurrect trigger when crashflag set. |

## Collision Response Math
- Broadphase uses `colrad` sphere test: `dsq > (colrad_sum^2)` rejects.
- `setFBCollisionForce()`:
  - Computes relative velocity in other car's frame (`rvel`).
  - Forces are proportional to `rvel * 4000.0`, clamped to +/-8000 for x/y depending on contact side.
  - If major overlap, calls `ForceApart()` with fixed `force = 100000.0`.
- `simpleCollForce()`:
  - `force = (vin + 5.0) * 80000.0`, clamp 0..100000.
  - Applies along normalized center-to-center vector.

## N64 Function Mapping
| N64 Function | Address | Arcade Equivalent |
|--------------|---------|-------------------|
| `collision_check_thunk()` | `src/game/game.c:3323` | `collision()` (car-to-car driver) |
| `collision_response_calc()` | `src/game/game.c:3321` | `setFBCollisionForce()` / `simpleCollForce()` |
| `collision_narrowphase()` | `src/game/game.c:3396` | `PointInBody()` + corner tests |
| `collision_resolve()` | `src/game/game.c:4160` | `setCollisionDamage()` / apply `CENTERFORCE` |
| `collision_broadphase()` | `src/game/game.c:5304` | `colrad` sphere test |
| `track_collision()` | `src/game/game.c:7338` | Wall/track collision handling |
| `entity_collision_test()` | `src/game/game.c:1085` | World collision query (object/track) |

## Damage System
| Damage Type | Threshold | Effect |
|-------------|-----------|--------|
| Level 1 | `DAMAGE1_FORCE` = 40000 | Update damage bitfield for impacted quadrant. |
| Level 2 | `DAMAGE2_FORCE` = 155000 | Higher damage level for impacted quadrant. |

## Special Cases
- `collidable` flag: collision checks skip if false (`collision.c`).
- `SW_DEBUG10` disables collisions entirely.
- `MAX_VEL_SQ` guard skips collision for extreme velocities.
- Crash flow uses `CheckCrash()` in `resurrect.c` to trigger explosion and deathcam.
