# Tire Physics System Mapping: Arcade to N64

This document maps the arcade tire physics system (Rush The Rock) to the N64 version (Rush 2049).

## Overview

The tire physics system is based on the **Milliken tire model**, developed by Max Behensky at Atari Games in 1986. The model implements:
- Extended relaxation length theory for lateral forces
- Friction circle for combined slip
- Suspension forces with anti-roll bar
- Non-linear tire characteristics at high slip angles

The system is documented in two research papers from Ohio State University:
1. "Lateral Stiffness, Cornering Stiffness, and Relaxation Length of the Pneumatic Tire" (Loeb, Ellis, Guenther)
2. "The Effect of Steering Amplitude on Relaxation Length" (Ellis)

---

## 1. Function Mapping Table

| Arcade Function | Location | N64 Equivalent | Location | Confidence |
|----------------|----------|----------------|----------|------------|
| `calctireuv()` | tires.c:52-118 | `wheel_rotation_update()` | game.c:11157 | Medium |
| `dotireforce()` | tires.c:120-243 | `suspension_update()` | game.c:11218 | High |
| `frictioncircle()` | tires.c:248-456 | `tire_force_calc()` | game.c:12274 | High |
| `calcalpha()` | tires.c:458-477 | (inline in wheel_rotation_update) | game.c:11205-11213 | High |
| `make_tire_road_uvs()` | tires.c:480-527 | (not found - likely simplified) | - | Low |
| `tire_constants()` | initiali.c:377-391 | (not decompiled - likely ROM data) | - | Low |
| `copy_tire_info()` | initiali.c:364-374 | (not found) | - | Low |

---

## 2. Algorithm Descriptions with Formulas

### 2.1 calctireuv() - Tire Coordinate Basis

**Purpose**: Calculates the tire's local coordinate system (unit vectors) and tire velocity in the wheel reference frame.

**Algorithm**:
1. Calculate rotational velocity component: `cartirev = W x R`
2. Add translational velocity: `cartirev += V`
3. Apply steering rotation to front tires
4. Project road surface normal to tire frame
5. Pitch-correct tire unit vectors to road surface

**Key Formula**:
```
tirev = bodtorw(cartirev, tireuvs)
```

Where:
- `W` = angular velocity vector (rad/s)
- `R` = tire position relative to CG (TIRER)
- `V` = body velocity (ft/s)
- `steer` = steering angle (radians)

**N64 Equivalent**: `wheel_rotation_update()` calculates slip angle and slip ratio from contact patch velocity.

---

### 2.2 dotireforce() - Suspension and Friction Forces

**Purpose**: Calculates the complete tire force vector including suspension spring/damper forces and tire friction forces.

**Algorithm**:
1. Update tire surface grip constants (Cstiff, Cfmax) based on terrain
2. Calculate anti-roll bar force between tire pairs
3. Apply suspension spring and damper forces
4. Call `frictioncircle()` for lateral and longitudinal forces
5. Transform forces to body coordinates

**Key Formulas**:

Suspension Force (Normal):
```
if (suscomp > 10.0)
    Fz = arforce + suscomp * springrate + damping * tirev[Z]
else if (suscomp > 0)
    Fz = arforce + suscomp * springrate + damping * tirev[Z]
else
    Fz = 0
```

Anti-Roll Bar Force:
```
arforce = (suscomp - otsuscomp) * arspringrate
```

Damping Selection:
```
if (tirev[Z] > 0)  // Compression
    damping = cdamping
else
    damping = rdamping
```

**Parameters**:
| Parameter | Description | Typical Value |
|-----------|-------------|---------------|
| suscomp | Suspension compression (ft) | 0-0.5 |
| springrate | Spring rate (lb/ft) | 1000-2000 |
| arspringrate | Anti-roll bar rate (lb/ft) | 500-1500 |
| cdamping | Compression damping (lb-s/ft) | 100-300 |
| rdamping | Rebound damping (lb-s/ft) | 100-300 |

**N64 Equivalent**: `suspension_update()` at game.c:11218

---

### 2.3 frictioncircle() - Friction Circle Model

**Purpose**: Implements the friction circle to limit combined lateral and longitudinal tire forces.

**Algorithm**:
1. Calculate effective torque accounting for wheel inertia
2. Determine max traction from normal force: `maxtraction = Cfmax * Fn`
3. Check for wheel spin or lockup conditions
4. Calculate slip ratio-based longitudinal force
5. Calculate slip angle-based lateral force
6. Apply friction circle limit to combined forces

**Key Formulas**:

Effective Torque:
```
temp = mass * tradius
torque_eff = temp * torque / (temp + 1.0/invmi)
```

Max Traction:
```
maxtraction = Cfmax * normalforce
maxtorque = maxtraction * tradius
```

Road Angular Velocity:
```
roadangvel = tirev[X] / tradius
```

Wheel Spin Detection:
```
if (angvel > roadangvel)  // Accelerating/wheelspin
    sliptorque = maxtorque
    angvel += (torque - sliptorque) * invmi * dt
```

Contact Patch Velocity:
```
patchvel = tirev[X] - angvel * tradius
patchspeed = sqrt(patchvel^2 + tirev[Y]^2)
```

Force Distribution (when slipping):
```
traction = -maxtraction * patchvel / patchspeed
sideforce = -maxtraction * tirev[Y] / patchspeed
```

Lateral Force Polynomial (non-slipping):
```
sideforce = -(k1*alpha - k2*alpha^2 + k3*alpha^3) * normalforce
```

**Friction Circle Limit**:
```
Cfmax_reduced = sqrt(Cfmax^2 - (traction/normalforce)^2)
```

**N64 Equivalent**: `tire_force_calc()` at game.c:12274

---

### 2.4 calcalpha() - Slip Angle Calculation

**Purpose**: Calculates the tire slip angle from tire velocity components.

**Algorithm**:
```c
if (tirev[X] > 0) {
    if (tirev[X] < DAMPSPEED)  // DAMPSPEED = 400 ft/s
        alpha = tirev[Y] / tirev[X]
    else
        alpha = tirev[Y] / DAMPSPEED
}
else if (tirev[X] < 0) {
    if (tirev[X] > -DAMPSPEED)
        alpha = -tirev[Y] / tirev[X]
    else
        alpha = tirev[Y] / DAMPSPEED
}
```

**Key Points**:
- Slip angle is the arctangent of lateral/longitudinal velocity
- Small angle approximation: `alpha = atan(Vy/Vx) approx Vy/Vx`
- DAMPSPEED prevents divide-by-zero at low speeds
- Sign handling for reverse direction

**N64 Equivalent**: Inline in `wheel_rotation_update()` at game.c:11205-11213

---

## 3. Key Physics Constants

### 3.1 Tire Structure (tiredes)

| Field | Description | Typical Front | Typical Rear |
|-------|-------------|---------------|--------------|
| tradius | Tire radius (ft) | 1.0 | 1.0 |
| springK | Lateral spring (lb/ft) | 4080 (340*12) | 4080 |
| rubdamp | Lateral damping | 200 | 200 |
| PaveCstiff | Cornering stiffness on pavement | 16000 | 32000-50000 |
| PaveCfmax | Max friction on pavement | 1.0-1.25 | 1.1-1.3 |
| Cstiff | Active cornering stiffness | = PaveCstiff | = PaveCstiff |
| Cfmax | Active max friction | = PaveCfmax | = PaveCfmax |
| invmi | Wheel 1/moment of inertia | 0.1 | 0.1 |
| Zforce | Static loading (lb) | Calculated | Calculated |

### 3.2 Derived Constants (from tire_constants())

```c
Afmax = 3 * Cfmax * Zforce / Cstiff    // Alpha at max force
k1 = Cstiff / Zforce                    // Linear coefficient
k2 = k1^2 / (3 * Cfmax)                 // Quadratic coefficient
k3 = k1^3 / (27 * Cfmax^2)              // Cubic coefficient
l2 = k2 * 2                             // Doubled k2
l3 = k3 * 3                             // Tripled k3
m1 = 40 * 0.052 / (Cfmax / 2)           // Aligning torque coef 1
m2 = m1^2 / 3.4                         // Aligning torque coef 2
m3 = m1^3 / 46.3                        // Aligning torque coef 3
m4 = 1.0 / 0.000055                     // Aligning torque coef 4
```

### 3.3 Model Constants

| Constant | Value | Description |
|----------|-------|-------------|
| GRAVCON | 32.2 | Gravity acceleration (ft/s^2) |
| DAMPSPEED | 400 | Slip angle damping threshold (ft/s) |
| radtosdeg | ~5729.58 | Radians to scaled degrees (0x4000 = 90 deg) |
| degtorad | ~0.01745 | Degrees to radians |

---

## 4. Pseudocode for N64 Implementation

### 4.1 tire_force_calc() Implementation

```c
void tire_force_calc(TireState *tire, f32 normalForce, f32 driveTorque) {
    f32 slipAngle, slipRatio;
    f32 maxTraction, lateralForce, tractionForce;
    f32 patchVel, patchSpeed, roadAngVel;
    f32 forceMag;

    // Skip if no contact
    if (normalForce <= 0.0f) {
        tire->latForce = 0.0f;
        tire->lonForce = 0.0f;
        return;
    }

    // Maximum friction force
    maxTraction = tire->Cfmax * normalForce;

    // Road angular velocity (if pure rolling)
    roadAngVel = tire->contactVel[0] / tire->radius;

    // Calculate slip ratio
    f32 wheelSpeed = tire->angVel * tire->radius;
    f32 roadSpeed = tire->contactVel[0];
    if (fabsf(roadSpeed) > 0.1f || fabsf(wheelSpeed) > 0.1f) {
        f32 maxSpeed = fmaxf(fabsf(roadSpeed), fabsf(wheelSpeed));
        slipRatio = (wheelSpeed - roadSpeed) / maxSpeed;
    } else {
        slipRatio = 0.0f;
    }

    // Calculate slip angle
    if (fabsf(tire->contactVel[0]) > 0.1f) {
        slipAngle = tire->contactVel[1] / tire->contactVel[0];
        slipAngle = CLAMP(slipAngle, -0.5f, 0.5f);
    } else {
        slipAngle = 0.0f;
    }

    // Check for wheel spin/lockup
    if (tire->angVel > roadAngVel + 0.1f) {
        // Spinning - apply max traction proportionally
        patchVel = roadSpeed - wheelSpeed;
        patchSpeed = sqrtf(patchVel * patchVel +
                          tire->contactVel[1] * tire->contactVel[1]);

        if (patchSpeed > 0.01f) {
            tractionForce = -maxTraction * patchVel / patchSpeed;
            lateralForce = -maxTraction * tire->contactVel[1] / patchSpeed;
        } else {
            tractionForce = maxTraction;
            lateralForce = 0.0f;
        }

        // Update wheel speed
        tire->slipTorque = maxTraction * tire->radius;
        tire->angVel += (driveTorque - tire->slipTorque) * tire->invMI * DT;

    } else if (tire->angVel < roadAngVel - 0.1f) {
        // Locked - apply max braking proportionally
        // Similar to spinning case but reversed
        ...

    } else {
        // Pure rolling - use polynomial force curve
        tire->angVel = roadAngVel;

        // Longitudinal from torque
        tractionForce = driveTorque / tire->radius;

        // Lateral from slip angle polynomial
        // F = -(k1*a - k2*a^2 + k3*a^3) * Fn  for positive alpha
        f32 a = fabsf(slipAngle);
        f32 Fy_coef = tire->k1 * a - tire->k2 * a * a + tire->k3 * a * a * a;
        lateralForce = -Fy_coef * normalForce;
        if (slipAngle < 0.0f) lateralForce = -lateralForce;

        // Apply friction circle
        forceMag = sqrtf(lateralForce * lateralForce +
                        tractionForce * tractionForce);
        if (forceMag > maxTraction) {
            f32 scale = maxTraction / forceMag;
            lateralForce *= scale;
            tractionForce *= scale;
        }
    }

    tire->latForce = lateralForce;
    tire->lonForce = tractionForce;
}
```

### 4.2 suspension_update() Implementation

```c
void suspension_update(SuspState *susp, f32 tireVelZ, f32 otherSusComp, f32 dt) {
    f32 springForce, damperForce, arForce, totalForce;
    f32 damping;

    // Anti-roll bar force
    if (susp->compression > 0.0f && otherSusComp > 0.0f) {
        arForce = (susp->compression - otherSusComp) * susp->arSpringRate;
    } else {
        arForce = 0.0f;
    }

    // Select compression or rebound damping
    if (tireVelZ > 0.0f) {
        damping = susp->comprDamping;
    } else {
        damping = susp->reboundDamping;
    }

    // Calculate forces
    if (susp->compression > 10.0f) {
        // High compression - handle bump stop
        if (tireVelZ > -1.0f) {
            // Approaching bump stop - extra resistance
            totalForce = (tireVelZ + 1.0f) * susp->mass * -0.25f / dt;
        } else {
            totalForce = arForce + susp->compression * susp->springRate +
                        damping * tireVelZ;
        }
    } else if (susp->compression > 0.0f) {
        // Normal compression
        totalForce = arForce + susp->compression * susp->springRate +
                    damping * tireVelZ;
    } else {
        // Fully extended
        totalForce = 0.0f;
    }

    susp->normalForce = -totalForce;
}
```

### 4.3 wheel_rotation_update() Implementation

```c
void wheel_rotation_update(WheelState *wheel, f32 dt) {
    const f32 DAMPSPEED = 400.0f;
    f32 roadSpeed, wheelSpeed;

    // Update visual rotation
    wheel->rotAngle += wheel->angVel * dt;
    while (wheel->rotAngle > 6.283185f) wheel->rotAngle -= 6.283185f;
    while (wheel->rotAngle < 0.0f) wheel->rotAngle += 6.283185f;

    // Calculate slip ratio
    roadSpeed = wheel->contactVel[0];
    wheelSpeed = wheel->angVel * wheel->radius;

    if (fabsf(roadSpeed) > 0.1f || fabsf(wheelSpeed) > 0.1f) {
        f32 maxSpeed = fmaxf(fabsf(roadSpeed), fabsf(wheelSpeed));
        wheel->slipRatio = (wheelSpeed - roadSpeed) / maxSpeed;
    } else {
        wheel->slipRatio = 0.0f;
    }

    // Calculate slip angle (calcalpha equivalent)
    f32 Vx = wheel->contactVel[0];
    f32 Vy = wheel->contactVel[1];

    if (Vx > 0.0f) {
        if (Vx < DAMPSPEED) {
            wheel->slipAngle = Vy / Vx;
        } else {
            wheel->slipAngle = Vy / DAMPSPEED;
        }
    } else if (Vx < 0.0f) {
        if (Vx > -DAMPSPEED) {
            wheel->slipAngle = -Vy / Vx;
        } else {
            wheel->slipAngle = Vy / DAMPSPEED;
        }
    } else {
        wheel->slipAngle = (Vy > 0.0f) ? Vy :
                          (Vy < 0.0f) ? Vy : 0.0f;
    }

    // Clamp for stability
    wheel->slipAngle = CLAMP(wheel->slipAngle, -0.5f, 0.5f);
}
```

---

## 5. Data Structure Mapping

### 5.1 Arcade tiredes Structure

```c
typedef struct tiredes {
    F32 tradius;        // 0x00: Tire radius (ft)
    F32 springK;        // 0x04: Lateral spring constant (lb/ft)
    F32 rubdamp;        // 0x08: Lateral damping
    F32 PaveCstiff;     // 0x0C: Pavement cornering stiffness
    F32 PaveCfmax;      // 0x10: Pavement max friction
    F32 Cstiff;         // 0x14: Active cornering stiffness
    F32 Cfmax;          // 0x18: Active max friction
    F32 invmi;          // 0x1C: 1/moment of inertia
    F32 Zforce;         // 0x20: Normal force loading
    F32 Afmax;          // 0x24: Alpha at max force
    F32 k1, k2, k3;     // 0x28-0x30: Force polynomial coefficients
    F32 l2, l3;         // 0x34-0x38: Derivative coefficients
    F32 m1, m2, m3, m4; // 0x3C-0x48: Aligning torque coefficients
    F32 patchy;         // 0x4C: Contact patch lateral displacement
    F32 angvel;         // 0x50: Wheel angular velocity (rad/s)
    F32 sliptorque;     // 0x54: Torque from slipping
    F32 sideforce;      // 0x58: Lateral force output
    F32 traction;       // 0x5C: Longitudinal force output
    char slipflag;      // 0x60: Slip state flag
} tiredes;              // Total: 0x64 bytes (100 bytes)
```

### 5.2 N64 Tire Structure (Inferred from game.c)

```c
typedef struct N64_TireState {
    f32 rotAngle;       // 0x00: Visual rotation angle
    f32 angVel;         // 0x04: Angular velocity (rad/s)
    f32 radius;         // 0x08: Tire radius
    f32 slipAngle;      // 0x0C: Current slip angle
    f32 slipRatio;      // 0x10: Current slip ratio
    f32 contactVel[3];  // 0x14: Contact patch velocity
    f32 normalForce;    // 0x20: Normal (Z) force
    f32 latForce;       // 0x24: Lateral force
    f32 lonForce;       // 0x28: Longitudinal force
    f32 cStiff;         // 0x2C: Cornering stiffness
    f32 cfMax;          // 0x30: Max friction coefficient
    f32 driveTorque;    // 0x34: Input drive torque
} N64_TireState;        // Total: 0x38 bytes (56 bytes)
```

---

## 6. N64-Specific Simplifications

The N64 version appears to simplify the arcade tire model in several ways:

1. **No terrain-adaptive grip**: The arcade code modifies Cstiff/Cfmax based on terrain. N64 may use fixed values.

2. **Simplified coordinate transforms**: The arcade uses full 3x3 rotation matrices for tire unit vectors. N64 may use simpler approximations.

3. **No aligning torque**: The m1-m4 coefficients for aligning torque calculation are not found in N64 code.

4. **Reduced contact patch modeling**: The arcade tracks patchy (contact patch displacement) for smooth force transitions. N64 may not.

5. **Fixed-point math considerations**: N64 may use fixed-point for some calculations where arcade uses floating-point.

---

## 7. Related Files

### Arcade Source Files
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/tires.c` - Main tire physics
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/initiali.c` - Tire constants initialization
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/tiredes.h` - Tire structure definition
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/drivsym.c` - Integration with vehicle model
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/cars.c` - Car-specific tire parameters

### N64 Source Files
- `/home/cburnes/projects/rush2049-decomp/src/game/game.c` - Contains tire_force_calc, wheel_rotation_update, suspension_update
- `/home/cburnes/projects/rush2049-decomp/src/game/tire.c` - Tire-specific functions (if exists)
- `/home/cburnes/projects/rush2049-decomp/src/game/physics.c` - Physics subsystem

---

## 8. TODO: Functions Needing Implementation

| Function | Priority | Notes |
|----------|----------|-------|
| `tire_force_calc()` | High | Core friction circle - needs matching |
| `suspension_update()` | High | Spring/damper forces |
| `wheel_rotation_update()` | Medium | Slip calculations |
| `tire_constants_init()` | Low | May be ROM data |
| `tire_squeal()` | Low | Audio effect |
| `tire_skid_mark()` | Low | Visual effect |
