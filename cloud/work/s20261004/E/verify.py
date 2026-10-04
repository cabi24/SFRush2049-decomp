"""verify.py OBJ name1,name2,...  -- relocate each member slice to its image
address (blob_group.relocate) and compare with build/game_code.bin bytes."""
import json, sys, struct, subprocess, hashlib
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]; sys.path.insert(0, str(ROOT))
from tools.conveyor.pipeline import blob_layout, blob_group, blob_splice
obj = Path(sys.argv[1]).resolve(); names = sys.argv[2].split(',')
l = blob_layout.load()
slots = {e['target_id']: e for r in l['regions'] for e in r['entries'] if e['kind'] == 'function'}
image = (ROOT / 'build/game_code.bin').read_bytes(); base = 0x80086A50
syms = subprocess.check_output(['mips-linux-gnu-nm', '-S', str(obj)], text=True)
sizes = {a[-1]: int(a[1], 16) for x in syms.splitlines() if len(a := x.split()) == 4}
import os
extra = [x for x in os.environ.get('DIAG_EXTRA', '').split(',') if x]
for x in extra: slots[x] = {'vaddr': 0x80000000, 'size': sizes[x]}
slices, ndx = blob_group.member_slices(obj, names + extra, slots)
rows = []
for n in names:
    row = {'name': n, 'vaddr': hex(slots[n]['vaddr']), 'slot_bytes': slots[n]['size'], 'symbol_bytes': sizes.get(n)}
    try:
        body = blob_group.relocate(obj, slices, ndx, blob_splice.image_symbols(l), members=[n], image=(image, base))[n]
        orig = image[slots[n]['vaddr'] - base: slots[n]['vaddr'] - base + slots[n]['size']]
        Path('obj/%s.%s.bin' % (obj.stem, n)).write_bytes(body)
        d = [i for i in range(max(len(body), len(orig)) // 4) if body[i*4:i*4+4] != orig[i*4:i*4+4]]
        row.update(word_differences=len(d), difference_indices=d[:40], identical=(body == orig))
    except Exception as e:
        row['refusal'] = repr(e)
        try:
            s2 = dict(slices); o, v, _ = s2[n]; s2[n] = (o, v, sizes[n])
            body = blob_group.relocate(obj, s2, ndx, blob_splice.image_symbols(l), members=[n], image=(image, base))[n]
            Path('obj/%s.%s.bin' % (obj.stem, n)).write_bytes(body)
        except Exception as e2:
            row['diag_refusal'] = repr(e2)
    rows.append(row)
out = {'object': obj.name, 'object_sha256': hashlib.sha256(obj.read_bytes()).hexdigest(), 'results': rows}
print(json.dumps(out, indent=1))
