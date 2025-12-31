# Track/Road Loading System Documentation

## Overview

The Rush 2049 N64 track loading system is derived from the arcade version (Rush The Rock). It handles track geometry, road surfaces, collision detection, and checkpoint placement. This document maps the arcade source code to the N64 decompilation.

## Key Source Files

### Arcade Source (reference/repos/rushtherock/)

| File | Description |
|------|-------------|
| `game/road.c` | Road/tire surface interaction |
| `game/stree.c` | Surface tree traversal, collision geometry |
| `game/stree.h` | Surface tree data structures |
| `game/checkpoint.c` | Checkpoint detection and lap counting |
| `game/cpinit.c` | Checkpoint initialization data per track |
| `game/collision.c` | Car-to-car and car-to-world collision |
| `game/maxpath.c` | AI pathfinding and track centerline |

### N64 Source (src/)

| File | Description |
|------|-------------|
| `src/game/road.c` | Road surface system (426 lines) |
| `src/game/track.c` | Track loading/management (825 lines) |
| `src/game/checkpoint.c` | Checkpoint system (735 lines) |
| `include/game/road.h` | Road definitions and types |
| `include/game/track.h` | Track structures and API |

---

## 1. Track Data Format

### Surface Tree Header (STHEADERTYP)

The track data begins with a header describing all surface geometry:

```c
typedef struct stheadertyp {
    S32 treebytes;          /* Total size of tree data */
    S32 tnum;               /* Number of tree nodes (STREETYP) */
    S32 snum;               /* Number of surfaces (SURFTYP) */
    S32 vnum;               /* Number of vertices (SVERTTYP) */
    S32 pnum;               /* Number of path points (PATHTYP) */
    S32 slnum;              /* Surface list count */
    S32 vlnum;              /* Vertex list count */
    S32 startpos[3];        /* Starting position */
    S16 startuvs[3][3];     /* Starting orientation (unit vectors) */
    S16 pathstart;          /* First path point index */
    S16 pathend;            /* Last path point index */
    S16 pathloop;           /* Loop point for lap boundary */
} STHEADERTYP;
```

### Data Layout in ROM

Track data is stored in a specific order after the header:

1. **STHEADERTYP** - Header (variable size)
2. **STREETYP[]** - Tree nodes (`hp->tnum` entries)
3. **SURFTYP[]** - Surface definitions (`hp->snum` entries)
4. **SVERTTYP[]** - Surface vertices (`hp->vnum` entries)
5. **PATHTYP[]** - Path/centerline points (`hp->pnum` entries)
6. **U16[] slist** - Surface index lists (`hp->slnum` entries)
7. **U16[] vlist** - Vertex index lists (`hp->vlnum` entries)

---

## 2. Road Segment Structure

### Surface Tree Node (STREETYP)

The collision geometry uses a quadtree for spatial partitioning:

```c
typedef struct streetyp {
    S32 parent;                 /* Parent node index (-1 for root) */
    S32 minh, maxh;             /* Horizontal bounds */
    S32 minv, maxv;             /* Vertical bounds */
    S32 child[4];               /* Child node indices or surface lists */
} STREETYP;
```

- Positive `child[]` values: Index to child STREETYP node
- Negative `child[]` values: Negated index into surface list (slist)

### Surface Definition (SURFTYP)

Each collidable surface polygon:

```c
typedef struct surftyp {
    U16 flags;          /* Surface type and behavior flags */
    U16 snd_flags;      /* Sound effect flags */
    U16 cut_flags;      /* Shortcut detection flags */
    S16 uvs[3][3];      /* Surface orientation (3x3 matrix) */
    S32 vli;            /* Vertex list index */
} SURFTYP;
```

**Surface Flags (`flags` field):**

| Mask | Value | Description |
|------|-------|-------------|
| `SURF_MASK` (0x000F) | | Surface type |
| `SURF_PAVE` | 0x0000 | Asphalt/pavement |
| `SURF_DIRT` | 0x0001 | Dirt/gravel |
| `SURF_COLLIDE` | 0x0005 | Wall collision |
| `SVIS_MASK` (0x00F0) | | Visual surface type |
| `SURF_BOOST_MASK` (0x0F00) | | Boost pad strength |
| `INTERP_BIT` (0x1000) | | Use path interpolation |

### Surface Vertex (SVERTTYP)

```c
typedef struct sverttyp {
    S32 pos[3];     /* Position (scaled by S4SCALE = 40) */
} SVERTTYP;
```

Positions are stored in fixed-point format with a scale factor of 40 (1 foot = 40 units).

---

## 3. Track Geometry Loading

### Initialization Function Mapping

| Arcade Function | N64 Function | Address |
|-----------------|--------------|---------|
| `init_stree()` | `track_segment_setup` | 0x8009DC50 |
| `tiresurf()` | `road_tire_surface` | via `track_collision` |
| `bodysurf()` | `road_body_surface` | via `track_collision` |
| `updownleaf()` | (inlined) | N/A |
| `downleaf()` | (inlined) | N/A |

### N64 Loading Process

```c
/* From symbol_addrs.us.txt */
track_segment_load = 0x800A1648;    /* Load track segment from ROM (2256 bytes) */
track_data_decompress = 0x800A1F18; /* Decompress GZIP/ZLIB/LZSS track data (644 bytes) */
track_collision_setup = 0x800A21A4; /* Setup 64x64 collision height/surface grids (732 bytes) */
```

The N64 version uses DEFLATE/GZIP compression for track data, decompressed at runtime to reduce ROM size.

### Arcade init_stree() Logic

```c
void init_stree(void) {
    STHEADERTYP *hp = (STHEADERTYP *)gRoadDat;
    gRoadHead = hp;

    /* Setup pointers to data sections */
    stree = (STREETYP *)(hp + 1);
    surf = (SURFTYP *)(stree + hp->tnum);
    svert = (SVERTTYP *)(surf + hp->snum);
    path = (PATHTYP *)(svert + hp->vnum);
    slist = (U16 *)(path + hp->pnum);
    vlist = (U16 *)(slist + hp->slnum);

    /* Setup path indices for lap detection */
    path_start = (U16)(hp->pathstart);
    path_end = (U16)(hp->pathend);
    path_loop = (U16)(hp->pathloop);
}
```

---

## 4. Collision Mesh Setup

### Quadtree Traversal

The arcade uses a quadtree (`updownleaf`/`downleaf`) for efficient surface lookup:

```c
STREETYP *updownleaf(STREETYP *tp, S32 pos[3], S16 *quad) {
    /* Check if position is outside current node bounds */
    S16 out = ((pos[HPOS] > tp->maxh) || (pos[HPOS] < tp->minh) ||
               (pos[VPOS] > tp->maxv) || (pos[VPOS] < tp->minv));

    /* Move up to parent if outside bounds */
    if (out && (tp->parent != -1))
        return updownleaf(&stree[tp->parent], pos, quad);
    else
        return downleaf(tp, pos, quad);
}

STREETYP *downleaf(STREETYP *tp, S32 pos[3], S16 *quad) {
    S32 midh = (tp->minh + tp->maxh) >> 1;
    S32 midv = (tp->minv + tp->maxv) >> 1;

    /* Determine quadrant */
    S16 q = (pos[HPOS] < midh) ? 0 : 1;
    if (pos[VPOS] < midv)
        q += 2;

    S32 child = tp->child[q];
    if (child > 0)  /* Not a leaf, recurse */
        return downleaf(&stree[child], pos, quad);
    else {          /* Found leaf */
        *quad = q;
        return tp;
    }
}
```

### N64 Collision Functions

| N64 Function | Address | Description |
|--------------|---------|-------------|
| `track_collision` | 0x8009DC50 | Main collision entry point |
| `track_collision_edge` | 0x8009DD18 | Edge collision helper |
| `track_collision_wall` | 0x8009DD88 | Wall collision helper |
| `track_collision_curb` | 0x8009EA70 | Curb/bump collision |
| `track_collision_test` | 0x8009F064 | Raycast against geometry (8600 bytes) |

### Surface-In-Polygon Test (mInSurf)

Tests if a point is within a surface polygon and calculates distance:

```c
S16 mInSurf(S32 ipos[3], S32 opos[3], SURFTYP *sp, S32 *dist) {
    /* Transform point to surface coordinate system */
    /* Perform 2D edge tests for all polygon edges */
    /* Return distance to surface in Z axis */
}
```

---

## 5. Checkpoint Placement

### Checkpoint Structure

**Arcade (checkpoint.h):**

```c
typedef struct {
    S32   pos[3];           /* World position (x, y, z) */
    F32   radius;           /* Trigger radius (squared feet) */
    S16   logical_cp;       /* Logical order (-1 = end marker) */
    F32   bonus_t[2];       /* Bonus time [first lap, other laps] */
    S16   track_cent;       /* Closest path point index */
    F32   uvs[3][3];        /* Orientation unit vectors */
} CheckPoint;
```

**N64 (track.h):**

```c
typedef struct TrackCheckpoint {
    f32     pos[3];             /* Position */
    f32     dir[3];             /* Direction vector */
    f32     width;              /* Checkpoint width */
    f32     radius;             /* Trigger radius */
    s16     logical_cp;         /* Logical order (0 = finish) */
    s16     next_cp;            /* Next checkpoint index */
    f32     bonus_time[NUM_DIFFICULTIES];   /* Bonus time per difficulty */
} TrackCheckpoint;
```

### Initialization Data (cpinit.c)

Checkpoint positions are defined in `cpinit.c`:

```c
const InitCheckPoint InitTrack0CPs[] = {
    {   /* Starting position */
        {-1630, 276, 5972},     /* pos[3] */
        250000,                  /* radius (squared feet) */
        0,                       /* logical checkpoint */
        {0, 0}                   /* best times [first, subsequent] */
    },
    {   /* checkpoint 1 */
        {-1621, 78, 2640},
        250000,
        1,
        {20, 0}                  /* 20 sec first lap bonus */
    },
    /* ... more checkpoints ... */
    {   /* null checkpoint (end marker) */
        {0, 0, 0}, 0, -1, {0, 0}
    }
};
```

### Checkpoint Function Mapping

| Arcade Function | N64 Function | Address |
|-----------------|--------------|---------|
| `init_cp_data()` | `init_cp_data` | 0x800C8000 |
| `InitCPS()` | `InitCPS` | 0x800C8300 |
| `CheckCPs()` | `checkpoint_check` | 0x800C85F0 |
| `PassedCP()` | `checkpoint_hit` | 0x801073D8 |
| `get_next_checkpoint()` | `get_next_checkpoint` | 0x800C8800 |
| `get_next_center()` | `get_next_center` | 0x800C8600 |
| `get_prev_center()` | `get_prev_center` | 0x800C8700 |

### Checkpoint Detection Algorithm

```c
void CheckCPs(void) {
    for (index = 0; index < num_active_cars; index++) {
        /* Get car position */
        for (i = 0; i < 3; i++)
            obs_posn[i] = car_array[index].dr_pos[i];

        /* Calculate difference from next checkpoint */
        for (i = 0; i < 3; i++) {
            diff[i] = obs_posn[i] - chkpnt[next_cp].pos[i];
            zvec[i] = chkpnt[next_cp].uvs[2][i];
        }

        /* Check distance (XZ plane) */
        cent_dist = diff[0]*diff[0] + diff[2]*diff[2];

        if (cent_dist < chkpnt[next_cp].radius) {
            /* Plane test: car passed if dot(diff, zvec) > 0 */
            if (DotVector(diff, zvec) > 0.0) {
                PassedCP(m, false);
            }
        }
    }
}
```

---

## 6. Track-Specific Settings

### Track Data Structure

**Arcade (Track_Data):**

```c
typedef struct {
    U32   start_time[8];      /* Initial time per difficulty */
    U32   end_time[8];        /* End time bonus per difficulty */
    S16   loop_chkpnt;        /* Checkpoint to loop to for laps */
    S16   finish_line;        /* Finish line checkpoint index */
    S16   before_finish;      /* Checkpoint before finish */
    S16   number_of_laps;     /* Total laps for race */
} Track_Data;
```

**N64 (TrackTiming):**

```c
typedef struct TrackTiming {
    f32     start_time[NUM_DIFFICULTIES];   /* Initial time per difficulty */
    f32     target_time[NUM_DIFFICULTIES];  /* Target finish time */
    f32     par_time;           /* Par time for track */
    f32     record_time;        /* Track record */
    s16     num_laps;           /* Number of laps */
    s16     loop_checkpoint;    /* Checkpoint to loop to */
    s16     finish_line;        /* Finish line checkpoint index */
    s16     before_finish;      /* Checkpoint before finish */
} TrackTiming;
```

### Path Point Structure (PATHTYP)

Used for AI navigation and surface interpolation:

```c
typedef struct pathtyp {
    F32 fpos[3];        /* World position */
    F32 fuvs[3][3];     /* Orientation matrix */

    /* Surface smoothing data */
    F32 ve[3];          /* Edge0 vector */
    F32 vl[3];          /* Left tangent vector */
    F32 vr[3];          /* Right tangent vector */
    F32 fwidth;         /* Road width */
    F32 w;              /* X width at Y=0 */
    F32 m0, m1;         /* X/Y slopes at pos and next */
    F32 wl, ml;         /* Left Y width and slope */
    F32 wr, mr;         /* Right Y width and slope */
    F32 m0l, m0r;       /* Left/right surface loft at edge0 */
    F32 m1l, m1r;       /* Left/right surface loft at edge1 */

    /* Integer versions (legacy) */
    S32 pos[3];
    S16 uvs[3][3];
    S16 width;
} PATHTYP;
```

---

## 7. N64 vs Arcade Differences

### Memory Management

| Aspect | Arcade | N64 |
|--------|--------|-----|
| Memory | Direct access to ROM/RAM | DMA from ROM, limited RAM |
| Track data | Uncompressed | GZIP/DEFLATE compressed |
| Loading | Instant | Streaming with progress |
| Surfaces | Full resolution | Potentially simplified |

### Coordinate System

Both systems use the same coordinate convention:
- **X**: East/West (positive = East)
- **Y**: Up/Down (positive = Up)
- **Z**: North/South (positive = North)
- **Scale**: 1 foot = 40 units in fixed-point

### Surface Type Codes

| Code | Arcade | N64 |
|------|--------|-----|
| 0 | SURF_PAVE / PAVE | ROAD_ASPHALT |
| 1 | SURF_DIRT | ROAD_DIRT |
| 2 | SVIS_WATER | ROAD_GRASS |
| 3 | AIR | ROAD_AIR |
| 4 | - | ROAD_WATER |
| 5 | SURF_COLLIDE | ROAD_ICE |
| 6 | - | ROAD_SAND |
| 7 | (boost value) | ROAD_BOOST |

### API Differences

**Arcade road interaction:**
```c
/* Per-frame update */
road(MODELDAT *m);

/* Tire surface detection */
tiresurf(m, tire_pos, ground_pos, &roadcode, uvs, tire_index);

/* Body corner collision */
bodysurf(m, body_pos, body_vel, uvs, corner_index);
```

**N64 road interaction:**
```c
/* Per-frame update */
road_update(CarPhysics *m);

/* Tire surface detection */
road_tire_surface(m, tire_pos, ground_pos, &roadcode, uvs, tire_index);

/* Body corner collision */
road_body_surface(m, body_pos, body_vel, uvs, corner_index);
```

### Track Loading Flow

**Arcade:**
1. Load roaddat from hard drive
2. Call `init_stree()` to parse header and set pointers
3. Call `init_cp_data()` to initialize checkpoints

**N64:**
1. Read compressed track segment from ROM via DMA
2. Decompress using DEFLATE/GZIP (`track_data_decompress`)
3. Initialize collision grid (`track_collision_setup`)
4. Initialize checkpoints (`init_cp_data`)

---

## Key N64 Address Reference

### Track/Road Functions

| Function | Address | Size | Description |
|----------|---------|------|-------------|
| `track_render` | 0x800992AC | - | Track generation/rendering |
| `track_geometry_process` | 0x8009C8F0 | 5368 | Track geometry processing |
| `track_segment_setup` | 0x8009DC50 | - | Track segment initialization |
| `track_texture_setup` | 0x8009DD18 | - | Track texture loading |
| `track_segment_render` | 0x8009DD88 | 3304 | Render single track segment |
| `track_bounds_check` | 0x8009EA70 | 168 | Check position within boundaries |
| `track_height_query` | 0x8009EB18 | 168 | Get terrain height at position |
| `track_surface_type` | 0x8009EBC0 | 1188 | Get surface material type |
| `track_collision_test` | 0x8009F064 | 8600 | Raycast against track geometry |
| `track_segment_load` | 0x800A1648 | 2256 | Load track segment from ROM |
| `track_data_decompress` | 0x800A1F18 | 644 | Decompress track data |
| `track_collision_setup` | 0x800A21A4 | 732 | Setup collision grids |
| `track_process_main` | 0x800A2D4C | 3116 | Per-frame track processing |
| `track_spline_interp` | 0x800A3654 | 296 | Catmull-Rom spline interpolation |
| `track_render_process` | 0x800A377C | 3156 | Per-frame track rendering |

### Track Data Variables

| Variable | Address | Description |
|----------|---------|-------------|
| `gTrackDataA` | 0x8014A110 | Track data array A |
| `gTrackDataB` | 0x8014A250 | Track data array B |
| `gTrackBounds` | 0x8015C000 | Track bounds (minX, minZ, etc.) |
| `gTrackNo` | 0x80152A90 | Current selected track index |
| `gTrackLocked` | 0x80152A80 | Track selection locked flag |

### Checkpoint Data Variables

| Variable | Address | Description |
|----------|---------|-------------|
| `gNumberCheckpoints` | 0x80142B44 | Total checkpoints on track |
| `gLastCheckpointTime` | 0x80142B48 | Time of last checkpoint hit |
| `gGlobalLaps` | 0x80142B4C | Global lap counter |
| `gGlobalLastCp` | 0x80142B4E | Last checkpoint passed |
| `gGlobalNextCp` | 0x80142B50 | Next checkpoint to reach |
| `gLapLoopIndex` | 0x80142B52 | Checkpoint index for lap loop |
| `gTrackCPs` | 0x8014B100 | Runtime checkpoint data |
| `gCP_Track` | 0x8014B800 | Per-track checkpoint info |
| `gPathDist` | 0x80153200 | Cumulative path distance array |
| `gPathIndex` | 0x80153E00 | Path point indices |

---

## Summary

The N64 track loading system closely mirrors the arcade implementation while adapting to N64 hardware constraints:

1. **Compression**: Track data is DEFLATE-compressed in ROM
2. **Streaming**: Data is loaded via DMA rather than direct memory access
3. **Simplification**: Some path interpolation features may be reduced
4. **Memory layout**: Same fundamental data structures (STREETYP, SURFTYP, PATHTYP)
5. **Checkpoint logic**: Nearly identical plane-test algorithm

The primary reference files for decompilation work are:
- `reference/repos/rushtherock/game/stree.c` - Surface tree and collision
- `reference/repos/rushtherock/game/road.c` - Road interaction
- `reference/repos/rushtherock/game/checkpoint.c` - Checkpoint detection
- `reference/repos/rushtherock/game/cpinit.c` - Checkpoint initialization data
