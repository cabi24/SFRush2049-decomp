#!/usr/bin/env python3
"""swap.py BASE_GROUP.c SRC_GROUP.c OUT.c : take BASE and replace the func_803A1EAC definition with SRC's."""
import sys,re
def split(s, name):
    i = s.index('\nvoid %s(void)\n' % name) + 1
    # function ends at the first '\n}\n' after i
    j = s.index('\n}\n', i) + 3
    return i, j
b = open(sys.argv[1]).read(); s = open(sys.argv[2]).read()
bi, bj = split(b, 'func_803A1EAC'); si, sj = split(s, 'func_803A1EAC')
open(sys.argv[3], 'w').write(b[:bi] + s[si:sj] + b[bj:])
