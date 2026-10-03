#!/usr/bin/env python3
"""Replay seven C13 tail reconstructions and bounded source controls."""
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


def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    sources = [(ROOT / 'cloud/matches/boot_tail/func_80021700.c', 'func_80021700', 'submitted')]
    sources += [(p, p.stem, 'needs-rodata-proof' if p.stem == 'func_80021548' else 'complete-nonmatch')
                for p in sorted((WORK / 'nonmatch').glob('*.c'))]
    sources += [(p, p.stem[:13], 'directed-control') for p in sorted((WORK / 'variants').glob('*.c'))]
    results = []
    for source, name, purpose in sources:
        want = score.targets()[name]
        for level in (2, 1):
            flags = '-g0 -O%d -mips2 -G 0 -non_shared' % level
            with tempfile.TemporaryDirectory(prefix='c13-tail-') as tmp:
                obj = Path(tmp) / 'function.o'
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                words = score.text_words(obj)
                assert score.symbols(obj) == {name: 0}
                relocated, masks, unresolved, unverified, errors = score.relocate(
                    obj, words, 0, min(len(want), len(words)) * 4, score.image_symbols())
                exact = (relocated[:len(want)] == want and len(words) >= len(want)
                         and not any(words[len(want):]) and not masks
                         and not unresolved and not unverified and not errors)
                expected = purpose == 'submitted' and level == 2
                if result.accepted() != expected or (expected and not exact):
                    raise ValueError('Unexpected strict status: ' + name + ' ' + flags)
                results.append({'name': name, 'purpose': purpose,
                    'source_path': str(source.relative_to(ROOT)), 'source_sha256': digest(source),
                    'flags': flags, 'effective_flags': flags + ' -Wab,-r4300_mul',
                    'target_bytes': len(want)*4, 'object_text_bytes': len(words)*4,
                    'target_body_sha256': hashlib.sha256(struct.pack('>%dI' % len(want), *want)).hexdigest(),
                    'differing_words': result.differing, 'total_words': result.total,
                    'extra_words': result.extra_words, 'unresolved': result.unresolved,
                    'unverified': result.unverified, 'errors': result.errors,
                    'masked_relocations': len(masks), 'strict_match': result.accepted(),
                    'full_relocated_equality': exact})
    paths = ['asm/us/boot_tail/SHA256SUMS', 'asm/us/boot_tail/boot_tail_8000f3a4.s',
             'asm/us/boot_tail/extents.json', 'asm/us/boot_tail/symbols.json',
             'specs/015-boot-tail-runtime/inventory.json', 'tools/cloud/score.py']
    return {'schema_version': 1, 'source_base': '301d9e7552ad4fd7f54a38796db84671e1000d35',
            'source_hashes': {p: digest(ROOT/p) for p in paths},
            'compiler_files_sha256': {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()},
            'results': results}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    text = json.dumps(run(), indent=2) + '\n'
    if args.check:
        if text != (WORK / 'scores.json').read_text():
            raise SystemExit('C13-tail receipt drift')
        print('C13-tail: one strict match, five complete nonmatches and one blocked source lead reproduced')
    else:
        print(text, end='')
