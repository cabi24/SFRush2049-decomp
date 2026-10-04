#!/usr/bin/env python3
"""Replay pinned strict scores; all native objects remain temporary."""
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

def scored(source, name, flags, obj):
    score.compile_single(source, flags, obj)
    result = score.compare(obj, name, show=0)
    data, sections = score._elf(obj)
    symbol = next(symbol for index, section in enumerate(sections) if section['type'] == 2
                  for symbol in score._symbol_table(data, sections, index)
                  if symbol['name'] == name)
    return {'strict_match': result.accepted(), 'differing_words': result.differing,
            'total_words': result.total, 'extra_words': result.extra_words,
            'unresolved': result.unresolved, 'unverified': result.unverified,
            'errors': result.errors, 'symbol_bytes': symbol['size']}

def verify():
    pins = json.loads((PACKET / 'input_pins.json').read_text())
    for path, expected in pins['source_hashes'].items():
        assert digest(ROOT / path) == expected, path
    assert {p.name: digest(p) for p in score.IDO.iterdir() if p.is_file()} == pins['compiler_files_sha256']
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    manifest = subprocess.check_output(['sha256sum', '-c', 'SHA256SUMS'], cwd=score.ASM_DIR, text=True).splitlines()
    inventory = json.loads((ROOT / 'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    extents = json.loads((score.ASM_DIR / 'extents.json').read_text())['functions']
    native = {r['address']: r['size'] for r in inventory}
    assert native == {r['address']: r['size'] for r in extents}
    assert len(native) == 439 and sum(native.values()) == 99120
    sources, controls = [], []
    with tempfile.TemporaryDirectory(prefix='bt02-audio-remaining-') as directory:
        obj = Path(directory) / 'candidate.o'
        getter = ROOT / 'cloud/matches/boot_tail/func_80010A00.c'
        assert scored(getter, getter.stem, FLAGS, obj)['strict_match']
        for address, different, size in [('80013DEC', 164, 828), ('80014198', 13, 480)]:
            name = 'func_' + address
            source = PACKET / 'nonmatch' / (name + '.c')
            assert source.read_text().splitlines()[0] == '/* flags: ' + FLAGS + ' */'
            result = scored(source, name, FLAGS, obj)
            assert result['differing_words'] == different and result['symbol_bytes'] == size
            assert not result['strict_match']
            assert not (result['unresolved'] or result['unverified'] or result['errors'])
            sources.append({'name': name, 'source': str(source.relative_to(ROOT)), 'source_sha256': digest(source),
                            'native_bytes': native['0x' + address], 'flags': FLAGS,
                            'effective_flags': FLAGS + ' ' + score.R4300_CC,
                            'o2': result, 'o1': scored(source, name, FLAGS.replace('-O2', '-O1'), obj)})
            baseline = PACKET / 'controls' / (name + '_baseline.c')
            controls.append({'name': name, 'variant': 'baseline', 'source_sha256': digest(baseline),
                             'o2': scored(baseline, name, FLAGS, obj),
                             'o1': scored(baseline, name, FLAGS.replace('-O2', '-O1'), obj)})
        for row in json.loads((PACKET / 'variants.json').read_text()):
            source = ROOT / row['source']
            assert digest(source) == row['source_sha256']
            result = scored(source, row['name'], row['flags'], obj)
            assert result == {key: row[key] for key in result}, row['variant']
            controls.append({'name': row['name'], 'variant': row['variant'], 'source_sha256': digest(source), **result})
    return {'result': 'PASS', 'base': '96b9dd1979f7c797f97a1dd2317fc0852092c0dc',
            'activation_commit': '9a542435', 'base_correction_commit': 'c3b07951',
            'manifest': manifest, 'compiler_matches_packet2_pin': True,
            'census': {'functions': 439, 'bytes': 99120, 'equal': True},
            'existing_getter_strict_match': True,
            'strict_match_functions': 0, 'strict_match_bytes': 0,
            'nonmatch_functions': 2, 'nonmatch_bytes': 1256,
            'sources': sources, 'controls': controls}

if __name__ == '__main__':
    print(json.dumps(verify(), indent=2))
