#!/usr/bin/env python3
"""Replay both bounded BT02 audio-pair nonmatches with fixed O2/O1 controls."""
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
    manifest = subprocess.check_output(['sha256sum', '-c', 'SHA256SUMS'], cwd=score.ASM_DIR, text=True)
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())['functions']
    expected = {r['address']: r['size'] for r in inventory}
    actual = {r['address']: r['size'] for r in extents}
    assert expected == actual and len(actual) == 439 and sum(actual.values()) == 99120
    rows = []
    controls = []
    with tempfile.TemporaryDirectory(prefix='bt02-audio-pair-verify-') as directory:
        for suffix in ['11F60', '123A8']:
            name = 'func_800' + suffix
            source = PACKET / 'nonmatch' / (name + '.c')
            assert source.read_text().splitlines()[0] == '/* flags: ' + FLAGS + ' */'
            row = {'name': name, 'native_bytes': expected['0x800' + suffix],
                   'source': str(source.relative_to(ROOT)), 'source_sha256': digest(source), 'flag_results': []}
            for flags in [FLAGS, FLAGS.replace('-O2', '-O1')]:
                obj = Path(directory) / (name + '.o')
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                assert not (result.unresolved or result.unverified or result.errors)
                assert not result.accepted()
                if flags == FLAGS:
                    assert result.differing == 74 and result.extra_words == 0
                row['flag_results'].append({'flags': flags, 'effective_flags': flags + ' ' + score.R4300_CC, **result_record(result)})
            rows.append(row)
        for control in json.loads((PACKET / 'variants.json').read_text())['results']:
            source = ROOT / control['source']
            assert digest(source) == control['source_sha256']
            obj = Path(directory) / 'control.o'
            score.compile_single(source, FLAGS, obj)
            result = score.compare(obj, 'func_' + control['address'], show=0)
            assert result.differing == control['diff'] and result.extra_words == control['extra']
            assert not (result.unresolved or result.unverified or result.errors)
            controls.append({'address': control['address'], 'variant': control['variant'], **result_record(result)})
        getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
        obj = Path(directory) / 'getter.o'
        score.compile_single(getter, FLAGS, obj)
        assert score.compare(obj, getter.stem, show=0).accepted()
    return {'schema_version': 1, 'result': 'PASS', 'base': 'ea50bee96b5d732ba9efcac8f433134090f41950',
            'central_claim_commit': '1535b793', 'manifest': manifest.splitlines(),
            'compiler_matches_packet2_pin': True, 'census': {'functions': 439, 'bytes': 99120, 'equal': True},
            'existing_getter_strict_match': True, 'strict_match_functions': 0, 'strict_match_bytes': 0,
            'nonmatch_functions': 2, 'nonmatch_bytes': 1232, 'sources': rows, 'controls': controls}

if __name__ == '__main__':
    print(json.dumps(verify(), indent=2))
