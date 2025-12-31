# Drivetrain and Transmission System

Technical documentation for the Rush 2049 N64 drivetrain simulation, based on the arcade Rush The Rock source code.

## Overview

The drivetrain system simulates:
- **Engine**: Torque curve lookup with RPM/throttle interpolation
- **Clutch**: Centrifugal (auto) or friction-based (manual) engagement
- **Transmission**: 4-speed automatic or manual with gear ratios
- **Differential**: Standard or load-sensitive "magic" torque split

The N64 implementation closely follows the arcade `drivetra.c` but adapts for N64 constraints (fixed-point where possible, simplified physics when needed).

---

## 1. Engine Torque Curve

### Arcade Source Reference
- **File**: `reference/repos/rushtherock/game/cars.c` (lines 130-193)
- **Function**: `enginetorque()` in `drivetra.c` (lines 249-328)

### Torque Curve Format

The engine torque is defined as a 10x12 lookup table:
- **Rows (10)**: Throttle position (0% to 100%, ~11% increments)
- **Columns (12)**: RPM (0 to 11,000 RPM, 1000 RPM increments)

Values are in ft-lbs of torque, which are then scaled by `torquescale`.

### Standard Torque Curve (`stdtorquecurve`)

```c
const short stdtorquecurve[10][12] = {
    {  75, -10, -30, -50, -70, -80, -80, -80, -80, -80, -80, -80 },  /* 0% throttle */
    {  75, 100,   1, -12, -29, -41, -49, -60, -71, -76, -78, -80 },
    {  75, 150,  32,  26,  12,  -2, -18, -40, -62, -71, -76, -80 },
    {  75, 150,  63,  63,  53,  37,  13, -20, -53, -67, -73, -80 },
    {  75, 150,  94, 101,  94,  76,  44,   0, -44, -62, -71, -80 },
    {  75, 175, 126, 139, 136, 114,  76,  20, -36, -58, -69, -80 },
    {  75, 175, 157, 177, 177, 153, 107,  40, -27, -53, -67, -80 },
    {  75, 175, 188, 214, 218, 192, 138,  60, -18, -49, -64, -80 },
    {  75, 177, 219, 252, 259, 231, 169,  80,  -9, -44, -62, -80 },
    {  75, 200, 250, 290, 300, 270, 200, 100,   0, -40, -60, -80 }   /* 100% throttle */
};
```

### Rush Torque Curve (`rushtorquecurve`)

The Rush games use a more aggressive torque curve:

```c
const short rushtorquecurve[10][12] = {
    { 100, -12, -33, -51, -61, -54, -71, -75, -75, -75, -75, -75 },
    { 100, 112,  62,   3, -30, -51, -65, -69, -71, -73, -75, -75 },
    { 100, 114, 113, 112, 104,  70,  87,  32, -32, -62, -70, -75 },
    { 100, 116, 126, 131, 136, 136, 131,  90,  -1, -36, -69, -75 },
    { 100, 119, 137, 151, 161, 170, 175, 160,  90,   0, -50, -75 },
    { 100, 127, 156, 178, 196, 215, 223, 225, 205,  75, -32, -75 },
    { 100, 133, 163, 190, 212, 230, 245, 256, 243, 108, -23, -75 },
    { 100, 137, 175, 206, 230, 250, 265, 280, 282, 175, -10, -75 },
    { 100, 146, 186, 222, 248, 275, 293, 305, 310, 251,   4, -69 },
    { 100, 155, 202, 240, 275, 300, 320, 327, 328, 312,  49, -65 }
};
```

### Dirt Torque Curve

Reduced torque for off-road conditions:

```c
const short stddirttorquecurve[10][12] = {
    {  75, -10, -30, -30, -40, -40, -40, -40, -40, -40, -40, -40 },
    {  75, 100,   1, -12, -29, -41, -40, -40, -40, -40, -40, -40 },
    /* ... reduced values above 4000 RPM ... */
};
```

### Torque Scaling

Different gear positions use different torque multipliers:

| Gear | Scale Variable | Typical Value |
|------|----------------|---------------|
| 1st | `fgtorquescale` | 1.1 - 1.2 |
| 2nd | `sgtorquescale` | 1.0 - 1.05 |
| 3rd+ | `torquescale` | 1.0 |
| Off-road | `dirttorquescale` | 0.9 - 1.0 |

### N64 Symbol Mapping

| Arcade Variable | N64 Symbol | N64 Address |
|-----------------|------------|-------------|
| `stdtorquecurve` | `gTorqueCurveStandard` | 0x80160800 |
| `stddirttorquecurve` | `gTorqueCurveDirt` | 0x80160900 |
| `rushtorquecurve` | `gTorqueCurveRush` | 0x80160A00 |
| `engtorque` | `gEngineTorque` | 0x80152A90 |
| `engangvel` | `gEngineAngularVelocity` | 0x80152A94 |
| `rpm` | `gEngineRPM` | 0x80152A8C |

---

## 2. Transmission Gear Ratios

### Arcade Source Reference
- **File**: `reference/repos/rushtherock/game/cars.c` (lines 532-545 for ratios)
- **Function**: `transmission()` in `drivetra.c` (lines 190-194)

### Gear Ratio Array

Gear ratios are stored as `transarray[gear + 1]` (offset by 1 for reverse at index 0):

```c
/* Index: 0=R, 1=N, 2=1st, 3=2nd, 4=3rd, 5=4th */
const f32 transarray[6] = {
    -2.4,   /* Reverse */
     0.0,   /* Neutral */
     1.8,   /* 1st gear - high ratio */
     1.45,  /* 2nd gear */
     1.2,   /* 3rd gear */
     1.0    /* 4th gear - direct drive */
};
```

### Total Gear Ratio

Total ratio includes the differential (final drive):

```c
totalratio = transratio * dwratio;
```

Typical differential ratios: 3.0 - 3.7

### N64 Car Definitions (from `src/game/car.c`)

| Car | 1st | 2nd | 3rd | 4th | Diff Ratio |
|-----|-----|-----|-----|-----|------------|
| Serpent | 1.9 | 1.5 | 1.2 | 1.0 | 3.5 |
| Venom | 1.8 | 1.45 | 1.2 | 1.0 | 3.3 |
| Crusher | 2.0 | 1.6 | 1.3 | 1.0 | 3.0 |
| Phantom | 2.0 | 1.55 | 1.25 | 1.0 | 3.6 |
| Wraith | 1.85 | 1.48 | 1.22 | 1.0 | 3.4 |
| Eliminator | 1.75 | 1.4 | 1.15 | 0.95 | 3.2 |
| Banshee | 1.95 | 1.52 | 1.22 | 1.0 | 3.7 |

### N64 Symbol Mapping

| Arcade Variable | N64 Symbol | N64 Address |
|-----------------|------------|-------------|
| `transratio` | `gTransmissionRatio` | 0x80152AB0 |
| `totalratio` | `gTotalGearRatio` | 0x80152AB4 |
| `dwratio` | `gDifferentialRatio` | 0x80152AAC |
| `gear` | `gGear` | 0x80152AA4 |

---

## 3. Differential Model

### Arcade Source Reference
- **File**: `reference/repos/rushtherock/game/drivetra.c` (lines 39-57)
- **Field**: `magicdif` in `drivsym.h` (line 183)

### Standard Differential

The standard differential splits torque 50/50 to the rear wheels:

```c
m->torque[2] += m->dwtorque * 0.5;  /* Right rear */
m->torque[3] += m->dwtorque * 0.5;  /* Left rear */
```

### Magic Differential

The "magic differential" is a load-sensitive limited-slip differential that provides better traction without inducing yaw moments:

```c
if (m->magicdif && (rearload <= -500)) {
    /* Load-proportional torque split */
    if (m->TIREFORCE[2][ZCOMP] >= 0) {
        /* Right rear unloaded - all torque to left */
        m->torque[2] = 0;
        m->torque[3] = m->dwtorque;
    } else if (m->TIREFORCE[3][ZCOMP] >= 0) {
        /* Left rear unloaded - all torque to right */
        m->torque[2] = m->dwtorque;
        m->torque[3] = 0;
    } else {
        /* Proportional split based on load */
        m->torque[2] = (m->dwtorque * m->TIREFORCE[2][ZCOMP]) / rearload;
        m->torque[3] = (m->dwtorque * m->TIREFORCE[3][ZCOMP]) / rearload;
    }
}
```

### N64 Implementation

The N64 `src/game/drivetrain.c` implements identical logic in `drivetrain_update()` (lines 143-167).

### N64 Symbol Mapping

| Arcade Variable | N64 Symbol | N64 Address |
|-----------------|------------|-------------|
| `dwtorque` | `gDriveWheelTorque` | 0x80152AB8 |

---

## 4. Automatic Transmission Logic

### Arcade Source Reference
- **File**: `reference/repos/rushtherock/game/drivetra.c` (lines 164-221)
- **Function**: `autoshift()`, `upshift()`, `downshift()`, `find_best_gear()`

### Shift Point Calculation

Shift points are modified by throttle position for throttle-dependent shifting:

```c
void autoshift(MODELDAT *m) {
    /* Modify shift points for throttle position */
    fact = (3.0 + m->throttle) * 0.25;  /* Range: 0.75 to 1.0 */
    modupshiftangvel = m->upshiftangvel * fact;
    moddownshiftangvel = m->downshiftangvel * fact;

    if (m->engangvel > modupshiftangvel)
        upshift(m);
    if (m->engangvel < moddownshiftangvel)
        downshift(m);
}
```

### Default Shift Points (RPM)

Stored in angular velocity (rad/sec), converted from RPM:

| Car | Upshift RPM | Downshift RPM |
|-----|-------------|---------------|
| Serpent | 5750 | 3500 |
| Venom | 5750 | 3500 |
| Crusher | 5500 | 3200 |
| Phantom | 6000 | 3800 |
| Eliminator | 6200 | 4000 |

### Shift Timing

A 1-second delay prevents rapid gear changes:

```c
void upshift(MODELDAT *m) {
    if (m->gear < m->topgear && (m->thetime > m->shifttime)) {
        m->shifttime = m->thetime + 1;  /* 1 second delay */
        m->gear++;
    }
}
```

### N64 Symbol Mapping

| Arcade Variable | N64 Symbol | N64 Address |
|-----------------|------------|-------------|
| `autotrans` | `gAutoTrans` | 0x80152AA8 |
| `upshiftangvel` | `gUpshiftRPM` | 0x80152AC4 |
| `downshiftangvel` | `gDownshiftRPM` | 0x80152AC8 |

### N64 Functions

| N64 Function | Address | Size | Arcade Equivalent |
|--------------|---------|------|-------------------|
| `drivetrain_autoshift` | - | - | `autoshift()` |
| `car_gear_shift` | 0x800A7C9C | 236 bytes | manual gear change |
| `car_get_gear` | 0x800A7D88 | 104 bytes | get current gear |
| `transmission_shift` | 0x8010BC84 | - | gear change with timing |

---

## 5. Clutch Behavior

### Arcade Source Reference
- **File**: `reference/repos/rushtherock/game/drivetra.c` (lines 64-161)
- **Function**: `whatslips()`

### Automatic Transmission (Centrifugal Clutch)

For automatic transmission, a centrifugal clutch engages based on engine RPM:

```c
if (m->autotrans) {
    if (m->engangvel < (m->clutchangvel - 100 * rpmtordps)) {
        /* Engine dragging - full engagement */
        curclmaxt = m->clutchmaxt;
    } else {
        centrifvel = m->engangvel - (m->clutchangvel * 0.25);

        if (centrifvel < 1500 * rpmtordps)
            curclmaxt = 0;  /* Not engaged */
        else if (centrifvel > 3000 * rpmtordps)
            curclmaxt = m->clutchmaxt;  /* Fully engaged */
        else
            /* Linear engagement ramp */
            curclmaxt = clutchmaxt * (centrifvel - 1500*rpmtordps) / (1500*rpmtordps);
    }
}
```

### Manual Transmission (Friction Clutch)

For manual transmission, clutch engagement is based on pedal position:

```c
if (m->clutch < 0)
    curclmaxt = m->clutchmaxt;  /* Engaged */
else if (m->clutch > 0.8)
    curclmaxt = 0;  /* Disengaged */
else
    curclmaxt = (1.0/0.8) * (0.8 - m->clutch) * m->clutchmaxt;  /* Friction point at 0.8 */
```

### Clutch Slip Logic

The clutch determines torque transfer between engine and driveshaft:

```c
/* Torque through clutch limited by clutch capacity */
if (m->engangvel > m->clutchangvel) {
    m->clutchtorque = curclmaxt;
    m->engangvel += (m->engtorque - m->clutchtorque) * m->enginvmi * m->dt;

    /* Check if clutch locked */
    if (m->engangvel < m->clutchangvel && m->engtorque > -curclmaxt) {
        m->engangvel = m->clutchangvel;
        m->clutchtorque = m->engtorque;
    }
}

/* Drive wheel torque = clutch torque * total ratio */
m->dwtorque = m->clutchtorque * m->totalratio;
```

### Clutch Parameters

| Parameter | Value | Description |
|-----------|-------|-------------|
| `clutchmaxt` | 600-900 ft-lb | Maximum clutch torque capacity |
| Friction point | 0.8 | Pedal position where clutch engages |
| Min engage RPM | 1500 | Centrifugal clutch starts engaging |
| Full engage RPM | 3000 | Centrifugal clutch fully engaged |

### N64 Symbol Mapping

| Arcade Variable | N64 Symbol | N64 Address |
|-----------------|------------|-------------|
| `clutch` | `gClutch` | 0x80152AA0 |
| `clutchtorque` | `gClutchTorque` | 0x80152AC0 |

---

## 6. N64 vs Arcade Differences

### Structural Differences

| Aspect | Arcade | N64 |
|--------|--------|-----|
| **Floating Point** | Full float | Float, some fixed-point |
| **Torque Curves** | `const short [10][12]` | Same format |
| **Update Rate** | Variable (60-120 Hz) | Fixed 60 Hz |
| **Physics Thread** | Dedicated AST | Main game loop |

### Simplified Systems

1. **Torque Converter Loss**: Arcade uses `AUTOLOSS = 0.925` when not in top gear. N64 implements the same.

2. **Gauges**: Arcade updates amperes, oil pressure, engine temp. N64 uses these for HUD display.

3. **Surface Detection**: Arcade checks `m->roadcode[2]` and `m->roadcode[3]` for both rear tires. N64 simplifies to a single surface flag.

### N64-Specific Adaptations

1. **No Clutch Pedal**: N64 uses button combinations for manual shifting instead of a physical clutch pedal.

2. **Transmission Selection**: N64 uses `trans_select_handler` (0x800D1CE0) similar to arcade `ClutchSel()` for auto/manual selection.

3. **Shift Sound**: N64 adds `sound_play_menu(26)` on gear changes not present in arcade physics code.

### Function Mapping Summary

| Arcade Function | N64 Function | N64 Address |
|-----------------|--------------|-------------|
| `drivetrain()` | `drivetrain_update` | 0x800AB544 |
| `engine()` | `drivetrain_engine` | - |
| `transmission()` | `drivetrain_transmission` | - |
| `whatslips()` | `drivetrain_clutch` | - |
| `autoshift()` | `drivetrain_autoshift` | - |
| `upshift()` | `drivetrain_upshift` | - |
| `downshift()` | `drivetrain_downshift` | - |
| `enginetorque()` | `drivetrain_engine_torque` | - |
| `find_best_gear()` | `drivetrain_find_best_gear` | - |
| `interp()` | `drivetrain_interp` | - |
| (car shift handler) | `car_gear_shift` | 0x800A7C9C |
| (engine RPM calc) | `engine_rpm_calc` | 0x800AB70C |
| (engine torque calc) | `engine_torque_calc` | 0x800AB7D8 |
| (transmission shift) | `transmission_shift` | 0x800ABBD0 |
| (wheel torque apply) | `wheel_torque_apply` | 0x800AC668 |

---

## 7. Constants and Conversions

### RPM Conversion

```c
#define RPM_TO_RDPS     (2.0f * PI / 60.0f)   /* 0.1047 rad/sec per RPM */
#define RDPS_TO_RPM     (60.0f / (2.0f * PI)) /* 9.549 RPM per rad/sec */
```

### Arcade Equivalents

```c
/* From arcade drivsym.h */
#define rpmtordps   (2.0 * 3.14159265 / 60.0)
#define rdpstorpm   (60.0 / (2.0 * 3.14159265))
```

### Drivetrain Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `MINGEAR` | 1 | Minimum forward gear |
| `NEUTRALGEAR` | 0 | Neutral position |
| `REVERSEGEAR` | -1 | Reverse gear |
| `AUTOLOSS` | 0.925 | Auto trans torque loss |

---

## 8. Data Structures

### Arcade MODELDAT (Drivetrain Fields)

From `reference/repos/rushtherock/game/modeldat.h` (lines 541-576):

```c
struct MODELDAT {
    /* Engine/drivetrain at offset ~541 */
    F32     engtorque;          /* Engine output torque (ft-lb) */
    F32     enginvmi;           /* 1/flywheel moment of inertia */
    F32     engangvel;          /* Engine angular velocity (rad/sec) */
    F32     dwratio;            /* Differential gear ratio */
    F32     dwtorque;           /* Driveshaft torque */
    F32     dwangvel;           /* Driveshaft angular velocity */

    /* Clutch */
    F32     clutchmaxt;         /* Max clutch torque capacity */
    F32     clutchtorque;       /* Current clutch torque */
    F32     clutchangvel;       /* Clutch output angular velocity */

    /* Transmission */
    F32     transratio;         /* Current gear ratio */
    F32     totalratio;         /* Trans * diff ratio */
    S16     *torquecurve;       /* Pavement torque curve */
    S16     *dirttorquecurve;   /* Dirt torque curve */
    F32     *transarray;        /* Gear ratio array */
    S16     topgear;            /* Top forward gear */
    F32     upshiftangvel;      /* Upshift threshold (rad/sec) */
    F32     downshiftangvel;    /* Downshift threshold (rad/sec) */
};
```

### N64 DrivetrainState

From `include/game/drivetrain.h`:

```c
typedef struct DrivetrainState {
    /* Engine state */
    f32     engangvel;
    f32     engtorque;
    f32     enginvmi;

    /* Clutch state */
    f32     clutchangvel;
    f32     clutchtorque;
    f32     clutchmaxt;
    f32     clutch;

    /* Transmission state */
    s32     gear;
    s32     commandgear;
    s32     topgear;
    s32     autotrans;
    f32     transratio;
    f32     totalratio;

    /* Differential state */
    f32     dwangvel;
    f32     dwratio;
    f32     dwtorque;
    f32     dwinvmi;
    f32     efdwinvmi;

    /* ... additional fields ... */
} DrivetrainState;
```

---

## 9. Source File Locations

### Arcade Source Files

| File | Purpose |
|------|---------|
| `reference/repos/rushtherock/game/drivetra.c` | Drivetrain simulation |
| `reference/repos/rushtherock/game/mdrive.c` | Car controls interface |
| `reference/repos/rushtherock/game/cars.c` | Car/torque definitions |
| `reference/repos/rushtherock/game/drivsym.h` | Car structure definition |
| `reference/repos/rushtherock/game/modeldat.h` | MODELDAT structure |

### N64 Source Files

| File | Purpose |
|------|---------|
| `src/game/drivetrain.c` | Drivetrain simulation (full implementation) |
| `src/game/car.c` | Car definitions with gear ratios |
| `src/game/game.c` | Additional drivetrain functions |
| `include/game/drivetrain.h` | DrivetrainState structure |
| `include/game/car.h` | Car structure, torque curves |

---

## 10. Implementation Notes

### Horsepower Calculation

From arcade source comments:

```
Actual BHP = [torque * RPM * 6.28 * torquescale / 33000]
```

Where:
- `torque` is from the torque curve (ft-lb)
- `RPM` is engine speed
- `6.28` is 2*PI (angular velocity conversion)
- `33000` is ft-lb/min per horsepower

### Engine Stall Prevention

At very low RPM (< 500), the engine provides minimal motoring torque:

```c
if (rpm < 500) {
    if (rpm >= 0) {
        if (startermotor)
            return(*zzp);  /* Starter motor torque */
        else
            return((short)((-*zzp * rpm) / 500));  /* Resistance */
    }
}
```

### Effective Moment of Inertia

When the clutch is locked, the effective drivetrain inertia combines engine and differential:

```c
m->efdwinvmi = 1.0 / (1.0/m->dwinvmi + totratsq/m->enginvmi);
```

This affects acceleration response and engine braking behavior.
