# Rush 2049 N64 - Physics Collision System Mapping

## Overview

The Rush 2049 collision detection and response system is based on the original arcade Rush The Rock source code (`reference/repos/rushtherock/game/collision.c` and related files). The system uses a two-phase detection approach:

1. **Bounding Sphere Test** - Fast rejection test using collision radii
2. **Bounding Box Test** - Detailed corner-in-body penetration detection

The collision response applies spring/damping forces to separate overlapping vehicles and calculates visual damage based on impact severity.

## Architecture

### Arcade Source Files

| File | Purpose |
|------|---------|
| `game/collision.c` | Car-to-car collision detection and response |
| `game/stree.c` | Surface tree collision (car-to-wall, car-to-surface) |
| `game/drivsym.c` | Physics simulation, crash detection, force summation |
| `game/resurrect.c` | Crash handling, respawn system |
| `game/cars.c` | Collision box dimensions per car model |

### N64 Source Files

| File | Purpose |
|------|---------|
| `src/game/collision.c` | Car-to-car collision (decompiled) |
| `src/game/game.c` | Contains `entity_collision_test`, `collision_response_calc` |
| `src/game/physics.c` | Milliken model tire/suspension physics |

## Collision Types

### 1. Car-to-Car Collision

**Detection Method:**
1. Bounding sphere overlap test: `(dsq <= (colrad1 + colrad2)^2)`
2. Corner-in-body test: Check each car's 4 body corners against other car's bounding box

**Arcade Functions:**
- `collision()` - Main per-car collision check
- `PointInBody()` - Test point inside bounding box
- `setFBCollisionForce()` - Calculate front/back/side collision forces
- `ForceApart()` - Handle major overlap (stuck cars)
- `simpleCollForce()` - Simplified collision force

**Response:**
- Force magnitude based on relative velocity into collision
- Front/back force scaled by 4000 * relative_velocity
- Minimum 8000 lb force applied if overlap detected
- Major overlap triggers 100,000 lb separation force

### 2. Car-to-Wall (Surface Collision)

**Detection Method:**
- Surface tree (BSP-like) traversal in `stree.c`
- `tiresurf()` - Find surface under each tire
- `bodysurf()` - Find collision surfaces near body corners

**Arcade Functions:**
- `bodysurf()` - Body corner vs collision surface
- `COLL_force()` - Calculate wall collision force
- `COLL_test0()` through `COLL_test4()` - Different collision algorithms

**Surface Types:**
```c
#define SURF_COLLIDE  /* Collision surface (walls, barriers) */
#define SURF_PAVE     /* Pavement - driveable */
#define SURF_DIRT     /* Dirt - reduced traction */
```

### 3. Ground Contact (Tire-to-Surface)

**Detection Method:**
- Surface tree traversal per tire
- `tiresurf()` finds ground plane under tire contact patch

**Arcade Functions:**
- `tiresurf()` - Tire-surface intersection
- `fPathInterp()` - Interpolated road surface for smooth driving
- `fSurfDist()` - Calculate distance to surface

### 4. Flip/Rollover Detection

**Detection Method:**
- Compare car's Z-axis vector to road surface normal
- If `dot(car_z, road_z) < -0.7071` (45+ degrees inverted), car is upside-down

**Arcade Functions (drivsym.c):**
```c
/* To see if we are upside down on the road */
rwtobod(ucar, rcar, &m->roaduvs[0]);
if (rcar[ZCOMP] < -.7071) {
    /* Car is upside-down */
    if (all_wheels_on_ground) {
        m->crashflag = 1;  /* Force crash */
        m->topscrape = 1;  /* Top scrape damage */
    }
}
```

### 5. Crash Detection

**Detection Method:**
- Force threshold exceeded
- Angular velocity exceeded (spin-out)
- Upside-down on road surface

**Arcade Functions (drivsym.c:checkok):**
```c
/* Crash thresholds */
if ((m->forceavg > m->crashthreshold) || (m->forceavg < -m->crashthreshold))
    m->crashflag = 1;

if ((m->W[ZCOMP] > 10.0) || (m->W[ZCOMP] < -10.0))
    m->crashflag = 1;  /* Spinning too fast */
```

## Collision Response Formulas

### Spring-Damping Model (Wall Collision)

```c
/* Soft wall algorithm (COLL_ALG_SOFTWALL) */
if (vin < 0.0)    /* Compression */
    force = (din * g_Kcs) - (vin * vin * g_Kcd);
else              /* Rebound */
    force = (din * g_Krs) + (vin * vin * g_Krd);

/* Constants */
const F32 g_Kcs = (-10000.0);  /* Compression spring */
const F32 g_Krs = (-11000.0);  /* Rebound spring */
const F32 g_Kcd = (-50.0);     /* Compression damping */
const F32 g_Krd = (0.0);       /* Rebound damping */
```

### Hard Wall Algorithm

```c
/* COLL_ALG_HARDWALL */
vout = (din * 4.0) + 1.0;  /* 5fps @ 1 foot penetration */
force = (vin + vout) * m->mass * 0.5 * m->idt;
if (force < 0.0) force = 0.0;  /* Only force AWAY */
```

### Car Body Collision

```c
/* COLL_ALG_CARBODY */
/* Medium scrape force = 4000 ft-lb */
/* Spring force: 16000 * 0.25 feet */
if (din > 0.25)
    /* Deep penetration - force velocity to 1fps out */
    force = (vin + 1.0) * m->mass * m->idt * -0.5;
else
    force = ((din + 0.25) * -16000.0) + (vin * -2000.0);
```

### Car-to-Car Force Distribution

```c
/* Front/back force (setFBCollisionForce) */
if (!beside) {
    force[0] = rvel[0] * 4000.0;
    if ((pos[0] > 0.0) && (force[0] > -8000.0))
        force[0] = -8000.0;  /* Minimum bounce */
}

/* Side force */
if (!behind) {
    force[1] = rvel[1] * 4000.0;
    if ((pos[1] > 0.0) && (force[1] > -8000.0))
        force[1] = -8000.0;
}

/* Up/down force */
force[2] = rvel[2] * 4000.0;
```

## Arcade Functions Table

| Arcade Function | File | Purpose | N64 Equivalent |
|-----------------|------|---------|----------------|
| `init_collision` | collision.c | Initialize collision data | `init_collision` |
| `collision` | collision.c | Main collision loop | `collision` |
| `PointInBody` | collision.c | Point-in-box test | `point_in_body` |
| `setFBCollisionForce` | collision.c | Front/back collision | `set_collision_force` |
| `ForceApart` | collision.c | Major overlap handler | `ForceApart` |
| `simpleCollForce` | collision.c | Simple collision force | (inline in collision) |
| `setCenterForce` | collision.c | Set center force | `set_center_force` |
| `setCollisionDamage` | collision.c | Calculate damage | `set_collision_damage` |
| `collForceNormal` | collision.c | Wall force calculation | (in stree.c equivalent) |
| `distributeForce` | collision.c | Corner force distribution | `set_body_force` |
| `tiresurf` | stree.c | Tire-to-surface collision | (physics system) |
| `bodysurf` | stree.c | Body-to-surface collision | (physics system) |
| `COLL_force` | stree.c | Surface collision force | (physics system) |
| `CheckCrash` | resurrect.c | Crash state handler | (game state machine) |
| `are_we_dead_or_dying` | resurrect.c | Death state check | (game state) |
| `resurrect_car` | resurrect.c | Respawn car | (resurrect system) |
| `checkok` | drivsym.c | Crash threshold check | (physics update) |
| `antispin` | drivsym.c | Anti-spinout moment | (physics) |

## Damage System

### Force Thresholds

| Damage Level | Force Threshold | Description |
|--------------|-----------------|-------------|
| Level 0 | < 40,000 lb | No visible damage |
| Level 1 | >= 40,000 lb | Minor damage |
| Level 2 | >= 155,000 lb | Major damage |

### Damage Quadrants

```c
/* Damage applied to car corners/zones */
#define MAX_DAMAGE 2

/* Quadrant determination from force direction */
if ((f[2] > af[0]) && (f[2] > af[1]))
    quad = 4;        /* Top */
else if (f[0] > 0.0)
    quad = (f[1] > 0.0) ? 3 : 2;  /* Back */
else
    quad = (f[1] > 0.0) ? 1 : 0;  /* Front */
```

### Appearance Bit Masks

```c
/* Damage appearance flags */
#define App_M_HULK        /* Car destroyed */
#define App_M_TRANSLUCENT /* Invincibility flash */
#define App_M_TOP_DAMAGE  /* Roof damage */
#define App_M_ANY_DAMAGE  /* Any damage visible */
```

## N64 Function Mapping

| N64 Address | Size | Function | Arcade Equivalent |
|-------------|------|----------|-------------------|
| 0x800EE88C | 32 | collision_check_thunk | collision() wrapper |
| TBD | TBD | entity_collision_test | collision() |
| TBD | TBD | collision_response_calc | setFBCollisionForce() |
| TBD | TBD | player_states_reset | check_if_finished_resurrecting |
| TBD | TBD | effects_reset | init_collision |

## Special Cases

### 1. Flip Detection and Response

When car is detected upside-down on road:
1. Set `crashflag = 1`
2. Set `topscrape = 1` for visual effect
3. Apply top damage: `appearance |= (MAX_DAMAGE << gDamageShift[4])`
4. Initiate crash/respawn sequence

### 2. Rollover Detection

Rollover is detected when:
- Opposing corners touch road surface simultaneously
- Suspension compression is negative (car body on ground)
- Car velocity is below 100 fps

```c
if (((m->suscomp[0]+m->suscomp[1]+m->suscomp[2]+m->suscomp[3]) < 0) &&
    (((m->BODYFORCE[0][2] != 0.0) && (m->BODYFORCE[3][2] != 0.0)) ||
     ((m->BODYFORCE[1][2] != 0.0) && (m->BODYFORCE[2][2] != 0.0))))
{
    if (m->magvel < 100.0)
        m->crashflag = 1;
    m->topscrape = 1;
}
```

### 3. Water Reset (Track Hazard)

Rush 2049 includes water hazards that trigger immediate respawn:
- Detected via surface type flags in road data
- Sets `we_died = true` to trigger resurrection
- No crash explosion shown

### 4. Out-of-Bounds Reset

When car strays too far from track:
- Auto-abort triggered after 3+ seconds stuck
- Wrong-way arrows displayed after 0.5 seconds no progress
- Drone cars auto-abort if stuck with low velocity

```c
/* Auto-abort conditions */
if (IRQTIME - m->stuck_time > 3*ONE_SEC * wait_time)
    abort_me = true;
```

### 5. Respawn Invincibility

After resurrection:
- Car is non-collidable for up to 5 seconds
- Visual flashing indicates invincibility
- Collision re-enabled when:
  - 5 seconds elapsed, OR
  - Car regains full velocity

## Constants Reference

```c
/* Velocity limits */
#define MAX_VEL_SQ (300*300)  /* 300 fps max */

/* Force limits */
#define MAXFORCE 4e3          /* Max force cap */

/* Collision box height */
#define COLL_TOP_DIST (-5.0)  /* Top of box */
#define COLL_BOT_DIST (1.0)   /* Bottom of box */

/* Damage force thresholds */
#define DAMAGE1_FORCE (40000.0)
#define DAMAGE2_FORCE (155000.0)

/* Respawn timing */
#define RESURRECT_TIME 1000   /* ms to respawn */
#define ONE_SEC 1000          /* 1 second in ticks */
```

## Implementation Notes

1. **Coordinate System**: Arcade uses (X=forward, Y=right, Z=up) body frame
2. **Units**: Distances in feet, forces in pounds, time in seconds
3. **Dead Reckoning**: Network play uses `reckon` data for collision positions
4. **Performance**: Bounding sphere test provides O(1) rejection before O(n) box test

## See Also

- `docs/physics_tire_model.md` - Tire physics and traction
- `docs/arcade_function_mapping.md` - Complete function mapping
- `reference/repos/rushtherock/game/collision.c` - Original arcade source
