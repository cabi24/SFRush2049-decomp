#!/usr/bin/env python3
"""var.py BASE.c VARFILE -- VARFILE holds variants separated by lines '%%% name'; each replaces the
text between the markers '/*L2*/' ... '/*E2*/' in BASE (or whole loop-2 region lines). Scores each."""
import sys, subprocess, os, re
base = open(sys.argv[1]).read()
vs = open(sys.argv[2]).read().split('%%% ')[1:]
start = base.index('    done = 0;\n')
end = base.rindex('}')  # end of function
os.makedirs('vv', exist_ok=True)
for v in vs:
    name, body = v.split('\n', 1)
    name = name.strip()
    if body.startswith('@DECL '):
        decl, body = body.split('\n', 1)
        src = base.replace('    s32 k;\n', '    s32 k;\n' + decl[6:] + '\n', 1)
        st = src.index('    done = 0;\n'); en = src.rindex('}')
    else:
        src, st, en = base, start, end
    out = src[:st] + body + src[en:]
    p = 'vv/%s.c' % name
    open(p, 'w').write(out)
    r = subprocess.run(['sh', 'tools/am.sh', p], capture_output=True, text=True).stdout.strip().splitlines()
    print(name, '|', r[0] if r else '??', flush=True)
