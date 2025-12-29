# Completed: CODEX_AI_DRONE_02

## Summary
Implemented path interval finding, relative position calculation, and look-ahead point lookup for the drone maxpath helpers, and added per-drone path tracking fields.

## Findings
- Uses existing maxpath tables (`gMPathTables`, `gMPathHeaders`) as data sources for interval logic.
- `drone_find_interval()` updates segment based on forward distance and wraps at lap start.
- `drone_interval_pos()` computes xrel/yrel via segment projection in XZ plane.
- `drone_find_path_dist()` walks forward along segments with wrap to find a look-ahead point.

## Files Created/Modified
- `include/game/drone.h`
- `src/game/drone.c`
- `codex_homework/completed/completed_ai_drone_02.md`

## Notes for Claude
- Path functions are defensive against null path tables and invalid indices.
- These helpers currently use `car->dr_pos` for position and do not yet integrate into maxpath steering logic.

## Time Spent
~1.5 hours
