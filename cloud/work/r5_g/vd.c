/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct QNode {
    s16 next;
    u8 pad2;
    u8 mask;
    s16 x0;
    s16 x1;
    s16 y0;
    s16 y1;
    u16 child[4];
} QNode;
extern QNode *D_80124EEC;
void *func_800AC9BC(QNode *n, s16 x, s16 y, s16 *out);

void *handbrake_apply(QNode *n, s16 x, s16 y, s16 *out) {
    s16 xx = x, yy = y;
    s16 outside;
    if (n == 0) {
        n = D_80124EEC;
    }
    while (1) {
        outside = (xx < n->x1 || xx >= n->x0 || yy < n->y1 || yy >= n->y0);
        if (!outside) break;
        if (n->next == -1) {
            return 0;
        }
        n = &D_80124EEC[n->next];
    }
    return func_800AC9BC(n, x, y, out);
}
