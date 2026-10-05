#!/usr/bin/env python3
"""ins.py BASE.c OUTDIR STMT [STMT...]: insert each STMT before every statement line of the function body."""
import sys
from pathlib import Path
src = open(sys.argv[1]).read().split('\n'); out = Path(sys.argv[2]); out.mkdir(exist_ok=True)
start = next(i for i, l in enumerate(src) if l.startswith('s32 object_manager_update('))
n = 0
for k, stmt in enumerate(sys.argv[3:]):
    for i in range(start + 2, len(src) - 1):
        l = src[i]; st = l.strip()
        if not st or st.startswith('}') or st.startswith('else') or st.startswith('s32 ') or st.startswith('u8 '): continue
        ind = l[:len(l) - len(l.lstrip())]
        t = src[:i] + [ind + stmt] + src[i:]
        (out / ('s%d_L%03d.c' % (k, i))).write_text('\n'.join(t)); n += 1
print(n)
