#!/usr/bin/env python3
"""w25b: dump an IDO binary Ucode stream (cc -K's r.O = uopt output / r.B = cfe output) one record per line,
with the source line from the preceding loc. Uses the vendored workbench parser (7.1 streams parse unchanged).
usage: PYTHONPATH=third_party/n64-decomp-workbench/src python3 udump.py r.O [FIRSTLINE LASTLINE]"""
import sys
from decomp_workbench.ucode import parse_ucode
recs = parse_ucode(open(sys.argv[1], 'rb').read())
lo, hi = (int(sys.argv[2]), int(sys.argv[3])) if len(sys.argv) > 3 else (0, 1 << 30)
line = 0
for r in recs:
    if r.name == 'loc': line = r.words[1]
    if lo <= line <= hi:
        print(line, r.index, r.name, r.dtype, r.mtype, ' '.join('%x' % w for w in r.words[1:]))
