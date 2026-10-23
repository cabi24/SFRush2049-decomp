#!/usr/bin/env python3
"""vrun.py BASEGROUP FILE FUNC OLD NEW1 [NEW2 ...]: for each NEWi, copy BASEGROUP to BASEGROUP_vI with FILE's
OLD replaced by NEWi, compile on the builder (gd.sh) and print the udiff summary for FUNC."""
import sys, shutil, subprocess, os
base, fn, func, old = sys.argv[1:5]
news = sys.argv[5:]
here = os.path.dirname(os.path.abspath(__file__))
for i, new in enumerate(news):
    d = f"{base}_v{i}"
    shutil.rmtree(d, ignore_errors=True)
    shutil.copytree(base, d)
    p = os.path.join(d, fn)
    s = open(p).read()
    assert old in s, old
    open(p, 'w').write(s.replace(old, new.replace('\\n', '\n'), 1))
    out = subprocess.run([os.path.join(here, '..', 'gd.sh'), d, func, '--summary'], capture_output=True, text=True)
    print(f"v{i}: {(out.stdout.strip().splitlines() or [out.stderr.strip()[-300:]])[-1]}  <- {new[:120]!r}")
