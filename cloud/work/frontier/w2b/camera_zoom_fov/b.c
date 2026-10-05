typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;

extern s8 D_80149B70;
s32 camera_shake_update(u16 ch);

s16 camera_zoom_fov(s16 width, u8 *text) {
    u8 *start;
    s16 lines;
    s16 w = 0;
    s16 n = 0;
    s32 step;
    u8 *p;
    s16 ch;
    u8 *space;
    s16 gap;
    s8 wide;

    wide = (*text == 0xFF);
    if (wide) {
        start = text + 1;
        step = 2;
    } else {
        start = text;
        step = 1;
    }
    gap = (s16)D_80149B70;
    lines = 0;
    space = 0;
    for (;;) {
        p = start;
        for (;;) {
            if (wide) {
                ch = p[1] | (p[0] << 8);
            } else {
                ch = *p;
            }
            if (ch == ' ' && w == 0) {
                start += step;
                p = start;
                continue;
            }
            if (ch == 0 || ch == 13 || ch == 10) {
                break;
            }
            w += camera_shake_update(ch);
            if (ch != ' ') {
                w += gap;
            } else if (space != p - step) {
                space = p;
            }
            n++;
            p += step;
            if (width < w && space != 0) {
                p = space;
                break;
            }
        }
        lines++;
        start = step + p;
        if (ch == 0 || lines == 0x7FFF) {
            break;
        }
        n = 0;
        w = 0;
        space = 0;
    }
    return lines;
}
