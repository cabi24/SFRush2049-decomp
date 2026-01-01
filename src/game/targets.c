/**
 * targets.c - Collidable target objects for Rush 2049 N64
 *
 * Based on arcade game/targets.c (Time Warner Interactive, 1996)
 * Handles destructible objects like cones, poles, trees, fences, etc.
 *
 * See: reference/repos/rushtherock/game/targets.c
 *
 * Key functions:
 * - SetupTargets: Initialize target system for a track
 * - TargetCheck: Check car against all target groups
 * - TargetAppearance: Handle target appearance changes (network/local)
 *
 * Target types include:
 * - TARG_CONE: Traffic cones (flyaway animation)
 * - TARG_POLE: Telephone poles, traffic lights (knockdown)
 * - TARG_TREE: Trees (knockdown animation)
 * - TARG_FENCE: Breakaway fences
 * - TARG_CROWD: Animated crowd sprites
 * - TARG_FLAG: Waving flag animations
 * - TARG_FOG: Fog volume triggers
 * - TARG_FINISH: Finish line texture swap
 */

#include "types.h"
#include "game/targets.h"
#include "game/structs.h"
#include "game/vecmath.h"

/* ========================== GLOBALS ========================== */

/* Target list for entire track (arcade: gTargetList[MAX_TARGETS]) */
Target gTargetList[MAX_TARGETS];

/* Target groups for spatial acceleration */
static TargetGroup gTargetGroups[MAX_TARG_GROUPS];

/* Group and target counts */
static s16 gGroupCount;
static s16 gTargCount;

/* Current active fog group */
TargetGroup *gFogGroup;

/* Finish line object info */
u32 gFinishObject[10];
s32 gFinishPoly[10];
s32 num_finish_polys;

/* Fog state */
static s32 fogMode;
static s32 last_fogMode;
static s32 save_fogMode;
static u32 fogDensity;
static u32 fogColor;

/* Object list stub (would point to actual object definitions) */
s32 gObjList[256];

/* Object instance list stub (would contain object transforms) */
ObjInstance ObjInstanceList[512];

/* External game state */
extern u8 gstate;
extern CarData car_array[];
extern u32 frame_counter;
extern s32 this_node;
extern s32 num_humans;
extern s32 trackno;
extern BOOL demo_game;
extern BOOL in_tunnel;
extern u32 dlevels;

/* External arcade timer */
#define IRQTIME frame_counter

/* Debug switch for disabling fog */
#define SW_DEBUG15 0x8000

/* ========================== STUBS ========================== */

#ifdef NON_MATCHING

/* Mathbox interface stubs */
void MBOX_ResetUnlockedModels(const MBOXANIMFUNC *funcs, s32 numfuncs) {
    /* N64: Registers animation functions for target types */
    /* Would map target type indices to callback functions */
}

f32 MBOX_GetObjectRadius(s32 mbidx) {
    /* N64: Return bounding radius for object */
    return 5.0f;
}

void MBOX_HideObject(s32 objnum, s32 mode) {
    /* N64: Hide object from rendering */
}

s32 MBOX_FindTexture(const char *name, s32 *texid) {
    /* N64: Look up texture by name, return ID */
    *texid = 0;
    return 0;
}

/* ZOID interface stubs (arcade 3dfx rendering system) */
void ZOID_SetObjectDef(s32 objnum, s32 objdef) {
    /* N64: Update object definition (change model/frame) */
}

u32 ZOID_ObjectFlags(s32 objnum) {
    /* N64: Get object rendering flags */
    return 0;
}

void ZOID_SetObjectFlags(s32 objnum, u32 flags) {
    /* N64: Set object rendering flags */
}

void ZOID_UpdateObject(s32 objnum, f32 *pos, f32 uv[3][3]) {
    /* N64: Update object position and orientation */
}

void ZOID_ChangeTexture(s32 objnum, s32 lod, s32 polyid, s32 texid) {
    /* N64: Change texture on a polygon */
}

/* Visual effect stubs */
Visual *GrabEnvEntry(void) {
    /* N64: Allocate visual effect slot */
    static Visual dummy;
    return &dummy;
}

void ReleaseEnviron(Visual *v, BOOL reset) {
    /* N64: Release visual effect slot */
}

void AddToEnvList(Visual *v) {
    /* N64: Add visual to active list */
}

/* Sound stub */
void target_sound(Target *t, u32 slot) {
    /* N64: Play target hit sound based on type */
}

/* Vector math stubs (if not in vecmath.c) */
static void vecsub_local(const f32 a[3], const f32 b[3], f32 r[3]) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
}

/* ========================== TARGET FUNCTIONS ========================== */

/**
 * SetupTargets - Initialize target system for track loading
 * Based on arcade targets.c:SetupTargets()
 *
 * Called right before a track is loaded to reset all target data.
 *
 * @param track Track number being loaded
 */
void SetupTargets(s16 track) {
    s16 i, j;

    /* Would call MBOX_ResetUnlockedModels with animation function table */
    /* MBOX_ResetUnlockedModels(RTanimation, NUM_TARGTYPES); */

    /* Reset target list */
    gTargCount = 0;
    num_finish_polys = 0;

    for (i = 0; i < MAX_TARGETS; i++) {
        gTargetList[i].next = NULL;
        gTargetList[i].oanim = NULL;
        gTargetList[i].objnum = -1;
        gTargetList[i].type = -1;
        gTargetList[i].otype = -1;
        gTargetList[i].active = FALSE;
        gTargetList[i].name[0] = 0;
        gTargetList[i].frames = 0;
    }

    /* Reset target groups */
    gGroupCount = 0;
    gFogGroup = NULL;

    for (i = 0; i < MAX_TARG_GROUPS; i++) {
        gTargetGroups[i].type = 0;
        gTargetGroups[i].count = 0;
        gTargetGroups[i].list = NULL;
        gTargetGroups[i].last = NULL;
        gTargetGroups[i].radius = 0.0f;
        gTargetGroups[i].dist = 9999999.0f;
        for (j = 0; j < 3; j++) {
            gTargetGroups[i].pos[j] = 0.0f;
            gTargetGroups[i].min[j] = 0.0f;
            gTargetGroups[i].max[j] = 0.0f;
        }
    }
}

/**
 * SetTargets - Finalize target setup after track load
 * Based on arcade targets.c:SetTargets()
 *
 * Called after world loading to set up object definitions.
 */
void SetTargets(void) {
    s32 i;
    u32 flags;
    Target *t;

    for (t = gTargetList, i = 0; i < gTargCount; i++, t++) {
        if (t->objnum >= 0 && t->type >= TARG_NONE) {
            ZOID_SetObjectDef(t->objnum, gObjList[t->otype]);
            /* Parking meters are front-facing */
            if (t->otype == TARG_METER) {
                flags = ZOID_ObjectFlags(t->objnum);
                ZOID_SetObjectFlags(t->objnum, flags | ZATR_FRONTFACE);
            }
        }
    }
}

/**
 * car_inside_target - Check if car is inside target bounds
 * Based on arcade targets.c:car_inside_target()
 *
 * Performs detailed car-to-target overlap test.
 *
 * @param m Pointer to model slot (MODELDAT*)
 * @param t Target to check against
 * @return TRUE if car overlaps target
 */
BOOL car_inside_target(void *m, Target *t) {
    /* Simplified stub - would do body coordinate transform */
    /* Arcade version transforms target pos to car body coords */
    /* Returns true if within ~7ft longitudinal and ~3ft lateral */
    return FALSE;
}

/**
 * OverlapTarget - Check if car bounding sphere overlaps target
 * Based on arcade targets.c:OverlapTarget()
 *
 * @param slot Car slot index
 * @param pos Target position
 * @param radius Target radius
 * @param dist Output: gap distance (optional)
 * @return TRUE if overlap
 */
static BOOL OverlapTarget(s16 slot, f32 pos[3], f32 radius, f32 *dist) {
    f32 dsq, gap;
    f32 vec[3];
    CarData *gc;

    gc = &car_array[slot];

    /* Vector from target to car */
    vecsub_local(gc->RWR, pos, vec);
    dsq = vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
    radius += CAR_RADIUS;
    gap = dsq - radius*radius;

    if (dist) {
        *dist = gap;
    }

    return (gap <= 0.0f);
}

/**
 * distance - Calculate squared distance from car to position
 * Based on arcade targets.c:distance()
 *
 * @param slot Car slot index
 * @param pos Target position
 * @return Squared distance
 */
static f32 distance(s16 slot, f32 pos[3]) {
    f32 vec[3];
    CarData *gc;

    gc = &car_array[slot];

    /* Note: arcade uses display coords with axis swap */
    vec[0] = gc->dr_pos[2] - pos[0];
    vec[1] = gc->dr_pos[0] - pos[1];
    vec[2] = -gc->dr_pos[1] - pos[2];

    return vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2];
}

/**
 * TargetCheck - Check car against all target groups
 * Based on arcade targets.c:TargetCheck()
 *
 * Main per-frame collision check for targets.
 *
 * @param slot Car slot index
 */
void TargetCheck(s16 slot) {
    TargetGroup *g, *fogGroup;
    Target *t;
    BOOL overlap;
    f32 minDist;
    s16 i, type;

    /* Check against all groups */
    minDist = 1.0e16f;
    fogGroup = NULL;
    type = -1;

    for (g = &gTargetGroups[0], i = 0; i < gGroupCount; g++, i++) {
        /* Get type from first target in group */
        type = (g->list) ? g->list->oanim->udata[0] : -1;

        /* Special handling for fog groups */
        if (slot == this_node && type == TARG_FOG) {
            g->dist = distance((s16)this_node, g->pos);
            if ((g->dist <= (g->radius * g->radius)) && (g->dist < minDist)) {
                minDist = g->dist;
                fogGroup = g;
            }
            overlap = FALSE;  /* Don't check fog objects further */
        } else {
            overlap = OverlapTarget(slot, g->pos, g->radius, NULL);
        }

        /* If overlapping group, check individual targets */
        if (overlap) {
            /* Crowds and flags trigger on group overlap */
            if (type == TARG_CROWD || type == TARG_FLAG) {
                for (t = g->list; t != NULL; t = t->next) {
                    if (t->active && t->func != NULL) {
                        t->func(t, (u32)slot);
                    }
                }
            } else {
                /* Check individual targets */
                for (t = g->list; t != NULL; t = t->next) {
                    if (t->active && OverlapTarget(slot, t->pos, t->radius, NULL)) {
                        /* Collision detected - would send appearance PDU */
                        if (t->func != NULL) {
                            t->func(t, (u32)slot);
                            t->active = FALSE;
                        }
                    }
                }
            }
        }
    }

    /* Update fog group for this car */
    if (slot == this_node) {
        gFogGroup = fogGroup;
    }
}

/**
 * TargetAppearance - Handle target appearance changes
 * Based on arcade targets.c:TargetAppearance()
 *
 * Called when a target appearance message is received (network or local).
 *
 * @param ID Target index
 * @param flag Flags (unused)
 * @param dataV Data value (slot that hit target)
 * @param appearA Appearance data (unused)
 */
void TargetAppearance(s16 ID, u8 flag, u32 dataV, u32 appearA) {
    Target *t;

    t = &gTargetList[ID];

    /* Kick off animation if target is active */
    if (t->active && t->func != NULL) {
        t->func(t, dataV);
        t->active = FALSE;
    }
}

/**
 * ChangeFinishLine - Swap checkpoint texture to finish line
 * Based on arcade targets.c:ChangeFinishLine()
 *
 * Called on final lap to change checkpoint appearance.
 */
void ChangeFinishLine(void) {
    s32 tex, i;

    MBOX_FindTexture("FINISH", &tex);
    for (i = 0; i < num_finish_polys; i++) {
        ZOID_ChangeTexture(gFinishObject[i], 0, gFinishPoly[i], tex);
    }
    num_finish_polys = 0;
}

/* ========================== FOG FUNCTIONS ========================== */

/**
 * SetFog - Set fog mode and parameters
 * Based on arcade targets.c:SetFog()
 *
 * @param mode Fog mode (FOG_OFF, FOG_TINT, FOG_TRACK, FOG_TRACK_TINT)
 * @param density Fog density (0-255)
 * @param color Fog color (RGB)
 */
void SetFog(s32 mode, u32 density, u32 color) {
    /* Sanity check mode */
    if (mode < FOG_OFF || mode > FOG_TRACK_TINT) {
        mode = FOG_OFF;
    }

    /* Clamp density */
    if (density > 0xFF) {
        density = 0xFF;
    }

    /* Mask color */
    color |= 0xFFFFFF;

    fogMode = mode;
    fogDensity = density;
    fogColor = color;
    save_fogMode = fogMode;
}

/**
 * UpdateFog - Update fog state before rendering
 * Based on arcade targets.c:UpdateFog()
 *
 * Called right before EndFrame to update 3dfx/N64 fog parameters.
 */
void UpdateFog(void) {
    /* Disable fog on debug switch */
    if (dlevels & SW_DEBUG15) {
        fogMode = FOG_OFF;
    }

    switch (fogMode) {
        default:
            fogMode = FOG_OFF;
            /* fall through */
        case FOG_OFF:
            FogOff();
            break;
        case FOG_TINT:
            FogTint();
            break;
        case FOG_TRACK:
            FogTrack();
            break;
        case FOG_TRACK_TINT:
            FogTrackTint();
            break;
    }

    last_fogMode = fogMode;
}

/**
 * FogOff - Disable fog
 * Based on arcade targets.c:FogOff()
 */
void FogOff(void) {
    if (last_fogMode == FOG_OFF) {
        return;
    }

    /* N64: Would disable fog in RDP */
    /* grFogMode(GR_FOG_DISABLE) equivalent */
}

/**
 * FogTint - Apply tint fog (constant density)
 * Based on arcade targets.c:FogTint()
 */
void FogTint(void) {
    /* N64: Would set constant fog density */
    /* Used for screen fade effects */
}

/**
 * FogTrack - Apply track-specific fog
 * Based on arcade targets.c:FogTrack()
 */
void FogTrack(void) {
    s32 tgtfog, force;

    /* Check if using target-based fog */
    tgtfog = (gFogGroup != NULL) && !demo_game;
    force = (last_fogMode != FOG_TRACK);

    if (tgtfog) {
        FogTrackTarget(force);
    } else {
        FogTrackNormal(force);
    }
}

/**
 * FogTrackNormal - Normal distance-based fog
 * Based on arcade targets.c:FogTrackNormal()
 *
 * @param force Force fog table rebuild
 */
void FogTrackNormal(s32 force) {
    /* N64: Would build fog table from track parameters */
    /* Uses dbgFogColor/MinDist/MaxDist/MaxLevel per track */
}

/**
 * FogTrackTarget - Target-based pocket fog
 * Based on arcade targets.c:FogTrackTarget()
 *
 * @param force Force fog table rebuild
 */
void FogTrackTarget(s32 force) {
    /* N64: Would calculate fog based on distance from fog target */
    /* Used for tunnel fog effects */
}

/**
 * FogTrackTint - Track fog with attract mode tint
 * Based on arcade targets.c:FogTrackTint()
 */
void FogTrackTint(void) {
    /* N64: Track fog combined with screen tint */
    /* Used to darken screen in attract mode */
}

/**
 * MakeFogRamp - Build linear fog ramp
 * Based on arcade targets.c:MakeFogRamp()
 *
 * @param color Fog color
 * @param mindist Minimum fog distance
 * @param maxdist Maximum fog distance
 * @param minfog Minimum fog level (0-1)
 * @param maxfog Maximum fog level (0-1)
 */
void MakeFogRamp(u32 color, f32 mindist, f32 maxdist, f32 minfog, f32 maxfog) {
    /* N64: Would build fog table for RDP */
    /* Maps distance to fog density linearly */
}

#endif /* NON_MATCHING */
