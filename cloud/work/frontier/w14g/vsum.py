#!/usr/bin/env python3
"""vsum.py BDIR: summarise a vbatch round: histogram of strict counts and option frequencies among the best."""
import sys, re, collections
d = sys.argv[1]
rows = []
for line in open(d + '/score.txt'):
    m = re.search(r'mnem-missing\s+(\d+)\s+aligned-missing\s+(\d+)\s+strict\s+(\d+)\s+size\s+(\S+)\s+(\S+\.c)', line)
    if m: rows.append((int(m[3]), int(m[2]), int(m[1]), m[5]))
man = {}
hdr = None
for line in open(d + '/manifest.tsv'):
    p = line.rstrip('\n').split('\t')
    if p[0] == 'file': hdr = p; continue
    man[p[0]] = p[1].split()
rows.sort()
print('variants scored:', len(rows), ' strict hist:', sorted(collections.Counter(r[0] for r in rows).items())[:6])
best = [r for r in rows if r[0] == rows[0][0]]
print('best strict', rows[0][0], 'count', len(best))
for r in rows[:3]: print(' ', r)
ax = collections.defaultdict(collections.Counter)
for r in best:
    for i, v in enumerate(man.get(r[3], [])): ax[i][v] += 1
print('option counts among best:', {k: dict(v) for k, v in ax.items()})
