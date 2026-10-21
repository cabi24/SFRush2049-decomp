/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * w36s: Resurrect cluster (GC resurrect.c), N64 typed port of the MAT3 GameCube decomp.
 *   func_800D348C        = FindHumanResPt   (MAT3 resurrect.c, GC 0x800xxxxx; private IPA: m/outPath/outIdx in s0/s2/s3)
 *   func_800D3B20        = FindDroneResPt   (static in GC; inlined into Resurrect, deleted-procedure stub)
 *   stunt_combo_display  = Resurrect        (MAT3 resurrect.c)
 * Field names from MAT3 rush_types.h; offsets checked against the N64 displacements.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct tTire {          /* anon_tradius_springK_5c */
    float tradius;              /* 0x00 */
    u8 pad04[0x48 - 0x04];
    float angvel;               /* 0x48 */
    u8 pad4C[0x5C - 0x4C];
} tTire;

typedef struct tResurrect {     /* anon_pos_save_pos_70 */
    float pos[3];               /* 0x00 */
    float save_pos[3];          /* 0x0c */
    float uvs[3][3];            /* 0x18 */
    float quat_start[4];        /* 0x3c */
    float quat_end[4];          /* 0x4c */
    int velocity;               /* 0x5c */
    float crash_time;           /* 0x60 */
    s16 moving_state;           /* 0x64 */
    float resurrect_time;       /* 0x68 */
    s8 autoAbort;               /* 0x6c */
    s8 waterAbort;              /* 0x6d */
} tResurrect;

typedef struct tInitIn {        /* anon_polepos_pos_40 */
    s16 polepos;                /* 0x00 */
    float pos[3];               /* 0x04 */
    float uvs[3][3];            /* 0x10 */
    float offset[3];            /* 0x34 */
} tInitIn;

typedef struct tFastIn {        /* anon_modelIteration_modelTime_10 */
    int modelIteration;         /* 0x0 */
    float modelTime;            /* 0x4 */
    float dt;                   /* 0x8 */
    s16 modelrun;               /* 0xc */
} tFastIn;

typedef struct tReckon {        /* anon_base_rwa_base_rwv_90 */
    u8 pad00[0x60];
    float rwr[3];               /* 0x60 */
    float uvs[3][3];            /* 0x6c */
} tReckon;

typedef struct tModelData {
    u8 pad000[0x40];
    float v[3];                 /* 0x040 */
    u8 pad04C[0x94 - 0x4C];
    float tireV[4][3];          /* 0x094 */
    float bodyForce[4][3];      /* 0x0c4 */
    u8 pad0F4[0x220 - 0xF4];
    float rwv[3];               /* 0x220 */
    float rwr[3];               /* 0x22c */
    u8 pad238[0x2EC - 0x238];
    float uvs[3][3];            /* 0x2ec */
    u8 pad310[0x430 - 0x310];
    tTire tires[4];             /* 0x430 */
    u8 pad5A0[0x5CC - 0x5A0];
    float suscomp[4];           /* 0x5cc */
    u8 pad5DC[0x62C - 0x5DC];
    u16 sound_flags[4];         /* 0x62c */
    u8 pad634[0x640 - 0x634];
    s8 crashflag;               /* 0x640 */
    s8 topscrape;               /* 0x641 */
    s8 thumpflag;               /* 0x642 */
    s8 mthumpflag;              /* 0x643 */
    u8 pad644[0x660 - 0x644];
    tResurrect resurrect;       /* 0x660 */
    tInitIn initin;             /* 0x6d0 */
    tFastIn fastin;             /* 0x710 */
    u8 pad720[0x734 - 0x720];
    tReckon reckon;             /* 0x734 */
    s16 obResPt;                /* 0x7c4 */
    s16 slot;                   /* 0x7c6 */
    u8 pad7C8[0x7D4 - 0x7C8];
    u32 appearance;             /* 0x7d4 */
    u8 pad7D8[0x7DF - 0x7D8];
    s8 hide_car;                /* 0x7df */
    s16 please_abort_me;        /* 0x7e0 */
    s16 last_cp_index;          /* 0x7e2 */
    s16 next_cp_index;          /* 0x7e4 */
    s16 drone_target;           /* 0x7e6 */
    s8 laps;                    /* 0x7e8 */
    s8 cpSign;                  /* 0x7e9 */
    s8 collidable;              /* 0x7ea */
    s8 solid;                   /* 0x7eb */
    u8 pad7EC[0x808 - 0x7EC];
} tModelData;

typedef struct tPathPoint {
    u8 pad00[0x24];
    s16 idx;                    /* 0x24 */
    s16 newIdx;                 /* 0x26 */
    s16 path;                   /* 0x28 */
    s16 basePath;               /* 0x2a */
} tPathPoint;

typedef struct tCarData {       /* N64 0x3B8 */
    u8 pad000[0xED];
    s8 restart;                 /* 0x0ed */
    u8 pad0EE[0x308 - 0xEE];
    s8 collidable;              /* 0x308 */
    s8 solid;                   /* 0x309 */
    float collide_time;         /* 0x30c */
    s8 collide_count;           /* 0x310 */
    s8 collide_state;           /* 0x311 */
    tPathPoint mpctl;           /* 0x314 */
    u8 pad340[0x344 - 0x340];
    u16 sound_flags[4];         /* 0x344 */
    u8 pad34C[0x3B8 - 0x34C];
} tCarData;

typedef struct tLink {          /* GC gLink (0x0A); N64 0x08 */
    u8 track, car, PaintJob, b3, b4, b5, flags;
    u8 owner;                   /* 0x07 */
} tLink;

typedef struct tDronePt {       /* anon_pos_mph_8 */
    s16 pos[3];
    u8 mph;
    s8 hints;
} tDronePt;

typedef struct tDronePath {     /* anon_numPts_pts_8 */
    u16 numPts;
    tDronePt *pts;
} tDronePath;

typedef struct tAltPath {       /* anon_resurrect_firstPath_10 */
    u8 resurrect;               /* 0x0 */
    s8 firstPath;               /* 0x1 */
    u16 firstIdx;               /* 0x2 */
    s8 lastPath;                /* 0x4 */
    u16 lastIdx;                /* 0x6 */
    s8 startSection;            /* 0x8 */
    u16 numPts;                 /* 0xa */
    s16 (*pts)[3];              /* 0xc */
} tAltPath;

typedef struct tResPath {       /* anon_numPts_pts_10 */
    u16 numPts;                 /* 0x0 */
    s16 (*pts)[3];              /* 0x4 */
    u8 numAlt;                  /* 0x8 */
    tAltPath *alt;              /* 0xc */
} tResPath;

typedef struct tCheckpoint {    /* anon_pos_normal_50 */
    float pos[3];               /* 0x00 */
    float normal[3];            /* 0x0c */
    int radius;                 /* 0x18 */
    s16 flags;                  /* 0x1c */
    s16 time[2];                /* 0x1e */
    s16 pathPts[20];            /* 0x22 */
    float distToNext;           /* 0x4c */
} tCheckpoint;

typedef struct tCheckpoints {   /* anon_startTime_loopPt_32c */
    s16 startTime;              /* 0x0 */
    s16 loopPt;                 /* 0x2 */
    s16 finishPt;               /* 0x4 */
    s16 startPt;                /* 0x6 */
    s16 numPoints;              /* 0x8 */
    tCheckpoint points[10];     /* 0xc */
} tCheckpoints;

typedef struct surftyp {
    u16 flags;
} surftyp;

#define kOneRace 0
#define kPractice 1
#define kGhost 2
#define kCircuit 3
#define kStunt 4
#define kObstacle 5
#define kBattle 6
#define LINK_OWNER_HUMAN 6
#define GS_DEMO 0x8
#define APPEAR_HULK 0x10
#define RES_IDLE (-1)
#define RES_START 0
#define MPH_TO_FTPS 1.4666667f

extern tCarData D_80152818[];       /* gGameCar */
extern tModelData D_8014A250[];     /* model */
extern float D_8011418C[3][3];      /* unitmatrix */
extern s8 D_80142760;               /* gDeathMode */
extern int D_8014A110;              /* gRace */
extern tDronePath D_8012E5E8[];     /* gDronePaths */
extern tCheckpoints D_80151CE8;     /* gCheckpoints */
extern tResPath D_801407F0;         /* gResPath */
extern tLink D_80153E88[];          /* gLink */
extern u32 D_801174B4;              /* gGameState */
extern s8 D_801407B0;               /* gResurrectInPlace */
extern s16 D_8014A108;              /* gNumPlayers */
extern s16 D_80152734;              /* gNumLaps */
extern float D_80154390;            /* resurrectTime */
extern float D_80117360[4];         /* res_last_time */
extern float D_801210E8[12][3];     /* pole_pos_offset */

#define gGameCar D_80152818
#define model D_8014A250
#define unitmatrix D_8011418C
#define gDeathMode D_80142760
#define gRace D_8014A110
#define gDronePaths D_8012E5E8
#define gCheckpoints D_80151CE8
#define gResPath D_801407F0
#define gLink D_80153E88
#define gGameState D_801174B4
#define gResurrectInPlace D_801407B0
#define gNumPlayers D_8014A108
#define gNumLaps D_80152734
#define resurrectTime D_80154390
#define res_last_time D_80117360
#define pole_pos_offset D_801210E8

extern float sinf(float);
extern float cosf(float);
extern float fabsf(float);
#pragma intrinsic(fabsf)

void math_utility(float (*src)[3], float (*dst)[3]);                          /* CopyUV */
void func_800D11BC(tModelData *m);                                            /* syminit */
void func_800C4180(float (*mat)[3], float *yaw);                              /* ExtractYaw */
surftyp *camera_trigger_check(float *ipos, float *opos, float (*uvs)[3]);     /* RoadPosition */
void func_800C40E8(float sint, float cost, float (*uv)[3]);                   /* fyaw */
void func_800D14F4(float (*uvs)[3], float *q);                                /* MakeQuatFromUvs */
void func_8009E820(float *src, float *dst, float (*uvs)[3]);                  /* WorldVector */
void func_800C4C9C(tModelData *m, s16 idx);                                   /* SetNewPathIndex */
s16 func_800D2C10(s16 *pos, s16 path);                                        /* FindClosestPathPt */
int difficulty_select(int res, int inPath, int inIdx, int cp);                /* NumPtsToCheckpoint */
void lap_count_select(int res, int inPath, int inIdx, int numPts, int *outPath, int *outIdx); /* TracePathPtForward */
int viDeadlinePassed(void);                                                   /* CountdownTimeOut */
void race_countdown_display(tModelData *m, float time);                       /* PassedCP */
void camera_follow_path(int res, int path, int idx, float (*uvs)[3]);         /* FindSurfaceUvsFromResPoint */
int func_800D3430(int inPath, int inIdx, int *outPath, int *outIdx, int honorAltRes); /* FindResPt */
void func_8038CA24(int slot);
f32 func_8008B2E4(f32 max);                                                   /* Random */

void func_800D348C(tModelData *m, int *outPath, int *outIdx);
void func_800D3B20(tModelData *m, int *outPath, int *outIdx);

/* Respawn point for a human car (MAT3 FindHumanResPt). */
void func_800D348C(tModelData *m, int *outPath, int *outIdx)
{
    int a;
    int p;
    int path = -1;
    int idx = 0;
    float min = 1.0e20f;
    float x, y, z;
    float dx, dy, dz, dist;
    if (gRace == kBattle) {
        float min2;
        int bestfit;
        int pOffset;
        int pReal;
        func_8038CA24(m->slot);
        min2 = 0.0f;
        bestfit = 0;
        pOffset = func_8008B2E4(gResPath.numPts);
        for (p = 0; p < gResPath.numPts; p++) {
            min = 1.0e20f;
            pReal = pOffset + p;
            if (pReal >= gResPath.numPts) pReal -= gResPath.numPts;
            for (a = 0; a < gNumPlayers; a++) {
                if (m->slot == a) continue;
                if (model[a].resurrect.moving_state >= 0) {
                    x = model[a].resurrect.pos[0];
                    z = model[a].resurrect.pos[2];
                } else {
                    x = model[a].rwr[0];
                    z = model[a].rwr[2];
                }
                dx = gResPath.pts[pReal][0] - x;
                dz = gResPath.pts[pReal][2] - z;
                dist = dx * dx + dz * dz;
                if (dist < min) min = dist;
            }
            if (min > min2) {
                bestfit = pReal;
                min2 = min;
            }
        }
        idx = bestfit;
    } else if (gRace == kObstacle) {
        idx = m->obResPt;
    } else {
        x = m->resurrect.save_pos[0];
        y = m->resurrect.save_pos[1];
        z = m->resurrect.save_pos[2];
        for (p = 0; p < gResPath.numPts; p++) {
            dx = gResPath.pts[p][0] - x;
            dy = gResPath.pts[p][1] - y;
            dz = gResPath.pts[p][2] - z;
            dist = dx * dx + dy * dy + dz * dz;
            if (dist < min) { min = dist; idx = p; }
        }
        if (gRace != kPractice && gRace != kStunt && gRace != kObstacle) {
            if (min > 1600.0f) {
                for (a = 0; a < gResPath.numAlt; a++) {
                    for (p = 0; p < gResPath.alt[a].numPts; p++) {
                        dx = gResPath.alt[a].pts[p][0] - x;
                        dy = gResPath.alt[a].pts[p][1] - y;
                        dz = gResPath.alt[a].pts[p][2] - z;
                        dist = dx * dx + dy * dy + dz * dz;
                        if (dist < min) { min = dist; idx = p; path = a; }
                    }
                }
            }
            if (path >= 0 && gResPath.alt[path].resurrect == 0)
                func_800D3430(path, idx, &path, &idx, 1);
            if (!(path >= 0 && gResPath.alt[path].resurrect == 2)) {
                if (path >= 0) {
                    a = path + 5;
                    if (gCheckpoints.points[m->last_cp_index].pathPts[a] >= 0) {
                        if (gCheckpoints.points[m->next_cp_index].pathPts[a] >= 0) {
                            if (idx < gCheckpoints.points[m->last_cp_index].pathPts[a] ||
                                idx > gCheckpoints.points[m->next_cp_index].pathPts[a]) {
                                min = 1.0e20f;
                                for (p = gCheckpoints.points[m->last_cp_index].pathPts[a];
                                     p <= gCheckpoints.points[m->next_cp_index].pathPts[a]; p++) {
                                    dx = gResPath.alt[path].pts[p][0] - x;
                                    dy = gResPath.alt[path].pts[p][1] - y;
                                    dz = gResPath.alt[path].pts[p][2] - z;
                                    dist = dx * dx + dy * dy + dz * dz;
                                    if (dist < min) { min = dist; idx = p; }
                                }
                            }
                        } else if (idx < gCheckpoints.points[m->last_cp_index].pathPts[a]) {
                            idx = gCheckpoints.points[m->last_cp_index].pathPts[a];
                        }
                    } else if (gCheckpoints.points[m->next_cp_index].pathPts[a] < 0 &&
                               m->last_cp_index != gResPath.alt[path].startSection) {
                        path = -1;
                        idx = -1;
                    }
                }
                if (path < 0) {
                    a = m->next_cp_index < m->last_cp_index ? gResPath.numPts :
                        gCheckpoints.points[m->next_cp_index].pathPts[0];
                    if (idx < gCheckpoints.points[m->last_cp_index].pathPts[0] || idx >= a) {
                        min = 1.0e20f;
                        for (p = gCheckpoints.points[m->last_cp_index].pathPts[0]; p < a; p++) {
                            dx = gResPath.pts[p][0] - x;
                            dy = gResPath.pts[p][1] - y;
                            dz = gResPath.pts[p][2] - z;
                            dist = dx * dx + dy * dy + dz * dz;
                            if (dist < min) { min = dist; idx = p; }
                        }
                    }
                }
            }
        }
    }
    *outPath = path;
    *outIdx = idx;
}

/* Respawn point for an AI car: nearest point of its own racing line between its checkpoints
 * (MAT3 FindDroneResPt, static: inlined into Resurrect, deleted stub at 0x800D3B20). */
void func_800D3B20(tModelData *m, int *outPath, int *outIdx)
{
    int pathIndex = gGameCar[m->slot].mpctl.path;
    int p, idx, end;
    float min, x, y, z, dx, dy, dz, dist;
    tDronePath *path;
    path = &gDronePaths[pathIndex];
    min = 1.0e20f;
    x = m->resurrect.save_pos[0];
    y = m->resurrect.save_pos[1];
    z = m->resurrect.save_pos[2];
    idx = 0;
    end = m->next_cp_index < m->last_cp_index ? path->numPts : gCheckpoints.points[m->next_cp_index].pathPts[pathIndex + 1];
    for (p = gCheckpoints.points[m->last_cp_index].pathPts[pathIndex + 1]; p < end; p++) {
        dx = path->pts[p].pos[0] - x;
        dy = path->pts[p].pos[1] - y;
        dz = path->pts[p].pos[2] - z;
        dist = dx * dx + dy * dy + dz * dz;
        if (dist < min) { min = dist; idx = p; }
    }
    *outPath = pathIndex;
    *outIdx = idx;
    func_800C4C9C(m, idx);
}

/* Start a car's respawn (or, at race start, place it on the grid) (MAT3 Resurrect). */
void stunt_combo_display(tModelData *m)
{
    int inPlace;
    s16 node = m->slot;
    int i;
    int startPath, endPath, startIdx, endIdx;
    int jumpPts, numPts;
    float time_scale, crash_time;
    tCarData *gc = &gGameCar[node];
    int human;
    float vel[3];
    if (gDeathMode && m->resurrect.crash_time != 0.0f && !m->resurrect.autoAbort) return;
    m->resurrect.autoAbort = 0;
    do {} while (0);
    human = gLink[node].owner == LINK_OWNER_HUMAN;
    if (human && m->fastin.modelTime < res_last_time[m->slot]) res_last_time[m->slot] = 0;
    inPlace = 0;
    if (human && !m->resurrect.waterAbort && gResurrectInPlace &&
        2.0f + res_last_time[m->slot] < m->fastin.modelTime && !gDeathMode) {
        inPlace = 1;
        res_last_time[m->slot] = m->fastin.modelTime;
    }
    m->resurrect.waterAbort = 0;
    if (m->resurrect.crash_time == 0.0f) {
        m->initin.pos[0] = m->resurrect.save_pos[0];
        m->initin.pos[1] = m->resurrect.save_pos[1];
        m->initin.pos[2] = m->resurrect.save_pos[2];
        math_utility(m->resurrect.uvs, m->initin.uvs);
        if (gRace == kBattle || gRace == kStunt || gRace == kObstacle || (!human && m->initin.polepos >= 6)) {
            m->initin.offset[0] = 0.0f;
            m->initin.offset[1] = 0.0f;
            m->initin.offset[2] = 0.0f;
        } else {
            m->initin.offset[0] = pole_pos_offset[m->initin.polepos][0];
            m->initin.offset[1] = pole_pos_offset[m->initin.polepos][1];
            m->initin.offset[2] = pole_pos_offset[m->initin.polepos][2];
        }
        m->resurrect.moving_state = RES_IDLE;
        func_800D11BC(m);
        if (gGameState & GS_DEMO) {
            m->resurrect.velocity = (MPH_TO_FTPS * gDronePaths[gc->mpctl.path].pts[gCheckpoints.points[m->last_cp_index].pathPts[gc->mpctl.path + 1]].mph) / 2;
            m->v[2] = m->resurrect.velocity;
            for (i = 0; i < 4; i++) {
                m->tireV[i][2] = m->resurrect.velocity;
                m->tires[i].angvel = m->tireV[i][2] / m->tires[i].tradius;
            }
        }
        model[node].fastin.modelrun = 1;
        return;
    }
    if (inPlace) {
        m->resurrect.pos[0] = m->rwr[0];
        m->resurrect.pos[1] = m->rwr[1];
        m->resurrect.pos[2] = m->rwr[2];
        m->resurrect.velocity = 0;
        {
            /* FindSurfaceUvsInPlace(m->resurrect.pos, m->reckon.uvs, m->resurrect.uvs) */
            float sine, cosine, inYaw, outYaw;
            float snapPos[3];
            surftyp *surf;
            int res;
            func_800C4180(m->reckon.uvs, &inYaw);
            inYaw = -inYaw;
            surf = camera_trigger_check(m->resurrect.pos, snapPos, m->resurrect.uvs);
            if (surf) {
                if (surf->flags & 0x2000) {
                    res = 1;
                    goto done;
                }
                func_800C4180(m->resurrect.uvs, &outYaw);
                sine = sinf(inYaw + outYaw);
                cosine = cosf(inYaw + outYaw);
            } else {
                sine = sinf(inYaw);
                cosine = cosf(inYaw);
                math_utility(unitmatrix, m->resurrect.uvs);
            }
            func_800C40E8(-sine, cosine, m->resurrect.uvs);
            res = 0;
done:
            if (!res) {
                m->resurrect.pos[1] += 10.0f;
                m->appearance &= ~APPEAR_HULK;
                m->crashflag = 0;
                goto finish;
            }
        }
    }
    time_scale = gc->solid ? 1.1f : 0.8f;
    gc->collidable = m->collidable = 0;
    gc->solid = m->solid = 0;
    gc->collide_time = m->fastin.modelTime;
    gc->collide_count = gc->collide_state = -1;
    m->hide_car = 1;
    if (human) {
        func_800D348C(m, &startPath, &startIdx);
        if (startPath >= 0 && gResPath.alt[startPath].resurrect == 2) {
            m->resurrect.pos[0] = gResPath.alt[startPath].pts[startIdx][0];
            m->resurrect.pos[1] = 3.0f + gResPath.alt[startPath].pts[startIdx][1];
            m->resurrect.pos[2] = gResPath.alt[startPath].pts[startIdx][2];
            m->resurrect.velocity = 0;
            camera_follow_path(1, startPath, startIdx, m->resurrect.uvs);
            m->appearance &= ~APPEAR_HULK;
            m->crashflag = 0;
            goto finish;
        }
    } else func_800D3B20(m, &startPath, &startIdx);
    m->resurrect.resurrect_time = m->fastin.modelTime;
    if (gRace == kObstacle || gRace == kOneRace || gRace == kPractice || gRace == kCircuit || gRace == kGhost)
        resurrectTime = 0.01f;
    else resurrectTime = 1.5f;
    crash_time = resurrectTime + (m->resurrect.resurrect_time - m->resurrect.crash_time);
    if (gRace == kPractice || gRace == kBattle || gRace == kStunt || gRace == kObstacle || gResurrectInPlace) {
        m->resurrect.velocity = 0;
    } else if (m->resurrect.velocity < 0) {
        m->resurrect.velocity = 1;
    } else if (human) {
        i = func_800D2C10(startPath >= 0 ? gResPath.alt[startPath].pts[startIdx] : gResPath.pts[startIdx], 0);
        m->resurrect.velocity = MPH_TO_FTPS * gDronePaths[0].pts[i].mph;
    } else {
        m->resurrect.velocity = MPH_TO_FTPS * gDronePaths[startPath].pts[startIdx].mph;
    }
    if (m->resurrect.velocity > 146.66667f) m->resurrect.velocity = 146.66667f;
    jumpPts = crash_time * (m->resurrect.velocity * time_scale) / 20.0f;
    if (m->next_cp_index == gCheckpoints.finishPt && m->laps == gNumLaps - 1 &&
        gRace != kPractice && gRace != kStunt && gRace != kBattle && gRace != kObstacle) {
        numPts = difficulty_select(human, startPath, startIdx, m->next_cp_index) / 2;
        if (jumpPts > numPts) jumpPts = numPts;
    }
    lap_count_select(human, startPath, startIdx, jumpPts, &endPath, &endIdx);
    if (endPath >= 0) {
        if (human) {
            m->resurrect.pos[0] = gResPath.alt[endPath].pts[endIdx][0];
            m->resurrect.pos[1] = 3.0f + gResPath.alt[endPath].pts[endIdx][1];
            m->resurrect.pos[2] = gResPath.alt[endPath].pts[endIdx][2];
            if (gRace != kPractice && gRace != kBattle && gRace != kStunt &&
                gCheckpoints.points[m->next_cp_index].pathPts[endPath + 5] >= 0 &&
                gCheckpoints.points[m->next_cp_index].pathPts[endPath + 5] <= endIdx && !viDeadlinePassed())
                race_countdown_display(m, m->fastin.modelTime);
        } else {
            m->resurrect.pos[0] = gDronePaths[endPath].pts[endIdx].pos[0];
            m->resurrect.pos[1] = 3.0f + gDronePaths[endPath].pts[endIdx].pos[1];
            m->resurrect.pos[2] = gDronePaths[endPath].pts[endIdx].pos[2];
            if (((m->next_cp_index < m->last_cp_index && gCheckpoints.points[m->last_cp_index].pathPts[endPath + 1] > endIdx) ||
                 (m->next_cp_index > m->last_cp_index && gCheckpoints.points[m->next_cp_index].pathPts[endPath + 1] <= endIdx)) &&
                !viDeadlinePassed()) race_countdown_display(m, m->fastin.modelTime);
        }
    } else {
        m->resurrect.pos[0] = gResPath.pts[endIdx][0];
        m->resurrect.pos[1] = 3.0f + gResPath.pts[endIdx][1];
        m->resurrect.pos[2] = gResPath.pts[endIdx][2];
        if (gRace != kPractice && gRace != kBattle && gRace != kStunt &&
            ((m->next_cp_index < m->last_cp_index && gCheckpoints.points[m->last_cp_index].pathPts[0] > endIdx) ||
             (m->next_cp_index > m->last_cp_index && gCheckpoints.points[m->next_cp_index].pathPts[0] <= endIdx)) &&
            !viDeadlinePassed()) race_countdown_display(m, m->fastin.modelTime);
    }
    if (gRace != kPractice && gRace != kBattle && gRace != kStunt && gRace != kObstacle)
        m->resurrect.velocity = 58;
    camera_follow_path(human, endPath, endIdx, m->resurrect.uvs);
    if (human) {
        /* N64: humans respawn at the multiplayer grid offsets (pole_pos_offset rows 6-11) */
        if (gRace == kGhost) {
            m->initin.offset[0] = pole_pos_offset[6][0];
            m->initin.offset[1] = pole_pos_offset[6][1];
            m->initin.offset[2] = pole_pos_offset[6][2];
        } else {
            m->initin.offset[0] = pole_pos_offset[m->slot + 6][0];
            m->initin.offset[1] = pole_pos_offset[m->slot + 6][1];
            m->initin.offset[2] = pole_pos_offset[m->slot + 6][2];
        }
        if (gRace != kBattle && gRace != kStunt && gRace != kObstacle && !gResurrectInPlace) {
            func_8009E820(m->initin.offset, vel, m->resurrect.uvs);
            m->resurrect.pos[0] += vel[0];
            m->resurrect.pos[1] += vel[1];
            m->resurrect.pos[2] += vel[2];
        } else {
            m->initin.offset[0] = 0.0f;
            m->initin.offset[1] = 0.0f;
            m->initin.offset[2] = 0.0f;
        }
    }
finish:
    m->initin.offset[0] = 0.0f;
    m->initin.offset[1] = 0.0f;
    m->initin.offset[2] = 0.0f;
    func_800D14F4(m->uvs, m->resurrect.quat_start);
    func_800D14F4(m->resurrect.uvs, m->resurrect.quat_end);
    {
        /* FindBestQuat(m->resurrect.quat_start, m->resurrect.quat_end) */
        float *q1 = m->resurrect.quat_start;
        float *q2 = m->resurrect.quat_end;
        s16 j;
        float dq1;
        float dq2;
        dq1 = dq2 = 0.0f;
        for (j = 0; j < 4; j++) {
            dq1 += fabsf(q1[j] - q2[j]);
            dq2 += fabsf(q1[j] + q2[j]);
        }
        if (dq2 < dq1) {
            for (j = 0; j < 4; j++) q2[j] = -q2[j];
        }
    }
    m->resurrect.moving_state = RES_START;
    math_utility(m->reckon.uvs, m->initin.uvs);
    m->initin.pos[0] = m->reckon.rwr[0];
    m->initin.pos[1] = m->reckon.rwr[1];
    m->initin.pos[2] = m->reckon.rwr[2];
    vel[0] = m->rwv[0];
    vel[1] = m->rwv[1];
    vel[2] = m->rwv[2];
    func_800D11BC(m);
    for (i = 0; i < 4; i++) {
        m->suscomp[i] = 0.0f;
        m->bodyForce[i][0] = 0.0f;
        gc->sound_flags[i] = m->sound_flags[i] = 0;
    }
    m->topscrape = 0;
    m->mthumpflag = m->thumpflag = 0;
    m->rwv[0] = vel[0];
    m->rwv[1] = vel[1];
    m->rwv[2] = vel[2];
    gc->restart = 1;
}
