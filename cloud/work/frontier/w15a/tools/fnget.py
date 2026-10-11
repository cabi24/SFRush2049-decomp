#!/usr/bin/env python3
"""fnget.py IN FN: print the definition of FN."""
import sys, re
i, fn = sys.argv[1:3]
s = open(i).read()
m = re.search(r"^[^\n;]*\b%s\([^;{]*\)\s*\{" % re.escape(fn), s, re.M)
a = m.start(); e = s.index('\n}\n', a) + 3
sys.stdout.write(s[a:e])
