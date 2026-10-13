#!/usr/bin/env python3
"""splice.py BASE PIECE OUT: replace each top-level function defined in PIECE inside BASE (by name)."""
import re, sys
base = open(sys.argv[1]).read(); piece = open(sys.argv[2]).read()
def funcs(text):
    out = {}
    for m in re.finditer(r'^[A-Za-z][^;{}\n]*?\b(\w+)\s*\([^;{}]*\)\s*\{', text, re.M):
        start = m.start(); depth = 0; i = m.end() - 1
        while True:
            c = text[i]
            if c == '{': depth += 1
            elif c == '}':
                depth -= 1
                if depth == 0: break
            i += 1
        out[m.group(1)] = (start, i + 1)
    return out
pf = funcs(piece); bf = funcs(base)
extra = piece
for name, (s, e) in sorted(pf.items(), key=lambda kv: -kv[1][0]):
    if name in bf:
        bs, be = bf[name]
        base = base[:bs] + piece[s:e] + base[be:]
        bf = funcs(base)
        extra = extra.replace(piece[s:e], '')
pre = extra.strip()
if pre:  # declarations/types/helpers placed before the first group member definition
    m = re.search(r'^/\* group members \*/', base, re.M)
    base = base[:m.start()] + pre + '\n\n' + base[m.start():]
open(sys.argv[3], 'w').write(base)
