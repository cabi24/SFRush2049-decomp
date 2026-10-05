#!/usr/bin/env python3
"""permute.py ORDER OUTDIR: write copies of the parts with render_display_list's parameters in ORDER
(a permutation of 'tex,rect,clip,gp,pals'), rewriting the definition, prototype and every call."""
import sys, re, os, shutil
order = sys.argv[1].split(','); out = sys.argv[2]
base = ['tex', 'rect', 'clip', 'gp', 'pals']
decl = {'tex': 'Texture *tex', 'rect': 'TexRect *rect', 'clip': 'TexRect *clip', 'gp': 'Gfx **gp', 'pals': 'Palette *palettes'}
os.makedirs(out, exist_ok=True)
def split_args(s):
    args, depth, cur = [], 0, ''
    for ch in s:
        if ch == ',' and depth == 0: args.append(cur.strip()); cur = ''; continue
        if ch in '([': depth += 1
        if ch in ')]': depth -= 1
        cur += ch
    args.append(cur.strip()); return args
for f in os.listdir('.'):
    if not (f.startswith('part_') or f in ('build.sh', 'standin.c')): continue
    if os.path.isdir(f): continue
    s = open(f).read()
    s = s.replace('void render_display_list(Texture *tex, TexRect *rect, TexRect *clip, Gfx **gp, Palette *palettes)',
                  'void render_display_list(' + ', '.join(decl[k] for k in order) + ')')
    res, i = '', 0
    for m in re.finditer(r'render_display_list\(', s):
        if m.start() < i: continue
        j = m.end(); depth = 1
        while depth: 
            depth += {'(': 1, ')': -1}.get(s[j], 0); j += 1
        inner = s[m.end():j - 1]
        if re.match(r'\s*(Texture|TexRect|Gfx|Palette) \*', inner): continue
        a = dict(zip(base, split_args(inner)))
        res += s[i:m.end()] + ', '.join(a[k] for k in order) + ')'; i = j
    res += s[i:]
    open(os.path.join(out, f), 'w').write(res)
