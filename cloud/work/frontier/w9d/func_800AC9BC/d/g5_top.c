/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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

QNode *func_800AC9BC(QNode *n, s16 x, s16 y, s16 *quad)
{
    s32 my;
    s32 mx;
    s16 q;

    do {
        mx = (n->x0 + n->x1) / 2;
        my = (n->y0 + n->y1) / 2;
        q = 1;
        if (x < mx) {
            q = 0;
        }
        if (y < my) {
            q += 2;
        }
        if (!(n->leafMask & (1 << q))) {
            goto found;
        }
        n = &D_80124EEC[n->child[q]];
    } while (n != D_80124EEC);
    return 0;
found:
    *quad = q;
    return n;
}
