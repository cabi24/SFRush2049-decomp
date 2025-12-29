# Collision Detection System - N64 to Arcade Source Mapping

This document provides comprehensive documentation of the collision detection system in San Francisco Rush 2049 N64, mapped to the original Rush The Rock arcade source code.

## Table of Contents

1. [Overview](#overview)
2. [Collision Detection Algorithms](#collision-detection-algorithms)
3. [Data Structures](#data-structures)
4. [Function Mapping Table](#function-mapping-table)
5. [Car-to-Car Collision](#car-to-car-collision)
6. [Car-to-Wall/Environment Collision](#car-to-wallenvironment-collision)
7. [Collision Response Forces](#collision-response-forces)
8. [Key Global Variables](#key-global-variables)
9. [Coordinate Transform Functions](#coordinate-transform-functions)
10. [Damage System](#damage-system)

---

## Overview

The collision system uses a **two-phase approach** common in racing games:

1. **Broadphase**: Bounding sphere test for fast rejection
2. **Narrowphase**: Oriented bounding box (OBB) corner penetration test

The arcade source (`reference/repos/rushtherock/game/collision.c`) serves as the primary reference, with the N64 implementation adapted for the platform's constraints.

### Architecture Differences

| Aspect | Arcade | N64 |
|--------|--------|-----|
| Max cars | `MAX_LINKS` (linked machines) | `MAX_CARS` (8) |
| Data structure | `MODELDAT` (full physics) | `CarData` + `CollisionData` |
| Coordinate system | Body/World with UV matrices | Same, simplified transforms |
| Force application | Direct to physics engine | Via `CENTERFORCE`/`BODYFORCE` |

---

## Collision Detection Algorithms

### 1. Bounding Sphere Test (Fast Rejection)

Both N64 and arcade use identical sphere-overlap logic:

**Arcade (collision.c:199-204)**:
```c
/* see if bounding spheres intersect (vec = vector from other CG to our CG)*/
vecsub(m->reckon.RWR, m2->reckon.RWR, vec);
for(i=0, dsq=0.0; i<3; i++)
    dsq += (vec[i]*vec[i]);
if(dsq > ((m->colrad + m2->colrad) * (m->colrad + m2->colrad)))
    continue;   /* no overlap - test next model */
```

**N64 (collision.c:166-188)**:
```c
s32 check_sphere_collision(s32 car1, s32 car2, f32 *dist_sq) {
    CollisionData *c1, *c2;
    f32 dsq;
    f32 combined_rad;

    c1 = &col_data[car1];
    c2 = &col_data[car2];

    /* Calculate squared distance between car centers */
    dsq = vec_dist_sq(car_array[car1].RWR, car_array[car2].RWR);

    /* Check if spheres overlap */
    combined_rad = c1->colrad + c2->colrad;
    if (dsq > combined_rad * combined_rad) {
        return 0;  /* No overlap */
    }
    return 1;  /* Spheres overlap */
}
```

**Key constant**: Default collision radius is **6.0 feet** (`default_colrad`).

### 2. Corner-in-Body Test (OBB Penetration)

After sphere overlap is confirmed, the system tests if any of the 4 body corners of car A penetrate car B's bounding box.

**Arcade (collision.c:207-227)**:
```c
/* test if our corners in other */
for(i=0; i<4; i++) {
    fbodtorw(m->BODYR[i], pos, m->reckon.UV[0]);  /* Body to world */
    vecadd(pos, m->reckon.RWR, pos);
    vecsub(pos, m2->reckon.RWR, pos);
    frwtobod(pos, posr, m2->reckon.UV[0]);        /* World to other body */
    if(PointInBody(m2, posr)) {
        setFBCollisionForce(m, m, m2, vec, posr);
        return;
    }
}
```

**N64 (collision.c:351-362)**:
```c
/* Phase 2: Check body corners */
for (i = 0; i < 4; i++) {
    /* Transform body corner to world coords */
    body_to_world(col->BODYR[i], pos, car->dr_uvs, car->RWR);

    /* Check if corner is inside other car's body */
    if (point_in_body(other, pos)) {
        /* Collision detected! Apply forces */
        set_collision_force(car_index, other, vec, pos);
        return;
    }
}
```

### 3. Point-in-Body Test (AABB in Body Space)

**Arcade (collision.c:258-265)**:
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

**N64 (collision.c:198-227)**:
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

    /* Check if inside box */
    if (body_point[0] < -half_x || body_point[0] > half_x) return 0;
    if (body_point[1] < -half_y || body_point[1] > half_y) return 0;
    if (body_point[2] < -half_z || body_point[2] > half_z) return 0;
    return 1;
}
```

### 4. Separating Axis Theorem (SAT) - N64 Extended

The N64 version includes an additional SAT-based narrowphase for more accurate OBB collision:

**N64 (game.c:14806-14941)**:
```c
void collision_narrowphase(void *pairA, void *pairB) {
    /* Set up 15 potential separating axes:
     * 3 from A's local axes
     * 3 from B's local axes
     * 9 from cross products of A and B axes
     */

    /* A's local axes */
    for (i = 0; i < 3; i++) {
        axes[i][0] = rotA[i * 3 + 0];
        axes[i][1] = rotA[i * 3 + 1];
        axes[i][2] = rotA[i * 3 + 2];
    }

    /* Test all 15 axes for separation */
    for (i = 0; i < 15; i++) {
        /* Project both OBBs onto axis */
        /* If no overlap on any axis, no collision */
        if (overlap < 0.0f) {
            return;  /* Separating axis found */
        }
    }
    /* No separating axis - collision confirmed */
    collision_response_calc(pairA, pairB, contactNormal);
}
```

---

## Data Structures

### Arcade: MODELDAT (modeldat.h)

The arcade stores collision data directly in the physics model structure:

```c
typedef struct mdattyp {
    /* Body Axis Variables */
    F32 BODYFORCE[4][3];        /* Force at each body corner */
    F32 BODYR[4][3];            /* Body corner positions (body coords) */
    F32 peak_body_force[2][3];  /* Peak body forces */
    F32 CENTERFORCE[3];         /* Center of gravity force */
    F32 CENTERMOMENT[3];        /* Torque about CG */
    F32 peak_center_force[2][3];/* Peak center forces */

    /* Real World Axis Variables */
    F32 RWA[3], RWV[3], RWR[3]; /* Acceleration, Velocity, Position */
    uvect UV;                    /* Rotation matrix (unit vectors) */

    /* Collision specific */
    F32 colrad;                  /* Collision radius (bounding sphere) */
    S16 collidable;              /* Is this car collidable? */
    S16 in_game;                 /* Is this car in the race? */

    /* Dead reckoning for network */
    RECKON reckon;               /* Dead reckoned state */
} MODELDAT;
```

### N64: CollisionData (collision.h)

The N64 separates collision data into its own structure:

```c
typedef struct CollisionData {
    f32     colrad;             /* Collision radius (bounding sphere) */
    f32     BODYR[4][3];        /* Body corner positions (body coords) */
    f32     CENTERFORCE[3];     /* Center force vector */
    f32     BODYFORCE[4][3];    /* Force at each body corner */
    f32     peak_center_force[2][3]; /* Peak center forces */
    f32     peak_body_force[2][3];   /* Peak body forces */
    s8      collidable;         /* Is this car collidable? */
    s8      in_game;            /* Is this car in the game? */
    u8      pad[2];
} CollisionData;
```

### N64: CarData (structs.h)

Runtime car state includes collision-related fields:

```c
typedef struct CarData {
    /* Dead reckoned display coordinates */
    f32     dr_pos[3];          /* Dead reckoned position */
    f32     dr_vel[3];          /* Dead reckoned velocity */
    f32     dr_uvs[3][3];       /* Dead reckoned orientation */

    /* World coordinates */
    f32     RWV[3];             /* Real world velocity */
    f32     RWR[3];             /* Real world position */

    /* Collision state */
    s16     collidable;         /* Is collidable */
    u32     collide_time;       /* Collision timestamp */
    s8      collide_count;      /* Transparency countdown */
    s8      collide_state;      /* Transparency state */
} CarData;
```

### Body Corner Layout

```
         Z+ (forward)
          ^
          |
    [1]---+---[0]    BODYR indices:
     |    |    |     [0] = Front Right (+X, +Z)
X- <-+----+----+-> X+ [1] = Front Left  (-X, +Z)
     |    |    |     [2] = Rear Left   (-X, -Z)
    [2]---+---[3]    [3] = Rear Right  (+X, -Z)
          |
          v
         Z- (back)
```

Default body corners (half-sizes in feet):
```c
static const f32 default_body_corners[4][3] = {
    {  2.0f, 0.0f,  4.5f },  /* Front right */
    { -2.0f, 0.0f,  4.5f },  /* Front left */
    { -2.0f, 0.0f, -4.5f },  /* Rear left */
    {  2.0f, 0.0f, -4.5f }   /* Rear right */
};
```

---

## Function Mapping Table

| N64 Address | N64 Function | Arcade Function | File | Description |
|-------------|--------------|-----------------|------|-------------|
| `0x800BB000` | `collision_detect` | `collision()` | collision.c:163 | Main per-car collision check |
| `0x800BB400` | `collision_car_car` | `setFBCollisionForce()` | collision.c:341 | Car-to-car collision response |
| `0x800BB800` | `collision_car_wall` | `ForceApart()` | collision.c:306 | Force overlapping cars apart |
| `0x800BBC00` | `collision_point_in_body` | `PointInBody()` | collision.c:258 | Point inside bounding box test |
| N/A | `init_collision()` | `init_collision()` | collision.c:138 | Initialize collision data |
| N/A | `check_sphere_collision()` | (inline in collision) | collision.c:199 | Bounding sphere overlap test |
| N/A | `set_collision_force()` | `setCollisionForce()` | collision.c:257 | Calculate collision impulse |
| N/A | `set_collision_damage()` | `setCollisionDamage()` | collision.c:420 | Apply damage from collision |
| `0x80098FC0` | `world_collision` | N/A | N/A | World/environment collision |
| `0x8009DC50` | `track_collision` | `stree` functions | stree.c | Track surface collision |
| `0x8009DD18` | `track_collision_edge` | N/A | N/A | Track edge collision |
| `0x8009DD88` | `track_collision_wall` | N/A | N/A | Track wall collision |
| `0x8009EA70` | `track_collision_curb` | N/A | N/A | Curb/bump collision |
| `0x800C85F0` | `checkpoint_check` | `CheckCPs()` | checkpoint.c | Checkpoint collision |
| `0x80097798` | `vehicle_physics_sync` | N/A | N/A | Physics/collision sync |

### Additional N64 Collision Functions (game.c)

| Address | Function | Size | Description |
|---------|----------|------|-------------|
| `0x800EE88C` | `collision_check_thunk` | - | Wrapper to collision_response_calc |
| `0x800B8C14` | `collision_check` | - | Mode-based collision check |
| N/A | `collision_broadphase` | 744 bytes | Spatial partitioning grid |
| N/A | `collision_narrowphase` | 3448 bytes | SAT-based OBB collision |
| N/A | `collision_response_calc` | 2380 bytes | Impulse-based response |
| N/A | `entity_collision_test` | 2904 bytes | Full entity collision test |
| N/A | `entity_collision_detect` | 820 bytes | Entity collision detection |
| N/A | `entity_collision_response` | 876 bytes | Entity collision response |

---

## Car-to-Car Collision

### Main Detection Flow

```
collision(car_index)
    |
    +-- Check collidable flag
    |
    +-- Velocity sanity check (< 300 fps)
    |
    +-- For each other car:
        |
        +-- check_sphere_collision() [BROADPHASE]
        |
        +-- For each body corner:
            |
            +-- body_to_world() transform
            |
            +-- point_in_body() test [NARROWPHASE]
            |
            +-- set_collision_force() [RESPONSE]
```

### Collision Response Algorithm

**Arcade setFBCollisionForce() (collision.c:341-415)**:

The arcade uses a position-based force calculation:

```c
void setFBCollisionForce(MODELDAT *m, MODELDAT *m1, MODELDAT *m2,
                         F32 dir[3], F32 pos[3]) {
    F32 dist[6], force[3], temp[3], rvel[3], cent[3];
    BOOL beside, behind;

    /* Calculate distance to each face */
    dist[0] = m2->BODYR[0][0] - pos[0];  /* front */
    dist[1] = pos[0] - m2->BODYR[3][0];  /* back */
    dist[2] = m2->BODYR[0][1] - pos[1];  /* right */
    dist[3] = pos[1] - m2->BODYR[3][1];  /* left */
    dist[4] = COLL_BOT_DIST - pos[2];    /* bottom */
    dist[5] = pos[2] - COLL_TOP_DIST;    /* top */

    /* Check relative position */
    vecsub(m1->reckon.RWR, m2->reckon.RWR, temp);
    frwtobod(temp, cent, m2->reckon.UV[0]);
    beside = ((cent[0] < m2->BODYR[0][0]) && (cent[0] > m2->BODYR[3][0]));
    behind = ((cent[1] < m2->BODYR[0][1]) && (cent[1] > m2->BODYR[3][1]));

    /* Major overlap - emergency separation */
    if(beside && behind) {
        ForceApart(m, m1, m2, dir, pos);
        setCollisionDamage(m);
        return;
    }

    /* Get relative velocity in m2's frame */
    vecsub(m1->reckon.base_RWV, m2->reckon.base_RWV, temp);
    frwtobod(temp, rvel, m2->reckon.UV[0]);

    /* Calculate forces based on relative velocity */
    /* X force (front/back) */
    if(!beside) {
        force[0] = rvel[0] * 4000.0;
        if((pos[0] > 0.0) && (force[0] > -8000.0))
            force[0] = -8000.0;
        else if((pos[0] < 0.0) && (force[0] < 8000.0))
            force[0] = 8000.0;
    }

    /* Y force (side) */
    if(!behind) {
        force[1] = rvel[1] * 4000.0;
        /* ... similar clamping ... */
    }

    /* Z force (up/down) */
    force[2] = rvel[2] * 4000.0;

    /* Transform forces to correct frame */
    if(m == m2) {
        veccopy(force, m->CENTERFORCE);
    } else {
        /* Negate and transform for m1 */
        for(i=0; i<3; i++)
            force[i] = -force[i];
        fbodtorw(force, temp, m2->reckon.UV[0]);
        rwtobod(temp, m->CENTERFORCE, &m->UV);
    }

    setCollisionDamage(m);
}
```

### N64 Impulse-Based Response (game.c)

The N64 uses a more physically-accurate impulse-based model:

```c
void collision_response_calc(void *entityA, void *entityB, f32 *normal) {
    f32 *velA, *velB, *massA, *massB;
    f32 relVelX, relVelY, relVelZ, normalVel;
    f32 restitution, impulse, invMassSum;

    velA = (f32 *)((u8 *)entityA + 0x34);
    velB = (f32 *)((u8 *)entityB + 0x34);
    massA = (f32 *)((u8 *)entityA + 0x48);
    massB = (f32 *)((u8 *)entityB + 0x48);

    /* Calculate relative velocity */
    relVelX = velA[0] - velB[0];
    relVelY = velA[1] - velB[1];
    relVelZ = velA[2] - velB[2];

    /* Velocity component along collision normal */
    normalVel = relVelX * normal[0] + relVelY * normal[1] + relVelZ * normal[2];

    /* Objects separating - no response needed */
    if (normalVel > 0.0f) {
        return;
    }

    /* Coefficient of restitution (bounciness) */
    restitution = 0.3f;

    /* Calculate inverse mass sum */
    invMassSum = (1.0f / *massA) + (1.0f / *massB);

    /* Impulse magnitude: J = -(1+e) * Vrel / (1/mA + 1/mB) */
    impulse = -(1.0f + restitution) * normalVel / invMassSum;

    /* Apply impulse */
    velA[0] += impulse * normal[0] / *massA;
    velA[1] += impulse * normal[1] / *massA;
    velA[2] += impulse * normal[2] / *massA;

    velB[0] -= impulse * normal[0] / *massB;
    velB[1] -= impulse * normal[1] / *massB;
    velB[2] -= impulse * normal[2] / *massB;

    /* Friction impulse (Coulomb model) */
    /* ... tangential velocity calculation ... */
}
```

---

## Car-to-Wall/Environment Collision

### Track Collision Functions

The N64 has specialized functions for different track elements:

| Function | Address | Purpose |
|----------|---------|---------|
| `track_collision` | `0x8009DC50` | Main track surface collision |
| `track_collision_edge` | `0x8009DD18` | Track edge/boundary |
| `track_collision_wall` | `0x8009DD88` | Wall/barrier collision |
| `track_collision_curb` | `0x8009EA70` | Curb and bump collision |
| `track_collision_setup` | N/A | Initialize track collision data |

### Track Collision Data Structure

```c
/* Track collision data format (from game.c) */
struct TrackCollision {
    u32     magic;              /* 0x00: Identifier */
    u32     numVerts;           /* 0x04: Vertex count */
    u32     vertOffset;         /* 0x08: Offset to vertex array */
    u32     indexOffset;        /* 0x0C: Offset to index array */
    u32     flags;              /* 0x10: Collision flags */
};
```

### World Boundary Collision (N64)

```c
/* entity_collision_test boundary checks */
if (pos[0] - bounds[0] < worldMin[0]) {
    pos[0] = worldMin[0] + bounds[0];
    vel[0] = -vel[0] * 0.5f;  /* Bounce with 50% energy loss */
    *flags |= 0x0200;  /* Hit west wall */
}
if (pos[0] + bounds[0] > worldMax[0]) {
    pos[0] = worldMax[0] - bounds[0];
    vel[0] = -vel[0] * 0.5f;
    *flags |= 0x0400;  /* Hit east wall */
}
/* Similar for Z axis... */
```

### ForceApart - Emergency Separation

When cars are severely overlapping, `ForceApart()` applies a large emergency force:

**Arcade (collision.c:306-335)**:
```c
void ForceApart(MODELDAT *m, MODELDAT *m1, MODELDAT *m2,
                F32 dir[3], F32 pos[3]) {
    F32 invdist, force, temp[3];
    S32 i;

    /* Normalize direction vector */
    invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
    if(invdist < 0.0001) {
        /* Coincident centers - force based on slot order */
        MODELDAT *mother = (m == m1) ? m2 : m1;
        dir[0] = dir[2] = 0.0;
        dir[1] = (m->slot > mother->slot) ? 1.0 : -1.0;
        invdist = 1.0;
    } else {
        invdist = 1.0 / sqrt(invdist);
        for(i=0; i<3; i++)
            dir[i] = dir[i] * invdist;
    }

    /* Large separation force */
    force = 100000.0;

    for(i=0; i<3; i++)
        temp[i] = force * dir[i];
    rwtobod(temp, m->CENTERFORCE, &m->UV);
}
```

---

## Collision Response Forces

### Force Constants

**Arcade (collision.c:126-131)**:
```c
const F32 g_Kcs = (-10000.0);   /* Compression spring constant */
const F32 g_Krs = (-11000.0);   /* Rebound spring constant */
const F32 g_Kcd = (-50.0);      /* Compression damping */
const F32 g_Krd = (0.0);        /* Rebound damping */

#define MAXFORCE  4e3           /* Maximum collision force (lbs) */
#define MAX_VEL_SQ (300*300)    /* Max velocity squared (fps^2) */
```

**N64 (collision.h)**:
```c
#define MAXFORCE        4000.0f     /* Maximum collision force (lbs) */
#define MAX_VEL_SQ      (300*300)   /* Max velocity squared (fps^2) */
#define COL_SPRING_K    (-10000.0f) /* Spring constant for collisions */
#define COL_DAMP_K      (-50.0f)    /* Damping constant for collisions */
```

### Collision Algorithms

Three collision response algorithms are available:

```c
#define COLL_ALG_SOFTWALL 0  /* Spring-damper wall */
#define COLL_ALG_HARDWALL 1  /* Impulse-based wall */
#define COLL_ALG_CARBODY  2  /* Car body collision */
```

**collForceNormal() (arcade collision.c:590-628)**:
```c
F32 collForceNormal(MODELDAT *m, U32 coll_alg, F32 din, F32 vin) {
    F32 force;

    switch(coll_alg) {
        case COLL_ALG_SOFTWALL:
            if(vin < 0.0)   /* compression */
                force = (din * g_Kcs) - (vin * vin * g_Kcd);
            else            /* rebound */
                force = (din * g_Krs) + (vin * vin * g_Krd);
            force *= 2.0;
            if(force < 0.0) force = 0.0;
            break;

        case COLL_ALG_HARDWALL:
            vout = (din * 4.0) + 1.0; /* 5fps @ 1 foot */
            force = (vin + vout) * m->mass * 0.5 * m->idt;
            if(force < 0.0) force = 0.0;
            break;

        case COLL_ALG_CARBODY:
            force = 10000.0 * vin;
            if(force < 10000.0) force = 10000.0;
            if(force < 0.0) force = 0.0;
            break;
    }
    return(force);
}
```

### Force Application

Forces are stored in two arrays:

1. **CENTERFORCE[3]** - Force applied at center of gravity
2. **BODYFORCE[4][3]** - Force applied at each body corner

The physics integrator uses these to calculate linear acceleration and angular acceleration (torque).

---

## Key Global Variables

### N64 Collision Variables

| Variable | Address | Type | Description |
|----------|---------|------|-------------|
| `col_data[]` | TBD | `CollisionData[8]` | Collision data per car |
| `car_array[]` | `0x80152818` | `CarData[]` | Car runtime state |
| `collision_mode_flags[]` | TBD | `u8[2]` | Mode/state flags |
| `num_active_cars` | TBD | `s32` | Number of cars in race |

### Arcade Collision Variables

| Variable | Type | Description |
|----------|------|-------------|
| `model[]` | `MODELDAT[]` | Array of all car physics models |
| `MAX_LINKS` | `#define` | Maximum linked cars |
| `gThisNode` | `S16` | This machine's car index |
| `dlevels` | `U32` | Debug level flags |
| `SW_DEBUG10` | `#define` | Collision disable flag |
| `gDamageMask[]` | `U32[]` | Damage appearance bitmasks |
| `gDamageShift[]` | `S16[]` | Damage bit shift amounts |

### Collision Flags (N64)

```c
/* Entity collision flags (stored in flags field) */
#define COLL_ON_GROUND      0x0100  /* On track surface */
#define COLL_HIT_WEST       0x0200  /* Hit west boundary */
#define COLL_HIT_EAST       0x0400  /* Hit east boundary */
#define COLL_HIT_SOUTH      0x0800  /* Hit south boundary */
#define COLL_HIT_NORTH      0x1000  /* Hit north boundary */
#define COLL_HIT_ENTITY     0x2000  /* Hit another entity */
```

---

## Coordinate Transform Functions

### Arcade Transform Functions

```c
/* Body to Real World */
void bodtorw(F32 *bod, F32 *rw, uvect *uv);
void fbodtorw(F32 *bod, F32 *rw, F32 *uv);  /* Fast version */

/* Real World to Body */
void rwtobod(F32 *rw, F32 *bod, uvect *uv);
void frwtobod(F32 *rw, F32 *bod, F32 *uv);  /* Fast version */

/* Vector operations */
void vecadd(F32 *a, F32 *b, F32 *r);
void vecsub(F32 *a, F32 *b, F32 *r);
void veccopy(F32 *a, F32 *r);
void scalmul(F32 *a, F32 b, F32 *r);
F32 dotprod(F32 a[3], F32 b[3]);
void crossprod(F32 a[3], F32 b[3], F32 r[3]);
F32 magnitude(F32 vec[3]);
```

### N64 Transform Functions

```c
/* Body to World (collision.c:123-132) */
void body_to_world(f32 body_pos[3], f32 world_pos[3],
                   f32 uvs[3][3], f32 rwr[3]) {
    s32 i;
    for (i = 0; i < 3; i++) {
        world_pos[i] = rwr[i] +
                       body_pos[0] * uvs[0][i] +
                       body_pos[1] * uvs[1][i] +
                       body_pos[2] * uvs[2][i];
    }
}

/* World to Body (collision.c:142-155) */
void world_to_body(f32 world_pos[3], f32 body_pos[3],
                   f32 uvs[3][3], f32 rwr[3]) {
    f32 diff[3];
    s32 i;

    /* Get offset from body origin */
    for (i = 0; i < 3; i++) {
        diff[i] = world_pos[i] - rwr[i];
    }

    /* Dot with each unit vector row */
    body_pos[0] = diff[0] * uvs[0][0] + diff[1] * uvs[0][1] + diff[2] * uvs[0][2];
    body_pos[1] = diff[0] * uvs[1][0] + diff[1] * uvs[1][1] + diff[2] * uvs[1][2];
    body_pos[2] = diff[0] * uvs[2][0] + diff[1] * uvs[2][1] + diff[2] * uvs[2][2];
}
```

---

## Damage System

### Damage Levels

```c
#define DAMAGE1_FORCE (40000.0)   /* Minor damage threshold */
#define DAMAGE2_FORCE (155000.0)  /* Major damage threshold */
```

### Damage Quadrants

The car body is divided into 5 damage zones:

```
       +---[4: TOP]---+
      /               \
    [1]     [BODY]    [0]
     |               |
    [2]             [3]

Zone mapping:
  0 = Front Right
  1 = Front Left
  2 = Rear Left
  3 = Rear Right
  4 = Top
```

### Damage Calculation (arcade collision.c:420-458)

```c
void setCollisionDamage(MODELDAT *m) {
    S16 i, quad, damage, olddamage;
    F32 fsq, f[3], af[3];

    /* Calculate force magnitude squared */
    for(fsq=0.0, i=0; i<3; i++) {
        f[i] = m->CENTERFORCE[i];
        af[i] = (f[i] < 0.0) ? -f[i] : f[i];
        fsq += (f[i]*f[i]);
    }

    /* Determine damage level */
    if(fsq >= (DAMAGE2_FORCE * DAMAGE2_FORCE))
        damage = 2;
    else if(fsq >= (DAMAGE1_FORCE * DAMAGE1_FORCE))
        damage = 1;
    else
        return;  /* No damage */

    /* Determine which corner */
    if((f[2] > af[0]) && (f[2] > af[1]))  /* top */
        quad = 4;
    else if(f[0] > 0.0)  /* back */
        quad = (f[1] > 0.0) ? 3 : 2;
    else                 /* front */
        quad = (f[1] > 0.0) ? 1 : 0;

    /* Update appearance if damage increased */
    olddamage = (m->appearance & gDamageMask[quad]) >> gDamageShift[quad];
    if(damage > olddamage) {
        m->appearance &= ~gDamageMask[quad];
        m->appearance |= (damage << gDamageShift[quad]);
    }
}
```

### Appearance Flags (N64 structs.h)

```c
/* Car appearance flags */
#define APP_F_SMOKE         0x0001  /* Smoke effect */
#define APP_F_DAMAGE_RF     0x0010  /* Right front damage */
#define APP_F_DAMAGE_LF     0x0020  /* Left front damage */
#define APP_F_DAMAGE_RR     0x0040  /* Right rear damage */
#define APP_F_DAMAGE_LR     0x0080  /* Left rear damage */
#define APP_F_DAMAGE_TOP    0x0100  /* Top damage */
```

---

## Summary

The N64 collision system is a faithful adaptation of the arcade's Rush The Rock collision code, with the following key differences:

1. **Data Separation**: N64 separates collision data (`CollisionData`) from car physics data (`CarData`)
2. **Extended Narrowphase**: N64 adds SAT-based OBB collision for more accuracy
3. **Impulse-Based Response**: N64 uses modern impulse physics with restitution and friction
4. **Spatial Partitioning**: N64 adds grid-based broadphase for performance

Both versions share:
- Two-phase collision (sphere + OBB corner test)
- Same force constants and thresholds
- Same damage system with 5 zones
- Same coordinate transform approach

---

## References

### Source Files

**N64 Decompilation**:
- `/home/cburnes/projects/rush2049-decomp/src/game/collision.c` - Main collision implementation
- `/home/cburnes/projects/rush2049-decomp/src/game/game.c` - Extended collision functions
- `/home/cburnes/projects/rush2049-decomp/include/game/collision.h` - Collision header
- `/home/cburnes/projects/rush2049-decomp/include/game/structs.h` - Data structures

**Arcade Source (Rush The Rock)**:
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/collision.c` - Main collision
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/scollision.c` - Secondary collision
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/modeldat.h` - Data structures
- `/home/cburnes/projects/rush2049-decomp/reference/repos/rushtherock/game/vecmath.c` - Vector math

### Symbol Addresses
- `/home/cburnes/projects/rush2049-decomp/symbol_addrs.us.txt` - Function addresses
