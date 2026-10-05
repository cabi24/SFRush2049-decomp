#!/usr/bin/env python3
"""bscore.py DIR FN [--flags ...] [--top N]: (runs on the builder, inside the agent copy)
compile every DIR/*.c, print per file: strict differing words, aligned-missing words (LCS), size delta.
Sorted by (aligned-missing, strict). Strict MATCH must still be confirmed with tools/cloud/score.py."""
import sys, argparse, tempfile, difflib, os
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, 'tools/cloud')
import score

def one(src, fn, flags, want, syms, keep=None):
    try:
        with tempfile.TemporaryDirectory() as t:
            obj = Path(t) / 'o.o'
            try:
                if keep:
                    import json
                    g = Path(t) / 'g'; g.mkdir()
                    (g / 'group.c').write_text(src.read_text())
                    (g / 'group.json').write_text(json.dumps({'files': ['group.c'], 'keep': keep, 'flags': flags}))
                    score.compile_group(g, obj)
                else:
                    score.compile_single(str(src), flags, obj)
            except SystemExit as e:
                return (9999, 9999, 9999, 0, src.name + ' COMPILE FAIL')
            words = score.text_words(obj); fns = score.symbols(obj)
            start = fns[fn]
            end = min((o for o in fns.values() if o > start), default=len(words) * 4)
            res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, syms)
            got = res[start // 4:end // 4]
        while got and got[-1] == 0 and len(got) > len(want): got.pop()
        gm = [g & masks.get(start + 4 * i, 0xFFFFFFFF) for i, g in enumerate(got)]
        wm = [w & masks.get(start + 4 * i, 0xFFFFFFFF) for i, w in enumerate(want)]
        strict = sum(1 for i in range(max(len(gm), len(wm))) if i >= len(gm) or i >= len(wm) or gm[i] != wm[i])
        sm = difflib.SequenceMatcher(None, want, got, autojunk=False)
        ex = sum(b.size for b in sm.get_matching_blocks())
        def key(x):
            op = x >> 26
            if op == 0: return (0, x & 0x3f)
            if op == 1: return (1, (x >> 16) & 31)
            if op in (0x10, 0x11): return (op, (x >> 21) & 31, x & 0x3f if (x >> 21) & 16 else 0)
            return (op,)
        kw = [key(x) for x in want]; kg = [key(x) for x in got]
        sm2 = difflib.SequenceMatcher(None, kw, kg, autojunk=False)
        ex2 = sum(b.size for b in sm2.get_matching_blocks())
        return (max(len(want), len(got)) - ex2, max(len(want), len(got)) - ex, strict, len(got) - len(want), src.name + (' unverified=%d' % len(unverified) if unverified else ''))
    except Exception as e:
        return (9998, 9998, 9998, 0, src.name + ' ERR ' + repr(e)[:80])

def main():
    ap = argparse.ArgumentParser(); ap.add_argument('dir'); ap.add_argument('fn')
    ap.add_argument('--flags', default='-g0 -O2 -mips2 -G 0 -non_shared'); ap.add_argument('--top', type=int, default=15); ap.add_argument('--keep', default='')
    a = ap.parse_args()
    want = score.targets()[a.fn]; syms = score.image_symbols()
    files = sorted(Path(a.dir).glob('*.c'))
    with ThreadPoolExecutor(4) as ex:
        res = list(ex.map(lambda f: one(f, a.fn, a.flags, want, syms, [k for k in a.keep.split(',') if k]), files))
    res.sort()
    for r in res[:a.top]:
        print('mnem-missing %3d  aligned-missing %3d  strict %3d  size %+d  %s' % r)
    print('%d files' % len(res))
main()
