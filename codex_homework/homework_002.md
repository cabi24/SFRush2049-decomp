# Homework 002: Document Arcade Drivetrain/Transmission System

## Objective
Create comprehensive documentation of the arcade drivetrain and transmission system to guide N64 stub implementation.

## Context
The N64 game.c has drivetrain-related functions like `drivetrain_update`, `gear_shift`, `engine_rpm_calc`. The arcade source in `drivsym.c` (~20K lines) contains the reference implementation.

## Tasks
1. Analyze these arcade files:
   - `reference/repos/rushtherock/game/drivsym.c` - Main drivetrain simulation
   - `reference/repos/rushtherock/game/modeldat.h` - MODELDAT struct (engine/trans fields)

2. Document key arcade functions:
   ```
   UpdateDriveSym()     - Main drivetrain update
   calc_engine_torque() - Engine torque curve
   do_transmission()    - Gear selection logic
   calc_wheel_torque()  - Torque to wheels
   ```

3. Extract key parameters:
   - Gear ratios (1st through 6th + reverse)
   - Engine RPM ranges (idle, redline, shift points)
   - Torque curve coefficients
   - Clutch/lockup behavior

4. Identify N64 equivalents in `src/game/game.c`:
   - Search for `gear`, `rpm`, `throttle`, `engine`, `torque`
   - Map to arcade function calls

## Deliverables
Create `docs/physics_drivetrain_mapping.md`:

```markdown
# Drivetrain System Mapping

## Overview
[2-3 sentence summary]

## Arcade Implementation

### Key Functions
| Function | File:Line | Purpose |
|----------|-----------|---------|

### Gear Ratios
| Gear | Ratio | Shift Up RPM | Shift Down RPM |
|------|-------|--------------|----------------|

### Engine Torque Curve
[Describe polynomial or lookup table]

### Transmission Logic
[State machine or decision tree]

## N64 Function Mapping
| N64 Function | Address | Arcade Equivalent | Confidence |
|--------------|---------|-------------------|------------|

## Key Constants
| Name | Arcade Value | N64 Address (if found) |
|------|--------------|------------------------|

## Implementation Notes
[What changes for N64 vs arcade]
```

## Reference Files
- `reference/repos/rushtherock/game/drivsym.c`
- `reference/repos/rushtherock/game/modeldat.h`
- `src/game/game.c` (search for drivetrain functions)
- `docs/struct_modeldat.md` (existing MODELDAT docs)

## Success Criteria
- All 6 gear ratios documented
- Engine torque curve extracted
- At least 5 N64 function mappings
- Transmission state machine documented
