void *func_800AC9BC(QNode *n, s16 x, s16 y, s16 *out) {
    s16 q;
    for (;;) {
        q = 1;
        if (x < (n->x0 + n->x1) / 2) q = 0;
        if (y < (n->y0 + n->y1) / 2) q += 2;
        if (!(n->mask & (1 << q))) break;
        n = &D_80124EEC[n->child[q]];
        if (n == D_80124EEC) return 0;
    }
    *out = q;
    return n;
}