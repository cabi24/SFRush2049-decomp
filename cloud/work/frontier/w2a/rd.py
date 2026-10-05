#!/usr/bin/env python3
"""rd.py ADDR [N]: retail words at image address (from asm/us/blob_data), with float view."""
import sys, struct
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import owndata
img = owndata.ImageData.from_artifact(owndata.artifact_dir(ROOT / 'asm/us/blob'))
a = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 1
for i in range(n):
    b = img.read(a + 4*i, 4)
    if b is None: print(f'{a+4*i:08X}: ?'); continue
    w = struct.unpack('>I', b)[0]
    print(f'{a+4*i:08X}: {w:08X} {struct.unpack(">f", b)[0]!r}')
