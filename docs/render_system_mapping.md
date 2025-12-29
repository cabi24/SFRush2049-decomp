# Render System Mapping: N64 RDP/RSP vs Arcade 3dfx

## Overview

This document maps the N64 Rush 2049 rendering system (RDP/RSP with F3DEX2 microcode) to the arcade Rush The Rock rendering system (3dfx Voodoo/Glide API).

### Architecture Comparison

| Aspect | N64 (RDP/RSP) | Arcade (3dfx Voodoo) |
|--------|---------------|----------------------|
| **GPU Architecture** | Fixed-function pipeline via microcode | Fixed-function TMU + FBI |
| **Command Interface** | Display lists (64-bit Gfx commands) | Direct MMIO register writes |
| **Triangle Setup** | RSP microcode (F3DEX2) | Glide API / SST registers |
| **Texture Units** | 1 TMU (shared RDP tile memory) | 1-3 TMUs (dedicated SDRAM) |
| **Framebuffer** | RDRAM (shared with CPU) | Dedicated FBI memory |
| **Max Resolution** | 320x240 (typical) to 640x480 | 640x480 to 800x600 |
| **Color Depth** | 16-bit RGBA5551 / 32-bit RGBA8888 | 16-bit RGB565 |
| **Z-Buffer** | 16-bit in RDRAM | 16-bit dedicated |

## Key N64 Files

| File | Description | Lines |
|------|-------------|-------|
| `src/game/gfx.c` | Display list management | 39 |
| `src/game/display.c` | Viewport/display setup | 348 |
| `src/game/render.c` | 3D object rendering stubs | 54 |
| `src/game/game.c` | Main rendering functions | ~9400 |

## Key Arcade Files

| File | Description |
|------|-------------|
| `game/visuals.c` | Visual effects, car rendering, shadows |
| `GUTS/3dfx/glide-rush2.7/csrc/gdraw.c` | Triangle drawing, vertex setup |
| `GUTS/3dfx/glide-rush2.7/csrc/gtex.c` | Texture management |
| `GUTS/3dfx/glide-rush2.7/csrc/gsst.c` | SST hardware interface |

---

## Data Structures

### N64 Display List (Gfx)

The N64 uses 64-bit display list commands stored in RDRAM:

```c
/* From include/types.h */
typedef union {
    struct {
        u32 w0;  /* Opcode + parameters */
        u32 w1;  /* Additional parameters */
    } words;
    u64 force_structure_alignment;
} Gfx;
```

### Arcade GrVertex Structure

The arcade uses the Glide GrVertex structure for triangle submission:

```c
/* From glide.h */
typedef struct {
    float x, y;           /* Screen coordinates */
    float ooz;            /* 1/Z for depth */
    float oow;            /* 1/W for perspective */
    float r, g, b, a;     /* Vertex colors (0-255 range as float) */
    GrTmuVertex tmuvtx[GLIDE_NUM_TMU];  /* Per-TMU texture coords */
} GrVertex;

typedef struct {
    float oow;            /* 1/W for this TMU */
    float sow, tow;       /* S/W, T/W texture coordinates */
} GrTmuVertex;
```

### N64 Display State Structure

```c
/* From src/game/display.c */
typedef struct {
    s32 x_start;      /* Packed (top << 16) | bottom */
    s32 time;         /* Frame time/counter */
    s32 width;        /* Width * 2 */
    s32 y_pos;        /* Y position (low 16 bits) */
    s32 y_start;      /* Packed (top << 16) | bottom */
    s32 scale_x;      /* X scale factor */
    s32 flags;        /* Flags (bit 6 = interlace) */
    s32 height;       /* Height */
    s32 y_offset;     /* Y offset */
    s32 scissor;      /* Scissor rect */
} DisplayState;
```

---

## Function Mapping Table

### Display/Viewport Setup

| N64 Function | Address | Arcade Equivalent | Notes |
|--------------|---------|-------------------|-------|
| `gfx_init_dl` | - | N/A | Initializes display list buffer |
| `gfx_alloc_dl` | - | N/A | Allocates space in display list |
| `display_update` | 0x800015F0 | `grBufferSwap` | Updates viewport, handles double-buffering |
| `viewport_setup` | 0x80001B44 | `grClipWindow` | Sets viewport parameters |
| `display_process` | 0x80001D60 | N/A | Processes pending display updates |
| `get_tv_offset` | 0x80001DFC | N/A | PAL/NTSC timing offset |
| `get_viewport_pos` | 0x80001E84 | N/A | Gets current viewport position |
| `update_viewport` | 0x80001F2C | `grClipWindow` | Updates viewport with bounds checking |

### 3D Rendering

| N64 Function | Address | Size | Arcade Equivalent | Notes |
|--------------|---------|------|-------------------|-------|
| `render_objects_3d` | 0x80099BFC | 10KB | `MBOX_ShowObject` | Main model rendering (uses G_DL) |
| `render_scene_large` | 0x80087A08 | 10KB | Scene traversal | Possibly track/scene rendering |
| `render_util_common` | 0x800B65B4 | - | Various | Called 65+ times |
| `render_scene` | 0x800A04C4 | 2.7KB | `grDrawTriangle` | Viewport/camera setup (G_SETGEOMETRYMODE) |
| `track_render` | 0x800F34D8 | 3.5KB | Track rendering | Renders visible track sections |
| `skybox_render` | 0x800F2A28 | 2.7KB | N/A | Skybox around camera |

### Visual Effects

| N64 Function | Address | Arcade Equivalent | Notes |
|--------------|---------|-------------------|-------|
| `environment_map_setup` | 0x800F20A0 | `SetCarReflection` | Envmap for reflective surfaces |
| `reflection_setup` | 0x800F2720 | `ZOID_SetReflect` | Planar reflections for wet surfaces |
| `water_surface_render` | 0x800F2890 | N/A | Animated water with wave distortion |
| `shadow_render` | - | `AnimateShadow` | Blob shadow under objects |
| `lens_flare` | - | N/A | Sun lens flare effect |
| `lighting_setup` | - | `UpdateFog` | Scene lighting configuration |
| `fog_setup` | 0x- | `grFogMode`, `grFogTable` | Fog configuration |

### Car Rendering (Arcade visuals.c)

| Arcade Function | Purpose | N64 Mapping |
|-----------------|---------|-------------|
| `CreateCar` | Initializes car object hierarchy | Car object setup in game.c |
| `SetupCar` | Configures car parts based on type | Part selection logic |
| `StartCar` | Starts car visual effects | Effect initialization |
| `PickCar` | Selects car body model | LOD/damage selection |
| `SetCarReflection` | Sets car reflection textures | `environment_map_setup` |
| `AnimateFrame` | Updates car body position | Matrix updates |
| `AnimateTire` | Tire rotation/position | Wheel matrix transforms |
| `AnimateShadow` | Shadow projection | `shadow_render` |
| `AnimateBrakeLights` | Brake light visibility | Primitive color changes |
| `DoSkid` | Skid mark rendering | Decal rendering |
| `AnimateSmoke` | Tire smoke particles | Particle system |
| `AnimateBlast` | Explosion effects | Explosion sprites |

---

## RDP Command Patterns

### Common GBI Macros Found in Code

```c
/* Render mode setup */
gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
gDPSetRenderMode(gfx++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
gDPSetRenderMode(gfx++, G_RM_OPA_SURF, G_RM_OPA_SURF2);

/* Color/combine modes */
gDPSetPrimColor(gfx++, 0, 0, r, g, b, alpha);
gDPSetCombineMode(gfx++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
gDPSetCombineMode(gfx++, G_CC_MODULATERGBA, G_CC_MODULATERGBA);
gDPSetCombineMode(gfx++, G_CC_SHADE, G_CC_SHADE);

/* Texture setup */
gSPTexture(gfx++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
gDPSetTexturePersp(gfx++, G_TP_PERSP);
gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, texture_ptr);
gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, ...);
gDPSetTileSize(gfx++, G_TX_RENDERTILE, s0, t0, s1, t1);

/* Geometry setup */
gSPMatrix(gfx++, OS_K0_TO_PHYSICAL(mtx), G_MTX_MODELVIEW | G_MTX_LOAD);
gSPDisplayList(gfx++, display_list_ptr);
gSPVertex(gfx++, vertex_ptr, count, 0);
gSP2Triangles(gfx++, v0, v1, v2, 0, v3, v4, v5, 0);
gSPPopMatrix(gfx++, G_MTX_MODELVIEW);

/* Geometry mode */
gSPSetGeometryMode(gfx++, G_ZBUFFER);
gSPClearGeometryMode(gfx++, G_ZBUFFER);
gSPNumLights(gfx++, 1);
gSPFogPosition(gfx++, fogMin, fogMax);
gDPSetFogColor(gfx++, r, g, b, 255);
```

### Raw Command Opcodes Found

```c
/* G_SETOTHERMODE_L - Mode setting */
dl->words.w0 = 0xE200001C;  /* G_SETOTHERMODE_L opcode */
dl->words.w1 = mode_bits;

/* G_RDPPIPESYNC - Pipeline sync (implicit in macros) */
```

---

## F3DEX2 Microcode Usage

The N64 version uses F3DEX2 microcode, which provides:

### RSP Features
- **Matrix stack**: Push/pop for hierarchical transforms
- **Vertex cache**: 32 vertices cached for triangle assembly
- **Clipping**: Near/far plane, guard band clipping
- **Lighting**: Up to 8 lights, ambient + directional
- **Fog**: Linear fog blending

### Typical Render Flow
1. `gSPViewport` - Set viewport transformation
2. `gSPMatrix` - Load projection matrix
3. `gSPMatrix` - Load/multiply modelview matrix
4. `gSPVertex` - Load vertices into RSP cache
5. `gSP1Triangle` / `gSP2Triangles` - Draw primitives
6. `gSPDisplayList` - Call nested display lists

### Comparison with Glide

| F3DEX2 Operation | Glide Equivalent |
|------------------|------------------|
| `gSPMatrix` | Matrix computed on CPU, applied to GrVertex |
| `gSPVertex` | Vertex data in GrVertex struct |
| `gSP2Triangles` | `grDrawTriangle` |
| `gSPDisplayList` | No equivalent (display lists are N64-specific) |
| `gDPSetPrimColor` | `grConstantColorValue` |
| `gDPSetCombineMode` | `grColorCombine`, `grAlphaCombine` |
| `gDPSetRenderMode` | `grAlphaBlendFunction`, `grDepthBufferMode` |
| `gDPSetTextureImage` | `grTexSource`, `grTexDownloadMipMapLevel` |

---

## Texture System Comparison

### N64 RDP Texture Pipeline

```
ROM -> RDRAM (via DMA) -> TMEM (4KB) -> Texture Unit
                                     -> RDP Color Combiner
```

- **TMEM**: 4KB texture cache, shared between tiles
- **Tile descriptors**: 8 tiles (0-7), configurable format/size
- **Load tile**: Usually tile 7 (`G_TX_LOADTILE`)
- **Render tile**: Usually tile 0 (`G_TX_RENDERTILE`)
- **Max texture size**: 32x64 (16-bit) or 64x64 (8-bit) per tile

### Arcade 3dfx Texture Pipeline

```
System RAM -> TMU SDRAM (1-4MB per TMU) -> Texture Unit
                                        -> FBI Color Combiner
```

- **TMU memory**: 1-4MB dedicated per TMU
- **Multiple TMUs**: Up to 3 for multitexturing
- **MipMap support**: Hardware LOD selection
- **NCC format**: YIQ compression for textures

### Texture Mapping (From visuals.c)

| Arcade Texture | Purpose |
|----------------|---------|
| `SPARK01A`-`SPARK06A` | Spark particle frames |
| `SHADBLUR` | Shadow texture |
| `SKID04` | Skid mark decal |
| `FBALL00`-`FBALL30` | Explosion fireball frames |
| `SMOKE1`-`SMOKE16` | Smoke particle frames |
| `REF_BCLOUD`, `REF_WCLOUD` | Reflection textures |
| `REF_BTUNNEL`, `REF_WTUNNEL` | Tunnel reflections |

---

## Rendering Techniques

### Environment Mapping (Car Reflections)

**Arcade (visuals.c)**:
```c
void SetCarReflection(S16 slot, S32 wintex, S32 bodtex, F32 speed) {
    win_rtex = ZOID_GetRomTex(gTexList[wintex]);
    bod_rtex = ZOID_GetRomTex(gTexList[bodtex]);
    ZOID_SetReflect(car->hood, bod_rtex, speed);
    ZOID_SetReflect(car->frame, bod_rtex, speed);
    // ... for each car part
}
```

**N64 (game.c)**:
```c
void environment_map_setup(void *object) {
    gSPTexture(gfx++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
    gDPSetTexturePersp(gfx++, G_TP_PERSP);
    gDPSetCombineMode(gfx++, G_CC_MODULATERGBA, G_CC_MODULATERGBA);
    gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, envmap_texture);
}
```

### Shadow Rendering

**Arcade**: Uses `AnimateShadow` to project blob shadow, fades with height.

**N64**: Uses `shadow_render` which:
1. Gets ground height at object position
2. Calculates height above ground
3. Scales/fades shadow based on height
4. Draws as 2D decal

### Fog Implementation

**Arcade (Glide)**:
```c
grFogMode(GR_FOG_WITH_TABLE);
grFogTable(fogTable);  /* 64-entry lookup table */
grFogColorValue(fog_color);
```

**N64 (game.c)**:
```c
gDPSetFogColor(gfx++, fog_color_r, fog_color_g, fog_color_b, 255);
gSPFogPosition(gfx++, fogMin, fogMax);  /* Linear fog range */
```

### Translucent Sorting

**Arcade**: Uses `ZATR_SORTALPHA` flag for objects requiring alpha sorting.

**N64**: Objects with translucency must be rendered back-to-front. Common patterns:
1. Render opaque geometry first
2. Sort translucent objects by distance
3. Render translucent geometry

---

## Performance Considerations

### N64 Bottlenecks
- **RDRAM bandwidth**: Shared between CPU, RSP, RDP
- **TMEM size**: Only 4KB for textures
- **RDP fill rate**: ~150K pixels/sec for textured, lit triangles
- **RSP transform rate**: ~100K vertices/sec

### Arcade Advantages
- **Dedicated texture memory**: 1-4MB per TMU
- **Higher fill rate**: Voodoo1 ~50M pixels/sec
- **No CPU contention**: Dedicated 3D hardware
- **Multitexturing**: Up to 3 textures per pass

### Optimization Strategies (N64)

1. **Display list caching**: Pre-build static geometry
2. **LOD selection**: Reduce triangle count at distance
3. **Texture atlasing**: Minimize TMEM loads
4. **Culling**: Aggressive frustum/occlusion culling
5. **Batch triangles**: Minimize state changes

---

## Key Global Variables

### N64 (from game.c)

| Variable | Address | Purpose |
|----------|---------|---------|
| `gfx_dl_ptr` | - | Current display list pointer |
| `gfx_dl_base` | - | Display list base |
| `gfx_task` | - | Graphics OSTask |
| `frame_counter` | 0x80142AFC | Frame counter |
| `gstate` | 0x801146EC | Game state byte |

### Arcade (from visuals.c)

| Variable | Purpose |
|----------|---------|
| `gEnvirons[]` | Environmental visual effects list |
| `gCarParts[]` | Car part object references |
| `gTexList[]` | Dynamic texture handles |
| `gObjList[]` | Pre-located object handles |
| `gSkidList` | Active skid marks list |

---

## Decompilation Priority

### High Priority (Core Rendering)
1. `render_objects_3d` (0x80099BFC) - 10KB, main model rendering
2. `render_scene_large` (0x80087A08) - 10KB, scene/track rendering
3. `render_scene` (0x800A04C4) - 2.7KB, viewport setup

### Medium Priority (Effects)
4. `environment_map_setup` (0x800F20A0) - Car reflections
5. `track_render` (0x800F34D8) - Track section rendering
6. `skybox_render` (0x800F2A28) - Skybox rendering

### Lower Priority (Polish)
7. Shadow rendering functions
8. Water surface effects
9. Lens flare effects
10. Particle systems

---

## References

### N64 Documentation
- N64 Programming Manual (RDP/RSP chapters)
- F3DEX2 Microcode Reference
- libultra os_vi, os_sp, gbi.h

### Arcade Documentation
- Glide 2.x API Reference
- 3dfx SST1 Hardware Reference
- Rush The Rock source (game/visuals.c, GUTS/3dfx/)

### Related Project Files
- `docs/effects_particle_mapping.md` - Particle effects
- `docs/camera_system_mapping.md` - Camera rendering
- `docs/track_geometry_mapping.md` - Track rendering details
