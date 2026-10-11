/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;

typedef struct QNode {
    u8 f0[3];
    u8 leafMask;
    s16 x0, x1;
    s16 y0, y1;
    u16 child[4];
} QNode; /* 0x14 */

extern QNode *D_80124EEC;

QNode *func_800AC9BC(QNode *tp, s16 x, s16 y, s16 *quad)
{
    s32 midh, midv;
    s16 q;

    for (;;) {
        q = 1;
        midh = (tp->x0 + tp->x1) / 2;
        midv = (tp->y0 + tp->y1) / 2;
        if (x < midh) {
            q = 0;
        }
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
