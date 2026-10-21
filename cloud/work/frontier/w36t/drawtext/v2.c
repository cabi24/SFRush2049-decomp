/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* audio_doppler_calc = _DrawText (MAT3 GameSource/src/HAL_text.c static _DrawText, GC 0x8001B410).
 * Private IPA callee of HAL_FinishText (world_velocity_integrate): text in s2, length in s0.
 * N64: screen-size scissor, BlitColors(color, format, size, 0), the font texture page is loaded
 * (object_render) for page 0 and on every page change, BlitDraw has no texture argument and no +1s. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct anon_version_type_10 {
    unsigned char version, type, ascent, descent, leading, totalHeight, maxWidth, spaceWidth;
    unsigned char spacing, nonProp, kern, numTextures, numChars, pad16[3];
} anon_version_type_10;
typedef struct anon_kerns_value_c {
    unsigned char *kerns;
    unsigned char value, texIdx, uls, ult, lrs, lrt;
    signed short pad12;
} anon_kerns_value_c;
typedef struct anon_fore_4 { unsigned char rgba[4]; } anon_fore_4;
typedef struct tTexture {
    unsigned char pad00[16];
    unsigned short width;           /* 0x10 */
    unsigned short height;          /* 0x12 */
    unsigned char pad14[4];
    void *image;                    /* 0x18 */
} tTexture;

extern anon_version_type_10 *D_801497F0;    /* gFontHeader */
extern anon_kerns_value_c *D_80149800;      /* gFontLetters */
extern tTexture *D_80149820[16];            /* gFontTextures */
extern s16 D_80149878[256];                 /* gCharMap */
extern anon_fore_4 D_80149B48;              /* gFontColor */
extern s8 D_80149B60;                       /* gLeading */
extern s8 D_80149B70;                       /* gSpacing */
extern s32 D_80149B88;                      /* gFontAttribute */
extern s32 D_80149B08;                      /* font texture format */
extern s32 D_80149B28;                      /* font texture size */
extern s16 D_80149D92;                      /* gFontXpos */
extern s16 D_80149D9E;                      /* gFontYpos */
extern f32 D_80114748;                      /* gTrans */
extern s32 D_8002AFC0, D_8002AFC4;          /* screen width, height */
extern void *D_8012E6D0;                    /* current blit palette */
#define gFontHeader D_801497F0
#define gFontLetters D_80149800
#define gFontTextures D_80149820
#define gCharMap D_80149878
#define gFontColor D_80149B48
#define gLeading D_80149B60
#define gSpacing D_80149B70
#define gFontAttribute D_80149B88
#define gFontXpos D_80149D92
#define gFontYpos D_80149D9E
#define gTrans D_80114748

void sound_update_channel(s32 force);        /* _SetupFontData */
void func_8008A3E4(s32 ulx, s32 uly, s32 lrx, s32 lry);     /* BlitScissor */
void func_800878E0(u32 attribute);           /* BlitSetAttribute */
void func_8008705C(u32 attribute);           /* BlitClearAttribute */
void func_8008A148(void *colors, s32 fmt, s32 siz, s32 bank); /* BlitColors */
void object_render(void *img, u16 type, u16 siz, u16 width, u16 height, u16 uls, u16 ult, u16 lrs, u16 lrt,
                   u16 pal, s32 tile);       /* load a texture block */
void func_80087110(s32 x0, s32 y0, s32 x1, s32 y1, s32 s, s32 t); /* BlitDraw */

static void BlitClearColors(void)
{
    D_8012E6D0 = 0;
}

void audio_doppler_calc(const unsigned char *text, unsigned short length)
{
    int i;
    int x;
    int x0;
    int y0;
    int step;
    int ch;
    int chPrev;
    int lastPage;
    const unsigned char *t;
    int dbl;
    anon_kerns_value_c *l;
    sound_update_channel(0);
    func_8008A3E4(0, 0, D_8002AFC0 - 1, D_8002AFC4 - 1);
    func_800878E0(gFontAttribute);
    func_8008705C(~gFontAttribute);
    BlitClearColors();
    if (gFontAttribute & 32)
        gFontColor.rgba[3] = gTrans;
    func_8008A148(&gFontColor, D_80149B08, D_80149B28, 0);
    lastPage = 0;
    object_render(gFontTextures[0]->image, D_80149B08, D_80149B28, gFontTextures[0]->width, gFontTextures[0]->height, 0, 0, gFontTextures[0]->width - 1, gFontTextures[0]->height - 1, 0, 0);
    if (text[0] == 255) {
        t = text + 1;
        dbl = 1;
        step = 2;
        length -= 2;
    } else {
        t = text;
        dbl = 0;
        step = 1;
        length -= 1;
    }
    x0 = gFontXpos;
    y0 = gFontYpos;
    i = 0;
    chPrev = -1;
    for (; i < length;) {
        if (dbl)
            ch = (t[i] << 8) | t[i + 1];
        else
            ch = t[i];
        if (ch == 10) {
            ch = -1;
            y0 += gFontHeader->ascent + gFontHeader->descent + gLeading;
            x0 = gFontXpos;
        } else {
            if (ch == 32 || ch > 255) {
            space:
                x0 += gFontHeader->nonProp ? gFontHeader->maxWidth + gSpacing : gFontHeader->spaceWidth;
                ch = -1;
            } else {
                ch = gCharMap[ch];
                if (ch < 0)
                    goto space;
                if (chPrev > 0 && gFontHeader->kern && !gFontHeader->nonProp)
                    x0 -= gFontLetters[ch].kerns[chPrev];
                l = gFontLetters + ch;
                if (l->texIdx != lastPage) {
                    lastPage = l->texIdx;
                    object_render(gFontTextures[lastPage]->image, D_80149B08, D_80149B28, gFontTextures[lastPage]->width, gFontTextures[lastPage]->height, 0, 0, gFontTextures[lastPage]->width - 1, gFontTextures[lastPage]->height - 1, 0, 0);
                }
                x = gFontHeader->nonProp ? (gFontHeader->maxWidth - (l->lrs - l->uls + 1)) / 2 : 0;
                x = x0 + x;
                func_80087110(x, y0, x + l->lrs - l->uls, y0 + l->lrt - l->ult, l->uls, l->ult);
                x0 += (gFontHeader->nonProp ? gFontHeader->maxWidth : l->lrs - l->uls + 1) + gSpacing;
            }
        }
        i += step;
        chPrev = ch;
    }
}
