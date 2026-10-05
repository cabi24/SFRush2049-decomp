#!/usr/bin/env python3
"""adiff.py SRC FN [--flags ...] [--keep a,b] [-c N]: (builder side) compile, relocate, print an aligned
diff (difflib) of disassembled retail vs candidate words, plus strict/aligned counts."""
import sys, argparse, tempfile, difflib, json, subprocess
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score

def dis(words):
    import struct
    with tempfile.TemporaryDirectory() as t:
        p = Path(t) / 'w.bin'
        p.write_bytes(b''.join(struct.pack('>I', w) for w in words))
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-mmips:4000', '-EB', str(p)], capture_output=True, text=True).stdout
    res = []
    for line in out.splitlines():
        parts = line.split('\t')
        if len(parts) >= 3 and parts[0].strip().endswith(':'):
            res.append(' '.join(parts[2:]).strip())
    return res

ap = argparse.ArgumentParser(); ap.add_argument('src'); ap.add_argument('fn')
ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared'); ap.add_argument('--keep', default=''); ap.add_argument('-c', type=int, default=2)
ap.add_argument('--full', action='store_true')
a = ap.parse_args()
want = score.targets()[a.fn]; syms = score.image_symbols()
with tempfile.TemporaryDirectory() as t:
    obj = Path(t) / 'o.o'
    if a.keep:
        g = Path(t) / 'g'; g.mkdir()
        (g / 'group.c').write_text(Path(a.src).read_text())
        (g / 'group.json').write_text(json.dumps({'files': ['group.c'], 'keep': a.keep.split(','), 'flags': a.flags}))
        score.compile_group(g, obj)
    else:
        score.compile_single(a.src, a.flags, obj)
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[a.fn]
    end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, syms)
    got = res[start // 4:end // 4]
while got and got[-1] == 0 and len(got) > len(want): got.pop()
gm = [g & masks.get(start + 4 * i, 0xFFFFFFFF) for i, g in enumerate(got)]
wm = [w & masks.get(start + 4 * i, 0xFFFFFFFF) for i, w in enumerate(want)]
strict = sum(1 for i in range(max(len(gm), len(wm))) if i >= len(gm) or i >= len(wm) or gm[i] != wm[i])
dw = dis(want); dg = dis(got)
import re
def norm(s): return re.sub(r'0x[0-9a-f]+ <.*', 'L', s) if re.match(r'(b|j)', s) else s
nw = [norm(x) for x in dw]; ng = [norm(x) for x in dg]
sm = difflib.SequenceMatcher(None, nw, ng, autojunk=False)
ex = sum(b.size for b in sm.get_matching_blocks())
if a.full:
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            for k in range(i1, i2): print('    %03x  %s' % (k * 4, dw[k]))
        else:
            for k in range(i1, i2): print('  - %03x  %s' % (k * 4, dw[k]))
            for k in range(j1, j2): print('  + %03x  %s' % (k * 4, dg[k]))
else:
    for grp in sm.get_grouped_opcodes(a.c):
        print('@@ want +0x%x  got +0x%x' % (grp[0][1] * 4, grp[0][3] * 4))
        for tag, i1, i2, j1, j2 in grp:
            if tag == 'equal':
                for k in range(i1, i2): print('    %03x  %s' % (k * 4, dw[k]))
            else:
                for k in range(i1, i2): print('  - %03x  %s' % (k * 4, dw[k]))
                for k in range(j1, j2): print('  + %03x  %s' % (k * 4, dg[k]))
print('strict %d  aligned-missing %d  size %+d  unverified %d unresolved %d' % (strict, max(len(want), len(got)) - ex, len(got) - len(want), len(unverified), len(unresolved)))
