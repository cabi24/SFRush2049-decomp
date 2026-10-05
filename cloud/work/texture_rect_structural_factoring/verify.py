#!/usr/bin/env python3
"""One predeclared factoring experiment, not a source/score search.

SDK headers, native/linker words, objects and logs remain under ignored build/.
Published evidence contains hashes, dimensions, class counts and scalar results.
"""
from pathlib import Path
import argparse
import importlib.util
import json
import shutil
import sys

ROOT = Path(__file__).resolve().parents[3]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


REPLAY = load('factoring_replay', HERE.parent / 'texture_rect_verification/replay.py')
FRESH = load('factoring_fresh', HERE.parent / 'texture_rect_fresh_behavior/verify_behavior.py')


def shape(words):
    """Static instruction classes, not a claim to recover source macro sites."""
    allocations = []
    for word in words:
        if word >> 26 == 9 and (word >> 21) & 31 == 29 and (word >> 16) & 31 == 29:
            immediate = REPLAY.machine.signed(word & 65535, 16)
            if immediate < 0:
                allocations.append(-immediate)
    return {
        'instruction_words': len(words),
        'stack_allocations_bytes': allocations,
        'packet_tag_lui_count': {
            name: sum(word >> 26 == 15 and word & 65535 == immediate for word in words)
            for name, immediate in [('G_TEXRECT', 0xe400), ('G_RDPHALF_1', 0xe100),
                                    ('G_RDPHALF_2', 0xf100)]},
        'instruction_class_counts': {
            name: sum(word >> 26 == opcode for word in words)
            for name, opcode in [('beq', 4), ('bne', 5), ('beql', 20), ('bnel', 21),
                                 ('lw', 35), ('sw', 43)]}}


def named_events(events, symbols):
    """No instruction words or raw disassembly; external object names only."""
    names = {symbols[name]: name for name in REPLAY.machine.GLOBALS + ['D_80149438']}
    return [(kind, names.get(address, 'packet_word_' + str((address - 0x100000) // 4)))
            for kind, address in events]


def semantic_check(source, directory, native, symbols, all_cases, linked=None):
    host = REPLAY.host_candidate(source, directory) if linked is None else None
    visited, paths, tally = set(), set(), {}
    event_differences = 0
    read_differences = 0
    write_differences = 0
    first_difference = None
    for category, arguments, state, unused in all_cases:
        expected = REPLAY.oracle(arguments, state)
        nout, nadvance, nevents, nvisited = REPLAY.machine.execute(
            native, symbols[REPLAY.NAME], symbols, arguments, state)
        assert nout == expected and nadvance == 4 * len(expected)
        if linked is None:
            out, advance = host(arguments, state)
        else:
            out, advance, events, offsets = REPLAY.machine.execute(
                linked, symbols[REPLAY.NAME], symbols, arguments, state)
            visited |= offsets
            event_differences += events != nevents
            read_differences += ([event for event in events if event[0] == 'read'] !=
                                 [event for event in nevents if event[0] == 'read'])
            write_differences += ([event for event in events if event[0] == 'write'] !=
                                  [event for event in nevents if event[0] == 'write'])
            if events != nevents and first_difference is None:
                first_difference = {'category': category, 'arguments': arguments, 'state': state,
                                    'native': named_events(nevents, symbols),
                                    'candidate': named_events(events, symbols)}
        assert out == expected and advance == nadvance, (category, arguments, state)
        tally[category] = tally.get(category, 0) + 1
        if expected:
            paths.add((state[5] != 0, state[0] & 12, bool(state[0] & 0x8000)))
    result = {'result': 'PASS', 'cases': len(all_cases), 'categories': tally,
              'all_mode_flip_stretch_paths': len(paths)}
    assert len(paths) == 16
    if linked is not None:
        result.update({'candidate_instruction_offsets_executed': len(visited),
                       'candidate_unexecuted_offsets': [hex(offset) for offset in range(0, len(linked) * 4, 4)
                                                       if offset not in visited],
                       'external_event_order_difference_cases': event_differences,
                       'external_read_order_difference_cases': read_differences,
                       'external_write_order_difference_cases': write_differences,
                       'first_external_order_difference': first_difference})
    return result


def wrong_join_controls(build, all_cases):
    """Prove the corpus rejects lost path-dependent packet values at the join."""
    source_text = (HERE / 'stretched_tail_join.c').read_text()
    variants = [
        ('drop_joined_phase', '0,tex_s<<5,t_fixed,ds,dt);',
         '0,tex_s<<5,tex_t<<5,ds,dt);', 1),
        ('drop_vertical_derivative_sign', 'dt=0U-step;', 'dt=step;', 2),
    ]
    controls = {}
    for name, before, after, count in variants:
        assert source_text.count(before) == count
        directory = build / name
        directory.mkdir(exist_ok=True)
        for header in ['sdk_context.h', 'libreultra_gbi.h']:
            shutil.copyfile(build / header, directory / header)
        source = directory / 'mutant.c'
        source.write_text(source_text.replace(before, after))
        host = REPLAY.host_candidate(source, directory)
        failures = []
        for category, arguments, state, unused in all_cases:
            actual, advance = host(arguments, state)
            if actual != REPLAY.oracle(arguments, state):
                failures.append({'category': category, 'arguments': arguments, 'state': state})
        assert failures, ('corpus failed to reject wrong join', name)
        controls[name] = {'rejected_cases': len(failures), 'first_failure': failures[0],
                          'source_sha256': REPLAY.sha(source.read_bytes())}
    return controls


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', type=Path, default=ROOT / 'build/87110_structural_factoring')
    args = parser.parse_args()
    build = args.build.resolve()
    build.mkdir(parents=True, exist_ok=True)
    FRESH.sdk_context(build)
    native, manifest = REPLAY.independent_target()
    symbols = REPLAY.score.image_symbols()
    all_cases = list(REPLAY.cases()) + list(FRESH.extra_cases())
    result = {
        'status': 'STRUCTURAL_RESEARCH_ONLY', 'matching_claim': False, 'cartridge_claim': False,
        'design_sha256': REPLAY.sha((HERE / 'DESIGN.md').read_bytes()),
        'historical_source_sha256': REPLAY.sha(REPLAY.DEFAULT.read_bytes()),
        'fresh_source_sha256': REPLAY.sha((HERE.parent / 'texture_rect_fresh_behavior/rectangle.c').read_bytes()),
        'SDK_git_blob_ids': FRESH.HEADERS, 'flags': REPLAY.FLAGS,
        'protected_manifest_entries_verified': len(manifest), 'native_shape': shape(native),
        'compiler_sha256': {name: REPLAY.sha((REPLAY.score.IDO / name).read_bytes())
                            for name in ['cc', 'cfe', 'uopt', 'ugen', 'as1']},
        'gnu_linker': REPLAY.run(['mips-linux-gnu-ld', '--version']).splitlines()[0],
        'host_compiler': REPLAY.run(['cc', '--version']).splitlines()[0],
        'variants': {}}
    for name, count in [('unsigned_eight', 8), ('stretched_tail_join', 5)]:
        directory = build / name
        directory.mkdir(exist_ok=True)
        for header in ['sdk_context.h', 'libreultra_gbi.h']:
            shutil.copyfile(build / header, directory / header)
        source = directory / 'candidate.c'
        shutil.copyfile(HERE / (name + '.c'), source)
        assert source.read_text().count('gSPTextureRectangle(') == count
        entry = {'source_sha256': REPLAY.sha(source.read_bytes()), 'source_macro_sites': count}
        entry['host_UBSan'] = semantic_check(source, directory, native, symbols, all_cases)
        (directory / 'host_verification.json').write_text(json.dumps(entry, indent=2) + '\n')
        print(name, 'host behavior PASS before stock compile:', len(all_cases), flush=True)
        obj = directory / 'candidate.o'
        REPLAY.score.compile_single(source, REPLAY.FLAGS, obj)
        proof, ignored_native, linked, ignored_symbols = REPLAY.inspect(obj, directory)
        entry.update({'object_sha256': REPLAY.sha(obj.read_bytes()), 'full_link_proof': proof,
                      'shape': shape(linked),
                      'linked_semantics': semantic_check(source, directory, native, symbols, all_cases, linked)})
        result['variants'][name] = entry
        print(name, json.dumps({'shape': entry['shape'],
                               'different_words': proof['project_scorer_differing_words'],
                               'exact_match': proof['accepted_exact_match'],
                               'external_order_difference_cases': entry['linked_semantics']['external_event_order_difference_cases']}), flush=True)
    result['wrong_join_controls'] = wrong_join_controls(build, all_cases)
    (build / 'verification.json').write_text(json.dumps(result, indent=2) + '\n')


if __name__ == '__main__':
    main()
