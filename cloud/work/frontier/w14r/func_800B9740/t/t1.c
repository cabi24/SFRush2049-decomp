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
extern s16 D_801407D4[3];
extern s16 D_801407B4[3];
extern VertexSelection D_801407F0;
extern Vertex *  D_801409E8;
extern u16 D_801527A4;

void func_800B9740(void)
{
    /*@{DO*/int i, j, axis, eligible;/*@| int i, j, axis; @}*/
    /*@{RG*/VertexRange *range;/*@| @}*/
    Vertex *vertex;

    D_801407D4[0] = D_801407D4[1] = D_801407D4[2] = 32767;
    D_801407B4[0] = D_801407B4[1] = D_801407B4[2] = -32767;

    for (i = 0; i < D_801527A4; i++) {
        if (i < D_801407F0.primary_count) {
            /*@{EL*/eligible = 1;/*@| @}*/
        } else {
            /*@{EL*/eligible = 0;/*@| @}*/
            vertex = D_801409E8 + i;
            /*@{RG*/range = D_801407F0.ranges;/*@| @}*/
            for (j = 0; j < D_801407F0.range_count; j++/*@{RG*/, range++/*@| @}*/) {
                if (vertex >= /*@{RG*/range/*@| (D_801407F0.ranges + j) @}*/->points &&
                    vertex < /*@{RG*/range/*@| (D_801407F0.ranges + j) @}*/->points + /*@{RG*/range/*@| (D_801407F0.ranges + j) @}*/->count && /*@{RG*/range/*@| (D_801407F0.ranges + j) @}*/->kind == 1)
                    /*@{EL*/eligible = 1;/*@| if (1) { vertex = D_801409E8 + i; for (axis = 0; axis < 3; axis++) { if (D_801407D4[axis] > (*vertex)[axis]) D_801407D4[axis] = (*vertex)[axis]; if ((*vertex)[axis] > D_801407B4[axis]) D_801407B4[axis] = (*vertex)[axis]; } } @}*/
            }
        }
        /*@{EL*/if (eligible == 1) {/*@| if (0) { @}*/
            vertex = D_801409E8 + i;
            for (axis = 0; axis < 3; axis++) {
                /*@{MM*/D_801407D4[axis] = D_801407D4[axis] < (*vertex)[axis] ? D_801407D4[axis] : (*vertex)[axis];/*@| if (D_801407D4[axis] > (*vertex)[axis]) D_801407D4[axis] = (*vertex)[axis]; @}*/
                /*@{MM*/D_801407B4[axis] = (*vertex)[axis] < D_801407B4[axis] ? D_801407B4[axis] : (*vertex)[axis];/*@| if ((*vertex)[axis] > D_801407B4[axis]) D_801407B4[axis] = (*vertex)[axis]; @}*/
            }
        }
    }
}
