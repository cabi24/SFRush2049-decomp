
typedef struct {
    Vp *vp;          /* 0x00 */
    u16 *scissor;    /* 0x04 */
    s32 ortho;       /* 0x08 */
    u8 pad0C[4];
    f32 fov;         /* 0x10 */
    u8 pad14[0x10];
    f32 w;           /* 0x24 */
    f32 h;           /* 0x28 */
    f32 x;           /* 0x2C */
    f32 y;           /* 0x30 */
    f32 aspect;      /* 0x34 */
    f32 near;        /* 0x38 */
    f32 far;         /* 0x3C */
    u16 fogmin;      /* 0x40 */
    u16 fogmax;      /* 0x42 */
    u8 fog[4];       /* 0x44 */
} Camera; /* 0x48 */

typedef struct {
    f32 pos[3];
    s16 tc[2];
    u8 cn[4];
} PolyVtx; /* 20 bytes */

typedef struct {
    s16 count;       /* 0x00 */
    u16 flags;       /* 0x02 */
    u16 tex;         /* 0x04 */
    s16 z;           /* 0x06 */
    union {
        PolyVtx v[4];
        struct {
            f32 x0, y0;      /* 0x08 */
            u8 pad10[8];
            u8 col[4];       /* 0x18 */
            f32 x1, y1;      /* 0x1C */
        } rect;
    } u;
} Poly; /* 0x58 */

extern Gfx *D_801497C8;
extern Camera D_8017A510[];
extern s8 D_80151AD8;
extern s8 D_80140A04;
extern f32 D_801613B8;
extern MtxF D_801613F0;
extern s16 D_8014A108;
extern s16 D_8015B254;
extern s32 D_801613B4;
extern Poly D_8015B268[];
extern Vtx *D_80161434;

void physics_float_calc(s32 view, f32 *pos, s32 arg2);
void *memcpy(void *, const void *, unsigned int);
void math_utility(void *arg0, void *arg1);
void func_8009EA68(f32 angle, f32 uv[][3]);
void func_8009E9D8(f32 *m);
void func_8009E8B4(f32 matrix[4][4], u32 *fixed);
void func_8009E820(f32 *arg0, f32 *arg1, f32 *arg2);
void guOrtho(Mtx *m, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f, f32 scale);
void guPerspective(Mtx *m, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
void guLookAtF(f32 mf[4][4], f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp,
               f32 zUp);
void guMtxIdent(Mtx *m);

void func_8009F058(s32 view, s32 arg1, s32 arg2, View *eye)
{
    Vp *vp = &D_8015B250->vp[view];
    Camera *cam;
    Poly *poly;
    u16 ulx, uly, lrx, lry;
    s32 mode;
    s32 fog;
    s32 depth;
    s32 i;

    physics_float_calc(view, eye->pos, 0);
    cam = &D_8017A510[view];
    memcpy(vp, cam->vp, sizeof(Vp));
    vp->vp.vscale[0] = (s16)cam->w * 2;
    vp->vp.vscale[1] = (s16)cam->h * 2;
    vp->vp.vtrans[0] = (s16)cam->x * 4;
    vp->vp.vtrans[1] = (s16)cam->y * 4;
    ulx = cam->scissor[0];
    uly = cam->scissor[1];
    lrx = cam->scissor[2];
    lry = cam->scissor[3];
    gSPViewport(D_801497C8++, &D_8015B250->vp[view]);
    gDPPipeSync(D_801497C8++);
    gDPSetScissor(D_801497C8++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
    gDPSetFogColor(D_801497C8++, cam->fog[0], cam->fog[1], cam->fog[2], cam->fog[3]);
    gSPFogPosition(D_801497C8++, cam->fogmin, cam->fogmax);
    gDPSetCycleType(D_801497C8++, G_CYC_2CYCLE);
    gSPSetGeometryMode(D_801497C8++, G_FOG);
    depth = -1;
    gDPSetDepthSource(D_801497C8++, G_ZS_PIXEL);
    if (D_8017A638 != 1) {
        gDPSetTextureLUT(D_801497C8++, G_TT_RGBA16);
        D_8017A638 = 1;
    }
    if (cam->ortho) {
        s32 pad1;

        guOrtho(&D_8015B250->proj[view], -cam->w * 0.5f, cam->w * 0.5f, -cam->h * 0.5f, cam->h * 0.5f,
                cam->near * 16.0f, cam->far * 16.0f, 1.0f);
    } else {
        u16 perspNorm;

        guPerspective(&D_8015B250->proj[view], &perspNorm, cam->fov * 57.295776f, cam->aspect, cam->near * 16.0f,
                      cam->far * 16.0f, 1.0f);
        gSPPerspNormalize(D_801497C8++, perspNorm);
    }
    gSPMatrix(D_801497C8++, &D_8015B250->proj[view], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    if (D_80151AD8) {
        f32 uv[3][3];

        math_utility(&D_80150B70[view], uv);
        func_8009EA68(3.1415927f, uv);
        if (D_80140A04) {
            uv[2][0] *= -1.0f;
            uv[1][0] *= -1.0f;
        }
        guLookAtF(D_801613F0, 0.0f, 0.0f, 0.0f, -uv[2][0], uv[2][1], uv[2][2], -uv[1][0], uv[1][1], uv[1][2]);
    } else if (D_80140A04) {
        guLookAtF(D_801613F0, 0.0f, 0.0f, 0.0f, -(-D_80150B70[view].rot[2][0]), D_80150B70[view].rot[2][1],
                  D_80150B70[view].rot[2][2], -(-D_80150B70[view].rot[1][0]), D_80150B70[view].rot[1][1],
                  D_80150B70[view].rot[1][2]);
    } else {
        guLookAtF(D_801613F0, 0.0f, 0.0f, 0.0f, -D_80150B70[view].rot[2][0], D_80150B70[view].rot[2][1],
                  D_80150B70[view].rot[2][2], -D_80150B70[view].rot[1][0], D_80150B70[view].rot[1][1],
                  D_80150B70[view].rot[1][2]);
    }
    guLookAtF(D_80124EF0, D_80150B70[view].rot[2][0], D_80150B70[view].rot[2][1], D_80150B70[view].rot[2][2], 0.0f,
              0.0f, 0.0f, D_80150B70[view].rot[1][0], D_80150B70[view].rot[1][1], D_80150B70[view].rot[1][2]);
    if (D_801613B8 != 1) {
        s32 j;

        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) D_80124EF0[i][j] *= D_801613B8;
        }
    }
    func_8009E9D8(D_80124F30[0]);
    i = D_8014A108;
    if ((i < 2) != 0) {
        D_8015B250->vtx[view].v.ob[0] = D_80124EF0[0][2] * 480.0f;
        D_8015B250->vtx[view].v.ob[1] = D_80124EF0[1][2] * 480.0f;
        D_8015B250->vtx[view].v.ob[2] = D_80124EF0[2][2] * 480.0f;
    } else {
        D_8015B250->vtx[view].v.ob[0] = 0;
        D_8015B250->vtx[view].v.ob[1] = 0;
        D_8015B250->vtx[view].v.ob[2] = 0;
    }
    func_8009E8B4(D_801613F0, (u32 *)&D_8015B250->look[view]);
    gSPMatrix(D_801497C8++, &D_8015B250->look[view], G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    guMtxIdent(&D_8015B250->ident);
    if (!D_80140A04) {
        D_8015B250->ident.m[0][0] = 0xFFFF0000;
    }
    gSPMatrix(D_801497C8++, &D_8015B250->ident, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    D_80161430 = NULL;
    D_80124FC4 = 0;
    D_80124FCC = -1;
    D_80124FE0 = 0;
    D_80124FE2 = -1;
    if (D_8015B254 >= 0) {
        track_collision_wall(&D_801497C8, NULL, &D_8012E700[D_8015B254], view, 0, 1, 0, NULL);
        D_8017A638 = -1;
        D_8017A508 = NULL;
    }
    fog = 1;
    poly = D_8015B268;
    mode = -1;
    for (i = 0; i < D_801613B4; i++, poly++) {
        s32 j;

        if (poly->flags & 0x8000) {
            continue;
        }
        if (!(poly->flags & (1 << view))) {
            continue;
        }
        if (poly->count == 0) {
            continue;
        }
        gDPPipeSync(D_801497C8++);
        if (poly->flags & 0x2000) {
            if (depth < 0) {
                gDPSetDepthSource(D_801497C8++, G_ZS_PRIM);
                if (depth != poly->z) {
                    gDPSetPrimDepth(D_801497C8++, poly->z, 0);
                }
                depth = poly->z;
            }
        } else if (depth >= 0) {
            depth = -1;
            gDPSetDepthSource(D_801497C8++, G_ZS_PIXEL);
        }
        if (poly->flags & 0x4000) {
            if (mode != 0) {
                mode = 0;
                gDPSetRenderMode(D_801497C8++, 0x00504240, 0);
                gDPSetCombine(D_801497C8++, 0xFFFFFF, 0xFFFDF6FB);
            }
            gDPSetPrimColor(D_801497C8++, 0, 0, poly->u.rect.col[0], poly->u.rect.col[1], poly->u.rect.col[2],
                            poly->u.rect.col[3]);
            gDPFillRectangle(D_801497C8++, (s32)poly->u.rect.x0, (s32)poly->u.rect.y0, (s32)poly->u.rect.x1,
                             (s32)poly->u.rect.y1);
            gDPPipeSync(D_801497C8++);
            continue;
        }
        for (j = 0; j < poly->count; j++) {
            f32 p[3];

            if (!(poly->flags & 0x10)) {
                p[0] = poly->u.v[j].pos[0] - D_80150B70[view].pos[0] * 16.0f;
                p[1] = poly->u.v[j].pos[1] - D_80150B70[view].pos[1] * 16.0f;
                p[2] = poly->u.v[j].pos[2] - D_80150B70[view].pos[2] * 16.0f;
                if (p[0] <= -32768.0f || 32768.0f <= p[0] || p[1] <= -32768.0f || 32768.0f <= p[1] ||
                    p[2] <= -32768.0f || 32768.0f <= p[2]) {
                    goto skip;
                }
            } else {
                func_8009E820(poly->u.v[j].pos, p, (f32 *)&D_80150B70[view]);
            }
            D_80161434[j].v.ob[0] = p[0];
            D_80161434[j].v.ob[1] = p[1];
            D_80161434[j].v.ob[2] = p[2];
            memcpy((j + D_80161434)->v.tc, poly->u.v[j].tc, 8);
        }
        if ((poly->flags & 0x100) && fog) {
            fog = 0;
            mode = -1;
            gSPClearGeometryMode(D_801497C8++, G_FOG);
            gDPPipeSync(D_801497C8++);
            gDPSetCycleType(D_801497C8++, G_CYC_1CYCLE);
        } else if (!(poly->flags & 0x100) && !fog) {
            fog = 1;
            mode = -1;
            gSPSetGeometryMode(D_801497C8++, G_FOG);
            gDPPipeSync(D_801497C8++);
            gDPSetCycleType(D_801497C8++, G_CYC_2CYCLE);
        }
        if (fog) {
            gDPSetPrimColor(D_801497C8++, 0, 0, poly->u.rect.col[0], poly->u.rect.col[1], poly->u.rect.col[2],
                            poly->u.rect.col[3]);
        }
        if (poly->flags & 0x400) {
            Texture *tex;

            tex = &D_80151AE8[poly->tex >> 10].tex[poly->tex & 0x3FF];
            if (tex != D_8017A508) {
                render_display_list(&D_801497C8, &D_80151AE8[poly->tex >> 10].tex[poly->tex & 0x3FF], NULL, NULL,
                                    D_80138670[poly->tex >> 10].pal);
                D_8017A508 = tex;
            }
            if (poly->flags & 0x200) {
                if (mode != 2) {
                    mode = 2;
                    if (!fog) {
                        gDPSetRenderMode(D_801497C8++, 0x00504A70, 0);
                        gDPSetCombine(D_801497C8++, 0x121824, 0xFF33FFFF);
                    } else {
                        gDPSetRenderMode(D_801497C8++, 0xC8104A50, 0);
                        gDPSetCombine(D_801497C8++, 0x1217FF, 0xFFFFFE38);
                    }
                }
            } else if (mode != 3) {
                mode = 3;
                if (!fog) {
                    gDPSetRenderMode(D_801497C8++, 0x00504240, 0);
                    gDPSetCombine(D_801497C8++, 0x121824, 0xFF33FFFF);
                } else {
                    gDPSetRenderMode(D_801497C8++, 0xC8104240, 0);
                    gDPSetCombine(D_801497C8++, 0x1217FF, 0xFFFFFE38);
                }
            }
        } else if (mode != 1) {
            mode = 1;
            if (!fog) {
                gDPSetRenderMode(D_801497C8++, 0x00504A50, 0);
                gDPSetCombine(D_801497C8++, 0xFFFFFF, 0xFFFE793C);
            } else {
                gDPSetRenderMode(D_801497C8++, 0xC8104A70, 0);
                gDPSetCombine(D_801497C8++, 0xFFFFFF, 0xFFFE7638);
            }
        }
        gSPVertex(D_801497C8++, D_80161434, poly->count, 0);
        D_80161434 += poly->count;
        if (poly->flags & 0x1000) {
            s32 pad2[2];

            gSP2Triangles(D_801497C8++, 0, 2, 1, 0, 2, 0, 1, 0);
            if (poly->count >= 4) {
                gSP2Triangles(D_801497C8++, 2, 0, 3, 0, 0, 2, 3, 0);
            }
        } else if (D_80140A04) {
            if (poly->count == 3) {
                gSP1Triangle(D_801497C8++, 1, 2, 0, 0);
            } else {
                gSP2Triangles(D_801497C8++, 1, 0, 2, 0, 0, 3, 2, 0);
            }
        } else {
            if (poly->count == 3) {
                gSP1Triangle(D_801497C8++, 2, 0, 1, 0);
            } else {
                gSP2Triangles(D_801497C8++, 2, 0, 1, 0, 0, 2, 3, 0);
            }
        }
    skip:;
    }
    if (D_8017A638 != 1) {
        gDPSetTextureLUT(D_801497C8++, G_TT_RGBA16);
        D_8017A638 = 1;
    }
    if (depth >= 0) {
        gDPSetDepthSource(D_801497C8++, G_ZS_PIXEL);
    }
    if (!fog) {
        gSPSetGeometryMode(D_801497C8++, G_FOG);
        gDPPipeSync(D_801497C8++);
        gDPSetCycleType(D_801497C8++, G_CYC_2CYCLE);
    }
    if (D_80124FCC >= 0) {
        D_80124FC4 = 1;
        D_80124FE0 = 0;
        track_collision_wall(&D_801497C8, NULL, &D_8012E700[D_80124FCC], view, 0, 1, 0, NULL);
        D_8017A638 = -1;
        D_8017A508 = NULL;
    }
    if (D_80124FE2 >= 0) {
        D_80124FC4 = 0;
        D_80124FE0 = 1;
        track_collision_wall(&D_801497C8, NULL, &D_8012E700[D_80124FE2], view, 0, 1, 0, NULL);
        D_8017A638 = -1;
        D_8017A508 = NULL;
    }
}
