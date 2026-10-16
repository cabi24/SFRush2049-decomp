/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * audio_effect_apply @ 0x800BAAA0, 696 bytes (174 words) -- EQUAL in the whole-program unit with its internal
 * callee physics_response (w17h, provisional until now) as fellow member of group frontier_read_path_data.
 * Identity: GameCube ReadPathData (pathcommon.c, 0x8003220C; void ReadPathData(void); locals s16 i, p, j,
 * char *ptr), proposed rename (not applied). Not audio: it parses the track's path block at gPathWave+0x32C
 * (D_80153F20): the resurrection-path header gResPath (D_801407F0, 16 B), its alternate ranges gAltPaths
 * (D_80153E80, 16-B records), the point count gNumResPathPts (D_801527A4) and points gResPathPtList
 * (D_801409E8, s16[3]), then four drone paths gDronePaths (D_8012E5E8, 8-B {u16 num; pts}) with
 * gNumPathPts (D_801527D0) and gPathPtList (D_80153EF8); then SyncCheckpointsToPaths(-1) (audio_mixer_main),
 * SyncAltPaths (physics_friction_apply), CalcResPathBounds (func_800B9740), CalcCheckpointTimes
 * (physics_response). On gRestartGame (D_80114650) only the five calls run.
 * The GC build byte-swaps every field in its loops; N64 keeps the loops with empty bodies (big-endian data),
 * which is what retail compiles (the s16 counters still run). Ported from the GC body (w33c): GC statement
 * order, and the else arm as GC's chain `gResPath.alt = gAltPaths = 0;` -- with two separate stores the
 * gAltPaths value is not forwarded into the next loop (34 rows); the chain (or `gAltPaths = 0; alt = gAltPaths;`)
 * is EQUAL. N64 order of the three drone-path stores (count, list, pts) follows retail. No shaping devices.
 */
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef signed int s32;
typedef s16 Vertex[3];
typedef struct AltPath {
    u8 type;
    u8 unk01[9];
    u16 numPts;
    Vertex *pts;
} AltPath;
typedef struct ResPath {
    u16 numPrimary;
    u16 unk02;
    Vertex *pts;
    u8 numAlt;
    u8 unk09[3];
    AltPath *alt;
} ResPath;
typedef struct DronePath {
    u16 num;
    u16 unk02;
    void *pts;
} DronePath;
extern s8 D_80114650;
extern char *D_80153F20;
extern ResPath D_801407F0;
extern AltPath *D_80153E80;
extern u16 D_801527A4;
extern Vertex *D_801409E8;
extern DronePath D_8012E5E8[4];
extern s16 D_801527D0[4];
extern void *D_80153EF8[4];
extern void *memcpy(void *, const void *, unsigned int);
void physics_response(void);
void audio_mixer_main(s16 mode);
void physics_friction_apply(void);
void func_800B9740(void);

void audio_effect_apply(void)
{
    s16 i;
    s16 p;
    char *ptr;
    s16 j;

    if (D_80114650) {
        physics_response();
        audio_mixer_main(-1);
        physics_friction_apply();
        func_800B9740();
        physics_response();
        return;
    }
    ptr = D_80153F20 + 0x32C;
    memcpy(&D_801407F0, ptr, 0x10);
    ptr += 0x10;
    D_80153E80 = (AltPath *)ptr;
    ptr += D_801407F0.numAlt * 16;
    for (i = 0; i < D_801407F0.numAlt; i++) {
    }
    memcpy(&D_801527A4, ptr, 2);
    ptr += 2;
    D_801409E8 = (Vertex *)ptr;
    ptr += D_801407F0.numPrimary * 6;
    for (i = 0; i < D_801527A4; i++) {
        for (j = 0; j < 3; j++) {
        }
    }
    D_801407F0.pts = D_801409E8;
    if (D_801407F0.numAlt > 0) {
        D_801407F0.alt = D_80153E80;
    } else {
        D_801407F0.alt = D_80153E80 = 0;
    }
    for (i = 0; i < D_801407F0.numAlt; i++) {
        D_80153E80[i].pts = (Vertex *)ptr;
        ptr += D_80153E80[i].numPts * 6;
    }
    for (p = 0; p < 4; p++) {
        memcpy(&D_8012E5E8[p], ptr, 8);
        D_801527D0[p] = D_8012E5E8[p].num;
        ptr += 8;
        D_80153EF8[p] = ptr;
        D_8012E5E8[p].pts = ptr;
        ptr += D_8012E5E8[p].num * 8;
        for (i = 0; i < D_8012E5E8[p].num; i++) {
            for (j = 0; j < 3; j++) {
            }
        }
    }
    audio_mixer_main(-1);
    physics_friction_apply();
    func_800B9740();
    physics_response();
}
