#!/usr/bin/env python3
"""ocmp.py OBJ NAME [NAME...]: strict compare of NAME in OBJ against overlay A targets (local, no compile)."""
import sys, io, contextlib
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score
score.ASM_DIR = ROOT / 'asm/us/ovl_a'
for n in sys.argv[2:]:
    with contextlib.redirect_stdout(io.StringIO()):
        r = score.compare(Path(sys.argv[1]), n)
    print(n, r.differing)
