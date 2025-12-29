# Car Physics System Mapping: N64 to Arcade Source

This document maps the Rush 2049 N64 car physics system to the Rush the Rock arcade source code. It provides detailed information about the Milliken tire model, data structures, function mappings, and physics constants to assist future decompilation work.

## Table of Contents

1. [Overview](#overview)
2. [The Milliken Tire Model](#the-milliken-tire-model)
3. [Key Data Structures](#key-data-structures)
4. [Function Mapping Table](#function-mapping-table)
5. [Physics Constants](#physics-constants)
6. [Collision System Integration](#collision-system-integration)
7. [Code Organization](#code-organization)

---

## Overview

The Rush 2049 N64 physics system is derived from the Milliken Engineering vehicle dynamics model, originally purchased by Atari in 1985. The arcade source (Rush the Rock) contains the reference implementation in the `game/` directory, particularly:

- `drivsym.c` - Main physics simulation loop
- `drivsym.h` - Data structures and constants
- `tires.c` - Tire force calculations (friction circle)
- `cars.c` - Vehicle parameter definitions
- `drivetra.c` - Drivetrain simulation
- `model.c` - Rubber-band catchup system

### Simulation Architecture

The physics simulation follows this per-frame flow:

```
sym()
  |
  +-- controls()         # Read player input
  +-- checkok()          # Validate physics state
  +-- drivetrain()       # Engine/transmission/clutch
  +-- regular()
        |
        +-- road()           # Determine surface types
        +-- forces()
        |     +-- forces1()  # Tire and drag forces
        |     +-- forces2()  # Sum all forces
        +-- antispin()       # Anti-spin stabilization
        +-- torques()        # Calculate moments
        +-- accelerations()  # F=ma
        +-- velocities()     # Integrate acceleration
        +-- positions()      # Integrate velocity
  +-- mcommunication()   # Output for display/network
```

---

## The Milliken Tire Model

The tire model is the heart of the simulation. It was developed by Milliken Engineering (Doug and Bill Milliken) and extended by Max Behensky at Atari Games (1986).

### Research Foundation

The model is based on two OSU research papers:
1. "Lateral Stiffness, Cornering Stiffness, and Relaxation Length of the Pneumatic Tire" (Loeb, Ellis, Guenther)
2. "The Effect of Steering Amplitude on Relaxation Length" (Ellis)

### Key Tire Parameters

| Parameter | Description | Typical Value | Units |
|-----------|-------------|---------------|-------|
| `tradius` | Tire radius | 1.0 | feet |
| `springK` | Lateral spring constant | 340*12 = 4080 | lbs/ft |
| `rubdamp` | Lateral damping | 200 | - |
| `Cstiff` | Cornering stiffness | 16000-50000 | lbs/rad |
| `Cfmax` | Max friction coefficient | 1.0-1.3 | g |
| `invmi` | 1/moment of inertia | 0.1 | 1/(slug-ft^2) |

### Slip Angle Calculation

The slip angle (alpha) determines lateral force generation:

```c
F32 calcalpha(F32 tirev[3]) {
    F32 alpha = tirev[YCOMP];

    if (tirev[XCOMP] > 0) {
        if (tirev[XCOMP] < DAMPSPEED)  // DAMPSPEED = 400 fps
            alpha = tirev[YCOMP] / tirev[XCOMP];
        else
            alpha = tirev[YCOMP] * (1.0 / DAMPSPEED);
    }
    // Mirror for negative velocity
    ...
    return alpha;
}
```

### Lateral Force Curve

The lateral force uses a cubic polynomial that transitions from linear (Cstiff) to plateau (Cfmax):

```
Cf = k1*a - k2*|a|*a + k3*a^3
```

Where:
- `a` = slip angle
- `k1` = Cstiff / Zforce (cornering stiffness per unit load)
- `k2` = k1^2 / (3 * Cfmax)
- `k3` = k1^3 / (27 * Cfmax^2)
- `Afmax` = 3 * Cfmax / k1 (slip angle at max force)

### Friction Circle

The friction circle limits total tire force to the maximum available:

```c
void frictioncircle(MODELDAT *m, F32 tirev[3], F32 normalforce,
                    F32 torque, struct tiredes *tire,
                    F32 *sfp, F32 *trp)
```

Key behaviors:
1. **No load (normal <= 0)**: Zero forces, tire spins freely
2. **Max torque exceeded**: Full traction limit, no lateral force
3. **Partial torque**: Lateral capacity reduced by Pythagorean rule:
   ```
   Cfmax_adjusted = sqrt(Cfmax^2 - (traction/normalforce)^2)
   ```
4. **Wheel spin detection**: Compares tire angular velocity to road velocity
5. **Tire relaxation**: Uses contact patch deformation model (`patchy`)

---

## Key Data Structures

### N64: CarPhysics (include/game/physics.h)

```c
typedef struct CarPhysics {
    /* Position and orientation */
    f32     RWR[3];         // Real world position
    UVect   UV;             // 3x3 orientation matrix

    /* Velocities (body frame) */
    f32     V[3];           // Linear velocity (ft/s)
    f32     W[3];           // Angular velocity (rad/s)
    f32     magvel;         // Speed magnitude

    /* Forces and moments */
    f32     F[3];           // Total force
    f32     M[3];           // Total moment
    f32     TIREFORCE[4][3];// Per-tire forces
    f32     BODYFORCE[4][3];// Collision forces
    f32     CENTERMOMENT[3];// Anti-spin torque

    /* Suspension */
    f32     suscomp[4];     // Compression (feet)
    f32     springrate[4];  // Spring rates

    /* Drivetrain */
    f32     torque[4];      // Wheel torques
    s32     rpm;            // Engine RPM
    f32     throttle;       // 0-1
    f32     steerangle;     // radians

    /* Parameters */
    f32     mass;           // slugs
    f32     massinv;        // 1/mass
    f32     srefpcybo2;     // Aero drag
    ...
} CarPhysics;
```

### Arcade: MODELDAT (game/modeldat.h)

The arcade `MODELDAT` structure is essentially identical but uses different naming conventions and includes additional fields for network play and effects.

### Structure Mapping

| N64 (CarPhysics) | Arcade (MODELDAT) | Description |
|------------------|-------------------|-------------|
| `RWR[3]` | `RWR[3]` | World position |
| `UV.fpuvs[3][3]` | `UV.fpuvs[3][3]` | Orientation matrix |
| `V[3]` | `V[3]` | Body velocity |
| `W[3]` | `W[3]` | Angular velocity |
| `F[3]` | `F[3]` | Total force |
| `M[3]` | `M[3]` | Total moment |
| `TIREFORCE[4][3]` | `TIREFORCE[4][3]` | Tire forces |
| `BODYFORCE[4][3]` | `BODYFORCE[4][3]` | Collision forces |
| `CENTERMOMENT[3]` | `CENTERMOMENT[3]` | Anti-spin moment |
| `suscomp[4]` | `suscomp[4]` | Suspension compression |
| `springrate[4]` | `springrate[4]` | Spring rates |
| `torque[4]` | `torque[4]` | Wheel torques |
| `mass` | `mass` | Vehicle mass |
| `massinv` | `massinv` | 1/mass |
| `srefpcybo2` | `srefpcybo2` | Drag factor |
| `dt` | `dt` | Timestep |
| `thetime` | `thetime` | Simulation time |
| `crashflag` | `crashflag` | Crash state |
| `gameover` | `gameover` | Game over flag |
| `we_control` | `we_control` | AI flag |
| `carnum` | `carnum` | Car type index |

### Car Definition Structure

| N64 (Car) | Arcade (struct car) | Description |
|-----------|---------------------|-------------|
| `name` | `name` | Car name |
| `mass` | `mass` | Mass (slugs) |
| `I[3]` | `I[3]` | Inertia tensor |
| `springrate[4]` | `springrate[4]` | Spring rates |
| `farspringrate` | `farspringrate` | Front ARB rate |
| `cdamping[4]` | `cdamping[4]` | Compression damping |
| `rdamping[4]` | `rdamping[4]` | Rebound damping |
| `steerratio` | `steerratio` | Steering ratio |
| `swtpg` | `swtpg` | SW torque/G |
| `srefpcybo2` | `srefpcybo2` | Aero drag |
| `rollresist` | `rollresist` | Rolling resistance |
| `tires[4]` | `tires[4]` | Tire parameters |
| `TIRER[4][3]` | `TIRER[4][3]` | Tire positions |
| `brakebal` | `brakebal` | Front brake bias |
| `engmi` | `engmi` | Flywheel inertia |
| `dwratio` | `dwratio` | Differential ratio |
| `clutchmaxt` | `clutchmaxt` | Max clutch torque |
| `transarray[]` | `transarray[]` | Gear ratios |
| `torquecp` | `torquecp` | Torque curve ptr |
| `topgear` | `topgear` | Top gear |
| `upshiftangvel` | `upshiftangvel` | Upshift RPM |
| `downshiftangvel` | `downshiftangvel` | Downshift RPM |

### Tire Description Structure

| N64 (TireDes) | Arcade (tiredes) | Description |
|---------------|------------------|-------------|
| `tradius` | `tradius` | Tire radius |
| `springK` | `springK` | Lateral stiffness |
| `rubdamp` | `rubdamp` | Lateral damping |
| `PaveCstiff` | `PaveCstiff` | Pavement Cstiff |
| `PaveCfmax` | `PaveCfmax` | Pavement Cfmax |
| `Cstiff` | `Cstiff` | Current Cstiff |
| `Cfmax` | `Cfmax` | Current Cfmax |
| `invmi` | `invmi` | 1/MOI |
| `Afmax` | `Afmax` | Alpha at max |
| `k1, k2, k3` | `k1, k2, k3` | Force coefficients |
| `patchy` | `patchy` | Patch deformation |
| `angvel` | `angvel` | Angular velocity |
| `sliptorque` | `sliptorque` | Slip torque |
| `slipflag` | `slipflag` | Slip state |

---

## Function Mapping Table

### Main Physics Loop

| N64 Function | Arcade Function | File | Confidence | Notes |
|--------------|-----------------|------|------------|-------|
| `physics_sym` | `sym` | drivsym.c:186 | High | Main per-frame update |
| `physics_syminit` | `syminit` | drivsym.c:177 | High | Initialize simulation |
| `physics_regular` | `regular` | drivsym.c:285 | High | Standard physics cycle |
| `physics_init` | `initialize` | initiali.c | High | Full state init |

### Force Calculation

| N64 Function | Arcade Function | File | Confidence | Notes |
|--------------|-----------------|------|------------|-------|
| `physics_forces` | `forces` | drivsym.c:382 | High | Top-level forces |
| `physics_forces1` | `forces1` | drivsym.c:391 | High | Tire/drag forces |
| `physics_forces2` | `forces2` | drivsym.c:494 | High | Sum all forces |
| `physics_torques` | `torques` | drivsym.c:530 | High | Calculate moments |
| `physics_antispin` | `antispin` | drivsym.c:317 | High | Anti-spin torque |

### Motion Integration

| N64 Function | Arcade Function | File | Confidence | Notes |
|--------------|-----------------|------|------------|-------|
| `physics_accelerations` | `accelerations` | drivsym.c:560 | High | F=ma |
| `physics_velocities` | `velocities` | drivsym.c:573 | High | Integrate accel |
| `physics_positions` | `positions` | drivsym.c:594 | High | Integrate velocity |

### Tire Model

| N64 Function | Arcade Function | File | Confidence | Notes |
|--------------|-----------------|------|------------|-------|
| `tire_friction_circle` | `frictioncircle` | tires.c:248 | High | Core tire model |
| `tire_calc_alpha` | `calcalpha` | tires.c:458 | High | Slip angle |
| `tire_calc_uv` | `calctireuv` | tires.c:52 | High | Tire orientation |
| `tire_calc_force` | `dotireforce` | tires.c:120 | High | Full tire calculation |
| `tire_constants` | `tire_constants` | initiali.c | High | Derived constants |

### Drivetrain

| N64 Function | Arcade Function | File | Confidence | Notes |
|--------------|-----------------|------|------------|-------|
| `drivetrain_update` | `drivetrain` | drivetra.c:24 | High | Main drivetrain |
| `drivetrain_engine` | `engine` | drivetra.c:223 | High | Engine torque |
| `drivetrain_transmission` | `transmission` | drivetra.c:190 | High | Gear ratios |
| `drivetrain_clutch` | `whatslips` | drivetra.c:64 | High | Clutch slip |
| `drivetrain_autoshift` | `autoshift` | drivetra.c:164 | High | Auto transmission |
| `drivetrain_engine_torque` | `enginetorque` | drivetra.c:249 | High | Torque lookup |

### Utility

| N64 Function | Arcade Function | File | Confidence | Notes |
|--------------|-----------------|------|------------|-------|
| `vec_add` | `vecadd` | vecmath.c | High | Vector addition |
| `vec_sub` | `vecsub` | vecmath.c | High | Vector subtraction |
| `vec_scale` | `scalmul` | vecmath.c | High | Scalar multiply |
| `vec_cross` | `crossprod` | vecmath.c | High | Cross product |
| `vec_magnitude` | `magnitude` | vecmath.c | High | Vector length |
| `rw_to_body` | `rwtobod` | unitvecs.c | High | World to body |
| `body_to_rw` | `bodtorw` | unitvecs.c | High | Body to world |

---

## Physics Constants

### Fundamental Constants

| Constant | N64 Value | Arcade Value | Description |
|----------|-----------|--------------|-------------|
| `GRAVITY` | 32.174 | 32.2 | Gravitational acceleration (ft/s^2) |
| `MAX_VELOCITY` | 1000.0 | 1000.0 | Speed of sound limit (fps) |
| `DT_PHYSICS` | 1/60 | 1/60 | Physics timestep (sec) |

### Angle Conversions

| Constant | Value | Description |
|----------|-------|-------------|
| `radtodeg` | 180/PI | Radians to degrees |
| `degtorad` | PI/180 | Degrees to radians |
| `rpmtordps` | 2*PI/60 | RPM to radians/sec |
| `rdpstorpm` | 60/(2*PI) | Radians/sec to RPM |

### Tire Physics

| Constant | Value | Description |
|----------|-------|-------------|
| `DAMPSPEED` | 400 | Velocity for alpha damping (fps) |
| `dirtdamp` | -10 | Dirt drag coefficient |

### Anti-Spin Parameters

The anti-spin system uses per-car-type tables:

```c
// Anti-spin scale per car type (0=beginner, 3=expert)
const F32 as_scale[4] = { 1200.0, 600.0, 100.0, 0.0 };

// Anti-spin on road/air per car type and difficulty
const F32 as_road[4][3] = {
    { 600.0, 300.0, 150.0 },  // Beginner
    { 400.0, 200.0, 100.0 },  // Intermediate
    { 200.0, 100.0,  50.0 },  // Advanced
    {  10.0,   5.0,   0.0 }   // Expert
};
```

### Drivetrain Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `AUTOLOSS` | 0.925 | Torque converter efficiency |
| `MINGEAR` | 1 | Minimum gear for auto |
| `MAXGEAR` | 4 | Maximum gear (typically) |
| Friction point | 0.8 | Clutch engagement point |
| Centrifugal min | 1500 RPM | Start of clutch engagement |
| Centrifugal max | 3000 RPM | Full clutch engagement |

---

## Collision System Integration

The physics system interfaces with collision through:

### Body Forces

Each corner of the car body can receive collision forces:
```c
m->BODYFORCE[corner][component]  // 0=FR, 1=FL, 2=RR, 3=RL
```

These are summed with tire forces in `forces2()`.

### Center Forces

Special centering forces for recovery:
```c
m->CENTERFORCE[3]   // Applied at CG
m->CENTERMOMENT[3]  // Applied torques
```

### Wall Stuck Detection

The anti-spin system includes stuck detection:
```c
if ((magvel < 5.0) && (V[XCOMP] < 0.0) &&       // Slow, backing up
    (UV.fpuvs[ZCOMP][ZCOMP] > 0.0) &&            // Right-side up
    (BODYFORCE[FRONT] < 0.0)) {                   // Front contact
    // Apply torque to turn away from wall
    CENTERMOMENT[ZCOMP] = (steerangle > 0.0) ? 90000.0 : -90000.0;
}
```

### Surface Types

Road surface affects tire physics:
```c
#define ROAD_ASPHALT  0   // Full grip
#define ROAD_DIRT     1   // Reduced grip, extra drag
#define ROAD_GRASS    2   // Low grip
#define ROAD_AIR      3   // No forces
#define ROAD_WATER    4   // Variable grip
```

---

## Code Organization

### N64 Source Files

| File | Lines | Description |
|------|-------|-------------|
| `src/game/physics.c` | 934 | Main physics system |
| `src/game/car.c` | 706 | Car parameters and state |
| `src/game/tire.c` | 507 | Tire force calculation |
| `src/game/drivetrain.c` | 596 | Engine/transmission |

### N64 Header Files

| File | Description |
|------|-------------|
| `include/game/physics.h` | CarPhysics, Car, TireDes structs |
| `include/game/car.h` | Car definitions, torque curves |
| `include/game/tire.h` | TireState, friction circle API |
| `include/game/drivetrain.h` | DrivetrainState |

### Arcade Source Files

| File | Description |
|------|-------------|
| `game/drivsym.c` | Main physics loop |
| `game/drivsym.h` | MODELDAT, Car structures |
| `game/tires.c` | Tire force model |
| `game/tiredes.h` | tiredes structure |
| `game/cars.c` | Vehicle definitions |
| `game/drivetra.c` | Drivetrain simulation |
| `game/initiali.c` | Initialization functions |
| `game/model.c` | Catchup/rubber-band system |
| `game/modeldat.h` | MODELDAT fields |

---

## Decompilation Notes

### High Confidence Matches

Functions with arcade equivalents that are structurally identical:
- All vector math (vecadd, vecsub, etc.)
- Tire force calculation (frictioncircle, calcalpha)
- Drivetrain logic (autoshift, find_best_gear)
- Engine torque interpolation

### Medium Confidence

Functions adapted for N64:
- `physics_init` - May have different defaults
- `physics_road` - Different surface detection
- `physics_mcommunication` - N64-specific output

### Low Confidence / Unique

Functions that may differ significantly:
- Network-related code
- Sound integration
- Effect triggers

### Debugging Tips

1. Compare `as_scale[]` values to verify car type mapping
2. Check `dt` value (should be ~0.0167 for 60Hz)
3. Verify torque curve dimensions (10x12)
4. Watch for fixed-point vs floating-point differences

---

## References

- Milliken, W.F. and D.L. Milliken. "Race Car Vehicle Dynamics"
- Loeb, J.S. et al. "Lateral Stiffness, Cornering Stiffness, and Relaxation Length of the Pneumatic Tire" (OSU)
- Ellis, J.R. "The Effect of Steering Amplitude on Relaxation Length" (OSU)
- Rush the Rock arcade source: `reference/repos/rushtherock/`

---

*Last updated: 2025-12-29*
*Document generated for Rush 2049 N64 decompilation project*
