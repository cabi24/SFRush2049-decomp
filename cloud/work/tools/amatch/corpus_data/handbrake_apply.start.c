s32 handbrake_apply(Node *p, s16 x, s16 y, s32 a3) {
    s16 outside;
    if (p == 0) {
        p = D_80124EEC;
    }
    while (1) {
        outside = x >= p->x1 || x < p->x0 || y >= p->y1 || y < p->y0;
        if (outside) {
            if (p->child == -1) {
                return 0;
            }
            p = &D_80124EEC[p->child];
        } else {
            return func_800AC9BC(p, x, y, a3);
        }
    }
}