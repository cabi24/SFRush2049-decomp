#!/usr/bin/env python3
"""Replay a source-only VoiceState adaptation; never relock or promote a TU."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import struct
import subprocess
import sys
import tempfile

if not __debug__:
    raise SystemExit('Verification requires assertions; do not use optimized Python.')

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
GROUP = 'cloud/matches/voice_unblock_production'
SOURCE = GROUP + '/func_8001F954.c'
LEGACY = 'cloud/matches/boot_tail/func_8001F954.c'
TU = 'src/rom/lib_1f5b0.c'
FN = 'func_8001F954'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
PRODUCTION_FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
MARKER = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001F954.s")'
DATA = {'.data', '.rodata', '.rdata', '.bss', '.sdata', '.sbss', '.lit4', '.lit8'}
HELPERS = {
    'func_80014AF0': ('cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014AF0.c', 'src/rom/lib_11640.c'),
    'func_8001C7F4': ('cloud/work/boot_tail_promotion/sources/func_8001C7F4.c', 'src/rom/lib_1cf90.c'),
}
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor.seeds.extract_candidates import extract_functions


def sha(data):
    return hashlib.sha256(data).hexdigest()


def words_bytes(words):
    return struct.pack('>' + str(len(words)) + 'I', *words)


def git(path, root=ROOT):
    return subprocess.check_output(['git', 'show', BASE + ':' + path], cwd=root)


def norm(text):
    return ' '.join(text.split())


def bodies(text):
    return {name: text[start:end] for name, start, end in extract_functions(text)}


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def source_contract(source, baseline, legacy):
    """The only body changes are names of already-established fields."""
    assert source.splitlines()[0] == '/* flags: ' + FLAGS + ' */'
    candidate = bodies(source)
    assert set(candidate) == {FN}
    expected = bodies(legacy)[FN].replace('.identifier =', '.identifier60 =')
    expected = expected.replace('.valueBD =', '.activeBD =')
    assert norm(candidate[FN]) == norm(expected), 'unexpected native body change'
    for name in ('SequenceNode', 'VoiceState'):
        pattern = r'typedef struct ' + name + r' \{.*?\} ' + name + ';'
        actual = re.findall(pattern, source, re.S)
        production = re.findall(pattern, baseline, re.S)
        assert len(actual) == len(production) == 1
        assert norm(actual[0]) == norm(production[0]), name + ' contract changed'
    assert source.count('extern void func_8001F6EC(VoiceState *state);') == 1
    assert baseline.count(MARKER) == 1, 'base must contain the original passthrough'
    return {'body_change': ['identifier -> identifier60', 'valueBD -> activeBD'],
            'record_declarations_equal_base': ['SequenceNode', 'VoiceState'],
            'genuine_context': 'standalone, external real helpers; no fabricated bodies'}


def packet_hashes():
    return {SOURCE: sha((ROOT / SOURCE).read_bytes()),
            GROUP + '/group.json': sha((ROOT / GROUP / 'group.json').read_bytes()),
            'verify.py': sha((PACKET / 'verify.py').read_bytes()),
            'host_test.c': sha((PACKET / 'host_test.c').read_bytes())}


def check_packet_binding(receipt):
    assert packet_hashes() == receipt['packet_sha256'], 'proof-source binding drift'


def replay_invariants(result):
    """Recipe observations are metadata, not a pin on the evolving scorer."""
    projected = json.loads(json.dumps(result))
    projected['proof']['O3_group'].pop('backend_invocations', None)
    return projected


def compile_group_with_recipe(group_obj):
    """Observe stock scorer commands; delegate every invocation unchanged."""
    invocations = []
    original_run = score._run

    def observe(arguments, **options):
        argv = [str(argument) for argument in arguments]
        recorded = [Path(argv[0]).name] + [
            '<output.o>' if argument == str(group_obj) else argument
            for argument in argv[1:]]
        invocations.append(recorded)
        return original_run(arguments, **options)

    score._run = observe
    try:
        spec = score.compile_group(ROOT / GROUP, group_obj)
    finally:
        score._run = original_run
    return spec, invocations


def command(args, work, expect_error=None):
    result = subprocess.run([str(arg) for arg in args], cwd=work,
                            capture_output=True, text=True, env=dict(os.environ,
                            ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1'))
    if expect_error:
        assert result.returncode != 0 and expect_error in result.stderr, result.stdout + result.stderr
    else:
        assert result.returncode == 0, result.stdout + result.stderr
    return result


def compile_source(source, work, label, flags):
    path, obj = work / (label + '.c'), work / (label + '.o')
    path.write_text(source)
    # Invoke cc directly: the new O3 claim uses exactly the bare header flags.
    command([score.IDO / 'cc', '-c', *shlex.split(flags), '-o', obj, path], work)
    return path, obj


def native_row(obj, name):
    result = score.compare(obj, name, show=0)
    assert result.accepted(False), name + ': ' + result.summary()
    raw, sections = score._elf(obj)
    symbols = [symbol for index, section in enumerate(sections) if section['type'] == 2
               for symbol in score._symbol_table(raw, sections, index)]
    own = [symbol for symbol in symbols if symbol['name'] == name and symbol['type'] == 2]
    assert len(own) == 1
    target = score.targets()[name]
    assert own[0]['size'] == len(target) * 4
    start = own[0]['value']
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, score.text_words(obj), start, start + len(target) * 4, score.image_symbols())
    assert not (masks or unresolved or unverified or errors)
    actual = resolved[start // 4:start // 4 + len(target)]
    assert actual == target
    assert not any(section['size'] for section in sections if section['name'] in DATA)
    return {'comparison': asdict(result), 'elf_bytes': own[0]['size'],
            'native_sha256': sha(words_bytes(target)),
            'compiled_body_sha256': sha(words_bytes(actual)), 'owned_data_bytes': 0}


def independent_link(obj, work, name):
    raw, sections = score._elf(obj)
    syms = [s for i, section in enumerate(sections) if section['type'] == 2
            for s in score._symbol_table(raw, sections, i)]
    addresses = score.image_symbols()
    bindings = {s['name']: addresses.get(s['name'], score.address_named(s['name']))
                for s in syms if s['section'] == 0 and s['name']}
    assert all(value is not None for value in bindings.values())
    script = 'SECTIONS { .text ' + hex(addresses[name]) + ' : SUBALIGN(4) { *(.text) }\n'
    script += '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.mdebug) *(.comment) *(.gnu.attributes) } }\n'
    script += ''.join(symbol + ' = ' + hex(value) + ';\n' for symbol, value in sorted(bindings.items()))
    ld, elf, binary = obj.with_suffix('.ld'), obj.with_suffix('.elf'), obj.with_suffix('.bin')
    ld.write_text(script)
    command(['mips-linux-gnu-ld', '-T', ld, '-o', elf, obj], work)
    command(['mips-linux-gnu-objcopy', '-O', 'binary', '--only-section=.text', elf, binary], work)
    data = binary.read_bytes()
    target = words_bytes(score.targets()[name])
    assert data[:len(target)] == target and not any(data[len(target):])
    symbols = command(['mips-linux-gnu-readelf', '-sW', elf], work).stdout
    match = re.search(r'\d+:\s+([0-9a-fA-F]+)\s+(\d+)\s+FUNC\s+GLOBAL\s+DEFAULT\s+\d+\s+' + name + r'\s', symbols)
    assert match and int(match[1], 16) == addresses[name] and int(match[2]) == len(target)
    return {'body_sha256': sha(data[:len(target)]), 'linked_text_bytes': len(data),
            'alignment_zero_bytes': len(data) - len(target), 'bindings': bindings}


def splice(text, row, body):
    """The existing batch promotion's default whole-file declaration dedup."""
    have = {norm(line) for line in text.splitlines() if line.strip()}
    declarations = [s for s in row['preamble'] if s.startswith('#') or norm(s) not in have]
    assert text.count(MARKER) == 1
    return text.replace(MARKER, '\n'.join(declarations) + '\n' + body, 1), declarations


def verify(work):
    assert (score.IDO / 'cc').is_file() and shutil.which('mips-linux-gnu-ld'), 'pinned IDO and MIPS GNU linker required'
    source = (ROOT / SOURCE).read_text()
    baseline, legacy = git(TU).decode(), git(LEGACY).decode()
    proof = {'source_contract': source_contract(source, baseline, legacy)}
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    # Read production context exclusively from BASE, never live src/rom or locks.
    paths = subprocess.check_output(['git', 'ls-tree', '-r', '--name-only', BASE,
        'include', 'tools/asm-processor', 'src/rom/rom_tu.h',
        'asm/us/nonmatchings/rom/lib_1f5b0'], cwd=ROOT, text=True).splitlines()
    for path in paths:
        destination = work / path
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(git(path))
    context_path = work / 'context_check.py'
    context_path.write_bytes(git('cloud/work/boot_tail_promotion/context_check.py'))
    context = load('voice_unblock_adapt_context', context_path)
    context.ROOT, context.CC = work, score.IDO / 'cc'
    context.CFLAGS = ['-G', '0', '-mips2', '-O2', '-non_shared', '-I' + str(work / 'include'),
        '-I' + str(work / 'include/PR'), '-D_LANGUAGE_C', '-Wab,-r4300_mul', '-Xcpluscomm',
        '-I' + str(work / 'src/rom')]

    def build(text, label, error=None):
        path, obj = work / (label + '.c'), work / (label + '.o')
        path.write_text(text)
        command([sys.executable, work / 'tools/asm-processor/build.py', score.IDO / 'cc',
            '--', 'mips-linux-gnu-as', '-march=vr4300', '-mabi=32', '-Iinclude', '--',
            '-c', *context.CFLAGS, '-o', obj, path], work, error)
        return obj

    rows, objects = {}, {}
    for label, text, flags in [('adapted_exact_O3', source, FLAGS),
                                ('adapted_production_O2', source, PRODUCTION_FLAGS),
                                ('legacy_O2', legacy, PRODUCTION_FLAGS)]:
        path, obj = compile_source(text, work, label, flags)
        objects[label] = obj
        rows[label] = {'flags': flags, **native_row(obj, FN), 'gnu_link': independent_link(obj, work, FN)}
    proof['standalone'] = rows
    group_obj = work / 'group.o'
    spec, backend_invocations = compile_group_with_recipe(group_obj)
    assert spec == {'files': ['func_8001F954.c'], 'flags': FLAGS, 'keep': [FN],
                    'members': [FN], 'claims': [FN], 'context': [], 'allow_unverified': False}
    proof['O3_group'] = {'spec': spec, 'backend_invocations': backend_invocations,
                         **native_row(group_obj, FN),
                         'gnu_link': independent_link(group_obj, work, FN)}
    candidate = work / 'candidate.c'
    candidate.write_text(source)
    row = context.check(FN, work, str(candidate))
    assert row['status'] == 'ok', row
    combined, inserted = splice(baseline, row, bodies(source)[FN])
    old_bodies = bodies(baseline)
    assert bodies(combined) == dict(old_bodies, **{FN: bodies(source)[FN]})
    assert inserted == ['#pragma pack(1)', '#pragma pack(0)', 'extern void func_8001F6EC(VoiceState *state);']
    proof['context'] = {'status': row['status'], 'dropped_basic_typedefs': row['dropped'],
                        'inserted_declarations': inserted, 'all_base_C_bodies_unchanged': True}
    old_path = work / 'legacy.c'
    old_path.write_text(legacy)
    old_row = context.check(FN, work, str(old_path))
    assert old_row['status'] == 'ok'
    rejected, _ = splice(baseline, old_row, bodies(legacy)[FN])
    build(rejected, 'legacy_refusal', 'VoiceState')
    # The old default dedup also drops an identical helper declaration located
    # after this slot. A real named formal keeps the prototype visible in scope.
    unnamed = source.replace('func_8001F6EC(VoiceState *state);', 'func_8001F6EC(VoiceState *);')
    unnamed_path = work / 'unnamed.c'
    unnamed_path.write_text(unnamed)
    unnamed_row = context.check(FN, work, str(unnamed_path))
    assert unnamed_row['status'] == 'ok'
    bad_scope, _ = splice(baseline, unnamed_row, bodies(unnamed)[FN])
    build(bad_scope, 'prototype_scope_refusal', 'func_8001F6EC')
    proof['expected_refusals'] = ['legacy conflicting VoiceState', 'later-only helper prototype removed by whole-file dedup']
    baseline_obj, combined_obj = build(baseline, 'base_TU'), build(combined, 'adapted_TU')
    slots = set(old_bodies) | set(re.findall(r'GLOBAL_ASM\("[^\"]+/(func_[0-9A-F]+)\.s"\)', baseline))
    assert set(score.symbols(baseline_obj)) == set(score.symbols(combined_obj)) == slots
    all_rows = {fn: native_row(combined_obj, fn) for fn in sorted(slots)}
    assert all_rows == {fn: native_row(baseline_obj, fn) for fn in sorted(slots)}
    proof['whole_TU'] = {'rows': all_rows, 'base_C_bodies': sorted(old_bodies),
                         'unchanged_passthroughs': sorted(slots - set(old_bodies) - {FN}),
                         'membership_unchanged': True, 'newly_compiled_C_body': FN,
                         'recipe': '-G 0 -mips2 -O2 -non_shared -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm; base include paths; asm-processor + GNU as -march=vr4300 -mabi=32'}
    # Real existing caller and helper source are independently tied to BASE's
    # accepted TU bodies. No assertion about later live lock state is made.
    helper_rows = {}
    for name, (path, tu) in HELPERS.items():
        text = git(path).decode()
        assert norm(bodies(text)[name]) == norm(bodies(git(tu).decode())[name])
        _, obj = compile_source(text, work, name, PRODUCTION_FLAGS + ' -I' + str(work / 'include'))
        helper_rows[name] = {'source_path': path, 'equals_base_production_body': True,
                              **native_row(obj, name), 'gnu_link': independent_link(obj, work, name)}
    proof['genuine_caller_helper'] = helper_rows
    checks = ['sizeof(void *) == 4', 'sizeof(SequenceNode) == 16', 'sizeof(VoiceState) == 416']
    offsets = {'command00': 0, 'next_identifier': 16, 'parent_identifier': 20, 'entry18': 24,
               'flags24': 36, 'value28': 40, 'external4C': 76, 'identifier60': 96, 'activeBD': 189}
    checks += ['OFF(VoiceState, ' + field + ') == ' + str(offset) for field, offset in offsets.items()]
    layout = source + '\n#define OFF(T, f) ((unsigned int)&((T *)0)->f)\n'
    layout += 'typedef char native_layout[(' + ' && '.join(checks) + ') ? 1 : -1];\n'
    compile_source(layout, work, 'layout', FLAGS)
    proof['native_layout'] = {'VoiceState_bytes': 416, 'SequenceNode_bytes': 16, 'offsets': offsets}
    # The proven body is unchanged at machine level; replay the existing bounded
    # interpreter from this base, with freshly linked candidate instructions.
    behavior_path = work / 'native_behavior.py'
    behavior_path.write_bytes(git('cloud/work/boot_tail/BT03-voice-unblock-topology/native_behavior.py'))
    behavior = load('voice_unblock_adapt_native', behavior_path)
    data = objects['adapted_exact_O3'].with_suffix('.bin').read_bytes()[:124]
    proof['native_behavior'] = behavior.verify(score.targets()[FN], list(struct.unpack('>31I', data)))
    harness = work / 'host_test.c'
    harness.write_bytes((PACKET / 'host_test.c').read_bytes())
    flags = ['-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-fstrict-aliasing',
             '-O2', '-fsanitize=address,undefined,bounds', '-fno-omit-frame-pointer', '-no-pie']
    executable = work / 'host_test'
    command(['cc', *flags, harness, '-o', executable], work)
    host = command([executable], work).stdout.strip()
    assert host == '2112 actual-source cases passed'
    proof['host_behavior'] = host
    mutants = [('wrong_clear_field', '.activeBD = 0;', '.unknownBE[0] = 0;'),
               ('wrong_identifier', '.identifier60 = index;', '.identifier60 = index + 1;'),
               ('wrong_sentinel', 'index == 0xFFFFFFFFU', 'index == 0xFFFFFFFEU')]
    rejected = []
    for label, before, after in mutants:
        assert source.count(before) == 1
        (work / 'mutant.c').write_text(source.replace(before, after))
        command(['cc', *[flag for flag in flags if flag != '-O2'], '-O0',
                 '-DCANDIDATE="mutant.c"', harness, '-o', executable], work)
        result = subprocess.run([str(executable)], cwd=work, capture_output=True,
                                env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',
                                         UBSAN_OPTIONS='halt_on_error=1'))
        assert result.returncode != 0, label + ' survived'
        rejected.append(label)
    proof['host_mutants_rejected'] = rejected
    return {'status': 'PASS', 'base': BASE, 'packet_sha256': packet_hashes(), 'proof': proof,
            'new_matching_bytes': 0, 'previous_match_preserved_bytes': 124, 'accepted_byte_gain': 0,
            'limits': ['No live source, context, lock, target, gate or promotion mutation',
                       'Production O2 relock/context/promotion and private full-ROM gate remain maintainer work',
                       'Bounded slots 0..31 and sentinel only; boundary hooks do not execute helper bodies',
                       'No image, compression, full-ROM, hardware, gameplay or hosted CI claim']}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    if not (score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        print(json.dumps({'status': 'SKIP', 'reason': 'pinned IDO and MIPS GNU linker required'}))
        return
    receipt = None
    if args.check:
        receipt = json.loads((PACKET / 'evidence.json').read_text())
        check_packet_binding(receipt)
    with tempfile.TemporaryDirectory(prefix='voice-unblock-contract-') as directory:
        result = verify(Path(directory))
    if args.check:
        # C behavioral proof source is bound; editable pytest wrappers are not.
        # No manifest, scorer or accepted-context hash appears on either side.
        assert replay_invariants(result) == replay_invariants(receipt), 'source or proof invariant drift'
    encoded = json.dumps(result, indent=2) + '\n'
    if args.output:
        args.output.write_text(encoded)
    print(encoded, end='')


if __name__ == '__main__':
    main()
