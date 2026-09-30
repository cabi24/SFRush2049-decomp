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