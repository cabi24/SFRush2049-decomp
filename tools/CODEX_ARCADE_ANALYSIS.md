# Codex Task: Arcade Source Function Catalog (Read-Only)

## Goal
Create a comprehensive reference document mapping arcade source functions to help with N64 decompilation naming.

## Input (READ-ONLY)
- `reference/repos/rushtherock/game/*.c` - Main game code
- `reference/repos/rushtherock/game/*.h` - Headers with structs/enums

## Tasks

### 1. Catalog Game State Functions
Read `reference/repos/rushtherock/game/game.c` and document:

```markdown
# Arcade Game State Functions

## Main Loop
- `game()` - Main game loop, handles gstate switch
- `playgame()` - Active racing gameplay
- `preplay()` - Pre-race setup
- `attract()` - Attract mode handler

## State Handlers
| Function | GState | Purpose |
|----------|--------|---------|
| attract() | ATTRACT | Demo mode |
| TrackSel() | TRKSEL | Track selection |
| CarSel() | CARSEL | Car selection |
| ...
```

### 2. Catalog Physics Functions
Read `reference/repos/rushtherock/game/cars.c` and `tires.c`:

```markdown
# Physics Functions

## Car Update
- `Update_MDrive()` - Main car physics update
- `CheckCrash()` - Collision response
- `ApplyForces()` - Force integration

## Tire Model
- `TireForce()` - Tire friction calculation
- `SlipAngle()` - Slip angle computation
```

### 3. Catalog AI/Drone Functions
Read `reference/repos/rushtherock/game/drones.c`:

```markdown
# AI Functions
- `DoDrones()` - Main AI update loop
- `InitDrones()` - Initialize AI cars
- `DroneThink()` - AI decision making
```

### 4. Catalog Rendering Functions
Read `reference/repos/rushtherock/game/visuals.c`:

```markdown
# Rendering Functions
- `UpdateVisuals()` - Main render update
- `DrawCar()` - Car model rendering
- `DrawTrack()` - Track geometry
```

### 5. Document Key Structures
Read headers and document important structs:

```markdown
# Key Structures

## CAR (car state)
- position, velocity, orientation
- engine RPM, gear, throttle
- tire states

## DRONE (AI car)
- target waypoint
- behavior flags
```

## Output
Create: `docs/arcade_function_mapping.md`

Structure:
1. Game State Machine (with state diagram)
2. Physics System Functions
3. AI/Drone System Functions
4. Rendering Functions
5. Audio Functions
6. Key Data Structures
7. Global Variables Reference

## Guidelines
- READ-ONLY: Do not modify any source files
- Focus on function PURPOSE, not implementation details
- Note function signatures (parameters, return types)
- Cross-reference related functions
- Flag functions likely shared with N64 version

## After Completion
The output document will help Claude rename N64 `func_*` functions by matching behavior patterns to arcade equivalents.
