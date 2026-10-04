#!/usr/bin/env python3
"""Strictly reproduce all five first-try dispatch bodies and O1 controls."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score

FINAL = {'800150C8': (88, True, 0), '80015120': (88, True, 0),
         '80015178': (88, True, 0), '800151D0': (88, True, 0),
         '800152A8': (112, True, 0)}


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    targets = score.targets()
    sources = []
    for address, (_, match, _) in FINAL.items():
        name = 'func_' + address
        source = (ROOT / 'cloud/matches/boot_tail' / (name + '.c') if match else
                  WORK / (name + '_NONMATCH.c'))
        sources.append((source, 'retained'))
    sources += [(p, 'control') for p in sorted((WORK / 'controls').glob('*.c'))]
    results = []
    for source, purpose in sources:
        name = source.stem[:13]
        size, match, differing = FINAL[name[5:]]
        if len(targets[name]) * 4 != size:
            raise ValueError('extent drift')
        for level in (2, 1):
            flags = '-g0 -O%d -mips2 -G 0 -non_shared' % level
            with tempfile.TemporaryDirectory(prefix='low-dispatch-') as tmp:
                obj = Path(tmp) / 'candidate.o'
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                words = score.text_words(obj)
                relocated, masks, unresolved, unverified, errors = score.relocate(
                    obj, words, 0, len(words) * 4, score.image_symbols())
                if score.symbols(obj) != {name: 0}:
                    raise ValueError('unexpected function boundary')
                if purpose == 'retained' and level == 2:
                    if (result.accepted(), result.differing) != (match, differing):
                        raise ValueError('retained result drift: ' + name)
                equality = relocated[:len(targets[name])] == targets[name]
                if result.accepted() and (not equality or masks or unresolved or unverified or errors
                                          or any(relocated[len(targets[name]):])):
                    raise ValueError('strict equality/proof failure')
                if result.unresolved or result.unverified or result.errors:
                    raise ValueError('relocation uncertainty')
                results.append({'name': name, 'purpose': purpose,
                    'source_path': str(source.relative_to(ROOT)), 'source_sha256': digest(source),
                    'target_sha256': hashlib.sha256(struct.pack('>%dI' % len(targets[name]), *targets[name])).hexdigest(),
                    'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
                    'native_bytes': size, 'object_text_bytes': len(words) * 4,
                    'differing_words': result.differing, 'total_words': result.total,
                    'extra_words': result.extra_words, 'unresolved': result.unresolved,
                    'unverified': result.unverified, 'errors': result.errors,
                    'masked_relocations': len(masks), 'relocated_full_word_equality': equality,
                    'strict_match': result.accepted()})
    paths = ['asm/us/boot_tail/SHA256SUMS', 'asm/us/boot_tail/boot_tail_8000f3a4.s',
             'asm/us/boot_tail/extents.json', 'asm/us/boot_tail/symbols.json',
             'specs/015-boot-tail-runtime/inventory.json', 'tools/cloud/score.py']
    return {'schema_version': 1, 'base': '301d9e7552ad4fd7f54a38796db84671e1000d35',
            'functions': 5, 'attempted_bytes': 464, 'matching_functions': 5,
            'matching_bytes': 464, 'complete_nonmatches': 0, 'nonmatching_bytes': 0,
            'input_sha256': {p: digest(ROOT / p) for p in paths},
            'compiler_sha256': {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()},
            'results': results}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    text = json.dumps(run(), indent=2) + '\n'
    if args.check:
        if text != (WORK / 'verification.json').read_text():
            raise SystemExit('BT03-low-dispatch receipt drift')
        print('BT03-low-dispatch: 5 strict matches / 464 B; all O1 controls reproduced')
    else:
        print(text, end='')
