#!/usr/bin/env python3
"""gfull.py GROUPDIR FN[,FN...] [--all] [--sum]: (builder) compile the -O3 group and print instruction-aligned diffs."""
import sys, argparse, tempfile, difflib, struct, subprocess, re
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score

def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack('>%dI' % len(words), *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-z', '-b', 'binary', '-m', 'mips:4300', '-EB', f.name],
                             capture_output=True, text=True).stdout
    r = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m: r.append(re.sub(r'\s+', ' ', m.group(3)))
    return r

ap = argparse.ArgumentParser(); ap.add_argument('dir'); ap.add_argument('fns')
ap.add_argument('--all', action='store_true'); ap.add_argument('--sum', action='store_true')
a = ap.parse_args()
T = score.targets(); syms = score.image_symbols()
with tempfile.TemporaryDirectory() as t:
    obj = Path(t) / 'o.o'
    score.compile_group(Path(a.dir), obj)
    words = score.text_words(obj); fns = score.symbols(obj)
    for fn in a.fns.split(','):
        want = T[fn]
        if fn not in fns: print(fn, 'NOT EMITTED'); continue
        start = fns[fn]
        end = min((o for o in fns.values() if o > start), default=len(words) * 4)
        res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, syms)
        got = res[start // 4:end // 4]
        while got and got[-1] == 0 and len(got) > len(want): got.pop()
        sm = difflib.SequenceMatcher(None, want, got, autojunk=False)
        rows = []
        for tag, i1, i2, j1, j2 in sm.get_opcodes():
            if tag == 'equal':
                for k in range(i2 - i1): rows.append((' ', i1 + k, i1 + k, j1 + k))
            else:
                n = max(i2 - i1, j2 - j1)
                for k in range(n):
                    rows.append(('|', i1 + k if i1 + k < i2 else None, i1 + k if i1 + k < i2 else None, j1 + k if j1 + k < j2 else None))
        nd = sum(1 for r in rows if r[0] == '|')
        strict = sum(1 for i in range(max(len(got), len(want))) if i >= len(got) or i >= len(want) or got[i] != want[i])
        if not a.sum:
            dw, dg = dis(want), dis(got)
            show = [a.all or r[0] == '|' for r in rows]
            for i, r in enumerate(rows):
                if show[i] or (i > 0 and show[i - 1]) or (i + 1 < len(rows) and show[i + 1]):
                    print('%s %5s  %-34s %s' % (r[0], ('+%03x' % (r[1] * 4)) if r[1] is not None else '', dw[r[2]] if r[2] is not None else '', dg[r[3]] if r[3] is not None else ''))
        print('%s: want %d got %d; aligned-diff rows %d; strict diff %d; unverified %d unresolved %d' % (fn, len(want), len(got), nd, strict, len(unverified), len(unresolved)))
