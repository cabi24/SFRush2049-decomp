/**
 * @file shadow.c
 * @brief Shadow rendering for Rush 2049 N64
 *
 * Based on reference/repos/rushtherock/game/visuals.c shadow functions:
 * - StartShadow (line 1969-1984)
 * - AnimateShadow (line 2008-2108)
 * - shadow_scale table (line 1986-2000)
 *
 * Also implements shadow_project and shadow_render from game.c.
 *
 * Copyright 1996 Time Warner Interactive.
 */

#include "types.h"
#include "game/shadow.h"
#include "game/visuals.h"

/* ======================= EXTERNAL DECLARATIONS ======================== */

/* Game state */
extern s32 this_node;           /* Current player's car slot */
extern s8 view;                 /* Current camera view */
extern u32 IRQTIME;             /* System time counter (ms) */

/* Camera position for distance calculations */
extern f32 gCamPos[3];

/* Math functions */
extern f32 sqrtf(f32 x);
extern void *memset(void *s, s32 c, u32 n);

/* Display list pointer */
extern void *gfx_dl_ptr;

/* Ground query function */
extern void track_surface_query(f32 *pos, void *surface, f32 *groundY);
extern f32 replay_start(f32 x, f32 z);

/* Drawing functions */
extern void draw_fill_rect(s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b, s32 a);
extern void draw_ui_element(s32 id, s32 x, s32 y, s32 w, s32 h, s32 alpha);

/* ======================= STUB IMPLEMENTATIONS ======================== */

#ifdef NON_MATCHING

/* ======================= STATIC VARIABLES ======================== */

/* Per-car shadow state */
static CarShadowState gCarShadows[8];  /* MAX_LINKS */

/* Shadow polygon pool */
static s32 gShadowPolyPool[8];

/* Shadow system initialized flag */
static s32 gShadowInitialized = 0;

/* ======================= INITIALIZATION ======================== */

/**
 * shadow_init - Initialize shadow system
 *
 * Sets up shadow resources for all cars.
 * Call once at game start.
 */
void shadow_init(void) {
    s32 i;

    if (gShadowInitialized) {
        return;
    }

    /* Clear shadow state */
    memset((void *)gCarShadows, 0, sizeof(gCarShadows));

    /* Initialize polygon pool */
    for (i = 0; i < 8; i++) {
        gShadowPolyPool[i] = -1;
        gCarShadows[i].polyId = -1;
        gCarShadows[i].hidden = 0;
    }

    gShadowInitialized = 1;
}

/* ======================= CAR SHADOW FUNCTIONS ======================== */

/**
 * shadow_create - Create shadow for a car
 * @param slot Car slot index
 *
 * Based on arcade StartShadow (visuals.c:1969-1984):
 * - Creates a 4-vertex polygon for the shadow
 * - Assigns AnimateShadow as the update function
 * - Initial placement at car position
 *
 * Arcade implementation:
 *   static void StartShadow( S16 slot )
 *   {
 *       CAR_DATA    *car = &game_car[slot];
 *       Visual      *v   = &car->visuals[VIS_SHADOW];
 *       S32         i,j;
 *       F32         pos[3],mat[3][3];
 *       const F32   xyz[4][3] = {{ 1, 1, 0}, {-1, 1, 0}, {-1,-1, 0}, { 1,-1, 0}};
 *
 *       v->func     = AnimateShadow;
 *       v->slot     = slot;
 *       v->timeStamp= IRQTIME;
 *
 *       v->objnum = ZOID_NewPoly(4, gTexList[TEX_SHADOW], xyz, 0, 0x20);
 *   }
 */
void shadow_create(s16 slot) {
    CarShadowState *state;

    /* Validate slot */
    if (slot < 0 || slot >= 8) {
        return;
    }

    state = &gCarShadows[slot];

    /* Clear state */
    memset((void *)state, 0, sizeof(CarShadowState));

    /* Allocate polygon (stub - would call ZOID_NewPoly or N64 equivalent) */
    /* On N64, we use display list commands instead of mathbox polys */
    state->polyId = slot;  /* Use slot as ID placeholder */
    state->hidden = 0;

    /* Initialize scale factors from shadow_scale table */
    /* These are indexed by body_type, but we use slot as placeholder */
    if (slot < NCARS) {
        state->wscale = shadow_scale[slot % NCARS][0];
        state->fscale = shadow_scale[slot % NCARS][1];
        state->bscale = shadow_scale[slot % NCARS][2];
    } else {
        /* Default scales */
        state->wscale = 0.17f;
        state->fscale = 0.34f;
        state->bscale = 0.35f;
    }
}

/**
 * shadow_update - Update shadow for a car each frame
 * @param slot Car slot index
 *
 * Based on arcade AnimateShadow (visuals.c:2008-2108):
 * - Checks if shadow should be hidden (resurrect, view, tilted car)
 * - Positions shadow at tire positions
 * - Scales shadow based on body type
 * - Fades shadow based on height
 *
 * Key arcade logic:
 *   d1 = m->reckon.airdist[0] - m->reckon.airdist[3];
 *   d2 = m->reckon.airdist[1] - m->reckon.airdist[2];
 *   if (abs(d1) > 8 || abs(d2) > 8 || (m->resurrect.moving_state > -1) ||
 *       (v->slot == this_node && view < 3) || m->hide_car) {
 *       ZOID_HidePoly(v->objnum);
 *       return;
 *   }
 */
void shadow_update(s16 slot) {
    CarShadowState *state;
    f32 xyz[4][3];
    f32 dist[4], d1, d2;
    f32 wscale, fscale, bscale, len, offset;
    s32 i, whl;
    s32 xlu;

    /* Validate slot */
    if (slot < 0 || slot >= 8) {
        return;
    }

    state = &gCarShadows[slot];

    /* Skip if no polygon */
    if (state->polyId < 0) {
        return;
    }

    /* Check diagonal vertical distance for hide condition */
    /* From arcade: abs(d1) > 8 || abs(d2) > 8 */
    d1 = state->airdist[0] - state->airdist[3];
    d2 = state->airdist[1] - state->airdist[2];
    if (d1 < 0) d1 = -d1;
    if (d2 < 0) d2 = -d2;

    if (d1 > SHADOW_MAX_DIAGONAL || d2 > SHADOW_MAX_DIAGONAL) {
        state->hidden = 1;
        return;
    }

    /* Check view for player car */
    if (slot == this_node && view < 3) {
        state->hidden = 1;
        return;
    }

    state->hidden = 0;

    /* Calculate average distance to ground */
    d1 = 0.0f;
    for (i = 0; i < 4; i++) {
        dist[i] = state->airdist[i];
        d1 += dist[i];
    }
    d1 /= 4.0f;

    /* Position shadow vertices at tire positions */
    /* Wheel order: 0,1,3,2 (front-right, front-left, rear-right, rear-left) */
    for (i = 0; i < 4; i++) {
        whl = (i < 2) ? i : 5 - i;  /* 0,1,3,2 mapping */
        xyz[i][0] = state->tirePos[whl][0];
        xyz[i][1] = state->tirePos[whl][1];
        xyz[i][2] = state->tirePos[whl][2];

        /* Apply vertical offset */
        offset = (slot == this_node) ? V3_SHADOW_OFFSET - dist[whl]
                                     : STD_SHADOW_OFFSET - dist[whl];
        xyz[i][1] += offset;
    }

    /* Scale shadow width */
    wscale = state->wscale;
    for (i = 0; i < 3; i++) {
        len = xyz[0][i] - xyz[1][i];
        xyz[0][i] += (len * wscale);
        xyz[1][i] -= (len * wscale);

        len = xyz[3][i] - xyz[2][i];
        xyz[3][i] += (len * wscale);
        xyz[2][i] -= (len * wscale);
    }

    /* Scale shadow front and back length */
    fscale = state->fscale;
    bscale = state->bscale;
    if (slot == this_node) {
        bscale *= 1.1f;  /* Fudge factor from arcade */
    }
    for (i = 0; i < 3; i++) {
        len = xyz[0][i] - xyz[3][i];
        xyz[0][i] += (len * fscale);
        xyz[3][i] -= (len * bscale);

        len = xyz[1][i] - xyz[2][i];
        xyz[1][i] += (len * fscale);
        xyz[2][i] -= (len * bscale);
    }

    /* Calculate alpha based on height */
    /* From arcade: xlu = rng( d1*8, 20, 255 ); */
    xlu = (s32)(d1 * SHADOW_ALPHA_MULTIPLIER);
    if (xlu < SHADOW_MIN_ALPHA) xlu = SHADOW_MIN_ALPHA;
    if (xlu > SHADOW_MAX_ALPHA) xlu = SHADOW_MAX_ALPHA;

    /* Would update polygon vertices and alpha here */
    /* ZOID_ShowPoly(v->objnum); */
    /* ZOID_UpdatePoly(v->objnum, 0, -2, xyz, -1, xlu); */
}

/**
 * shadow_remove - Remove shadow for a car
 * @param slot Car slot index
 */
void shadow_remove(s16 slot) {
    CarShadowState *state;

    if (slot < 0 || slot >= 8) {
        return;
    }

    state = &gCarShadows[slot];

    /* Would remove polygon here */
    /* ZOID_RemovePoly(polyId); */

    state->polyId = -1;
    state->hidden = 1;
}

/**
 * shadow_hide - Hide shadow temporarily
 * @param slot Car slot index
 */
void shadow_hide(s16 slot) {
    if (slot < 0 || slot >= 8) {
        return;
    }

    gCarShadows[slot].hidden = 1;

    /* Would call ZOID_HidePoly here */
}

/**
 * shadow_show - Show previously hidden shadow
 * @param slot Car slot index
 */
void shadow_show(s16 slot) {
    if (slot < 0 || slot >= 8) {
        return;
    }

    gCarShadows[slot].hidden = 0;

    /* Would call ZOID_ShowPoly here */
}

/* ======================= BLOB SHADOW FUNCTIONS ======================== */

/**
 * shadow_render_blob - Render a blob shadow under an object
 * @param object Pointer to object data
 *
 * Based on shadow_render in game.c (968 bytes).
 * Renders a simple shadow that scales/fades with height.
 */
void shadow_render_blob(void *object) {
    u8 *objData;
    f32 *objPos;
    f32 groundY;
    f32 shadowScale;
    f32 height;
    f32 baseSize;
    f32 shadowSize;

    objData = (u8 *)object;
    if (objData == NULL) {
        return;
    }

    objPos = (f32 *)(objData + 0x24);

    /* Get ground height at object position */
    groundY = shadow_get_ground_y(objPos[0], objPos[2]);

    /* Calculate height above ground */
    height = objPos[1] - groundY;
    if (height < 0.0f) height = 0.0f;
    if (height > SHADOW_MAX_HEIGHT) {
        return;  /* Too high, no shadow */
    }

    /* Shadow shrinks and fades with height */
    shadowScale = shadow_calc_scale(height, SHADOW_MAX_HEIGHT);

    /* Get base shadow size from object (offset 0x48) */
    baseSize = *(f32 *)(objData + 0x48);
    shadowSize = baseSize * shadowScale * OBJECT_SHADOW_SCALE;

    /* Draw shadow decal at ground level */
    draw_fill_rect(
        (s32)(objPos[0] - shadowSize),
        (s32)(objPos[2] - shadowSize),
        (s32)(shadowSize * 2.0f),
        (s32)(shadowSize * 2.0f),
        0, 0, 0, (s32)(OBJECT_SHADOW_ALPHA * shadowScale)
    );
}

/* ======================= SHADOW PROJECTION ======================== */

/**
 * shadow_project - Calculate shadow projection for an object
 * @param object Pointer to object data
 * @param lightDir Light direction vector
 *
 * Based on shadow_project in game.c (1596 bytes).
 * Projects shadow onto ground plane based on light direction.
 */
void shadow_project(void *object, f32 *lightDir) {
    f32 *objPos, *objBounds;
    f32 shadowVerts[8][3];
    f32 groundY;
    f32 lightDirNorm[3], lightLen;
    s32 i;
    f32 cornerX, cornerY, cornerZ, t;
    f32 *shadowData;

    if (object == NULL || lightDir == NULL) {
        return;
    }

    objPos = (f32 *)((u8 *)object + 0x24);
    objBounds = (f32 *)((u8 *)object + 0x50);

    /* Normalize light direction */
    lightLen = sqrtf(lightDir[0] * lightDir[0] +
                    lightDir[1] * lightDir[1] +
                    lightDir[2] * lightDir[2]);
    if (lightLen < 0.001f) {
        return;
    }

    lightDirNorm[0] = lightDir[0] / lightLen;
    lightDirNorm[1] = lightDir[1] / lightLen;
    lightDirNorm[2] = lightDir[2] / lightLen;

    /* Get ground height at object position */
    track_surface_query(objPos, NULL, &groundY);

    /* Generate bounding box corners and project to ground */
    for (i = 0; i < 8; i++) {
        cornerX = objPos[0] + ((i & 1) ? objBounds[0] : -objBounds[0]);
        cornerY = objPos[1] + ((i & 2) ? objBounds[1] : -objBounds[1]);
        cornerZ = objPos[2] + ((i & 4) ? objBounds[2] : -objBounds[2]);

        /* Project corner onto ground along light direction */
        t = (cornerY - groundY) / (-lightDirNorm[1] + 0.001f);

        shadowVerts[i][0] = cornerX + lightDirNorm[0] * t;
        shadowVerts[i][1] = groundY + SHADOW_Z_OFFSET;
        shadowVerts[i][2] = cornerZ + lightDirNorm[2] * t;
    }

    /* Store shadow vertices for rendering */
    shadowData = (f32 *)((u8 *)object + 0x100);
    for (i = 0; i < 8; i++) {
        shadowData[i * 3 + 0] = shadowVerts[i][0];
        shadowData[i * 3 + 1] = shadowVerts[i][1];
        shadowData[i * 3 + 2] = shadowVerts[i][2];
    }
}

/**
 * shadow_project_car - Calculate car shadow projection
 * @param car Pointer to car data
 * @param lightDir Light direction vector
 * @param outVerts Output array of 4 shadow vertices
 *
 * Projects car shadow based on tire positions and light direction.
 */
void shadow_project_car(void *car, f32 *lightDir, f32 outVerts[4][3]) {
    f32 *carPos;
    f32 groundY;
    f32 lightDirNorm[3], lightLen;
    s32 i;
    f32 cornerX, cornerY, cornerZ, t;

    if (car == NULL || lightDir == NULL || outVerts == NULL) {
        return;
    }

    carPos = (f32 *)((u8 *)car + 0x24);

    /* Normalize light direction */
    lightLen = sqrtf(lightDir[0] * lightDir[0] +
                    lightDir[1] * lightDir[1] +
                    lightDir[2] * lightDir[2]);
    if (lightLen < 0.001f) {
        return;
    }

    lightDirNorm[0] = lightDir[0] / lightLen;
    lightDirNorm[1] = lightDir[1] / lightLen;
    lightDirNorm[2] = lightDir[2] / lightLen;

    /* Get ground height */
    groundY = shadow_get_ground_y(carPos[0], carPos[2]);

    /* Project 4 corners of car to ground */
    for (i = 0; i < 4; i++) {
        /* Use car half-dimensions */
        cornerX = carPos[0] + ((i & 1) ? CAR_SHADOW_HALF_WIDTH : -CAR_SHADOW_HALF_WIDTH);
        cornerY = carPos[1];
        cornerZ = carPos[2] + ((i & 2) ? CAR_SHADOW_HALF_LENGTH : -CAR_SHADOW_HALF_LENGTH);

        /* Project to ground */
        t = (cornerY - groundY) / (-lightDirNorm[1] + 0.001f);

        outVerts[i][0] = cornerX + lightDirNorm[0] * t;
        outVerts[i][1] = groundY + SHADOW_Z_OFFSET;
        outVerts[i][2] = cornerZ + lightDirNorm[2] * t;
    }
}

/* ======================= UTILITY FUNCTIONS ======================== */

/**
 * shadow_calc_alpha - Calculate shadow alpha based on height
 * @param height Height above ground
 * @param maxHeight Maximum height for shadow visibility
 * @return Alpha value (0-255)
 */
s32 shadow_calc_alpha(f32 height, f32 maxHeight) {
    f32 scale;

    if (height < 0.0f) height = 0.0f;
    if (height >= maxHeight) {
        return 0;
    }

    scale = 1.0f - (height / maxHeight);
    return (s32)(SHADOW_MAX_ALPHA * scale);
}

/**
 * shadow_calc_scale - Calculate shadow scale based on height
 * @param height Height above ground
 * @param maxHeight Maximum height for shadow visibility
 * @return Scale factor (0.0-1.0)
 */
f32 shadow_calc_scale(f32 height, f32 maxHeight) {
    f32 scale;

    if (height < 0.0f) height = 0.0f;
    if (height >= maxHeight) {
        return SHADOW_MIN_SCALE;
    }

    scale = 1.0f - (height / maxHeight);
    if (scale < SHADOW_MIN_SCALE) {
        scale = SHADOW_MIN_SCALE;
    }

    return scale;
}

/**
 * shadow_get_ground_y - Get ground height at position
 * @param x X coordinate
 * @param z Z coordinate
 * @return Ground Y height
 */
f32 shadow_get_ground_y(f32 x, f32 z) {
    /* Use replay_start as ground height query (from game.c) */
    return replay_start(x, z);
}

/**
 * shadow_check_visibility - Check if shadow should be visible
 * @param slot Car slot index
 * @param viewNum Current camera view
 * @param isResurrecting TRUE if car is resurrecting
 * @param isHidden TRUE if car is hidden
 * @return TRUE if shadow should be rendered
 *
 * From arcade AnimateShadow logic:
 *   if (abs(d1) > 8 || abs(d2) > 8 ||
 *       (m->resurrect.moving_state > -1) ||
 *       (v->slot == this_node && view < 3) ||
 *       m->hide_car) {
 *       ZOID_HidePoly(v->objnum);
 *       return;
 *   }
 */
s32 shadow_check_visibility(s16 slot, s8 viewNum, s32 isResurrecting, s32 isHidden) {
    /* Hide if car is resurrecting */
    if (isResurrecting) {
        return 0;
    }

    /* Hide if car is hidden */
    if (isHidden) {
        return 0;
    }

    /* Hide player shadow in views 1 and 2 (hood/bumper views) */
    if (slot == this_node && viewNum < 3) {
        return 0;
    }

    return 1;
}

#endif /* NON_MATCHING */
