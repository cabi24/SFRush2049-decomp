# Rush 2049 N64 Car Selection System Documentation

This document provides comprehensive documentation for the car selection system in San Francisco Rush 2049 for Nintendo 64, including comparisons to the original arcade version.

## Overview

The car selection system in Rush 2049 N64 allows players to choose from multiple vehicles, each with distinct performance characteristics. The system is implemented primarily in:

- `/home/cburnes/projects/rush2049-decomp/src/game/game.c` - Main game logic with car stats display
- `/home/cburnes/projects/rush2049-decomp/src/game/select.c` - Track and car selection flow
- `/home/cburnes/projects/rush2049-decomp/src/game/carsel.c` - Car selection subsystem
- `/home/cburnes/projects/rush2049-decomp/src/game/garage.c` - Car customization (N64-specific)

## Car Roster

### Complete Car List (12 Vehicles)

| ID | Name | Speed | Accel | Handling | Weight | Class | Initial Status |
|----|------|-------|-------|----------|--------|-------|----------------|
| 0 | Crusher | 6 | 5 | 4 | 8 | Muscle | Unlocked |
| 1 | Dominator | 7 | 6 | 5 | 7 | Muscle | Unlocked |
| 2 | Velocity | 9 | 7 | 6 | 4 | Sports | Unlocked |
| 3 | Rocket | 10 | 8 | 5 | 3 | Exotic | Unlocked |
| 4 | Panther | 7 | 6 | 8 | 5 | Sports | Locked |
| 5 | Bruiser | 5 | 4 | 4 | 10 | Truck | Locked |
| 6 | Speedster | 8 | 9 | 7 | 4 | Sports | Locked |
| 7 | Rampage | 6 | 5 | 5 | 9 | Muscle | Locked |
| 8 | Nitro | 9 | 10 | 6 | 5 | Exotic | Locked |
| 9 | Blaze | 8 | 7 | 9 | 4 | Sports | Locked |
| 10 | Phantom | 10 | 8 | 8 | 3 | Exotic | Locked |
| 11 | Prototype | 10 | 10 | 10 | 5 | Special | Hidden |

**Note**: Stats are on a 0-10 scale. Source: `src/game/game.c` lines 34023-34038.

### Car Classes

From `/home/cburnes/projects/rush2049-decomp/include/game/carsel.h`:

```c
#define CAR_CLASS_COMPACT       0   // Lightweight, nimble
#define CAR_CLASS_SPORTS        1   // Balanced performance
#define CAR_CLASS_MUSCLE        2   // High power, heavy
#define CAR_CLASS_EXOTIC        3   // Top-tier performance
```

### Alternate Car List (from carsel.c structure)

The `gCarList` array in `/home/cburnes/projects/rush2049-decomp/src/game/carsel.c` shows an alternative categorization used in the selection screen:

| Category | Cars | Speed | Accel | Handling | Weight | Traction |
|----------|------|-------|-------|----------|--------|----------|
| Compact 1 | Compact | 60 | 70 | 80 | 40 | 75 |
| Compact 2 | Compact | 65 | 75 | 75 | 45 | 70 |
| Compact 3 | Compact | 70 | 65 | 85 | 35 | 80 |
| Sports 1 | Sports | 80 | 80 | 70 | 55 | 65 |
| Sports 2 | Sports | 85 | 75 | 75 | 50 | 70 |
| Sports 3 | Sports | 90 | 70 | 65 | 60 | 60 |
| Muscle 1 | Muscle | 85 | 90 | 55 | 75 | 55 |
| Muscle 2 | Muscle | 90 | 85 | 50 | 80 | 50 |
| Muscle 3 | Muscle | 95 | 80 | 45 | 85 | 45 |
| Exotic 1 | Exotic | 95 | 85 | 70 | 55 | 60 |
| Exotic 2 | Exotic | 100 | 80 | 65 | 50 | 55 |
| Exotic 3 | Exotic | 100 | 90 | 60 | 45 | 50 |

**Note**: These stats use a 0-100 scale for finer granularity.

## Car Unlock Conditions

From `/home/cburnes/projects/rush2049-decomp/src/game/game.c` lines 34064-34121:

| Cars | Unlock Requirement |
|------|-------------------|
| 0-3 (Crusher, Dominator, Velocity, Rocket) | Always unlocked |
| 4-5 (Panther, Bruiser) | Complete 2 tracks |
| 6-7 (Speedster, Rampage) | Complete 4 tracks |
| 8-9 (Nitro, Blaze) | Complete 6 tracks |
| 10 (Phantom) | Complete 8 tracks |
| 11 (Prototype) | Achieve gold time on all 8 tracks |

### Unlock Check Function

```c
s32 car_unlock_check(s32 carId) {
    // First 4 cars always unlocked
    if (carId < 4) return 1;

    // Check unlock bits from save data
    if (unlockedCars & (1 << carId)) return 1;

    // Count completed tracks and check thresholds
    // ...
}
```

## Car Selection UI Flow

### Phase-Based Selection

From `/home/cburnes/projects/rush2049-decomp/include/game/select.h`:

1. **Phase 0**: Track Selection
2. **Phase 1**: Car Selection
3. **Phase 2**: Transmission Selection

### State Machine

```c
/* Selection states */
#define SELECT_STATE_IDLE       0   // Not active
#define SELECT_STATE_INIT       1   // Initializing
#define SELECT_STATE_ACTIVE     2   // Player browsing
#define SELECT_STATE_LOCKED     3   // Selection confirmed
#define SELECT_STATE_DONE       4   // Complete
#define SELECT_STATE_TIMEOUT    5   // Timer expired

/* Car selection sub-states */
#define CARSEL_BROWSING         0   // Browsing cars
#define CARSEL_CONFIRM          1   // Confirming selection
#define CARSEL_COLOR            2   // Selecting color
#define CARSEL_LOCKED           3   // Locked in
```

### Selection Timing

From `/home/cburnes/projects/rush2049-decomp/include/game/select.h`:

```c
#define TRK_SELECT_TIME     (20 * 60)   // 20 seconds track select
#define CAR_SELECT_TIME     (30 * 60)   // 30 seconds car select
#define CAR_DELAY_TIME      (60 / 3)    // ~0.33s delay before lock
#define HURRY_BLINK_TIME    30          // Blink rate for timeout warning
```

### UI Flow

1. Player enters car selection screen
2. Car model rotates on turntable (auto-rotate at 0.5 deg/frame)
3. Left/Right to browse cars
4. Up/Down to change color
5. A button to confirm selection
6. B button to go back to track selection
7. Timer counts down (30 seconds)
8. On timeout: auto-select current car

## Paint/Color Selection

### Available Colors (8 Standard)

From `/home/cburnes/projects/rush2049-decomp/src/game/game.c` lines 34128-34170:

| ID | Color Name | Primary RGB | Secondary RGB |
|----|-----------|-------------|---------------|
| 0 | Red | 0xFF2020 | 0x800000 |
| 1 | Blue | 0x2020FF | 0x000080 |
| 2 | Green | 0x20FF20 | 0x008000 |
| 3 | Yellow | 0xFFFF20 | 0x808000 |
| 4 | Orange | 0xFF8020 | 0x804000 |
| 5 | Purple | 0x8020FF | 0x400080 |
| 6 | White | 0xE0E0E0 | 0x808080 |
| 7 | Black | 0x404040 | 0x202020 |

### Extended Color Palette (Garage Mode)

From `/home/cburnes/projects/rush2049-decomp/src/game/garage.c`:

| Category | Colors |
|----------|--------|
| Gloss | Red, Blue, Green, Yellow, Orange, Purple, White, Black |
| Metallic | Silver, Gold, Steel Blue, Crimson, Forest |
| Pearl | Indigo, Pink, Turquoise |
| Matte | Matte Black, Gunmetal, Brown, Olive |
| Chrome | Chrome, Ice Chrome, Rose Gold |

### Paint Types

```c
#define PAINT_GLOSS      0   // Standard glossy
#define PAINT_METALLIC   1   // Metallic flake
#define PAINT_PEARL      2   // Pearlescent
#define PAINT_MATTE      3   // Flat finish
#define PAINT_CHROME     4   // Reflective chrome
```

## Car Stats Structure

### CarStats (from carsel.h)

```c
typedef struct CarStats {
    char    name[16];           // Car name
    u8      car_class;          // CAR_CLASS_* enum
    u8      speed;              // Max speed rating (0-100)
    u8      accel;              // Acceleration rating (0-100)
    u8      handling;           // Handling rating (0-100)
    u8      weight;             // Weight rating (0-100)
    u8      traction;           // Traction rating (0-100)
    u8      unlocked;           // Is car unlocked
    u8      pad;
    u32     model_id;           // 3D model ID
    u32     preview_texture;    // Preview image
} CarStats;
```

### CarInfo (from select.h)

```c
typedef struct CarInfo {
    const char  *name;              // Car name
    u8          car_id;             // Internal ID
    u8          flags;              // CAR_FLAG_*
    u8          speed_rating;       // 1-10 speed
    u8          handling_rating;    // 1-10 handling
    u8          accel_rating;       // 1-10 acceleration
    u8          weight_rating;      // 1-10 weight
    u8          default_color;      // Default color index
    u8          pad;
} CarInfo;
```

### Car Flags

```c
#define CAR_FLAG_UNLOCKED       0x01    // Car available
#define CAR_FLAG_HIDDEN         0x02    // Hidden car
#define CAR_FLAG_SPECIAL        0x04    // Special car (stunt mode)
```

## Comparison to Arcade Version

### Arcade Car Selection (Rush The Rock)

The arcade version uses a different car selection approach based on difficulty levels rather than individual vehicles:

#### Arcade Cars (from `reference/repos/rushtherock/game/cars.c`)

| Model | Name | Difficulty Level |
|-------|------|------------------|
| 0 | Beginner | Easiest - most forgiving |
| 1 | Advanced | Medium difficulty |
| 2 | Expert | Challenging |
| 3 | Extreme | Most difficult |

#### Arcade Car Parameters

The arcade uses a detailed physics-based car structure:

```c
typedef struct car {
    char *name;                 // Name of car
    F32 mass;                   // Mass in slugs
    F32 I[3];                   // Moment of inertia (roll, pitch, yaw)
    F32 springrate[4];          // Spring constant of suspension
    F32 farspringrate;          // Front anti-roll bar
    F32 rarspringrate;          // Rear anti-roll bar
    F32 cdamping[4];            // Compression damping
    F32 rdamping[4];            // Rebound damping
    F32 steerratio;             // Steering ratio (wheel/kingpin)
    F32 swtpg;                  // Steering wheel torque per G
    F32 srefpcybo2;             // Aerodynamic drag factor
    F32 rollresist;             // Rolling resistance
    struct tiredes tires[4];    // Tire descriptions per wheel
    F32 TIRER[4][3];            // CG to wheel vectors
    F32 brakebal;               // Front brake balance
    F32 engmi;                  // Engine flywheel inertia
    F32 dwratio;                // Differential gear ratio
    F32 clutchmaxt;             // Max clutch torque
    F32 torquescale;            // Engine torque multiplier
    F32 transarray[6];          // Gear ratios
    // ... more physics parameters
} Car;
```

### Key Differences

| Feature | N64 Version | Arcade Version |
|---------|-------------|----------------|
| **Car Count** | 12 unique vehicles | 4 difficulty-based cars |
| **Selection Style** | Browse individual cars | Choose difficulty level |
| **Stats Display** | Speed/Accel/Handling/Weight bars | No visible stats |
| **Customization** | Color selection, garage | Limited color options |
| **Unlock System** | Track completion based | N/A (arcade credits) |
| **Physics Model** | Simplified arcade | Full Milliken model |
| **Car Names** | Unique names (Crusher, etc.) | Beginner/Advanced/Expert/Extreme |

### Arcade Selection Screen

From `reference/repos/rushtherock/game/sselect.c`:

```c
#define NPADS           4       // Number of car pads
#define CAR_DELAY_TIME  (ONE_SEC/3)  // Delay before lock
#define NCARS           12      // Total car models available
```

The arcade uses:
- **4 selection pads** displayed simultaneously
- **Steering wheel** to select between pads
- **Gas pedal** to confirm selection
- **VIEW1 button** to toggle between car sets (if >4 cars)
- **Clutch selection** after car selection (Auto/Manual)

### Arcade Clutch Selection

The arcade has a separate transmission selection phase:

```c
#define CLUTCH_SEL_TIME (11 * ONE_SEC)  // 11 seconds for transmission

void ClutchSel(void) {
    // Handle tourney mode overrides
    // Lock transmission choice
    // Proceed to race negotiation
}
```

### Arcade Car List Reference

From `reference/repos/rushtherock/game/cars.c`:

The arcade defines 12 car collision boxes, suggesting it had plans for more cars:

```c
const COLLSIZE collsize[NCARS] = {
    /* BMW     */   { 5.75, 6.50, 3.0, 2.0 },
    /* INTEGRA */   { 6.29, 6.58, 2.9, 2.0 },
    /* VETTE   */   { 6.05, 6.65, 2.9, 2.0 },
    /* BUGATTI */   { 6.25, 5.75, 3.0, 2.0 },
    /* TAXI    */   { 6.44, 6.64, 3.0, 2.0 },
    /* VIPER   */   { 6.25, 6.75, 3.0, 2.0 },
    /* BUG     */   { 5.75, 6.00, 3.0, 2.0 },
    /* TRUCK   */   { 6.70, 6.97, 2.8, 2.0 },
    /* BUS     */   { 6.25, 6.00, 3.0, 2.0 },
    /* CAMERO  */   { 6.75, 7.00, 3.0, 2.0 },
    /* SUPRA   */   { 6.50, 6.50, 3.0, 2.0 },
    /* CONCEPT */   { 6.25, 5.50, 3.0, 2.0 },
};
```

## Technical Implementation Details

### Car Selection State

From `/home/cburnes/projects/rush2049-decomp/include/game/carsel.h`:

```c
typedef struct PlayerSelect {
    s32     car_index;          // Selected car index
    s32     color_index;        // Selected car color
    s32     trans_mode;         // Transmission mode
    u8      state;              // Selection state
    u8      is_active;          // Player is active
    u8      is_ready;           // Player confirmed selection
    u8      pad;
    u32     input_timer;        // Input repeat timer
    f32     turntable_angle;    // Car preview rotation
} PlayerSelect;
```

### Car Preview Rotation

```c
#define TURNTABLE_SPEED     0.02f   // Radians per frame

void carsel_player_update(s32 player) {
    ps->turntable_angle += TURNTABLE_SPEED;
    if (ps->turntable_angle > 6.28318f) {
        ps->turntable_angle -= 6.28318f;
    }
}
```

### Input Handling

```c
#define INPUT_REPEAT_DELAY      12  // Frames before repeat
#define INPUT_REPEAT_RATE       4   // Frames between repeats

void carsel_select_next_car(s32 player) {
    if (ps->input_timer > 0) return;

    ps->car_index++;
    if (ps->car_index >= gNumCars) ps->car_index = 0;

    // Skip unavailable cars
    while (!carsel_is_car_available(ps->car_index)) {
        ps->car_index++;
        if (ps->car_index >= gNumCars) ps->car_index = 0;
    }

    ps->input_timer = INPUT_REPEAT_DELAY;
}
```

## Garage System (N64 Exclusive)

The N64 version includes a garage system not present in the arcade:

### Garage States

```c
#define GARAGE_STATE_CLOSED     0
#define GARAGE_STATE_BROWSE     1   // Browsing cars
#define GARAGE_STATE_PAINT      2   // Paint selection
#define GARAGE_STATE_UPGRADES   3   // Upgrade menu
```

### Car Definitions (Garage)

From `/home/cburnes/projects/rush2049-decomp/src/game/garage.c`:

| Type | Name | Base Speed | Accel | Handling | Traction | Weight | Grip |
|------|------|------------|-------|----------|----------|--------|------|
| Compact | Zipster | 180 | 3.2 | 0.9 | 0.7 | 2200 | 0.6 |
| Sports | Velocity | 200 | 2.8 | 0.85 | 0.8 | 2800 | 0.7 |
| Muscle | Dominator | 210 | 2.5 | 0.7 | 0.75 | 3400 | 0.8 |
| Exotic | Phantom | 230 | 2.2 | 0.9 | 0.85 | 2600 | 0.65 |
| Truck | Bruiser | 170 | 3.5 | 0.6 | 0.7 | 4200 | 1.0 |
| Concept | Prototype X | 240 | 2.0 | 0.95 | 0.9 | 2400 | 0.55 |
| Classic | Retro Racer | 190 | 3.0 | 0.75 | 0.65 | 3000 | 0.75 |
| Special | Rush 2049 | 250 | 1.8 | 0.95 | 0.95 | 2200 | 0.7 |

### Upgrade System

```c
typedef enum UpgradeType {
    UPGRADE_ENGINE      = 0,
    UPGRADE_TURBO       = 1,
    UPGRADE_HANDLING    = 2,
    UPGRADE_BRAKES      = 3,
    UPGRADE_TIRES       = 4,
    UPGRADE_SUSPENSION  = 5,
    UPGRADE_WEIGHT      = 6,
    UPGRADE_ARMOR       = 7
} UpgradeType;
```

Upgrade costs per level:

| Upgrade | Level 1 | Level 2 | Level 3 | Level 4 |
|---------|---------|---------|---------|---------|
| Engine | 1000 | 2500 | 5000 | 10000 |
| Turbo | 800 | 2000 | 4000 | 8000 |
| Handling | 600 | 1500 | 3000 | 6000 |
| Brakes | 500 | 1200 | 2500 | 5000 |
| Tires | 700 | 1800 | 3500 | 7000 |
| Suspension | 600 | 1500 | 3000 | 6000 |
| Weight | 1200 | 3000 | 6000 | 12000 |
| Armor | 800 | 2000 | 4000 | 8000 |

## Sound Effects

From `/home/cburnes/projects/rush2049-decomp/src/game/sound.c`:

```c
#define S_CARSELECT     // Car selected confirmation
#define S_KCARSELECT    // Kill car select music

void start_car_select_music(void) {
    SOUND(S_CARSELECT);
}

void stop_car_select_music(void) {
    SOUND(S_KCARSELECT);
}
```

## Related Files

### Source Files
- `src/game/game.c` - Main car stats and UI rendering
- `src/game/select.c` - Selection flow controller
- `src/game/carsel.c` - Car selection subsystem
- `src/game/garage.c` - Garage/customization system
- `src/game/state.c` - Game state machine (GS_CARSEL)

### Header Files
- `include/game/select.h` - Selection structures and constants
- `include/game/carsel.h` - Car selection definitions
- `include/game/garage.h` - Garage system definitions
- `include/game/state.h` - Game state definitions

### Arcade Reference
- `reference/repos/rushtherock/game/sselect.c` - Arcade selection screen
- `reference/repos/rushtherock/game/cars.c` - Arcade car physics data
- `reference/repos/rushtherock/game/drivsym.h` - Car structure definition
- `reference/repos/rushtherock/game/globals.h` - NCARS definition

---

*Documentation generated from Rush 2049 N64 decompilation project*
*Last updated: 2025-12-31*
