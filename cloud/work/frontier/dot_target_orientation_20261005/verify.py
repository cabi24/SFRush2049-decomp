"""Rebuild the six-word NONMATCH and its complete bounded proof."""
import argparse
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import random
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
spec = importlib.util.spec_from_file_location('orientation_native', HERE / 'native.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
FN = 'func_8010E72C'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT = ['func_80090284', 'vector_normalize_length', 'math_utility', 'stat_lap_split']
DIFFS = [0, 140, 148, 152, 172, 240]


def sha(data): return hashlib.sha256(data).hexdigest()


def symbol(obj, name=FN):
    data, sections = score._elf(obj)
    ti = score._text_index(sections)
    matches = [sym for i, sec in enumerate(sections) if sec['type'] == 2
               for sym in score._symbol_table(data, sections, i)
               if sym['name'] == name and sym['type'] == 2 and sym['section'] == ti]
    assert len(matches) == 1
    return matches[0]


def inspect(obj, name=FN):
    comparison = score.compare(obj, name, show=0)
    return dict(asdict(comparison), verdict=comparison.summary(),
                symbol_bytes=symbol(obj, name)['size'],
                native_bytes=4 * len(score.targets()[name]),
                own_data_notes=list(comparison.notes))


def complete_match(record):
    return (record['verdict'] == 'MATCH' and record['symbol_bytes'] == record['native_bytes']
            and not any(record[k] for k in ['differing', 'extra_words', 'unresolved', 'unverified', 'errors']))


def link(obj, work, label):
    symbols = score.image_symbols()
    script = work / (label + '.ld')
    script.write_text('SECTIONS { .text 0x8010e72c : SUBALIGN(4) { *(.text) } }\n' +
                      ''.join('%s = 0x%x;\n' % (name, address) for name, address in symbols.items() if name != FN))
    elf, binary = work / (label + '.elf'), work / (label + '.bin')
    subprocess.run(['mips-linux-gnu-ld', '-EB', '-T', str(script), '-o', str(elf), str(obj)], check=True, capture_output=True)
    subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', str(elf), str(binary)], check=True)
    data = binary.read_bytes()
    assert symbol(elf)['size'] == 252 and len(data) == 256 and not any(data[252:])
    return list(struct.unpack('>63I', data[:252]))


def cases():
    result = []
    times = [0, 0x80000000, 0x3f800000, 0x7f800000, 0xff800000, 0x7fc12345, 0x7fa12345]
    for empty in range(2):
        for flags in range(256):
            for mutations in [0, 1, 2, 4, 8, 15]:
                result.append([empty, flags % 4, (flags // 4) % 4, flags, times[flags % len(times)], mutations, 0x55667788 ^ flags])
    rng = random.Random(0x10e72c)
    for _ in range(512):
        result.append([rng.randrange(2), rng.randrange(4), rng.randrange(4), rng.randrange(256),
                       rng.getrandbits(32), rng.randrange(16), rng.getrandbits(32)])
    return result


def host_library(folder, source):
    folder.mkdir(exist_ok=True)
    (folder / 'candidate.c').write_text(source)
    shutil.copyfile(HERE / 'host.c', folder / 'host.c')
    out = folder / 'host.so'
    subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-O1', '-shared', '-fPIC',
                    '-Wall', '-Wextra', '-Werror', str(folder / 'host.c'), '-o', str(out)], check=True, capture_output=True)
    lib = ctypes.CDLL(str(out))
    lib.run.argtypes = [ctypes.POINTER(ctypes.c_uint32), ctypes.POINTER(ctypes.c_uint32)]
    return lib


def host_run(lib, case):
    out = (ctypes.c_uint32 * 71)()
    lib.run((ctypes.c_uint32 * 7)(*case), out)
    return list(out)


def behavior(work, want, linked, relocated):
    fixture = cases()
    source = (HERE / 'candidate.c').read_text()
    lib = host_library(work / 'host', source)
    covered = [set(), set(), set()]
    canonical, candidate = [], []
    for case in fixture:
        expected = native.oracle(case)
        results = []
        for i, words in enumerate([want, linked, relocated]):
            machine = native.Machine(words, case)
            results.append(machine.run())
            covered[i] |= machine.visited
        assert results[0] == results[1] == results[2] == expected == host_run(lib, case), case
        canonical.append(results[0])
    assert all(len(offsets) == 63 for offsets in covered)
    executable = work / 'host_ubsan'
    subprocess.run(['cc', '-std=c89', '-pedantic-errors', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
                    '-fsanitize=undefined', '-fno-sanitize-recover=all', '-DHOST_MAIN',
                    str(HERE / 'host.c'), '-o', str(executable)], check=True, capture_output=True)
    inputs = ''.join(' '.join(map(str, case)) + '\n' for case in fixture)
    run = subprocess.run([str(executable)], input=inputs, capture_output=True, text=True, check=True)
    assert [list(map(int, line.split())) for line in run.stdout.splitlines()] == canonical
    mutants = {
        'wrong_state': source.replace('actor->state = 4;', 'actor->state = 5;'),
        'wrong_flag_mask': source.replace('actor->flags &= ~6;', 'actor->flags &= ~4;'),
        'wrong_resource_row': source.replace('node->resource = D_8011753C[actor->index].resource;',
                                              'node->resource = D_8011753C[0].resource;'),
        'stale_sound_player': source.replace('actor->player, actor->position, 2);',
                                                '(int)(car - player_array), actor->position, 2);'),
        'stale_list_head': source.replace('    Model952 *car;', '    Model952 *car;\n    Node24 *old_head;')
                                .replace('    node = func_80090284();', '    old_head = D_801391F0;\n    node = func_80090284();')
                                .replace('node->next = D_801391F0;', 'node->next = old_head;'),
    }
    kills = {}
    for label, mutant in mutants.items():
        assert mutant != source
        mutant_lib = host_library(work / label, mutant)
        kills[label] = sum(host_run(mutant_lib, case) != expected for case, expected in zip(fixture, canonical))
        assert kills[label] > 0
    # The restricted decoder and ABI guards are part of the proof, not assumed.
    for label, offset, value in [('unknown_instruction', 0, 0xffffffff),
                                 ('missing_restore', 59, 0)]:
        words = want[:]
        words[offset] = value
        try:
            native.Machine(words, fixture[0]).run()
        except AssertionError:
            kills[label] = 'rejected'
        else:
            raise AssertionError('decoder/ABI mutation survived: ' + label)
    return {'cases': len(fixture), 'native_executions': 3 * len(fixture),
            'instruction_offsets_covered': [len(x) for x in covered],
            'unchanged_host_c89': 'passed', 'ubsan_same_corpus': 'passed',
            'whole_external_memory_and_stack_canaries': 'passed',
            'saved_registers_and_stack_restore': 'passed',
            'wrong_contract_controls': kills,
            'output_sha256': sha(json.dumps(canonical, separators=(',', ':')).encode()),
            'limits': 'Mapped records with indices 0..3. Four dependencies are explicit O32 hooks; their internals and gameplay are not executed.'}


def controls(work):
    source = (HERE / 'candidate.c').read_text()
    result = {}
    for matrix in [False, True]:
        for car in [False, True]:
            variant = source
            if not matrix:
                variant = variant.replace('    Matrix matrix;', '    struct { struct { f32 uvs[3][3]; } mat3; } matrix;')
            if not car:
                variant = variant.replace('    Model952 *car;\n', '').replace('        car = &player_array[actor->player];\n', '')
                variant = variant.replace('car->direction, matrix.mat3.uvs', 'player_array[actor->player].direction, matrix.mat3.uvs')
            label = ('matrix48' if matrix else 'basis36') + ('_car_local' if car else '_direct_array')
            path, obj = work / (label + '.c'), work / (label + '.o')
            path.write_text(variant)
            score.compile_single(path, FLAGS, obj)
            result[label] = dict(inspect(obj), source_sha256=sha(path.read_bytes()),
                                 frame_bytes=-(score.text_words(obj)[0] & 65535) + 65536)
    for label, path in [('archived_B77_resource', ROOT / 'cloud/work/near_miss_B77/func_8010E72C_resource.c'),
                         ('archived_B77_native', ROOT / 'cloud/work/near_miss_B77/func_8010E72C_native.c')]:
        obj = work / (label + '.o')
        score.compile_single(path, FLAGS, obj)
        result[label] = dict(inspect(obj), source_sha256=sha(path.read_bytes()),
                             frame_bytes=-(score.text_words(obj)[0] & 65535) + 65536)
    return result


def verify(work, run_behavior=True):
    source = HERE / 'candidate.c'
    obj = work / 'candidate.o'
    score.compile_single(source, FLAGS, obj)
    want, raw = score.targets()[FN], score.text_words(obj)
    proof = inspect(obj)
    assert proof['differing'] == 6 and proof['symbol_bytes'] == 252
    assert not any(proof[k] for k in ['unresolved', 'unverified', 'errors', 'extra_words'])
    assert len(raw) == 64 and raw[-1] == 0
    relocated, masks, unresolved, unverified, errors = score.relocate(obj, raw, 0, 252, score.image_symbols())
    assert not any([masks, unresolved, unverified, errors])
    relocated = relocated[:63]
    linked = link(obj, work, 'candidate')
    assert linked == relocated
    differences = [i * 4 for i, pair in enumerate(zip(want, linked)) if pair[0] != pair[1]]
    assert differences == DIFFS
    for off in differences:
        expected, actual = want[off // 4], linked[off // 4]
        assert expected >> 16 == actual >> 16 and expected >> 21 & 31 == 29
        delta = native.signed(expected, 16) - native.signed(actual, 16)
        assert delta == (-8 if off == 0 else 8)
    data, sections = score._elf(obj)
    assert not any(s['size'] for s in sections if s['name'] in ['.data', '.rodata', '.rdata', '.bss'])
    relocation_count = sum(sec['size'] // 8 for sec in sections if sec['type'] == 9 and sec['info'] == score._text_index(sections))
    abi = work / 'abi.c'
    abi.write_text('#include "' + str(source) + '"\n' + '''
#define OFFSET(t, f) ((u32)&(((t *)0)->f))
#define CHECK(n, e) typedef char n[(e) ? 1 : -1]
CHECK(matrix_size, sizeof(Matrix) == 48);
CHECK(matrix_position, OFFSET(Matrix3, pos) == 36);
CHECK(actor_size, sizeof(Actor96) == 96);
CHECK(actor_flags, OFFSET(Actor96, flags) == 4);
CHECK(actor_index, OFFSET(Actor96, index) == 16);
CHECK(actor_basis, OFFSET(Actor96, basis) == 20);
CHECK(actor_position, OFFSET(Actor96, position) == 56);
CHECK(actor_state, OFFSET(Actor96, state) == 90);
CHECK(actor_player, OFFSET(Actor96, player) == 92);
CHECK(model_size, sizeof(Model952) == 952);
CHECK(model_direction, OFFSET(Model952, direction) == 20);
CHECK(node_size, sizeof(Node24) == 24);
CHECK(node_state, OFFSET(Node24, state) == 4);
CHECK(node_owner, OFFSET(Node24, owner) == 12);
CHECK(node_time, OFFSET(Node24, time) == 16);
CHECK(node_resource, OFFSET(Node24, resource) == 20);
CHECK(row_size, sizeof(Row48) == 48);
CHECK(row_event, OFFSET(Row48, event) == 16);
''')
    score.compile_single(abi, FLAGS, work / 'abi.o')
    group = work / 'context'
    group.mkdir()
    shutil.copyfile(source, group / 'candidate.c')
    for name in CONTEXT: shutil.copyfile(ROOT / 'src/blob' / (name + '.c'), group / (name + '.c'))
    (group / 'group.json').write_text(json.dumps({'files': ['candidate.c'] + [n + '.c' for n in CONTEXT],
        'members': [FN], 'context': CONTEXT, 'keep': [FN] + CONTEXT, 'flags': FLAGS, 'claims': []}))
    group_obj = work / 'context.o'
    score.compile_group(group, group_obj)
    context = {name: inspect(group_obj, name) for name in [FN] + CONTEXT}
    assert all(complete_match(context[name]) for name in CONTEXT), context
    assert context[FN]['differing'] == 6 and context[FN]['symbol_bytes'] == 252, context[FN]
    group_symbol = symbol(group_obj)
    group_words, gm, gu, gv, ge = score.relocate(group_obj, score.text_words(group_obj),
        group_symbol['value'], group_symbol['value'] + 252, score.image_symbols())
    assert not any([gm, gu, gv, ge])
    group_body = group_words[group_symbol['value'] // 4:group_symbol['value'] // 4 + 63]
    assert group_body == linked
    pointers = []
    for address, block in score.own_data().runs:
        for offset in range(0, len(block) - 3, 4):
            if int.from_bytes(block[offset:offset + 4], 'big') == native.BASE:
                pointers.append(hex(address + offset))
    direct_callers = []
    for name, words in score.targets().items():
        for offset, word in enumerate(words):
            if word >> 26 == 3 and (0x80000000 | ((word & 0x3ffffff) << 2)) == native.BASE:
                direct_callers.append({'function': name, 'address': hex(score.image_symbols()[name] + offset * 4)})
    assert len(pointers) == 10 and not direct_callers
    time_bytes = score.own_data().read(native.TIME, 4)
    assert time_bytes is not None

    obj_o2 = work / 'candidate_o2.o'
    score.compile_single(source, FLAGS.replace('-O3', '-O2'), obj_o2)
    assert link(obj_o2, work, 'candidate_o2') == linked
    result = {'status': 'NONMATCH', 'claims': [], 'new_matching_bytes': 0, 'accepted_byte_gain': 0,
              'base_revision': '31b2799ebb821a7ec0983a34d2611bba2cedaab9',
              'function': FN, 'range': ['0x8010E72C', '0x8010E828'],
              'source_sha256': sha(source.read_bytes()), 'compiler_sha256': sha((score.IDO / 'cc').read_bytes()),
              'flags': FLAGS, 'assembler_erratum_flag': score.R4300_CC,
              'target_sha256': sha(struct.pack('>63I', *want)),
              'linked_candidate_sha256': sha(struct.pack('>63I', *linked)),
              'object': proof, 'object_o2': inspect(obj_o2),
              'complete_gnu_link_agrees_with_project_relocation': True,
              'text_relocation_count': relocation_count, 'own_data_bytes': 0,
              'text_alignment_bytes_outside_symbol': 4,
              'differing_byte_offsets': differences, 'native_frame_bytes': 96, 'candidate_frame_bytes': 88,
              'all_differences_are_eight_byte_stack_displacements': True,
              'native_abi_compile_checks': 18,
              'context': context, 'context_candidate_body_unchanged': True,
              'registration': {'protected_data_pointer_addresses': pointers, 'direct_jal_callers': direct_callers,
                  'limit': 'Pointer presence supports callback use; dispatcher execution and original ownership are not established.'},
              'external_time_scalar': {'address': hex(native.TIME), 'value': struct.unpack('>f', time_bytes)[0],
                  'sha256': sha(time_bytes), 'candidate_owns_literal': False},
              'context_source_sha256': {name: sha((group / (name + '.c')).read_bytes()) for name in CONTEXT},
              'controls': controls(work),
              'input_sha256': {str(path.relative_to(ROOT)): sha(path.read_bytes()) for path in
                  [ROOT / 'asm/us/blob/SHA256SUMS', ROOT / 'asm/us/blob/symbols.json', ROOT / 'tools/cloud/score.py']}}
    if run_behavior: result['behavior'] = behavior(work, want, linked, relocated)
    result['packet_sha256'] = {name: sha((HERE / name).read_bytes()) for name in ['candidate.c', 'host.c', 'native.py', 'verify.py', 'claim.json']}
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--write', action='store_true')
    parser.add_argument('--skip-behavior', action='store_true')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='orientation-proof-') as directory:
        result = verify(Path(directory), not args.skip_behavior)
    destination = HERE / 'verification.json' if args.write else args.output
    if destination: destination.write_text(json.dumps(result, indent=2) + '\n')
    else: print(json.dumps(result, indent=2))
