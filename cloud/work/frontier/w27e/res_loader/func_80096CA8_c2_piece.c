void func_80096CA8(s32 slot, s32 first) {
    ParseCtx pc;
    u8 *src;
    u8 *base;
    s32 idx;
    s32 flags;
    s32 i;
    s32 j;
    ResSlot *r;
    s32 tx;
    s32 img;

    r = &D_80156D38[slot];
    flags = 0;
    if (r->f3 == 0) {
        r->loaded = 1;
        return;
    }
    if (first != 0 || r->f5 != 0) {
        if (active_player_count == 1) {
            flags = 2;
        } else if (active_player_count == 2) {
            flags = 4;
        }
        if (D_80152570 != 0) {
            flags |= 0x10;
        } else {
            flags |= 8;
        }
        if (r->f5 != 0) {
            src = *r->p0C;
        } else {
            src = (u8 *)r->p0C;
        }
        idx = slot;
        if (first != 0) {
            func_80096C28(pc.ctx, src);
            base = pc.base;
        } else {
            base = *r->p0C - (u32)r->w10;
            func_80096BBC(pc.ctx, (s32)src, (s32)base);
        }
        r->w10 = *r->p0C;
        D_801161F4[slot].ptr = (void *)lookup_with_output(pc.ctx, 0x4F424844, &D_801161F4[slot].count);
        D_80151AE8[slot].ptr = (void *)lookup_with_output(pc.ctx, 0x54584844, &D_80151AE8[slot].count);
        ((Tab *)D_80138670)[slot].ptr = (void *)lookup_with_output(pc.ctx, 0x504C4844, &((Tab *)D_80138670)[slot].count);
        if (r->type >= 101 && r->type < 120) {
            D_801392D0 = (TexRec *)lookup_with_output(pc.ctx, 0x50544844, &D_801392D4);
        }
        tx = lookup_with_output(pc.ctx, 0x54584C44, &pc.size);
        img = lookup_with_output(pc.ctx, 0x494D4147, NULL);
        entity_cull_check(tx, tx + pc.size, img);
        for (i = 0; i < D_80151AE8[slot].count; i++) {
            ((TexRec *)D_80151AE8[slot].ptr)[i].w18 += img;
        }
        for (i = 0; i < ((Tab *)D_80138670)[slot].count; i++) {
            ((PalRec *)((Tab *)D_80138670)[slot].ptr)[i].w14 += img;
        }
        if (first != 0) {
            for (i = 0; i < D_801161F4[idx].count; i++) {
                for (j = 0; j < ((ObjRec *)D_801161F4[idx].ptr)[i].nsub; j++) {
                    ((ObjRec *)D_801161F4[slot].ptr)[i].sub[j].a += (u32)(first != 0 ? src : base);
                    ((ObjRec *)D_801161F4[slot].ptr)[i].sub[j].b += (u32)(first != 0 ? src : base);
                    entity_lod_select((u32 *)((ObjRec *)D_801161F4[slot].ptr)[i].sub[j].a, (s32)(first != 0 ? src : base), -1, D_80140A04 ? 1 : 0, flags);
                }
            }
        }
        if (r->type >= 101 && r->type < 120) {
            for (i = 0; i < D_801392D4; i++) {
                D_801392D0[i].w1C += (u32)(first != 0 ? src : base);
                D_801392D0[i].w18 += (u32)(first != 0 ? src : base);
            }
        }
        r->loaded = 1;
        entity_render_mode(slot);
    }
}
