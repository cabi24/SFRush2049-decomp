#!/usr/bin/env python3
"""Strict source replay and read-only compiler/target/extent preflight."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score

ADDRESSES = ['800148F8', '80014D30', '800171C0', '8001729C',
             '800173B4', '80017410', '80017470', '800174D0']


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run():
    expected = json.loads((PACKET / 'preflight.json').read_text())
    compiler = {p.name: digest(p) for p in score.IDO.iterdir() if p.is_file()}
    if compiler != expected['compiler_files_sha256']:
        raise ValueError('IDO compiler-file pin mismatch')
    for path, wanted in expected['source_hashes'].items():
        if digest(ROOT / path) != wanted:
            raise ValueError('preflight source/target hash mismatch: ' + path)
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())
    left = {r['address']: r['size'] for r in inventory['functions']}
    right = {r['address']: r['size'] for r in extents['functions']}
    if left != right or len(left) != 439 or sum(left.values()) != 99120:
        raise ValueError('extent drift')
    targets = score.targets()
    results = []
    with tempfile.TemporaryDirectory(prefix='bt03-low-eight-replay-') as directory:
        tmp = Path(directory)
        getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
        score.compile_single(getter, score.DEFAULT_FLAGS, tmp / 'getter.o')
        if not score.compare(tmp / 'getter.o', getter.stem, show=0).accepted():
            raise ValueError('strict getter replay failed')
        for address in ADDRESSES:
            name = 'func_' + address
            source = ROOT / 'cloud/matches/boot_tail' / (name + '.c')
            native = targets[name]
            calls = [w for w in native if w >> 26 == 3 or
                     (w >> 26 == 0 and w & 63 == 9)]
            if calls:
                raise ValueError('unexpected native call in ' + name)
            row = {'function': name, 'bytes': len(native) * 4,
                   'source_path': str(source.relative_to(ROOT)),
                   'source_sha256': digest(source),
                   'target_sha256': hashlib.sha256(struct.pack('>%dI' % len(native), *native)).hexdigest(),
                   'native_direct_calls': 0, 'native_indirect_calls': 0, 'trials': []}
            for level in (2, 1):
                flags = '-g0 -O%d -mips2 -G 0 -non_shared' % level
                obj = tmp / (name + '-O%d.o' % level)
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                row['trials'].append({'flags': flags,
                    'effective_flags': flags + ' -Wab,-r4300_mul',
                    'differing_words': result.differing, 'total_words': result.total,
                    'extra_words': result.extra_words, 'unresolved': result.unresolved,
                    'unverified': result.unverified, 'errors': result.errors,
                    'strict_match': result.accepted()})
            if not row['trials'][0]['strict_match']:
                raise ValueError('strict O2 match failed for ' + name)
            if row['trials'][1]['strict_match']:
                raise ValueError('unexpected O1 control match for ' + name)
            results.append(row)
    return {'schema_version': 1, 'packet': 'BT03-low-eight',
            'base_commit': '301d9e75', 'result': 'PASS',
            'preflight': {'compiler_files_match': True, 'source_hashes_match': True,
                          'target_manifest_pass': True, 'getter_strict_match': True,
                          'extent_functions': 439, 'extent_bytes': 99120},
            'matched_functions': len(results), 'matched_bytes': sum(r['bytes'] for r in results),
            'results': results}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    text = json.dumps(run(), indent=2) + '\n'
    if args.output:
        args.output.write_text(text)
    else:
        print(text, end='')
