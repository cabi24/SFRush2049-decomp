/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
extern s16 *D_801409E8;
extern u16 D_801527A4;

void func_800B9740(void)
{
    int i, j, axis, eligible;
    s16 *vertex;

    D_801407D4[0] = D_801407D4[1] = D_801407D4[2] = 32767;
    D_801407B4[0] = D_801407B4[1] = D_801407B4[2] = -32767;
    for (i = 0; i < D_801527A4; i++) {
        if (i < D_801407F0.primary_count) {
            eligible = 1;
        } else {
            eligible = 0;
            for (j = 0; j < D_801407F0.range_count; j++) {
                vertex = &D_801409E8[i * 3];
                if ((Vertex *)vertex >= D_801407F0.ranges[j].points &&
                    (Vertex *)vertex < D_801407F0.ranges[j].points + D_801407F0.ranges[j].count && D_801407F0.ranges[j].kind == 1)
                    eligible = 1;
            }
        }
        if (eligible == 1) {
            vertex = &D_801409E8[i * 3];
            for (axis = 0; axis < 3; axis++) {
                D_801407D4[axis] = D_801407D4[axis] < vertex[axis]
                    ? D_801407D4[axis] : vertex[axis];
                D_801407B4[axis] = vertex[axis] < D_801407B4[axis]
                    ? D_801407B4[axis] : vertex[axis];
            }
        }
    }
}
