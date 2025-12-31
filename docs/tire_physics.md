# Tire Physics Model - Rush 2049 N64 Decompilation

This document provides a comprehensive analysis of the tire physics model used in San Francisco Rush 2049 for N64, based on the original arcade source code at `reference/repos/rushtherock/game/tires.c`.

## Table of Contents

1. [Overview](#1-overview)
2. [Tire Force Components](#2-tire-force-components)
3. [Slip Angle and Slip Ratio](#3-slip-angle-and-slip-ratio)
4. [Force Curve Coefficients (Pseudo-Pacejka)](#4-force-curve-coefficients-pseudo-pacejka)
5. [Contact Patch Model](#5-contact-patch-model)
6. [Tire Temperature Effects](#6-tire-temperature-effects)
7. [N64 vs Arcade Differences](#7-n64-vs-arcade-differences)
8. [Function Mapping](#8-function-mapping)

---

## 1. Overview

The tire physics system is based on the **Milliken tire model**, developed by Max Behensky at Atari Games on November 25, 1986. The implementation draws from two Ohio State University research papers:

1. **"Lateral Stiffness, Cornering Stiffness, and Relaxation Length of the Pneumatic Tire"**
   - Authors: Jeffery S. Loeb (M.S.), John R. Ellis (Ph.D.), Dennis A. Guenther (Ph.D., P.E.)
   - Establishes the extended tire model for linear characteristics

2. **"The Effect of Steering Amplitude on Relaxation Length"**
   - Author: John R. Ellis (Ph.D.)
   - Extends the model into non-linear tire behavior regions

The model implements:
- Friction circle constraint for combined slip
- Non-linear tire force curves (cubic polynomial)
- Contact patch relaxation dynamics
- Surface-dependent grip coefficients
- Anti-roll bar suspension coupling

---

## 2. Tire Force Components

### 2.1 Coordinate System

Forces are calculated in the tire coordinate frame:
- **X-axis (XCOMP)**: Forward/longitudinal direction
- **Y-axis (YCOMP)**: Lateral/sideways direction
- **Z-axis (ZCOMP)**: Vertical/normal direction

### 2.2 Lateral Force (Cornering Force)

The lateral force is the primary cornering force generated when the tire has a non-zero slip angle.

**Linear Region (small slip angles)**:
```
Fy = -Cstiff * alpha
```

**Non-Linear Region (polynomial approximation)**:
```
Fy = -(k1*alpha - k2*alpha^2 + k3*alpha^3) * Fn
```

Where:
- `Cstiff`: Cornering stiffness (lb/radian) - typically 16,000-50,000
- `alpha`: Slip angle (radians)
- `Fn`: Normal (vertical) force
- `k1, k2, k3`: Polynomial coefficients derived from tire constants

**Saturation Behavior**: When slip angle exceeds `Afmax` (angle at maximum force), the tire enters a sliding regime where force is limited to `Cfmax * Fn`.

### 2.3 Longitudinal Force (Traction/Braking)

The longitudinal force arises from wheel torque (acceleration or braking).

**Pure Rolling**:
```
Fx = torque / radius
```

**Wheel Spin (accelerating beyond grip)**:
```
sliptorque = maxtorque
angvel += (torque - sliptorque) * invmi * dt
```

**Maximum Traction**:
```
maxtraction = Cfmax * normalforce
maxtorque = maxtraction * radius
```

### 2.4 Combined Slip (Friction Circle)

The friction circle limits the total horizontal force the tire can produce:

```
Ftotal = sqrt(Fx^2 + Fy^2) <= Cfmax * Fn
```

When both lateral and longitudinal forces are demanded, they share the available grip:

**Reduced Lateral Capacity under Traction**:
```c
// From arcade frictioncircle()
temp = traction / normalforce;
Cfmax_reduced = sqrt(Cfmax^2 - temp^2);
```

**Force Distribution when Slipping**:
```c
patchvel = tirev[X] - angvel * radius;
patchspeed = sqrt(patchvel^2 + tirev[Y]^2);
traction = -maxtraction * patchvel / patchspeed;
sideforce = -maxtraction * tirev[Y] / patchspeed;
```

This ensures forces are proportioned based on the slip velocity direction.

### 2.5 Normal Force (Suspension)

The vertical force comes from suspension compression:

```c
if (suscomp > 10.0)  // High compression (bump stop)
    if (tirev[Z] > -1.0)
        Fz = (tirev[Z] + 1.0) * mass * -0.25 * idt;
    else
        Fz = arforce + suscomp * springrate + damping * tirev[Z];
else if (suscomp > 0)
    Fz = arforce + suscomp * springrate + damping * tirev[Z];
else
    Fz = 0;
```

**Anti-Roll Bar Force**:
```c
arforce = (suscomp - other_suscomp) * arspringrate;
```

**Damping Selection**:
```c
damping = (tirev[Z] > 0) ? cdamping : rdamping;
```

---

## 3. Slip Angle and Slip Ratio

### 3.1 Slip Angle Calculation

The slip angle is the angle between the tire's heading and its velocity vector.

**Arcade Implementation** (`calcalpha()` at tires.c:458-477):

```c
#define DAMPSPEED 400  // ft/s threshold

F32 calcalpha(F32 tirev[3]) {
    F32 alpha;

    alpha = tirev[YCOMP];

    if (tirev[XCOMP] > 0) {
        if (tirev[XCOMP] < DAMPSPEED)
            alpha = tirev[YCOMP] / tirev[XCOMP];
        else
            alpha = tirev[YCOMP] / DAMPSPEED;  // Clamp for high speed
    }
    else if (tirev[XCOMP] < 0) {
        if (tirev[XCOMP] > -DAMPSPEED)
            alpha = -tirev[YCOMP] / tirev[XCOMP];
        else
            alpha = tirev[YCOMP] / DAMPSPEED;
    }

    return alpha;
}
```

Key characteristics:
- Uses small-angle approximation: `alpha = atan(Vy/Vx) ~ Vy/Vx`
- DAMPSPEED (400 ft/s) prevents numerical instability at low speeds
- Sign handling for reverse direction

### 3.2 Slip Ratio Calculation

The slip ratio quantifies wheel spin or lockup.

**N64 Implementation** (wheel_rotation_update at game.c:11237-11250):

```c
roadSpeed = patchVel[0];
wheelSpeed = angVel * radius;

if (fabsf(roadSpeed) > 0.1f || fabsf(wheelSpeed) > 0.1f) {
    maxSpeed = fmaxf(fabsf(roadSpeed), fabsf(wheelSpeed));
    slipRatio = (wheelSpeed - roadSpeed) / maxSpeed;
} else {
    slipRatio = 0.0f;
}
```

Interpretation:
- `slipRatio > 0`: Wheel spinning faster than road (acceleration wheelspin)
- `slipRatio < 0`: Wheel slower than road (braking lockup)
- `slipRatio = 0`: Pure rolling

---

## 4. Force Curve Coefficients (Pseudo-Pacejka)

The tire force model uses a cubic polynomial rather than a true Pacejka "Magic Formula," but achieves similar characteristic shapes.

### 4.1 Coefficient Derivation

From arcade `tire_constants()` (initiali.c:377-391):

```c
void tire_constants(TireDes *tire) {
    f32 Cfmax = tire->Cfmax;
    f32 k1 = tire->Cstiff;

    // Slip angle at maximum force
    tire->Afmax = (3.0f * Cfmax) / k1;

    // Polynomial coefficients: Cf = k1*a - k2*a^2 + k3*a^3
    tire->k1 = k1;
    tire->k2 = (k1 * k1) / (3.0f * Cfmax);
    tire->k3 = (k1 * k1 * k1) / (27.0f * Cfmax * Cfmax);

    // Derivative coefficients for transient response
    tire->l2 = tire->k2;
    tire->l3 = tire->k3;
}
```

### 4.2 Coefficient Table

| Coefficient | Formula | Purpose |
|-------------|---------|---------|
| `Afmax` | 3 * Cfmax / k1 | Slip angle at peak force |
| `k1` | Cstiff | Linear slope (cornering stiffness) |
| `k2` | k1^2 / (3 * Cfmax) | Quadratic term (peak shape) |
| `k3` | k1^3 / (27 * Cfmax^2) | Cubic term (saturation curve) |

### 4.3 Force Curve Equation

```
Fy(alpha) = -(k1*alpha - k2*alpha^2 + k3*alpha^3) * Fn    for alpha >= 0
Fy(alpha) = -(k1*alpha + k2*alpha^2 + k3*alpha^3) * Fn    for alpha < 0
```

This produces a curve that:
1. Rises linearly with slip angle at low alpha
2. Reaches a peak at `alpha = Afmax`
3. Falls off at higher slip angles (tire sliding)

### 4.4 Comparison to Pacejka Magic Formula

| Feature | Pacejka | Rush Model |
|---------|---------|------------|
| Formula | `D*sin(C*atan(B*x - E*(B*x - atan(B*x))))` | `k1*a - k2*a^2 + k3*a^3` |
| Parameters | B, C, D, E | k1, k2, k3, Cfmax |
| Load sensitivity | Separate coefficients per load | Scales with normal force |
| Combined slip | Explicit friction ellipse | Friction circle with reduced Cfmax |
| Complexity | High | Medium |

---

## 5. Contact Patch Model

### 5.1 Contact Patch Relaxation

The model tracks contact patch lateral displacement (`patchy`) for smooth force transitions:

```c
// From frictioncircle() at tires.c:375-412
p = tirev[XCOMP] * tire->springK /
    (normalforce * (tire->k1 - l2*alpha + l3*alpha*alpha));
if (p < 0) p = -p;

if (p * m->dt < 0.5) {
    // Transient response
    ydot = -p * tire->patchy - tirev[YCOMP];
    tire->patchy += ydot * m->dt;
    *sfp = tire->springK * tire->patchy + tire->rubdamp * ydot;
} else {
    // Steady-state (polynomial curve)
    *sfp = -(tire->k1*alpha - k2*alpha*alpha +
             k3*alpha*alpha*alpha) * normalforce;
    tire->patchy = *sfp / tire->springK;
}
```

### 5.2 Contact Patch Parameters

| Parameter | Description | Typical Value |
|-----------|-------------|---------------|
| `springK` | Lateral spring constant | 4080 lb/ft (340*12) |
| `rubdamp` | Lateral damping | 200 |
| `patchy` | Lateral displacement | Dynamic, ~0-0.1 ft |

### 5.3 Relaxation Length

The contact patch model implicitly handles relaxation length - the distance a tire must roll before fully developing lateral force. The parameter `p` controls the transition rate:

```
relaxation_rate = springK * forward_velocity / (Fn * tire_stiffness)
```

At low speeds, force builds slowly (high relaxation). At high speeds, force responds quickly.

---

## 6. Tire Temperature Effects

### 6.1 Arcade Implementation

The arcade source does **not** implement tire temperature physics. The comment at tires.c:131-137 shows surface-based grip adjustment:

```c
if (tire->Cstiff != tire->PaveCstiff) {
    tire->Cstiff = tire->PaveCstiff;
    tire->Cfmax = tire->PaveCfmax;
    tire_constants(tire);
}
```

Disabled code suggests planned features:
```c
#if 0  // Not enabled
if (poortract) {
    tire->Cstiff = tire->PaveCstiff * 0.8f;
    tire->Cfmax = tire->PaveCfmax * 0.8f;
}
if (airfact < 0.5f) {
    tire->Cstiff = tire->PaveCstiff * (0.5f + airfact);
}
#endif
```

### 6.2 N64 Implementation

The N64 version implements surface-based friction modification in `tire.c`:

```c
f32 tire_surface_friction(u8 surface) {
    switch (surface) {
        case SURFACE_PAVEMENT: return 1.0f;
        case SURFACE_DIRT:     return 0.7f;
        case SURFACE_GRASS:    return 0.6f;
        case SURFACE_SAND:     return 0.5f;
        case SURFACE_ICE:      return 0.2f;
        case SURFACE_WATER:    return 0.4f;
        default:               return 1.0f;
    }
}

void tire_update_constants(TireState *tire, u8 surface) {
    f32 friction = tire_surface_friction(surface);
    tire->params.Cstiff = tire->params.PaveCstiff * friction;
    tire->params.Cfmax = tire->params.PaveCfmax * friction;
    tire_constants(&tire->params);
}
```

### 6.3 Heat Model Considerations

Neither version implements true thermal tire modeling:
- No heat generation from slip
- No heat dissipation over time
- No optimal temperature window
- No grip degradation from overheating

This was likely omitted for:
1. Computational cost
2. Gameplay simplicity (arcade focus)
3. Memory constraints on N64

---

## 7. N64 vs Arcade Differences

### 7.1 Simplifications on N64

| Feature | Arcade | N64 | Notes |
|---------|--------|-----|-------|
| Tire coordinate transforms | Full 3x3 rotation matrix | Simplified | `make_tire_road_uvs()` not found on N64 |
| Aligning torque | m1-m4 coefficients | Not implemented | Steering feel feature |
| Contact patch tracking | `patchy` variable | Likely omitted | Simpler force model |
| Unit vector precision | 16-bit scaled integers | 32-bit float | Hardware differences |
| Terrain grip modifiers | Planned but disabled | Implemented | N64 has more surfaces |

### 7.2 Structure Size Comparison

**Arcade tiredes** (100 bytes):
```c
typedef struct tiredes {
    F32 tradius;        // 0x00
    F32 springK;        // 0x04
    F32 rubdamp;        // 0x08
    F32 PaveCstiff;     // 0x0C
    F32 PaveCfmax;      // 0x10
    F32 Cstiff;         // 0x14
    F32 Cfmax;          // 0x18
    F32 invmi;          // 0x1C
    F32 Zforce;         // 0x20
    F32 Afmax;          // 0x24
    F32 k1, k2, k3;     // 0x28-0x30
    F32 l2, l3;         // 0x34-0x38
    F32 m1,m2,m3,m4;    // 0x3C-0x48
    F32 patchy;         // 0x4C
    F32 angvel;         // 0x50
    F32 sliptorque;     // 0x54
    F32 sideforce;      // 0x58
    F32 traction;       // 0x5C
    char slipflag;      // 0x60
} tiredes;
```

**N64 Tire Structure** (56 bytes inferred):
```c
typedef struct N64_TireState {
    f32 rotAngle;       // 0x00
    f32 angVel;         // 0x04
    f32 radius;         // 0x08
    f32 slipAngle;      // 0x0C
    f32 slipRatio;      // 0x10
    f32 contactVel[3];  // 0x14-0x1C
    f32 normalForce;    // 0x20
    f32 latForce;       // 0x24
    f32 lonForce;       // 0x28
    f32 cStiff;         // 0x2C
    f32 cfMax;          // 0x30
    f32 driveTorque;    // 0x34
} N64_TireState;
```

### 7.3 Performance Considerations

**N64 Constraints**:
- 93.75 MHz CPU vs arcade's higher clock
- Limited FPU throughput
- 4MB RAM (expandable to 8MB)
- 4 cars maximum vs arcade's 1-2

**Optimizations Applied**:
- Reduced calculation frequency (possibly every other frame)
- Simplified polynomial evaluation
- Fixed-point where possible
- Shared tire constants per car type

---

## 8. Function Mapping

### 8.1 Arcade to N64 Function Map

| Arcade Function | Arcade Location | N64 Function | N64 Address | Confidence |
|-----------------|-----------------|--------------|-------------|------------|
| `calctireuv()` | tires.c:52-118 | `wheel_rotation_update()` | 0x800A4940 | Medium |
| `dotireforce()` | tires.c:120-243 | `suspension_update()` | 0x800A4CC0 | High |
| `frictioncircle()` | tires.c:248-456 | `tire_force_calc()` | 0x800A7E00 | High |
| `calcalpha()` | tires.c:458-477 | (inline) | game.c:11252-11260 | High |
| `make_tire_road_uvs()` | tires.c:480-527 | (not found) | - | Low |
| `tire_constants()` | initiali.c:377-391 | `tire_constants()` | tire.c:89 | High |

### 8.2 Symbol Address Reference

From `symbol_addrs.us.txt`:

```
// Core tire physics
tire_force_calc = 0x800A7E00;      // 1200 bytes - friction circle
suspension_update = 0x800A4CC0;    // 412 bytes - spring/damper
wheel_rotation_update = 0x800A4940; // 388 bytes - slip angle/ratio

// Tire parameters
tire_params_load = 0x800A5A74;     // Load tire data
tire_compound_set = 0x800A5BB8;    // Set tire compound

// Additional functions
tire_calc_force = 0x800BA000;      // Full tire force wrapper
tire_suspension_update = 0x800BA800; // Suspension wrapper
tire_skid_mark = 0x800A4E60;       // Visual skid marks
tire_sound_update = 0x800B338C;    // Tire squeal audio

// Tire state globals
gTireForce = 0x801528B8;           // f32[4][3] - force vectors
gTirePosition = 0x801528E8;        // f32[4][3] - body coords
gTireVelocity = 0x80152948;        // f32[4][3] - velocities
gTireAngularVelocity = 0x80152978; // f32[4] - wheel spin
gTireSlipAngle = 0x80152988;       // f32[4] - slip angles
gTireSlipFlag = 0x80152998;        // s8[4] - slip state
gTireTraction = 0x801529AC;        // f32[4] - traction
gTireSideForce = 0x801529BC;       // f32[4] - lateral force
gTireCornStiff = 0x801529DC;       // f32[4] - cornering stiffness
gTireFrictionMax = 0x801529EC;     // f32[4] - max friction
```

### 8.3 Slip Flag Values

From `include/game/tire.h`:

```c
#define SLIP_NONE           0   // Pure rolling
#define SLIP_LOCKED_BRAKE   10  // Wheels locked, returning to rolling
#define SLIP_ACCEL_SPIN     20  // Wheelspin during acceleration
#define SLIP_DECEL_SPIN     30  // Deceleration spin (rare)
#define SLIP_BRAKE_SPIN     40  // Brake lockup
#define SLIP_NO_LOAD        3   // Tire in air (no normal force)
#define SLIP_MAX_ACCEL      4   // At maximum acceleration grip
#define SLIP_MAX_BRAKE      5   // At maximum braking grip
#define SLIP_SIDE_MAX_POS   6   // Positive slip angle saturated
#define SLIP_SIDE_GRIP_POS  7   // Positive slip, gripping
#define SLIP_SIDE_MAX_NEG   8   // Negative slip angle saturated
#define SLIP_SIDE_GRIP_NEG  9   // Negative slip, gripping
```

Slip flags are used for:
- Tire squeal audio triggering
- Visual smoke/skid effects
- Debug display

---

## References

### Source Files

**Arcade**:
- `/reference/repos/rushtherock/game/tires.c` - Main tire physics
- `/reference/repos/rushtherock/game/tiredes.h` - Tire structure
- `/reference/repos/rushtherock/game/drivsym.h` - Vehicle definitions
- `/reference/repos/rushtherock/game/initiali.c` - Tire constant calculation

**N64**:
- `/src/game/tire.c` - Tire physics implementation
- `/src/game/game.c` - Contains tire_force_calc, wheel_rotation_update
- `/include/game/tire.h` - Tire structures and constants
- `/include/game/car.h` - TireDes structure

### Research Papers

1. Loeb, Ellis, Guenther. "Lateral Stiffness, Cornering Stiffness, and Relaxation Length of the Pneumatic Tire." Ohio State University.

2. Ellis, John R. "The Effect of Steering Amplitude on Relaxation Length." Ohio State University.

### Related Documentation

- `/docs/physics_tire_mapping.md` - Detailed function pseudocode
- `/docs/car_physics.md` - Complete vehicle physics overview
- `/docs/car_physics_mapping.md` - Arcade to N64 physics mapping
