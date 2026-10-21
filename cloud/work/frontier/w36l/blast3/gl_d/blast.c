/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;

typedef union tRGBA { unsigned char c[4]; unsigned int rgba; } tRGBA;
typedef struct tUVMatrix { float uvs[3][3]; float pos[3]; } tUVMatrix;
typedef struct tBlastObj {
    int mbIdx; tUVMatrix mat; float scale; float scaleInc; float yaw; float yawInc; float pitch; float pitchInc;
    unsigned char alpha; unsigned char alphaInc; signed short unused; float ht;
} tBlastObj;
typedef struct tBlastStruct {
    tBlastObj blastObjs[4]; int glowIdx; tUVMatrix mat;
    unsigned char alpha; unsigned char alphaCnt; unsigned char ralpha; unsigned char ralphaCnt;
    float lifeTime; float incTime;
} tBlastStruct;
typedef struct tBlastRingStruct { int ringIdx; tUVMatrix mat; unsigned char ralpha; unsigned char ralphaCnt; signed short unused; float incTime; } tBlastRingStruct;
typedef struct tBlastMat { tUVMatrix *mat; } tBlastMat;
typedef struct tVisual {
    struct tVisual *next;       /* 0x00 */
    s16 index;                  /* 0x04 */
    s16 objnum;                 /* 0x06 */
    s16 slot;                   /* 0x08 */
    u8 pad0A[2];
    void *data;                 /* 0x0C */
    float timeStamp;            /* 0x10 */
    void (*func)(struct tVisual *, s16);   /* 0x14 */
} tVisual;
typedef struct tCarData { u8 pad0[8]; float dr_pos[3]; u8 pad14[952 - 0x14]; } tCarData;
typedef struct tModelData { u8 pad0[0x640]; s8 crashflag; u8 pad641[0x808 - 0x641]; } tModelData;
typedef struct ObjInstance { unsigned int flags; char pad4[4]; tUVMatrix *mat; float scale; float w; u16 index; char pad16[0x26]; unsigned int color; unsigned int envcolor; } ObjInstance;

extern s32 D_801170FC;                  /* gPauseState */
extern volatile f32 D_8002EB94;         /* gFrameSeconds */
extern f32 D_801543CC;                  /* gModelTime */
extern tBlastStruct D_80154660[];       /* gBlastObjs */
extern tBlastRingStruct D_80154FD8[];   /* gBlastRingObjs */
extern tBlastMat D_8013C238[];          /* gBlastMats */
extern s16 D_8013C094;                  /* gBlastMatCnt */
extern tCarData player_array[];         /* gGameCar */
extern tModelData D_8014A250[];         /* model */
extern ObjInstance D_8012E700[];        /* ObjInstanceList */
extern u16 D_801427C0[];                /* gObjList */
extern float D_8011418C[4][3];          /* unitmatrix */
extern tVisual *D_801391F0;             /* gEnvList */
extern s8 D_80156994;
extern s8 D_8014978C;                   /* gTrackNum */
extern s16 active_player_count;
extern s8 D_8010FFC0;
extern int D_8011735C;

tVisual *func_80090284(void);                                   /* GrabEnvEntry */
void entity_spawn_callback(s16 objnum, s32 removeChildren, s32 b);  /* _RemoveObjectTree */
void entity_transform_apply(tVisual *v, s32 unlink);            /* ReleaseEnviron */
void math_utility(float (*src)[3], float (*dst)[3]);            /* CopyUV */
s32 func_8008E26C(s32 objnum, tUVMatrix *mat, s16 parent, s32 flags);   /* MBOX_NewObjectCam? */
void entity_spawn_init(s16 slot, u32 type, s32 a, f32 *b);      /* StartSmoke */
void func_8038A408(f32 *pos, s16 slot);                         /* newWorldDamage */
void func_80090770(s16 objnum, u16 def);                        /* ZOID_SetObjectDef */
s32 camera_target_track(float *pos, float *vel, f32 a, f32 b, f32 c, f32 d, s32 snd, s32 objnum, s32 e, s32 f); /* EmitGameSound */
void entity_update_callback(tVisual *v, s16 op);                /* AnimateNewBlast */
void entity_physics_update(tVisual *v, s16 op);
void func_80090B68(tVisual *v, s16 op);                         /* AnimateNewBlastRing */

static int rand(void)
{
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}

static f32 Random(f32 max)
{
    f32 rannum;

    rannum = ((f32)(rand() & 0x07FFF) * max) / 32768.0f;
    return rannum;
}

static void ZOID_RemoveObjectTree(s16 objnum, int removeChildren)
{
    entity_spawn_callback(objnum, removeChildren, 0);
}

static void ZOID_RemoveObject(s16 objnum)
{
    ZOID_RemoveObjectTree(objnum, 0);
}

static void ZOID_SetColor(s16 objnum, tRGBA *col)
{
    D_8012E700[objnum].color = col->rgba;
}

static int MBOX_NewObject(int objnum, tUVMatrix *mat, int parent, unsigned int flags)
{
    return func_8008E26C(objnum, mat, parent, flags);
}

static void SetObjColor(int objnum, tRGBA *col)
{
    D_8012E700[objnum].color = col->rgba;
}

static void AddToEnvList(tVisual *v)
{
    v->next = D_801391F0;
    D_801391F0 = v;
}

/* StartNewBlast */
void func_80090308(s16 slot)
{
    tVisual *v;
    int i;
    tUVMatrix mat;
    tRGBA clr = { { 255, 255, 255, 255 } };
    v = func_80090284();
    if (v) {
        v->func = entity_update_callback;
        v->timeStamp = D_801543CC;
        v->slot = slot;
        clr.c[0] = 244;
        clr.c[1] = 205;
        clr.c[2] = 20;
        for (i = 0; i < 4; i++) {
            tBlastObj *b;
            b = (tBlastObj *)(u32)&D_80154660[slot].blastObjs[i];
            if (D_80154660[slot].blastObjs[i].mbIdx != -1) ZOID_RemoveObject(D_80154660[slot].blastObjs[i].mbIdx);
            math_utility(D_8011418C, D_80154660[slot].blastObjs[i].mat.uvs);
            D_80154660[slot].blastObjs[i].mat.pos[0] = player_array[slot].dr_pos[0];
            D_80154660[slot].blastObjs[i].mat.pos[1] = player_array[slot].dr_pos[1] - 3.0f;
            D_80154660[slot].blastObjs[i].mat.pos[2] = player_array[slot].dr_pos[2];
            D_80154660[slot].blastObjs[i].mbIdx = MBOX_NewObject(D_801427C0[158 + i], &D_80154660[slot].blastObjs[i].mat, -1, 0x42000);
            D_80154660[slot].blastObjs[i].scale = 0.4f;
            D_80154660[slot].blastObjs[i].scaleInc = 0.05f + Random(0.005f);
            D_80154660[slot].blastObjs[i].mat.uvs[0][0] = D_80154660[slot].blastObjs[i].mat.uvs[1][1] = D_80154660[slot].blastObjs[i].mat.uvs[2][2] = D_80154660[slot].blastObjs[i].scale;
            D_80154660[slot].blastObjs[i].yawInc = 0.1f - Random(0.2f);
            D_80154660[slot].blastObjs[i].pitchInc = 0.1f - Random(0.2f);
            clr.c[3] = 128;
            D_80154660[slot].blastObjs[i].ht = 0.0f;
            D_80154660[slot].blastObjs[i].yaw = 0.0f;
            D_80154660[slot].blastObjs[i].pitch = 0.0f;
            b->alpha = 128;
            b->alphaInc = (int)Random(10.0f) + 25;
            ZOID_SetColor(b->mbIdx, &clr);
        }
        if (active_player_count < 4 && (D_80156994 || D_8014978C >= 6)) {
            math_utility(D_8011418C, D_80154660[slot].mat.uvs);
            D_80154660[slot].mat.pos[0] = player_array[slot].dr_pos[0];
            D_80154660[slot].mat.pos[1] = player_array[slot].dr_pos[1];
            D_80154660[slot].mat.pos[2] = player_array[slot].dr_pos[2];
            if (D_80154660[slot].glowIdx != -1) ZOID_RemoveObject(D_80154660[slot].glowIdx);
            D_80154660[slot].glowIdx = MBOX_NewObject(D_801427C0[162], &D_80154660[slot].mat, -1, 0x52000);
            D_80154660[slot].alpha = 95;
            D_80154660[slot].alphaCnt = 0;
            D_8012E700[D_80154660[slot].glowIdx].scale = 0.2f;
            D_8012E700[D_80154660[slot].glowIdx].w = 1.4f;
            clr.c[0] = 255;
            clr.c[1] = 255;
            clr.c[2] = 255;
            clr.c[3] = D_80154660[slot].alpha;
            SetObjColor(D_80154660[slot].glowIdx, &clr);
        }
        D_80154660[slot].lifeTime = 0.7f;
        D_80154660[slot].incTime = 0.0333333f;
        AddToEnvList(v);
    }
    if (active_player_count < 4 && (D_80156994 || D_8014978C >= 6)) entity_spawn_init(slot, 5, 0, 0);
}

/* StartNewBlastRing (inlined into StartBlast) */
static void StartNewBlastRing(s16 slot)
{
    tVisual *v;
    int i;
    tUVMatrix mat;
    tRGBA clr = { { 255, 255, 255, 255 } };
    v = func_80090284();
    if (!v) return;
    v->func = func_80090B68;
    v->timeStamp = D_801543CC;
    v->slot = slot;
    math_utility(D_8011418C, D_80154FD8[slot].mat.uvs);
    D_80154FD8[slot].mat.pos[0] = player_array[slot].dr_pos[0];
    D_80154FD8[slot].mat.pos[1] = player_array[slot].dr_pos[1];
    D_80154FD8[slot].mat.pos[2] = player_array[slot].dr_pos[2];
    if (D_80154FD8[slot].ringIdx != -1) ZOID_RemoveObject(D_80154FD8[slot].ringIdx);
    D_80154FD8[slot].ringIdx = MBOX_NewObject(D_801427C0[163], &D_80154FD8[slot].mat, -1, 0x42000);
    D_80154FD8[slot].ralpha = 240;
    D_80154FD8[slot].ralphaCnt = 0;
    SetObjColor(D_80154FD8[slot].ringIdx, &clr);
    D_80154FD8[slot].incTime = 0.0333333f;
    AddToEnvList(v);
}

/* AnimateBlast */
void entity_physics_update(tVisual *v, s16 op)
{
    if (D_801170FC) return;
    if (!op) {
release:
        ZOID_RemoveObject(v->objnum);
        v->objnum = -1;
        entity_transform_apply(v, 1);
        return;
    }
    if (v->slot >= 0) {
        if (!D_8014A250[v->slot].crashflag) goto release;
        D_8012E700[v->objnum].mat->pos[0] = player_array[v->slot].dr_pos[0];
        D_8012E700[v->objnum].mat->pos[1] = player_array[v->slot].dr_pos[1];
        D_8012E700[v->objnum].mat->pos[2] = player_array[v->slot].dr_pos[2];
    }
    v->timeStamp -= D_8002EB94;
    if (v->slot < 0 || (v->slot >= 0 && v->timeStamp <= 0.0f)) {
        v->timeStamp = 0.0333333f;
        if (v->index == 30) goto release;
        func_80090770(v->objnum, D_801427C0[164 + v->index]);
        v->index++;
        if (v->slot >= 0) {
            if (active_player_count < 4 && (D_80156994 || D_8014978C >= 6) && v->index == 4) func_8038A408(player_array[v->slot].dr_pos, v->slot);
            if (v->index == 3) func_80090308(v->slot);
        }
    }
}

static void EmitGameSound(float *pos, float *vel, f32 a, f32 b, f32 c, f32 d, s32 snd, s32 objnum, s32 e, s32 f)
{
    if (!D_8010FFC0) return;
    if (snd == -1) return;
    camera_target_track(pos, vel, a, b, c, d, snd, objnum, e, f);
}

/* StartBlast */
void save_write_data(void *data, int linkToCar, float scale, int playSound)
{
    tVisual *v;
    int snd;
    tUVMatrix *mat;
    if (!D_80156994 && D_8014978C >= 0 && D_8014978C < 6) {
        func_80090308(*(s16 *)data);
        return;
    }
    if (linkToCar && active_player_count < 4) StartNewBlastRing(*(s16 *)data);
    v = func_80090284();
    if (!v) return;
    mat = D_8013C238[D_8013C094].mat;
    math_utility(D_8011418C, mat->uvs);
    v->func = entity_physics_update;
    v->timeStamp = 0.0333333f;
    if (linkToCar) {
        v->slot = *(s16 *)data;
        mat->pos[0] = player_array[v->slot].dr_pos[0];
        mat->pos[1] = player_array[v->slot].dr_pos[1];
        mat->pos[2] = player_array[v->slot].dr_pos[2];
    } else {
        v->slot = -1;
        mat->pos[0] = ((float *)data)[0];
        mat->pos[1] = ((float *)data)[1];
        mat->pos[2] = ((float *)data)[2];
    }
    v->index = 0;
    v->objnum = MBOX_NewObject(D_801427C0[164], mat, -1, 0x50000);
    AddToEnvList(v);
    D_8012E700[v->objnum].scale = scale;
    if (++D_8013C094 >= 50) D_8013C094 = 0;
    if (playSound) {
        snd = scale >= 1.0f ? 45 : scale >= 0.5f ? 69 : 47;
        EmitGameSound(mat->pos, D_8011418C[3], 400.0f, 0.0f, 1.0f, 0.0f, snd, v->objnum, 0, 128);
    }
}
