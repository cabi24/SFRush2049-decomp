void func_800C26C4(s32 idx) {
    Ply *p;
    PCar *c;
    f32 v[3];
    f32 d;
    s32 fl;

    p = &D_801569B8[idx];
    fl = p->flags;
    if (fl & 0x10) {
        if (D_80156BD8 != 0 || D_80157238 >= 3) {
            p->flags = fl & ~0x10;
        } else {
            c = (PCar *)(u32)&player_array[idx];
            p->a1C += c->vx;
            p->a20 += c->vy;
            p->a24 += c->vz;
        }
        v[0] = fabsf(p->a1C);
        v[1] = fabsf(p->a20);
        v[2] = fabsf(p->a24);
        if (D_80123F04 < v[0] || D_80123F04 < v[2]) {
            p->flags &= ~0x10;
            return;
        }
        if ((D_80123F08 < v[1] && v[1] < D_80123F0C) || ((D_80156BD8 != 0 || D_80157238 > 0 || ((c = (PCar *)(u32)&player_array[idx])->fE8 & 0x100000)) && D_80123F10 < v[1])) {
            p->flags &= ~0x80;
            func_800C1B60(4, idx);
            d = (p->a20 > 0.0f) ? D_80123F14 : D_80123F18;
            p->a24 = 0.0f;
            p->a1C = 0.0f;
            p->a20 = p->a20 + d;
        }
    } else if (D_80161360 != 0 && D_80161388[0] < D_80161388[1] && D_80161388[2] < D_80161388[1]) {
        p->flags = fl | 0x10;
        c = (PCar *)(u32)&player_array[idx];
        p->a1C = c->vx;
        p->a20 = c->vy;
        p->a24 = c->vz;
    }
}