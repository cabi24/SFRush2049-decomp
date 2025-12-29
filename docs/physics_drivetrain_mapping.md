# Rush 2049 Drivetrain System - Arcade to N64 Mapping

This document describes the drivetrain simulation system used in San Francisco Rush 2049, based on analysis of the Rush The Rock arcade source code and mapping to the N64 decompilation.

## Overview

The drivetrain system is based on the **Milliken Engineering** vehicle dynamics model, originally developed in 1985 and translated to C by Max Behensky at Atari. The system simulates:

- Engine torque generation (with RPM/throttle-dependent curves)
- Transmission with 4-6 forward gears plus reverse
- Clutch with slip simulation (centrifugal for auto, friction for manual)
- Differential with optional load-sensitive torque split
- Torque distribution to drive wheels

## Key Arcade Source Files

| File | Purpose |
|------|---------|
| `game/drivsym.c` | Main physics simulation (sym() function) |
| `game/drivetra.c` | Drivetrain subsystem (engine, trans, clutch) |
| `game/cars.c` | Car definitions with all parameters |
| `game/modeldat.h` | MODELDAT structure (full car state) |
| `game/drivsym.h` | Constants and Car structure |

## N64 Source Files

| File | Purpose |
|------|---------|
| `src/game/drivetrain.c` | Full drivetrain simulation (port of drivetra.c) |
| `include/game/drivetrain.h` | DrivetrainState structure and constants |
| `src/game/game.c` | Contains inline drivetrain functions |

---

## Transmission Gear Ratios

The arcade source defines gear ratios in the Car structure. The index is `gear + 1` to handle reverse at index 0.

### Standard Race Car (shorttrkcar) Gear Ratios

| Gear | Index | Ratio | Description |
|------|-------|-------|-------------|
| Reverse | 0 | -2.4 | Backing up |
| Neutral | 1 | 0.0 | No drive |
| 1st | 2 | 1.8 | Launch gear |
| 2nd | 3 | 1.45 | Low speed |
| 3rd | 4 | 1.2 | Mid speed |
| 4th | 5 | 1.0 | Top gear |

### Differential Ratios by Car Type

| Car Type | Differential Ratio | Notes |
|----------|-------------------|-------|
| shorttrkcar | 3.3 | Race configuration |
| crownvic | 3.0 | Full-size sedan |
| Standard cars | 3.5 | General vehicles |
| Rush 2049 cars | 3.3 | Default for game |

### Total Ratio Calculation

```c
totalratio = transratio * dwratio;
```

For example, 1st gear in shorttrkcar:
- `transratio = 1.8`
- `dwratio = 3.3`
- `totalratio = 1.8 * 3.3 = 5.94:1`

---

## Engine Torque Curves

The engine uses a 10x12 lookup table with bilinear interpolation:
- **10 rows**: Throttle positions (0% to 100%, ~11% increments)
- **12 columns**: RPM values (typically 0-11000 RPM at 1000 RPM intervals)

### Standard Torque Curve (stdtorquecurve)

```c
const short stdtorquecurve[10][12] = {
    /*  0RPM  1K   2K   3K   4K   5K   6K   7K   8K   9K   10K  11K */
    {  75, -10, -30, -50, -70, -80, -80, -80, -80, -80, -80, -80 }, /* 0% throttle */
    {  75, 100,   1, -12, -29, -41, -49, -60, -71, -76, -78, -80 }, /* 11% */
    {  75, 150,  32,  26,  12,  -2, -18, -40, -62, -71, -76, -80 }, /* 22% */
    {  75, 150,  63,  63,  53,  37,  13, -20, -53, -67, -73, -80 }, /* 33% */
    {  75, 150,  94, 101,  94,  76,  44,   0, -44, -62, -71, -80 }, /* 44% */
    {  75, 175, 126, 139, 136, 114,  76,  20, -36, -58, -69, -80 }, /* 55% */
    {  75, 175, 157, 177, 177, 153, 107,  40, -27, -53, -67, -80 }, /* 66% */
    {  75, 175, 188, 214, 218, 192, 138,  60, -18, -49, -64, -80 }, /* 77% */
    {  75, 177, 219, 252, 259, 231, 169,  80,  -9, -44, -62, -80 }, /* 88% */
    {  75, 200, 250, 290, 300, 270, 200, 100,   0, -40, -60, -80 }  /* 100% */
};
```

### Rush Torque Curve (rushtorquecurve)

Higher power output, flatter curve at high RPM:

```c
const short rushtorquecurve[10][12] = {
    /* 0% */  { 100, -12, -33, -51, -61, -54, -71, -75, -75, -75, -75, -75 },
    /* 100% */{ 100, 155, 202, 240, 275, 300, 320, 327, 328, 312,  49, -65 }
    /* ... intermediate rows ... */
};
```

### Torque Curve Characteristics

| Curve | Peak Torque | Peak RPM | Powerband |
|-------|-------------|----------|-----------|
| stdtorquecurve | 300 ft-lb | 4000 RPM | 2000-6000 |
| rushtorquecurve | 328 ft-lb | 8000 RPM | 3000-9000 |
| racetorquecurve | 400 ft-lb | 8000 RPM | 3000-9000 |
| subtorquecurve | 170 ft-lb | 4000 RPM | 2000-5000 |

### Actual Horsepower Calculation

From arcade source comment:
```c
BHP = [torque * RPM * 6.28 * torquescale] / 33000
```

---

## RPM and Shift Points

### Conversion Constants

```c
#define rpmtordps  (2.0 * 3.14159 / 60.0)   /* RPM to radians/sec */
#define rdpstorpm  (60.0 / (2.0 * 3.14159)) /* radians/sec to RPM */
```

### Automatic Transmission Shift Points

| Parameter | Value | Description |
|-----------|-------|-------------|
| Upshift RPM | 5750 | Shift to higher gear |
| Downshift RPM | 3500 | Shift to lower gear |
| UPSHIFT_RPM (define) | 5500 | Default upshift |
| DNSHIFT_RPM (define) | 3000 | Default downshift |

### Throttle-Modified Shift Points

```c
/* Modify shift points for throttle position */
fact = (3.0 + throttle) * 0.25;
modupshiftangvel = upshiftangvel * fact;
moddownshiftangvel = downshiftangvel * fact;
```

At 0% throttle: shift factor = 0.75 (shift earlier)
At 100% throttle: shift factor = 1.0 (shift at normal RPM)

### Minimum Gear

```c
#define MINGEAR 1   /* Minimum gear for auto trans (can't downshift below 1st) */
#define MAXGEAR 4   /* Number of forward speeds */
```

---

## Clutch System

### Automatic Transmission (Centrifugal Clutch)

```c
/* Centrifugal clutch engagement based on engine RPM */
#define CLUTCH_MIN_RPM  1500.0f  /* Start engaging */
#define CLUTCH_MAX_RPM  3000.0f  /* Fully engaged */

if (centrifvel < 1500 * rpmtordps) {
    curclmaxt = 0;  /* Clutch disengaged */
} else if (centrifvel > 3000 * rpmtordps) {
    curclmaxt = clutchmaxt;  /* Fully engaged */
} else {
    /* Linear ramp between min and max */
    curclmaxt = clutchmaxt * (centrifvel - 1500) / 1500;
}
```

### Manual Transmission

```c
#define CLUTCH_FRICTION_POINT  0.8f  /* Friction engagement at 80% pedal travel */

if (clutch > 0.8) {
    curclmaxt = 0;  /* Clutch fully out */
} else {
    curclmaxt = (0.8 - clutch) / 0.8 * clutchmaxt;
}
```

### Torque Converter Loss

```c
#define AUTOLOSS 0.925f  /* 7.5% power loss when not in top gear */

if (autotrans && gear != topgear) {
    engtorque *= AUTOLOSS;
}
```

---

## Differential System

### Magic Load-Sensitive Differential

The arcade has a special differential that:
1. Does NOT produce yaw torque from thrust imbalance
2. Splits torque proportional to wheel load

```c
if (magicdif && rearload <= -500) {
    /* Load-proportional split */
    if (TIREFORCE[2][Z] >= 0) {
        torque[2] = 0;           /* Right rear unloaded */
        torque[3] = dwtorque;    /* All to left */
    } else if (TIREFORCE[3][Z] >= 0) {
        torque[2] = dwtorque;    /* All to right */
        torque[3] = 0;           /* Left rear unloaded */
    } else {
        torque[2] = dwtorque * TIREFORCE[2][Z] / rearload;
        torque[3] = dwtorque * TIREFORCE[3][Z] / rearload;
    }
} else {
    /* Standard 50/50 split */
    torque[2] = dwtorque * 0.5;
    torque[3] = dwtorque * 0.5;
}
```

---

## Key Constants Summary

| Constant | Arcade Value | N64 Value | Description |
|----------|--------------|-----------|-------------|
| AUTO_TRANS_LOSS | 0.925 | 0.925f | Torque converter loss |
| MINGEAR | 1 | 1 | Minimum forward gear |
| MAXGEAR | 4 | 4-6 | Maximum forward gears |
| Upshift RPM | 5750 | 5750 | Auto upshift point |
| Downshift RPM | 3500 | 3500 | Auto downshift point |
| Clutch engage start | 1500 | 1500 | Centrifugal clutch min |
| Clutch engage full | 3000 | 3000 | Centrifugal clutch max |
| Friction point | 0.8 | 0.8 | Manual clutch bite point |
| Max clutch torque | 800 | 800 | ft-lb |

---

## N64 Function Mapping

### Arcade to N64 Function Equivalents

| Arcade Function | N64 Function | File | Notes |
|-----------------|--------------|------|-------|
| `drivetrain()` | `drivetrain_update()` | drivetrain.c | Main update |
| `engine()` | `drivetrain_engine()` | drivetrain.c | Torque calc |
| `transmission()` | `drivetrain_transmission()` | drivetrain.c | Ratio calc |
| `whatslips()` | `drivetrain_clutch()` | drivetrain.c | Clutch slip |
| `autoshift()` | `drivetrain_autoshift()` | drivetrain.c | Auto trans |
| `find_best_gear()` | `drivetrain_find_best_gear()` | drivetrain.c | Gear select |
| `upshift()` | `drivetrain_upshift()` | drivetrain.c | Shift up |
| `downshift()` | `drivetrain_downshift()` | drivetrain.c | Shift down |
| `enginetorque()` | `drivetrain_engine_torque()` | drivetrain.c | Curve lookup |
| `interp()` | `drivetrain_interp()` | drivetrain.c | Interpolation |

### Game.c Inline Functions

| N64 Function | Address | Purpose |
|--------------|---------|---------|
| `throttle_brake_input()` | game.c:12072 | Process throttle/brake |
| `car_gear_shift()` | game.c:12173 | Gear change |
| `car_get_gear()` | game.c:12246 | Query gear |
| `engine_sound_update()` | game.c:23001 | Audio RPM sync |
| `engine_particle_effect()` | game.c:11439 | Exhaust effects |

### Car State Structure Offsets (N64)

| Offset | Type | Field | Description |
|--------|------|-------|-------------|
| 0x100 | f32 | throttle | Throttle position |
| 0x104 | f32 | brake | Brake position |
| 0x108 | s32 | gear | Current gear |
| 0x10C | f32 | rpm | Engine RPM |
| 0x110 | f32 | torque | Output torque |
| 0x148 | f32 | maxTorque | Max engine torque |
| 0x14C | f32 | maxBrake | Max brake force |
| 0x150 | f32 | idleRpm | Idle RPM |
| 0x154 | f32 | redlineRpm | Redline RPM |
| 0x158 | f32[6] | gearRatios | Gear ratio array |
| 0x170 | s32 | autoTrans | Auto transmission flag |

---

## Drivetrain Call Flow

```
drivetrain_update()
    |
    +-- drivetrain_autoshift()     [if autotrans]
    |       +-- drivetrain_find_best_gear()
    |       +-- drivetrain_upshift()
    |       +-- drivetrain_downshift()
    |
    +-- drivetrain_engine()
    |       +-- drivetrain_engine_torque()  [torque curve lookup]
    |               +-- drivetrain_interp() [bilinear interpolation]
    |
    +-- drivetrain_transmission()  [gear ratio selection]
    |
    +-- drivetrain_clutch()        [clutch slip calculation]
    |
    +-- [differential split]       [torque to wheels]
```

---

## Torque Flow

1. **Throttle Input** -> Engine torque curve lookup
2. **Engine Torque** * gear_scale -> Scaled torque
3. **Clutch** -> Slip calculation, torque transfer
4. **Transmission Ratio** -> `dwtorque = clutchtorque * totalratio`
5. **Differential** -> Split to left/right wheels
6. **Wheel Torque** -> Tire force calculation

---

## Implementation Notes

### N64 Simplifications

The N64 version may use simplified drivetrain in some areas:

1. **Fixed time step**: 60Hz physics (dt = 1/60)
2. **Simplified clutch**: May skip full slip simulation
3. **Fixed gear ratios**: 6-speed instead of 4-speed
4. **Inline torque curve**: Polynomial approximation vs. table lookup

### Arcade-Specific Features

These may not be present in N64 version:

1. **Starter motor simulation**: Engine cranking
2. **Engine temperature warmup**: Gauge simulation
3. **Ammeter/oil pressure**: Dashboard gauges
4. **Bog sound state**: Engine stall detection

---

## References

- Original Milliken Engineering model: 1985
- Arcade C translation: Max Behensky, 1985
- Rush The Rock source: `reference/repos/rushtherock/game/`
- N64 decompilation: `src/game/drivetrain.c`
