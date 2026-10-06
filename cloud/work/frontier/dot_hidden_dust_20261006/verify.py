#!/usr/bin/env python3
"""Replay the bounded Hidden source-boundary falsification; never claims a match."""
import argparse
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import shutil
import struct
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

FN = 'dust_cloud_effect'
BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
SOURCES = ('baseline.c', 'candidate.c', 'baseline_selector.c', 'candidate_selector.c')

def digest(data):
    return hashlib.sha256(data).hexdigest()

def packed(words):
    return struct.pack('>%dI' % len(words), *words)

def identity(directory=HERE):
    return {
        'base_commit': BASE,
        'source_sha256': {name: digest((directory / name).read_bytes()) for name in SOURCES},
        'verifier_sha256': digest((directory / 'verify.py').read_bytes()),
        'native_sha256': digest(packed(score.targets()[FN])),
        'native_bytes': len(score.targets()[FN]) * 4,
        'function': FN,
        'flags': FLAGS,
        'single_backend_flag': score.R4300_CC,
        'group_backend_flag': score.R4300_AS1,
    }

def inspect(obj):
    data, sections = score._elf(obj)
    ti = score._text_index(sections)
    symbols = [s for i, sec in enumerate(sections) if sec['type'] == 2
               for s in score._symbol_table(data, sections, i)]
    fn, = [s for s in symbols if s['name'] == FN and s['type'] == 2 and s['section'] == ti]
    text = sections[ti]
    words = list(struct.unpack('>%dI' % (text['size'] // 4),
                              data[text['off']:text['off'] + text['size']]))
    start, size = fn['value'], fn['size']
    relocated, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start + size, score.image_symbols())
    assert not (masks or unresolved or unverified or errors), (masks, unresolved, unverified, errors)
    body = relocated[start // 4:(start + size) // 4]
    native = score.targets()[FN]
    storage = {s['name']: s['size'] for s in sections
               if s['name'] in ('.data', '.bss', '.rodata', '.sdata', '.sbss') and s['size']}
    assert not storage, storage
    assert body[0] >> 16 == 0x27bd
    full_differences = sum(a != b for a, b in zip(native, body)) + abs(len(native) - len(body))
    result = {
        'canonical': asdict(score.compare(obj, FN, show=0)),
        'elf_function_bytes': size,
        'native_bytes': len(native) * 4,
        'full_extent_differing_positions': full_differences,
        'missing_words': max(0, len(native) - len(body)),
        'excess_words': max(0, len(body) - len(native)),
        'frame_bytes': (-body[0]) & 0xffff,
        'full_relocated_body_sha256': digest(packed(body)),
        'all_body_relocations_resolved': True,
        'owned_storage_sections': storage,
        'strict_match': body == native,
    }
    assert not result['strict_match']
    return result

def compile_replay():
    if not (score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        raise SystemExit('pinned IDO and MIPS GNU linker required')
    result = {}
    with tempfile.TemporaryDirectory(prefix='hidden-dust-') as tmp:
        directory = Path(tmp)
        for name in SOURCES:
            single = directory / (name + '.o')
            score.compile_single(HERE / name, FLAGS, single)
            result['single/' + name] = inspect(single)
            group = directory / name
            group.mkdir()
            (group / 'group.c').write_text((HERE / name).read_text())
            (group / 'group.json').write_text(json.dumps({
                'files': ['group.c'], 'flags': FLAGS, 'keep': [FN], 'claims': []}))
            grouped = directory / (name + '.group.o')
            score.compile_group(group, grouped)
            result['group/' + name] = inspect(grouped)
    for route in ('single', 'group'):
        for suffix in ('.c', '_selector.c'):
            assert result[route + '/baseline' + suffix] == result[route + '/candidate' + suffix]
    return result

def check_identity(receipt, directory=HERE):
    assert receipt['identity'] == identity(directory), 'packet source or selected native body changed'
    assert receipt['status'] == 'NONMATCH_SOURCE_BOUNDARY_FALSIFIED'
    assert receipt['claims'] == []
    return True

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler', action='store_true')
    parser.add_argument('--record', action='store_true')
    args = parser.parse_args()
    path = HERE / 'verification.json'
    if args.record:
        receipt = {'schema': 1, 'status': 'NONMATCH_SOURCE_BOUNDARY_FALSIFIED', 'claims': [],
                   'identity': identity(), 'replay': compile_replay(),
                   'compiler_provenance': {name: digest((score.IDO / name).read_bytes())
                                           for name in ('cc', 'cfe', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1')}}
        path.write_text(json.dumps(receipt, indent=2, sort_keys=True) + '\n')
    receipt = json.loads(path.read_text())
    check_identity(receipt)
    if args.compiler and not args.record:
        assert compile_replay() == receipt['replay'], 'fresh replay differs from bounded receipt'
    print('Verified: Hidden changes no complete callback bytes; every control remains NONMATCH.')

if __name__ == '__main__':
    main()
