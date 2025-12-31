# Rush 2049 Reckoning/Position Calculation System

This document describes the dead reckoning and position calculation system used in the San Francisco Rush series, based on the arcade source code at `reference/repos/rushtherock/game/reckon.c` and its adaptation for the N64 platform.

## Table of Contents

1. [Overview](#overview)
2. [Dead Reckoning for Network Play](#dead-reckoning-for-network-play)
3. [Position Interpolation](#position-interpolation)
4. [State Prediction](#state-prediction)
5. [Synchronization](#synchronization)
6. [Latency Compensation](#latency-compensation)
7. [N64 vs Arcade Differences](#n64-vs-arcade-differences)
8. [Function Mapping](#function-mapping)
9. [Data Structures](#data-structures)

---

## Overview

The reckoning system in Rush 2049 handles prediction and interpolation of vehicle positions to maintain smooth visuals despite network latency or asynchronous physics updates. The arcade version uses this for linked cabinet-to-cabinet multiplayer, while the N64 version adapts these concepts for split-screen multiplayer and ghost/replay systems.

### Key Concepts

- **Dead Reckoning**: Extrapolating future positions from known position, velocity, and acceleration
- **Base State**: The authoritative state snapshot from which predictions are calculated
- **Time Fudge**: Scaling factor to adjust for clock differences between systems
- **Freeze Time**: Maximum time to continue prediction before stopping (2.0 seconds in arcade)

---

## Dead Reckoning for Network Play

### Arcade Implementation

The arcade system uses IEEE DIS (Distributed Interactive Simulation) protocol concepts for entity state synchronization. Key functions:

#### `dead_reckon_posuv()` - Core Prediction Algorithm

Location: `reference/repos/rushtherock/game/reckon.c:209`

```c
void dead_reckon_posuv(MODELDAT *m, F32 dt)
{
    if(dt > RECKON_FREEZE_TIME)  /* 2.0 seconds */
    {
        /* Freeze car - zero velocity, move below track */
        m->reckon.RWV[0] = m->reckon.RWV[1] = m->reckon.RWV[2] = 0.0;
        m->reckon.RWR[2] = 200.0;  /* Below track surface */
    }
    else
    {
        /* Linear extrapolation: R' = R0 + V0*dt */
        veccopy(m->reckon.base_RWV, m->reckon.RWV);
        scalmul(m->reckon.base_RWV, dt, temp);
        vecadd(m->reckon.base_RWR, temp, m->reckon.RWR);

        /* Quaternion interpolation for orientation */
        if (m->reckon.num_quat_steps > 0) {
            m->reckon.num_quat_steps--;
            for (i=0; i<4; i++)
                m->reckon.base_quat[i] += m->reckon.delta_quat[i];
            make_uvs_from_quat(m->reckon.base_quat, m->reckon.UV);
        }
    }
}
```

#### Position Formula

The fundamental dead reckoning equation:

```
R'(t) = R0 + V0 * dt + 0.5 * A0 * dt^2
```

Where:
- `R'(t)` = Predicted position at time t
- `R0` = Base position (last known authoritative)
- `V0` = Base velocity
- `A0` = Base acceleration (optional, arcade uses linear only)
- `dt` = Time delta since base state

### N64 Adaptation

The N64 version at `net_latency_predict` (0x800F2E3C) uses similar logic:

```c
void net_latency_predict(void *entity, s32 frames) {
    f32 dt = 1.0f / 60.0f;  /* 60fps timestep */

    for (i = 0; i < frames && i < 10; i++) {
        /* Euler integration with acceleration */
        pos[0] += vel[0] * dt + 0.5f * accel[0] * dt * dt;
        pos[1] += vel[1] * dt + 0.5f * accel[1] * dt * dt;
        pos[2] += vel[2] * dt + 0.5f * accel[2] * dt * dt;

        vel[0] += accel[0] * dt;
        vel[1] += accel[1] * dt;
        vel[2] += accel[2] * dt;
    }
}
```

---

## Position Interpolation

### Display Coordinates

Both versions maintain separate "dead reckoned display coordinates" for smooth rendering:

#### Arcade CAR_DATA Structure

```c
typedef struct CAR_DATA {
    f32 dr_pos[3];      /* Dead reckoned position */
    f32 dr_vel[3];      /* Dead reckoned velocity */
    f32 dr_uvs[3][3];   /* Dead reckoned orientation */
    f32 dr_tirepos[4][3];  /* Dead reckoned tire positions */
    f32 dr_acc[3];      /* Reckon base acceleration */
    /* ... */
} CAR_DATA;
```

#### N64 CarData Structure

From `include/game/structs.h`:

```c
typedef struct CarData {
    f32     dr_pos[3];          /* 0x00: Dead reckoned position */
    f32     dr_vel[3];          /* 0x0C: Dead reckoned velocity */
    f32     dr_uvs[3][3];       /* 0x18: Dead reckoned orientation */
    f32     dr_tirepos[4][3];   /* 0x3C: Dead reckoned tire positions */
    f32     dr_acc[3];          /* 0x6C: Reckon base acceleration */

    f32     RWV[3];             /* Real world velocity (authoritative) */
    f32     RWR[3];             /* Real world position (authoritative) */
    /* ... */
} CarData;
```

### Usage Pattern

The `dr_*` fields are used throughout the N64 codebase for rendering:

- **Camera**: `src/game/camera.c` - Camera follows `car->dr_pos`
- **HUD/Minimap**: `src/game/hud.c` - Minimap uses `car->dr_pos`
- **Effects**: `src/game/effects.c` - Particle spawning at `car->dr_pos`
- **Collision**: `src/game/collision.c` - Uses `car->RWR` for physics, `dr_pos` for display
- **AI Drones**: `src/game/drone.c`, `src/game/maxpath.c` - Path following uses `dr_pos`

---

## State Prediction

### Arcade: Model Task vs Game Task

The arcade separates physics (model task) from game logic (game task):

#### `dead_reckon_all()` - Model Task Prediction

```c
void dead_reckon_all(S32 time)
{
    for (i=0; i<MAX_LINKS; i++) {
        m = &model[i];
        if(m->in_game) {
            deltime = (F32)(time - m->reckon.base_time) * gNetTimeToSec * m->reckon.base_fudge;
            dead_reckon_posuv(m, deltime);
        }
    }
}
```

#### `game_reckon_all()` - Game Task Prediction

```c
void game_reckon_all(S32 time)
{
    for (i=0; i<MAX_LINKS; i++) {
        m = &model[i];
        if(m->in_game) {
            if(m->we_control || (i == gThisNode)) {
                /* Local car: use more accurate model data */
                deltime = (F32)(time - m->fastin.lasttime) * gNetTimeToSec * m->time_fudge;
                veccopy(m->RWV, m->reckon.RWV);
                scalmul(m->RWV, deltime, temp);
                vecadd(m->RWR, temp, m->reckon.RWR);
                /* Also extrapolate orientation and suspension */
            }
            else {
                /* Remote car: use network reckon base */
                deltime = (F32)(time - m->reckon.base_time) * gNetTimeToSec * m->reckon.base_fudge;
                dead_reckon_posuv(m, deltime);
            }
        }
    }
}
```

### N64: Unified State Sync

The N64 uses `net_state_sync` (0x800F0C18) for multiplayer state synchronization:

```c
void net_state_sync(void *state) {
    /* Serialize each player's car state */
    for (i = 0; i < *numPlayers && i < 4; i++) {
        playerCar = (void *)(game_car + i * 0x400);
        carPos = (f32 *)((u8 *)playerCar + 0x24);
        carVel = (f32 *)((u8 *)playerCar + 0x34);
        carRot = (f32 *)((u8 *)playerCar + 0x60);

        /* Pack position (12 bytes) + velocity (12 bytes) + rotation (12 bytes) + inputs (2 bytes) */
        memcpy(&syncBuffer[bufferPos], carPos, 12);
        bufferPos += 12;
        /* ... */
    }

    net_message_send(syncBuffer, bufferPos);
}
```

---

## Synchronization

### Update Triggering

The arcade uses error-based update triggering to minimize network bandwidth:

#### `reckon_check()` - Bandwidth Optimization

```c
BOOL reckon_check(S32 slot)
{
    MODELDAT *m = &model[slot];

    #define DIST_ERR_LIMIT    (0.25*0.25)  /* 0.25 ft squared */
    #define VEL_ERR_LIMIT     (0.25*0.25)  /* 0.25 fps squared */
    #define DEAD_RECKON_TIMEOUT 200        /* Force update after 200ms */

    /* Force send during resurrect transition */
    if(m->resurrect.moving_state == 0) return 1;

    /* Suppress updates during resurrect animation */
    if(m->resurrect.moving_state > 0) return 0;

    /* Force update on timeout */
    if (IRQTIME - lastSend[slot] > DEAD_RECKON_TIMEOUT)
        return 1;

    /* Check position and velocity error */
    for (epos=evel=i=0; i<3; ++i) {
        diff = m->reckon.RWR[i] - m->RWR[i];
        if ((epos += diff * diff) > DIST_ERR_LIMIT) break;
        diff = m->reckon.RWV[i] - m->RWV[i];
        if ((evel += diff * diff) > VEL_ERR_LIMIT) break;
    }

    return (epos >= DIST_ERR_LIMIT || evel > VEL_ERR_LIMIT);
}
```

### Base State Updates

#### `update_reckon_base()` - Authoritative State Snapshot

```c
void update_reckon_base(S32 slot, S32 time)
{
    MODELDAT *m = &model[slot];

    /* Capture current state as new base */
    m->reckon.base_time = time;
    m->reckon.base_fudge = m->time_fudge;
    veccopy(m->RWA, m->reckon.base_RWA);  /* Acceleration */
    veccopy(m->RWV, m->reckon.base_RWV);  /* Velocity */
    veccopy(m->RWR, m->reckon.base_RWR);  /* Position */
    veccopy(m->W, m->reckon.base_W);      /* Angular velocity */
    fmatcopy(m->UV.fpuvs[0], m->reckon.base_UV[0]);  /* Orientation */
    make_quat_from_uvs(m->reckon.base_UV, m->reckon.base_quat);

    /* Capture visual state */
    m->reckon.torque = m->engtorque;
    m->reckon.rpm = m->rpm;
    m->reckon.steerangle = m->steerangle;
    for(i=0; i<4; i++) {
        m->reckon.tireW[i] = m->tires[i].angvel;
        m->reckon.suscomp[i] = m->suscomp[i];
        m->reckon.base_airdist[i] = m->airdist[i];
        m->reckon.base_airvel[i] = m->airvel[i];
    }

    /* Zero velocities if resurrecting */
    if(m->resurrect.moving_state >= 0) {
        /* Move car below track to hide it */
        m->reckon.base_RWR[2] = 200.0;
    }

    /* Send network PDU if multiplayer */
    if (!solo_flag && (m->we_control || slot == gThisNode))
        PutEntityStatePDU(slot);
}
```

---

## Latency Compensation

### Time Fudge Factors

The arcade uses "time fudge" to handle clock drift between linked cabinets:

```c
/* In MODELDAT */
F32 time_fudge;        /* All time fudges (catchup and boost) */

/* In RECKON */
F32 base_fudge;        /* Time fudge at base state capture */
```

Usage in time delta calculation:
```c
deltime = (F32)(time - m->reckon.base_time) * gNetTimeToSec * m->reckon.base_fudge;
```

### Freeze Timeout

If no update received for 2 seconds, the car is "frozen":

```c
#define RECKON_FREEZE_TIME (2.0)

if(dt > RECKON_FREEZE_TIME) {
    /* Zero velocity */
    m->reckon.RWV[0] = m->reckon.RWV[1] = m->reckon.RWV[2] = 0.0;

    /* Move below track surface (hide from view) */
    m->reckon.RWR[2] = 200.0;  /* Positive Z is down */
}
```

### N64 Latency Handling

The N64 limits prediction to 10 frames maximum:

```c
void net_latency_predict(void *entity, s32 frames) {
    if (frames > 10) {
        frames = 10;  /* Cap at ~167ms at 60fps */
    }
    /* ... */
}
```

---

## N64 vs Arcade Differences

### Network Architecture

| Aspect | Arcade | N64 |
|--------|--------|-----|
| **Multiplayer Type** | Cabinet-to-cabinet link | Local split-screen |
| **Protocol** | DIS-based PDUs | Internal messaging |
| **Max Players** | 8 (MAX_LINKS) | 4 |
| **Network Latency** | 10-100ms typical | N/A (same machine) |
| **Clock Sync** | Required (time fudge) | Shared clock |

### Data Structures

| Arcade | N64 | Notes |
|--------|-----|-------|
| `MODELDAT` (0x400+ bytes) | `CarData` (~0x200 bytes) | N64 is more compact |
| `RECKON` (0x11C bytes) | Embedded in `CarData` | Simplified structure |
| `model[MAX_LINKS]` | `car_array[8]` at 0x80152818 | Similar array pattern |

### Function Differences

| Purpose | Arcade Function | N64 Function | Address |
|---------|----------------|--------------|---------|
| State sync all cars | `game_reckon_all()` | `net_state_sync()` | 0x800F0C18 |
| Position prediction | `dead_reckon_posuv()` | `net_latency_predict()` | 0x800F2E3C |
| Base state update | `update_reckon_base()` | `ghost_record_frame()` | 0x800F38BC |
| Update check | `reckon_check()` | (not needed - local) | N/A |
| Model task reckon | `dead_reckon_all()` | (not needed - no model task) | N/A |

### Feature Differences

| Feature | Arcade | N64 |
|---------|--------|-----|
| **Quaternion Interpolation** | Yes (smooth orientation) | Simplified |
| **Shadow Distance Tracking** | Yes (`airdist`, `airvel`) | Partial |
| **Tire Angular Velocity** | Yes (network tire spin) | Yes |
| **Resurrection Handling** | Complex (hide/freeze) | Simplified |
| **Bandwidth Optimization** | Yes (`reckon_check`) | Not needed |
| **Mirror Mode Support** | Yes (PDU transforms) | Yes |

---

## Function Mapping

### Arcade to N64 Function Mapping

| Arcade Function | N64 Address | N64 Name | Size |
|-----------------|-------------|----------|------|
| `game_reckon_all()` | 0x800F0C18 | `net_state_sync` | 2036 bytes |
| `dead_reckon_posuv()` | 0x800F2E3C | `net_latency_predict` | 784 bytes |
| `update_reckon_base()` | 0x800F38BC | `ghost_record_frame` | 1912 bytes |
| `reckon_check()` | N/A | (no direct equivalent) | N/A |
| `zero_reckon_pcts()` | N/A | (debug counters removed) | N/A |
| `dead_reckon_all()` | N/A | (model task merged) | N/A |

### Related N64 Functions

| Address | Name | Description |
|---------|------|-------------|
| 0x800F13F0 | `net_lobby_update` | Multiplayer lobby state |
| 0x800F1944 | `net_player_join` | Player join handling |
| 0x800F1C98 | `net_player_leave` | Player leave handling |
| 0x800F1F3C | `net_game_start` | Game start sync |
| 0x800F27DC | `net_input_sync` | Input synchronization |
| 0x800F314C | `net_error_handle` | Network error handling |
| 0x800F3588 | `net_session_manage` | Session lifecycle |

---

## Data Structures

### RECKON Structure (Arcade)

From `reference/repos/rushtherock/game/modeldat.h`:

```c
typedef struct reckontyp {
    /* Base Data for Dead Reckoning */
    S32 base_time;          /* 10 microsecond units */
    F32 base_fudge;         /* Time scale factor */
    F32 base_RWA[3];        /* Real World Acceleration (ft/s^2) */
    F32 base_RWV[3];        /* Real World Velocity (ft/s) */
    F32 base_RWR[3];        /* Real World Position (ft) */
    F32 base_W[3];          /* Angular Velocity (rad/s) */
    F32 base_UV[3][3];      /* Rotation Matrix */
    F32 base_airdist[4];    /* Shadow Distance per tire */
    F32 base_airvel[4];     /* Shadow Velocity per tire */
    F32 base_quat[4];       /* Quaternion base */
    F32 delta_quat[4];      /* Quaternion delta per step */
    S16 num_quat_steps;     /* Interpolation steps remaining */

    /* Updated on Change */
    S16 torque;             /* Engine Torque (ft*lbs) */
    U16 rpm;                /* Engine RPM */
    F32 steerangle;         /* Steering angle (rad) */
    F32 tireW[4];           /* Tire rotation velocity (rad/s) */
    F32 suscomp[4];         /* Suspension compression (ft) */
    F32 airdist[4];         /* Ground clearance per tire */
    U32 look;               /* Visual flags */
    U32 appearance;         /* Appearance flags */

    /* Dead Reckoned Output */
    F32 RWV[3];             /* Predicted velocity */
    F32 RWR[3];             /* Predicted position */
    F32 UV[3][3];           /* Predicted orientation */
} RECKON;
```

### N64 Reckon Structure

From `include/game/structs.h`:

```c
typedef struct Reckon {
    s32     base_time;          /* 0x00 */
    f32     base_fudge;         /* 0x04 */
    f32     base_RWA[3];        /* 0x08 */
    f32     base_RWV[3];        /* 0x14 */
    f32     base_RWR[3];        /* 0x20 */
    f32     base_W[3];          /* 0x2C */
    f32     base_UV[3][3];      /* 0x38 */
    f32     base_airdist[4];    /* 0x5C */
    f32     base_airvel[4];     /* 0x6C */
    f32     base_quat[4];       /* 0x7C */
    f32     delta_quat[4];      /* 0x8C */
    s16     num_quat_steps;     /* 0x9C */
    s16     torque;             /* 0x9E */
    u16     rpm;                /* 0xA0 */
    f32     steerangle;         /* 0xA4 */
    f32     tireW[4];           /* 0xA8 */
    f32     suscomp[4];         /* 0xB8 */
    f32     airdist[4];         /* 0xC8 */
    u32     look;               /* 0xD8 */
    u32     appearance;         /* 0xDC */
    f32     RWV[3];             /* 0xE0 */
    f32     RWR[3];             /* 0xEC */
    f32     UV[3][3];           /* 0xF8 */
} Reckon; /* ~0x11C bytes */
```

---

## References

### Source Files

- **Arcade Reckoning**: `reference/repos/rushtherock/game/reckon.c`
- **Arcade Shadow Reckoning**: `reference/repos/rushtherock/game/sreckon.c`
- **Arcade Data Structures**: `reference/repos/rushtherock/game/modeldat.h`
- **Arcade PDU Handling**: `reference/repos/rushtherock/game/pdu.c`
- **N64 Game Code**: `src/game/game.c`
- **N64 Structures**: `include/game/structs.h`

### Symbol Addresses

From `symbol_addrs.us.txt`:

```
net_state_sync = 0x800F0C18;           // arcade:reckon.c:game_reckon_all
net_latency_predict = 0x800F2E3C;      // arcade:reckon.c:dead_reckon_posuv
net_game_state_send = 0x800F38BC;      // arcade:reckon.c:update_reckon_base
gCarPosition = 0x80152848;             // car position vector (f32[3])
car_array = 0x80152818;                // main car data array
```

### Related Documentation

- `docs/arcade_function_mapping.md` - Complete function mapping
- `docs/PROGRESS_REPORT.md` - Project status
- `reference/lessons-learned.md` - Decompilation best practices
