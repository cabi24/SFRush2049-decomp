#!/usr/bin/env python3
"""try.py GROUPDIR FN 'old=>new' ... : apply replacements to a temp copy, print diff count (strict words)."""
import sys, shutil, tempfile, subprocess, json
from pathlib import Path
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
def run(gd, src, fns):
    t = Path(tempfile.mkdtemp()); shutil.copy(Path(gd)/'group.json', t/'group.json'); (t/'group.c').write_text(src)
    try:
        obj = t/'o.o'; score.compile_group(t, obj)
    except SystemExit as e:
        return None
    out = {}
    for f in fns:
        c = score.compare(obj, f, show=0); out[f] = (c.differing if hasattr(c,'differing') else None)
    return out
