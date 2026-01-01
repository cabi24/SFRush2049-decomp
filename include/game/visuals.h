/**
 * visuals.h - Visual overlay effects for Rush 2049 N64
 *
 * Based on reference/repos/rushtherock/game/visuals.h
 * Handles car visual effects: brake lights, sparks, smoke, shadows,
 * skid marks, car body/quad/window animations, and blast effects.
 *
 * Copyright 1996 Time Warner Interactive.
 */

#ifndef VISUALS_H
#define VISUALS_H

#include "types.h"

/* ======================= DEFINES ======================== */

#define MAX_VISUALS         100     /* Max concurrent environmental visuals */
#define HULK_FRAME          7       /* Frame # in blast when car turns to hulk */
#define BLAST_FRAMES        16      /* Number of frames in blast sequence */
#define BLAST_HOLD          40      /* msecs to hold each frame of blast animation */
#define SMOKE_FRAMES        16      /* Number of frames in smoke sequence */
#define SMOKE_OBJS          (3 * SMOKE_FRAMES)  /* Number of smokes to keep predefined/tire */
#define SKID_OBJS           100     /* Number of skids to keep predefined */
#define SKID_DEVIATION      1.0f    /* Max deviation from straight skid allowed */

/* Visual control bits (used in Visual.data field) */
#define VIS_BIT             0x10    /* Bit used in data field to indicate visibility */
#define XLU_BIT             0x20    /* Bit used in data field to indicate translucency */
#define HULK_BIT            0x40    /* Bit used in data field to indicate car is hulk */
#define HOOD_BIT            0x80    /* Bit used in data field to indicate car view 2 */
#define SHOW_ONLY           0x01    /* Show object only (not children) */
#define HIDE_ONLY           0x01    /* Hide object only (not children) */
#define SHOW_EACHCHILD      0x02    /* Show object and each child */
#define HIDE_EACHCHILD      0x02    /* Hide object and each child */
#define SHOW_ALL            0xFF    /* Show all objects */

/* Car type for PickCar */
#define HULK_TYPE           0xFF    /* Car is a hulk (destroyed) */

/* Maximum number of cars */
#ifndef MAX_LINKS
#define MAX_LINKS           8       /* Max cars active at once */
#endif
#ifndef NCARS
#define NCARS               12      /* Total number of car types */
#endif
#ifndef MAX_DAMAGE
#define MAX_DAMAGE          3       /* Damage levels per quad */
#endif

/* ======================= TYPEDEFS ======================== */

/**
 * Visual types enumeration
 * Defines indices for each visual effect attached to a car
 */
typedef enum VisualTypes {
    VIS_BLAST = 0,      /* Explosion/destruction animation */
    VIS_BRAKES,         /* Brake lights */
    VIS_LSPARK,         /* Left side sparks */
    VIS_RSPARK,         /* Right side sparks */
    VIS_BSPARK,         /* Bottom sparks */
    VIS_TIRE1,          /* Front-right tire */
    VIS_TIRE2,          /* Front-left tire */
    VIS_TIRE3,          /* Rear-right tire */
    VIS_TIRE4,          /* Rear-left tire */
    VIS_SHADOW,         /* Car shadow */
    VIS_QUAD0,          /* Front-right body quad */
    VIS_QUAD1,          /* Front-left body quad */
    VIS_QUAD2,          /* Rear-right body quad */
    VIS_QUAD3,          /* Rear-left body quad */
    VIS_QUAD4,          /* Top body quad */
    VIS_WINDOW0,        /* Front-right window */
    VIS_WINDOW1,        /* Front-left window */
    VIS_WINDOW2,        /* Rear-right window */
    VIS_WINDOW3,        /* Rear-left window */
    VIS_FRAME,          /* Car frame/body */
    NUM_VISUALS         /* Total number of visual types */
} VisualTypes;

/* Forward declarations */
struct Visual;
struct Skid;
struct NewSkid;

/**
 * Visual function pointer type
 * @param v Pointer to visual structure
 * @param op Operation code (0=cleanup, 1=update)
 */
typedef void (*VisFunc)(struct Visual *v, s16 op);

/**
 * Visual structure - Used for visual overlays of objects
 * Manages animated visual effects attached to cars
 */
typedef struct Visual {
    struct Visual *next;        /* Next node in linked list */
    s32     index;              /* Index of animation sequence */
    s32     objnum;             /* Object number of overlay (-1 if none) */
    u32     slot;               /* Slot number of the visual owner's car */
    u32     data;               /* Used to store arbitrary data */
    u32     timeStamp;          /* Used to set last time updated (IRQTIME) */
    VisFunc func;               /* Controller function */
} Visual;

/**
 * Skid structure - Represents a completed skid mark on the ground
 * Managed in linked lists (gSkidFree and gSkidList)
 */
typedef struct Skid {
    struct Skid *prev;          /* Previous link in list */
    struct Skid *next;          /* Next link in list */
    u32     lastTime;           /* IRQTIME of last update */
    s32     objnum;             /* Mathbox polygon index */
    f32     pos[3];             /* Skid center (for distance-based removal) */
    u8      xlu;                /* Skid translucency level (0x00 to 0xFF) */
} Skid;

/**
 * NewSkid structure - Skid mark being actively created
 * Tracks polygon vertices as tire moves
 */
typedef struct NewSkid {
    Skid    *skid;              /* Unlinked Skid for creation (NULL if no skid in progress) */
    f32     start[3];           /* Start position of poly */
    f32     end[3];             /* End position of poly */
    f32     lensq;              /* Skid length squared (0.0 when initially created) */
    f32     dir[3];             /* Skid direction vector (valid if lensq > 0.0) */
    f32     vert[4][3];         /* Skid polygon vertices (0,1=start 2,3=end) */
} NewSkid;

/**
 * CarParts structure - Pre-created objects attached to each car
 */
typedef struct CarParts {
    s32     frame;              /* Car frame/body object */
    s32     hood;               /* Hood object (for view 2) */
    s32     brakes;             /* Brake lights object */
    s32     blast;              /* Explosion object */
    s32     window[4];          /* Window objects (one per quadrant) */
    s32     tires[4];           /* Tire objects (one per wheel) */
    s32     quad[5];            /* Body quad objects (4 corners + top) */
    s32     smoke[SMOKE_OBJS];  /* Pre-created smoke objects */
    s8      nextSmoke;          /* Index of next smoke object to use */
    s32     lSpark;             /* Left spark object */
    s32     rSpark;             /* Right spark object */
    s32     bSpark;             /* Bottom spark object */
} CarParts;

/**
 * Dynamic texture types
 */
typedef enum DynTextures {
    TEX_SPARK1, TEX_SPARK2, TEX_SPARK3, TEX_SPARK4, TEX_SPARK5, TEX_SPARK6,
    TEX_SHADOW,
    TEX_SKID,
    TEX_FBALL00, TEX_FBALL01, TEX_FBALL02, TEX_FBALL03, TEX_FBALL04, TEX_FBALL05,
    TEX_FBALL06, TEX_FBALL07, TEX_FBALL08, TEX_FBALL09, TEX_FBALL10, TEX_FBALL11,
    TEX_FBALL12, TEX_FBALL13, TEX_FBALL14, TEX_FBALL15, TEX_FBALL16, TEX_FBALL17,
    TEX_FBALL18, TEX_FBALL19, TEX_FBALL20, TEX_FBALL21, TEX_FBALL22, TEX_FBALL23,
    TEX_BCLOUD, TEX_WCLOUD, TEX_BTUNNEL, TEX_WTUNNEL, TEX_BTUNNEL2, TEX_WTUNNEL2,
    TEX_WCLOUDA,
    NUM_DYN_TEXS
} DynTextures;

/* ======================= GLOBAL VARIABLES ======================== */

/* Smoke/skid intensity arrays (per-car, per-tire, 2 values) */
extern s16 skid_intensity[MAX_LINKS][4][2];
extern s16 smoke_intensity[MAX_LINKS][4][2];

/* Car model numbers for each car slot */
extern s16 gCarModelNums[NCARS];

/* View 2 spring/shake value */
extern f32 spring_save;

/* Damage shift amounts (from arcade) */
extern const u8 gDamageShift[5];

/* Damage masks (from arcade) */
extern const u32 gDamageMask[5];

/* Shadow scale factors per car body type [side, front, back] */
extern const f32 shadow_scale[NCARS][3];

/* ======================= FUNCTION DECLARATIONS ======================== */

/* ---- Initialization ---- */

/**
 * InitDynamicObjs - Pre-locate dynamic objects for cars and track
 * @param for_game TRUE if for actual gameplay, FALSE if for select screens
 */
void InitDynamicObjs(s32 for_game);

/**
 * InitVisuals - Initialize visual effects for the environment
 * @param for_game TRUE if for gameplay, FALSE for select screens
 */
void InitVisuals(s32 for_game);

/**
 * InitSkids - Initialize skid mark system
 */
void InitSkids(void);

/* ---- Car Management ---- */

/**
 * CreateCar - Create car objects and attach to car slot
 * @param slot Car slot index
 * @param for_game TRUE if for gameplay
 */
void CreateCar(s16 slot, s32 for_game);

/**
 * SetupCar - Configure car parts based on type and appearance
 * @param slot Car slot index
 * @param type Car type (or HULK_TYPE)
 * @param for_game TRUE if for gameplay
 */
void SetupCar(s16 slot, s16 type, s32 for_game);

/**
 * StartCar - Start visual animations for a car
 * @param slot Car slot index
 */
void StartCar(s16 slot);

/**
 * PickCar - Select car model based on slot and type
 * @param slot Car slot index
 * @param type Car type (or HULK_TYPE for destroyed car)
 * @return Object list index
 */
s32 PickCar(s16 slot, s16 type);

/**
 * RemoveCar - Remove car and its visuals
 * @param slot Car slot index
 */
void RemoveCar(s16 slot);

/**
 * SetCarReflection - Set reflection textures for car
 * @param slot Car slot index
 * @param wintex Window texture index
 * @param bodtex Body texture index
 * @param speed Reflection speed
 */
void SetCarReflection(s16 slot, s32 wintex, s32 bodtex, f32 speed);

/* ---- Environment ---- */

/**
 * StartEnvironment - Initialize environmental effects for all cars
 */
void StartEnvironment(void);

/**
 * UpdateEnvirons - Update all environmental visuals
 */
void UpdateEnvirons(void);

/**
 * RemoveEnvirons - Remove all environmental effects
 */
void RemoveEnvirons(void);

/* ---- Per-Car Updates ---- */

/**
 * UpdateVisuals - Update visual effects for a single car
 * @param slot Car slot index
 */
void UpdateVisuals(s16 slot);

/* ---- Visual List Management ---- */

/**
 * GrabEnvEntry - Allocate an entry from the environment list
 * @return Pointer to available Visual, or NULL if none
 */
Visual *GrabEnvEntry(void);

/**
 * AddToEnvList - Add a visual to the environment list
 * @param v Visual to add
 */
void AddToEnvList(Visual *v);

/**
 * ReleaseEnviron - Free an environmental entry
 * @param v Visual to release
 * @param unlink TRUE to remove from env list
 */
void ReleaseEnviron(Visual *v, s32 unlink);

/* ---- Individual Visual Effects ---- */

/**
 * StartFrame - Initialize car frame animation
 * @param slot Car slot index
 */
void StartFrame(s16 slot);

/**
 * StartBlast - Initiate explosion/destruction sequence
 * @param slot Car slot index
 */
void StartBlast(s16 slot);

/**
 * StartSparks - Enable sparks for a car
 * @param slot Car slot index
 */
void StartSparks(s16 slot);

/**
 * ShowHood - Show or hide hood based on camera view
 * @param slot Car slot index
 * @param show TRUE to show hood
 */
void ShowHood(s16 slot, s32 show);

/* ---- Skid Mark Functions ---- */

/**
 * GetSkid - Get a new skid from free list or steal farthest
 * @return Pointer to Skid, or NULL if none available
 */
Skid *GetSkid(void);

/**
 * AddSkid - Add unlinked skid to gSkidList
 * @param s Skid to add
 */
void AddSkid(Skid *s);

/**
 * ReleaseSkid - Release polygon, unlink from gSkidList, add to gFreeList
 * @param s Skid to release
 */
void ReleaseSkid(Skid *s);

/* ---- Utility Functions ---- */

/**
 * CheckVisible - Check and update visibility of a visual
 * @param v Visual to check
 * @param on Desired visibility state
 * @return TRUE if visible
 */
s32 CheckVisible(Visual *v, s32 on);

/**
 * CheckXlu - Check and update translucency of a visual
 * @param v Visual to check
 * @param lucent TRUE if should be translucent
 * @param xlu Translucency level (0x00-0xFF)
 * @return TRUE if translucent
 */
s32 CheckXlu(Visual *v, s32 lucent, s32 xlu);

/**
 * Transpose3x3 - Transpose a 3x3 matrix in place
 * @param mat Matrix to transpose
 */
void Transpose3x3(f32 mat[3][3]);

/* ---- Animation Functions (internal but exposed for reference) ---- */

void AnimateBlast(Visual *v, s16 op);
void AnimateBrakeLights(Visual *v, s16 op);
void AnimateLSpark(Visual *v, s16 op);
void AnimateRSpark(Visual *v, s16 op);
void AnimateBSpark(Visual *v, s16 op);
void AnimateSmoke(Visual *v, s16 op);
void AnimateTire(Visual *v, s16 op);
void AnimateShadow(Visual *v, s16 op);
void AnimateFrame(Visual *v, s16 op);
void AnimateQuad(Visual *v, s16 op);
void AnimateWindow(Visual *v, s16 op);

#endif /* VISUALS_H */
