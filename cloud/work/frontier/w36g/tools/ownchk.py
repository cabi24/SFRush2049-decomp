#!/usr/bin/env python3
"""ownchk.py OBJ NAME... : verify each function's own .rodata/.data/.bss references in a compiled group object
against the retail runtime image A (build/ovl/a/image.bin) with tools/cloud/owndata.verify (Pi side)."""
import sys, struct, json
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import owndata, score
obj = sys.argv[1]
data = (ROOT / 'build/ovl/a/image.bin').read_bytes(); base = 0x8038A400
img = owndata.ImageData.from_image(data, base)
syms = score.symbols(Path(obj))
e = json.load(open(ROOT / 'asm/us/ovl_a/extents.json'))
ext = {f['name']: (int(f['address'], 16), f['size']) for f in e['functions']}
for name in sys.argv[2:]:
    va, size = ext[name]
    want = list(struct.unpack('>%dI' % (size // 4), data[va - base:va - base + size]))
    r = owndata.verify(obj, name, want, address=va, image=img, start=syms[name], addresses=score.address_named)
    try: b = {k: hex(v) for k, v in r.bases().items()}
    except Exception as ex: b = 'bases err %s' % ex
    print(name, 'sites', len(r.sites), 'failures', r.failures, 'unverified', r.unverified, 'bases', b)
