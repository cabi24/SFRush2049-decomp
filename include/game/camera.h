/**
 * camera.h - Camera system for Rush 2049 N64
 *
 * Based on arcade game/camera.c
 * Handles multiple camera views for racing:
 * - View 1: Worm's eye (low behind car)
 * - View 2: Hood cam (on car hood)
 * - View 3: Chase cam (behind and above)
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "types.h"

/* Camera view modes */
#define CAM_VIEW_WORM       1   /* Low behind car */
#define CAM_VIEW_HOOD       2   /* On hood */
#define CAM_VIEW_CHASE      3   /* Chase cam */
#define CAM_VIEW_DEATH      4   /* Death/crash cam */
#define CAM_VIEW_CIRCLE     5   /* Circle around car */
#define CAM_VIEW_TRACK      6   /* Fixed on track */
#define CAM_VIEW_ZOOM       7   /* Zoom-in cam */
#define CAM_VIEW_FIXED_DIR  8   /* Fixed direction */
#define CAM_VIEW_MAXPATH    9   /* Maxpath cam */
#define CAM_VIEW_BACKWARDS  10  /* Rear view */

#define DEFAULT_VIEW        CAM_VIEW_CHASE

/* Camera constants */
#define CAM_WORM_OFFSET_Y   1.172f  /* Worm's eye up/down */
#define CAM_WORM_OFFSET_Z   3.906f  /* Worm's eye forward/back */
#define CAM_DRIVER_OFFSET_X (-0.547f)
#define CAM_DRIVER_OFFSET_Y 3.203f
#define CAM_DRIVER_OFFSET_Z 0.0f
#define CAM_ELASTICITY      0.85f   /* Camera lag factor */
#define CAM_ELASTICITY2     0.35f   /* Secondary elasticity */
#define CAM_MAX_VEL         80      /* Max velocity for camera */

/* Camera data structure */
typedef struct CameraData {
    f32     pos[3];             /* Camera position (world coords) */
    f32     target[3];          /* Look-at target position */
    f32     uvs[3][3];          /* Camera orientation matrix */
    f32     offset[3];          /* Offset from car */
    f32     elasticity;         /* Camera lag (0=rigid, 1=loose) */
    f32     elastic_factor;     /* Secondary elasticity */
    f32     acc[3];             /* Acceleration smoothing */
    s16     view;               /* Current view mode */
    s16     saved_view;         /* Saved view for restore */
    s32     view_time;          /* Time in current view */
    s32     rear_view_time;     /* Time in rear view */
    f32     rear_x;             /* Rear camera X offset */
    f32     rear_y;             /* Rear camera Y offset */
    s32     target_car;         /* Which car camera follows */
    s8      is_demo;            /* In demo mode? */
    s8      transitioning;      /* Camera transitioning? */
    u8      pad[2];
} CameraData;

/* External camera globals */
extern CameraData gCamera;
extern f32 gCamPos[3];
extern f32 gCamUvs[3][3];

/* Initialization */
void camera_init(void);
void camera_init_view(s32 view_mode);
void camera_init_view3(void);
void camera_set_mode(s32 mode);

/* Per-frame update */
void camera_update(void);
void camera_check_view_change(void);
void camera_set_view(void);

/* Specific camera modes */
void camera_update_chase(s32 car_index);
void camera_update_hood(s32 car_index);
void camera_update_worm(s32 car_index);
void camera_update_death(s32 car_index);
void camera_update_rear(s32 car_index);

/* Camera for menus */
void camera_select_screen(void);
void camera_zero(void);

/* Utility */
void camera_look_at(f32 target[3]);
void camera_look_in_dir(f32 dir[3]);
f32  camera_get_distance_behind(s32 car_index);
void camera_interpolate(f32 from[3], f32 to[3], f32 t, f32 out[3]);

#endif /* CAMERA_H */
