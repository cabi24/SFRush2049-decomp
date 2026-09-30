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

void *func_800AC9BC(QNode *n, s16 x, s16 y, s16 *out) {
    short yy = y;
    int q;
    for (;;) {
        q = 1;
        if (x < (n->x0 + n->x1) / 2) q = 0;
        q += (yy < (n->y0 + n->y1) / 2) ? 2 : 0;
        if (!(n->mask & (1 << q))) break;
        n = &D_80124EEC[n->child[q]];
        if (n == D_80124EEC) return 0;
    }
    *out = q;
    return n;
}
void *handbrake_apply(QNode *n, s16 x, s16 y, s16 *out) {
    s16 outside;
    if (n == 0) {
        n = D_80124EEC;
    }
    while (1) {
        outside = (x >= n->x1 || x < n->x0 || y >= n->y1 || y < n->y0);
        if (!outside) break;
        if (n->next == -1) {
            return 0;
        }
        n = &D_80124EEC[n->next];
    }
    return func_800AC9BC(n, x, y, out);
}


void *__standin_hb_a(QNode *n, s16 x, s16 y, s16 *o) { return handbrake_apply(n, x, y, o); }
void *__standin_hb_b(QNode *n, s16 x, s16 y, s16 *o) { return handbrake_apply(n, y, x, o); }
