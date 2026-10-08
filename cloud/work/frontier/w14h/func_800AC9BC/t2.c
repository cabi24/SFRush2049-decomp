/* flags: -g0 -O3 -mips2 -G 0 -non_shared -- NOT A MATCH (w14h vbatch template) */
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
    /*@{dec*/s16 q;
    s32 mx;
    s32 my;/*@| s32 mx;
    s32 my;
    s16 q; @| s32 my;
    s32 mx;
    s16 q; @| s16 q;
    s32 my;
    s32 mx; @}*/

    /*@{lp*/do {/*@| for (;;) { @| while (n != D_80124EEC) { @}*/
        /*@{l0*/lm = n->leafMask;/*@|@}*/
        /*@{qe*/q = 1;/*@|@}*/
        mx = /*@{mxf*/(n->x0 + n->x1) / 2/*@| (n->x1 + n->x0) / 2 @| ((s32)n->x0 + n->x1) / 2 @}*/;
        my = /*@{myf*/(n->y0 + n->y1) / 2/*@| (n->y1 + n->y0) / 2 @| ((s32)n->y0 + n->y1) / 2 @}*/;
        /*@{qi*/q = 1;
        if (x < mx) {
            q = 0;
        }/*@| q = 0;
        if (x >= mx) {
            q = 1;
        } @| if (x < mx) {
            q = 0;
        } else {
            q = 1;
        } @| q = (x < mx) ? 0 : 1; @| q = (x >= mx) ? 1 : 0; @| q = 1 - (x < mx); @}*/
        /*@{l1*/lm = n->leafMask;/*@|@}*/
        /*@{yq*/if (y < my) {
            q += 2;
        }/*@| if (y < my) {
            q = q + 2;
        } @| if (my > y) {
            q += 2;
        } @| if (!(y >= my)) {
            q += 2;
        } @| q += 2 * (y < my); @| if (y < my) {
            q |= 2;
        } @| if (y >= my) {
        } else {
            q += 2;
        } @}*/
        /*@{l2*/lm = n->leafMask;/*@|@}*/
        /*@{lf*/if (!(n->leafMask & (1 << q))) goto found;/*@|
        if ((n->leafMask & (1 << q)) == 0) goto found; @| if (!((n->leafMask >> q) & 1)) goto found; @| if (((n->leafMask >> q) & 1) == 0) goto found; @| if (!(n->leafMask & (1 << q))) {
            goto found;
        } @|if (!(lm & (1 << q))) goto found; @|
        if ((lm & (1 << q)) == 0) goto found; @| if (!((lm >> q) & 1)) goto found; @| if (((lm >> q) & 1) == 0) goto found; @| if (!(lm & (1 << q))) {
            goto found;
        } @}*/
        /*@{cp*/n = &D_80124EEC[n->child[q]];/*@| n = D_80124EEC + n->child[q]; @| n = &D_80124EEC[(s16)n->child[q]]; @}*/
    /*@{lp*/} while (n != D_80124EEC);
    return 0;/*@| if (n == D_80124EEC) return 0;
    } @| } 
    return 0; @}*/
found:
    *quad = q;
    return n;
}
