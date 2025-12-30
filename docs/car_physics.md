# Rush 2049 Car Physics System

This document provides comprehensive documentation of the car physics system in the Rush 2049 N64 decompilation project.

## Table of Contents

1. [Overview](#overview)
2. [Data Structures](#data-structures)
3. [Physics Update Loop](#physics-update-loop)
4. [Tire Force Model](#tire-force-model)
5. [Drivetrain Simulation](#drivetrain-simulation)
6. [Collision Detection](#collision-detection)
7. [Key Constants and Tuning](#key-constants-and-tuning)
8. [Arcade Comparison](#arcade-comparison)

---

## Overview

The Rush 2049 physics system is based on the **Milliken Engineering vehicle dynamics model**, originally developed by Max Behensky at Atari Games in 1986. This model was refined through the Hard Drivin' and Race Drivin' series before being adapted for the Rush games.

### Key Features

- **4-wheel independent suspension** with compression and rebound damping
- **Friction circle tire model** with slip angle and slip ratio calculations
- **Full drivetrain simulation** including engine, clutch, transmission, and differential
- **Anti-spin stabilization** torque to prevent spinouts
- **Aerodynamic drag and rolling resistance**
- **Spring/damper collision response**

### Coordinate System

The physics uses a **body-fixed coordinate system**:

| Axis | Direction | Component Index |
|------|-----------|-----------------|
| X | Forward | `XCOMP = 0` |
| Y | Left | `YCOMP = 1` |
| Z | Up | `ZCOMP = 2` |

### Source Files

| File | Purpose | Lines |
|------|---------|-------|
| `src/game/car.c` | Car definitions, torque curves | 709 |
| `src/game/physics.c` | Main physics simulation | 934 |
| `src/game/tire.c` | Tire force calculations | 511 |
| `src/game/drivetrain.c` | Engine/transmission | 596 |
| `src/game/collision.c` | Collision detection | 560 |

---

## Data Structures

### Car Definition (`Car` struct)

The static car definition contains all physical parameters for a vehicle type.

```c
typedef struct Car {
    const char  *name;          /* Car name string */

    /* Mass and inertia */
    f32     mass;               /* Mass in slugs (weight/32.2) */
    f32     I[3];               /* Moment of inertia (roll, pitch, yaw) slug-ft^2 */

    /* Suspension */
    f32     springrate[4];      /* Spring constant per tire (lbs/ft) */
    f32     farspringrate;      /* Front anti-roll bar rate */
    f32     rarspringrate;      /* Rear anti-roll bar rate */
    f32     cdamping[4];        /* Compression damping (lbs-sec/ft) */
    f32     rdamping[4];        /* Rebound damping (lbs-sec/ft) */

    /* Steering */
    f32     steerratio;         /* Steering ratio (wheel deg / tire deg) */
    f32     swtpg;              /* Steering wheel torque per G */

    /* Aerodynamics */
    f32     srefpcybo2;         /* Drag factor (includes Cd, frontal area, air density) */
    f32     rollresist;         /* Rolling resistance (lbs) */

    /* Tires */
    TireDes tires[4];           /* Tire parameters per wheel */

    /* Geometry */
    f32     TIRER[4][3];        /* Vector from CG to each tire contact patch */

    /* Drivetrain */
    f32     brakebal;           /* Front brake balance (0-1) */
    f32     engmi;              /* Flywheel moment of inertia (slug-ft^2) */
    f32     dwratio;            /* Differential gear ratio */
    f32     clutchmaxt;         /* Max clutch torque (lbs-ft) */
    f32     transarray[6];      /* Gear ratios: [reverse, neutral, 1st, 2nd, 3rd, 4th] */
    const s16 *torquecp;        /* Engine torque curve pointer */
    f32     upshiftangvel;      /* Auto upshift threshold (rad/sec) */
    f32     downshiftangvel;    /* Auto downshift threshold (rad/sec) */
} Car;
```

### Tire Parameters (`TireDes` struct)

Each tire has parameters controlling grip and force generation:

```c
typedef struct TireDes {
    f32     tradius;            /* Tire radius (feet) */
    f32     springK;            /* Lateral spring constant (lbs/ft) - low speed stability */
    f32     rubdamp;            /* Lateral damping coefficient */
    f32     PaveCstiff;         /* Cornering stiffness on pavement (lbs/rad) */
    f32     PaveCfmax;          /* Max friction coefficient on pavement (g) */
    f32     Cstiff;             /* Current cornering stiffness */
    f32     Cfmax;              /* Current max friction coefficient */
    f32     invmi;              /* 1/moment of inertia (wheel rotational) */

    /* Derived coefficients (calculated at runtime) */
    f32     Zforce;             /* Vertical loading */
    f32     Afmax;              /* Slip angle at max force */
    f32     k1, k2, k3;         /* Force curve polynomial coefficients */
    f32     l2, l3;             /* Additional derivative coefficients */
    f32     patchy;             /* Contact patch Y deformation */
    f32     angvel;             /* Current angular velocity */
    f32     sliptorque;         /* Torque from tire slip */
    s8      slipflag;           /* Slip state indicator */
} TireDes;
```

### Physics State (`CarPhysics` struct)

Runtime physics state for each car:

```c
typedef struct CarPhysics {
    /* Position and orientation */
    f32     RWR[3];         /* Real world position (center of mass) */
    UVect   UV;             /* Orientation matrix (3x3) */

    /* Velocities (body frame) */
    f32     V[3];           /* Linear velocity (ft/sec) */
    f32     W[3];           /* Angular velocity (rad/sec) */
    f32     magvel;         /* Speed magnitude */

    /* Accelerations */
    f32     A[3];           /* Linear acceleration */
    f32     AA[3];          /* Angular acceleration */

    /* Forces and moments (body frame) */
    f32     F[3];           /* Total force */
    f32     M[3];           /* Total moment (torque) */
    f32     G[3];           /* Gravity in body frame */
    f32     D[3];           /* Drag force */

    /* Per-tire forces */
    f32     TIREFORCE[4][3];    /* Force from each tire */
    f32     TIREV[4][3];        /* Tire velocity */
    f32     suscomp[4];         /* Suspension compression */
    u8      roadcode[4];        /* Surface type per tire */

    /* Collision */
    f32     BODYFORCE[4][3];    /* Force at body corners */
    f32     CENTERFORCE[3];     /* Center collision force */
    f32     CENTERMOMENT[3];    /* Anti-spin moment */

    /* Vehicle parameters */
    f32     mass;               /* Vehicle mass (slugs) */
    f32     massinv;            /* 1/mass */
    f32     rollresist;         /* Rolling resistance */
    f32     srefpcybo2;         /* Aero drag coefficient */

    /* Drivetrain */
    f32     torque[4];          /* Torque to each wheel */
    s32     rpm;                /* Engine RPM */
    f32     throttle;           /* Throttle position (0-1) */
    f32     brake;              /* Brake position (0-1) */
    f32     steerangle;         /* Steering angle (radians) */

    /* State */
    f32     thetime;            /* Simulation time */
    f32     dt;                 /* Timestep (1/60 sec) */
    s32     crashflag;          /* Car has crashed */
    s32     autotrans;          /* Using automatic transmission */
} CarPhysics;
```

### Car State Flags

```c
#define CAR_FLAG_AIRBORNE       0x0001  /* All tires off ground */
#define CAR_FLAG_CRASHED        0x0002  /* Crashed/disabled */
#define CAR_FLAG_REVERSED       0x0004  /* Going backwards */
#define CAR_FLAG_BRAKING        0x0008  /* Brakes applied */
#define CAR_FLAG_WHEELSPIN      0x0010  /* Tires spinning */
#define CAR_FLAG_OFFROAD        0x0020  /* Off paved surface */
#define CAR_FLAG_IN_TUNNEL      0x0040  /* In tunnel */
#define CAR_FLAG_BOOSTING       0x0080  /* Speed boost active */
```

---

## Physics Update Loop

### Main Update Flow

The physics update runs at **60 Hz** with a fixed timestep of `dt = 1/60` seconds.

```
physics_sym(m)
    |
    +-- physics_controls(m)       // Apply control inputs
    |
    +-- [Zero controls if crashed]
    |
    +-- physics_checkok(m)        // Validate physics values
    |
    +-- physics_drivetrain(m)     // Engine/transmission update
    |
    +-- physics_regular(m)        // Standard physics cycle
    |       |
    |       +-- physics_road(m)           // Determine surfaces
    |       +-- physics_forces(m)         // Calculate all forces
    |       |       +-- physics_forces1(m)  // Tire & drag forces
    |       |       +-- physics_forces2(m)  // Sum all forces
    |       +-- physics_antispin(m)       // Anti-spin stabilization
    |       +-- physics_torques(m)        // Calculate moments
    |       +-- physics_accelerations(m)  // F=ma, M=Ia
    |       +-- physics_velocities(m)     // Integrate accelerations
    |       +-- physics_positions(m)      // Integrate velocities
    |
    +-- [Calculate steering feedback]
    |
    +-- physics_mcommunication(m) // Update display/network
```

### Integration Method

The physics uses **semi-implicit Euler integration**:

1. **Acceleration**: `A = F / mass`
2. **Velocity**: `V = V + A * dt`
3. **Position**: `R = R + V * dt`

For angular motion:
1. **Angular Acceleration**: `AA = M / I` (simplified diagonal inertia)
2. **Angular Velocity**: `W = W + AA * dt`
3. **Orientation**: Updated via rotation matrix

### Force Summation

Total force on vehicle:

```
F_total = F_tires + F_gravity + F_drag + F_collision + F_antispin
```

Where:
- **F_tires**: Sum of forces from all four tires
- **F_gravity**: Gravity transformed to body frame
- **F_drag**: Aerodynamic drag + rolling resistance
- **F_collision**: Forces from body corner collisions
- **F_antispin**: Stabilization force

---

## Tire Force Model

### Friction Circle Concept

The tire model uses a **friction circle** to limit the combined lateral and longitudinal forces a tire can produce. The maximum force is determined by:

```
F_max = Cfmax * Normal_Force
```

Where `Cfmax` is the maximum friction coefficient (typically 1.0-1.3 for racing tires).

### Slip Angle Calculation

The slip angle (alpha) represents the difference between the tire's heading and its velocity direction:

```c
f32 tire_calc_alpha(f32 tirev[3]) {
    f32 vx = tirev[XCOMP];  /* Forward velocity */
    f32 vy = tirev[YCOMP];  /* Lateral velocity */

    if (vx < 1.0f && vx > -1.0f) {
        /* Low speed - direct ratio */
        return -vy / 1.0f;
    }
    /* Normal case - atan approximation */
    return -vy / vx;
}
```

### Lateral Force Polynomial

Lateral force follows a polynomial relationship with slip angle:

```
Fy = (k1*alpha - k2*alpha^2 + k3*alpha^3) * Normal_Force
```

The coefficients are derived from cornering stiffness (`Cstiff`) and max friction (`Cfmax`):

```c
void tire_constants(TireDes *tire) {
    f32 Cfmax = tire->Cfmax;
    f32 k1 = tire->Cstiff;

    /* Slip angle at max force */
    tire->Afmax = (3.0f * Cfmax) / k1;

    /* Polynomial coefficients */
    tire->k1 = k1;
    tire->k2 = (k1 * k1) / (3.0f * Cfmax);
    tire->k3 = (k1 * k1 * k1) / (27.0f * Cfmax * Cfmax);
}
```

### Friction Circle Implementation

```c
void tire_friction_circle(
    ModelDat *m,
    f32 tirev[3],
    f32 normalforce,
    f32 torque,
    TireDes *tire,
    f32 *sideforce,
    f32 *traction
) {
    f32 maxtraction = tire->Cfmax * normalforce;
    f32 maxtorque = maxtraction * tire->tradius;
    f32 roadangvel = tirev[XCOMP] / tire->tradius;

    /* Check for wheel spin - accelerating */
    if (tire->angvel > roadangvel) {
        tire->sliptorque = maxtorque;
        tire->angvel += (torque - tire->sliptorque) * tire->invmi * m->dt;

        if (tire->angvel <= roadangvel) {
            tire->angvel = roadangvel;
        } else {
            /* Spinning - use friction circle */
            f32 patchvel = tirev[XCOMP] - tire->angvel * tire->tradius;
            f32 patchspeed = sqrtf(patchvel*patchvel + tirev[YCOMP]*tirev[YCOMP]);

            *traction = -maxtraction * patchvel / patchspeed;
            *sideforce = -maxtraction * tirev[YCOMP] / patchspeed;
            return;
        }
    }

    /* Calculate lateral force based on slip angle */
    f32 alpha = tire_calc_alpha(tirev);
    /* ... polynomial calculation ... */
}
```

### Surface Friction Modifiers

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
```

### Slip State Flags

Used for tire squeal sounds and visual effects:

| Flag | Value | Meaning |
|------|-------|---------|
| `SLIP_NONE` | 0 | Normal grip |
| `SLIP_LOCKED_BRAKE` | 10 | Wheel locked under braking |
| `SLIP_ACCEL_SPIN` | 20 | Wheel spinning from acceleration |
| `SLIP_DECEL_SPIN` | 30 | Wheel spinning during deceleration |
| `SLIP_BRAKE_SPIN` | 40 | Wheel spinning under brakes |
| `SLIP_NO_LOAD` | 3 | Tire in the air |
| `SLIP_MAX_ACCEL` | 4 | At max acceleration force |
| `SLIP_MAX_BRAKE` | 5 | At max braking force |

---

## Drivetrain Simulation

### Component Overview

```
Throttle --> Engine --> Clutch --> Transmission --> Differential --> Wheels
                            |
                          (slip)
```

### Engine Torque Curves

Engine torque is stored in a 10x12 lookup table:
- **Rows**: Throttle position (0% to 100% in ~11% steps)
- **Columns**: Engine RPM (0 to 11000 in 1000 RPM steps)

```c
const s16 rushtorquecurve[10][12] = {
    /* 0% throttle - engine braking */
    { 100, -12, -33, -51, -61, -54, -71, -75, -75, -75, -75, -75 },
    /* ... */
    /* 100% throttle - max power */
    { 100, 155, 202, 240, 275, 300, 320, 327, 328, 312,  49, -65 }
};
```

**Horsepower Calculation:**
```
BHP = torque * RPM * 6.28 * torquescale / 33000
```

### Bilinear Torque Interpolation

```c
s16 drivetrain_engine_torque(DrivetrainState *dt, s16 rpm, s16 throttle,
                              s16 ignition, s16 start, const s16 *curve) {
    /* Index calculation */
    s16 rindex = rpm / dt->rpmperent;
    s16 tindex = throttle / 14;

    /* Interpolate in RPM direction */
    left = drivetrain_interp(curve[t0*12 + r0], curve[t0*12 + r1],
                             rpm % dt->rpmperent, dt->rpmperent);

    /* Interpolate in throttle direction */
    right = drivetrain_interp(curve[t1*12 + r0], curve[t1*12 + r1],
                              rpm % dt->rpmperent, dt->rpmperent);

    /* Final interpolation */
    return drivetrain_interp(left, right, throttle % 14, 14);
}
```

### Clutch Simulation

The clutch determines torque transfer between engine and transmission:

```c
void drivetrain_clutch(DrivetrainState *dt) {
    f32 totratsq = dt->totalratio * dt->totalratio;

    /* Neutral - no torque transfer */
    if (dt->gear == GEAR_NEUTRAL) {
        dt->clutchtorque = 0.0f;
        dt->dwtorque = 0.0f;
        dt->engangvel += dt->engtorque * dt->enginvmi * dt->dt;
        return;
    }

    /* Clutch output angular velocity */
    dt->clutchangvel = dt->dwangvel * dt->totalratio;

    /* Calculate max clutch torque (centrifugal for auto) */
    if (dt->autotrans) {
        if (dt->engangvel < CLUTCH_MIN_RPM * RPM_TO_RDPS) {
            curclmaxt = 0.0f;  /* Not engaged */
        } else if (dt->engangvel > CLUTCH_MAX_RPM * RPM_TO_RDPS) {
            curclmaxt = dt->clutchmaxt;  /* Fully engaged */
        } else {
            curclmaxt = /* Linear ramp */;
        }
    }

    /* Slip logic */
    if (dt->engangvel > dt->clutchangvel) {
        /* Engine faster - positive slip */
        dt->clutchtorque = curclmaxt;
        /* Update engine speed accounting for slip */
    } else {
        /* Driveshaft faster - negative slip */
        dt->clutchtorque = -curclmaxt;
    }

    dt->dwtorque = dt->clutchtorque * dt->totalratio;
}
```

### Automatic Transmission

```c
void drivetrain_autoshift(DrivetrainState *dt) {
    /* Modify shift points for throttle position */
    f32 fact = (3.0f + dt->throttle) * 0.25f;
    f32 modupshiftangvel = dt->upshiftangvel * fact;
    f32 moddownshiftangvel = dt->downshiftangvel * fact;

    /* Check for shifts */
    if (dt->engangvel > modupshiftangvel) {
        drivetrain_upshift(dt);
    }
    if (dt->engangvel < moddownshiftangvel) {
        drivetrain_downshift(dt);
    }
}
```

### Gear Ratios

Example gear ratios (index = gear + 1):

| Index | Gear | Typical Ratio |
|-------|------|---------------|
| 0 | Reverse | -2.4 |
| 1 | Neutral | 0.0 |
| 2 | 1st | 1.9 |
| 3 | 2nd | 1.5 |
| 4 | 3rd | 1.2 |
| 5 | 4th | 1.0 |

Total ratio = `transratio * dwratio` (differential ratio ~3.0-3.5)

### Load-Sensitive Differential

The "magic differential" splits torque based on tire load:

```c
void drivetrain_update(DrivetrainState *dt) {
    f32 rearload = dt->tireforce_z[2] + dt->tireforce_z[3];

    if (!dt->magicdif || rearload > -500.0f) {
        /* Standard 50/50 split */
        dt->torque[2] = dt->dwtorque * 0.5f;
        dt->torque[3] = dt->dwtorque * 0.5f;
    } else {
        /* Load-proportional split */
        dt->torque[2] = (dt->dwtorque * dt->tireforce_z[2]) / rearload;
        dt->torque[3] = (dt->dwtorque * dt->tireforce_z[3]) / rearload;
    }
}
```

---

## Collision Detection

### Algorithm Overview

1. **Bounding Sphere Test** (fast rejection)
2. **Body Corner Test** (detailed check)
3. **Spring/Damper Response** (force application)

### Bounding Sphere Check

```c
s32 check_sphere_collision(s32 car1, s32 car2, f32 *dist_sq) {
    CollisionData *c1 = &col_data[car1];
    CollisionData *c2 = &col_data[car2];

    /* Squared distance between centers */
    f32 dsq = vec_dist_sq(car_array[car1].RWR, car_array[car2].RWR);

    /* Check sphere overlap */
    f32 combined_rad = c1->colrad + c2->colrad;
    return (dsq <= combined_rad * combined_rad);
}
```

### Point-in-Body Test

```c
s32 point_in_body(s32 car_index, f32 point[3]) {
    /* Transform point to body coordinates */
    f32 body_point[3];
    world_to_body(point, body_point, car->dr_uvs, car->RWR);

    /* Get half-sizes from body corners */
    f32 half_x = col->BODYR[0][0];
    f32 half_y = 2.0f;
    f32 half_z = col->BODYR[0][2];

    /* Check if inside box */
    if (body_point[0] < -half_x || body_point[0] > half_x) return 0;
    if (body_point[1] < -half_y || body_point[1] > half_y) return 0;
    if (body_point[2] < -half_z || body_point[2] > half_z) return 0;

    return 1;
}
```

### Collision Response

Spring-damper model for collision forces:

```c
void set_collision_force(s32 car1, s32 car2, f32 force[3], f32 point[3]) {
    /* Calculate relative velocity */
    f32 rel_vel[3];
    for (int i = 0; i < 3; i++) {
        rel_vel[i] = d1->RWV[i] - d2->RWV[i];
    }

    /* Velocity along collision normal */
    f32 vel_along_normal = vec_dot(rel_vel, normal);

    /* Spring + damping force: F = Ks * penetration + Kd * velocity */
    f32 force_mag = COL_SPRING_K * 0.5f + COL_DAMP_K * vel_along_normal;

    /* Clamp force magnitude */
    if (force_mag > MAXFORCE) force_mag = MAXFORCE;
    if (force_mag < -MAXFORCE) force_mag = -MAXFORCE;

    /* Apply to center */
    for (int i = 0; i < 3; i++) {
        c1->CENTERFORCE[i] += normal[i] * force_mag;
    }
}
```

### Collision Constants

```c
#define MAXFORCE        4000.0f     /* Maximum collision force (lbs) */
#define MAX_VEL_SQ      (300*300)   /* Max velocity squared (fps^2) */
#define COL_SPRING_K    (-10000.0f) /* Spring constant */
#define COL_DAMP_K      (-50.0f)    /* Damping constant */
```

---

## Key Constants and Tuning

### Physics Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `GRAVITY_ACCEL` | 32.2 ft/s^2 | Acceleration of gravity |
| `DT_PHYSICS` | 1/60 sec | Physics timestep |
| `MAX_VELOCITY` | 1000 fps | Speed of sound limit |
| `RPM_TO_RDPS` | 0.1047 | RPM to rad/sec conversion |

### Car Parameters Summary

**Example: Rocket (Car 0)**

| Parameter | Value | Notes |
|-----------|-------|-------|
| Mass | 99.4 slugs | 3200 lbs |
| Wheelbase | 9.4 ft | Front to rear |
| Track Width | 5.0 ft | Left to right |
| CG Height | ~1.5 ft | Above ground |
| Spring Rate (F) | 4200 lbs/ft | Stiff for speed |
| Spring Rate (R) | 2800 lbs/ft | Softer rear |
| Steering Ratio | 14:1 | Sports car |
| Brake Balance | 68% front | Normal |
| Diff Ratio | 3.5:1 | |
| Top Gear | 4th | |
| Upshift RPM | 5750 | |
| Downshift RPM | 3500 | |

### Tire Parameters

**Front Tires:**
| Parameter | Value | Description |
|-----------|-------|-------------|
| `tradius` | 1.0 ft | Tire radius |
| `PaveCstiff` | 16000 lbs/rad | Cornering stiffness |
| `PaveCfmax` | 1.15 g | Max friction |
| `springK` | 4080 lbs/ft | Lateral stiffness |
| `invmi` | 0.1 | Wheel inertia factor |

**Rear Tires:**
| Parameter | Value | Description |
|-----------|-------|-------------|
| `tradius` | 1.0 ft | Tire radius |
| `PaveCstiff` | 50000 lbs/rad | Higher stiffness (driven) |
| `PaveCfmax` | 1.3 g | Higher friction |
| `springK` | 4080 lbs/ft | Lateral stiffness |
| `invmi` | 0.1 | Wheel inertia factor |

### Anti-Spin Constants

Per car type and difficulty:

| Car Type | Easy | Medium | Hard |
|----------|------|--------|------|
| Beginner | 600 | 300 | 150 |
| Intermediate | 400 | 200 | 100 |
| Expert | 200 | 100 | 50 |
| Pro | 10 | 5 | 0 |

---

## Arcade Comparison

### Source Files Correspondence

| N64 File | Arcade File | Match Quality |
|----------|-------------|---------------|
| `src/game/physics.c` | `game/drivsym.c` | High |
| `src/game/tire.c` | `game/tires.c` | High |
| `src/game/car.c` | `game/cars.c` | High |
| `src/game/drivetrain.c` | `game/drivetra.c` | High |
| `src/game/collision.c` | `game/collision.c` | High |

### Key Differences

1. **Fixed-Point Math**: Arcade uses 16-bit fixed-point for some calculations; N64 uses floating-point throughout.

2. **Force Feedback**: Arcade has steering wheel force feedback hardware; N64 uses controller rumble.

3. **Tire Model Simplifications**: Some transient tire response features are simplified for N64 performance.

4. **Collision**: N64 collision may have reduced resolution or simplified geometry.

### Arcade Function Mapping

| Arcade Function | N64 Equivalent | Notes |
|-----------------|----------------|-------|
| `sym()` | `physics_sym()` | Main step |
| `regular()` | `physics_regular()` | Standard update |
| `forces1()` | `physics_forces1()` | Tire/drag |
| `forces2()` | `physics_forces2()` | Force sum |
| `antispin()` | `physics_antispin()` | Anti-spin |
| `accelerations()` | `physics_accelerations()` | Integration |
| `velocities()` | `physics_velocities()` | Integration |
| `positions()` | `physics_positions()` | Integration |
| `frictioncircle()` | `tire_friction_circle()` | Tire forces |
| `calcalpha()` | `tire_calc_alpha()` | Slip angle |
| `tire_constants()` | `tire_constants()` | Same name |
| `calctireuv()` | `tire_calc_uv()` | Tire orientation |
| `dotireforce()` | `tire_calc_force()` | Force calc |

### Torque Curve Comparison

Both versions use identical torque curve format and values:

```c
/* Arcade: game/cars.c */
const short rushtorquecurve[10][12] = { ... };

/* N64: src/game/car.c */
const TorqueCurve gRushTorqueCurve = { ... };
```

### References

The physics model is documented in these research papers (cited in arcade source):

1. "Lateral Stiffness, Cornering Stiffness, and Relaxation Length of the Pneumatic Tire" - Loeb, Ellis, Guenther (OSU)

2. "The Effect of Steering Amplitude on Relaxation Length" - Ellis (OSU)

The arcade source credits **Max Behensky** (1986, Atari Games) for the tire model implementation, and **Doug Milliken** (Milliken Research Associates) for vehicle dynamics consultation.

---

## Future Work

### Remaining Decompilation Tasks

1. Match exact N64 assembly for core physics functions
2. Identify any N64-specific optimizations or simplifications
3. Document crash detection thresholds
4. Reverse engineer boost/stunt mode physics modifications

### Potential Improvements

1. Add quaternion-based orientation for proper rotation
2. Implement full inertia tensor instead of diagonal
3. Add tire temperature modeling
4. Improve collision geometry resolution

---

*Document generated for Rush 2049 N64 Decompilation Project*
*Based on analysis of N64 decompiled source and San Francisco Rush: The Rock arcade source code*
