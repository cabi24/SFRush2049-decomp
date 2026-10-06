#!/usr/bin/env python3
"""gvar.py SRCGROUP FUNC OUTDIR body.c...: make one group dir per body, replacing FUNC's definition in the group's
group.c (from 'void FUNC(' to the next line that is exactly '}') with body file contents; FUNC moved to members."""
import sys, json, re, shutil
from pathlib import Path
src, func, out = Path(sys.argv[1]), sys.argv[2], Path(sys.argv[3])
out.mkdir(parents=True, exist_ok=True)
for b in sys.argv[4:]:
    d = out / Path(b).stem
    if d.exists(): shutil.rmtree(d)
    shutil.copytree(src, d)
    g = json.loads((d / 'group.json').read_text())
    if func in g.get('context', []): g['context'].remove(func)
    if func not in g['members']: g['members'].append(func)
    (d / 'group.json').write_text(json.dumps(g, indent=1))
    for fn in g['files']:
        p = d / fn; s = p.read_text()
        m = re.search(r'^[^\n;]*\b%s\([^;{]*\)\s*\{' % func, s, re.M)
        if not m: continue
        e = s.index('\n}\n', m.start()) + 3
        p.write_text(s[:m.start()] + Path(b).read_text() + s[e:])
