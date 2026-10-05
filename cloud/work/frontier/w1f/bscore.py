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
                return (9999, 9999, 0, src.name + ' COMPILE FAIL', 0)
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
        lead = next((i for i in range(min(len(gm), len(wm))) if gm[i] != wm[i]), min(len(gm), len(wm)))
        return (max(len(want), len(got)) - ex, strict, len(got) - len(want), src.name + (' unverified=%d' % len(unverified) if unverified else '') + ' lead=%d' % lead, -lead)
    except Exception as e:
        return (9998, 9998, 0, src.name + ' ERR ' + repr(e)[:80], 0)

def main():
    ap = argparse.ArgumentParser(); ap.add_argument('dir'); ap.add_argument('fn')
    ap.add_argument('--flags', default='-g0 -O2 -mips2 -G 0 -non_shared'); ap.add_argument('--top', type=int, default=15); ap.add_argument('--keep', default=''); ap.add_argument('--lead', action='store_true')
    a = ap.parse_args()
    want = score.targets()[a.fn]; syms = score.image_symbols()
    files = sorted(Path(a.dir).glob('*.c'))
    with ThreadPoolExecutor(2) as ex:
        res = list(ex.map(lambda f: one(f, a.fn, a.flags, want, syms, [k for k in a.keep.split(',') if k]), files))
    res.sort(key=(lambda r: (r[4], r[0], r[1])) if a.lead else None)
    for r in res[:a.top]:
        print('aligned-missing %3d  strict %3d  size %+d  %s' % r[:4])
    print('%d files' % len(res))
main()
