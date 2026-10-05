#!/usr/bin/env python3
"""Reproduce the bounded A150C nonmatch without changing any acceptance gate."""
import argparse
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

FUNCTION = 'func_800A150C'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
SOURCE = Path(__file__).with_name(FUNCTION + '.c')


def sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def verify(directory):
    directory = Path(directory)
    directory.mkdir(parents=True, exist_ok=True)
    target = score.targets()[FUNCTION]
    assert len(target) * 4 == 312
    assert score.image_symbols()[FUNCTION] == 0x800A150C
    assert SOURCE.read_text().splitlines()[0] == '/* flags: ' + FLAGS + ' */'
    obj = directory / 'candidate.o'
    score.compile_single(SOURCE, FLAGS, obj)
    comparison = score.compare(obj, FUNCTION, show=0)
    assert (comparison.differing, comparison.total, comparison.extra_words) == (1, 78, 0)
    assert not comparison.unresolved and not comparison.unverified and not comparison.errors
    assert not comparison.accepted()

    data, sections = score._elf(obj)
    symbols = [symbol for index, section in enumerate(sections) if section['type'] == 2
               for symbol in score._symbol_table(data, sections, index)]
    functions = [symbol for symbol in symbols if symbol['type'] == 2]
    assert len(functions) == 1 and functions[0]['name'] == FUNCTION
    function = functions[0]
    assert function['value'] == 0 and function['size'] == 312
    text = next(section for section in sections if section['name'] == '.text')
    padding = data[text['off'] + 312:text['off'] + text['size']]
    assert padding == bytes(len(padding))
    assert all(section['size'] == 0 for section in sections
               if section['name'] in ('.data', '.rodata', '.bss'))

    words = score.text_words(obj)
    linked, masks, unresolved, unverified, errors = score.relocate(
        obj, words, 0, 312, score.image_symbols())
    assert not masks and not unresolved and not unverified and not errors
    differing = [index * 4 for index in range(78) if linked[index] != target[index]]
    assert differing == [0xD4]
    native, candidate = target[0xD4 // 4], linked[0xD4 // 4]
    # The one real mismatch is bnel rs,rt: the two compared registers swap.
    assert native >> 26 == candidate >> 26 == 0x15
    assert native & 0xFFFF == candidate & 0xFFFF
    assert (native >> 21) & 31 == (candidate >> 16) & 31
    assert (native >> 16) & 31 == (candidate >> 21) & 31
    relocations = []
    for section in sections:
        if section['type'] != 9 or sections[section['info']]['name'] != '.text':
            continue
        table = score._symbol_table(data, sections, section['link'])
        for offset in range(section['off'], section['off'] + section['size'], 8):
            address, info = struct.unpack_from('>II', data, offset)
            relocations.append({'offset': address, 'type': info & 255,
                                'symbol': table[info >> 8]['name']})
    assert len(relocations) == 4
    assert {item['symbol'] for item in relocations} == {'D_8011EAEC'}
    callers = []
    jal = 0x0C000000 | ((0x800A150C >> 2) & 0x03FFFFFF)
    for name, body in score.targets().items():
        for index, word in enumerate(body):
            if word == jal:
                callers.append({'function': name, 'offset': index * 4})
    assert callers == [{'function': 'track_process_main', 'offset': 164},
                       {'function': 'track_process_main', 'offset': 184}]
    controls = []
    for label, source, flags, expected, extras in [
        ('historical_final_o2', ROOT / 'cloud/work/tiny_A110/func_800A150C.c',
         FLAGS.replace('-O3', '-O2'), 70, 0),
        ('genuine_output_cursor_o2', ROOT / 'cloud/work/tiny_A110/control.output_cursor.c',
         FLAGS.replace('-O3', '-O2'), 77, 3),
        ('genuine_output_cursor_o3', ROOT / 'cloud/work/tiny_A110/control.output_cursor.c',
         FLAGS, 4, 0),
    ]:
        control_obj = directory / (label + '.o')
        score.compile_single(source, flags, control_obj)
        result = score.compare(control_obj, FUNCTION, show=0)
        assert (result.differing, result.extra_words) == (expected, extras)
        assert not result.unresolved and not result.unverified and not result.errors
        controls.append({'label': label, 'source': str(source.relative_to(ROOT)),
                         'source_sha256': sha256(source), 'flags': flags,
                         'comparison': asdict(result)})
    return {
        'function': FUNCTION, 'status': 'NONMATCH', 'claims': [],
        'source': str(SOURCE.relative_to(ROOT)), 'source_sha256': sha256(SOURCE),
        'flags': FLAGS, 'target_address': '0x800A150C', 'end_exclusive': '0x800A1644',
        'target_bytes': 312, 'candidate_elf_size_bytes': function['size'],
        'candidate_text_section_bytes': text['size'], 'zero_alignment_bytes': len(padding),
        'target_manifest_sha256': sha256(score.ASM_DIR / 'SHA256SUMS'),
        'target_body_sha256': hashlib.sha256(struct.pack('>78I', *target)).hexdigest(),
        'candidate_relocated_body_sha256': hashlib.sha256(struct.pack('>78I', *linked[:78])).hexdigest(),
        'comparison': asdict(comparison), 'differing_offsets': differing,
        'residual': 'One symmetric bnel equality operand swap; byte identity is not established.',
        'relocations': relocations, 'direct_callers': callers, 'controls': controls,
        'image_gate_run': False, 'full_rom_gate_run': False, 'new_coverage_bytes': 0,
    }


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build-dir', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    receipt = verify(args.build_dir)
    args.output.write_text(json.dumps(receipt, indent=2) + '\n')
    print('func_800A150C: verified bounded NONMATCH, 1/78 differing words, no credit')
