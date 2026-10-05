#!/usr/bin/env python3
"""rd.py ADDR [NWORDS] [f]: print retail game-image words at ADDR (hex); 'f' also prints them as floats."""
import sys, struct, json
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.conveyor.pipeline import blob_unit
from tools.conveyor.pipeline import blob_layout
doc = blob_layout.load()
img, base = blob_unit.load_image(doc)
a = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 4
for i in range(n):
    w, = struct.unpack_from('>I', img, a - base + 4 * i)
    extra = ''
    if 'f' in sys.argv[3:]: extra = '  %r' % struct.unpack('>f', struct.pack('>I', w))[0]
    print('%08X: %08X%s' % (a + 4 * i, w, extra))
