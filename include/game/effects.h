/**
 * effects.h - Visual effects and particle system for Rush 2049 N64
 *
 * Handles tire smoke, sparks, dust, water splash, and other particles.
 */

#ifndef EFFECTS_H
#define EFFECTS_H

#include "types.h"

/* Maximum particles */
#define MAX_PARTICLES       256
#define MAX_PARTICLE_TYPES  16

/* Particle types */
#define PARTICLE_NONE       0
#define PARTICLE_SMOKE      1   /* Tire smoke */
#define PARTICLE_DUST       2   /* Dust from dirt/grass */
#define PARTICLE_SPARK      3   /* Collision sparks */
#define PARTICLE_WATER      4   /* Water splash */
#define PARTICLE_FIRE       5   /* Fire/explosion */
#define PARTICLE_DEBRIS     6   /* Car debris */
#define PARTICLE_SKID       7   /* Skid mark particles */
#define PARTICLE_EXHAUST    8   /* Exhaust smoke */
#define PARTICLE_BOOST      9   /* Boost effect */
#define PARTICLE_TRAIL      10  /* Wheel trail */

/* Particle flags */
#define PARTICLE_FLAG_ACTIVE    0x01
#define PARTICLE_FLAG_FADE      0x02    /* Fading out */
#define PARTICLE_FLAG_GROW      0x04    /* Growing */
#define PARTICLE_FLAG_SHRINK    0x08    /* Shrinking */
#define PARTICLE_FLAG_GRAVITY   0x10    /* Affected by gravity */
#define PARTICLE_FLAG_COLLIDE   0x20    /* Collides with ground */
#define PARTICLE_FLAG_TRAIL     0x40    /* Leaves trail */

/* Effect emitter types */
#define EMITTER_NONE        0
#define EMITTER_TIRE        1   /* Attached to tire */
#define EMITTER_EXHAUST     2   /* Attached to exhaust */
#define EMITTER_POINT       3   /* Fixed point emitter */
#define EMITTER_BURST       4   /* One-shot burst */

/* Colors (RGBA8888) */
#define COLOR_SMOKE_WHITE   0xFFFFFF80
#define COLOR_SMOKE_BLACK   0x40404080
#define COLOR_DUST_BROWN    0xA0805080
#define COLOR_DUST_YELLOW   0xC0B06080
#define COLOR_SPARK_ORANGE  0xFF8000FF
#define COLOR_SPARK_WHITE   0xFFFFFFFF
#define COLOR_WATER_BLUE    0x8080FFFF
#define COLOR_FIRE_ORANGE   0xFF6000FF
#define COLOR_FIRE_RED      0xFF2000FF

/* Single particle */
typedef struct Particle {
    f32     pos[3];         /* World position */
    f32     vel[3];         /* Velocity */
    f32     acc[3];         /* Acceleration */
    f32     size;           /* Current size */
    f32     size_vel;       /* Size change rate */
    f32     rotation;       /* Rotation angle */
    f32     rot_vel;        /* Rotation velocity */
    u32     color;          /* Current color (RGBA) */
    u32     start_color;    /* Initial color */
    u32     end_color;      /* Final color */
    u16     life;           /* Remaining lifetime (frames) */
    u16     max_life;       /* Initial lifetime */
    u8      type;           /* Particle type */
    u8      flags;          /* Particle flags */
    u8      alpha;          /* Current alpha */
    u8      texture;        /* Texture ID */
} Particle;

/* Particle emitter */
typedef struct Emitter {
    f32     pos[3];         /* Emitter position */
    f32     dir[3];         /* Emission direction */
    f32     spread;         /* Spread angle (radians) */
    f32     speed_min;      /* Min initial speed */
    f32     speed_max;      /* Max initial speed */
    f32     size_min;       /* Min particle size */
    f32     size_max;       /* Max particle size */
    u16     life_min;       /* Min lifetime (frames) */
    u16     life_max;       /* Max lifetime (frames) */
    u16     rate;           /* Particles per frame (fixed point 8.8) */
    u16     accumulator;    /* Rate accumulator */
    u32     start_color;    /* Starting color */
    u32     end_color;      /* Ending color */
    u8      type;           /* Emitter type */
    u8      particle_type;  /* Type of particles to emit */
    u8      active;         /* Is emitter active */
    u8      flags;          /* Emitter flags */
    s32     car_index;      /* Attached car (-1 if none) */
    s32     attach_point;   /* Attachment point on car */
} Emitter;

/* Skid mark segment */
typedef struct SkidMark {
    f32     start_pos[3];   /* Start position */
    f32     end_pos[3];     /* End position */
    f32     width;          /* Mark width */
    u8      alpha;          /* Visibility */
    u8      surface;        /* Surface type */
    u8      active;         /* Is active */
    u8      pad;
} SkidMark;

/* Effect system state */
typedef struct EffectsState {
    s32         num_active_particles;
    s32         num_active_emitters;
    s32         num_skid_marks;
    u8          effects_enabled;
    u8          particle_density;   /* 0-100% */
    u8          pad[2];
} EffectsState;

/* Global state */
extern Particle gParticles[];
extern Emitter gEmitters[];
extern SkidMark gSkidMarks[];
extern EffectsState gEffects;

/* Initialization */
void effects_init(void);
void effects_reset(void);

/* Per-frame update */
void effects_update(void);
void effects_draw(void);

/* Particle management */
s32 particle_spawn(u8 type, f32 pos[3], f32 vel[3], f32 size, u16 life);
void particle_spawn_burst(u8 type, f32 pos[3], s32 count, f32 spread, f32 speed);
void particle_kill(s32 index);
void particle_kill_all(void);

/* Emitter management */
s32 emitter_create(u8 type, f32 pos[3], u8 particle_type);
void emitter_destroy(s32 index);
void emitter_set_rate(s32 index, u16 rate);
void emitter_set_active(s32 index, u8 active);
void emitter_attach_to_car(s32 emitter, s32 car_index, s32 attach_point);

/* Car-specific effects */
void effects_tire_smoke(s32 car_index, s32 tire, f32 slip);
void effects_tire_dust(s32 car_index, s32 tire, u8 surface);
void effects_sparks(f32 pos[3], f32 vel[3], s32 count);
void effects_water_splash(f32 pos[3], f32 speed);
void effects_collision(f32 pos[3], f32 force, u8 type);
void effects_boost_flame(s32 car_index, f32 intensity);
void effects_exhaust(s32 car_index, f32 throttle);

/* Skid marks */
void skidmark_start(s32 car_index, s32 tire, f32 pos[3]);
void skidmark_update(s32 car_index, s32 tire, f32 pos[3]);
void skidmark_end(s32 car_index, s32 tire);
void skidmark_draw_all(void);

/* Utility */
void effects_set_density(u8 percent);
void effects_enable(s32 enable);

/* ========================================================================
 * ARCADE-COMPATIBLE VISUAL EFFECTS (from Rush The Rock visuals.c/visuals.h)
 * ======================================================================== */

/* Maximum visual effects */
#define MAX_VISUALS         100     /* Max concurrent environmental visuals */
#define SMOKE_FRAMES        16      /* Number of frames in smoke sequence */
#define SMOKE_OBJS          (3*SMOKE_FRAMES)  /* Smoke objects per tire */
#define SKID_OBJS           100     /* Number of skid mark objects */
#define SKID_DEVIATION      1.0f    /* Max deviation from straight skid allowed */
#define BLAST_FRAMES        16      /* Number of frames in blast sequence */
#define BLAST_HOLD          40      /* msecs to hold each frame of blast animation */

/* Visual types (from arcade visuals.h) */
typedef enum VisualTypes {
    VIS_BLAST = 0,
    VIS_BRAKES,
    VIS_LSPARK,
    VIS_RSPARK,
    VIS_BSPARK,
    VIS_TIRE1,
    VIS_TIRE2,
    VIS_TIRE3,
    VIS_TIRE4,
    VIS_SHADOW,
    VIS_QUAD0, VIS_QUAD1, VIS_QUAD2, VIS_QUAD3, VIS_QUAD4,
    VIS_WINDOW0, VIS_WINDOW1, VIS_WINDOW2, VIS_WINDOW3,
    VIS_FRAME,
    NUM_VISUALS_TYPES
} VisualTypes;

/* Appearance mask bits (from arcade dis.h) - Car visual appearance flags */
#define App_M_PAINT         0x00000001  /*  1 = team, 0 = normal (currently unused) */
#define App_M_DAMAGE        0x00000018  /*  Generic damage (used for hulk and translucency) */
#define App_M_SMOKE         0x00000060  /*  Engine smoke */
#define App_M_TRAILS        0x00000180  /*  Trailing effects (Used for L&R sparks) */
#define App_M_HATCH         0x00000C00  /*  Hatch (Used for bottom sparks) */
#define App_M_LIGHTS        0x00007000  /*  Any Lights (high bit unused) */
#define App_M_HEAD_LIGHTS   0x00002000  /*    Head lights (unused) */
#define App_M_BRAKE_LIGHTS  0x00001000  /*    Brake lights */
#define App_M_FLAMES        0x00008000  /*  Flames rising from entity (currently unused) */
#define App_M_FROZEN        0x00200000  /*  Entity is frozen (currently unused) */
#define App_M_DEACTIVATED   0x00400000  /*  Entity is deactivated */

/* Car damage appearance bits */
#define App_M_ANY_DAMAGE    0xFFC00000  /*  Any damage to car */
#define App_M_LF_DAMAGE     0xC0000000  /*  4 levels, left-front */
#define App_M_RF_DAMAGE     0x30000000  /*  4 levels, right-front */
#define App_M_LR_DAMAGE     0x0C000000  /*  4 levels, left-rear */
#define App_M_RR_DAMAGE     0x03000000  /*  4 levels, right-rear */
#define App_M_TOP_DAMAGE    0x00C00000  /*  4 levels, top */

/* Tire smoke appearance bits */
#define App_M_TIRE_SMOKE    0x000F0000  /*  Any Tire Smoke */
#define App_M_LF_SMOKE      0x00080000  /*    Left Front Smoke */
#define App_M_RF_SMOKE      0x00040000  /*    Right Front Smoke */
#define App_M_LR_SMOKE      0x00020000  /*    Left Rear Smoke */
#define App_M_RR_SMOKE      0x00010000  /*    Right Rear Tire Smoke */

/* Skid mark appearance bits */
#define App_M_SKID_MARK     0x00000606  /*  Any Tire leaving skid marks */
#define App_M_LF_SKID       0x00000002  /*    Left Front Skid */
#define App_M_RF_SKID       0x00000004  /*    Right Front Skid */
#define App_M_LR_SKID       0x00000200  /*    Left Rear Skid */
#define App_M_RR_SKID       0x00000400  /*    Right Rear Tire Skid */

/* Spark appearance bits */
#define App_M_SPARKS        0x00000980  /*  Any spark */
#define App_M_LSPARK        0x00000100  /*    Left side sparks */
#define App_M_RSPARK        0x00000080  /*    Right side sparks */
#define App_M_BSPARK        0x00000800  /*    Bottom sparks */

/* Car state appearance bits */
#define App_M_HULK          0x00000010  /*  Car is a hulk (wrecked) */
#define App_M_TRANSLUCENT   0x00000008  /*  Car is translucent */

/* Mirroring helper bits for appearance swapping */
#define App_SR2BITS (App_M_LF_DAMAGE|App_M_LR_DAMAGE)
#define App_SL2BITS (App_M_RF_DAMAGE|App_M_RR_DAMAGE)
#define App_SH2BITS (App_SR2BITS|App_SL2BITS)
#define App_SR1BITS (App_M_LF_SMOKE|App_M_LR_SMOKE|App_M_RF_SKID|App_M_RR_SKID|App_M_LSPARK)
#define App_SL1BITS (App_M_RF_SMOKE|App_M_RR_SMOKE|App_M_LF_SKID|App_M_LR_SKID|App_M_RSPARK)
#define App_SH1BITS (App_SR1BITS|App_SL1BITS)
#define App_SHBITS  (App_SH2BITS|App_SH1BITS)

/* Visual control bits */
#define VIS_BIT             0x10    /*  Bit used in data field to indicate visibility */
#define XLU_BIT             0x20    /*  Bit used in data field to indicate translucency */
#define HULK_BIT            0x40    /*  Bit used in data field to indicate car is hulk */
#define HOOD_BIT            0x80    /*  Bit used in data field to indicate car view 2 */

/* Surface type codes for smoke selection (from arcade) */
#define SURFACE_AIR         0       /*  In air (no surface contact) */
#define SURFACE_PAVEMENT    1       /*  Pavement - tire smoke */
#define SURFACE_DIRT        2       /*  Dirt - dust particles */
#define SURFACE_WATER       3       /*  Water - splash particles */
#define SURFACE_GRASS       4       /*  Grass - grass/dust particles */

/* Smoke intensity array index */
#define MAX_LINKS           8       /*  Max cars active at any one time */

/* Forward declarations for arcade types */
struct Visual;
struct Skid;
struct NewSkid;

/* Arcade Visual structure (used for visual overlays) */
typedef struct Visual {
    struct Visual *next;        /* Next node in linked list */
    s32     index;              /* Index of animation sequence */
    s32     objnum;             /* Object number of overlay */
    u32     slot;               /* Slot number of the visual owner's car */
    u32     data;               /* Used to store arbitrary data */
    u32     timeStamp;          /* Used to set last time updated */
    void    (*func)(struct Visual *v, s16 op);  /* Controller function */
} Visual;

/* Arcade Skid structure */
typedef struct Skid {
    struct Skid *prev;          /* Links for gSkidFree and gSkidList */
    struct Skid *next;          /* Links for gSkidFree and gSkidList */
    u32     lastTime;           /* IRQTIME of last update */
    s32     objnum;             /* Mathbox polygon index */
    f32     pos[3];             /* Skid center (used for removing most distant skid) */
    u8      xlu;                /* Skid translucency level (0x00 to 0xff) */
} Skid;

/* Arcade NewSkid structure (skid being created) */
typedef struct NewSkid {
    Skid    *skid;              /* Unlinked Skid for creation (nil if no skid in progress) */
    f32     start[3];           /* Start position of poly */
    f32     end[3];             /* End position of poly */
    f32     lensq;              /* Skid length squared (0.0 when initially created) */
    f32     dir[3];             /* Skid direction vector (valid if lensq > 0.0) */
    f32     vert[4][3];         /* Skid polygon vertices (0,1=start 2,3=end) */
} NewSkid;

/* Typedef for visual function pointer */
typedef void (*VisFunc)(struct Visual *v, s16 op);

/* Global smoke/skid intensity arrays */
extern s16 skid_intensity[MAX_LINKS][4][2];
extern s16 smoke_intensity[MAX_LINKS][4][2];

/* Arcade-compatible function declarations */
void InitDynamicObjs(s32 for_game);
void UpdateVisuals(s32 slot);
void StartSparks(s16 slot);
void StartSmoke(s16 slot, u32 tire, s32 fast);
void DoSkid(s16 slot, u32 tire, s32 on);
void AnimateSkids(void);

/* Visual system management */
Visual *GrabEnvEntry(void);
void AddToEnvList(Visual *v);
void ReleaseEnviron(Visual *v, s32 hide);
void RemoveVisuals(s16 slot);

/* Skid management */
Skid *GetSkid(void);
void AddSkid(Skid *s);
void ReleaseSkid(Skid *s);
void StartSkid_m(NewSkid *ns, s16 slot, u32 tire);
void ContinueSkid_m(NewSkid *ns, s16 slot, u32 tire);
void StopSkid(NewSkid *ns);
void UpdateSkid_m(NewSkid *ns, s16 slot, u32 tire);

/* Visual animation functions */
void AnimateSmoke(Visual *v, s16 op);
void AnimateLSpark(Visual *v, s16 op);
void AnimateRSpark(Visual *v, s16 op);
void AnimateBSpark(Visual *v, s16 op);
void HandleASpark(Visual *v, s32 on, f32 x, f32 y, f32 z, s32 spark, s16 op);

/* Utility functions */
s32 CheckVisible(Visual *v, s32 on);
s32 CheckXlu(Visual *v, s32 lucent, s32 xlu);

#endif /* EFFECTS_H */
