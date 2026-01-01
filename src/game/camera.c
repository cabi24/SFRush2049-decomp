/**
 * @file camera.c
 * @brief Camera system for Rush 2049 N64
 *
 * Based on arcade game/camera.c
 * Handles chase cam, cockpit view, replay cameras
 *
 * Key functions:
 * - 0x800A04C4 (render_scene) - viewport/camera setup, 2720 bytes
 * - 0x800A0BAC - camera state update
 * - 0x800A0F74 - camera initialization
 * - 0x800A0FDC - camera mode switching
 */

#include "types.h"
#include "game/camera.h"
#include "game/structs.h"

/* External declarations */
extern u8 gstate;
extern u32 frame_counter;
extern CarData car_array[];
extern s32 num_active_cars;
extern s32 this_car;

/* External math functions */
extern f32 sqrtf(f32 x);
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);

/* Global camera state */
CameraData gCamera;
f32 gCamPos[3];
f32 gCamUvs[3][3];

/* Static state */
static f32 last_car_pos[3];
static f32 camera_velocity[3];
static f32 cur_acc[3];               /* Acceleration offset for camera shake */
static f32 old_vec0;                 /* Previous frame velocity for smoothing */
static s32 view3_switch;             /* View 3 state */

/* Arcade constants (from camera.c) */
static const f32 acc_elasticity = 0.85f;    /* Lateral/longitudinal smoothing */
static const f32 acc_elasticity2 = 0.35f;   /* Vertical smoothing (bouncier) */

/**
 * camera_init - Initialize camera system
 * Based on arcade: camera.c:init_view()
 */
void camera_init(void) {
    s32 i, j;

    gCamera.pos[0] = 0.0f;
    gCamera.pos[1] = 10.0f;
    gCamera.pos[2] = -30.0f;

    gCamera.target[0] = 0.0f;
    gCamera.target[1] = 0.0f;
    gCamera.target[2] = 0.0f;

    /* Initialize orientation matrix to identity */
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            gCamera.uvs[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }

    gCamera.offset[0] = 0.0f;
    gCamera.offset[1] = CAM_WORM_OFFSET_Y;
    gCamera.offset[2] = -CAM_WORM_OFFSET_Z;

    gCamera.elasticity = CAM_ELASTICITY;
    gCamera.elastic_factor = CAM_ELASTICITY2;
    gCamera.view = DEFAULT_VIEW;
    gCamera.saved_view = DEFAULT_VIEW;
    gCamera.view_time = 0;
    gCamera.rear_view_time = 0;
    gCamera.rear_x = 0.0f;
    gCamera.rear_y = 0.0f;
    gCamera.target_car = 0;
    gCamera.is_demo = 0;
    gCamera.transitioning = 0;

    for (i = 0; i < 3; i++) {
        gCamera.acc[i] = 0.0f;
        last_car_pos[i] = 0.0f;
        camera_velocity[i] = 0.0f;
        cur_acc[i] = 0.0f;
    }

    old_vec0 = 0.0f;
    view3_switch = 1;
}

/**
 * camera_init_view - Initialize camera for specific view mode
 * Based on arcade: camera.c:init_view()
 *
 * @param view_mode View mode to initialize
 */
void camera_init_view(s32 view_mode) {
    gCamera.view = view_mode;
    gCamera.view_time = 0;

    switch (view_mode) {
        case CAM_VIEW_WORM:
            gCamera.offset[0] = 0.0f;
            gCamera.offset[1] = CAM_WORM_OFFSET_Y;
            gCamera.offset[2] = -CAM_WORM_OFFSET_Z;
            gCamera.elasticity = CAM_ELASTICITY;
            break;

        case CAM_VIEW_HOOD:
            gCamera.offset[0] = CAM_DRIVER_OFFSET_X;
            gCamera.offset[1] = CAM_DRIVER_OFFSET_Y;
            gCamera.offset[2] = CAM_DRIVER_OFFSET_Z;
            gCamera.elasticity = 0.0f;  /* Rigid camera */
            break;

        case CAM_VIEW_CHASE:
            gCamera.offset[0] = 0.0f;
            gCamera.offset[1] = 8.0f;
            gCamera.offset[2] = -20.0f;
            gCamera.elasticity = CAM_ELASTICITY;
            break;

        case CAM_VIEW_DEATH:
            /* Death cam orbits the car */
            gCamera.offset[0] = 0.0f;
            gCamera.offset[1] = 15.0f;
            gCamera.offset[2] = -25.0f;
            gCamera.elasticity = 0.5f;
            break;

        default:
            camera_init_view(CAM_VIEW_CHASE);
            break;
    }
}

/**
 * camera_init_view3 - Initialize default chase camera
 * Based on arcade: camera.c:init_view3()
 */
void camera_init_view3(void) {
    camera_init_view(CAM_VIEW_CHASE);
}

/**
 * camera_set_mode - Set camera mode
 *
 * @param mode New camera mode
 */
void camera_set_mode(s32 mode) {
    if (mode != gCamera.view) {
        gCamera.saved_view = gCamera.view;
        camera_init_view(mode);
        gCamera.transitioning = 1;
    }
}

/**
 * camera_update - Per-frame camera update
 * Based on arcade: camera.c:UpdateCamera()
 */
void camera_update(void) {
    s32 target = gCamera.target_car;

    if (target < 0 || target >= num_active_cars) {
        target = this_car;
    }

    /* Check for view change input */
    camera_check_view_change();

    /* Update based on current view mode */
    switch (gCamera.view) {
        case CAM_VIEW_WORM:
        case CAM_VIEW_CHASE:
            camera_update_chase(target);
            break;

        case CAM_VIEW_HOOD:
            camera_update_hood(target);
            break;

        case CAM_VIEW_DEATH:
            camera_update_death(target);
            break;

        case CAM_VIEW_BACKWARDS:
            camera_update_rear(target);
            break;

        default:
            camera_update_chase(target);
            break;
    }

    /* Copy to global camera position */
    gCamPos[0] = gCamera.pos[0];
    gCamPos[1] = gCamera.pos[1];
    gCamPos[2] = gCamera.pos[2];

    gCamera.view_time++;
}

/**
 * camera_check_view_change - Check for view change button press
 * Based on arcade: camera.c:CheckCameraView()
 */
void camera_check_view_change(void) {
    /* View change would be triggered by controller input */
    /* This would check for the camera button and cycle views */
}

/**
 * camera_set_view - Apply current camera view settings
 * Based on arcade: camera.c:setcamview()
 */
void camera_set_view(void) {
    /* Build view matrix from camera position and orientation */
}

/**
 * camera_update_chase - Update chase camera
 * Based on arcade: camera.c worm's eye and chase cam logic
 *
 * @param car_index Target car to follow
 */
void camera_update_chase(s32 car_index) {
    CarData *car;
    f32 target_pos[3];
    f32 camera_pos[3];
    f32 dx, dy, dz;
    f32 elastic;

    car = &car_array[car_index];
    elastic = gCamera.elasticity;

    /* Calculate target camera position based on car position and offset */
    /* In arcade, this uses the car's orientation matrix to transform offset */
    target_pos[0] = car->dr_pos[0] + gCamera.offset[0];
    target_pos[1] = car->dr_pos[1] + gCamera.offset[1];
    target_pos[2] = car->dr_pos[2] + gCamera.offset[2];

    /* Apply elasticity (camera lag) */
    dx = target_pos[0] - gCamera.pos[0];
    dy = target_pos[1] - gCamera.pos[1];
    dz = target_pos[2] - gCamera.pos[2];

    gCamera.pos[0] += dx * (1.0f - elastic);
    gCamera.pos[1] += dy * (1.0f - elastic);
    gCamera.pos[2] += dz * (1.0f - elastic);

    /* Camera always looks at car position */
    gCamera.target[0] = car->dr_pos[0];
    gCamera.target[1] = car->dr_pos[1] + 2.0f;  /* Slightly above car */
    gCamera.target[2] = car->dr_pos[2];

    /* Update last car position */
    last_car_pos[0] = car->dr_pos[0];
    last_car_pos[1] = car->dr_pos[1];
    last_car_pos[2] = car->dr_pos[2];
}

/**
 * camera_update_hood - Update hood/cockpit camera
 * Based on arcade: camera.c view 2 (hood cam)
 *
 * Hood camera includes acceleration-based shake from collisions
 * and road bumps for immersive effect.
 *
 * @param car_index Target car
 */
void camera_update_hood(s32 car_index) {
    CarData *car;
    f32 pos_in[3];
    s32 i;

    car = &car_array[car_index];

    /* Calculate camera shake from acceleration */
    /* From arcade: collision forces scaled and smoothed */
    pos_in[0] = car->dr_acc[0] * -0.00002f;  /* Left/right shake */
    pos_in[1] = car->dr_acc[1] * -0.00002f;  /* Forward/back shake */
    pos_in[2] = car->dr_acc[2] * -0.003f;    /* Up/down shake (stronger) */

    /* Clamp vertical acceleration effect */
    if (pos_in[2] < -0.003f) {
        pos_in[2] = -0.003f;
    }

    /* Smooth left/right and forward/back bumps */
    for (i = 0; i < 2; i++) {
        cur_acc[i] = cur_acc[i] * acc_elasticity + pos_in[i] * (1.0f - acc_elasticity);
    }

    /* Smooth up/down bumps (different elasticity for more bounce) */
    cur_acc[2] = cur_acc[2] * acc_elasticity2 + pos_in[2] * (1.0f - acc_elasticity2);

    /* Hood cam is attached to car with shake offset */
    gCamera.pos[0] = car->dr_pos[0] + gCamera.offset[0] - cur_acc[1];
    gCamera.pos[1] = car->dr_pos[1] + gCamera.offset[1] + cur_acc[2];
    gCamera.pos[2] = car->dr_pos[2] + gCamera.offset[2] - cur_acc[0];

    /* Look forward from car's perspective */
    /* Would use car's orientation matrix here */
    gCamera.target[0] = car->dr_pos[0];
    gCamera.target[1] = car->dr_pos[1] + gCamera.offset[1];
    gCamera.target[2] = car->dr_pos[2] + 100.0f;  /* Look far ahead */
}

/**
 * camera_update_worm - Update worm's eye camera
 * Based on arcade: camera.c view 1
 *
 * @param car_index Target car
 */
void camera_update_worm(s32 car_index) {
    /* Worm's eye is similar to chase but lower */
    camera_update_chase(car_index);
}

/**
 * camera_update_death - Update death/crash camera
 * Based on arcade: camera.c death cam
 *
 * @param car_index Target car
 */
void camera_update_death(s32 car_index) {
    CarData *car;
    f32 angle;
    f32 radius = 25.0f;

    car = &car_array[car_index];

    /* Death cam orbits around the car */
    angle = (f32)gCamera.view_time * 0.02f;

    gCamera.pos[0] = car->dr_pos[0] + radius * sinf(angle);
    gCamera.pos[1] = car->dr_pos[1] + 15.0f;
    gCamera.pos[2] = car->dr_pos[2] + radius * cosf(angle);

    /* Always look at car */
    gCamera.target[0] = car->dr_pos[0];
    gCamera.target[1] = car->dr_pos[1];
    gCamera.target[2] = car->dr_pos[2];
}

/**
 * camera_update_rear - Update rear view camera
 *
 * @param car_index Target car
 */
void camera_update_rear(s32 car_index) {
    CarData *car;

    car = &car_array[car_index];

    /* Rear view - look behind the car */
    gCamera.pos[0] = car->dr_pos[0];
    gCamera.pos[1] = car->dr_pos[1] + 5.0f;
    gCamera.pos[2] = car->dr_pos[2] + 5.0f;  /* Slightly ahead */

    /* Look backwards */
    gCamera.target[0] = car->dr_pos[0];
    gCamera.target[1] = car->dr_pos[1] + 2.0f;
    gCamera.target[2] = car->dr_pos[2] - 100.0f;  /* Behind */
}

/**
 * camera_select_screen - Camera for car/track select screens
 */
void camera_select_screen(void) {
    /* Stationary camera for menu screens */
    gCamera.pos[0] = 0.0f;
    gCamera.pos[1] = 5.0f;
    gCamera.pos[2] = -15.0f;

    gCamera.target[0] = 0.0f;
    gCamera.target[1] = 2.0f;
    gCamera.target[2] = 0.0f;
}

/**
 * camera_zero - Zero out camera position
 */
void camera_zero(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        gCamera.pos[i] = 0.0f;
        gCamera.target[i] = 0.0f;
    }
}

/**
 * camera_look_at - Point camera at target
 *
 * @param target Target position to look at
 */
void camera_look_at(f32 target[3]) {
    gCamera.target[0] = target[0];
    gCamera.target[1] = target[1];
    gCamera.target[2] = target[2];
}

/**
 * camera_look_in_dir - Point camera in direction
 *
 * @param dir Direction vector
 */
void camera_look_in_dir(f32 dir[3]) {
    gCamera.target[0] = gCamera.pos[0] + dir[0] * 100.0f;
    gCamera.target[1] = gCamera.pos[1] + dir[1] * 100.0f;
    gCamera.target[2] = gCamera.pos[2] + dir[2] * 100.0f;
}

/**
 * camera_get_distance_behind - Get camera distance behind car
 *
 * @param car_index Car index
 * @return Distance behind car
 */
f32 camera_get_distance_behind(s32 car_index) {
    CarData *car;
    f32 dx, dy, dz;

    car = &car_array[car_index];

    dx = gCamera.pos[0] - car->dr_pos[0];
    dy = gCamera.pos[1] - car->dr_pos[1];
    dz = gCamera.pos[2] - car->dr_pos[2];

    return sqrtf(dx*dx + dy*dy + dz*dz);
}

/**
 * camera_interpolate - Interpolate between two positions
 *
 * @param from Starting position
 * @param to Ending position
 * @param t Interpolation factor (0-1)
 * @param out Output position
 */
void camera_interpolate(f32 from[3], f32 to[3], f32 t, f32 out[3]) {
    out[0] = from[0] + (to[0] - from[0]) * t;
    out[1] = from[1] + (to[1] - from[1]) * t;
    out[2] = from[2] + (to[2] - from[2]) * t;
}

/* Stub declarations for original disassembled functions */
void render_scene(void);  /* TODO: Full decompile */
void camera_state_update(void);  /* TODO: Decompile */
/* camera_init is already defined above with void parameter */
void camera_mode_switch(u32 a0, u32 a1);  /* TODO: Decompile */

/* ========================================================================
 * Arcade-compatible function aliases (camera.c)
 * ======================================================================== */

/**
 * init_view - Initialize camera system (arcade name)
 * Wrapper for camera_init()
 */
void init_view(void) {
    camera_init();
}

/**
 * init_view3 - Initialize default chase camera (arcade name)
 * Wrapper for camera_init_view3()
 */
void init_view3(void) {
    camera_init_view3();
}

/**
 * ZeroCamera - Zero out camera position (arcade name)
 * Wrapper for camera_zero()
 */
void ZeroCamera(void) {
    camera_zero();
}

/**
 * SetMCamera - Set camera mode (arcade name)
 * Based on arcade: camera.c:SetMCamera()
 */
void SetMCamera(s16 mode) {
    s16 i, j;

    if (mode == 0) {  /* Initialize mode */
        gCamPos[0] = 0.0f;
        gCamPos[1] = 2.2f;
        gCamPos[2] = 0.0f;

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                gCamUvs[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    cur_acc[0] = 0.0f;
    cur_acc[1] = 0.0f;
    cur_acc[2] = 0.0f;
}

/**
 * CheckCameraView - Check for view change button (arcade name)
 * Wrapper for camera_check_view_change()
 */
void CheckCameraView(void) {
    camera_check_view_change();
}

/**
 * setcamview - Set camera view (arcade name)
 * Wrapper for camera_set_view()
 */
void setcamview(void) {
    camera_set_view();
}

/**
 * UpdateCam - Update camera (arcade name)
 * Based on arcade: camera.c:UpdateCam()
 */
void UpdateCam(void) {
    /* Simple update - just ensure camera is valid */
    camera_update();
}

/**
 * View3Cam - View 3 (chase) camera (arcade name)
 * Based on arcade: camera.c:View3Cam()
 */
void View3Cam(f32 pos[3], f32 uvs[3][3]) {
    f32 rear_dist, up_dist;
    f32 magvel, scale, rad;
    f32 camoff[3];
    s32 i;

    /* Default chase camera distances */
    rear_dist = 16.0f;
    up_dist = 6.0f;

    /* Use velocity to scale distance from car */
    magvel = sqrtf(camera_velocity[0]*camera_velocity[0] +
                   camera_velocity[1]*camera_velocity[1] +
                   camera_velocity[2]*camera_velocity[2]);

    scale = (magvel > V3_MAX_MAGVEL) ? V3_MAX_STRECH : magvel * V3_VEL_SCALE;
    rad = sqrtf(rear_dist*rear_dist + up_dist*up_dist);
    rad *= (1.0f + scale);
    rear_dist *= (1.0f + scale);
    up_dist *= (1.0f + scale);

    /* Calculate camera offset */
    camoff[0] = 0.0f;
    camoff[1] = up_dist;
    camoff[2] = -rear_dist;

    /* Apply offset to position */
    for (i = 0; i < 3; i++) {
        gCamera.pos[i] = pos[i] + camoff[i];
    }
}

/**
 * DeathCam - Death/crash camera (arcade name)
 * Based on arcade: camera.c:DeathCam()
 */
void DeathCam(f32 pos[3], f32 uvs[3][3]) {
    f32 rpos[3], carpos[3];
    f32 elasticity_val = 0.13f;
    s32 i;

    /* Car position */
    for (i = 0; i < 3; i++) {
        carpos[i] = pos[i];
    }

    /* Get offset relative to car (30 behind, 8 up) */
    rpos[0] = 0.0f;
    rpos[1] = 8.0f;
    rpos[2] = -30.0f;

    /* Follow the car with elasticity */
    for (i = 0; i < 3; i++) {
        gCamera.pos[i] = gCamera.pos[i] * (1.0f - elasticity_val) +
                         (carpos[i] + rpos[i]) * elasticity_val;
    }

    /* Look at car */
    gCamera.target[0] = carpos[0];
    gCamera.target[1] = carpos[1];
    gCamera.target[2] = carpos[2];
}

/**
 * UpdateCarObj - Update car object position (arcade name)
 * Based on arcade: camera.c:UpdateCarObj()
 */
void UpdateCarObj(f32 pos[3], f32 uvs[3][3]) {
    f32 rpos[3], carpos[3], res[3];
    f32 elasticity_val;
    s32 i;

    /* Update car object */
    update_car_object(pos, uvs);

    /* Car position */
    for (i = 0; i < 3; i++) {
        carpos[i] = pos[i];
    }

    /* Get offset relative to car */
    res[0] = 0.0f;
    res[1] = gCamera.rear_y;
    res[2] = -gCamera.rear_x;

    elasticity_val = 0.6f * gCamera.elastic_factor;

    /* Follow the car */
    for (i = 0; i < 3; i++) {
        gCamera.pos[i] = gCamera.pos[i] * elasticity_val +
                         (carpos[i] + res[i]) * (1.0f - elasticity_val);
        rpos[i] = carpos[i] - gCamera.pos[i];
    }

    /* Look at car */
    LookInDir(rpos, &gCamera);
}

/**
 * SelectCam - Camera for select screens (arcade name)
 * Wrapper for camera_select_screen()
 */
void SelectCam(void) {
    camera_select_screen();
}

/**
 * circle_camera_around_car - Orbit camera around car (arcade name)
 * Based on arcade: camera.c:circle_camera_around_car()
 */
void circle_camera_around_car(f32 pos[3], f32 uvs[3][3]) {
    f32 res[3], ang;
    f32 radius = 30.0f;
    f32 height = 20.0f;

    /* Calculate angle based on frame counter */
    ang = 2.0f * 3.1415926535f * ((f32)(frame_counter % 360) / 360.0f);

    res[0] = cosf(ang) * radius;
    res[1] = height;
    res[2] = sinf(ang) * radius;

    point_at_car(pos, uvs, res);
}

/**
 * init_camera_on_track - Initialize fixed camera on track (arcade name)
 * Based on arcade: camera.c:init_camera_on_track()
 */
void init_camera_on_track(void) {
    f32 pos[3], uvs[3][3];

    pos[0] = 0.0f;
    pos[1] = 12.0f;
    pos[2] = 0.0f;

    fix_camera_in_space(0, pos, uvs);  /* Initialize mode */
}

/**
 * fix_camera_in_space - Fixed camera in space (arcade name)
 * Based on arcade: camera.c:fix_camera_in_space()
 */
void fix_camera_in_space(s16 mode, f32 pos[3], f32 uvs[3][3]) {
    static f32 cam_pos[3];
    f32 res[3];
    s32 i;

    if (mode == 0) {  /* Initialize */
        for (i = 0; i < 3; i++) {
            cam_pos[i] = pos[i];
        }
    } else {
        for (i = 0; i < 3; i++) {
            res[i] = cam_pos[i] - pos[i];
        }
        point_at_car(pos, uvs, res);
    }
}

/**
 * init_steady_move_cam - Initialize steady move camera (arcade name)
 * Based on arcade: camera.c:init_steady_move_cam()
 */
void init_steady_move_cam(void) {
    f32 pos[3], uvs[3][3];

    pos[0] = 0.0f;
    pos[1] = 5.0f;
    pos[2] = 0.0f;

    uvs[0][0] = 0.0f;

    steady_move_cam(0, pos, uvs);  /* Initialize mode */
}

/**
 * steady_move_cam - Steady moving camera (arcade name)
 * Based on arcade: camera.c:steady_move_cam()
 */
void steady_move_cam(s16 mode, f32 pos[3], f32 uvs[3][3]) {
    static f32 cam_pos[3], delta[3];
    static s16 count;
    f32 res[3];
    s32 i;

    if (mode == 0) {  /* Initialize */
        for (i = 0; i < 3; i++) {
            cam_pos[i] = pos[i];
            gCamera.pos[i] = pos[i];
        }
        count = -1;
    } else {
        if (count == 0) {
            init_view3();
        } else {
            if (count == -1) {
                count = 30;
                for (i = 0; i < 3; i++) {
                    delta[i] = (pos[i] - cam_pos[i]) / (f32)count;
                }
            }

            count--;
            for (i = 0; i < 3; i++) {
                cam_pos[i] += delta[i];
                res[i] = cam_pos[i] - pos[i];
            }
            point_at_car(pos, uvs, res);
        }
    }
}

/**
 * init_maxpath_cam - Initialize maxpath camera (arcade name)
 * Based on arcade: camera.c:init_maxpath_cam()
 */
void init_maxpath_cam(void) {
    f32 pos[3], uvs[3][3];

    pos[0] = 0.0f;

    gCamera.elastic_factor = 0.0f;

    maxpath_cam(0, pos, uvs);  /* Initialize mode */
}

/**
 * maxpath_cam - Maxpath camera (arcade name)
 * Based on arcade: camera.c:maxpath_cam()
 */
void maxpath_cam(s16 mode, f32 pos[3], f32 uvs[3][3]) {
    static f32 cam_pos[3], delta[3];
    static s16 count;
    f32 res[3], new_pos[3];
    s32 i;

    if (mode == 0) {  /* Initialize */
        count = 0;
    } else {
        if (count == 0) {
            count = 2;
            /* Would get position from maxpath here */
            for (i = 0; i < 3; i++) {
                cam_pos[i] = pos[i];
                new_pos[i] = pos[i];
            }
            cam_pos[1] += 10.0f;
            new_pos[1] += 10.0f;

            for (i = 0; i < 3; i++) {
                delta[i] = (new_pos[i] - cam_pos[i]) / (f32)count;
            }
        }

        count--;
        for (i = 0; i < 3; i++) {
            cam_pos[i] += delta[i];
            res[i] = cam_pos[i] - pos[i];
        }
        point_at_car(pos, uvs, res);
    }
}

/**
 * update_car_object - Update car visual object (arcade name)
 * Based on arcade: camera.c:update_car_object()
 */
void update_car_object(f32 pos[3], f32 uvs[3][3]) {
    /* In arcade, this calls ZOID_UpdateObject */
    /* For N64, this would update the car model position */
}

/**
 * point_at_car - Point camera at car with offset (arcade name)
 * Based on arcade: camera.c:point_at_car()
 */
void point_at_car(f32 pos[3], f32 uvs[3][3], f32 res[3]) {
    f32 rpos[3], carpos[3];
    f32 elasticity_val;
    s32 i;

    update_car_object(pos, uvs);

    /* Car position */
    for (i = 0; i < 3; i++) {
        carpos[i] = pos[i];
    }

    elasticity_val = 0.6f * gCamera.elastic_factor;

    /* Follow the car */
    for (i = 0; i < 3; i++) {
        gCamera.pos[i] = gCamera.pos[i] * elasticity_val +
                         (carpos[i] + res[i]) * (1.0f - elasticity_val);
        rpos[i] = carpos[i] - gCamera.pos[i];
    }

    /* Look at car */
    LookInDir(rpos, &gCamera);
}

/**
 * LookInDir - Point camera in direction (arcade name)
 * Based on arcade: camera.c:LookInDir()
 *
 * Creates a view matrix looking in the specified direction
 */
void LookInDir(f32 lookdir[3], void *mat) {
    f32 length;
    f32 xuv[3], yuv[3], zuv[3];
    s32 i;

    /* Normalize look direction to get Z axis */
    length = sqrtf(lookdir[0]*lookdir[0] + lookdir[1]*lookdir[1] + lookdir[2]*lookdir[2]);
    if (length > 0.001f) {
        for (i = 0; i < 3; i++) {
            zuv[i] = lookdir[i] / length;
        }
    } else {
        zuv[0] = 0.0f;
        zuv[1] = 0.0f;
        zuv[2] = 1.0f;
    }

    /* Create X axis perpendicular to look direction (cross with up) */
    xuv[0] = lookdir[2];
    xuv[1] = 0.0f;
    xuv[2] = -lookdir[0];

    length = sqrtf(xuv[0]*xuv[0] + xuv[2]*xuv[2]);
    if (length <= 0.1f) {
        /* Looking straight up/down, use right vector */
        xuv[0] = 1.0f;
        xuv[1] = 0.0f;
        xuv[2] = 0.0f;
    } else {
        xuv[0] /= length;
        xuv[2] /= length;
    }

    /* Y axis is cross of Z and X */
    yuv[0] = zuv[1] * xuv[2] - zuv[2] * xuv[1];
    yuv[1] = zuv[2] * xuv[0] - zuv[0] * xuv[2];
    yuv[2] = zuv[0] * xuv[1] - zuv[1] * xuv[0];

    /* Copy to camera UVs */
    gCamUvs[0][0] = xuv[0]; gCamUvs[0][1] = xuv[1]; gCamUvs[0][2] = xuv[2];
    gCamUvs[1][0] = yuv[0]; gCamUvs[1][1] = yuv[1]; gCamUvs[1][2] = yuv[2];
    gCamUvs[2][0] = zuv[0]; gCamUvs[2][1] = zuv[1]; gCamUvs[2][2] = zuv[2];
}

/**
 * view1_suscomp_offset - View 1 suspension compensation offset (arcade name)
 * Based on arcade: camera.c:view1_suscomp_offset()
 */
s32 view1_suscomp_offset(void *m) {
    /* Returns suspension-based camera offset */
    /* In arcade, this uses m->reckon.suscomp to adjust camera height */
    return 0;
}
