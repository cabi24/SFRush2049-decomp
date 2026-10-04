#!/usr/bin/env python3
"""Replay all six exact sources with the protected strict scorer."""
from contextlib import redirect_stdout
from hashlib import sha256
from io import StringIO
from pathlib import Path
import json
import struct
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score

score.ASM_DIR = ROOT / 'asm/us/boot_tail'
ADDRESSES = ['80014550', '80014594', '800145DC', '8001489C', '80014AF0', '80014CAC']
NONMATCHES = {'8001489C'}
EXPECTED = {'80014550': [(0, 0), (0, 0)], '80014594': [(0, 0), (15, 1)],
            '800145DC': [(0, 0), (18, 0)], '8001489C': [(11, 0), (22, 8)],
            '80014AF0': [(0, 0), (19, 10)], '80014CAC': [(0, 0), (17, 4)]}
compiler_files = {p.name: sha256(p.read_bytes()).hexdigest() for p in sorted(score.IDO.iterdir()) if p.is_file()}
compiler_pin = sha256(json.dumps(compiler_files, sort_keys=True).encode()).hexdigest()
assert compiler_pin == '8ca550d30c1fef7c14b1470a1ef93cc6d415a507c6e18c04a7bfeaf8016a7b89'
inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
extents = json.loads((score.ASM_DIR / 'extents.json').read_text())['functions']
assert len(inventory) == len(extents) == 439
assert sum(r['size'] for r in inventory) == 99120
assert {r['address']: r['size'] for r in inventory} == {r['address']: r['size'] for r in extents}
receipt = {'schema_version': 1, 'base_commit': '301d9e7552ad4fd7f54a38796db84671e1000d35',
           'target_manifest_sha256': sha256((score.ASM_DIR / 'SHA256SUMS').read_bytes()).hexdigest(),
           'scorer_sha256': sha256((ROOT / 'tools/cloud/score.py').read_bytes()).hexdigest(),
           'compiler_pin_sha256': compiler_pin, 'census_functions': len(inventory), 'census_bytes': 99120,
           'results': []}
assert receipt['scorer_sha256'] == '9de8385b28938c012ba679ea1f899cd7e5fe351d2c51129c5319442c84b679c4'
assert receipt['target_manifest_sha256'] == '6bd3e6ef05e6123e4ea4dd0d814c0abb16f202a91d8d0a381eca14322c8a36d5'
with tempfile.TemporaryDirectory(prefix='bt03-low-medium-verify-') as directory:
    for address in ADDRESSES:
        name = 'func_' + address
        source = (PACKET if address in NONMATCHES else ROOT / 'cloud/matches/boot_tail') / (name + '.c')
        native = score.targets()[name]
        row = {'name': name, 'native_bytes': len(native) * 4,
               'target_words_sha256': sha256(struct.pack('>%dI' % len(native), *native)).hexdigest(),
               'source_path': str(source.relative_to(ROOT)),
               'source_sha256': sha256(source.read_bytes()).hexdigest(), 'trials': []}
        for trial, level in enumerate((2, 1)):
            flags = '-g0 -O%d -mips2 -G 0 -non_shared' % level
            obj = Path(directory) / (name + '-O%d.o' % level)
            score.compile_single(source, flags, obj)
            with redirect_stdout(StringIO()):
                comparison = score.compare(obj, name)
            words = score.text_words(obj)
            relocated, masks, unresolved, unverified, errors = score.relocate(
                obj, words, 0, len(native) * 4, score.image_symbols())
            strict = comparison.accepted()
            assert score.symbols(obj) == {name: 0}
            assert (comparison.differing, comparison.extra_words) == EXPECTED[address][trial]
            assert not comparison.unresolved and not comparison.unverified and not comparison.errors
            assert not masks and not unresolved and not unverified and not errors
            if strict:
                assert relocated[:len(native)] == native and not any(words[len(native):])
            row['trials'].append({'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
                                  'object_sha256': sha256(obj.read_bytes()).hexdigest(),
                                  'candidate_text_words': len(words),
                                  'differing_words': comparison.differing, 'total_words': comparison.total,
                                  'extra_words': comparison.extra_words, 'unresolved': comparison.unresolved,
                                  'unverified': comparison.unverified, 'errors': comparison.errors,
                                  'strict_match': strict, 'relocation_masks': 0,
                                  'sole_body_symbol': True})
        assert row['trials'][0]['strict_match'] == (address not in NONMATCHES)
        row['selected_flags'] = score.DEFAULT_FLAGS
        receipt['results'].append(row)
    getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
    obj = Path(directory) / 'existing_getter.o'
    score.compile_single(getter, score.DEFAULT_FLAGS, obj)
    assert score.compare(obj, getter.stem, show=0).accepted()
receipt['existing_getter_strict_match'] = True
receipt['strict_match_functions'] = 5
receipt['strict_match_bytes'] = 360
receipt['complete_nonmatch_functions'] = 1
receipt['complete_nonmatch_bytes'] = 92
print(json.dumps(receipt, indent=2))
