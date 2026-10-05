
/* ---- Input_ProcessGameplayPad (0x800A04C4): per-frame scene pass + 2D sprite list ---- */
#define G_MDSFT_TEXTFILT 12
#define G_TF_POINT (0 << G_MDSFT_TEXTFILT)
#define gDPSetTextureFilter(pkt, type) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTFILT, 2, type)
typedef struct {
    Palette *pal;   /* 0x00 palette (textured) or u8 colour (fill) */
    Texture *tex;   /* 0x04 texture, or a callback when flags & 0x80 */
    u16 model;      /* 0x08 >>10 palette table */
    s16 x, y;       /* 0x0A */
    u16 mode;       /* 0x0E */
    s16 w, h;       /* 0x10 */
    u8 color;       /* 0x14 */
    u8 flags;       /* 0x15 */
    s8 hidden;      /* 0x16 */
    u8 pad17;
    s16 t0, s0, t1, s1; /* 0x18 */
} Sprite;
extern s32 D_8002AFC0;
extern s32 D_8002AFC4;
extern Lights2 *D_801406B8;
extern s16 D_80140618;
extern volatile s8 D_8015F72D;
extern Vtx D_80161498[][3200];
extern u8 D_80146204;
extern s32 D_801613AC;
extern Sprite D_80140BF0[];
extern u8 D_8011ED08[4];
extern u16 D_8011ED0C[];
void func_8008705C(u32 mask);
void func_800878E0(u32 mask);
void func_8008A148(u8 *img, s32 mode, s32 pal, s32 bank);
void func_8008A38C(u16 arg0);
void func_8008A3E4(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8008A46C(s32 x0, s32 y0, s32 x1, s32 y1, u8 *color);
void func_8008A644(unsigned short arg0);
void object_render(void *img, u16 type, u16 siz, u16 width, u16 height, u16 uls, u16 ult, u16 lrs, u16 lrt,
                   u16 pal, s32 tile);
void func_80087110(s32 x, s32 y, s32 right, s32 bottom, s32 s, s32 t);

void Input_ProcessGameplayPad(s32 arg0)
{
    s32 i;

    D_8017A4B0 = 0;
    D_8017A638 = -1;
    D_8017A508 = NULL;
    gSPClearGeometryMode(D_801497C8++, G_LIGHTING | G_TEXTURE_GEN);
    D_80124F84 = 0;
    if (D_80140618 >= 0) {
        switch (D_80140618) {
        case 0:
            gSPSetLights0(D_801497C8++, (*D_801406B8));
            break;
        case 1:
            gSPSetLights1(D_801497C8++, (*D_801406B8));
            break;
        case 2:
            gSPSetLights2(D_801497C8++, (*D_801406B8));
            break;
        }
    }
    D_80161434 = D_80161498[D_8015F72D];
    for (i = 0; i < D_80146204; i++) {
        func_8009F058(i, &D_80150B70[i]);
    }
    gSPClearGeometryMode(D_801497C8++, G_FOG);
    gDPPipeSync(D_801497C8++);
    gDPSetScissor(D_801497C8++, G_SC_NON_INTERLACE, 0, 0, D_8002AFC0, D_8002AFC4);
    gDPSetDepthSource(D_801497C8++, G_ZS_PIXEL);
    gDPSetCycleType(D_801497C8++, G_CYC_1CYCLE);
    gDPSetTextureFilter(D_801497C8++, G_TF_POINT);
    if (arg0 != 0) {
        return;
    }
    if (D_8002AFC4 > 240) {
        func_800878E0(0x8000);
    }
    for (i = 0; i < D_801613AC; i++) {
        Sprite *e;
        Texture *tex;
        s32 width;
        s32 h;
        s32 sw;
        s32 th;
        s32 lines;
        s32 x;
        s32 y;
        s32 tile;
        s32 t;
        s32 next;
        s32 flip;
        s32 x1;

        e = &D_80140BF0[i];
        tile = 0;
        tex = e->tex;
        if (e->hidden) {
            continue;
        }
        if (e->flags & 0x80) {
            if (e->tex != NULL) {
                ((void (*)(Palette *))e->tex)(e->pal);
            }
            continue;
        }
        if (e->mode != 0) {
            func_8008A644(e->mode);
        } else {
            func_8008705C(16);
        }
        if (e->flags & 4) {
            func_800878E0(4);
        } else {
            func_8008705C(4);
        }
        if (e->w == 0 || e->h == 0) {
            continue;
        }
        if (tex == NULL) {
            if (e->color == 255) {
                func_8008705C(32);
            } else {
                func_8008A38C(e->color);
            }
            x = e->x;
            y = e->y;
            flip = e->x + e->w;
            lines = e->y + e->h;
            func_8008A3E4(x, y, flip - 1, lines - 1);
            func_8008A46C(x, y, flip, lines, (u8 *)e->pal);
            continue;
        }
        x = e->x;
        y = e->y;
        lines = e->h;
        if (D_8002AFC4 > 240) {
            lines = lines + lines;
        }
        func_8008A3E4(x, y, x + e->w - 1, y + lines - 1);
        sw = e->s1 - e->s0 + 1;
        th = e->t1 - e->t0 + 1;
        flip = tex->width;
        lines = D_8011ED0C[flip >> (4 - tex->siz)];
        if (tex->fmt == 2 || tex->fmt == 5) {
            lines >>= 1;
        }
        if (lines < 2 || ((15 >> tex->siz) & flip)) {
            tile = 1;
            if (tex->siz == 0) {
                lines = 4096 / tex->width;
            } else if (tex->siz == 1 || tex->siz == 2) {
                lines = 2048 / tex->width;
            } else {
                lines = 1024 / tex->width;
            }
        }
        flip = 0;
        if (e->flags & 8) {
            func_8008705C(8);
        } else {
            flip = 1;
            func_800878E0(8);
        }
        if (e->flags & 1) {
            func_800878E0(1);
        }
        if (e->color == 255) {
            func_8008705C(32);
        } else {
            func_8008A38C(e->color);
        }
        if (e->pal != NULL) {
            func_8008A148((u8 *)e->pal->table, tex->fmt, tex->siz, 0);
        } else if (tex->palette >= 0) {
            func_8008A148((u8 *)D_80138670[e->model >> 10].pal[tex->palette].table, tex->fmt, tex->siz, 0);
        } else if (tex->fmt == 3) {
            D_8011ED08[3] = e->color;
            func_8008A148(D_8011ED08, tex->fmt, tex->siz, 0);
        } else if (tex->fmt != 0) {
            if (D_80138670[e->model >> 10].count != 0) {
                func_8008A148((u8 *)D_80138670[e->model >> 10].pal->table, tex->fmt, tex->siz, 0);
            }
        }
        if (e->t1 >= tex->height) {
            e->t1 = tex->height - 1;
        }
        if (e->t0 < 0) {
            t = 0;
        } else {
            t = e->t0;
        }
        if (th < lines) {
            lines = th;
        }
        if (flip) {
            while (t < e->t1) {
                next = t + lines;
                if (e->t1 < next) {
                    lines = e->t1 - t + 1;
                    next = t + lines;
                }
                object_render(tex->image, tex->fmt, tex->siz, tex->width, tex->height, 0,
                              tex->height - t - lines, sw - 1, tex->height - t - 1, 0, tile);
                h = y + lines;
                func_80087110(x, y, x + sw - 1, h - 1, e->s0, tex->height - t - lines);
                if (D_8002AFC4 > 240) {
                    h += lines;
                }
                t = next;
                y = h;
            }
        } else {
            while (t < e->t1) {
                next = t + lines;
                if (e->t1 < next) {
                    lines = e->t1 - t + 1;
                    next = t + lines;
                }
                object_render(tex->image, tex->fmt, tex->siz, tex->width, tex->height, 0, t, sw - 1, next - 1,
                              0, tile);
                h = y + lines;
                func_80087110(x, y, x + sw - 1, h - 1, e->s0, t);
                if (D_8002AFC4 > 240) {
                    h += lines;
                }
                t = next;
                y = h;
            }
        }
        if (e->flags & 1) {
            func_8008705C(1);
        }
    }
}
