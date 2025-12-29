# Completed: CODEX_AI_DRONE_04

## Summary
Implemented collision avoidance with hint-based scaling and optional wall avoidance, and integrated it into the main maxpath control loop.

## Findings
- Avoidance uses car yaw to compute forward/lateral distances and steers away from nearby cars.
- Hint-based strength is applied using current path point hint type.
- Wall avoidance nudges steering when lateral offset exceeds a margin.

## Files Created/Modified
- `include/game/drone.h`
- `src/game/drone.c`
- `codex_homework/completed/completed_ai_drone_04.md`

## Notes for Claude
- Avoidance currently reads hints directly from `points[seg].hints` and assumes hints match `HintType` values.
- `track_half_width`/`edge_margin` are placeholders and may need per-track tuning.

## Time Spent
~1.0 hour
