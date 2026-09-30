/*
 * Track path graph: nodes (16 bytes each) linking runs of points, and the
 * searches that connect them. Hand-written from the assembly (cloud Lane A).
 * Group func_800D2FA8: func_800D2FA8 / split_time_display / time_of_day_select,
 * with func_800B98D8 and minimap_render (matched in group func_800B9B64) as context.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;

typedef struct {
    s16 x, y, z;
} PathPoint;

typedef struct {
    /* 0x0 */ u8 type;       /* 2 = standalone (not linked) */
    /* 0x1 */ s8 next;       /* node joined at this node's start */
    /* 0x2 */ u16 nextPos;   /* point index in `next` */
    /* 0x4 */ s8 prev;       /* node joined at this node's end */
    /* 0x6 */ u16 prevPos;   /* point index in `prev` */
    /* 0x8 */ s8 section;
    /* 0xA */ u16 numPoints;
    /* 0xC */ PathPoint *points;
} PathNode;

typedef struct {
    /* 0x0 */ u16 numPoints;
    /* 0x4 */ PathPoint *points;
    /* 0x8 */ u8 numNodes;
    /* 0xC */ PathNode *nodes;
} PathGraph;

typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ s16 last;      /* valid in element 0 */
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s16 count;     /* valid in element 0 */
    /* 0x0A */ u8 padA[0x2E - 0xA];
    /* 0x2E */ s16 start;
    /* 0x30 */ u8 pad30[8];
    /* 0x38 */ s16 nodeStart[12];
} Section;                    /* 0x50 */

extern PathGraph D_801407F0;
extern s32 D_80143A88[];      /* nodes visited by the current walk */
extern f32 D_80123DF8;        /* initial (large) best distance */
extern Section D_80151CE8[];

s32 func_800B98D8(s32 node, s32 depth);
s32 minimap_render(s32 node, s32 pos, s32 *outNode, s32 *outPos, s32 *dist, s32 stopAtTyped, s32 depth);

/* record `node` at `depth` in the walk; 0 if it was already visited or is a dead end */
s32 func_800B98D8(s32 node, s32 depth)
{
    s32 i;

    if (node >= 0 && D_801407F0.nodes[node].type != 0) {
        if ((D_801407F0.nodes[node].next >= 0 &&
             D_801407F0.nodes[D_801407F0.nodes[node].next].type == 0) ||
            (D_801407F0.nodes[node].prev >= 0 &&
             D_801407F0.nodes[D_801407F0.nodes[node].prev].type == 0)) {
            return 0;
        }
    }
    D_80143A88[depth] = node;
    for (i = 0; i < depth; i++) {
        if (node == D_80143A88[i]) {
            return 0;
        }
    }
    return 1;
}

/* follow `prev` links from `node`, summing the distance walked */
s32 minimap_render(s32 node, s32 pos, s32 *outNode, s32 *outPos, s32 *dist, s32 stopAtTyped, s32 depth)
{
    if (!func_800B98D8(node, depth)) {
        return 0;
    }
    if (depth == 0) {
        *dist = 0;
    }
    if (node < 0 || (stopAtTyped && D_801407F0.nodes[node].type != 0)) {
        *outNode = node;
        *outPos = pos;
        return 1;
    }
    *dist += D_801407F0.nodes[node].numPoints - pos;
    return minimap_render(D_801407F0.nodes[node].prev, D_801407F0.nodes[node].prevPos,
                          outNode, outPos, dist, stopAtTyped, depth + 1);
}

extern s32 D_80124F88[];      /* nodes visited by the current search */

s32 time_of_day_select(s32 node, s32 pos, s32 *outNode, s32 *dist, s32 depth);
void split_time_display(s32 node, s32 pos, s32 remain, s32 *outNode, s32 *outPos);
s32 func_800D2FA8(s32 node, s32 pos, s32 *outNode, s32 *outPos, s32 stopAtTyped, s32 depth);

/* follow `next` links from `node`, summing the distance walked */
s32 time_of_day_select(s32 node, s32 pos, s32 *outNode, s32 *dist, s32 depth)
{
    if (!func_800B98D8(node, depth)) {
        return 0;
    }
    if (depth == 0) {
        *dist = 0;
    }
    if (node < 0) {
        *outNode = pos;
        return 1;
    }
    *dist += pos;
    return time_of_day_select(D_801407F0.nodes[node].next, D_801407F0.nodes[node].nextPos,
                              outNode, dist, depth + 1);
}

/* walk `remain` points back along `next` links; result node/point in outNode/outPos */
void split_time_display(s32 node, s32 pos, s32 remain, s32 *outNode, s32 *outPos)
{
    if (node < 0) {
        *outNode = node;
        *outPos = pos - remain;
        if (pos < D_80151CE8[D_80151CE8[0].last].start) {
            if (*outPos < 0) {
                *outPos = 0;
            }
        } else {
            node = *outPos;
            while (node < D_80151CE8[D_80151CE8[0].last].start) {
                node = node + D_801407F0.numPoints - D_80151CE8[D_80151CE8[0].last].start;
                *outPos = node;
            }
        }
    } else if (pos >= remain) {
        *outNode = node;
        *outPos = pos - remain;
    } else {
        split_time_display(D_801407F0.nodes[node].next, D_801407F0.nodes[node].nextPos, remain - pos,
                           outNode, outPos);
    }
}

s32 func_800D2FA8(s32 node, s32 pos, s32 *outNode, s32 *outPos, s32 stopAtTyped, s32 depth)
{
    s32 i;
    volatile s32 padv[2];
    s32 dist1, total, dist2, start1, node2, pos2, start2;
    s32 v;

    if (depth == 0) {
        *outNode = node;
        *outPos = pos;
    }
    D_80124F88[depth] = node;
    for (i = 0; i < depth; i++) {
        if (node == D_80124F88[i]) {
            return 0;
        }
    }
    if (D_801407F0.nodes[node].next == D_801407F0.nodes[node].prev) {
        if (D_801407F0.nodes[node].nextPos < D_801407F0.nodes[node].prevPos) {
            *outPos = pos * (D_801407F0.nodes[node].prevPos - D_801407F0.nodes[node].nextPos + 1) /
                          D_801407F0.nodes[node].numPoints + D_801407F0.nodes[node].nextPos;
        } else {
            if (D_801407F0.nodes[node].next != -1) {
                return 0;
            }
            *outPos = pos * (D_801407F0.nodes[node].prevPos - D_80151CE8[D_80151CE8[0].last].start +
                             D_801407F0.numPoints - D_801407F0.nodes[node].nextPos + 1) /
                          D_801407F0.nodes[node].numPoints + D_801407F0.nodes[node].nextPos;
            if (*outPos >= D_801407F0.numPoints) {
                *outPos = D_80151CE8[D_80151CE8[0].last].start + *outPos - D_801407F0.numPoints;
            }
        }
        *outNode = D_801407F0.nodes[node].next;
        if (*outNode < 0 || (stopAtTyped && D_801407F0.nodes[*outNode].type != 0)) {
            return 1;
        }
        return func_800D2FA8(*outNode, *outPos, outNode, outPos, stopAtTyped, depth + 1);
    }
    if (!time_of_day_select(node, pos, &start1, &dist1, 0)) {
        return 0;
    }
    if (!minimap_render(node, pos, &node2, &pos2, &total, stopAtTyped, 0)) {
        return 0;
    }
    if (!time_of_day_select(node2, pos2, &start2, &dist2, 0)) {
        return 0;
    }
    if (start2 < start1) {
        v = start2 + D_801407F0.numPoints - start1 - D_80151CE8[D_80151CE8[0].last].start;
        if (start1 - start2 < v) {
            dist1 = dist1 + start1 - start2;
            start1 = start2;
        } else {
            dist2 = dist2 + v;
        }
    } else if (start1 + D_801407F0.numPoints - start2 < start2 - start1) {
        start1 = start2;
        dist1 = dist1 + start1 + D_801407F0.numPoints - start2;
    } else {
        dist2 = dist2 + start2 - start1;
    }
    total = total + dist1;
    split_time_display(node2, pos2, dist2 - dist1 * dist2 / total, outNode, outPos);
    return 1;
}

/* stand-in callers: keep the callees out of line under -O3 */
void __standin_split_time_display(void)
{
    split_time_display(0, 0, 0, 0, 0);
}

void __standin_time_of_day_select(void)
{
    time_of_day_select(0, 0, 0, 0, 0);
}
