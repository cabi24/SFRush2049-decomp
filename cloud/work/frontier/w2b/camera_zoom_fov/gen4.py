import itertools, os, shutil
PRE = '''typedef signed char s8;
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
'''
DECL = {
 'd0': '''    u8 *start;
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
''',
 'd1': '''    u8 *start;
    s16 lines;
    s32 step;
    u8 *p;
    s16 ch;
    s16 w = 0;
    s16 n = 0;
    s16 gap;
    u8 *space;
    s8 wide;

''',
 'd2': '''    u8 *start;
    s16 lines = 0;
    s16 w = 0;
    s16 n = 0;
    s16 ch;
    s16 gap;
    u8 *space = 0;
    u8 *p;
    s32 step;
    s8 wide;

''',
}
HEAD = '''    wide = (*text == 0xFF);
    if (wide) {
        start = text + 1;
        step = 2;
    } else {
        start = text;
        step = 1;
    }
    gap = func_800BDEB0();
'''
INIT2 = {'d0': '    lines = 0;\n    space = 0;\n', 'd1': '    lines = 0;\n    space = 0;\n', 'd2': ''}
GETCH = {
 'g0': '''            if (wide) {
                ch = (p[0] << 8) | p[1];
            } else {
                ch = *p;
            }
''',
}
SKIP = {
 's0': '''            if (ch == ' ' && w == 0) {
                start += step;
                p = start;
                continue;
            }
''',
 's1': '''            if (ch == ' ' && w == 0) {
                start += step;
                p = start;
            } else {
''',
}
TERM = {
 't0': '''            if (ch == 0 || ch == 13 || ch == 10) {
                break;
            }
''',
 't1': '''            if (ch == 0) {
                break;
            }
            if (ch == 13) {
                break;
            }
            if (ch == 10) {
                break;
            }
''',
}
BODY = '''            w += camera_shake_update(ch);
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
'''
OUT = {
 'o_f': ('    do {\n        p = start;\n', '''        lines++;
        start = step + p;
        n = 0;
        w = 0;
        space = 0;
    } while (ch != 0 && lines != 0x7FFF);
    return lines;
}
'''),
 'o_g': ('    for (;;) {\n        p = start;\n', '''        lines++;
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
'''),
 'o_g2': ('    for (;;) {\n        p = start;\n', '''        lines++;
        start = step + p;
        if (ch == 0) {
            break;
        }
        if (lines == 0x7FFF) {
            break;
        }
        n = 0;
        w = 0;
        space = 0;
    }
    return lines;
}
'''),
 'o_h': ('    do {\n        p = start;\n', '''        lines++;
        start = step + p;
        if (ch != 0 && lines != 0x7FFF) {
            n = 0;
            w = 0;
            space = 0;
        }
    } while (ch != 0 && lines != 0x7FFF);
    return lines;
}
'''),
 'o_r': ('    for (;;) {\n        p = start;\n', '''        lines++;
        start = step + p;
        if (ch == 0 || lines == 0x7FFF) {
            return lines;
        }
        n = 0;
        w = 0;
        space = 0;
    }
}
'''),
 'o_w': ('    while (1) {\n        p = start;\n', '''        lines++;
        start = step + p;
        if (ch != 0 && lines != 0x7FFF) {
            n = 0;
            w = 0;
            space = 0;
            continue;
        }
        break;
    }
    return lines;
}
'''),
}
INNER = {'i_for': '        for (;;) {\n', 'i_wh': '        while (1) {\n'}
d = 'camera_zoom_fov/v4'
shutil.rmtree(d, ignore_errors=True); os.makedirs(d)
for dk, sk, tk, ok, ik in itertools.product(DECL, SKIP, TERM, OUT, INNER):
    body = BODY
    close = '        }\n'
    if sk == 's1':
        body = TERM[tk].replace('            ', '                ') + BODY.replace('            ', '                ').replace('\n                ', '\n                ')
        body = '\n'.join(('    ' + l if l.strip() and not l.startswith('                ') else l) for l in (TERM[tk] + BODY).split('\n'))
        src = PRE + DECL[dk] + HEAD + INIT2[dk] + OUT[ok][0] + INNER[ik] + GETCH['g0'] + SKIP[sk] + body + '            }\n' + close + OUT[ok][1]
    else:
        src = PRE + DECL[dk] + HEAD + INIT2[dk] + OUT[ok][0] + INNER[ik] + GETCH['g0'] + SKIP[sk] + TERM[tk] + BODY + close + OUT[ok][1]
    open('%s/%s_%s_%s_%s_%s.c' % (d, dk, sk, tk, ok, ik), 'w').write(src)
