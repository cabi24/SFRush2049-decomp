#!/usr/bin/env python3
"""subfn.py FILE FUNC OLD NEW : replace OLD->NEW only within FUNC's definition body (all occurrences). @file reads file."""
import sys
p, fn, old, new = sys.argv[1:5]
rd = lambda x: open(x[1:]).read() if x.startswith('@') else x
old, new = rd(old), rd(new)
s = open(p).read()
import re
m = re.search(r'\n[^\n;]*\b' + fn + r'\([^;{]*\)\s*\{', s)
a = m.start()
b = s.index('\n}\n', a)
body = s[a:b]
if old not in body: print('NOT FOUND in', fn); sys.exit(1)
s = s[:a] + body.replace(old, new) + s[b:]
open(p, 'w').write(s)
