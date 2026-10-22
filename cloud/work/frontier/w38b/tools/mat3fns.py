#!/usr/bin/env python3
"""List MAT3 functions of a GameSource file in source order with scope and the
calls each makes (in source order), restricted to names defined in the file
(plus any extra names given with -x). Usage: mat3fns.py FILE.c [-a]"""
import csv, re, sys
M = 'reference/repos/mat3'
f = sys.argv[1]
allcalls = '-a' in sys.argv
rows = [r for r in csv.DictReader(open(f'{M}/rush_d_functions.csv')) if r['source_file'] == f]
rows.sort(key=lambda r: int(r['line'] or 0))
names = {r['name'] for r in rows}
src = open(f'{M}/decomp/src/GameSource/src/{f}', errors='replace').read()
# crude function body extraction
def body(name):
    m = re.search(r'^[^\n;]*\b' + re.escape(name) + r'\s*\([^;{]*\)\s*\{', src, re.M)
    if not m: return None
    i = m.end(); depth = 1
    while depth and i < len(src):
        c = src[i]
        if c == '{': depth += 1
        elif c == '}': depth -= 1
        i += 1
    return src[m.end():i]
for r in rows:
    b = body(r['name'])
    calls = []
    if b:
        for c in re.findall(r'\b([A-Za-z_]\w*)\s*\(', b):
            if c in ('if', 'while', 'for', 'switch', 'return', 'sizeof'): continue
            if allcalls or c in names:
                calls.append(c)
    print(f"{r['line']:>5} {r['scope']:7} {r['size'] or '-':>5} {r['name']:28s} -> {' '.join(calls)}")
