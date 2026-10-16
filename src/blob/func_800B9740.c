/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800B9740 @ 0x800B9740, 408 bytes (102 words) -- MATCH (score.py fn standalone; unit EQUAL).
 * Identity: GameCube CalcResPathBounds (pathcommon.c, 0x800331B4; void CalcResPathBounds(void), locals
 * int i, j, validPt), proposed rename (not applied). Computes the bounding box of the resurrection path
 * points: gPathMin = D_801407D4, gPathMax = D_801407B4 (s16[3]), gResPath = D_801407F0, gResPathPtList =
 * D_801409E8 (s16[3] points), gNumResPathPts = D_801527A4. A point counts when it is one of the first
 * numPrimary points, or lies inside an alternate range of type 1 (pointer range test, no early break).
 * Port of the GC body (w33c): s16 extrema, `validPt`, indexed alternate ranges `alt[j]`, no break, and a
 * ternary store per axis (min = min < p ? min : p; max = max > p ? max : p) in an axis loop; the chained
 * extrema initialisers give retail's reload-and-copy. Layout field names follow GC (types.h has no
 * DWARF struct for gResPath; offsets are N64 retail's). No shaping devices, pads or false prototypes.
 * w14k/w14r (break on hit, pointer-walk vertex, if-form updates) plateaued at 83/102.
 */
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef s16 Vertex[3];
typedef struct AltPath {
    u8 type;
    u8 unk01[9];
    u16 numPts;
    Vertex *pts;
} AltPath;
typedef struct ResPath {
    u16 numPrimary;
    u8 unk02[6];
    u8 numAlt;
    u8 unk09[3];
    AltPath *alt;
} ResPath;
extern s16 D_801407D4[3];
extern s16 D_801407B4[3];
extern ResPath D_801407F0;
extern Vertex *D_801409E8;
extern u16 D_801527A4;

void func_800B9740(void)
{
    int i, j, validPt;

    D_801407D4[0] = D_801407D4[1] = D_801407D4[2] = 0x7FFF;
    D_801407B4[0] = D_801407B4[1] = D_801407B4[2] = -0x7FFF;
    for (i = 0; i < D_801527A4; i++) {
        if (i < D_801407F0.numPrimary) {
            validPt = 1;
        } else {
            validPt = 0;
            for (j = 0; j < D_801407F0.numAlt; j++) {
                if (&D_801409E8[i] >= D_801407F0.alt[j].pts &&
                    &D_801409E8[i] < D_801407F0.alt[j].pts + D_801407F0.alt[j].numPts &&
                    D_801407F0.alt[j].type == 1) {
                    validPt = 1;
                }
            }
        }
        if (validPt == 1) {
            for (j = 0; j < 3; j++) {
                D_801407D4[j] = (D_801407D4[j] < D_801409E8[i][j]) ? D_801407D4[j] : D_801409E8[i][j];
                D_801407B4[j] = (D_801407B4[j] > D_801409E8[i][j]) ? D_801407B4[j] : D_801409E8[i][j];
            }
        }
    }
}
