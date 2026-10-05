#define gDPSetTileSizeS(pkt, t, uls, ult, lrs, lrt) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (G_SETTILESIZE << 24) | (((uls) & 0xFFF) << 12) | ((ult) & 0xFFF); \
    _g->words.w1 = (((t) & 7) << 24) | (((lrs) & 0xFFF) << 12) | ((lrt) & 0xFFF); \
}
extern s32 D_8017A4B0;

void func_80099B30(Gfx **gfxp, Palette *tex) {
    Gfx *gfx = *gfxp;

    if (D_8017A4B0 != tex->table || D_80161430 != 0) {
        D_8017A4B0 = tex->table;
        gDPLoadTLUT(gfx++, tex->last - tex->first + 1, 0x100 + tex->first, tex->table);
        *gfxp = gfx;
    }
}

#define TILE(fmt, sz) gDPLoadTextureTile(gfx++, tex->image, fmt, G_IM_SIZ_##sz, tex->width, tex->height, rect->x0 >> 5, rect->y0 >> 5, rect->x1 >> 5, rect->y1 >> 5, 0, cms, cmt, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)
#define TILE4(fmt) gDPLoadTextureTile_4b(gfx++, tex->image, fmt, tex->width, tex->height, rect->x0 >> 5, rect->y0 >> 5, rect->x1 >> 5, rect->y1 >> 5, 0, cms, cmt, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)
#define BLOCK(fmt, sz) gDPLoadTextureBlock(gfx++, tex->image, fmt, G_IM_SIZ_##sz, tex->width, tex->height, 0, cms, cmt, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)
#define BLOCK4(fmt) gDPLoadTextureBlock_4b(gfx++, tex->image, fmt, tex->width, tex->height, 0, cms, cmt, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)

RDL_STORAGE void render_display_list(Gfx **gp, Texture *tex, TexRect *rect, TexRect *clip, Palette *palettes)
{
    u16 masks, maskt;
    u16 cms, cmt;
    Gfx *gfx = *gp;

    if (tex == NULL) {
        return;
    }
    if (!(tex->flags & TEX_TEXTURE)) {
        gSPDisplayList(gfx++, tex->image);
    } else {
        if (rect != NULL) {
            masks = func_80087804(rect->x0 - rect->x1) - 5;
            maskt = func_80087804(rect->y0 - rect->y1) - 5;
        } else {
            masks = func_80087804(tex->width);
            maskt = func_80087804(tex->height);
        }
        if (tex->flags & TEX_CLAMP_S) {
            cms = G_TX_CLAMP;
            masks = 0;
        } else {
            cms = G_TX_WRAP;
        }
        cms = (tex->flags & TEX_MIRROR_S) ? cms | G_TX_MIRROR : cms;
        if (tex->flags & TEX_CLAMP_T) {
            cmt = G_TX_CLAMP;
            maskt = 0;
        } else {
            cmt = G_TX_WRAP;
        }
        cmt = (tex->flags & TEX_MIRROR_T) ? cmt | G_TX_MIRROR : cmt;
        if (tex->fmt == G_IM_FMT_RGBA) {
            if (D_8017A638 != 0) {
                gDPSetTextureLUT(gfx++, G_TT_NONE);
                D_8017A638 = 0;
            }
            if (tex->siz == G_IM_SIZ_16b) {
                if (rect != NULL) {
                    TILE(G_IM_FMT_RGBA, 16b);
                } else {
                    BLOCK(G_IM_FMT_RGBA, 16b);
                }
            } else {
                if (rect != NULL) {
                    TILE(G_IM_FMT_RGBA, 32b);
                } else {
                    BLOCK(G_IM_FMT_RGBA, 32b);
                }
            }
        } else if (tex->fmt == G_IM_FMT_CI) {
            if (D_8017A638 != 1) {
                gDPSetTextureLUT(gfx++, G_TT_RGBA16);
                D_8017A638 = 1;
            }
            if (tex->siz == G_IM_SIZ_8b) {
                if (rect != NULL) {
                    TILE(G_IM_FMT_CI, 8b);
                } else {
                    BLOCK(G_IM_FMT_CI, 8b);
                }
            } else {
                if (rect != NULL) {
                    TILE4(G_IM_FMT_CI);
                } else {
                    BLOCK4(G_IM_FMT_CI);
                }
            }
        } else if (tex->fmt == G_IM_FMT_I) {
            if (D_8017A638 != 0) {
                gDPSetTextureLUT(gfx++, G_TT_NONE);
                D_8017A638 = 0;
            }
            if (tex->siz == G_IM_SIZ_8b) {
                if (rect != NULL) {
                    TILE(G_IM_FMT_I, 8b);
                } else {
                    BLOCK(G_IM_FMT_I, 8b);
                }
            } else {
                if (rect != NULL) {
                    TILE4(G_IM_FMT_I);
                } else {
                    BLOCK4(G_IM_FMT_I);
                }
            }
        } else if (tex->fmt == G_IM_FMT_IA) {
            if (D_8017A638 != 0) {
                gDPSetTextureLUT(gfx++, G_TT_NONE);
                D_8017A638 = 0;
            }
            if (tex->siz == G_IM_SIZ_16b) {
                if (rect != NULL) {
                    TILE(G_IM_FMT_IA, 16b);
                } else {
                    BLOCK(G_IM_FMT_IA, 16b);
                }
            } else if (tex->siz == G_IM_SIZ_8b) {
                if (rect != NULL) {
                    TILE(G_IM_FMT_IA, 8b);
                } else {
                    BLOCK(G_IM_FMT_IA, 8b);
                }
            } else {
                if (rect != NULL) {
                    TILE4(G_IM_FMT_IA);
                } else {
                    BLOCK4(G_IM_FMT_IA);
                }
            }
        }
    }
    if (clip != NULL) {
        gfx--;
        gDPSetTileSizeS(gfx++, G_TX_RENDERTILE, clip->x0 >> 3, clip->y0 >> 3, clip->x1 >> 3, clip->y1 >> 3);
    } else if (rect != NULL) {
        gfx--;
        gDPSetTileSizeS(gfx++, G_TX_RENDERTILE, rect->x0 >> 3, rect->y0 >> 3, rect->x1 >> 3, rect->y1 >> 3);
    }
    if (tex->palette >= 0 && !(tex->flags & TEX_NOPALETTE) && palettes != NULL) {
        func_80099B30(&gfx, &palettes[tex->palette]);
    }
    *gp = gfx;
}
