void func_800B9B64(s32 skip, s32 atStart, s32 *outNode, PathPoint *pos, s32 *outIdx)
{
    f32 best;
    f32 dx, dy, dz, d;
    s32 i, j;

    best = D_80123DF8;
    *outNode = -1;
    for (i = 0; i < D_801407F0.numPoints; i++) {
        dx = D_801407F0.points[i].x - pos->x;
        dy = D_801407F0.points[i].y - pos->y;
        dz = D_801407F0.points[i].z - pos->z;
        d = dx * dx + dy * dy + dz * dz;
        if (d < best) {
            best = d;
            *outIdx = i;
        }
    }
    for (j = 0; j < D_801407F0.numNodes; j++) {
        if (j == skip) {
            continue;
        }
        if (atStart && j < skip && D_801407F0.nodes[j].next == skip &&
            D_801407F0.nodes[j].nextPos == 0) {
            continue;
        }
        if (!atStart && j < skip && D_801407F0.nodes[j].prev == skip &&
            D_801407F0.nodes[j].prevPos + 1 == D_801407F0.nodes[skip].numPoints) {
            continue;
        }
        for (i = 0; i < D_801407F0.nodes[j].numPoints; i++) {
            dx = D_801407F0.nodes[j].points[i].x - pos->x;
            dy = D_801407F0.nodes[j].points[i].y - pos->y;
            dz = D_801407F0.nodes[j].points[i].z - pos->z;
            d = dx * dx + dy * dy + dz * dz;
            if (d < best) {
                *outIdx = i;
                *outNode = j;
                best = d;
            }
        }
    }
}