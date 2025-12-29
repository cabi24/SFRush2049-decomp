# Effects and Particle System Mapping

This document maps the visual effects and particle systems between the Rush The Rock arcade source code and the Rush 2049 N64 decompilation.

## Overview

Both versions share a similar effects architecture:
- **Particle system**: Individual particles with position, velocity, life, and color
- **Emitters**: Continuous particle sources attached to cars or world positions
- **Visual effects**: Sparks, smoke, dust, water splash, fire/explosions
- **Skid marks**: Track-based tire marks that fade over time

## Arcade Effect System (visuals.c)

### Core Architecture

The arcade uses a `Visual` structure for per-car effects and an environment list for world effects:

```c
typedef struct Visual {
    struct Visual *next;        // Linked list
    S32 index;                  // Animation sequence index
    S32 objnum;                 // Object number
    U32 slot;                   // Owner car slot
    U32 data;                   // Arbitrary data
    U32 timeStamp;              // Last update time
    void (*func)();             // Controller function
} Visual;
```

Key constants:
- `MAX_VISUALS = 100` - Maximum concurrent environment visuals
- `SMOKE_FRAMES = 16` - Smoke animation frames
- `SMOKE_OBJS = 48` - Smoke objects per tire (3 * 16 frames)
- `SKID_OBJS = 100` - Maximum skid mark segments
- `BLAST_FRAMES = 16` - Explosion animation frames
- `BLAST_HOLD = 40ms` - Frame hold time for explosions

### Particle/Effect Types

| Arcade Type | Object Name | Purpose |
|-------------|-------------|---------|
| `OBJ_SPARKS` | "SPARKO3" | Metal sparks from collisions |
| `OBJ_SMOKE01-16` | "SMOKE1"-"SMOKE16" | Slow tire smoke animation |
| `OBJ_FSMOKE01-16` | "SMOKE_FAST1"-"SMOKE_FAST16" | Fast tire smoke (>60 mph) |
| `OBJ_BSMOKE01-16` | "SMOKE_DARK1"-"SMOKE_DARK16" | Dark engine/crash smoke |
| `OBJ_DUST01-16` | "DUST1"-"DUST16" | Dirt surface dust |
| `OBJ_SPLASH01-16` | "SPLASH1"-"SPLASH16" | Water splash |
| `OBJ_BLAST01-15` | "CARBLAST1"-"CARBLAST15" | Explosion animation |

### Appearance Bitfield Masks (dis.h)

```c
// Tire smoke (per-wheel)
#define App_M_TIRE_SMOKE    0x000F0000  // Any tire smoke
#define App_M_LF_SMOKE      0x00080000  // Left front
#define App_M_RF_SMOKE      0x00040000  // Right front
#define App_M_LR_SMOKE      0x00020000  // Left rear
#define App_M_RR_SMOKE      0x00010000  // Right rear

// Sparks (collision locations)
#define App_M_SPARKS        0x00000980  // Any spark
#define App_M_LSPARK        0x00000100  // Left side sparks
#define App_M_RSPARK        0x00000080  // Right side sparks
#define App_M_BSPARK        0x00000800  // Bottom sparks

// Skid marks (per-wheel)
#define App_M_SKID_MARK     0x00000606  // Any skid
#define App_M_LF_SKID       0x00000002  // Left front
#define App_M_RF_SKID       0x00000004  // Right front
#define App_M_LR_SKID       0x00000200  // Left rear
#define App_M_RR_SKID       0x00000400  // Right rear

// Engine smoke
#define App_M_SMOKE         0x00000060  // Engine smoke

// Other effects
#define App_M_FLAMES        0x00008000  // Flames (unused)
#define App_M_DAMAGE        0x00000018  // Damage state
#define App_M_HULK          0x00000010  // Car is destroyed hulk
```

## Arcade Effect Functions

### Visual Types (visuals.h)

```c
enum VisualTypes {
    VIS_BLAST = 0,      // Explosion
    VIS_BRAKES,         // Brake lights
    VIS_LSPARK,         // Left sparks
    VIS_RSPARK,         // Right sparks
    VIS_BSPARK,         // Bottom sparks
    VIS_TIRE1-4,        // Tires 0-3
    VIS_SHADOW,         // Car shadow
    VIS_QUAD0-4,        // Body quadrants
    VIS_WINDOW0-3,      // Windows
    VIS_FRAME,          // Main body frame
    NUM_VISUALS
};
```

### Core Functions

| Function | Purpose | Location |
|----------|---------|----------|
| `InitVisuals()` | Initialize effects system | visuals.c:600 |
| `UpdateVisuals(slot)` | Per-frame car effects update | visuals.c:1006 |
| `UpdateEnvirons()` | Update environment effects | visuals.c:931 |
| `StartEnvironment()` | Create environment effects | visuals.c:847 |

### Smoke Functions

| Function | Purpose | Trigger |
|----------|---------|---------|
| `StartSmoke(slot, tire, fast)` | Spawn tire smoke | `App_M_*_SMOKE` bit set |
| `AnimateSmoke(v, op)` | Animate smoke object | Per-frame callback |

**Smoke Trigger Conditions:**
- `tire == 4`: Engine smoke (explosion aftermath)
- `surface == DIRT`: Dust cloud instead of smoke
- `surface == WATER`: Water splash instead of smoke
- `mph >= 60`: Fast smoke animation
- `mph < 60`: Normal smoke animation
- `mph < 10`: No tire smoke

**Smoke Animation:**
1. Create smoke object at tire position
2. Move with car's dead-reckoned velocity
3. Fade translucency over time
4. Auto-destroy after 16 frames

### Spark Functions

| Function | Purpose | Trigger |
|----------|---------|---------|
| `StartSparks(slot)` | Initialize spark visuals | Car creation |
| `AnimateLSpark(v, op)` | Left side sparks | `App_M_LSPARK` |
| `AnimateRSpark(v, op)` | Right side sparks | `App_M_RSPARK` |
| `AnimateBSpark(v, op)` | Bottom sparks | `App_M_BSPARK` |
| `HandleASpark(v, on, x, y, z, name, op)` | Generic spark handler | Internal |

**Spark Trigger Conditions:**
- Car must be moving (>5 mph for side, >3 mph for bottom)
- Appropriate appearance bit must be set
- Position based on tire rear positions

**Spark Animation:**
1. Position at collision point (tire position with offset)
2. Animate through 6 spark textures (SPARK01A-SPARK06A)
3. Random frame selection during active spark
4. Fade out when spark disabled

### Skid Mark Functions

| Function | Purpose |
|----------|---------|
| `InitSkids()` | Initialize skid pool |
| `DoSkid(slot, tire, on)` | Handle skid mark state |
| `StartSkid(ns, slot, tire)` | Start new skid mark |
| `ContinueSkid(ns, slot, tire)` | Extend skid mark |
| `StopSkid(ns)` | Finalize skid mark |
| `UpdateSkid(ns, slot, tire)` | Update skid vertices |
| `AnimateSkids()` | Fade all skid marks |
| `GetSkid()` | Allocate skid from pool |
| `ReleaseSkid(s)` | Free skid to pool |

**Skid Mark System:**
- Uses `NewSkid` structure for active creation
- Uses `Skid` structure for completed marks
- Pool-based allocation (100 skid objects)
- Fades over 2-4 seconds based on pool availability
- Creates 4-vertex quads textured with "SKID04"
- Width based on tire width (0.75 units each side)

**Skid Conditions:**
- `surface == PAVEMENT` (not dirt, air, or water)
- Car not resurrecting
- Appropriate appearance bit set

### Blast/Explosion Functions

| Function | Purpose |
|----------|---------|
| `StartBlast(slot)` | Start explosion animation |
| `AnimateBlast(v, op)` | Animate explosion frames |

**Blast Animation:**
1. 16-frame animation (CARBLAST1-CARBLAST15)
2. Each frame held for 40ms
3. At frame 7, car becomes hulk
4. Translucent, alpha-sorted rendering

## N64 Effect System

### Particle Types (particles.h)

```c
#define PARTICLE_SPARK          0   // Metal sparks
#define PARTICLE_SMOKE          1   // Smoke puff
#define PARTICLE_DUST           2   // Dust cloud
#define PARTICLE_TIRE_SMOKE     3   // Tire smoke (burnout)
#define PARTICLE_EXHAUST        4   // Exhaust fumes
#define PARTICLE_DEBRIS         5   // Debris chunks
#define PARTICLE_GLASS          6   // Glass shards
#define PARTICLE_FIRE           7   // Fire/flame
#define PARTICLE_EXPLOSION      8   // Explosion flash
#define PARTICLE_WATER_SPLASH   9   // Water splash
#define PARTICLE_WATER_SPRAY    10  // Water spray
#define PARTICLE_DIRT           11  // Dirt clumps
#define PARTICLE_GRASS          12  // Grass particles
#define PARTICLE_BOOST_TRAIL    13  // Boost afterburner
#define PARTICLE_WING_TRAIL     14  // Wing deploy trail
#define PARTICLE_PICKUP_SPARKLE 15  // Pickup collection
```

### N64 Particle Structure

```c
typedef struct Particle {
    u8      active;
    u8      type;
    u8      blend_mode;
    u8      flags;
    f32     pos[3];
    f32     vel[3];
    f32     accel[3];
    f32     rotation, rot_vel;
    f32     size, start_size, end_size;
    f32     life, max_life;
    ParticleColor start_color, end_color, current_color;
    f32     gravity, drag, bounce;
    u8      texture_id, frame, num_frames, frame_rate;
    f32     frame_timer;
} Particle;
```

### N64 Effect Functions

| N64 Function | Address | Arcade Equivalent |
|--------------|---------|-------------------|
| `effects_init` | - | `InitVisuals` |
| `effects_update` | 0x800B811C | `UpdateEnvirons` |
| `effects_update_emitters` | 0x800B811C | `UpdateEnvirons` |
| `engine_particle_effect` | 0x800A5588 | `StartSmoke(slot, 4, ...)` |
| `exhaust_smoke_effect` | 0x800A5744 | `StartSmoke` (exhaust) |
| `debris_spawn` | - | (no direct equivalent) |
| `smoke_puff_spawn` | - | `StartSmoke` |
| `particle_spawn` | - | `StartSmoke`/`HandleASpark` |
| `particle_update` | 0x800EEDB0 | `AnimateSmoke` |
| `effect_system_init` | 0x800C3614 | `InitVisuals` |
| `effect_slot_init` | 0x800C3594 | `GrabEnvEntry` |

### N64 Emitter System

The N64 uses a more structured emitter system:

```c
// Emitter structure (152 bytes)
// offset 0x24: position (vec3)
// offset 0x34: velocity (vec3)
// offset 0x48: acceleration
// offset 0x54: previous position (vec3)

void effects_update_emitters(void) {
    for each emitter:
        1. Copy current position to previous
        2. Call emitter_update()
        3. Update velocity with acceleration
        4. Conditionally spawn particles
}
```

## Effect Trigger Mapping

### Collision Sparks

| Trigger | Arcade | N64 |
|---------|--------|-----|
| Side scrape | `App_M_LSPARK`/`App_M_RSPARK` | `engine_particle_effect(car, 1)` |
| Bottom scrape | `App_M_BSPARK` | `engine_particle_effect(car, 1)` |
| High-speed impact | `StartBlast` | `effects_collision()` |

### Tire Smoke

| Trigger | Arcade | N64 |
|---------|--------|-----|
| Tire slip | `App_M_*_SMOKE` bits | `effects_tire_smoke(car, tire, slip)` |
| Burnout | Fast smoke animation | `PARTICLE_TIRE_SMOKE` |
| On dirt | `DUST` objects | `PARTICLE_DUST` |
| In water | `SPLASH` objects | `PARTICLE_WATER_SPLASH` |

### Engine Effects

| Trigger | Arcade | N64 |
|---------|--------|-----|
| Engine damage | `App_M_SMOKE` | `engine_particle_effect(car, 2)` |
| Exhaust | - | `exhaust_smoke_effect()` |
| Boost flame | - | `effects_boost_flame()` |

### Crash Debris

| Trigger | Arcade | N64 |
|---------|--------|-----|
| Light crash | - | `debris_spawn(car, 0)` - 3 debris |
| Medium crash | - | `debris_spawn(car, 1)` - 8 debris |
| Heavy crash | `StartBlast` | `debris_spawn(car, 2)` - 15 debris |

## Visual Parameters

### Smoke Colors

| Type | Start Color | End Color |
|------|-------------|-----------|
| Tire smoke | White (0xE0E0E0FF) | Black (0x202020FF) |
| Dark smoke | Gray (0x606060FF) | Black (0x000000FF) |
| Dust | Brown (0x8B7355FF) | Yellow (0xC0B090FF) |
| Water | Blue (0x4080C0FF) | Transparent |

### Particle Lifetimes

| Type | Arcade Frames | N64 Frames |
|------|---------------|------------|
| Smoke | 16 (animation) | 30-60 |
| Dust | 16 | 40 |
| Sparks | 3-6 | 15-30 |
| Explosion | 16 | 60 |
| Skid fade | 2-4 seconds | 2-4 seconds |

### Spark Textures

Arcade uses 6 spark textures cycled randomly:
- SPARK01A - SPARK06A

N64 likely uses similar sprite-based approach with RDP texture loading.

## Key Differences

1. **Rendering**: Arcade uses 3dfx/Glide with pre-made 3D objects; N64 uses RDP with billboarded sprites
2. **Pool management**: Arcade uses linked lists; N64 uses arrays with indices
3. **Rush 2049 additions**: Boost flame, wing trails, pickup sparkles (not in Rock)
4. **Physics**: N64 has more detailed particle physics (bounce, drag, ground collision)
5. **Emitter system**: N64 has formal emitter concept; arcade creates particles directly

## Implementation Notes

### Arcade to N64 Porting

When porting arcade effects to N64:

1. Replace `MBOX_*` object calls with particle spawns
2. Convert `ZOID_*` texture calls to RDP texture loads
3. Replace `IRQTIME` timestamps with N64 frame counter
4. Convert appearance bitfield checks to N64 car state structure
5. Adjust particle counts for N64 performance (~256 max vs arcade's 100+ objects)

### Performance Considerations

- N64 limits: ~256 particles, 32 emitters
- Use LOD for distant effects
- Reduce particle density setting based on car count
- Pool-based allocation critical for avoiding fragmentation

## Files

| File | Purpose |
|------|---------|
| `reference/repos/rushtherock/game/visuals.c` | Arcade effects implementation |
| `reference/repos/rushtherock/game/visuals.h` | Arcade effects structures |
| `reference/repos/rushtherock/game/dis.h` | Appearance bitfield defines |
| `src/game/effects.c` | N64 effects implementation |
| `include/game/particles.h` | N64 particle system header |
| `src/game/game.c` | N64 game loop with effects calls |
