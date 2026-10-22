#!/usr/bin/env python3
"""ocmp.py OBJ NAME [ovl_a|ovl_b] [--show N]: compare one function of a local object with the retail overlay target
(score.compare, relocation-masked; own data not verified)."""
import sys, io, contextlib
sys.path.insert(0, 'tools/cloud')
from pathlib import Path
import score
obj, name = sys.argv[1], sys.argv[2]
img = sys.argv[3] if len(sys.argv) > 3 and not sys.argv[3].startswith('--') else 'ovl_a'
show = int(sys.argv[sys.argv.index('--show') + 1]) if '--show' in sys.argv else 12
score.ASM_DIR = Path('asm/us/' + img).resolve()
score.own_data = lambda: None
r = score.compare(obj, name, show=show)
print(name, r.summary())
