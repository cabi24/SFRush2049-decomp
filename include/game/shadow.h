/**
 * @file shadow.h
 * @brief Shadow rendering for Rush 2049 N64
 *
 * Based on reference/repos/rushtherock/game/visuals.c shadow functions.
 * Handles car shadows, blob shadows, and shadow projection.
 *
 * Copyright 1996 Time Warner Interactive.
 */

#ifndef SHADOW_H
#define SHADOW_H

#include "types.h"

/* ======================= DEFINES ======================== */

/**
 * Shadow rendering constants
 * From arcade visuals.c
 */
#define SHADOW_MAX_HEIGHT       50.0f   /* Maximum height for shadow rendering */
#define SHADOW_Z_OFFSET         0.1f    /* Z offset to avoid z-fighting */
#define SHADOW_MIN_SCALE        0.3f    /* Minimum shadow scale at max height */
#define SHADOW_FADE_DIVISOR     100.0f  /* Height divisor for fade calculation */

/**
 * Shadow offset for different views
 * From arcade visuals.c:2002-2003
 */
#define V3_SHADOW_OFFSET        0.7f    /* View 3 (chase) shadow offset */
#define STD_SHADOW_OFFSET       0.5f    /* Standard shadow offset */

/**
 * Shadow diagonal vertical distance threshold
 * Used to hide shadow when car is tilted too much
 * From arcade visuals.c:2028-2032
 */
#define SHADOW_MAX_DIAGONAL     8.0f    /* Max diagonal distance before hiding */

/**
 * Shadow alpha constants
 */
#define SHADOW_MIN_ALPHA        20      /* Minimum shadow alpha */
#define SHADOW_MAX_ALPHA        255     /* Maximum shadow alpha (opaque) */
#define SHADOW_ALPHA_MULTIPLIER 8       /* Alpha = distance * this */

/**
 * Default shadow dimensions for car blob shadows
 */
#define CAR_SHADOW_HALF_WIDTH   3.0f    /* Half-width of car shadow */
#define CAR_SHADOW_HALF_LENGTH  5.0f    /* Half-length of car shadow */
#define CAR_SHADOW_BASE_ALPHA   100     /* Base alpha for car shadow */

/**
 * Object shadow constants
 */
#define OBJECT_SHADOW_SCALE     0.8f    /* Scale factor for object shadows */
#define OBJECT_SHADOW_ALPHA     128     /* Base alpha for object shadows */

/* ======================= TYPEDEFS ======================== */

/**
 * Shadow vertex structure for projected shadows
 * Used for shadow volume calculations
 */
typedef struct ShadowVert {
    f32 pos[3];     /* World position */
    f32 uv[2];      /* Texture coordinates */
} ShadowVert;

/**
 * Shadow polygon data
 * Used for dynamic shadow rendering
 */
typedef struct ShadowPoly {
    ShadowVert verts[4];    /* Quad vertices */
    f32 groundY;            /* Ground height at shadow */
    f32 alpha;              /* Shadow opacity (0-255) */
    s32 visible;            /* Visibility flag */
} ShadowPoly;

/**
 * Car shadow state
 * Per-car shadow rendering data
 */
typedef struct CarShadowState {
    f32 tirePos[4][3];      /* Tire positions for shadow corners */
    f32 airdist[4];         /* Distance from each tire to ground */
    f32 wscale;             /* Width scale factor */
    f32 fscale;             /* Front scale factor */
    f32 bscale;             /* Back scale factor */
    s32 polyId;             /* Polygon ID for shadow */
    s32 hidden;             /* Shadow hidden flag */
} CarShadowState;

/* ======================= GLOBAL VARIABLES ======================== */

/**
 * Shadow scale factors per car body type
 * [NCARS][3] = [side_scale, front_scale, back_scale]
 * From arcade visuals.c:1986-2000
 */
#ifndef NCARS
#define NCARS 12
#endif

/* Declared in visuals.c, exposed here for shadow module */
extern const f32 shadow_scale[NCARS][3];

/* ======================= FUNCTION DECLARATIONS ======================== */

/**
 * Car Shadow Functions
 * Based on arcade visuals.c StartShadow/AnimateShadow
 */

/**
 * shadow_init - Initialize shadow system
 * Call once at game start to set up shadow resources
 */
void shadow_init(void);

/**
 * shadow_create - Create shadow for a car
 * @param slot Car slot index (0 to MAX_LINKS-1)
 * Creates the shadow polygon for the specified car
 * Based on arcade StartShadow (visuals.c:1969-1984)
 */
void shadow_create(s16 slot);

/**
 * shadow_update - Update shadow for a car each frame
 * @param slot Car slot index
 * Updates shadow position, size, and opacity based on car state
 * Based on arcade AnimateShadow (visuals.c:2008-2108)
 */
void shadow_update(s16 slot);

/**
 * shadow_remove - Remove shadow for a car
 * @param slot Car slot index
 * Destroys the shadow polygon for the specified car
 */
void shadow_remove(s16 slot);

/**
 * shadow_hide - Hide shadow temporarily
 * @param slot Car slot index
 * Hides shadow without destroying (for resurrection, tunnel, etc.)
 */
void shadow_hide(s16 slot);

/**
 * shadow_show - Show previously hidden shadow
 * @param slot Car slot index
 * Shows shadow after being hidden
 */
void shadow_show(s16 slot);

/**
 * Blob Shadow Functions
 * Generic blob shadow for objects (not cars)
 */

/**
 * shadow_render_blob - Render a blob shadow under an object
 * @param object Pointer to object data
 * Renders a simple circular shadow that scales with height
 * Based on shadow_render in game.c
 */
void shadow_render_blob(void *object);

/**
 * car_shadow_render - Render car-specific blob shadow
 * @param car Pointer to car data
 * @param ground Pointer to ground surface data (may be NULL)
 * Renders a car-shaped shadow that accounts for terrain
 * Address: 0x800A5D34
 */
void car_shadow_render(void *car, void *ground);

/**
 * Shadow Projection Functions
 * For directional light shadows
 */

/**
 * shadow_project - Calculate shadow projection for an object
 * @param object Pointer to object data
 * @param lightDir Light direction vector (normalized)
 * Projects shadow onto ground plane based on light direction
 * Address: 0x800A5D34 (approximate - 1596 bytes)
 */
void shadow_project(void *object, f32 *lightDir);

/**
 * shadow_project_car - Calculate car shadow projection
 * @param car Pointer to car data
 * @param lightDir Light direction vector
 * @param outVerts Output array of 4 shadow vertices
 * Projects car shadow based on tire positions and light direction
 */
void shadow_project_car(void *car, f32 *lightDir, f32 outVerts[4][3]);

/**
 * Utility Functions
 */

/**
 * shadow_calc_alpha - Calculate shadow alpha based on height
 * @param height Height above ground
 * @param maxHeight Maximum height for shadow visibility
 * @return Alpha value (0-255), or 0 if too high
 */
s32 shadow_calc_alpha(f32 height, f32 maxHeight);

/**
 * shadow_calc_scale - Calculate shadow scale based on height
 * @param height Height above ground
 * @param maxHeight Maximum height for shadow visibility
 * @return Scale factor (0.0-1.0)
 */
f32 shadow_calc_scale(f32 height, f32 maxHeight);

/**
 * shadow_get_ground_y - Get ground height at position
 * @param x X coordinate
 * @param z Z coordinate
 * @return Ground Y height at position
 */
f32 shadow_get_ground_y(f32 x, f32 z);

/**
 * shadow_check_visibility - Check if shadow should be visible
 * @param slot Car slot index
 * @param view Current camera view
 * @param isResurrecting TRUE if car is resurrecting
 * @param isHidden TRUE if car is hidden
 * @return TRUE if shadow should be rendered
 */
s32 shadow_check_visibility(s16 slot, s8 view, s32 isResurrecting, s32 isHidden);

#endif /* SHADOW_H */
