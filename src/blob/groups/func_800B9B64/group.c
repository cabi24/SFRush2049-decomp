/*
 * Track path graph: nodes (16 bytes each) linking runs of points, and the
 * searches that connect them. Hand-written from the assembly (cloud Lane A).
 * The same module contains func_800D2FA8 / split_time_display /
 * time_of_day_select (group func_800D2FA8).
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
    /* 0x00 */ u8 pad0[8];
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
void func_800B9B64(s32 skip, s32 atStart, s32 *outNode, PathPoint *pos, s32 *outIdx);
s32 minimap_render(s32 node, s32 pos, s32 *outNode, s32 *outPos, s32 *dist, s32 stopAtTyped, s32 depth);
void physics_friction_apply(void);
s32 physics_velocity_clamp(s32 node, s32 pos, s32 *out, s32 depth);

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

/* nearest point to `pos`: global points first (outNode = -1), then node points */
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

/* link every node to its nearest neighbours, then assign sections */
void physics_friction_apply(void)
{
    s32 i;
    s32 section;
    s32 node;
    s32 idx;

    for (i = 0; i < D_801407F0.numNodes; i++) {
        if (D_801407F0.nodes[i].type == 2) {
            D_801407F0.nodes[i].next = -1;
            D_801407F0.nodes[i].nextPos = 0;
            D_801407F0.nodes[i].prev = -1;
            D_801407F0.nodes[i].prevPos = 1;
        } else {
            func_800B9B64(i, 1, &node, D_801407F0.nodes[i].points, &idx);
            D_801407F0.nodes[i].next = node;
            D_801407F0.nodes[i].nextPos = idx;
            func_800B9B64(i, 0, &node, &D_801407F0.nodes[i].points[D_801407F0.nodes[i].numPoints - 1],
                      &idx);
            D_801407F0.nodes[i].prev = node;
            D_801407F0.nodes[i].prevPos = idx;
        }
    }
    for (i = 0; i < D_801407F0.numNodes; i++) {
        if (D_801407F0.nodes[i].type == 2) {
            D_801407F0.nodes[i].section = -1;
        } else if (!physics_velocity_clamp(i, 0, &section, 0)) {
            D_801407F0.nodes[i].section = -1;
        } else {
            D_801407F0.nodes[i].section = section;
        }
    }
}

/* section containing point `pos` of `node`, following `next` links back */
s32 physics_velocity_clamp(s32 node, s32 pos, s32 *out, s32 depth)
{
    s32 i;
    s16 best;

    if (!func_800B98D8(node, depth)) {
        return 0;
    }
    if (node < 0) {
        for (i = 1; i < D_80151CE8[0].count; i++) {
            if (pos < D_80151CE8[i].start) {
                break;
            }
        }
        *out = i - 1;
        return 1;
    }
    best = -1;
    for (i = 0; i < D_80151CE8[0].count; i++) {
        if (D_80151CE8[i].nodeStart[node] >= 0 &&
            (best < 0 || best < D_80151CE8[i].nodeStart[node]) &&
            pos >= D_80151CE8[i].nodeStart[node]) {
            *out = i;
            best = D_80151CE8[i].nodeStart[node];
        }
    }
    if (best >= 0) {
        return 1;
    }
    return physics_velocity_clamp(D_801407F0.nodes[node].next, D_801407F0.nodes[node].nextPos,
                                  out, depth + 1);
}

/* stand-in caller: keeps func_800B9B64 out of line under -O3 */
void __standin_func_800B9B64(void)
{
    func_800B9B64(0, 0, 0, 0, 0);
}

/* stand-in caller: keeps physics_velocity_clamp out of line under -O3 */
void __standin_physics_velocity_clamp(void)
{
    physics_velocity_clamp(0, 0, 0, 0);
}
