#!/usr/bin/env python3
"""fnset.py IN OUT FN NEWFILE: replace the whole definition of FN in IN with the text of NEWFILE."""
import sys, re
i, o, fn, nf = sys.argv[1:5]
s = open(i).read()
m = re.search(r"^[^\n;]*\b%s\([^;{]*\)\s*\{" % re.escape(fn), s, re.M)
a = m.start(); e = s.index('\n}\n', a) + 3
open(o, 'w').write(s[:a] + open(nf).read().rstrip('\n') + '\n' + s[e:])
