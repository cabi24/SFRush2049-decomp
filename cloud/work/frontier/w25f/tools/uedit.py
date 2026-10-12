#!/usr/bin/env python3
"""w25f: splice records into a binary Ucode stream.  uedit.py IN.O OUT.O OP...
OP = ins:AFTER:SRC[:const]   insert a copy of record SRC after record AFTER (ldc: replace the constant word)
     del:IDX                 delete record IDX
     dt:IDX:DTYPE            set record IDX's dtype
Indices refer to the ORIGINAL stream.  usage needs PYTHONPATH=third_party/n64-decomp-workbench/src"""
import sys, struct
from decomp_workbench.ucode import parse_ucode
recs = parse_ucode(open(sys.argv[1], 'rb').read())
words = {r.index: list(r.words) for r in recs}
after = {}
dele = set()
for op in sys.argv[3:]:
    f = op.split(':')
    if f[0] == 'ins':
        w = list(recs[int(f[2])].words)
        if len(f) > 3:
            w[recs[int(f[2])].base_word_length] = int(f[3], 0)
        after.setdefault(int(f[1]), []).append(w)
    elif f[0] == 'del':
        dele.add(int(f[1]))
    elif f[0] == 'dt':
        w = words[int(f[1])]; w[0] = (w[0] & ~(0x1F << 16)) | (int(f[2], 0) << 16)
out = []
for r in recs:
    if r.index not in dele: out += words[r.index]
    for w in after.get(r.index, []): out += w
open(sys.argv[2], 'wb').write(struct.pack('>%dI' % len(out), *out))
