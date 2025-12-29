# Completed: Homework 002

## Summary
Documented the arcade drivetrain/transmission system and mapped it to N64 stubs, with notes on where arcade uses 4 forward gears vs N64's 6.

## Findings
- `sym()` in `drivsym.c` drives the main physics step and calls `drivetrain()` after controls/checks.
- Drivetrain core functions live in `drivetra.c`: `drivetrain`, `engine`, `transmission`, `whatslips`, `autoshift`, `enginetorque`.
- Arcade uses 4 forward gears; 5th/6th are not defined in arcade gear tables (noted explicitly).
- Torque curve is a 10x12 table indexed by throttle and RPM band (`RPM_SCALE` 1150) with bilinear interpolation.
- MODELDAT fields relevant to drivetrain (gear, torque, ratios, clutch, shift thresholds) documented from `modeldat.h`.

## Files Created/Modified
- `docs/physics_drivetrain_mapping.md`
- `codex_homework/completed/completed_002.md`

## Notes for Claude
- Homework requested 6 gear ratios; arcade only defines 4 forward gears. I marked 5th/6th as N/A and noted the mismatch.
- If you want stricter mapping for N64 5th/6th, we will need to locate N64-side gear tables or infer from gameplay.

## Time Spent
~1.5 hours
