/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* CalcResPathBounds (historical N64 name func_800B9740), 0x800B9740.
 * Compute s16 bounds for primary resurrection points and alternate points whose
 * alternate path has resurrect kind 1. Empty selections retain +/-32767.
 * Source evidence: MAT3 GameSource/src/pathcommon.c CalcResPathBounds, and N64
 * ReadPathData callers, 6-byte points, 16-byte alternate records and extrema.
 * Shared j spans the alternate and axis loops; all matching alternates are
 * visited. Extrema use ternary assignments, including stores when unchanged.
 * Disclosed source spelling: the exclusive alternate endpoint is formed by a
 * byte offset (count * sizeof(Vertex)); this gives retail's addition operand
 * order. No unused locals, compiled-out reads, padding locals or ABI stand-ins.
 */
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef s16 Vertex[3];
typedef struct VertexRange {
    u8 kind;
    u8 unknown01[9];
    u16 count;
    Vertex *points;
} VertexRange;
typedef struct VertexSelection {
    u16 primary_count;
    u8 unknown02[6];
    u8 range_count;
    u8 unknown09[3];
    VertexRange *ranges;
} VertexSelection;
extern s16 D_801407D4[3]; /* minimum */
extern s16 D_801407B4[3]; /* maximum */
extern VertexSelection D_801407F0;
extern Vertex *  D_801409E8;
extern u16 D_801527A4;

void func_800B9740(void)
{
    int i;
    int j;
    int validPt;
    D_801407D4[0] = D_801407D4[1] = D_801407D4[2] = 32767;
    D_801407B4[0] = D_801407B4[1] = D_801407B4[2] = -32767;
    for (i = 0; i < D_801527A4; ++i) {
        if (i < D_801407F0.primary_count)
            validPt = 1;
        else {
            validPt = 0;
            for (j = 0; j < D_801407F0.range_count; ++j)
                if (D_801409E8[i] >= D_801407F0.ranges[j].points[0] &&
                    D_801409E8[i] < ((s16 *)((u8 *)D_801407F0.ranges[j].points + D_801407F0.ranges[j].count * sizeof(Vertex))) &&
                    D_801407F0.ranges[j].kind == 1)
                    validPt = 1;
        }
        if (validPt == 1)
            for (j = 0; j < 3; ++j) {
                D_801407D4[j] = D_801407D4[j] < D_801409E8[i][j] ? D_801407D4[j] : D_801409E8[i][j];
                D_801407B4[j] = D_801407B4[j] > D_801409E8[i][j] ? D_801407B4[j] : D_801409E8[i][j];
            }
    }
}
