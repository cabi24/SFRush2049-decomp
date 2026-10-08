/* Research, NOT MATCHED. flags: -g0 -O3 -mips2 -G 0 -non_shared  (w14k round b1: choice-point template from w3c best)
 * Vertex/range layouts inferred from retail loads, not arcade type names.
 * Range pointers must refer into the same vertex array as D_801409E8.
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
    /*@{*/int i, j, axis, eligible;/*@| int i, eligible; @| int i, j, eligible; @| int i, j, axis, eligible, off; @}*/
    VertexRange *range;
    Vertex *vertex;
    /*@{*//*@| int count; @| VertexRange *ranges; @}*/
    
    D_801407D4[0] = D_801407D4[1] = D_801407D4[2] = 32767;
    D_801407B4[0] = D_801407B4[1] = D_801407B4[2] = -32767;
    
    for (i = 0; i < D_801527A4; i++) {
        if (/*@{*/D_801407F0.primary_count > i/*@| i < D_801407F0.primary_count @}*/) {
            eligible = 1;
        } else {
            eligible = 0;
            vertex = &D_801409E8[i];
            /*@{*/range = D_801407F0.ranges;
            for (j = 0; j < D_801407F0.range_count; j++, range++) {
                if (vertex >= range->points &&
                    vertex < range->points + range->count && range->kind == 1)
                    eligible = 1;
            }/*@| range = D_801407F0.ranges;
            for (j = 0; j < D_801407F0.range_count; j++, range++) {
                if (vertex >= range->points &&
                    vertex < range->points + range->count && range->kind == 1) {
                    eligible = 1;
                    break;
                }
            }@| for (j = 0; j < D_801407F0.range_count; j++) {
                range = (VertexRange *)((u8 *)D_801407F0.ranges + j * 16);
                if (vertex >= range->points &&
                    vertex < range->points + range->count && range->kind == 1)
                    eligible = 1;
            }
            @| for (j = 0; j < D_801407F0.range_count * 16; j += 16) {
                range = (VertexRange *)((u8 *)D_801407F0.ranges + j);
                if (vertex >= range->points &&
                    vertex < range->points + range->count && range->kind == 1)
                    eligible = 1;
            }
            @}*/
        }
        if (eligible == 1) {
            vertex = D_801409E8 + i;
            /*@{*/for (axis = 0; axis < 3; axis++) {
                if ((*vertex)[axis] < D_801407D4[axis]) D_801407D4[axis] = (*vertex)[axis];
                if ((*vertex)[axis] > D_801407B4[axis]) D_801407B4[axis] = (*vertex)[axis];
            }/*@| if ((*vertex)[0] < D_801407D4[0]) D_801407D4[0] = (*vertex)[0];
            if ((*vertex)[0] > D_801407B4[0]) D_801407B4[0] = (*vertex)[0];
            if ((*vertex)[1] < D_801407D4[1]) D_801407D4[1] = (*vertex)[1];
            if ((*vertex)[1] > D_801407B4[1]) D_801407B4[1] = (*vertex)[1];
            if ((*vertex)[2] < D_801407D4[2]) D_801407D4[2] = (*vertex)[2];
            if ((*vertex)[2] > D_801407B4[2]) D_801407B4[2] = (*vertex)[2];
            @| for (axis = 0; axis < 3; axis++) {
                if ((*vertex)[axis] < D_801407D4[axis]) D_801407D4[axis] = (*vertex)[axis];
                if ((*vertex)[axis] > D_801407B4[axis]) D_801407B4[axis] = (*vertex)[axis];
                if (axis == 0) {}
            } @}*/
        }
    }
}
