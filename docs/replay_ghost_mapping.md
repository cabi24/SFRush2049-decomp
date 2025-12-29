# Replay and Ghost Car System Analysis

This document analyzes the replay/ghost car systems comparing the arcade Rush The Rock source with the N64 Rush 2049 implementation.

## Overview

### Arcade (Rush The Rock)
The arcade version does NOT have a traditional replay/ghost system. Instead:
- **Demo Mode**: Uses AI drones (maxpath system) to drive cars in attract mode
- **Dead Reckoning**: Network synchronization system for linked cabinets (not replay)
- **MaxPath**: Pre-recorded path data for AI drone navigation

### N64 (Rush 2049)
The N64 version has a FULL replay and ghost car system:
- **Instant Replay**: Records all car states for post-race playback
- **Ghost Car**: Records best lap for time trial comparison
- **Controller Pak**: Saves ghost data for persistent ghost races

---

## Recording Data Structures

### N64 ReplayFrame (24 bytes per frame)
From `include/game/replay.h`:

```c
typedef struct ReplayFrame {
    /* Position (16-bit fixed point for compression) */
    s16     pos_x;          /* X position / 16.0 */
    s16     pos_y;          /* Y position / 16.0 */
    s16     pos_z;          /* Z position / 16.0 */

    /* Orientation (compressed quaternion) */
    s16     quat_x;         /* Quaternion X * 16384 */
    s16     quat_y;         /* Quaternion Y * 16384 */
    s16     quat_z;         /* Quaternion Z * 16384 */
    s16     quat_w;         /* Quaternion W * 16384 */

    /* Velocity (8-bit scaled) */
    s8      vel_x;          /* X velocity / 128.0 */
    s8      vel_y;          /* Y velocity / 128.0 */
    s8      vel_z;          /* Z velocity / 128.0 */

    /* Wheel rotation angles (8-bit) */
    u8      wheel_rot[4];   /* Visual wheel rotation */

    /* Steering angle (8-bit) */
    s8      steer_angle;

    /* State flags */
    u8      flags;          /* On ground, boosting, etc */

    /* Pad for alignment */
    u8      pad[2];
} ReplayFrame;
```

### N64 Extended Frame (68 bytes, every 30 frames)
```c
typedef struct ReplayFrameExt {
    /* Full precision position */
    f32     pos[3];         /* XYZ position */
    f32     vel[3];         /* XYZ velocity */

    /* Full orientation matrix */
    f32     matrix[3][3];   /* 3x3 rotation matrix */

    /* Engine/physics state */
    f32     engine_rpm;
    f32     speed;
    s8      gear;
    u8      on_ground;
    u8      surface_type;
    u8      pad;
} ReplayFrameExt;
```

### N64 Ghost Lap Structure
```c
typedef struct GhostLap {
    CarReplay   data;       /* Frame data */
    u32         lap_time;   /* Lap time in frames */
    u8          lap_number;
    u8          is_best;
    u8          track_id;
    u8          pad;
} GhostLap;
```

### Arcade Dead Reckoning (Network, not Replay)
From `reference/repos/rushtherock/game/modeldat.h`:

```c
typedef struct reckontyp {
    /* Base Data (sent over network) */
    S32 base_time;          /* Timestamp (10 microseconds) */
    F32 base_fudge;         /* Time correction factor */
    F32 base_RWA[3];        /* Acceleration (ft/s^2) */
    F32 base_RWV[3];        /* Velocity (ft/s) */
    F32 base_RWR[3];        /* Position (ft) */
    F32 base_W[3];          /* Angular velocity (rad/s) */
    F32 base_UV[3][3];      /* Rotation matrix */
    F32 base_airdist[4];    /* Tire-to-ground distance */
    F32 base_airvel[4];     /* Tire vertical velocity */
    F32 base_quat[4];       /* Quaternion orientation */
    F32 delta_quat[4];      /* Quaternion change per step */
    S16 num_quat_steps;     /* Steps to target orientation */

    /* Data Updated on change */
    S16 torque;             /* Engine torque (ft*lbs) */
    U16 rpm;                /* Engine RPM */
    F32 steerangle;         /* Front tire angle (rad) */
    F32 tireW[4];           /* Wheel rotation (rad/s) */
    F32 suscomp[4];         /* Suspension compression */
    F32 airdist[4];         /* Tire-to-ground distance */
    U32 look;               /* Car appearance flags */
    U32 appearance;         /* Visual effects */

    /* Dead Reckoned Data (extrapolated) */
    F32 RWV[3];             /* Current velocity */
    F32 RWR[3];             /* Current position */
    F32 UV[3][3];           /* Current rotation */
} RECKON;
```

---

## N64 Replay Functions

### Recording Functions
| Function | Address | Description |
|----------|---------|-------------|
| `replay_init` | - | Initialize replay system |
| `replay_start_recording` | - | Begin recording |
| `replay_stop_recording` | - | End recording |
| `replay_record_frame` | `0x800...` | Record current frame (every 2 frames) |
| `replay_record_car_state` | - | Record single car state |

### Playback Functions
| Function | Address | Description |
|----------|---------|-------------|
| `replay_start_playback` | - | Begin playback |
| `replay_stop_playback` | - | End playback |
| `replay_update_playback` | - | Advance playback frame |
| `replay_playback` | `0x80...` | Apply frame data to cars |
| `replay_update` | `0x80...` (1012 bytes) | Main replay update |

### Playback Controls
| Function | Description |
|----------|-------------|
| `replay_play` | Normal playback (1.0x) |
| `replay_pause` | Pause |
| `replay_rewind` | Rewind (2.0x reverse) |
| `replay_fast_forward` | Fast forward (2.0x) |
| `replay_slow_motion` | Slow motion (0.25x) |
| `replay_skip_forward` | Skip N frames |
| `replay_skip_backward` | Skip N frames back |

### UI/Camera Functions
| Function | Bytes | Description |
|----------|-------|-------------|
| `replay_ui_draw` | 1680 | Draw replay controls |
| `replay_camera_update` | 1908 | Cinematic camera |
| `camera_replay_mode` | - | Load position from `replay_camera_data[3600][6]` |

### Ghost Functions
| Function | Description |
|----------|-------------|
| `ghost_init` | Initialize ghost system |
| `ghost_start_lap` | Begin recording lap |
| `ghost_end_lap` | Finish lap, check for best |
| `ghost_record_frame` | Record player state |
| `ghost_playback` | Interpolate ghost position |
| `ghost_render` | Draw translucent ghost car |
| `ghost_save` | Save to Controller Pak |
| `ghost_load` | Load from Controller Pak |

---

## Data Format Details

### Frame Rate and Compression
| Parameter | Value |
|-----------|-------|
| Game frame rate | 60 fps |
| Replay sample rate | Every 2 frames (30 fps) |
| Extended frame interval | Every 30 frames (1/sec) |
| Max replay length | 5 minutes (18000 frames) |
| Max cars recorded | 8 |

### Storage Calculations
```
Per-frame data:     24 bytes
Extended frame:     68 bytes
Sample rate:        2 frames
Extended interval:  30 frames

5 minutes @ 30fps = 9000 samples

Per car:
  Frames:      9000 * 24 = 216,000 bytes
  Extended:    300 * 68 = 20,400 bytes
  Total:       ~236 KB per car

8 cars:
  Total:       ~1.8 MB
```

### Playback Speed Values (Fixed Point 8.8)
| Speed | Value | Multiplier |
|-------|-------|------------|
| SPEED_SLOW | 64 | 0.25x |
| SPEED_NORMAL | 256 | 1.0x |
| SPEED_FAST | 512 | 2.0x |
| SPEED_SUPER_FAST | 1024 | 4.0x |

### Position Compression
```c
#define POS_SCALE   16.0f   /* Units per fixed point */
#define VEL_SCALE   128.0f  /* Velocity scale */
#define QUAT_SCALE  16384.0f /* Quaternion scale */

/* Compress */
frame->pos_x = (s16)(car->pos[0] / POS_SCALE);

/* Decompress */
pos[0] = (f32)frame->pos_x * POS_SCALE;
```

---

## Arcade vs N64 Comparison

### Key Differences

| Feature | Arcade | N64 |
|---------|--------|-----|
| Replay System | No | Yes (full) |
| Ghost Car | No | Yes |
| Demo Mode | AI drones (maxpath) | AI drones |
| Network Sync | Dead reckoning | N/A |
| Save Ghost | N/A | Controller Pak |
| Camera Replay | N/A | 60 sec camera data |

### Why No Arcade Replay?
1. **Arcade Focus**: Designed for quick play sessions, not review
2. **Network Priority**: CPU resources devoted to linked cabinet sync
3. **Memory**: Limited RAM for game logic, not storage
4. **Time Pressure**: Players want to insert coins, not watch replays

### N64 Additions
The N64 version added significant replay functionality:
1. **Time Attack Mode**: Ghost car racing for lap records
2. **Instant Replay**: Post-race review from any angle
3. **Controller Pak**: Persistent ghost storage
4. **UI Controls**: Pause, rewind, fast-forward, slow-mo

---

## N64 Game.c Replay Implementation

### Key Globals
```c
extern u32 ghost_data_buffer[32];   /* Ghost data array */
extern s32 ghost_frame_count;       /* Ghost frame count */
extern f32 replay_camera_data[3600][6]; /* 60 sec camera (60fps) */
```

### Replay Structure Offsets
From game.c analysis:
```c
struct replay {
    s32 mode;           /* +0x00: 0=off, 1=record, 2=playback */
    s32 frameIndex;     /* +0x04: Current frame */
    s32 maxFrames;      /* +0x08: Max frames */
    f32 playbackSpeed;  /* +0x0C: Playback speed */
    s32 recordBuffer;   /* +0x10: Buffer pointer */
    /* ... */
    s32 cameraMode;     /* +0x80: Camera mode */
    s32 cameraTimer;    /* +0x84: Camera timer */
    s32 targetCar;      /* +0x88: Target car index */
    s32 playbackSpeed2; /* +0x8C: Speed setting */
    s32 numCars;        /* +0xFC: Number of cars */
    void *carList;      /* +0x100: Car pointers */
};
```

### Ghost Structure Offsets
```c
struct ghost {
    s32 playbackState;  /* +0x00: Playback state */
    s32 playbackFrame;  /* +0x04: Current frame */
    s32 maxFrames;      /* +0x08: Total frames */
    s32 trackId;        /* +0x0C: Track ID */
    s32 lapTime;        /* +0x10: Lap time */
    s32 carModel;       /* +0x20: Car model index */
    f32 ghostPos[3];    /* +0x40: Position */
    f32 ghostRot[3];    /* +0x50: Rotation */
    f32 ghostVel[3];    /* +0x60: Velocity */
    f32 frameData[];    /* +0x100: Frame data array */
};
```

---

## Playback Interpolation

### Linear Interpolation (Ghost)
From `ghost_playback` in game.c:
```c
/* Calculate interpolation factor */
t = frac(playbackFrame);

/* Interpolate position */
ghostPos[0] = frameData[offset0 + 0] * (1.0f - t) + frameData[offset1 + 0] * t;
ghostPos[1] = frameData[offset0 + 1] * (1.0f - t) + frameData[offset1 + 1] * t;
ghostPos[2] = frameData[offset0 + 2] * (1.0f - t) + frameData[offset1 + 2] * t;

/* Interpolate rotation */
ghostRot[0] = frameData[offset0 + 3] * (1.0f - t) + frameData[offset1 + 3] * t;
ghostRot[1] = frameData[offset0 + 4] * (1.0f - t) + frameData[offset1 + 4] * t;
ghostRot[2] = frameData[offset0 + 5] * (1.0f - t) + frameData[offset1 + 5] * t;
```

### Quaternion to Matrix (Replay)
```c
/* From replay_get_car_state */
f32 qx = (f32)rf->quat_x / QUAT_SCALE;
f32 qy = (f32)rf->quat_y / QUAT_SCALE;
f32 qz = (f32)rf->quat_z / QUAT_SCALE;
f32 qw = (f32)rf->quat_w / QUAT_SCALE;

/* Standard quaternion to matrix conversion */
matrix[0] = 1.0f - 2.0f * (yy + zz);
matrix[1] = 2.0f * (xy - zw);
matrix[2] = 2.0f * (xz + yw);
/* ... */
```

---

## Controller Pak Storage

### Ghost Save Format
```c
/* Header */
u32 magic;          /* 'RUSH' */
u32 version;        /* Save version */
u32 trackId;        /* Track identifier */
u32 lapTime;        /* Lap time in frames */
u32 frameCount;     /* Number of frames */
u32 carType;        /* Car model */
u32 checksum;       /* Data checksum */

/* Frame Data */
ReplayFrame frames[frameCount];
```

### Storage Size
- Controller Pak page: 256 bytes
- Ghost data: ~10-20 KB per lap
- Typically 3-5 ghosts can be stored

---

## Related Files

### N64 Source
- `src/game/replay.c` - Full implementation
- `include/game/replay.h` - Structures and constants
- `src/game/game.c` - Integration (lines 16666-17100)

### Arcade Reference
- `reference/repos/rushtherock/game/reckon.c` - Dead reckoning (network)
- `reference/repos/rushtherock/game/modeldat.h` - RECKON structure
- `reference/repos/rushtherock/game/maxpath.c` - AI path system
- `reference/repos/rushtherock/game/attract.c` - Demo mode

---

## Summary

The N64 Rush 2049 has a sophisticated replay/ghost system not present in the arcade version. The arcade relies on:
- Dead reckoning for network synchronization
- MaxPath AI for demo mode

The N64 version adds:
- Full instant replay with cinematic cameras
- Ghost car for time trials
- Controller Pak persistence
- Playback controls (pause, rewind, slow-mo)

This is a **significant N64-specific addition** rather than a port of arcade functionality.
