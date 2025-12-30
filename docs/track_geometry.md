# Track Geometry System Documentation

This document describes the track/road geometry system used in San Francisco Rush 2049 for N64, based on analysis of both the N64 decompilation and the Rush The Rock arcade source code.

## Overview

The track system in Rush 2049 consists of several interconnected components:

1. **Surface Tree (stree)** - Spatial partitioning for efficient surface lookup
2. **Road Surfaces (surf)** - Individual surface polygons with properties
3. **Path Points** - Center-line of the track for AI and position calculation
4. **Checkpoints** - Race progress triggers
5. **Maxpath** - AI drone driving paths

## Track Data Format

### File Structure

Track data is stored in a binary format loaded from ROM. The arcade source reveals the header structure:

```c
typedef struct stheadertyp {
    S32 treebytes;      /* Size of tree data in bytes */
    S32 tnum;           /* Number of tree nodes */
    S32 snum;           /* Number of surfaces */
    S32 vnum;           /* Number of vertices */
    S32 pnum;           /* Number of path points */
    S32 slnum;          /* Number of surface list entries */
    S32 vlnum;          /* Number of vertex list entries */
    S32 startpos[3];    /* Start position (fixed point) */
    S16 startuvs[3][3]; /* Start orientation matrix */
    S16 pathstart;      /* First path point index */
    S16 pathend;        /* Last path point index */
    S16 pathloop;       /* Loop point for lap counting */
} STHEADERTYP;
```

Data sections follow the header in order:
1. **Tree nodes** (STREETYP array)
2. **Surfaces** (SURFTYP array)
3. **Vertices** (SVERTTYP array)
4. **Path points** (PATHTYP array)
5. **Surface lists** (U16 array)
6. **Vertex lists** (U16 array)

### Initialization

```c
void init_stree(void) {
    STHEADERTYP *hp = (STHEADERTYP *)gRoadDat;

    stree = (STREETYP *)(hp + 1);
    surf = (SURFTYP *)(stree + hp->tnum);
    svert = (SVERTTYP *)(surf + hp->snum);
    path = (PATHTYP *)(svert + hp->vnum);
    slist = (U16 *)(path + hp->pnum);
    vlist = (U16 *)(slist + hp->slnum);

    path_start = hp->pathstart;
    path_end = hp->pathend;
    path_loop = hp->pathloop;
}
```

## Road Segment Representation

### N64 Implementation

The N64 version uses `RoadSegment` structures:

```c
typedef struct RoadSegment {
    f32     start_pos[3];       /* Segment start position */
    f32     end_pos[3];         /* Segment end position */
    f32     width;              /* Road width */
    f32     banking;            /* Road banking angle */
    u8      surface_type;       /* Primary surface type */
    u8      flags;              /* Segment flags */
    u16     next_segment;       /* Index of next segment */
    u16     alt_segment;        /* Alternate path segment */
    u16     checkpoint_id;      /* Associated checkpoint */
} RoadSegment;
```

### Arcade Implementation (Surface Tree Node)

```c
typedef struct streetyp {
    S32 parent;         /* Parent node index */
    S32 minh, maxh;     /* Horizontal bounds (X axis) */
    S32 minv, maxv;     /* Vertical bounds (Y axis) */
    S32 child[4];       /* Child nodes (quadtree) */
} STREETYP;
```

Child values:
- Positive: index to child tree node
- Negative: negated index to surface list
- Zero: no data

### Surface Definition

```c
typedef struct surftyp {
    U16 flags;          /* Surface type and rendering flags */
    U16 snd_flags;      /* Sound effect flags */
    U16 cut_flags;      /* Shortcut detection flags */
    S16 uvs[3][3];      /* Surface orientation matrix */
    S32 vli;            /* Vertex list index */
} SURFTYP;
```

### Path Point Structure

Path points define the track centerline for AI and position calculation:

```c
typedef struct pathtyp {
    /* Floating point data for physics */
    F32 fpos[3];        /* World position */
    F32 fuvs[3][3];     /* Orientation matrix */

    /* Surface interpolation data */
    F32 ve[3];          /* Edge vector */
    F32 vl[3];          /* Left tangent vector */
    F32 vr[3];          /* Right tangent vector */
    F32 fwidth;         /* Road width (left to right) */
    F32 w;              /* X width at y=0 */
    F32 m0, m1;         /* X/Y slopes at current/next pos */
    F32 wl, wr;         /* Left/right Y widths */
    F32 ml, mr;         /* Left/right Y/X slopes */
    F32 m0l, m0r;       /* Left/right surface loft (edge 0) */
    F32 m1l, m1r;       /* Left/right surface loft (edge 1) */

    /* Integer data (fixed point, scaled by 40) */
    S32 pos[3];         /* Position (world units * 40) */
    S16 uvs[3][3];      /* Orientation (fixed point) */
    S16 width;          /* Road width (fixed point) */
} PATHTYP;
```

## Surface Types and Properties

### Surface Type Codes

| Code | Name | N64 Define | Description |
|------|------|------------|-------------|
| 0 | PAVEMENT | ROAD_ASPHALT | Normal road surface |
| 1 | DIRT | ROAD_DIRT | Dirt/gravel, lower friction |
| 2 | WATER | ROAD_WATER | Water, low friction |
| 3 | - | ROAD_AIR | Airborne (no surface) |
| 4 | - | ROAD_WATER | Water hazard |
| 5 | COLLIDE | ROAD_ICE | Wall collision / Ice |
| 6 | - | ROAD_SAND | Sand surface |
| 7 | CRASH | ROAD_BOOST | Boost pad |
| 8 | - | ROAD_JUMP | Jump ramp |
| 9 | - | ROAD_WALL | Wall collision |
| 10 | - | ROAD_SHORTCUT | Shortcut surface |

### Friction Coefficients

```c
static const f32 friction_table[] = {
    1.0f,   /* ASPHALT - full grip */
    0.7f,   /* DIRT - reduced grip */
    0.5f,   /* GRASS - low grip */
    0.0f,   /* AIR - no friction */
    0.3f,   /* WATER - very low grip */
    0.2f,   /* ICE - minimal grip */
    0.4f,   /* SAND - moderate low grip */
    1.2f,   /* BOOST - enhanced grip */
    1.0f,   /* JUMP - normal grip */
    0.0f,   /* WALL - no friction */
    1.0f,   /* SHORTCUT - normal grip */
};
```

### Surface Flags

```c
/* Surface mask bits (arcade stree.h) */
#define SURF_MASK       0x000F  /* Surface type (0-15) */
#define SVIS_MASK       0x00F0  /* Visual surface type */
#define SVIS_SHIFT      4
#define SURF_BOOST_MASK 0x0F00  /* Boost strength */
#define SURF_BOOST_SHIFT 8
#define INTERP_BIT      0x1000  /* Use path interpolation */

/* Shortcut flags */
#define S_CUT_MASK      0x8000  /* Is shortcut */
#define NOWRONGWAY_MASK 0x4000  /* Don't trigger wrong-way */
#define YANK_MASK       0x0300  /* Respawn behavior */
#define SC_DIST_MASK    0x003F  /* Shortcut distance value */

/* Sound flags (reverb) */
#define UP2_REVERB_MASK 0x8000
#define UP_REVERB_MASK  0x4000
#define LF_REVERB_MASK  0x2000
#define RT_REVERB_MASK  0x1000
```

### N64 Surface Flags

```c
#define ROAD_FLAG_BOOST     0x01  /* Surface provides boost */
#define ROAD_FLAG_SHORTCUT  0x02  /* On shortcut path */
#define ROAD_FLAG_BUMPY     0x04  /* Bumpy surface */
#define ROAD_FLAG_WET       0x08  /* Wet surface */
#define ROAD_FLAG_RUMBLE    0x10  /* Rumble strip */
```

## Track Boundaries and Walls

### Wall Collision Detection

The arcade uses a separate collision surface type (`SURF_COLLIDE`) for walls:

```c
void bodysurf(MODELDAT *m, F32 ipos[3], F32 ivel[3], F32 uvs[3][3], int whl) {
    // ... surface lookup ...

    if (maxdist != MAXSURFPOS && maxdist > -10) {
        switch (savesp->flags & SURF_MASK) {
            case SURF_COLLIDE:
                /* Wall collision - apply sideways force */
                xyvel = xxsqrt(tvel[0]*tvel[0] + tvel[1]*tvel[1]);
                force = (opos[2] + 0.25) * m->weight * 1.5;
                m->BODYFORCE[whl][0] = -force * (tvel[0]/xyvel);
                m->BODYFORCE[whl][1] = -force * (tvel[1]/xyvel);
                break;

            default:  /* DIRT or PAVE */
                /* Ground collision - apply upward force */
                // ...
        }
    }
}
```

### Collision Force Calculation

```c
/* Wall collision algorithm (stree.c) */
F32 COLL_test4(MODELDAT *m, F32 r[3], F32 v[3]) {
    F32 din = r[2];  /* Penetration depth */
    F32 vin = v[2];  /* Velocity into surface */

    if (din > 0.25) {
        /* Deep penetration - force velocity to 1fps out */
        force = (vin + 1.0) * m->mass * m->idt * -0.5;
    } else {
        /* Spring + damping force */
        /* Ks = 16000, Kd = 2000 */
        force = ((din + 0.25) * -16000.0) + (vin * -2000.0);
    }

    if (force > 0.0) force = 0.0;  /* Only push away */
    return force;
}
```

## Shortcut Detection

### Shortcut Flags

Each surface can have shortcut flags indicating:
- Whether it's a shortcut surface
- Distance credit for taking the shortcut
- Wrong-way detection behavior

```c
/* Check shortcut status */
s32 road_is_shortcut(u8 roadcode) {
    return (roadcode == ROAD_SHORTCUT);
}
```

### Shortcut Path Tracking

The arcade maintains separate path distances for shortcuts:

```c
extern S16 path_to_maxpath[MAX_PATH_POINTS];
extern U16 maxpath_dist[MAXMPATH];
```

Cars on shortcuts get distance credit based on the main path equivalent:

```c
/* From maxpath.c */
maxpath_to_path[i];  /* Maps maxpath index to main path */
maxpath_dist[i];     /* Cumulative distance at each maxpath point */
```

## LOD (Level of Detail) for Track Rendering

The N64 version likely implements LOD for track geometry, though specific details are not fully decompiled. Key observations:

### Surface Tree Culling

The quadtree structure allows efficient culling:
- Tree traversal eliminates invisible sections
- Depth-based LOD selection is possible via tree level

### Visual Surface Type

The `SVIS_MASK` separate from `SURF_MASK` suggests different visual representations:

```c
#define SVIS_PAVE    0x0000  /* Visual: pavement */
#define SVIS_DIRT    0x0010  /* Visual: dirt */
#define SVIS_WATER   0x0020  /* Visual: water */
#define SVIS_COLLIDE 0x0050  /* Visual: collision */
```

This allows physics surfaces and visual surfaces to differ (e.g., invisible walls).

## Key Data Structures

### N64 TrackData

```c
typedef struct TrackData {
    s32         num_segments;       /* Total road segments */
    RoadSegment *segments;          /* Segment array */
    f32         total_length;       /* Track total length */
    s32         num_checkpoints;    /* Number of checkpoints */
    s32         num_laps;           /* Laps for this track */
    u8          track_id;           /* Track identifier */
    u8          difficulty;         /* Track difficulty */
    u8          has_shortcuts;      /* Track has shortcuts */
    u8          pad;
} TrackData;
```

### N64 SurfaceInfo

```c
typedef struct SurfaceInfo {
    f32     ground_pos[3];      /* Ground position under tire */
    f32     ground_normal[3];   /* Surface normal vector */
    f32     friction;           /* Surface friction coefficient */
    u8      roadcode;           /* Surface type code */
    u8      flags;              /* Surface flags */
    u8      boost_amount;       /* Boost strength if applicable */
    u8      pad;
} SurfaceInfo;
```

### Arcade MPHEADER (AI Path Header)

```c
typedef struct MPHEADER {
    S32 num_points;     /* Number of path points */
    S32 mpath_active;   /* Path is active */
    S8  priority;       /* Path priority for drones */
    S8  dummy;
    S16 lap_start;      /* Lap start point index */
    S16 lap_end;        /* Lap end point index */
    S16 num_in_lap;     /* Points in one lap */
    S32 spare[2];
} MPHEADER;
```

### Arcade MPATH (AI Path Point)

```c
typedef struct MPATH {
    F32 pos[3];         /* World position */
    F32 speed;          /* Target speed at this point */
    S32 hints;          /* Driving hints for AI */
    S16 base_to_path;   /* Base path to this path offset */
    S16 path_to_base;   /* This path to base path offset */
} MPATH;
```

## Surface Lookup Algorithm

### Tire Surface Detection (tiresurf)

```c
void tiresurf(MODELDAT *m, F32 ipos[3], F32 opos[3],
              S32 *roadcode, F32 uvs[3][3], int whl) {

    /* Convert to fixed point for tree lookup */
    for (i = 0; i < 3; i++)
        tpos[i] = ipos[i] * S4SCALE;  /* S4SCALE = 40 */

    /* Traverse tree from previous leaf */
    ltp = updownleaf(m->lasttp[whl], tpos, &quad);
    m->lasttp[whl] = ltp;  /* Cache for next frame */

    /* Get surface list from leaf */
    sli = -((S32)ltp->child[quad]);
    if (sli) {
        slp = &slist[sli];
        snum = *slp++;

        maxdist = MAXSURFPOS;  /* -10000 */
        for (i = 0; i < snum; i++) {
            sp = &surf[*slp++];
            if (mInSurf(tpos, topos, sp, &dist)) {
                /* Find highest surface below tire */
                if (dist < MAXSURFHOP && dist > maxdist) {
                    maxdist = dist;
                    savesp = sp;
                }
            }
        }

        if (maxdist != MAXSURFPOS) {
            /* Use surface interpolation if flagged */
            if (savesp->flags & INTERP_BIT)
                fPathInterp(savesp, ipos, opos, uvs);
            else
                fSurfDist(savesp, ipos, opos, uvs);

            *roadcode = savesp->flags & SURF_MASK;
            return;
        }
    }

    /* No surface found - use default ground */
    *roadcode = DIRT;
    opos[2] = ipos[2] - 200.0;  /* Default plane at -200ft */
}
```

## Coordinate Systems

### Fixed Point Scaling

The arcade uses fixed-point arithmetic with a scale factor of 40:

```c
#define S4SCALE 40

/* Converting float to fixed */
tpos[i] = ipos[i] * S4SCALE;

/* Converting fixed to float */
world_pos = fixed_pos / 40.0;
```

### Coordinate Mapping

The arcade uses a different axis convention than typical 3D:
- X: Lateral (left/right)
- Y: Vertical (up/down)
- Z: Forward (track direction)

Path points in the arcade are stored with swapped axes:
```c
chkpnt->pos[0] = path[i].pos[1] / 40;   /* Arcade Y -> World X */
chkpnt->pos[1] = -path[i].pos[2] / 40;  /* Arcade -Z -> World Y */
chkpnt->pos[2] = path[i].pos[0] / 40;   /* Arcade X -> World Z */
```

## Track File Format Insights

Based on arcade code analysis:

1. **Binary Format**: Track data is stored as packed binary structures
2. **Fixed Point**: Positions use 40x scale factor (0.025 foot precision)
3. **Quadtree Spatial Index**: Surface tree uses quadtree for O(log n) lookup
4. **Vertex Lists**: Surfaces reference shared vertex pools
5. **Path Interpolation**: Smooth surfaces use cubic interpolation data
6. **Multiple AI Paths**: Up to 8 different paths for drone variety
7. **Checkpoint Binding**: Checkpoints linked to nearest path points

### Loading Sequence

1. Load binary track data from ROM
2. Parse header for section sizes
3. Set up pointers to each section
4. Initialize path start/end/loop indices
5. Optionally mirror track (negate X coordinates)

## References

- `/home/cburnes/projects/rush2049-decomp/src/game/road.c` - N64 road implementation
- `/home/cburnes/projects/rush2049-decomp/src/game/collision.c` - N64 collision system
- `/home/cburnes/projects/rush2049-decomp/include/game/road.h` - N64 road structures
- `/home/cburnes/projects/rush2049-decomp/include/game/checkpoint.h` - N64 checkpoint structures
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/stree.c` - Arcade surface tree
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/stree.h` - Arcade surface structures
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/road.c` - Arcade road system
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/road.h` - Arcade road defines
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/checkpoint.c` - Arcade checkpoints
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/checkpoint.h` - Arcade checkpoint structures
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/maxpath.c` - Arcade AI paths
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/maxpath.h` - Arcade maxpath structures
