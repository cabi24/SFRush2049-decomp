/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct FontHdr {
    /* 0x0 */ u8 pad0;
    /* 0x1 */ u8 mode;
    /* 0x2 */ u8 f2;
    /* 0x3 */ u8 f3;
    /* 0x4 */ u8 f4;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u8 fixedWidth;
    /* 0x7 */ u8 spaceWidth;
    /* 0x8 */ u8 f8;
    /* 0x9 */ u8 monospace;
    /* 0xA */ u8 kerning;
    /* 0xB */ u8 f11;
    /* 0xC */ u8 count;
} FontHdr;

typedef struct Glyph {
    /* 0x0 */ u8 *kern;
    /* 0x4 */ u8 id;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u8 left;
    /* 0x7 */ u8 pad7;
    /* 0x8 */ u8 right;
    /* 0x9 */ u8 pad9[3];
} Glyph;

extern FontHdr *D_801497F0;
extern Glyph *D_80149800;
extern s16 D_80149878[256];
extern s8 D_80149B70;

void sound_update_channel(s32 force);

s32 object_manager_update(u8 *str, s16 maxlen)
{
    s32 width;
    s32 pos;
    s32 wide;
    s32 ch;
    s32 step;
    s32 prev;
    u8 *s;

    sound_update_channel(0);
    pos = 0;
    width = 0;
    if (str[0] == 255) {
        s = str + 1;
        wide = 1;
        step = 2;
    } else {
        s = str;
        wide = 0;
        step = 1;
    }
    str = 0;
    prev = -1;
    while ((maxlen < 0 || (s32) str < maxlen) && (s[pos] != 0 || (wide && s[pos + 1] != 0))) {
        if (D_801497F0->monospace) {
            width += D_801497F0->fixedWidth + D_80149B70;
            prev = ch;
        } else {
            if (wide) {
                ch = (s[pos] << 8) | s[pos + 1];
            } else {
                ch = s[pos];
            }
            if (width * D_80149B70) {}
            if (ch == 32 || ch >= 256) {
                width += D_801497F0->spaceWidth;
                ch = -1;
                prev = -1;
            } else {
                if (ch == 10) {
                    break;
                }
                ch = D_80149878[ch];
                if (ch < 0) {
                    width += D_801497F0->spaceWidth;
                    prev = ch;
                } else {
                    width += D_80149800[ch].right - D_80149800[ch].left + D_80149B70 + 1;
                    if (prev > 0 && D_801497F0->kerning) {
                        width -= D_80149800[ch].kern[prev];
                    }
                    prev = ch;
                }
            }
        }
        pos += step;
        str++;
    }
    return width - D_80149B70;
}
