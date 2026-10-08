#!/usr/bin/env python3
"""ofat.py TEMPLATE OUTDIR [BASE_CHOICES]: one-factor-at-a-time variants around a base choice vector.
BASE_CHOICES: comma list of option indices per axis (default all 0). Writes OUTDIR/v%04d.c and manifest.tsv."""
import re, sys
from pathlib import Path
PAT = re.compile(r'/\*@\{(\w*)\*/(.*?)/\*@\|(.*?)@\}\*/', re.S)
tpl, out = sys.argv[1], Path(sys.argv[2])
text = Path(tpl).read_text()
pieces, points, pos = [], [], 0
for m in PAT.finditer(text):
    pieces.append(text[pos:m.start()])
    points.append((m.group(1), [m.group(2)] + m.group(3).split('@|')))
    pieces.append(len(points) - 1); pos = m.end()
pieces.append(text[pos:])
axes, axis_of, names = [], [], {}
for name, opts in points:
    if name and name in names:
        axis_of.append(names[name]); continue
    if name: names[name] = len(axes)
    axis_of.append(len(axes)); axes.append(len(opts))
base = [0] * len(axes)
if len(sys.argv) > 3 and sys.argv[3]:
    base = [int(x) for x in sys.argv[3].split(',')]
def render(c):
    return ''.join(p if isinstance(p, str) else points[p][1][c[axis_of[p]]] for p in pieces)
combos = [tuple(base)]
for ax, n in enumerate(axes):
    for o in range(n):
        if o != base[ax]:
            c = list(base); c[ax] = o; combos.append(tuple(c))
out.mkdir(parents=True, exist_ok=True)
rows = []
for k, c in enumerate(combos):
    f = 'v%04d.c' % k
    (out / f).write_text(render(c)); rows.append('%s\t%s' % (f, ' '.join(map(str, c))))
(out / 'manifest.tsv').write_text('file\taxes(%s)\n' % ','.join(map(str, axes)) + '\n'.join(rows) + '\n')
print('%d axes, wrote %d one-factor variants' % (len(axes), len(combos)))
