#!/usr/bin/env python3
"""ownwin.py OBJ ovl_a|ovl_b NAME...: run owndata.verify against the local retail image and print, per function,
how many own-data windows were content-compared and equal, any content failures ("differs from retail"), and the
one-base-rule note separately. Content failures = 0 means every referenced literal/table equals retail bytes."""
import sys, json
sys.path.insert(0, 'tools/cloud')
from pathlib import Path
import score, owndata
obj, img = sys.argv[1], sys.argv[2]
score.ASM_DIR = Path('asm/us/' + img).resolve()
data = Path(f'build/ovl/{img[-1]}/image.bin').read_bytes()
image = owndata.ImageData.from_image(data, 0x8038A400)
table = score.image_symbols()
for name in sys.argv[3:]:
    want = score.targets()[name]
    r = owndata.verify(obj, name, want, address=table[name], image=image,
                       addresses=lambda s: table.get(s))
    differs = [f for f in r.failures if 'differs from retail' in f or 'different addresses' in f]
    base = [f for f in r.failures if 'disagree on the section' in f]
    other = [f for f in r.failures if f not in differs and f not in base]
    placed = sum(len(v) for v in r.placements.values())
    print(f'{name}: refs={r.references} content_failures={len(differs)} other_failures={len(other)} '
          f'one_base_rule={len(base)} unverified={len(r.unverified)}')
    for f in differs + other: print('   ', f[:200])
