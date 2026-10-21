#!/usr/bin/env python3
# usage: spmap.py LISTING(want or got rows "N insn") -> offsets relative to frame top
import re,sys,collections
rows=[l.strip() for l in open(sys.argv[1]) if re.match(r'\s*\d+ ',l)]
fr=None; offs=collections.defaultdict(list)
for r in rows:
    i,ins=r.split(' ',1)
    m=re.match(r'addiu sp,sp,-(\d+)',ins)
    if m and fr is None: fr=int(m.group(1)); continue
    m=re.search(r'(-?\d+)\(sp\)',ins) or re.search(r',sp,(-?\d+)$',ins)
    if m:
        o=int(m.group(1)); op=ins.split()[0]
        offs[o].append(op)
for o in sorted(offs, reverse=True):
    c=collections.Counter(offs[o])
    print(f'{o:5d} top{o-fr:+5d}  '+' '.join(f'{k}x{v}' for k,v in c.items()))
