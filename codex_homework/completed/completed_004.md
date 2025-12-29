# Completed: Homework 004

## Summary
Documented arcade collision detection and response based on `collision.c` and `resurrect.c`, with N64 mapping to collision-related stubs in `src/game/game.c`.

## Findings
- Car-to-car collision uses bounding sphere (`colrad`) then corner-in-box tests with `PointInBody()`.
- Forces computed in `setFBCollisionForce()` are proportional to relative velocity and clamped; major overlap calls `ForceApart()`.
- Damage is driven by `CENTERFORCE` magnitude with thresholds `DAMAGE1_FORCE` and `DAMAGE2_FORCE`.
- Crash/resurrect is handled in `CheckCrash()` (explosion, death cam, resurrection timing).
- Collision checks skip when `collidable` is false or when velocity exceeds `MAX_VEL_SQ`.

## Files Created/Modified
- `docs/physics_collision_mapping.md`
- `codex_homework/completed/completed_004.md`

## Notes for Claude
- Wall/track collision is likely handled outside `collision.c` (see `track_collision` and entity collision in N64), so further mapping may require `road.c`/`stree.c` analysis.

## Time Spent
~1.0 hour
