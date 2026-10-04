#!/usr/bin/env python3
"""Replay packet C with the unmodified strict scorer, storing no native words."""
from contextlib import redirect_stdout
from hashlib import sha256
from io import StringIO
from pathlib import Path
import json
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score

score.ASM_DIR = ROOT / 'asm/us/boot_tail'
ADDRESSES = ['80015574', '800156E8', '80016BF8', '80016CE0', '80016E40', '80016F58', '80017018']
MATCHES = set(ADDRESSES) - {'800156E8'}
compiler_files = {p.name: sha256(p.read_bytes()).hexdigest() for p in sorted(score.IDO.iterdir()) if p.is_file()}
compiler_pin = sha256(json.dumps(compiler_files, sort_keys=True).encode()).hexdigest()
assert compiler_pin == '8ca550d30c1fef7c14b1470a1ef93cc6d415a507c6e18c04a7bfeaf8016a7b89', 'compiler differs from Packet 2 pin'
inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
extents = json.loads((score.ASM_DIR / 'extents.json').read_text())['functions']
assert len(inventory) == len(extents) == 439
assert sum(r['size'] for r in inventory) == 99120
assert {r['address']: r['size'] for r in inventory} == {r['address']: r['size'] for r in extents}
receipt = {'schema_version': 1, 'base_commit': '301d9e7552ad4fd7f54a38796db84671e1000d35',
           'target_manifest_sha256': sha256((score.ASM_DIR / 'SHA256SUMS').read_bytes()).hexdigest(),
           'scorer_sha256': sha256((ROOT / 'tools/cloud/score.py').read_bytes()).hexdigest(),
           'compiler_pin_sha256': compiler_pin, 'census_functions': len(inventory), 'results': []}
with tempfile.TemporaryDirectory(prefix='bt03-verify-') as directory:
    for address in ADDRESSES:
        name = 'func_' + address
        source = (ROOT / 'cloud/matches/boot_tail' if address in MATCHES else PACKET) / (name + '.c')
        row = {'name': name, 'bytes': len(score.targets()[name]) * 4,
               'source_path': str(source.relative_to(ROOT)),
               'source_sha256': sha256(source.read_bytes()).hexdigest(), 'trials': []}
        for level in (2, 1):
            flags = '-g0 -O%d -mips2 -G 0 -non_shared' % level
            obj = Path(directory) / (name + '-O%d.o' % level)
            score.compile_single(source, flags, obj)
            with redirect_stdout(StringIO()):
                comparison = score.compare(obj, name)
            strict = not (comparison.differing or comparison.extra_words or comparison.unresolved or comparison.unverified or comparison.errors)
            row['trials'].append({'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
                                  'differing_words': comparison.differing, 'total_words': comparison.total,
                                  'extra_words': comparison.extra_words, 'unresolved': comparison.unresolved,
                                  'unverified': comparison.unverified, 'errors': comparison.errors,
                                  'strict_match': strict})
        assert row['trials'][0]['strict_match'] == (address in MATCHES), name
        if address not in MATCHES:
            assert (row['trials'][0]['differing_words'], row['trials'][0]['extra_words']) == (8, 1)
        row['selected_flags'] = score.DEFAULT_FLAGS
        receipt['results'].append(row)

    getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
    obj = Path(directory) / 'existing_getter.o'
    score.compile_single(getter, score.DEFAULT_FLAGS, obj)
    assert score.compare(obj, getter.stem, show=0).accepted()
receipt['existing_getter_strict_match'] = True
print(json.dumps(receipt, indent=2))
