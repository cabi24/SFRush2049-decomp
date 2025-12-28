# Camera Modes Mapping: Arcade to N64

This document maps the arcade Rush The Rock camera system to the N64 Rush 2049 implementation.

## Overview

The arcade camera system (from `reference/repos/rushtherock/game/camera.c`) implements 10 camera view modes with sophisticated follow algorithms. The N64 version preserves most of this functionality with platform-specific adaptations.

## Camera Modes

### Mode ID Table

| ID | Arcade Name | N64 Constant | Description |
|----|-------------|--------------|-------------|
| 1 | Worm's Eye (view 1) | `CAM_VIEW_WORM` | Low behind car, between front tires |
| 2 | Driver's Eye (view 2) | `CAM_VIEW_HOOD` | On car hood with shake effects |
| 3 | Hover Eye (view 3) | `CAM_VIEW_CHASE` | Default chase cam behind and above |
| 4 | Death Eye | `CAM_VIEW_DEATH` | Crash camera, orbits crashed car |
| 5 | Circle Cam | `CAM_VIEW_CIRCLE` | Orbits around car (demo mode) |
| 6 | Fixed Cam | `CAM_VIEW_TRACK` | Static position on track |
| 7 | Zoom-In Cam | `CAM_VIEW_ZOOM` | Race start zoom-in effect |
| 8 | Fixed Direction | `CAM_VIEW_FIXED_DIR` | Camera follows but fixed orientation |
| 9 | Maxpath Cam | `CAM_VIEW_MAXPATH` | Follows track waypoints |
| 10 | Backwards Cam | `CAM_VIEW_BACKWARDS` | Rear view mirror equivalent |

### Mode Parameters

#### View 1: Worm's Eye
```
Arcade constants:
  wormoff_y = 1.172  (up/down offset)
  wormoff_z = 3.906  (forward/back offset)

N64 constants (camera.h):
  CAM_WORM_OFFSET_Y = 1.172f
  CAM_WORM_OFFSET_Z = 3.906f
```

#### View 2: Driver's Eye (Hood Cam)
```
Arcade constants:
  driveroff_x = -0.547  (left/right)
  driveroff_y = 3.203   (up/down)
  driveroff_z = 0.0     (forward/back)
  acc_elasticity = 0.85  (lateral smoothing)
  acc_elasticity2 = 0.35 (vertical smoothing - bouncier)

N64 constants:
  CAM_DRIVER_OFFSET_X = -0.547f
  CAM_DRIVER_OFFSET_Y = 3.203f
  CAM_DRIVER_OFFSET_Z = 0.0f
  CAM_ELASTICITY = 0.85f
  CAM_ELASTICITY2 = 0.35f
```

#### View 3: Chase Cam
```
Arcade variables:
  rear_x = 16 (normal) or 30 (demo mode)
  rear_y = 6 (normal) or 18 (demo mode)
  elastic_factor = variable (0.2 to 0.8 based on velocity)

View3 velocity scaling:
  V3_MAX_MAGVEL = 100.0  (max velocity for distance scaling)
  V3_MAX_STRECH = 0.125  (max distance stretch factor)
  V3_MIN_ELAS = 0.2      (min elasticity at high speed)
  V3_MAX_ELAS = 0.8      (max elasticity at low speed)
```

#### View 4: Death Cam
```
Arcade:
  elasticity = 0.13 (fast response)
  rear distance = 30
  up distance = 8
```

#### View 5: Circle Cam
```
Arcade circle_camera_around_car():
  angle = 2*PI * (IRQTIME % 6sec) / 6sec
  x = cos(angle) * 30
  y = 20 (fixed height)
  z = sin(angle) * 30
  Period: 6 seconds per revolution
```

## Function Mapping Table

### Initialization Functions

| Arcade Function | N64 Equivalent | Location | Notes |
|-----------------|----------------|----------|-------|
| `init_view()` | `camera_init()` | src/game/camera.c:51 | Full camera system init |
| `init_view3()` | `camera_init_view3()` | src/game/camera.c:146 | Chase cam init |
| `SetMCamera(Initialize)` | `camera_init()` | game.c calls | Clears acceleration arrays |
| `ZeroCamera()` | `camera_zero()` | src/game/camera.c:390 | Zero out position |
| `init_steady_move_cam()` | - | Not ported | Zoom-in cam init |
| `init_camera_on_track()` | - | Not ported | Fixed cam init |
| `init_maxpath_cam()` | - | Not ported | Maxpath cam init |

### Per-Frame Update Functions

| Arcade Function | N64 Equivalent | Address | Size | Notes |
|-----------------|----------------|---------|------|-------|
| `UpdateCam()` | `camera_update()` | 0x800C0AC0 | 2884 bytes | Main camera tick |
| `CheckCameraView()` | `camera_check_view_change()` | - | - | View button handling |
| `setcamview()` | `camera_set_view()` | - | - | Apply current view settings |
| `View3Cam()` | `camera_update_chase()` | game.c | - | Chase cam update |
| `DeathCam()` | `camera_update_death()` | game.c | - | Crash cam orbit |
| `SelectCam()` | `camera_select_screen()` | game.c | - | Menu camera |

### Camera Mode Functions

| Arcade Function | N64 Equivalent | Address | Size | Notes |
|-----------------|----------------|---------|------|-------|
| `circle_camera_around_car()` | `camera_orbit_delta()` | game.c | 548 bytes | Orbital camera |
| `fix_camera_in_space()` | - | - | - | Fixed position cam |
| `steady_move_cam()` | - | - | - | Smooth transition cam |
| `maxpath_cam()` | - | - | - | Track waypoint cam |
| `point_at_car()` | `camera_follow_target()` | 0x800BC3E0 | 7640 bytes | Target tracking |
| `LookInDir()` | `camera_look_in_dir()` | src/game/camera.c:415 | - | Point camera |
| `update_rear_camera()` | `camera_update_rear()` | src/game/camera.c:357 | - | Rear view |

### Camera Effect Functions

| Arcade Function | N64 Equivalent | Address | Size | Notes |
|-----------------|----------------|---------|------|-------|
| - | `camera_shake()` | extern | - | Shake input |
| - | `camera_shake_start()` | game.c | 852 bytes | Initiate shake |
| - | `camera_shake_update()` | game.c | 192 bytes | Per-frame shake |
| - | `camera_shake_apply()` | game.c | 848 bytes | Apply shake to camera |
| - | `camera_lerp()` | game.c | extern | Position interpolation |
| - | `camera_lerp_position()` | game.c | 808 bytes | Smooth position lerp |
| - | `camera_zoom()` | game.c:7189 | - | Zoom control |
| - | `camera_zoom_set()` | game.c | 948 bytes | Set zoom distance |
| - | `camera_zoom_fov()` | game.c | 444 bytes | FOV-based zoom |

### Replay Camera Functions

| Arcade Function | N64 Equivalent | Address | Size | Notes |
|-----------------|----------------|---------|------|-------|
| - | `replay_camera_update()` | 0x800E2A3C | 1908 bytes | Replay playback cam |
| - | `attract_camera_update()` | 0x800F9428 | 1604 bytes | Demo/attract cam |

## Follow Algorithm (Spring-Damper Math)

The arcade uses a spring-damper system for smooth camera following.

### Arcade Implementation (View3Cam)

```c
/* Velocity-based distance scaling */
magvel = magnitude(car->dr_vel);
scale = (magvel > V3_MAX_MAGVEL) ? V3_MAX_STRECH : magvel * V3_VEL_SCALE;
rad = fsqrt(rear_dist*rear_dist + up_dist*up_dist);
rad *= (1.0 + scale);

/* Velocity-based elasticity */
elast = (magvel < 140.0) ? V3_MAX_ELAS - ((magvel/140.0)*V3_DEL_ELAS) : V3_MIN_ELAS;

/* Z vector interpolation (behind direction) */
/* Blends between velocity-based and car-orientation-based direction */
if (zf == 0.0)
    CopyVector(flyVec, zvec);      /* Pure velocity-based */
else if (zf == 1.0)
    CopyVector(backVec, zvec);     /* Pure car-based */
else
    interpolate(flyVec, backVec, 3, 0.0, 1.0, zf, zvec);

/* Apply elasticity (temporal smoothing) */
ScaleVector(zvec, 1.0 - elast, zvec);
ScaleAddVector(last_zvec, elast, zvec, zvec);

/* Camera position from offset vectors */
ScaleVector(zvec, rear_dist, camoff);
ScaleAddVector(yvec, up_dist, camoff, camoff);
AddVector(pos, camoff, fcam.mat3.pos);
```

### N64 Implementation (camera_smooth_follow)

```c
/* Lerp position with smoothing factor */
t = 1.0f - smoothing;
camPos[0] = camPos[0] * smoothing + targetPos[0] * t;
camPos[1] = camPos[1] * smoothing + targetPos[1] * t;
camPos[2] = camPos[2] * smoothing + targetPos[2] * t;
```

The N64 version simplifies the arcade's multi-stage interpolation into a single lerp with configurable smoothing factor.

## Mode Switching Logic

### Arcade (CheckCameraView)

```c
/* Button-based view switching */
if ((edge & SW_VIEW1) && view != 1) {
    view = 1;
    ctl_mod_latch(~(LA_VIEW2|LA_VIEW3));
    ctl_mod_latch(LA_VIEW1);
}
else if ((edge & SW_VIEW2) && view != 2) {
    view = 2;
    ctl_mod_latch(~(LA_VIEW1|LA_VIEW3));
    ctl_mod_latch(LA_VIEW2);
    cur_acc[0] = cur_acc[1] = cur_acc[2] = 0;  /* Reset shake */
}
else if ((edge & SW_VIEW3) && view != 3) {
    view = 3;
    ctl_mod_latch(~(LA_VIEW1|LA_VIEW2));
    ctl_mod_latch(LA_VIEW3);
    /* Reset chase cam position */
    fcam.mat3.pos[0] = game_car[gThisNode].dr_pos[0];
    fcam.mat3.pos[1] = game_car[gThisNode].dr_pos[1] + 1.0;
    fcam.mat3.pos[2] = game_car[gThisNode].dr_pos[2];
    rear_x = 16;
    rear_y = 6;
}

/* Demo mode random view switching (every 3 seconds) */
if (demo_game && IRQTIME - lastSwitch > 3 * ONE_SEC) {
    switch (Random(11)) {
        case 1: edge = SW_VIEW1; break;
        case 2: edge = SW_VIEW2; break;
        case 3: edge = SW_VIEW3; break;
        case 4: view = 5; break;  /* circle-cam */
        case 5: view = 8; break;  /* fixed-direction-cam */
        case 6: view = 9; init_maxpath_cam(); break;
        case 7: view = 10; break; /* backwards-cam */
        default: view = 6; init_camera_on_track(); break;
    }
}
```

### N64 (camera_mode_set)

```c
void camera_mode_set(void *camera, s32 mode) {
    s32 *camMode = (s32 *)((u8 *)camera + 0x48);
    f32 *distance = (f32 *)((u8 *)camera + 0x50);
    f32 *yaw = (f32 *)((u8 *)camera + 0x54);
    f32 *pitch = (f32 *)((u8 *)camera + 0x58);

    *camMode = mode;

    switch (mode) {
        case 0:  /* Chase cam */
            *distance = 8.0f;
            *pitch = 0.2f;
            break;
        case 1:  /* First person */
            *distance = 0.0f;
            *pitch = 0.0f;
            break;
        case 2:  /* Orbit */
            *distance = 12.0f;
            break;
        case 3:  /* Cinematic */
            break;
    }
}
```

## Key Camera Variables

### Arcade Globals

| Variable | Type | Description |
|----------|------|-------------|
| `fcam` | MATRIX | Current camera transformation matrix |
| `view` | S8 | Current view mode (1-10) |
| `saveview` | S8 | Saved view for restore |
| `cur_acc[3]` | F32 | Acceleration offset for camera shake |
| `rear_x` | F32 | Distance behind car |
| `rear_y` | F32 | Height above car |
| `old_rear_x` | F32 | Previous rear_x for interpolation |
| `elasticity` | F32 | Current camera lag factor |
| `elastic_factor` | F32 | Velocity-based elasticity modifier |
| `view_3_time` | S32 | Time in view 3 for adjustments |
| `rear_view_time` | S32 | Time in rear view |
| `old_vec0` | F32 | Previous velocity for smoothing |
| `view3_switch` | S32 | View 3 initialization flag |
| `gCamPos[3]` | F32 | Global camera position |
| `gCamUvs[3][3]` | F32 | Global camera orientation matrix |

### N64 Globals

| Variable | Type | Address | Description |
|----------|------|---------|-------------|
| `gCamera` | CameraData | - | Main camera state struct |
| `gCamPos[3]` | f32 | - | Global camera position |
| `gCamUvs[3][3]` | f32 | - | Camera orientation matrix |
| `D_80170000` | f32 | 0x80170000 | Shake intensity |
| `D_80170004` | f32 | 0x80170004 | Shake timer |
| `D_80170008` | f32 | 0x80170008 | Shake decay rate |
| `D_80170020` | void* | 0x80170020 | Active camera pointer |
| `D_80158000[3]` | f32 | 0x80158000 | Camera position array |
| `D_801146EC` | u8 | 0x801146EC | Game state (gstate) |

## Camera Structure Offsets (N64)

Based on game.c analysis:

| Offset | Type | Field |
|--------|------|-------|
| 0x00 | f32[3] | Position |
| 0x0C | f32[3] | Target position |
| 0x18 | f32[3] | Look direction |
| 0x24 | f32[3] | Up vector |
| 0x30 | f32[3] | Right vector |
| 0x3C | f32 | FOV |
| 0x40 | f32 | Near plane |
| 0x44 | f32 | Far plane |
| 0x48 | s32 | Mode |
| 0x4C | f32 | Shake intensity |
| 0x50 | f32 | Distance |
| 0x54 | f32 | Yaw |
| 0x58 | f32 | Pitch |

## Hood Camera Shake (Collision Effects)

The arcade hood cam (view 2) includes detailed collision-based shake:

```c
/* Limit collision forces */
if (m->peak_center_force[0][j] > 100000)
    m->peak_center_force[0][j] = 100000;
if (m->peak_center_force[1][j] < -100000)
    m->peak_center_force[1][j] = -100000;

/* Scale collision force to camera shake */
pos_in[j]  = m->peak_center_force[0][j] * 0.00002;
pos_in[j] += m->peak_center_force[1][j] * 0.00002;

/* Add body forces */
pos_in[j] += m->peak_body_force[0][j] * 0.00002;
pos_in[j] += m->peak_body_force[1][j] * 0.00002;

/* Vertical acceleration effect */
pos_in[2] = car->dr_acc[1] * -0.003;

/* Smooth with different elasticities */
cur_acc[i] = cur_acc[i] * 0.85 + pos_in[i] * 0.15;  /* Lateral */
cur_acc[2] = cur_acc[2] * 0.35 + pos_in[2] * 0.65;  /* Vertical (bouncier) */

/* Apply with road rumble */
cur_acc[2] -= spring_save * 0.04;  /* Suspension feedback */
```

## References

- Arcade source: `reference/repos/rushtherock/game/camera.c`
- N64 camera header: `include/game/camera.h`
- N64 camera implementation: `src/game/camera.c`
- N64 camera functions in game.c: Lines 3593-3648, 4684-4691, 4726-4737, 4940-4950, 5136-5156, 5291-5321, 5372-5395, 15088-15095, 20511-20672, 24294-24541, 29388-29707, 30235-30351, 30620-30666
