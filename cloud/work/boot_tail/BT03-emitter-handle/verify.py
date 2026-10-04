#!/usr/bin/env python3
"""Reproduce one exact emitter caller and its O1 control and bounded O2/O1 controls."""
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

FINAL = {'8001DDE0': (736, True, 0)}


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
            with tempfile.TemporaryDirectory(prefix='bt03-emitter-handle-') as tmp:
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
                data, sections = score._elf(obj)
                text_index = score._text_index(sections)
                functions = [sym for i, section in enumerate(sections) if section['type'] == 2
                             for sym in score._symbol_table(data, sections, i)
                             if sym['section'] == text_index and sym['type'] == 2]
                if len(functions) != 1 or functions[0]['name'] != name or functions[0]['value'] != 0:
                    raise ValueError('ELF function identity drift')
                if result.accepted() and functions[0]['size'] != size:
                    raise ValueError('ELF function extent mismatch')
                equality = relocated[:len(targets[name])] == targets[name]
                if result.accepted() and (not equality or masks or unresolved or unverified or errors
                                          or any(relocated[len(targets[name]):])):
                    raise ValueError('strict equality/proof failure')
                if purpose == 'retained' and level == 2 and (masks or unresolved or unverified or errors):
                    raise ValueError('retained O2 relocation uncertainty')
                results.append({'name': name, 'purpose': purpose,
                    'source_path': str(source.relative_to(ROOT)), 'source_sha256': digest(source),
                    'target_sha256': hashlib.sha256(struct.pack('>%dI' % len(targets[name]), *targets[name])).hexdigest(),
                    'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
                    'native_bytes': size, 'elf_function_bytes': functions[0]['size'], 'object_text_bytes': len(words) * 4,
                    'differing_words': result.differing, 'total_words': result.total,
                    'extra_words': result.extra_words, 'unresolved': result.unresolved,
                    'unverified': result.unverified, 'errors': result.errors,
                    'masked_relocations': len(masks), 'relocated_full_word_equality': equality,
                    'strict_match': result.accepted()})
    paths = ['asm/us/boot_tail/SHA256SUMS', 'asm/us/boot_tail/boot_tail_8000f3a4.s',
             'asm/us/boot_tail/extents.json', 'asm/us/boot_tail/symbols.json',
             'specs/015-boot-tail-runtime/inventory.json', 'tools/cloud/score.py']
    return {'schema_version': 1, 'master_reference': '301d9e7552ad4fd7f54a38796db84671e1000d35',
            'base': '01eba3fdaf662189ca1fbe4275529c0be491541e',
            'functions': 1, 'attempted_bytes': 736, 'matching_functions': 1,
            'matching_bytes': 736, 'complete_nonmatches': 0, 'nonmatching_bytes': 0,
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
            raise SystemExit('BT03-emitter-handle receipt drift')
        print('BT03-emitter-handle: exact full body / 736 B; both rows reproduced')
    else:
        print(text, end='')
