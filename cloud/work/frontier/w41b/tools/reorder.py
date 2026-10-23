#!/usr/bin/env python3
"""reorder.py IN.c OUT.c NAME[,NAME...]: move top-level function definitions into the given order (others keep their
relative order after the listed ones unless --rest-first); every non-function top-level text stays in a preamble in
original order; prototypes for all moved functions are emitted after the preamble."""
import re, sys
src = open(sys.argv[1]).read().split('\n')
order = sys.argv[3].split(',')
funcs = {}; fseq = []; pre = []
i = 0
hdr = re.compile(r'^(static\s+)?[A-Za-z_][\w \*]*\b(func_[0-9A-Fa-f]{8})\(([^;]*)\)\s*$')
while i < len(src):
    m = hdr.match(src[i])
    if m and i + 1 < len(src) and src[i + 1].startswith('{'):
        j = i + 1
        while not src[j].startswith('}'): j += 1
        funcs[m.group(2)] = (src[i:j + 1], src[i])
        fseq.append(m.group(2)); i = j + 1; continue
    pre.append(src[i]); i += 1
rest = [f for f in fseq if f not in order]
seq = [f for f in order if f in funcs] + rest
out = pre + [''] + [funcs[f][1] + ';' for f in fseq] + ['']
for f in seq: out += funcs[f][0] + ['']
open(sys.argv[2], 'w').write('\n'.join(out) + '\n')
print('functions', len(fseq), 'order', seq)
