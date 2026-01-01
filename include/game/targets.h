/**
 * targets.h - Collidable target objects for Rush 2049 N64
 *
 * Based on arcade game/targets.c
 * Handles destructible objects like cones, poles, trees, fences, etc.
 *
 * Original arcade code by Time Warner Interactive (1996).
 * See: reference/repos/rushtherock/game/targets.h
 */

#ifndef TARGETS_H
#define TARGETS_H

#include "types.h"
#include "game/structs.h"

/* ========================== DEFINES ========================== */

/* Maximum targets per track */
#define MAX_TARGETS         300
#define MAX_TARG_GROUPS     30

/* Car collision radius (feet) */
#define CAR_RADIUS          11.2f

/* One second in milliseconds (arcade uses IRQTIME) */
#define ONE_SEC             1000

/* Fog modes */
#define FOG_INIT            0
#define FOG_OFF             1
#define FOG_TINT            2
#define FOG_TRACK           3
#define FOG_TRACK_TINT      4

/* ========================== ENUMS ========================== */

/**
 * Target types - indexed from MultiGen linker file
 * Based on arcade targets.h TargetType enum
 */
typedef enum TargetType {
    TARG_NONE = 0,
    TARG_CONE,
    TARG_METER,
    TARG_FENCE,
    TARG_POLE,
    TARG_TREE,
    TARG_CROWD,
    TARG_WINDOWA,
    TARG_WINDOWB,
    TARG_FOG,
    TARG_FRONTFACE,
    TARG_CABLECAR,
    TARG_SPEAKER,
    TARG_FIRECRACKER,
    TARG_SMALLHOOT,
    TARG_SMALLCLAP,
    TARG_MARKER,
    TARG_BIGCHEER,
    TARG_BIGCHEER2,
    TARG_GONG,
    TARG_BUSH,
    TARG_FINISH,
    TARG_TIME,
    TARG_FLAG,
    TARG_SIREN,
    NUM_TARGTYPES
} TargetType;

/* ========================== TYPEDEFS ========================== */

/* Forward declarations */
struct Target;
struct TargetGroup;
struct Visual;
struct OBJANIM;

/**
 * Visual function pointer - animation update callback
 * @param v Visual effect pointer
 * @param op Operation code (0 = cleanup, 1 = update)
 */
typedef void (*VisFunc)(struct Visual *v, s16 op);

/**
 * Target function pointer - collision response callback
 * @param t Target that was hit
 * @param data Slot index of car that hit the target
 */
typedef void (*TargFunc)(struct Target *t, u32 data);

/**
 * MBAnimFunc - Mathbox animation function pointer
 * Called during world loading to set up target objects
 * @param mbidx Mathbox object index
 * @param dlidx Display list object index
 * @param oanim Object animation data pointer
 */
typedef void (*MBAnimFunc)(s32 mbidx, s32 dlidx, struct OBJANIM *oanim);

/**
 * OBJANIM - Object animation data from world file
 * Based on arcade mb.h OBJANIM struct
 * Populated by tools from MultiGen comments
 */
typedef struct OBJANIM {
    s32     id;             /* Object/polygon ID */
    s32     odata[2];       /* User-defined parameters from linker */
    s32     udata[2];       /* User-defined parameters from MultiGen */
} OBJANIM;

/**
 * MBOXANIMFUNC - Mathbox animation function table entry
 * Used with MBOX_ResetUnlockedModels to map types to functions
 */
typedef struct MBOXANIMFUNC {
    MBAnimFunc func;        /* Function pointer */
} MBOXANIMFUNC;

/**
 * Target - Individual target object
 * Based on arcade targets.h Target struct
 */
typedef struct Target {
    struct Target   *next;          /* Next target in this group */
    OBJANIM         *oanim;         /* Animation data pointer */
    s32             objnum;         /* Object number for rendering */
    f32             pos[3];         /* Position in world coords */
    f32             uv[3][3];       /* Orientation matrix */
    f32             vel[3];         /* Linear velocity (for dynamics) */
    f32             angV[3];        /* Angular velocity (roll, pitch, yaw) */
    s16             data;           /* Type-specific data storage */
    s16             type;           /* Current object type index */
    f32             radius;         /* Collision radius */
    TargFunc        func;           /* Collision response function */
    VisFunc         VisFunc;        /* Animation controller function */
    BOOL            active;         /* True if target is strikable */
    char            name[15];       /* Debug: type name */
    s16             otype;          /* Original starting object type */
    s16             frames;         /* Number of animation frames */
} Target;

/**
 * TargetGroup - Group of related targets
 * Based on arcade targets.h TargetGroup struct
 * Used for spatial acceleration of collision checks
 */
typedef struct TargetGroup {
    s16             type;           /* Type of targets in group */
    s16             count;          /* Number of targets in group */
    f32             radius;         /* Bounding radius for group */
    f32             dist;           /* Distance from camera */
    f32             pos[3];         /* Center position */
    f32             min[3];         /* Bounding box minimum */
    f32             max[3];         /* Bounding box maximum */
    struct Target   *list;          /* First target in group */
    struct Target   *last;          /* Last target in group */
} TargetGroup;

/**
 * Visual - Visual effect state for animated targets
 * Based on arcade visuals.h Visual struct
 */
typedef struct Visual {
    s32             index;          /* Animation frame index */
    VisFunc         func;           /* Animation function */
    u32             data;           /* Associated data (Target pointer) */
    u32             timeStamp;      /* Animation start time */
    struct Visual   *next;          /* Next visual in list */
} Visual;

/* ========================== GLOBALS ========================== */

/* Target list for entire track */
extern Target gTargetList[];

/* Current active fog group */
extern TargetGroup *gFogGroup;

/* Finish line object info */
extern u32 gFinishObject[];
extern s32 gFinishPoly[];
extern s32 num_finish_polys;

/* ========================== FUNCTIONS ========================== */

/* Initialization */
void SetupTargets(s16 track);
void SetTargets(void);

/* Main update/check */
void TargetCheck(s16 slot);
void TargetAppearance(s16 ID, u8 flag, u32 dataV, u32 appearA);

/* Collision testing */
BOOL car_inside_target(void *m, Target *t);

/* Finish line */
void ChangeFinishLine(void);

/* Fog control */
void SetFog(s32 mode, u32 density, u32 color);
void UpdateFog(void);
void FogOff(void);
void FogTint(void);
void FogTrack(void);
void FogTrackNormal(s32 force);
void FogTrackTarget(s32 force);
void FogTrackTint(void);
void MakeFogRamp(u32 color, f32 mindist, f32 maxdist, f32 minfog, f32 maxfog);

/* Visual effect helpers */
Visual *GrabEnvEntry(void);
void ReleaseEnviron(Visual *v, BOOL reset);
void AddToEnvList(Visual *v);

/* Sound */
void target_sound(Target *t, u32 slot);

/* Mathbox interface stubs */
void MBOX_ResetUnlockedModels(const MBOXANIMFUNC *funcs, s32 numfuncs);
f32 MBOX_GetObjectRadius(s32 mbidx);
void MBOX_HideObject(s32 objnum, s32 mode);
s32 MBOX_FindTexture(const char *name, s32 *texid);

/* ZOID interface stubs (arcade rendering system) */
void ZOID_SetObjectDef(s32 objnum, s32 objdef);
u32 ZOID_ObjectFlags(s32 objnum);
void ZOID_SetObjectFlags(s32 objnum, u32 flags);
void ZOID_UpdateObject(s32 objnum, f32 *pos, f32 uv[3][3]);
void ZOID_ChangeTexture(s32 objnum, s32 lod, s32 polyid, s32 texid);

/* Object list stub */
extern s32 gObjList[];

/* Object list instance matrix stub */
typedef struct {
    f32 mat[4][4];
} ObjInstance;
extern ObjInstance ObjInstanceList[];

/* ZATR flags */
#define ZATR_FRONTFACE  0x0001
#define HIDE_ONLY       0x0001

/* ========================== ARCADE ALIASES ========================== */

/* For arcade source compatibility */
#define nil             NULL
#define S32             s32
#define S16             s16
#define S08             s8
#define U32             u32
#define U16             u16
#define U08             u8
#define F32             f32

#endif /* TARGETS_H */
