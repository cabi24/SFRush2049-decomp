#!/usr/bin/env python3
"""hv.py NAME 'lineno:old@@new' ... : make hx/v/NAME.c from hx/base.c with per-line replacements (1-based line numbers)."""
import sys, os
name = sys.argv[1]
L = open('hx/base.c').read().split('\n')
for spec in sys.argv[2:]:
    ln, rest = spec.split(':', 1)
    old, new = rest.split('@@', 1)
    ln = int(ln) - 1
    assert old in L[ln], (ln + 1, old, L[ln])
    L[ln] = L[ln].replace(old, new)
os.makedirs('hx/v', exist_ok=True)
open('hx/v/%s.c' % name, 'w').write('\n'.join(L))
