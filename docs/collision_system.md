# Rush 2049 Collision Detection System

This document describes the collision detection and physics response system used in San Francisco Rush 2049 for N64, based on analysis of decompiled code and the Rush The Rock arcade source.

## Overview

Rush 2049 uses a multi-phase collision detection system optimized for arcade racing:

1. **Bounding Sphere Test** - Fast rejection of non-colliding car pairs
2. **Body Corner Test** - Detailed AABB-based collision detection
3. **Surface Tree (STREE)** - Track/wall collision via hierarchical spatial partitioning
4. **Spring-Damper Response** - Physics-based collision response

The system is derived from the Milliken Engineering vehicle dynamics model, with arcade-friendly simplifications.

## File Structure

| File | Purpose |
|------|---------|
| `src/game/collision.c` | Car-to-car collision detection and response |
| `src/game/physics.c` | Vehicle physics including collision force integration |
| `src/game/car.c` | Car parameter definitions including collision boxes |
| `include/game/collision.h` | Collision data structures and constants |
| `include/game/physics.h` | Physics structures including body forces |
| `include/game/structs.h` | Game data structures |

**Arcade Source References:**
- `reference/repos/rushtherock/game/collision.c` - Original collision detection
- `reference/repos/rushtherock/game/cars.c` - Car collision box definitions
- `reference/repos/rushtherock/game/stree.c` - Surface tree for track collision
- `reference/repos/rushtherock/game/modeldat.h` - MODELDAT structure with collision fields

---

## Data Structures

### CollisionData (Per-Car Collision State)

```c
typedef struct CollisionData {
    f32     colrad;             /* Collision radius (bounding sphere) */
    f32     BODYR[4][3];        /* Body corner positions (body coords) */
    f32     CENTERFORCE[3];     /* Center force vector */
    f32     BODYFORCE[4][3];    /* Force at each body corner */
    f32     peak_center_force[2][3]; /* Peak center forces [min/max] */
    f32     peak_body_force[2][3];   /* Peak body forces [min/max] */
    s8      collidable;         /* Is this car collidable? */
    s8      in_game;            /* Is this car in the game? */
    u8      pad[2];
} CollisionData;
```

Located in: `/home/cburnes/projects/rush2049-decomp/include/game/collision.h`

### Body Corner Layout

The `BODYR[4][3]` array defines the 4 corners of the collision box in body-local coordinates:

```
Corner Index    Position (X, Y, Z)         Description
-----------     ------------------         -----------
0               (+half_x, 0, +half_z)      Front right
1               (-half_x, 0, +half_z)      Front left
2               (-half_x, 0, -half_z)      Rear left
3               (+half_x, 0, -half_z)      Rear right
```

Default values:
```c
static const f32 default_body_corners[4][3] = {
    {  2.0f, 0.0f,  4.5f },  /* Front right */
    { -2.0f, 0.0f,  4.5f },  /* Front left */
    { -2.0f, 0.0f, -4.5f },  /* Rear left */
    {  2.0f, 0.0f, -4.5f }   /* Rear right */
};
```

### Arcade MODELDAT Collision Fields

From `reference/repos/rushtherock/game/modeldat.h`:

```c
/* Body Axis Variables */
F32     BODYFORCE[4][3],BODYR[4][3];
F32     peak_body_force[2][3];
F32     CENTERFORCE[3],CENTERMOMENT[3];
F32     peak_center_force[2][3];
F32     colrad;     /* collision radius */
S16     collidable; /* true when drone/car is collidable */
```

---

## Collision Detection Algorithms

### Phase 1: Bounding Sphere Test

The fastest rejection test. Checks if two cars' bounding spheres overlap.

```c
s32 check_sphere_collision(s32 car1, s32 car2, f32 *dist_sq) {
    CollisionData *c1, *c2;
    f32 dsq, combined_rad;

    c1 = &col_data[car1];
    c2 = &col_data[car2];

    /* Calculate squared distance between car centers */
    dsq = vec_dist_sq(car_array[car1].RWR, car_array[car2].RWR);

    if (dist_sq) {
        *dist_sq = dsq;
    }

    /* Check if spheres overlap */
    combined_rad = c1->colrad + c2->colrad;
    if (dsq > combined_rad * combined_rad) {
        return 0;  /* No overlap */
    }

    return 1;  /* Spheres overlap */
}
```

**Arcade equivalent** (collision.c:163-205):
```c
/* see if bounding spheres intersect */
vecsub(m->reckon.RWR, m2->reckon.RWR, vec);
for(i=0,dsq=0.0;i<3;i++)
    dsq += (vec[i]*vec[i]);
if(dsq > ((m->colrad + m2->colrad) * (m->colrad + m2->colrad)))
    continue;   /* no overlap - test next model */
```

### Phase 2: Body Corner Test

If spheres overlap, test each body corner against the other car's bounding box.

```c
s32 point_in_body(s32 car_index, f32 point[3]) {
    CollisionData *col;
    CarData *car;
    f32 body_point[3];
    f32 half_x, half_y, half_z;

    col = &col_data[car_index];
    car = &car_array[car_index];

    /* Transform point to body coordinates */
    world_to_body(point, body_point, car->dr_uvs, car->RWR);

    /* Get half-sizes from body corners */
    half_x = col->BODYR[0][0];  /* Right side = positive X */
    half_y = 2.0f;              /* Fixed height */
    half_z = col->BODYR[0][2];  /* Front = positive Z */

    /* Check if inside box (AABB test) */
    if (body_point[0] < -half_x || body_point[0] > half_x) return 0;
    if (body_point[1] < -half_y || body_point[1] > half_y) return 0;
    if (body_point[2] < -half_z || body_point[2] > half_z) return 0;

    return 1;
}
```

**Arcade equivalent** (collision.c:243-250):
```c
#define COLL_TOP_DIST (-5.0)
#define COLL_BOT_DIST (1.0)

BOOL PointInBody(MODELDAT *m, F32 pt[3]) {
    if((pt[0] > m->BODYR[0][0]) || (pt[0] < m->BODYR[3][0])) return(0);
    if((pt[1] > m->BODYR[0][1]) || (pt[1] < m->BODYR[3][1])) return(0);
    if((pt[2] > COLL_BOT_DIST) || (pt[2] < COLL_TOP_DIST)) return(0);
    return(1);
}
```

### Phase 3: Main Collision Loop

```c
void collision(s32 car_index) {
    /* For each other car */
    for (other = 0; other < num_active_cars; other++) {
        if (other == car_index) continue;
        if (!col2->in_game || !col2->collidable) continue;

        /* Phase 1: Bounding sphere test */
        if (!check_sphere_collision(car_index, other, &dsq)) continue;

        /* Calculate vector from other car to this car */
        for (i = 0; i < 3; i++) {
            vec[i] = car->RWR[i] - car2->RWR[i];
        }

        /* Phase 2: Check our body corners against other's box */
        for (i = 0; i < 4; i++) {
            body_to_world(col->BODYR[i], pos, car->dr_uvs, car->RWR);
            if (point_in_body(other, pos)) {
                set_collision_force(car_index, other, vec, pos);
                return;
            }
        }
    }
}
```

---

## Collision Response

### Spring-Damper Force Model

The collision response uses a spring-damper model to calculate separation forces:

```
F = Ks * penetration + Kd * relative_velocity
```

Where:
- `Ks` = Spring constant (`COL_SPRING_K = -10000.0f`)
- `Kd` = Damping constant (`COL_DAMP_K = -50.0f`)
- Maximum force clamped to `MAXFORCE = 4000.0f`

```c
void set_collision_force(s32 car1, s32 car2, f32 force[3], f32 point[3]) {
    /* Calculate relative velocity */
    for (i = 0; i < 3; i++) {
        rel_vel[i] = d1->RWV[i] - d2->RWV[i];
    }

    /* Normalize direction vector */
    dist = force[0]*force[0] + force[1]*force[1] + force[2]*force[2];
    if (dist > 0.001f) {
        dist = 1.0f / sqrtf(dist);
        for (i = 0; i < 3; i++) normal[i] = force[i] * dist;
    }

    /* Calculate velocity component along collision normal */
    vel_along_normal = rel_vel[0] * normal[0] +
                       rel_vel[1] * normal[1] +
                       rel_vel[2] * normal[2];

    /* Spring + damping force */
    force_mag = COL_SPRING_K * 0.5f + COL_DAMP_K * vel_along_normal;

    /* Clamp force magnitude */
    if (force_mag > MAXFORCE) force_mag = MAXFORCE;
    if (force_mag < -MAXFORCE) force_mag = -MAXFORCE;

    /* Apply force to center */
    for (i = 0; i < 3; i++) {
        c1->CENTERFORCE[i] += normal[i] * force_mag;
    }
}
```

**Arcade equivalent** (collision.c:341-415 `setFBCollisionForce()`):

```c
void setFBCollisionForce(MODELDAT *m, MODELDAT *m1, MODELDAT *m2,
                         F32 dir[3], F32 pos[3]) {
    /* Get rel velocity from m1 to m2 into m2 frame */
    vecsub(m1->reckon.base_RWV, m2->reckon.base_RWV, temp);
    frwtobod(temp, rvel, m2->reckon.UV[0]);

    /* Calculate forces based on relative velocity */
    if (beside)
        force[0] = 0.0;
    else {
        force[0] = rvel[0] * 4000.0;
        if ((pos[0] > 0.0) && (force[0] > -8000.0))
            force[0] = -8000.0;
        else if ((pos[0] < 0.0) && (force[0] < 8000.0))
            force[0] = 8000.0;
    }
    /* ... similar for Y and Z */
}
```

### Force Apart (Unstuck)

When cars are deeply penetrating (major overlap), a large separation force is applied:

```c
void ForceApart(s32 car1, s32 car2, f32 dir[3]) {
    /* Normalize direction */
    if (invdist < 0.0001f) {
        /* If coincident centers, force based on slot order */
        dir[0] = dir[2] = 0.0f;
        dir[1] = (car1 > car2) ? 1.0f : -1.0f;
    }

    /* Large separation force */
    force = 100000.0f;

    for (i = 0; i < 3; i++)
        temp[i] = force * dir[i];

    world_to_body(temp, c1->CENTERFORCE, d1->dr_uvs, d1->RWR);
}
```

---

## Car-to-Track Collision

Track collision uses a hierarchical surface tree (STREE) for efficient spatial queries.

### Surface Tree Structure (Arcade)

From `reference/repos/rushtherock/game/stree.c`:

```c
typedef struct {
    /* Tree nodes */
    STREETYP    *stree;     /* Tree node array */
    SURFTYP     *surf;      /* Surface polygons */
    SVERTTYP    *svert;     /* Surface vertices */
    PATHTYP     *path;      /* Path data for AI */
    U16         *slist;     /* Surface list indices */
    U16         *vlist;     /* Vertex list indices */
} SurfaceTree;
```

### Tire-Surface Collision

```c
void tiresurf(MODELDAT *m, F32 ipos[3], F32 opos[3],
              S32 *roadcode, F32 uvs[3][3], int whl) {
    /* Scale position for tree lookup */
    for (i = 0; i < 3; i++)
        tpos[i] = ipos[i] * S4SCALE;

    /* Traverse tree from previous leaf */
    ltp = updownleaf(m->lasttp[whl], tpos, &quad);
    m->lasttp[whl] = ltp;

    /* Get surface list for this quadrant */
    sli = -((S32)ltp->child[quad]);
    if (sli) {
        slp = &slist[sli];
        snum = *slp++;

        /* Find closest intersecting surface */
        maxdist = MAXSURFPOS;
        for (i = 0; i < snum; i++) {
            sp = &surf[*slp++];
            if (mInSurf(tpos, topos, sp, &dist)) {
                if ((dist < MAXSURFHOP) && (dist > maxdist)) {
                    maxdist = dist;
                    savesp = sp;
                }
            }
        }
    }
}
```

### Wall Collision Response

Wall collisions use a different algorithm based on surface type flags:

```c
/* Surface type flags */
#define SURF_NORMAL    0   /* Driveable surface */
#define SURF_COLLIDE   1   /* Wall/barrier */
#define SURF_SLIPPERY  2   /* Ice/oil */
#define SURF_BOOST     3   /* Speed boost pad */
```

---

## Physics Integration

Collision forces are integrated into the main physics loop via `CENTERFORCE` and `BODYFORCE`:

```c
void physics_forces2(CarPhysics *m) {
    /* Sum tire forces */
    vec_add(m->TIREFORCE[0], m->TIREFORCE[1], m->F);
    vec_add(m->TIREFORCE[2], m->F, m->F);
    vec_add(m->TIREFORCE[3], m->F, m->F);

    /* Add gravity in body frame */
    rw_to_body(m->GRW, m->G, &m->UV);
    vec_add(m->G, m->F, m->F);

    /* Add drag */
    vec_add(m->D, m->F, m->F);

    /* Add body corner forces (collision) */
    vec_add(m->BODYFORCE[0], m->F, m->F);
    vec_add(m->BODYFORCE[1], m->F, m->F);
    vec_add(m->BODYFORCE[2], m->F, m->F);
    vec_add(m->BODYFORCE[3], m->F, m->F);

    /* Add centering force (collision response) */
    vec_add(m->CENTERFORCE, m->F, m->F);
}
```

### Moment Calculation

Collision forces at body corners generate rotational moments:

```c
void physics_torques(CarPhysics *m) {
    /* Zero moment */
    m->M[0] = m->M[1] = m->M[2] = 0.0f;

    /* Add moment from body corners: M = r x F */
    for (i = 0; i < 4; i++) {
        vec_copy(m->TIRER[i], temp1);
        vec_cross(temp1, m->BODYFORCE[i], temp);
        vec_add(m->M, temp, m->M);
    }

    /* Add anti-spin moment (stabilization) */
    vec_add(m->CENTERMOMENT, m->M, m->M);
}
```

---

## Function Call Hierarchy

```
check_all_collisions()
    |
    +-> collision(car_index)
        |
        +-> check_sphere_collision(car1, car2)   [Fast rejection]
        |
        +-> body_to_world()                       [Transform corners]
        |
        +-> point_in_body()                       [AABB test]
        |
        +-> set_collision_force()                 [Response calc]
            |
            +-> ForceApart()                      [If major overlap]
            |
            +-> set_collision_damage()            [Damage effects]

apply_collision_forces(car_index)
    |
    +-> Integrate CENTERFORCE into velocity

physics_sym()
    |
    +-> physics_forces2()                         [Sum all forces]
    |
    +-> physics_torques()                         [Calculate moments]
    |
    +-> physics_accelerations()                   [F = ma]
    |
    +-> physics_velocities()                      [Integrate accel]
    |
    +-> physics_positions()                       [Integrate velocity]
```

---

## Constants and Tuning

### Collision Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `MAXFORCE` | 4000.0 lbs | Maximum collision force |
| `MAX_VEL_SQ` | 90000 (300^2) | Max velocity squared (fps^2) |
| `COL_SPRING_K` | -10000.0 | Spring constant |
| `COL_DAMP_K` | -50.0 | Damping constant |
| `COLL_TOP_DIST` | -5.0 ft | Collision box top |
| `COLL_BOT_DIST` | 1.0 ft | Collision box bottom |
| `default_colrad` | 6.0 ft | Default bounding sphere radius |

### Arcade Collision Box Sizes (cars.c)

```c
const COLLSIZE collsize[NUM_CARS] = {
    { 3.0f, 6.0f, 2.8f, 2.0f },   /* Car 0: colfront, colrear, colside, colheight */
    { 3.0f, 6.0f, 2.8f, 2.0f },   /* Car 1 */
    { 3.2f, 6.5f, 3.0f, 2.2f },   /* Car 2: Larger muscle car */
    /* ... */
};
```

### Collision Radius Calculation (initiali.c:219)

```c
maxlen = (sizep->colfront > sizep->colrear) ? sizep->colfront : sizep->colrear;
m->colrad = sqrt((maxlen * maxlen) +
                 (sizep->colside * sizep->colside) +
                 (sizep->colheight * sizep->colheight));
```

---

## Damage System

Collision forces above threshold trigger visual damage:

```c
#define DAMAGE1_FORCE (40000.0)   /* Minor damage threshold */
#define DAMAGE2_FORCE (155000.0)  /* Major damage threshold */

void set_collision_damage(s32 car_index) {
    /* Calculate force magnitude */
    force_mag = sqrtf(col->CENTERFORCE[0] * col->CENTERFORCE[0] +
                      col->CENTERFORCE[1] * col->CENTERFORCE[1] +
                      col->CENTERFORCE[2] * col->CENTERFORCE[2]);

    /* Determine damage level */
    if (fsq >= (DAMAGE2_FORCE * DAMAGE2_FORCE))
        damage = 2;
    else if (fsq >= (DAMAGE1_FORCE * DAMAGE1_FORCE))
        damage = 1;

    /* Determine damage quadrant from force direction */
    if (f[2] > af[0] && f[2] > af[1])
        quad = 4;   /* Top */
    else if (f[0] > 0.0)
        quad = (f[1] > 0.0) ? 3 : 2;   /* Rear */
    else
        quad = (f[1] > 0.0) ? 1 : 0;   /* Front */

    /* Update appearance flags */
    m->appearance |= (damage << gDamageShift[quad]);
}
```

---

## Coordinate Transforms

### Body to World

```c
void body_to_world(f32 body_pos[3], f32 world_pos[3],
                   f32 uvs[3][3], f32 rwr[3]) {
    for (i = 0; i < 3; i++) {
        world_pos[i] = rwr[i] +
                       body_pos[0] * uvs[0][i] +
                       body_pos[1] * uvs[1][i] +
                       body_pos[2] * uvs[2][i];
    }
}
```

### World to Body

```c
void world_to_body(f32 world_pos[3], f32 body_pos[3],
                   f32 uvs[3][3], f32 rwr[3]) {
    /* Get offset from body origin */
    for (i = 0; i < 3; i++) {
        diff[i] = world_pos[i] - rwr[i];
    }

    /* Dot with each unit vector row (transpose multiply) */
    body_pos[0] = diff[0]*uvs[0][0] + diff[1]*uvs[0][1] + diff[2]*uvs[0][2];
    body_pos[1] = diff[0]*uvs[1][0] + diff[1]*uvs[1][1] + diff[2]*uvs[1][2];
    body_pos[2] = diff[0]*uvs[2][0] + diff[1]*uvs[2][1] + diff[2]*uvs[2][2];
}
```

---

## Collidable State Management

Cars can be temporarily non-collidable (after respawn, during certain game states):

```c
void set_car_collidable(s32 car_index, s32 collidable) {
    col_data[car_index].collidable = collidable ? 1 : 0;
}

void set_car_in_game(s32 car_index, s32 in_game) {
    col_data[car_index].in_game = in_game ? 1 : 0;
}
```

From arcade `sresurrect.c`:
- After crash: `m->collidable = false;` for 5 seconds
- Flashing transparency indicates non-collidable state
- Prevents respawn griefing in multiplayer

---

## Global Variables

| Variable | Address | Description |
|----------|---------|-------------|
| `col_data[]` | - | Per-car collision state array |
| `car_array[]` | 0x80152818 | Car runtime data (positions, velocities) |
| `num_active_cars` | - | Number of cars currently in race |
| `gstate` | 0x801146EC | Current game state |

---

## Cross-References to Arcade Source

| N64 Function | Arcade Equivalent | File |
|--------------|-------------------|------|
| `init_collision()` | `init_collision()` | collision.c:138 |
| `collision()` | `collision()` | collision.c:163 |
| `point_in_body()` | `PointInBody()` | collision.c:243 |
| `set_collision_force()` | `setFBCollisionForce()` | collision.c:341 |
| `ForceApart()` | `ForceApart()` | collision.c:306 |
| `set_collision_damage()` | `setCollisionDamage()` | collision.c:420 |
| `tiresurf()` | `tiresurf()` | stree.c:243 |

---

## Future Work

1. **Track Collision** - The STREE (surface tree) system needs full decompilation
2. **Wall Response** - Wall collision forces use different algorithms than car-car
3. **Slip Detection** - Tire slip affects friction during collision
4. **Wings/Downforce** - Rush 2049 has car wings that affect physics

---

*Document generated from analysis of:*
- `/home/cburnes/projects/rush2049-decomp/src/game/collision.c`
- `/home/cburnes/projects/rush2049-decomp/src/game/physics.c`
- `/home/cburnes/projects/rush2049-decomp/src/game/car.c`
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/collision.c`
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/cars.c`
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/stree.c`
