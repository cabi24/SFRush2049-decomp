#!/usr/bin/env python3
"""var.py BASE.c OUT.c 'old=>new' ... : literal replacements (each must hit)"""
import sys
s = open(sys.argv[1]).read()
for r in sys.argv[3:]:
    a, b = r.split('=>')
    a = a.replace('\\n', '\n'); b = b.replace('\\n', '\n')
    if a not in s: sys.exit('miss: ' + a)
    s = s.replace(a, b, 1)
open(sys.argv[2], 'w').write(s)
