#!/usr/bin/env python3
"""wbgen.py BASE.c OUTDIR [--lines LO..HI] [--classes O,P,A,H]: mechanical one-edit variants of BASE.c.

Runs the vendored n64-decomp-workbench sweep generators (third_party/n64-decomp-workbench) and collects
their sources into one flat directory that vbatch.sh can score:
  - commute: every textually pure commutative operand exchange (value-preserving);
  - copies: every removable `Y = X;` copy between two locals;
  - hoist: at each line in LO..HI (default: the whole file), hoist an operand into an existing dead local
    (classes O = nested leaf, P = compound assignment, A = call argument, H = top-level side).
Every emitted file is one edit away from BASE. Byte-identical duplicates are dropped. OUTDIR/v0000.c is
BASE itself (the control). OUTDIR/manifest.tsv names each file's generator and edit. Generators that
refuse an edit are skipped. Review a winner's edit before adopting it (`python3 tools/workbench.py
experiment review-mutation BASE.c WINNER.c`). Then use the winner as the next BASE (repeat), or fold the
edit into a vgen.py template.
"""
import argparse, hashlib, json, shutil, subprocess, sys, tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[5]
WB = [sys.executable, str(REPO / 'tools/workbench.py')]

def run(args, out):
    p = subprocess.run(WB + args + ['--write', str(out)], capture_output=True, text=True)
    return sorted(out.glob('*.c')) if out.is_dir() else []

def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('base'); ap.add_argument('outdir')
    ap.add_argument('--lines', default=''); ap.add_argument('--classes', default='O,P,A,H')
    a = ap.parse_args()
    base = Path(a.base).resolve(); text = base.read_text()
    n = len(text.splitlines())
    lo, hi = (map(int, a.lines.split('..')) if a.lines else (1, n))
    out = Path(a.outdir); out.mkdir(parents=True, exist_ok=True)
    seen = {hashlib.sha256(text.encode()).hexdigest(): 'v0000.c'}
    (out / 'v0000.c').write_text(text)
    rows = ['v0000.c\tcontrol\tbase unedited']
    counts = {}
    with tempfile.TemporaryDirectory() as t:
        t = Path(t); jobs = [('commute', ['sweep', 'commute', str(base)]), ('copies', ['sweep', 'copies', str(base)])]
        for line in range(lo, hi + 1):
            jobs.append(('hoist', ['sweep', 'hoist', str(base), '--line', str(line), '--class', a.classes]))
        for k, (gen, args) in enumerate(jobs):
            d = t / ('j%d' % k)
            for f in run(args, d):
                body = f.read_text(); h = hashlib.sha256(body.encode()).hexdigest()
                if h in seen: continue
                name = 'v%04d.c' % len(seen); seen[h] = name
                (out / name).write_text(body)
                rows.append('%s\t%s\t%s' % (name, gen, f.stem))
                counts[gen] = counts.get(gen, 0) + 1
    (out / 'manifest.tsv').write_text('file\tgenerator\tedit\n' + '\n'.join(rows) + '\n')
    print('wrote %d variants (+ control) to %s: %s' % (len(seen) - 1, out,
          ', '.join('%s %d' % kv for kv in sorted(counts.items())) or 'none'))

main()
