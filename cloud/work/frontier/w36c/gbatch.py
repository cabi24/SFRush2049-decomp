#!/usr/bin/env python3
"""On builder: gbatch.py VARDIR GROUPJSON FUNC [FUNC...] -> for each vNNNN.c in VARDIR compile as group.c with GROUPJSON,
print strict differing words of each FUNC (and -1 on build failure), sorted at the end."""
import sys, os, json, shutil, tempfile, subprocess, glob
sys.path.insert(0, 'tools/cloud')
import score
from pathlib import Path
score.ASM_DIR = Path('asm/us/ovl_a').resolve()
vd, gj, funcs = Path(sys.argv[1]), sys.argv[2], sys.argv[3:]
res = []
for f in sorted(vd.glob('v*.c')):
    d = Path(tempfile.mkdtemp(prefix='gb-'))
    shutil.copy(f, d / 'group.c'); shutil.copy(gj, d / 'group.json')
    try:
        obj = d / 'o.o'
        score.compile_group(d, obj)
        row = []
        import io, contextlib
        for fn in funcs:
            with contextlib.redirect_stdout(io.StringIO()):
                r = score.compare(obj, fn)
            row.append(r.differing)
        res.append((row, f.name))
    except SystemExit as e:
        res.append(([-1] * len(funcs), f.name))
    shutil.rmtree(d, ignore_errors=True)
res.sort(key=lambda x: (x[0][0] if x[0][0] >= 0 else 10**9))
for row, name in res[:int(os.environ.get('TOP', '25'))]:
    print(name, row)
