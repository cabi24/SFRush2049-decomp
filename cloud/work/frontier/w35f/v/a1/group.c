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
    u8 pad124[0x640 - 0x124];
    s8 crashflag;         /* 0x640 */
    u8 pad641[0x654 - 0x641];
    f32 colrad;           /* 0x654 */
    u8 pad658[0x808 - 0x658];
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
    s32 mbIdx;            /* 0x04 */
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
    s32 mbIdx;            /* 0x04 */
    UVMat mat;            /* 0x08 */
    f32 time;             /* 0x38 */
    f32 vel[3];           /* 0x3c */
} ShotCase;

typedef struct HitPoly { struct HitPoly *next; void *poly; } HitPoly;

typedef struct Shield { s32 dlIdx; UVMat mat; } Shield;

extern f32 D_8002EB94;          /* gFrameSeconds */
extern f32 D_80142764;          /* gGravity */
extern s16 D_801543CA;          /* gNumCars */
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
#pragma intrinsic(sqrtf)
extern void func_8038E114(void);                /* UpdateShots */

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

/* 0x8038D200 _UpdateShotObject */
void func_8038D200(Shot *shot, s32 unt)
{
    ShotObj *o = shot->obj;
    o->mat.pos[0] = shot->pos[0];
    o->mat.pos[1] = shot->pos[1];
    o->mat.pos[2] = shot->pos[2];
    if (unt == 1)
        math_utility(D_8011418C, &o->mat.uvs[0][0]);
    else if (unt != 2)
        math_utility(&shot->uvs[0][0], &o->mat.uvs[0][0]);

    if (shot->flags & 0x70) {
        if ((shot->flags & 0x80) && (shot->flags & 0x10) &&
            (shot->type == 1 || shot->type == 8)) {
reduceScale:
            o->mat.uvs[2][0] *= 0.2f;
            o->mat.uvs[2][1] *= 0.2f;
            o->mat.uvs[2][2] *= 0.2f;
        } else if (shot->type == 0) {
            if (shot->flags & 0x80) {
                goto reduceScale;
            } else {
                o->mat.uvs[2][0] *= 0.4f;
                o->mat.uvs[2][1] *= 0.4f;
                o->mat.uvs[2][2] *= 0.4f;
            }
        }
    }
    if (shot->flags & 2)
        func_80090F44(0.18f, &o->mat.uvs[0][0]);
}

/* 0x8038D328 _CreateShotObject */
ShotObj *func_8038D328(s32 idx, s32 parent, u32 flags, s32 o2)
{
    ShotObj *o;
    o = func_8008E3C0(&D_80394F70);
    if (o2)
        o->mbIdx = sign_extend_call(D_80399B18[idx], 0, parent, flags);
    else
        o->mbIdx = sign_extend_call(D_80399B18[idx], &o->mat, parent, flags);
    return o;
}

/* 0x8038E088 ShotDone */
void func_8038E088(Shot *shot)
{
    ShotObj *o;
    switch (shot->type) {
    case 3:
        o = shot->obj2;
        sound_call_minimal(o->mbIdx);
        func_800AFA84(&D_80394F70, o);
    case 0:
    case 1:
    case 2:
    case 4:
    case 6:
    case 7:
    case 8:
        o = shot->obj;
        sound_call_minimal(o->mbIdx);
        func_800AFA84(&D_80394F70, o);
        break;
    }
}

/* 0x8038F568 ElevateGun */
void func_8038F568(Car *car)
{
    Car *hit;
    f32 vec[3];
    f32 pos[3];
    f32 best = 2000.0f;
    f32 pitch = 0.0f;
    f32 yaw = 0.0f;
    f32 h;

    for (hit = D_80152818; hit < &D_80152818[D_801543CA]; hit++) {
        if (car->slot == hit->slot || D_8012E67C[car->slot] == D_8012E67C[hit->slot] ||
            D_8014A250[hit->slot].crashflag || !hit->collidable || hit->dead)
            continue;
        pos[0] = hit->pos[0] - car->pos[0];
        pos[1] = hit->pos[1] - car->pos[1];
        pos[2] = hit->pos[2] - car->pos[2];
        if (car->battle.type >= 8)
            h = -1.0f * ShotOffsets[car->battle.type][1];
        else
            h = -1.0f * (ShotOffsets[car->battle.type][1] +
                  D_803943A4[car->battle.type][D_8014A250[car->slot].body_type][1]);
        pos[0] += car->uvs[1][0] * h;
        pos[1] += car->uvs[1][1] * h;
        pos[2] += car->uvs[1][2] * h;
        func_800A61B0(pos, vec, &car->uvs[0][0]);
        if (vec[2] < 0.0f || vec[0] < -0.5f * vec[2] || vec[0] > 0.5f * vec[2] ||
            vec[1] < -vec[2] || vec[1] > vec[2] || vec[2] > best)
            continue;
        best = vec[2];
        yaw = func_8008C768(vec[0], vec[2]);
        pitch = func_8008C768(vec[1], vec[2]);
    }
    if (yaw < car->battle.yaw)
        car->battle.yaw -= 0.01f;
    else if (yaw > car->battle.yaw)
        car->battle.yaw += 0.01f;
    if (pitch < car->battle.pitch)
        car->battle.pitch -= 0.01f;
    else if (pitch > car->battle.pitch)
        car->battle.pitch += 0.01f;
}

/* 0x8038F938 UpdateDefense */
void func_8038F938(Car *car, Model *m, Defense *defense, s32 i)
{
    RGBA clr = { { 255, 255, 255, 255 } };
    s32 j, k;
    Shield *shield;
    UVMat *mat;

    if ((defense->active & 1) && car->battle.fadeState != 2) {
        defense->invsTime -= D_8002EB94;
        if (defense->invsTime <= 0.0f || m->crashflag) {
            defense->invsTime = 0.0f;
            car->battle.fadeState = 2;
            car->battle.fadeTime = 0.666667f;
            car->battle.fadeAlpha = 16;
        }
    }
    if (defense->active & 2) {
        if (m->crashflag)
            goto removeShield;
        shield = &D_80394F90[i];
        mat = &shield->mat;
        shield->mat.pos[0] = car->pos[0];
        shield->mat.pos[1] = car->pos[1];
        shield->mat.pos[2] = car->pos[2];
        math_utility(&car->uvs[0][0], &mat->uvs[0][0]);
        defense->shldTime -= D_8002EB94;
        if (defense->shldTime <= 0.0f) {
            if (defense->active & 8) {
                if (defense->shldScale >= gShieldSizes[m->body_type]) {
                    defense->shldScale = gShieldSizes[m->body_type];
                    defense->active &= ~8;
                    defense->active |= 4;
                    defense->shldTime = 30.0f;
                } else {
                    defense->shldScale += 0.05f;
                    if (defense->shldScale > gShieldSizes[m->body_type])
                        defense->shldScale = gShieldSizes[m->body_type];
                    defense->shldTime = 0.0333333f;
                }
            } else if (defense->active & 16) {
                if (defense->shldScale <= 0.06f) {
removeShield:
                    defense->shldTime = 0.0f;
                    defense->active &= ~0x1e;
                    if (D_80394F90[i].dlIdx != -1) {
                        sound_call_minimal(D_80394F90[i].dlIdx);
                        D_80394F90[i].dlIdx = -1;
                    }
                    return;
                } else {
                    defense->shldScale -= 0.05f;
                    defense->shldTime = 0.0333333f;
                }
            } else {
                defense->active &= ~4;
                defense->active |= 16;
                defense->shldTime = 0.0333333f;
            }
        }
        for (j = 0; j < 3; j++)
            for (k = 0; k < 3; k++)
                shield->mat.uvs[j][k] *= defense->shldScale;
        if (shield->dlIdx == -1)
            shield->dlIdx = sign_extend_call(D_80399B40[5], mat, -1, 0x802000);
        if (car->battle.defense.active & 1) {
            clr.c[3] = car->battle.fadeAlpha;
            if (clr.c[3] < 48)
                clr.c[3] = 48;
        }
        func_8008E06C(shield->dlIdx, &clr);
    }
}

/* 0x8038FCE0 ShotUpdate */
void func_8038FCE0(void)
{
    s32 i;
    s32 StartStreak;
    s32 streaktype;
    Shot *shot;
    ShotCase *h;
    HitPoly *hp, *nexth;
    Defense *defense;
    Car *car;
    Model *m;
    ColSize *sizep;
    ShotCase *casing, *next;

    for (casing = D_8039AE80.head; casing; casing = next) {
        next = casing->next;
        casing->time -= D_8002EB94;
        if (casing->time <= 0.0f) {
            sound_call_minimal(casing->mbIdx);
            func_800AFA84(&D_8039AE80, casing);
        } else {
            casing->vel[1] -= 2.5f * (D_80142764 * D_8002EB94);
            casing->mat.pos[0] += casing->vel[0] * D_8002EB94;
            casing->mat.pos[1] += casing->vel[1] * D_8002EB94;
            casing->mat.pos[2] += casing->vel[2] * D_8002EB94;
        }
    }
    for (i = 0, car = D_80152818; i < D_801543CA; i++, car++) {
        m = &D_8014A250[car->slot];
        defense = &car->battle.defense;
        if (defense->active)
            func_8038F938(car, m, defense, car->slot);
        switch (car->battle.type) {
        case 0: case 1: case 6: case 8:
            func_8038F568(car);
            break;
        }
        if (car->player->edges & car->player->controls[6])
            goto resetWeapon;
        if (!car->battle.ammoCnt) {
            if (car->battle.type == 5) {
                f32 z;
                sizep = &D_8011F844[m->body_type];
                m->bodyR[0][2] = m->bodyR[1][2] = sizep->colfront;
                z = m->bodyR[0][2] > sizep->colrear ? m->bodyR[0][2] : sizep->colrear;
                m->colrad = sqrtf(z*z + sizep->colside*sizep->colside + sizep->colheight*sizep->colheight);
            }
resetWeapon:
            car->battle.type = 8;
            car->battle.ammoCnt = -1;
            car->battle.pitch = 0.0f;
            car->battle.yaw = 0.0f;
        }
        if (car->battle.reloadingTime > 0.0f)
            car->battle.reloadingTime -= D_8002EB94;
        if (car->battle.reloadingTime <= 0.0f && car->battle.ammoCnt) {
            if (car->battle.type == 3) {
                if (car->battle.loaded)
                    continue;
                car->battle.loaded = 1;
            } else {
                if (!car->player || car->battle.type == 5 ||
                    (!(car->player->edges & car->player->controls[9]) &&
                     (car->battle.type != 1 || !(car->player->levels & car->player->controls[9]))) ||
                    m->crashflag || !car->collidable || car->dead)
                    continue;
                car->battle.shoot = 6;
            }
            shot = func_8008E3C0(&D_8039A520);
            if (!shot)
                continue;
            shot->slotDamaged = 0;
            shot->obj = 0;
            shot->obj2 = 0;
            shot->flags = 0x10;
            shot->type = car->battle.type;
            shot->slot = car->slot;
            shot->incTime = 0.0f;
            shot->vel[0] = 0.0f;
            shot->vel[1] = 0.0f;
            shot->vel[2] = 0.0f;
            shot->pos[0] = car->pos[0];
            shot->pos[1] = car->pos[1];
            shot->pos[2] = car->pos[2];
            shot->old[0] = car->pos[0];
            shot->old[1] = car->pos[1];
            shot->old[2] = car->pos[2];
            math_utility(&car->uvs[0][0], &shot->uvs[0][0]);
            if (shot->type != 3)
                func_800B61A8(sWeaponFx[car->battle.type], car->slot, 1, 1);
            StartStreak = 0;
            switch (shot->type) {
            case 2:
                StartStreak = 1;
                streaktype = 1;
                func_80090F44(0.5f, &shot->uvs[0][0]);
                car->battle.reloadingTime = 0.1666667f;
                shot->lifeTime = 3.1666667f;
                shot->radius = 15.0f;
                shot->vel[0] = car->vel[0] + 75.0f*shot->uvs[2][0];
                shot->vel[1] = car->vel[1] + 75.0f*shot->uvs[2][1];
                shot->vel[2] = car->vel[2] + 75.0f*shot->uvs[2][2];
                break;
            case 4:
                StartStreak = 1;
                streaktype = 0;
                car->battle.reloadingTime = 1.0f;
                shot->lifeTime = 15.0f;
                shot->radius = 2.0f;
                shot->vel[2] = 165.0f + (car->vel[0]*shot->uvs[2][0] + car->vel[1]*shot->uvs[2][1] + car->vel[2]*shot->uvs[2][2]);
                break;
            case 6:
                StartStreak = 1;
                streaktype = 2;
                car->battle.reloadingTime = 0.1666667f;
                shot->lifeTime = 10.0f;
                shot->radius = 1.0f;
                func_80090F44(car->battle.pitch, &shot->uvs[0][0]);
                shot->vel[0] = car->vel[0] + 50.0f*shot->uvs[2][0];
                shot->vel[1] = car->vel[1] + 50.0f*shot->uvs[2][1];
                shot->vel[2] = car->vel[2] + 50.0f*shot->uvs[2][2];
                break;
            case 8:
                car->battle.reloadingTime = 0.1666667f;
                shot->lifeTime = 4.0f;
                shot->radius = 0.5f;
                func_80090E9C(-car->battle.yaw, &shot->uvs[0][0]);
                func_80090F44(car->battle.pitch, &shot->uvs[0][0]);
                break;
            case 0:
                car->battle.reloadingTime = 0.1666667f;
                shot->lifeTime = 4.0f;
                shot->radius = 0.5f;
                func_80090F44(car->battle.pitch, &shot->uvs[0][0]);
                break;
            case 1:
                car->battle.reloadingTime = 0.0666667f;
                shot->lifeTime = 4.0f;
                shot->radius = 0.25f;
                func_80090E9C(func_8008B2E4(0.025f) - 0.0125f - car->battle.yaw, &shot->uvs[0][0]);
                func_80090F44(car->battle.pitch + (func_8008B2E4(0.025f) - 0.0125f), &shot->uvs[0][0]);
                h = func_8008E3C0(&D_8039AE80);
                if (h) {
                    f32 x = D_803943A4[1][m->body_type][0] + CaseOffsets[0][0];
                    f32 y = D_803943A4[1][m->body_type][1] + CaseOffsets[0][1];
                    f32 z = D_803943A4[1][m->body_type][2] + CaseOffsets[0][2];
                    h->mat.pos[0] = shot->pos[0] + car->uvs[2][0]*z;
                    h->mat.pos[1] = shot->pos[1] + car->uvs[2][1]*z;
                    h->mat.pos[2] = shot->pos[2] + car->uvs[2][2]*z;
                    h->mat.pos[0] += car->uvs[1][0]*y;
                    h->mat.pos[1] += car->uvs[1][1]*y;
                    h->mat.pos[2] += car->uvs[1][2]*y;
                    h->mat.pos[0] += car->uvs[0][0]*x;
                    h->mat.pos[1] += car->uvs[0][1]*x;
                    h->mat.pos[2] += car->uvs[0][2]*x;
                    math_utility(&car->uvs[0][0], &h->mat.uvs[0][0]);
                    func_8009EA68(0.3f + func_8008B2E4(0.23f), &h->mat.uvs[0][0]);
                    h->mbIdx = sign_extend_call(D_80399B18[0], &h->mat, -1, 0);
                    h->time = 0.5f + func_8008B2E4(0.3f);
                    h->vel[0] = car->vel[0] + 28.0f*h->mat.uvs[1][0];
                    h->vel[1] = car->vel[1] + 28.0f*h->mat.uvs[1][1];
                    h->vel[2] = car->vel[2] + 28.0f*h->mat.uvs[1][2];
                    h->vel[2] *= 0.88f;
                    h->vel[1] *= 0.88f;
                    h->vel[0] *= 0.88f;
                }
                break;
            case 7:
                car->battle.reloadingTime = 1.0f;
                shot->lifeTime = 1.0f;
                shot->radius = 2.0f;
                break;
            }
            {
                f32 x, y, z;
                if (car->battle.type >= 8) {
                    x = ShotOffsets[car->battle.type][0];
                    y = ShotOffsets[car->battle.type][1];
                    z = ShotOffsets[car->battle.type][2];
                } else {
                    x = ShotOffsets[car->battle.type][0] + D_803943A4[car->battle.type][m->body_type][0];
                    y = ShotOffsets[car->battle.type][1] + D_803943A4[car->battle.type][m->body_type][1];
                    z = ShotOffsets[car->battle.type][2] + D_803943A4[car->battle.type][m->body_type][2];
                }
                shot->pos[0] += car->uvs[2][0]*z;
                shot->pos[1] += car->uvs[2][1]*z;
                shot->pos[2] += car->uvs[2][2]*z;
                shot->pos[0] += car->uvs[1][0]*y;
                shot->pos[1] += car->uvs[1][1]*y;
                shot->pos[2] += car->uvs[1][2]*y;
                shot->pos[0] += car->uvs[0][0]*x;
                shot->pos[1] += car->uvs[0][1]*x;
                shot->pos[2] += car->uvs[0][2]*x;
            }
            if (StartStreak)
                shot->streak = func_8038D054(streaktype, shot->pos, shot->pos);
            else
                shot->streak = 0;
            if (car->battle.ammoCnt > 0 && car->battle.type != 5 && car->battle.type != 3)
                car->battle.ammoCnt--;
        }
    }
    for (hp = D_8039A5F8.head; hp; hp = nexth) {
        nexth = hp->next;
        func_8008D0C0(hp->poly);
        func_800AFA84(&D_8039A5F8, hp);
    }
    func_8038E114();
    func_8038CB20();
}
