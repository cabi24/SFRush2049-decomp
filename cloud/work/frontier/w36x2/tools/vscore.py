#!/usr/bin/env python3
"""ON BUILDER (cwd = scratch): vscore.py FUNCS(comma) DIR... -> per dir, per function: aligned diff rows (difflib on
relocation-resolved words), positional differing words, or MATCH. Env TGT (default asm/us/ovl_a)."""
import sys, os, json, tempfile, difflib
sys.path.insert(0, 'tools/cloud')
import score
from pathlib import Path
score.ASM_DIR = Path(os.environ.get('TGT', 'asm/us/ovl_a')).resolve()
funcs = sys.argv[1].split(',')
T = score.targets(); IS = score.image_symbols()
for d in sys.argv[2:]:
    obj = Path(tempfile.mkdtemp()) / 'o.o'
    try:
        if (Path(d) / 'group.json').exists():
            score.compile_group(Path(d), obj)
        else:
            score.compile_single(str(Path(d) / 'cand.c'), os.environ.get('FLAGS', '-g0 -O3 -mips2 -G 0 -non_shared'), obj)
    except (Exception, SystemExit) as e:
        print(f'{d}\tERR {str(e)[:200]}'); continue
    words = score.text_words(obj); fn = score.symbols(obj)
    res = []
    for name in funcs:
        if name not in fn: res.append(f'{name[-4:]}:absent'); continue
        start = fn[name]
        end = min((o for o in fn.values() if o > start), default=len(words)*4)
        want = list(T[name])
        resolved, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, IS)
        got = list(resolved[start//4:end//4])
        for o, v in masks.items():
            if start <= o < end:
                k = (o - start)//4
                if k < len(want): want[k] &= v
                got[k] &= v
        # strip trailing zero padding words in got beyond want
        while len(got) > len(want) and got[-1] == 0: got.pop()
        pos = sum(1 for a, b in zip(want, got) if a != b) + abs(len(want) - len(got))
        sm = difflib.SequenceMatcher(None, want, got, autojunk=False)
        rows = sum(max(i2-i1, j2-j1) for t, i1, i2, j1, j2 in sm.get_opcodes() if t != 'equal')
        res.append(f'{name[-4:]}:' + ('M' if pos == 0 else f'{rows}r/{pos}p'))
    print(f'{d}\t' + ' '.join(res), flush=True)
