#!/usr/bin/env python3
"""gbatch_remote.py DIR FN: (builder) DIR holds group dirs (group.json + files); compile each, print aligned-missing/strict for FN."""
import sys, tempfile, difflib
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
d, fn = sys.argv[1], sys.argv[2]
want = score.targets()[fn]; syms = score.image_symbols()
out = []
for g in sorted(Path(d).iterdir()):
    if not (g / 'group.json').exists(): continue
    try:
        with tempfile.TemporaryDirectory() as t:
            obj = Path(t) / 'o.o'
            score.compile_group(g, obj)
            words = score.text_words(obj); fns = score.symbols(obj)
            start = fns[fn]; end = min((o for o in fns.values() if o > start), default=len(words) * 4)
            res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, syms)
            got = res[start // 4:end // 4]
        while got and got[-1] == 0 and len(got) > len(want): got.pop()
        strict = sum(1 for i in range(max(len(got), len(want))) if i >= len(got) or i >= len(want) or (got[i] & masks.get(start + 4 * i, 0xFFFFFFFF)) != (want[i] & masks.get(start + 4 * i, 0xFFFFFFFF)))
        sm = difflib.SequenceMatcher(None, want, got, autojunk=False)
        ex = sum(b.size for b in sm.get_matching_blocks())
        out.append((max(len(want), len(got)) - ex, strict, g.name))
    except BaseException as e:
        out.append((9999, 9999, g.name + ' ' + repr(e)[:60]))
for r in sorted(out): print('aligned-missing %3d strict %3d %s' % r)
