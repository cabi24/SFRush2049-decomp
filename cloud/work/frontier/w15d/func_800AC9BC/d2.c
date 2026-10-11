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

QNode *func_800AC9BC(QNode *n, s16 x, s16 y, s16 *quad)
{
    s16 q;

    do {
        if (x < (n->x0 + n->x1) / 2) {
            q = 0;
        } else {
            q = 1;
        }
        if (y < (n->y0 + n->y1) / 2) {
            q += 2;
        }
        if (!(n->leafMask & (1 << q))) {
            *quad = q;
            return n;
        }
        n = &D_80124EEC[n->child[q]];
    } while (n != D_80124EEC);
    return 0;
}
