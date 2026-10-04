#!/usr/bin/env python3
"""Replay final sources with the unchanged strict relocated full-word scorer."""
from contextlib import redirect_stdout
from hashlib import sha256
from io import StringIO
from pathlib import Path
import json
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score

score.ASM_DIR = ROOT / 'asm/us/boot_tail'
SOURCES = {
    'func_80015720': PACKET / 'func_80015720_NONMATCH.c',
    'func_800164D0': ROOT / 'cloud/matches/boot_tail/func_800164D0.c',
}
EXPECTED = {'func_80015720': [(6, 0), (108, 44)], 'func_800164D0': [(0, 0), (82, 25)]}
def digest(path):
    return sha256(path.read_bytes()).hexdigest()

compiler_files = {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()}
compiler_pin = sha256(json.dumps(compiler_files, sort_keys=True).encode()).hexdigest()
assert compiler_pin == '8ca550d30c1fef7c14b1470a1ef93cc6d415a507c6e18c04a7bfeaf8016a7b89'
manifest = score.target_manifest()
for filename in manifest:
    score.verified_bytes(score.ASM_DIR / filename, manifest)
inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
extents = json.loads((score.ASM_DIR / 'extents.json').read_text())['functions']
assert len(inventory) == len(extents) == 439
assert sum(r['size'] for r in inventory) == 99120
assert {r['address']: r['size'] for r in inventory} == {r['address']: r['size'] for r in extents}
receipt = {'schema_version': 1, 'base_commit': '301d9e7552ad4fd7f54a38796db84671e1000d35',
           'target_manifest_sha256': digest(score.ASM_DIR / 'SHA256SUMS'),
           'scorer_sha256': digest(ROOT / 'tools/cloud/score.py'),
           'compiler_pin_sha256': compiler_pin, 'census_functions': len(inventory), 'census_bytes': 99120,
           'results': []}
assert receipt['scorer_sha256'] == '9de8385b28938c012ba679ea1f899cd7e5fe351d2c51129c5319442c84b679c4'
assert receipt['target_manifest_sha256'] == '6bd3e6ef05e6123e4ea4dd0d814c0abb16f202a91d8d0a381eca14322c8a36d5'
with tempfile.TemporaryDirectory(prefix='bt03-low-larger-verify-') as directory:
    for name, source in SOURCES.items():
        native = score.targets()[name]
        row = {'name': name, 'native_bytes': len(native) * 4,
               'target_words_sha256': sha256(struct.pack('>%dI' % len(native), *native)).hexdigest(),
               'source_path': str(source.relative_to(ROOT)), 'source_sha256': digest(source), 'trials': []}
        for trial, level in enumerate((2, 1)):
            flags = '-g0 -O%d -mips2 -G 0 -non_shared' % level
            obj = Path(directory) / (name + '-O%d.o' % level)
            score.compile_single(source, flags, obj)
            with redirect_stdout(StringIO()):
                comparison = score.compare(obj, name)
            words = score.text_words(obj)
            relocated, masks, unresolved, unverified, errors = score.relocate(
                obj, words, 0, len(native) * 4, score.image_symbols())
            assert score.symbols(obj) == {name: 0}
            assert (comparison.differing, comparison.extra_words) == EXPECTED[name][trial]
            assert not masks and not unresolved and not unverified and not errors
            assert not comparison.unresolved and not comparison.unverified and not comparison.errors
            if comparison.accepted():
                assert relocated[:len(native)] == native and not any(words[len(native):])
            row['trials'].append({'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
                'object_sha256': digest(obj), 'candidate_text_words': len(words),
                'differing_words': comparison.differing, 'total_words': comparison.total,
                'extra_words': comparison.extra_words, 'unresolved': [], 'unverified': [], 'errors': [],
                'strict_match': comparison.accepted(), 'relocation_masks': 0, 'sole_body_symbol': True})
        assert row['trials'][0]['strict_match'] == (name == 'func_800164D0')
        row['selected_flags'] = score.DEFAULT_FLAGS
        receipt['results'].append(row)
    getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
    obj = Path(directory) / 'existing_getter.o'
    score.compile_single(getter, score.DEFAULT_FLAGS, obj)
    with redirect_stdout(StringIO()):
        assert score.compare(obj, getter.stem).accepted()
receipt.update(existing_getter_strict_match=True, strict_match_functions=1, strict_match_bytes=332,
               complete_nonmatch_functions=1, complete_nonmatch_bytes=440)
print(json.dumps(receipt, indent=2))
