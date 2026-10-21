"""Scan sources/RESULTS for 'N64 = GC name' statements; prints candidate rows for manual review."""
import json, os, re, sys, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from data import REPO, load_gc, n64_population
gc = load_gc()
imgs, static, pop = n64_population()
gcnames = {}
for k, v in gc.items():
    if v['gamesource']:
        gcnames.setdefault(v['name'], []).append(k)
        if v['name'].startswith('_'):
            gcnames.setdefault(v['name'][1:], []).append(k)
n64names = {}
for i, v in pop.items():
    n64names.setdefault(v['name'], []).append(i)
STOP = set(['initialize', 'magnitude', 'direction', 'Random', 'Hidden', 'Interpolate', 'drawRect'])
pat_n64 = re.compile(r'\b(func_[0-9A-F]{8}|[a-z][a-z0-9]*(?:_[a-z0-9]+)+|[A-Za-z][A-Za-z0-9_]{4,})\b')
pat_gc = re.compile(r'\b(_?[A-Za-z][A-Za-z0-9_]+)(@[a-z_A-Z0-9]+\.c)?')
files = sys.argv[1:]
out = []
for f in files:
    try:
        lines = open(f, errors='replace').read().split('\n')
    except Exception:
        continue
    for ln, line in enumerate(lines, 1):
        if not re.search(r'\bGC\b|MAT3|GameCube|@[a-z]+\.c|rush_d', line):
            continue
        n64s = [m for m in pat_n64.finditer(line) if m.group(1) in n64names]
        gcs = [m for m in pat_gc.finditer(line) if m.group(1) in gcnames and m.group(1) not in STOP]
        for a in n64s:
            for b in gcs:
                if a.group(1) == b.group(1):
                    continue
                if b.start() < a.start():
                    continue
                between = line[a.end():b.start()]
                if len(between) > 40 or not re.search(r'=|\(GC|GC\b|MAT3|->|is\b', between):
                    continue
                if re.search(r'\b(func_[0-9A-F]{8})\b', between):
                    continue
                g = b.group(1) + (b.group(2) or '')
                out.append((a.group(1), g, os.path.relpath(f, REPO), ln, line.strip()[:220]))
seen = set()
for r in out:
    k = (r[0], r[1])
    if k in seen: continue
    seen.add(k)
    print('%s\t%s\t%s:%d\t%s' % r)
