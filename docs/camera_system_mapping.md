# Camera System Mapping

## Overview

The Rush 2049 camera system is derived from the Rush The Rock arcade source code (`reference/repos/rushtherock/game/camera.c`, 1,593 lines). The N64 version implements similar camera modes with adaptations for the N64 hardware and game-specific features.

### Key Characteristics

- **Multiple view modes**: 10 different camera views for gameplay and cinematics
- **Dead reckoning**: Uses predicted car position (`dr_pos`, `dr_uvs`) for smooth camera tracking
- **Elasticity-based smoothing**: Camera lag controlled by `elasticity` factor (0=rigid, 1=loose)
- **Collision avoidance**: Camera stays above road surface using suspension data
- **Per-frame update**: Camera position/orientation updated each game frame

### Architecture

```
[Car Position] --> [Dead Reckoning] --> [View Mode Handler] --> [Camera Position]
                                              |
                                    +---------+---------+
                                    |         |         |
                                [Chase]   [Hood]   [Death] ...
                                    |         |         |
                                    +---------+---------+
                                              |
                                    [Elasticity Smoothing]
                                              |
                                    [Collision Check]
                                              |
                                    [gCamPos/gCamUvs]
                                              |
                                    [MBOX_UpdateCamera / N64 ViewMatrix]
```

## Camera Modes Table

| Mode | Name | Arcade Const | Description | Usage |
|------|------|--------------|-------------|-------|
| 1 | Worm's Eye | `view==1` | Low behind car, between front tires | Default gameplay |
| 2 | Hood Cam | `view==2` | On car hood with acceleration shake | Immersive driving |
| 3 | Chase Cam | `view==3` (DEFAULT) | Behind and above car, elastic follow | Default racing |
| 4 | Death Cam | `view==4` | Orbits crashed car | Crash sequences |
| 5 | Circle Cam | `view==5` | Orbits around car | Demo/attract mode |
| 6 | Fixed Track Cam | `view==6` | Fixed position on track | Demo mode |
| 7 | Zoom-in Cam | `view==7` | Steady move camera | Race start |
| 8 | Fixed Direction | `view==8` | Camera in fixed direction | Demo mode |
| 9 | Maxpath Cam | `view==9` | Follows AI path nodes | Demo mode |
| 10 | Backwards Cam | `view==10` | Rear view mirror | Gameplay |

### Demo Mode Random Camera

In demo mode, camera randomly switches between views every 3 seconds:
```c
switch ((long)Random(11.0)) {
    case 1: edge = SW_VIEW1; break;    // Worm's eye
    case 2: edge = SW_VIEW2; break;    // Hood cam
    case 3: edge = SW_VIEW3; break;    // Chase cam
    case 4: view = 5; break;           // Circle-cam (+2 sec extra)
    case 5: view = 8; break;           // Fixed-direction
    case 6: view = 9; break;           // Maxpath-cam
    case 7: view = 10; break;          // Backwards-cam
    default: view = 6; break;          // Fixed-on-track (+2 sec extra)
}
```

## Position/Orientation Math

### Coordinate System

The arcade uses a right-handed coordinate system:
- **X**: Left/Right
- **Y**: Up/Down
- **Z**: Forward/Back

Orientation is stored as a 3x3 UVS matrix:
```c
typedef struct {
    F32 uvs[3][3];   // Orientation matrix (right-handed)
    F32 pos[3];      // Position (world coords)
} MAT3;

// Access patterns:
mat3.uvs[0][i] = xuv (right vector)
mat3.uvs[1][i] = yuv (up vector)
mat3.uvs[2][i] = zuv (forward vector)
```

### Key Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `wormoff_y` | 1.172 | Worm's eye height offset |
| `wormoff_z` | 3.906 | Worm's eye distance behind |
| `driveroff_x` | -0.547 | Hood cam left/right |
| `driveroff_y` | 3.203 | Hood cam height |
| `driveroff_z` | 0.0 | Hood cam forward |
| `MAX_VEL` | 80 | Max velocity for camera scaling |
| `V3_MAX_STRECH` | 0.125 | Max chase cam distance stretch |
| `V3_MIN_ELAS` | 0.2 | Min elasticity at high speed |
| `V3_MAX_ELAS` | 0.8 | Max elasticity at low speed |
| `BOTTOM_OUT` | 0.5 | Suspension threshold |

### Camera Position Calculation

#### Chase Camera (View 3)

The most complex camera mode. Position is calculated from:
1. **Car velocity** - scales camera distance
2. **Z-vector interpolation** - blends between "back of car" and "velocity direction"
3. **Y-vector interpolation** - blends between "car up" and "world up"
4. **Elasticity smoothing** - applies lag based on speed

```c
// Distance scaling based on velocity
magvel = magnitude(car->dr_vel);
scale = (magvel > V3_MAX_MAGVEL) ? V3_MAX_STRECH : magvel * V3_VEL_SCALE;
rad = fsqrt(rear_dist*rear_dist + up_dist*up_dist);
rad *= (1.0 + scale);  // Camera pulls back at high speed

// Elasticity based on velocity (slower = more elastic)
elast = (magvel < 140.0) ? V3_MAX_ELAS - ((magvel/140.0)*V3_DEL_ELAS) : V3_MIN_ELAS;
```

#### Hood Camera (View 2) Shake

Hood camera includes collision/acceleration shake:
```c
// Limit collision forces
if (m->peak_center_force[0][j] > 100000)
    m->peak_center_force[0][j] = 100000;

// Calculate shake from collision
pos_in[j] = m->peak_center_force[0][j] * .00002;
pos_in[j] += m->peak_center_force[1][j] * .00002;
pos_in[j] += m->peak_body_force[0][j] * .00002;
pos_in[j] += m->peak_body_force[1][j] * .00002;

// Add acceleration shake (vertical stronger)
pos_in[2] = car->dr_acc[1] * -0.003;

// Smooth left/right and forward/back
cur_acc[i] = (cur_acc[i]*acc_elasticity + pos_in[i]*(1.0-acc_elasticity));

// Smooth up/down with road rumble
cur_acc[2] = (cur_acc[2]*acc_elasticity2 + pos_in[2]*(1.0-acc_elasticity2)) - spring_save*0.04;
```

## Smoothing Algorithms

### Elasticity-Based Interpolation

The primary smoothing method uses weighted interpolation:
```c
// elasticity: 0 = instant (rigid), 1 = infinite lag (never moves)
// Typical values: 0.13 (death cam) to 0.85 (chase cam)

fcam.mat3.pos[i] = fcam.mat3.pos[i]*elasticity +
                   (carpos[i]+res[i])*(1.0-elasticity);
```

### Acceleration Smoothing

Two different elasticity values for different axes:
```c
const F32 acc_elasticity  = 0.85;  // Lateral/longitudinal (smooth)
const F32 acc_elasticity2 = 0.35;  // Vertical (bouncier for road feel)
```

### Linear Interpolation

Used for camera transitions and smooth blending:
```c
// interpolate(from, to, dimensions, minT, maxT, t, result)
interpolate(fvec, bvec, 3, 0.0, 1.0, zf, zvec);
```

### Time-Based Interpolation

View 3 uses time-based blending for smooth transitions:
```c
// zf: Z-axis interpolation (0=fly mode, 1=back vector)
// Transition rates:
if(gSusAvg > 0.5) zf += dtime/1000.0;   // 1 second to backz
else if(magvel < 30.0) zf += dtime/100.0;  // 0.1 second to backz
else zf -= dtime/1000.0;                 // 1 second to flyz

// Clamp
if(zf < 0.0) zf = 0.0;
else if(zf > 1.0) zf = 1.0;
```

## Collision with Geometry

### Road Surface Avoidance

Camera stays above road using suspension compression data:
```c
// Calculate average suspension compression
for(i=0, gTireCnt=0, gSusAvg=0.0; i<4; i++) {
    if(m->suscomp[i] > 0.0) {
        gTireCnt++;
        gSusAvg += m->suscomp[i];
    }
}

// Only consider if 3+ tires touching
gSusAvg = (gTireCnt >= 3) ? gSusAvg/(F32)gTireCnt : 0.0;

// Push camera up if suspension compressed
if(gSusAvg > BOTTOM_OUT) {
    outdist = gSusAvg - BOTTOM_OUT;
    if((view == 1) || (view == 2))
        outdist *= 1.5;  // Extra fudge for worm/hood
    for(i=0; i<3; i++)
        gCamPos[i] += (car->dr_uvs[i][1] * outdist);
}
```

### N64 Collision Avoidance

The N64 version adds additional collision checking:
```c
// Minimum distance to target
if (dist < minDist && dist > 0.001f) {
    f32 scale = minDist / dist;
    camPos[0] = camTarget[0] - dx * scale;
    camPos[1] = camTarget[1] - dy * scale;
    camPos[2] = camTarget[2] - dz * scale;
}

// Ground query
if (track_surface_query(camPos, NULL, &groundHeight) != 0) {
    if (camPos[1] < groundHeight + 2.0f) {
        camPos[1] = groundHeight + 2.0f;
    }
}
```

## Replay Camera

The replay system implements 5 cinematic camera modes that auto-switch every 5 seconds:

| Mode | Name | Position | Target |
|------|------|----------|--------|
| 0 | Chase | Behind car at 80 units | Ahead of car at 20 units |
| 1 | Orbit | Orbits at 100 unit radius | Car center |
| 2 | Helicopter | 200 units above | Car center |
| 3 | Front | 60 units ahead of car | Car center |
| 4 | Wide Angle | 150 units diagonal | Car center |

```c
// Auto-switch every 300 frames (5 seconds at 60fps)
if (*cameraTimer > 300) {
    *cameraMode = (*cameraMode + 1) % 5;
    *cameraTimer = 0;
}
```

## Key Arcade Functions

### Main Functions

| Function | Lines | Description |
|----------|-------|-------------|
| `setcamview()` | 672-858 | Main camera update, dispatches to mode handlers |
| `View3Cam()` | 911-1081 | Chase camera implementation |
| `DeathCam()` | 1124-1151 | Death/crash camera |
| `UpdateCarObj()` | 1084-1120 | Fixed direction camera |
| `LookInDir()` | 1571-1591 | Point camera at direction vector |
| `update_rear_camera()` | 1160-1302 | Calculate rear view offset |

### Initialization Functions

| Function | Lines | Description |
|----------|-------|-------------|
| `init_view()` | 365-376 | Initialize camera system |
| `init_view3()` | 378-401 | Initialize chase camera |
| `SetMCamera()` | 492-507 | Set camera initial state |
| `ZeroCamera()` | 404-407 | Reset camera to origin |

### Demo/Attract Mode Functions

| Function | Lines | Description |
|----------|-------|-------------|
| `CheckCameraView()` | 513-649 | Handle view switching and demo mode |
| `circle_camera_around_car()` | 1310-1322 | Orbiting camera |
| `fix_camera_in_space()` | 1355-1367 | Fixed position camera |
| `steady_move_cam()` | 1400-1457 | Smooth transition camera |
| `maxpath_cam()` | 1489-1527 | AI path following camera |

### Select Screen Camera

| Function | Lines | Description |
|----------|-------|-------------|
| `SelectCam()` | 414-481 | Camera for track/car select screens |

## N64 Function Mapping

### Confirmed Mappings

| N64 Function | Arcade Equivalent | Confidence | Notes |
|--------------|-------------------|------------|-------|
| `camera_init` | `init_view()` | High | Initialization |
| `camera_update` | `setcamview()` | High | Per-frame update |
| `camera_update_chase` | `View3Cam()` | High | Chase camera |
| `camera_update_hood` | `setcamview() case 2` | High | Hood camera |
| `camera_update_death` | `DeathCam()` | High | Death camera |
| `camera_track_target` | `point_at_car()` | High | Generic tracking |
| `camera_collision_avoid` | suspension check in `setcamview()` | Medium | Collision |
| `camera_smooth_follow` | elasticity calculation | Medium | Smoothing |
| `camera_mode_set` | view switch in `CheckCameraView()` | Medium | Mode switching |
| `replay_camera_update` | N/A (N64 specific) | N/A | Replay cinematic |

### N64 Function Addresses (from game.c)

| Address | Function | Size | Description |
|---------|----------|------|-------------|
| 0x800A04C4 | `render_scene` | 2720 | Viewport/camera setup |
| 0x800BC3E0 | `camera_follow_target` | 7640 | Main camera update |
| - | `camera_track_target` | 1016 | Chase cam tracking |
| - | `camera_shake_apply` | 848 | Camera shake effect |
| - | `camera_collision_avoid` | 1596 | Collision avoidance |
| - | `camera_smooth_follow` | 872 | Smooth interpolation |
| - | `camera_mode_set` | 1156 | Mode switching |

### N64 Camera Structure

```c
// Camera structure offsets (from game.c analysis)
// Offset  Size  Field
// 0x00    12    pos[3] (f32)
// 0x0C    12    target[3] (f32)
// 0x18    12    look direction[3] (f32)
// 0x24    12    up vector[3] (f32)
// 0x30    12    right vector[3] (f32)
// 0x3C    4     fov (f32)
// 0x40    4     near plane (f32)
// 0x44    4     far plane (f32)
// 0x48    4     mode (s32)
// 0x4C    4     shake intensity (f32)
// 0x50    4     distance (f32)
// 0x54    4     yaw (f32)
// 0x58    4     pitch (f32)
```

## Camera Parameters and Tuning

### Default Camera Offsets

| View | Offset X | Offset Y | Offset Z | Elasticity |
|------|----------|----------|----------|------------|
| Worm's Eye | 0.0 | 1.172 | 3.906 | 0.85 |
| Hood Cam | -0.547 | 3.203 | 0.0 | 0.0 (rigid) |
| Chase Cam | 0.0 | 6.0-8.0 | 16.0-20.0 | 0.2-0.8 (speed-based) |
| Death Cam | 0.0 | 8.0 | 30.0 | 0.13 |

### Speed-Based Scaling

| Speed (ft/sec) | Distance Stretch | Elasticity |
|----------------|------------------|------------|
| 0 | 1.0x | 0.8 (max) |
| 50 | 1.06x | 0.58 |
| 100 | 1.125x (max) | 0.2 (min) |
| 140+ | 1.125x (max) | 0.2 (min) |

### Shake Parameters

| Parameter | Hood Cam Value | Description |
|-----------|----------------|-------------|
| acc_elasticity | 0.85 | Lateral shake smoothing |
| acc_elasticity2 | 0.35 | Vertical shake smoothing |
| Force scale | 0.00002 | Collision force to offset |
| Acc scale | 0.003 | Vertical acceleration scale |
| Spring scale | 0.04 | Road rumble multiplier |

### Collision Parameters

| Parameter | Value | Description |
|-----------|-------|-------------|
| BOTTOM_OUT | 0.5 | Suspension threshold for offset |
| Min tire count | 3 | Tires needed for suspension check |
| Fudge factor | 1.5 | Extra offset for worm/hood views |
| Ground clearance | 2.0 | Minimum camera height above ground |

## Global Variables

### Arcade Globals

| Variable | Type | Description |
|----------|------|-------------|
| `gCamPos[3]` | F32 | Global camera position |
| `gCamUvs[3][3]` | F32 | Global camera orientation |
| `fcam` | MATRIX | Camera matrix (pos + uvs) |
| `view` | S8 | Current view mode |
| `saveview` | S8 | Saved view for restore |
| `elasticity` | F32 (static) | Current elasticity |
| `elastic_factor` | F32 (static) | Elasticity modifier |
| `cur_acc[3]` | F32 | Acceleration smoothing state |
| `rear_x`, `rear_y` | F32 | Rear camera offsets |
| `view_3_time` | S32 | Time in view 3 |
| `rear_view_time` | S32 | Time in rear view |
| `this_node` | S16 | Which car camera follows |
| `demo_game` | S16 | Demo mode flag |

### N64 Equivalents

| Arcade Global | N64 Equivalent | Address |
|---------------|----------------|---------|
| `gCamPos` | `gCamPos` | TBD |
| `gCamUvs` | `gCamUvs` | TBD |
| `view` | `cam_mode` | D_801461D0 area |
| `this_node` | `this_car` | D_801461D0 area |
| `demo_game` | `is_demo_mode` | D_801461D0 area |

## References

- **Arcade source**: `reference/repos/rushtherock/game/camera.c` (1,593 lines)
- **N64 camera header**: `include/game/camera.h`
- **N64 camera source**: `src/game/camera.c` (459 lines)
- **N64 game.c camera functions**: `src/game/game.c` (lines 15088-24900)
- **Math library**: `reference/repos/rushtherock/LIB/fmath.h`
