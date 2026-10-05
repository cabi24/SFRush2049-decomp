#!/usr/bin/env python3
"""gen.py OUTDIR: choice-point generator for object_manager_update (combined single-file groups)."""
import sys, itertools
from pathlib import Path
from comb import comb
HEAD = open('k2.c').read().split('s32 object_manager_update(')[0]
def build(o):
    L = []
    decl = ['s32 width', 's32 pos', o['third'], 's32 ch'] + [d for d in ['s32 wide', 's32 step', 's32 prev', 'u8 *s'] if d != o['third']]
    if o['font']: decl.append('FontHdr *font')
    if o['g']: decl.append('Glyph *g')
    F = 'font' if o['font'] else 'D_801497F0'
    G = 'g' if o['g'] else '(&D_80149800[ch])'
    L.append('s32 object_manager_update(u8 *str, s16 maxlen)\n{')
    L += ['    %s;' % d for d in decl]
    L.append('''
    sound_update_channel(0);
    %s
    if (str[0] == 255) {
        s = str + 1;
        wide = 1;
        step = 2;
    } else {
        s = str;
        wide = 0;
        step = 1;
    }''' % ['pos = 0;\n    width = 0;', 'width = 0;\n    pos = 0;', 'pos = width = 0;', 'width = pos = 0;'][o['zi']])
    L.append('    str = 0;\n    prev = -1;' if o['init'] == 0 else '    prev = -1;\n    str = 0;')
    nz = ' != 0' if o['nz'] else ''
    L.append('    while ((maxlen < 0 || (s32) str < maxlen) && (s[pos]%s || (wide && s[pos + 1]%s))) {' % (nz, nz))
    if o['font']: L.append('        font = D_801497F0;')
    if o['mono'] == 0: L.append('        if (%s->monospace) {\n            width += %s->fixedWidth + D_80149B70;' % (F, F))
    else: L.append('        if (%s->monospace) {\n            width += %s->fixedWidth;\n            width += D_80149B70;' % (F, F))
    L.append('            prev = ch;\n        } else {')
    if o['fetch'] == 0: L.append('            if (wide) {\n                ch = (s[pos] << 8) | s[pos + 1];\n            } else {\n                ch = s[pos];\n            }')
    else: L.append('            ch = wide ? (s[pos] << 8) | s[pos + 1] : s[pos];')
    L.append('            if (ch == 32 || ch %s) {\n                width += %s->spaceWidth;' % (['>= 256', '> 255'][o['cmp']], F))
    L.append(['                ch = -1;\n                prev = -1;', '                prev = ch = -1;', '                ch = -1;\n                prev = ch;'][o['neg']])
    L.append('            } else {\n                if (ch == 10) {\n                    break;\n                }\n                ch = D_80149878[ch];\n                if (ch < 0) {\n                    width += %s->spaceWidth;\n                    prev = ch;\n                } else {' % F)
    if o['g']: L.append('                    g = &D_80149800[ch];')
    L.append(['                    width += {G}->right - {G}->left + D_80149B70 + 1;',
              '                    width += {G}->right - {G}->left + D_80149B70;\n                    width++;',
              '                    width += {G}->right - {G}->left;\n                    width += D_80149B70 + 1;',
              '                    width += {G}->right;\n                    width -= {G}->left;\n                    width += D_80149B70 + 1;',
              '                    width += {G}->right - {G}->left + 1 + D_80149B70;',
              '                    width += D_80149B70 + {G}->right - {G}->left + 1;'][o['gl']].replace('{G}', G))
    L.append('                    if (prev > 0 && %s->kerning) {\n                        width -= %s->kern[prev];\n                    }\n                    prev = ch;\n                }\n            }\n        }' % (F, G))
    L.append('        pos += step;\n        str++;\n    }')
    L.append(['    return width - D_80149B70;', '    width -= D_80149B70;\n    return width;'][o['ret']])
    L.append('}\n')
    return HEAD + '\n'.join(L)
space = dict(zi=[0, 1, 2, 3], third=['s32 wide', 's32 step', 's32 prev', 'u8 *s'], font=[0, 1], g=[0, 1], init=[0, 1], nz=[0, 1], mono=[0, 1], fetch=[0, 1], cmp=[0, 1], neg=[0, 1, 2], gl=[0, 1, 2, 3, 4, 5], ret=[0, 1])
if __name__ == '__main__':
    out = Path(sys.argv[1]); out.mkdir(exist_ok=True)
    fixed = dict(a.split('=') for a in sys.argv[2:])
    keys = list(space)
    n = 0
    for vals in itertools.product(*[[v for v in space[k] if k not in fixed or str(v) == fixed[k]] for k in keys]):
        o = dict(zip(keys, vals))
        name = '_'.join('%s%s' % (k[:2], keys and space[k].index(o[k])) for k in keys)
        (out / (name + '.c')).write_text(comb(build(o))); n += 1
    print(n, 'variants')
