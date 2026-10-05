
s32 track_collision_wall(Gfx **gp, Node *parent, Node *node, s32 view, s32 count, s32 more_in, s32 flags6,
                         Palette *pal)
{
    Gfx *gfx = *gp;
    s32 more;
    s32 pushed = 0, total = 0, saved = 0;
    s32 i, j;
    s32 n;
    Vtx *vtx;
    View *v;
    f32 x, z, r;
    s32 pad[4];

    for (;;) {
        if (node->flags & 0x80000000) {
            goto next;
        }
        if (node->next >= 0 || more_in) {
            more = 1;
        } else {
            more = 0;
        }
        if (D_80124FC4 == 0 && (node->flags & 0x40000)) {
            if (D_80124FCC < 0) {
                D_80124FCC = node - D_8012E700;
            }
            goto next;
        }
        if (D_80124FE0 == 0 && (node->flags & 0x800000)) {
            if (D_80124FE2 < 0) {
                D_80124FE2 = node - D_8012E700;
            }
            goto next;
        }
        if (!track_collision_edge(node, view)) {
            goto next;
        }
        if (node->child >= 0 || !(node->flags & (0x100 << view))) {
            if (node->flags & 0x1000) {
                gSPVertex(gfx++, &D_8015B250->vtx[view], 1, 8);
            }
            if (node->flags & 0x80) {
                more = 0;
            } else {
                D_80124F78[0] = node->obj->pos[0];
                D_80124F78[1] = node->obj->pos[1];
                D_80124F78[2] = node->obj->pos[2];
                if (node->flags & 0x10000) {
                    if (node->scale != 1.0f) {
                        for (i = 0; i < 3; i++) {
                            for (j = 0; j < 3; j++) {
                                D_801403D8[i][j] = D_80124EF0[i][j] * node->scale;
                            }
                        }
                        for (j = 0; j < 3; j++) {
                            D_801403D8[3][j] = D_80124EF0[3][j];
                            D_801403D8[j][3] = D_80124EF0[j][3];
                        }
                        if (!func_8009D99C(view, D_801403D8, node->obj->pos, D_8015B260, node->w, count)) {
                            goto next;
                        }
                    } else if (!func_8009D99C(view, D_80124EF0, node->obj->pos, D_8015B260, node->w, count)) {
                        goto next;
                    }
                } else if (node->flags & 0x8000) {
                    if (!func_8009D99C(view, D_80124F30, node->obj->pos, D_8015B260, node->w, count)) {
                        goto next;
                    }
                } else if (node->flags & 0x400000) {
                    func_8009D708(view, node->obj, D_8015B260, node->w, count);
                } else if (!func_8009D45C(view, node->obj, D_8015B260, node->w, count)) {
                    goto next;
                }
                gSPMatrix(gfx++, D_8015B260, more);
                D_8015B260++;
            }
            if (node->mode >= 0) {
                switch (node->mode) {
                    case 0:
                        gSPSetLights0(gfx++, (*node->light));
                        break;
                    case 1:
                        gSPSetLights1(gfx++, (*node->light));
                        break;
                    case 2:
                        gSPSetLights2(gfx++, (*node->light));
                        break;
                }
            }
            if ((node->flags & 0x10) || (node->flags & 0x20)) {
                D_80161430 = gfx + 2;
                gSPDisplayList(gfx++, D_80161430);
                gSPBranchList(gfx++, NULL);
                pushed = 1;
                saved = total;
                total = 0;
                if (node->flags & 0x10) {
                    gSPVertex(gfx++, D_8011EF10, 8, 0);
                } else {
                    gSPVertex(gfx++, D_8011EF90, 8, 0);
                }
                gSPCullDisplayList(gfx++, 0, 7);
            } else if (node->lights & 7) {
                n = node->lights & 7;
                vtx = &D_80157248[(node->lights & 0x1FF8) >> 3];
                if (!(node->flags & 0x400000) && D_80151AA0 < 2000.0f) {
                    v = &D_80150B70[view];
                    x = (vtx[0].v.ob[0] < vtx[7].v.ob[0]) ? vtx[7].v.ob[0] : vtx[0].v.ob[0];
                    z = (vtx[0].v.ob[2] < vtx[7].v.ob[2]) ? vtx[7].v.ob[2] : vtx[0].v.ob[2];
                    r = sqrtf(x * x + z * z) * 0.0625f;
                    x = node->obj->pos[0] - v->pos[0];
                    z = node->obj->pos[2] - v->pos[2];
                    if (D_80151AA0 < sqrtf(x * x + z * z) - r) {
                        goto children;
                    }
                }
                D_80161430 = gfx + 2;
                gSPDisplayList(gfx++, D_80161430);
                gSPBranchList(gfx++, NULL);
                D_8017A4B0 = 0;
                pushed = 1;
                D_8017A508 = NULL;
                saved = total;
                gSPVertex(gfx++, vtx, n + 1, 0);
                gSPCullDisplayList(gfx++, 0, n);
                total = 0;
            }
            total += particle_system(&gfx, parent, count, flags6, node, view, (node->pal != NULL) ? node->pal : pal);
        } else {
            more = 0;
        }
    children:
        if (node->child >= 0) {
            total += track_collision_wall(&gfx, node, &D_8012E700[node->child], view,
                                          (more == 1) ? count + 1 : count, node->next >= 0,
                                          (node->flags & 8) | flags6, (node->pal != NULL) ? node->pal : pal);
        }
        if (pushed) {
            pushed = 0;
            if (total == 0) {
                gfx = D_80161430 - 2;
            } else {
                gSPEndDisplayList(gfx++);
                D_80161430[-1].words.w1 = (u32)gfx;
            }
            D_80161430 = NULL;
            total += saved;
            saved = 0;
        }
        if (more == 1) {
            if (total == 0) {
                gfx--;
            } else {
                gSPPopMatrix(gfx++, G_MTX_MODELVIEW);
            }
        }
    next:
        if (node->next < 0) {
            break;
        }
        node = &D_8012E700[node->next];
        if (D_80124FC4 != 0 && !(node->flags & 0x40000)) {
            goto next;
        }
        if (D_80124FE0 != 0 && !(node->flags & 0x800000)) {
            goto next;
        }
    }
    *gp = gfx;
    return total;
}
