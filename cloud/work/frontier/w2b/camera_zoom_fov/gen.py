import sys
HDR = '''typedef signed char s8;
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

    w = 0;
    n = 0;
    wide = (*text == 0xFF);
    if (wide) {
        start = text + 1;
        step = 2;
    } else {
        start = text;
        step = 1;
    }
    gap = func_800BDEB0();
    lines = 0;
    space = 0;
'''
INNER = '''            if (wide) {
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
'''
V = {}
V['f'] = HDR + '''    do {
        p = start;
        for (;;) {
''' + INNER + '''        }
        lines++;
        start = step + p;
        n = 0;
        w = 0;
        space = 0;
    } while (ch != 0 && lines != 0x7FFF);
    return lines;
}
'''
V['g'] = HDR + '''    for (;;) {
        p = start;
        for (;;) {
''' + INNER + '''        }
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
'''
V['h'] = HDR + '''    do {
        p = start;
        while (1) {
''' + INNER + '''        }
        lines++;
        start = step + p;
        if (ch != 0 && lines != 0x7FFF) {
            n = 0;
            w = 0;
            space = 0;
        }
    } while (ch != 0 && lines != 0x7FFF);
    return lines;
}
'''
V['i'] = HDR + '''loop:
        p = start;
        for (;;) {
''' + INNER + '''        }
        lines++;
        start = step + p;
        if (ch != 0 && lines != 0x7FFF) {
            n = 0;
            w = 0;
            space = 0;
            goto loop;
        }
    return lines;
}
'''
import os
os.makedirs('camera_zoom_fov/v', exist_ok=True)
for k, v in V.items():
    open('camera_zoom_fov/v/%s.c' % k, 'w').write(v)
