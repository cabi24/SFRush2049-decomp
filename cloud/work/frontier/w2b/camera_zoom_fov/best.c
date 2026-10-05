/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * Historical label camera_zoom_fov is misleading: this counts the lines a
 * string occupies when word-wrapped to `width` pixels. A leading 0xFF byte
 * selects 16-bit big-endian characters (step 2). Per line it skips leading
 * spaces, adds each glyph's width (camera_shake_update is the glyph-width
 * lookup) plus the inter-character gap for non-space characters, remembers the
 * last space that starts a run, and breaks the line at that space once the
 * width is exceeded. CR, LF and NUL end a line; NUL or 0x7FFF lines end the
 * count. No arcade ancestor identified (N64 front-end text code).
 *
 * func_800BDEB0 (the gap getter, locked at 0x800BDEB0) is inlined by -O3 and
 * must be defined in the same file: `lb v0; sll s8,v0,16; sra; move` is the
 * inlined s8 return converted to the s16 local.
 *
 * Shaping notes:
 *  - n (characters on the line) is counted but never read; its increment is
 *    the last statement of the inner loop (after the break test).
 *  - `start = step + p` sits after the exit test; as1 hoists the addu above
 *    both branches, and its position relative to `li at,32767` follows the
 *    source line order (test line < assignment line).
 *  - QUIRK: the empty `if (start) {}` after the inner loop is code-free; it
 *    only lengthens start's live range so that uopt colours space (s3) before
 *    start (s4). It is probably the remains of a use of the line start in the
 *    drawing sibling (camera_auto_follow), not proven original source.
 *  - Three separate `break` tests (0, 13, 10) rather than one `||` chain keep
 *    13 and 10 out of callee-saved registers.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;

extern s8 D_80149B70;
s32 camera_shake_update(u16 ch);

s8 func_800BDEB0(void) {
    return D_80149B70;
}

s16 camera_zoom_fov(s16 width, u8 *text) {
    u8 *start;
    s16 lines;
    s32 step;
    u8 *p;
    s16 ch;
    s16 w;
    s16 n;
    s16 gap;
    u8 *space;
    s8 wide;

    wide = (*text == 0xFF);
    if (wide) {
        start = text + 1;
        step = 2;
    } else {
        start = text;
        step = 1;
    }
    gap = func_800BDEB0();
    w = 0;
    n = 0;
    lines = 0;
    space = 0;
    for (;;) {
        p = start;
        for (;;) {
            if (wide) {
                ch = (p[0] << 8) | p[1];
            } else {
                ch = *p;
            }
            if (ch == ' ' && w == 0) {
                start += step;
                p = start;
                continue;
            }
            if (ch == 0) {
                break;
            }
            if (ch == 13) {
                break;
            }
            if (ch == 10) {
                break;
            }
            w += camera_shake_update(ch);
            if (ch != ' ') {
                w += gap;
            } else if (space != p - step) {
                space = p;
            }
            p += step;
            if (width < w && space != 0) {
                p = space;
                break;
            }
            n++;
        }
        if (start) {
        }
        lines++;
        if (ch == 0 || lines == 0x7FFF) {
            break;
        }
        start = step + p;
        n = 0;
        w = 0;
        space = 0;
    }
    return lines;
}
