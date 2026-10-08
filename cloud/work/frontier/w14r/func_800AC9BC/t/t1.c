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
    /*@{QT*/u8 lm;/*@| @}*/
    /*@{DM*/s32 my;/*@| @}*/
    /*@{DX*/s32 mx;/*@| @}*/
    /*@{QT*/s16 q;/*@| s32 q; @}*/

    for (;;) {
        /*@{DX*/mx = ((s32)n->x0 + n->x1) / 2;/*@| @}*/
        /*@{DM*/my = (n->y0 + n->y1) / 2;/*@| @}*/
        q = /*@{DX*/(x < mx)/*@| (x < ((s32)n->x0 + n->x1) / 2) @}*/ ? 0 : 1;
        /*@{DL*/lm = n->leafMask;/*@| @}*/
        if (y < /*@{DM*/my/*@| (n->y0 + n->y1) / 2 @}*/) {
            /*@{QO*/q |= 2;/*@| q += 2; @}*/
        }
        /*@{DL*/lm = n->leafMask;/*@| @}*/
        if (!(/*@{DL*/lm/*@| n->leafMask @}*/ & (1 << q))) goto found;
        /*@{NC*/n = D_80124EEC + n->child[q];/*@| n = &D_80124EEC[n->child[q]]; @}*/
        if (n == D_80124EEC) return 0;
    }
found:
    *quad = q;
    return n;
}
