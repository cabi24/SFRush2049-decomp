void func_800C2944(s32 idx) {
    Ply *p;
    PCar *c;
    f32 v[3];
    s32 fl;

    p = &D_801569B8[idx];
    fl = p->flags;
    if (fl & 0x20) {
        if (D_80156CE4 != 0 || D_80157238 >= 3) {
            p->flags = fl & ~0x20;
        } else {
            c = (PCar *)(u32)&player_array[idx];
            p->a04 += c->vx;
            p->a08 += c->vy;
            p->a0C += c->vz;
        }
        v[0] = fabsf(p->a04);
        v[1] = fabsf(p->a08);
        v[2] = fabsf(p->a0C);
        if (D_80123F1C < v[0] || D_80123F20 < v[1]) {
            p->flags &= ~0x20;
            return;
        }
        if ((D_80123F24 < v[2] && v[2] < D_80123F28) || ((D_80156CE4 != 0 || D_80157238 > 0 || (player_array[idx].fE8 & 0x100000)) && D_80123F2C < v[2])) {
            p->flags &= ~0x80;
            if (p->a0C > 0.0f) {
                func_800C1B60(3, idx);
                p->a0C += D_80123F30;
            } else {
                func_800C1B60(2, idx);
                p->a0C += D_80123F34;
            }
            p->a08 = 0.0f;
            p->a04 = 0.0f;
        }
    } else if (D_80161360 != 0 && D_80161388[1] < D_80161388[2] && D_80161388[0] < D_80161388[2]) {
        p->flags = fl | 0x20;
        c = (PCar *)(u32)&player_array[idx];
        p->a04 = c->vx;
        p->a08 = c->vy;
        p->a0C = c->vz;
    }
}