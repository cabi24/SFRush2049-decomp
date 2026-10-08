#!/usr/bin/env python3
"""vgen.py TEMPLATE.c OUTDIR [--max 300] [--seed 1]: expand choice points into many variant files.

Mark each choice point in an otherwise ordinary C file like this:

    x = /*@{*/a + b/*@| b + a @| (a + b) @}*/;

Every option is inserted verbatim (spaces included). The text between `/*@{*/` and `/*@|` is option 0, so the
template itself compiles as option 0. The alternatives follow, separated by `@|`, and the comment closes with
`@}*/`. An alternative may span lines
and may be empty (`@|@}*/` deletes the code), but may not contain `*/`.
Name a choice point to link several (same name → same option index in every variant):

    /*@{decl*/s32 i; s32 n;/*@| s32 n; s32 i; @}*/   ...   /*@{decl*/A/*@| B @}*/

Writes OUTDIR/v0000.c … plus OUTDIR/manifest.tsv (file, option index per choice point). When the full
product exceeds --max, a random sample of --max combinations is written (variant 0 = all option 0 always).
"""
import argparse, itertools, random, re, sys
from pathlib import Path

PAT = re.compile(r'/\*@\{(\w*)\*/(.*?)/\*@\|(.*?)@\}\*/', re.S)

def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('template'); ap.add_argument('outdir')
    ap.add_argument('--max', type=int, default=300); ap.add_argument('--seed', type=int, default=1)
    a = ap.parse_args()
    text = Path(a.template).read_text()
    pieces, points = [], []          # pieces: str or index into points
    pos = 0
    for m in PAT.finditer(text):
        pieces.append(text[pos:m.start()])
        opts = [m.group(2)] + m.group(3).split('@|')
        points.append((m.group(1), opts))
        pieces.append(len(points) - 1)
        pos = m.end()
    pieces.append(text[pos:])
    if not points:
        sys.exit('no choice points found (syntax: /*@{*/opt0/*@| opt1 @| opt2 @}*/)')
    # axes: one per unnamed point, one per distinct name
    axes, axis_of = [], []
    names = {}
    for name, opts in points:
        if name:
            if name not in names:
                names[name] = len(axes); axes.append(len(opts))
            elif axes[names[name]] != len(opts):
                sys.exit('linked choice "%s" has differing option counts' % name)
            axis_of.append(names[name])
        else:
            axis_of.append(len(axes)); axes.append(len(opts))
    total = 1
    for n in axes: total *= n
    if total <= a.max:
        combos = list(itertools.product(*[range(n) for n in axes]))
    else:
        rng = random.Random(a.seed); seen = {tuple(0 for _ in axes)}; combos = [tuple(0 for _ in axes)]
        while len(combos) < a.max:
            c = tuple(rng.randrange(n) for n in axes)
            if c not in seen: seen.add(c); combos.append(c)
    out = Path(a.outdir); out.mkdir(parents=True, exist_ok=True)
    rows = []
    for k, c in enumerate(combos):
        body = ''.join(p if isinstance(p, str) else points[p][1][c[axis_of[p]]] for p in pieces)
        f = 'v%04d.c' % k
        (out / f).write_text(body)
        rows.append(f + '\t' + ' '.join(map(str, c)))
    (out / 'manifest.tsv').write_text('file\taxes(%s)\n' % ','.join(map(str, axes)) + '\n'.join(rows) + '\n')
    print('%d choice points, %d axes, product %d, wrote %d variants to %s' % (len(points), len(axes), total, len(combos), out))

main()
