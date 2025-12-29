# Drivetrain System Mapping

## Overview
The arcade drivetrain system runs from `sym()` in `drivsym.c`, which processes controls, sanity checks, then calls `drivetrain()` for engine torque, clutch slip, transmission ratios, and torque split. It uses a torque-curve lookup (RPM x throttle) and automatic shift logic with throttle-adjusted shift points.

## Arcade Implementation

### Key Functions
| Function | File:Line | Purpose |
|----------|-----------|---------|
| `sym(MODELDAT *m)` | reference/repos/rushtherock/game/drivsym.c:146 | Main physics step; calls `controls()`, `checkok()`, `drivetrain()`, then `regular()`. |
| `drivetrain(MODELDAT *m)` | reference/repos/rushtherock/game/drivetra.c:19 | Main drivetrain update (engine, transmission, clutch, differential torque split). |
| `engine(MODELDAT *m)` | reference/repos/rushtherock/game/drivetra.c:204 | Selects torque curve, applies gear scaling and autotrans loss. |
| `transmission(MODELDAT *m)` | reference/repos/rushtherock/game/drivetra.c:112 | Sets current gear ratio and total ratio. |
| `whatslips(MODELDAT *m)` | reference/repos/rushtherock/game/drivetra.c:33 | Clutch slip/lock logic; computes `dwtorque`. |
| `autoshift(MODELDAT *m)` | reference/repos/rushtherock/game/drivetra.c:83 | Automatic gear selection with throttle-based shift points. |
| `enginetorque(...)` | reference/repos/rushtherock/game/drivetra.c:249 | Bilinear lookup into torque curve table (RPM x throttle). |

Related physics flow:
- `sym()` in `reference/repos/rushtherock/game/drivsym.c` calls `drivetrain(m)` after `controls()`.

Arcade naming note:
- `calc_engine_torque()` in the homework corresponds to `enginetorque()`.
- `do_transmission()` corresponds to `transmission()` + `autoshift()`.
- `calc_wheel_torque()` corresponds to `whatslips()` (and the torque split inside `drivetrain()`).

### Gear Ratios
Reverse/neutral are shared; forward gear ratios vary by car tier. Arcade uses 4 forward gears (no 5th/6th).

| Gear | Beginner | Advanced | Expert | Extreme |
|------|----------|----------|--------|---------|
| Reverse | -3.10 | -3.10 | -3.10 | -3.10 |
| Neutral | 0.00 | 0.00 | 0.00 | 0.00 |
| 1st | 3.10 | 3.10 | 3.10 | 3.10 |
| 2nd | 1.78 | 1.78 | 1.77 | 1.75 |
| 3rd | 1.29 | 1.28 | 1.27 | 1.25 |
| 4th | 0.99 | 0.98 | 0.97 | 0.95 |
| 5th | N/A | N/A | N/A | N/A |
| 6th | N/A | N/A | N/A | N/A |

Source: `reference/repos/rushtherock/game/cars.c` (`*_GEAR_RATIO_*` defines).

### Shift Points

| Parameter | Value | Notes |
|-----------|-------|-------|
| UPSHIFT_RPM | 6325 | Multiplied by `rpmtordps` in `cars.c` when stored in MODELDAT. |
| DNSHIFT_RPM | 4100 | Multiplied by `rpmtordps` in `cars.c`. |

Throttle affects shift points: `modshift = shift * ((3.0 + throttle) * 0.25)`.

### Engine Torque Curve

The torque curve is a 10 (throttle bands) x 12 (RPM bands) table:
- Table: `stdtorquecurve[10][12]` in `reference/repos/rushtherock/game/cars.c`.
- RPM index uses `rpmperent` (RPM per entry) which is `RPM_SCALE = 1150`.
- Throttle index uses `throttle / 14` (0..9), then bilinear interpolation between rows.

Dirt torque curve uses `stddirttorquecurve[10][12]`.

### MODELDAT Drivetrain Fields

Key MODELDAT fields used by `drivetrain()` and related code (`reference/repos/rushtherock/game/modeldat.h`):

| Field | Purpose |
|-------|---------|
| `gear`, `commandgear`, `autotrans` | Current gear, requested gear, auto/manual mode. |
| `engtorque`, `engangvel`, `enginvmi` | Engine torque, angular velocity (rad/sec), inverse inertia. |
| `dwratio`, `dwtorque`, `dwangvel`, `dwinvmi` | Differential ratio, wheel torque, wheel angular velocity, inverse inertia. |
| `clutch`, `clutchtorque`, `clutchangvel`, `clutchmaxt` | Clutch input and engagement dynamics. |
| `transratio`, `totalratio`, `transarray`, `topgear` | Current gear ratio, overall ratio, ratio table, max gear. |
| `rpmperent`, `torquecurve`, `dirttorquecurve` | Torque curve indexing and tables (RPM x throttle). |
| `upshiftangvel`, `downshiftangvel`, `shifttime` | Auto shift thresholds and shift delay. |
| `magicdif`, `dwslipflag` | Torque split behavior and wheel slip flag. |

### Transmission Logic

- **Auto shift** (`autoshift`):
  - If gear is neutral/reverse, assigns directly from `commandgear`.
  - Else, if in N/R, calls `find_best_gear` based on current driveshaft speed.
  - Upshift if `engangvel > modupshiftangvel`, downshift if `engangvel < moddownshiftangvel`.
  - Shift delay enforced by `shifttime` (1 second).

- **Clutch logic** (`whatslips`):
  - Auto uses a centrifugal clutch:
    - Full engagement below `clutchangvel - 100 RPM`.
    - Linear ramp between ~1500–3000 RPM.
  - Manual uses `clutch` with friction point at 0.8.
  - Determines effective inverse inertia (`efdwinvmi`) and `dwtorque`.

### Differential / Wheel Torque Split

- `dwtorque = clutchtorque * totalratio`.
- Rear-wheel drive with optional load-sensitive split (`magicdif` flag):
  - Default 50/50 split.
  - If one wheel unloaded, route torque to the other wheel.

## N64 Function Mapping

| N64 Function | Address | Arcade Equivalent | Confidence |
|--------------|---------|-------------------|------------|
| `drivetrain_update()` | `src/game/drivetrain.c` | `drivetrain()` | High |
| `drivetrain_engine()` | `src/game/drivetrain.c` | `engine()` | High |
| `drivetrain_transmission()` | `src/game/drivetrain.c` | `transmission()` | High |
| `drivetrain_clutch()` | `src/game/drivetrain.c` | `whatslips()` | High |
| `drivetrain_autoshift()` | `src/game/drivetrain.c` | `autoshift()` | High |
| `drivetrain_engine_torque()` | `src/game/drivetrain.c` | `enginetorque()` | High |
| `throttle_brake_input()` | `src/game/game.c` | `drivetrain()` + `controls()` | Med |
| `car_gear_shift()` | `src/game/game.c` | `autoshift()` / manual shift logic | Med |
| `physics_drivetrain()` | `src/game/physics.c` | `drivetrain()` (simplified) | Low |

## Key Constants

| Name | Arcade Value | N64 Address (if found) |
|------|--------------|------------------------|
| `GEAR_RATIO_REV` | -3.10 | N/A |
| `GEAR_RATIO_NEU` | 0.0 | N/A |
| `UPSHIFT_RPM` | 6325 | N/A |
| `DNSHIFT_RPM` | 4100 | N/A |
| `RPM_SCALE` | 1150 | N/A |
| `TORQUE_SCALE` | 1.8 | N/A |
| `AUTOLOSS` | 0.925 | N/A |

## Implementation Notes

- Arcade uses 4 forward gears + reverse + neutral. N64 `car_gear_shift` allows 6 forward gears; confirm if higher gears are actually used or placeholders.
- Torque curves are discrete tables with bilinear interpolation; N64 currently uses a simplified torque curve (in `throttle_brake_input`).
- Clutch logic (centrifugal in auto, friction point in manual) is a major behavior gap between arcade and N64 stubs.
- Arcade `drivetrain()` integrates `magicdif` torque split, which is not yet modeled in N64 `physics_drivetrain`.
