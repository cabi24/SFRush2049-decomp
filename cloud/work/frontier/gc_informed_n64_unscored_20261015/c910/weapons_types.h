/* Research proposal only, for a private N64 IDO experiment. Not an applied
 * repository header, original N64 source, compiled match or new coverage.
 * GC names come from ELF DWARF; consumed N64 offsets come from image B at
 * c226123281724be9459ffa5d8ea41850bede3187. See ../RESEARCH.md.
 * Native 32-bit pointers are required. Preserve the existing lock contracts.
 * Source correction: locked func_800B27E4 initializes 21 N64 visuals at +0x110.
 * GC has 22 at +0x110. N64 weapon index20 remains at +0x2f0; no compiler proof.
 */
#ifndef RESEARCH_N64_WEAPONS_TYPES_H
#define RESEARCH_N64_WEAPONS_TYPES_H

typedef signed char WpnS8;
typedef unsigned char WpnU8;
typedef signed short WpnS16;
typedef signed int WpnS32;
typedef unsigned int WpnU32;
typedef float WpnF32;

typedef struct WpnMatrix {
    WpnF32 uvs[3][3];
    WpnF32 pos[3];
} WpnMatrix;                         /* 0x30 */

typedef struct WpnVisual WpnVisual;
struct WpnVisual {
    WpnVisual *next;                 /* 0x00, GC type; N64 prefix not used here */
    WpnS16 index;                    /* 0x04 */
    WpnS16 objnum;                   /* 0x06 */
    WpnS16 slot;                     /* 0x08 */
    WpnU32 data;                     /* 0x0c, GC type; not used by this callback */
    WpnF32 timeStamp;                /* 0x10 */
    void (*func)(WpnVisual *, WpnS16);/* 0x14: authentic a0 + signed low a1 */
};                                  /* 0x18 */

typedef struct WpnMuzzle {
    WpnS32 mdls[5];                  /* 0x000: full word, narrowed at helpers */
    WpnMatrix mats[5];               /* 0x014 */
    WpnS32 on;                       /* 0x104 */
    WpnF32 onTime;                   /* 0x108 */
} WpnMuzzle;                         /* 0x10c */

typedef struct WpnCannonMuzzle {
    WpnS32 mdls[5];                  /* 0x000 */
    WpnMatrix mats[5];               /* 0x014 */
    WpnS32 ripdl;                    /* 0x104 */
    WpnMatrix mat;                   /* 0x108 */
    WpnS8 ripCnt;                    /* 0x138, -1 = idle */
    WpnS8 ripTIdx;                   /* 0x139 */
    WpnS16 on;                       /* 0x13a */
    WpnF32 ripScale;                 /* 0x13c */
    WpnF32 ripTime;                  /* 0x140 */
    WpnF32 onTime;                   /* 0x144 */
} WpnCannonMuzzle;                   /* 0x148 */

typedef struct WpnDefense {
    WpnU32 active;                   /* 0x00 */
    WpnF32 invsTime;                 /* 0x04 */
    WpnF32 shldTime;                 /* 0x08 */
    WpnF32 shldScale;                /* 0x0c */
    WpnF32 shield_phase;             /* 0x10: N64 extra, consumed by PitchUV.
                                     * Descriptive name. Absent in GC's 0x10 type. */
} WpnDefense;                        /* 0x14 */

typedef struct WpnBattle {
    WpnS8 type, ammoCnt;              /* 0x00, 0x01 */
    WpnS16 health;                   /* 0x02 */
    WpnS16 damageThisFrame;          /* 0x04 */
    WpnU8 unknown06[2];
    WpnDefense defense;              /* 0x08 */
    WpnS8 shoot;                     /* 0x1c */
    WpnU8 fadeAlpha;                 /* 0x1d */
    WpnS8 fadeState, Points;          /* 0x1e, 0x1f */
    WpnS32 loaded;                   /* 0x20 */
    WpnF32 fadeTime;                 /* 0x24 */
    WpnF32 reloadingTime;            /* 0x28 */
    WpnF32 pitch, yaw;               /* 0x2c, 0x30; F568 writes these */
} WpnBattle;                         /* 0x34, GC counterpart is 0x30 */

/* Only this subsystem's consumed fields are named. Unknown arrays preserve
 * persistent record storage; they are not stack-pressure devices. */
typedef struct WpnCar {
    WpnU8 unknown000[0x110];         /* includes native +0xf8 rwmph; unused here */
    WpnVisual visuals[21];           /* +0x110; visuals[20] = car+0x2f0 */
    WpnS8 collidable;                /* 0x308 */
    WpnU8 unknown309[0x43];
    WpnU32 IRclr;                    /* 0x34c; RGBA union suitable for color use */
    WpnU8 unknown350[0xb];
    WpnS8 slot, camera, view;         /* 0x35b, 0x35c, 0x35d */
    WpnU8 unknown35e[0x26];
    WpnBattle battle;                /* 0x384 */
} WpnCar;                            /* 0x3b8 */

typedef struct WpnCarParts {
    WpnS32 frame;                    /* 0x00 */
    WpnU8 unknown04[0x14];
    WpnS32 weap;                     /* 0x18, GC CarParts.weap is +0x38 */
    WpnU8 unknown1c[0x24];
} WpnCarParts;                       /* 0x40, GC CarParts is 0x60 */

typedef struct WpnShotObject WpnShotObject;
struct WpnShotObject {
    WpnShotObject *next;             /* 0x00 */
    WpnU32 mbIdx;                    /* 0x04 */
    WpnMatrix mat;                   /* 0x08 */
    WpnU32 unknown38;                /* GC has shotInfo pointer here;
                                     * keep unknown until N64 consumers prove it. */
};                                  /* 0x3c */

typedef struct WpnShot WpnShot;
struct WpnShot {
    WpnShot *next;                   /* 0x00 */
    WpnS8 slot;                      /* 0x04, owner */
    WpnS8 slotDamaged;               /* 0x05, hit bitmap in SonicCarCollision */
    WpnS8 type, flags;               /* 0x06, 0x07 */
    WpnF32 lifeTime, incTime, radius; /* 0x08, 0x0c, 0x10 */
    WpnF32 vel[3], pos[3], old[3];    /* 0x14, 0x20, 0x2c */
    WpnF32 uvs[3][3];                /* 0x38 */
    void *streak;                    /* 0x5c, 0x40-byte trail record */
    WpnShotObject *obj, *obj2;        /* 0x60, 0x64 */
};                                  /* 0x68 */

/* GC spellings are hypotheses for N64 names; IDs are separate namespaces.
 * car.battle.type == 8 means NONE. shot.type == 8 renders a TRACER. */
#define WPN_NONE 8
#define WPN_CACHE_UNINITIALIZED 9
#define WPN_SHOT_TRACER 8

#define WPN_LAYOUT_ASSERT(name, condition) typedef char name[(condition) ? 1 : -1]
WPN_LAYOUT_ASSERT(wpn_native_pointers, sizeof(void *) == 4);
WPN_LAYOUT_ASSERT(wpn_visual_size, sizeof(WpnVisual) == 0x18);
WPN_LAYOUT_ASSERT(wpn_muzzle_size, sizeof(WpnMuzzle) == 0x10c);
WPN_LAYOUT_ASSERT(wpn_cannon_size, sizeof(WpnCannonMuzzle) == 0x148);
WPN_LAYOUT_ASSERT(wpn_defense_size, sizeof(WpnDefense) == 0x14);
WPN_LAYOUT_ASSERT(wpn_battle_size, sizeof(WpnBattle) == 0x34);
WPN_LAYOUT_ASSERT(wpn_car_size, sizeof(WpnCar) == 0x3b8);
WPN_LAYOUT_ASSERT(wpn_parts_size, sizeof(WpnCarParts) == 0x40);
WPN_LAYOUT_ASSERT(wpn_shot_size, sizeof(WpnShot) == 0x68);
WPN_LAYOUT_ASSERT(wpn_object_size, sizeof(WpnShotObject) == 0x3c);
#undef WPN_LAYOUT_ASSERT
#endif
