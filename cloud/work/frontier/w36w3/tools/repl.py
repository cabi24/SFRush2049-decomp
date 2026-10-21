#!/usr/bin/env python3
"""repl.py SRC.c FUNC BODY.c OUT.c : replace the definition of FUNC in SRC.c with the text of BODY.c."""
import re, sys
src, func, body, out = sys.argv[1:5]
s = open(src).read()
m = re.search(r'^[^\n;{}]*\b' + re.escape(func) + r'\s*\([^;{]*\)\s*\{', s, re.M)
if not m:
    sys.exit('definition not found')
i = m.end(); depth = 1
while depth:
    c = s[i]
    depth += (c == '{') - (c == '}')
    i += 1
b = open(body).read()
open(out, 'w').write(s[:m.start()] + b.rstrip('\n') + s[i:])
