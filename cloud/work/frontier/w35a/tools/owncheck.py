#!/usr/bin/env python3
"""owncheck.py OBJ NAME... : run score.compare with the PRIVATE decompressed image A (build/ovl/a/image.bin) supplied as
retail own-data (the public tree has no asm/us/ovl_a_data), so .rodata/.data/.bss references of an overlay function
are actually verified. Local only (the image is not in git)."""
import sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud'))
import score, owndata
img = ROOT / 'build/ovl/a/image.bin'
score.ASM_DIR = ROOT / 'asm/us/ovl_a'
image = owndata.ImageData.from_image(img.read_bytes(), 0x8038A400)
score.own_data = lambda: image
for n in sys.argv[2:]:
    r = score.compare(sys.argv[1], n, show=0)
    print(n, ':', r.summary())
    for note in r.notes: print('   ', note)
