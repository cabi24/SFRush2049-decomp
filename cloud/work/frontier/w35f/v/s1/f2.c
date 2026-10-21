/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w35f draft: image B shots.c cluster (MAT3 shots.c). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef union RGBA { u8 c[4]; u32 rgba; } RGBA;
typedef struct UVMat { f32 uvs[3][3]; f32 pos[3]; } UVMat;
typedef struct StatLL { s8 dbl; s32 numElements; s32 elementSize; void *list; void *head; void *store; } StatLL;

typedef struct Pad {
    u8 slot, controller; u8 pad02[2];
    u32 edges;            /* 0x04 */
    u32 levels;           /* 0x08 */
    u32 autoEdges;        /* 0x0c */
    f32 analog[2];        /* 0x10 */
    u32 controls[11];     /* 0x18 */
    u8 pad44[0x50 - 0x44];
} Pad;

typedef struct Defense {
    u32 active;           /* 0x00 */
    f32 invsTime;         /* 0x04 */
    f32 shldTime;         /* 0x08 */
    f32 shldScale;        /* 0x0c */
    u32 unknown10;        /* 0x10 */
} Defense;

typedef struct Battle {
    s8 type;              /* 0x00 */
    s8 ammoCnt;           /* 0x01 */
    s16 health;           /* 0x02 */
    s16 damageThisFrame;  /* 0x04 */
    u8 pad06[2];
    Defense defense;      /* 0x08 */
    s8 shoot;             /* 0x1c */
    u8 fadeAlpha;         /* 0x1d */
    s8 fadeState;         /* 0x1e */
    s8 Points;            /* 0x1f */
    s32 loaded;           /* 0x20 */
    f32 fadeTime;         /* 0x24 */
    f32 reloadingTime;    /* 0x28 */
    f32 pitch;            /* 0x2c */
    f32 yaw;              /* 0x30 */
} Battle;

typedef struct Car {
    u8 pad00[8];
    f32 pos[3];           /* 0x08 */
    f32 vel[3];           /* 0x14 */
    f32 acc[3];           /* 0x20 */
    f32 uvs[3][3];        /* 0x2c */
    u8 pad50[0x308 - 0x50];
    s8 collidable;        /* 0x308 */
    u8 pad309[0x34c - 0x309];
    RGBA IRclr;           /* 0x34c */
    u8 pad350[0x359 - 0x350];
    s8 dead;              /* 0x359 */
    u8 pad35a;
    s8 slot;              /* 0x35b */
    u8 pad35c[0x380 - 0x35c];
    Pad *player;          /* 0x380 */
    Battle battle;        /* 0x384 */
} Car;

typedef struct Model {
    u8 pad00[8];
    u8 body_type;         /* 0x08 */
    u8 pad09[0xf4 - 9];
    f32 bodyR[4][3];      /* 0xf4 */
    f32 centerForce[3];   /* 0x124 */
    u8 pad130[0x13c - 0x130];
    f32 centerMoment[3];  /* 0x13c */
    u8 pad148[0x640 - 0x148];
    s8 crashflag;         /* 0x640 */
    u8 pad641[0x654 - 0x641];
    f32 colrad;           /* 0x654 */
    u8 pad658[0x6c4 - 0x658];
    s16 moving_state;     /* 0x6c4 */
    u8 pad6c6[0x808 - 0x6c6];
} Model;

typedef struct ColSize { f32 colfront, colrear, colside, colheight; } ColSize;

typedef struct Streak {
    void *next, *prev;
    u32 flags;
    f32 delay;
    f32 newpos[3];
    f32 lastpos[3];
    f32 backVerts[2][3];
} Streak;

typedef struct ShotObj {
    struct ShotObj *next;
    u32 mbIdx;            /* 0x04 */
    UVMat mat;            /* 0x08 */
    struct Shot *shotInfo;/* 0x38 */
} ShotObj;

typedef struct Shot {
    struct Shot *next;    /* 0x00 */
    s8 slot;              /* 0x04 */
    s8 slotDamaged;       /* 0x05 */
    s8 type;              /* 0x06 */
    s8 flags;             /* 0x07 */
    f32 lifeTime;         /* 0x08 */
    f32 incTime;          /* 0x0c */
    f32 radius;           /* 0x10 */
    f32 vel[3];           /* 0x14 */
    f32 pos[3];           /* 0x20 */
    f32 old[3];           /* 0x2c */
    f32 uvs[3][3];        /* 0x38 */
    Streak *streak;       /* 0x5c */
    ShotObj *obj;         /* 0x60 */
    ShotObj *obj2;        /* 0x64 */
} Shot;

typedef struct ShotCase {
    struct ShotCase *next;
    u32 mbIdx;            /* 0x04 */
    UVMat mat;            /* 0x08 */
    f32 time;             /* 0x38 */
    f32 vel[3];           /* 0x3c */
} ShotCase;

typedef struct HitPoly { struct HitPoly *next; void *poly; } HitPoly;

typedef struct Shield { s32 dlIdx; UVMat mat; } Shield;
typedef struct TexDef { u8 b[36]; } TexDef;
typedef struct TexDefSet { TexDef *texDefs; s32 num; } TexDefSet;
typedef struct ObjInst { u8 pad00[12]; f32 scale; u8 pad10[0x44 - 16]; } ObjInst;

extern f32 D_8002EB94;          /* gFrameSeconds */
extern f32 D_80142764;          /* gGravity */
extern volatile s16 D_801543CA;          /* gNumCars */
extern Car D_80152818[];        /* gGameCar */
extern Model D_8014A250[];      /* model */
extern s8 D_8012E67C[];         /* gCarTeam */
extern ColSize D_8011F844[];    /* collsize */
extern f32 D_8011418C[];        /* unitmatrix */
extern f32 D_803943A4[8][13][3];/* weaponOffsets */
extern StatLL D_80394F70;       /* gShotObjList */
extern Shield D_80394F90[4];    /* gCarShield */
extern StatLL D_8039A520;       /* gShotList */
extern StatLL D_8039A5F8;       /* gHitPolyList */
extern StatLL D_8039AE80;       /* gShotCaseList */
extern s32 D_80399B18[10];      /* gWprObjList */
extern s32 D_80399B40[7];       /* gWfxObjList */
extern u16 D_80399AF8[5];       /* gMineTextures */
extern TexDefSet D_80151AE8[];  /* gTexDefPtrs */
extern ObjInst D_8012E700[];    /* ObjInstanceList */
extern u16 D_80394F88;          /* wallTidx */
extern u8 D_80140BDC;           /* gNumModels */
extern RGBA D_803942E8;         /* hitColor */

extern void *func_8008E3C0(void *list);         /* GetStatLLE */
extern void func_800AFA84(void *list, void *e); /* FreeStatLLE */
extern void sound_call_minimal(s16 obj);        /* ZOID_RemoveObject */
extern void func_8008E06C(s16 obj, RGBA *clr);  /* ZOID_SetColor */
extern void func_8008D0C0(void *poly);          /* ZOID_RemovePoly */
extern s32 sign_extend_call(s32 obj, void *mat, s32 parent, u32 flags); /* MBOX_NewObject */
extern void math_utility(f32 *src, f32 *dst);   /* CopyUV */
extern void func_80090F44(f32 a, f32 *uvs);     /* PitchUV */
extern void func_80090E9C(f32 a, f32 *uvs);     /* YawUV */
extern void func_8009EA68(f32 a, f32 *uvs);     /* RollUV */
extern f32 func_8008B2E4(f32 scale);            /* Random */
extern f32 func_8008C768(f32 y, f32 x);         /* fatan2 */
extern void func_800A61B0(f32 *v, f32 *out, f32 *uvs); /* BodyVector */
extern void func_800B61A8(s32 fx, s32 slot, s32 a, s32 b); /* PlayGameSound */
extern Streak *func_8038D054(s32 type, f32 *pos, f32 *pos2); /* NewStreak */
extern void func_8038CB20(void);                /* UpdateStreaks */
extern f32 sqrtf(f32);
extern f32 fabsf(f32);
#pragma intrinsic(sqrtf)
#pragma intrinsic(fabsf)
extern void func_8038D200(Shot *shot, s32 unt);
extern ShotObj *func_8038D328(s32 idx, s32 parent, u32 flags, s32 o2);
extern void func_8038E088(Shot *shot);
extern Car *func_8038DA78(Shot *shot, f32 *old);
extern void func_8038D498(Shot *shot);
extern void func_8038E114(void);                /* UpdateShots */
extern void func_8038D3A4(Car *car, Car *hit, s32 damage); /* BattleDamage */
extern void func_8038D798(f32 *pos, f32 *old, s32 owner, f32 radiussq, s32 damage); /* ProximityDamage */
extern void func_8038DDDC(Shot *shot);          /* MissileLock */
extern s32 input_deadzone_apply(f32 *old, f32 *pos, f32 *uvs, f32 radius, s32 a, s32 b); /* ShotToWorldCollision */
extern void func_8009E820(f32 *v, f32 *out, f32 *uvs);   /* WorldVector */
extern void func_8008D870(s16 obj, void *tex, s32 a);    /* ZOID_SetTexture */
extern void func_8008B32C(f32 *src, f32 *dst, f32 s);    /* ScaleUV */
extern void *func_800A78BC(s32 n, f32 *verts, u16 tidx, RGBA *color, s32 flags, s32 a); /* ZOID_NewPoly */
extern void func_800B24EC(char *name, u16 *out, s32 a, s32 b, s32 c); /* MBOX_FindTexture_WORK */
extern void transmission_ratio_get(char *name, s32 a, f32 *pos, s32 b, s32 slot, s32 c, s32 d); /* SetupTarget */
extern s32 camera_collision_check(f32 *pos, f32 *pos2, f32 a, f32 b, f32 c, f32 d, s32 e, s32 slot, s32 f, s32 g); /* EmitGameSound */
extern void save_write_data(f32 *pos, s32 a, f32 b, s32 c); /* StartBlast */

static const f32 CaseOffsets[1][3] = { 0.6f, 0.65f, -0.69f };
static const f32 ShotOffsets[10][3] = {
    0.16f, 0.49f, 0.57f, 0.01f, 0.65f, 1.59f, 0.0f, 0.9f, 1.1f, 0.0f, -0.3f, -0.7f,
    0.01f, 2.04f, 0.28f, 0.0f, 0.0f, 0.0f, -0.04f, 0.61f, 0.23f, 0.0f, 1.25f, 0.0f,
    0.0f, 1.5f, 2.0f, 0.0f, 0.0f, 0.0f
};
static const f32 gShieldSizes[13] = {
    0.85f, 0.8f, 0.75f, 0.77f, 0.83f, 0.85f, 0.75f, 0.75f, 0.65f, 0.85f, 1.1f, 0.8f, 0.73f
};
static s32 sWeaponFx[10] = { 92, 87, 90, 88, 91, 93, 94, 95, 89, -1 };

/* 0x8038D498 SonicCarCollision */
void func_8038D498(Shot *shot)
{
    Car *car;
    Car *our;
    Model *m;
    s32 i;
    s32 dam;
    f32 vec[3];
    f32 vec2[3];
    f32 dist;
    f32 rad;
    f32 mul;
    f32 scale;
    ShotObj *o = shot->obj;
    f32 pos[3];

    our = &D_80152818[shot->slot];
    scale = D_8012E700[o->mbIdx].scale;
    if (scale <= 8.0f) {
        mul = 1.0f;
        dam = 800;
    } else if (scale <= 20.0f) {
        mul = 0.6f;
        dam = 400;
    } else {
        dam = 200;
        mul = 0.5f;
    }
    pos[0] = o->mat.pos[0];
    pos[1] = o->mat.pos[1];
    pos[2] = o->mat.pos[2];
    for (i = 0, car = D_80152818; i < D_801543CA; i++, car++) {
        if (shot->slot == car->slot || D_8014A250[i].moving_state != -1 ||
            !car->collidable || (shot->slotDamaged & (1 << car->slot)))
            continue;
        vec[0] = car->pos[0] - pos[0];
        vec[1] = car->pos[1] - pos[1];
        vec[2] = car->pos[2] - pos[2];
        dist = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
        if (dist <= (4.0f * scale) * (4.0f * scale)) {
            shot->slotDamaged |= 1 << car->slot;
            func_8038D3A4(our, car, dam);
            m = &D_8014A250[car->slot];
            rad = sqrtf(dist);
            vec2[2] = mul * (330000.0f * (vec[2] / rad));
            vec2[1] = mul * (330000.0f * (vec[1] / rad) + 66000.0f);
            vec2[0] = mul * (330000.0f * (vec[0] / rad));
            func_800A61B0(vec2, vec, &car->uvs[0][0]);
            m->centerForce[0] += vec[0];
            m->centerForce[1] += vec[1];
            m->centerForce[2] += vec[2];
            vec[1] = 0.0f;
            m->centerMoment[0] += vec[0];
            m->centerMoment[1] += vec[1];
            m->centerMoment[2] += vec[2];
        }
    }
}

