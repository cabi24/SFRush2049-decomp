#!/usr/bin/env python3
"""ownverify.py OBJ NAME: verify a fetched object's own .rodata/.data references against build/game_code.bin
(tools/cloud/owndata.verify, the same check blob_splice.link_function runs)."""
import sys, json
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud'))
import score, owndata
obj, name = sys.argv[1], sys.argv[2]
syms = {k: int(v, 16) for k, v in json.load(open(ROOT / 'asm/us/blob/symbols.json'))['symbols'].items()}
want = score.targets()[name]
image = owndata.ImageData.from_image((ROOT / 'build/game_code.bin').read_bytes(), 0x80086A50)
r = owndata.verify(obj, name, want, address=syms[name], image=image)
print(name, 'ok=%s' % r.ok, 'references=%d' % r.references, 'sites=%d' % len(r.sites))
for n in r.notes: print('  note:', n)
for f in r.failures: print('  FAIL:', f)
for u in r.unverified: print('  unverified:', u)
try: print('  bases:', {k: hex(v) for k, v in r.bases().items()})
except ValueError as e: print('  bases error:', e)
