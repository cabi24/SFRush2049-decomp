#!/usr/bin/env python3
"""fnrep.py IN OUT FN old new [old new ...]: replace ALL occurrences of old->new within function FN's body only."""
import sys
i, o, fn = sys.argv[1:4]; reps = sys.argv[4:]
s = open(i).read()
import re
m = re.search(r'^[^\n;]*\b%s\([^;{]*\)\s*\{' % re.escape(fn), s, re.M)
a = m.start(); e = s.index('\n}\n', a) + 3
b = s[a:e]
for k in range(0, len(reps), 2):
    if reps[k] not in b: sys.exit('MISSING ' + reps[k])
    b = b.replace(reps[k], reps[k + 1])
open(o, 'w').write(s[:a] + b + s[e:])
