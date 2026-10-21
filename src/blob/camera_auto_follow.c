/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * camera_auto_follow (0x800BE078, 1084 B) = MAT3 TB_text.c TB_DrawTextScrollBox (GC 0x8004fd1c): draws `text`
 * word-wrapped into a box, starting at `page`/`line` (the historical label is wrong). Lane w36v, wave 36.
 * Direct typed port of the MAT3 body; callees (N64 names): object_bytes_sum_global = TB_GetFontLineHeight,
 * camera_zoom_fov = TB_GetTextNumLines (locked), camera_shake_update = HAL_GetCharWidth, object_manager_update =
 * HAL_GetTextWidth, menu_input_process = HAL_DrawText; func_800BDEB0 = HAL_GetFontSpacing and func_800B70EC =
 * HAL_SetTextPos (N64 version keeps the -32768 "unchanged" sentinel) are the locked kept getters, inlined by -O3
 * and therefore defined here verbatim (same precedent as camera_zoom_fov.c's func_800BDEB0).
 * Data: D_80118E20 = gJust {h, v}; D_80149B70 gSpacing; D_80149D92/D_80149D9E gFontXpos/gFontYpos.
 * N64 differences from the GC C (all from the retail words, no devices):
 *  - `tw` is a function-scope s32 declared first (its slot is the frame's top word; GC has a block `short tw`),
 *    and the justified x is passed straight to HAL_SetTextPos as one conditional expression (GC: `xpos` if/else);
 *    the if/else forms colour the two arms the other way round.
 *  - `str = (u8 *)s + charSize` (GC `(unsigned char *)(s + charSize)`) gives retail's addu operand order.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct { s32 h, v; } Just;
extern Just D_80118E20;  /* gJust */
extern s8 D_80149B70;    /* gSpacing */
extern s16 D_80149D92;   /* gFontXpos */
extern s16 D_80149D9E;   /* gFontYpos */
extern s16 object_bytes_sum_global(void);          /* TB_GetFontLineHeight */
extern s16 camera_zoom_fov(s16 width, void *text);  /* TB_GetTextNumLines */
extern u32 camera_shake_update(u16 ch);             /* HAL_GetCharWidth */
extern u32 object_manager_update(void *text, s16 numChars);  /* HAL_GetTextWidth */
extern void menu_input_process(void *text, s16 numChars);    /* HAL_DrawText */

s8 func_800BDEB0(void) {
    return D_80149B70;
}

void func_800B70EC(s16 x, s16 y) {
    if (x != -32768) D_80149D92 = x;
    if (y != -32768) D_80149D9E = y;
}

void camera_auto_follow(s16 x, s16 y, s16 width, s16 height, s16 page, s16 line, void *text)
{
    s32 tw;
    s16 top, bottom, fontHeight, spacing, charSize, numLines, currentLine, currentPage, len, overlap, currWidth, ch;
    const u8 *s, *lastSpace;
    u8 *str;
    u8 saveChar;
    s8 dbl;

    fontHeight = object_bytes_sum_global();
    spacing = func_800BDEB0();
    top = y;
    if (D_80118E20.v != 3) {
        numLines = camera_zoom_fov(width, text);
        if (D_80118E20.v == 1) top = y - (numLines * fontHeight) / 2;
        else top = y - numLines * fontHeight;
    }
    str = text;
    bottom = top + height;
    dbl = *str == 255;
    if (dbl) {
        str++;
        charSize = 2;
    } else charSize = 1;
    currentLine = 0;
    if (page < 0) {
        currentPage = page;
        bottom += fontHeight;
    } else currentPage = 0;
    overlap = currentPage < page ? fontHeight : (s16)0;
    do {
        s = str;
        len = 0;
        currWidth = 0;
        lastSpace = 0;
        for (;;) {
            if (dbl) ch = (s[0] << 8) | s[1];
            else ch = *s;
            if (ch == ' ' && currWidth == 0) {
                str += charSize;
                s = str;
                continue;
            }
            if (!ch || ch == 13 || ch == 10) break;
            currWidth += camera_shake_update(ch);
            if (ch != ' ') currWidth += spacing;
            else if (lastSpace != s - charSize) lastSpace = s;
            if (currWidth > width && lastSpace) {
                if (dbl) len -= (s - lastSpace) >> 1;
                else len -= s - lastSpace;
                s = lastSpace;
                break;
            }
            s += charSize;
            len++;
        }
        if (currentPage >= page) {
            if (currentLine >= line) {
                if (len > 0) {
                    if (dbl) {
                        saveChar = str[-1];
                        str[-1] = 255;
                    }
                    tw = object_manager_update(str - (dbl ? 1 : 0), len);
                    func_800B70EC((D_80118E20.h == 1) ? x - (tw >> 1) : (D_80118E20.h == 2) ? x - tw : x, top);
                    menu_input_process(str - (dbl ? 1 : 0), len);
                    if (dbl) str[-1] = saveChar;
                }
                top += fontHeight;
            }
            currentLine++;
        } else top += fontHeight;
        if (!ch) break;
        str = (u8 *)s + charSize;
        if (top > bottom - fontHeight - overlap) {
            currentPage++;
            top = y;
            if (currentPage > page) break;
            if (currentPage == page) overlap = 0;
        }
    } while (1);
}
