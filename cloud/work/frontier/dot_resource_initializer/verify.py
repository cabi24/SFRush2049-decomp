#!/usr/bin/env python3
"""Replay the bounded caller-only NONMATCH without publishing native bytes.

Uses the existing fail-closed O32 interpreter as a behavioral oracle. The
independent GNU link verifies all relocations of the final standalone object.
The accepted-arity group is a negative diagnostic, never a matching claim.
"""
import argparse
import ctypes
import dataclasses
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
NAME = 'func_8010D85C'
START = 0x8010D85C
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
OLD = ROOT / 'cloud/work/dot_resource_init'
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score
spec = importlib.util.spec_from_file_location('old_resource_replay', OLD / 'verify.py')
replay = importlib.util.module_from_spec(spec)
spec.loader.exec_module(replay)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def function_record(obj, name=NAME):
    data, sections = score._elf(obj)
    text_index = score._text_index(sections)
    candidates = [symbol for i, sec in enumerate(sections) if sec['type'] == 2
                  for symbol in score._symbol_table(data, sections, i)
                  if symbol['name'] == name and symbol['type'] == 2
                  and symbol['section'] == text_index]
    assert len(candidates) == 1
    return candidates[0]


def summarize(obj, name=NAME):
    comparison = dataclasses.asdict(score.compare(obj, name, show=0))
    fn = function_record(obj, name)
    comparison['elf_function_bytes'] = fn['size']
    comparison['object_sha256'] = sha(Path(obj).read_bytes())
    return comparison


def compile_control(source, label, outdir, flags=FLAGS):
    src, obj = outdir / (label + '.c'), outdir / (label + '.o')
    src.write_text(source)
    score.compile_single(src, flags, obj)
    result = summarize(obj)
    result['source_sha256'] = sha(src.read_bytes())
    return result


def run_controls(outdir):
    old = (OLD / 'candidate.c').read_text()
    volatile = old.replace('extern u8 D_80140BDC', 'extern volatile u8 D_80140BDC')
    pointer = volatile.replace('typedef signed short s16;', 'typedef signed short s16;\ntypedef unsigned short u16;')
    pointer = pointer.replace('s32 handle; u8 unknown04[64];', 'char *handle; u8 unknown04[64];')
    pointer = pointer.replace('extern s32 D_80118E08[];', 'extern char *D_80118E08[];')
    pointer = pointer.replace('extern s32 sound_bank_load(s32, s16 *, s8, s8, s32);',
                              'extern char *sound_bank_load(char *, u16 *, s8, s8, s32);')
    pointer = pointer.replace('    s32 handle;', '    char *handle;').replace('    s16 resource_id;', '    u16 resource_id;')
    four = pointer.replace('s8, s8, s32);', 's8, s8);').replace('(s8)(D_80140BDC - 1), 1);', '(s8)(D_80140BDC - 1));')
    slot = pointer.replace('        D_8012E738[state->slot].handle = handle;',
                           '        kind = state->slot;\n        D_8012E738[kind].handle = handle;')
    variant = pointer.replace('    s8 variant;\n', '').replace(
        "if (found) variant = found[4] - '0';\n        else variant = 0;\n        node->variant = variant;",
        "if (found) kind = (s8)(found[4] - '0');\n        else kind = 0;\n        node->variant = kind;")
    controls = {'baseline': old, 'count_volatile': volatile, 'pointer_contract': pointer,
                'pointer_four_args': four, 'slot_local': slot, 'kind_and_variant': variant,
                'reuse_index': variant.replace('        D_8012E738[state->slot].handle = handle;',
                           '        kind = state->slot;\n        D_8012E738[kind].handle = handle;'),
                'reuse_pointer': pointer.replace('    u8 *found;\n', '').replace(
                    '        found = func_800A464C(state->text, D_80121DA8);',
                    '        text = func_800A464C(state->text, D_80121DA8);').replace(
                    "if (found) variant = found[4] - '0';", "if (text) variant = text[4] - '0';")}
    declarations = ['    ResourceState *state;', '    u8 *text;', '    u8 *found;', '    s32 kind;',
                    '    char *handle;', '    s8 variant;', '    u16 resource_id;']
    for label, order in {'output_first': [6, 0, 1, 2, 3, 4, 5],
                         'output_before_text': [0, 6, 1, 2, 3, 4, 5],
                         'text_last': [0, 2, 3, 4, 5, 6, 1],
                         'output_and_text_last': [0, 2, 3, 4, 5, 1, 6]}.items():
        controls[label] = slot.replace('\n'.join(declarations), '\n'.join(declarations[i] for i in order))
    result = {key: compile_control(source, key, outdir) for key, source in controls.items()}
    # Four true parameters plus the exact accepted body, in separate real units.
    group = outdir / 'accepted_arity_group'
    group.mkdir()
    (group / 'candidate.c').write_text(four)
    shutil.copyfile(ROOT / 'src/blob/sound_bank_load.c', group / 'sound_bank_load.c')
    (group / 'group.json').write_text(json.dumps({
        'files': ['candidate.c', 'sound_bank_load.c'], 'flags': FLAGS,
        'members': [NAME], 'context': ['sound_bank_load'],
        'keep': [NAME, 'sound_bank_load'], 'claims': []}))
    obj = outdir / 'accepted_arity_group.o'
    score.compile_group(group, obj)
    result['accepted_arity_group'] = {
        'caller': summarize(obj), 'read_only_callee': summarize(obj, 'sound_bank_load'),
        'callee_source_sha256': sha((group / 'sound_bank_load.c').read_bytes())}
    result['final_O2'] = compile_control((HERE / 'candidate.c').read_text(), 'final_O2', outdir,
                                        FLAGS.replace('-O3', '-O2'))
    return result


def cases():
    rng = random.Random(0x10D85C)
    result = []
    for mode in range(64):
        for byte in [0, 1, 47, 48, 49, 127, 128, 175, 176, 255]:
            for count in [0, 1, 127, 128, 129, 255]:
                result.append([0 if mode & 1 else 0x10001, int(bool(mode & 2)),
                               0x5a | int(bool(mode & 4)), int(bool(mode & 8)),
                               int(bool(mode & 16)), int(bool(mode & 32)), byte, count,
                               2, 7, rng.getrandbits(32)])
    for _ in range(10000):
        result.append([rng.getrandbits(32), rng.randrange(2), rng.randrange(256),
                       rng.randrange(2), rng.randrange(2), rng.randrange(2),
                       rng.randrange(256), rng.randrange(256), rng.randrange(8),
                       rng.randrange(8), rng.getrandbits(32)])
    return result


def verify(outdir):
    want = score.targets()[NAME]
    obj = outdir / 'final.o'
    score.compile_single(HERE / 'candidate.c', FLAGS, obj)
    raw = score.text_words(obj)
    fn = function_record(obj)
    assert fn['value'] == 0 and fn['size'] == len(want) * 4 == 368
    got, masks, unresolved, unverified, errors = score.relocate(
        obj, raw, 0, len(raw) * 4, score.image_symbols())
    assert not any((masks, unresolved, unverified, errors))
    assert len(got) == 92  # Neither hidden alignment nor an ignored body tail.
    linker_script = outdir / 'link.ld'
    linker_script.write_text('SECTIONS { . = 0x8010D85C; .text : { *(.text) } }\n' + ''.join(
        '%s = 0x%x;\n' % (key, value) for key, value in score.image_symbols().items() if key != NAME))
    linked = outdir / 'linked.elf'
    subprocess.run(['mips-linux-gnu-ld', '-EB', '-T', str(linker_script), '-o', str(linked), str(obj)], check=True)
    subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', str(linked), str(outdir / 'linked.bin')], check=True)
    linked_bytes = (outdir / 'linked.bin').read_bytes()
    independent = list(struct.unpack('>92I', linked_bytes))
    assert independent == got
    differences = [i * 4 for i, (a, b) in enumerate(zip(want, got)) if a != b]
    assert len(differences) == 12
    assert all((want[offset // 4] ^ got[offset // 4]) & 0xffff0000 == 0 for offset in differences)
    assert all((want[offset // 4] >> 21 & 31) == 29 for offset in differences)
    assert function_record(linked)['size'] == 368
    layout = outdir / 'layout.c'
    layout.write_text('#include "' + str(HERE / 'candidate.c') + '"\n' +
                     '#define CHECK(n,e) typedef char n[(e)?1:-1]\n' +
                     'CHECK(node_size,sizeof(ResourceNode)==24);\n' +
                     'CHECK(state_size,sizeof(ResourceState)==112);\n' +
                     'CHECK(slot_size,sizeof(ResourceSlot)==68);\n')
    score.compile_single(layout, FLAGS, outdir / 'layout.o')
    host_lib = outdir / 'host.so'
    subprocess.run(['cc', '-std=c99', '-Wall', '-Wextra', '-Werror', '-O1', '-shared', '-fPIC',
                    str(HERE / 'host.c'), '-o', str(host_lib)], check=True)
    lib = ctypes.CDLL(str(host_lib))
    lib.run.argtypes = [ctypes.POINTER(ctypes.c_uint32), ctypes.POINTER(ctypes.c_uint32)]
    fixtures = cases()
    for fixture in fixtures:
        native = replay.execute(want, fixture)
        candidate = replay.execute(got, fixture)
        output = (ctypes.c_uint32 * 20)()
        lib.run((ctypes.c_uint32 * 11)(*fixture), output)
        assert native == candidate == list(output)[:len(native)], fixture
    host = outdir / 'host'
    subprocess.run(['cc', '-std=c99', '-Wall', '-Wextra', '-Werror', '-O1', '-g',
                    '-fsanitize=address,undefined', '-fno-sanitize-recover=all', '-DHOST_MAIN',
                    str(HERE / 'host.c'), '-o', str(host)], check=True)
    subprocess.run([str(host)], env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0'), check=True)
    locks = json.loads((ROOT / 'blob_matched.lock.json').read_text())
    # The target was unlocked at the packet's base commit; it was later accepted
    # independently (3eb2e97f, src/blob/func_8010D85C.c). This NONMATCH replay
    # stays historical research evidence and never consults that lock entry.
    callee_path = ROOT / locks['sound_bank_load']['source']
    assert sha(callee_path.read_bytes()) == locks['sound_bank_load']['source_sha256']
    result = {
        'status': 'NONMATCH', 'claims': [], 'accepted_byte_gain': 0,
        'base_commit': 'cf10b3392d7f00ae42d75c008b79fdc2541aab6b',
        'target': NAME, 'start': hex(START), 'end': hex(START + 368), 'flags': FLAGS,
        'toolkit_sha256': locks['sound_bank_load']['toolkit_sha'],
        'compiler_sha256': sha((score.IDO / 'cc').read_bytes()),
        'source_sha256': sha((HERE / 'candidate.c').read_bytes()),
        'host_source_sha256': sha((HERE / 'host.c').read_bytes()),
        'target_sha256': sha(struct.pack('>92I', *want)),
        'linked_candidate_sha256': sha(linked_bytes),
        'comparison': summarize(obj), 'candidate_text_bytes': len(raw) * 4,
        'candidate_alignment_bytes': 0, 'independent_gnu_link_agrees': True,
        'full_body_relocations_verified': True,
        'differing_word_offsets': differences,
        'residual': '12 stack-relative immediate operands; all opcodes/registers identical',
        'native_frame_bytes': 88, 'candidate_frame_bytes': 64,
        'differential_cases': len(fixtures), 'sanitizer_cases': 100000,
        'o32_layout_checks': True,
        'abi_caveat': 'Native fifth stack argument 1 conflicts with accepted four-parameter callee declaration; no incompatible combined group is claimed',
        'controls': run_controls(outdir)}
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='resource-initializer-') as tmp:
        result = verify(Path(tmp))
    output = json.dumps(result, indent=2) + '\n'
    if args.output:
        args.output.write_text(output)
    print(output, end='')


if __name__ == '__main__':
    main()
