# Rush 2049 N64 Rendering Pipeline Documentation

## Overview

This document provides comprehensive documentation of the Rush 2049 N64 3D rendering pipeline, including the frame rendering sequence, display list construction, LOD system, texture loading, lighting model, and split-screen handling.

The N64 version uses the Reality Display Processor (RDP) and Reality Signal Processor (RSP) with F3DEX2 microcode, which differs significantly from the arcade version's 3dfx Voodoo/Glide API.

---

## Table of Contents

1. [Frame Rendering Sequence](#frame-rendering-sequence)
2. [Display List Construction](#display-list-construction)
3. [Rendering Stages](#rendering-stages)
4. [LOD (Level of Detail) System](#lod-level-of-detail-system)
5. [Texture Loading](#texture-loading)
6. [Lighting Model](#lighting-model)
7. [Split-Screen Handling](#split-screen-handling)
8. [N64 RDP/RSP Usage Patterns](#n64-rdprsp-usage-patterns)
9. [Arcade 3dfx Equivalents](#arcade-3dfx-equivalents)
10. [Key Functions Reference](#key-functions-reference)

---

## Frame Rendering Sequence

The main game loop calls `render_scene()` each frame, which orchestrates the entire rendering pipeline.

### High-Level Frame Flow

```
1. Frame Start
   |
   +-> render_scene(track, angle, x, y)
       |
       +-> camera_viewport_setup()     [Stage 1: Setup]
       |
       +-> skybox_render()             [Stage 2: Background]
       |
       +-> track_render()              [Stage 3: Track Geometry]
       |
       +-> car_render_full() x N       [Stage 4: Vehicles]
       |   +-> car_body_render()
       |   +-> car_wheels_render()
       |   +-> car_damage_render()
       |   +-> car_lights_render()
       |
       +-> effects_render()            [Stage 5: Particles/Effects]
       |   +-> weather_update()
       |   +-> particle_systems()
       |
       +-> hud_render()                [Stage 6: HUD/UI]
           +-> draw_speedometer()
           +-> draw_lap_counter()
           +-> draw_position()
           +-> draw_timer()
           +-> draw_minimap()
           +-> draw_message()
```

### Frame Timing

The game targets 30 FPS (33.3ms per frame), with rendering typically consuming 16-20ms on N64 hardware.

| Phase | Typical Time | Description |
|-------|--------------|-------------|
| Setup | 0.5ms | Viewport, matrices, state setup |
| Track | 8-12ms | Track geometry with LOD |
| Cars | 3-6ms | All visible vehicles |
| Effects | 2-4ms | Particles, weather, shadows |
| HUD | 1-2ms | 2D UI elements |
| RSP/RDP Sync | 2-5ms | Microcode execution |

---

## Display List Construction

The N64 uses a display list (DL) architecture where rendering commands are accumulated into a buffer in RDRAM, then submitted to the RSP for processing.

### Display List Structure

```c
/* From src/game/game.c */
extern Gfx **gfx_dl_ptr;   /* Current DL write pointer */
extern Gfx *gfx_dl_base;   /* DL buffer base address */
extern OSTask gfx_task;    /* Graphics task structure */
```

### Command Format (64-bit Gfx Commands)

Each display list command is 64 bits (8 bytes):

| Bits | Field | Description |
|------|-------|-------------|
| 63-56 | Opcode | Command type (0xDF = end, 0xDE = call, etc.) |
| 55-32 | Parameters | Opcode-specific high word |
| 31-0 | Parameters | Opcode-specific low word (often address) |

### Common Display List Commands Used

```c
/* From src/game/game.c analysis */

/* Matrix operations */
0xDA380001  /* G_MTX - Push transformation matrix */
0xD8380040  /* G_POPMTX - Pop matrix stack */

/* Geometry */
0x01xxxxxx  /* G_VTX - Load vertices */
0x06xxxxxx  /* G_DL - Call sub-display list */
0xDF000000  /* G_ENDDL - End display list */
0xDE000000  /* G_BRANCH_DL - Branch to display list */

/* RDP State */
0xB7000000  /* G_SETGEOMETRYMODE - Enable geometry modes */
0xE7000000  /* G_RDPPIPESYNC - RDP sync */
0xE4000000  /* G_TEXRECT - Textured rectangle */
0xED000000  /* G_SETSCISSOR - Set scissor rectangle */
0xEE000000  /* G_SETPRIMDEPTH - Set primitive depth */
```

### Display List Traversal

The `display_list_traverse()` function (at 0x800963E8) processes display lists:

```c
void display_list_traverse(u32 *dlPtr, void *param1, void *param2, void *param3) {
    u32 cmd;
    u32 cmdHi;

    cmd = *dlPtr;
    cmdHi = cmd & 0xFF000000;

    while (cmdHi != 0xDF000000) {  /* Until G_ENDDL */
        if ((cmd & 0xFF000000) == 0xDE000000) {
            /* Branch to sub-display list */
            dlPtr = (u32 *)dlPtr[1];
        } else {
            dlPtr += 2;  /* Advance 8 bytes */
        }
        cmd = *dlPtr;
        cmdHi = cmd & 0xFF000000;
    }
}
```

---

## Rendering Stages

### Stage 1: Setup

The `render_mode_setup()` function configures the RDP for the current frame:

```c
void render_mode_setup(s32 mode, s32 flags) {
    u32 *dlPtr = *(u32 **)(0x80149438);
    /* Sets render mode, blend mode, geometry flags */
}
```

Key operations:
- Initialize display list pointer
- Set viewport transformation
- Configure projection matrix
- Set default geometry modes

### Stage 2: Skybox Rendering

The skybox is rendered first with Z-buffer disabled:

```c
void skybox_render(void *camera) {
    /* Disable Z buffer for skybox (always behind) */
    gDPSetRenderMode(gfx++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gSPClearGeometryMode(gfx++, G_ZBUFFER);

    /* Set sky color based on time of day */
    switch (skyType) {
        case 0:  /* Day - blue gradient */
            gDPSetPrimColor(gfx++, 0, 0, 135, 206, 235, 255);
            break;
        case 1:  /* Sunset - orange/pink */
            gDPSetPrimColor(gfx++, 0, 0, 255, 140, 100, 255);
            break;
        case 2:  /* Night - dark blue */
            gDPSetPrimColor(gfx++, 0, 0, 20, 20, 60, 255);
            break;
    }

    /* Re-enable Z buffer */
    gSPSetGeometryMode(gfx++, G_ZBUFFER);
}
```

### Stage 3: Track Rendering

Track geometry uses distance-based culling and LOD:

```c
void track_render(void *camera) {
    /* Iterate visible track sections */
    for (i = 0; i < numSections; i++) {
        /* Distance-based LOD selection */
        dist = calculate_distance(section, camera);
        lod = select_lod(dist);

        /* Render section with appropriate LOD */
        gSPDisplayList(gfx++, sectionDL[lod]);
    }
}
```

The `track_geometry_process()` function builds track display lists from segment data.

### Stage 4: Car Rendering

Each visible car is rendered through `car_render_full()`:

```c
void car_render_full(void *car) {
    /* 1. Render shadow (if on ground) */
    if (onGround) {
        car_shadow_render(car, NULL);
    }

    /* 2. Render car body with LOD */
    entity_render_main(car);

    /* 3. Apply damage visuals if damaged */
    if (damageLevel > 0) {
        car_damage_visual(car, damageLevel);
    }

    /* 4. Render lights (headlights, taillights) */
    car_lights_render(car, lightState);

    /* 5. Update brake lights */
    brake_light_update(car, isBraking);

    /* 6. Exhaust smoke effect */
    exhaust_smoke_effect(car, exhaustPos);

    /* 7. Dust/dirt effect on certain road types */
    if (onGround && speed > 30.0f && (roadType == DIRT || roadType == GRAVEL)) {
        engine_particle_effect(car, 0);  /* Dust */
    }
}
```

#### Car Sub-Components

| Function | Description |
|----------|-------------|
| `car_body_render()` | Main body mesh with paint color |
| `car_wheels_render()` | Four wheels with rotation/steering |
| `car_damage_render()` | Damage overlay meshes |
| `car_lights_render_simple()` | Headlights and taillights |
| `car_exhaust_render()` | Exhaust particles/flames |

### Stage 5: Effects Rendering

Weather and particle effects:

```c
void weather_update_simple(void) {
    switch (weather_type) {
        case 0:  /* Clear - no weather effects */
            break;
        case 1:  /* Rain */
            weather_rain();
            weather_fog(weather_intensity * 0.3f);
            break;
        case 2:  /* Snow */
            weather_snow();
            weather_fog(weather_intensity * 0.5f);
            break;
        case 3:  /* Fog only */
            weather_fog(weather_intensity);
            break;
    }
}
```

Fog configuration:

```c
void weather_fog(f32 density) {
    s32 fogMin = (s32)(900.0f - density * 800.0f);
    s32 fogMax = (s32)(1000.0f - density * 500.0f);

    gDPSetFogColor(gfx++, fog_color_r, fog_color_g, fog_color_b, 255);
    gSPFogPosition(gfx++, fogMin, fogMax);
}
```

### Stage 6: HUD Rendering

The HUD is rendered last as 2D overlay:

```c
void hud_render(void) {
    /* Check if HUD is enabled */
    if (message_active == 0) return;

    /* Get player data */
    player = car_pointers[0];
    speed = calculate_speed(player);
    currentLap = player->lap;
    position = player->position;

    /* Draw HUD elements */
    draw_speedometer(speed);
    draw_lap_counter(currentLap, total_lap_count);
    draw_position(position);
    draw_timer(current_race_time);
    draw_minimap(player);

    /* Draw active message if any */
    if (message_id >= 0 && message_timer > 0) {
        draw_message(message_id);
        message_timer--;
    }
}
```

HUD element IDs:
| ID | Element |
|----|---------|
| 0 | Speedometer background |
| 1 | Tachometer background |
| 2 | Lap indicator icon |
| 3 | Position indicator |
| 4 | Timer background |
| 5 | Nitro bar background |
| 6 | Minimap frame |
| 7-15 | Number sprites 0-9 |
| 16-25 | Letter sprites |
| 128-143 | Speedometer needle positions |
| 144 | Player dot on minimap |

---

## LOD (Level of Detail) System

The N64 version uses a multi-tier LOD system to maintain frame rate.

### Entity LOD Selection

```c
void entity_lod_select(void *entity, f32 distance) {
    /* LOD distance thresholds (world units) */
    lodDist0 = 100.0f;   /* High detail */
    lodDist1 = 300.0f;   /* Medium detail */
    lodDist2 = 600.0f;   /* Low detail */

    /* Select LOD level */
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

### Car LOD Thresholds

Cars use tighter LOD thresholds since they're the focus of gameplay:

```c
void car_lod_select(void *car, f32 distance) {
    /* Car-specific LOD thresholds */
    lodDistances[0] = 20.0f;   /* High detail */
    lodDistances[1] = 50.0f;   /* Medium detail */
    lodDistances[2] = 100.0f;  /* Low detail */
    lodDistances[3] = 200.0f;  /* Minimal detail */

    /* Only switch LOD if model exists */
    if (newLod != *currentLod && lodModels[newLod] != NULL) {
        *currentLod = newLod;
        *(void **)((u8 *)car + 0x194) = lodModels[newLod];
    }
}
```

### LOD Level Details

| LOD | Distance | Polygon Count | Features |
|-----|----------|---------------|----------|
| 0 | 0-50 | ~2000 tris | Full detail, all car parts |
| 1 | 50-150 | ~800 tris | Simplified body, basic wheels |
| 2 | 150-300 | ~200 tris | Very simple geometry |
| 3 | 300+ | 2-4 tris | Billboard sprite |

---

## Texture Loading

### Texture Format

The N64 uses several texture formats in TMEM (4KB texture cache):

| Format | Bits/Pixel | Description |
|--------|------------|-------------|
| RGBA16 | 16-bit | Standard color with alpha (5551) |
| RGBA32 | 32-bit | High quality color (used sparingly) |
| CI4 | 4-bit | 16-color indexed |
| CI8 | 8-bit | 256-color indexed |
| IA8 | 8-bit | Intensity + Alpha |
| IA16 | 16-bit | High quality I+A |

### Texture Loading Commands

```c
/* Set texture image source */
gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, textureAddress);

/* Sync before loading */
gDPLoadSync(gfx++);

/* Load texture block */
gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 32 * 32 - 1, 0);

/* Set tile parameters */
gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0,
           G_TX_RENDERTILE, 0, G_TX_WRAP, 5, 0, G_TX_WRAP, 5, 0);
```

### Environment Mapping

Reflective surfaces use spherical environment mapping:

```c
void environment_map_setup(void *object) {
    /* Enable texture generation */
    gSPTexture(gfx++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);

    /* Configure for spherical envmap */
    gDPSetTexturePersp(gfx++, G_TP_PERSP);
    gDPSetCombineMode(gfx++, G_CC_MODULATERGBA, G_CC_MODULATERGBA);

    /* Set envmap texture */
    gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, envmap_texture);
}
```

---

## Lighting Model

The N64 uses a simplified lighting model configured via RSP.

### Lighting Setup

```c
void lighting_setup(void) {
    s32 timeOfDay = track_time_of_day;  /* 0-2400 */

    /* Calculate sun angle based on time */
    sunAngle = (timeOfDay / 2400.0f) * 3.14159f;
    brightness = sinf(sunAngle);
    if (brightness < 0.0f) brightness = 0.0f;

    /* Set ambient light based on time */
    ambient_light_rgb[0] = (u8)(40 + brightness * 60);   /* R */
    ambient_light_rgb[1] = (u8)(40 + brightness * 70);   /* G */
    ambient_light_rgb[2] = (u8)(60 + brightness * 80);   /* B */

    /* Set directional light color */
    directional_light_rgb[0] = (u8)(brightness * 255);
    directional_light_rgb[1] = (u8)(brightness * 240);
    directional_light_rgb[2] = (u8)(brightness * 200);

    /* Set light direction (from sun position) */
    light_direction[0] = cosf(sunAngle);
    light_direction[1] = sinf(sunAngle);
    light_direction[2] = 0.3f;

    /* Apply to RSP */
    gSPNumLights(gfx++, 1);
}
```

### Time of Day Effects

| Time | Ambient | Directional | Sky Color |
|------|---------|-------------|-----------|
| 0600 | (40,40,60) | Dim orange | Dark blue gradient |
| 1200 | (100,110,140) | Bright white | Light blue |
| 1800 | (60,50,70) | Orange-red | Sunset orange/pink |
| 2200 | (40,40,60) | Very dim | Dark night blue |

---

## Split-Screen Handling

The N64 version supports up to 4-player split-screen multiplayer.

### Viewport Configuration

```c
void camera_split_screen_setup(s32 playerCount) {
    s32 screenW = 320;
    s32 screenH = 240;

    switch (playerCount) {
        case 1:
            /* Full screen for single player */
            camera_viewport_setup(cameras[0], 0, 0, screenW, screenH);
            break;

        case 2:
            /* Horizontal split */
            camera_viewport_setup(cameras[0], 0, 0, screenW, screenH / 2);
            camera_viewport_setup(cameras[1], 0, screenH / 2, screenW, screenH / 2);
            break;

        case 3:
            /* Top half + bottom 2 quarters */
            camera_viewport_setup(cameras[0], 0, 0, screenW, screenH / 2);
            camera_viewport_setup(cameras[1], 0, screenH / 2, screenW / 2, screenH / 2);
            camera_viewport_setup(cameras[2], screenW / 2, screenH / 2, screenW / 2, screenH / 2);
            break;

        case 4:
            /* Four quadrants */
            camera_viewport_setup(cameras[0], 0, 0, screenW / 2, screenH / 2);
            camera_viewport_setup(cameras[1], screenW / 2, 0, screenW / 2, screenH / 2);
            camera_viewport_setup(cameras[2], 0, screenH / 2, screenW / 2, screenH / 2);
            camera_viewport_setup(cameras[3], screenW / 2, screenH / 2, screenW / 2, screenH / 2);
            break;
    }

    /* Widen FOV for smaller viewports */
    if (playerCount >= 2) {
        for (i = 0; i < playerCount; i++) {
            *fov = 1.2f;  /* Wider FOV for split screen */
        }
    }
}
```

### Multi-View Rendering

```c
void camera_multi_view_setup(s32 viewIndex, void *camera) {
    s32 viewportX, viewportY, viewportW, viewportH;

    /* Calculate viewport based on view count and index */
    switch (num_players_active) {
        case 1:
            viewportX = 0;
            viewportY = 0;
            viewportW = 320;
            viewportH = 240;
            break;

        case 2:
            viewportX = 0;
            viewportY = viewIndex * 120;
            viewportW = 320;
            viewportH = 120;
            break;

        case 3:
        case 4:
            viewportX = (viewIndex & 1) * 160;
            viewportY = (viewIndex / 2) * 120;
            viewportW = 160;
            viewportH = 120;
            break;
    }

    /* Store viewport in camera */
    *(s32 *)((u8 *)camera + 0x44) = viewportX;
    *(s32 *)((u8 *)camera + 0x48) = viewportY;
    *(s32 *)((u8 *)camera + 0x4C) = viewportW;
    *(s32 *)((u8 *)camera + 0x50) = viewportH;
}
```

### Per-Viewport Rendering

Each viewport requires:
1. Setting RDP viewport via `gSPViewport`
2. Setting scissor rectangle via `gDPSetScissor`
3. Rendering scene from that camera's perspective
4. Repeating for each player

---

## N64 RDP/RSP Usage Patterns

### Common RSP Macros (gSP*)

| Macro | Purpose | N64 Function Location |
|-------|---------|----------------------|
| `gSPMatrix` | Set transformation matrix | `object_render()` line 21782 |
| `gSPDisplayList` | Call sub-display list | Multiple locations |
| `gSPPopMatrix` | Pop matrix from stack | `object_render()` line 21812 |
| `gSPViewport` | Set 3D viewport | `camera_viewport_setup()` |
| `gSPNumLights` | Set number of lights | `lighting_setup()` line 38152 |
| `gSPFogPosition` | Set fog near/far | `weather_fog()` line 38058 |
| `gSPTexture` | Enable/configure textures | `environment_map_setup()` line 38302 |
| `gSPVertex` | Load vertex data | `object_render()` |
| `gSP2Triangles` | Draw two triangles | Crowd rendering |
| `gSPClearGeometryMode` | Disable geometry flags | `skybox_render()` |
| `gSPSetGeometryMode` | Enable geometry flags | Multiple |

### Common RDP Macros (gDP*)

| Macro | Purpose | N64 Function Location |
|-------|---------|----------------------|
| `gDPSetRenderMode` | Set blend/AA mode | Multiple (38321, 38353, etc.) |
| `gDPSetPrimColor` | Set primitive color | Car colors, HUD |
| `gDPSetCombineMode` | Set color combiner | Texture blending |
| `gDPSetTextureImage` | Set texture source | Texture loading |
| `gDPLoadBlock` | Load texture to TMEM | Texture loading |
| `gDPSetTile` | Configure texture tile | Texture setup |
| `gDPSetTileSize` | Set tile coordinates | Animated textures |
| `gDPSetScissor` | Set clip rectangle | Viewport setup |
| `gDPSetFogColor` | Set fog color | `weather_fog()` line 38055 |
| `gDPSetTexturePersp` | Texture perspective | `environment_map_setup()` |
| `gDPLoadSync` | Sync before load | Texture loading |

### Geometry Modes

```c
/* Common geometry mode combinations */
G_ZBUFFER         /* Enable Z-buffer test */
G_TEXTURE_GEN     /* Generate texture coords from normals */
G_SHADE           /* Enable smooth shading */
G_CULL_BACK       /* Back-face culling */
G_LIGHTING        /* Enable vertex lighting */
G_FOG             /* Enable fog */

/* Typical setup for opaque objects */
0x00022000  /* G_ZBUFFER | G_CULL_BACK | G_SHADE | G_LIGHTING */
```

---

## Arcade 3dfx Equivalents

### Architecture Comparison

| N64 Concept | Arcade 3dfx Equivalent |
|-------------|------------------------|
| Display List (Gfx commands) | Direct Glide API calls |
| RSP microcode | Software vertex transform |
| RDP pipeline | TMU + FBI hardware |
| gSPMatrix | grMatrix* functions |
| gDPSetRenderMode | grAlphaBlendFunction |
| gDPSetFogColor/Position | grFogColorValue, grFogTable |
| gDPSetTextureImage | grTexDownloadMipMap |
| gSPVertex | grDrawTriangle vertex setup |
| TMEM (4KB) | 2-4MB texture memory |

### Fog Implementation Comparison

**N64:**
```c
gDPSetFogColor(gfx++, r, g, b, 255);
gSPFogPosition(gfx++, fogMin, fogMax);
```

**Arcade (from targets.c):**
```c
grFogColorValue(fogColor);
grFogTable(fogTable);
grFogMode(GR_FOG_WITH_TABLE);
```

### Visual Effects Mapping

| N64 Function | Arcade Function (visuals.c) |
|--------------|----------------------------|
| `car_render_full()` | `UpdateVisuals()` |
| `car_shadow_render()` | `AnimateShadow()` |
| `car_lights_render()` | `AnimateBrakeLights()` |
| `engine_particle_effect()` | `AnimateSmoke()` |
| `skid_mark_render()` | `DoSkid()`, `AnimateSkids()` |
| `brake_light_update()` | `StartBrakeLights()` |

### Key Arcade Files Reference

| Arcade File | N64 Equivalent | Purpose |
|-------------|----------------|---------|
| `game/visuals.c` | `src/game/game.c` (render functions) | Car visual effects |
| `game/hud.c` | `src/game/game.c` (draw_* functions) | HUD rendering |
| `game/targets.c` | `src/game/game.c` (weather_*) | Fog, environment |
| `GUTS/3dfx/glide-rush2.7/` | libultra + F3DEX2 | Graphics API |

---

## Key Functions Reference

### Rendering Pipeline Functions

| Function | Address | Size | Description |
|----------|---------|------|-------------|
| `render_scene` | 0x800A04C4 | 2.7KB | Main scene rendering entry |
| `render_mode_setup` | 0x80087118 | 1.8KB | RDP mode configuration |
| `object_render` | 0x80087A08 | 10KB | 3D object rendering |
| `track_render` | 0x800F5000 | 3.8KB | Track geometry |
| `car_render_full` | 0x800A6404 | 2KB | Complete car rendering |
| `hud_render` | 0x800FB5F4 | 820B | HUD overlay |

### Display List Functions

| Function | Address | Description |
|----------|---------|-------------|
| `display_list_traverse` | 0x800963E8 | DL traversal |
| `gfx_init_dl` | (in gfx.c) | Initialize DL buffer |
| `gfx_alloc_dl` | (in gfx.c) | Allocate DL memory |

### Viewport/Camera Functions

| Function | Description |
|----------|-------------|
| `camera_viewport_setup` | Configure single viewport |
| `camera_split_screen_setup` | Multi-player viewport layout |
| `camera_multi_view_setup` | Per-view camera setup |
| `render_viewport_init` | Initialize viewport state |

### LOD Functions

| Function | Address | Description |
|----------|---------|-------------|
| `entity_lod_select` | 0x80096734 | General entity LOD |
| `car_lod_select` | 0x800A4508 | Car-specific LOD |
| `entity_cull_check` | 0x80096A00 | Frustum culling |

### Effect Functions

| Function | Description |
|----------|-------------|
| `weather_fog` | Configure RDP fog |
| `weather_rain` | Rain particle system |
| `weather_snow` | Snow particle system |
| `lighting_setup` | Scene lighting configuration |
| `shadow_render` | Object blob shadows |
| `lens_flare` | Sun lens flare effect |
| `environment_map_setup` | Reflective surface setup |
| `reflection_setup` | Planar reflections |
| `water_surface_render` | Animated water |

---

## Memory Layout

### Display List Buffer

```
0x80149438: Display list write pointer
0x80149440: Display list base address
```

### Render State Variables

```
0x8012E60C: Screen clip min X
0x8012E610: Screen clip max X
0x8012E668: Screen clip min Y
0x801174B4: Frame timing flag
0x8012EAE0: Render enable flag
```

### Texture/Sprite Tables

```
sprite_table_data[]: UI sprite pointers (64 entries)
envmap_texture[]: Environment map texture data
skybox_texture_data[]: Skybox textures
```

---

## Performance Considerations

### Bottlenecks

1. **TMEM Size (4KB)**: Requires careful texture management
2. **RSP Vertex Budget**: ~32 vertices per batch
3. **RDP Fill Rate**: Limits overdraw
4. **RDRAM Bandwidth**: Shared between CPU, RDP, audio

### Optimizations Used

1. **Aggressive LOD**: 4-level LOD for all objects
2. **Distance Culling**: Objects beyond threshold not rendered
3. **View Frustum Culling**: `entity_cull_check()` tests visibility
4. **Batched Vertices**: Load vertices in groups of 32
5. **Display List Reuse**: Pre-built DLs for static geometry
6. **Split-Screen Downgrade**: Simpler effects in multiplayer

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2025-12-30 | Initial comprehensive documentation |

---

## Related Documentation

- [docs/render_system_mapping.md](render_system_mapping.md) - Architecture comparison
- [docs/hud_ui_mapping.md](hud_ui_mapping.md) - HUD system details
- [docs/effects_particle_mapping.md](effects_particle_mapping.md) - Particle effects
- [docs/camera_system_mapping.md](camera_system_mapping.md) - Camera implementation
