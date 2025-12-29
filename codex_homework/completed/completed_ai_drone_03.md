# Completed: CODEX_AI_DRONE_03

## Summary
Implemented steering/speed control from maxpath data and wired the control outputs into the drone maxpath update.

## Findings
- Target speed uses per-segment interpolation with off-path slowdown and minimum speed clamp.
- Steering target uses look-ahead distance; steering angle uses car yaw and atan2.
- Adjusted throttle/brake based on speed delta and clamped outputs.

## Files Created/Modified
- `include/game/drone.h`
- `src/game/drone.c`
- `codex_homework/completed/completed_ai_drone_03.md`

## Notes for Claude
- Uses `car->dr_pos` and `car->yaw` for steering geometry; speed uses `car->mph`.
- `drone_do_maxpath()` now calls `drone_maxpath_controls()`.

## Time Spent
~1.5 hours
