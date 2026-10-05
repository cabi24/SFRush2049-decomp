
s32 particle_system(Gfx **gp, Node *parent, s32 count, s32 flag, Node *node, s32 view, Palette *pal)
{
    Gfx *gfx = *gp;
    LodSet *set;
    s32 lod;
    TexRect rect;
    Gfx *dl;
    View *v;
    f32 d[3];
    s32 drawn = 0;
    f32 dir[3];
    Palette *pals;
    u8 alpha;
    f32 x, y, z;
    f32 yaw, pitch;

    set = &D_801161F4[node->model >> 10].sets[node->model & 0x3FF];
    lod = set->lv[0].count - 1;
    if (node->flags & 4) {
        lod = node->flags & 3;
        if (lod == 0) {
            D_80124EE8 = 0.0f;
        } else {
            D_80124EE8 = set->lv[lod - 1].dist;
        }
    } else if ((set->lv[lod].dist != 0.0f && !(node->flags & (0x100 << view)) && !flag) || (node->flags & 8)) {
        if (count < 2) {
            if (count > 0) {
                d[0] = parent->obj->pos[0] + node->obj->pos[0];
                d[1] = parent->obj->pos[1] + node->obj->pos[1];
                d[2] = parent->obj->pos[2] + node->obj->pos[2];
                v = &D_80150B70[view];
                d[0] = d[0] - v->pos[0];
                d[1] = d[1] - v->pos[1];
                d[2] = d[2] - v->pos[2];
            } else {
                v = &D_80150B70[view];
                d[0] = node->obj->pos[0] - v->pos[0];
                d[1] = node->obj->pos[1] - v->pos[1];
                d[2] = node->obj->pos[2] - v->pos[2];
            }
        }
        x = d[0];
        y = d[1];
        z = d[2];
        D_80124EE8 = sqrtf(x * x + y * y + z * z);
    }
    if (!(node->flags & (0x100 << view))) {
        if (set->lv[lod].dist == 0.0f || !(set->lv[lod].dist * node->scale < D_80124EE8)) {
            if (node->flags & 4) {
                lod = node->flags & 3;
                if (lod >= set->lv[0].count) {
                    lod = set->lv[0].count - 1;
                }
            } else if (lod > 0) {
                while (lod != 0 && D_80124EE8 < set->lv[lod - 1].dist * node->scale) {
                    lod--;
                }
            }
            dl = set->lv[lod].dl;
            if (dl != NULL) {
                if (node->flags & 0x80000) {
                    v = &D_80150B70[view];
                    camera_update_d(&D_8015B250->la[D_80124F84], -v->pos[0], v->pos[1], v->pos[2],
                                    -D_80124F78[0], D_80124F78[1], D_80124F78[2], 0.0f, 1.0f, 0.0f);
                    gSPLookAt(gfx++, &D_8015B250->la[D_80124F84]);
                    gSPSetGeometryMode(gfx++, G_TEXTURE_GEN);
                    D_80124F84++;
                    gSPClearGeometryMode(gfx++, G_FOG);
                    gDPPipeSync(gfx++);
                    gDPSetCycleType(gfx++, G_CYC_1CYCLE);
                    if (node->tex[lod] != NULL) {
                        gSPTexture(gfx++, node->tex[lod]->width << 7, node->tex[lod]->height << 6, 0,
                                   G_TX_RENDERTILE, G_ON);
                    }
                    dir[0] = D_80124F78[0] - v->pos[0];
                    dir[1] = D_80124F78[1] - v->pos[1];
                    dir[2] = D_80124F78[2] - v->pos[2];
                    func_8008E0B8(dir);
                    yaw = func_8008C768(dir[0], dir[2]);
                    rect.x0 = -yaw * 1303.7972f + 6144.0f;
                    pitch = func_8009C3F8(0, dir[1]);
                    rect.y0 = -pitch * 1303.7972f + 2048.0f;
                    rect.x1 = rect.x0 + 4064;
                    rect.y1 = rect.y0 + 2016;
                    node->clip = &rect;
                }
                if (set->lv[lod].flags & 0x10) {
                    gSPSetGeometryMode(gfx++, G_LIGHTING);
                }
                if (node->pal != NULL) {
                    func_80099B30(&gfx, node->pal);
                }
                if (set->lv[lod].flags & 1) {
                    pals = (node->pal != NULL) ? NULL : D_80138670[node->model >> 10].pal;
                    if (node->tex[lod] != NULL) {
                        render_display_list(&gfx, node->tex[lod], node->rect, node->clip, pals);
                    } else if (set->lv[lod].flags & 0x8000) {
                        render_display_list(&gfx, (set->lv[lod].tex & 0x3FF) + D_80151AE8[set->lv[lod].tex >> 10].tex, node->rect, node->clip, pals);
                    }
                } else if (node->clip != NULL) {
                    gDPSetTileSize(gfx++, G_TX_RENDERTILE, node->clip->x0 >> 3, node->clip->y0 >> 3,
                                   node->clip->x1 >> 3, node->clip->y1 >> 3);
                } else if (node->rect != NULL) {
                    gDPSetTileSize(gfx++, G_TX_RENDERTILE, node->rect->x0 >> 3, node->rect->y0 >> 3,
                                   node->rect->x1 >> 3, node->rect->y1 >> 3);
                }
                if ((set->lv[lod].flags & 4) || (node->flags & 0x2000)) {
                    alpha = node->prim[3];
                    if ((node->flags & 0x200000) && D_80124EE8 > 24.0f) {
                        if (D_80124EE8 > 80.0f) {
                            alpha = 0;
                        } else {
                            alpha = alpha * (80.0f - D_80124EE8) / 56.0f;
                        }
                    }
                    gDPSetPrimColor(gfx++, 0, 0, node->prim[0], node->prim[1], node->prim[2], alpha);
                }
                if (node->flags & 0x4000) {
                    gDPSetEnvColor(gfx++, node->env[0], node->env[1], node->env[2], node->env[3]);
                }
                gSPDisplayList(gfx++, dl);
                if (D_8017A638 == 0) {
                    gDPSetTextureLUT(gfx++, G_TT_RGBA16);
                    D_8017A638 = 1;
                }
                if (set->lv[lod].flags & 2) {
                    D_8017A4B0 = 0;
                    if (pal != NULL) {
                        func_80099B30(&gfx, pal);
                    }
                }
                drawn = 1;
                if (node->flags & 0x80000) {
                    gDPPipeSync(gfx++);
                    gDPSetCycleType(gfx++, G_CYC_2CYCLE);
                    gSPSetGeometryMode(gfx++, G_FOG);
                    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                    gSPClearGeometryMode(gfx++, G_TEXTURE_GEN);
                    node->clip = NULL;
                }
                if (set->lv[lod].flags & 0x10) {
                    gSPClearGeometryMode(gfx++, G_LIGHTING);
                }
            }
        }
    }
    *gp = gfx;
    return drawn;
}
