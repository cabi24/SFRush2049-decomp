/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800D2FA8 @ 0x800D2FA8, 1,160 bytes: track path graph search (N64 code, no arcade ancestor
 * found). From (node, pos) find the node/point the path continues at:
 * - records `node` in the visited list D_80124F88[depth]; returns 0 when it was seen before;
 * - a node whose `next` and `prev` links are the same node is mapped proportionally onto the span
 *   nextPos..prevPos of that node (wrapping round the lap when next == -1), then the search
 *   recurses from there unless the result is the main path (< 0) or a typed node with stopAtTyped;
 * - otherwise the distances both ways round are measured with time_of_day_select (follow `next`)
 *   and minimap_render (follow `prev`), and split_time_display walks back the proportional
 *   remainder. Returns 1 on success. The callee names are historical labels.
 *
 * Matches standalone at -O3 (the frontier's `group` label is a false positive) and is EQUAL in
 * the whole-program unit. -O2: 278 words differ.
 *
 * Shaping facts (each needed):
 * - `v` is one scratch variable: first the `next` link, then the span length, later the wrap
 *   distance; the fields are re-read in source (no named copies of nextPos/prevPos);
 * - two unused locals declared before the seven address-taken ones give the 112-byte frame;
 * - operand orders: `nextPos + pos * v / numPoints`, `*outPos + start - numPoints`, and the last
 *   test written `start2 - start1 > start1 + numPoints - start2`;
 * - `dist1` is updated before `start1 = start2` (it uses the old start1).
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

s32 minimap_render(s32 node, s32 pos, s32 *outNode, s32 *outPos, s32 *dist, s32 stopAtTyped, s32 depth);
extern s32 D_80124F88[];      /* nodes visited by the current search */
s32 time_of_day_select(s32 node, s32 pos, s32 *outNode, s32 *dist, s32 depth);
void split_time_display(s32 node, s32 pos, s32 remain, s32 *outNode, s32 *outPos);
s32 func_800D2FA8(s32 node, s32 pos, s32 *outNode, s32 *outPos, s32 stopAtTyped, s32 depth);

s32 func_800D2FA8(s32 node, s32 pos, s32 *outNode, s32 *outPos, s32 stopAtTyped, s32 depth)
{
    s32 i;
    s32 unused0, unused1;
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
    v = D_801407F0.nodes[node].next;
    if (v == D_801407F0.nodes[node].prev) {
        if (D_801407F0.nodes[node].nextPos < D_801407F0.nodes[node].prevPos) {
            v = D_801407F0.nodes[node].prevPos - D_801407F0.nodes[node].nextPos + 1;
            *outPos = D_801407F0.nodes[node].nextPos + pos * v / D_801407F0.nodes[node].numPoints;
        } else {
            if (v != -1) {
                return 0;
            }
            v = D_801407F0.nodes[node].prevPos - D_80151CE8[D_80151CE8[0].last].start + D_801407F0.numPoints - D_801407F0.nodes[node].nextPos + 1;
            *outPos = D_801407F0.nodes[node].nextPos + pos * v / D_801407F0.nodes[node].numPoints;
            if (*outPos >= D_801407F0.numPoints) {
                *outPos = *outPos + D_80151CE8[D_80151CE8[0].last].start - D_801407F0.numPoints;
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
    } else if (start2 - start1 > start1 + D_801407F0.numPoints - start2) {
        dist1 = dist1 + start1 + D_801407F0.numPoints - start2;
        start1 = start2;
    } else {
        dist2 = dist2 + start2 - start1;
    }
    total = total + dist1;
    split_time_display(node2, pos2, dist2 - dist1 * dist2 / total, outNode, outPos);
    return 1;
}

