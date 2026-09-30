void func_800C2430(s32 idx) {
    Ply *p;
    PCar *c;
    f32 v[3];
    s32 fl;

    p = &D_801569B8[idx];
    fl = p->flags;
    if (fl & 0x40) {
        if (D_80156BC8 != 0 || D_80157238 >= 3) {
            p->flags = fl & ~0x40;
        } else {
            c = (PCar *)(u32)&player_array[idx];
            p->a10 += c->vx;
            p->a14 += c->vy;
            p->a18 += c->vz;
        }
        v[0] = fabsf(p->a10);
        v[1] = fabsf(p->a14);
        v[2] = fabsf(p->a18);
        if (D_80123EEC < v[1] || D_80123EEC < v[2]) {
            p->flags &= ~0x40;
            return;
        }
        if ((D_80123EF0 < v[0] && v[0] < D_80123EF4) || ((D_80156BC8 != 0 || D_80157238 > 0 || ((c = (PCar *)(u32)&player_array[idx])->fE8 & 0x100000)) && D_80123EF8 < v[0])) {
            p->flags &= ~0x80;
            if (p->a10 > 0.0f) {
                func_800C1B60(0, idx);
                p->a10 += D_80123EFC;
            } else {
                func_800C1B60(1, idx);
                p->a10 += D_80123F00;
            }
            p->a18 = 0.0f;
            p->a14 = 0.0f;
        }
    } else if (D_80161360 != 0 && D_80161388[1] < D_80161388[0] && D_80161388[2] < D_80161388[0]) {
        p->flags = fl | 0x40;
        c = (PCar *)(u32)&player_array[idx];
        p->a10 = c->vx;
        p->a14 = c->vy;
        p->a18 = c->vz;
    }
}