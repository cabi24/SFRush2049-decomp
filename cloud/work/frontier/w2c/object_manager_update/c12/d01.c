/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * object_manager_update (historical label): width in pixels of a text string in the current font.
 *   str[0] == 0xFF marks a 2-byte (wide) string; maxlen < 0 means unlimited. Monospace fonts add
 *   fixedWidth + D_80149B70 (letter spacing) per character; space or code >= 256 adds spaceWidth;
 *   '\n' stops; otherwise the glyph index D_80149878[ch] (-1 = missing -> spaceWidth) gives
 *   right - left + spacing + 1, minus the kerning byte glyph.kern[prev] when the font has kerning.
 *   Returns width - D_80149B70 (no spacing after the last character).
 * sound_update_channel (locked) refreshes the cached font bank first; it preserves t1/t4 (IPA), which is
 * why str lives in t1 across the call. Compile it only in the whole-program unit (blob_unit score).
 *
 * NOT A MATCH: 2 words (frontier wave 2, w2c). Residual: `li a3,-1` / `li v1,-1` emitted in the
 * opposite order in the space branch (prev before ch); everything else identical.
 * Shaping quirks (compile-affecting):
 *   - the parameter `str` is reused as the character counter (`str = 0; ... (s32) str < maxlen; str++`):
 *     retail keeps both in t1; a separate `s32 i` costs 13-19 words;
 *   - `prev = ch` in the monospace arm reads `ch` before any write (retail loads it from its stack home);
 *   - the dead read `if (width + D_80149B70 + pos + (s32)str) {}` in the space arm is a FAKE: it raises the
 *     colouring priority of width/pos/str/&D_80149B70 so that width takes a1 (not a2), wide t3, the
 *     spacing address t2 and pos t0, as in retail. Without it 16 words differ (a1/a2 swapped). Any
 *     second empty `if` anywhere collapses the body (constants 32/10/12 are no longer hoisted into
 *     s0/s1/s4: the function sits exactly at the register-pressure edge).
 */
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
    s32 ch;
    s32 prev;
    s32 width;
    s32 pos;
    s32 wide;
    s32 step;
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
            if (ch == 32 || ch >= 256) {
                width += D_801497F0->spaceWidth;
                if (width + D_80149B70 + pos + (s32)str) {}
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
