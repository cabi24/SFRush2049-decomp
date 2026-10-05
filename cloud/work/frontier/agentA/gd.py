#!/usr/bin/env python3
"""Aligned listing for one member of a group build: gd.py GROUP_DIR NAME"""
import sys, tempfile, difflib
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score as S
gdir, name = Path(sys.argv[1]), sys.argv[2]
with tempfile.TemporaryDirectory() as tmp:
    obj = Path(tmp) / 'out.o'
    S.compile_group(gdir, obj)
    want = S.targets()[name]
    words = S.text_words(obj)
    fns = S.symbols(obj)
    start = fns[name]
    end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    resolved, masks, unresolved, unverified, errors = S.relocate(obj, words, start, end, S.image_symbols())
    got = resolved[start // 4:end // 4]
    while got and got[-1] == 0 and len(got) > len(want):
        got.pop()
    a = [S.disasm_word(w) for w in want]
    b = [S.disasm_word(w) for w in got]
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    nd = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            for k in range(i2 - i1):
                print(f"  {4*(i1+k):03x} {a[i1+k]:34s}| {b[j1+k]}")
        else:
            n = max(i2 - i1, j2 - j1)
            nd += n
            for k in range(n):
                l = a[i1+k] if i1 + k < i2 else ''
                r = b[j1+k] if j1 + k < j2 else ''
                o = f"{4*(i1+k):03x}" if i1 + k < i2 else '   '
                print(f"! {o} {l:34s}| {r}")
    pos = sum(1 for i in range(max(len(want), len(got))) if i >= len(want) or i >= len(got) or want[i] != got[i])
    print(f"aligned-diff {nd}  positional {pos}  want {len(want)} got {len(got)}")
