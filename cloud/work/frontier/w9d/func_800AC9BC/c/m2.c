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
    s16 q;
    s32 mx;
    s32 my;

    for (;;) {
        mx = (n->x0 + n->x1) / 2;
        my = (n->y0 + n->y1) / 2;
        q = 1;
        if (x < mx) {
            q = 0;
        }
        if (y < my) {
            q += 2;
        }
        if (n->leafMask & (1 << q)) {
            n = &D_80124EEC[n->child[q]];
            if (n == D_80124EEC) {
                return 0;
            }
        } else {
            break;
        }
    }
    *quad = q;
    return n;
}
