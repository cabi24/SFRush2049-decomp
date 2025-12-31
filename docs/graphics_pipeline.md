# Rush 2049 N64 Graphics Pipeline

## Overview

This document provides a comprehensive analysis of the Rush 2049 N64 rendering system, covering display list construction, RDP command usage, viewport setup, texture handling, and F3DEX2 microcode utilization. It also includes a comparison with the arcade Rush The Rock `ZOID/visuals.c` rendering system.

The N64 version uses the Reality Display Processor (RDP) and Reality Signal Processor (RSP) with F3DEX2 microcode, which differs fundamentally from the arcade's 3dfx Voodoo/Glide API.

---

## Table of Contents

1. [Architecture Overview](#architecture-overview)
2. [Display List System](#display-list-system)
3. [RDP Command Reference](#rdp-command-reference)
4. [Viewport and Display Setup](#viewport-and-display-setup)
5. [Texture Handling](#texture-handling)
6. [F3DEX2 Microcode Usage](#f3dex2-microcode-usage)
7. [Render Functions Analysis](#render-functions-analysis)
8. [Arcade visuals.c Comparison](#arcade-visualsc-comparison)
9. [Performance Considerations](#performance-considerations)
10. [Key Data Structures](#key-data-structures)

---

## Architecture Overview

### N64 Graphics Hardware

The N64 graphics system consists of two cooperating processors:

| Component | Function | Memory |
|-----------|----------|--------|
| **RSP (Reality Signal Processor)** | Vertex transformation, lighting, clipping | 4KB IMEM, 4KB DMEM |
| **RDP (Reality Display Processor)** | Rasterization, texturing, blending | 4KB TMEM |

Both processors share access to RDRAM (4MB or 8MB) which holds:
- Display lists (rendering commands)
- Vertex data
- Texture data
- Framebuffer (16-bit or 32-bit)
- Z-buffer (16-bit)

### Command Flow

```
CPU (game.c)                    RSP (F3DEX2)              RDP
    |                               |                       |
    | Build display list            |                       |
    | in RDRAM                      |                       |
    |                               |                       |
    +---> osSpTaskStart() --------->|                       |
                                    | Process vertices      |
                                    | Transform, light, clip|
                                    |                       |
                                    +-----> RDP commands -->|
                                                            | Rasterize
                                                            | Texture
                                                            | Blend
                                                            | Z-test
                                                            |
                                                            +--> Framebuffer
```

---

## Display List System

### Core Data Structures

The display list system is managed in `src/game/gfx.c`:

```c
/* Global display list pointers */
extern Gfx *gDisplayListHead;  /* D_800354C4 - Current write position */
extern Gfx *gDisplayListEnd;   /* D_800354CC - End of buffer */
extern u32 gDisplayListSize;   /* D_800354C8 - Commands allocated */
```

### Display List Initialization

```c
/**
 * Initialize display list buffer
 * @param start Pointer to start of display list buffer
 * @param end Pointer to end of display list buffer
 */
void gfx_init_dl(Gfx *start, Gfx *end) {
    gDisplayListHead = start;
    gDisplayListEnd = end;
    gDisplayListSize = 0;
}
```

### Display List Allocation

```c
/**
 * Allocate space in display list
 * @param size Number of Gfx commands to allocate
 * @return Pointer to allocated display list space
 */
Gfx *gfx_alloc_dl(u32 size) {
    Gfx *result;
    gDisplayListSize += size;
    result = gDisplayListHead + (gDisplayListSize - size);
    return result;
}
```

### Command Format

Each display list command is 64 bits (8 bytes):

```c
typedef union {
    struct {
        u32 w0;  /* Bits 63-32: Opcode + parameters */
        u32 w1;  /* Bits 31-0: Additional parameters/address */
    } words;
    u64 force_structure_alignment;
} Gfx;
```

| Bits | Field | Description |
|------|-------|-------------|
| 63-56 | Opcode | Command type identifier |
| 55-32 | Params High | Opcode-specific parameters |
| 31-0 | Params Low | Address or additional parameters |

---

## RDP Command Reference

### Command Opcodes Used in Rush 2049

| Opcode | Macro | Purpose | Location in game.c |
|--------|-------|---------|-------------------|
| 0x01 | G_VTX | Load vertices | object_render (line 21759) |
| 0x06 | G_DL | Call sub-display list | object_render (line 21767) |
| 0xB7 | G_SETGEOMETRYMODE | Enable geometry flags | object_render (line 21749) |
| 0xD8 | G_POPMTX | Pop matrix stack | object_render (line 21774) |
| 0xDA | G_MTX | Push/load matrix | object_render (line 21744) |
| 0xDE | G_BRANCH_DL | Branch to display list | entity_render_main |
| 0xDF | G_ENDDL | End display list | frame_end (line 39213) |
| 0xE2 | G_SETOTHERMODE_L | Set RDP mode low | Multiple locations |
| 0xE4 | G_TEXRECT | Textured rectangle | HUD rendering |
| 0xE7 | G_RDPPIPESYNC | Pipeline sync | frame_start (line 39194) |
| 0xED | G_SETSCISSOR | Set scissor rectangle | Viewport setup |
| 0xF5 | G_SETTILE | Configure texture tile | track_segment_render |
| 0xFD | G_SETTIMG | Set texture image | track_segment_render |

### GBI Macro Usage Patterns

From `src/game/game.c`, the game uses these macro patterns:

#### Render Mode Setup
```c
/* Opaque Z-buffered surfaces */
gDPSetRenderMode(gfx++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);

/* Translucent surfaces */
gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

/* Anti-aliased opaque (high quality) */
gDPSetRenderMode(gfx++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
```

#### Color and Combine Setup
```c
/* Set primitive color for tinting */
gDPSetPrimColor(gfx++, 0, 0, r, g, b, alpha);

/* Modulate texture with primitive color */
gDPSetCombineMode(gfx++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

/* Vertex shading only */
gDPSetCombineMode(gfx++, G_CC_SHADE, G_CC_SHADE);
```

#### Matrix Operations
```c
/* Push transformation matrix */
gSPMatrix(gfx++, OS_K0_TO_PHYSICAL(mtx),
          G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_PUSH);

/* Call object display list */
gSPDisplayList(gfx++, car_model_display_list + carModel * 0x1000);

/* Pop matrix after rendering */
gSPPopMatrix(gfx++, G_MTX_MODELVIEW);
```

#### Fog Configuration
```c
gDPSetFogColor(gfx++, fog_color_r, fog_color_g, fog_color_b, 255);
gSPFogPosition(gfx++, fogMin, fogMax);  /* Linear fog range */
```

---

## Viewport and Display Setup

### Display State Management

From `src/game/display.c`, the display system uses double-buffered state:

```c
typedef struct {
    s32 x_start;      /* 0x1C: packed (top << 16) | bottom */
    s32 time;         /* 0x20: frame time/counter */
    s32 width;        /* 0x28: width * 2 */
    s32 y_pos;        /* 0x2C: y position (low 16 bits) */
    s32 y_start;      /* 0x30: packed (top << 16) | bottom */
    s32 scale_x;      /* 0x3C: x scale factor */
    s32 flags;        /* 0x04: flags (bit 6 = interlace) */
    s32 height;       /* 0x08: height */
    s32 y_offset;     /* 0x40: y offset (low 16 bits) */
    s32 scissor;      /* 0x44: scissor rect */
} DisplayState;
```

### Key Display Functions

| Function | Address | Purpose |
|----------|---------|---------|
| `display_update` | 0x800015F0 | Main display update with viewport interpolation |
| `viewport_setup` | 0x80001B44 | Configure viewport for player/mode |
| `display_process` | 0x80001D60 | Process pending display updates |
| `get_tv_offset` | 0x80001DFC | Get PAL/NTSC timing offset |
| `update_viewport` | 0x80001F2C | Update viewport with bounds checking |

### Viewport Setup Process

```c
void viewport_setup(s32 mode, s32 player, s32 width, s32 height) {
    /* Set scaling factors based on mode */
    if (mode == 0) {
        gScaleFactorX = 50.0f;  /* Low res X scale */
        gScaleFactorY = gScaleLowResY;
    } else {
        gScaleFactorX = 60.0f;  /* High res X scale */
        gScaleFactorY = gScaleHighResY;
    }

    /* Store mode parameters */
    gDisplayMode = (s16)mode;
    gPlayerViewCount = (s16)player;

    /* Select display state from table (0x50 bytes per entry) */
    gReferenceDisplayState = &gDisplayModeTable[player * 5];

    /* Extract viewport bounds from state */
    gViewportXStart = (gReferenceDisplayState->x_start >> 16) & 0x3FF;
    gViewportXEnd = gReferenceDisplayState->x_start & 0x3FF;
    gViewportYStart = (gReferenceDisplayState->y_start >> 16) & 0x3FF;
    gViewportYEnd = gReferenceDisplayState->y_start & 0x3FF;

    /* Update display */
    display_update();
}
```

### Split-Screen Configuration

The game supports up to 4-player split-screen:

| Players | Layout | Viewport Size |
|---------|--------|---------------|
| 1 | Full screen | 320x240 |
| 2 | Horizontal split | 320x120 each |
| 3 | Top half + 2 quarters | 320x120, 160x120, 160x120 |
| 4 | Four quadrants | 160x120 each |

---

## Texture Handling

### TMEM Architecture

The RDP has 4KB of texture memory (TMEM) organized as:

- 512 bytes x 8 banks
- Two halves (low and high) for simultaneous access
- Up to 8 tile descriptors for referencing texture regions

### Texture Loading Pattern

```c
/* Set texture image source address */
gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, textureAddress);

/* Synchronize before loading */
gDPLoadSync(gfx++);

/* Load texture block into TMEM */
gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 32 * 32 - 1, 0);

/* Configure render tile */
gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0,
           G_TX_RENDERTILE, 0, G_TX_WRAP, 5, 0, G_TX_WRAP, 5, 0);
```

### Texture Formats Used

| Format | Bits/Pixel | Max Size (TMEM) | Usage |
|--------|------------|-----------------|-------|
| RGBA16 | 16 | 64x32 or 32x64 | Cars, HUD, general |
| RGBA32 | 32 | 32x32 | High-quality sprites |
| CI4 | 4 | 128x64 | Large textures (paletted) |
| CI8 | 8 | 64x64 | Medium textures (paletted) |
| IA8 | 8 | 64x64 | Shadows, alpha masks |
| IA16 | 16 | 64x32 | High-quality alpha |

### Environment Mapping

The game uses spherical environment mapping for car reflections:

```c
void environment_map_setup(void *object) {
    /* Enable texture with full scale */
    gSPTexture(gfx++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);

    /* Configure for spherical envmap */
    gDPSetTexturePersp(gfx++, G_TP_PERSP);
    gDPSetCombineMode(gfx++, G_CC_MODULATERGBA, G_CC_MODULATERGBA);

    /* Set envmap texture */
    gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, envmap_texture);
}
```

---

## F3DEX2 Microcode Usage

### Microcode Configuration

From `src/game/game.c`, the RSP task setup:

```c
void display_list_flush(void) {
    OSTask *task = &gfx_task;

    /* Configure graphics task */
    task->t.type = M_GFXTASK;
    task->t.flags = 0;
    task->t.ucode_boot = (u64 *)rspbootTextStart;
    task->t.ucode_boot_size = (s32)rspbootTextEnd - (s32)rspbootTextStart;
    task->t.ucode = (u64 *)gspF3DEX2_fifoTextStart;
    task->t.ucode_data = (u64 *)gspF3DEX2_fifoDataStart;
    task->t.ucode_data_size = SP_UCODE_DATA_SIZE;
    task->t.dram_stack = NULL;
    task->t.dram_stack_size = 0;
    task->t.output_buff = NULL;
    task->t.output_buff_size = NULL;
    task->t.data_ptr = (u64 *)gfx_dl_base;
    task->t.data_size = ((u8 *)*gfx_dl_ptr - (u8 *)gfx_dl_base);

    /* Start RSP task */
    osSpTaskStart(task);

    /* Wait for completion */
    osRecvMesg(&dma_message_queue, NULL, OS_MESG_BLOCK);
}
```

### F3DEX2 Features Used

| Feature | GBI Macro | Description |
|---------|-----------|-------------|
| Matrix Stack | gSPMatrix/gSPPopMatrix | Hierarchical transforms for car parts |
| Vertex Cache | gSPVertex | 32-vertex cache for batching |
| Lighting | gSPNumLights | 1 directional + ambient |
| Fog | gSPFogPosition | Linear depth-based fog |
| Geometry Mode | gSPSetGeometryMode | Z-buffer, culling, shading |
| Clipping | Automatic | Near/far + guard band |

### Geometry Mode Flags

```c
/* Common flags from include/types.h */
#define G_ZBUFFER        0x00000001  /* Z-buffer testing */
#define G_SHADE          0x00000004  /* Smooth shading */
#define G_CULL_FRONT     0x00000200  /* Front-face culling */
#define G_CULL_BACK      0x00000400  /* Back-face culling */
#define G_FOG            0x00010000  /* Fog enable */
#define G_LIGHTING       0x00020000  /* Vertex lighting */
#define G_TEXTURE_GEN    0x00040000  /* Texture coord generation */
#define G_SHADING_SMOOTH 0x00200000  /* Gouraud shading */
#define G_CLIPPING       0x00800000  /* Clipping enable */

/* Typical opaque object setup */
/* 0x00022000 = G_ZBUFFER | G_CULL_BACK | G_SHADE | G_LIGHTING */
```

---

## Render Functions Analysis

### Main Rendering Functions

| Function | Address | Size | Description |
|----------|---------|------|-------------|
| `render_scene` | 0x800A04C4 | 2.7KB | Main scene entry point |
| `object_render` | 0x80087A08 | 10KB | 3D object with transforms |
| `entity_render_main` | 0x80099BFC | 10KB | Entity rendering (cars, props) |
| `track_render_process` | 0x800A377C | 3.2KB | Track section rendering |
| `car_render_full` | 0x800A6404 | 2KB | Complete car with effects |

### object_render Analysis

From `src/game/game.c` (line 21710+):

```c
void object_render(void *object, void *matrix) {
    u32 *dlPtr;
    u8 *objData;
    s32 *mtx;
    void *model;
    s32 flags;
    s32 numVerts, numTris;

    if (object == NULL) return;

    objData = (u8 *)object;
    dlPtr = *(u32 **)0x80149438;

    /* Get object flags and model pointer */
    flags = *(s32 *)(objData + 0x08);
    model = *(void **)(objData + 0x10);

    if (model == NULL) return;

    /* Check visibility flag */
    if (!(flags & 0x01)) return;

    /* Push matrix if provided */
    if (matrix != NULL) {
        /* G_MTX with push */
        *dlPtr++ = 0xDA380001;
        *dlPtr++ = (u32)matrix;
    }

    /* Set geometry mode */
    *dlPtr++ = 0xB7000000;
    *dlPtr++ = 0x00022000;  /* Enable lighting and culling */

    /* Load model vertices and triangles */
    numVerts = *(s32 *)((u8 *)model + 0x00);
    numTris = *(s32 *)((u8 *)model + 0x04);

    if (numVerts > 0 && numVerts <= 32) {
        void *vertData = *(void **)((u8 *)model + 0x08);

        /* G_VTX - load vertices */
        *dlPtr++ = 0x01000000 | ((numVerts - 1) << 12) | (numVerts << 1);
        *dlPtr++ = (u32)vertData;
    }

    if (numTris > 0) {
        void *triData = *(void **)((u8 *)model + 0x0C);

        /* G_DL - call display list for triangles */
        *dlPtr++ = 0x06000000;
        *dlPtr++ = (u32)triData;
    }

    /* Pop matrix if we pushed one */
    if (matrix != NULL) {
        *dlPtr++ = 0xD8380040;
        *dlPtr++ = 0x00000040;
    }

    *(u32 **)0x80149438 = dlPtr;
}
```

### Frame Rendering Sequence

From `scene_render_main` (0x800F93A0):

```c
void scene_render_main(void) {
    /* 1. Begin frame - reset display list */
    frame_start();

    /* 2. Setup Z-buffer */
    zbuffer_setup();

    /* 3. Render skybox (Z-buffer disabled) */
    track_load();

    /* 4. Render track geometry */
    checkpoint_init();

    /* 5. Render cars */
    race_end();

    /* 6. Render particle effects */
    effects_update();

    /* 7. Render HUD elements */
    text_color();

    /* 8. End frame - submit to RDP */
    frame_end();
}
```

### Z-Buffer Setup

```c
void zbuffer_setup(void) {
    Gfx *dl = *gfx_dl_ptr;

    /* Set Z-buffer mode */
    gDPSetDepthSource(dl++, G_ZS_PIXEL);
    gDPSetRenderMode(dl++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);
    gDPSetDepthImage(dl++, 0x802C0000);  /* Z-buffer address */

    /* Clear Z-buffer with fill rectangle */
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, 0x802C0000);
    gDPSetFillColor(dl++, 0xFFFCFFFC);  /* Max depth */
    gDPFillRectangle(dl++, 0, 0, 319, 239);
    gDPPipeSync(dl++);

    /* Restore color image for rendering */
    gDPSetColorImage(dl++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, 0x80200000);

    *gfx_dl_ptr = dl;
}
```

---

## Arcade visuals.c Comparison

### Architecture Differences

| Aspect | N64 (RDP/RSP) | Arcade (3dfx Voodoo) |
|--------|---------------|----------------------|
| API | Display lists (Gfx commands) | Direct Glide API calls |
| Transform | RSP microcode | CPU software |
| Rasterization | RDP hardware | TMU + FBI hardware |
| Texture Memory | 4KB TMEM | 2-4MB dedicated per TMU |
| Framebuffer | Shared RDRAM | Dedicated FBI memory |
| Command Submission | Batch (display list) | Immediate mode |

### Arcade Visual Effects (from visuals.c)

The arcade `game/visuals.c` manages car visual effects:

```c
/* Visual effect types */
#define VIS_FRAME      0   /* Car body */
#define VIS_TIRE0-3    1-4 /* Tires */
#define VIS_BLAST      5   /* Explosion */
#define VIS_SHADOW     6   /* Shadow projection */
#define VIS_BRAKE      7   /* Brake lights */
#define VIS_LSPARK     8   /* Left sparks */
#define VIS_RSPARK     9   /* Right sparks */
#define VIS_BSPARK     10  /* Bumper sparks */
#define VIS_QUAD0-4    11-15 /* Body panels (damage) */
#define VIS_WIN0-3     16-19 /* Windows */
```

### Function Mapping: Arcade to N64

| Arcade Function | Purpose | N64 Equivalent |
|-----------------|---------|----------------|
| `CreateCar` | Initialize car object hierarchy | Car setup in game.c |
| `SetupCar` | Configure car parts | Part selection logic |
| `StartCar` | Start visual effects | Effect initialization |
| `PickCar` | Select body model (damage/view) | LOD and damage selection |
| `SetCarReflection` | Set reflection textures | `environment_map_setup` |
| `AnimateFrame` | Update car body | Matrix transforms |
| `AnimateTire` | Tire rotation | Wheel matrix updates |
| `AnimateShadow` | Shadow projection | `car_shadow_render` |
| `AnimateBrakeLights` | Brake light state | Primitive color changes |
| `AnimateSmoke` | Tire smoke particles | Particle system |
| `AnimateBlast` | Explosion animation | Explosion sprites |
| `DoSkid` | Skid mark decals | Decal rendering |
| `UpdateVisuals` | Per-frame visual update | `car_render_full` |
| `UpdateEnvirons` | Environmental effects | Effects in game loop |

### Arcade ZOID/MBOX API

The arcade uses the ZOID (3dfx wrapper) and MBOX (Mathbox) APIs:

```c
/* Arcade object control */
MBOX_ShowObject(objnum, SHOW_EACHCHILD);
MBOX_HideObject(objnum, HIDE_ONLY);
ZOID_SetObjectDef(objnum, modelDef);
ZOID_SetObjectXlu(objnum, xlucency);
ZOID_UpdateObject(objnum, position, orientation);
ZOID_SetReflect(objnum, texture, speed);
ZOID_ChangeTexture(objDef, texIndex, lodIndex, texHandle);
```

**N64 Equivalent Operations:**
```c
/* N64 object control via display lists */
gSPDisplayList(gfx++, objectDL);           /* Show object */
/* (skip DL call to hide) */                /* Hide object */
/* Different DL pointer */                  /* Change model */
gDPSetPrimColor(gfx++, 0, 0, r, g, b, a);  /* Set translucency */
gSPMatrix(gfx++, mtx, flags);              /* Position/orientation */
/* Envmap setup macros */                   /* Reflection */
gDPSetTextureImage(gfx++, ...);            /* Change texture */
```

### Visual System Comparison

#### Arcade Visual Structure
```c
typedef struct {
    VisFunc *func;      /* Animation callback */
    S16 slot;           /* Car slot */
    S16 data;           /* Type-specific data */
    S32 objnum;         /* ZOID object handle */
    S32 index;          /* Animation frame */
    U32 timeStamp;      /* Last update time */
} Visual;
```

#### N64 Entity Render Pattern
```c
/* Entity structure offsets used in game.c */
offset 0x08: flags (visibility, etc.)
offset 0x10: model pointer
offset 0x18: display list pointer
offset 0x1C: render flags (bit 31 = skip render)
offset 0x2C: resource reference
```

### Texture Management Comparison

#### Arcade (visuals.c textures)
```c
static const char * const TexNames[NUM_DYN_TEXS] = {
    "SPARK01A", "SPARK02A", ..., "SPARK06A",  /* Spark particles */
    "SHADBLUR",                                /* Shadow */
    "SKID04",                                  /* Skid marks */
    "FBALL00", ..., "FBALL30",                /* Explosion frames */
    "REF_BCLOUD", "REF_WCLOUD",               /* Cloud reflections */
    "REF_BTUNNEL", "REF_WTUNNEL",             /* Tunnel reflections */
    ...
};
```

#### N64 Texture Loading
- Textures loaded via DMA from ROM to RDRAM
- Transferred to TMEM just before use
- Limited to 4KB at a time
- Requires careful scheduling to avoid TMEM thrashing

---

## Performance Considerations

### N64 Bottlenecks

| Resource | Limit | Impact |
|----------|-------|--------|
| TMEM | 4KB | Max ~64x32 16-bit texture per tile |
| RDRAM Bandwidth | ~500MB/s shared | CPU, RSP, RDP contention |
| RSP Vertex Rate | ~100K/sec | Limits poly count |
| RDP Fill Rate | ~150K pix/sec (textured, lit) | Limits screen coverage |
| Display List | ~64KB typical | Memory budget |

### Optimization Techniques Used

1. **Aggressive LOD System**
   - 4-level LOD for cars (20ft, 50ft, 100ft, 200ft thresholds)
   - Billboard sprites at extreme distance

2. **Distance Culling**
   - Objects beyond threshold not rendered
   - Track sections culled based on camera position

3. **Display List Caching**
   - Pre-built DLs for static geometry
   - Reusable sub-display lists via G_DL

4. **Vertex Batching**
   - Load up to 32 vertices per gSPVertex call
   - Minimize state changes between triangles

5. **Texture Atlasing**
   - Multiple small textures packed into one load
   - Reduces TMEM load operations

6. **Split-Screen Degradation**
   - Simpler effects in multiplayer modes
   - Reduced draw distance
   - Lower LOD thresholds

### Frame Budget (Target 30 FPS = 33.3ms)

| Phase | Typical Time | Notes |
|-------|--------------|-------|
| Game Logic | 8-10ms | Physics, AI, input |
| Display List Build | 3-5ms | CPU building commands |
| RSP Processing | 5-8ms | Vertex transform, lighting |
| RDP Rasterization | 8-12ms | Texturing, blending |
| Buffer Swap | 1-2ms | VSync wait |

---

## Key Data Structures

### Memory Map (Rendering)

| Address | Purpose |
|---------|---------|
| 0x80149438 | Display list write pointer |
| 0x80149440 | Display list base address |
| 0x80200000 | Framebuffer (320x240x16-bit) |
| 0x802C0000 | Z-buffer (320x240x16-bit) |
| 0x8012E60C | Screen clip min X |
| 0x8012E610 | Screen clip max X |
| 0x8012EAE0 | Render enable flag |

### OSTask Structure

```c
typedef struct {
    u32 type;                /* M_GFXTASK */
    u32 flags;               /* Task flags */
    u64 *ucode_boot;         /* RSP boot microcode */
    u32 ucode_boot_size;
    u64 *ucode;              /* F3DEX2 microcode */
    u32 ucode_size;
    u64 *ucode_data;         /* Microcode data */
    u32 ucode_data_size;     /* 0x800 (2KB) */
    u64 *dram_stack;         /* NULL for graphics */
    u32 dram_stack_size;
    u64 *output_buff;        /* NULL for FIFO mode */
    u64 *output_buff_size;
    u64 *data_ptr;           /* Display list base */
    u32 data_size;           /* Display list size */
    u64 *yield_data_ptr;     /* NULL */
    u32 yield_data_size;
} OSTask_t;
```

---

## Related Documentation

- [docs/render_system_mapping.md](render_system_mapping.md) - Detailed N64 vs Arcade comparison
- [docs/rendering_pipeline.md](rendering_pipeline.md) - Frame sequence and stages
- [docs/hud_ui_mapping.md](hud_ui_mapping.md) - HUD rendering details
- [docs/effects_particle_mapping.md](effects_particle_mapping.md) - Particle effects
- [docs/camera_system_mapping.md](camera_system_mapping.md) - Camera and viewport

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2025-12-31 | Initial comprehensive documentation |

---

## References

### N64 Technical
- N64 Programming Manual (RDP/RSP chapters)
- F3DEX2 Microcode Reference
- libultra gbi.h header

### Arcade Source
- `reference/repos/rushtherock/game/visuals.c` - Visual effects
- `reference/repos/rushtherock/GUTS/3dfx/` - Glide wrapper

### Project Files
- `src/game/gfx.c` - Display list management
- `src/game/display.c` - Viewport setup
- `src/game/game.c` - Render functions (lines 21680-22100, 38000-39400)
- `include/types.h` - GBI constants and structures
