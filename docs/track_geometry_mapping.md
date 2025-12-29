# Track/Road Geometry System Mapping

This document maps the arcade Rush The Rock track/road geometry system to the N64 Rush 2049 decompilation.

## Overview

The Rush track geometry system consists of several interconnected components:

1. **Surface Tree (stree)** - Spatial partitioning for efficient surface lookup
2. **Road Surfaces** - Individual surface polygons with physics properties
3. **Path System** - Racing line/centerline for checkpoints and AI
4. **Checkpoints** - Race progress markers tied to path positions
5. **Maxpath** - AI drone path recording and playback

The arcade uses a quadtree-based spatial index (stree) to quickly find which road surfaces are under each tire, then calculates suspension compression, friction, and collision forces.

## Data Structures

### Surface Tree Header (STHEADERTYP)

From `reference/repos/rushtherock/game/stree.h`:

```c
typedef struct stheadertyp {
    S32 treebytes;      /* Size of tree data */
    S32 tnum;           /* Number of tree nodes */
    S32 snum;           /* Number of surfaces */
    S32 vnum;           /* Number of vertices */
    S32 pnum;           /* Number of path points */
    S32 slnum;          /* Number of surface list entries */
    S32 vlnum;          /* Number of vertex list entries */
    S32 startpos[3];    /* Start position */
    S16 startuvs[3][3]; /* Start orientation */
    S16 pathstart;      /* First path index */
    S16 pathend;        /* Last path index */
    S16 pathloop;       /* Loop-back path index */
} STHEADERTYP;
```

### Tree Node (STREETYP)

Quadtree node for spatial partitioning:

```c
typedef struct streetyp {
    S32 parent;           /* Parent node index (-1 for root) */
    S32 minh, maxh;       /* Horizontal bounds */
    S32 minv, maxv;       /* Vertical bounds */
    S32 child[4];         /* Child indices (negative = surface list) */
} STREETYP;
```

The `child[4]` array holds either:
- Positive value: Index of child tree node
- Negative value: Negated index into surface list (leaf node)

### Surface (SURFTYP)

Individual road surface polygon:

```c
typedef struct surftyp {
    U16 flags;        /* Road surface flags (SURF_PAVE, SURF_DIRT, etc.) */
    U16 snd_flags;    /* Sound effect flags */
    U16 cut_flags;    /* Shortcut flags */
    S16 uvs[3][3];    /* Surface orientation matrix */
    S32 vli;          /* Vertex list index */
} SURFTYP;
```

### Vertex (SVERTTYP)

Surface vertex:

```c
typedef struct sverttyp {
    S32 pos[3];    /* Position (scaled by S4SCALE = 40) */
} SVERTTYP;
```

### Path Point (PATHTYP)

Racing line waypoint:

```c
typedef struct pathtyp {
    /* Floating point data for surface smoothing */
    F32 fpos[3];        /* Position */
    F32 fuvs[3][3];     /* Orientation matrix */
    F32 ve[3];          /* Edge vector */
    F32 vl[3];          /* Left tangent vector */
    F32 vr[3];          /* Right tangent vector */
    F32 fwidth;         /* Road width (left to right) */
    F32 w;              /* X width at Y=0 */
    F32 m0, m1;         /* X/Y slope at pos and next pos */
    F32 wl, ml;         /* Y left width and slope */
    F32 wr, mr;         /* Y right width and slope */
    F32 m0l, m0r;       /* Left/right surface loft (edge0) */
    F32 m1l, m1r;       /* Left/right surface loft (edge1) */

    /* Fixed-point data (for saving/loading) */
    S32 pos[3];         /* Position (scaled) */
    S16 uvs[3][3];      /* Orientation */
    S16 width;          /* Road width */
} PATHTYP;
```

## Surface Type Definitions

### Road Surface Codes

From `reference/repos/rushtherock/game/road.h`:

| Code | Name | Description |
|------|------|-------------|
| 0 | PAVEMENT | Regular pavement (default) |
| 1 | DIRT | Dirt, low friction |
| 2 | WATER | Water, low friction |
| 5 | COLLIDE | Immovable collision object |
| 6 | AIR | Airborne (no ground contact) |
| 7 | CRASH | Crash trigger |

### Surface Flags

From `reference/repos/rushtherock/game/stree.h`:

```c
#define SURF_MASK       0x000F    /* Surface type mask */
#define SURF_PAVE       0x0000    /* Pavement */
#define SURF_DIRT       0x0001    /* Dirt */
#define SURF_COLLIDE    0x0005    /* Collision wall */

#define SVIS_MASK       0x00F0    /* Visual surface type */
#define SVIS_SHIFT      4
#define SVIS_PAVE       0x0000
#define SVIS_DIRT       0x0010
#define SVIS_WATER      0x0020
#define SVIS_COLLIDE    0x0050

#define SURF_BOOST_MASK  0x0F00   /* Speed boost level (0-15) */
#define SURF_BOOST_SHIFT 8

#define INTERP_BIT      0x1000    /* Use path interpolation */

/* Shortcut/wrong-way flags */
#define S_CUT_MASK      0x8000    /* Shortcut marker */
#define NOWRONGWAY_MASK 0x4000    /* Disable wrong-way warning */
#define YANK_MASK       0x0300    /* Yank direction */
#define SC_DIST_MASK    0x003F    /* Shortcut distance */

/* Reverb/sound flags */
#define UP2_REVERB_MASK 0x8000
#define UP_REVERB_MASK  0x4000
#define LF_REVERB_MASK  0x2000
#define RT_REVERB_MASK  0x1000
#define ALL_REVERB_MASK (UP2_REVERB_MASK|UP_REVERB_MASK|LF_REVERB_MASK|RT_REVERB_MASK)
```

## Key Arcade Functions

### Initialization Functions

| Function | File | Description |
|----------|------|-------------|
| `init_stree()` | stree.c | Initialize surface tree from loaded road data |
| `init_road()` | road.c | Initialize car tire/body positions |
| `init_cp_data()` | checkpoint.c | Initialize checkpoint positions from path |
| `InitCPS()` | checkpoint.c | Initialize race checkpoint state |
| `InitMaxPath()` | maxpath.c | Initialize AI drone path data |

### Per-Frame Functions

| Function | File | Description |
|----------|------|-------------|
| `road()` | road.c | Main road interaction (calls tiresurf + bodysurf) |
| `tiresurf()` | stree.c | Find surface under each tire |
| `bodysurf()` | stree.c | Check body corner collisions |
| `CheckCPs()` | checkpoint.c | Check checkpoint crossings |
| `find_maxpath_intervals()` | maxpath.c | Update AI path positions |

### Surface Lookup Functions

| Function | File | Description |
|----------|------|-------------|
| `updownleaf()` | stree.c | Traverse tree up/down to find leaf |
| `downleaf()` | stree.c | Traverse tree downward only |
| `mInSurf()` | stree.c | Test if point is inside surface |
| `fSurfDist()` | stree.c | Get distance from surface |
| `fPathInterp()` | stree.c | Interpolate position along path |

### Checkpoint Functions

| Function | File | Description |
|----------|------|-------------|
| `get_next_checkpoint()` | checkpoint.c | Get next checkpoint in sequence |
| `get_next_center()` | checkpoint.c | Get next path center point |
| `get_prev_center()` | checkpoint.c | Get previous path center point |
| `PassedCP()` | checkpoint.c | Handle checkpoint crossing |
| `check_everyones_checkpoints()` | checkpoint.c | Update global leader state |

## N64 Function Mapping

### Currently Implemented (src/game/)

| N64 File | N64 Function | Arcade Equivalent | Status |
|----------|--------------|-------------------|--------|
| road.c | `road_init()` | `init_stree()` | Stub |
| road.c | `init_road()` | `road.c:init_road()` | Partial |
| road.c | `road_update()` | `road.c:road()` | Partial |
| road.c | `road_tire_surface()` | `stree.c:tiresurf()` | Stub (flat ground only) |
| road.c | `road_body_surface()` | `stree.c:bodysurf()` | Stub (flat ground only) |
| road.c | `uvinterp()` | `road.c:uvinterp()` | Complete |
| road.c | `vecinterp()` | `road.c:vecinterp()` | Complete |
| checkpoint.c | `InitCPS()` | `checkpoint.c:InitCPS()` | Partial |
| checkpoint.c | `CheckCPs()` | `checkpoint.c:CheckCPs()` | Partial |
| checkpoint.c | `PassedCP()` | `checkpoint.c:PassedCP()` | Partial |
| checkpoint.c | `get_next_checkpoint()` | Same | Complete |
| checkpoint.c | `get_next_center()` | Same | Complete |
| checkpoint.c | `get_prev_center()` | Same | Complete |
| track.c | `track_init()` | Track loading | Stub |
| track.c | `track_load()` | ROM loading | Stub |

### Missing/Needed

| Arcade Function | Purpose | Priority |
|-----------------|---------|----------|
| `updownleaf()` | Quadtree traversal | High |
| `downleaf()` | Quadtree traversal | High |
| `mInSurf()` | Point-in-polygon test | High |
| `fSurfDist()` | Surface distance calculation | High |
| `fPathInterp()` | Path interpolation | Medium |
| `AlignSurf()` | Surface alignment | Medium |
| `frwtobod()` | Real-world to body coords | Medium |
| `fbodtorw()` | Body to real-world coords | Medium |
| `COLL_force()` | Collision force calculation | High |

## Track Loading Process

### Arcade Track Loading

1. **Load Road Data**
   - `gRoadDat` points to loaded track road data
   - Contains header + tree + surfaces + vertices + paths + lists

2. **Initialize Surface Tree**
   - `init_stree()` parses header and sets up pointers:
     - `stree` = tree nodes
     - `surf` = surface array
     - `svert` = vertex array
     - `path` = path points
     - `slist` = surface index lists
     - `vlist` = vertex index lists
     - `path_start`, `path_end`, `path_loop` = path bounds

3. **Initialize Checkpoints**
   - `init_cp_data()` finds path points nearest to checkpoint positions
   - Calculates checkpoint orientations from path direction
   - Builds path distance tables for position calculation

### N64 Track Loading (Expected)

1. **DMA Read** - Copy compressed track data from ROM
2. **Decompress** - DEFLATE/LZSS decompression
3. **Parse Header** - Extract counts and offsets
4. **Set Up Pointers** - Point to surfaces, vertices, etc.
5. **Initialize State** - Reset car positions, checkpoints

## Data Flow

### Tire-Surface Interaction

```
1. Get tire world position (TIRERWR)
2. Scale to fixed-point (pos * S4SCALE)
3. Traverse quadtree from last leaf (updownleaf)
4. Get surface list for quadrant
5. For each surface in list:
   - Transform position to surface space (irwtobod)
   - Check if inside polygon (mInSurf)
   - Track closest surface above tire
6. If surface found:
   - Get surface type (roadcode)
   - Get visual type (sviscode)
   - Get boost value (roadboost)
   - Calculate suspension compression
   - Store surface UVs for friction direction
7. If no surface:
   - Set roadcode = AIR
   - Default to DIRT visual
```

### Checkpoint Detection

```
1. Get car position (obs_posn)
2. Get next checkpoint (next_cp_index)
3. Calculate difference from checkpoint center
4. Check distance within radius
5. If close enough:
   - Dot product with checkpoint Z-vector
   - If > 0, car passed checkpoint plane
   - Call PassedCP() to update state
```

## Scale Factors

- **S4SCALE = 40** - Position fixed-point scale
- **INVS4SCALE = 1/40 = 0.025** - Inverse scale
- **Unit vector scale = 0x4000 (16384)** - Short to float conversion

## Coordinate Systems

### Real World Coordinates
- X: East-West (model Y)
- Y: Up (negative model Z)
- Z: North-South (model X)

### Model/Body Coordinates
- X: Right (arcade game coords)
- Y: Forward
- Z: Up

### Path Coordinate Transform
Path positions use different axis mapping:
```c
pos[0] = path[i].pos[1]/40;   /* Model Y -> World X */
pos[1] = -path[i].pos[2]/40;  /* Negated Model Z -> World Y */
pos[2] = path[i].pos[0]/40;   /* Model X -> World Z */
```

## Memory Layout (Arcade)

```
gRoadDat:
+0x00: STHEADERTYP header
+sizeof(header): STREETYP stree[header.tnum]
+...: SURFTYP surf[header.snum]
+...: SVERTTYP svert[header.vnum]
+...: PATHTYP path[header.pnum]
+...: U16 slist[header.slnum]
+...: U16 vlist[header.vlnum]
```

## Implementation Notes

### N64 Differences Expected

1. **Fixed-Point Math** - N64 likely uses more fixed-point for performance
2. **Simplified Trees** - May use simpler spatial partitioning for RAM limits
3. **Compressed Storage** - Track data likely heavily compressed in ROM
4. **Fewer Path Points** - May have reduced path resolution
5. **Simplified Collision** - Body collision may be simplified

### Key Areas to Match

1. `tiresurf()` - Tire/ground contact is critical for physics feel
2. Checkpoint plane test - Must match arcade for proper lap counting
3. Surface friction - Must match for consistent handling
4. Path interpolation - Affects AI behavior and wrong-way detection

## References

- `reference/repos/rushtherock/game/road.c` - Road initialization
- `reference/repos/rushtherock/game/stree.c` - Surface tree code
- `reference/repos/rushtherock/game/stree.h` - Data structures
- `reference/repos/rushtherock/game/checkpoint.c` - Checkpoint system
- `reference/repos/rushtherock/game/checkpoint.h` - Checkpoint structures
- `reference/repos/rushtherock/game/maxpath.c` - AI path system
- `reference/repos/rushtherock/game/maxpath.h` - AI path structures
- `reference/repos/rushtherock/game/modeldat.h` - Car/model data structures
