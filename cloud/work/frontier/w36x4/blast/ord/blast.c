/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * visuals.c blast trio, MAT3 GameSource/src/visuals.c (w36x4, wave 36; builds on w36l groups/w36l_blast, w26c f308).
 *   func_80090308         = StartNewBlast (GC 0x80057998, static; internal here: slot read from the caller's 0(sp)
 *                           home, s0-s8 clobbered unsaved)
 *   entity_physics_update = AnimateBlast  (GC 0x800587C0, static; the tVisual callback StartBlast installs)
 *   save_write_data       = StartBlast    (GC 0x800585E4; StartNewBlastRing inlined)
 * All three MATCH in this group (score.py group) and are EQUAL in the whole-program unit.
 * N64 differences from MAT3: StartNewBlast's loop works through `tBlastObj *b = &gBlastObjs[slot].blastObjs[i]`
 * for every field and sets clr.c[3] from b->alpha after `b->alpha = 128` (MAT3: `clr.c[3] = 128` earlier); alphaInc
 * Random(10)+25 (MAT3 +17); glow alpha 95 (MAT3 56); the glow and StartSmoke (entity_spawn_init) need
 * active_player_count < 4 and (D_80156994 || gTrackNum >= 6). StartBlast runs StartNewBlast directly for tracks 0..5
 * without D_80156994; the ring needs active_player_count < 4; StartNewBlastRing tests `if (v) {...}` (MAT3: early
 * return); the if-arm copies the slot through an s16 local; sound ids 45/69/47 through a `snd` local.
 * Why these forms (traced, w36x4 RESULTS): a store through the pointer variable b kills the rand seed in uopt (no PRE
 * of the next Random's seed load) although uopt then folds b into the IV (s0-relative stores as in retail); the byte
 * load of `clr.c[3] = b->alpha` takes one ring temp, so the 4th Random reloads the seed into t6 (`sw t9,0(s1);
 * lw t6,0(s1)`); CopyUV through b decides the s2/s3 colouring tie. In StartBlast the ring's early return stopped uopt
 * from carrying MBOX_NewObject's -1/gObjList/0x50000 argument webs over `if (linkToCar)` (retail sets a0..a3 before
 * the branch and multiplies by 952 with shifts); the s16 local gives retail's `lh v0,0(t9)`.
 * Helpers: Random = func_8008B2E4 over libc rand (func_8008B2B4); ZOID_RemoveObject -> ZOID_RemoveObjectTree ->
 * _RemoveObjectTree (entity_spawn_callback) as in GC zoid.c; ZOID_SetColor = func_8008E06C; MBOX_NewObject wraps
 * ZOID_AddObject (func_8008E26C); GrabEnvEntry = func_80090284; AddToEnvList inline; ReleaseEnviron =
 * entity_transform_apply; ZOID_SetObjectDef = func_80090770 and EmitGameSound = camera_collision_check (both locked
 * singles, byte-identical context copies, inlined by umerge); AudEmitEffect = camera_target_track; newWorldDamage =
 * func_8038A408 (image B); StartSmoke = entity_spawn_init.
 * Unused locals (the exact frame residual, disclosed): StartNewBlast and the inlined StartNewBlastRing share the local
 * template `tVisual *v; int i; tUVMatrix mat; tRGBA clr;` with `mat` unused in both and `i` unused in the ring.
 *   StartNewBlast: retail frame 272, v at sp+268 (-4), clr at sp+212 (-60); without `mat` the frame is 224 and 11 words
 *   differ (12 slots).
 *   StartBlast: retail frame 352, ring v at sp+324 (-28), ring clr at sp+268 (-84). Without the ring's `mat` the frame
 *   is 304 (32 words differ); without the ring's `i`, 2 words differ (clr slot); the ring's `int u0, u1` after clr are
 *   2 more slots: without them the frame is 344 (34 words differ: named slots match top-relative, the spill homes at
 *   sp+144/148 bottom-relative, so the local area is 8 B short).
 */
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
int camera_collision_check(const float *pos, int dir, float fadeDistance, float fadeCompression, float maxVol, float minVol, int fxid, int subId, int replace, unsigned char priority); /* EmitGameSound (locked, kept) */
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
            b = &D_80154660[slot].blastObjs[i];
            if (b->mbIdx != -1) ZOID_RemoveObject(b->mbIdx);
            math_utility(D_8011418C, b->mat.uvs);
            b->mat.pos[0] = player_array[slot].dr_pos[0];
            b->mat.pos[1] = player_array[slot].dr_pos[1] - 3.0f;
            b->mat.pos[2] = player_array[slot].dr_pos[2];
            b->mbIdx = MBOX_NewObject(D_801427C0[158 + i], &b->mat, -1, 0x42000);
            b->scale = 0.4f;
            b->scaleInc = 0.05f + Random(0.005f);
            b->mat.uvs[0][0] = b->mat.uvs[1][1] = b->mat.uvs[2][2] = b->scale;
            b->yawInc = 0.1f - Random(0.2f);
            b->pitchInc = 0.1f - Random(0.2f);
            b->ht = 0.0f;
            b->yaw = 0.0f;
            b->pitch = 0.0f;
            b->alpha = 128;
            clr.c[3] = b->alpha;
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
    int u0, u1;
    v = func_80090284();
    if (v) {
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
        s16 slot;
        slot = *(s16 *)data;
        v->slot = slot;
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
        camera_collision_check(mat->pos, (int)D_8011418C[3], 400.0f, 0.0f, 1.0f, 0.0f, snd, v->objnum, 0, 128);
    }
}
