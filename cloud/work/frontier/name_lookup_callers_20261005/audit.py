#!/usr/bin/env python3
"""Reproduce ABI-blocked diagnostics. Never changes targets, locks, or context.

Outputs hashes, counts, and relocation/extent status, never target bytes or
assembly. Build outputs stay in the ignored build directory.
"""
import argparse
import contextlib
import hashlib
import io
import json
from pathlib import Path
import re
import struct
import sys

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score

FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
BASE = '55ddfc6b53d96d4c5dcaaa8891dedafb037b3994'
TARGETS = ('collision_sound_play', 'physics_collision_test', 'func_800B24EC')


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def direct_calls(words, address):
    call = 0x0c000000 | ((address >> 2) & 0x03ffffff)
    return [i * 4 for i, word in enumerate(words) if word == call]


def outgoing_word(words, call_offset):
    """A narrow local observation, not generic control-flow/dataflow analysis.

    Find the last fifth-slot store within twelve instructions of the call,
    including its delay slot. Report an immediately defined constant only;
    unknown values remain unknown. This covers the two owned callers and the
    independently inspected zero-valued sfx_position_3d calls.
    """
    call_index = call_offset // 4
    begin = max(0, call_index - 12)
    stores = [i for i in range(begin, min(len(words), call_index + 2))
              if words[i] >> 26 == 43 and (words[i] >> 21) & 31 == 29
              and words[i] & 0xffff == 16]
    if not stores:
        return None
    index = stores[-1]
    register = (words[index] >> 16) & 31
    value = 0 if register == 0 else None
    for i in range(index - 1, begin - 1, -1):
        word = words[i]
        # Stop at the nearest simple immediate definition of that register.
        if word >> 26 in (9, 13) and (word >> 16) & 31 == register:
            if (word >> 21) & 31 == 0:
                value = word & 0xffff
                if word >> 26 == 9 and value & 0x8000:
                    value -= 0x10000
            break
    return {'store_offset': index * 4, 'stack_offset': 16,
            'source_register_number': register, 'local_constant': value}


def native_audit():
    targets, symbols = score.targets(), score.image_symbols()
    callee = symbols['func_800B24EC']
    callers = {name: direct_calls(words, callee)
               for name, words in targets.items() if direct_calls(words, callee)}
    result = {}
    for name in TARGETS:
        words = targets[name]
        result[name] = {
            'address': f'0x{symbols[name]:08X}', 'extent_bytes': len(words) * 4,
            'target_sha256': hashlib.sha256(struct.pack(f'>{len(words)}I', *words)).hexdigest(),
            'callee_calls': [{'call_offset': offset, 'fifth_slot': outgoing_word(words, offset)}
                             for offset in direct_calls(words, callee)]}
    body = targets['func_800B24EC']
    frame = 0x10000 - (body[0] & 0xffff)
    loads = [i * 4 for i, word in enumerate(body)
             if word >> 26 in (32, 33, 35, 36, 37, 49, 53)
             and (word >> 21) & 31 == 29 and (word & 0xffff) >= frame + 16]
    result['func_800B24EC']['entry_frame_bytes'] = frame
    result['func_800B24EC']['direct_stack_loads_at_or_above_fifth_input'] = loads
    source = (ROOT / 'src/blob/func_800B24EC.c').read_text()
    definition = re.search(r'NameEntry \*func_800B24EC\(([^)]*)\)\s*\{', source)
    arity = len(definition[1].split(',')) if definition else None
    return {'targets': result, 'direct_callers': callers,
            'direct_call_count': sum(map(len, callers.values())),
            'canonical_formal_count': arity,
            'zero_fifth_controls': [outgoing_word(targets['sfx_position_3d'], offset)
                                    for offset in callers['sfx_position_3d']]}


def strict_object(obj, name):
    """Full declared ELF extent and full relocation checks supplement scorer.

    score.compare alone tolerates trailing zero padding. A function's own
    st_size must equal the native extent; full-range relocation verification
    also covers any extra function words, including zeros.
    """
    data, sections = score._elf(obj)
    text_index = score._text_index(sections)
    symbols = [symbol for i, section in enumerate(sections) if section['type'] == 2
               for symbol in score._symbol_table(data, sections, i)]
    fn = next(symbol for symbol in symbols if symbol['name'] == name
              and symbol['type'] == 2 and symbol['section'] == text_index)
    words = score.text_words(obj)
    start, end = fn['value'], fn['value'] + fn['size']
    expected = score.targets()[name]
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, end, score.image_symbols())
    own_sections = {section['name']: section['size'] for section in sections
                    if section['name'] in ('.rodata', '.data', '.lit4', '.lit8', '.bss')
                    and section['size']}
    extent_ok = fn['size'] == len(expected) * 4
    full_words_equal = extent_ok and resolved[start // 4:end // 4] == expected
    with contextlib.redirect_stdout(io.StringIO()):
        comparison = score.compare(obj, name, show=0)
    return {'elf_extent_bytes': fn['size'], 'target_extent_bytes': len(expected) * 4,
            'extent_equal': extent_ok, 'full_relocated_words_equal': full_words_equal,
            'scorer_summary': comparison.summary(), 'differing_words': comparison.differing,
            'extra_nonzero_words': comparison.extra_words,
            'unresolved': unresolved, 'unverified': unverified, 'errors': errors,
            'relocation_masks': len(masks), 'owned_sections': own_sections,
            'strict_object_equal': full_words_equal and not (masks or unresolved or unverified or errors or own_sections)}


def replay(build):
    build = Path(build).resolve()
    build.mkdir(parents=True, exist_ok=True)
    caller = PACKET / 'collision_sound_play.c'
    callee = ROOT / 'src/blob/func_800B24EC.c'
    physics = ROOT / 'cloud/work/near_miss_B4/physics_collision_test_best.c'
    pool = ROOT / 'src/blob/pool_linked_list_init.c'
    results = {}
    for name, path, flags in [('collision_sound_play', caller, FLAGS),
                              ('physics_collision_test', physics, FLAGS),
                              ('func_800B24EC', callee, FLAGS),
                              ('pool_linked_list_init', pool, FLAGS.replace('-O3', '-O2'))]:
        obj = build / (name + '.o')
        score.compile_single(path, flags, obj)
        results[name] = strict_object(obj, name)
    group = build / 'real_context'
    group.mkdir(exist_ok=True)
    (group / 'caller.c').write_bytes(caller.read_bytes())
    (group / 'callee.c').write_bytes(callee.read_bytes())
    (group / 'group.json').write_text(json.dumps({
        'files': ['caller.c', 'callee.c'],
        'keep': ['collision_sound_play', 'func_800B24EC'],
        'flags': FLAGS, 'claims': []}, indent=2) + '\n')
    score.compile_group(group, group / 'group.o')
    results['real_context'] = {name: strict_object(group / 'group.o', name)
                               for name in ('collision_sound_play', 'func_800B24EC')}
    typed = caller.read_text().replace('func_800B24EC();', 'func_800B24EC(char *,s16 *,s8,s8);')
    typed_path = build / 'typed_arity_control.c'
    typed_path.write_text(typed)
    try:
        score.compile_single(typed_path, FLAGS, build / 'typed_arity_control.o')
    except SystemExit as exc:
        error = str(exc)
        arity_rejected = "number of arguments doesn't agree" in error
    else:
        arity_rejected = False
    return {
        'base_revision': BASE,
        'claims': [], 'rom_coverage_claimed': False,
        'source_contract_status': 'unresolved_five_actuals_four_formals',
        'accepted_caller_match': False,
        'flags': FLAGS, 'assembler_erratum_flag_added_by_scorer': score.R4300_CC,
        'native': native_audit(),
        'source_sha256': {str(path.relative_to(ROOT)): sha(path)
                          for path in (caller, callee, physics, pool)},
        'compiler_sha256': {name: sha(score.IDO / name)
                            for name in ('cc', 'cfe', 'uopt', 'ugen', 'as1', 'uld')},
        'replay': results, 'typed_four_formal_control_rejects_five_actuals': arity_rejected,
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', type=Path, default=ROOT / 'build/name_lookup_callers/replay')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    result = replay(args.build)
    text = json.dumps(result, indent=2, sort_keys=True) + '\n'
    if args.output:
        args.output.write_text(text)
    else:
        print(text, end='')


if __name__ == '__main__':
    main()
