/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800AC9BC (0x800AC9BC, 224 bytes): quadtree descent, the N64 port of the arcade downleaf()
 * (reference/repos/rushtherock/game/stree.c):
 *     midh = (tp->minh + tp->maxh) >> 1;  midv = (tp->minv + tp->maxv) >> 1;
 *     q = (pos[HPOS] < midh) ? 0 : 1;  if (pos[VPOS] < midv) q += 2;
 *     child = tp->child[q]; if (child > 0) return downleaf(&stree[child], pos, quad);
 *     else { *quad = q; return tp; }
 * N64 changes: s16 x/y instead of pos[3]; a leaf bit mask at +3 decides "descend" (bit q set) vs "found";
 * child indices are u16 into the 20-byte node array D_80124EEC; child node == the array base means
 * "no node" (return 0). The tail recursion is written as a loop (a recursive tail call re-truncates the
 * s16 x/y every iteration, retail does not).
 * The midpoint is a signed halving written out (`if (s < 0) s++; s >>= 1;`): retail's
 * `bgezl s; addiu s,s,1; sra` modifies the sum in place, which `/ 2` (as1's div expansion through at)
 * does not give; this idiom occurs nowhere else in the game code. Both shifts after both sums puts
 * midh >> 1 in ugen's first ring temp (t6), as in retail.
 * No shaping devices (no compiled-out reads, volatile, unused locals).
 */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;

typedef struct QNode {
    u8 f0[3];
    u8 leafMask;    /* 0x03: bit q set = quadrant q has a child node */
    s16 x0, x1;     /* 0x04, 0x06: horizontal bounds */
    s16 y0, y1;     /* 0x08, 0x0A: vertical bounds */
    u16 child[4];   /* 0x0C: node indices into D_80124EEC */
} QNode; /* 0x14 */

extern QNode *D_80124EEC;

QNode *func_800AC9BC(QNode *tp, s16 x, s16 y, s16 *quad)
{
    s32 midh, midv;
    s16 q;

    for (;;) {
        midh = tp->x0 + tp->x1;
        if (midh < 0) midh++;
        midv = tp->y0 + tp->y1;
        if (midv < 0) midv++;
        midh >>= 1;
        midv >>= 1;
        q = (x < midh) ? 0 : 1;
        if (y < midv) {
            q += 2;
        }
        if (!(tp->leafMask & (1 << q))) {
            break;
        }
        tp = &D_80124EEC[tp->child[q]];
        if (tp == D_80124EEC) {
            return 0;
        }
    }
    *quad = q;
    return tp;
}
