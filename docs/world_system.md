# World/Environment System Documentation

This document describes the world/environment system for San Francisco Rush 2049 on N64, based on analysis of both the N64 decompilation and the Rush The Rock arcade source code (`reference/repos/rushtherock/`).

## Overview

The world/environment system manages:
1. **World Coordinate System** - 3D positioning and transformations
2. **Environment Objects** - Static and interactive world geometry
3. **Weather/Fog Effects** - Atmospheric rendering
4. **Skybox Rendering** - Background environment
5. **Lighting Model** - Surface illumination
6. **Dynamic Objects** - Traffic, obstacles, destructibles
7. **Target System** - Interactive collidable objects

---

## 1. World Coordinate System

### Arcade Coordinate System (Rush The Rock)

The arcade version uses a right-handed coordinate system with specific conventions:

```c
/* From modeldat.h */
F32 RWR[3];        /* Real World Position (ft) - X, Y, Z */
F32 UV[3][3];      /* Rotation Matrix - unit length orthonormal */
F32 V[3];          /* Velocity in world coordinates */
```

**Axis conventions:**
- **X axis**: Left/Right (lateral)
- **Y axis**: Up/Down (vertical)
- **Z axis**: Forward/Back (longitudinal)

**Units**: Feet (ft) for positions, feet/second for velocities.

### N64 Coordinate System

The N64 version maintains similar conventions but uses display coordinates for rendering:

```c
/* Display coordinate conversion (camera.c) */
#define vcopy120(d, s) { d[0] = s[1]; d[1] = -(s[2]); d[2] = s[0]; }
```

This macro converts from model coordinates to display coordinates:
- Model X -> Display Z
- Model Y -> Display X
- Model Z -> negated Display Y

### Key Position Data Structures

| Arcade Field | N64 Equivalent | Description |
|--------------|----------------|-------------|
| `RWR[3]` | `gCarPosition` (0x80152848) | Real world position |
| `UV[3][3]` | `gCarOrientation` | Rotation matrix |
| `TIRERWR[4][3]` | `gTirePositionWorld` (0x80152918) | Tire world positions |
| `GRW[3]` | `gGravityWorld` (0x80152AFC) | Gravity vector in world |

### World Bounds

```c
/* N64 symbol addresses */
gWorldBoundsMin = 0x8015B000;  /* type:data world bounds min (f32[3]) */
gWorldBoundsMax = 0x8015B00C;  /* type:data world bounds max (f32[3]) */
```

The world bounds define the playable area. Objects outside these bounds are culled or wrapped.

---

## 2. Environment Objects

### Arcade World Object System (world.c)

The arcade uses a hierarchical object tree for world geometry:

```c
/* World Object structure (world.h) */
typedef struct {
    char name[16];          /* Object name */
    F32 matrix[3][3];       /* Rotation matrix */
    F32 objpos[3];          /* Position in parent's frame */
    S32 dlflags;            /* Display list flags */
    S16 next;               /* Next sibling index */
    S16 child;              /* First child index */
    S16 sfx1;               /* Special effects flag 1 */
    S16 sfx2;               /* Special effects flag 2 */
} WOBJ;

/* World header for lookup */
typedef struct {
    U32 woff;               /* Offset from 0 */
    char wname[16];         /* World name */
} WORLDHDR;
```

**Loading process:**
1. `FindWorld()` - Binary search for world by name
2. `NewWorldObject()` - Recursively create display list objects
3. `LoadWorld()` - Main entry point, also calls `modelinit()` for road surface

### N64 World Loading

```c
/* N64 equivalent functions (symbol_addrs.us.txt) */
world_load_segment     = 0x800E8000;  /* Load world segment from ROM */
world_unload_segment   = 0x800E8200;  /* Unload world segment */
world_visibility_update = 0x800E8400; /* Update visible sectors */
world_lod_update       = 0x800E8600;  /* Update LOD levels */
world_stream_update    = 0x800E8800;  /* Update streaming state */
```

### World Object Attributes

| Flag | Value | Description |
|------|-------|-------------|
| `ZATR_SORTALPHA` | - | Enable alpha sorting |
| `ZATR_FRONTFACE` | - | Billboard toward camera |

---

## 3. Weather/Fog Effects

### Arcade Fog System (targets.fog.c)

The arcade implements a sophisticated fog system with multiple modes:

```c
/* Fog modes (targets.h) */
#define FOG_INIT       0    /* Initialization state */
#define FOG_OFF        1    /* Fog disabled */
#define FOG_TINT       2    /* Constant tint fog */
#define FOG_TRACK      3    /* Track-specific distance fog */
#define FOG_TRACK_TINT 4    /* Combined tint and distance */
```

**Per-track fog parameters:**
```c
extern U32 dbgFogColor[MAX_TRACKS];      /* RGBA fog color */
extern F32 dbgFogMinDist[MAX_TRACKS];    /* Fog start distance */
extern F32 dbgFogMaxDist[MAX_TRACKS];    /* Full fog distance */
extern F32 dbgFogMaxLevel[MAX_TRACKS];   /* Maximum fog density (0-1) */
```

**Core fog functions:**
```c
void SetFog(S32 mode, U32 density, U32 color);  /* Set fog parameters */
void UpdateFog(void);                            /* Apply fog to 3dfx */
void HandleFog(void);                            /* Per-frame fog update */
void MakeFogRamp(U32 color, F32 mindist, F32 maxdist, F32 minfog, F32 maxfog);
void StaticFog(F32 mindist, F32 maxdist, F32 maxfog, U32 color);
void ConstantFog(U32 density, U32 color);
```

### N64 Fog/Weather Functions

```c
/* From symbol_addrs.us.txt */
weather_select       = 0x800D2CDC;  /* Sets weather conditions (240 bytes) */
time_of_day_select   = 0x800D2DCC;  /* Sets lighting conditions (200 bytes) */
```

**Arcade to N64 mapping:**
| Arcade Function | N64 Function | Address |
|-----------------|--------------|---------|
| `SetFog()` | `weather_select` | 0x800D2CDC |
| `FogTrack()` | `time_of_day_select` | 0x800D2DCC |
| `UpdateFog()` | (inline in render loop) | - |

### Fog Zone Targets

Fog can be tied to world regions using `TARG_FOG` targets:

```c
/* Fog target in targets.c */
static void TargetFog(S32 mbidx, S32 dlidx, OBJANIM *oanim) {
    Target *t = AddToAnimList(oanim, mbidx, dlidx);

    /* odata[0] = fog group */
    /* odata[1] = color/max encoded: color*1000 + max */
    t->type = t->otype = -1;
    t->func = nil;  /* Not interactive */
}
```

The `gFogGroup` global tracks which fog zone the player occupies, enabling localized fog effects (e.g., tunnels).

### Tunnel Fog

Special handling for tunnel areas:

```c
/* globals.h */
S8 in_tunnel;  /* 1=enter tunnel, -1=exit tunnel, 0=hold */
```

When `in_tunnel` is set, black fog is applied to simulate darkness:
- Distance-based density calculation
- Color changes from white (outdoor) to black (tunnel)

---

## 4. Skybox Rendering

### N64 Skybox System

The N64 renders skyboxes as part of the main rendering pipeline:

```c
/* From rendering_pipeline.md frame flow */
render_scene() {
    camera_viewport_setup();  /* Stage 1: Setup */
    skybox_render();          /* Stage 2: Background */
    track_render();           /* Stage 3: Track Geometry */
    /* ... */
}
```

Skybox rendering occurs after viewport setup but before track geometry, using the far Z-buffer to place it at maximum distance.

### Arcade Skybox

The arcade version uses the 3dfx depth buffer similarly. The skybox is rendered with:
- Maximum depth (always behind all geometry)
- No fog applied (to prevent color blending artifacts)
- Fixed orientation relative to world (not camera-relative)

---

## 5. Lighting Model

### Arcade Lighting (ZOID Library)

The arcade uses the ZOID rendering library for lighting:

```c
/* Lighting setup call (referenced in symbol_addrs.us.txt comment) */
ZOID_SetLightDir([0.577, -0.577, 0.577], 1);  /* Normalized sun direction */
```

**Light properties:**
- Single directional light (sun)
- Per-vertex lighting calculated during model rendering
- Ambient + diffuse model

### N64 Lighting Functions

```c
/* From symbol_addrs.us.txt */
lighting_setup      = 0x8009C5BC;  /* Lighting setup helper */
lighting_calc       = 0x8009C5E0;  /* Surface lighting calculation (1560 bytes) */
track_lighting_setup = 0x800F5EF8; /* Track lighting setup (588 bytes) */
```

### Track Ambient Color

```c
gTrackAmbientColor = 0x8003794C;  /* Track ambient lighting color (u32) */
gTrackWeather      = 0x80037954;  /* Weather setting (0=clear, 1=rain) */
```

### Per-Car Lighting

Cars have individual lighting applied:
- `car_lights_render` (0x800A6094) - Headlight/taillight rendering
- `brake_light_update` (0x800A6244) - Brake light state updates

### Reflections

The arcade implements car body reflections:

```c
/* From visuals.c */
/* Window/body reflections change based on in_tunnel flag */
/* Reflection textures swap for tunnel vs outdoor */
```

---

## 6. Dynamic Objects (Traffic, Obstacles)

### Target System Overview

The arcade uses a comprehensive "target" system for all interactive objects:

```c
/* Target types (targets.h) */
enum TargetType {
    TARG_NONE = 0,
    TARG_CONE,        /* Traffic cone - flies when hit */
    TARG_METER,       /* Parking meter - knockdown */
    TARG_FENCE,       /* Breakaway fence */
    TARG_POLE,        /* Traffic/street light poles */
    TARG_TREE,        /* Trees (5 types: A-E) */
    TARG_CROWD,       /* Animated crowd (3 types) */
    TARG_WINDOWA,     /* Breakable window type A */
    TARG_WINDOWB,     /* Breakable window type B */
    TARG_FOG,         /* Fog zone trigger */
    TARG_FRONTFACE,   /* Billboard object */
    TARG_CABLECAR,    /* Cable car ambient sound */
    TARG_SPEAKER,     /* Speaker ambient sound */
    TARG_FIRECRACKER, /* Firecracker ambient sound */
    TARG_SMALLHOOT,   /* Small crowd cheer zone */
    TARG_SMALLCLAP,   /* Small crowd clap zone */
    TARG_MARKER,      /* Trigger marker for grouped targets */
    TARG_BIGCHEER,    /* Large crowd cheer zone */
    TARG_BIGCHEER2,   /* Large crowd cheer zone (alt) */
    TARG_GONG,        /* Gong ambient sound */
    TARG_BUSH,        /* Bush collision */
    TARG_FINISH,      /* Finish line marker */
    TARG_TIME,        /* Time bonus pickup */
    TARG_FLAG,        /* Animated flag */
    TARG_SIREN,       /* Siren ambient sound */
    NUM_TARGTYPES
};
```

### Target Structure

```c
typedef struct Target {
    struct Target  *next;       /* Next target in group */
    OBJANIM        *oanim;      /* Animation data */
    S32            objnum;      /* Display object number */
    F32            pos[3];      /* World position */
    F32            uv[3][3];    /* Orientation matrix */
    F32            vel[3];      /* Linear velocities (physics) */
    F32            angV[3];     /* Angular velocities */
    S16            data;        /* Type-specific data */
    S16            type;        /* Object type index */
    F32            radius;      /* Collision radius */
    void           (*func)();   /* Collision callback */
    VisFunc        *VisFunc;    /* Animation controller */
    BOOL           active;      /* Collision active flag */
    char           name[15];    /* Debug name */
    S16            otype;       /* Original type (for reset) */
    S16            frames;      /* Animation frame count */
} Target;
```

### Target Groups

Targets are organized into spatial groups for efficient collision testing:

```c
typedef struct TargetGroup {
    S16     type;           /* Group type */
    S16     count;          /* Number of targets */
    F32     radius;         /* Bounding radius */
    F32     dist;           /* Distance from camera */
    F32     pos[3];         /* Group center */
    F32     min[3], max[3]; /* Bounding box */
    Target  *list;          /* Target linked list */
    Target  *last;          /* List tail */
} TargetGroup;

#define MAX_TARG_GROUPS 30
#define MAX_TARGETS     300
```

### N64 Target/Object Functions

```c
/* From symbol_addrs.us.txt */
world_collision          = 0x80098FC0;  /* World/environment collision */
world_object_destroy     = 0x800EB690;  /* World object destroy (396 bytes) */
world_physics_tick       = 0x800EC2F8;  /* World physics tick (1584 bytes) */
world_collision_detect   = 0x800EC928;  /* Bounding sphere check (588 bytes) */
world_bounds_check       = 0x800ECB74;  /* Position bounds check (164 bytes) */
world_gravity_apply      = 0x800ECC18;  /* Gravity application (2652 bytes) */
world_friction_apply     = 0x800ED674;  /* Drag/friction (400 bytes) */
world_velocity_integrate = 0x800ED804;  /* Euler integration (712 bytes) */
world_collision_response = 0x800EDACC;  /* Impulse collision (540 bytes) */
world_effect_update      = 0x800EE8B4;  /* Particle position/lifetime (456 bytes) */
world_trigger_check      = 0x800EDCE8;  /* Trigger volume check (2292 bytes) */
world_trigger_activate   = 0x800EE5DC;  /* Activate checkpoint/boost (580 bytes) */
```

### Collision Detection

Arcade uses a two-phase collision approach:

1. **Group-level test**: Check car bounding sphere against group sphere
2. **Target-level test**: If group overlaps, test individual targets

```c
/* Arcade collision test (targets.c) */
static BOOL OverlapTarget(S16 slot, F32 pos[3], F32 radius, F32 *dist) {
    F32 dsq, gap, vec[3];
    CAR_DATA *gc = &game_car[slot];

    if (model[slot].collidable == false)
        return false;

    vecsub(gc->RWR, pos, vec);
    dsq = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
    radius += CAR_RADIUS;  /* CAR_RADIUS = 11.2 ft */
    gap = dsq - radius*radius;

    if (dist) *dist = gap;
    return (gap <= 0.0);
}
```

### Knockdown Physics

When objects are struck, they use simple physics:

```c
/* Cone animation example (targets.c) */
static void AnimateCone(Visual *v, S16 op) {
    static const F32 gravity[3] = {0.0, -0.5, +0.25};
    F32 dt = 0.15;

    /* Euler integration */
    for (i = 0; i < 3; ++i) {
        t->vel[i] += gravity[i] * dt;
        t->pos[i] += t->vel[i] + gravity[i] * dt;
    }

    /* Tumble rotation */
    RotateUV(t->angV, t->uv);
}
```

---

## 7. N64 vs Arcade Differences

### Rendering API

| Feature | Arcade (3dfx Glide) | N64 (RDP/RSP) |
|---------|---------------------|---------------|
| Fog | `grFogTable()`, `grFogMode()` | RDP fog registers |
| Objects | Mathbox display lists | F3DEX2 display lists |
| Textures | 256x256, TMU-based | 64x64 max, TMEM |
| Lighting | Per-vertex in Mathbox | RSP microcode |
| Alpha | Hardware sorting | Software sorting |

### Memory Constraints

| Aspect | Arcade | N64 |
|--------|--------|-----|
| World buffer | 128KB (`WORLD_BSIZE`) | Reduced, streaming |
| Max targets | 300 | Reduced |
| Max target groups | 30 | Reduced |
| Fog table | 64 entries (`GR_FOG_TABLE_SIZE`) | RDP-specific |

### Visual Effects

| Effect | Arcade | N64 Equivalent |
|--------|--------|----------------|
| Skid marks | Per-tire polygons | `tire_skid_mark` (0x800A4E60) |
| Sparks | Billboard particles | `engine_particle_effect` (0x800A5588) |
| Smoke | Animated sprites | `exhaust_smoke_effect` (0x800A5744) |
| Shadows | Ground projection | `car_shadow_render` (0x800A5D34) |
| Reflections | Cube map textures | Simplified/removed |

### Simplifications on N64

1. **Fewer destructible objects** - Memory constraints limit target count
2. **Simpler fog model** - No per-pixel fog table
3. **Reduced particle counts** - Smoke, sparks limited
4. **No car body reflections** - TMU limitations
5. **Simplified crowd animations** - Fewer frames

---

## Function Mapping Reference

### World Loading

| Arcade Function | N64 Function | Address |
|-----------------|--------------|---------|
| `LoadWorld()` | `world_load_segment` | 0x800E8000 |
| `NewWorldObject()` | (inline) | - |
| `FindWorld()` | (table lookup) | - |

### Collision

| Arcade Function | N64 Function | Address |
|-----------------|--------------|---------|
| `TargetCheck()` | `world_trigger_check` | 0x800EDCE8 |
| `OverlapTarget()` | `world_collision_detect` | 0x800EC928 |
| `TargetAppearance()` | `world_trigger_activate` | 0x800EE5DC |

### Fog/Weather

| Arcade Function | N64 Function | Address |
|-----------------|--------------|---------|
| `SetFog()` | `weather_select` | 0x800D2CDC |
| `FogTrack()` | `time_of_day_select` | 0x800D2DCC |
| `HandleFog()` | (inline in render) | - |

### Visual Effects

| Arcade Function | N64 Function | Address |
|-----------------|--------------|---------|
| `UpdateVisuals()` | `visual_objects_update` | 0x800B55FC |
| `UpdateEnvirons()` | `visual_effects_update` | 0x800B811C |
| `DoSkid()` | `tire_skid_mark` | 0x800A4E60 |
| `StartSmoke()` | `exhaust_smoke_effect` | 0x800A5744 |

### Drone/AI

| Arcade Function | N64 Function | Address |
|-----------------|--------------|---------|
| `DoDrones()` | `DoDrones` | 0x800A0000 |
| `InitDrones()` | `InitDrones` | 0x8009FF00 |
| `avoid_areas()` | `avoid_areas` | 0x800A1800 |

---

## Key Data Structures Reference

### Global Variables

| Name | N64 Address | Description |
|------|-------------|-------------|
| `gWorldBoundsMin` | 0x8015B000 | World minimum bounds (f32[3]) |
| `gWorldBoundsMax` | 0x8015B00C | World maximum bounds (f32[3]) |
| `gCameraPosition` | 0x80036640 | Camera world position (f32[3]) |
| `gCarPosition` | 0x80152848 | Car position vector (f32[3]) |
| `gTirePositionWorld` | 0x80152918 | Tire world positions (f32[4][3]) |
| `gGravityWorld` | 0x80152AFC | Gravity vector (f32[3]) |
| `gTrackAmbientColor` | 0x8003794C | Track ambient color (u32) |
| `gTrackWeather` | 0x80037954 | Weather setting (u32) |

---

## See Also

- [Track Geometry Documentation](track_geometry.md) - Surface tree and road data
- [Rendering Pipeline Documentation](rendering_pipeline.md) - Full render sequence
- [Collision System Documentation](collision_system.md) - Physics collision
- [AI Drone System](ai_drone_system.md) - Drone path following
- [Effects/Particle System](particle_effects.md) - Visual effects

---

## Revision History

| Date | Author | Changes |
|------|--------|---------|
| 2025-12-31 | Claude | Initial documentation from arcade source analysis |
