#!/usr/bin/env python3
"""Strictly replay the BT02 context pair and its bounded source controls."""
import hashlib
import json
from pathlib import Path
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
    for name, expected in pins['source_hashes'].items():
        assert digest(ROOT / name) == expected, name
    actual = {p.name: digest(p) for p in score.IDO.iterdir() if p.is_file()}
    assert actual == pins['compiler_files_sha256'], 'pinned compiler differs'
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    manifest = subprocess.check_output(['sha256sum', '-c', 'SHA256SUMS'],
                                       cwd=score.ASM_DIR, text=True).splitlines()
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())['functions']
    expected = {r['address']: r['size'] for r in inventory}
    actual = {r['address']: r['size'] for r in extents}
    assert expected == actual and len(actual) == 439
    assert sum(actual.values()) == 99120
    rows = []
    with tempfile.TemporaryDirectory(prefix='bt02-context-verify-') as temporary:
        obj = Path(temporary) / 'candidate.o'
        for suffix, source in [
            ('10A40', ROOT / 'cloud/matches/boot_tail/func_80010A40.c'),
            ('10E80', PACKET / 'nonmatch/func_80010E80.c')]:
            name = 'func_800' + suffix
            assert source.read_text().splitlines()[0] == '/* flags: ' + FLAGS + ' */'
            score.compile_single(source, FLAGS, obj)
            result = score.compare(obj, name, show=0)
            assert not (result.unresolved or result.unverified or result.errors)
            assert result.extra_words == 0
            assert result.differing == (0 if suffix == '10A40' else 53)
            assert result.accepted() == (suffix == '10A40')
            row = {'name': name, 'native_bytes': expected['0x800' + suffix],
                   'source': str(source.relative_to(ROOT)),
                   'source_sha256': digest(source), 'flags': FLAGS,
                   'effective_flags': FLAGS + ' ' + score.R4300_CC,
                   **result_record(result)}
            score.compile_single(source, FLAGS.replace('-O2', '-O1'), obj)
            row['o1_control'] = result_record(score.compare(obj, name, show=0))
            rows.append(row)
        controls = []
        for archived in ['initial_controls.json', 'refinement_controls.json']:
            for row in json.loads((PACKET / archived).read_text()):
                if archived == 'initial_controls.json':
                    name = row['name']
                    variant = 'baseline'
                    flags = row['flags']
                else:
                    suffix, variant = row['variant'].split('_', 1)
                    name = 'func_800' + suffix
                    flags = FLAGS
                source = PACKET / 'controls' / (name + '_' + variant + '.c')
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                assert result_record(result) == {key: row[key] for key in result_record(result)}, (name, variant)
                controls.append({'source': str(source.relative_to(ROOT)),
                                 'source_sha256': digest(source), 'flags': flags,
                                 **result_record(result)})
        getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
        score.compile_single(getter, FLAGS, obj)
        assert score.compare(obj, getter.stem, show=0).accepted()
        score.compile_single(PACKET / 'layout_check.c', FLAGS, obj)
    return {'schema_version': 1, 'result': 'PASS',
            'base': '301d9e7552ad4fd7f54a38796db84671e1000d35',
            'claim_commit': '425bfa4d', 'manifest': manifest,
            'compiler_matches_packet2_pin': True,
            'census': {'functions': 439, 'bytes': 99120, 'equal': True},
            'getter_strict_match': True, 'native_layout_assertions': 'PASS',
            'strict_match_functions': 1, 'strict_match_bytes': 552,
            'nonmatch_functions': 1, 'nonmatch_bytes': 500,
            'sources': rows, 'controls': controls,
            'packet_file_hashes': {str(path.relative_to(PACKET)): digest(path)
                                  for path in sorted(PACKET.rglob('*'))
                                  if path.is_file() and path.name not in
                                  ('verification.json', 'independent_review.json')
                                  and '__pycache__' not in path.parts}}

if __name__ == '__main__':
    print(json.dumps(verify(), indent=2))
