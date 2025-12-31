# Rush 2049 N64 Model/Object Rendering System

This document describes the model and object rendering system used in San Francisco Rush 2049 for N64, with comparisons to the arcade (Rush The Rock) implementation.

## Table of Contents

1. [Model Data Format](#1-model-data-format)
2. [Object Transform Hierarchy](#2-object-transform-hierarchy)
3. [LOD (Level of Detail) Switching](#3-lod-level-of-detail-switching)
4. [Culling and Visibility](#4-culling-and-visibility)
5. [Animation System](#5-animation-system)
6. [N64 vs Arcade Differences](#6-n64-vs-arcade-differences-f3dex2-vs-3dfx)
7. [Function Mapping](#7-arcade-to-n64-function-mapping)

---

## 1. Model Data Format

### Arcade Model Structure (3dfx/ZOID)

The arcade version uses the ZOID/Mathbox rendering system with these key structures from `MB/mb_struct.h`:

```c
/* ROM Object Definition */
typedef struct {
    F32     InvRad;         /* Invisibility radius */
    F32     BndRad;         /* Bounding radius for culling */
    S32     LodCount;       /* Number of LOD levels (max 4) */
    ROMLOD  LOD[4];         /* LOD data for each detail level */
} RomObject, ROMOBJECT;

/* LOD Data */
typedef struct {
    F32     LodRad;         /* Switching radius for this LOD */
    U32     Flags;          /* LOD_ALPHA, LOD_NORMALS */
    S32     VertCount;      /* Number of vertices */
    S32     VertIndex;      /* Vertex pool index */
    S32     TriCount;       /* Number of triangles */
    S32     TriIndex;       /* Triangle index */
    S32     NormIndex;      /* Normal index */
} RomLOD, ROMLOD;

/* Vertex Data */
typedef struct {
    Vec3    Pos;            /* Position [3] floats */
    F32     S, T;           /* Texture coordinates */
    S16     PrevPos;        /* Animation reference */
    U8      Shade;          /* Vertex shading */
    U8      Extra;          /* Additional data */
} RomVert;
```

### N64 Model Structure (F3DEX2)

The N64 version uses display lists compatible with the F3DEX2 microcode:

```c
/* N64 Model Layout (from game.c analysis) */
struct N64Model {
    s32     numVerts;       /* Vertex count at offset 0x00 */
    s32     numTris;        /* Triangle count at offset 0x04 */
    void    *vertData;      /* Pointer to Vtx array at 0x08 */
    void    *triData;       /* Display list for triangles at 0x0C */
};

/* N64 Vertex Format (Vtx structure) */
/* Standard libultra Vtx format:
 *   Position: s16 x, y, z
 *   Texture:  s16 s, t
 *   Color/Normal: u8 r, g, b, a or nx, ny, nz, a
 */
```

### Key Differences

| Feature | Arcade (3dfx) | N64 (F3DEX2) |
|---------|---------------|--------------|
| Vertex Format | F32 floats | S16 fixed-point |
| Texture Coords | F32 S,T | S10.5 fixed |
| Matrix Format | 4x3 float | 4x4 Mtx (s15.16) |
| Draw Calls | Direct API | Display lists |
| Max Vertices | 2048 per object | 32 per batch |

---

## 2. Object Transform Hierarchy

### Arcade Object Instance (OBJINSTANCE)

From `MB/mb_objects.h`:

```c
typedef struct {
    S32     color;          /* Per-object color override */
    U32     flags;          /* ZATR_* visibility/render flags */
    F32     mat[4][3];      /* 4x3 transformation matrix */
    S32     zmod;           /* Z-buffer modifier */
    S32     index;          /* Index into ROM object data */
    S16     parent;         /* Parent object index (-1 = none) */
    S16     child;          /* Child object index (-1 = none) */
    S16     dopolytype;     /* Polygon type to render */
    U08     xlucency;       /* Alpha blend level */
    U08     ambientsub;     /* Ambient reduction */
    F32     reflect_speed;  /* Reflection animation speed */
    F32     sort_offset;    /* Sorting bias */
    S16     next[MBNUMWINDOWS]; /* Per-window linked list */
} OBJINSTANCE;
```

### N64 Matrix Operations

The N64 uses the RSP matrix stack via display list commands:

```c
/* Matrix load to RSP (from game.c:8173) */
void matrix_load_rsp(void *matrix) {
    u32 *dlPtr = *(u32 **)0x80149438;
    *(u32 **)0x80149438 = dlPtr + 2;

    /* G_MTX command: 0xDA380003 = load, no push */
    dlPtr[0] = 0xDA380003;
    dlPtr[1] = (u32)matrix;
}

/* Matrix push (game.c:8189) */
void matrix_push_stack(void *matrix) {
    /* G_MTX with push: 0xDA380001 */
    dlPtr[0] = 0xDA380001;
    dlPtr[1] = (u32)matrix;
}

/* Matrix pop (game.c:8205) */
void matrix_pop_stack(void) {
    /* G_POPMTX: 0xD8380040 */
    dlPtr[0] = 0xD8380040;
    dlPtr[1] = 0x00000040;
}
```

### Car Parts Hierarchy (Arcade)

From `visuals.c:CreateCar()`:

```c
/* Car is composed of hierarchical parts attached to frame */
typedef struct CarParts {
    S32 frame;              /* Root transform - body */
    S32 hood;               /* Child of frame */
    S32 brakes;             /* Child of frame */
    S32 blast;              /* Explosion effect */
    S32 window[4];          /* 4 window sections */
    S32 tires[4];           /* 4 wheel models */
    S32 quad[5];            /* Body damage panels */
    S32 smoke[SMOKE_OBJS];  /* Particle effects */
    S8  nextSmoke;
    S32 lSpark, rSpark, bSpark; /* Spark effects */
} CarParts;
```

The N64 replicates this structure with display list calls building the same hierarchy.

---

## 3. LOD (Level of Detail) Switching

### Arcade LOD System

From `mb_struct.h`, each object can have up to 4 LOD levels:

```c
/* LOD switching based on LodRad radius */
typedef struct {
    F32     LodRad;         /* Distance threshold for this LOD */
    /* ... geometry data ... */
} ROMLOD;
```

The arcade flags `ZATR_LOD` (bits 0-1) store the current LOD level per object.

### N64 LOD Implementation

From `game.c`, the N64 implements similar LOD switching:

#### Entity LOD Selection (`entity_lod_select` at 0x80096734)

```c
void entity_lod_select(void *entity, f32 distance) {
    s32 *lodLevel = (s32 *)((u8 *)entity + 0xF0);
    s32 *modelIndex = (s32 *)((u8 *)entity + 0xF4);
    s32 baseModel = *((s32 *)((u8 *)entity + 0xF8));

    /* LOD distance thresholds */
    f32 lodDist0 = 100.0f;   /* High detail */
    f32 lodDist1 = 300.0f;   /* Medium detail */
    f32 lodDist2 = 600.0f;   /* Low detail */

    if (distance < lodDist0) {
        *lodLevel = 0;
        *modelIndex = baseModel;
    } else if (distance < lodDist1) {
        *lodLevel = 1;
        *modelIndex = baseModel + 1;
    } else if (distance < lodDist2) {
        *lodLevel = 2;
        *modelIndex = baseModel + 2;
    } else {
        *lodLevel = 3;
        *modelIndex = baseModel + 3;  /* Billboard/sprite */
    }
}
```

#### Car-Specific LOD (`car_lod_select` at 0x800A4508)

Cars use tighter thresholds for better visual quality:

```c
void car_lod_select(void *car, f32 distance) {
    s32 *currentLod = (s32 *)((u8 *)car + 0x180);
    void **lodModels = (void **)((u8 *)car + 0x184);

    /* Car LOD thresholds (tighter than entities) */
    f32 lodDistances[4] = {
        20.0f,   /* LOD 0: High detail */
        50.0f,   /* LOD 1: Medium detail */
        100.0f,  /* LOD 2: Low detail */
        200.0f   /* LOD 3: Minimal/billboard */
    };

    /* Select LOD and update model pointer */
    /* ... */
    *(void **)((u8 *)car + 0x194) = lodModels[newLod];
}
```

#### Track LOD (`track_lod_select` at 0x800F42D0)

Track geometry uses distance-based LOD:

```c
s32 track_lod_select(f32 distance) {
    if (distance < 200.0f) return 0;   /* Full detail */
    if (distance < 500.0f) return 1;   /* Medium */
    if (distance < 1000.0f) return 2;  /* Low */
    return 3;                          /* Minimal */
}
```

### LOD Comparison Table

| Object Type | LOD 0 | LOD 1 | LOD 2 | LOD 3 |
|-------------|-------|-------|-------|-------|
| Car | <20ft | <50ft | <100ft | <200ft |
| Entity | <100ft | <300ft | <600ft | >600ft |
| Track | <200ft | <500ft | <1000ft | >1000ft |

---

## 4. Culling and Visibility

### Arcade Visibility Flags

From `mb_objects.h`:

```c
#define ZATR_OFF        0x0100  /* Don't show this object */
#define ZATR_ALL_OFF    0x0200  /* Don't show object or children */
#define ZATR_NOZCOMP    0x0400  /* Skip Z compare */
#define ZATR_NOZWRITE   0x0800  /* Skip Z write */
#define ZATR_VIS        0x0004  /* Object is visible (set by mathbox) */
```

### N64 Frustum Culling

#### Camera Frustum Extraction (`camera_frustum_extract` at 0x800C14F4)

```c
void camera_frustum_extract(void *camera, f32 *frustum) {
    /* Extract 6 frustum planes as plane equations (nx, ny, nz, d) */

    /* Left plane (indices 0-3) */
    frustum[0] = camRight[0] + camLook[0] * nearW;
    frustum[3] = -(frustum[0] * camPos[0] + ...);

    /* Right plane (indices 4-7) */
    /* Top plane (indices 8-11) */
    /* Bottom plane (indices 12-15) */
    /* Near plane (indices 16-19) */
    /* Far plane (indices 20-23) */
}
```

#### Entity Cull Check (`entity_cull_check` at 0x80096A00)

```c
s32 entity_cull_check(void *entity, void *camera) {
    f32 *entityPos = (f32 *)((u8 *)entity + 0x24);
    f32 entityRadius = *((f32 *)((u8 *)entity + 0x5C));

    /* Distance culling */
    f32 nearPlane = 1.0f;
    f32 farPlane = 2000.0f;

    if (dist - entityRadius > farPlane) return 1;  /* Cull: too far */
    if (dist + entityRadius < nearPlane) return 1; /* Cull: behind */

    /* Cone culling using dot product */
    f32 dotProduct = dx * cameraDir[0] + dy * cameraDir[1] + dz * cameraDir[2];
    if (dotProduct < -0.2f) return 1;  /* Cull: behind camera */

    return 0;  /* Visible */
}
```

### Culling Globals

```c
gCulledObjects = 0x80037990;       /* Count of culled objects */
gVisibleObjectCount = 0x80036748;  /* Count of visible objects */
```

---

## 5. Animation System

### Arcade Animation Structure

From `mb_struct.h`:

```c
typedef struct {
    S16     type;       /* Animation type */
    S16     id;         /* Polygon/vertex ID */
    S16     lod;        /* Which LOD to apply to */
    S16     fid;        /* Function ID */
    S16     odata[2];   /* Object-specific data */
    S16     udata[2];   /* User-defined data */
} AnimEntry, OBJANIM;
```

### Visual Effect Types (Arcade)

From `visuals.h`:

```c
enum VisualTypes {
    VIS_BLAST = 0,   /* Explosion effect */
    VIS_BRAKES,      /* Brake lights */
    VIS_LSPARK,      /* Left spark */
    VIS_RSPARK,      /* Right spark */
    VIS_BSPARK,      /* Back spark */
    VIS_TIRE1-4,     /* Wheel effects */
    VIS_SHADOW,      /* Car shadow */
    VIS_QUAD0-4,     /* Damage panels */
    VIS_WINDOW0-3,   /* Window reflections */
    VIS_FRAME,       /* Main body */
    NUM_VISUALS
};
```

### N64 Visual System

Key N64 visual update functions:

| Address | Function | Description |
|---------|----------|-------------|
| 0x800B811C | `visual_effects_update` | Main visual effects loop |
| 0x800B55FC | `visual_objects_update` | Per-car visual updates |
| 0x800A4940 | `wheel_rotation_update` | Wheel spin animation |
| 0x800A5588 | `engine_particle_effect` | Engine particles |
| 0x800A5744 | `exhaust_smoke_effect` | Exhaust smoke |
| 0x800A5D34 | `car_shadow_render` | Dynamic shadows |
| 0x800A6094 | `car_lights_render` | Headlights/taillights |

### Wheel Animation (N64)

```c
/* wheel_rotation_update at 0x800A4940 */
void wheel_rotation_update(void *wheel, f32 dt) {
    f32 *rotAngle = (f32 *)((u8 *)wheel + 0x00);
    f32 *angVel = (f32 *)((u8 *)wheel + 0x04);

    /* Update rotation angle for visual */
    f32 newAngle = *rotAngle + (*angVel * dt);

    /* Keep angle in 0-2pi range */
    while (newAngle > 6.283185f) {
        newAngle -= 6.283185f;
    }
    *rotAngle = newAngle;
}
```

---

## 6. N64 vs Arcade Differences (F3DEX2 vs 3dfx)

### Rendering API Comparison

| Feature | 3dfx (Arcade) | F3DEX2 (N64) |
|---------|---------------|--------------|
| **API Type** | Direct function calls | Display list commands |
| **Vertex Limit** | 2048 per object | 32 per RSP batch |
| **Texture Size** | Up to 256x256 | 64x64 typical (TMEM limited) |
| **Color Depth** | 16-bit or 24-bit | 16-bit (RGBA5551) |
| **Z-Buffer** | Per-pixel, float | 16-bit fixed point |
| **Fog** | Per-pixel table | Linear per-vertex |
| **Reflections** | Real-time environment map | Pre-baked or simplified |
| **Translucency** | Full alpha blending | Limited blend modes |

### Display List Commands Used

The N64 version emits these RDP/RSP commands:

```c
/* From game.c analysis */

/* Geometry Setup */
gSPMatrix(gfx, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_PUSH);
gSPVertex(gfx, vtx, n, v0);
gSP2Triangles(gfx, v0, v1, v2, 0, v3, v4, v5, 0);
gSPDisplayList(gfx, subdl);
gSPPopMatrix(gfx, G_MTX_MODELVIEW);

/* Render Mode */
gDPSetRenderMode(gfx, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
gDPSetRenderMode(gfx, G_RM_XLU_SURF, G_RM_XLU_SURF2);

/* Texturing */
gDPSetTextureImage(gfx, fmt, size, width, addr);
gDPLoadBlock(gfx, tile, uls, ult, lrs, dxt);
gDPSetTile(gfx, fmt, size, line, tmem, tile, ...);

/* Combine/Blend */
gDPSetPrimColor(gfx, m, l, r, g, b, a);
gDPSetCombineMode(gfx, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

/* Fog */
gDPSetFogColor(gfx, r, g, b, a);
gSPFogPosition(gfx, min, max);
```

### Raw Command Values

From `game.c` low-level analysis:

| Command | Hex Value | Description |
|---------|-----------|-------------|
| G_MTX (load) | 0xDA380003 | Load matrix, no push |
| G_MTX (push) | 0xDA380001 | Load matrix with push |
| G_POPMTX | 0xD8380040 | Pop matrix stack |
| G_VTX | 0x01xxxxxx | Load vertices |
| G_DL | 0x06000000 | Call display list |
| G_SETGEOMODE | 0xB7000000 | Set geometry mode |

### Reflection Handling Differences

**Arcade**: Uses `ZATR_REFLECT` flag with dynamic environment mapping:

```c
ZOID_SetReflect(car->frame, bod_rtex, speed);
ZOID_SetReflect(car->window[j], win_rtex, speed);
```

**N64**: Uses simplified pre-baked reflections or omits them entirely due to TMEM constraints.

### Visual Quality Trade-offs

| Effect | Arcade | N64 |
|--------|--------|-----|
| Car Reflections | Dynamic env map | Static or none |
| Shadow Quality | Soft-edge blur | Hard-edge sprite |
| Particle Count | High | Reduced |
| Skid Marks | Extended trails | Limited count |
| Fog | Per-pixel table | Linear vertex fog |

---

## 7. Arcade to N64 Function Mapping

### Rendering Core

| Arcade Function | N64 Address | N64 Name |
|-----------------|-------------|----------|
| MBOX_ShowObject | - | (display list emit) |
| MBOX_HideObject | - | (skip in DL) |
| ZOID_SetObjectDef | - | model pointer set |
| - | 0x80099BFC | render_object (10KB) |
| - | 0x80087A08 | object_render (10KB) |
| - | 0x800A04C4 | render_scene (2.7KB) |

### Visual Effects

| Arcade Function (visuals.c) | N64 Address | N64 Name |
|-----------------------------|-------------|----------|
| InitVisuals | - | (during init) |
| UpdateEnvirons | 0x800B811C | visual_effects_update |
| CreateCar | - | (part of car_init) |
| SetupCar | - | (integrated) |
| StartCar | - | (integrated) |
| PickCar | 0x800A6404 | car_render_full |
| DoSkid | 0x800A4E60 | tire_skid_mark |
| StartSmoke | 0x800A5744 | exhaust_smoke_effect |
| AnimateShadow | 0x800A5D34 | car_shadow_render |

### LOD and Culling

| Arcade Function | N64 Address | N64 Name |
|-----------------|-------------|----------|
| (in ZOID LOD calc) | 0x80096734 | entity_lod_select |
| (in ZOID LOD calc) | 0x800A4508 | car_lod_select |
| (in ZOID LOD calc) | 0x800F42D0 | lod_select |
| (in mathbox) | 0x80096A00 | entity_cull_check |
| - | 0x800C14F4 | camera_frustum_extract |
| - | 0x800F6144 | track_culling_update |

### Target/Prop System

| Arcade Function (targets.c) | N64 Address | N64 Name |
|-----------------------------|-------------|----------|
| SetupTargets | - | (track load) |
| SetTargets | 0x800F6AB8 | props_render |
| OverlapTarget | 0x800EC928 | world_collision_detect |
| TargetCheck | 0x800EC928 | target_collision_check |

### Matrix Operations

| Arcade Function | N64 Address | N64 Name |
|-----------------|-------------|----------|
| ZOID_SetMatrix | 0x80099C00 | matrix_load_rsp |
| (matrix push) | 0x80098620 | matrix_push_stack |
| (matrix pop) | - | matrix_pop_stack |
| guMtxF2L | 0x800091E0 | guMtxF2L |
| guMtxIdentF | 0x80009394 | guMtxIdentF |

---

## Data Address References

### Render System Globals

```c
/* Display List Pointer */
gDisplayListPtr = 0x80149438;      /* Current DL write position */
gRenderDataPtr = 0x8011EA18;       /* Render data base */

/* Sync Objects */
gRenderSync = 0x801427A8;          /* Render sync semaphore */

/* Object Tracking */
gObjectMaxCount = 0x801569A8;      /* Max active objects */
gObjectSlotTable = 0x801569B8;     /* Object slot array */

/* Visual Effects */
gVisualsPool = 0x80150C20;         /* Visual effect pool */
gVisualsListHead = 0x80153000;     /* Active visuals list */
gVisualsFreeHead = 0x80153004;     /* Free visual slots */
gCarPartsArray = 0x80153100;       /* Per-car visual parts */

/* LOD State */
gTextureLodEnabled = 0x80036714;   /* LOD enable flag */
gTrackLodLevel = 0x80037944;       /* Current track LOD */

/* Culling State */
gCulledObjects = 0x80037990;       /* Culled object count */
gVisibleObjectCount = 0x80036748;  /* Visible after cull */
```

---

## Summary

The Rush 2049 N64 rendering system adapts the arcade's ZOID/Mathbox architecture to the N64's F3DEX2 microcode constraints. Key adaptations include:

1. **Display Lists**: All rendering converted from direct API calls to RSP display lists
2. **Vertex Batching**: Large meshes split into 32-vertex batches for RSP processing
3. **LOD Simplification**: Distance-based model switching with 3-4 detail levels
4. **Frustum Culling**: Bounding sphere tests against camera frustum planes
5. **Visual Effects**: Particle counts reduced, reflections simplified or removed
6. **Matrix Stack**: Uses F3DEX2 matrix stack (4 levels) for hierarchical transforms

The arcade source in `reference/repos/rushtherock/` provides direct insight into the original algorithms, which are then adapted for N64 hardware limitations.
