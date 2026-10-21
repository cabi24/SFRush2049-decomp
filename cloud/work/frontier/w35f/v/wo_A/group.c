/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w35f draft: image B weapons.c cluster (MAT3 weapons.c AnimateWeapon / _RemoveGattMFlash / _RemoveCannMFlash). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef union RGBA { u8 c[4]; u32 rgba; } RGBA;
typedef struct UVMat { f32 uvs[3][3]; f32 pos[3]; } UVMat;

typedef struct Defense {
    u32 active; f32 invsTime; f32 shldTime; f32 shldScale; f32 unknown10;
} Defense;

typedef struct Battle {
    s8 type; s8 ammoCnt; s16 health; s16 damageThisFrame; u8 pad06[2];
    Defense defense;      /* 0x08 */
    s8 shoot;             /* 0x1c */
    u8 fadeAlpha;         /* 0x1d */
    s8 fadeState;         /* 0x1e */
    s8 Points;            /* 0x1f */
    s32 loaded;           /* 0x20 */
    f32 fadeTime; f32 reloadingTime; f32 pitch; f32 yaw;
} Battle;

typedef struct Car {
    u8 pad00[0x34c];
    RGBA IRclr;           /* 0x34c */
    u8 pad350[0x35c - 0x350];
    s8 camera;            /* 0x35c */
    s8 view;              /* 0x35d */
    u8 pad35e[0x384 - 0x35e];
    Battle battle;        /* 0x384 */
} Car;

typedef struct Model {
    u8 pad00[8];
    u8 body_type;         /* 0x08 */
    u8 pad09[0x640 - 9];
    s8 crashflag;         /* 0x640 */
    u8 pad641[0x808 - 0x641];
} Model;

typedef struct Visual {
    struct Visual *next;  /* 0x00 */
    s16 index;            /* 0x04 */
    s16 objnum;           /* 0x06 */
    s16 slot;             /* 0x08 */
    u32 data;             /* 0x0c */
    f32 timeStamp;        /* 0x10 */
    void (*func)(struct Visual *, s16); /* 0x14 */
} Visual;

typedef struct CarParts {
    s32 frame;            /* 0x00 */
    u8 pad04[0x14];
    s32 weap;             /* 0x18 */
    u8 pad1c[0x24];
} CarParts;

typedef struct MuzzleMat { UVMat mat; } MuzzleMat;
typedef struct MuzzleObj {
    s32 mdls[5];          /* 0x00 */
    MuzzleMat mats[5];    /* 0x14 */
    s32 on;               /* 0x104 */
    f32 onTime;           /* 0x108 */
} MuzzleObj;
typedef struct CMuzzleObj {
    s32 mdls[5];          /* 0x00 */
    MuzzleMat mats[5];    /* 0x14 */
    s32 ripdl;            /* 0x104 */
    UVMat mat;            /* 0x108 */
    s8 ripCnt;            /* 0x138 */
    s8 ripTIdx;           /* 0x139 */
    s16 on;               /* 0x13a */
    f32 ripScale;         /* 0x13c */
    f32 ripTime;          /* 0x140 */
    f32 onTime;           /* 0x144 */
} CMuzzleObj;

typedef struct TexDef { u8 b[36]; } TexDef;
typedef struct TexDefSet { TexDef *texDefs; s32 num; } TexDefSet;

extern volatile f32 D_8002EB94;          /* gFrameSeconds */
extern Car D_80152818[];                 /* gGameCar, stride 0x3b8 */
extern Model D_8014A250[];               /* model */
extern CarParts D_80139320[];            /* gCarParts */
extern u16 D_8014254C[];                 /* gObjList */
extern s32 D_80399B40[7];                /* gWfxObjList */
extern u16 D_80399B08[5];                /* gRipTextures */
extern TexDefSet D_80151AE8[];           /* gTexDefPtrs */

extern void sound_call_minimal(s16 obj);          /* ZOID_RemoveObject */
extern void func_8008E06C(s16 obj, RGBA *clr);    /* ZOID_SetColor */
extern void model_data_load(s32 obj, s32 a, s32 mask);       /* MBOX_HideObject */
extern void model_transform_setup(s32 obj, s32 a, s32 mask); /* MBOX_ShowObject */
extern UVMat *func_8008B3A0(s16 idx);             /* ZOID_FindMatrix */
extern void func_80090770(s16 obj, s32 def);      /* ZOID_SetObjectDef */
extern void func_8008D6FC(s16 obj, f32 *pos, s32 a);       /* ZOID_UpdateObject */
extern s32 sign_extend_call(s32 obj, void *mat, s32 parent, u32 flags); /* MBOX_NewObject */
extern void math_utility(f32 *src, f32 *dst);     /* CopyUV */
extern void func_8009EA68(f32 a, f32 *uvs);       /* RollUV */
extern void func_8008B32C(f32 *src, f32 *dst, f32 s);      /* ScaleUV */
extern f32 func_8008B2E4(f32 scale);              /* Random */
extern void func_8008D870(s16 obj, void *tex, s32 a);      /* ZOID_SetTexture */

f32 D_803943A4[8][13][3] = {
    0.0f, 2.5f, 0.3f, 0.0f, 3.0f, 0.3f, 0.0f, 2.8f, 0.3f, 0.0f, 3.0f, 0.1f, 0.0f, 3.0f, 0.1f, 0.0f,
    3.4f, -0.4f, 0.0f, 3.1f, 0.3f, 0.0f, 3.3f, 1.0f, 0.0f, 3.39f, -1.82f, 0.0f, 3.05f, 0.65f, 0.0f,
    5.04f, 1.33f, 0.0f, 2.95f, -0.9f, 0.0f, 2.79f, 0.52f, 0.0f, 2.7f, 2.1f, 0.0f, 3.1f, 1.1f, 0.0f,
    2.7f, 0.1f, 0.0f, 2.9f, -0.1f, 0.0f, 2.9f, 0.4f, 0.0f, 3.5f, 0.1f, 0.0f, 3.0f, 0.3f, 0.0f,
    3.3f, 1.2f, 0.0f, 3.35f, -0.69f, 0.0f, 3.04f, 0.97f, 0.0f, 5.05f, 2.02f, 0.0f, 2.98f, -0.31f,
    0.0f, 2.79f, 0.92f, 0.0f, 2.5f, 1.3f, 0.0f, 3.0f, -0.5f, 0.0f, 2.6f, -0.6f, 0.0f, 2.7f, -1.1f,
    0.0f, 2.7f, -1.1f, 0.0f, 3.3f, -1.6f, 0.0f, 2.9f, -0.6f, 0.0f, 3.0f, 0.1f, 0.0f, 3.13f, -1.73f,
    0.0f, 2.89f, -0.41f, 0.0f, 4.84f, 0.37f, 0.0f, 2.75f, -1.67f, 0.0f, 2.66f, -0.35f, 0.0f, 1.9f,
    -5.3f, 0.0f, 1.7f, -5.7f, 0.0f, 1.9f, -5.6f, 0.0f, 2.0f, -5.9f, 0.0f, 1.4f, -6.1f, 0.0f, 1.4f,
    -7.0f, 0.0f, 1.9f, -5.7f, 0.0f, 1.8f, -6.0f, 0.0f, 1.3f, -5.71f, 0.0f, 1.6f, -6.73f, 0.0f,
    1.71f, -9.28f, 0.0f, 1.74f, -6.95f, 0.0f, 1.28f, -5.54f, 0.0f, 2.2f, 0.6f, 0.0f, 2.6f, -0.7f,
    0.0f, 2.4f, -0.8f, 0.0f, 2.4f, -1.1f, 0.0f, 2.6f, -0.5f, 0.0f, 3.0f, -1.7f, 0.0f, 2.6f, -0.5f,
    0.0f, 2.7f, -0.5f, 0.0f, 3.16f, -2.52f, 0.0f, 2.67f, -0.39f, 0.0f, 4.8f, 0.44f, 0.0f, 2.52f,
    -2.15f, 0.0f, 2.37f, -0.44f, 0.0f, 0.0f, 6.4f, 0.0f, 0.0f, 6.7f, 0.0f, 0.0f, 6.0f, 0.0f, 0.0f,
    6.0f, 0.0f, 0.2f, 6.4f, 0.0f, 0.2f, 6.8f, 0.0f, 0.1f, 5.9f, 0.0f, 0.2f, 5.8f, 0.0f, 0.25f,
    4.86f, 0.0f, 0.44f, 7.38f, 0.0f, -0.18f, 8.07f, 0.0f, 0.14f, 5.89f, 0.0f, 0.14f, 5.89f, 0.0f,
    2.9f, 0.3f, 0.0f, 3.3f, -0.7f, 0.0f, 3.1f, -0.9f, 0.0f, 3.2f, -0.9f, 0.0f, 3.2f, -0.9f, 0.0f,
    3.6f, -1.4f, 0.0f, 3.3f, -0.7f, 0.0f, 3.4f, -0.3f, 0.0f, 3.65f, -2.06f, 0.0f, 3.17f, -0.44f,
    0.0f, 5.28f, 0.66f, 0.0f, 3.15f, -1.52f, 0.0f, 3.08f, -0.6f, 0.0f, 2.6f, 0.4f, 0.0f, 3.0f,
    -0.8f, 0.0f, 2.7f, -0.8f, 0.0f, 3.1f, -0.4f, 0.0f, 3.1f, -0.2f, 0.0f, 3.4f, -1.8f, 0.0f, 3.1f,
    -0.9f, 0.0f, 3.1f, -0.9f, 0.0f, 3.52f, -2.0f, 0.0f, 3.4f, -0.33f, 0.0f, 4.38f, -4.39f, 0.0f,
    3.25f, -2.05f, 0.0f, 3.12f, -1.11f
};
static s8 D_80399118[4];            /* gCurWeaponType */
static MuzzleObj D_80399120[4];     /* gGattMuzzleObjs */
static CMuzzleObj D_80399550[4];    /* gCannMuzzleObjs */

/* 0x8038AA14 _RemoveGattMFlash */
void func_8038AA14(s16 slot)
{
    s32 i;
    for (i = 0; i < 5; i++) {
        if (D_80399120[slot].mdls[i] != -1) {
            sound_call_minimal(D_80399120[slot].mdls[i]);
            D_80399120[slot].mdls[i] = -1;
        }
    }
}

/* 0x8038A95C _RemoveCannMFlash */
void func_8038A95C(s16 slot)
{
    s32 i;
    for (i = 0; i < 5; i++) {
        if (D_80399550[slot].mdls[i] != -1) {
            sound_call_minimal(D_80399550[slot].mdls[i]);
            D_80399550[slot].mdls[i] = -1;
        }
    }
    if (D_80399550[slot].ripdl != -1) {
        sound_call_minimal(D_80399550[slot].ripdl);
        D_80399550[slot].ripdl = -1;
        D_80399550[slot].ripTIdx = -1;
    }
}

/* 0x8038AA8C AnimateWeapon */
void func_8038AA8C(Visual *v, s16 op)
{
    Car *car = &D_80152818[v->slot];
    s32 def;
    s32 i;
    s32 r1;
    f32 r2;
    RGBA clr;
    RGBA clr2 = { { 0, 255, 0, 255 } };
    UVMat *mat;
    f32 off[3];
    s32 pad1;
    s32 pad2;

    if (!op || D_8014A250[v->slot].crashflag) {
        if (D_80399118[v->slot] == 1)
            func_8038AA14(v->slot);
        else if (D_80399118[v->slot] == 0)
            func_8038A95C(v->slot);
        car->battle.type = 8;
        car->battle.ammoCnt = -1;
        model_data_load(v->objnum, 0, 15);
        return;
    }
    clr.rgba = car->IRclr.rgba;
    if (car->battle.defense.active & 1) {
        clr.c[3] = car->battle.fadeAlpha;
        if (clr.c[3] < 48)
            clr.c[3] = 48;
    }
    func_8008E06C(D_80139320[v->slot].weap, &clr);
    mat = func_8008B3A0(D_80139320[v->slot].frame);
    if (car->battle.type == 8) {
        model_data_load(v->objnum, 1, 15);
    } else if (car->camera >= 0 && car->view <= 1) {
        model_transform_setup(v->objnum, 0, 15);
        model_data_load(v->objnum, 1, 1 << car->camera);
    } else {
        model_transform_setup(v->objnum, 0, 15);
    }
    if (D_80399118[v->slot] != car->battle.type) {
        off[0] = D_803943A4[car->battle.type][D_8014A250[v->slot].body_type][0];
        off[1] = D_803943A4[car->battle.type][D_8014A250[v->slot].body_type][1];
        off[2] = D_803943A4[car->battle.type][D_8014A250[v->slot].body_type][2];
        if (D_80399118[v->slot] == 1)
            func_8038AA14(v->slot);
        else if (D_80399118[v->slot] == 0)
            func_8038A95C(v->slot);
        D_80399118[v->slot] = car->battle.type;
        switch (car->battle.type) {
        case 8:
            return;
        case 0:
            def = 216;
            off[0] += 0.16f;
            off[1] += 0.48f;
            off[2] += 2.57f;
            for (i = 0; i < 5; i++) {
                math_utility(&mat->uvs[0][0], &D_80399550[v->slot].mats[i].mat.uvs[0][0]);
                D_80399550[v->slot].mats[i].mat.pos[0] = off[0] * mat->uvs[0][0] + mat->pos[0];
                D_80399550[v->slot].mats[i].mat.pos[1] = off[0] * mat->uvs[0][1] + mat->pos[1];
                D_80399550[v->slot].mats[i].mat.pos[2] = off[0] * mat->uvs[0][2] + mat->pos[2];
                D_80399550[v->slot].mats[i].mat.pos[0] = off[1] * mat->uvs[1][0] + D_80399550[v->slot].mats[i].mat.pos[0];
                D_80399550[v->slot].mats[i].mat.pos[1] = off[1] * mat->uvs[1][1] + D_80399550[v->slot].mats[i].mat.pos[1];
                D_80399550[v->slot].mats[i].mat.pos[2] = off[1] * mat->uvs[1][2] + D_80399550[v->slot].mats[i].mat.pos[2];
                D_80399550[v->slot].mats[i].mat.pos[0] = off[2] * mat->uvs[2][0] + D_80399550[v->slot].mats[i].mat.pos[0];
                D_80399550[v->slot].mats[i].mat.pos[1] = off[2] * mat->uvs[2][1] + D_80399550[v->slot].mats[i].mat.pos[1];
                D_80399550[v->slot].mats[i].mat.pos[2] = off[2] * mat->uvs[2][2] + D_80399550[v->slot].mats[i].mat.pos[2];
                D_80399550[v->slot].mdls[i] = sign_extend_call(D_80399B40[i], &D_80399550[v->slot].mats[i].mat, -1, 0x42000);
                func_8008E06C(D_80399550[v->slot].mdls[i], &clr2);
                model_data_load(D_80399550[v->slot].mdls[i], 1, 15);
            }
            math_utility(&mat->uvs[0][0], &D_80399550[v->slot].mat.uvs[0][0]);
            D_80399550[v->slot].mat.pos[0] = off[0] * mat->uvs[0][0] + mat->pos[0];
            D_80399550[v->slot].mat.pos[1] = off[0] * mat->uvs[0][1] + mat->pos[1];
            D_80399550[v->slot].mat.pos[2] = off[0] * mat->uvs[0][2] + mat->pos[2];
            D_80399550[v->slot].mat.pos[0] = off[1] * mat->uvs[1][0] + D_80399550[v->slot].mat.pos[0];
            D_80399550[v->slot].mat.pos[1] = off[1] * mat->uvs[1][1] + D_80399550[v->slot].mat.pos[1];
            D_80399550[v->slot].mat.pos[2] = off[1] * mat->uvs[1][2] + D_80399550[v->slot].mat.pos[2];
            D_80399550[v->slot].mat.pos[0] = off[2] * mat->uvs[2][0] + D_80399550[v->slot].mat.pos[0];
            D_80399550[v->slot].mat.pos[1] = off[2] * mat->uvs[2][1] + D_80399550[v->slot].mat.pos[1];
            D_80399550[v->slot].mat.pos[2] = off[2] * mat->uvs[2][2] + D_80399550[v->slot].mat.pos[2];
            D_80399550[v->slot].ripdl = sign_extend_call(D_80399B40[6], &D_80399550[v->slot].mat, -1, 0x42000);
            func_8008E06C(D_80399550[v->slot].ripdl, &clr2);
            model_data_load(D_80399550[v->slot].ripdl, 1, 15);
            D_80399550[v->slot].ripCnt = -1;
            D_80399550[v->slot].ripTime = 0.0f;
            D_80399550[v->slot].ripScale = 1.5f + func_8008B2E4(1.5f);
            break;
        case 1:
            def = 217;
            off[0] += 0.01f;
            off[1] += 0.65f;
            off[2] += 3.59f;
            for (i = 0; i < 5; i++) {
                math_utility(&mat->uvs[0][0], &D_80399120[v->slot].mats[i].mat.uvs[0][0]);
                D_80399120[v->slot].mats[i].mat.pos[0] = off[0] * mat->uvs[0][0] + mat->pos[0];
                D_80399120[v->slot].mats[i].mat.pos[1] = off[0] * mat->uvs[0][1] + mat->pos[1];
                D_80399120[v->slot].mats[i].mat.pos[2] = off[0] * mat->uvs[0][2] + mat->pos[2];
                D_80399120[v->slot].mats[i].mat.pos[0] = off[1] * mat->uvs[1][0] + D_80399120[v->slot].mats[i].mat.pos[0];
                D_80399120[v->slot].mats[i].mat.pos[1] = off[1] * mat->uvs[1][1] + D_80399120[v->slot].mats[i].mat.pos[1];
                D_80399120[v->slot].mats[i].mat.pos[2] = off[1] * mat->uvs[1][2] + D_80399120[v->slot].mats[i].mat.pos[2];
                D_80399120[v->slot].mats[i].mat.pos[0] = off[2] * mat->uvs[2][0] + D_80399120[v->slot].mats[i].mat.pos[0];
                D_80399120[v->slot].mats[i].mat.pos[1] = off[2] * mat->uvs[2][1] + D_80399120[v->slot].mats[i].mat.pos[1];
                D_80399120[v->slot].mats[i].mat.pos[2] = off[2] * mat->uvs[2][2] + D_80399120[v->slot].mats[i].mat.pos[2];
                clr2.c[0] = 255;
                clr2.c[1] = 255;
                clr2.c[2] = 43;
                D_80399120[v->slot].mdls[i] = sign_extend_call(D_80399B40[i], &D_80399120[v->slot].mats[i].mat, -1, 0x42000);
                func_8008E06C(D_80399120[v->slot].mdls[i], &clr2);
                model_data_load(D_80399120[v->slot].mdls[i], 1, 15);
            }
            break;
        case 2: def = 218; break;
        case 3: def = 219; break;
        case 4: def = 220; break;
        case 5: def = 221; break;
        case 6: def = 222; break;
        case 7: def = 223; break;
        }
        func_80090770(D_80139320[v->slot].weap, D_8014254C[def + 314]);
        func_8008D6FC(D_80139320[v->slot].weap, D_803943A4[car->battle.type][D_8014A250[v->slot].body_type], 0);
    }
    {
        off[0] = D_803943A4[car->battle.type][D_8014A250[v->slot].body_type][0];
        off[1] = D_803943A4[car->battle.type][D_8014A250[v->slot].body_type][1];
        off[2] = D_803943A4[car->battle.type][D_8014A250[v->slot].body_type][2];
        if (D_80399118[v->slot] == 1) {
            off[0] += 0.01f;
            off[1] += 0.65f;
            off[2] += 3.59f;
            for (i = 0; i < 5; i++) {
                math_utility(&mat->uvs[0][0], &D_80399120[v->slot].mats[i].mat.uvs[0][0]);
                D_80399120[v->slot].mats[i].mat.pos[0] = off[0] * mat->uvs[0][0] + mat->pos[0];
                D_80399120[v->slot].mats[i].mat.pos[1] = off[0] * mat->uvs[0][1] + mat->pos[1];
                D_80399120[v->slot].mats[i].mat.pos[2] = off[0] * mat->uvs[0][2] + mat->pos[2];
                D_80399120[v->slot].mats[i].mat.pos[0] = off[1] * mat->uvs[1][0] + D_80399120[v->slot].mats[i].mat.pos[0];
                D_80399120[v->slot].mats[i].mat.pos[1] = off[1] * mat->uvs[1][1] + D_80399120[v->slot].mats[i].mat.pos[1];
                D_80399120[v->slot].mats[i].mat.pos[2] = off[1] * mat->uvs[1][2] + D_80399120[v->slot].mats[i].mat.pos[2];
                D_80399120[v->slot].mats[i].mat.pos[0] = off[2] * mat->uvs[2][0] + D_80399120[v->slot].mats[i].mat.pos[0];
                D_80399120[v->slot].mats[i].mat.pos[1] = off[2] * mat->uvs[2][1] + D_80399120[v->slot].mats[i].mat.pos[1];
                D_80399120[v->slot].mats[i].mat.pos[2] = off[2] * mat->uvs[2][2] + D_80399120[v->slot].mats[i].mat.pos[2];
            }
            if (D_80399120[v->slot].on) {
                D_80399120[v->slot].onTime += D_8002EB94;
                if (D_80399120[v->slot].onTime >= 0.0333333f) {
                    D_80399120[v->slot].on = 0;
                    for (i = 0; i < 5; i++)
                        model_data_load(D_80399120[v->slot].mdls[i], 1, 15);
                }
            }
            if (car->battle.shoot >= 5) {
                D_80399120[v->slot].on = 1;
                D_80399120[v->slot].onTime = 0.0f;
                model_transform_setup(D_80399120[v->slot].mdls[0], 0, 15);
                if (car->camera >= 0 && car->view <= 1)
                    model_data_load(D_80399120[v->slot].mdls[0], 1, 1 << car->camera);
                r1 = (s32)func_8008B2E4(4.0f) + 1;
                r2 = func_8008B2E4(0.5f);
                func_8009EA68(r2, &D_80399120[v->slot].mats[r1].mat.uvs[0][0]);
                func_8008B32C(&D_80399120[v->slot].mats[r1].mat.uvs[0][0], &D_80399120[v->slot].mats[r1].mat.uvs[0][0], 0.85f + func_8008B2E4(0.75f));
                model_transform_setup(D_80399120[v->slot].mdls[r1], 0, 15);
                if (car->camera >= 0 && car->view <= 1)
                    model_data_load(D_80399120[v->slot].mdls[r1], 1, 1 << car->camera);
                r1++;
                if (r1 >= 5)
                    r1 = 1;
                func_8009EA68(r2, &D_80399120[v->slot].mats[r1].mat.uvs[0][0]);
                func_8008B32C(&D_80399120[v->slot].mats[r1].mat.uvs[0][0], &D_80399120[v->slot].mats[r1].mat.uvs[0][0], 0.85f + func_8008B2E4(0.75f));
                model_transform_setup(D_80399120[v->slot].mdls[r1], 0, 15);
                if (car->camera >= 0 && car->view <= 1)
                    model_data_load(D_80399120[v->slot].mdls[r1], 1, 1 << car->camera);
            }
        }
        if (D_80399118[v->slot] == 0) {
            off[0] += 0.16f;
            off[1] += 0.48f;
            off[2] += 2.57f;
            for (i = 0; i < 5; i++) {
                math_utility(&mat->uvs[0][0], &D_80399550[v->slot].mats[i].mat.uvs[0][0]);
                D_80399550[v->slot].mats[i].mat.pos[0] = off[0] * mat->uvs[0][0] + mat->pos[0];
                D_80399550[v->slot].mats[i].mat.pos[1] = off[0] * mat->uvs[0][1] + mat->pos[1];
                D_80399550[v->slot].mats[i].mat.pos[2] = off[0] * mat->uvs[0][2] + mat->pos[2];
                D_80399550[v->slot].mats[i].mat.pos[0] = off[1] * mat->uvs[1][0] + D_80399550[v->slot].mats[i].mat.pos[0];
                D_80399550[v->slot].mats[i].mat.pos[1] = off[1] * mat->uvs[1][1] + D_80399550[v->slot].mats[i].mat.pos[1];
                D_80399550[v->slot].mats[i].mat.pos[2] = off[1] * mat->uvs[1][2] + D_80399550[v->slot].mats[i].mat.pos[2];
                D_80399550[v->slot].mats[i].mat.pos[0] = off[2] * mat->uvs[2][0] + D_80399550[v->slot].mats[i].mat.pos[0];
                D_80399550[v->slot].mats[i].mat.pos[1] = off[2] * mat->uvs[2][1] + D_80399550[v->slot].mats[i].mat.pos[1];
                D_80399550[v->slot].mats[i].mat.pos[2] = off[2] * mat->uvs[2][2] + D_80399550[v->slot].mats[i].mat.pos[2];
            }
            math_utility(&mat->uvs[0][0], &D_80399550[v->slot].mat.uvs[0][0]);
            D_80399550[v->slot].mat.pos[0] = off[0] * mat->uvs[0][0] + mat->pos[0];
            D_80399550[v->slot].mat.pos[1] = off[0] * mat->uvs[0][1] + mat->pos[1];
            D_80399550[v->slot].mat.pos[2] = off[0] * mat->uvs[0][2] + mat->pos[2];
            D_80399550[v->slot].mat.pos[0] = off[1] * mat->uvs[1][0] + D_80399550[v->slot].mat.pos[0];
            D_80399550[v->slot].mat.pos[1] = off[1] * mat->uvs[1][1] + D_80399550[v->slot].mat.pos[1];
            D_80399550[v->slot].mat.pos[2] = off[1] * mat->uvs[1][2] + D_80399550[v->slot].mat.pos[2];
            D_80399550[v->slot].mat.pos[0] = off[2] * mat->uvs[2][0] + D_80399550[v->slot].mat.pos[0];
            D_80399550[v->slot].mat.pos[1] = off[2] * mat->uvs[2][1] + D_80399550[v->slot].mat.pos[1];
            D_80399550[v->slot].mat.pos[2] = off[2] * mat->uvs[2][2] + D_80399550[v->slot].mat.pos[2];
            if (D_80399550[v->slot].ripCnt != -1) {
                D_80399550[v->slot].ripTime -= D_8002EB94;
                if (D_80399550[v->slot].ripTime <= 0.0f) {
                    D_80399550[v->slot].ripTime = 0.0333333f;
                    D_80399550[v->slot].ripCnt++;
                    if (D_80399550[v->slot].ripCnt >= 5) {
                        model_data_load(D_80399550[v->slot].ripdl, 1, 15);
                        D_80399550[v->slot].ripCnt = -1;
                    } else {
                        D_80399550[v->slot].ripTIdx++;
                        if (D_80399550[v->slot].ripTIdx >= 5)
                            D_80399550[v->slot].ripTIdx = 0;
                        func_8008D870(D_80399550[v->slot].ripdl, &D_80151AE8[D_80399B08[D_80399550[v->slot].ripTIdx] >> 10].texDefs[D_80399B08[D_80399550[v->slot].ripTIdx] & 1023], -1);
                    }
                }
            }
            if (D_80399550[v->slot].on) {
                D_80399550[v->slot].onTime += D_8002EB94;
                if (D_80399550[v->slot].onTime >= 0.0666667f) {
                    D_80399550[v->slot].on = 0;
                    for (i = 0; i < 5; i++)
                        model_data_load(D_80399550[v->slot].mdls[i], 1, 15);
                }
            }
            if (car->battle.shoot >= 5) {
                D_80399550[v->slot].ripScale = 1.5f + func_8008B2E4(1.5f);
                D_80399550[v->slot].ripTime = 0.0333333f;
                D_80399550[v->slot].ripCnt = 0;
                D_80399550[v->slot].ripTIdx = (s32)func_8008B2E4(5.0f);
                func_8008D870(D_80399550[v->slot].ripdl, &D_80151AE8[D_80399B08[0] >> 10].texDefs[D_80399B08[0] & 1023], -1);
                model_transform_setup(D_80399550[v->slot].ripdl, 0, 15);
                if (car->camera >= 0 && car->view <= 1)
                    model_data_load(D_80399550[v->slot].ripdl, 1, 1 << car->camera);
                D_80399550[v->slot].on = 1;
                D_80399550[v->slot].onTime = 0.0f;
                model_transform_setup(D_80399550[v->slot].mdls[0], 0, 15);
                if (car->camera >= 0 && car->view <= 1)
                    model_data_load(D_80399550[v->slot].mdls[0], 1, 1 << car->camera);
                for (i = 1; i < 5; i++) {
                    func_8009EA68(func_8008B2E4(1.25f) - 0.5f, &D_80399550[v->slot].mats[i].mat.uvs[0][0]);
                    func_8008B32C(&D_80399550[v->slot].mats[i].mat.uvs[0][0], &D_80399550[v->slot].mats[i].mat.uvs[0][0], 0.4f + func_8008B2E4(0.25f));
                    model_transform_setup(D_80399550[v->slot].mdls[i], 0, 15);
                    if (car->camera >= 0 && car->view <= 1)
                        model_data_load(D_80399550[v->slot].mdls[i], 1, 1 << car->camera);
                }
            }
            func_8008B32C(&D_80399550[v->slot].mat.uvs[0][0], &D_80399550[v->slot].mat.uvs[0][0], D_80399550[v->slot].ripScale);
        }
    }
}
