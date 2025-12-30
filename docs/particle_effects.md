# Particle and Visual Effects System

This document describes the particle and visual effects system in San Francisco Rush 2049 (N64), cross-referenced with the arcade source code from Rush: The Rock (reference/repos/rushtherock/).

## Overview

The effects system handles:
- Tire smoke (burnout, skidding)
- Tire dust (dirt/grass surfaces)
- Water splash effects
- Collision sparks (car scraping)
- Skid marks on pavement
- Engine smoke (damage)
- Exhaust particles
- Boost flames
- Explosion/debris effects
- Weather effects (rain)

## Architecture Comparison

### Arcade (Rush: The Rock)

The arcade uses a Visual overlay system with pool-based management:

```c
// Maximum concurrent effects
#define MAX_VISUALS     100   // Pool size for environmental visuals
#define SMOKE_FRAMES    16    // Animation frames per smoke sequence
#define SMOKE_OBJS      48    // 3 * SMOKE_FRAMES per tire
#define SKID_OBJS       100   // Maximum skid marks
#define BLAST_FRAMES    16    // Explosion animation frames

// Global pools
static Visual gEnvirons[MAX_VISUALS];   // Effect overlay pool
static Visual *gEnvList, *gFreeVis;     // Used and free linked lists
static Skid   gSkid[SKID_OBJS];         // Skid mark pool
static Skid   *gSkidFree, *gSkidList;   // Skid linked lists
```

### N64 (Rush 2049)

The N64 uses a simplified particle system optimized for limited hardware:

```c
// Maximum particles (reduced for N64)
#define MAX_PARTICLES       256
#define MAX_PARTICLE_TYPES  16

// Global arrays
Particle gParticles[MAX_PARTICLES];
Emitter gEmitters[MAX_PARTICLE_TYPES];
SkidMark gSkidMarks[128];
```

## Effect Types

### 1. Tire Smoke

**Spawn Conditions:**
- Tire slip exceeds threshold on pavement
- Car speed > 10 mph (arcade requires > 5 mph for sparks)
- Frame rate is acceptable (arcade: > 20 FPS)
- Not in view 1 or 2 (behind/hood view)

**Arcade Implementation (`visuals.c:StartSmoke`):**
```c
static void StartSmoke(S16 slot, U32 tire, BOOL fast) {
    // Skip if in air, low speed, or first-person view
    if (surface == AIR || (mph < 10 && tire != 4) ||
        (slot == this_node && view < 3))
        return;

    // Determine smoke type based on surface
    if (tire == 4)
        // Engine smoke (black)
        ZOID_SetObjectDef(v->objnum, gObjList[OBJ_BSMOKE01]);
    else if (surface == DIRT)
        ZOID_SetObjectDef(v->objnum, gObjList[OBJ_DUST01]);
    else if (surface == WATER)
        ZOID_SetObjectDef(v->objnum, gObjList[OBJ_SPLASH01]);
    else if (mph >= 60)
        // Fast smoke (more opaque)
        ZOID_SetObjectDef(v->objnum, gObjList[OBJ_FSMOKE01]);
    else
        ZOID_SetObjectDef(v->objnum, gObjList[OBJ_SMOKE01]);
}
```

**N64 Implementation (`effects.c:effects_tire_smoke`):**
```c
void effects_tire_smoke(s32 car_index, s32 tire, f32 slip) {
    if (slip < 0.3f) return;  // Threshold

    intensity = (slip - 0.3f) / 0.7f;

    // Get tire position from car data
    pos[0] = car->dr_pos[0];
    pos[1] = car->dr_pos[1];
    pos[2] = 0.0f;  // Ground level

    if (rand_float() < intensity * 0.5f) {
        particle_spawn(PARTICLE_SMOKE, pos, vel,
                      rand_range(1.0f, 3.0f) * intensity,
                      (u16)(60 * intensity));
    }
}
```

### 2. Sparks

**Types:**
- `App_M_LSPARK` (0x0100) - Left side sparks
- `App_M_RSPARK` (0x0080) - Right side sparks
- `App_M_BSPARK` (0x0800) - Bottom/undercarriage sparks

**Spawn Conditions:**
- Car speed > 5 mph (left/right) or > 3 mph (bottom)
- Collision with environment (walls, rails)
- Top scrape condition (roof collision)

**Arcade Spark Positioning (`visuals.c:AnimateLSpark`):**
```c
spark = game_car[v->slot].mph > 5.0 &&
        (game_car[v->slot].appearance & App_M_LSPARK) != 0;
x = game_car[v->slot].TIRER[3][1] - 0.6;      // Left rear tire Y
y = -game_car[v->slot].TIRER[3][2] + tradius*2;  // Tire Z + radius
z = game_car[v->slot].TIRER[3][0];            // Tire X
HandleASpark(v, spark, x,y,z, OBJ_SPARKS, op);
```

**Spark Animation:**
- 6 spark textures (SPARK01A-SPARK06A)
- Random texture cycling during active spark
- Fades out over 3-5 frames when condition ends

### 3. Skid Marks

**Spawn Conditions (from `appearance` bitfield):**
- `App_M_LF_SKID` (0x0002) - Left front tire skid
- `App_M_RF_SKID` (0x0004) - Right front tire skid
- `App_M_LR_SKID` (0x0200) - Left rear tire skid
- `App_M_RR_SKID` (0x0400) - Right rear tire skid

**Additional Requirements:**
- Surface must be PAVEMENT (not DIRT, WATER, or AIR)
- Car not resurrecting (respawning)

**Arcade Skid System (`visuals.c:DoSkid`):**
```c
static void DoSkid(S16 slot, U32 tire, BOOL on) {
    on = on &&
         (m->sviscode[tire] == PAVEMENT) &&
         (m->resurrect.moving_state == -1);
    laston = (ns->skid != nil);

    if (!laston && on)
        StartSkid(ns, slot, tire);
    else if (laston && on)
        ContinueSkid(ns, slot, tire);
    else if (laston && !on)
        StopSkid(ns);
}
```

**Skid Properties:**
- Width: 0.75 * tire width (1.5 ft total)
- Height offset: 0.8 ft above road surface
- Texture: "SKID04"
- Translucency based on skid intensity (squared)
- Fade time: 2-4 seconds based on pool availability

**Deviation Detection:**
When the skid mark deviates more than `SKID_DEVIATION` (1.0 ft) from a straight line, the current skid is stopped and a new one started. This prevents stretching during turns.

### 4. Dust/Dirt Effects

**Spawn Conditions:**
- Surface type is DIRT
- Tire contact with ground
- Car moving

**Arcade:**
```c
if (surface == DIRT)
    ZOID_SetObjectDef(v->objnum, gObjList[OBJ_DUST01]);
```

16 dust animation frames (DUST1-DUST16).

### 5. Water Splash

**Spawn Conditions:**
- Surface type is WATER
- Tire contact with water
- Or car driving through water hazard

**Arcade:**
```c
if (surface == WATER)
    ZOID_SetObjectDef(v->objnum, gObjList[OBJ_SPLASH01]);
```

16 splash animation frames (SPLASH1-SPLASH16).

### 6. Engine Smoke

**Spawn Conditions:**
- `App_M_SMOKE` (0x0060) in appearance flags
- Triggered by crash/damage
- Tied to crash time for intensity

**Arcade Crash Smoke:**
```c
if ((car->appearance & App_M_SMOKE) && gGoodFrameRate)
    StartSmoke(slot, 4, false);  // tire=4 means engine smoke

// Crash smoke intensity fades over time
mult = 1 + ((IRQTIME - game_car[slot].crashtime) >> 10);
if (mult > 5) mult = 5;
```

Uses black smoke textures (SMOKE_DARK1-16).

### 7. Explosion/Blast Effects

**Sequence:**
1. 16-frame fireball animation (CARBLAST1-15)
2. Frame 7: Car transforms to hulk (burnt wreck)
3. Frame hold time: 40ms per frame
4. Total duration: ~640ms

**Arcade Blast Animation (`visuals.c:AnimateBlast`):**
```c
if (++v->index == HULK_FRAME)
    SetupCar(slot, HULK_TYPE, true);  // Replace car with hulk

if (v->index == BLAST_FRAMES) {
    MBOX_HideObject(v->objnum, HIDE_ONLY);
    v->func = 0;  // Cleanup
}
```

**Fireball Textures:**
24 fireball frames (FBALL00-FBALL30, not all sequential).

## Appearance Flags Reference

The `appearance` field is a 32-bit bitfield controlling visual effects:

| Bit(s) | Mask | Name | Description |
|--------|------|------|-------------|
| 0 | 0x00000001 | App_M_PAINT | Team color |
| 1 | 0x00000002 | App_M_LF_SKID | Left front skid |
| 2 | 0x00000004 | App_M_RF_SKID | Right front skid |
| 3 | 0x00000008 | App_M_TRANSLUCENT | Car is translucent |
| 4 | 0x00000010 | App_M_HULK | Car is destroyed |
| 5-6 | 0x00000060 | App_M_SMOKE | Engine smoke |
| 7 | 0x00000080 | App_M_RSPARK | Right side sparks |
| 8 | 0x00000100 | App_M_LSPARK | Left side sparks |
| 9-10 | 0x00000600 | App_M_LR/RR_SKID | Rear skid marks |
| 11 | 0x00000800 | App_M_BSPARK | Bottom sparks |
| 12 | 0x00001000 | App_M_BRAKE_LIGHTS | Brake lights on |
| 13 | 0x00002000 | App_M_HEAD_LIGHTS | Headlights on |
| 15 | 0x00008000 | App_M_FLAMES | On fire |
| 16-19 | 0x000F0000 | App_M_TIRE_SMOKE | Per-tire smoke |
| 22-31 | 0xFFC00000 | App_M_ANY_DAMAGE | Damage levels |

## Particle Pool Management

### Arcade Pool System

```c
Visual *GrabEnvEntry(void) {
    state = prc_set_ipl(INTS_OFF);  // Disable interrupts
    if (!gFreeVis) {
        prc_set_ipl(state);
        return nil;  // Pool exhausted
    }
    v = gFreeVis;
    gFreeVis = gFreeVis->next;
    prc_set_ipl(state);

    // Clear entry
    v->next = nil;
    v->index = -1;
    v->objnum = -1;
    v->slot = 0;
    v->data = 0;
    v->timeStamp = 0;
    v->func = nil;
    return v;
}

void ReleaseEnviron(Visual *v, BOOL unlink) {
    // Hide object
    if (v->objnum >= 0)
        MBOX_HideObject(v->objnum, HIDE_ONLY);

    // Unlink from active list
    // Add back to free list
    v->next = gFreeVis;
    gFreeVis = v;
}
```

### N64 Pool System

```c
s32 particle_spawn(u8 type, f32 pos[3], f32 vel[3], f32 size, u16 life) {
    // Density check for performance
    if (rand_float() * 100.0f > gEffects.particle_density)
        return -1;

    // Find free slot (linear scan)
    for (i = 0; i < MAX_PARTICLES; i++) {
        if (!(gParticles[i].flags & PARTICLE_FLAG_ACTIVE))
            break;
    }
    if (i >= MAX_PARTICLES) return -1;

    // Initialize particle...
}
```

### Skid Mark Pool (Arcade)

When the skid pool is exhausted, the system recycles the skid mark furthest from the camera:

```c
static Skid *GetSkid(void) {
    if (!gSkidFree && !gSkidList)
        return nil;

    if (!gSkidFree) {
        // Find skid furthest from camera
        for (s = gSkidList; s->next; s = s->next) {
            for (dsq = 0.0, i = 0; i < 3; i++) {
                d = s->pos[i] - gCamPos[i];
                dsq += d * d;
            }
            if (dsq > fardsq) {
                fardsq = dsq;
                fars = s;
            }
        }
        ReleaseSkid(fars);
    }

    // Remove from free list
    s = gSkidFree;
    gSkidFree = s->next;

    // Create mathbox polygon
    s->objnum = ZOID_NewPoly(4, gTexList[TEX_SKID], initvert, 0, 0xff);
    return s;
}
```

## Rendering Order and Blending

### Arcade Render Flags

```c
// Visual type flags (ZOID_* flags)
#define ZATR_SORTALPHA    // Alpha-sorted (back-to-front)
#define ZATR_CAMERAFACE   // Billboard facing camera
#define ZATR_CAR_BODY     // Part of car body (for reflections)
#define ZATR_CAR_WINDOW   // Window (special reflection)
#define ZATR_OFF          // Object hidden
```

**Render Order:**
1. Opaque geometry (car body, track)
2. Alpha-tested geometry (trees, fences)
3. Alpha-blended sorted (smoke, sparks, windows)
4. Overlays (HUD, skid marks)

### Translucency Levels

```c
// Translucency values (0xFF = opaque, 0x00 = invisible)
xlu = 0x60;  // Default translucent (car windows)
xlu = 0x50;  // Slightly more transparent
xlu = 0xFF;  // Fully opaque

// Fade calculation for smoke
t = (F32)(IRQTIME - v->timeStamp) / (8.0 * ONE_SEC);
xlu = (t >= 1.0) ? 0 : ((intensity) * (1.0 - t) * 0.66);
```

### N64 Blending Modes

```c
// Particle flags affecting rendering
#define PARTICLE_FLAG_FADE      0x02   // Alpha fades over lifetime
#define PARTICLE_FLAG_GROW      0x04   // Size increases
#define PARTICLE_FLAG_SHRINK    0x08   // Size decreases

// Alpha calculation
if (p->flags & PARTICLE_FLAG_FADE) {
    life_t = 1.0f - (f32)p->life / (f32)p->max_life;
    p->alpha = (u8)(255 * (1.0f - life_t));
}
```

## N64-Specific Optimizations

### 1. Reduced Particle Count

- Arcade: 100+ visual overlays
- N64: 256 particles maximum
- Density setting allows runtime reduction

### 2. Billboard Sprites

N64 uses billboarded sprite quads instead of 3D smoke models:
- Simpler geometry (single quad vs mesh)
- Camera-facing always
- Texture animation for detail

### 3. No Per-Tire Emitters

N64 simplifies to single burst spawns rather than continuous emitters per tire.

### 4. Frame Rate Throttling

```c
// Arcade checks frame rate before spawning
gGoodFrameRate = gGameExecTime < (1.0/20.0);  // 20 fps threshold

// N64 uses density percentage
if (rand_float() * 100.0f > gEffects.particle_density)
    return -1;  // Skip particle
```

### 5. Simpler Skid Marks

N64 uses fixed-size segments rather than dynamic polygon extension.

## Texture Assets

### Smoke Variants
| Name | Frames | Usage |
|------|--------|-------|
| SMOKE1-16 | 16 | Normal tire smoke |
| SMOKE_FAST1-16 | 16 | High-speed smoke (>60 mph) |
| SMOKE_DARK1-16 | 16 | Engine/damage smoke |

### Other Effects
| Name | Frames | Usage |
|------|--------|-------|
| DUST1-16 | 16 | Dirt surface particles |
| SPLASH1-16 | 16 | Water splash |
| SPARK01A-06A | 6 | Collision sparks |
| FBALL00-30 | 24 | Explosion fireball |
| CARBLAST1-15 | 15 | Car explosion sequence |
| SKID04 | 1 | Skid mark decal |
| SHADBLUR | 1 | Car shadow |

## Integration with Physics

Effects are triggered by the physics system setting appearance flags in `mdrive.c`:

```c
// Spark trigger from collision
if (left_collision)
    m->appearance |= App_M_LSPARK;
else
    m->appearance &= ~App_M_LSPARK;

if (right_collision)
    m->appearance |= App_M_RSPARK;
else
    m->appearance &= ~App_M_RSPARK;

if (bottom_scrape || top_scrape)
    m->appearance |= App_M_BSPARK;
else
    m->appearance &= ~App_M_BSPARK;
```

Skid and smoke are set by `carsnd.c` based on tire slip and surface:

```c
const U32 tire_smoke[4] = {
    App_M_LR_SMOKE, App_M_RR_SMOKE,
    App_M_LF_SMOKE | App_M_RF_SMOKE,
    App_M_LR_SMOKE | App_M_RR_SMOKE
};

const U32 tire_skid[4] = {
    App_M_LF_SKID | App_M_LR_SKID,
    App_M_RF_SKID | App_M_RR_SKID,
    App_M_LF_SKID | App_M_RF_SKID,
    App_M_LR_SKID | App_M_RR_SKID
};

if (tire_slip > threshold)
    m->appearance |= App_M_TIRE_SMOKE;
else
    m->appearance &= ~(App_M_TIRE_SMOKE | App_M_SKID_MARK);
```

## Source Files

### Arcade
- `reference/repos/rushtherock/game/visuals.c` - Main effect system
- `reference/repos/rushtherock/game/visuals.h` - Structures and enums
- `reference/repos/rushtherock/game/dis.h` - Appearance flag definitions
- `reference/repos/rushtherock/game/carsnd.c` - Tire effect triggers
- `reference/repos/rushtherock/game/mdrive.c` - Collision effect triggers

### N64
- `src/game/effects.c` - Particle system implementation
- `include/game/effects.h` - Effect types and structures
- `src/game/game.c` - Effect integration with game loop

## Key Differences Summary

| Feature | Arcade (Rush: The Rock) | N64 (Rush 2049) |
|---------|-------------------------|-----------------|
| Pool size | 100 visuals | 256 particles |
| Skid marks | 100 pooled | 128 fixed array |
| Smoke model | 3D mesh objects | Billboard sprites |
| Spark textures | 6 variants | Simplified |
| Frame rate check | 20 FPS minimum | Density percentage |
| Memory management | Linked list pools | Linear arrays |
| Draw calls | Individual objects | Batched quads |
| Reflections | Car window/body | None (simplified) |
