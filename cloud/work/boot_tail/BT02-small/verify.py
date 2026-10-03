#!/usr/bin/env python3
"""Strictly replay the nine BT02 sources and recheck immutable inputs."""
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

SUFFIXES = ('1061C', '10A0C', '10A14', '10D3C', '11894',
            '119E0', '11A10', '11A3C', '12200')
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def verify():
    target = ROOT / 'asm/us/boot_tail'
    score.ASM_DIR = target
    manifest = subprocess.run(['sha256sum', '-c', 'SHA256SUMS'], cwd=target,
                              check=True, text=True, capture_output=True)
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    extents = json.loads((target / 'extents.json').read_text())['functions']
    expected = {r['address']: r['size'] for r in inventory}
    actual = {r['address']: r['size'] for r in extents}
    assert len(expected) == len(actual) == 439
    assert expected == actual and sum(expected.values()) == 99120
    pinned = json.loads((ROOT / 'cloud/work/boot_tail/packet2/preflight.json').read_text())
    compiler = {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()}
    assert compiler == pinned['compiler_files_sha256'], 'compiler differs from pinned Packet 2 receipt'
    rows = []
    with tempfile.TemporaryDirectory(prefix='bt02-verify-') as tmp:
        for suffix in SUFFIXES:
            name = 'func_800' + suffix
            source = ROOT / 'cloud/matches/boot_tail' / (name + '.c')
            flags = re.fullmatch(r'/\* flags: (.*?) \*/', source.read_text().splitlines()[0]).group(1)
            assert flags == FLAGS
            obj = Path(tmp) / (name + '.o')
            score.compile_single(source, flags, obj)
            result = score.compare(obj, name, show=0)
            assert result.accepted(), name + ': ' + result.summary()
            rows.append({'name': name, 'native_bytes': expected['0x800' + suffix],
                         'source': str(source.relative_to(ROOT)), 'source_sha256': digest(source),
                         'flags': flags, 'effective_flags': flags + ' ' + score.R4300_CC,
                         'differing_words': result.differing, 'total_words': result.total,
                         'extra_words': result.extra_words, 'unresolved': result.unresolved,
                         'unverified': result.unverified, 'errors': result.errors,
                         'strict_match': result.accepted()})
            control_flags = flags.replace('-O2', '-O1')
            score.compile_single(source, control_flags, obj)
            control = score.compare(obj, name, show=0)
            rows[-1]['o1_control'] = {
                'flags': control_flags,
                'effective_flags': control_flags + ' ' + score.R4300_CC,
                'differing_words': control.differing, 'total_words': control.total,
                'extra_words': control.extra_words, 'unresolved': control.unresolved,
                'unverified': control.unverified, 'errors': control.errors,
                'strict_match': control.accepted()}
        getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
        obj = Path(tmp) / 'existing_getter.o'
        score.compile_single(getter, FLAGS, obj)
        baseline = score.compare(obj, getter.stem, show=0)
        assert baseline.accepted()
    return {'schema_version': 1, 'result': 'PASS',
            'packet_base': '76780b3a1b3e26c54b86e1f153344e92dac15b20',
            'manifest': manifest.stdout.splitlines(),
            'census': {'functions': 439, 'bytes': 99120, 'equal': True},
            'compiler_matches_packet2_pin': True,
            'existing_getter_strict_match': True,
            'new_matching_functions': len(rows),
            'new_matching_bytes': sum(r['native_bytes'] for r in rows),
            'matches': rows}


if __name__ == '__main__':
    print(json.dumps(verify(), indent=2))
