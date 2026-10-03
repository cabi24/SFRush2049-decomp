#!/usr/bin/env python3
"""Reproduce C13 strict-score receipts without storing objects or target words."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

NAMES = ['func_%08X' % (0x80021428 + 32 * i) for i in range(9)]
FLAGS = '-g0 -O%d -mips2 -G 0 -non_shared'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    targets = score.targets()
    rows = []
    for index, name in enumerate(NAMES):
        source = ROOT / ('cloud/matches/boot_tail/' + name + '.c')
        want = targets[name]
        assert len(want) == 8
        for level in (2, 1):
            flags = FLAGS % level
            with tempfile.TemporaryDirectory(prefix='boot-c13-') as tmp:
                obj = Path(tmp) / 'function.o'
                score.compile_single(source, flags, obj)
                result = score.compare(obj, name, show=0)
                words = score.text_words(obj)
                definitions = score.symbols(obj)
                assert definitions == {name: 0}, definitions
                relocated, masks, unresolved, unverified, errors = score.relocate(
                    obj, words, 0, min(len(want), len(words)) * 4,
                    score.image_symbols())
                full_equality = relocated[:len(want)] == want and len(words) >= len(want)
                padding = words[len(want):]
                data, sections = score._elf(obj)
                text_index = score._text_index(sections)
                relocs = []
                for section in sections:
                    if section['type'] == 9 and section['info'] == text_index:
                        syms = score._symbol_table(data, sections, section['link'])
                        for k in range(section['size'] // 8):
                            offset, info = struct.unpack_from('>II', data, section['off'] + 8*k)
                            relocs.append({'offset': offset, 'type': info & 255,
                                           'symbol': syms[info >> 8]['name']})
                if level == 2:
                    assert result.accepted() and full_equality and not any(padding)
                    assert not masks and not unresolved and not unverified and not errors
                    assert relocs == [{'offset': 8, 'type': 4, 'symbol': 'func_80021150'}]
                rows.append({'name': name, 'source_path': str(source.relative_to(ROOT)),
                    'source_sha256': digest(source), 'flags': flags,
                    'effective_flags': flags + ' -Wab,-r4300_mul',
                    'control_offset': 196 + 18 * index,
                    'target_bytes': len(want) * 4,
                    'target_body_sha256': hashlib.sha256(struct.pack('>%dI' % len(want), *want)).hexdigest(),
                    'object_text_bytes': len(words) * 4,
                    'zero_padding_words': len(padding) if not any(padding) else None,
                    'differing_words': result.differing, 'extra_words': result.extra_words,
                    'unresolved': result.unresolved, 'unverified': result.unverified,
                    'errors': result.errors, 'masked_relocation_count': len(masks),
                    'relocations': relocs, 'strict_match': result.accepted(),
                    'relocated_full_word_equality': full_equality})
    paths = ['asm/us/boot_tail/SHA256SUMS', 'asm/us/boot_tail/boot_tail_8000f3a4.s',
             'asm/us/boot_tail/extents.json', 'asm/us/boot_tail/symbols.json',
             'specs/015-boot-tail-runtime/inventory.json', 'tools/cloud/score.py']
    return {'schema_version': 1, 'base': '76780b3a1b3e26c54b86e1f153344e92dac15b20',
            'source_hashes': {path: digest(ROOT / path) for path in paths},
            'compiler_files_sha256': {p.name: digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()},
            'results': rows}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = json.dumps(run(), indent=2) + '\n'
    if args.check:
        expected = Path(__file__).with_name('scores.json').read_text()
        if result != expected:
            raise SystemExit('C13 receipt drift')
        print('C13: all nine strict O2 matches and O1 controls reproduced')
    else:
        print(result, end='')
