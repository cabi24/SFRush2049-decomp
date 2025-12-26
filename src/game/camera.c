/**
 * @file camera.c
 * @brief Camera system for Rush 2049 N64
 *
 * Based on arcade game/camera.c
 * Handles chase cam, cockpit view, replay cameras
 *
 * Key functions:
 * - func_800A04C4 (render_scene) - viewport/camera setup, 2720 bytes
 * - func_800A0BAC - camera state update
 * - func_800A0F74 - camera initialization
 * - func_800A0FDC - camera mode switching
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
void func_800A04C4(void);  /* render_scene - TODO: Full decompile */
void func_800A0BAC(void);  /* camera state update */
void func_800A0F74(u32 param);  /* camera init */
void func_800A0FDC(u32 a0, u32 a1);  /* camera mode switch */
