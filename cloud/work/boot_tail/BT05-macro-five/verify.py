#!/usr/bin/env python3
"""Strict full-relocation replay of the bounded BT05 macro-five research packet."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
EXTENTS = {'80022324': 172, '800230B0': 224, '80023AD4': 124,
           '80023B50': 140, '80023DB8': 228}
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
BASE = '301d9e7552ad4fd7f54a38796db84671e1000d35'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())['functions']
    native = {x['address']: x['size'] for x in extents}
    assert len(inventory) == len(native) == 439
    assert {x['address']: x['size'] for x in inventory} == native
    assert sum(native.values()) == 99120
    indexed = {x['name']: x for x in inventory}
    results = []
    with tempfile.TemporaryDirectory(prefix='bt05-macro-five-') as tmp:
        for source in sorted(WORK.glob('nonmatch/*.c')) + sorted(WORK.glob('variants/*.c')):
            name = source.stem[:13]
            address = name[5:]
            size = EXTENTS[address]
            assert indexed[name]['scope'] == 'in_scope'
            assert indexed[name]['size'] == size == len(score.targets()[name]) * 4
            before = source.read_bytes()
            assert before.splitlines()[0] == ('/* flags: ' + FLAGS + ' */').encode()
            for flags in (FLAGS, FLAGS.replace('-O2', '-O1')):
                obj = Path(tmp) / 'candidate.o'
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                words = score.text_words(obj)
                resolved, masks, unresolved, unverified, errors = score.relocate(
                    obj, words, 0, len(words) * 4, score.image_symbols())
                want = score.targets()[name]
                exact = (len(words) >= len(want) and resolved[:len(want)] == want
                         and not any(resolved[len(want):]) and not masks
                         and not unresolved and not unverified and not errors)
                assert before == source.read_bytes()
                assert not result.accepted() and not exact, 'unexpected new equality requires review'
                if source.parent.name == 'nonmatch' and flags == FLAGS:
                    assert not unresolved and not unverified and not errors and not masks
                results.append({'name': name, 'bytes': size,
                    'source_path': str(source.relative_to(ROOT)),
                    'source_sha256': digest(source), 'flags': flags,
                    'effective_flags': flags + ' -Wab,-r4300_mul',
                    'differing_words': result.differing, 'total_words': result.total,
                    'extra_words': result.extra_words, 'unresolved': result.unresolved,
                    'unverified': result.unverified, 'errors': result.errors,
                    'strict_match': result.accepted(), 'full_relocated_equality': exact})
    return {'schema_version': 1, 'result': 'COMPLETE_NONMATCH_RESEARCH',
        'base_commit': BASE, 'functions': 5, 'native_bytes': 888,
        'matching_functions': 0, 'verified_body_bytes': 0,
        'extents_equal': True, 'census_functions': 439, 'census_bytes': 99120,
        'target_manifest_sha256': digest(score.ASM_DIR / 'SHA256SUMS'),
        'score_sha256': digest(ROOT / 'tools/cloud/score.py'), 'results': results}


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
