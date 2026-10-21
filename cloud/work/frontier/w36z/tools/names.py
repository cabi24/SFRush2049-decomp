#!/usr/bin/env python3
# names.py SPLFILE PREREPORT [SPLITLINE-REGEX]: label the counted interferers of the cols split piece at bb45 with the
# expression text of their symbol (= PRE bit) from pretrace's report.txt.
import re, sys
spl, rep = sys.argv[1], sys.argv[2]
pat = re.compile(sys.argv[3] if len(sys.argv) > 3 else r'split lr=\d+ sym=\d+ start bb=39')
bits = {}
cur = None
for l in open(rep):
    m = re.match(r'\s*(\d+)\s+(\S.*)$', l)
    if m and not l.startswith('        '):
        bits[int(m.group(1))] = m.group(2)[:70]
on = False
for l in open(spl):
    if pat.search(l):
        on = True; print(l.strip()); continue
    if on and '[SPL] split' in l:
        on = False
    if on and l.startswith('[SPLI] bb=45:'):
        for t in l.split()[2:]:
            if '*' in t or '+' in t:
                lr, sym = t.rstrip('*+').split('/')
                print('  %-10s %s' % (t, bits.get(int(sym), '?') if int(lr) else '(null)'))
        break
