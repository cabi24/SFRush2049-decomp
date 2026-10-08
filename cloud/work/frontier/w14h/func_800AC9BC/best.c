/* flags: -g0 -O3 -mips2 -G 0 -non_shared -- NOT A MATCH: 25 differing words standalone (score.py fn); see ../RESULTS.md */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;

typedef struct QNode {
    u8 pad0[3];
    u8 leafMask;
    s16 x0, x1;
    s16 y0, y1;
    u16 child[4];
} QNode; /* 0x14 */

extern QNode *D_80124EEC;

QNode *func_800AC9BC(QNode *n, s16 x, s16 y, s16 *quad) {
    u8 lm;
    s32 my;
    s32 mx;
    s16 q;

    for (;;) {
        mx = ((s32)n->x0 + n->x1) / 2;
        my = (n->y0 + n->y1) / 2;
        q = (x < mx) ? 0 : 1;
        lm = n->leafMask;
        if (y < my) {
            q |= 2;
        }
        lm = n->leafMask;
        if (!(lm & (1 << q))) goto found;
        n = D_80124EEC + n->child[q];
        if (n == D_80124EEC) return 0;
    }
found:
    *quad = q;
    return n;
}
