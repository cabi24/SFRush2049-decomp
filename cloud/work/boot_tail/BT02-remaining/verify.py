#!/usr/bin/env python3
"""Replay all eight actual BT02 remaining sources, including fixed O1 controls."""
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score

MATCHES = ('10980', '109C0', '11074', '110C4', '118C0', '11CD8', '14488', '144F0')
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def result_record(result):
    return {'strict_match': result.accepted(), 'differing_words': result.differing,
            'total_words': result.total, 'extra_words': result.extra_words,
            'unresolved': result.unresolved, 'unverified': result.unverified,
            'errors': result.errors}


def verify():
    pins = json.loads((PACKET / 'input_pins.json').read_text())
    for path, expected in pins['source_hashes'].items():
        assert digest(ROOT / path) == expected, path
    compiler = {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()}
    assert compiler == pins['compiler_files_sha256'], 'compiler pin differs'
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    manifest = subprocess.check_output(['sha256sum', '-c', 'SHA256SUMS'],
                                       cwd=score.ASM_DIR, text=True)
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())['functions']
    expected = {r['address']: r['size'] for r in inventory}
    actual = {r['address']: r['size'] for r in extents}
    assert len(expected) == len(actual) == 439
    assert expected == actual and sum(expected.values()) == 99120
    rows = []
    with tempfile.TemporaryDirectory(prefix='bt02-remaining-verify-') as tmp:
        for suffix in sorted(MATCHES):
            name = 'func_800' + suffix
            source = ROOT / 'cloud/matches/boot_tail' / (name + '.c')
            flags = re.fullmatch(r'/\* flags: (.*?) \*/', source.read_text().splitlines()[0]).group(1)
            assert flags == FLAGS
            obj = Path(tmp) / (name + '.o')
            score.compile_single(source, flags, obj)
            result = score.compare(obj, name, show=0)
            assert result.accepted(), name + ': ' + result.summary()
            row = {'name': name, 'native_bytes': expected['0x800' + suffix],
                   'source': str(source.relative_to(ROOT)), 'source_sha256': digest(source),
                   'flags': flags, 'effective_flags': flags + ' ' + score.R4300_CC,
                   **result_record(result)}
            control_flags = flags.replace('-O2', '-O1')
            score.compile_single(source, control_flags, obj)
            row['o1_control'] = {'flags': control_flags,
                                 **result_record(score.compare(obj, name, show=0))}
            rows.append(row)
        getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
        obj = Path(tmp) / 'getter.o'
        score.compile_single(getter, FLAGS, obj)
        assert score.compare(obj, getter.stem, show=0).accepted()
    return {'schema_version': 1, 'result': 'PASS',
            'base': '301d9e7552ad4fd7f54a38796db84671e1000d35',
            'claim_commit': 'b7d5cab0', 'manifest': manifest.splitlines(),
            'compiler_matches_packet2_pin': True,
            'census': {'functions': 439, 'bytes': 99120, 'equal': True},
            'existing_getter_strict_match': True,
            'strict_match_functions': len(MATCHES),
            'strict_match_bytes': sum(r['native_bytes'] for r in rows if r['strict_match']),
            'nonmatch_functions': 0,
            'nonmatch_bytes': sum(r['native_bytes'] for r in rows if not r['strict_match']),
            'sources': rows,
            'harness_hashes': {p.name: digest(p) for p in
                               (PACKET / 'verify.py', PACKET / 'test_host.py',
                                PACKET / 'host_behavior.c')}}


if __name__ == '__main__':
    print(json.dumps(verify(), indent=2))
